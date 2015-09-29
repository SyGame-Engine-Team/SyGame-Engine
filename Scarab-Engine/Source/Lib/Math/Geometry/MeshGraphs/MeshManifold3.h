/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/MeshManifold3.h
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
// Header prelude
#ifndef SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_MESHMANIFOLD3_H
#define SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_MESHMANIFOLD3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "MeshGraph.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The MeshManifold3 class
class MeshManifold3 : public MeshGraph
{
public: // Features
    typedef MeshVertex * (*VertexBuilder)( const MeshVertexKey & );
    typedef MeshEdge *   (*EdgeBuilder)  ( const MeshEdgeKey & );
    typedef MeshFacet *  (*FacetBuilder) ( const MeshFacetKey & );

    typedef MeshVertexManifoldN_EF   DefaultVertex;
    typedef MeshEdgeManifold3_VF     DefaultEdge;
    typedef MeshTriangleManifold3_VE DefaultTriangle;
    typedef MeshQuadManifold3_VE     DefaultQuad;
    typedef MeshPolygonManifold3_VE  DefaultPolygon;

    inline static MeshVertex * DefaultVertexBuilder( const MeshVertexKey & vertexKey );
    inline static MeshEdge *   DefaultEdgeBuilder  ( const MeshEdgeKey & edgeKey );
    inline static MeshFacet *  DefaultFacetBuilder ( const MeshFacetKey & facetKey );

public:
    MeshManifold3( VertexBuilder pfVertexBuilder = DefaultVertexBuilder,
                   EdgeBuilder   pfEdgeBuilder   = DefaultEdgeBuilder,
                   FacetBuilder  pfFacetBuilder  = DefaultFacetBuilder );
    virtual ~MeshManifold3();

    // Type
    inline virtual MeshGraphType GetType() const;

    // delayed construction
    inline virtual Void UseMemoryContext( MemoryContextID idMemoryContext, const GChar * strAllocatorName );

    // Common minimal methods
    virtual Void Create();
    virtual Void Destroy();

    inline virtual UInt MemorySize() const;
    inline virtual UInt Count() const;

    virtual Void Clear();

    // Vertex access
    inline UInt GetVertexCount() const;
    inline Bool HasVertex( const MeshVertexKey & vertexKey ) const;
    inline MeshVertex * GetVertex( const MeshVertexKey & vertexKey ) const;

    inline Void EnumVertices() const;
    inline MeshVertex * EnumNextVertex() const;

    // Edge access
    inline UInt GetEdgeCount() const;
    inline Bool HasEdge( const MeshEdgeKey & edgeKey ) const;
    inline MeshEdge * GetEdge( const MeshEdgeKey & edgeKey ) const;

    inline Void EnumEdges() const;
    inline MeshEdge * EnumNextEdge() const;

    // Facet access
    inline UInt GetFacetCount() const;
    inline Bool HasFacet( const MeshFacetKey & facetKey ) const;
    inline MeshFacet * GetFacet( const MeshFacetKey & facetKey ) const;

    inline Void EnumFacets() const;
    inline MeshFacet * EnumNextFacet() const;

    // Build operations
    MeshFacet * CreateFacet( const MeshFacetKey & facetKey );
    Bool DestroyFacet( const MeshFacetKey & facetKey );

    // Sorting
        // Edges & facets gets the same ordering as facets
        // inside vertices links, such that edges (k, k-1)
        // enclose facet k, from the vertex's viewpoint
    Void SortVertexLinkage();
        // Facet 0 gets on the side where edge's vertex ordering
        // matches that facet's ordering, Facet 1 gets opposite
        // ordering (matching edge's orientation with facet 0)
    Void SortEdgeLinkage();

    // Topology operations
    Bool IsClosed() const;

private:
    typedef TreeMap< const MeshVertexKey *, MeshVertex* > VertexMap;
    typedef TreeMap< const MeshEdgeKey *,   MeshEdge*   > EdgeMap;
    typedef TreeMap< const MeshFacetKey *,  MeshFacet*  > FacetMap;

    // Builders
    VertexBuilder m_pfVertexBuilder;
    EdgeBuilder m_pfEdgeBuilder;
    FacetBuilder m_pfFacetBuilder;

    // Vertices
    VertexMap m_mapVertices;
    mutable VertexMap::Iterator m_itVertex;

    // Edges
    EdgeMap m_mapEdges;
    mutable EdgeMap::Iterator m_itEdge;

    // Facets
    FacetMap m_mapFacets;
    mutable FacetMap::Iterator m_itFacet;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MeshManifold3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_MESHGRAPHS_MESHMANIFOLD3_H

