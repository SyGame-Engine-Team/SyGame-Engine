/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/Edges/MeshEdgeManifoldN.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Edge features : N-Manifold MeshGraphs
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// MeshEdgeManifoldN implementation
inline MeshEdgeType MeshEdgeManifoldN::GetEdgeType() const {
    return MESH_EDGE_MANIFOLDN;
}

//inline MeshEdge * MeshEdgeManifoldN::Upgrade() {
//    return NULL;
//}

/////////////////////////////////////////////////////////////////////////////////
// MeshEdgeManifoldN_F implementation
inline MeshLinkageType MeshEdgeManifoldN_F::GetLinkageType() const {
    return MESH_LINKAGE_F;
}

_MESHFEATURE_IMPLEMENT_INLINES_DYNARRAY( MeshEdgeManifoldN_F, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshEdgeManifoldN_VF implementation
inline MeshLinkageType MeshEdgeManifoldN_VF::GetLinkageType() const {
    return MESH_LINKAGE_VF;
}

_MESHFEATURE_IMPLEMENT_INLINES_2( MeshEdgeManifoldN_VF, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_INLINES_DYNARRAY( MeshEdgeManifoldN_VF, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshEdgeManifoldN_EF implementation
inline MeshLinkageType MeshEdgeManifoldN_EF::GetLinkageType() const {
    return MESH_LINKAGE_EF;
}

_MESHFEATURE_IMPLEMENT_INLINES_DYNARRAY( MeshEdgeManifoldN_EF, Edge, Edges )
_MESHFEATURE_IMPLEMENT_INLINES_DYNARRAY( MeshEdgeManifoldN_EF, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshEdgeManifoldN_VEF implementation
inline MeshLinkageType MeshEdgeManifoldN_VEF::GetLinkageType() const {
    return MESH_LINKAGE_VEF;
}

_MESHFEATURE_IMPLEMENT_INLINES_2( MeshEdgeManifoldN_VEF, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_INLINES_DYNARRAY( MeshEdgeManifoldN_VEF, Edge, Edges )
_MESHFEATURE_IMPLEMENT_INLINES_DYNARRAY( MeshEdgeManifoldN_VEF, Facet, Facets )

