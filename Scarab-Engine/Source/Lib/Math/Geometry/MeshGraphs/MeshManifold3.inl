/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/MeshManifold3.inl
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
// MeshManifold3 implementation
inline MeshVertex * MeshManifold3::DefaultVertexBuilder( const MeshVertexKey & vertexKey ) {
    return New DefaultVertex( vertexKey );
}
inline MeshEdge * MeshManifold3::DefaultEdgeBuilder( const MeshEdgeKey & edgeKey ) {
    return New DefaultEdge( edgeKey );
}
inline MeshFacet * MeshManifold3::DefaultFacetBuilder( const MeshFacetKey & facetKey ) {
    switch( facetKey.GetFacetType() ) {
        case MESH_FACET_TRIANGLE: return New DefaultTriangle( (const MeshTriangleKey &)facetKey );
        case MESH_FACET_QUAD:     return New DefaultQuad    ( (const MeshQuadKey &)    facetKey );
        case MESH_FACET_POLYGON:  return New DefaultPolygon ( (const MeshPolygonKey &) facetKey );
        default: Assert(false); break;
    }
    return NULL;
}

inline MeshGraphType MeshManifold3::GetType() const {
    return MESHGRAPH_MANIFOLD3;
}

inline Void MeshManifold3::UseMemoryContext( MemoryContextID idMemoryContext, const GChar * strAllocatorName ) {
    Datastruct::UseMemoryContext( idMemoryContext, strAllocatorName );
    m_mapVertices.UseMemoryContext( idMemoryContext, strAllocatorName );
    m_mapEdges.UseMemoryContext( idMemoryContext, strAllocatorName );
    m_mapFacets.UseMemoryContext( idMemoryContext, strAllocatorName );
}

inline UInt MeshManifold3::MemorySize() const {
    return ( m_mapVertices.MemorySize() + m_mapEdges.MemorySize() + m_mapFacets.MemorySize() );
}
inline UInt MeshManifold3::Count() const {
    return ( m_mapVertices.Count() + m_mapEdges.Count() + m_mapFacets.Count() );
}

inline UInt MeshManifold3::GetVertexCount() const {
    return m_mapVertices.Count();
}
inline Bool MeshManifold3::HasVertex( const MeshVertexKey & vertexKey ) const {
    return m_mapVertices.Contains( &vertexKey );
}
inline MeshVertex * MeshManifold3::GetVertex( const MeshVertexKey & vertexKey ) const {
    VertexMap::Iterator itVertex = m_mapVertices.Get( &vertexKey );
    if ( itVertex.IsNull() )
        return NULL;
    return itVertex.GetItem();
}

inline Void MeshManifold3::EnumVertices() const {
    m_itVertex = m_mapVertices.Begin();
}
inline MeshVertex * MeshManifold3::EnumNextVertex() const {
    if ( m_itVertex.IsNull() )
        return NULL;
    MeshVertex * pVertex = m_itVertex.GetItem();
    ++m_itVertex;
    return pVertex;
}

inline UInt MeshManifold3::GetEdgeCount() const {
    return m_mapEdges.Count();
}
inline Bool MeshManifold3::HasEdge( const MeshEdgeKey & edgeKey ) const {
    return m_mapEdges.Contains( &edgeKey );
}
inline MeshEdge * MeshManifold3::GetEdge( const MeshEdgeKey & edgeKey ) const {
    EdgeMap::Iterator itEdge = m_mapEdges.Get( &edgeKey );
    if ( itEdge.IsNull() )
        return NULL;
    return itEdge.GetItem();
}

inline Void MeshManifold3::EnumEdges() const {
    m_itEdge = m_mapEdges.Begin();
}
inline MeshEdge * MeshManifold3::EnumNextEdge() const {
    if ( m_itEdge.IsNull() )
        return NULL;
    MeshEdge * pEdge = m_itEdge.GetItem();
    ++m_itEdge;
    return pEdge;
}

inline UInt MeshManifold3::GetFacetCount() const {
    return m_mapFacets.Count();
}
inline Bool MeshManifold3::HasFacet( const MeshFacetKey & facetKey ) const {
    return m_mapFacets.Contains( &facetKey );
}
inline MeshFacet * MeshManifold3::GetFacet( const MeshFacetKey & facetKey ) const {
    FacetMap::Iterator itFacet = m_mapFacets.Get( &facetKey );
    if ( itFacet.IsNull() )
        return NULL;
    return itFacet.GetItem();
}

inline Void MeshManifold3::EnumFacets() const {
    m_itFacet = m_mapFacets.Begin();
}
inline MeshFacet * MeshManifold3::EnumNextFacet() const {
    if ( m_itFacet.IsNull() )
        return NULL;
    MeshFacet * pFacet = m_itFacet.GetItem();
    ++m_itFacet;
    return pFacet;
}

