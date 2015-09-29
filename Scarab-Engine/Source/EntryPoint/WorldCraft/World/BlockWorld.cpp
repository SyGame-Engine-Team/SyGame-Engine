/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/World/BlockWorld.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World base interface
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
#include "BlockWorld.h"

/////////////////////////////////////////////////////////////////////////////////
// UIDs implementation
RegionUID::RegionUID():
    vRegionCoords()
{
    iDimendionID = INVALID_OFFSET;
    iGalaxyID = INVALID_OFFSET;
    iStarSystemID = INVALID_OFFSET;
    iOrbit = INVALID_OFFSET;
    iRegionLayer = REGION_LAYER_INVALID;
    iMoonID = INVALID_OFFSET;
}
RegionUID::RegionUID( const RegionUID & rhs ):
    vRegionCoords( rhs.vRegionCoords )
{
    iDimendionID = rhs.iDimendionID;
    iGalaxyID = rhs.iGalaxyID;
    iStarSystemID = rhs.iStarSystemID;
    iOrbit = rhs.iOrbit;
    iRegionLayer = rhs.iRegionLayer;
    iMoonID = rhs.iMoonID;
}

AreaUID::AreaUID():
    hRegionUID(), vAreaCoords()
{
    // nothing to do
}
AreaUID::AreaUID( const AreaUID & rhs ):
    hRegionUID( rhs.hRegionUID ), vAreaCoords( rhs.vAreaCoords )
{
    // nothing to do
}

BlockSpaceUID::BlockSpaceUID():
    hAreaUID(), vBlockSpaceCoords()
{
    // nothing to do
}
BlockSpaceUID::BlockSpaceUID( const BlockSpaceUID & rhs ):
    hAreaUID( rhs.hAreaUID ), vBlockSpaceCoords( rhs.vBlockSpaceCoords )
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// BlockWorldManager implementation
BlockWorldManager::BlockWorldManager():
    Manager<BlockWorldManager>(), m_mapDimensions(), m_mapDirtyRegions()
{
    m_strWorldName[0] = NULLBYTE;
}
BlockWorldManager::~BlockWorldManager()
{
    // nothing to do
}

Void BlockWorldManager::Initialize()
{
    m_mapDimensions.UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
    m_mapDimensions.SetComparator( _Compare_DimensionID );
    m_mapDimensions.Create();

    m_mapDirtyRegions.UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
    m_mapDirtyRegions.SetComparator( _Compare_RegionUID );
    m_mapDirtyRegions.Create();
}
Void BlockWorldManager::Cleanup()
{
    Bool bSync = SynchronizeWorld();
    Assert( bSync && m_mapDirtyRegions.IsEmpty() );
    m_mapDirtyRegions.Destroy();

    EnumDimensions();
    Dimension * pDimension = EnumNextDimension();
    while( pDimension != NULL ) {
        DestroyDimension( pDimension->GetID() );
        pDimension = EnumNextDimension();
    }
    m_mapDimensions.Destroy();
}

Dimension * BlockWorldManager::CreateDimension( DimensionID iDimensionID )
{
    if ( m_mapDimensions.Contains(iDimensionID) )
        return m_mapDimensions[iDimensionID];

    BlockWorldFn->SelectMemory( TEXT("Dimensions") );
    Dimension * pDimension = New Dimension( iDimensionID );
    BlockWorldFn->UnSelectMemory();

    Bool bInserted = m_mapDimensions.Insert( iDimensionID, pDimension );
    Assert( bInserted );

    return pDimension;
}
Void BlockWorldManager::DestroyDimension( DimensionID iDimensionID )
{
    if ( m_mapDimensions.Contains(iDimensionID) ) {
        Dimension * pDimension = m_mapDimensions[iDimensionID];

        BlockWorldFn->SelectMemory( TEXT("Dimensions") );
        Delete( pDimension );
        BlockWorldFn->UnSelectMemory();

        Bool bRemoved = m_mapDimensions.Remove( iDimensionID );
        Assert( bRemoved );
    }
}

Void BlockWorldManager::DestroyWorld()
{
    EnumDimensions();
    Dimension * pDimension = EnumNextDimension();
    while( pDimension != NULL ) {
        DestroyDimension( pDimension->GetID() );
        pDimension = EnumNextDimension();
    }
}

Region * BlockWorldManager::GetRegion( const RegionUID & hRegionUID ) const
{
    Dimension * pDimension = GetDimension( hRegionUID.iDimendionID );

    // OblivionSpace
    if ( hRegionUID.iGalaxyID == INVALID_OFFSET ) {
        Assert( hRegionUID.iRegionLayer == REGION_LAYER_OBLIVIONSPACE );
        VoidSpace * pOblivionSpace = pDimension->GetOblivionSpace();
        return pOblivionSpace->GetRegion( hRegionUID.vRegionCoords );
    }

    Galaxy * pGalaxy = pDimension->GetGalaxy( hRegionUID.iGalaxyID );

    // DeepSpace
    if ( hRegionUID.iStarSystemID == INVALID_OFFSET ) {
        Assert( hRegionUID.iRegionLayer == REGION_LAYER_DEEPSPACE );
        VoidSpace * pDeepSpace = pGalaxy->GetDeepSpace();
        return pDeepSpace->GetRegion( hRegionUID.vRegionCoords );
    }

    StarSystem * pStarSystem = pGalaxy->GetStarSystem( hRegionUID.iStarSystemID );

    // FarSpace
    if ( hRegionUID.iOrbit == INVALID_OFFSET ) {
        Assert( hRegionUID.iRegionLayer == REGION_LAYER_FARSPACE );
        VoidSpace * pFarSpace = pStarSystem->GetFarSpace();
        return pFarSpace->GetRegion( hRegionUID.vRegionCoords );
    }

    Planet * pPlanet;
    if ( hRegionUID.iOrbit == STAR_ORBIT_PRIMARY )
        pPlanet = pStarSystem->GetPrimaryStar();
    else if ( hRegionUID.iOrbit == STAR_ORBIT_BINARY )
        pPlanet = pStarSystem->GetBinaryStar();
    else
        pPlanet = pStarSystem->GetPlanet( hRegionUID.iOrbit );

    // OuterSpace
    if ( hRegionUID.iRegionLayer == REGION_LAYER_OUTERSPACE ) {
        VoidSpace * pOuterSpace = pPlanet->GetOuterSpace();
        return pOuterSpace->GetRegion( hRegionUID.vRegionCoords );
    }

    // Moons
    if ( hRegionUID.iRegionLayer == REGION_LAYER_MOONS )
        return pPlanet->GetMoonRegion( hRegionUID.iMoonID, hRegionUID.vRegionCoords );

    // Underground/Surface/Atmosphere
    return pPlanet->GetRegion( hRegionUID.iRegionLayer, hRegionUID.vRegionCoords );
}

Region * BlockWorldManager::CreateRegion( const RegionUID & hRegionUID )
{
    Dimension * pDimension = GetDimension( hRegionUID.iDimendionID );

    // OblivionSpace
    if ( hRegionUID.iGalaxyID == INVALID_OFFSET ) {
        Assert( hRegionUID.iRegionLayer == REGION_LAYER_OBLIVIONSPACE );
        VoidSpace * pOblivionSpace = pDimension->GetOblivionSpace();
        return pOblivionSpace->CreateRegion( hRegionUID.vRegionCoords );
    }

    Galaxy * pGalaxy = pDimension->GetGalaxy( hRegionUID.iGalaxyID );

    // DeepSpace
    if ( hRegionUID.iStarSystemID == INVALID_OFFSET ) {
        Assert( hRegionUID.iRegionLayer == REGION_LAYER_DEEPSPACE );
        VoidSpace * pDeepSpace = pGalaxy->GetDeepSpace();
        return pDeepSpace->CreateRegion( hRegionUID.vRegionCoords );
    }

    StarSystem * pStarSystem = pGalaxy->GetStarSystem( hRegionUID.iStarSystemID );

    // FarSpace
    if ( hRegionUID.iOrbit == INVALID_OFFSET ) {
        Assert( hRegionUID.iRegionLayer == REGION_LAYER_FARSPACE );
        VoidSpace * pFarSpace = pStarSystem->GetFarSpace();
        return pFarSpace->CreateRegion( hRegionUID.vRegionCoords );
    }

    Planet * pPlanet;
    if ( hRegionUID.iOrbit == STAR_ORBIT_PRIMARY )
        pPlanet = pStarSystem->GetPrimaryStar();
    else if ( hRegionUID.iOrbit == STAR_ORBIT_BINARY )
        pPlanet = pStarSystem->GetBinaryStar();
    else
        pPlanet = pStarSystem->GetPlanet( hRegionUID.iOrbit );

    // OuterSpace
    if ( hRegionUID.iRegionLayer == REGION_LAYER_OUTERSPACE ) {
        VoidSpace * pOuterSpace = pPlanet->GetOuterSpace();
        return pOuterSpace->CreateRegion( hRegionUID.vRegionCoords );
    }

    // Moons
    if ( hRegionUID.iRegionLayer == REGION_LAYER_MOONS )
        return pPlanet->CreateMoonRegion( hRegionUID.iMoonID, hRegionUID.vRegionCoords );

    // Underground/Surface/Atmosphere
    return pPlanet->CreateRegion( hRegionUID.iRegionLayer, hRegionUID.vRegionCoords );
}

