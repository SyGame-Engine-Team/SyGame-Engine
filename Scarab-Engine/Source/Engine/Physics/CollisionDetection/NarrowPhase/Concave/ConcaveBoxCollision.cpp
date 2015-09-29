/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/NarrowPhase/Concave/ConcaveBoxCollision.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Narrow-Phase : Concave, Concave-Box
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
#include "ConcaveBoxCollision.h"

/////////////////////////////////////////////////////////////////////////////////
// ConcaveBoxCollision implementation
ConcaveBoxCollision::ConcaveBoxCollision( TransformedShapeConcave * pShapeA, TransformedShapeConvex * pShapeB ):
    NarrowPhaseConcave( pShapeA, pShapeB ), m_hCollisionQuery()
{
    Assert( pShapeB->GetType() == SHAPE_PRIMITIVE_BOX );

    m_pConcave = pShapeA;
    m_pBox = pShapeB;

    m_bReverseNormal = false;
}
ConcaveBoxCollision::ConcaveBoxCollision( TransformedShapeConvex * pShapeA, TransformedShapeConcave * pShapeB ):
    NarrowPhaseConcave( pShapeA, pShapeB ), m_hCollisionQuery()
{
    Assert( pShapeA->GetType() == SHAPE_PRIMITIVE_BOX );

    m_pConcave = pShapeB;
    m_pBox = pShapeA;

    m_bReverseNormal = true;
}
ConcaveBoxCollision::~ConcaveBoxCollision()
{
    // nothing to do
}

Void ConcaveBoxCollision::Initialize()
{
    // nothing to do
}
Bool ConcaveBoxCollision::DetectCollision( Bool bGenerateContactPoints )
{
    // Setup query data
    m_hCollisionQuery.pThis = this;
    m_hCollisionQuery.bCollisionFound = false;
    m_hCollisionQuery.bGenerateContactPoints = bGenerateContactPoints;
    if ( bGenerateContactPoints )
        m_pCollisionManifold->Clear();

    // Query concave shape's triangles
    m_pConcave->QueryTriangles( m_pBox->GetAAB(), &m_hCollisionQuery );

    // Done
    return m_hCollisionQuery.bCollisionFound;
}

/////////////////////////////////////////////////////////////////////////////////

ConcaveBoxCollision::CollisionQuery::CollisionQuery():
    TransformedShapeConcave::HTransformedTriangleQuery()
{
    pThis = NULL;
    bCollisionFound = false;
    bGenerateContactPoints = false;
}
ConcaveBoxCollision::CollisionQuery::~CollisionQuery()
{
    // nothing to do
}

Void ConcaveBoxCollision::CollisionQuery::ProcessTransformed( const ShapeConcave::TriangleLeaf & triLeaf )
{
    // Build triangle shape
    ShapeTriangle triangleShape( triLeaf.vTriangle );
    TransformedShapeConvex triangleTrShape( &triangleShape, Transform3::Identity );

    // Set transformed shapes
    TransformedShapeConvex *pTrShapeA, *pTrShapeB;
    if ( pThis->m_bReverseNormal ) {
        pTrShapeA = pThis->m_pBox;
        pTrShapeB = &triangleTrShape;
    } else {
        pTrShapeA = &triangleTrShape;
        pTrShapeB = pThis->m_pBox;
    }

    // Compute collision
    TriangleBoxCollision hCollision( pTrShapeA, pTrShapeB );
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

