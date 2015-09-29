/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Graphs/MeshVoronoiGraph.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Mesh Graph : Specialization for collision detection
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TMeshVoronoiGraph:Features implementation
template<typename Real>
TMeshVoronoiGraph<Real>::Vertex::Vertex( const MeshVertexKey & vertexKey ):
    MeshVertexManifoldN_E( vertexKey ), Position()
{
    VoronoiRegion = NULL;
}
template<typename Real>
TMeshVoronoiGraph<Real>::Vertex::~Vertex()
{
    if ( VoronoiRegion != NULL )
        DeleteA( VoronoiRegion );
}

template<typename Real>
TMeshVoronoiGraph<Real>::Edge::Edge( const MeshEdgeKey & edgeKey ):
    MeshEdgeManifold3_VF( edgeKey )
{
    // nothing to do
}
template<typename Real>
TMeshVoronoiGraph<Real>::Edge::~Edge()
{
    // nothing to do
}

template<typename Real>
TMeshVoronoiGraph<Real>::Triangle::Triangle( const MeshTriangleKey & triangleKey ):
    MeshTriangleManifold3_VE( triangleKey ), Normal()
{
    // nothing to do
}
template<typename Real>
TMeshVoronoiGraph<Real>::Triangle::~Triangle()
{
    // nothing to do
}

template<typename Real>
TMeshVoronoiGraph<Real>::Quad::Quad( const MeshQuadKey & quadKey ):
    MeshQuadManifold3_VE( quadKey ), Normal()
{
    // nothing to do
}
template<typename Real>
TMeshVoronoiGraph<Real>::Quad::~Quad()
{
    // nothing to do
}

template<typename Real>
TMeshVoronoiGraph<Real>::Polygon::Polygon( const MeshPolygonKey & polygonKey ):
    MeshPolygonManifold3_VE( polygonKey ), Normal()
{
    VoronoiRegion = NULL;
}
template<typename Real>
TMeshVoronoiGraph<Real>::Polygon::~Polygon()
{
    if ( VoronoiRegion != NULL )
        DeleteA( VoronoiRegion );
}

/////////////////////////////////////////////////////////////////////////////////
// TMeshVoronoiGraph implementation
template<typename Real>
TMeshVoronoiGraph<Real>::TMeshVoronoiGraph():
    MeshManifold3( _VoronoiVertexBuilder, _VoronoiEdgeBuilder, _VoronoiFacetBuilder )
{
    // nothing to do
}
template<typename Real>
TMeshVoronoiGraph<Real>::~TMeshVoronoiGraph()
{
    // nothing to do
}

template<typename Real>
inline const TVertex3<Real> & TMeshVoronoiGraph<Real>::GetVertexPosition( const MeshVertexKey & vertexKey ) const {
    const Vertex * pVertex = (const Vertex *)( GetVertex(vertexKey) );
    if ( pVertex == NULL )
        return TVertex3<Real>::Null;
    return pVertex->Position;
}
template<typename Real>
inline const TVertex3<Real> & TMeshVoronoiGraph<Real>::GetVertexPosition( const MeshVertex * pVertex ) const {
    return ((const Vertex *)pVertex)->Position;
}

template<typename Real>
inline const TVector3<Real> & TMeshVoronoiGraph<Real>::GetFacetNormal( const MeshFacetKey & facetKey ) const {
    const MeshFacet * pFacet = GetFacet( facetKey );
    if ( pFacet == NULL )
        return TVector3<Real>::Null;
    return GetFacetNormal( pFacet );
}
template<typename Real>
const TVector3<Real> & TMeshVoronoiGraph<Real>::GetFacetNormal( const MeshFacet * pFacet ) const
{
    switch( pFacet->GetFacetType() ) {
        case MESH_FACET_TRIANGLE: return ((const Triangle *)pFacet)->Normal;
        case MESH_FACET_QUAD:     return ((const Quad *)pFacet)->Normal;
        case MESH_FACET_POLYGON:  return ((const Polygon *)pFacet)->Normal;
        default: Assert(false); break;
    }
    return TVector3<Real>::Null; // Should never happen
}

