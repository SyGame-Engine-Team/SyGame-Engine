/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/ResourceCommands/CommandExport.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Commands : Resource, Export
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
#include "CommandExport.h"

#include "../ConsoleManager.h"

/////////////////////////////////////////////////////////////////////////////////
// CommandExport implementation
CommandExport::CommandExport():
    ConsoleCommand( CONSOLECOMMAND_RESOURCE, TEXT("export"), CONSOLECOMMAND_LEVEL_DEV )
{
    // Build argument list
    m_iMinArgCount = 3;
    m_iMaxArgCount = 3;
    m_iArgCount = 3;

    m_arrArguments[0].iTypes = 0;
    m_arrArguments[0].iFlags = 0;
    m_arrArguments[0].AddType( CONSOLECOMMAND_ARG_STRING );

    m_arrArguments[1].iTypes = 0;
    m_arrArguments[1].iFlags = 0;
    m_arrArguments[1].AddType( CONSOLECOMMAND_ARG_STRING );

    m_arrArguments[2].iTypes = 0;
    m_arrArguments[2].iFlags = 0;
    m_arrArguments[2].AddType( CONSOLECOMMAND_ARG_STRING );
}
CommandExport::~CommandExport()
{
    // nothing to do
}

GChar * CommandExport::GetHelp( GChar * strOutput ) const
{
    // Prototype & arguments
    strOutput = StringFn->Copy( strOutput, TEXT("Prototype = /resource export respath filepath filetype\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  respath = path to resource to export\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  filepath = path to export destination file\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  filetype = type of export destination file\n") );

    // Description
    strOutput = StringFn->Copy( strOutput, TEXT("Description = Export resource respath to filetype filepath\n") );

    return strOutput;
}

Bool CommandExport::Execute( GChar * strOutput, const GChar * strArgList )
{
    // Get console environment
    ConsoleEnvironment * pEnv = ConsoleFn->GetEnvironment();

    // Parse arguments
    GChar strArg0[CONSOLECOMMAND_INPUT_LENGTH];
    GChar strArg1[CONSOLECOMMAND_INPUT_LENGTH];
    GChar strArg2[CONSOLECOMMAND_INPUT_LENGTH];

    // Pick first argument
    strArgList = _ExtractArgument_String( strArg0, strArgList );

    // No argument case
    if ( strArgList == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Missing parameter 0 : respath ...") );
        return false;
    }

    // Pick second argument
    strArgList = _ExtractArgument_String( strArg1, strArgList );

    // One argument case
    if ( strArgList == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Missing parameter 1 : filepath ...") );
        return false;
    }

    // Pick third argument
    strArgList = _ExtractArgument_String( strArg2, strArgList );

    // Two arguments case
    if ( strArgList == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Missing parameter 2 : filetype ...") );
        return false;
    }

    // Three arguments case
    ResourceDB * pRDB = pEnv->GetActiveRDB();
    if ( pRDB == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Can't export resource, active RDB is NULL ...") );
        return false;
    }

    if ( pEnv->HasOpenedResource() ) {
        strOutput = StringFn->Format( strOutput, TEXT("Closing opened resource : %s"), pEnv->GetOpenedResourceName() );
        pEnv->CloseResource();
    }

    RDB_ResourceInfos resInfos;
    Bool bOk = pRDB->GetResourceInfo( &resInfos, strArg0 );
    if ( !bOk ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 0 : Invalid resource ...") );
        return false;
    }

    GChar strFileName[FILENAME_LENGTH];
    StringFn->Format( strFileName, TEXT("%s/%s"), RDB_PATH_EXPORTS, strArg1 );

    ResourceType iType = resInfos.iType;
    switch( iType ) {
        case RESOURCE_TYPE_TEXTURE: {
                TextureFileType iTexType = TextureFile::GetType( strArg2 );
                switch( iTexType ) {
                    case TEXTUREFILE_TEX: {
                            TextureTEX texExport;
                            bOk = texExport.Export( pRDB->GetName(), strArg0 );
                            if ( !bOk ) {
                                strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 0 : Invalid respath ...") );
                                return false;
                            }
                            bOk = texExport.Save( strFileName );
                            if ( !bOk ) {
                                strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 1 : Invalid tex file ...") );
                                return false;
                            }
                        } break;
                    case TEXTUREFILE_BMP: {
                            TextureBMP bmpExport;
                            bOk = bmpExport.Export( pRDB->GetName(), strArg0 );
                            if ( !bOk ) {
                                strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 0 : Invalid respath ...") );
                                return false;
                            }
                            bOk = bmpExport.Save( strFileName );
                            if ( !bOk ) {
                                strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 1 : Invalid bmp file ...") );
                                return false;
                            }
                        } break;
                    default:
                        strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 2 : Invalid texture file type ...") );
                        return false;
                }
            } break;
        default:
            strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 0 : Invalid resource type ...") );
            return false;
    }

    strOutput = StringFn->Format( strOutput, TEXT("Exported (%s) %s\n"), Resource::GetResourceTypeName(iType), strArg0 );
    strOutput = StringFn->Format( strOutput, TEXT("  to %s"), strFileName );
    return true;
}


