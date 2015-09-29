/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/GlobalCommands/CommandVarDel.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Commands : Global, VarDel
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
#include "CommandVarDel.h"

#include "../ConsoleManager.h"

/////////////////////////////////////////////////////////////////////////////////
// CommandVarDel implementation
CommandVarDel::CommandVarDel():
    ConsoleCommand( CONSOLECOMMAND_GLOBAL, TEXT("vardel"), CONSOLECOMMAND_LEVEL_DEV )
{
    // Build argument list
    m_iMinArgCount = 1;
    m_iMaxArgCount = 1;
    m_iArgCount = 1;

    m_arrArguments[0].iTypes = 0;
    m_arrArguments[0].iFlags = 0;
    m_arrArguments[0].AddType( CONSOLECOMMAND_ARG_STRING );
}
CommandVarDel::~CommandVarDel()
{
    // nothing to do
}

GChar * CommandVarDel::GetHelp( GChar * strOutput ) const
{
    // Prototype & arguments
    strOutput = StringFn->Copy( strOutput, TEXT("Prototype = /vardel varname\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  varname = Variable name\n") );

    // Description
    strOutput = StringFn->Copy( strOutput, TEXT("Description = Delete variable varname\n") );

    return strOutput;
}

Bool CommandVarDel::Execute( GChar * strOutput, const GChar * strArgList )
{
    // Get console environment
    ConsoleEnvironment * pEnv = ConsoleFn->GetEnvironment();

    // Parse arguments
    GChar strArg0[CONSOLECOMMAND_OUTPUT_LENGTH];

    // Pick first argument
    strArgList = _ExtractArgument_String( strArg0, strArgList );

    // No argument case
    if ( strArgList == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Missing parameter 0 : varname ...") );
        return false;
    }

    // One argument case
    Bool bOk = pEnv->DestroyVariable( strArg0 );
    if ( !bOk ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 0 : Invalid varname ...") );
        return false;
    }

    strOutput = StringFn->Format( strOutput, TEXT("Deleted variable %s"), strArg0 );
    return true;
}


