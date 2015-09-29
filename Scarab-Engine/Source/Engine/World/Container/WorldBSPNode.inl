/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/Container/WorldBSPNode.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Container : BSP nodes
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : none
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// WorldBSPNode implementation
inline WorldNodeType WorldBSPNode::GetType() const {
    return WORLDNODE_BSP;
}

inline Bool WorldBSPNode::HasPositiveChild() const {
    return ( m_pPositive != NULL );
}
inline Bool WorldBSPNode::HasCoplanarChild() const {
    return ( m_pCoplanar != NULL );
}
inline Bool WorldBSPNode::HasNegativeChild() const {
    return ( m_pNegative != NULL );
}

inline WorldBaseNode * WorldBSPNode::GetPositiveChild() const {
    return m_pPositive;
}
inline WorldBaseNode * WorldBSPNode::GetCoplanarChild() const {
    return m_pCoplanar;
}
inline WorldBaseNode * WorldBSPNode::GetNegativeChild() const {
    return m_pNegative;
}

inline WorldBaseNode * WorldBSPNode::SetPositiveChild( WorldBaseNode * pChild ) {
    WorldBaseNode * pOldNode = m_pPositive;
    if ( pOldNode != NULL )
        RemoveChild( pOldNode->GetName() );
    m_pPositive = pChild;
    if ( m_pPositive != NULL )
        AddChild( m_pPositive );
    return pOldNode;
}
inline WorldBaseNode * WorldBSPNode::SetCoplanarChild( WorldBaseNode * pChild ) {
    WorldBaseNode * pOldNode = m_pCoplanar;
    if ( pOldNode != NULL )
        RemoveChild( pOldNode->GetName() );
    m_pCoplanar = pChild;
    if ( m_pCoplanar != NULL )
        AddChild( m_pCoplanar );
    return pOldNode;
}
inline WorldBaseNode * WorldBSPNode::SetNegativeChild( WorldBaseNode * pChild ) {
    WorldBaseNode * pOldNode = m_pNegative;
    if ( pOldNode != NULL )
        RemoveChild( pOldNode->GetName() );
    m_pNegative = pChild;
    if ( m_pNegative != NULL )
        AddChild( m_pNegative );
    return pOldNode;
}

