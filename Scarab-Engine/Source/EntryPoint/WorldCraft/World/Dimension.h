/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/World/Dimension.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World hierarchy : Dimension (dynamic set of Galaxies)
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
#ifndef SCARAB_ENTRYPOINT_WORLDCRAFT_WORLD_DIMENSION_H
#define SCARAB_ENTRYPOINT_WORLDCRAFT_WORLD_DIMENSION_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Galaxy.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Dimension IDs
typedef DWord DimensionID;

/////////////////////////////////////////////////////////////////////////////////
// The Dimension class
class Dimension
{
public:
    Dimension( DimensionID iDimensionID );
    ~Dimension();

    // Dimension data
    inline DimensionID GetID() const;

    // Adjacent linkage : Tunnels (sparse)
    inline UInt GetTunnelCount() const;
    inline Dimension * GetTunnel( DimensionID iDimensionID ) const;

    inline Void EnumTunnels();
    inline Dimension * EnumNextTunnel();

    DimensionID CreateTunnel( Dimension * pTargetDimension );
    Void DestroyTunnel( DimensionID iDimensionID );

    // Child linkage
        // Galaxies
    inline UInt GetGalaxyCount() const;
    inline Galaxy * GetGalaxy( GalaxyID iGalaxyID ) const;

    inline Void EnumGalaxies();
    inline Galaxy * EnumNextGalaxy();

    Galaxy * CreateGalaxy( GalaxyID iGalaxyID );
    Void DestroyGalaxy( GalaxyID iGalaxyID );

        // OblivionSpace
    inline VoidSpace * GetOblivionSpace() const;

    VoidSpace * CreateOblivionSpace();
    Void DestroyOblivionSpace();

private:
    // Dimension data
    DimensionID m_iDimensionID;

    // Adjacency data : Tunnels (sparse)
    inline static Int _Compare_DimensionID( const DimensionID & rLeft, const DimensionID & rRight, Void * pUserData );
    typedef TreeMap<DimensionID,Dimension*> TunnelMap;
    TunnelMap m_mapTunnels;
    TunnelMap::Iterator m_itEnumerateTunnel;

    // Child data : Galaxies, OblivionSpace
    inline static Int _Compare_GalaxyID( const GalaxyID & rLeft, const GalaxyID & rRight, Void * pUserData );
    typedef TreeMap<GalaxyID,Galaxy*> GalaxyMap;
    GalaxyMap m_mapGalaxies;
    GalaxyMap::Iterator m_itEnumerateGalaxy;

    VoidSpace * m_pOblivionSpace;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Dimension.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDCRAFT_WORLD_DIMENSION_H

