/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Resources/Camera.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : The Camera rendering primitive.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Camera.h"

/////////////////////////////////////////////////////////////////////////////////
// Camera implementation
ProjectionDepthMapping Camera::sm_iDefaultDepthMapping = PROJECT_DEPTH_ZERO_ONE;

Camera::Camera( Bool bPerspective )
{
    m_iDepthMapping = sm_iDefaultDepthMapping;

    m_matPreView = Matrix4::Identity;
    m_bHasPreView = false;

    // SetView( Vertex3::Null, Vector3::eI, Vector3::eK );
    m_vEye = Vertex3::Null;
    m_vView = Vector3::eI;
    m_vUp = Vector3::eK;
    m_vSide = Vector3::eJ;

    // SetFrustrum( SCALAR_PI_2, 4.0f / 3.0f, 1.0f, 1000.0f );
    m_fFrustrum[FRUSTRUM_TOP]    = MathFn->Tan(SCALAR_PI_4);
    m_fFrustrum[FRUSTRUM_LEFT]   = (4.0f/3.0f) * m_fFrustrum[FRUSTRUM_TOP];
    m_fFrustrum[FRUSTRUM_BOTTOM] = -m_fFrustrum[FRUSTRUM_TOP];
    m_fFrustrum[FRUSTRUM_RIGHT]  = -m_fFrustrum[FRUSTRUM_LEFT];
    m_fFrustrum[FRUSTRUM_NEAR]   = 1.0f;
    m_fFrustrum[FRUSTRUM_FAR]    = 1000.0f;

    m_bPerspective = bPerspective;

    m_matPostProjection = Matrix4::Identity;
    m_bHasPostProjection = false;

    _UpdateViewMatrix();
    _UpdateProjectionMatrix();
}
Camera::~Camera()
{
    // nothing to do
}

Void Camera::SetView( const Vertex3 & vEye, const Vector3 & vDir, const Vector3 & vUp )
{
    m_vEye = vEye;

    m_vView = vDir;
    m_vView.Normalize();
    m_vSide = vUp ^ m_vView;
    m_vSide.Normalize();
    m_vUp = m_vView ^ m_vSide;
    m_vUp.Normalize();

    _UpdateViewMatrix();
}
Void Camera::SetLocation( const Vertex3 & vEye )
{
    m_vEye = vEye;

    _UpdateViewMatrix();
}
Void Camera::SetOrientation( const Vector3 & vDir, const Vector3 & vUp )
{
    m_vView = vDir;
    m_vView.Normalize();
    m_vSide = vUp ^ m_vView;
    m_vSide.Normalize();
    m_vUp = m_vView ^ m_vSide;
    m_vUp.Normalize();

    _UpdateViewMatrix();
}

Void Camera::SetFrustrum( const Scalar arrValues[FRUSTRUM_COUNT] )
{
    for( UInt iLimit = 0; iLimit < FRUSTRUM_COUNT; ++iLimit )
        m_fFrustrum[iLimit] = arrValues[iLimit];

    _UpdateProjectionMatrix();
}
Void Camera::SetFrustrum( FrustrumLimit iLimit, Scalar fValue )
{
    Assert(iLimit < FRUSTRUM_COUNT);
    m_fFrustrum[iLimit] = fValue;

    _UpdateProjectionMatrix();
}
Void Camera::SetFrustrum( Scalar fLeft, Scalar fRight, Scalar fBottom, Scalar fTop, Scalar fNear, Scalar fFar )
{
    m_fFrustrum[FRUSTRUM_LEFT]   = fLeft;
    m_fFrustrum[FRUSTRUM_RIGHT]  = fRight;
    m_fFrustrum[FRUSTRUM_BOTTOM] = fBottom;
    m_fFrustrum[FRUSTRUM_TOP]    = fTop;
    m_fFrustrum[FRUSTRUM_NEAR]   = fNear;
    m_fFrustrum[FRUSTRUM_FAR]    = fFar;

    _UpdateProjectionMatrix();
}
Void Camera::SetFrustrum( Scalar fFOV, Scalar fAspect, Scalar fNear, Scalar fFar )
{
    m_fFrustrum[FRUSTRUM_TOP]    = fNear * MathFn->Tan(0.5f * fFOV);
    m_fFrustrum[FRUSTRUM_LEFT]   = fAspect * m_fFrustrum[FRUSTRUM_TOP];
    m_fFrustrum[FRUSTRUM_BOTTOM] = -m_fFrustrum[FRUSTRUM_TOP];
    m_fFrustrum[FRUSTRUM_RIGHT]  = -m_fFrustrum[FRUSTRUM_LEFT];
    m_fFrustrum[FRUSTRUM_NEAR]   = fNear;
    m_fFrustrum[FRUSTRUM_FAR]    = fFar;

    _UpdateProjectionMatrix();
}