Bool BlockWorldManager::LoadArea( const AreaUID & hAreaUID )
{
    GChar strRegionPath[PATHNAME_LENGTH + 1];
    _GetRegionPath( strRegionPath, hAreaUID.hRegionUID );

    Area * pArea = CreateArea( hAreaUID );

    Bool bOK = _LoadArea( pArea, strRegionPath );
    Assert( bOK );

    // Remove from dirty area
    _ClearDirtyArea( hAreaUID );

    return true;
}
Bool BlockWorldManager::LoadRegion( const RegionUID & hRegionUID )
{
    GChar strLayerPath[PATHNAME_LENGTH + 1];
    GChar * strPathPtr = _GetRegionLayerPath( strLayerPath, hRegionUID );

    Region * pRegion = CreateRegion( hRegionUID );

    Bool bOK = _LoadRegion( pRegion, strLayerPath, strPathPtr );
    Assert( bOK );

    // Remove from dirty region
    _ClearDirtyRegion( hRegionUID );

    return true;
}
Bool BlockWorldManager::LoadWorld()
{
    const GChar * strTmp = NULL;
    Bool bOK;

    // World directory
    GChar strAbsolutePath[PATHNAME_LENGTH + 1];
    GChar * strPathPtr = _GetWorldPath( strAbsolutePath );

    // Read World file
    GChar strWorldFile[PATHNAME_LENGTH + 1];
    StringFn->Format( strWorldFile, TEXT("%s/world.xml"), strAbsolutePath );

    XMLDocument * pXMLDoc = XMLDocument::CreateDocument( strWorldFile );
    Assert( StringFn->Cmp(pXMLDoc->GetTagName(), TEXT("WorldDescriptor")) == 0 );
    
        // Create all dimensions
        XMLNode * pDimensionArray = pXMLDoc->GetChildByTag( TEXT("DimensionArray"), 0 );
        Assert( pDimensionArray != NULL );

        UInt iDimensionCount = pDimensionArray->GetChildCount();
        for( UInt i = 0; i < iDimensionCount; ++i ) {
            XMLNode * pDimensionNode = pDimensionArray->GetChildByTag( TEXT("Dimension"), i );
            strTmp = pDimensionNode->GetAttribute( TEXT("ID") )->GetValue();
            DimensionID iDimensionID = (DimensionID)( StringFn->ToUInt(strTmp) );
            CreateDimension( iDimensionID );
        }

    // Done
    XMLDocument::DestroyDocument( pXMLDoc );
    pXMLDoc = NULL;

    // Load all dimensions (Defer to handle tunnels)
    EnumDimensions();
    Dimension * pDimension = EnumNextDimension();
    while( pDimension != NULL ) {
        bOK = _LoadDimension( pDimension, strAbsolutePath, strPathPtr );
        Assert( bOK );

        pDimension = EnumNextDimension();
    }

    // Clear dirty regions
    _ClearDirtyRegions();

    return true;
}

Bool BlockWorldManager::SaveArea( const AreaUID & hAreaUID )
{
    Bool bOK;

    GChar strRegionPath[PATHNAME_LENGTH + 1];
    _GetRegionPath( strRegionPath, hAreaUID.hRegionUID );

    // Backup previous one (if any)
    GChar strBackupPath[PATHNAME_LENGTH + 1];
    StringFn->Format( strBackupPath, TEXT("%s_Backup"), strRegionPath );
    SystemFn->MoveFile( strBackupPath, strRegionPath );

    // Save
    bOK = _SaveArea( GetArea(hAreaUID), strRegionPath );
    Assert( bOK );

    // Destroy Backup (if any)
    SystemFn->DestroyDirectory( strBackupPath );

    // Remove from dirty area
    _ClearDirtyArea( hAreaUID );

    return true;
}
Bool BlockWorldManager::SaveRegion( const RegionUID & hRegionUID )
{
    Bool bOK;

    GChar strLayerPath[PATHNAME_LENGTH + 1];
    GChar * strPathPtr = _GetRegionLayerPath( strLayerPath, hRegionUID );

    // Backup previous one (if any)
    GChar strBackupPath[PATHNAME_LENGTH + 1];
    StringFn->Format( strBackupPath, TEXT("%s_Backup"), strLayerPath );
    SystemFn->MoveFile( strBackupPath, strLayerPath );

    // Save
    bOK = _SaveRegion( GetRegion(hRegionUID), strLayerPath, strPathPtr );
    Assert( bOK );

    // Destroy Backup (if any)
    SystemFn->DestroyDirectory( strBackupPath );

    // Remove from dirty region
    _ClearDirtyRegion( hRegionUID );

    return true;
}
Bool BlockWorldManager::SaveWorld()
{
    GChar strTmp[64];
    Bool bOK;

    // World directory
    GChar strAbsolutePath[PATHNAME_LENGTH + 1];
    GChar * strPathPtr = _GetWorldPath( strAbsolutePath );

    // Backup previous one (if any)
    GChar strBackupPath[PATHNAME_LENGTH + 1];
    StringFn->Format( strBackupPath, TEXT("%s_Backup"), strAbsolutePath );
    SystemFn->MoveFile( strBackupPath, strAbsolutePath, true );

    // Start fresh
    bOK = SystemFn->CreateDirectory( strAbsolutePath );
    Assert( bOK );

    // Write World file
    GChar strWorldFile[PATHNAME_LENGTH + 1];
    StringFn->Format( strWorldFile, TEXT("%s/world.xml"), strAbsolutePath );

    XMLDocument * pXMLDoc = XMLDocument::CreateDocument( TEXT("WorldDescriptor"), TEXT("1.0a") );
        XMLNode * pDimensionArray = XMLDocument::CreateNode( TEXT("DimensionArray"), false );
        
            // Save all dimensions
            EnumDimensions();
            Dimension * pDimension = EnumNextDimension();
            while( pDimension != NULL ) {
                XMLNode * pDimensionNode = XMLDocument::CreateNode( TEXT("Dimension"), true );
                    StringFn->Format( strTmp, TEXT("%d"), pDimension->GetID() );
                    pDimensionNode->CreateAttribute( TEXT("ID"), strTmp );
                pDimensionArray->AppendChild( pDimensionNode );

                bOK = _SaveDimension( pDimension, strAbsolutePath, strPathPtr );
                Assert( bOK );

                pDimension = EnumNextDimension();
            }

        pXMLDoc->AppendChild( pDimensionArray );

    // Done
    bOK = pXMLDoc->Render( strWorldFile );
    Assert( bOK );
    XMLDocument::DestroyDocument( pXMLDoc );
    pXMLDoc = NULL;

    // Destroy Backup (if any)
    SystemFn->DestroyDirectory( strBackupPath );

    // Clear dirty regions
    _ClearDirtyRegions();

    return true;
}

Bool BlockWorldManager::SynchronizeRegion( const RegionUID & hRegionUID )
{
    if ( m_mapDirtyRegions.Contains(hRegionUID) ) {
        DirtyAreaMap * pDirtyAreaMap = m_mapDirtyRegions[hRegionUID];
        AreaUID hAreaUID;
        hAreaUID.hRegionUID = hRegionUID;

        DirtyAreaMap::Iterator itArea = pDirtyAreaMap->Begin();
        while( !(itArea.IsNull()) ) {
            hAreaUID.vAreaCoords = itArea.Get();

            Bool bOK = SaveArea( hAreaUID );
            Assert( bOK );

            ++itArea;
        }

        Bool bRemoved = m_mapDirtyRegions.Remove( hRegionUID );
        Assert( bRemoved );

        pDirtyAreaMap->Destroy();

        SelectMemory( TEXT("Scratch") );
        Delete( pDirtyAreaMap );
        pDirtyAreaMap = NULL;
        UnSelectMemory();
    }

    return true;
}
Bool BlockWorldManager::SynchronizeWorld()
{
    while( !(m_mapDirtyRegions.IsEmpty()) ) {
        DirtyRegionMap::Iterator itRegion = m_mapDirtyRegions.Begin();

        Bool bOK = SynchronizeRegion( itRegion.GetKey() );
        Assert( bOK );
    }

    return true;
}

Void BlockWorldManager::RegisterUpdateArea( const AreaUID & hAreaUID )
{
}
Void BlockWorldManager::UnRegisterUpdateArea( const AreaUID & hAreaUID )
{
}

Void BlockWorldManager::UpdateWorld()
{

}

/////////////////////////////////////////////////////////////////////////////////

GChar * BlockWorldManager::_GetRegionLayerPath( GChar * outPath, RegionLayer iRegionLayer, MoonID iMoonID ) const
{
    switch( iRegionLayer ) {
        case REGION_LAYER_OBLIVIONSPACE: return StringFn->Format( outPath, TEXT("/_OblivionSpace") );
        case REGION_LAYER_DEEPSPACE:     return StringFn->Format( outPath, TEXT("/_DeepSpace") );
        case REGION_LAYER_FARSPACE:      return StringFn->Format( outPath, TEXT("/_FarSpace") );
        case REGION_LAYER_MOONS:         return StringFn->Format( outPath, TEXT("/Moon-%d"), iMoonID );
        case REGION_LAYER_OUTERSPACE:    return StringFn->Format( outPath, TEXT("/_OuterSpace") );
        case REGION_LAYER_ATMOSPHERE:    return StringFn->Format( outPath, TEXT("/Atmosphere") );
        case REGION_LAYER_SURFACE:       return StringFn->Format( outPath, TEXT("/Surface") );
        case REGION_LAYER_UNDERGROUND:   return StringFn->Format( outPath, TEXT("/Underground") );
        default: Assert(false);  return NULL;
    }
}
GChar * BlockWorldManager::_GetRegionLayerPath( GChar * outPath, const RegionUID & hRegionUID ) const
{
    outPath = _GetWorldPath( outPath );
    outPath = _GetDimensionPath( outPath, hRegionUID.iDimendionID );
    if ( hRegionUID.iGalaxyID == INVALID_OFFSET ) {
        Assert( hRegionUID.iRegionLayer == REGION_LAYER_OBLIVIONSPACE );
        return _GetRegionLayerPath( outPath, REGION_LAYER_OBLIVIONSPACE, INVALID_OFFSET );
    }
    outPath = _GetGalaxyPath( outPath, hRegionUID.iGalaxyID );
    if ( hRegionUID.iStarSystemID == INVALID_OFFSET ) {
        Assert( hRegionUID.iRegionLayer == REGION_LAYER_DEEPSPACE );
        return _GetRegionLayerPath( outPath, REGION_LAYER_DEEPSPACE, INVALID_OFFSET );
    }
    outPath = _GetStarSystemPath( outPath, hRegionUID.iStarSystemID );
    if ( hRegionUID.iOrbit == INVALID_OFFSET ) {
        Assert( hRegionUID.iRegionLayer == REGION_LAYER_FARSPACE );
        return _GetRegionLayerPath( outPath, REGION_LAYER_FARSPACE, INVALID_OFFSET );
    }
    outPath = _GetPlanetPath( outPath, hRegionUID.iOrbit );
    return _GetRegionLayerPath( outPath, hRegionUID.iRegionLayer, hRegionUID.iMoonID );
}

