/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/Facets/MeshPolygonManifoldN.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Polygon features : N-Manifold MeshGraphs
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// MeshPolygonManifoldN implementation
inline MeshPolygonType MeshPolygonManifoldN::GetPolygonType() const {
    return MESH_POLYGON_MANIFOLDN;
}

//inline MeshFacet * MeshPolygonManifoldN::Upgrade() {
//    return NULL;
//}

/////////////////////////////////////////////////////////////////////////////////
// MeshPolygonManifoldN_F implementation
inline MeshLinkageType MeshPolygonManifoldN_F::GetLinkageType() const {
    return MESH_LINKAGE_F;
}

_MESHFEATURE_IMPLEMENT_INLINES_DYNARRAY( MeshPolygonManifoldN_F, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshPolygonManifoldN_VF implementation
inline MeshLinkageType MeshPolygonManifoldN_VF::GetLinkageType() const {
    return MESH_LINKAGE_VF;
}

_MESHFEATURE_IMPLEMENT_INLINES_ARRAY( MeshPolygonManifoldN_VF, Vertex, Vertices, Key.SIZE )
_MESHFEATURE_IMPLEMENT_INLINES_DYNARRAY( MeshPolygonManifoldN_VF, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshPolygonManifoldN_EF implementation
inline MeshLinkageType MeshPolygonManifoldN_EF::GetLinkageType() const {
    return MESH_LINKAGE_EF;
}

_MESHFEATURE_IMPLEMENT_INLINES_ARRAY( MeshPolygonManifoldN_EF, Edge, Edges, Key.SIZE )
_MESHFEATURE_IMPLEMENT_INLINES_DYNARRAY( MeshPolygonManifoldN_EF, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshPolygonManifoldN_VEF implementation
inline MeshLinkageType MeshPolygonManifoldN_VEF::GetLinkageType() const {
    return MESH_LINKAGE_VEF;
}

_MESHFEATURE_IMPLEMENT_INLINES_ARRAY( MeshPolygonManifoldN_VEF, Vertex, Vertices, Key.SIZE )
_MESHFEATURE_IMPLEMENT_INLINES_ARRAY( MeshPolygonManifoldN_VEF, Edge, Edges, Key.SIZE )
_MESHFEATURE_IMPLEMENT_INLINES_DYNARRAY( MeshPolygonManifoldN_VEF, Facet, Facets )

