/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/World/BlockWorld.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World base interface
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// UIDs implementation
inline RegionUID & RegionUID::operator=( const RegionUID & rhs ) {
    iDimendionID = rhs.iDimendionID;
    iGalaxyID = rhs.iGalaxyID;
    iStarSystemID = rhs.iStarSystemID;
    iOrbit = rhs.iOrbit;
    iRegionLayer = rhs.iRegionLayer;
    iMoonID = rhs.iMoonID;
    vRegionCoords = rhs.vRegionCoords;
    return (*this);
}

inline Bool RegionUID::operator==( const RegionUID & rhs ) const {
    return (
        ( iDimendionID == rhs.iDimendionID ) && ( iGalaxyID == rhs.iGalaxyID ) && ( iStarSystemID == rhs.iStarSystemID ) &&
        ( iOrbit == rhs.iOrbit ) && ( iRegionLayer == rhs.iRegionLayer ) && ( iMoonID == rhs.iMoonID ) && ( vRegionCoords == rhs.vRegionCoords )
    );
}
inline Bool RegionUID::operator!=( const RegionUID & rhs ) const {
    return (
        ( iDimendionID != rhs.iDimendionID ) || ( iGalaxyID != rhs.iGalaxyID ) || ( iStarSystemID != rhs.iStarSystemID ) ||
        ( iOrbit != rhs.iOrbit ) || ( iRegionLayer != rhs.iRegionLayer ) || ( iMoonID != rhs.iMoonID ) || ( vRegionCoords != rhs.vRegionCoords )
    );
}

inline AreaUID & AreaUID::operator=( const AreaUID & rhs ) {
    hRegionUID = rhs.hRegionUID;
    vAreaCoords = rhs.vAreaCoords;
    return (*this);
}

inline Bool AreaUID::operator==( const AreaUID & rhs ) const {
    return ( ( hRegionUID == rhs.hRegionUID ) && ( vAreaCoords == rhs.vAreaCoords ) );
}
inline Bool AreaUID::operator!=( const AreaUID & rhs ) const {
    return ( ( hRegionUID != rhs.hRegionUID ) || ( vAreaCoords != rhs.vAreaCoords ) );
}

inline BlockSpaceUID & BlockSpaceUID::operator=( const BlockSpaceUID & rhs ) {
    hAreaUID = rhs.hAreaUID;
    vBlockSpaceCoords = rhs.vBlockSpaceCoords;
    return (*this);
}

inline Bool BlockSpaceUID::operator==( const BlockSpaceUID & rhs ) const {
    return ( ( hAreaUID == rhs.hAreaUID ) && ( vBlockSpaceCoords == rhs.vBlockSpaceCoords ) );
}
inline Bool BlockSpaceUID::operator!=( const BlockSpaceUID & rhs ) const {
    return ( ( hAreaUID != rhs.hAreaUID ) || ( vBlockSpaceCoords != rhs.vBlockSpaceCoords ) );
}

/////////////////////////////////////////////////////////////////////////////////
// BlockWorldManager implementation
inline const GChar * BlockWorldManager::GetName() {
    return TEXT("BlockWorldManager");
}
inline const GChar * BlockWorldManager::GetMemoryBudget() {
    return TEXT (
"Heap:Scratch(4194304);Heap:BlockInstances(4194304);Heap:BlockSpaces(4194304);Heap:Areas(4194304);Heap:Regions(4194304);\
Heap:Planets(4194304);Heap:StarSystems(4194304);Heap:Galaxies(4194304);Heap:Dimensions(4194304);Heap:VoidSpaces(4194304);"
    );
}

inline UInt BlockWorldManager::GetDimensionCount() const {
    return m_mapDimensions.Count();
}
inline Dimension * BlockWorldManager::GetDimension( DimensionID iDimensionID ) const {
    if ( m_mapDimensions.Contains(iDimensionID) )
        return m_mapDimensions[iDimensionID];
    return NULL;
}

inline Void BlockWorldManager::EnumDimensions() {
    m_itEnumerateDimension = m_mapDimensions.Begin();
}
inline Dimension * BlockWorldManager::EnumNextDimension() {
    if ( m_itEnumerateDimension.IsNull() )
        return NULL;
    Dimension * pDimension = m_itEnumerateDimension.GetItem();
    ++m_itEnumerateDimension;
    return pDimension;
}

inline Area * BlockWorldManager::GetArea( const AreaUID & hAreaUID ) const {
    Region * pRegion = GetRegion( hAreaUID.hRegionUID );
    return pRegion->GetArea( hAreaUID.vAreaCoords );
}
inline BlockSpace * BlockWorldManager::GetBlockSpace( const BlockSpaceUID & hBlockSpaceUID ) const {
    Area * pArea = GetArea( hBlockSpaceUID.hAreaUID );
    return pArea->GetBlockSpace( hBlockSpaceUID.vBlockSpaceCoords );
}

inline Area * BlockWorldManager::CreateArea( const AreaUID & hAreaUID ) {
    Region * pRegion = GetRegion( hAreaUID.hRegionUID );
    return pRegion->CreateArea( hAreaUID.vAreaCoords );
}

inline Void BlockWorldManager::SetWorldName( const GChar * strName ) {
    StringFn->NCopy( m_strWorldName, strName, FILENAME_LENGTH );
}

inline Bool BlockWorldManager::NeedSynchonization( const AreaUID & hAreaUID ) const {
    if ( m_mapDirtyRegions.Contains(hAreaUID.hRegionUID) ) {
        DirtyAreaMap * pDirtyAreas = m_mapDirtyRegions[hAreaUID.hRegionUID];
        return pDirtyAreas->Contains( hAreaUID.vAreaCoords );
    }
    return false;
}
inline Bool BlockWorldManager::NeedSynchonization( const RegionUID & hRegionUID ) const {
    return m_mapDirtyRegions.Contains( hRegionUID );
}
inline Bool BlockWorldManager::NeedSynchonization() const {
    return !( m_mapDirtyRegions.IsEmpty() );
}

