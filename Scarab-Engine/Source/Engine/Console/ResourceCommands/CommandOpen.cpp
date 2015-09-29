/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/ResourceCommands/CommandOpen.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Commands : Resource, Open
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
#include "CommandOpen.h"

#include "../ConsoleManager.h"

/////////////////////////////////////////////////////////////////////////////////
// CommandOpen implementation
CommandOpen::CommandOpen():
    ConsoleCommand( CONSOLECOMMAND_RESOURCE, TEXT("open"), CONSOLECOMMAND_LEVEL_DEV )
{
    // Build argument list
    m_iMinArgCount = 0;
    m_iMaxArgCount = 1;
    m_iArgCount = 1;

    m_arrArguments[0].iTypes = 0;
    m_arrArguments[0].iFlags = 0;
    m_arrArguments[0].AddType( CONSOLECOMMAND_ARG_STRING );
    m_arrArguments[0].SetOptional( true );
}
CommandOpen::~CommandOpen()
{
    // nothing to do
}

GChar * CommandOpen::GetHelp( GChar * strOutput ) const
{
    // Prototype & arguments
    strOutput = StringFn->Copy( strOutput, TEXT("Prototype = /resource open (filepath|respath)\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  filepath = open RDB file, full path\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  respath  = open RDB resource in active RDB\n") );

    // Description
    strOutput = StringFn->Copy( strOutput, TEXT("Description = Open RDB file given by filepath or\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  open RDB resource in active file given by respath\n") );

    return strOutput;
}

Bool CommandOpen::Execute( GChar * strOutput, const GChar * strArgList )
{
    // Get console environment
    ConsoleEnvironment * pEnv = ConsoleFn->GetEnvironment();

    // Parse arguments
    GChar strArg0[CONSOLECOMMAND_INPUT_LENGTH];

    // Pick first argument
    strArgList = _ExtractArgument_String( strArg0, strArgList );

    // No argument case
    if ( strArgList == NULL ) {
        if ( pEnv->HasOpenedResource() )
            strOutput = StringFn->Format( strOutput, TEXT("Current open resource : %s"), pEnv->GetOpenedResourceName() );
        else
            strOutput = StringFn->Copy( strOutput, TEXT("No current open resource") );
        return true;
    }

    // One argument case
    ResourceDB * pRDB = ResourceFn->OpenRDB( strArg0 );
    if ( pRDB != NULL ) {
        strOutput = StringFn->Format( strOutput, TEXT("Opened RDB file : %s"), pRDB->GetName() );
        return true;
    }

    pRDB = pEnv->GetActiveRDB();
    if ( pRDB == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Can't open resource, active RDB is NULL ...") );
        return false;
    }

    if ( pEnv->HasOpenedResource() ) {
        strOutput = StringFn->Format( strOutput, TEXT("Closed previously opened resource : %s\n"), pEnv->GetOpenedResourceName() );
        pEnv->CloseResource();
    }

    Bool bOk = pEnv->OpenResource( strArg0 );
    if ( !bOk ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 0 : Invalid filepath / respath or already opened ...") );
        return false;
    }

    strOutput = StringFn->Format( strOutput, TEXT("Opened resource : %s"), pEnv->GetOpenedResourceName() );
    return true;
}


