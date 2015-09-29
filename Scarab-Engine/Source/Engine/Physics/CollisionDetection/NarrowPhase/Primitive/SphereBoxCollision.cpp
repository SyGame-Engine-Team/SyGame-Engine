/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/NarrowPhase/Primitive/SphereBoxCollision.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Narrow-Phase : Primitive, Sphere-Box
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
#include "SphereBoxCollision.h"

/////////////////////////////////////////////////////////////////////////////////
// SphereBoxCollision implementation
SphereBoxCollision::SphereBoxCollision( TransformedShapeConvex * pShapeA, TransformedShapeConvex * pShapeB ):
    NarrowPhaseConvex( pShapeA, pShapeB )
{
    if ( pShapeA->GetType() == SHAPE_PRIMITIVE_SPHERE ) {
        Assert( pShapeB->GetType() == SHAPE_PRIMITIVE_BOX );
        m_pSphere = pShapeA;
        m_pBox = pShapeB;
        m_bReverseNormal = false;
    } else {
        Assert( pShapeA->GetType() == SHAPE_PRIMITIVE_BOX );
        Assert( pShapeB->GetType() == SHAPE_PRIMITIVE_SPHERE );
        m_pSphere = pShapeB;
        m_pBox = pShapeA;
        m_bReverseNormal = true;
    }
}
SphereBoxCollision::~SphereBoxCollision()
{
    // nothing to do
}

Void SphereBoxCollision::Initialize()
{
    // nothing to do
}
Bool SphereBoxCollision::DetectCollision( Bool bGenerateContactPoints )
{
    const ShapeSphere * pSphere = (const ShapeSphere *)( m_pSphere->GetShape() );
    const ShapeBox * pBox = (const ShapeBox *)( m_pBox->GetShape() );

    Vertex3 vSphereCenter = m_pSphere->GetTransform() * pSphere->GetSphere().Center;
    Scalar fSphereRadius = pSphere->GetSphere().Radius;

    Vertex3 vBoxCenter = m_pBox->GetTransform() * pBox->GetBox().Center;
    const Vector3 & vBoxExtents = pBox->GetBox().Extents;

    // Get OBB axis
    Vector3 vAxis[3];
    m_pBox->GetTransform().GetRotate().GetColumn( vAxis[0], 0 );
    m_pBox->GetTransform().GetRotate().GetColumn( vAxis[1], 1 );
    m_pBox->GetTransform().GetRotate().GetColumn( vAxis[2], 2 );
    vAxis[0].Normalize();
    vAxis[1].Normalize();
    vAxis[2].Normalize();

    // Compute closest point on the box to sphere
    // center, also handle penetration case
    Vector3 vU = ( vSphereCenter - vBoxCenter );
    Scalar fDist, fMinPenetration;
    UInt iMinAxis;

    UInt iPenetrationCount = 0;
    Scalar fMinAbsPenetration = SCALAR_INFINITE;
    Scalar fAbsPenetration;

    Vertex3 vClosestPointBox = vBoxCenter;
    for( UInt i = 0; i < 3; ++i ) {
        fDist = ( vU * vAxis[i] );
        if ( fDist > vBoxExtents[i] )
            fDist = vBoxExtents[i];
        else if ( fDist < -vBoxExtents[i] )
            fDist = -vBoxExtents[i];
        else {
            ++iPenetrationCount;
            fAbsPenetration = ( vBoxExtents[i] - MathFn->Abs(fDist) );
            if ( fAbsPenetration < fMinAbsPenetration ) {
                fMinAbsPenetration = fAbsPenetration;
                fMinPenetration = (fDist < 0.0f) ? -fAbsPenetration : fAbsPenetration;
                iMinAxis = i;
            }
        }
        vClosestPointBox += ( vAxis[i] * fDist );
    }
    if ( iPenetrationCount == 3 )
        vClosestPointBox += ( vAxis[iMinAxis] * fMinPenetration );

    // Compute normal & distance
    Vector3 vNormal = ( vSphereCenter - vClosestPointBox );
    Scalar fDistance = ( vNormal.Normalize() - fSphereRadius );

    // Compute closest point on the sphere
    Vertex3 vClosestPointSphere = vSphereCenter - ( vNormal * fSphereRadius );

    // Generate contact points
    m_pCollisionManifold->Clear();

    if ( bGenerateContactPoints ) {
        if ( m_bReverseNormal )
            m_pCollisionManifold->AddPoint( ContactPointID::CPID_NULL, vClosestPointBox, vClosestPointSphere, -vNormal, fDistance );
        else
            m_pCollisionManifold->AddPoint( ContactPointID::CPID_NULL, vClosestPointSphere, vClosestPointBox, vNormal, fDistance );
    }

    return ( fDistance < 0.0f );
}

