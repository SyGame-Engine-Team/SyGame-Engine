/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/Facets/MeshTriangleManifold3.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Triangle features : 3-Manifold MeshGraphs
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// MeshTriangleManifold3 implementation
inline MeshTriangleType MeshTriangleManifold3::GetTriangleType() const {
    return MESH_TRIANGE_MANIFOLD3;
}

//inline MeshFacet * MeshTriangleManifold3::Downgrade() {
//    return NULL;
//}

/////////////////////////////////////////////////////////////////////////////////
// MeshTriangleManifold3_V implementation
inline MeshLinkageType MeshTriangleManifold3_V::GetLinkageType() const {
    return MESH_LINKAGE_V;
}

_MESHFEATURE_IMPLEMENT_INLINES_3( MeshTriangleManifold3_V, Vertex, Vertices )

/////////////////////////////////////////////////////////////////////////////////
// MeshTriangleManifold3_E implementation
inline MeshLinkageType MeshTriangleManifold3_E::GetLinkageType() const {
    return MESH_LINKAGE_E;
}

_MESHFEATURE_IMPLEMENT_INLINES_3( MeshTriangleManifold3_E, Edge, Edges )

/////////////////////////////////////////////////////////////////////////////////
// MeshTriangleManifold3_F implementation
inline MeshLinkageType MeshTriangleManifold3_F::GetLinkageType() const {
    return MESH_LINKAGE_F;
}

_MESHFEATURE_IMPLEMENT_INLINES_3( MeshTriangleManifold3_F, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshTriangleManifold3_VE implementation
inline MeshLinkageType MeshTriangleManifold3_VE::GetLinkageType() const {
    return MESH_LINKAGE_VE;
}

_MESHFEATURE_IMPLEMENT_INLINES_3( MeshTriangleManifold3_VE, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_INLINES_3( MeshTriangleManifold3_VE, Edge, Edges )

/////////////////////////////////////////////////////////////////////////////////
// MeshTriangleManifold3_VF implementation
inline MeshLinkageType MeshTriangleManifold3_VF::GetLinkageType() const {
    return MESH_LINKAGE_VF;
}

_MESHFEATURE_IMPLEMENT_INLINES_3( MeshTriangleManifold3_VF, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_INLINES_3( MeshTriangleManifold3_VF, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshTriangleManifold3_EF implementation
inline MeshLinkageType MeshTriangleManifold3_EF::GetLinkageType() const {
    return MESH_LINKAGE_EF;
}

_MESHFEATURE_IMPLEMENT_INLINES_3( MeshTriangleManifold3_EF, Edge, Edges )
_MESHFEATURE_IMPLEMENT_INLINES_3( MeshTriangleManifold3_EF, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshTriangleManifold3_VEF implementation
inline MeshLinkageType MeshTriangleManifold3_VEF::GetLinkageType() const {
    return MESH_LINKAGE_VEF;
}

_MESHFEATURE_IMPLEMENT_INLINES_3( MeshTriangleManifold3_VEF, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_INLINES_3( MeshTriangleManifold3_VEF, Edge, Edges )
_MESHFEATURE_IMPLEMENT_INLINES_3( MeshTriangleManifold3_VEF, Facet, Facets )

