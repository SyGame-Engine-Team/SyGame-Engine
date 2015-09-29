/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/World/StarSystem.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World hierarchy : StarSystem (Up to 16 orbits for Planets)
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
#ifndef SCARAB_ENTRYPOINT_WORLDCRAFT_WORLD_STARSYSTEM_H
#define SCARAB_ENTRYPOINT_WORLDCRAFT_WORLD_STARSYSTEM_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Planet.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// StarSystems IDs
typedef DWord StarSystemID;

// Dimensions
#define STARSYSTEM_MAX_ORBITS 16

// Prototypes
class Galaxy;
class Dimension;

/////////////////////////////////////////////////////////////////////////////////
// The StarSystem class
class StarSystem
{
public:
    StarSystem( Galaxy * pParentGalaxy, StarSystemID iStarSystemID, UInt iOrbitCount );
    ~StarSystem();

    // StarSystem data
    inline StarSystemID GetID() const;

    // Parent linkage
    inline Galaxy * GetParentGalaxy() const;
    Dimension * GetParentDimension() const;

    // Adjacent linkage : Tunnels (sparse)
    inline UInt GetTunnelCount() const;
    inline StarSystem * GetTunnel( StarSystemID iStarSystemID ) const;

    inline Void EnumTunnels();
    inline StarSystem * EnumNextTunnel();

    StarSystemID CreateTunnel( StarSystem * pTargetStarSystem );
    Void DestroyTunnel( StarSystemID iStarSystemID );

    // Child linkage
        // Stars
    inline Planet * GetPrimaryStar() const;
    inline Planet * GetBinaryStar() const;

    Planet * CreatePrimaryStar();
    Planet * CreateBinaryStar();
    Void DestroyPrimaryStar();
    Void DestroyBinaryStar();

        // Planets
    inline UInt GetOrbitCount() const;
    inline Planet * GetPlanet( UInt iOrbit ) const;

    Planet * CreatePlanet( UInt iOrbit, UInt iMoonCount );
    Void DestroyPlanet( UInt iOrbit );

        // FarSpace
    inline VoidSpace * GetFarSpace() const;

    VoidSpace * CreateFarSpace();
    Void DestroyFarSpace();

private:
    // StarSystem data
    StarSystemID m_iStarSystemID;

    // Parent data : Galaxy
    Galaxy * m_pParentGalaxy;

    // Adjacency data : Tunnels (sparse)
    inline static Int _Compare_StarSystemID( const StarSystemID & rLeft, const StarSystemID & rRight, Void * pUserData );
    typedef TreeMap<StarSystemID,StarSystem*> TunnelMap;
    TunnelMap m_mapTunnels;
    TunnelMap::Iterator m_itEnumerateTunnel;

    // Child data : Stars, Planets, FarSpace
    Planet * m_pPrimaryStar;
    Planet * m_pBinaryStar;

    UInt m_iOrbitCount;
    Planet * m_arrPlanets[STARSYSTEM_MAX_ORBITS];

    VoidSpace * m_pFarSpace;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "StarSystem.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDCRAFT_WORLD_STARSYSTEM_H
