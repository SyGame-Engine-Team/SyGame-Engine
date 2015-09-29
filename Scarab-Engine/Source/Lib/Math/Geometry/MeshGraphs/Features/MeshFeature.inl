/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/MeshFeature.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Vertex-Edge-Facet features for MeshGraphs
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// MeshFeature implementation
inline MeshLinkageType MeshFeature::GetLinkageType() const {
    return MESH_LINKAGE_NULL;
}

inline UInt MeshFeature::GetVertexCapacity() const {
    return 0;
}
inline UInt MeshFeature::GetVertexCount() const {
    Assert( false );
    return 0;
}
inline MeshVertex * MeshFeature::GetVertex( UInt /*iVertex*/ ) const {
    Assert( false );
    return NULL;
}

inline UInt MeshFeature::FindVertex( const MeshVertex * /*pVertex*/ ) const {
    Assert( false );
    return INVALID_OFFSET;
}
inline Void MeshFeature::ReplaceVertex( UInt /*iVertex*/, MeshVertex * /*pVertex*/ ) {
    Assert( false );
}

inline UInt MeshFeature::ConnectVertex( MeshVertex * /*pVertex*/ ) {
    Assert( false );
    return INVALID_OFFSET;
}
inline Void MeshFeature::DisconnectVertex( UInt /*iVertex*/ ) {
    Assert( false );
}
inline Void MeshFeature::DisconnectVertices() {
    Assert( false );
}

inline UInt MeshFeature::GetEdgeCapacity() const {
    return 0;
}
inline UInt MeshFeature::GetEdgeCount() const {
    Assert( false );
    return 0;
}
inline MeshEdge * MeshFeature::GetEdge( UInt /*iEdge*/ ) const {
    Assert( false );
    return NULL;
}

inline UInt MeshFeature::FindEdge( const MeshEdge * /*pEdge*/ ) const {
    Assert( false );
    return INVALID_OFFSET;
}
inline Void MeshFeature::ReplaceEdge( UInt /*iEdge*/, MeshEdge * /*pEdge*/ ) {
    Assert( false );
}

inline UInt MeshFeature::ConnectEdge( MeshEdge * /*pEdge*/ ) {
    Assert( false );
    return INVALID_OFFSET;
}
inline Void MeshFeature::DisconnectEdge( UInt /*iEdge*/ ) {
    Assert( false );
}
inline Void MeshFeature::DisconnectEdges() {
    Assert( false );
}

inline UInt MeshFeature::GetFacetCapacity() const {
    return 0;
}
inline UInt MeshFeature::GetFacetCount() const {
    Assert( false );
    return 0;
}
inline MeshFacet * MeshFeature::GetFacet( UInt /*iFacet*/ ) const {
    Assert( false );
    return NULL;
}

inline UInt MeshFeature::FindFacet( const MeshFacet * /*pFacet*/ ) const {
    Assert( false );
    return INVALID_OFFSET;
}
inline Void MeshFeature::ReplaceFacet( UInt /*iFacet*/, MeshFacet * /*pFacet*/ ) {
    Assert( false );
}

inline UInt MeshFeature::ConnectFacet( MeshFacet * /*pFacet*/ ) {
    Assert( false );
    return INVALID_OFFSET;
}
inline Void MeshFeature::DisconnectFacet( UInt /*iFacet*/ ) {
    Assert( false );
}
inline Void MeshFeature::DisconnectFacets() {
    Assert( false );
}

inline Void MeshFeature::DisconnectAll() {
    DisconnectVertices();
    DisconnectEdges();
    DisconnectFacets();
}

/////////////////////////////////////////////////////////////////////////////////
// MeshVertex implementation
inline MeshFeatureType MeshVertex::GetType() const {
    return MESH_FEATURE_VERTEX;
}

/////////////////////////////////////////////////////////////////////////////////
// MeshEdge implementation
inline MeshFeatureType MeshEdge::GetType() const {
    return MESH_FEATURE_EDGE;
}

/////////////////////////////////////////////////////////////////////////////////
// MeshFacet implementation
inline MeshFeatureType MeshFacet::GetType() const {
    return MESH_FEATURE_FACET;
}

/////////////////////////////////////////////////////////////////////////////////
// MeshFacetTriangle implementation
inline MeshFacetType MeshFacetTriangle::GetFacetType() const {
    return MESH_FACET_TRIANGLE;
}
inline UInt MeshFacetTriangle::GetFacetSize() const {
    return 3;
}

//inline MeshFacet * MeshFacetTriangle::Shrink( const MeshFacetKey & newKey ) {
//    return NULL;
//}

inline const MeshFacetKey * MeshFacetTriangle::GetKey() const {
    return &Key;
}

/////////////////////////////////////////////////////////////////////////////////
// MeshFacetQuad implementation
inline MeshFacetType MeshFacetQuad::GetFacetType() const {
    return MESH_FACET_QUAD;
}
inline UInt MeshFacetQuad::GetFacetSize() const {
    return 4;
}

inline const MeshFacetKey * MeshFacetQuad::GetKey() const {
    return &Key;
}

/////////////////////////////////////////////////////////////////////////////////
// MeshFacetPolygon implementation
inline MeshFacetType MeshFacetPolygon::GetFacetType() const {
    return MESH_FACET_POLYGON;
}
inline UInt MeshFacetPolygon::GetFacetSize() const {
    return Key.SIZE;
}

inline const MeshFacetKey * MeshFacetPolygon::GetKey() const {
    return &Key;
}


