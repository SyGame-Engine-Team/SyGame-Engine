/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/ResourceCommands/CommandRename.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Commands : Resource, Rename
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
#include "CommandRename.h"

#include "../ConsoleManager.h"

/////////////////////////////////////////////////////////////////////////////////
// CommandRename implementation
CommandRename::CommandRename():
    ConsoleCommand( CONSOLECOMMAND_RESOURCE, TEXT("rename"), CONSOLECOMMAND_LEVEL_DEV )
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
CommandRename::~CommandRename()
{
    // nothing to do
}

GChar * CommandRename::GetHelp( GChar * strOutput ) const
{
    // Prototype & arguments
    strOutput = StringFn->Copy( strOutput, TEXT("Prototype = /resource rename (respath|dirpath) name\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  respath/dirpath = relative path for resource/directory\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  name            = new resource/directory name\n") );

    // Description
    strOutput = StringFn->Copy( strOutput, TEXT("Description = Rename directory or resource given by respath/dirpath to name\n") );

    return strOutput;
}

Bool CommandRename::Execute( GChar * strOutput, const GChar * strArgList )
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
        strOutput = StringFn->Copy( strOutput, TEXT("Missing parameter 0 : respath / dirpath ...") );
        return false;
    }

    // Pick second argument
    strArgList = _ExtractArgument_String( strArg1, strArgList );

    // One argument case
    if ( strArgList == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Missing parameter 1 : name ...") );
        return false;
    }

    // Two arguments case
    ResourceDB * pRDB = pEnv->GetActiveRDB();
    if ( pRDB == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Can't rename resource / directory, active RDB is NULL ...") );
        return false;
    }

    if ( pEnv->HasOpenedResource() ) {
        strOutput = StringFn->Format( strOutput, TEXT("Closing opened resource : %s"), pEnv->GetOpenedResourceName() );
        pEnv->CloseResource();
    }

    Bool bOk = pRDB->RenameResource( strArg0, strArg1 );
    if ( bOk ) {
        strOutput = StringFn->Format( strOutput, TEXT("Renamed resource %s to %s"), strArg0, strArg1 );
        return true;
    }

    bOk = pRDB->RenameDirectory( strArg0, strArg1 );
    if ( bOk ) {
        strOutput = StringFn->Format( strOutput, TEXT("Renamed directory %s to %s"), strArg0, strArg1 );
        return true;
    }

    strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 0/1 : Invalid respath / dirpath or new name already exists ...") );
    return false;
}

