/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/World/Dimension.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World hierarchy : Dimension (dynamic set of Galaxies)
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Dimension implementation
inline DimensionID Dimension::GetID() const {
    return m_iDimensionID;
}

inline UInt Dimension::GetTunnelCount() const {
    return m_mapTunnels.Count();
}
inline Dimension * Dimension::GetTunnel( DimensionID iDimensionID ) const {
    if ( m_mapTunnels.Contains(iDimensionID) )
        return m_mapTunnels[iDimensionID];
    return NULL;
}

inline Void Dimension::EnumTunnels() {
    m_itEnumerateTunnel = m_mapTunnels.Begin();
}
inline Dimension * Dimension::EnumNextTunnel() {
    if ( m_itEnumerateTunnel.IsNull() )
        return NULL;
    Dimension * pDimension = m_itEnumerateTunnel.GetItem();
    ++m_itEnumerateTunnel;
    return pDimension;
}

inline UInt Dimension::GetGalaxyCount() const {
    return m_mapGalaxies.Count();
}
inline Galaxy * Dimension::GetGalaxy( GalaxyID iGalaxyID ) const {
    if ( m_mapGalaxies.Contains(iGalaxyID) )
        return m_mapGalaxies[iGalaxyID];
    return NULL;
}

inline Void Dimension::EnumGalaxies() {
    m_itEnumerateGalaxy = m_mapGalaxies.Begin();
}
inline Galaxy * Dimension::EnumNextGalaxy() {
    if ( m_itEnumerateGalaxy.IsNull() )
        return NULL;
    Galaxy * pGalaxy = m_itEnumerateGalaxy.GetItem();
    ++m_itEnumerateGalaxy;
    return pGalaxy;
}

inline VoidSpace * Dimension::GetOblivionSpace() const {
    return m_pOblivionSpace;
}

/////////////////////////////////////////////////////////////////////////////////

inline Int Dimension::_Compare_DimensionID( const DimensionID & rLeft, const DimensionID & rRight, Void * /*pUserData*/ ) {
    if ( rLeft < rRight )
        return +1;
    if ( rLeft > rRight )
        return -1;
    return 0;
}

inline Int Dimension::_Compare_GalaxyID( const GalaxyID & rLeft, const GalaxyID & rRight, Void * /*pUserData*/ ) {
    if ( rLeft < rRight )
        return +1;
    if ( rLeft > rRight )
        return -1;
    return 0;
}
