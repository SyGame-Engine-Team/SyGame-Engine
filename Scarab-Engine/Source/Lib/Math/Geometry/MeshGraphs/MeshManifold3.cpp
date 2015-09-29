/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/MeshManifold3.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Mesh Graph : 3-Manifold VEF graph.
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
#include "MeshManifold3.h"

/////////////////////////////////////////////////////////////////////////////////
// MeshManifold3 implementation
MeshManifold3::MeshManifold3( VertexBuilder pfVertexBuilder, EdgeBuilder pfEdgeBuilder, FacetBuilder pfFacetBuilder ):
    MeshGraph(),
    m_mapVertices(), m_mapEdges(), m_mapFacets(),
    m_itVertex(), m_itEdge(), m_itFacet()
{
    m_pfVertexBuilder = pfVertexBuilder;
    m_pfEdgeBuilder = pfEdgeBuilder;
    m_pfFacetBuilder = pfFacetBuilder;
}
MeshManifold3::~MeshManifold3()
{
    // nothing to do
    // Call Destroy explicitly
}

Void MeshManifold3::Create()
{
    m_mapVertices.SetComparator( MeshVertexKey::Compare, NULL );
    m_mapVertices.Create();

    m_mapEdges.SetComparator( MeshEdgeKey::Compare, NULL );
    m_mapEdges.Create();

    m_mapFacets.SetComparator( MeshFacetKey::Compare, NULL );
    m_mapFacets.Create();
}
Void MeshManifold3::Destroy()
{
    Clear();

    m_mapVertices.Destroy();
    m_mapEdges.Destroy();
    m_mapFacets.Destroy();
}

Void MeshManifold3::Clear()
{
    _SelectMemory();

    VertexMap::Iterator itVertex;
    for( itVertex = m_mapVertices.Begin(); !(itVertex.IsNull()); ++itVertex )
        Delete( itVertex.GetItem() );

    EdgeMap::Iterator itEdge;
    for( itEdge = m_mapEdges.Begin(); !(itEdge.IsNull()); ++itEdge )
        Delete( itEdge.GetItem() );

    FacetMap::Iterator itFacet;
    for( itFacet = m_mapFacets.Begin(); !(itFacet.IsNull()); ++itFacet )
        Delete( itFacet.GetItem() );

    _UnSelectMemory();

    m_mapVertices.Clear();
    m_mapEdges.Clear();
    m_mapFacets.Clear();
}

MeshFacet * MeshManifold3::CreateFacet( const MeshFacetKey & facetKey )
{
    MeshFacet * pFacet = NULL;
    Bool bInserted;
    _LinkageResult iResult;

    // Already existing
    if ( m_mapFacets.Contains(&facetKey) )
        return NULL;

    // Create Facet
    _SelectMemory();
    pFacet = m_pfFacetBuilder( facetKey );
    _UnSelectMemory();

    bInserted = m_mapFacets.Insert( pFacet->GetKey(), pFacet );
    Assert( bInserted );

    // Solve facet-vertex connections
    VertexMap::Iterator itVertex;
    MeshVertexKey vertexKey;
    MeshVertex * pVertex;
    for( UInt i = 0; i < facetKey.GetFacetSize(); ++i ) {
        vertexKey = facetKey.GetVertexKey(i);
        itVertex = m_mapVertices.Get( &vertexKey );

        if ( itVertex.IsNull() ) {
            // Create vertex
            _SelectMemory();
            pVertex = m_pfVertexBuilder( vertexKey );
            _UnSelectMemory();

            bInserted = m_mapVertices.Insert( &(pVertex->Key), pVertex );
            Assert( bInserted );
        } else {
            // Pick vertex
            pVertex = itVertex.GetItem();
        }

        // Linkage
        iResult = _Connect( pFacet, pVertex );
        Assert( iResult == _LINKAGE_DONE );
    }

    // Solve facet-edge connections
    EdgeMap::Iterator itEdge;
    MeshEdgeKey edgeKey;
    MeshEdge * pEdge;

    for( UInt i = 0; i < facetKey.GetFacetSize(); ++i ) {
        edgeKey = facetKey.GetEdgeKey(i);
        itEdge = m_mapEdges.Get( &edgeKey );

        if ( itEdge.IsNull() ) {
            // Create edge
            _SelectMemory();
            pEdge = m_pfEdgeBuilder( edgeKey );
            _UnSelectMemory();

            bInserted = m_mapEdges.Insert( &(pEdge->Key), pEdge );
            Assert( bInserted );

            // Solve edge-vertex connections
            for( UInt j = 0; j < 2; ++j ) {
                vertexKey = edgeKey.GetVertexKey(j);
                itVertex = m_mapVertices.Get( &vertexKey );
                pVertex = itVertex.GetItem();

                // Linkage
                iResult = _Connect( pEdge, pVertex );
                Assert( iResult == _LINKAGE_DONE );
            }
        } else {
            // Pick edge
            pEdge = itEdge.GetItem();
        }

        // Linkage
        iResult = _Connect( pFacet, pEdge );
        Assert( iResult == _LINKAGE_DONE );
    }

    return pFacet;
}
Bool MeshManifold3::DestroyFacet( const MeshFacetKey & facetKey )
{
    MeshFacet * pFacet = NULL;
    _LinkageResult iResult;
    Bool bRemoved;

    // Not existing
    FacetMap::Iterator itFacet = m_mapFacets.Get( &facetKey );
    if ( itFacet.IsNull() )
        return false;

    pFacet = itFacet.GetItem();

    // Solve facet-vertex connections
    MeshVertex * pVertex;

    for( UInt i = 0; i < pFacet->GetVertexCount(); ++i ) {
        pVertex = pFacet->GetVertex(i);

        // Linkage
        iResult = _Disconnect( pFacet, pVertex );
        Assert( iResult == _LINKAGE_DONE );
    }

    // Solve facet-edge connections
    MeshEdge * pEdge;

    for( UInt i = 0; i < pFacet->GetEdgeCount(); ++i ) {
        pEdge = pFacet->GetEdge(i);

        // Linkage
        iResult = _Disconnect( pFacet, pEdge );
        Assert( iResult == _LINKAGE_DONE );

        // Destroy edge case
        if ( pEdge->GetFacetCount() == 0 ) {

            // Solve edge-vertex connections
            for( UInt j = 0; j < 2; ++j ) {
                pVertex = pEdge->GetVertex(j);

                // Linkage
                iResult = _Disconnect( pEdge, pVertex );
                Assert( iResult == _LINKAGE_DONE );

                // Destroy vertex case
                if ( pVertex->GetEdgeCount() == 0 ) {
                    bRemoved = m_mapVertices.Remove( &(pVertex->Key) );
                    Assert( bRemoved );

                    _SelectMemory();
                    Delete( pVertex );
                    _UnSelectMemory();
                }
            }

            bRemoved = m_mapEdges.Remove( &(pEdge->Key) );
            Assert( bRemoved );

            _SelectMemory();
            Delete( pEdge );
            _UnSelectMemory();
        }
    }

    // Destroy facet
    bRemoved = m_mapFacets.Remove( pFacet->GetKey() );
    Assert( bRemoved );

    _SelectMemory();
    Delete( pFacet );
    _UnSelectMemory();

    return true;
}

