/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/WorldCamera.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Graph : Camera management
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
#include "WorldCamera.h"

#include "WorldManager.h"

/////////////////////////////////////////////////////////////////////////////////
// WorldCamera implementation
WorldCamera::WorldCamera( Camera * pCamera, WorldBaseNode * pTarget )
{
    Assert( pCamera != NULL && pTarget != NULL );

    m_pCamera = pCamera;
    m_pTarget = pTarget;
}
WorldCamera::~WorldCamera()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// WorldCameraSubjective implementation
WorldCameraSubjective::WorldCameraSubjective( Camera * pCamera, WorldBaseNode * pTarget ):
    WorldCamera( pCamera, pTarget )
{
    // nothing to do
}
WorldCameraSubjective::~WorldCameraSubjective()
{
    // nothing to do
}

Void WorldCameraSubjective::Update( Scalar /*fTimeStep*/ )
{
    // Update camera frame
    const Transform3 & vWorldTransform = m_pTarget->WorldTransform;

    Vector3 vUp, vDir;
    Vertex3 vEye = ( Vertex3::Null + vWorldTransform.GetTranslate() );
    vWorldTransform.GetRotate().GetColumn( vUp, 1 );
    vWorldTransform.GetRotate().GetColumn( vDir, 2 );

    m_pCamera->SetView( vEye, vDir, vUp );
}

/////////////////////////////////////////////////////////////////////////////////
// WorldCamera3rdPerson implementation
WorldCamera3rdPerson::WorldCamera3rdPerson( Camera * pCamera, WorldBaseNode * pTarget, ManualController * pCameraController,
                                            Scalar fRadius, Scalar fAngleH, Scalar fAngleV, Axis iUpAxis ):
    WorldCamera( pCamera, pTarget ),
    m_vUpAxis( Vector3::Null )
{
    m_pCameraController = pCameraController;

    m_iUpAxis = iUpAxis;
    m_vUpAxis[m_iUpAxis] = 1.0f;

    m_fMinRadius = WORLDCAMERA_MIN_RADIUS;
    m_fMaxRadius = WORLDCAMERA_MAX_RADIUS;

    m_fRadius = 0.0f;
    m_fAngleH = 0.0f;
    m_fAngleV = 0.0f;
    m_fInvAngleV = -1.0f;
    m_fTargetHeight = 0.0f;

    Zoom( fRadius );
    RotateH( fAngleH );
    RotateV( fAngleV );
}
WorldCamera3rdPerson::~WorldCamera3rdPerson()
{
    // nothing to do
}

Void WorldCamera3rdPerson::RotateV( Scalar fAngle )
{
	if ( m_fAngleV >= 0.0f ) {
        m_fAngleV += fAngle;
        if ( m_fAngleV >= SCALAR_PI_2 )
            m_fAngleV = ( SCALAR_PI_2 - 1.0e-3f );
        if ( m_fAngleV < 0.0f ) {
            m_fAngleV = -1.0f; // flag unactive
            m_fInvAngleV = 1.0e-2f;
            m_fTargetHeight = MathFn->Tan(m_fInvAngleV) * m_fRadius;
        }
    } else {
        m_fInvAngleV -= fAngle;
        if (m_fInvAngleV >= SCALAR_PI_2)
			m_fInvAngleV = ( SCALAR_PI_2 - 1.0e-3f );
        if ( m_fInvAngleV < 0.0f ) {
            m_fInvAngleV = -1.0f; // flag unactive
            m_fAngleV = 1.0e-2f;
            m_fTargetHeight = 0.0f;
        } else
            m_fTargetHeight = MathFn->Tan(m_fInvAngleV) * m_fRadius;
	}
}

Void WorldCamera3rdPerson::Update( Scalar /*fTimeStep*/ )
{
    // Update camera frame
    Vertex3 vTarget = ( Vertex3::Null + m_pTarget->WorldTransform.GetTranslate() );
    Bool bLookAtTangent = ( m_fAngleV < 0 );

    Scalar fAngleV = bLookAtTangent ? 0.0f : m_fAngleV;
    Scalar fSinAngleH, fCosAngleH, fSinAngleV, fCosAngleV;
    MathFn->SinCos( &fSinAngleH, &fCosAngleH, m_fAngleH );
    MathFn->SinCos( &fSinAngleV, &fCosAngleV, fAngleV );

    Axis iAxis0 = (Axis)( (m_iUpAxis + 1) % 3 );
    Axis iAxis1 = (Axis)( (iAxis0 + 1) % 3 );
    Vertex3 vEye;
    vEye[iAxis0]    = vTarget[iAxis0]    + ( fCosAngleV * fCosAngleH * m_fRadius );
    vEye[iAxis1]    = vTarget[iAxis1]    + ( fCosAngleV * fSinAngleH * m_fRadius );
    vEye[m_iUpAxis] = vTarget[m_iUpAxis] + ( fSinAngleV * m_fRadius );

    if ( bLookAtTangent )
        vTarget[m_iUpAxis] += m_fTargetHeight;

    m_pCamera->SetView( vEye, (vTarget - vEye), m_vUpAxis );

    // Update camera controller
    if ( m_pCameraController != NULL ) {
        m_pCameraController->SetPosition( vEye );

        Matrix3 matRotation( m_pCamera->GetViewSide(),
                             m_pCamera->GetViewUp(),
                             m_pCamera->GetViewDir(), false );
        Quaternion qOrientation( matRotation );
        m_pCameraController->SetOrientation( matRotation );
    }
}

