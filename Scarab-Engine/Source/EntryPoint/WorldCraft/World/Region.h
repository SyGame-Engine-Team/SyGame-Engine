/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/World/Region.h
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
// Header prelude
#ifndef SCARAB_ENTRYPOINT_WORLDCRAFT_WORLD_REGION_H
#define SCARAB_ENTRYPOINT_WORLDCRAFT_WORLD_REGION_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Math/Random/Random.h"

#include "Area.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Region coordinates (relative to parent planet/moon/voidspace)
typedef Point2 RegionCoords;

// Dimensions
#define REGION_SIZE      16  // in areas
#define REGION_AREACOUNT ( REGION_SIZE * REGION_SIZE )

#define REGION_UNIT      ( AREA_UNIT * (Scalar)REGION_SIZE )

// Adjacency
enum RegionAdjacency {
    REGION_ADJACENCY_WEST  = 0x01, // -X
    REGION_ADJACENCY_EAST  = 0x02, // +X
    REGION_ADJACENCY_SOUTH = 0x04, // -Y
    REGION_ADJACENCY_NORTH = 0x08, // +Y
    REGION_ADJACENCY_DOWN  = 0x10, // -Z
    REGION_ADJACENCY_UP    = 0x20  // +Z
};

// Prototypes
class Planet;
class VoidSpace;
class StarSystem;
class Galaxy;
class Dimension;

/////////////////////////////////////////////////////////////////////////////////
// The Region class
class Region
{
public:
    Region( VoidSpace * pParentVoidSpace, RegionLayer iLayer, const RegionCoords & vRegionCoords );
    Region( Planet * pParentPlanet, RegionLayer iLayer, const RegionCoords & vRegionCoords );
    Region( Planet * pParentPlanet, MoonID iMoonID, const RegionCoords & vRegionCoords );
    ~Region();

    // Region data
    inline RegionLayer GetLayer( MoonID * outMoonID = NULL ) const;
    inline const RegionCoords & GetRegionCoords() const;

    Void MakeUID( RegionUID * outUID ) const;

    // Parent linkage
    inline Planet * GetParentPlanet() const;
    inline VoidSpace * GetParentVoidSpace() const;

    StarSystem * GetParentStarSystem() const;
    Galaxy * GetParentGalaxy() const;
    Dimension * GetParentDimension() const;

    // Adjacent linkage
    Region * GetAdjacentRegion( RegionAdjacency iAdjacentRegion ) const;

    // Child linkage
        // Areas
    inline UInt GetAreaCount() const;
    inline Area * GetArea( const AreaCoords & vAreaCoords ) const;

    Area * CreateArea( const AreaCoords & vAreaCoords );
    Void DestroyArea( const AreaCoords & vAreaCoords );

private:
    // Region data
    RegionLayer m_iLayer;
    MoonID m_iMoonID;
    RegionCoords m_vRegionCoords;

    // Parent data : Planet OR VoidSpace
    Planet * m_pParentPlanet;
    VoidSpace * m_pParentVoidSpace;

    // Child data : Areas
    UInt m_iAreaCount;
    Area * m_arrAreas[REGION_AREACOUNT];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Region.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDCRAFT_WORLD_REGION_H

