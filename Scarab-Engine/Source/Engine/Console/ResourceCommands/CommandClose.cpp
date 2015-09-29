/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/ResourceCommands/CommandClose.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Commands : Resource, Close
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
#include "CommandClose.h"

#include "../ConsoleManager.h"

/////////////////////////////////////////////////////////////////////////////////
// CommandClose implementation
CommandClose::CommandClose():
    ConsoleCommand( CONSOLECOMMAND_RESOURCE, TEXT("close"), CONSOLECOMMAND_LEVEL_DEV )
{
    // Build argument list
    m_iMinArgCount = 0;
    m_iMaxArgCount = 1;
    m_iArgCount = 1;

    m_arrArguments[0].iTypes = 0;
    m_arrArguments[0].iFlags = 0;
    m_arrArguments[0].AddType( CONSOLECOMMAND_ARG_STRING );
    m_arrArguments[0].SetOptional( true );
}
CommandClose::~CommandClose()
{
    // nothing to do
}

GChar * CommandClose::GetHelp( GChar * strOutput ) const
{
    // Prototype & arguments
    strOutput = StringFn->Copy( strOutput, TEXT("Prototype = /resource close (filepath)\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  filepath = close RDB file, full path\n") );

    // Description
    strOutput = StringFn->Copy( strOutput, TEXT("Description = Close RDB file given by filepath or\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  close current open RDB resource in active file\n") );

    return strOutput;
}

Bool CommandClose::Execute( GChar * strOutput, const GChar * strArgList )
{
    // Get console environment
    ConsoleEnvironment * pEnv = ConsoleFn->GetEnvironment();

    // Parse arguments
    GChar strArg0[CONSOLECOMMAND_INPUT_LENGTH];

    // Pick first argument
    strArgList = _ExtractArgument_String( strArg0, strArgList );

    // No argument case
    if ( strArgList == NULL ) {
        ResourceDB * pActiveRDB = pEnv->GetActiveRDB();
        if ( pActiveRDB == NULL ) {
            strOutput = StringFn->Copy( strOutput, TEXT("Can't close resource, active RDB is NULL ...") );
            return false;
        }
        if ( !(pEnv->HasOpenedResource()) ) {
            strOutput = StringFn->Copy( strOutput, TEXT("No current open resource ...") );
            return false;
        }

        strOutput = StringFn->Format( strOutput, TEXT("Closed resource : %s"), pEnv->GetOpenedResourceName() );
        pEnv->CloseResource();
        return true;
    }

    // One argument case
    ResourceDB * pRDB = ResourceFn->GetRDB( strArg0 );
    if ( pRDB == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 0 : Invalid filepath or not opened ...") );
        return false;
    }

    ResourceDB * pActiveRDB = pEnv->GetActiveRDB();
    if ( pRDB == pActiveRDB ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 0 : Can't close active RDB ...") );
        return false;
    }

    if ( StringFn->Cmp(pRDB->GetName(), RDB_MASTER) == 0 ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 0 : Can't close master RDB ...") );
        return false;
    }

    strOutput = StringFn->Format( strOutput, TEXT("Closed RDB : %s"), pRDB->GetName() );
    ResourceFn->CloseRDB( pRDB->GetName() );
    return true;
}

