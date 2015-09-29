/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/Edges/MeshEdgeManifold3.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Edge features : 3-Manifold MeshGraphs
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// MeshEdgeManifold3 implementation
inline MeshEdgeType MeshEdgeManifold3::GetEdgeType() const {
    return MESH_EDGE_MANIFOLD3;
}

/////////////////////////////////////////////////////////////////////////////////
// MeshEdgeManifold3_E implementation
inline MeshLinkageType MeshEdgeManifold3_E::GetLinkageType() const {
    return MESH_LINKAGE_E;
}

_MESHFEATURE_IMPLEMENT_INLINES_DYNARRAY( MeshEdgeManifold3_E, Edge, Edges )

/////////////////////////////////////////////////////////////////////////////////
// MeshEdgeManifold3_F implementation
inline MeshLinkageType MeshEdgeManifold3_F::GetLinkageType() const {
    return MESH_LINKAGE_F;
}

_MESHFEATURE_IMPLEMENT_INLINES_2( MeshEdgeManifold3_F, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshEdgeManifold3_VE implementation
inline MeshLinkageType MeshEdgeManifold3_VE::GetLinkageType() const {
    return MESH_LINKAGE_VE;
}

_MESHFEATURE_IMPLEMENT_INLINES_2( MeshEdgeManifold3_VE, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_INLINES_DYNARRAY( MeshEdgeManifold3_VE, Edge, Edges )

/////////////////////////////////////////////////////////////////////////////////
// MeshEdgeManifold3_VF implementation
inline MeshLinkageType MeshEdgeManifold3_VF::GetLinkageType() const {
    return MESH_LINKAGE_VF;
}

_MESHFEATURE_IMPLEMENT_INLINES_2( MeshEdgeManifold3_VF, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_INLINES_2( MeshEdgeManifold3_VF, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshEdgeManifold3_EF implementation
inline MeshLinkageType MeshEdgeManifold3_EF::GetLinkageType() const {
    return MESH_LINKAGE_EF;
}

_MESHFEATURE_IMPLEMENT_INLINES_DYNARRAY( MeshEdgeManifold3_EF, Edge, Edges )
_MESHFEATURE_IMPLEMENT_INLINES_2( MeshEdgeManifold3_EF, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshEdgeManifold3_VEF implementation
inline MeshLinkageType MeshEdgeManifold3_VEF::GetLinkageType() const {
    return MESH_LINKAGE_VEF;
}

_MESHFEATURE_IMPLEMENT_INLINES_2( MeshEdgeManifold3_VEF, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_INLINES_DYNARRAY( MeshEdgeManifold3_VEF, Edge, Edges )
_MESHFEATURE_IMPLEMENT_INLINES_2( MeshEdgeManifold3_VEF, Facet, Facets )

