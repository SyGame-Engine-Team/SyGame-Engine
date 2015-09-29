/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/ResourceCommands/CommandRDB.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Commands : Resource, RDB
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
#include "CommandRDB.h"

#include "../ConsoleManager.h"

/////////////////////////////////////////////////////////////////////////////////
// CommandRDB implementation
CommandRDB::CommandRDB():
    ConsoleCommand( CONSOLECOMMAND_RESOURCE, TEXT("rdb"), CONSOLECOMMAND_LEVEL_DEV )
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
CommandRDB::~CommandRDB()
{
    // nothing to do
}

GChar * CommandRDB::GetHelp( GChar * strOutput ) const
{
    // Prototype & arguments
    strOutput = StringFn->Copy( strOutput, TEXT("Prototype = /resource rdb (rdbname|null)\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  rdbname = (optional) RDB file name with extension\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  null   = (optional) Set active RDB to NULL\n") );

    // Description
    strOutput = StringFn->Copy( strOutput, TEXT("Description = Switches active RDB file or prints opened RDB\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  list (also marks active one) when no argument is provided\n") );

    return strOutput;
}

Bool CommandRDB::Execute( GChar * strOutput, const GChar * strArgList )
{
    // Get console environment
    ConsoleEnvironment * pEnv = ConsoleFn->GetEnvironment();

    // Parse arguments
    GChar strArg0[CONSOLECOMMAND_INPUT_LENGTH];

    // Pick first argument
    strArgList = _ExtractArgument_String( strArg0, strArgList );

    // No argument case
    if ( strArgList == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Opened RDB list :\n") );
        ResourceFn->EnumRDBs();
        const ResourceDB * pRDB = ResourceFn->EnumNextRDB();
        while( pRDB != NULL ) {
            if ( pRDB == pEnv->GetActiveRDB() )
                strOutput = StringFn->Format( strOutput, TEXT("%s (Active)\n"), pRDB->GetName() );
            else
                strOutput = StringFn->Format( strOutput, TEXT("%s\n"), pRDB->GetName() );
            pRDB = ResourceFn->EnumNextRDB();
        }
        return true;
    }

    // One argument case
    if ( StringFn->Cmp( strArg0, TEXT("null") ) == 0 ) {
        if ( pEnv->HasOpenedResource() ) {
            strOutput = StringFn->Format( strOutput, TEXT("Closed previously opened resource : %s\n"), pEnv->GetOpenedResourceName() );
            pEnv->CloseResource();
        }

        pEnv->SetActiveRDB( NULL );
        strOutput = StringFn->Copy( strOutput, TEXT("Switched active RDB to NULL (inactive)") );
        return true;
    }

    ResourceDB * pRDB = ResourceFn->GetRDB( strArg0 );
    if ( pRDB == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Bad argument 0 : Invalid RDB file name (WITHOUT extension, MUST be .rdb),\n") );
        strOutput = StringFn->Copy( strOutput, TEXT("  also the name must point to an opened RDB (or just created) ...") );
        return false;
    }

    if ( pEnv->HasOpenedResource() ) {
        strOutput = StringFn->Format( strOutput, TEXT("Closed previously opened resource : %s\n"), pEnv->GetOpenedResourceName() );
        pEnv->CloseResource();
    }

    pEnv->SetActiveRDB( pRDB );
    strOutput = StringFn->Format( strOutput, TEXT("Switched active RDB to %s"), pRDB->GetName() );
    return true;
}
