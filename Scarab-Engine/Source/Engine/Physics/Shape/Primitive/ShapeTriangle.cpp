/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Shape/Primitive/ShapeTriangle.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Shapes : Primitive, Triangle
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
#include "ShapeTriangle.h"

#include "../../PhysicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// ShapeTriangle implementation
ShapeTriangle::ShapeTriangle( const Triangle3 & vTriangle, Scalar fThickness ):
    ShapeConvex( NULL ), m_hGraph(), m_vTriangle( vTriangle )
{
    m_fThickness = fThickness;

    // Graph data
    m_hGraph.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("Shapes") );
    m_hGraph.Create();

    MeshTriangle::GenerateGraph( &m_hGraph );

    Vector3 vNormal = ( (m_vTriangle.B - m_vTriangle.A) ^ (m_vTriangle.C - m_vTriangle.A) );
    vNormal.Normalize();
    vNormal *= m_fThickness;
    const Vertex3 arrV[6] = { m_vTriangle.A, m_vTriangle.B, m_vTriangle.C,
                              m_vTriangle.A - vNormal, m_vTriangle.C - vNormal, m_vTriangle.B - vNormal };
    m_hGraph.ComputeFeatureData( arrV );

    // Local AAB
    m_bvLocalAAB.Reset( arrV[0] );
    for( UInt i = 1; i < 6; ++i )
        m_bvLocalAAB.Grow( arrV[i] );
}
ShapeTriangle::ShapeTriangle( MeshTriangle * pTriangleMesh, Scalar fThickness ):
    ShapeConvex( pTriangleMesh ), m_hGraph(), m_vTriangle( pTriangleMesh->GetGeometry() )
{
    m_fThickness = fThickness;

    // Graph data
    m_hGraph.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("Shapes") );
    m_hGraph.Create();

    MeshTriangle::GenerateGraph( &m_hGraph );

    Vector3 vNormal = ( (m_vTriangle.B - m_vTriangle.A) ^ (m_vTriangle.C - m_vTriangle.A) );
    vNormal.Normalize();
    vNormal *= m_fThickness;
    const Vertex3 arrV[6] = { m_vTriangle.A, m_vTriangle.B, m_vTriangle.C,
                              m_vTriangle.A - vNormal, m_vTriangle.C - vNormal, m_vTriangle.B - vNormal };
    m_hGraph.ComputeFeatureData( arrV );

    // Local AAB
    m_bvLocalAAB.Reset( arrV[0] );
    for( UInt i = 1; i < 6; ++i )
        m_bvLocalAAB.Grow( arrV[i] );
}
ShapeTriangle::~ShapeTriangle()
{
    m_hGraph.Destroy();
}

Void ShapeTriangle::ComputeMassProperties( Scalar fDensity, Scalar * outMass, Vertex3 * outCenterOfMass, Matrix3 * outInertiaTensor ) const
{
    static const Scalar fInv3 = MathFn->Invert( 3.0f );
    static const Scalar fInv24 = MathFn->Invert( 24.0f );
    static const Matrix3 matS(
        2.0f, 1.0f, 1.0f,
        1.0f, 2.0f, 1.0f,
        1.0f, 1.0f, 2.0f
    );

    Vector3 vA = m_vTriangle.A.ToVector();
    Vector3 vB = m_vTriangle.B.ToVector();
    Vector3 vC = m_vTriangle.C.ToVector();

    Vector3 vNormal = ( (m_vTriangle.B - m_vTriangle.A) ^ (m_vTriangle.C - m_vTriangle.A) );
    Scalar fTwiceArea = vNormal.Norm();

    // Mass
    *outMass = ( (fTwiceArea * 0.5f) * m_fThickness ) * fDensity;

    // Center of mass
    *outCenterOfMass = m_vTriangle.A;
    *outCenterOfMass += vB;
    *outCenterOfMass += vC;
    *outCenterOfMass *= fInv3;

    // Inertia tensor
    Scalar fNormSqrA = vA.NormSqr();
    Scalar fNormSqrB = vB.NormSqr();
    Scalar fNormSqrC = vC.NormSqr();
    Scalar fNormSqrSum = (vA + vB + vC).NormSqr();

    outInertiaTensor->MakeIdentity();
    *outInertiaTensor *= ( fNormSqrA + fNormSqrB + fNormSqrC + fNormSqrSum );

    Matrix3 matV, matInvV;
    matV.SetRow( 0, vA );
    matV.SetRow( 1, vB );
    matV.SetRow( 2, vC );
    matV.Transpose( matInvV );
    *outInertiaTensor -= ( matInvV * matS * matV);

    *outInertiaTensor *= ( fTwiceArea * fInv24 );
}