template<typename Real>
UInt TMeshVoronoiGraph<Real>::GetVoronoiPlaneCount( const MeshFeatureKey & featureKey ) const
{
    const MeshFeature * pFeature;
    const MeshFacet * pFacet;
    switch( featureKey.GetType() ) {
        case MESH_FEATURE_VERTEX:
            pFeature = GetVertex( (const MeshVertexKey &)featureKey );
            if ( pFeature == NULL )
                return 0;
            return ((const Vertex *)pFeature)->GetEdgeCount();
        case MESH_FEATURE_EDGE:
            pFeature = GetEdge( (const MeshEdgeKey &)featureKey );
            if ( pFeature == NULL )
                return 0;
            return 4;
        case MESH_FEATURE_FACET:
            pFacet = GetFacet( (const MeshFacetKey &)featureKey );
            if ( pFacet == NULL )
                return 0;
            switch( pFacet->GetFacetType() ) {
                case MESH_FACET_TRIANGLE: return 3;
                case MESH_FACET_QUAD:     return 4;
                case MESH_FACET_POLYGON:  return ((const Polygon *)pFacet)->GetEdgeCount();
                default: Assert(false); break;
            }
        default: Assert(false); break;
    }
    return 0; // Should never happen
}
template<typename Real>
UInt TMeshVoronoiGraph<Real>::GetVoronoiPlaneCount( const MeshFeature * pFeature ) const
{
    switch( pFeature->GetType() ) {
        case MESH_FEATURE_VERTEX: return ((const Vertex *)pFeature)->GetEdgeCount();
        case MESH_FEATURE_EDGE:   return 4;
        case MESH_FEATURE_FACET: {
            const MeshFacet * pFacet = (const MeshFacet *)pFeature;
            switch( pFacet->GetFacetType() ) {
                case MESH_FACET_TRIANGLE: return 3;
                case MESH_FACET_QUAD:     return 4;
                case MESH_FACET_POLYGON:  return ((const Polygon *)pFacet)->GetEdgeCount();
                default: Assert(false); break;
            }
        } break;
        default: Assert(false); break;
    }
    return 0; // Should never happen
}

template<typename Real>
const TPlane<Real> & TMeshVoronoiGraph<Real>::GetVoronoiPlane( const MeshFeatureKey & featureKey, UInt iPlane ) const
{
    static TPlane<Real> nullPlane( TVector3<Real>::Null, TVertex3<Real>::Null );
    const MeshFeature * pFeature;
    const MeshFacet * pFacet;
    switch( featureKey.GetType() ) {
        case MESH_FEATURE_VERTEX:
            pFeature = GetVertex( (const MeshVertexKey &)featureKey );
            if ( pFeature == NULL )
                return nullPlane;
            return ((const Vertex *)pFeature)->VoronoiRegion[iPlane];
        case MESH_FEATURE_EDGE:
            pFeature = GetEdge( (const MeshEdgeKey &)featureKey );
            if ( pFeature == NULL )
                return nullPlane;
            return ((const Edge *)pFeature)->VoronoiRegion[iPlane];
        case MESH_FEATURE_FACET:
            pFacet = GetFacet( (const MeshFacetKey &)featureKey );
            if ( pFacet == NULL )
                return nullPlane;
            switch( pFacet->GetFacetType() ) {
                case MESH_FACET_TRIANGLE: return ((const Triangle *)pFacet)->VoronoiRegion[iPlane];
                case MESH_FACET_QUAD:     return ((const Quad *)pFacet)->VoronoiRegion[iPlane];
                case MESH_FACET_POLYGON:  return ((const Polygon *)pFacet)->VoronoiRegion[iPlane];
                default: Assert(false); break;
            }
        default: Assert(false); break;
    }
    return nullPlane; // Should never happen
}
template<typename Real>
const TPlane<Real> & TMeshVoronoiGraph<Real>::GetVoronoiPlane( const MeshFeature * pFeature, UInt iPlane ) const
{
    static TPlane<Real> nullPlane( TVector3<Real>::Null, TVertex3<Real>::Null );
    switch( pFeature->GetType() ) {
        case MESH_FEATURE_VERTEX: return ((const Vertex *)pFeature)->VoronoiRegion[iPlane];
        case MESH_FEATURE_EDGE:   return ((const Edge *)pFeature)->VoronoiRegion[iPlane];
        case MESH_FEATURE_FACET: {
            const MeshFacet * pFacet = (const MeshFacet *)pFeature;
            switch( pFacet->GetFacetType() ) {
                case MESH_FACET_TRIANGLE: return ((const Triangle *)pFacet)->VoronoiRegion[iPlane];
                case MESH_FACET_QUAD:     return ((const Quad *)pFacet)->VoronoiRegion[iPlane];
                case MESH_FACET_POLYGON:  return ((const Polygon *)pFacet)->VoronoiRegion[iPlane];
                default: Assert(false); break;
            }
        } break;
        default: Assert(false); break;
    }
    return nullPlane; // Should never happen
}

