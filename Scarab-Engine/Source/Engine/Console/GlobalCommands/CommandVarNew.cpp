/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/GlobalCommands/CommandVarNew.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Commands : Global, VarNew
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
#include "CommandVarNew.h"

#include "../ConsoleManager.h"

/////////////////////////////////////////////////////////////////////////////////
// CommandVarNew implementation
CommandVarNew::CommandVarNew():
    ConsoleCommand( CONSOLECOMMAND_GLOBAL, TEXT("varnew"), CONSOLECOMMAND_LEVEL_DEV )
{
    // Build argument list
    m_iMinArgCount = 1;
    m_iMaxArgCount = 2;
    m_iArgCount = 2;

    m_arrArguments[0].iTypes = 0;
    m_arrArguments[0].iFlags = 0;
    m_arrArguments[0].AddType( CONSOLECOMMAND_ARG_STRING );
    m_arrArguments[0].SetOptional( true );

    m_arrArguments[1].iTypes = 0;
    m_arrArguments[1].iFlags = 0;
    m_arrArguments[1].AddType( CONSOLECOMMAND_ARG_STRING );
}
CommandVarNew::~CommandVarNew()
{
    // nothing to do
}

GChar * CommandVarNew::GetHelp( GChar * strOutput ) const
{
    // Prototype & arguments
    strOutput = StringFn->Copy( strOutput, TEXT("Prototype = /varnew (vartype) varname\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  vartype = (optional) Variable type\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  varname = Variable name\n") );

    // Description
    strOutput = StringFn->Copy( strOutput, TEXT("Description = Create variable varname with type vartype if provided,\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  variable value is initialized to null\n") );

    return strOutput;
}

Bool CommandVarNew::Execute( GChar * strOutput, const GChar * strArgList )
{
    // Get console environment
    ConsoleEnvironment * pEnv = ConsoleFn->GetEnvironment();

    // Parse arguments
    GChar strArg0[CONSOLECOMMAND_OUTPUT_LENGTH];
    GChar strArg1[CONSOLECOMMAND_OUTPUT_LENGTH];

    // Pick first argument
    strArgList = _ExtractArgument_String( strArg0, strArgList );

    // No argument case
    if ( strArgList == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Missing parameter 0 : varname ...") );
        return false;
    }

    // Pick second argument
    strArgList = _ExtractArgument_String( strArg1, strArgList );

    // One argument case
    if ( strArgList == NULL ) {
        ConsoleVariable * pVar = pEnv->CreateVariable( strArg0 );
        if ( pVar == NULL ) {
            strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 0 : varname already exists ...") );
            return false;
        }

        strOutput = StringFn->Format( strOutput, TEXT("Created variable %s"), pVar->GetName() );
        return true;
    }

    // Two arguments case
    ConsoleVariableType iVarType = ConsoleVariable::GetTypeValue( strArg0 );
    if ( iVarType == CONSOLEVAR_TYPE_UNDEFINED ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 0 : Invalid vartype ...") );
        return false;
    }

    ConsoleVariable * pVar = pEnv->CreateVariable( strArg1, iVarType, NULL );
    if ( pVar == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 1 : varname already exists ...") );
        return false;
    }

    strOutput = StringFn->Format( strOutput, TEXT("Created variable %s"), pVar->GetName() );
    return true;
}

