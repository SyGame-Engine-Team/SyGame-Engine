/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/GlobalCommands/CommandClear.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Commands : Global, Clear
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
#include "CommandClear.h"

#include "../ConsoleManager.h"

/////////////////////////////////////////////////////////////////////////////////
// CommandClear implementation
CommandClear::CommandClear():
    ConsoleCommand( CONSOLECOMMAND_GLOBAL, TEXT("clear"), CONSOLECOMMAND_LEVEL_PLAYER )
{
    // Build argument list
    m_iMinArgCount = 0;
    m_iMaxArgCount = 0;
    m_iArgCount = 0;
}
CommandClear::~CommandClear()
{
    // nothing to do
}

GChar * CommandClear::GetHelp( GChar * strOutput ) const
{
    // Prototype & arguments
    strOutput = StringFn->Copy( strOutput, TEXT("Prototype = /clear\n") );

    // Description
    strOutput = StringFn->Copy( strOutput, TEXT("Description = Clear console log, no output\n") );

    return strOutput;
}

Bool CommandClear::Execute( GChar * /*strOutput*/, const GChar * /*strArgList*/ )
{
    // No argument case
    ConsoleFn->GetLogModel()->Clear();

    return true;
}


