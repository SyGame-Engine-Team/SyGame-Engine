/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/World/Galaxy.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World hierarchy : Galaxy (dynamic set of StarSystems)
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Galaxy implementation
inline GalaxyID Galaxy::GetID() const {
    return m_iGalaxyID;
}

inline Dimension * Galaxy::GetParentDimension() const {
    return m_pParentDimension;
}

inline UInt Galaxy::GetTunnelCount() const {
    return m_mapTunnels.Count();
}
inline Galaxy * Galaxy::GetTunnel( GalaxyID iGalaxyID ) const {
    if ( m_mapTunnels.Contains(iGalaxyID) )
        return m_mapTunnels[iGalaxyID];
    return NULL;
}

inline Void Galaxy::EnumTunnels() {
    m_itEnumerateTunnel = m_mapTunnels.Begin();
}
inline Galaxy * Galaxy::EnumNextTunnel() {
    if ( m_itEnumerateTunnel.IsNull() )
        return NULL;
    Galaxy * pGalaxy = m_itEnumerateTunnel.GetItem();
    ++m_itEnumerateTunnel;
    return pGalaxy;
}

inline UInt Galaxy::GetStarSystemCount() const {
    return m_mapStarSystems.Count();
}
inline StarSystem * Galaxy::GetStarSystem( StarSystemID iStarSystemID ) const {
    if ( m_mapStarSystems.Contains(iStarSystemID) )
        return m_mapStarSystems[iStarSystemID];
    return NULL;
}

inline Void Galaxy::EnumStarSystems() {
    m_itEnumerateStarSystem = m_mapStarSystems.Begin();
}
inline StarSystem * Galaxy::EnumNextStarSystem() {
    if ( m_itEnumerateStarSystem.IsNull() )
        return NULL;
    StarSystem * pStarSystem = m_itEnumerateStarSystem.GetItem();
    ++m_itEnumerateStarSystem;
    return pStarSystem;
}

inline VoidSpace * Galaxy::GetDeepSpace() const {
    return m_pDeepSpace;
}

/////////////////////////////////////////////////////////////////////////////////

inline Int Galaxy::_Compare_GalaxyID( const GalaxyID & rLeft, const GalaxyID & rRight, Void * /*pUserData*/ ) {
    if ( rLeft < rRight )
        return +1;
    if ( rLeft > rRight )
        return -1;
    return 0;
}

inline Int Galaxy::_Compare_StarSystemID( const StarSystemID & rLeft, const StarSystemID & rRight, Void * /*pUserData*/ ) {
    if ( rLeft < rRight )
        return +1;
    if ( rLeft > rRight )
        return -1;
    return 0;
}
