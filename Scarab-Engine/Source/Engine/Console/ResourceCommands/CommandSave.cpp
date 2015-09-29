/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/ResourceCommands/CommandSave.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Commands : Resource, Save
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
#include "CommandSave.h"

#include "../ConsoleManager.h"

/////////////////////////////////////////////////////////////////////////////////
// CommandSave implementation
CommandSave::CommandSave():
    ConsoleCommand( CONSOLECOMMAND_RESOURCE, TEXT("save"), CONSOLECOMMAND_LEVEL_DEV )
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
CommandSave::~CommandSave()
{
    // nothing to do
}

GChar * CommandSave::GetHelp( GChar * strOutput ) const
{
    // Prototype & arguments
    strOutput = StringFn->Copy( strOutput, TEXT("Prototype = /resource save respath varname\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  respath = resource to receive saved variable\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  varname = variable to save\n") );

    // Description
    strOutput = StringFn->Copy( strOutput, TEXT("Description = Save a variable varname to a resource respath\n") );

    return strOutput;
}

Bool CommandSave::Execute( GChar * strOutput, const GChar * strArgList )
{
    // Get console environment
    ConsoleEnvironment * pEnv = ConsoleFn->GetEnvironment();

    // Parse arguments
    GChar strArg0[CONSOLECOMMAND_INPUT_LENGTH];
    GChar strArg1[CONSOLECOMMAND_INPUT_LENGTH];

    // Pick first argument
    strArgList = _ExtractArgument_String( strArg0, strArgList );

    // No argument case
    if ( strArgList == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Missing parameter 0 : respath ...") );
        return false;
    }

    // Pick second argument
    strArgList = _ExtractArgument_String( strArg1, strArgList );

    // One argument case
    if ( strArgList == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Missing parameter 1 : varname ...") );
        return false;
    }

    // Two arguments case
    ResourceDB * pRDB = pEnv->GetActiveRDB();
    if ( pRDB == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Can't save resource, active RDB is NULL ...") );
        return false;
    }

    ConsoleVariable * pSaveVar = pEnv->GetVariable( strArg1 );
    if ( pSaveVar == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 1 : Invalid varname ...") );
        return false;
    }

    ConsoleVariableType iVarSaveType = pSaveVar->GetType();
    if ( iVarSaveType == CONSOLEVAR_TYPE_UNDEFINED ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 1 : Variable has no type (null) ...") );
        return false;
    }
    if ( iVarSaveType >= (ConsoleVariableType)RESOURCE_TYPE_COUNT ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 1 : Not a resource-type variable ...") );
        return false;
    }

    ResourceType iResSaveType = (ResourceType)( iVarSaveType );
    Resource * pSaveResource = (Resource*)( pSaveVar->GetValue() );
    if ( pSaveResource == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 1 : Variable is null ...") );
        return false;
    }

    Bool bOk = pSaveResource->Save( pRDB->GetName(), strArg0 );
    if ( !bOk ) {
        strOutput = StringFn->Format( strOutput, TEXT("Save error, %s resource is corrupted ..."), Resource::GetResourceTypeName(iResSaveType) );
        return false;
    }

    strOutput = StringFn->Format( strOutput, TEXT("Saved variable (%s) %s\n"), ConsoleVariable::GetTypeName(iVarSaveType), pSaveVar->GetName() );
    strOutput = StringFn->Format( strOutput, TEXT("   to resource (%s) %s"), Resource::GetResourceTypeName(iResSaveType), strArg0 );
    return true;
}