Void MeshManifold3::SortVertexLinkage()
{
    // Run through all vertices
    VertexMap::Iterator itVertex;
    for( itVertex = m_mapVertices.Begin(); !(itVertex.IsNull()); ++itVertex ) {
        MeshVertex * pVertex = itVertex.GetItem();
        if ( pVertex->GetEdgeCapacity() == 0 )
            continue;

        // Pick first edge as start point (arbitrary)
        MeshEdge * pPrevEdge = pVertex->GetEdge(0);
        if ( pPrevEdge->GetFacetCapacity() == 0 )
            continue;

        // Pick first facet as start point (arbitrary)
        MeshFacet * pFacet = pPrevEdge->GetFacet(0);
        if ( pFacet->GetVertexCapacity() == 0 )
            continue;
        if ( pFacet->GetEdgeCapacity() == 0 )
            continue;

        // Run around the vertex using facet-edge linkage
        for( UInt iNewIndex = 0; iNewIndex < pVertex->GetEdgeCount(); ++iNewIndex ) {
            UInt iPrevEdge = pFacet->FindVertex( pVertex );
            if ( iPrevEdge == 0 )
                iPrevEdge = pFacet->GetEdgeCount();
            --iPrevEdge;

            pPrevEdge = pFacet->GetEdge( iPrevEdge );

            // Linkage
            pVertex->ReplaceEdge( iNewIndex, pPrevEdge );
            if ( pVertex->GetFacetCapacity() != 0 )
                pVertex->ReplaceFacet( iNewIndex, pFacet );

            // Next facet
            UInt iPrevFacet = pPrevEdge->FindFacet( pFacet );
            pFacet = pPrevEdge->GetFacet( 1 - iPrevFacet );
        }
    }
}
Void MeshManifold3::SortEdgeLinkage()
{
    // Run through all edges
    EdgeMap::Iterator itEdge;
    for( itEdge = m_mapEdges.Begin(); !(itEdge.IsNull()); ++itEdge ) {
        MeshEdge * pEdge = itEdge.GetItem();
        if ( pEdge->GetVertexCapacity() == 0 )
            continue;
        if ( pEdge->GetFacetCapacity() == 0 )
            continue;

        // Pick facet 0
        MeshFacet * pFacet = pEdge->GetFacet( 0 );
        if ( pFacet->GetVertexCapacity() == 0 )
            continue;

        UInt iVertex0 = pFacet->FindVertex( pEdge->GetVertex(0) );
        UInt iNext = (iVertex0 + 1) % pFacet->GetVertexCount();
        Bool bCCW = ( pFacet->GetVertex(iNext) == pEdge->GetVertex(1) );

        // Linkage (just swap facets)
        if ( !bCCW ) {
            pFacet = pEdge->GetFacet( 0 );
            pEdge->ReplaceFacet( 0, pEdge->GetFacet(1) );
            pEdge->ReplaceFacet( 1, pFacet );
        }
    }
}

Bool MeshManifold3::IsClosed() const
{
    // A 3-manifold mesh is closed if each edge has
    // exactly 2 adjacent facets ...
    EdgeMap::Iterator itEdge;
    const MeshEdge * pEdge = NULL;
    for( itEdge = m_mapEdges.Begin(); !(itEdge.IsNull()); ++itEdge ) {
        pEdge = itEdge.GetItem();
        if ( pEdge->GetFacetCount() != 2 )
            return false;
    }
    return true;
}

