/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/World/StarSystem.cpp
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
// Includes
#include "StarSystem.h"

#include "BlockWorld.h"

/////////////////////////////////////////////////////////////////////////////////
// StarSystem implementation
StarSystem::StarSystem( Galaxy * pParentGalaxy, StarSystemID iStarSystemID, UInt iOrbitCount ):
    m_mapTunnels(), m_itEnumerateTunnel()
{
    Assert( iOrbitCount <= STARSYSTEM_MAX_ORBITS );

    // StarSystem data
    m_iStarSystemID = iStarSystemID;

    // Parent linkage
    m_pParentGalaxy = pParentGalaxy;

    // Adjacent linkage : Tunnels (sparse)
    m_mapTunnels.UseMemoryContext( BlockWorldFn->GetMemoryContext(), TEXT("Scratch") );
    m_mapTunnels.SetComparator( _Compare_StarSystemID );
    m_mapTunnels.Create();

    // Create content
    m_pPrimaryStar = NULL;
    m_pBinaryStar = NULL;

    m_iOrbitCount = iOrbitCount;
    for( UInt i = 0; i < STARSYSTEM_MAX_ORBITS; ++i )
        m_arrPlanets[i] = NULL;

    m_pFarSpace = NULL;
}
StarSystem::~StarSystem()
{
    // Destroy content
    DestroyFarSpace();
    
    for( UInt i = 0; i < m_iOrbitCount; ++i )
        DestroyPlanet( i );

    DestroyBinaryStar();
    DestroyPrimaryStar();

    // Adjacent linkage : Tunnels (sparse)
    m_mapTunnels.Destroy();
}

Dimension * StarSystem::GetParentDimension() const
{
    return m_pParentGalaxy->GetParentDimension();
}

StarSystemID StarSystem::CreateTunnel( StarSystem * pTargetStarSystem )
{
    Assert( pTargetStarSystem != this );
    Assert( pTargetStarSystem->GetParentGalaxy() == m_pParentGalaxy );
    if ( m_mapTunnels.Contains(pTargetStarSystem->m_iStarSystemID) )
        return pTargetStarSystem->m_iStarSystemID;

    Bool bInserted = m_mapTunnels.Insert( pTargetStarSystem->m_iStarSystemID, pTargetStarSystem );
    Assert( bInserted );

    return pTargetStarSystem->m_iStarSystemID;
}
Void StarSystem::DestroyTunnel( StarSystemID iStarSystemID )
{
    if ( m_mapTunnels.Contains(iStarSystemID) ) {
        Bool bRemoved = m_mapTunnels.Remove( iStarSystemID );
        Assert( bRemoved );
    }
}

Planet * StarSystem::CreatePrimaryStar()
{
    if ( m_pPrimaryStar == NULL ) {
        BlockWorldFn->SelectMemory( TEXT("Planets") );
        m_pPrimaryStar = New Planet( this, STAR_ORBIT_PRIMARY, 0 );
        BlockWorldFn->UnSelectMemory();
    }
    return m_pPrimaryStar;
}
Planet * StarSystem::CreateBinaryStar()
{
    CreatePrimaryStar();

    if ( m_pBinaryStar == NULL ) {
        BlockWorldFn->SelectMemory( TEXT("Planets") );
        m_pBinaryStar = New Planet( this, STAR_ORBIT_BINARY, 0 );
        BlockWorldFn->UnSelectMemory();
    }
    return m_pBinaryStar;
}
Void StarSystem::DestroyPrimaryStar()
{
    if ( m_pPrimaryStar != NULL ) {
        BlockWorldFn->SelectMemory( TEXT("Planets") );
        Delete( m_pPrimaryStar );
        m_pPrimaryStar = NULL;
        BlockWorldFn->UnSelectMemory();
    }

    m_pPrimaryStar = m_pBinaryStar;
    m_pBinaryStar = NULL;
}
Void StarSystem::DestroyBinaryStar()
{
    if ( m_pBinaryStar != NULL ) {
        BlockWorldFn->SelectMemory( TEXT("Planets") );
        Delete( m_pBinaryStar );
        m_pBinaryStar = NULL;
        BlockWorldFn->UnSelectMemory();
    }
}

Planet * StarSystem::CreatePlanet( UInt iOrbit, UInt iMoonCount )
{
    Assert( iOrbit < m_iOrbitCount );
    if ( m_arrPlanets[iOrbit] != NULL )
        return m_arrPlanets[iOrbit];

    BlockWorldFn->SelectMemory( TEXT("Planets") );
    m_arrPlanets[iOrbit] = New Planet( this, iOrbit, iMoonCount );
    BlockWorldFn->UnSelectMemory();

    return m_arrPlanets[iOrbit];
}
Void StarSystem::DestroyPlanet( UInt iOrbit )
{
    Assert( iOrbit < m_iOrbitCount );
    if ( m_arrPlanets[iOrbit] != NULL ) {
        BlockWorldFn->SelectMemory( TEXT("Planets") );
        Delete( m_arrPlanets[iOrbit] );
        m_arrPlanets[iOrbit] = NULL;
        BlockWorldFn->UnSelectMemory();
    }
}

VoidSpace * StarSystem::CreateFarSpace()
{
    if ( m_pFarSpace == NULL ) {
        BlockWorldFn->SelectMemory( TEXT("VoidSpaces") );
        m_pFarSpace = New VoidSpace( this );
        BlockWorldFn->UnSelectMemory();
    }

    return m_pFarSpace;
}
Void StarSystem::DestroyFarSpace()
{
    if ( m_pFarSpace != NULL ) {
        BlockWorldFn->SelectMemory( TEXT("VoidSpaces") );
        Delete( m_pFarSpace );
        m_pFarSpace = NULL;
        BlockWorldFn->UnSelectMemory();
    }
}