Bool BlockWorldManager::_LoadDimension( Dimension * pDimension, GChar * strAbsolutePath, GChar * strPathPtr )
{
    const GChar * strTmp = NULL;
    Bool bOK;

    // Dimension directory
    strPathPtr = _GetDimensionPath( strPathPtr, pDimension->GetID() );

    // Load OblivionSpace
    bOK = _LoadVoidSpace( pDimension->CreateOblivionSpace(), strAbsolutePath, strPathPtr );
    Assert( bOK );

    // Read Dimension file
    GChar strDimensionFile[PATHNAME_LENGTH + 1];
    StringFn->Format( strDimensionFile, TEXT("%s/dimension.xml"), strAbsolutePath );

    XMLDocument * pXMLDoc = XMLDocument::CreateDocument( strDimensionFile );
    Assert( StringFn->Cmp(pXMLDoc->GetTagName(), TEXT("DimensionDescriptor")) == 0 );
    
        // Load Tunnels
        XMLNode * pTunnelArray = pXMLDoc->GetChildByTag( TEXT("TunnelArray"), 0 );
        Assert( pTunnelArray != NULL );

        UInt iTunnelCount = pTunnelArray->GetChildCount();
        for( UInt i = 0; i < iTunnelCount; ++i ) {
            XMLNode * pTunnelNode = pTunnelArray->GetChildByTag( TEXT("Tunnel"), i );
            strTmp = pTunnelNode->GetAttribute( TEXT("ID") )->GetValue();
            Dimension * pTargetDimension = GetDimension( (DimensionID)(StringFn->ToUInt(strTmp)) );
            pDimension->CreateTunnel( pTargetDimension );
        }

        // Create all galaxies
        XMLNode * pGalaxyArray = pXMLDoc->GetChildByTag( TEXT("GalaxyArray"), 0 );
        Assert( pGalaxyArray != NULL );

        UInt iGalaxyCount = pGalaxyArray->GetChildCount();
        for( UInt i = 0; i < iGalaxyCount; ++i ) {
            XMLNode * pGalaxyNode = pGalaxyArray->GetChildByTag( TEXT("Galaxy"), i );
            strTmp = pGalaxyNode->GetAttribute( TEXT("ID") )->GetValue();
            GalaxyID iGalaxyID = (GalaxyID)( StringFn->ToUInt(strTmp) );
            pDimension->CreateGalaxy( iGalaxyID );
        }

    // Done
    XMLDocument::DestroyDocument( pXMLDoc );
    pXMLDoc = NULL;

    // Load all galaxies (Defer to handle tunnels)
    pDimension->EnumGalaxies();
    Galaxy * pGalaxy = pDimension->EnumNextGalaxy();
    while( pGalaxy != NULL ) {
        bOK = _LoadGalaxy( pGalaxy, strAbsolutePath, strPathPtr );
        Assert( bOK );

        pGalaxy = pDimension->EnumNextGalaxy();
    }

    return true;
}
Bool BlockWorldManager::_LoadGalaxy( Galaxy * pGalaxy, GChar * strAbsolutePath, GChar * strPathPtr )
{
    const GChar * strTmp = NULL;
    Bool bOK;

    // Galaxy directory
    strPathPtr = _GetGalaxyPath( strPathPtr, pGalaxy->GetID() );

    // Load DeepSpace
    bOK = _LoadVoidSpace( pGalaxy->CreateDeepSpace(), strAbsolutePath, strPathPtr );
    Assert( bOK );

    // Read Galaxy file
    GChar strGalaxyFile[PATHNAME_LENGTH + 1];
    StringFn->Format( strGalaxyFile, TEXT("%s/galaxy.xml"), strAbsolutePath );

    XMLDocument * pXMLDoc = XMLDocument::CreateDocument( strGalaxyFile );
    Assert( StringFn->Cmp(pXMLDoc->GetTagName(), TEXT("GalaxyDescriptor")) == 0 );
    
        // Load Tunnels
        XMLNode * pTunnelArray = pXMLDoc->GetChildByTag( TEXT("TunnelArray"), 0 );
        Assert( pTunnelArray != NULL );

        UInt iTunnelCount = pTunnelArray->GetChildCount();
        for( UInt i = 0; i < iTunnelCount; ++i ) {
            XMLNode * pTunnelNode = pTunnelArray->GetChildByTag( TEXT("Tunnel"), i );
            strTmp = pTunnelNode->GetAttribute( TEXT("ID") )->GetValue();
            Galaxy * pTargetGalaxy = pGalaxy->GetParentDimension()->GetGalaxy( (GalaxyID)(StringFn->ToUInt(strTmp)) );
            pGalaxy->CreateTunnel( pTargetGalaxy );
        }

        // Create all star systems
        XMLNode * pStarSystemArray = pXMLDoc->GetChildByTag( TEXT("StarSystemArray"), 0 );
        Assert( pStarSystemArray != NULL );

        UInt iStarSystemCount = pStarSystemArray->GetChildCount();
        for( UInt i = 0; i < iStarSystemCount; ++i ) {
            XMLNode * pStarSystemNode = pStarSystemArray->GetChildByTag( TEXT("StarSystem"), i );
            strTmp = pStarSystemNode->GetAttribute( TEXT("ID") )->GetValue();
            StarSystemID iStarSystemID = (StarSystemID)( StringFn->ToUInt(strTmp) );
            strTmp = pStarSystemNode->GetAttribute( TEXT("OrbitCount") )->GetValue();
            UInt iOrbitCount = (UInt)( StringFn->ToUInt(strTmp) );
            pGalaxy->CreateStarSystem( iStarSystemID, iOrbitCount );
        }

    // Done
    XMLDocument::DestroyDocument( pXMLDoc );
    pXMLDoc = NULL;

    // Load all star systems (Defer to handle tunnels)
    pGalaxy->EnumStarSystems();
    StarSystem * pStarSystem = pGalaxy->EnumNextStarSystem();
    while( pStarSystem != NULL ) {
        bOK = _LoadStarSystem( pStarSystem, strAbsolutePath, strPathPtr );
        Assert( bOK );

        pStarSystem = pGalaxy->EnumNextStarSystem();
    }

    return true;
}
Bool BlockWorldManager::_LoadStarSystem( StarSystem * pStarSystem, GChar * strAbsolutePath, GChar * strPathPtr )
{
    const GChar * strTmp = NULL;
    Bool bOK;

    // StarSystem directory
    strPathPtr = _GetStarSystemPath( strPathPtr, pStarSystem->GetID() );

    // Load FarSpace
    bOK = _LoadVoidSpace( pStarSystem->CreateFarSpace(), strAbsolutePath, strPathPtr );
    Assert( bOK );

    // Read StarSystem file
    GChar strStarSystemFile[PATHNAME_LENGTH + 1];
    StringFn->Format( strStarSystemFile, TEXT("%s/starsystem.xml"), strAbsolutePath );

    XMLDocument * pXMLDoc = XMLDocument::CreateDocument( strStarSystemFile );
    Assert( StringFn->Cmp(pXMLDoc->GetTagName(), TEXT("StarSystemDescriptor")) == 0 );
    
        // Load Tunnels
        XMLNode * pTunnelArray = pXMLDoc->GetChildByTag( TEXT("TunnelArray"), 0 );
        Assert( pTunnelArray != NULL );

        UInt iTunnelCount = pTunnelArray->GetChildCount();
        for( UInt i = 0; i < iTunnelCount; ++i ) {
            XMLNode * pTunnelNode = pTunnelArray->GetChildByTag( TEXT("Tunnel"), i );
            strTmp = pTunnelNode->GetAttribute( TEXT("ID") )->GetValue();
            StarSystem * pTargetStarSystem = pStarSystem->GetParentGalaxy()->GetStarSystem( (StarSystemID)(StringFn->ToUInt(strTmp)) );
            pStarSystem->CreateTunnel( pTargetStarSystem );
        }

        // Load stars
        XMLNode * pPrimaryStarNode = pXMLDoc->GetChildByTag( TEXT("PrimaryStar"), 0 );
        if ( StringFn->Cmp( pPrimaryStarNode->GetAttribute(TEXT("Exists"))->GetValue(), TEXT("true") ) == 0 ) {
            Planet * pPrimaryStar = pStarSystem->CreatePrimaryStar();

            bOK = _LoadPlanet( pPrimaryStar, strAbsolutePath, strPathPtr );
            Assert( bOK );
        }
        XMLNode * pBinaryStarNode = pXMLDoc->GetChildByTag( TEXT("BinaryStar"), 0 );
        if ( StringFn->Cmp( pBinaryStarNode->GetAttribute(TEXT("Exists"))->GetValue(), TEXT("true") ) == 0 ) {
            Planet * pBinaryStar = pStarSystem->CreateBinaryStar();

            bOK = _LoadPlanet( pBinaryStar, strAbsolutePath, strPathPtr );
            Assert( bOK );
        }

        // Load all planets
        XMLNode * pPlanetArray = pXMLDoc->GetChildByTag( TEXT("PlanetArray"), 0 );
        Assert( pPlanetArray != NULL );

        UInt iPlanetCount = pPlanetArray->GetChildCount();
        for( UInt i = 0; i < iPlanetCount; ++i ) {
            XMLNode * pPlanetNode = pPlanetArray->GetChildByTag( TEXT("Planet"), i );
            strTmp = pPlanetNode->GetAttribute( TEXT("Orbit") )->GetValue();
            UInt iOrbit = (UInt)( StringFn->ToUInt(strTmp) );
            strTmp = pPlanetNode->GetAttribute( TEXT("MoonCount") )->GetValue();
            UInt iMoonCount = (UInt)( StringFn->ToUInt(strTmp) );
            Planet * pPlanet = pStarSystem->CreatePlanet( iOrbit, iMoonCount );

            bOK = _LoadPlanet( pPlanet, strAbsolutePath, strPathPtr );
            Assert( bOK );
        }

    // Done
    XMLDocument::DestroyDocument( pXMLDoc );
    pXMLDoc = NULL;

    return true;
}
Bool BlockWorldManager::_LoadPlanet( Planet * pPlanet, GChar * strAbsolutePath, GChar * strPathPtr )
{
    const GChar * strTmp = NULL;
    Bool bOK;

    // StarSystem directory
    strPathPtr = _GetPlanetPath( strPathPtr, pPlanet->GetOrbit() );

    // Load Underground
    bOK = _LoadRegionLayer( pPlanet, REGION_LAYER_UNDERGROUND, INVALID_OFFSET, strAbsolutePath, strPathPtr );
    Assert( bOK );

    // Load Surface
    bOK = _LoadRegionLayer( pPlanet, REGION_LAYER_SURFACE, INVALID_OFFSET, strAbsolutePath, strPathPtr );
    Assert( bOK );

    // Load Atmosphere
    bOK = _LoadRegionLayer( pPlanet, REGION_LAYER_ATMOSPHERE, INVALID_OFFSET, strAbsolutePath, strPathPtr );
    Assert( bOK );

    // Load OuterSpace
    bOK = _LoadVoidSpace( pPlanet->CreateOuterSpace(), strAbsolutePath, strPathPtr );
    Assert( bOK );

    // Read Planet file
    GChar strPlanetFile[PATHNAME_LENGTH + 1];
    StringFn->Format( strPlanetFile, TEXT("%s/planet.xml"), strAbsolutePath );

    XMLDocument * pXMLDoc = XMLDocument::CreateDocument( strPlanetFile );
    Assert( StringFn->Cmp(pXMLDoc->GetTagName(), TEXT("PlanetDescriptor")) == 0 );

        // Load all moons
        XMLNode * pMoonArray = pXMLDoc->GetChildByTag( TEXT("MoonArray"), 0 );
        Assert( pMoonArray != NULL );

        UInt iMoonCount = pMoonArray->GetChildCount();
        for( UInt i = 0; i < iMoonCount; ++i ) {
            XMLNode * pMoonNode = pMoonArray->GetChildByTag( TEXT("Moon"), i );
            strTmp = pMoonNode->GetAttribute( TEXT("ID") )->GetValue();
            MoonID iMoonID = (UInt)( StringFn->ToUInt(strTmp) );
            pPlanet->CreateMoon( iMoonID );

            bOK = _LoadRegionLayer( pPlanet, REGION_LAYER_MOONS, iMoonID, strAbsolutePath, strPathPtr );
            Assert( bOK );
        }

    // Done
    XMLDocument::DestroyDocument( pXMLDoc );
    pXMLDoc = NULL;

    return true;
}
Bool BlockWorldManager::_LoadVoidSpace( VoidSpace * pVoidSpace, GChar * strAbsolutePath, GChar * strPathPtr )
{
    const GChar * strTmp = NULL;
    Bool bOK;

    // VoidSpace directory
    strPathPtr = _GetRegionLayerPath( strPathPtr, pVoidSpace->GetRegionLayer(), INVALID_OFFSET );

    // Read VoidSpace file
    GChar strVoidSpaceFile[PATHNAME_LENGTH + 1];
    StringFn->Format( strVoidSpaceFile, TEXT("%s/voidspace.xml"), strAbsolutePath );

    XMLDocument * pXMLDoc = XMLDocument::CreateDocument( strVoidSpaceFile );
    Assert( StringFn->Cmp(pXMLDoc->GetTagName(), TEXT("VoidSpaceDescriptor")) == 0 );

        // Load all regions
        XMLNode * pRegionArray = pXMLDoc->GetChildByTag( TEXT("RegionArray"), 0 );
        Assert( pRegionArray != NULL );

        UInt iRegionCount = pRegionArray->GetChildCount();
        for( UInt i = 0; i < iRegionCount; ++i ) {
            XMLNode * pRegionNode = pRegionArray->GetChildByTag( TEXT("Region"), i );
            RegionCoords vRegionCoords;
            strTmp = pRegionNode->GetAttribute( TEXT("X") )->GetValue();
            vRegionCoords.X = (Int)( StringFn->ToInt(strTmp) );
            strTmp = pRegionNode->GetAttribute( TEXT("Y") )->GetValue();
            vRegionCoords.Y = (Int)( StringFn->ToInt(strTmp) );
            Region * pRegion = pVoidSpace->CreateRegion( vRegionCoords );

            bOK = _LoadRegion( pRegion, strAbsolutePath, strPathPtr );
            Assert( bOK );
        }

    // Done
    XMLDocument::DestroyDocument( pXMLDoc );
    pXMLDoc = NULL;

    return true;
}
Bool BlockWorldManager::_LoadRegionLayer( Planet * pPlanet, RegionLayer iRegionLayer, MoonID iMoonID, GChar * strAbsolutePath, GChar * strPathPtr )
{
    const GChar * strTmp = NULL;
    Bool bOK;

    // RegionLayer directory
    strPathPtr = _GetRegionLayerPath( strPathPtr, iRegionLayer, iMoonID );

    // Read RegionLayer file
    GChar strRegionLayerFile[PATHNAME_LENGTH + 1];
    StringFn->Format( strRegionLayerFile, TEXT("%s/regionlayer.xml"), strAbsolutePath );

    XMLDocument * pXMLDoc = XMLDocument::CreateDocument( strRegionLayerFile );
    Assert( StringFn->Cmp(pXMLDoc->GetTagName(), TEXT("RegionLayerDescriptor")) == 0 );

        // Load all regions
        XMLNode * pRegionArray = pXMLDoc->GetChildByTag( TEXT("RegionArray"), 0 );
        Assert( pRegionArray != NULL );

        UInt iRegionCount = pRegionArray->GetChildCount();
        for( UInt i = 0; i < iRegionCount; ++i ) {
            XMLNode * pRegionNode = pRegionArray->GetChildByTag( TEXT("Region"), i );
            RegionCoords vRegionCoords;
            strTmp = pRegionNode->GetAttribute( TEXT("X") )->GetValue();
            vRegionCoords.X = (Int)( StringFn->ToInt(strTmp) );
            strTmp = pRegionNode->GetAttribute( TEXT("Y") )->GetValue();
            vRegionCoords.Y = (Int)( StringFn->ToInt(strTmp) );
            Region * pRegion = NULL;
            if ( iRegionLayer == REGION_LAYER_MOONS ) {
                Assert( pPlanet->HasMoon(iMoonID) );
                pRegion = pPlanet->CreateMoonRegion( iMoonID, vRegionCoords );
            } else
                pRegion = pPlanet->CreateRegion( iRegionLayer, vRegionCoords );

            bOK = _LoadRegion( pRegion, strAbsolutePath, strPathPtr );
            Assert( bOK );
        }

    // Done
    XMLDocument::DestroyDocument( pXMLDoc );
    pXMLDoc = NULL;

    return true;
}
Bool BlockWorldManager::_LoadRegion( Region * pRegion, GChar * strAbsolutePath, GChar * strPathPtr )
{
    const GChar * strTmp = NULL;
    Bool bOK;

    // Region directory
    strPathPtr = _GetRegionPath( strPathPtr, pRegion->GetRegionCoords() );

    // Read Region file
    GChar strRegionFile[PATHNAME_LENGTH + 1];
    StringFn->Format( strRegionFile, TEXT("%s/region.xml"), strAbsolutePath );

    XMLDocument * pXMLDoc = XMLDocument::CreateDocument( strRegionFile );
    Assert( StringFn->Cmp(pXMLDoc->GetTagName(), TEXT("RegionDescriptor")) == 0 );

        // Load all areas
        XMLNode * pAreaArray = pXMLDoc->GetChildByTag( TEXT("AreaArray"), 0 );
        Assert( pAreaArray != NULL );

        UInt iAreaCount = pAreaArray->GetChildCount();
        for( UInt i = 0; i < iAreaCount; ++i ) {
            XMLNode * pAreaNode = pAreaArray->GetChildByTag( TEXT("Area"), i );
            AreaCoords vAreaCoords;
            strTmp = pAreaNode->GetAttribute( TEXT("X") )->GetValue();
            vAreaCoords.X = (Int)( StringFn->ToInt(strTmp) );
            strTmp = pAreaNode->GetAttribute( TEXT("Y") )->GetValue();
            vAreaCoords.Y = (Int)( StringFn->ToInt(strTmp) );
            Area * pArea = pRegion->CreateArea( vAreaCoords );
            
            bOK = _LoadArea( pArea, strAbsolutePath );
            Assert( bOK );
        }

    // Done
    XMLDocument::DestroyDocument( pXMLDoc );
    pXMLDoc = NULL;

    return true;
}
Bool BlockWorldManager::_LoadArea( Area * pArea, GChar * strAbsolutePath )
{
    const GChar * strTmp = NULL;

    // Area file
    _GetAreaPath( strAbsolutePath, pArea->GetAreaCoords() );

    // Read Area file
    XMLDocument * pXMLDoc = XMLDocument::CreateDocument( strAbsolutePath );
    Assert( StringFn->Cmp(pXMLDoc->GetTagName(), TEXT("AreaDescriptor")) == 0 );

        // Header
        XMLNode * pHeader = pXMLDoc->GetChildByTag( TEXT("Header"), 0 );
        Assert( pHeader != NULL );

            XMLNode * pSize = pHeader->GetChildByTag( TEXT("Size"), 0 );
            strTmp = pSize->GetAttribute( TEXT("Value") )->GetValue();
            UInt iSize = (UInt)( StringFn->ToUInt(strTmp) );
            Assert( iSize == AREA_SIZE );

            XMLNode * pHeight = pHeader->GetChildByTag( TEXT("Height"), 0 );
            strTmp = pHeight->GetAttribute( TEXT("Value") )->GetValue();
            UInt iHeight = (UInt)( StringFn->ToUInt(strTmp) );
            Assert( iHeight == AREA_HEIGHT );

            XMLNode * pBlockCount = pHeader->GetChildByTag( TEXT("BlockCount"), 0 );
            strTmp = pBlockCount->GetAttribute( TEXT("Value") )->GetValue();
            UInt iBlockCount = (UInt)( StringFn->ToUInt(strTmp) );
            Assert( iBlockCount == AREA_BLOCKCOUNT );

        // Load all BlockSpaces
        XMLNode * pBlockSpaceArray = pXMLDoc->GetChildByTag( TEXT("BlockSpaceArray"), 0 );
        Assert( pBlockSpaceArray != NULL );

            UInt iBlockSpaceCount = pBlockSpaceArray->GetChildCount();
            for( UInt i = 0; i < iBlockSpaceCount; ++i ) {
                XMLNode * pBlockSpaceNode = pBlockSpaceArray->GetChildByTag( TEXT("BlockSpace"), i );
                BlockSpaceCoords vBlockSpaceCoords;
                strTmp = pBlockSpaceNode->GetAttribute( TEXT("X") )->GetValue();
                vBlockSpaceCoords.X = (Int)( StringFn->ToInt(strTmp) );
                strTmp = pBlockSpaceNode->GetAttribute( TEXT("Y") )->GetValue();
                vBlockSpaceCoords.Y = (Int)( StringFn->ToInt(strTmp) );
                strTmp = pBlockSpaceNode->GetAttribute( TEXT("Z") )->GetValue();
                vBlockSpaceCoords.Z = (Int)( StringFn->ToInt(strTmp) );
                BlockSpace * pBlockSpace = pArea->GetBlockSpace( vBlockSpaceCoords );

                // Load Block
                XMLNode * pBlockData = pBlockSpaceNode->GetChildByTag( TEXT("BlockData"), 0 );
                    XMLNode * pBlockClass = pBlockData->GetChildByTag( TEXT("Class"), 0 );
                    BlockID hBlockID;
                    strTmp = pBlockClass->GetAttribute( TEXT("ID") )->GetValue();
                    hBlockID.dwID = (Int)( StringFn->ToInt(strTmp) );
                    strTmp = pBlockClass->GetAttribute( TEXT("Specifier") )->GetValue();
                    hBlockID.dwSpecifier = (Int)( StringFn->ToInt(strTmp) );

                    Block * pBlock = BlockFn->GetBlock( hBlockID );
                    if ( pBlock != NULL ) {
                        XMLNode * pBlockOrientation = pBlockData->GetChildByTag( TEXT("Orientation"), 0 );
                        strTmp = pBlockOrientation->GetAttribute( TEXT("Value") )->GetValue();
                        BlockOrientation iBlockOrientation = (BlockOrientation)( StringFn->ToUInt(strTmp) );

                        BlockInstance * pBlockInstance = pBlock->CreateInstance( iBlockOrientation );

                        XMLNode * pBlockState = pBlockData->GetChildByTag( TEXT("State"), 0 );
                        pBlockInstance->_LoadState( pBlockState );

                        pBlockSpace->SetBlock( pBlockInstance );
                    }

                // Load MicroBlocks
                XMLNode * pMicroBlockData = pBlockSpaceNode->GetChildByTag( TEXT("MicroBlockData"), 0 );
                    XMLNode * pHasMicroBlockSpace = pMicroBlockData->GetChildByTag( TEXT("HasMicroBlockSpace"), 0 );
                    strTmp = pHasMicroBlockSpace->GetAttribute( TEXT("Value") )->GetValue();
                    if ( StringFn->Cmp(strTmp, TEXT("true")) == 0 ) {
                        XMLNode * pMicroBlockSpaceDepth = pMicroBlockData->GetChildByTag( TEXT("MicroBlockSpaceDepth"), 0 );
                        strTmp = pMicroBlockSpaceDepth->GetAttribute( TEXT("Value") )->GetValue();
                        UInt iMicroBlockSpaceDepth = (UInt)( StringFn->ToUInt(strTmp) );
                        pBlockSpace->CreateMicroBlockSpace( iMicroBlockSpaceDepth );

                        UInt iMicroBlockSpaceCount = ( pMicroBlockData->GetChildCount() - 2 );
                        for( UInt i = 0; i < iMicroBlockSpaceCount; ++i ) {
                            XMLNode * pMicroBlockSpaceNode = pMicroBlockData->GetChildByTag( TEXT("MicroBlockSpace"), 0 );
                            MicroBlockCoords vMicroBlockCoords;
                            strTmp = pMicroBlockSpaceNode->GetAttribute( TEXT("X") )->GetValue();
                            vMicroBlockCoords.X = (Int)( StringFn->ToInt(strTmp) );
                            strTmp = pMicroBlockSpaceNode->GetAttribute( TEXT("Y") )->GetValue();
                            vMicroBlockCoords.Y = (Int)( StringFn->ToInt(strTmp) );
                            strTmp = pMicroBlockSpaceNode->GetAttribute( TEXT("Z") )->GetValue();
                            vMicroBlockCoords.Z = (Int)( StringFn->ToInt(strTmp) );
                            
                            XMLNode * pMicroBlockClass = pMicroBlockSpaceNode->GetChildByTag( TEXT("Class"), 0 );
                            strTmp = pMicroBlockClass->GetAttribute( TEXT("ID") )->GetValue();
                            hBlockID.dwID = (Int)( StringFn->ToInt(strTmp) );
                            strTmp = pMicroBlockClass->GetAttribute( TEXT("Specifier") )->GetValue();
                            hBlockID.dwSpecifier = (Int)( StringFn->ToInt(strTmp) );

                            pBlock = BlockFn->GetBlock( hBlockID );
                            if ( pBlock != NULL ) {
                                XMLNode * pMicroBlockOrientation = pMicroBlockSpaceNode->GetChildByTag( TEXT("Orientation"), 0 );
                                strTmp = pMicroBlockOrientation->GetAttribute( TEXT("Value") )->GetValue();
                                BlockOrientation iBlockOrientation = (BlockOrientation)( StringFn->ToUInt(strTmp) );

                                BlockInstance * pBlockInstance = pBlock->CreateInstance( iBlockOrientation );

                                XMLNode * pBlockState = pMicroBlockSpaceNode->GetChildByTag( TEXT("State"), 0 );
                                pBlockInstance->_LoadState( pBlockState );

                                pBlockSpace->SetMicroBlock( vMicroBlockCoords, pBlockInstance );
                            }
                        }
                    }
            }

    // Done
    XMLDocument::DestroyDocument( pXMLDoc );
    pXMLDoc = NULL;

    return true;
}

