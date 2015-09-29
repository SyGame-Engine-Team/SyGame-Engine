/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Shape/Concave/ShapeDeformable.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Shapes : Concave, Deformable
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// ShapeDeformable::Node implementation
inline Sphere ShapeDeformable::Node::ComputeShape() const {
    return Sphere( Position, SHAPECONCAVE_NODE_RADIUS );
}
inline UInt ShapeDeformable::Node::GetIndex() const {
    return Key.V;
}

/////////////////////////////////////////////////////////////////////////////////
// ShapeDeformable::Link implementation
inline ShapeDeformable::Node * ShapeDeformable::Link::GetNode( UInt iNode ) const {
    Assert( iNode < 2 );
    return (Node*)( Vertices[iNode] );
}

/////////////////////////////////////////////////////////////////////////////////
// ShapeDeformable::Facet implementation
inline Triangle3 ShapeDeformable::Facet::ComputeShape() const {
    return Triangle3( GetNode(0)->Position, GetNode(1)->Position, GetNode(2)->Position );
}
inline ShapeDeformable::Node * ShapeDeformable::Facet::GetNode( UInt iNode ) const {
    Assert( iNode < 3 );
    return (Node*)( Vertices[iNode] );
}

/////////////////////////////////////////////////////////////////////////////////
// ShapeDeformable implementation
inline MeshVertex * ShapeDeformable::DefaultNodeBuilder( const MeshVertexKey & vertexKey ) {
    return New Node( vertexKey );
}
inline MeshEdge * ShapeDeformable::DefaultLinkBuilder( const MeshEdgeKey & edgeKey ) {
    return New Link( edgeKey );
}
inline MeshFacet * ShapeDeformable::DefaultFacetBuilder( const MeshFacetKey & facetKey ) {
    Assert( facetKey.GetFacetType() == MESH_FACET_TRIANGLE );
    return New Facet( (const MeshTriangleKey &)facetKey );
}

inline ShapeType ShapeDeformable::GetType() const {
    return SHAPE_CONCAVE_DEFORMABLE;
}

inline UInt ShapeDeformable::GetNodeCount() const {
    return m_pGraph->GetVertexCount();
}
inline Void ShapeDeformable::EnumNodes() const {
    m_pGraph->EnumVertices();
}
inline ShapeDeformable::Node * ShapeDeformable::EnumNextNode() const {
    return (Node*)( m_pGraph->EnumNextVertex() );
}
inline ShapeDeformable::Node * ShapeDeformable::GetNode( UInt iNode ) const {
    Assert( iNode < m_pGraph->GetVertexCount() );
    return (Node*)( m_pGraph->GetVertex(MeshVertexKey(iNode)) );
}

inline UInt ShapeDeformable::GetLinkCount() const {
    return m_pGraph->GetEdgeCount();
}
inline Void ShapeDeformable::EnumLinks() const {
    m_pGraph->EnumEdges();
}
inline ShapeDeformable::Link * ShapeDeformable::EnumNextLink() const {
    return (Link*)( m_pGraph->EnumNextEdge() );
}

inline UInt ShapeDeformable::GetFacetCount() const {
    return m_pGraph->GetFacetCount();
}
inline Void ShapeDeformable::EnumFacets() const {
    m_pGraph->EnumFacets();
}
inline ShapeDeformable::Facet * ShapeDeformable::EnumNextFacet() const {
    return (Facet*)( m_pGraph->EnumNextFacet() );
}

inline MeshDeformableGraph * ShapeDeformable::GetGraph() const {
    return m_pGraph;
}

inline const AABoxTree * ShapeDeformable::GetNodeTree() const {
    return m_pNodeTree;
}
inline const AABoxTree * ShapeDeformable::GetFacetTree() const {
    return m_pFacetTree;
}

inline Void ShapeDeformable::QueryNodes( const BVAABox & bvAABox, HNodeQuery * pQuery ) const {
    m_pNodeTree->CollideAABox( bvAABox, pQuery );
}

inline Void ShapeDeformable::QueryTriangles( const BVAABox & bvAABox, HTriangleQuery * pQuery ) const {
    m_pFacetTree->CollideAABox( bvAABox, pQuery );
}

inline Void ShapeDeformable::ToggleSelfCollisions( Bool bEnable ) {
    m_bUseSelfCollisions = bEnable;
}

inline UInt ShapeDeformable::GetSelfContactCount() const {
    if ( m_bUseSelfCollisions )
        return m_hSelfCollider.arrSelfContacts.Count();
    return 0;
}
inline const ContactPoint * ShapeDeformable::GetSelfContact( UInt iSelfContact ) const {
    if ( m_bUseSelfCollisions ) {
        Assert( iSelfContact < m_hSelfCollider.arrSelfContacts.Count() );
        return &(m_hSelfCollider.arrSelfContacts[iSelfContact] );
    }
    return NULL;
}

