/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/World/Planet.h
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
// Header prelude
#ifndef SCARAB_ENTRYPOINT_WORLDCRAFT_WORLD_PLANET_H
#define SCARAB_ENTRYPOINT_WORLDCRAFT_WORLD_PLANET_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Datastruct/Table/TreeMap.h"

#include "VoidSpace.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Dimensions
#define PLANET_MAX_MOONS 4

// Stars orbit values
#define STAR_ORBIT_PRIMARY 0x80000000
#define STAR_ORBIT_BINARY  0x80000001

// Adjacency
enum PlanetAdjacency {
    PLANET_ADJACENCY_INWARD  = 0x01, // -Orbit
    PLANET_ADJACENCY_OUTWARD = 0x02  // +Orbit
};

// Prototypes
class VoidSpace;
class StarSystem;
class Galaxy;
class Dimension;

/////////////////////////////////////////////////////////////////////////////////
// The Planet class
class Planet
{
public:
    Planet( StarSystem * pParentStarSystem, UInt iOrbit, UInt iMoonCount );
    ~Planet();

    // Planet data
    inline Bool IsStar() const;
    inline Bool IsPrimaryStar() const;
    inline Bool IsBinaryStar() const;

    inline UInt GetOrbit() const;

    // Parent linkage
    inline StarSystem * GetParentStarSystem() const;
    Galaxy * GetParentGalaxy() const;
    Dimension * GetParentDimension() const;

    // Adjacent linkage
    Planet * GetAdjacentPlanet( PlanetAdjacency iAdjacentPlanet ) const;

    // Child linkage
        // Layered Regions
    inline UInt GetRegionCount( RegionLayer iRegionLayer ) const;
    Region * GetRegion( RegionLayer iRegionLayer, const RegionCoords & vRegionCoords ) const;

    inline Void EnumRegions( RegionLayer iRegionLayer );
    inline Region * EnumNextRegion();

    Region * CreateRegion( RegionLayer iRegionLayer, const RegionCoords & vRegionCoords );
    Void DestroyRegion( RegionLayer iRegionLayer, const RegionCoords & vRegionCoords );

        // OuterSpace
    inline VoidSpace * GetOuterSpace() const;

    VoidSpace * CreateOuterSpace();
    Void DestroyOuterSpace();

        // Moons
    inline UInt GetMoonCount() const;
    inline Bool HasMoon( MoonID iMoonID ) const;
    Void CreateMoon( MoonID iMoonID );
    Void DestroyMoon( MoonID iMoonID );

    inline UInt GetMoonRegionCount( MoonID iMoonID ) const;
    Region * GetMoonRegion( MoonID iMoonID, const RegionCoords & vRegionCoords ) const;

    inline Void EnumMoonRegions( MoonID iMoonID );
    inline Region * EnumNextMoonRegion();

    Region * CreateMoonRegion( MoonID iMoonID, const RegionCoords & vRegionCoords );
    Void DestroyMoonRegion( MoonID iMoonID, const RegionCoords & vRegionCoords );

private:
    // Planet data
    UInt m_iOrbit;

    // Parent data : StarSystem
    StarSystem * m_pParentStarSystem;

    // Child data : Layered Regions, OuterSpace, Moons
    inline static Int _Compare_RegionCoords( const RegionCoords & rLeft, const RegionCoords & rRight, Void * pUserData );
    typedef TreeMap<RegionCoords,Region*> RegionMap;

    RegionMap m_mapUndergroundRegions;
    RegionMap m_mapSurfaceRegions;
    RegionMap m_mapAtmosphereRegions;
    RegionMap::Iterator m_itEnumerateRegion;

    VoidSpace * m_pOuterSpace;

    UInt m_iMoonCount;
    RegionMap * m_arrMoons[PLANET_MAX_MOONS];
    RegionMap::Iterator m_itEnumerateMoonRegion;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Planet.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDCRAFT_WORLD_PLANET_H