Bool BlockWorldManager::_SaveDimension( Dimension * pDimension, GChar * strAbsolutePath, GChar * strPathPtr )
{
    GChar strTmp[64];
    Bool bOK;

    // Dimension directory
    strPathPtr = _GetDimensionPath( strPathPtr, pDimension->GetID() );

    bOK = SystemFn->CreateDirectory( strAbsolutePath );
    Assert( bOK );

    // Save OblivionSpace
    bOK = _SaveVoidSpace( pDimension->GetOblivionSpace(), strAbsolutePath, strPathPtr );
    Assert( bOK );

    // Write Dimension file
    GChar strDimensionFile[PATHNAME_LENGTH + 1];
    StringFn->Format( strDimensionFile, TEXT("%s/dimension.xml"), strAbsolutePath );

    XMLDocument * pXMLDoc = XMLDocument::CreateDocument( TEXT("DimensionDescriptor"), TEXT("1.0a") );

        // Save Tunnels
        XMLNode * pTunnelArray = XMLDocument::CreateNode( TEXT("TunnelArray"), false );
            pDimension->EnumTunnels();
            Dimension * pTunnel = pDimension->EnumNextTunnel();
            while( pTunnel != NULL ) {
                XMLNode * pTunnelNode = XMLDocument::CreateNode( TEXT("Tunnel"), true );
                    StringFn->Format( strTmp, TEXT("%d"), pTunnel->GetID() );
                    pTunnelNode->CreateAttribute( TEXT("ID"), strTmp );
                pTunnelArray->AppendChild( pTunnelNode );

                pTunnel = pDimension->EnumNextTunnel();
            }
        pXMLDoc->AppendChild( pTunnelArray );

        // Save all galaxies
        XMLNode * pGalaxyArray = XMLDocument::CreateNode( TEXT("GalaxyArray"), false );
            pDimension->EnumGalaxies();
            Galaxy * pGalaxy = pDimension->EnumNextGalaxy();
            while( pGalaxy != NULL ) {
                XMLNode * pGalaxyNode = XMLDocument::CreateNode( TEXT("Galaxy"), true );
                    StringFn->Format( strTmp, TEXT("%d"), pGalaxy->GetID() );
                    pGalaxyNode->CreateAttribute( TEXT("ID"), strTmp );
                pGalaxyArray->AppendChild( pGalaxyNode );

                bOK = _SaveGalaxy( pGalaxy, strAbsolutePath, strPathPtr );
                Assert( bOK );

                pGalaxy = pDimension->EnumNextGalaxy();
            }
        pXMLDoc->AppendChild( pGalaxyArray );

    // Done
    bOK = pXMLDoc->Render( strDimensionFile );
    Assert( bOK );
    XMLDocument::DestroyDocument( pXMLDoc );
    pXMLDoc = NULL;

    return true;
}
Bool BlockWorldManager::_SaveGalaxy( Galaxy * pGalaxy, GChar * strAbsolutePath, GChar * strPathPtr )
{
    GChar strTmp[64];
    Bool bOK;

    // Galaxy directory
    strPathPtr = _GetGalaxyPath( strPathPtr, pGalaxy->GetID() );

    bOK = SystemFn->CreateDirectory( strAbsolutePath );
    Assert( bOK );

    // Save DeepSpace
    bOK = _SaveVoidSpace( pGalaxy->GetDeepSpace(), strAbsolutePath, strPathPtr );
    Assert( bOK );

    // Write Galaxy file
    GChar strGalaxyFile[PATHNAME_LENGTH + 1];
    StringFn->Format( strGalaxyFile, TEXT("%s/galaxy.xml"), strAbsolutePath );

    XMLDocument * pXMLDoc = XMLDocument::CreateDocument( TEXT("GalaxyDescriptor"), TEXT("1.0a") );

        // Save Tunnels
        XMLNode * pTunnelArray = XMLDocument::CreateNode( TEXT("TunnelArray"), false );
            pGalaxy->EnumTunnels();
            Galaxy * pTunnel = pGalaxy->EnumNextTunnel();
            while( pTunnel != NULL ) {
                XMLNode * pTunnelNode = XMLDocument::CreateNode( TEXT("Tunnel"), true );
                    StringFn->Format( strTmp, TEXT("%d"), pTunnel->GetID() );
                    pTunnelNode->CreateAttribute( TEXT("ID"), strTmp );
                pTunnelArray->AppendChild( pTunnelNode );

                pTunnel = pGalaxy->EnumNextTunnel();
            }
        pXMLDoc->AppendChild( pTunnelArray );

        // Save all star systems
        XMLNode * pStarSystemArray = XMLDocument::CreateNode( TEXT("StarSystemArray"), false );
            pGalaxy->EnumStarSystems();
            StarSystem * pStarSystem = pGalaxy->EnumNextStarSystem();
            while( pStarSystem != NULL ) {
                XMLNode * pStarSystemNode = XMLDocument::CreateNode( TEXT("StarSystem"), true );
                    StringFn->Format( strTmp, TEXT("%d"), pStarSystem->GetID() );
                    pStarSystemNode->CreateAttribute( TEXT("ID"), strTmp );
                    StringFn->Format( strTmp, TEXT("%d"), pStarSystem->GetOrbitCount() );
                    pStarSystemNode->CreateAttribute( TEXT("OrbitCount"), strTmp );
                pStarSystemArray->AppendChild( pStarSystemNode );

                bOK = _SaveStarSystem( pStarSystem, strAbsolutePath, strPathPtr );
                Assert( bOK );

                pStarSystem = pGalaxy->EnumNextStarSystem();
            }
        pXMLDoc->AppendChild( pStarSystemArray );

    // Done
    bOK = pXMLDoc->Render( strGalaxyFile );
    Assert( bOK );
    XMLDocument::DestroyDocument( pXMLDoc );
    pXMLDoc = NULL;

    return true;
}
Bool BlockWorldManager::_SaveStarSystem( StarSystem * pStarSystem, GChar * strAbsolutePath, GChar * strPathPtr )
{
    GChar strTmp[64];
    Bool bOK;

    // StarSystem directory
    strPathPtr = _GetStarSystemPath( strPathPtr, pStarSystem->GetID() );

    bOK = SystemFn->CreateDirectory( strAbsolutePath );
    Assert( bOK );

    // Save FarSpace
    bOK = _SaveVoidSpace( pStarSystem->GetFarSpace(), strAbsolutePath, strPathPtr );
    Assert( bOK );

    // Write StarSystem file
    GChar strStarSystemFile[PATHNAME_LENGTH + 1];
    StringFn->Format( strStarSystemFile, TEXT("%s/starsystem.xml"), strAbsolutePath );

    XMLDocument * pXMLDoc = XMLDocument::CreateDocument( TEXT("StarSystemDescriptor"), TEXT("1.0a") );

        // Save Tunnels
        XMLNode * pTunnelArray = XMLDocument::CreateNode( TEXT("TunnelArray"), false );
            pStarSystem->EnumTunnels();
            StarSystem * pTunnel = pStarSystem->EnumNextTunnel();
            while( pTunnel != NULL ) {
                XMLNode * pTunnelNode = XMLDocument::CreateNode( TEXT("Tunnel"), true );
                    StringFn->Format( strTmp, TEXT("%d"), pTunnel->GetID() );
                    pTunnelNode->CreateAttribute( TEXT("ID"), strTmp );
                pTunnelArray->AppendChild( pTunnelNode );

                pTunnel = pStarSystem->EnumNextTunnel();
            }
        pXMLDoc->AppendChild( pTunnelArray );

        // Save stars
        Planet * pPrimaryStar = pStarSystem->GetPrimaryStar();

        XMLNode * pPrimaryStarNode = XMLDocument::CreateNode( TEXT("PrimaryStar"), true );
            pPrimaryStarNode->CreateAttribute( TEXT("Exists"), (pPrimaryStar == NULL) ? TEXT("false") : TEXT("true") );
        pXMLDoc->AppendChild( pPrimaryStarNode );

        if ( pPrimaryStar != NULL ) {
            bOK = _SavePlanet( pPrimaryStar, strAbsolutePath, strPathPtr );
            Assert( bOK );
        }
        
        Planet * pBinaryStar = pStarSystem->CreateBinaryStar();

        XMLNode * pBinaryStarNode = XMLDocument::CreateNode( TEXT("BinaryStar"), true );
            pBinaryStarNode->CreateAttribute( TEXT("Exists"), (pBinaryStar == NULL) ? TEXT("false") : TEXT("true") );
        pXMLDoc->AppendChild( pBinaryStarNode );

        if ( pBinaryStar != NULL ) {
            bOK = _SavePlanet( pBinaryStar, strAbsolutePath, strPathPtr );
            Assert( bOK );
        }

        // Save all planets
        XMLNode * pPlanetArray = XMLDocument::CreateNode( TEXT("PlanetArray"), false );
            for( UInt i = 0; i < pStarSystem->GetOrbitCount(); ++i ) {
                Planet * pPlanet = pStarSystem->GetPlanet(i);
                if ( pPlanet != NULL ) {
                    XMLNode * pPlanetNode = XMLDocument::CreateNode( TEXT("Planet"), true );
                        StringFn->Format( strTmp, TEXT("%d"), i );
                        pPlanetNode->CreateAttribute( TEXT("Orbit"), strTmp );
                        StringFn->Format( strTmp, TEXT("%d"), pPlanet->GetMoonCount() );
                        pPlanetNode->CreateAttribute( TEXT("MoonCount"), strTmp );
                    pPlanetArray->AppendChild( pPlanetNode );

                    bOK = _SavePlanet( pPlanet, strAbsolutePath, strPathPtr );
                    Assert( bOK );
                }
            }
        pXMLDoc->AppendChild( pPlanetArray );

    // Done
    bOK = pXMLDoc->Render( strStarSystemFile );
    Assert( bOK );
    XMLDocument::DestroyDocument( pXMLDoc );
    pXMLDoc = NULL;

    return true;
}
Bool BlockWorldManager::_SavePlanet( Planet * pPlanet, GChar * strAbsolutePath, GChar * strPathPtr )
{
    GChar strTmp[64];
    Bool bOK;

    // Planet directory
    strPathPtr = _GetPlanetPath( strPathPtr, pPlanet->GetOrbit() );

    bOK = SystemFn->CreateDirectory( strAbsolutePath );
    Assert( bOK );

    // Save Underground
    bOK = _SaveRegionLayer( pPlanet, REGION_LAYER_UNDERGROUND, INVALID_OFFSET, strAbsolutePath, strPathPtr );
    Assert( bOK );

    // Save Surface
    bOK = _SaveRegionLayer( pPlanet, REGION_LAYER_SURFACE, INVALID_OFFSET, strAbsolutePath, strPathPtr );
    Assert( bOK );

    // Save Atmosphere
    bOK = _SaveRegionLayer( pPlanet, REGION_LAYER_ATMOSPHERE, INVALID_OFFSET, strAbsolutePath, strPathPtr );
    Assert( bOK );

    // Save OuterSpace
    bOK = _SaveVoidSpace( pPlanet->GetOuterSpace(), strAbsolutePath, strPathPtr );
    Assert( bOK );

    // Write Planet file
    GChar strPlanetFile[PATHNAME_LENGTH + 1];
    StringFn->Format( strPlanetFile, TEXT("%s/planet.xml"), strAbsolutePath );

    XMLDocument * pXMLDoc = XMLDocument::CreateDocument( TEXT("PlanetDescriptor"), TEXT("1.0a") );
        
        // Save all moons
        XMLNode * pMoonArray = XMLDocument::CreateNode( TEXT("MoonArray"), false );
            for( MoonID iMoonID = 0; iMoonID < pPlanet->GetMoonCount(); ++iMoonID ) {
                if ( pPlanet->HasMoon(iMoonID) ) {
                    XMLNode * pMoonNode = XMLDocument::CreateNode( TEXT("Moon"), true );
                        StringFn->Format( strTmp, TEXT("%d"), iMoonID );
                        pMoonNode->CreateAttribute( TEXT("ID"), strTmp );
                    pMoonArray->AppendChild( pMoonNode );

                    bOK = _SaveRegionLayer( pPlanet, REGION_LAYER_MOONS, iMoonID, strAbsolutePath, strPathPtr );
                    Assert( bOK );
                }
            }
        pXMLDoc->AppendChild( pMoonArray );
    
    // Done
    bOK = pXMLDoc->Render( strPlanetFile );
    Assert( bOK );
    XMLDocument::DestroyDocument( pXMLDoc );
    pXMLDoc = NULL;

    return true;
}
Bool BlockWorldManager::_SaveVoidSpace( VoidSpace * pVoidSpace, GChar * strAbsolutePath, GChar * strPathPtr )
{
    GChar strTmp[64];
    Bool bOK;

    // VoidSpace directory
    strPathPtr = _GetRegionLayerPath( strPathPtr, pVoidSpace->GetRegionLayer(), INVALID_OFFSET );

    bOK = SystemFn->CreateDirectory( strAbsolutePath );
    Assert( bOK );

    // Write VoidSpace file
    GChar strVoidSpaceFile[PATHNAME_LENGTH + 1];
    StringFn->Format( strVoidSpaceFile, TEXT("%s/voidspace.xml"), strAbsolutePath );

    XMLDocument * pXMLDoc = XMLDocument::CreateDocument( TEXT("VoidSpaceDescriptor"), TEXT("1.0a") );

        // Save all regions
        XMLNode * pRegionArray = XMLDocument::CreateNode( TEXT("RegionArray"), false );
            pVoidSpace->EnumRegions();
            Region * pRegion = pVoidSpace->EnumNextRegion();
            while( pRegion != NULL ) {
                XMLNode * pRegionNode = XMLDocument::CreateNode( TEXT("Region"), true );
                    StringFn->Format( strTmp, TEXT("%d"), pRegion->GetRegionCoords().X );
                    pRegionNode->CreateAttribute( TEXT("X"), strTmp );
                    StringFn->Format( strTmp, TEXT("%d"), pRegion->GetRegionCoords().Y );
                    pRegionNode->CreateAttribute( TEXT("Y"), strTmp );
                pRegionArray->AppendChild( pRegionNode );
            
                bOK = _SaveRegion( pRegion, strAbsolutePath, strPathPtr );
                Assert( bOK );

                pRegion = pVoidSpace->EnumNextRegion();
            }
        pXMLDoc->AppendChild( pRegionArray );

    // Done
    bOK = pXMLDoc->Render( strVoidSpaceFile );
    Assert( bOK );
    XMLDocument::DestroyDocument( pXMLDoc );
    pXMLDoc = NULL;

    return true;
}
Bool BlockWorldManager::_SaveRegionLayer( Planet * pPlanet, RegionLayer iRegionLayer, MoonID iMoonID, GChar * strAbsolutePath, GChar * strPathPtr )
{
    GChar strTmp[64];
    Bool bOK;

    // Atmosphere directory
    strPathPtr = _GetRegionLayerPath( strPathPtr, iRegionLayer, iMoonID );

    bOK = SystemFn->CreateDirectory( strAbsolutePath );
    Assert( bOK );

    // Write RegionLayer file
    GChar strRegionLayerFile[PATHNAME_LENGTH + 1];
    StringFn->Format( strRegionLayerFile, TEXT("%s/regionlayer.xml"), strAbsolutePath );

    XMLDocument * pXMLDoc = XMLDocument::CreateDocument( TEXT("RegionLayerDescriptor"), TEXT("1.0a") );

        // Save all regions
        XMLNode * pRegionArray = XMLDocument::CreateNode( TEXT("RegionArray"), false );
            Region * pRegion = NULL;
            if ( iRegionLayer == REGION_LAYER_MOONS ) {
                Assert( pPlanet->HasMoon(iMoonID) );
                pPlanet->EnumMoonRegions( iMoonID );
                pRegion = pPlanet->EnumNextMoonRegion();
            } else {
                pPlanet->EnumRegions( iRegionLayer );
                pRegion = pPlanet->EnumNextRegion();
            }
            while( pRegion != NULL ) {
                XMLNode * pRegionNode = XMLDocument::CreateNode( TEXT("Region"), true );
                    StringFn->Format( strTmp, TEXT("%d"), pRegion->GetRegionCoords().X );
                    pRegionNode->CreateAttribute( TEXT("X"), strTmp );
                    StringFn->Format( strTmp, TEXT("%d"), pRegion->GetRegionCoords().Y );
                    pRegionNode->CreateAttribute( TEXT("Y"), strTmp );
                pRegionArray->AppendChild( pRegionNode );
            
                bOK = _SaveRegion( pRegion, strAbsolutePath, strPathPtr );
                Assert( bOK );

                if ( iRegionLayer == REGION_LAYER_MOONS )
                    pRegion = pPlanet->EnumNextMoonRegion();
                else
                    pRegion = pPlanet->EnumNextRegion();
            }
        pXMLDoc->AppendChild( pRegionArray );

    // Done
    bOK = pXMLDoc->Render( strRegionLayerFile );
    Assert( bOK );
    XMLDocument::DestroyDocument( pXMLDoc );
    pXMLDoc = NULL;

    return true;
}
Bool BlockWorldManager::_SaveRegion( Region * pRegion, GChar * strAbsolutePath, GChar * strPathPtr )
{
    GChar strTmp[64];
    Bool bOK;

    // Region directory
    strPathPtr = _GetRegionPath( strPathPtr, pRegion->GetRegionCoords() );

    bOK = SystemFn->CreateDirectory( strAbsolutePath );
    Assert( bOK );

    // Write Region file
    GChar strRegionFile[PATHNAME_LENGTH + 1];
    StringFn->Format( strRegionFile, TEXT("%s/region.xml"), strAbsolutePath );

    XMLDocument * pXMLDoc = XMLDocument::CreateDocument( TEXT("RegionDescriptor"), TEXT("1.0a") );

        // Save all areas
        XMLNode * pAreaArray = XMLDocument::CreateNode( TEXT("AreaArray"), false );
            AreaCoords vAreaCoords;
            for( vAreaCoords.Y = 0; vAreaCoords.Y < REGION_SIZE; ++(vAreaCoords.Y) ) {
                for( vAreaCoords.X = 0; vAreaCoords.X < REGION_SIZE; ++(vAreaCoords.X) ) {
                    Area * pArea = pRegion->GetArea( vAreaCoords );
                    if ( pArea != NULL ) {
                        XMLNode * pAreaNode = XMLDocument::CreateNode( TEXT("Area"), true );
                            StringFn->Format( strTmp, TEXT("%d"), pArea->GetAreaCoords().X );
                            pAreaNode->CreateAttribute( TEXT("X"), strTmp );
                            StringFn->Format( strTmp, TEXT("%d"), pArea->GetAreaCoords().Y );
                            pAreaNode->CreateAttribute( TEXT("Y"), strTmp );
                        pAreaArray->AppendChild( pAreaNode );

                        bOK = _SaveArea( pArea, strAbsolutePath );
                        Assert( bOK );
                    }
                }
            }
        pXMLDoc->AppendChild( pAreaArray );

    // Done
    bOK = pXMLDoc->Render( strRegionFile );
    Assert( bOK );
    XMLDocument::DestroyDocument( pXMLDoc );
    pXMLDoc = NULL;

    return true;
}
Bool BlockWorldManager::_SaveArea( Area * pArea, GChar * strAbsolutePath )
{
    GChar strTmp[64];
    Bool bOK;

    // Area file
    _GetAreaPath( strAbsolutePath, pArea->GetAreaCoords() );

    // Write Area file
    XMLDocument * pXMLDoc = XMLDocument::CreateDocument( TEXT("AreaDescriptor"), TEXT("1.0a") );

        // Header
        XMLNode * pHeader = XMLDocument::CreateNode( TEXT("Header"), false );
            XMLNode * pSize = XMLDocument::CreateNode( TEXT("Size"), true );
                StringFn->Format( strTmp, TEXT("%d"), AREA_SIZE );
                pSize->CreateAttribute( TEXT("Value"), strTmp );
            pHeader->AppendChild( pSize );

            XMLNode * pHeight = XMLDocument::CreateNode( TEXT("Height"), true );
                StringFn->Format( strTmp, TEXT("%d"), AREA_HEIGHT );
                pHeight->CreateAttribute( TEXT("Value"), strTmp );
            pHeader->AppendChild( pHeight );

            XMLNode * pBlockCount = XMLDocument::CreateNode( TEXT("BlockCount"), true );
                StringFn->Format( strTmp, TEXT("%d"), AREA_BLOCKCOUNT );
                pBlockCount->CreateAttribute( TEXT("Value"), strTmp );
            pHeader->AppendChild( pBlockCount );
        pXMLDoc->AppendChild( pHeader );

        // Save all BlockSpaces
        XMLNode * pBlockSpaceArray = XMLDocument::CreateNode( TEXT("BlockSpaceArray"), false );
            BlockSpaceCoords vBlockSpaceCoords;
            for( vBlockSpaceCoords.Z = 0; vBlockSpaceCoords.Z < AREA_HEIGHT; ++(vBlockSpaceCoords.Z) ) {
                for( vBlockSpaceCoords.Y = 0; vBlockSpaceCoords.Y < AREA_SIZE; ++(vBlockSpaceCoords.Y) ) {
                    for( vBlockSpaceCoords.X = 0; vBlockSpaceCoords.X < AREA_SIZE; ++(vBlockSpaceCoords.X) ) {
                        BlockSpace * pBlockSpace = pArea->GetBlockSpace( vBlockSpaceCoords );
                        BlockInstance * pBlockInstance = pBlockSpace->GetBlock();
                        Bool bHasMicroBlockSpace = pBlockSpace->HasMicroBlockSpace();

                        // BlockSpace
                        XMLNode * pBlockSpaceNode = XMLDocument::CreateNode( TEXT("BlockSpace"), false );
                        StringFn->Format( strTmp, TEXT("%d"), vBlockSpaceCoords.X );
                        pBlockSpaceNode->CreateAttribute( TEXT("X"), strTmp );
                        StringFn->Format( strTmp, TEXT("%d"), vBlockSpaceCoords.Y );
                        pBlockSpaceNode->CreateAttribute( TEXT("Y"), strTmp );
                        StringFn->Format( strTmp, TEXT("%d"), vBlockSpaceCoords.Z );
                        pBlockSpaceNode->CreateAttribute( TEXT("Z"), strTmp );

                            // Block
                            XMLNode * pBlockData = XMLDocument::CreateNode( TEXT("BlockData"), false );
                                XMLNode * pBlockClass = XMLDocument::CreateNode( TEXT("Class"), true );
                                    if ( pBlockInstance == NULL ) {
                                        pBlockClass->CreateAttribute( TEXT("ID"), TEXT("0") );
                                        pBlockClass->CreateAttribute( TEXT("Specifier"), TEXT("0") );
                                    } else {
                                        StringFn->Format( strTmp, TEXT("%d"), pBlockInstance->GetBlock()->GetID().dwID );
                                        pBlockClass->CreateAttribute( TEXT("ID"), strTmp );
                                        StringFn->Format( strTmp, TEXT("%d"), pBlockInstance->GetBlock()->GetID().dwSpecifier );
                                        pBlockClass->CreateAttribute( TEXT("Specifier"), strTmp );
                                    }
                                pBlockData->AppendChild( pBlockClass );

                                XMLNode * pBlockOrientation = XMLDocument::CreateNode( TEXT("Orientation"), true );
                                    StringFn->Format( strTmp, TEXT("%d"), (UInt)( pBlockInstance->GetOrientation() ) );
                                    pBlockOrientation->CreateAttribute( TEXT("Value"), strTmp );
                                pBlockData->AppendChild( pBlockOrientation );

                                XMLNode * pBlockState = XMLDocument::CreateNode( TEXT("State"), false );
                                    pBlockInstance->_SaveState( pBlockState );
                                pBlockData->AppendChild( pBlockState );
                            pBlockSpaceNode->AppendChild( pBlockData );

                            // MicroBlocks
                            XMLNode * pMicroBlockData = XMLDocument::CreateNode( TEXT("MicroBlockData"), false );
                                XMLNode * pHasMicroBlockSpace = XMLDocument::CreateNode( TEXT("HasMicroBlockSpace"), true );
                                    StringFn->Format( strTmp, TEXT("%s"), bHasMicroBlockSpace ? TEXT("true") : TEXT("false") );
                                    pHasMicroBlockSpace->CreateAttribute( TEXT("Value"), strTmp );
                                pMicroBlockData->AppendChild( pHasMicroBlockSpace );

                                if ( bHasMicroBlockSpace ) {
                                    XMLNode * pMicroBlockSpaceDepth = XMLDocument::CreateNode( TEXT("MicroBlockSpaceDepth"), true );
                                        StringFn->Format( strTmp, TEXT("%d"), pBlockSpace->GetMicroBlockSpaceDepth() );
                                        pMicroBlockSpaceDepth->CreateAttribute( TEXT("Value"), strTmp );
                                    pMicroBlockData->AppendChild( pMicroBlockSpaceDepth );                                

                                    MicroBlockCoords vMicroBlockCoords;
                                    for( vMicroBlockCoords.Z = 0; vMicroBlockCoords.Z < (Int)(pBlockSpace->GetMicroBlockSpaceDepth()); ++(vMicroBlockCoords.Z) ) {
                                        for( vMicroBlockCoords.Y = 0; vMicroBlockCoords.Y < (Int)(pBlockSpace->GetMicroBlockSpaceDepth()); ++(vMicroBlockCoords.Y) ) {
                                            for( vMicroBlockCoords.X = 0; vMicroBlockCoords.X < (Int)(pBlockSpace->GetMicroBlockSpaceDepth()); ++(vMicroBlockCoords.X) ) {
                                                BlockInstance * pMicroBlockInstance = pBlockSpace->GetMicroBlock( vMicroBlockCoords );
                                                
                                                XMLNode * pMicroBlockSpaceNode = XMLDocument::CreateNode( TEXT("MicroBlockSpace"), false );
                                                StringFn->Format( strTmp, TEXT("%d"), vMicroBlockCoords.X );
                                                pMicroBlockSpaceNode->CreateAttribute( TEXT("X"), strTmp );
                                                StringFn->Format( strTmp, TEXT("%d"), vMicroBlockCoords.Y );
                                                pMicroBlockSpaceNode->CreateAttribute( TEXT("Y"), strTmp );
                                                StringFn->Format( strTmp, TEXT("%d"), vMicroBlockCoords.Z );
                                                pMicroBlockSpaceNode->CreateAttribute( TEXT("Z"), strTmp );

                                                    XMLNode * pMicroBlockClass = XMLDocument::CreateNode( TEXT("Class"), true );
                                                        if ( pMicroBlockInstance == NULL ) {
                                                            pMicroBlockClass->CreateAttribute( TEXT("ID"), TEXT("0") );
                                                            pMicroBlockClass->CreateAttribute( TEXT("Specifier"), TEXT("0") );
                                                            StringFn->Format( strTmp, TEXT("0-0") ); // Empty
                                                        } else {
                                                            StringFn->Format( strTmp, TEXT("%d"), pMicroBlockInstance->GetBlock()->GetID().dwID );
                                                            pMicroBlockClass->CreateAttribute( TEXT("ID"), strTmp );
                                                            StringFn->Format( strTmp, TEXT("%d"), pMicroBlockInstance->GetBlock()->GetID().dwSpecifier );
                                                            pMicroBlockClass->CreateAttribute( TEXT("Specifier"), strTmp );
                                                        }
                                                    pMicroBlockSpaceNode->AppendChild( pMicroBlockClass );

                                                    XMLNode * pMicroBlockOrientation = XMLDocument::CreateNode( TEXT("Orientation"), true );
                                                        StringFn->Format( strTmp, TEXT("%d"), (UInt)( pMicroBlockInstance->GetOrientation() ) );
                                                        pMicroBlockOrientation->CreateAttribute( TEXT("Value"), strTmp );
                                                    pMicroBlockSpaceNode->AppendChild( pMicroBlockOrientation );

                                                    XMLNode * pMicroBlockState = XMLDocument::CreateNode( TEXT("State"), false );
                                                        pMicroBlockInstance->_SaveState( pMicroBlockState );
                                                    pMicroBlockSpaceNode->AppendChild( pMicroBlockState );

                                                pMicroBlockData->AppendChild( pMicroBlockSpaceNode );
                                            }
                                        }
                                    }
                                }
                            pBlockSpaceNode->AppendChild( pMicroBlockData );

                        pBlockSpaceArray->AppendChild( pBlockSpaceNode );
                    }
                }
            }
        pXMLDoc->AppendChild( pBlockSpaceArray );

    // Done
    bOK = pXMLDoc->Render( strAbsolutePath );
    Assert( bOK );
    XMLDocument::DestroyDocument( pXMLDoc );
    pXMLDoc = NULL;

    return true;
}

