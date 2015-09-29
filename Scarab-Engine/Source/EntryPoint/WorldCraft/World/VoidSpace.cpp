/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/World/VoidSpace.cpp
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
// Includes
#include "VoidSpace.h"

#include "BlockWorld.h"

/////////////////////////////////////////////////////////////////////////////////
// VoidSpace implementation
VoidSpace::VoidSpace( Dimension * pParentDimension )
{
    // VoidSpace data
    m_iType = VOIDSPACE_OBLIVION;

    // Parent data
    m_pParent = (Void*)pParentDimension;

    // Create content
    m_mapVoidRegions.UseMemoryContext( BlockWorldFn->GetMemoryContext(), TEXT("Scratch") );
    m_mapVoidRegions.SetComparator( _Compare_RegionCoords, NULL );
    m_mapVoidRegions.Create();
}
VoidSpace::VoidSpace( Galaxy * pParentGalaxy )
{
    // VoidSpace data
    m_iType = VOIDSPACE_DEEP;

    // Parent data
    m_pParent = (Void*)pParentGalaxy;

    // Create content
    m_mapVoidRegions.UseMemoryContext( BlockWorldFn->GetMemoryContext(), TEXT("Scratch") );
    m_mapVoidRegions.SetComparator( _Compare_RegionCoords, NULL );
    m_mapVoidRegions.Create();
}
VoidSpace::VoidSpace( StarSystem * pParentStarSystem )
{
    // VoidSpace data
    m_iType = VOIDSPACE_FAR;

    // Parent data
    m_pParent = (Void*)pParentStarSystem;

    // Create content
    m_mapVoidRegions.UseMemoryContext( BlockWorldFn->GetMemoryContext(), TEXT("Scratch") );
    m_mapVoidRegions.SetComparator( _Compare_RegionCoords, NULL );
    m_mapVoidRegions.Create();
}
VoidSpace::VoidSpace( Planet * pParentPlanet )
{
    // VoidSpace data
    m_iType = VOIDSPACE_OUTER;

    // Parent data
    m_pParent = (Void*)pParentPlanet;

    // Create content
    m_mapVoidRegions.UseMemoryContext( BlockWorldFn->GetMemoryContext(), TEXT("Scratch") );
    m_mapVoidRegions.SetComparator( _Compare_RegionCoords, NULL );
    m_mapVoidRegions.Create();
}
VoidSpace::~VoidSpace()
{
    // Destroy content
    EnumRegions();
    Region * pRegion = EnumNextRegion();
    while( pRegion != NULL ) {
        DestroyRegion( pRegion->GetRegionCoords() );
        pRegion = EnumNextRegion();
    }
    m_mapVoidRegions.Destroy();
}

Region * VoidSpace::CreateRegion( const RegionCoords & vRegionCoords )
{
    if ( m_mapVoidRegions.Contains(vRegionCoords) )
        return m_mapVoidRegions[vRegionCoords];

    RegionLayer iLayer;
    switch( m_iType ) {
        case VOIDSPACE_OUTER:    iLayer = REGION_LAYER_OUTERSPACE;
        case VOIDSPACE_FAR:      iLayer = REGION_LAYER_FARSPACE;
        case VOIDSPACE_DEEP:     iLayer = REGION_LAYER_DEEPSPACE;
        case VOIDSPACE_OBLIVION: iLayer = REGION_LAYER_OBLIVIONSPACE;
        default: Assert(false); break;
    }

    BlockWorldFn->SelectMemory( TEXT("Regions") );
    Region * pRegion = New Region( this, iLayer, vRegionCoords );
    BlockWorldFn->UnSelectMemory();

    Bool bInserted = m_mapVoidRegions.Insert( vRegionCoords, pRegion );
    Assert( bInserted );

    return pRegion;
}
Void VoidSpace::DestroyRegion( const RegionCoords & vRegionCoords )
{
    if ( m_mapVoidRegions.Contains(vRegionCoords) ) {
        Region * pRegion = m_mapVoidRegions[vRegionCoords];

        BlockWorldFn->SelectMemory( TEXT("Regions") );
        Delete( pRegion );
        BlockWorldFn->UnSelectMemory();

        Bool bRemoved = m_mapVoidRegions.Remove( vRegionCoords );
        Assert( bRemoved );
    }
}

