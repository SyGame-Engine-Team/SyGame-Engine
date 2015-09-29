/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/NarrowPhase/Primitive/SphereSphereCollision.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Narrow-Phase : Primitive, Sphere-Sphere
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
#include "SphereSphereCollision.h"

/////////////////////////////////////////////////////////////////////////////////
// SphereSphereCollision implementation
SphereSphereCollision::SphereSphereCollision( TransformedShapeConvex * pSphereA, TransformedShapeConvex * pSphereB ):
    NarrowPhaseConvex( pSphereA, pSphereB )
{
    Assert( pSphereA->GetType() == SHAPE_PRIMITIVE_SPHERE );
    Assert( pSphereB->GetType() == SHAPE_PRIMITIVE_SPHERE );
}
SphereSphereCollision::~SphereSphereCollision()
{
    // nothing to do
}

Void SphereSphereCollision::Initialize()
{
    // nothing to do
}
Bool SphereSphereCollision::DetectCollision( Bool bGenerateContactPoints )
{
    const ShapeSphere * pSphereA = (const ShapeSphere*)( m_pShapeA->GetShape() );
    const ShapeSphere * pSphereB = (const ShapeSphere*)( m_pShapeB->GetShape() );

    Vertex3 vCenterA = m_pShapeA->GetTransform() * pSphereA->GetSphere().Center;
    Vertex3 vCenterB = m_pShapeB->GetTransform() * pSphereB->GetSphere().Center;

    Scalar fRadiusA = pSphereA->GetSphere().Radius;
    Scalar fRadiusB = pSphereB->GetSphere().Radius;

    // Compute normal & distance
    Vector3 vNormal = ( vCenterA - vCenterB );
    Scalar fDistance = ( vNormal.Normalize() - (fRadiusA + fRadiusB) );

    // Generate contact points
    m_pCollisionManifold->Clear();

    if ( bGenerateContactPoints ) {
        m_pCollisionManifold->AddPoint( ContactPointID::CPID_NULL, ( vCenterA - (vNormal * fRadiusA) ),
                                                                   ( vCenterB + (vNormal * fRadiusB) ),
                                                                   vNormal, fDistance );
    }

    return ( fDistance < 0.0f );
}
