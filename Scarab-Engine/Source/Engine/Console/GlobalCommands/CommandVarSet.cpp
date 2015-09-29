/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/GlobalCommands/CommandVarSet.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Commands : Global, VarSet
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
#include "CommandVarSet.h"

#include "../ConsoleManager.h"

/////////////////////////////////////////////////////////////////////////////////
// CommandVarSet implementation
CommandVarSet::CommandVarSet():
    ConsoleCommand( CONSOLECOMMAND_GLOBAL, TEXT("varset"), CONSOLECOMMAND_LEVEL_DEV )
{
    // Build argument list
    m_iMinArgCount = 2;
    m_iMaxArgCount = 2;
    m_iArgCount = 2;

    m_arrArguments[0].iTypes = 0;
    m_arrArguments[0].iFlags = 0;
    m_arrArguments[0].AddType( CONSOLECOMMAND_ARG_STRING );

    m_arrArguments[1].iTypes = 0;
    m_arrArguments[1].iFlags = 0;
    m_arrArguments[1].AddType( CONSOLECOMMAND_ARG_STRING );
}
CommandVarSet::~CommandVarSet()
{
    // nothing to do
}

GChar * CommandVarSet::GetHelp( GChar * strOutput ) const
{
    // Prototype & arguments
    strOutput = StringFn->Copy( strOutput, TEXT("Prototype = /varset dstname (srcname | null)\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  dstname = Variable name to receive value\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  srcname = Variable name to get value from\n") );

    // Description
    strOutput = StringFn->Copy( strOutput, TEXT("Description = Affect variable srcname's value to variable dstname,\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  dstname is automatically created if needed, previous non-null\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  value is automatically deleted, src variable is set to null\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  (Multiple referencing is prohibited here !)\n") );

    return strOutput;
}

Bool CommandVarSet::Execute( GChar * strOutput, const GChar * strArgList )
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
        strOutput = StringFn->Copy( strOutput, TEXT("Missing parameter 0 : dstname ...") );
        return false;
    }

    // Pick second argument
    strArgList = _ExtractArgument_String( strArg1, strArgList );

    // One argument case
    if ( strArgList == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Missing parameter 1 : srcname ...") );
        return false;
    }

    // Two arguments case
    ConsoleVariable * pSrcVar = NULL;
    if ( StringFn->Cmp( strArg1, TEXT("null") ) != 0 ) {
        pSrcVar = pEnv->GetVariable( strArg1 );
        if ( pSrcVar == NULL ) {
            strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 1 : Invalid srcname ...") );
            return false;
        }
    }

    ConsoleVariable * pDstVar = pEnv->GetVariable( strArg0 );
    if ( pDstVar == NULL ) {
        pDstVar = pEnv->CreateVariable( strArg0 );
        Assert( pDstVar != NULL );
        strOutput = StringFn->Copy( strOutput, TEXT("Variable didn't exist and was created automatically\n") );
    }

    if ( pSrcVar == NULL ) {
        pDstVar->Affect( CONSOLEVAR_TYPE_UNDEFINED, NULL );
        strOutput = StringFn->Format( strOutput, TEXT("Set %s to NULL"), pDstVar->GetName() );
    } else {
        pDstVar->Affect( pSrcVar );
        strOutput = StringFn->Format( strOutput, TEXT("Set %s to %8x"), pDstVar->GetName(), pDstVar->GetValue() );
    }
    return true;
}


