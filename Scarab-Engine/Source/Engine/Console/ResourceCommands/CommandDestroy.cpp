/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/ResourceCommands/CommandDestroy.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Commands : Resource, Destroy
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
#include "CommandDestroy.h"

#include "../ConsoleManager.h"

/////////////////////////////////////////////////////////////////////////////////
// CommandDestroy implementation
CommandDestroy::CommandDestroy():
    ConsoleCommand( CONSOLECOMMAND_RESOURCE, TEXT("destroy"), CONSOLECOMMAND_LEVEL_DEV )
{
    // Build argument list
    m_iMinArgCount = 1;
    m_iMaxArgCount = 1;
    m_iArgCount = 1;

    m_arrArguments[0].iTypes = 0;
    m_arrArguments[0].iFlags = 0;
    m_arrArguments[0].AddType( CONSOLECOMMAND_ARG_STRING );
}
CommandDestroy::~CommandDestroy()
{
    // nothing to do
}

GChar * CommandDestroy::GetHelp( GChar * strOutput ) const
{
    // Prototype & arguments
    strOutput = StringFn->Copy( strOutput, TEXT("Prototype = /resource destroy (respath|dirpath)\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  respath = relative path for resource destruction\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  dirpath = relative path for directory destruction\n") );

    // Description
    strOutput = StringFn->Copy( strOutput, TEXT("Description = Destroy directory or resource given by respath/dirpath\n") );

    return strOutput;
}

Bool CommandDestroy::Execute( GChar * strOutput, const GChar * strArgList )
{
    // Get console environment
    ConsoleEnvironment * pEnv = ConsoleFn->GetEnvironment();

    // Parse arguments
    GChar strArg0[CONSOLECOMMAND_INPUT_LENGTH];

    // Pick first argument
    strArgList = _ExtractArgument_String( strArg0, strArgList );

    // No argument case
    if ( strArgList == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Missing parameter 0 : respath / dirpath ...") );
        return false;
    }

    // One argument case
    ResourceDB * pRDB = pEnv->GetActiveRDB();
    if ( pRDB == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Can't destroy resource / directory, active RDB is NULL ...") );
        return false;
    }

    if ( pEnv->HasOpenedResource() ) {
        strOutput = StringFn->Format( strOutput, TEXT("Closing opened resource : %s"), pEnv->GetOpenedResourceName() );
        pEnv->CloseResource();
    }

    Bool bOk = pRDB->DestroyResource( strArg0 );
    if ( bOk ) {
        strOutput = StringFn->Format( strOutput, TEXT("Destroyed Resource : %s"), strArg0 );
        return true;
    }

    bOk = pRDB->DestroyDirectory( strArg0 );
    if ( bOk ) {
        strOutput = StringFn->Format( strOutput, TEXT("Destroyed Directory : %s"), strArg0 );
        return true;
    }

    strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 0 : Invalid respath / dirpath ...") );
    return false;
}

