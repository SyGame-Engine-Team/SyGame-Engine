/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/NarrowPhase/SAT/SAT.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Narrow-Phase : Convex, Separating Axis Theorem
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : none
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "SAT.h"

/////////////////////////////////////////////////////////////////////////////////
// SAT implementation
SAT::SAT( TransformedShapeConvex * pConvexA, TransformedShapeConvex * pConvexB ):
    NarrowPhaseConvex( pConvexA, pConvexB )
{
    // nothing to do
}
SAT::~SAT()
{
    // nothing to do
}

Void SAT::Initialize()
{
    // nothing to do ?
}
Bool SAT::DetectCollision( Bool bGenerateContactPoints )
{
    // Find separating axis between the 2 shapes, if it exists
    Vector3 vSeparatingAxis = Vector3::Null;
    Scalar fDistance = 0.0f;
    Bool bCollision = _FindSeparatingAxis( &vSeparatingAxis, &fDistance, !bGenerateContactPoints );

    // Generate contact points
    if ( bGenerateContactPoints )
        _GenerateContactPoints( vSeparatingAxis );

    // Done
    return bCollision;
}

/////////////////////////////////////////////////////////////////////////////////

Void SAT::_ProjectAxis( Scalar * outMin, Scalar * outMax, const TransformedShapeConvex * pShape, const Vector3 & vAxis )
{
    Vertex3 vMin, vMax;
    pShape->QueryExtremalVertices( vAxis, &vMax, &vMin );

    *outMin = ( vMin.ToVector() * vAxis );
    *outMax = ( vMax.ToVector() * vAxis );
    if ( *outMin > *outMax )
        Swap<Scalar>( outMin, outMax );

    Scalar fOffset = ( pShape->GetTransform().GetTranslate() * vAxis );
    *outMin += fOffset;
    *outMax += fOffset;
}
Bool SAT::_TestSeparatingAxis( Scalar * outDistance, const Vector3 & vCandidate )
{
    // Project A on axis
    Scalar fMinA, fMaxA;
    _ProjectAxis( &fMinA, &fMaxA, m_pShapeA, vCandidate );

    // Project B on axis
    Scalar fMinB, fMaxB;
    _ProjectAxis( &fMinB, &fMaxB, m_pShapeB, vCandidate );

    // Disjoint cases
    if ( fMaxA < fMinB ) {
        *outDistance = ( fMinB - fMaxA );
        return false;
    }
    if ( fMaxB < fMinA ) {
        *outDistance = ( fMinA - fMaxB );
        return false;
    }

    // Overlapping cases
    Scalar fDeltaA = ( fMaxA - fMinB );
    Assert( fDeltaA >= 0.0f );

    Scalar fDeltaB = ( fMaxB - fMinA );
    Assert( fDeltaB >= 0.0f );

    *outDistance = -( Min<Scalar>(fDeltaA, fDeltaB) );
    return true;

}
Bool SAT::_FindSeparatingAxis( Vector3 * outSeparatingAxis, Scalar * outDistance, Bool bTestOnly )
{
    *outDistance = -SCALAR_INFINITE;

    // Orientation consistency : B => A
    Vertex3 vPosA = m_pShapeA->GetTransform().GetTranslate();
    Vertex3 vPosB = m_pShapeB->GetTransform().GetTranslate();
    Vector3 vDelta = ( vPosA - vPosB );

    // Test normals from A
    m_pShapeA->GetGraph()->EnumFacets();
    MeshFacet * pFacet = m_pShapeA->GetGraph()->EnumNextFacet();
    while( pFacet != NULL ) {
        // Get facet normal
        Vector3 vFacetNormal;
        m_pShapeA->GetFacetNormal( &vFacetNormal, pFacet );

        // Make sure orientation is B => A
        if ( (vDelta * vFacetNormal) < 0.0f )
            vFacetNormal = -vFacetNormal;

        // Test that normal
        Scalar fDistance = 0.0f;
        Bool bOverlapped = _TestSeparatingAxis( &fDistance, vFacetNormal );
        if ( bTestOnly && !bOverlapped )
            return false;

        // Penetration distances will be negative so we seek the smallest penetration in absolute value
        // Separating distances will be positive, overriding penetration cases, so we seek the biggest
        // separation in absolute value.
        if ( fDistance > *outDistance ) {
            *outDistance = fDistance;
            *outSeparatingAxis = vFacetNormal;
        }

        // Next facet
        pFacet = m_pShapeA->GetGraph()->EnumNextFacet();
    }

    // Test normals from B
    m_pShapeB->GetGraph()->EnumFacets();
    pFacet = m_pShapeB->GetGraph()->EnumNextFacet();
    while( pFacet != NULL ) {
        // Get facet normal
        Vector3 vFacetNormal;
        m_pShapeB->GetFacetNormal( &vFacetNormal, pFacet );

        // Make sure orientation is B => A
        if ( (vDelta * vFacetNormal) < 0.0f )
            vFacetNormal = -vFacetNormal;

        // Test that normal
        Scalar fDistance = 0.0f;
        Bool bOverlapped = _TestSeparatingAxis( &fDistance, vFacetNormal );
        if ( bTestOnly && !bOverlapped )
            return false;

        // Penetration distances will be negative so we seek the smallest penetration in absolute value
        // Separating distances will be positive, overriding penetration cases, so we seek the biggest
        // separation in absolute value.
        if ( fDistance > *outDistance ) {
            *outDistance = fDistance;
            *outSeparatingAxis = vFacetNormal;
        }

        // Next facet
        pFacet = m_pShapeB->GetGraph()->EnumNextFacet();
    }

    // Test edges from AxB
    m_pShapeA->GetGraph()->EnumEdges();
    ShapeConvex::Edge * pEdgeA = (ShapeConvex::Edge*)( m_pShapeA->GetGraph()->EnumNextEdge() );
    while( pEdgeA != NULL ) {
        Vertex3 vA0, vA1;
        m_pShapeA->GetVertexPosition( &vA0, pEdgeA->Vertices[0] );
        m_pShapeA->GetVertexPosition( &vA1, pEdgeA->Vertices[1] );
        Vector3 vEdgeA = ( vA1 - vA0 );

        m_pShapeB->GetGraph()->EnumEdges();
        ShapeConvex::Edge * pEdgeB = (ShapeConvex::Edge*)( m_pShapeB->GetGraph()->EnumNextEdge() );
        while( pEdgeB != NULL ) {
            Vertex3 vB0, vB1;
            m_pShapeB->GetVertexPosition( &vB0, pEdgeB->Vertices[0] );
            m_pShapeB->GetVertexPosition( &vB1, pEdgeB->Vertices[1] );
            Vector3 vEdgeB = ( vB1 - vB0 );

            // Cross edges for a candidate axis
            Vector3 vEdgeCross = ( vEdgeA ^ vEdgeB );

            // Check for degenerate cases
            if ( vEdgeCross.NormSqr() > (ZeroTolerance * ZeroTolerance) ) {
                vEdgeCross.Normalize();

                // Make sure orientation is B => A
                if ( (vDelta * vEdgeCross) < 0.0f )
                    vEdgeCross = -vEdgeCross;

                // Test that edge normal
                Scalar fDistance = 0.0f;
                Bool bOverlapped = _TestSeparatingAxis( &fDistance, vEdgeCross );
                if ( bTestOnly && !bOverlapped )
                    return false;

                // Penetration distances will be negative so we seek the smallest penetration in absolute value
                // Separating distances will be positive, overriding penetration cases, so we seek the biggest
                // separation in absolute value.
                if ( fDistance > *outDistance ) {
                    *outDistance = fDistance;
                    *outSeparatingAxis = vEdgeCross;
                }
            }

            // Next edge
            pEdgeB = (ShapeConvex::Edge*)( m_pShapeB->GetGraph()->EnumNextEdge() );
        }

        // Next edge
        pEdgeA = (ShapeConvex::Edge*)( m_pShapeA->GetGraph()->EnumNextEdge() );
    }

    // Done
    return ( *outDistance < SCALAR_EPSILON );
}