Void Camera::GetFrustrum( Scalar & outLeft, Scalar & outRight, Scalar & outBottom, Scalar & outTop, Scalar & outNear, Scalar & outFar ) const
{
    outLeft = m_fFrustrum[FRUSTRUM_LEFT];
    outRight = m_fFrustrum[FRUSTRUM_RIGHT];
    outBottom = m_fFrustrum[FRUSTRUM_BOTTOM];
    outTop = m_fFrustrum[FRUSTRUM_TOP];
    outNear = m_fFrustrum[FRUSTRUM_NEAR];
    outFar  = m_fFrustrum[FRUSTRUM_FAR];
}
Bool Camera::GetFrustrum( Scalar & outFOV, Scalar & outAspect, Scalar & outNear, Scalar & outFar ) const
{
    if ( m_fFrustrum[FRUSTRUM_RIGHT] == -m_fFrustrum[FRUSTRUM_LEFT] &&
         m_fFrustrum[FRUSTRUM_BOTTOM] == -m_fFrustrum[FRUSTRUM_TOP] ) {
        Scalar fTmp = m_fFrustrum[FRUSTRUM_TOP] / m_fFrustrum[FRUSTRUM_NEAR];
        outFOV = 2.0f * MathFn->ArcTan(fTmp);
        outAspect = m_fFrustrum[FRUSTRUM_LEFT] / m_fFrustrum[FRUSTRUM_TOP];
        outNear = m_fFrustrum[FRUSTRUM_NEAR];
        outFar = m_fFrustrum[FRUSTRUM_FAR];
        return true;
    }
    return false;
}

