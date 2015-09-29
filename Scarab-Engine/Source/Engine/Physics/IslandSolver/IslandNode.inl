/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/IslandSolver/IslandNode.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Abstraction for island graphs nodes
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// IslandNode implementation
inline Island * IslandNode::GetIsland() const {
    return m_pIsland;
}

inline UInt IslandNode::GetStackLayer() const {
    return m_iStackLayer;
}

inline Bool IslandNode::IsFixed() const {
    return m_bFixed;
}
inline Void IslandNode::SetFixed( Bool bFixed ) {
    m_bFixed = bFixed;
}

inline UInt IslandNode::GetContactCount() const {
    return m_iContactCount;
}
inline UInt IslandNode::GetJointCount() const {
    return m_iJointCount;
}
inline UInt IslandNode::GetTotalEdgeCount() const {
    return ( m_iContactCount + m_iJointCount );
}

inline ContactEdge * IslandNode::GetContactList() const {
    return m_pListContacts;
}
inline JointEdge * IslandNode::GetJointList() const {
    return m_pListJoints;
}

inline IslandNode * IslandNode::GetNextInWorld() const {
    return m_pNextInWorld;
}
inline IslandNode * IslandNode::GetPrevInWorld() const {
    return m_pPrevInWorld;
}

inline IslandNode * IslandNode::GetNextInIsland() const {
    return m_pNextInIsland;
}
inline IslandNode * IslandNode::GetPrevInIsland() const {
    return m_pPrevInIsland;
}

inline Void * IslandNode::GetUserData() const {
    return m_pUserData;
}
inline Void IslandNode::SetUserData( Void * pUserData ) {
    m_pUserData = pUserData;
}

/////////////////////////////////////////////////////////////////////////////////

inline Void IslandNode::_SetIsland( Island * pIsland ) {
    m_pIsland = pIsland;
}
inline Void IslandNode::_AddIslandRef() {
    ++m_iIslandRefCount;
}
inline Bool IslandNode::_RemoveIslandRef() {
    if ( m_iIslandRefCount == 0 )
        return true;
    --m_iIslandRefCount;
    return ( m_iIslandRefCount == 0 );
}

inline Void IslandNode::_SetStackLayer( UInt iStackLayer ) {
    m_iStackLayer = iStackLayer;
}

inline Void IslandNode::_SetNextInWorld( IslandNode * pNode ) {
    m_pNextInWorld = pNode;
}
inline Void IslandNode::_SetPrevInWorld( IslandNode * pNode ) {
    m_pPrevInWorld = pNode;
}

inline Void IslandNode::_SetNextInIsland( IslandNode * pNode ) {
    m_pNextInIsland = pNode;
}
inline Void IslandNode::_SetPrevInIsland( IslandNode * pNode ) {
    m_pPrevInIsland = pNode;
}

