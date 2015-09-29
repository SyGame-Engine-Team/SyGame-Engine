/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/Indoor/WorldPortalCuller.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Adds unicity to VisibleGroups computed for portal systems
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : none
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// WorldPortalCuller implementation
inline UInt WorldPortalCuller::GetCount() const {
    return m_iCount;
}

inline Void WorldPortalCuller::EnumVisibles() const {
    m_itEnumerate = m_mapUniqueLeaves.Begin();
}
inline WorldLeaf * WorldPortalCuller::EnumNextVisible() const {
    if ( m_itEnumerate.IsNull() )
        return NULL;
    WorldLeaf * pVisibleLeaf = m_itEnumerate.GetKey();
    ++m_itEnumerate;
    return pVisibleLeaf;
}

inline Void WorldPortalCuller::Clear() {
    m_iCount = 0;
    m_mapUniqueLeaves.Clear();
}

/////////////////////////////////////////////////////////////////////////////////

inline Int WorldPortalCuller::_WorldLeafPtr_Compare( WorldLeaf * const & rLeft, WorldLeaf * const & rRight, Void * /*pUserData*/ ) {
    if ( rLeft < rRight )
        return +1;
    if ( rRight < rLeft )
        return -1;
    return 0;
}

