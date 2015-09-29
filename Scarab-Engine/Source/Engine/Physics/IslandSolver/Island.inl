/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/IslandSolver/Island.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : An Island represents a set of bodies in contact or close
//               proximity that must be integrated together.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Island implementation
inline Byte Island::GetColor() const {
    return m_iColor;
}
inline Void Island::SetColor( Byte iColor ) {
    m_iColor = iColor;
}

inline UInt Island::GetStackLayerCount() const {
    return m_iStackLayerCount;
}

inline UInt Island::GetBodyCount() const {
    return m_iBodyCount;
}
inline UInt Island::GetTotalNodeCount() const {
    return ( m_iBodyCount );
}

inline BodyNode * Island::GetBodyList() const {
    return m_pListBodies;
}

inline UInt Island::GetContactCount() const {
    return m_iContactCount;
}
inline UInt Island::GetJointCount() const {
    return m_iJointCount;
}
inline UInt Island::GetTotalEdgeCount() const {
    return ( m_iContactCount + m_iJointCount );
}

inline ContactEdge * Island::GetContactList() const {
    return m_pListContacts;
}
inline JointEdge * Island::GetJointList() const {
    return m_pListJoints;
}

inline Island * Island::GetNextInWorld() const {
    return m_pNextInWorld;
}
inline Island * Island::GetPrevInWorld() const {
    return m_pPrevInWorld;
}

/////////////////////////////////////////////////////////////////////////////////

inline Void Island::_SetStackLayerCount( UInt iStackLayerCount ) {
    m_iStackLayerCount = iStackLayerCount;
}

inline Void Island::_SetNextInWorld( Island * pIsland ) {
    m_pNextInWorld = pIsland;
}
inline Void Island::_SetPrevInWorld( Island * pIsland ) {
    m_pPrevInWorld = pIsland;
}

