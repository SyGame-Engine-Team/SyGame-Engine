/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/ResourceCommands/CommandList.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Commands : Resource, List
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
#include "CommandList.h"

#include "../ConsoleManager.h"

/////////////////////////////////////////////////////////////////////////////////
// CommandList implementation
CommandList::CommandList():
    ConsoleCommand( CONSOLECOMMAND_RESOURCE, TEXT("list"), CONSOLECOMMAND_LEVEL_DEV )
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
CommandList::~CommandList()
{
    // nothing to do
}

GChar * CommandList::GetHelp( GChar * strOutput ) const
{
    // Prototype & arguments
    strOutput = StringFn->Copy( strOutput, TEXT("Prototype = /resource list (relpath)\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  relpath = (optional) relative path to list, use '..' for parent\n") );

    // Description
    strOutput = StringFn->Copy( strOutput, TEXT("Description = In the active file, list content of relpath or\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  content of CWD when no argument is provided\n") );

    return strOutput;
}

Bool CommandList::Execute( GChar * strOutput, const GChar * strArgList )
{
    // Get console environment
    ConsoleEnvironment * pEnv = ConsoleFn->GetEnvironment();

    ResourceDB * pRDB = pEnv->GetActiveRDB();
    if ( pRDB == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Can't list directory, active RDB is NULL ...") );
        return false;
    }

    // Create temp arrays
    Array<RDB_DirectoryInfos> arrSubdirectories;
    Array<RDB_ResourceInfos> arrResources;

    arrSubdirectories.UseMemoryContext( ConsoleFn->GetMemoryContext(), TEXT("Commands") );
    arrSubdirectories.Create();
    arrResources.UseMemoryContext( ConsoleFn->GetMemoryContext(), TEXT("Commands") );
    arrResources.Create();

    // Parse arguments
    GChar strArg0[CONSOLECOMMAND_INPUT_LENGTH];

    // Pick first argument
    strArgList = _ExtractArgument_String( strArg0, strArgList );

    if ( strArgList == NULL ) {
        // No argument case
        pRDB->ListCWD( &arrSubdirectories, &arrResources );
    } else {
        // One argument case
        Bool bOk = pRDB->ListDirectory( strArg0, &arrSubdirectories, &arrResources );
        if ( !bOk ) {
            strOutput = StringFn->Copy( strOutput, TEXT("Bad argument 0 : Invalid path ...") );

            // Destroy temp arrays
            arrResources.Destroy();
            arrSubdirectories.Destroy();
            return false;
        }
    }

    UInt iSubdirectoryCount = arrSubdirectories.Count();
    UInt iResourceCount = arrResources.Count();
    UInt i;
    const GChar * strResourceType;

    strOutput = StringFn->Format( strOutput, TEXT("Found %d sub-directories and %d resources :\n"), iSubdirectoryCount, iResourceCount );
    for( i = 0; i < iSubdirectoryCount; ++i )
        strOutput = StringFn->Format( strOutput, TEXT("(Directory) %s\n"), arrSubdirectories[i].strName );
    for( i = 0; i < iResourceCount; ++i ) {
        strResourceType = Resource::GetResourceTypeName( arrResources[i].iType );
        strOutput = StringFn->Format( strOutput, TEXT("(%s) %s\n"), strResourceType, arrResources[i].strName );
    }

    // Destroy temp arrays
    arrResources.Destroy();
    arrSubdirectories.Destroy();
    return true;
}
