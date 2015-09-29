/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/NarrowPhase/VCLip/SingleSideVClip.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Narrow-Phase : Convex, V-Clip, Single-sided
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
#include "SingleSideVClip.h"

/////////////////////////////////////////////////////////////////////////////////
// SingleSideVClip implementation
SingleSideVClip::SingleSideVClip( TransformedShapeConvex * pConvexA, TransformedShapeConvex * pConvexB ):
    NarrowPhaseConvex( pConvexA, pConvexB )
{
    if ( pConvexB->GetType() == SHAPE_PRIMITIVE_SPHERE ) {
        m_pConvex = pConvexA;
        m_pSphere = pConvexB;
        m_bReverseNormal = false;
    } else {
        Assert( pConvexA->GetType() == SHAPE_PRIMITIVE_SPHERE );
        m_pConvex = pConvexB;
        m_pSphere = pConvexA;
        m_bReverseNormal = true;
    }

    // Null state
    m_vcState = SSVCLIP_STATE_SEPARATED;
    m_pClosestFeature = NULL;
}
SingleSideVClip::~SingleSideVClip()
{
    // nothing to do
}

Void SingleSideVClip::Initialize()
{
    // Compute an approximate vector from Polyhedron to Sphere
    Vector3 vAB = ( m_pSphere->GetAAB().GetCenter() - m_pConvex->GetAAB().GetCenter() );

    // Query extremal vertex along AB
    UInt iVertex = INVALID_OFFSET;

    m_pConvex->QueryExtremalVertices( vAB, &iVertex, NULL );
    Assert( iVertex != INVALID_OFFSET );

    // Initial state
    m_vcState = SSVCLIP_STATE_V;
    m_pClosestFeature = m_pConvex->GetGraph()->GetVertex( MeshVertexKey(iVertex) );
}
Bool SingleSideVClip::DetectCollision( Bool bGenerateContactPoints )
{
    const ShapeSphere * pSphere = (const ShapeSphere *)( m_pSphere->GetShape() );

    const Vertex3 & vSpherePosition = m_pSphere->GetTransform() * pSphere->GetSphere().Center;
    Scalar fSphereRadius = pSphere->GetSphere().Radius;

    // Voronoi-Clip state machine
    SingleSideVClipState iPrevStepState;

    UInt iIterations = 0;
    while( true ) {
        // Save previous step state
        if ( m_vcState >= SSVCLIP_STATE_V )
            iPrevStepState = m_vcState;

        // Check for infinite loops
        if ( iIterations > SSVCLIP_MAX_ITERATIONS ) {
// Debug
            DebugPrint( TEXT("SSV-Clip infinite loop avoided ...\n") );
// Debug
            m_vcState = SSVCLIP_STATE_PENETRATION;
        }

        // Process current state
        switch( m_vcState ) {
            case SSVCLIP_STATE_PENETRATION:
                // Unless huge penetration or epsilon-small radius,
                // this should never happen as we're using sphere's center
            case SSVCLIP_STATE_SEPARATED:
                // Done
                m_vcState = iPrevStepState;
                return _GenerateContactPoints( vSpherePosition, fSphereRadius, bGenerateContactPoints );
            case SSVCLIP_STATE_V:
                Assert( m_pClosestFeature->GetType() == MESH_FEATURE_VERTEX );
                m_vcState = _ProcessState_V( (const Vertex *)m_pClosestFeature, vSpherePosition );
                break; // Continue
            case SSVCLIP_STATE_E:
                Assert( m_pClosestFeature->GetType() == MESH_FEATURE_EDGE );
                m_vcState = _ProcessState_E( (const Edge *)m_pClosestFeature, vSpherePosition );
                break; // Continue
            case SSVCLIP_STATE_F:
                Assert( m_pClosestFeature->GetType() == MESH_FEATURE_FACET );
                m_vcState = _ProcessState_F( (const MeshFacet *)m_pClosestFeature, vSpherePosition );
                break; // Continue
            default: Assert(false); break;
        }

        ++iIterations; 
    }
}

/////////////////////////////////////////////////////////////////////////////////

