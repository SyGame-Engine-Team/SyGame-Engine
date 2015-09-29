/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/World/Region.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World hierarchy : Region (16*16 Areas)
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Region implementation
inline RegionLayer Region::GetLayer( MoonID * outMoonID ) const {
    if ( outMoonID != NULL ) {
        *outMoonID = INVALID_OFFSET;
        if ( m_iLayer == REGION_LAYER_MOONS )
            *outMoonID = m_iMoonID;
    }
    return m_iLayer;
}
inline const RegionCoords & Region::GetRegionCoords() const {
    return m_vRegionCoords;
}

inline Planet * Region::GetParentPlanet() const {
    Assert( m_iLayer <= REGION_LAYER_ATMOSPHERE || m_iLayer == REGION_LAYER_MOONS );
    return m_pParentPlanet;
}
inline VoidSpace * Region::GetParentVoidSpace() const {
    Assert( m_iLayer == REGION_LAYER_OUTERSPACE || m_iLayer >= REGION_LAYER_FARSPACE );
    return m_pParentVoidSpace;
}

inline UInt Region::GetAreaCount() const {
    return m_iAreaCount;
}
inline Area * Region::GetArea( const AreaCoords & vAreaCoords ) const {
    Assert( vAreaCoords.X >= 0 && vAreaCoords.X < REGION_SIZE );
    Assert( vAreaCoords.Y >= 0 && vAreaCoords.Y < REGION_SIZE );
    return m_arrAreas[ ( vAreaCoords.Y * REGION_SIZE ) + vAreaCoords.X ];
}


