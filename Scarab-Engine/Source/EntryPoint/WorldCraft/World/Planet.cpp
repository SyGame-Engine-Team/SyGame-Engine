/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/World/Planet.cpp
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
// Includes
#include "Planet.h"

#include "BlockWorld.h"

/////////////////////////////////////////////////////////////////////////////////
// Planet implementation
Planet::Planet( StarSystem * pParentStarSystem, UInt iOrbit, UInt iMoonCount ):
    m_mapUndergroundRegions(), m_mapSurfaceRegions(), m_mapAtmosphereRegions(), m_itEnumerateRegion(), m_itEnumerateMoonRegion()
{
    Assert( iMoonCount <= PLANET_MAX_MOONS );

    // Planet data
    m_iOrbit = iOrbit;

    // Parent linkage
    m_pParentStarSystem = pParentStarSystem;

    // Create content
    m_mapUndergroundRegions.UseMemoryContext( BlockWorldFn->GetMemoryContext(), TEXT("Scratch") );
    m_mapUndergroundRegions.SetComparator( _Compare_RegionCoords, NULL );
    m_mapUndergroundRegions.Create();

    m_mapSurfaceRegions.UseMemoryContext( BlockWorldFn->GetMemoryContext(), TEXT("Scratch") );
    m_mapSurfaceRegions.SetComparator( _Compare_RegionCoords, NULL );
    m_mapSurfaceRegions.Create();

    m_mapAtmosphereRegions.UseMemoryContext( BlockWorldFn->GetMemoryContext(), TEXT("Scratch") );
    m_mapAtmosphereRegions.SetComparator( _Compare_RegionCoords, NULL );
    m_mapAtmosphereRegions.Create();

    m_pOuterSpace = NULL;

    m_iMoonCount = iMoonCount;
    for( UInt i = 0; i < PLANET_MAX_MOONS; ++i )
        m_arrMoons[i] = NULL;
}
Planet::~Planet()
{
    // Destroy content
    for( UInt i = 0; i < m_iMoonCount; ++i )
        DestroyMoon( i );
    
    DestroyOuterSpace();
    
    EnumRegions( REGION_LAYER_ATMOSPHERE );
    Region * pRegion = EnumNextRegion();
    while( pRegion != NULL ) {
        DestroyRegion( REGION_LAYER_ATMOSPHERE, pRegion->GetRegionCoords() );
        pRegion = EnumNextRegion();
    }
    m_mapAtmosphereRegions.Destroy();

    EnumRegions( REGION_LAYER_SURFACE );
    pRegion = EnumNextRegion();
    while( pRegion != NULL ) {
        DestroyRegion( REGION_LAYER_SURFACE, pRegion->GetRegionCoords() );
        pRegion = EnumNextRegion();
    }
    m_mapSurfaceRegions.Destroy();

    EnumRegions( REGION_LAYER_UNDERGROUND );
    pRegion = EnumNextRegion();
    while( pRegion != NULL ) {
        DestroyRegion( REGION_LAYER_UNDERGROUND, pRegion->GetRegionCoords() );
        pRegion = EnumNextRegion();
    }
    m_mapUndergroundRegions.Destroy();
}

Galaxy * Planet::GetParentGalaxy() const
{
    return m_pParentStarSystem->GetParentGalaxy();
}
Dimension * Planet::GetParentDimension() const
{
    return m_pParentStarSystem->GetParentDimension();
}

Planet * Planet::GetAdjacentPlanet( PlanetAdjacency iAdjacentPlanet ) const
{
    UInt iAdjacentOrbit = m_iOrbit;

    if ( iAdjacentPlanet & PLANET_ADJACENCY_INWARD ) {
        if ( iAdjacentOrbit == 0 )
            return m_pParentStarSystem->GetPrimaryStar();
        else
            iAdjacentOrbit -= 1;
    } else if ( iAdjacentPlanet & PLANET_ADJACENCY_OUTWARD ) {
        if ( iAdjacentOrbit == STARSYSTEM_MAX_ORBITS - 1 )
            return NULL; // => FarSpace
        else
            iAdjacentOrbit += 1;
    }

    return m_pParentStarSystem->GetPlanet( iAdjacentOrbit );
}

Region * Planet::GetRegion( RegionLayer iRegionLayer, const RegionCoords & vRegionCoords ) const
{
    switch( iRegionLayer ) {
        case REGION_LAYER_UNDERGROUND:
            if ( m_mapUndergroundRegions.Contains(vRegionCoords) )
                return m_mapUndergroundRegions[vRegionCoords];
            break;
        case REGION_LAYER_SURFACE:
            if ( m_mapSurfaceRegions.Contains(vRegionCoords) )
                return m_mapSurfaceRegions[vRegionCoords];
            break;
        case REGION_LAYER_ATMOSPHERE:
            if ( m_mapAtmosphereRegions.Contains(vRegionCoords) )
                return m_mapAtmosphereRegions[vRegionCoords];
            break;
        default: Assert( false ); break;
    }

    return NULL;
}