Void Camera::ExtractFrustrumPlanes( Plane arrPlanes[FRUSTRUM_COUNT], const Scalar arrOverride[FRUSTRUM_COUNT] ) const
{
    // Assume RH camera frame, produce outward normals ...
    Vector3 vNormal;

    // Get Frustrum values
    Scalar fRight, fLeft, fBottom, fTop, fNear, fFar;
    if ( arrOverride == NULL ) {
        fRight  = m_fFrustrum[FRUSTRUM_RIGHT];
        fLeft   = m_fFrustrum[FRUSTRUM_LEFT];
        fBottom = m_fFrustrum[FRUSTRUM_BOTTOM];
        fTop    = m_fFrustrum[FRUSTRUM_TOP];
        fNear   = m_fFrustrum[FRUSTRUM_NEAR];
        fFar    = m_fFrustrum[FRUSTRUM_FAR];
    } else {
        fRight  = arrOverride[FRUSTRUM_RIGHT];
        fLeft   = arrOverride[FRUSTRUM_LEFT];
        fBottom = arrOverride[FRUSTRUM_BOTTOM];
        fTop    = arrOverride[FRUSTRUM_TOP];
        fNear   = arrOverride[FRUSTRUM_NEAR];
        fFar    = arrOverride[FRUSTRUM_FAR];
    }

    // Near/Far planes  
    arrPlanes[FRUSTRUM_NEAR] = Plane( -m_vView, m_vEye + ( m_vView * fNear ) );
    arrPlanes[FRUSTRUM_FAR]  = Plane( +m_vView, m_vEye + ( m_vView * fFar ) );

    // Right/Left planes
        // +( (nV + rS) ^ U ) = +( -nS + rV )
    vNormal = - (m_vSide * fNear) + (m_vView * fRight);
    vNormal.Normalize();
    arrPlanes[FRUSTRUM_RIGHT] = Plane( vNormal, m_vEye );
        // -( (nV + lS) ^ U ) = -( -nS + lV )
    vNormal = + (m_vSide * fNear) - (m_vView * fLeft);
    vNormal.Normalize();
    arrPlanes[FRUSTRUM_LEFT] = Plane( vNormal, m_vEye );

    // Bottom/Top planes
        // -( (nV + bU) ^ S ) = -( nU - bV )
    vNormal = - (m_vUp * fNear) + (m_vView * fBottom);
    vNormal.Normalize();
    arrPlanes[FRUSTRUM_BOTTOM] = Plane( vNormal, m_vEye );
        // +( (nV + tU) ^ S ) = +( nU - tV )
    vNormal = + (m_vUp * fNear) - (m_vView * fTop);
    vNormal.Normalize();
    arrPlanes[FRUSTRUM_TOP] = Plane( vNormal, m_vEye );
}
Void Camera::ExtractFrustrumVertices( Vertex3 arrVertices[8], const Scalar arrOverride[FRUSTRUM_COUNT] ) const
{
    // Assume RH camera frame ...

    // Get Frustrum values
    Scalar fRight, fLeft, fBottom, fTop, fNear, fFar;
    if ( arrOverride == NULL ) {
        fRight  = m_fFrustrum[FRUSTRUM_RIGHT];
        fLeft   = m_fFrustrum[FRUSTRUM_LEFT];
        fBottom = m_fFrustrum[FRUSTRUM_BOTTOM];
        fTop    = m_fFrustrum[FRUSTRUM_TOP];
        fNear   = m_fFrustrum[FRUSTRUM_NEAR];
        fFar    = m_fFrustrum[FRUSTRUM_FAR];
    } else {
        fRight  = arrOverride[FRUSTRUM_RIGHT];
        fLeft   = arrOverride[FRUSTRUM_LEFT];
        fBottom = arrOverride[FRUSTRUM_BOTTOM];
        fTop    = arrOverride[FRUSTRUM_TOP];
        fNear   = arrOverride[FRUSTRUM_NEAR];
        fFar    = arrOverride[FRUSTRUM_FAR];
    }

    // Near vertices
    Scalar fFdN = ( fFar / fNear );
    Vector3 vViewN = m_vView * fNear;
    Vector3 vViewF = m_vView * fFar;

    Vector3 vSideR = m_vSide * fRight;
    Vector3 vSideL = m_vSide * fLeft;
    Vector3 vUpB   = m_vUp * fBottom;
    Vector3 vUpT   = m_vUp * fTop;

    arrVertices[0] = m_vEye + vViewN + vSideR + vUpB;
    arrVertices[1] = m_vEye + vViewN + vSideL + vUpB;
    arrVertices[2] = m_vEye + vViewN + vSideR + vUpT;
    arrVertices[3] = m_vEye + vViewN + vSideL + vUpT;

    // Far vertices
    vSideR *= fFdN;
    vSideL *= fFdN;
    vUpB *= fFdN;
    vUpT *= fFdN;

    arrVertices[4] = m_vEye + vViewF + vSideR + vUpB;
    arrVertices[5] = m_vEye + vViewF + vSideL + vUpB;
    arrVertices[6] = m_vEye + vViewF + vSideR + vUpT;
    arrVertices[7] = m_vEye + vViewF + vSideL + vUpT;
}

Void Camera::ComputeAABB( const Byte * pVertices, UInt iVertexCount, UInt iStride, const Matrix4 & matWorld,
                          Scalar & outRight, Scalar & outLeft, Scalar & outBottom, Scalar & outTop )
{
    Matrix4 matWVP = m_matCamera * matWorld;

    outRight = SCALAR_INFINITE;
    outLeft = -SCALAR_INFINITE;
    outBottom = SCALAR_INFINITE;
    outTop = -SCALAR_INFINITE;

    for( UInt i = 0; i < iVertexCount; ++i ) {
        const Scalar * pVertex = (const Scalar *)pVertices;
        Vertex4 vPos( pVertex[0], pVertex[1], pVertex[2] );
        Vertex4 vTPos = matWVP * vPos;
        Scalar fInvW = MathFn->Invert( vTPos.W );
        Scalar fNDCX = vTPos.X * fInvW;
        Scalar fNDCY = vTPos.Y * fInvW;
        if (fNDCX < outRight)
            outRight = fNDCX;
        if (fNDCX > outLeft)
            outLeft = fNDCX;
        if (fNDCY < outBottom)
            outBottom = fNDCY;
        if (fNDCY > outTop)
            outTop = fNDCY;
        pVertices += iStride;
    }
}

