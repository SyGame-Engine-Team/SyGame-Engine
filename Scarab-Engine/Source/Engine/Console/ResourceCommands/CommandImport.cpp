/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/ResourceCommands/CommandImport.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Commands : Resource, Import
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
#include "CommandImport.h"

#include "../ConsoleManager.h"

/////////////////////////////////////////////////////////////////////////////////
// CommandImport implementation
CommandImport::CommandImport():
    ConsoleCommand( CONSOLECOMMAND_RESOURCE, TEXT("import"), CONSOLECOMMAND_LEVEL_DEV )
{
    // Build argument list
    m_iMinArgCount = 4;
    m_iMaxArgCount = 4;
    m_iArgCount = 4;

    m_arrArguments[0].iTypes = 0;
    m_arrArguments[0].iFlags = 0;
    m_arrArguments[0].AddType( CONSOLECOMMAND_ARG_STRING );

    m_arrArguments[1].iTypes = 0;
    m_arrArguments[1].iFlags = 0;
    m_arrArguments[1].AddType( CONSOLECOMMAND_ARG_STRING );

    m_arrArguments[2].iTypes = 0;
    m_arrArguments[2].iFlags = 0;
    m_arrArguments[2].AddType( CONSOLECOMMAND_ARG_STRING );

    m_arrArguments[3].iTypes = 0;
    m_arrArguments[3].iFlags = 0;
    m_arrArguments[3].AddType( CONSOLECOMMAND_ARG_STRING );
}
CommandImport::~CommandImport()
{
    // nothing to do
}

GChar * CommandImport::GetHelp( GChar * strOutput ) const
{
    // Prototype & arguments
    strOutput = StringFn->Copy( strOutput, TEXT("Prototype = /resource import respath restype filepath filetype\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  respath = pathname to import resource to\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  restype = resource type of the import\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  filepath = path to import source file\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  filetype = type of import source file\n") );

    // Description
    strOutput = StringFn->Copy( strOutput, TEXT("Description = Import filetype filepath to resource restype respath\n") );

    return strOutput;
}

Bool CommandImport::Execute( GChar * strOutput, const GChar * strArgList )
{
    // Get console environment
    ConsoleEnvironment * pEnv = ConsoleFn->GetEnvironment();

    // Parse arguments
    GChar strArg0[CONSOLECOMMAND_INPUT_LENGTH];
    GChar strArg1[CONSOLECOMMAND_INPUT_LENGTH];
    GChar strArg2[CONSOLECOMMAND_INPUT_LENGTH];
    GChar strArg3[CONSOLECOMMAND_INPUT_LENGTH];

    // Pick first argument
    strArgList = _ExtractArgument_String( strArg0, strArgList );

    // No argument case
    if ( strArgList == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Missing parameter 0 : respath ...") );
        return false;
    }

    // Pick second argument
    strArgList = _ExtractArgument_String( strArg1, strArgList );

    // One arguments case
    if ( strArgList == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Missing parameter 1 : restype ...") );
        return false;
    }

    // Pick third argument
    strArgList = _ExtractArgument_String( strArg2, strArgList );

    // Two arguments case
    if ( strArgList == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Missing parameter 2 : filepath ...") );
        return false;
    }

    // Pick fourth argument
    strArgList = _ExtractArgument_String( strArg3, strArgList );

    // Three arguments case
    if ( strArgList == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Missing parameter 3 : filetype ...") );
        return false;
    }

    // Four arguments case
    ResourceDB * pRDB = pEnv->GetActiveRDB();
    if ( pRDB == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Can't import resource, active RDB is NULL ...") );
        return false;
    }

    if ( pEnv->HasOpenedResource() ) {
        strOutput = StringFn->Format( strOutput, TEXT("Closing opened resource : %s"), pEnv->GetOpenedResourceName() );
        pEnv->CloseResource();
    }

    GChar strFileName[FILENAME_LENGTH];
    StringFn->Format( strFileName, TEXT("%s/%s"), RDB_PATH_IMPORTS, strArg2 );

    ResourceType iType = Resource::GetResourceType( strArg1 );
    switch( iType ) {
        case RESOURCE_TYPE_TEXTURE: {
                TextureFileType iTexType = TextureFile::GetType( strArg3 );
                switch( iTexType ) {
                    case TEXTUREFILE_TEX: {
                            TextureTEX texImport;
                            Bool bOk = texImport.Load( strFileName );
                            if ( !bOk ) {
                                strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 2 : Invalid tex file ...") );
                                return false;
                            }
                            bOk = texImport.Import( pRDB->GetName(), strArg0 );
                            if ( !bOk ) {
                                strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 0 : Invalid respath ...") );
                                return false;
                            }
                        } break;
                    case TEXTUREFILE_BMP: {
                            TextureBMP bmpImport;
                            Bool bOk = bmpImport.Load( strFileName );
                            if ( !bOk ) {
                                strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 2 : Invalid bmp file ...") );
                                return false;
                            }
                            bOk = bmpImport.Import( pRDB->GetName(), strArg0 );
                            if ( !bOk ) {
                                strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 0 : Invalid respath ...") );
                                return false;
                            }
                        } break;
                    default:
                        strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 3 : Invalid texture file type ...") );
                        return false;
                }
            } break;
        default:
            strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 1 : Invalid resource type ...") );
            return false;
    }

    strOutput = StringFn->Format( strOutput, TEXT("Imported (%s) %s\n"), strArg1, strArg0 );
    strOutput = StringFn->Format( strOutput, TEXT("  from %s"), strFileName );
    return true;
}

