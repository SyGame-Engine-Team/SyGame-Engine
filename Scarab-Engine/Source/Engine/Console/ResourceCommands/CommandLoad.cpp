/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/ResourceCommands/CommandLoad.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Commands : Resource, Load
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
#include "CommandLoad.h"

#include "../ConsoleManager.h"

/////////////////////////////////////////////////////////////////////////////////
// CommandLoad implementation
CommandLoad::CommandLoad():
    ConsoleCommand( CONSOLECOMMAND_RESOURCE, TEXT("load"), CONSOLECOMMAND_LEVEL_DEV )
{
    // Build argument list
    m_iMinArgCount = 2;
    m_iMaxArgCount = 2;
    m_iArgCount = 2;

    m_arrArguments[0].iTypes = 0;
    m_arrArguments[0].iFlags = 0;
    m_arrArguments[0].AddType( CONSOLECOMMAND_ARG_STRING );

    m_arrArguments[1].iTypes = 0;
    m_arrArguments[1].iFlags = 0;
    m_arrArguments[1].AddType( CONSOLECOMMAND_ARG_STRING );
}
CommandLoad::~CommandLoad()
{
    // nothing to do
}

GChar * CommandLoad::GetHelp( GChar * strOutput ) const
{
    // Prototype & arguments
    strOutput = StringFn->Copy( strOutput, TEXT("Prototype = /resource load varname respath\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  varname = variable to receive loaded resource\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  respath = resource to load\n") );

    // Description
    strOutput = StringFn->Copy( strOutput, TEXT("Description = Load a resource respath to a variable varname\n") );

    return strOutput;
}

Bool CommandLoad::Execute( GChar * strOutput, const GChar * strArgList )
{
    // Get console environment
    ConsoleEnvironment * pEnv = ConsoleFn->GetEnvironment();

    // Parse arguments
    GChar strArg0[CONSOLECOMMAND_INPUT_LENGTH];
    GChar strArg1[CONSOLECOMMAND_INPUT_LENGTH];

    // Pick first argument
    strArgList = _ExtractArgument_String( strArg0, strArgList );

    // No argument case
    if ( strArgList == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Missing parameter 0 : varname ...") );
        return false;
    }

    // Pick second argument
    strArgList = _ExtractArgument_String( strArg1, strArgList );

    // One argument case
    if ( strArgList == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Missing parameter 1 : respath ...") );
        return false;
    }

    // Two arguments case
    ResourceDB * pRDB = pEnv->GetActiveRDB();
    if ( pRDB == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Can't load resource, active RDB is NULL ...") );
        return false;
    }

    RDB_ResourceInfos resInfos;
    Bool bOk = pRDB->GetResourceInfo( &resInfos, strArg1 );
    if ( !bOk ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 1 : Invalid respath ...") );
        return false;
    }

    Resource * pLoadResource = NULL;
    switch( resInfos.iType ) {
        case RESOURCE_TYPE_TEXTURE:
            // Get type from header
            RDB_ResourceHandle hResource;
            bOk = pRDB->ResourceOpen( &hResource, strArg1, true );
            if ( !bOk ) {
                strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 1 : Invalid respath ...") );
                return false;
            }
            TextureHeader texHeader;
            pRDB->ResourceRead( &hResource, sizeof(TextureHeader), (Byte*)(&texHeader) );
            pRDB->ResourceClose( &hResource );

            switch( texHeader.dwType ) {
                case GPURESOURCE_TEXTURE_1D:   pLoadResource = RenderingFn->CreateTexture1D( (PixelFormat)(texHeader.dwFormat), texHeader.dwBounds[0], texHeader.dwMipLevelCount, 1, NULL ); break;
                case GPURESOURCE_TEXTURE_2D:   pLoadResource = RenderingFn->CreateTexture2D( (PixelFormat)(texHeader.dwFormat), texHeader.dwBounds[0], texHeader.dwBounds[1], texHeader.dwMipLevelCount, 1, texHeader.dwSampleCount, NULL ); break;
                case GPURESOURCE_TEXTURE_3D:   pLoadResource = RenderingFn->CreateTexture3D( (PixelFormat)(texHeader.dwFormat), texHeader.dwBounds[0], texHeader.dwBounds[1], texHeader.dwBounds[2], texHeader.dwMipLevelCount, NULL ); break;
                case GPURESOURCE_TEXTURE_CUBE: pLoadResource = RenderingFn->CreateTextureCube( (PixelFormat)(texHeader.dwFormat), texHeader.dwBounds[0], texHeader.dwMipLevelCount, 1, NULL ); break;
                default:
                    strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 1 : Invalid resource type (corrupted) ...") );
                    return false;
            }            
            break;
        default:
            strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 1 : Invalid resource type (corrupted) ...") );
            return false;
    }

    bOk = pLoadResource->Load( pRDB->GetName(), strArg1 );
    if ( !bOk ) {
        Delete( pLoadResource );
        strOutput = StringFn->Format( strOutput, TEXT("Load error, %s resource is corrupted ..."), Resource::GetResourceTypeName(resInfos.iType) );
        return false;
    }

    ConsoleVariable * pLoadVar = pEnv->GetVariable( strArg0 );
    if ( pLoadVar == NULL ) {
        pLoadVar = pEnv->CreateVariable( strArg0 );
        Assert( pLoadVar != NULL );
        strOutput = StringFn->Copy( strOutput, TEXT("Variable didn't exist and was created automatically\n") );
    }
    pLoadVar->Affect( (ConsoleVariableType)(resInfos.iType), pLoadResource );

    strOutput = StringFn->Format( strOutput, TEXT("Loaded resource (%s) %s\n"), Resource::GetResourceTypeName(resInfos.iType), resInfos.strName );
    strOutput = StringFn->Format( strOutput, TEXT("    to variable (%s) %s"), ConsoleVariable::GetTypeName(pLoadVar->GetType()), pLoadVar->GetName() );
    return true;
}

