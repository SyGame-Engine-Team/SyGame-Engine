/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/GlobalCommands/CommandLookup.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Commands : Global, Lookup
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
#include "CommandLookup.h"

#include "../ConsoleManager.h"

/////////////////////////////////////////////////////////////////////////////////
// CommandLookup implementation
CommandLookup::CommandLookup():
    ConsoleCommand( CONSOLECOMMAND_GLOBAL, TEXT("lookup"), CONSOLECOMMAND_LEVEL_PLAYER )
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
CommandLookup::~CommandLookup()
{
    // nothing to do
}

GChar * CommandLookup::GetHelp( GChar * strOutput ) const
{
    // Prototype & arguments
    strOutput = StringFn->Copy( strOutput, TEXT("Prototype = /lookup (category | (category) namepart)\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  category = (optional) Category name\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  namepart = (optional) Partial name, search string\n") );

    // Description
    strOutput = StringFn->Copy( strOutput, TEXT("Description = Prints commands matching a partial name or\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  all commands in a given category or category list\n") );

    return strOutput;
}

Bool CommandLookup::Execute( GChar * strOutput, const GChar * strArgList )
{
    // Parse arguments
    GChar strArg0[CONSOLECOMMAND_NAME_LENGTH];
    GChar strArg1[CONSOLECOMMAND_NAME_LENGTH];

    UInt i, iResultCount;
    ConsoleCommand * arrLookupResults[CONSOLE_COMMAND_LOOKUP_MAXRESULTS];

    // Pick first argument
    strArgList = _ExtractArgument_String( strArg0, strArgList );

    // No argument case
    if ( strArgList == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Category list :\n") );
        for( i = 0; i < CONSOLECOMMAND_CATEGORY_COUNT; ++i )
            strOutput = StringFn->Format( strOutput, TEXT("%s\n"), ConsoleCommand::GetCategoryName((ConsoleCommandCategory)i) );
        return true;
    }

    // Pick second argument
    strArgList = _ExtractArgument_String( strArg1, strArgList );

    if ( strArgList == NULL ) {
        // One argument case
        if ( StringFn->Cmp(strArg0, TEXT("global")) == 0 ) {
            strOutput = StringFn->Copy( strOutput, TEXT("All global commands :\n") );

            ConsoleFn->EnumCommands( CONSOLECOMMAND_GLOBAL );
            const ConsoleCommand * pCmd = ConsoleFn->EnumNextCommand();
            while( pCmd != NULL ) {
                strOutput = StringFn->Format( strOutput, TEXT("%s\n"), pCmd->GetName() );
                pCmd = ConsoleFn->EnumNextCommand();
            }

            return true;
        }

        ConsoleCommandCategory iCategory = ConsoleCommand::ParseCategory( strArg0 );
        if ( iCategory != CONSOLECOMMAND_GLOBAL ) {
            strOutput = StringFn->Format( strOutput, TEXT("All commands in category %s :\n"), strArg0 );

            ConsoleFn->EnumCommands( iCategory );
            const ConsoleCommand * pCmd = ConsoleFn->EnumNextCommand();
            while( pCmd != NULL ) {
                strOutput = StringFn->Format( strOutput, TEXT("%s\n"), pCmd->GetName() );
                pCmd = ConsoleFn->EnumNextCommand();
            }

            return true;
        }

        iResultCount = ConsoleFn->LookupCommand( arrLookupResults, CONSOLECOMMAND_GLOBAL, strArg0 );
    } else {
        // Two arguments case
        ConsoleCommandCategory iCategory = ConsoleCommand::ParseCategory( strArg0 );
        if ( iCategory == CONSOLECOMMAND_GLOBAL ) {
            strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 0 : Invalid category name ...") );
            return false;
        }

        iResultCount = ConsoleFn->LookupCommand( arrLookupResults, iCategory, strArg1 );
    }

    // Output lookup result
    strOutput = StringFn->Format( strOutput, TEXT("Found %d results (%d max) :\n"), iResultCount, CONSOLE_COMMAND_LOOKUP_MAXRESULTS );
    for( i = 0; i < iResultCount; ++i )
        strOutput = StringFn->Format( strOutput, TEXT("%s\n"), arrLookupResults[i]->GetName() );

    return true;
}

