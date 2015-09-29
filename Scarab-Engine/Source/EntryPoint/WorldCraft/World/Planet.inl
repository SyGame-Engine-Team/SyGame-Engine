/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/World/Planet.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World hierarchy : Planet (layered dynamic set of Regions)
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Planet implementation
inline Bool Planet::IsStar() const {
    return ( m_iOrbit == STAR_ORBIT_PRIMARY || m_iOrbit == STAR_ORBIT_BINARY );
}
inline Bool Planet::IsPrimaryStar() const {
    return ( m_iOrbit == STAR_ORBIT_PRIMARY );
}
inline Bool Planet::IsBinaryStar() const {
    return ( m_iOrbit == STAR_ORBIT_BINARY );
}

inline UInt Planet::GetOrbit() const {
    return m_iOrbit;
}

inline StarSystem * Planet::GetParentStarSystem() const {
    return m_pParentStarSystem;
}

inline UInt Planet::GetRegionCount( RegionLayer iRegionLayer ) const {
    switch( iRegionLayer ) {
        case REGION_LAYER_UNDERGROUND: return m_mapUndergroundRegions.Count();
        case REGION_LAYER_SURFACE:     return m_mapSurfaceRegions.Count();
        case REGION_LAYER_ATMOSPHERE:  return m_mapAtmosphereRegions.Count();
        default: Assert( false ); return 0;
    }
}

inline Void Planet::EnumRegions( RegionLayer iRegionLayer ) {
    switch( iRegionLayer ) {
        case REGION_LAYER_UNDERGROUND: m_itEnumerateRegion = m_mapUndergroundRegions.Begin(); break;
        case REGION_LAYER_SURFACE:     m_itEnumerateRegion = m_mapSurfaceRegions.Begin();     break;
        case REGION_LAYER_ATMOSPHERE:  m_itEnumerateRegion = m_mapAtmosphereRegions.Begin();  break;
        default: Assert( false ); break;
    }
}
inline Region * Planet::EnumNextRegion() {
    if ( m_itEnumerateRegion.IsNull() )
        return NULL;
    Region * pRegion = m_itEnumerateRegion.GetItem();
    ++m_itEnumerateRegion;
    return pRegion;
}

inline VoidSpace * Planet::GetOuterSpace() const {
    return m_pOuterSpace;
}

inline UInt Planet::GetMoonCount() const {
    return m_iMoonCount;
}
inline Bool Planet::HasMoon( MoonID iMoonID ) const {
    Assert( iMoonID < m_iMoonCount );
    return ( m_arrMoons[iMoonID] != NULL );
}

inline UInt Planet::GetMoonRegionCount( MoonID iMoonID ) const {
    Assert( iMoonID < m_iMoonCount );
    return m_arrMoons[iMoonID]->Count();
}

inline Void Planet::EnumMoonRegions( MoonID iMoonID ) {
    Assert( iMoonID < m_iMoonCount );
    m_itEnumerateMoonRegion = m_arrMoons[iMoonID]->Begin();
}
inline Region * Planet::EnumNextMoonRegion() {
    if ( m_itEnumerateMoonRegion.IsNull() )
        return NULL;
    Region * pRegion = m_itEnumerateMoonRegion.GetItem();
    ++m_itEnumerateMoonRegion;
    return pRegion;
}

/////////////////////////////////////////////////////////////////////////////////

inline Int Planet::_Compare_RegionCoords( const RegionCoords & rLeft, const RegionCoords & rRight, Void * /*pUserData*/ ) {
    if ( rLeft.Y < rRight.Y )
        return +1;
    if ( rLeft.Y > rRight.Y )
        return -1;
    if ( rLeft.X < rRight.X )
        return +1;
    if ( rLeft.X > rRight.X )
        return -1;
    return 0;
}

