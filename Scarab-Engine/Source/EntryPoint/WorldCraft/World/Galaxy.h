/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/World/Galaxy.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World hierarchy : Galaxy (dynamic set of StarSystems)
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
#ifndef SCARAB_ENTRYPOINT_WORLDCRAFT_WORLD_GALAXY_H
#define SCARAB_ENTRYPOINT_WORLDCRAFT_WORLD_GALAXY_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "StarSystem.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Galaxy IDs
typedef DWord GalaxyID;

// Prototypes
class Dimension;

/////////////////////////////////////////////////////////////////////////////////
// The Galaxy class
class Galaxy
{
public:
    Galaxy( Dimension * pParentDimension, GalaxyID iGalaxyID );
    ~Galaxy();

    // Galaxy data
    inline GalaxyID GetID() const;

    // Parent linkage
    inline Dimension * GetParentDimension() const;

    // Adjacent linkage : Tunnels (sparse)
    inline UInt GetTunnelCount() const;
    inline Galaxy * GetTunnel( GalaxyID iGalaxyID ) const;

    inline Void EnumTunnels();
    inline Galaxy * EnumNextTunnel();

    GalaxyID CreateTunnel( Galaxy * pTargetGalaxy );
    Void DestroyTunnel( GalaxyID iGalaxyID );

    // Child linkage
        // StarSystems
    inline UInt GetStarSystemCount() const;
    inline StarSystem * GetStarSystem( StarSystemID iStarSystemID ) const;

    inline Void EnumStarSystems();
    inline StarSystem * EnumNextStarSystem();

    StarSystem * CreateStarSystem( StarSystemID iStarSystemID, UInt iOrbitCount );
    Void DestroyStarSystem( StarSystemID iStarSystemID );

        // DeepSpace
    inline VoidSpace * GetDeepSpace() const;

    VoidSpace * CreateDeepSpace();
    Void DestroyDeepSpace();

private:
    // Galaxy data
    DWord m_iGalaxyID;

    // Parent data : Dimension
    Dimension * m_pParentDimension;

    // Adjacency data : Tunnels (sparse)
    inline static Int _Compare_GalaxyID( const GalaxyID & rLeft, const GalaxyID & rRight, Void * pUserData );
    typedef TreeMap<GalaxyID,Galaxy*> TunnelMap;
    TunnelMap m_mapTunnels;
    TunnelMap::Iterator m_itEnumerateTunnel;

    // Child data : StarSystems, DeepSpace
    inline static Int _Compare_StarSystemID( const StarSystemID & rLeft, const StarSystemID & rRight, Void * pUserData );
    typedef TreeMap<StarSystemID,StarSystem*> StarSystemMap;
    StarSystemMap m_mapStarSystems;
    StarSystemMap::Iterator m_itEnumerateStarSystem;

    VoidSpace * m_pDeepSpace;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Galaxy.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDCRAFT_WORLD_GALAXY_H

