/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/NarrowPhase/Primitive/TriangleSphereCollision.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Narrow-Phase : Primitive, Triangle-Sphere
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
#include "TriangleSphereCollision.h"

/////////////////////////////////////////////////////////////////////////////////
// TriangleSphereCollision implementation
TriangleSphereCollision::TriangleSphereCollision( TransformedShapeConvex * pShapeA, TransformedShapeConvex * pShapeB ):
    NarrowPhaseConvex( pShapeA, pShapeB )
{
    if ( pShapeA->GetType() == SHAPE_PRIMITIVE_TRIANGLE ) {
        Assert( pShapeB->GetType() == SHAPE_PRIMITIVE_SPHERE );
        m_pTriangle = pShapeA;
        m_pSphere = pShapeB;
        m_bReverseNormal = false;
    } else {
        Assert( pShapeA->GetType() == SHAPE_PRIMITIVE_SPHERE );
        Assert( pShapeB->GetType() == SHAPE_PRIMITIVE_TRIANGLE );
        m_pTriangle = pShapeB;
        m_pSphere = pShapeA;
        m_bReverseNormal = true;
    }
}
TriangleSphereCollision::~TriangleSphereCollision()
{
    // nothing to do
}

Void TriangleSphereCollision::Initialize()
{
    // nothing to do
}
Bool TriangleSphereCollision::DetectCollision( Bool bGenerateContactPoints )
{
    const ShapeTriangle * pTriangle = (const ShapeTriangle *)( m_pTriangle->GetShape() );
    const ShapeSphere * pSphere = (const ShapeSphere *)( m_pSphere->GetShape() );

    const Triangle3 & vTriangle = pTriangle->GetTriangle();
    const Sphere & vSphere = pSphere->GetSphere();

    Vertex3 vA = ( m_pTriangle->GetTransform() * vTriangle.A );
    Vertex3 vB = ( m_pTriangle->GetTransform() * vTriangle.B );
    Vertex3 vC = ( m_pTriangle->GetTransform() * vTriangle.C );

    Plane vTrianglePlane = Triangle3(vA, vB, vC).GetPlane();

    Vertex3 vCenter = ( m_pSphere->GetTransform() * vSphere.Center );
    Scalar fRadius = vSphere.Radius;

    Vertex3 vClosestPointTriangle, vClosestPointSphere;
    Vector3 vNormal;
    Scalar fDistance;

    // Compute sphere center's plane distance
    Scalar fPlaneDistance = vTrianglePlane.DistanceNP( vCenter );
    if ( fPlaneDistance < 0.0f ) {
        vTrianglePlane.Normal = -(vTrianglePlane.Normal);
        fPlaneDistance = -fPlaneDistance;
    }

    // Triangle contains projected sphere center case
    Bool bInsideTriangle = vTriangle.Contains( vCenter, vTrianglePlane.Normal );
    if ( bInsideTriangle ) {
        // Project
        fDistance = ( fPlaneDistance - fRadius );
        vNormal = vTrianglePlane.Normal;
        vClosestPointTriangle = vCenter - ( vNormal * fPlaneDistance );
        vClosestPointSphere = vCenter - ( vNormal * fRadius );
    } else {
        // Check edge capsules
        Segment3 vEdge;
        Vertex3 vClosestPointEdge;
        Vector3 vEdgeNormal;
        Scalar fDistanceSqr;

        Vertex3 vMinDistanceClosestPointEdge;
        Vector3 vMinDistanceNormal;
        Scalar fMinDistanceSqr = SCALAR_INFINITE;

        const Vertex3 arrV[3] = { vA, vB, vC };

        for( UInt i = 0; i < 3; ++i ) {
            vEdge = Segment3( arrV[i], arrV[(i+1)%3] );
            vClosestPointEdge = vEdge.ClosestPoint( vCenter );
            vEdgeNormal = ( vCenter - vClosestPointEdge );
            fDistanceSqr = ( vEdgeNormal * vEdgeNormal );
            if ( fDistanceSqr < fMinDistanceSqr ) {
                vMinDistanceClosestPointEdge = vClosestPointEdge;
                vMinDistanceNormal = vEdgeNormal;
                fMinDistanceSqr = fDistanceSqr;
            }
        }

        fDistance = ( vMinDistanceNormal.Normalize() - fRadius );
        vNormal = vMinDistanceNormal;
        vClosestPointTriangle = vMinDistanceClosestPointEdge;
        vClosestPointSphere = vCenter - ( vNormal * fRadius );
    }

    // Generate contact points
    m_pCollisionManifold->Clear();

    if ( bGenerateContactPoints ) {
        if ( m_bReverseNormal )
            m_pCollisionManifold->AddPoint( ContactPointID::CPID_NULL, vClosestPointSphere, vClosestPointTriangle, vNormal, fDistance );
        else
            m_pCollisionManifold->AddPoint( ContactPointID::CPID_NULL, vClosestPointTriangle, vClosestPointSphere, -vNormal, fDistance );
    }

    return ( fDistance < 0.0f );
}

