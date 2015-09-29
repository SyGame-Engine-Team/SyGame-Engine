/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Bodies/SoftBody.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Body : Soft bodies
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// SoftBody::Node implementation

/////////////////////////////////////////////////////////////////////////////////
// SoftBody::Link implementation
inline SoftBody::Node * SoftBody::Link::GetNode( UInt iNode ) const {
    return (SoftBody::Node*)( ShapeDeformable::Link::GetNode(iNode) );
}

/////////////////////////////////////////////////////////////////////////////////
// SoftBody::Facet implementation
inline SoftBody::Node * SoftBody::Facet::GetNode( UInt iNode ) const {
    return (SoftBody::Node*)( ShapeDeformable::Facet::GetNode(iNode) );
}

inline Vertex3 SoftBody::Facet::BaryEval( const Vector3 & vWeights, Bool bUseOldPositions ) const {
    SoftBody::Node * pA = GetNode(0);
    SoftBody::Node * pB = GetNode(1);
    SoftBody::Node * pC = GetNode(2);
    Vertex3 vA;
    Vector3 vB, vC;
    if ( bUseOldPositions ) {
        vA = pA->OldPosition;
        vB = pB->OldPosition.ToVector();
        vC = pC->OldPosition.ToVector();
    } else {
        vA = pA->Position;
        vB = pB->Position.ToVector();
        vC = pC->Position.ToVector();
    }
    return ( (vA * vWeights.X) + (vB * vWeights.Y) + (vC * vWeights.Z) );
}

/////////////////////////////////////////////////////////////////////////////////
// SoftBody implementation
inline MeshVertex * SoftBody::NodeBuilder( const MeshVertexKey & vertexKey ) {
    return New Node( vertexKey );
}
inline MeshEdge * SoftBody::LinkBuilder( const MeshEdgeKey & edgeKey ) {
    return New Link( edgeKey );
}
inline MeshFacet * SoftBody::FacetBuilder( const MeshFacetKey & facetKey ) {
    Assert( facetKey.GetFacetType() == MESH_FACET_TRIANGLE );
    return New Facet( (const MeshTriangleKey &)facetKey );
}

inline BodyType SoftBody::GetBodyType() const {
    return BODY_TYPE_SOFT;
}

inline Bool SoftBody::IsInsulator() const {
    return false; // Even when static !
}

inline SoftBodyEnvironment * SoftBody::GetEnvironment() const {
    return m_pEnvironment;
}

inline UInt SoftBody::GetNodeCount() const {
    return m_pDeformableShape->GetNodeCount();
}
inline Void SoftBody::EnumNodes() const {
    m_pDeformableShape->EnumNodes();
}
inline SoftBody::Node * SoftBody::EnumNextNode() const {
    return (SoftBody::Node*)( m_pDeformableShape->EnumNextNode() );
}
inline SoftBody::Node * SoftBody::GetNode( UInt iNode ) const {
    return (SoftBody::Node*)( m_pDeformableShape->GetNode(iNode) );
}

inline UInt SoftBody::GetLinkCount() const {
    return m_pDeformableShape->GetLinkCount();
}
inline Void SoftBody::EnumLinks() const {
    m_pDeformableShape->EnumLinks();
}
inline SoftBody::Link * SoftBody::EnumNextLink() const {
    return (SoftBody::Link*)( m_pDeformableShape->EnumNextLink() );
}

inline UInt SoftBody::GetFacetCount() const {
    return m_pDeformableShape->GetFacetCount();
}
inline Void SoftBody::EnumFacets() const {
    m_pDeformableShape->EnumFacets();
}
inline SoftBody::Facet * SoftBody::EnumNextFacet() const {
    return (SoftBody::Facet*)( m_pDeformableShape->EnumNextFacet() );
}


