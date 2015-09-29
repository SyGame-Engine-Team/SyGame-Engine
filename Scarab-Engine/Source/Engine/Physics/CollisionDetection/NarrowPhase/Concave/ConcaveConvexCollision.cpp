/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/NarrowPhase/Concave/ConcaveConvexCollision.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Narrow-Phase : Concave, Concave-Convex
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
#include "ConcaveConvexCollision.h"

/////////////////////////////////////////////////////////////////////////////////
// ConcaveConvexCollision implementation
ConcaveConvexCollision::ConcaveConvexCollision( TransformedShapeConcave * pShapeA, TransformedShapeConvex * pShapeB ):
    NarrowPhaseConcave( pShapeA, pShapeB ), m_hCollisionQuery()
{
    Assert( pShapeB->GetType() == SHAPE_CONVEX_MESH );

    m_pConcave = pShapeA;
    m_pConvex = pShapeB;

    m_bReverseNormal = false;
}
ConcaveConvexCollision::ConcaveConvexCollision( TransformedShapeConvex * pShapeA, TransformedShapeConcave * pShapeB ):
    NarrowPhaseConcave( pShapeA, pShapeB ), m_hCollisionQuery()
{
    Assert( pShapeA->GetType() == SHAPE_CONVEX_MESH );

    m_pConcave = pShapeB;
    m_pConvex = pShapeA;

    m_bReverseNormal = true;
}
ConcaveConvexCollision::~ConcaveConvexCollision()
{
    // nothing to do
}

Void ConcaveConvexCollision::Initialize()
{
    // nothing to do
}
Bool ConcaveConvexCollision::DetectCollision( Bool bGenerateContactPoints )
{
    // Setup query data
    m_hCollisionQuery.pThis = this;
    m_hCollisionQuery.bCollisionFound = false;
    m_hCollisionQuery.bGenerateContactPoints = bGenerateContactPoints;
    if ( bGenerateContactPoints )
        m_pCollisionManifold->Clear();

    // Query concave shape's triangles
    m_pConcave->QueryTriangles( m_pConvex->GetAAB(), &m_hCollisionQuery );

    // Done
    return m_hCollisionQuery.bCollisionFound;
}

/////////////////////////////////////////////////////////////////////////////////

ConcaveConvexCollision::CollisionQuery::CollisionQuery():
    TransformedShapeConcave::HTransformedTriangleQuery()
{
    pThis = NULL;
    bCollisionFound = false;
    bGenerateContactPoints = false;
}
ConcaveConvexCollision::CollisionQuery::~CollisionQuery()
{
    // nothing to do
}

Void ConcaveConvexCollision::CollisionQuery::ProcessTransformed( const ShapeConcave::TriangleLeaf & triLeaf )
{
    // Build triangle shape
    ShapeTriangle triangleShape( triLeaf.vTriangle );
    TransformedShapeConvex triangleTrShape( &triangleShape, Transform3::Identity );

    // Set transformed shapes
    TransformedShapeConvex *pTrShapeA, *pTrShapeB;
    if ( pThis->m_bReverseNormal ) {
        pTrShapeA = pThis->m_pConvex;
        pTrShapeB = &triangleTrShape;
    } else {
        pTrShapeA = &triangleTrShape;
        pTrShapeB = pThis->m_pConvex;
    }

    // Compute collision
    ConvexTriangleCollision hCollision( pTrShapeA, pTrShapeB );
    hCollision.Initialize();
    if ( hCollision.DetectCollision( bGenerateContactPoints ) )
        bCollisionFound = true;

    // Build contact manifold
    if ( bGenerateContactPoints ) {
        ContactPointID curCPID;
        curCPID.dwSpecifierA = triLeaf.dwTriangleID;
        curCPID.dwSpecifierB = 0;

        UInt iContactPointCount = hCollision.GetManifold()->GetContactPointCount();
        const ContactPoint * pCP;

        for( UInt i = 0; i < iContactPointCount; ++i ) {
            pCP = hCollision.GetManifold()->GetContactPoint(i);

            curCPID.dwFeatureA = pCP->idCPID.dwFeatureA;
            curCPID.dwFeatureB = pCP->idCPID.dwFeatureB;
            if ( pThis->m_bReverseNormal )
                curCPID.ReverseObjects();

            pThis->m_pCollisionManifold->AddPoint( curCPID, pCP->vWorldPointA, pCP->vWorldPointB, pCP->vWorldNormalBA, pCP->fDistance );
        }
    }
}

