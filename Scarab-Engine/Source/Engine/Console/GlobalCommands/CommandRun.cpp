/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/GlobalCommands/CommandRun.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Commands : Global, Run
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
#include "CommandRun.h"

#include "../ConsoleManager.h"

/////////////////////////////////////////////////////////////////////////////////
// CommandRun implementation
CommandRun::CommandRun():
    ConsoleCommand( CONSOLECOMMAND_GLOBAL, TEXT("run"), CONSOLECOMMAND_LEVEL_PLAYER )
{
    // Build argument list
    m_iMinArgCount = 1;
    m_iMaxArgCount = 1;
    m_iArgCount = 1;

    m_arrArguments[0].iTypes = 0;
    m_arrArguments[0].iFlags = 0;
    m_arrArguments[0].AddType( CONSOLECOMMAND_ARG_STRING );
}
CommandRun::~CommandRun()
{
    // nothing to do
}

GChar * CommandRun::GetHelp( GChar * strOutput ) const
{
    // Prototype & arguments
    strOutput = StringFn->Copy( strOutput, TEXT("Prototype = /run scriptname\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  scriptname = script file name to run\n") );

    // Description
    strOutput = StringFn->Copy( strOutput, TEXT("Description = Run a console command script\n") );

    return strOutput;
}

Bool CommandRun::Execute( GChar * strOutput, const GChar * strArgList )
{
    // Parse arguments
    GChar strArg0[CONSOLECOMMAND_OUTPUT_LENGTH];

    // Pick first argument
    strArgList = _ExtractArgument_String( strArg0, strArgList );

    // No argument case
    if ( strArgList == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Missing parameter 0 : scriptname ...") );
        return false;
    }

    // One argument case
    return ConsoleFn->RunScript( strArg0, true );
}


