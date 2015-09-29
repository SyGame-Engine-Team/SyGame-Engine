/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/GlobalCommands/CommandEcho.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Commands : Global, Echo
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
#include "CommandEcho.h"

#include "../ConsoleManager.h"

/////////////////////////////////////////////////////////////////////////////////
// CommandEcho implementation
CommandEcho::CommandEcho():
    ConsoleCommand( CONSOLECOMMAND_GLOBAL, TEXT("echo"), CONSOLECOMMAND_LEVEL_PLAYER )
{
    // Build argument list
    m_iMinArgCount = 1;
    m_iMaxArgCount = 1;
    m_iArgCount = 1;

    m_arrArguments[0].iTypes = 0;
    m_arrArguments[0].iFlags = 0;
    m_arrArguments[0].AddType( CONSOLECOMMAND_ARG_STRING );
}
CommandEcho::~CommandEcho()
{
    // nothing to do
}

GChar * CommandEcho::GetHelp( GChar * strOutput ) const
{
    // Prototype & arguments
    strOutput = StringFn->Copy( strOutput, TEXT("Prototype = /echo message\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  message = string, can be quoted with \"\n") );

    // Description
    strOutput = StringFn->Copy( strOutput, TEXT("Description = Prints input message to console output\n") );

    return strOutput;
}

Bool CommandEcho::Execute( GChar * strOutput, const GChar * strArgList )
{
    // Parse arguments
    GChar strArg0[CONSOLECOMMAND_OUTPUT_LENGTH];

    // Pick first argument
    strArgList = _ExtractArgument_String( strArg0, strArgList );

    // No argument case
    if ( strArgList == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Missing parameter 0 : message ...") );
        return false;
    }

    // One argument case
    strOutput = StringFn->NCopy( strOutput, strArg0, CONSOLECOMMAND_OUTPUT_LENGTH - 1 );

    return true;
}

