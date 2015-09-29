/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/World/Area.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World hierarchy : Area (16*16*256 BlockSpaces)
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
#include "Area.h"

#include "BlockWorld.h"

/////////////////////////////////////////////////////////////////////////////////
// Area implementation
Area::Area( Region * pParentRegion, const AreaCoords & vAreaCoords ):
    m_vAreaCoords( vAreaCoords )
{
    // Parent linkage
    m_pParentRegion = pParentRegion;

    // Create content
    BlockWorldFn->SelectMemory( TEXT("BlockSpaces") );

    BlockSpaceCoords vBlockSpaceCoords;
    for( vBlockSpaceCoords.Z = 0; vBlockSpaceCoords.Z < AREA_HEIGHT; ++(vBlockSpaceCoords.Z) ) {
        for( vBlockSpaceCoords.Y = 0; vBlockSpaceCoords.Y < AREA_SIZE; ++(vBlockSpaceCoords.Y) ) {
            for( vBlockSpaceCoords.X = 0; vBlockSpaceCoords.X < AREA_SIZE; ++(vBlockSpaceCoords.X) ) {
                UInt iIndex = ( ( vBlockSpaceCoords.Z * AREA_SLICE ) + ( vBlockSpaceCoords.Y * AREA_SIZE ) + vBlockSpaceCoords.X );
                m_arrBlockSpaces[iIndex] = New BlockSpace( this, vBlockSpaceCoords );
            }
        }
    }

    BlockWorldFn->UnSelectMemory();
}
Area::~Area()
{
    // Destroy content
    BlockWorldFn->SelectMemory( TEXT("BlockSpaces") );

    for( UInt i = 0; i < AREA_BLOCKCOUNT; ++i ) {
        Delete( m_arrBlockSpaces[i] );
        m_arrBlockSpaces[i] = NULL;
    }

    BlockWorldFn->UnSelectMemory();
}

Void Area::MakeUID( AreaUID * outUID ) const
{
    m_pParentRegion->MakeUID( &(outUID->hRegionUID) );
    outUID->vAreaCoords = m_vAreaCoords;
}

RegionLayer Area::GetRegionLayer( MoonID * outMoonID ) const
{
    return m_pParentRegion->GetLayer( outMoonID );
}

Planet * Area::GetParentPlanet() const
{
    return m_pParentRegion->GetParentPlanet();
}
VoidSpace * Area::GetParentVoidSpace() const
{
    return m_pParentRegion->GetParentVoidSpace();
}

StarSystem * Area::GetParentStarSystem() const
{
    return m_pParentRegion->GetParentStarSystem();
}
Galaxy * Area::GetParentGalaxy() const
{
    return m_pParentRegion->GetParentGalaxy();
}
Dimension * Area::GetParentDimension() const
{
    return m_pParentRegion->GetParentDimension();
}

Area * Area::GetAdjacentArea( AreaAdjacency iAdjacentArea ) const
{
    Region * pAdjacentRegion = m_pParentRegion;
    AreaCoords vAdjacentCoords = m_vAreaCoords;

    if ( iAdjacentArea & AREA_ADJACENCY_WEST ) {
        if ( vAdjacentCoords.X == 0 ) {
            vAdjacentCoords.X = ( REGION_SIZE - 1 );
            pAdjacentRegion = pAdjacentRegion->GetAdjacentRegion( REGION_ADJACENCY_WEST );
        } else
            vAdjacentCoords.X -= 1;
    } else if ( iAdjacentArea & AREA_ADJACENCY_EAST ) {
        if ( vAdjacentCoords.X == REGION_SIZE - 1 ) {
            vAdjacentCoords.X = 0;
            pAdjacentRegion = pAdjacentRegion->GetAdjacentRegion( REGION_ADJACENCY_EAST );
        } else
            vAdjacentCoords.X += 1;
    }

    if ( iAdjacentArea & AREA_ADJACENCY_SOUTH ) {
        if ( vAdjacentCoords.Y == 0 ) {
            vAdjacentCoords.Y = ( REGION_SIZE - 1 );
            pAdjacentRegion = pAdjacentRegion->GetAdjacentRegion( REGION_ADJACENCY_SOUTH );
        } else
            vAdjacentCoords.Y -= 1;
    } else if ( iAdjacentArea & AREA_ADJACENCY_NORTH ) {
        if ( vAdjacentCoords.Y == REGION_SIZE - 1 ) {
            vAdjacentCoords.Y = 0;
            pAdjacentRegion = pAdjacentRegion->GetAdjacentRegion( REGION_ADJACENCY_NORTH );
        } else
            vAdjacentCoords.Y += 1;
    }

    if ( iAdjacentArea & AREA_ADJACENCY_DOWN )
        pAdjacentRegion = pAdjacentRegion->GetAdjacentRegion( REGION_ADJACENCY_DOWN );
    else if ( iAdjacentArea & AREA_ADJACENCY_UP )
        pAdjacentRegion = pAdjacentRegion->GetAdjacentRegion( REGION_ADJACENCY_UP );

    if ( pAdjacentRegion == NULL )
        return NULL;
    return pAdjacentRegion->GetArea( vAdjacentCoords );
}

