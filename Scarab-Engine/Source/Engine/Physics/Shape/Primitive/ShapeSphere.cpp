/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Shape/Primitive/ShapeSphere.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Shapes : Primitive, Sphere
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "ShapeSphere.h"

/////////////////////////////////////////////////////////////////////////////////
// ShapeSphere implementation
ShapeSphere::ShapeSphere( const Sphere & vSphere, UInt iSliceSamples, UInt iRadialSamples ):
    ShapeConvex( NULL ), m_vSphere( vSphere )
{
    Assert( iSliceSamples >= 1 && iRadialSamples >= 3 );

    // Sampling
    m_iSliceSamples = iSliceSamples;
    m_iRadialSamples = iRadialSamples;
    m_fSliceHeight = ( (m_vSphere.Radius * 2.0f) / (Scalar)(m_iSliceSamples + 1) );
    m_fRadialAngle = ( SCALAR_2PI / (Scalar)m_iRadialSamples );

    // Local AAB
    m_bvLocalAAB = BVAABox( m_vSphere.Center, Vector3(m_vSphere.Radius,m_vSphere.Radius,m_vSphere.Radius) );
}
ShapeSphere::ShapeSphere( MeshSphere * pSphereMesh ):
    ShapeConvex( pSphereMesh ), m_vSphere( pSphereMesh->GetGeometry() )
{
    // Sampling
    m_iSliceSamples = pSphereMesh->GetSliceSamples();
    m_iRadialSamples = pSphereMesh->GetRadialSamples();
    m_fSliceHeight = ( (m_vSphere.Radius * 2.0f) / (Scalar)(m_iSliceSamples + 1) );
    m_fRadialAngle = ( SCALAR_2PI / (Scalar)m_iRadialSamples );

    // Local AAB
    m_bvLocalAAB = BVAABox( m_vSphere.Center, Vector3(m_vSphere.Radius,m_vSphere.Radius,m_vSphere.Radius) );
}
ShapeSphere::~ShapeSphere()
{
    // nothing to do
}

Void ShapeSphere::ComputeMassProperties( Scalar fDensity, Scalar * outMass, Vertex3 * outCenterOfMass, Matrix3 * outInertiaTensor ) const
{
    Scalar fRadius2 = ( m_vSphere.Radius * m_vSphere.Radius );
    Scalar fRadius3 = ( m_vSphere.Radius * fRadius2 );

    *outMass = SCALAR_4PI_3 * fRadius3 * fDensity;

    *outCenterOfMass = m_vSphere.Center;

    outInertiaTensor->MakeIdentity();
    (*outInertiaTensor) *= ( 0.4f * (*outMass) * fRadius2 ); // 2/5 = 0.4
}

Void ShapeSphere::QueryExtremalVertices( const Vector3 & vDirection, UInt * outMax, UInt * outMin ) const
{
    Assert( m_pMesh != NULL );
    const MeshSphere * pSphereMesh = (const MeshSphere *)m_pMesh;

    Vertex3 vMax = m_vSphere.Center + ( vDirection * m_vSphere.Radius );
    Scalar fHeight = m_vSphere.Radius + (vMax.Z - m_vSphere.Center.Z);

    Scalar fMaxHeight = (m_vSphere.Radius * 2.0f);
    Scalar fHalfSliceHeight = (m_fSliceHeight * 0.5f);

    if ( fHeight < fHalfSliceHeight ) {
        if ( outMax != NULL )
            *outMax = pSphereMesh->GetVertexCount() - 2;
        if ( outMin != NULL )
            *outMin = pSphereMesh->GetVertexCount() - 1;
    } else if ( fHeight > (fMaxHeight - fHalfSliceHeight) ) {
        if ( outMax != NULL )
            *outMax = pSphereMesh->GetVertexCount() - 1;
        if ( outMin != NULL )
            *outMin = pSphereMesh->GetVertexCount() - 2;
    } else {
        Scalar fSlice = fHeight / m_fSliceHeight;
        UInt iSlice = (UInt)( MathFn->Round(fSlice) );

        Scalar fAngle = MathFn->ArcTan2( vMax.Y, vMax.X );
        Scalar fRadius = fAngle / m_fRadialAngle;
        UInt iRadius = (UInt)( MathFn->Round(fRadius) );

        if ( outMax != NULL )
            *outMax = (iSlice * m_iRadialSamples) + iRadius;
        if ( outMin != NULL ) {
            iSlice = (m_iSliceSamples - 1) - iSlice;
            iRadius = (m_iRadialSamples - 1) - iRadius;
            *outMin = (iSlice * m_iRadialSamples) + iRadius;
        }
    }
}
Void ShapeSphere::QueryExtremalVertices( const Vector3 & vDirection, Vertex3 * outMax, Vertex3 * outMin ) const
{
    if ( outMax != NULL )
        *outMax = m_vSphere.Center + ( vDirection * m_vSphere.Radius );
    if ( outMin != NULL )
        *outMin = m_vSphere.Center - ( vDirection * m_vSphere.Radius );
}