template<typename Real>
inline Void TMeshVoronoiGraph<Real>::ComputeFeatureData( const TVertex3<Real> * arrVertices ) {
    ComputeFeatureData( (const Byte *)arrVertices, sizeof(TVertex3<Real>) );
}
template<typename Real>
Void TMeshVoronoiGraph<Real>::ComputeFeatureData( const Byte * arrVertices, UInt iStride )
{
    // Compute vertex positions
    EnumVertices();
    MeshVertex * pVertex = EnumNextVertex();
    while( pVertex != NULL ) {
        ((Vertex*)pVertex)->Position = _GetPosition( pVertex->Key.V, arrVertices, iStride );
        pVertex = EnumNextVertex();
    }

    // Compute facet normals
    EnumFacets();
    MeshFacet * pFacet = EnumNextFacet();
    while( pFacet != NULL ) {
        switch( pFacet->GetFacetType() ) {
            case MESH_FACET_TRIANGLE: _ComputeFacetNormal( (Triangle*)pFacet ); break;
            case MESH_FACET_QUAD:     _ComputeFacetNormal( (Quad*)pFacet );     break;
            case MESH_FACET_POLYGON:  _ComputeFacetNormal( (Polygon*)pFacet );  break;
            default: Assert(false); break;
        }
        pFacet = EnumNextFacet();
    }

    // Compute voronoi regions
    EnumVertices();
    pVertex = EnumNextVertex();
    while( pVertex != NULL ) {
        _ComputeVoronoiRegion( (Vertex*)pVertex );
        pVertex = EnumNextVertex();
    }

    EnumEdges();
    MeshEdge * pEdge = EnumNextEdge();
    while( pEdge != NULL ) {
        _ComputeVoronoiRegion( (Edge*)pEdge );
        pEdge = EnumNextEdge();
    }

    EnumFacets();
    pFacet = EnumNextFacet();
    while( pFacet != NULL ) {
        switch( pFacet->GetFacetType() ) {
            case MESH_FACET_TRIANGLE: _ComputeVoronoiRegion( (Triangle*)pFacet ); break;
            case MESH_FACET_QUAD:     _ComputeVoronoiRegion( (Quad*)pFacet );     break;
            case MESH_FACET_POLYGON:  _ComputeVoronoiRegion( (Polygon*)pFacet );  break;
            default: Assert(false); break;
        }
        pFacet = EnumNextFacet();
    }
}

/////////////////////////////////////////////////////////////////////////////////

template<typename Real>
inline MeshVertex * TMeshVoronoiGraph<Real>::_VoronoiVertexBuilder( const MeshVertexKey & vertexKey ) {
    return New Vertex( vertexKey );
}
template<typename Real>
inline MeshEdge * TMeshVoronoiGraph<Real>::_VoronoiEdgeBuilder( const MeshEdgeKey & edgeKey ) {
    return New Edge( edgeKey );
}
template<typename Real>
inline MeshFacet * TMeshVoronoiGraph<Real>::_VoronoiFacetBuilder( const MeshFacetKey & facetKey ) {
    switch( facetKey.GetFacetType() ) {
        case MESH_FACET_TRIANGLE: return New Triangle( (const MeshTriangleKey &)facetKey );
        case MESH_FACET_QUAD:     return New Quad    ( (const MeshQuadKey &)    facetKey );
        case MESH_FACET_POLYGON:  return New Polygon ( (const MeshPolygonKey &) facetKey );
        default: Assert(false); break;
    }
    return NULL;
}

template<typename Real>
inline const TVertex3<Real> & TMeshVoronoiGraph<Real>::_GetPosition( UInt iVertex, const Byte * arrVertices, UInt iStride ) {
    const TVertex3<Real> * pPosition = (const TVertex3<Real> *)( arrVertices + (iVertex * iStride) );
    return *pPosition;
}