const MeshFeature * SingleSideVClip::_ClipVertex_V( const Vertex * pVertex, const Vertex3 & vVertexV ) const
{
    // Run through Voronoi planes
    UInt iPlaneCount = m_pConvex->GetVoronoiPlaneCount( pVertex );
    Plane vPlane;

    Scalar fDistance;
    
    for( UInt i = 0; i < iPlaneCount; ++i ) {
        m_pConvex->GetVoronoiPlane( &vPlane, pVertex, i );
        fDistance = vPlane.DistanceNP( vVertexV );

        // Exclusion case
        if ( fDistance > 0.0f )
            return pVertex->Edges[i];
    }

    // Inclusion case
    return NULL;
}
const MeshFeature * SingleSideVClip::_ClipVertex_E( const Edge * pEdge, const Vertex3 & vVertexV ) const
{
    // Run through Voronoi planes (EV planes first)
    UInt iPlaneCount = m_pConvex->GetVoronoiPlaneCount( pEdge );
    Plane vPlane;

    Scalar fDistance;

    for( UInt i = 0; i < iPlaneCount; ++i ) {
        m_pConvex->GetVoronoiPlane( &vPlane, pEdge, i );
        fDistance = vPlane.DistanceNP( vVertexV );

        // Exclusion case
        if ( fDistance > 0.0f ) {
            if ( i < 2 )
                return pEdge->Vertices[i];
            else
                return pEdge->Facets[i - 2];
        }
    }

    // Inclusion case
    return NULL;
}
const MeshFeature * SingleSideVClip::_ClipVertex_F( const MeshFacet * pFacet, const Vertex3 & vVertexV ) const
{
    // Run through Voronoi planes
    UInt iPlaneCount = m_pConvex->GetVoronoiPlaneCount( pFacet );
    Plane vPlane;

    Scalar fDistance;
    Scalar fMaxDistance = 0.0f;
    UInt iMaxDistancePlane = INVALID_OFFSET;

    for( UInt i = 0; i < iPlaneCount; ++i ) {
        m_pConvex->GetVoronoiPlane( &vPlane, pFacet, i );
        fDistance = vPlane.DistanceNP( vVertexV );

        // Compute max distance plane
        if ( fDistance > fMaxDistance ) {
            fMaxDistance = fDistance;
            iMaxDistancePlane = i;
        }
    }

    // Exclusion case
    if ( iMaxDistancePlane != INVALID_OFFSET )
        return pFacet->GetEdge(iMaxDistancePlane);

    // Inclusion case
    return NULL;
}

SingleSideVClipState SingleSideVClip::_EscapeLocalMinimum( const MeshFacet * pFacet, const Vertex3 & vPosition )
{
    // Get whole graph
    const MeshVoronoiGraph * pGraph = m_pConvex->GetGraph();

    // Walk through all facets of the polyhedron
    Plane facetPlane;

    Scalar fDistance;
    Scalar fMaxDistance = -SCALAR_INFINITE;
    const MeshFacet * pMaxDistanceFacet = NULL;

    pGraph->EnumFacets();
    const MeshFacet * pTestFacet = pGraph->EnumNextFacet();
    while( pTestFacet != NULL ) {
        if ( pTestFacet == pFacet ) {
            pTestFacet = pGraph->EnumNextFacet();
            continue;
        }

        // Get Facet plane
        m_pConvex->GetFacetPlane( &facetPlane, pTestFacet );

        // Compute max distance facet
        fDistance = facetPlane.DistanceNP( vPosition );
        if ( fDistance > fMaxDistance ) {
            fMaxDistance = fDistance;
            pMaxDistanceFacet = pTestFacet;
        }

        // Next facet
        pTestFacet = pGraph->EnumNextFacet();
    }

    // Move to max distance facet
    Assert( pMaxDistanceFacet != NULL );
    m_pClosestFeature = pMaxDistanceFacet;

    // Found a penetration witness pair
    if ( fMaxDistance < ZeroTolerance )
        return SSVCLIP_STATE_PENETRATION;

    // Escape local minimum
    return SSVCLIP_STATE_F;
}

/////////////////////////////////////////////////////////////////////////////////