/////////////////////////////////////////////////////////////////////////////////

Void Camera::_UpdateViewMatrix()
{
    // RH convention
    Vector3 vEyeVect(m_vEye.X, m_vEye.Y, m_vEye.Z);

    m_matView.m00 = -m_vSide.X; m_matView.m01 = -m_vSide.Y; m_matView.m02 = -m_vSide.Z; m_matView.m03 = +(vEyeVect * m_vSide);
    m_matView.m10 = m_vUp.X;    m_matView.m11 = m_vUp.Y;    m_matView.m12 = m_vUp.Z;    m_matView.m13 = -(vEyeVect * m_vUp);
    m_matView.m20 = -m_vView.X; m_matView.m21 = -m_vView.Y; m_matView.m22 = -m_vView.Z; m_matView.m23 = +(vEyeVect * m_vView);
    m_matView.m30 = 0.0f;       m_matView.m31 = 0.0f;       m_matView.m32 = 0.0f;       m_matView.m33 = 1.0f;

    m_matInvView.m00 = -m_vSide.X; m_matInvView.m01 = m_vUp.X; m_matInvView.m02 = -m_vView.X; m_matInvView.m03 = m_vEye.X;
    m_matInvView.m10 = -m_vSide.Y; m_matInvView.m11 = m_vUp.Y; m_matInvView.m12 = -m_vView.Y; m_matInvView.m13 = m_vEye.Y;
    m_matInvView.m20 = -m_vSide.Z; m_matInvView.m21 = m_vUp.Z; m_matInvView.m22 = -m_vView.Z; m_matInvView.m23 = m_vEye.Z;
    m_matInvView.m30 = 0.0f;       m_matInvView.m31 = 0.0f;    m_matInvView.m32 = 0.0f;       m_matInvView.m33 = 1.0f;

    m_matBillboard.m00 = -m_vSide.X; m_matBillboard.m01 = m_vUp.X; m_matBillboard.m02 = -m_vView.X; m_matBillboard.m03 = 0.0f;
    m_matBillboard.m10 = -m_vSide.Y; m_matBillboard.m11 = m_vUp.Y; m_matBillboard.m12 = -m_vView.Y; m_matBillboard.m13 = 0.0f;
    m_matBillboard.m20 = -m_vSide.Z; m_matBillboard.m21 = m_vUp.Z; m_matBillboard.m22 = -m_vView.Z; m_matBillboard.m23 = 0.0f;
    m_matBillboard.m30 = 0.0f;       m_matBillboard.m31 = 0.0f;    m_matBillboard.m32 = 0.0f;       m_matBillboard.m33 = 1.0f;

    _UpdateCameraMatrix();
}
Void Camera::_UpdateProjectionMatrix()
{
    // RH convention
    Scalar fInvLmR = MathFn->Invert( m_fFrustrum[FRUSTRUM_LEFT] - m_fFrustrum[FRUSTRUM_RIGHT] );
    Scalar fInvTmB = MathFn->Invert( m_fFrustrum[FRUSTRUM_TOP] - m_fFrustrum[FRUSTRUM_BOTTOM] );
    Scalar fInvFmN = MathFn->Invert( m_fFrustrum[FRUSTRUM_FAR] - m_fFrustrum[FRUSTRUM_NEAR] );

    if ( m_bPerspective ) {
        m_matProjection.m00 = 2.0f * m_fFrustrum[FRUSTRUM_NEAR] * fInvLmR;
        m_matProjection.m10 = 0.0f;
        m_matProjection.m20 = 0.0f;
        m_matProjection.m30 = 0.0f;

        m_matProjection.m01 = 0.0f;
        m_matProjection.m11 = 2.0f * m_fFrustrum[FRUSTRUM_NEAR] * fInvTmB;
        m_matProjection.m21 = 0.0f;
        m_matProjection.m31 = 0.0f;

        m_matProjection.m02 = (m_fFrustrum[FRUSTRUM_LEFT] + m_fFrustrum[FRUSTRUM_RIGHT]) * fInvLmR;
        m_matProjection.m12 = (m_fFrustrum[FRUSTRUM_TOP] + m_fFrustrum[FRUSTRUM_BOTTOM]) * fInvTmB;
        ///////////////////
        m_matProjection.m32 = -1.0f;

        m_matProjection.m03 = 0.0f;
        m_matProjection.m13 = 0.0f;
        ///////////////////
        m_matProjection.m33 = 0.0f;

        if ( m_iDepthMapping == PROJECT_DEPTH_ZERO_ONE ) {
            m_matProjection.m22 = -m_fFrustrum[FRUSTRUM_FAR] * fInvFmN;
            m_matProjection.m23 = -m_fFrustrum[FRUSTRUM_FAR] * m_fFrustrum[FRUSTRUM_NEAR] * fInvFmN;
        } else if ( m_iDepthMapping == PROJECT_DEPTH_MINUSONE_ONE ) {
            m_matProjection.m22 = -(m_fFrustrum[FRUSTRUM_FAR] + m_fFrustrum[FRUSTRUM_NEAR]) * fInvFmN;
            m_matProjection.m23 = -2.0f * m_fFrustrum[FRUSTRUM_FAR] * m_fFrustrum[FRUSTRUM_NEAR] * fInvFmN;
        } else
            Assert(false);
    } else {
        m_matProjection.m00 = 2.0f * fInvLmR;
        m_matProjection.m10 = 0.0f;
        m_matProjection.m20 = 0.0f;
        m_matProjection.m30 = 0.0f;

        m_matProjection.m01 = 0.0f;
        m_matProjection.m11 = 2.0f * fInvTmB;
        m_matProjection.m21 = 0.0f;
        m_matProjection.m31 = 0.0f;

        m_matProjection.m02 = 0.0f;
        m_matProjection.m12 = 0.0f;
        ///////////////////
        m_matProjection.m32 = 0.0f;

        m_matProjection.m03 = -(m_fFrustrum[FRUSTRUM_LEFT] + m_fFrustrum[FRUSTRUM_RIGHT]) * fInvLmR;
        m_matProjection.m13 = -(m_fFrustrum[FRUSTRUM_TOP] + m_fFrustrum[FRUSTRUM_BOTTOM]) * fInvTmB;
        ///////////////////
        m_matProjection.m33 = 1.0f;

        if ( m_iDepthMapping == PROJECT_DEPTH_ZERO_ONE ) {
            m_matProjection.m22 = -fInvFmN;
            m_matProjection.m23 = -m_fFrustrum[FRUSTRUM_NEAR] * fInvFmN;
        } else if ( m_iDepthMapping == PROJECT_DEPTH_MINUSONE_ONE ) {
            m_matProjection.m22 = -2.0f * fInvFmN;
            m_matProjection.m23 = -(m_fFrustrum[FRUSTRUM_FAR] + m_fFrustrum[FRUSTRUM_NEAR]) * fInvFmN;
        } else
            Assert(false);
    }

    _UpdateCameraMatrix();
}
Void Camera::_UpdateCameraMatrix()
{
    if ( m_bHasPostProjection ) {
        m_matCamera = m_matPostProjection * m_matProjection;
    } else
        m_matCamera = m_matProjection;
    m_matCamera *= m_matView;
    if ( m_bHasPreView )
        m_matCamera *= m_matPreView;
}

/////////////////////////////////////////////////////////////////////////////////
// Projector implementation
const Matrix4 Projector::BiasScaleMatrix[2] =
{
    Matrix4 (
        0.5f,  0.0f, 0.0f, 0.5f,
        0.0f, -0.5f, 0.0f, 0.5f,
        0.0f,  0.0f, 1.0f, 0.0f,
        0.0f,  0.0f, 0.0f, 1.0f
    ),
    Matrix4 (
        0.5f,  0.0f, 0.0f, 0.5f,
        0.0f, +0.5f, 0.0f, 0.5f,
        0.0f,  0.0f, 1.0f, 0.0f,
        0.0f,  0.0f, 0.0f, 1.0f
    )
};

Projector::Projector( ProjectionDepthMapping iDepthMapping, Bool bPerspective ):
    Camera(bPerspective)
{
    m_iDepthMapping = iDepthMapping;
    _UpdateProjectionMatrix();
}
Projector::~Projector()
{
    // nothing to do
}

