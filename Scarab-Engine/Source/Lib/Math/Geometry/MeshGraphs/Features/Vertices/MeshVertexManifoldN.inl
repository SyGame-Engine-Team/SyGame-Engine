/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/Vertices/MeshVertexManifoldN.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Vertex features : 3-Manifold, N-Manifold MeshGraphs
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// MeshVertexManifoldN implementation
inline MeshVertexType MeshVertexManifoldN::GetVertexType() const {
    return MESH_VERTEX_MANIFOLDN;
}

//inline MeshVertex * MeshVertexManifoldN::Upgrade() {
//    return NULL;
//}

/////////////////////////////////////////////////////////////////////////////////
// MeshVertexManifoldN_V implementation
inline MeshLinkageType MeshVertexManifoldN_V::GetLinkageType() const {
    return MESH_LINKAGE_V;
}

_MESHFEATURE_IMPLEMENT_INLINES_DYNARRAY( MeshVertexManifoldN_V, Vertex, Vertices )

/////////////////////////////////////////////////////////////////////////////////
// MeshVertexManifoldN_E implementation
inline MeshLinkageType MeshVertexManifoldN_E::GetLinkageType() const {
    return MESH_LINKAGE_E;
}

_MESHFEATURE_IMPLEMENT_INLINES_DYNARRAY( MeshVertexManifoldN_E, Edge, Edges )

/////////////////////////////////////////////////////////////////////////////////
// MeshVertexManifoldN_F implementation
inline MeshLinkageType MeshVertexManifoldN_F::GetLinkageType() const {
    return MESH_LINKAGE_F;
}

_MESHFEATURE_IMPLEMENT_INLINES_DYNARRAY( MeshVertexManifoldN_F, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshVertexManifoldN_VE implementation
inline MeshLinkageType MeshVertexManifoldN_VE::GetLinkageType() const {
    return MESH_LINKAGE_VE;
}

_MESHFEATURE_IMPLEMENT_INLINES_DYNARRAY( MeshVertexManifoldN_VE, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_INLINES_DYNARRAY( MeshVertexManifoldN_VE, Edge, Edges )

/////////////////////////////////////////////////////////////////////////////////
// MeshVertexManifoldN_VF implementation
inline MeshLinkageType MeshVertexManifoldN_VF::GetLinkageType() const {
    return MESH_LINKAGE_VF;
}

_MESHFEATURE_IMPLEMENT_INLINES_DYNARRAY( MeshVertexManifoldN_VF, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_INLINES_DYNARRAY( MeshVertexManifoldN_VF, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshVertexManifoldN_EF implementation
inline MeshLinkageType MeshVertexManifoldN_EF::GetLinkageType() const {
    return MESH_LINKAGE_EF;
}

_MESHFEATURE_IMPLEMENT_INLINES_DYNARRAY( MeshVertexManifoldN_EF, Edge, Edges )
_MESHFEATURE_IMPLEMENT_INLINES_DYNARRAY( MeshVertexManifoldN_EF, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshVertexManifoldN_VEF implementation
inline MeshLinkageType MeshVertexManifoldN_VEF::GetLinkageType() const {
    return MESH_LINKAGE_VEF;
}

_MESHFEATURE_IMPLEMENT_INLINES_DYNARRAY( MeshVertexManifoldN_VEF, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_INLINES_DYNARRAY( MeshVertexManifoldN_VEF, Edge, Edges )
_MESHFEATURE_IMPLEMENT_INLINES_DYNARRAY( MeshVertexManifoldN_VEF, Facet, Facets )

