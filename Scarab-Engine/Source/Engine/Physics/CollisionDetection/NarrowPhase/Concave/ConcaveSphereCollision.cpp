/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/NarrowPhase/Concave/ConcaveSphereCollision.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Narrow-Phase : Concave, Concave-Sphere
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
#include "ConcaveSphereCollision.h"

/////////////////////////////////////////////////////////////////////////////////
// ConcaveSphereCollision implementation
ConcaveSphereCollision::ConcaveSphereCollision( TransformedShapeConcave * pShapeA, TransformedShapeConvex * pShapeB ):
    NarrowPhaseConcave( pShapeA, pShapeB ), m_hCollisionQuery()
{
    Assert( pShapeB->GetType() == SHAPE_PRIMITIVE_SPHERE );

    m_pConcave = pShapeA;
    m_pSphere = pShapeB;

    m_bReverseNormal = false;
}
ConcaveSphereCollision::ConcaveSphereCollision( TransformedShapeConvex * pShapeA, TransformedShapeConcave * pShapeB ):
    NarrowPhaseConcave( pShapeA, pShapeB ), m_hCollisionQuery()
{
    Assert( pShapeA->GetType() == SHAPE_PRIMITIVE_SPHERE );

    m_pConcave = pShapeB;
    m_pSphere = pShapeA;

    m_bReverseNormal = true;
}
ConcaveSphereCollision::~ConcaveSphereCollision()
{
    // nothing to do
}

Void ConcaveSphereCollision::Initialize()
{
    // nothing to do
}
Bool ConcaveSphereCollision::DetectCollision( Bool bGenerateContactPoints )
{
    // Setup query data
    m_hCollisionQuery.pThis = this;
    m_hCollisionQuery.bCollisionFound = false;
    m_hCollisionQuery.bGenerateContactPoints = bGenerateContactPoints;
    if ( bGenerateContactPoints )
        m_pCollisionManifold->Clear();

    // Query concave shape's triangles
    m_pConcave->QueryTriangles( m_pSphere->GetAAB(), &m_hCollisionQuery );

    // Done
    return m_hCollisionQuery.bCollisionFound;
}

/////////////////////////////////////////////////////////////////////////////////

ConcaveSphereCollision::CollisionQuery::CollisionQuery():
    TransformedShapeConcave::HTransformedTriangleQuery()
{
    pThis = NULL;
    bCollisionFound = false;
    bGenerateContactPoints = false;
}
ConcaveSphereCollision::CollisionQuery::~CollisionQuery()
{
    // nothing to do
}

Void ConcaveSphereCollision::CollisionQuery::ProcessTransformed( const ShapeConcave::TriangleLeaf & triLeaf )
{
    // Build triangle shape
    ShapeTriangle triangleShape( triLeaf.vTriangle );
    TransformedShapeConvex triangleTrShape( &triangleShape, Transform3::Identity );

    // Set transformed shapes
    TransformedShapeConvex *pTrShapeA, *pTrShapeB;
    if ( pThis->m_bReverseNormal ) {
        pTrShapeA = pThis->m_pSphere;
        pTrShapeB = &triangleTrShape;
    } else {
        pTrShapeA = &triangleTrShape;
        pTrShapeB = pThis->m_pSphere;
    }

    // Compute collision
    TriangleSphereCollision hCollision( pTrShapeA, pTrShapeB );
    hCollision.Initialize();
    if ( hCollision.DetectCollision( bGenerateContactPoints ) )
        bCollisionFound = true;

    // Build contact manifold
    if ( bGenerateContactPoints ) {
        ContactPointID curCPID;
        curCPID.dwSpecifierA = triLeaf.dwTriangleID;
        curCPID.dwSpecifierB = 0;
        curCPID.dwFeatureA = 0;
        curCPID.dwFeatureB = 0;
        if ( pThis->m_bReverseNormal )
            curCPID.ReverseObjects();

        const ContactPoint * pCP = hCollision.GetManifold()->GetContactPoint(0);

        pThis->m_pCollisionManifold->AddPoint( curCPID, pCP->vWorldPointA, pCP->vWorldPointB, pCP->vWorldNormalBA, pCP->fDistance );
    }
}