SingleSideVClipState SingleSideVClip::_ProcessState_V( const Vertex * pVertex, const Vertex3 & vSpherePosition )
{
    const MeshFeature * pUpdated = NULL;

    // Clip V1 against VR(V0)
    pUpdated = _ClipVertex_V( pVertex, vSpherePosition );
    if ( pUpdated != NULL ) {
        m_pClosestFeature = pUpdated;

        Assert( pUpdated->GetType() == MESH_FEATURE_EDGE );
        return SSVCLIP_STATE_E;
    }

    // Found a closest feature pair
    Vertex3 vPosition;
    m_pConvex->GetVertexPosition( &vPosition, pVertex );
    Scalar fSqrDistance = (vSpherePosition - vPosition).NormSqr();
    if ( fSqrDistance < (ZeroTolerance * ZeroTolerance) )
        return SSVCLIP_STATE_PENETRATION;
    return SSVCLIP_STATE_SEPARATED;
}
SingleSideVClipState SingleSideVClip::_ProcessState_E( const Edge * pEdge, const Vertex3 & vSpherePosition )
{
    const MeshFeature * pUpdated = NULL;

    // Clip V1 against VR(E0)
    pUpdated = _ClipVertex_E( pEdge, vSpherePosition );
    if ( pUpdated != NULL ) {
        m_pClosestFeature = pUpdated;

        if ( pUpdated->GetType() == MESH_FEATURE_VERTEX )
            return SSVCLIP_STATE_V;
        Assert( pUpdated->GetType() == MESH_FEATURE_FACET );
        return SSVCLIP_STATE_F;
    }

    // Found a closest feature pair
    return SSVCLIP_STATE_SEPARATED;
}
SingleSideVClipState SingleSideVClip::_ProcessState_F( const MeshFacet * pFacet, const Vertex3 & vSpherePosition )
{
    const MeshFeature * pUpdated = NULL;

    // Clip V1 against VR(F0)
    pUpdated = _ClipVertex_F( pFacet, vSpherePosition );
    if ( pUpdated != NULL ) {
        m_pClosestFeature = pUpdated;

        Assert( pUpdated->GetType() == MESH_FEATURE_EDGE );
        return SSVCLIP_STATE_E;
    }

    // Get facet plane & vertex distance
    Plane facetPlane;
    m_pConvex->GetFacetPlane( &facetPlane, pFacet );
    Scalar fDistanceV = facetPlane.DistanceNP( vSpherePosition );

    // Check penetration
    if ( MathFn->EqualsZero(fDistanceV, ZeroTolerance) )
        return SSVCLIP_STATE_PENETRATION;

    // Stuck in a local minimum !
    if ( fDistanceV < 0.0f )
        return _EscapeLocalMinimum( pFacet, vSpherePosition );

    // Found a closest feature pair
    return SSVCLIP_STATE_SEPARATED;
}

/////////////////////////////////////////////////////////////////////////////////

Bool SingleSideVClip::_GenerateContactPoints( const Vertex3 & vSpherePosition, Scalar fSphereRadius, Bool bGenerateContactPoints )
{
    // Closest point to sphere center on polyhedron
    Vertex3 vClosestPointConvex;
    switch( m_pClosestFeature->GetType() ) {
        case MESH_FEATURE_VERTEX:
            m_pConvex->GetVertexPosition( &vClosestPointConvex, (const Vertex *)m_pClosestFeature );
            break;
        case MESH_FEATURE_EDGE: {
                // Compute closest point on edge
                Vertex3 vA, vB;
                m_pConvex->GetVertexPosition( &vA, ((const Edge *)m_pClosestFeature)->Vertices[0] );
                m_pConvex->GetVertexPosition( &vB, ((const Edge *)m_pClosestFeature)->Vertices[1] );
                Segment3 vEgde( vA, vB );
                vClosestPointConvex = vEgde.ClosestPoint( vSpherePosition );
            } break;
        case MESH_FEATURE_FACET: {
                Plane facetPlane;
                m_pConvex->GetFacetPlane( &facetPlane, (const MeshFacet *)m_pClosestFeature );
                vClosestPointConvex = facetPlane.ClosestPointNP( vSpherePosition );
            } break;
        default: Assert(false); break;
    }

    // Contact normal & distance
    Vector3 vNormal = ( vSpherePosition - vClosestPointConvex );
    Scalar fDistance = ( vNormal.Normalize() - fSphereRadius );

    // Closest point on sphere
    Vertex3 vClosestPointSphere = ( vSpherePosition - (vNormal * fSphereRadius) );

    // Build contact manifold
    m_pCollisionManifold->Clear();

    if ( bGenerateContactPoints ) {
        if ( m_bReverseNormal )
            m_pCollisionManifold->AddPoint( ContactPointID::CPID_NULL, vClosestPointSphere, vClosestPointConvex, -vNormal, fDistance );
        else
            m_pCollisionManifold->AddPoint( ContactPointID::CPID_NULL, vClosestPointConvex, vClosestPointSphere, vNormal, fDistance );
    }

    return ( fDistance <= ZeroTolerance );
}

