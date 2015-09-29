/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/GlobalCommands/CommandHelp.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Commands : Global, Help
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
#include "CommandHelp.h"

#include "../ConsoleManager.h"

/////////////////////////////////////////////////////////////////////////////////
// CommandHelp implementation
CommandHelp::CommandHelp():
    ConsoleCommand( CONSOLECOMMAND_GLOBAL, TEXT("help"), CONSOLECOMMAND_LEVEL_PLAYER )
{
    // Build argument list
    m_iMinArgCount = 0;
    m_iMaxArgCount = 2;
    m_iArgCount = 2;

    m_arrArguments[0].iTypes = 0;
    m_arrArguments[0].iFlags = 0;
    m_arrArguments[0].AddType( CONSOLECOMMAND_ARG_STRING );
    m_arrArguments[0].SetOptional( true );

    m_arrArguments[1].iTypes = 0;
    m_arrArguments[1].iFlags = 0;
    m_arrArguments[1].AddType( CONSOLECOMMAND_ARG_STRING );
    m_arrArguments[1].SetOptional( true );
}
CommandHelp::~CommandHelp()
{
    // nothing to do
}

GChar * CommandHelp::GetHelp( GChar * strOutput ) const
{
    // Prototype & arguments
    strOutput = StringFn->Copy( strOutput, TEXT("Prototype = /help ((category) name)\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  category = (optional) Category name\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  name     = (optional) Command name\n") );

    // Description
    strOutput = StringFn->Copy( strOutput, TEXT("Description = Prints help information about commands\n") );

    return strOutput;
}

Bool CommandHelp::Execute( GChar * strOutput, const GChar * strArgList )
{
    // Parse arguments
    GChar strArg0[CONSOLECOMMAND_NAME_LENGTH];
    GChar strArg1[CONSOLECOMMAND_NAME_LENGTH];

    // Pick first argument
    strArgList = _ExtractArgument_String( strArg0, strArgList );

    // No argument case
    if ( strArgList == NULL ) {
        GetHelp( strOutput );
        return true;
    }

    // Pick second argument
    strArgList = _ExtractArgument_String( strArg1, strArgList );

    // One argument case
    if ( strArgList == NULL ) {
        ConsoleCommand * pCommand = ConsoleFn->GetCommand( CONSOLECOMMAND_GLOBAL, strArg0 );
        if ( pCommand == NULL ) {
            strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 0 : Invalid command name / Not global ...") );
            return false;
        }
        pCommand->GetHelp( strOutput );
        return true;
    }

    // Two arguments case
    ConsoleCommandCategory iCategory = ConsoleCommand::ParseCategory( strArg0 );
    if ( iCategory == CONSOLECOMMAND_GLOBAL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 0 : Invalid category name ...") );
        return false;
    }

    ConsoleCommand * pCommand = ConsoleFn->GetCommand( iCategory, strArg1 );
    if ( pCommand == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 1 : Invalid command name / Wrong category ...") );
        return false;
    }

    pCommand->GetHelp( strOutput );
    return true;
}