template<typename Real>
Void TMeshVoronoiGraph<Real>::_ComputeFacetNormal( Triangle * pTriangle )
{
    const TVertex3<Real> & vA = ((Vertex*)(pTriangle->Vertices[0]))->Position;
    const TVertex3<Real> & vB = ((Vertex*)(pTriangle->Vertices[1]))->Position;
    const TVertex3<Real> & vC = ((Vertex*)(pTriangle->Vertices[2]))->Position;

    pTriangle->Normal = ( (vB - vA) ^ (vC - vA) );
    pTriangle->Normal.Normalize();
}
template<typename Real>
Void TMeshVoronoiGraph<Real>::_ComputeFacetNormal( Quad * pQuad )
{
    // TODO : Check ropes colinearity and pick a good pair to compute normal

    const TVertex3<Real> & vA = ((Vertex*)(pQuad->Vertices[0]))->Position;
    const TVertex3<Real> & vB = ((Vertex*)(pQuad->Vertices[1]))->Position;
    const TVertex3<Real> & vC = ((Vertex*)(pQuad->Vertices[2]))->Position;

    pQuad->Normal = ( (vB - vA) ^ (vC - vA) );
    pQuad->Normal.Normalize();
}
template<typename Real>
Void TMeshVoronoiGraph<Real>::_ComputeFacetNormal( Polygon * pPolygon )
{
    // TODO : Check ropes colinearity and pick a good pair to compute normal

    const TVertex3<Real> & vA = ((Vertex*)(pPolygon->Vertices[0]))->Position;
    const TVertex3<Real> & vB = ((Vertex*)(pPolygon->Vertices[1]))->Position;
    const TVertex3<Real> & vC = ((Vertex*)(pPolygon->Vertices[2]))->Position;

    pPolygon->Normal = ( (vB - vA) ^ (vC - vA) );
    pPolygon->Normal.Normalize();
}