Void ShapeTriangle::QueryExtremalVertices( const Vector3 & vDirection, UInt * outMax, UInt * outMin ) const
{
    Vector3 vProjDir = vDirection;
    vProjDir.ProjectToPlane( m_vTriangle.GetPlane().Normal );

    Vector3 vAB = ( m_vTriangle.B - m_vTriangle.A );
    Vector3 vAC = ( m_vTriangle.C - m_vTriangle.A );
    Vector3 vBC = ( m_vTriangle.C - m_vTriangle.B );
    Scalar fDotAB = ( vProjDir * vAB );
    Scalar fDotAC = ( vProjDir * vAC );
    Scalar fDotBC = ( vProjDir * vBC );

    if ( outMax != NULL ) {
        if ( fDotAB < 0.0f && fDotAC < 0.0f )
            *outMax = 0;
        else if ( fDotAB >= 0.0f && fDotBC < 0.0f )
            *outMax = 1;
        else if ( fDotAC >= 0.0f && fDotBC >= 0.0f )
            *outMax = 2;
        else
            Assert( false );
    }
    if ( outMin != NULL ) {
        if ( fDotAB >= 0.0f && fDotAC >= 0.0f )
            *outMin = 0;
        else if ( fDotAB < 0.0f && fDotBC >= 0.0f )
            *outMin = 1;
        else if ( fDotAC < 0.0f && fDotBC < 0.0f )
            *outMin = 2;
        else
            Assert( false );
    }
}
Void ShapeTriangle::QueryExtremalVertices( const Vector3 & vDirection, Vertex3 * outMax, Vertex3 * outMin ) const
{
    Plane vPlane = m_vTriangle.GetPlane();
    Scalar fDotN = ( vDirection * vPlane.Normal );

    Vector3 vProjDir = ( vDirection - (vPlane.Normal * fDotN) );

    Vector3 vAB = ( m_vTriangle.B - m_vTriangle.A );
    Vector3 vAC = ( m_vTriangle.C - m_vTriangle.A );
    Vector3 vBC = ( m_vTriangle.C - m_vTriangle.B );
    Scalar fDotAB = ( vProjDir * vAB );
    Scalar fDotAC = ( vProjDir * vAC );
    Scalar fDotBC = ( vProjDir * vBC );

    if ( outMax != NULL ) {
        if ( fDotAB < 0.0f && fDotAC < 0.0f )
            *outMax = m_vTriangle.A;
        else if ( fDotAB >= 0.0f && fDotBC < 0.0f )
            *outMax = m_vTriangle.B;
        else if ( fDotAC >= 0.0f && fDotBC >= 0.0f )
            *outMax = m_vTriangle.C;
        else
            Assert( false );
    }
    if ( outMin != NULL ) {
        if ( fDotAB >= 0.0f && fDotAC >= 0.0f )
            *outMin = m_vTriangle.A;
        else if ( fDotAB < 0.0f && fDotBC >= 0.0f )
            *outMin = m_vTriangle.B;
        else if ( fDotAC < 0.0f && fDotBC < 0.0f )
            *outMin = m_vTriangle.C;
        else
            Assert( false );
    }

    Vector3 vAdjust = ( vPlane.Normal * m_fThickness );
    if ( fDotN < 0.0f )
        vAdjust = -vAdjust;
    if ( outMax != NULL )
        *outMax += vAdjust;
    if ( outMin != NULL )
        *outMin -= vAdjust;
}


