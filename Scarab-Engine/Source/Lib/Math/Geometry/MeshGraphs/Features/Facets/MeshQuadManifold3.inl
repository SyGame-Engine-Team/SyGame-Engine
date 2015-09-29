/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/MeshGraphs/Features/Facets/MeshQuadManifold3.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Quad features : 3-Manifold MeshGraphs
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// MeshQuadManifold3 implementation
inline MeshQuadType MeshQuadManifold3::GetQuadType() const {
    return MESH_QUAD_MANIFOLD3;
}

//inline MeshFacet * MeshQuadManifold3::Downgrade() {
//    return NULL;
//}

/////////////////////////////////////////////////////////////////////////////////
// MeshQuadManifold3_V implementation
inline MeshLinkageType MeshQuadManifold3_V::GetLinkageType() const {
    return MESH_LINKAGE_V;
}

_MESHFEATURE_IMPLEMENT_INLINES_4( MeshQuadManifold3_V, Vertex, Vertices )

/////////////////////////////////////////////////////////////////////////////////
// MeshQuadManifold3_E implementation
inline MeshLinkageType MeshQuadManifold3_E::GetLinkageType() const {
    return MESH_LINKAGE_E;
}

_MESHFEATURE_IMPLEMENT_INLINES_4( MeshQuadManifold3_E, Edge, Edges )

/////////////////////////////////////////////////////////////////////////////////
// MeshQuadManifold3_F implementation
inline MeshLinkageType MeshQuadManifold3_F::GetLinkageType() const {
    return MESH_LINKAGE_F;
}

_MESHFEATURE_IMPLEMENT_INLINES_4( MeshQuadManifold3_F, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshQuadManifold3_VE implementation
inline MeshLinkageType MeshQuadManifold3_VE::GetLinkageType() const {
    return MESH_LINKAGE_VE;
}

_MESHFEATURE_IMPLEMENT_INLINES_4( MeshQuadManifold3_VE, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_INLINES_4( MeshQuadManifold3_VE, Edge, Edges )

/////////////////////////////////////////////////////////////////////////////////
// MeshQuadManifold3_VF implementation
inline MeshLinkageType MeshQuadManifold3_VF::GetLinkageType() const {
    return MESH_LINKAGE_VF;
}

_MESHFEATURE_IMPLEMENT_INLINES_4( MeshQuadManifold3_VF, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_INLINES_4( MeshQuadManifold3_VF, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshQuadManifold3_EF implementation
inline MeshLinkageType MeshQuadManifold3_EF::GetLinkageType() const {
    return MESH_LINKAGE_EF;
}

_MESHFEATURE_IMPLEMENT_INLINES_4( MeshQuadManifold3_EF, Edge, Edges )
_MESHFEATURE_IMPLEMENT_INLINES_4( MeshQuadManifold3_EF, Facet, Facets )

/////////////////////////////////////////////////////////////////////////////////
// MeshQuadManifold3_VEF implementation
inline MeshLinkageType MeshQuadManifold3_VEF::GetLinkageType() const {
    return MESH_LINKAGE_VEF;
}

_MESHFEATURE_IMPLEMENT_INLINES_4( MeshQuadManifold3_VEF, Vertex, Vertices )
_MESHFEATURE_IMPLEMENT_INLINES_4( MeshQuadManifold3_VEF, Edge, Edges )
_MESHFEATURE_IMPLEMENT_INLINES_4( MeshQuadManifold3_VEF, Facet, Facets )