inline Bool BlockWorldManager::SynchronizeArea( const AreaUID & hAreaUID ) {
    if ( m_mapDirtyRegions.Contains(hAreaUID.hRegionUID) ) {
        DirtyAreaMap * pDirtyAreaMap = m_mapDirtyRegions[hAreaUID.hRegionUID];
        if ( pDirtyAreaMap->Contains(hAreaUID.vAreaCoords) )
            return SaveArea( hAreaUID );
    }
    return true;
}

inline Void BlockWorldManager::RollbackArea( const AreaUID & hAreaUID ) {
    _ClearDirtyArea( hAreaUID );
}
inline Void BlockWorldManager::RollbackRegion( const RegionUID & hRegionUID ) {
    _ClearDirtyRegion( hRegionUID );
}
inline Void BlockWorldManager::RollbackWorld() {
    _ClearDirtyRegions();
}

/////////////////////////////////////////////////////////////////////////////////

inline Int BlockWorldManager::_Compare_DimensionID( const DimensionID & rLeft, const DimensionID & rRight, Void * /*pUserData*/ ) {
    if ( rLeft < rRight )
        return +1;
    if ( rLeft > rRight )
        return -1;
    return 0;
}
inline Int BlockWorldManager::_Compare_AreaCoords( const AreaCoords & rLeft, const AreaCoords & rRight, Void * /*pUserData*/ ) {
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
inline Int BlockWorldManager::_Compare_RegionUID( const RegionUID & rLeft, const RegionUID & rRight, Void * /*pUserData*/ ) {
    if ( rLeft.iDimendionID < rRight.iDimendionID )
        return +1;
    if ( rLeft.iDimendionID > rRight.iDimendionID )
        return -1;
    if ( rLeft.iGalaxyID < rRight.iGalaxyID )
        return +1;
    if ( rLeft.iGalaxyID > rRight.iGalaxyID )
        return -1;
    if ( rLeft.iStarSystemID < rRight.iStarSystemID )
        return +1;
    if ( rLeft.iStarSystemID > rRight.iStarSystemID )
        return -1;
    if ( rLeft.iOrbit < rRight.iOrbit )
        return +1;
    if ( rLeft.iOrbit > rRight.iOrbit )
        return -1;
    if ( rLeft.iRegionLayer < rRight.iRegionLayer )
        return +1;
    if ( rLeft.iRegionLayer > rRight.iRegionLayer )
        return -1;
    if ( rLeft.iMoonID < rRight.iMoonID )
        return +1;
    if ( rLeft.iMoonID > rRight.iMoonID )
        return -1;
    if ( rLeft.vRegionCoords.Y < rRight.vRegionCoords.Y )
        return +1;
    if ( rLeft.vRegionCoords.Y > rRight.vRegionCoords.Y )
        return -1;
    if ( rLeft.vRegionCoords.X < rRight.vRegionCoords.X )
        return +1;
    if ( rLeft.vRegionCoords.X > rRight.vRegionCoords.X )
        return -1;
    return 0;
}

inline GChar * BlockWorldManager::_GetWorldPath( GChar * outPath ) const {
    return StringFn->Format( outPath, TEXT("%s/%s"), BLOCKWORLD_SAVE_PATH, m_strWorldName );
}
inline GChar * BlockWorldManager::_GetDimensionPath( GChar * outPath, DimensionID iDimensionID ) const {
    return StringFn->Format( outPath, TEXT("/Dimension-%d"), iDimensionID );
}
inline GChar * BlockWorldManager::_GetGalaxyPath( GChar * outPath, GalaxyID iGalaxyID ) const {
    return StringFn->Format( outPath, TEXT("/Galaxy-%d"), iGalaxyID );
}
inline GChar * BlockWorldManager::_GetStarSystemPath( GChar * outPath, StarSystemID iStarSystemID ) const {
    return StringFn->Format( outPath, TEXT("/StarSystem-%d"), iStarSystemID );
}
inline GChar * BlockWorldManager::_GetPlanetPath( GChar * outPath, UInt iOrbit ) const {
    if ( iOrbit == STAR_ORBIT_PRIMARY )
        return StringFn->Format( outPath, TEXT("/Star-Primary") );
    if ( iOrbit == STAR_ORBIT_BINARY )
        return StringFn->Format( outPath, TEXT("/Star-Binary") );
    return StringFn->Format( outPath, TEXT("/Planet-%d"), iOrbit );
}
inline GChar * BlockWorldManager::_GetRegionPath( GChar * outPath, const RegionCoords & vRegionCoords ) const {
    return StringFn->Format( outPath, TEXT("/Region-%d-%d"), vRegionCoords.X, vRegionCoords.Y );
}
inline GChar * BlockWorldManager::_GetRegionPath( GChar * outPath, const RegionUID & hRegionUID ) const {
    outPath = _GetRegionLayerPath( outPath, hRegionUID );
    return _GetRegionPath( outPath, hRegionUID.vRegionCoords );
}
inline GChar * BlockWorldManager::_GetAreaPath( GChar * outPath, const AreaCoords & vAreaCoords ) const {
    return StringFn->Format( outPath, TEXT("/Area-%d-%d.xml"), vAreaCoords.X, vAreaCoords.Y );
}
inline GChar * BlockWorldManager::_GetAreaPath( GChar * outPath, const AreaUID & hAreaUID ) const {
    outPath = _GetRegionPath( outPath, hAreaUID.hRegionUID );
    return _GetAreaPath( outPath, hAreaUID.vAreaCoords );
}
