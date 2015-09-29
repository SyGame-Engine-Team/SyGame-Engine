/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/GlobalCommands/CommandDump.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Commands : Global, Dump
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
#include "CommandDump.h"

#include "../ConsoleManager.h"

/////////////////////////////////////////////////////////////////////////////////
// CommandDump implementation
CommandDump::CommandDump():
    ConsoleCommand( CONSOLECOMMAND_GLOBAL, TEXT("dump"), CONSOLECOMMAND_LEVEL_PLAYER )
{
    // Build argument list
    m_iMinArgCount = 1;
    m_iMaxArgCount = 1;
    m_iArgCount = 1;

    m_arrArguments[0].iTypes = 0;
    m_arrArguments[0].iFlags = 0;
    m_arrArguments[0].AddType( CONSOLECOMMAND_ARG_STRING );
}
CommandDump::~CommandDump()
{
    // nothing to do
}

GChar * CommandDump::GetHelp( GChar * strOutput ) const
{
    // Prototype & arguments
    strOutput = StringFn->Copy( strOutput, TEXT("Prototype = /dump dumpname\n") );
    strOutput = StringFn->Copy( strOutput, TEXT("  dumpname = name to use for dump file\n") );

    // Description
    strOutput = StringFn->Copy( strOutput, TEXT("Description = Dump console log\n") );

    return strOutput;
}

Bool CommandDump::Execute( GChar * strOutput, const GChar * strArgList )
{
    // Parse arguments
    GChar strArg0[CONSOLECOMMAND_OUTPUT_LENGTH];

    // Pick first argument
    strArgList = _ExtractArgument_String( strArg0, strArgList );

    // No argument case
    if ( strArgList == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Missing parameter 0 : dumpname ...") );
        return false;
    }

    // One argument case
    GChar strFileName[FILENAME_LENGTH];
    StringFn->Format( strFileName, TEXT("%s/%s%s.log"), CONSOLE_PATH_LOG, TEXT("ConsoleDump_"), strArg0 );

    HFile hFile = SystemFn->CreateFile( strFileName, FILE_WRITE );
    if ( !hFile.IsValid() ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Bad parameter 0 : Invalid filename, already existing ? ...") );
        return false;
    }

    const ConsoleLogModel * pLog = ConsoleFn->GetLogModel();
    UInt iLineCount = pLog->GetLineCount();

    for( UInt i = 0; i < iLineCount; ++i ) {
        hFile.WriteString( TEXT("> ") );
        hFile.WriteString( pLog->GetLine(i)->GetText() );
        hFile.WriteChar( TEXT('\n') );
    }

    hFile.Close();

    strOutput = StringFn->Format( strOutput, TEXT("Dumped console log to %s"), strFileName );
    return true;
}


