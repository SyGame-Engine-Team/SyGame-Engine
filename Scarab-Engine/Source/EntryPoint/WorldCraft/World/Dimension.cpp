/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/World/Dimension.cpp
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
// Includes
#include "Dimension.h"

#include "BlockWorld.h"

/////////////////////////////////////////////////////////////////////////////////
// Dimension implementation
Dimension::Dimension( DimensionID iDimensionID ):
    m_mapTunnels(), m_itEnumerateTunnel(), m_mapGalaxies(), m_itEnumerateGalaxy()
{
    // Dimension data
    m_iDimensionID = iDimensionID;

    // Adjacent linkage : Tunnels (sparse)
    m_mapTunnels.UseMemoryContext( BlockWorldFn->GetMemoryContext(), TEXT("Scratch") );
    m_mapTunnels.SetComparator( _Compare_DimensionID );
    m_mapTunnels.Create();

    // Create content
    m_mapGalaxies.UseMemoryContext( BlockWorldFn->GetMemoryContext(), TEXT("Scratch") );
    m_mapGalaxies.SetComparator( _Compare_GalaxyID );
    m_mapGalaxies.Create();

    m_pOblivionSpace = NULL;
}
Dimension::~Dimension()
{
    // Destroy content
    DestroyOblivionSpace();

    EnumGalaxies();
    Galaxy * pGalaxy = EnumNextGalaxy();
    while( pGalaxy != NULL ) {
        DestroyGalaxy( pGalaxy->GetID() );
        pGalaxy = EnumNextGalaxy();
    }
    m_mapGalaxies.Destroy();

    // Adjacent linkage : Tunnels (sparse)
    m_mapTunnels.Destroy();
}

DimensionID Dimension::CreateTunnel( Dimension * pTargetDimension )
{
    Assert( pTargetDimension != this );
    if ( m_mapTunnels.Contains(pTargetDimension->m_iDimensionID) )
        return pTargetDimension->m_iDimensionID;

    Bool bInserted = m_mapTunnels.Insert( pTargetDimension->m_iDimensionID, pTargetDimension );
    Assert( bInserted );

    return pTargetDimension->m_iDimensionID;
}
Void Dimension::DestroyTunnel( DimensionID iDimensionID )
{
    if ( m_mapTunnels.Contains(iDimensionID) ) {
        Bool bRemoved = m_mapTunnels.Remove( iDimensionID );
        Assert( bRemoved );
    }
}

Galaxy * Dimension::CreateGalaxy( GalaxyID iGalaxyID )
{
    if ( m_mapGalaxies.Contains(iGalaxyID) )
        return m_mapGalaxies[iGalaxyID];

    BlockWorldFn->SelectMemory( TEXT("Galaxies") );
    Galaxy * pGalaxy = New Galaxy( this, iGalaxyID );
    BlockWorldFn->UnSelectMemory();

    Bool bInserted = m_mapGalaxies.Insert( iGalaxyID, pGalaxy );
    Assert( bInserted );

    return pGalaxy;
}
Void Dimension::DestroyGalaxy( GalaxyID iGalaxyID )
{
    if ( m_mapGalaxies.Contains(iGalaxyID) ) {
        Galaxy * pGalaxy = m_mapGalaxies[iGalaxyID];

        BlockWorldFn->SelectMemory( TEXT("Galaxies") );
        Delete( pGalaxy );
        BlockWorldFn->UnSelectMemory();

        Bool bRemoved = m_mapGalaxies.Remove( iGalaxyID );
        Assert( bRemoved );
    }
}

VoidSpace * Dimension::CreateOblivionSpace()
{
    if ( m_pOblivionSpace == NULL ) {
        BlockWorldFn->SelectMemory( TEXT("VoidSpaces") );
        m_pOblivionSpace = New VoidSpace( this );
        BlockWorldFn->UnSelectMemory();
    }

    return m_pOblivionSpace;
}
Void Dimension::DestroyOblivionSpace()
{
    if ( m_pOblivionSpace != NULL ) {
        BlockWorldFn->SelectMemory( TEXT("VoidSpaces") );
        Delete( m_pOblivionSpace );
        m_pOblivionSpace = NULL;
        BlockWorldFn->UnSelectMemory();
    }
}
