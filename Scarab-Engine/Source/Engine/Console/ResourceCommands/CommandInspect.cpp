/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/ResourceCommands/CommandInspect.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Commands : Resource, Inspect
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "CommandInspect.h"

#include "../ConsoleManager.h"

/////////////////////////////////////////////////////////////////////////////////
// CommandInspect implementation
CommandInspect::CommandInspect():
    ConsoleCommand( CONSOLECOMMAND_RESOURCE, TEXT("inspect"), CONSOLECOMMAND_LEVEL_DEV )
{
    // Build argument list
    m_iMinArgCount = 0;
    m_iMaxArgCount = 0;
    m_iArgCount = 0;
}
CommandInspect::~CommandInspect()
{
    // nothing to do
}

GChar * CommandInspect::GetHelp( GChar * strOutput ) const
{
    // Prototype & arguments
    strOutput = StringFn->Copy( strOutput, TEXT("Prototype = /resource inspect\n") );

    // Description
    strOutput = StringFn->Copy( strOutput, TEXT("Description = Inspect opened resource, gives both rdb infos\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  and resource's header data\n") );

    return strOutput;
}

Bool CommandInspect::Execute( GChar * strOutput, const GChar * /*strArgList*/ )
{
    // Get console environment
    ConsoleEnvironment * pEnv = ConsoleFn->GetEnvironment();

    // No argument case
    ResourceDB * pRDB = pEnv->GetActiveRDB();
    if ( pRDB == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Can't inspect resource, active RDB is NULL ...") );
        return false;
    }

    if ( !(pEnv->HasOpenedResource()) ) {
        strOutput = StringFn->Copy( strOutput, TEXT("No current open resource ...") );
        return false;
    }

    RDB_ResourceHandle * pResource = pEnv->GetOpenedResource();

    RDB_ResourceInfos resInfos;
    pRDB->GetResourceInfo( &resInfos, pResource );

    strOutput = StringFn->Copy( strOutput, TEXT("RDB handle infos :\n") );
    strOutput = StringFn->Format( strOutput, TEXT("  Parent ID = %d\n"), resInfos.iParentID );
    strOutput = StringFn->Format( strOutput, TEXT("  Name     = %s\n"), resInfos.strName );
    strOutput = StringFn->Format( strOutput, TEXT("  Type     = %s\n"), Resource::GetResourceTypeName(resInfos.iType) );
    strOutput = StringFn->Format( strOutput, TEXT("  DataSize = %d\n"), resInfos.iDataSize );
    strOutput = StringFn->Format( strOutput, TEXT("  Flags    = %x\n"), resInfos.iFlags );

    UInt iSavedOffset = pRDB->ResourceTell( pResource );
    pRDB->ResourceSeekBegin( pResource );

    strOutput = StringFn->Copy( strOutput, TEXT("Header infos :\n") );
    switch( resInfos.iType ) {
        case RESOURCE_TYPE_TEXTURE: {
                TextureHeader texHeader;
                pRDB->ResourceRead( pResource, sizeof(TextureHeader), (Byte*)(&texHeader) );

                strOutput = StringFn->Format( strOutput, TEXT("  DataSize  = %d\n"), texHeader.dwDataSize );
                strOutput = StringFn->Format( strOutput, TEXT("  Type      = %d\n"), texHeader.dwType );
                strOutput = StringFn->Format( strOutput, TEXT("  Format    = %d\n"), texHeader.dwFormat );
                strOutput = StringFn->Format( strOutput, TEXT("  MipLevels = %d\n"), texHeader.dwMipLevelCount );
                strOutput = StringFn->Format( strOutput, TEXT("  Width     = %d\n"), texHeader.dwBounds[0] );
                if ( texHeader.dwType != GPURESOURCE_TEXTURE_1D )
                    strOutput = StringFn->Format( strOutput, TEXT("  Height   = %d\n"), texHeader.dwBounds[1] );
                if ( texHeader.dwType == GPURESOURCE_TEXTURE_3D )
                    strOutput = StringFn->Format( strOutput, TEXT("  Depth    = %d\n"), texHeader.dwBounds[2] );
            } break;
        default: Assert(false); return false;
    }

    pRDB->ResourceSeekAbs( pResource, iSavedOffset );

    return true;
}

