/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/Facets/MeshPolygonManifold3.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Polygon features : 3-Manifold MeshGraphs
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// MeshPolygonManifold3 implementation
inline MeshPolygonType MeshPolygonManifold3::GetPolygonType() const {
    return MESH_POLYGON_MANIFOLD3;
}

//inline MeshFacet * MeshPolygonManifold3::Downgrade() {
//    return NULL;
//}

/////////////////////////////////////////////////////////////////////////////////
// MeshPolygonManifold3_V implementation
inline MeshLinkageType MeshPolygonManifold3_V::GetLinkageType() const {
    return MESH_LINKAGE_V;
}

_MESHFEATURE_IMPLEMENT_INLINES_ARRAY( MeshPolygonManifold3_V, Vertex, Vertices, Key.SIZE )

/////////////////////////////////////////////////////////////////////////////////
// MeshPolygonManifold3_E implementation
inline MeshLinkageType MeshPolygonManifold3_E::GetLinkageType() const {
    return MESH_LINKAGE_E;
}

_MESHFEATURE_IMPLEMENT_INLINES_ARRAY( MeshPolygonManifold3_E, Edge, Edges, Key.SIZE )

/////////////////////////////////////////////////////////////////////////////////
// MeshPolygonManifold3_F implementation
inline MeshLinkageType MeshPolygonManifold3_F::GetLinkageType() const {
    return MESH_LINKAGE_F;
}

_MESHFEATURE_IMPLEMENT_INLINES_ARRAY( MeshPolygonManifold3_F, Facet, Facets, Key.SIZE )

/////////////////////////////////////////////////////////////////////////////////
// MeshPolygonManifold3_VE implementation
inline MeshLinkageType MeshPolygonManifold3_VE::GetLinkageType() const {
    return MESH_LINKAGE_VE;
}

_MESHFEATURE_IMPLEMENT_INLINES_ARRAY( MeshPolygonManifold3_VE, Vertex, Vertices, Key.SIZE )
_MESHFEATURE_IMPLEMENT_INLINES_ARRAY( MeshPolygonManifold3_VE, Edge, Edges, Key.SIZE )

/////////////////////////////////////////////////////////////////////////////////
// MeshPolygonManifold3_VF implementation
inline MeshLinkageType MeshPolygonManifold3_VF::GetLinkageType() const {
    return MESH_LINKAGE_VF;
}

_MESHFEATURE_IMPLEMENT_INLINES_ARRAY( MeshPolygonManifold3_VF, Vertex, Vertices, Key.SIZE )
_MESHFEATURE_IMPLEMENT_INLINES_ARRAY( MeshPolygonManifold3_VF, Facet, Facets, Key.SIZE )

/////////////////////////////////////////////////////////////////////////////////
// MeshPolygonManifold3_EF implementation
inline MeshLinkageType MeshPolygonManifold3_EF::GetLinkageType() const {
    return MESH_LINKAGE_EF;
}

_MESHFEATURE_IMPLEMENT_INLINES_ARRAY( MeshPolygonManifold3_EF, Edge, Edges, Key.SIZE )
_MESHFEATURE_IMPLEMENT_INLINES_ARRAY( MeshPolygonManifold3_EF, Facet, Facets, Key.SIZE )

/////////////////////////////////////////////////////////////////////////////////
// MeshPolygonManifold3_VEF implementation
inline MeshLinkageType MeshPolygonManifold3_VEF::GetLinkageType() const {
    return MESH_LINKAGE_VEF;
}

_MESHFEATURE_IMPLEMENT_INLINES_ARRAY( MeshPolygonManifold3_VEF, Vertex, Vertices, Key.SIZE )
_MESHFEATURE_IMPLEMENT_INLINES_ARRAY( MeshPolygonManifold3_VEF, Edge, Edges, Key.SIZE )
_MESHFEATURE_IMPLEMENT_INLINES_ARRAY( MeshPolygonManifold3_VEF, Facet, Facets, Key.SIZE )

