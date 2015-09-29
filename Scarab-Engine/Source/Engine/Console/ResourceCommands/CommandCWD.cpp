/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/ResourceCommands/CommandCWD.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Commands : Resource, CWD
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
#include "CommandCWD.h"

#include "../ConsoleManager.h"

/////////////////////////////////////////////////////////////////////////////////
// CommandCWD implementation
CommandCWD::CommandCWD():
    ConsoleCommand( CONSOLECOMMAND_RESOURCE, TEXT("cwd"), CONSOLECOMMAND_LEVEL_DEV )
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
CommandCWD::~CommandCWD()
{
    // nothing to do
}

GChar * CommandCWD::GetHelp( GChar * strOutput ) const
{
    // Prototype & arguments
    strOutput = StringFn->Copy( strOutput, TEXT("Prototype = /resource cwd (relpath)\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  relpath = (optional) relative path to walk through, use '..' for parent\n") );

    // Description
    strOutput = StringFn->Copy( strOutput, TEXT("Description = In the active file, changes CWD or prints\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  current CWD when no argument is provided\n") );

    return strOutput;
}

Bool CommandCWD::Execute( GChar * strOutput, const GChar * strArgList )
{
    // Get console environment
    ConsoleEnvironment * pEnv = ConsoleFn->GetEnvironment();

    ResourceDB * pRDB = pEnv->GetActiveRDB();
    if ( pRDB == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Can't access CWD, active RDB is NULL ...") );
        return false;
    }

    // Parse arguments
    GChar strArg0[CONSOLECOMMAND_INPUT_LENGTH];

    // Pick first argument
    strArgList = _ExtractArgument_String( strArg0, strArgList );

    // No argument case
    GChar strCWD[RDB_PATH_LENGTH];
    if ( strArgList == NULL ) {
        pRDB->GetCWD( strCWD );
        strOutput = StringFn->Format( strOutput, TEXT("Current CWD : %s/%s"), pRDB->GetName(), strCWD );
        return true;
    }

    // One argument case
    Bool bOk = pRDB->ChangeCWD( strArg0 );
    if ( !bOk ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Bad argument 0 : Invalid path ...") );
        return false;
    }

    pRDB->GetCWD( strCWD );
    strOutput = StringFn->Format( strOutput, TEXT("Changed CWD : %s/%s"), pRDB->GetName(), strCWD );
    return true;
}

