/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/NarrowPhase/Concave/ConcaveConcaveCollision.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Narrow-Phase : Concave, Concave-Concave
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
#include "ConcaveConcaveCollision.h"

/////////////////////////////////////////////////////////////////////////////////
// ConcaveConcaveCollision implementation
ConcaveConcaveCollision::ConcaveConcaveCollision( TransformedShapeConcave * pShapeA, TransformedShapeConcave * pShapeB ):
    NarrowPhaseConcave( pShapeA, pShapeB ), m_hCollisionQuery()
{
    m_pConcaveA = pShapeA;
    m_pConcaveB = pShapeB;
}
ConcaveConcaveCollision::~ConcaveConcaveCollision()
{
    // nothing to do
}

Void ConcaveConcaveCollision::Initialize()
{
    // nothing to do
}
Bool ConcaveConcaveCollision::DetectCollision( Bool bGenerateContactPoints )
{
    // Setup query data
    m_hCollisionQuery.pThis = this;
    m_hCollisionQuery.bCollisionFound = false;
    m_hCollisionQuery.bGenerateContactPoints = bGenerateContactPoints;
    if ( bGenerateContactPoints )
        m_pCollisionManifold->Clear();

    m_hCollisionQuery.bQueryB = false;
    m_hCollisionQuery.pTriLeafA = NULL;

    // Query concave shape A's triangles
    m_pConcaveA->QueryTriangles( m_pConcaveB->GetAAB(), &m_hCollisionQuery );

    // Done
    return m_hCollisionQuery.bCollisionFound;
}

/////////////////////////////////////////////////////////////////////////////////

ConcaveConcaveCollision::CollisionQuery::CollisionQuery():
    TransformedShapeConcave::HTransformedTriangleQuery()
{
    pThis = NULL;
    bCollisionFound = false;
    bGenerateContactPoints = false;

    bQueryB = false;
    pTriLeafA = NULL;
}
ConcaveConcaveCollision::CollisionQuery::~CollisionQuery()
{
    // nothing to do
}

Void ConcaveConcaveCollision::CollisionQuery::ProcessTransformed( const ShapeConcave::TriangleLeaf & triLeaf )
{
    // Query level 1 : A //////////////////////

    if ( !bQueryB ) {
        pTriLeafA = &triLeaf;

        // AABox of triangle A
        BVAABox bvAABox;
        bvAABox.Reset( pTriLeafA->vTriangle.A );
        bvAABox.Grow( pTriLeafA->vTriangle.B );
        bvAABox.Grow( pTriLeafA->vTriangle.C );
        bvAABox.AddMargin( SHAPE_MARGIN );

        // Query concave shape B's triangles
        bQueryB = true;
        pThis->m_pConcaveB->QueryTriangles( bvAABox, this );
        bQueryB = false;

        // Done
        return;
    }

    // Query level 2 : B //////////////////////

    const ShapeConcave::TriangleLeaf * pTriLeafB = &triLeaf;

    // Build triangle shapes
    ShapeTriangle triangleShapeA( pTriLeafA->vTriangle );
    TransformedShapeConvex triangleTrShapeA( &triangleShapeA, Transform3::Identity );

    ShapeTriangle triangleShapeB( pTriLeafB->vTriangle );
    TransformedShapeConvex triangleTrShapeB( &triangleShapeB, Transform3::Identity );

    // Set transformed shapes
    TransformedShapeConvex *pTrShapeA, *pTrShapeB;
    pTrShapeA = &triangleTrShapeA;
    pTrShapeB = &triangleTrShapeB;

    // Compute collision
    TriangleTriangleCollision hCollision( pTrShapeA, pTrShapeB );
    hCollision.Initialize();
    if ( hCollision.DetectCollision( bGenerateContactPoints ) )
        bCollisionFound = true;

    // Build contact manifold
    if ( bGenerateContactPoints ) {
        ContactPointID curCPID;
        curCPID.dwSpecifierA = pTriLeafA->dwTriangleID;
        curCPID.dwSpecifierB = pTriLeafB->dwTriangleID;

        UInt iContactPointCount = hCollision.GetManifold()->GetContactPointCount();
        const ContactPoint * pCP;

        for( UInt i = 0; i < iContactPointCount; ++i ) {
            pCP = hCollision.GetManifold()->GetContactPoint(i);

            curCPID.dwFeatureA = pCP->idCPID.dwFeatureA;
            curCPID.dwFeatureB = pCP->idCPID.dwFeatureB;

            pThis->m_pCollisionManifold->AddPoint( curCPID, pCP->vWorldPointA, pCP->vWorldPointB, pCP->vWorldNormalBA, pCP->fDistance );
        }
    }
}

