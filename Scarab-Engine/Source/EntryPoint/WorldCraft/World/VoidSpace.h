/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/World/VoidSpace.h
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
// Header prelude
#ifndef SCARAB_ENTRYPOINT_WORLDCRAFT_WORLD_VOIDSPACE_H
#define SCARAB_ENTRYPOINT_WORLDCRAFT_WORLD_VOIDSPACE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Datastruct/Table/TreeMap.h"

#include "Region.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// VoidSpace types
enum VoidSpaceType {
    VOIDSPACE_OUTER = 0,
    VOIDSPACE_FAR,
    VOIDSPACE_DEEP,
    VOIDSPACE_OBLIVION
    //VOIDSPACE_WARP
};

// Prototypes
class Planet;
class StarSystem;
class Galaxy;
class Dimension;

/////////////////////////////////////////////////////////////////////////////////
// The VoidSpace class
class VoidSpace
{
public:
    VoidSpace( Dimension * pParentDimension );
    VoidSpace( Galaxy * pParentGalaxy );
    VoidSpace( StarSystem * pParentStarSystem );
    VoidSpace( Planet * pParentPlanet );
    ~VoidSpace();

    // VoidSpace data
    inline VoidSpaceType GetType() const;
    inline RegionLayer GetRegionLayer() const;

    // Parent linkage
    inline Planet * GetParentPlanet() const;
    inline StarSystem * GetParentStarSystem() const;
    inline Galaxy * GetParentGalaxy() const;
    inline Dimension * GetParentDimension() const;

    // Child linkage
        // Regions
    inline UInt GetRegionCount() const;
    inline Region * GetRegion( const RegionCoords & vRegionCoords ) const;

    inline Void EnumRegions();
    inline Region * EnumNextRegion();

    Region * CreateRegion( const RegionCoords & vRegionCoords );
    Void DestroyRegion( const RegionCoords & vRegionCoords );

private:
    // VoidSpace data
    VoidSpaceType m_iType;

    // Parent data : Dimension OR Galaxy OR StarSystem OR Planet
    Void * m_pParent;

    // Child data : Void-Regions
    inline static Int _Compare_RegionCoords( const RegionCoords & rLeft, const RegionCoords & rRight, Void * pUserData );
    typedef TreeMap<RegionCoords,Region*> RegionMap;
    RegionMap m_mapVoidRegions;
    RegionMap::Iterator m_itEnumerateRegion;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "VoidSpace.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDCRAFT_WORLD_VOIDSPACE_H

