/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/World/Area.h
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
// Header prelude
#ifndef SCARAB_ENTRYPOINT_WORLDCRAFT_WORLD_AREA_H
#define SCARAB_ENTRYPOINT_WORLDCRAFT_WORLD_AREA_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "BlockSpace.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Area coordinates (relative to parent region)
typedef Point2 AreaCoords;

// Dimensions
#define AREA_SIZE        16    // in blocks
#define AREA_HEIGHT      256   // in blocks
#define AREA_SLICE       ( AREA_SIZE * AREA_SIZE )
#define AREA_BLOCKCOUNT  ( AREA_SLICE * AREA_HEIGHT )

#define AREA_UNIT        ( BLOCK_UNIT * (Scalar)AREA_SIZE )

// Adjacency
enum AreaAdjacency {
    AREA_ADJACENCY_WEST  = 0x01, // -X
    AREA_ADJACENCY_EAST  = 0x02, // +X
    AREA_ADJACENCY_SOUTH = 0x04, // -Y
    AREA_ADJACENCY_NORTH = 0x08, // +Y
    AREA_ADJACENCY_DOWN  = 0x10, // -Z
    AREA_ADJACENCY_UP    = 0x20  // +Z
};

// Prototypes
class Region;
class Planet;
class VoidSpace;
class StarSystem;
class Galaxy;
class Dimension;

/////////////////////////////////////////////////////////////////////////////////
// The Area class
class Area
{
public:
    Area( Region * pParentRegion, const AreaCoords & vAreaCoords );
    ~Area();

    // Area data
    inline const AreaCoords & GetAreaCoords() const;

    Void MakeUID( AreaUID * outUID ) const;

    // Parent linkage
    inline Region * GetParentRegion() const;
    RegionLayer GetRegionLayer( MoonID * outMoonID = NULL ) const;
    
    Planet * GetParentPlanet() const;
    VoidSpace * GetParentVoidSpace() const;

    StarSystem * GetParentStarSystem() const;
    Galaxy * GetParentGalaxy() const;
    Dimension * GetParentDimension() const;

    // Adjacency linkage
    Area * GetAdjacentArea( AreaAdjacency iAdjacentArea ) const;

    // Child linkage
    inline BlockSpace * GetBlockSpace( const BlockSpaceCoords & vBlockSpaceCoords ) const;

private:
    // Area data
    AreaCoords m_vAreaCoords;

    // Parent data : Region
    Region * m_pParentRegion;

    // Child data : BlockSpaces
    BlockSpace * m_arrBlockSpaces[AREA_BLOCKCOUNT];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Area.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDCRAFT_WORLD_AREA_H