template<typename Real>
Void TMeshVoronoiGraph<Real>::_ComputeVoronoiRegion( Vertex * pVertex )
{
    UInt iEdgeCount = pVertex->GetEdgeCount();

    // Allocate array if needed
    if ( pVertex->VoronoiRegion == NULL ) {
        _SelectMemory();
        pVertex->VoronoiRegion = New TPlane<Real>[iEdgeCount];
        _UnSelectMemory();
    }

    // Run around adjacent edges
    Edge * pEdge;
    UInt iOppositeVertex;
    Vertex * pOppositeVertex;
    TVector3<Real> vPlaneNormal;

    for( UInt iEdge = 0; iEdge < iEdgeCount; ++iEdge ) {
        pEdge = (Edge*)( pVertex->Edges[iEdge] );

        // Get opposite vertex on this edge
        iOppositeVertex = ( 1 - pEdge->FindVertex(pVertex) );
        pOppositeVertex = (Vertex*)( pEdge->Vertices[iOppositeVertex] );

        // Compute plane normal
        vPlaneNormal = ( pOppositeVertex->Position - pVertex->Position );
        vPlaneNormal.Normalize();

        // Setup Voronoi-Plane for this edge
        pVertex->VoronoiRegion[iEdge] = TPlane<Real>( vPlaneNormal, pVertex->Position );
    }
}
template<typename Real>
Void TMeshVoronoiGraph<Real>::_ComputeVoronoiRegion( Edge * pEdge )
{
    // Edge-Vertex planes
    const TVertex3<Real> & vA = ((Vertex*)(pEdge->Vertices[0]))->Position;
    const TVertex3<Real> & vB = ((Vertex*)(pEdge->Vertices[1]))->Position;
    TVector3<Real> vAB = ( vB - vA );

    TVector3<Real> vPlaneNormal = vAB;
    vPlaneNormal.Normalize();

    pEdge->VoronoiRegion[0] = TPlane<Real>( -vPlaneNormal, vA );
    pEdge->VoronoiRegion[1] = TPlane<Real>( vPlaneNormal, vB );

    // Edge-Facet planes
    MeshFacet * pFacet;
    TVector3<Real> vFacetNormal0, vFacetNormal1;
    pFacet = pEdge->Facets[0];
    switch( pFacet->GetFacetType() ) {
        case MESH_FACET_TRIANGLE: vFacetNormal0 = ( (Triangle*)pFacet )->Normal; break;
        case MESH_FACET_QUAD:     vFacetNormal0 = ( (Quad*)pFacet )->Normal;     break;
        case MESH_FACET_POLYGON:  vFacetNormal0 = ( (Polygon*)pFacet )->Normal;  break;
        default: Assert(false); break;
    }
    pFacet = pEdge->Facets[1];
    switch( pFacet->GetFacetType() ) {
        case MESH_FACET_TRIANGLE: vFacetNormal1 = ( (Triangle*)pFacet )->Normal; break;
        case MESH_FACET_QUAD:     vFacetNormal1 = ( (Quad*)pFacet )->Normal;     break;
        case MESH_FACET_POLYGON:  vFacetNormal1 = ( (Polygon*)pFacet )->Normal;  break;
        default: Assert(false); break;
    }

    vPlaneNormal = ( (-vAB) ^ vFacetNormal0 );
    vPlaneNormal.Normalize();
    pEdge->VoronoiRegion[2] = TPlane<Real>( vPlaneNormal, vA );

    vPlaneNormal = ( vAB ^ vFacetNormal1 );
    vPlaneNormal.Normalize();
    pEdge->VoronoiRegion[3] = TPlane<Real>( vPlaneNormal, vA );
}
template<typename Real>
Void TMeshVoronoiGraph<Real>::_ComputeVoronoiRegion( Triangle * pTriangle )
{
    // Run around adjacent edges
    Vertex *pVertexA, *pVertexB;
    TVector3<Real> vPlaneNormal;
    UInt iEdgeEnd;

    for( UInt iEdge = 0; iEdge < 3; ++iEdge ) {
        iEdgeEnd = (iEdge + 1) % 3;
        pVertexA = (Vertex*)( pTriangle->Vertices[iEdge] );
        pVertexB = (Vertex*)( pTriangle->Vertices[iEdgeEnd] );

        // Compute plane normal
        vPlaneNormal = ( (pVertexB->Position - pVertexA->Position) ^ pTriangle->Normal );
        vPlaneNormal.Normalize();

        // Setup Voronoi-Plane for this edge
        pTriangle->VoronoiRegion[iEdge] = TPlane<Real>( vPlaneNormal, pVertexA->Position );
    }
}
template<typename Real>
Void TMeshVoronoiGraph<Real>::_ComputeVoronoiRegion( Quad * pQuad )
{
    // Run around adjacent edges
    Vertex *pVertexA, *pVertexB;
    TVector3<Real> vPlaneNormal;
    UInt iEdgeEnd;

    for( UInt iEdge = 0; iEdge < 4; ++iEdge ) {
        iEdgeEnd = (iEdge + 1) % 4;
        pVertexA = (Vertex*)( pQuad->Vertices[iEdge] );
        pVertexB = (Vertex*)( pQuad->Vertices[iEdgeEnd] );

        // Compute plane normal
        vPlaneNormal = ( (pVertexB->Position - pVertexA->Position) ^ pQuad->Normal );
        vPlaneNormal.Normalize();

        // Setup Voronoi-Plane for this edge
        pQuad->VoronoiRegion[iEdge] = TPlane<Real>( vPlaneNormal, pVertexA->Position );
    }
}
template<typename Real>
Void TMeshVoronoiGraph<Real>::_ComputeVoronoiRegion( Polygon * pPolygon )
{
    UInt iEdgeCount = pPolygon->GetEdgeCount();

    // Allocate array if needed
    if ( pPolygon->VoronoiRegion == NULL ) {
        _SelectMemory();
        pPolygon->VoronoiRegion = New TPlane<Real>[iEdgeCount];
        _UnSelectMemory();
    }

    // Run around adjacent edges
    Vertex *pVertexA, *pVertexB;
    TVector3<Real> vPlaneNormal;
    UInt iEdgeEnd;

    for( UInt iEdge = 0; iEdge < iEdgeCount; ++iEdge ) {
        iEdgeEnd = (iEdge + 1) % iEdgeCount;
        pVertexA = (Vertex*)( pPolygon->Vertices[iEdge] );
        pVertexB = (Vertex*)( pPolygon->Vertices[iEdgeEnd] );

        // Compute plane normal
        vPlaneNormal = ( (pVertexB->Position - pVertexA->Position) ^ pPolygon->Normal );
        vPlaneNormal.Normalize();

        // Setup Voronoi-Plane for this edge
        pPolygon->VoronoiRegion[iEdge] = TPlane<Real>( vPlaneNormal, pVertexA->Position );
    }
}

