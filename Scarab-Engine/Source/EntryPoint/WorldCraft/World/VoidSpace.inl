/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/World/VoidSpace.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World hierarchy : VoidSpace (dynamic set of Regions)
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// VoidSpace implementation
inline VoidSpaceType VoidSpace::GetType() const {
    return m_iType;
}
inline RegionLayer VoidSpace::GetRegionLayer() const {
    switch( m_iType ) {
        case VOIDSPACE_OUTER:    return REGION_LAYER_OUTERSPACE;
        case VOIDSPACE_FAR:      return REGION_LAYER_FARSPACE;
        case VOIDSPACE_DEEP:     return REGION_LAYER_DEEPSPACE;
        case VOIDSPACE_OBLIVION: return REGION_LAYER_OBLIVIONSPACE;
        default: Assert(false);  return REGION_LAYER_INVALID;
    }
}

inline Planet * VoidSpace::GetParentPlanet() const {
    Assert( m_iType == VOIDSPACE_OUTER );
    return (Planet*)m_pParent;
}
inline StarSystem * VoidSpace::GetParentStarSystem() const {
    Assert( m_iType == VOIDSPACE_FAR );
    return (StarSystem*)m_pParent;
}
inline Galaxy * VoidSpace::GetParentGalaxy() const {
    Assert( m_iType == VOIDSPACE_DEEP );
    return (Galaxy*)m_pParent;
}
inline Dimension * VoidSpace::GetParentDimension() const {
    Assert( m_iType == VOIDSPACE_OBLIVION );
    return (Dimension*)m_pParent;
}

inline UInt VoidSpace::GetRegionCount() const {
    return m_mapVoidRegions.Count();
}
inline Region * VoidSpace::GetRegion( const RegionCoords & vRegionCoords ) const {
    if ( m_mapVoidRegions.Contains(vRegionCoords) )
        return m_mapVoidRegions[vRegionCoords];
    return NULL;
}

inline Void VoidSpace::EnumRegions() {
    m_itEnumerateRegion = m_mapVoidRegions.Begin();
}
inline Region * VoidSpace::EnumNextRegion() {
    if ( m_itEnumerateRegion.IsNull() )
        return NULL;
    Region * pRegion = m_itEnumerateRegion.GetItem();
    ++m_itEnumerateRegion;
    return pRegion;
}

/////////////////////////////////////////////////////////////////////////////////

inline Int VoidSpace::_Compare_RegionCoords( const RegionCoords & rLeft, const RegionCoords & rRight, Void * /*pUserData*/ ) {
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