Void BlockWorldManager::_MarkDirtyArea( const AreaUID & hAreaUID )
{
    Bool bInserted;
    DirtyAreaMap * pDirtyAreaMap;

    if ( m_mapDirtyRegions.Contains(hAreaUID.hRegionUID) ) {
        pDirtyAreaMap = m_mapDirtyRegions[hAreaUID.hRegionUID];
        if ( pDirtyAreaMap->Contains(hAreaUID.vAreaCoords) )
            return;
    } else {
        SelectMemory( TEXT("Scratch") );
        pDirtyAreaMap = New DirtyAreaMap();
        UnSelectMemory();

        pDirtyAreaMap->UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
        pDirtyAreaMap->SetComparator( _Compare_AreaCoords );
        pDirtyAreaMap->Create();

        bInserted = m_mapDirtyRegions.Insert( hAreaUID.hRegionUID, pDirtyAreaMap );
        Assert( bInserted );
    }

    bInserted = pDirtyAreaMap->Insert( hAreaUID.vAreaCoords );
    Assert( bInserted );
}
Void BlockWorldManager::_ClearDirtyArea( const AreaUID & hAreaUID )
{
    if ( m_mapDirtyRegions.Contains(hAreaUID.hRegionUID) ) {
        DirtyAreaMap * pDirtyAreaMap = m_mapDirtyRegions[hAreaUID.hRegionUID];
        if ( pDirtyAreaMap->Contains(hAreaUID.vAreaCoords) ) {
            Bool bRemoved = pDirtyAreaMap->Remove( hAreaUID.vAreaCoords );
            Assert( bRemoved );
        }
    }
}
Void BlockWorldManager::_ClearDirtyRegion( const RegionUID & hRegionUID )
{
    if ( m_mapDirtyRegions.Contains(hRegionUID) ) {
        DirtyAreaMap * pDirtyAreaMap = NULL;
        Bool bRemoved = m_mapDirtyRegions.Remove( hRegionUID, &pDirtyAreaMap );
        Assert( bRemoved && pDirtyAreaMap != NULL );

        pDirtyAreaMap->Destroy();

        SelectMemory( TEXT("Scratch") );
        Delete( pDirtyAreaMap );
        pDirtyAreaMap = NULL;
        UnSelectMemory();
    }
}
Void BlockWorldManager::_ClearDirtyRegions()
{
    while( !(m_mapDirtyRegions.IsEmpty()) ) {
        DirtyRegionMap::Iterator itRegion = m_mapDirtyRegions.Begin();

        DirtyAreaMap * pDirtyAreaMap = NULL;
        Bool bRemoved = m_mapDirtyRegions.Remove( itRegion.GetKey(), &pDirtyAreaMap );
        Assert( bRemoved && pDirtyAreaMap != NULL );

        pDirtyAreaMap->Destroy();

        SelectMemory( TEXT("Scratch") );
        Delete( pDirtyAreaMap );
        pDirtyAreaMap = NULL;
        UnSelectMemory();
    }
}

