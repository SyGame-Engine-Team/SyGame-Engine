/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/Facets/MeshQuadManifoldN.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Quad features : N-Manifold MeshGraphs
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// MeshQuadManifoldN implementation
inline MeshQuadType MeshQuadManifoldN::GetQuadType() const {
    return MESH_QUAD_MANIFOLDN;
}

//inline MeshFacet * MeshQuadManifoldN::Upgrade() {
//    return NULL;
//}

/////////////////////////////////////////////////////////////////////////////////
// MeshQuadManifoldN_F implementation
inline MeshLinkageType MeshQuadManifoldN_F::GetLinkageType() const {
    return MESH_LINKAGE_F;
}

_MESHFEATURE_IMPLEMENT_INLINES_DYNARRAY( MeshQuadManifoldN_F, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshQuadManifoldN_VF implementation
inline MeshLinkageType MeshQuadManifoldN_VF::GetLinkageType() const {
    return MESH_LINKAGE_VF;
}

_MESHFEATURE_IMPLEMENT_INLINES_4( MeshQuadManifoldN_VF, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_INLINES_DYNARRAY( MeshQuadManifoldN_VF, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshQuadManifoldN_EF implementation
inline MeshLinkageType MeshQuadManifoldN_EF::GetLinkageType() const {
    return MESH_LINKAGE_EF;
}

_MESHFEATURE_IMPLEMENT_INLINES_4( MeshQuadManifoldN_EF, Edge, Edges )
_MESHFEATURE_IMPLEMENT_INLINES_DYNARRAY( MeshQuadManifoldN_EF, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshQuadManifoldN_VEF implementation
inline MeshLinkageType MeshQuadManifoldN_VEF::GetLinkageType() const {
    return MESH_LINKAGE_VEF;
}

_MESHFEATURE_IMPLEMENT_INLINES_4( MeshQuadManifoldN_VEF, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_INLINES_4( MeshQuadManifoldN_VEF, Edge, Edges )
_MESHFEATURE_IMPLEMENT_INLINES_DYNARRAY( MeshQuadManifoldN_VEF, Facet, Facets )

