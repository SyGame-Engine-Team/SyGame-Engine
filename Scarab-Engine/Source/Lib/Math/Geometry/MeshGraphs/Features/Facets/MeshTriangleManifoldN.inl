/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/Facets/MeshTriangleManifoldN.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Triangle features : N-Manifold MeshGraphs
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// MeshTriangleManifoldN implementation
inline MeshTriangleType MeshTriangleManifoldN::GetTriangleType() const {
    return MESH_TRIANGE_MANIFOLDN;
}

//inline MeshFacet * MeshTriangleManifoldN::Upgrade() {
//    return NULL;
//}

/////////////////////////////////////////////////////////////////////////////////
// MeshTriangleManifoldN_F implementation
inline MeshLinkageType MeshTriangleManifoldN_F::GetLinkageType() const {
    return MESH_LINKAGE_F;
}

_MESHFEATURE_IMPLEMENT_INLINES_DYNARRAY( MeshTriangleManifoldN_F, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshTriangleManifoldN_VF implementation
inline MeshLinkageType MeshTriangleManifoldN_VF::GetLinkageType() const {
    return MESH_LINKAGE_VF;
}

_MESHFEATURE_IMPLEMENT_INLINES_3( MeshTriangleManifoldN_VF, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_INLINES_DYNARRAY( MeshTriangleManifoldN_VF, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshTriangleManifoldN_EF implementation
inline MeshLinkageType MeshTriangleManifoldN_EF::GetLinkageType() const {
    return MESH_LINKAGE_EF;
}

_MESHFEATURE_IMPLEMENT_INLINES_3( MeshTriangleManifoldN_EF, Edge, Edges )
_MESHFEATURE_IMPLEMENT_INLINES_DYNARRAY( MeshTriangleManifoldN_EF, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshTriangleManifoldN_VEF implementation
inline MeshLinkageType MeshTriangleManifoldN_VEF::GetLinkageType() const {
    return MESH_LINKAGE_VEF;
}

_MESHFEATURE_IMPLEMENT_INLINES_3( MeshTriangleManifoldN_VEF, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_INLINES_3( MeshTriangleManifoldN_VEF, Edge, Edges )
_MESHFEATURE_IMPLEMENT_INLINES_DYNARRAY( MeshTriangleManifoldN_VEF, Facet, Facets )