Region * Planet::CreateRegion( RegionLayer iRegionLayer, const RegionCoords & vRegionCoords )
{
    RegionMap * pMap = NULL;
    switch( iRegionLayer ) {
        case REGION_LAYER_UNDERGROUND: pMap = &m_mapUndergroundRegions; break;
        case REGION_LAYER_SURFACE:     pMap = &m_mapSurfaceRegions;     break;
        case REGION_LAYER_ATMOSPHERE:  pMap = &m_mapAtmosphereRegions;  break;
        default: Assert( false ); break;
    }

    if ( pMap->Contains(vRegionCoords) )
        return (*pMap)[vRegionCoords];

    BlockWorldFn->SelectMemory( TEXT("Regions") );
    Region * pRegion = New Region( this, iRegionLayer, vRegionCoords );
    BlockWorldFn->UnSelectMemory();

    Bool bInserted = pMap->Insert( vRegionCoords, pRegion );
    Assert( bInserted );

    return pRegion;
}
Void Planet::DestroyRegion( RegionLayer iRegionLayer, const RegionCoords & vRegionCoords )
{
    RegionMap * pMap = NULL;
    switch( iRegionLayer ) {
        case REGION_LAYER_UNDERGROUND: pMap = &m_mapUndergroundRegions; break;
        case REGION_LAYER_SURFACE:     pMap = &m_mapSurfaceRegions;     break;
        case REGION_LAYER_ATMOSPHERE:  pMap = &m_mapAtmosphereRegions;  break;
        default: Assert( false ); break;
    }

    if ( pMap->Contains(vRegionCoords) ) {
        Region * pRegion = (*pMap)[vRegionCoords];

        BlockWorldFn->SelectMemory( TEXT("Regions") );
        Delete( pRegion );
        BlockWorldFn->UnSelectMemory();

        Bool bRemoved = pMap->Remove( vRegionCoords );
        Assert( bRemoved );
    }
}

VoidSpace * Planet::CreateOuterSpace()
{
    if ( m_pOuterSpace == NULL ) {
        BlockWorldFn->SelectMemory( TEXT("VoidSpaces") );
        m_pOuterSpace = New VoidSpace( this );
        BlockWorldFn->UnSelectMemory();
    }

    return m_pOuterSpace;
}
Void Planet::DestroyOuterSpace()
{
    if ( m_pOuterSpace != NULL ) {
        BlockWorldFn->SelectMemory( TEXT("VoidSpaces") );
        Delete( m_pOuterSpace );
        m_pOuterSpace = NULL;
        BlockWorldFn->UnSelectMemory();
    }
}

Void Planet::CreateMoon( MoonID iMoonID )
{
    Assert( iMoonID < m_iMoonCount );
    if ( m_arrMoons[iMoonID] != NULL )
        return;

    BlockWorldFn->SelectMemory( TEXT("Scratch") );
    m_arrMoons[iMoonID] = New RegionMap();
    BlockWorldFn->UnSelectMemory();

    m_arrMoons[iMoonID]->UseMemoryContext( BlockWorldFn->GetMemoryContext(), TEXT("Scratch") );
    m_arrMoons[iMoonID]->SetComparator( _Compare_RegionCoords );
    m_arrMoons[iMoonID]->Create();
}
Void Planet::DestroyMoon( MoonID iMoonID )
{
    Assert( iMoonID < m_iMoonCount );
    if ( m_arrMoons[iMoonID] == NULL )
        return;

    EnumMoonRegions( iMoonID );
    Region * pMoonRegion = EnumNextMoonRegion();
    while( pMoonRegion != NULL ) {
        DestroyMoonRegion( iMoonID, pMoonRegion->GetRegionCoords() );
        pMoonRegion = EnumNextMoonRegion();
    }
    m_arrMoons[iMoonID]->Destroy();

    BlockWorldFn->SelectMemory( TEXT("Scratch") );
    Delete( m_arrMoons[iMoonID] );
    m_arrMoons[iMoonID] = NULL;
    BlockWorldFn->UnSelectMemory();
}

Region * Planet::GetMoonRegion( MoonID iMoonID, const RegionCoords & vRegionCoords ) const
{
    Assert( iMoonID < m_iMoonCount );
    if ( m_arrMoons[iMoonID] == NULL )
        return NULL;

    if ( m_arrMoons[iMoonID]->Contains(vRegionCoords) )
        return ( *(m_arrMoons[iMoonID]) )[vRegionCoords];

    return NULL;
}

Region * Planet::CreateMoonRegion( MoonID iMoonID, const RegionCoords & vRegionCoords )
{
    Assert( iMoonID < m_iMoonCount );
    RegionMap * pMap = m_arrMoons[iMoonID];

    if ( pMap->Contains(vRegionCoords) )
        return (*pMap)[vRegionCoords];

    BlockWorldFn->SelectMemory( TEXT("Regions") );
    Region * pRegion = New Region( this, iMoonID, vRegionCoords );
    BlockWorldFn->UnSelectMemory();

    Bool bInserted = pMap->Insert( vRegionCoords, pRegion );
    Assert( bInserted );

    return pRegion;
}
Void Planet::DestroyMoonRegion( MoonID iMoonID, const RegionCoords & vRegionCoords )
{
    Assert( iMoonID < m_iMoonCount );
    RegionMap * pMap = m_arrMoons[iMoonID];

    if ( pMap->Contains(vRegionCoords) ) {
        Region * pRegion = (*pMap)[vRegionCoords];

        BlockWorldFn->SelectMemory( TEXT("Regions") );
        Delete( pRegion );
        BlockWorldFn->UnSelectMemory();

        Bool bRemoved = pMap->Remove( vRegionCoords );
        Assert( bRemoved );
    }
}
