/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/World/StarSystem.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World hierarchy : StarSystem (Up to 16 orbits for Planets)
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// StarSystem implementation
inline StarSystemID StarSystem::GetID() const {
    return m_iStarSystemID;
}

inline Galaxy * StarSystem::GetParentGalaxy() const {
    return m_pParentGalaxy;
}

inline UInt StarSystem::GetTunnelCount() const {
    return m_mapTunnels.Count();
}
inline StarSystem * StarSystem::GetTunnel( StarSystemID iStarSystemID ) const {
    if ( m_mapTunnels.Contains(iStarSystemID) )
        return m_mapTunnels[iStarSystemID];
    return NULL;
}

inline Void StarSystem::EnumTunnels() {
    m_itEnumerateTunnel = m_mapTunnels.Begin();
}
inline StarSystem * StarSystem::EnumNextTunnel() {
    if ( m_itEnumerateTunnel.IsNull() )
        return NULL;
    StarSystem * pStarSystem = m_itEnumerateTunnel.GetItem();
    ++m_itEnumerateTunnel;
    return pStarSystem;
}

inline Planet * StarSystem::GetPrimaryStar() const {
    return m_pPrimaryStar;
}
inline Planet * StarSystem::GetBinaryStar() const {
    return m_pBinaryStar;
}

inline UInt StarSystem::GetOrbitCount() const {
    return m_iOrbitCount;
}
inline Planet * StarSystem::GetPlanet( UInt iOrbit ) const {
    Assert( iOrbit < m_iOrbitCount );
    return m_arrPlanets[iOrbit];
}

inline VoidSpace * StarSystem::GetFarSpace() const {
    return m_pFarSpace;
}

/////////////////////////////////////////////////////////////////////////////////

inline Int StarSystem::_Compare_StarSystemID( const StarSystemID & rLeft, const StarSystemID & rRight, Void * /*pUserData*/ ) {
    if ( rLeft < rRight )
        return +1;
    if ( rLeft > rRight )
        return -1;
    return 0;
}

