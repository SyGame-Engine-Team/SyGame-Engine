/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/ResourceCommands/CommandCreate.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Commands : Resource, Create
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
#include "CommandCreate.h"

#include "../ConsoleManager.h"

/////////////////////////////////////////////////////////////////////////////////
// CommandCreate implementation
CommandCreate::CommandCreate():
    ConsoleCommand( CONSOLECOMMAND_RESOURCE, TEXT("create"), CONSOLECOMMAND_LEVEL_DEV )
{
    // Build argument list
    m_iMinArgCount = 1;
    m_iMaxArgCount = 3;
    m_iArgCount = 4;

    m_arrArguments[0].iTypes = 0;
    m_arrArguments[0].iFlags = 0;
    m_arrArguments[0].AddType( CONSOLECOMMAND_ARG_STRING );

    m_arrArguments[1].iTypes = 0;
    m_arrArguments[1].iFlags = 0;
    m_arrArguments[1].AddType( CONSOLECOMMAND_ARG_STRING );
    m_arrArguments[1].SetOptional( true );

    m_arrArguments[2].iTypes = 0;
    m_arrArguments[2].iFlags = 0;
    m_arrArguments[2].AddType( CONSOLECOMMAND_ARG_STRING );

    m_arrArguments[3].iTypes = 0;
    m_arrArguments[3].iFlags = 0;
    m_arrArguments[3].AddType( CONSOLECOMMAND_ARG_STRING );
}
CommandCreate::~CommandCreate()
{
    // nothing to do
}

GChar * CommandCreate::GetHelp( GChar * strOutput ) const
{
    // Prototype & arguments
    strOutput = StringFn->Copy( strOutput, TEXT("Prototype = /resource create (filepath | (respath) name type)\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  filepath = create RDB file, full path\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  respath  = (optional) relative path for creation\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  name     = new name for created resource\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  type     = type for created resource\n") );

    // Description
    strOutput = StringFn->Copy( strOutput, TEXT("Description = Create RDB file given by filepath or\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  create a 'type' resource 'name' at 'respath'\n") );

    return strOutput;
}

Bool CommandCreate::Execute( GChar * strOutput, const GChar * strArgList )
{
    // Get console environment
    ConsoleEnvironment * pEnv = ConsoleFn->GetEnvironment();

    // Parse arguments
    GChar strArg0[CONSOLECOMMAND_INPUT_LENGTH];
    GChar strArg1[CONSOLECOMMAND_INPUT_LENGTH];
    GChar strArg2[CONSOLECOMMAND_INPUT_LENGTH];

    // Pick first argument
    strArgList = _ExtractArgument_String( strArg0, strArgList );

    // No argument case
    if ( strArgList == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Missing parameter 0 : filepath ...") );
        return false;
    }

    // Pick second argument
    strArgList = _ExtractArgument_String( strArg1, strArgList );

    // One argument case
    if ( strArgList == NULL ) {
        ResourceDB * pRDB = ResourceFn->CreateRDB( strArg0 );
        if ( pRDB == NULL ) {
            strOutput = StringFn->Copy( strOutput, TEXT("Can't create RDB file, already existing ? ...") );
            return false;
        }

        strOutput = StringFn->Format( strOutput, TEXT("Created RDB file : %s"), pRDB->GetName() );
        return true;
    }

    ResourceDB * pRDB = pEnv->GetActiveRDB();
    if ( pRDB == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Can't create directory/resource, active RDB is NULL ...") );
        return false;
    }

    // Pick third argument
    strArgList = _ExtractArgument_String( strArg2, strArgList );

    // Two arguments case
    Bool bOk;
    ResourceType iType;
    if ( strArgList == NULL ) {
        iType = Resource::GetResourceType( strArg1 );
        if ( iType == RESOURCE_TYPE_UNDEFINED ) {
            strOutput = StringFn->Copy( strOutput, TEXT("Bad argument 1 : Invalid resource type ...") );
            return false;
        }

        if ( iType == RESOURCE_TYPE_DIRECTORY ) {
            bOk = pRDB->CreateDirectory( TEXT(""), strArg0 );
            if ( !bOk ) {
                strOutput = StringFn->Copy( strOutput, TEXT("Bad argument 0 : Can't create directory, already existing ? ...") );
                return false;
            }
            strOutput = StringFn->Format( strOutput, TEXT("Created Directory : %s"), strArg0 );
            return true;
        }

        bOk = pRDB->CreateResource( TEXT(""), strArg0, iType );
        if ( !bOk ) {
            strOutput = StringFn->Copy( strOutput, TEXT("Bad argument 0 : Can't create resource, already existing ? ...") );
            return false;
        }
        strOutput = StringFn->Format( strOutput, TEXT("Created Resource : (%s) %s"), strArg1, strArg0 );
        return true;
    }

    // Three arguments case
    iType = Resource::GetResourceType( strArg2 );
    if ( iType == RESOURCE_TYPE_UNDEFINED ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Bad argument 2 : Invalid resource type ...") );
        return false;
    }

    if ( iType == RESOURCE_TYPE_DIRECTORY ) {
        bOk = pRDB->CreateDirectory( strArg0, strArg1 );
        if ( !bOk ) {
            strOutput = StringFn->Copy( strOutput, TEXT("Bad argument 0/1 : Can't create directory, bad path / already existing ? ...") );
            return false;
        }
        strOutput = StringFn->Format( strOutput, TEXT("Created Directory : %s"), strArg1 );
        return true;
    }

    bOk = pRDB->CreateResource( strArg0, strArg1, iType );
    if ( !bOk ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Bad argument 0/1 : Can't create resource, bad path / already existing ? ...") );
        return false;
    }
    strOutput = StringFn->Format( strOutput, TEXT("Created Resource : (%s) %s"), strArg2, strArg1 );
    return true;
}

