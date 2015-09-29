/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/World/Galaxy.cpp
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
// Includes
#include "Galaxy.h"

#include "BlockWorld.h"

/////////////////////////////////////////////////////////////////////////////////
// Galaxy implementation
Galaxy::Galaxy( Dimension * pParentDimension, GalaxyID iGalaxyID ):
    m_mapTunnels(), m_itEnumerateTunnel(), m_mapStarSystems(), m_itEnumerateStarSystem()
{
    // Galaxy data
    m_iGalaxyID = iGalaxyID;

    // Parent linkage
    m_pParentDimension = pParentDimension;

    // Adjacent linkage : Tunnels (sparse)
    m_mapTunnels.UseMemoryContext( BlockWorldFn->GetMemoryContext(), TEXT("Scratch") );
    m_mapTunnels.SetComparator( _Compare_GalaxyID );
    m_mapTunnels.Create();

    // Create content
    m_mapStarSystems.UseMemoryContext( BlockWorldFn->GetMemoryContext(), TEXT("Scratch") );
    m_mapStarSystems.SetComparator( _Compare_StarSystemID );
    m_mapStarSystems.Create();

    m_pDeepSpace = NULL;
}
Galaxy::~Galaxy()
{
    // Destroy content
    DestroyDeepSpace();

    EnumStarSystems();
    StarSystem * pStarSystem = EnumNextStarSystem();
    while( pStarSystem != NULL ) {
        DestroyStarSystem( pStarSystem->GetID() );
        pStarSystem = EnumNextStarSystem();
    }
    m_mapStarSystems.Destroy();

    // Adjacent linkage : Tunnels (sparse)
    m_mapTunnels.Destroy();
}

GalaxyID Galaxy::CreateTunnel( Galaxy * pTargetGalaxy )
{
    Assert( pTargetGalaxy != this );
    Assert( pTargetGalaxy->GetParentDimension() == m_pParentDimension );
    if ( m_mapTunnels.Contains(pTargetGalaxy->m_iGalaxyID) )
        return pTargetGalaxy->m_iGalaxyID;

    Bool bInserted = m_mapTunnels.Insert( pTargetGalaxy->m_iGalaxyID, pTargetGalaxy );
    Assert( bInserted );

    return pTargetGalaxy->m_iGalaxyID;
}
Void Galaxy::DestroyTunnel( GalaxyID iGalaxyID )
{
    if ( m_mapTunnels.Contains(iGalaxyID) ) {
        Bool bRemoved = m_mapTunnels.Remove( iGalaxyID );
        Assert( bRemoved );
    }
}

StarSystem * Galaxy::CreateStarSystem( StarSystemID iStarSystemID, UInt iOrbitCount )
{
    if ( m_mapStarSystems.Contains(iStarSystemID) )
        return m_mapStarSystems[iStarSystemID];

    BlockWorldFn->SelectMemory( TEXT("StarSystems") );
    StarSystem * pStarSystem = New StarSystem( this, iStarSystemID, iOrbitCount );
    BlockWorldFn->UnSelectMemory();

    Bool bInserted = m_mapStarSystems.Insert( iStarSystemID, pStarSystem );
    Assert( bInserted );

    return pStarSystem;
}
Void Galaxy::DestroyStarSystem( StarSystemID iStarSystemID )
{
    if ( m_mapStarSystems.Contains(iStarSystemID) ) {
        StarSystem * pStarSystem = m_mapStarSystems[iStarSystemID];

        BlockWorldFn->SelectMemory( TEXT("StarSystems") );
        Delete( pStarSystem );
        BlockWorldFn->UnSelectMemory();

        Bool bRemoved = m_mapStarSystems.Remove( iStarSystemID );
        Assert( bRemoved );
    }
}

VoidSpace * Galaxy::CreateDeepSpace()
{
    if ( m_pDeepSpace == NULL ) {
        BlockWorldFn->SelectMemory( TEXT("VoidSpaces") );
        m_pDeepSpace = New VoidSpace( this );
        BlockWorldFn->UnSelectMemory();
    }

    return m_pDeepSpace;
}
Void Galaxy::DestroyDeepSpace()
{
    if ( m_pDeepSpace != NULL ) {
        BlockWorldFn->SelectMemory( TEXT("VoidSpaces") );
        Delete( m_pDeepSpace );
        m_pDeepSpace = NULL;
        BlockWorldFn->UnSelectMemory();
    }
}
