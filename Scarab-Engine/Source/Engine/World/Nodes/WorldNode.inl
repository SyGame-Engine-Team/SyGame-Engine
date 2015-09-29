/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/Nodes/WorldNode.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Node : Internal nodes
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// WorldNode implementation
inline WorldNodeType WorldNode::GetType() const {
    return WORLDNODE_NODE;
}

inline UInt WorldNode::GetChildCount() const {
    return m_mapChildren.Count();
}
inline Bool WorldNode::HasChild( const GChar * strName ) const {
    return m_mapChildren.Contains(strName);
}
inline WorldBaseNode * WorldNode::GetChild( const GChar * strName ) const {
    ChildMap::Iterator itChild = m_mapChildren.Get(strName);
    if ( itChild.IsNull() )
        return NULL;
    return itChild.GetItem();
}

inline Void WorldNode::EnumChildren() const {
    m_itEnumerate = m_mapChildren.Begin();
}
inline WorldBaseNode * WorldNode::EnumNextChild() const {
    if ( m_itEnumerate.IsNull() )
        return NULL;
    WorldBaseNode * pNode = m_itEnumerate.GetItem();
    ++m_itEnumerate;
    return pNode;
}

/////////////////////////////////////////////////////////////////////////////////

inline Int WorldNode::_Name_Comparator( const GChar* const & rLeft, const GChar* const & rRight, Void * /*pUserData*/ ) {
    return StringFn->NCmp( rLeft, rRight, WORLDGRAPH_NAME_LENGTH - 1 );
}

