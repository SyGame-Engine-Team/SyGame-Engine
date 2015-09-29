/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/ConsoleManager.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Console interface allowing run-time engine control ...
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
#include "ConsoleManager.h"

/////////////////////////////////////////////////////////////////////////////////
// ConsoleLogModel implementation
ConsoleLogModel::ConsoleLogModel():
    GUITextLogModel( CONSOLE_LOGLINE_HEIGHT )
{
    // nothing to do
}
ConsoleLogModel::~ConsoleLogModel()
{
    // nothing to do
}

Void ConsoleLogModel::OnTextLog()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// ConsoleInputModel implementation
ConsoleInputModel::ConsoleInputModel( ConsoleLogModel * pLogModel ):
    GUITextInputModel( TEXT("Enter command here ...") )
{
    Assert( pLogModel != NULL );
    m_pLogModel = pLogModel;
}
ConsoleInputModel::~ConsoleInputModel()
{
    // nothing to do
}

Void ConsoleInputModel::OnKeyPress( KeyCode iKey, GUIEventFlag iFlags )
{
    // Flags
    Bool bShift = ( (iFlags & GUIEVENT_FLAG_SHIFT) != 0 );

    // Deal event
    switch( iKey ) {
        case KEYCODE_UP: {
                const GChar * strNewText = ConsoleFn->_History_PrevCommand( GetText() );
                if ( strNewText != NULL ) {
                    UnselectAll();
                    SetCursor( 0 );
                    SetText( strNewText );
                }
            } break;
        case KEYCODE_DOWN: {
                const GChar * strNewText = ConsoleFn->_History_NextCommand( GetText() );
                if ( strNewText != NULL ) {
                    UnselectAll();
                    SetCursor( 0 );
                    SetText( strNewText );
                }
            } break;
        case KEYCODE_ENTER: {
                if ( bShift ) {
                    // Push a newline char
                    if ( HasSelection() ) {
					    SetCursor( m_iSelectionStart );
					    DeleteText( GetSelectionLength(), false );
					    UnselectAll();
				    }
				    PutChar( TEXT('\n') );
                } else {
                    // Get input text
                    const GChar * strCommand = GetText();
                    if ( strCommand[0] == NULLBYTE )
                        return;

                    // Save input text to history
                    ConsoleFn->_History_PushCommand( strCommand );

                    // Prepare command output
                    GChar strOutput[CONSOLECOMMAND_OUTPUT_LENGTH];
                    strOutput[0] = NULLBYTE;

                    // Process command
                    ConsoleFn->Execute( strOutput, strCommand );

                    // Log command output
                    if ( strOutput[0] != NULLBYTE )
                        m_pLogModel->Log( strOutput );

                    // Clear input text
                    ClearText();
                }
            } break;
        default:
            // Let default handler treat other cases
            GUITextInputModel::OnKeyPress( iKey, iFlags );
            break;
    }
}

Void ConsoleInputModel::OnTextChange()
{
    // nothing to do
}
Void ConsoleInputModel::OnCursorChange()
{
    // nothing to do
}
Void ConsoleInputModel::OnSelectionChange()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// ConsoleManager implementation
ConsoleManager::ConsoleManager():
    m_cEnvironment()
{
    m_pInputModel = NULL;
    m_pLogModel = NULL;

    m_pWindow = NULL;

    m_pVideoSettingsWindow = NULL;
    m_pResourceViewerWindow = NULL;

    // Console's security level
    m_iConsoleLevel = CONSOLECOMMAND_LEVEL_DEV;

    // Command history
    m_bHistoryLoop = false;
    m_iHistoryStart = 0;
    m_iHistoryLength = 0;
    m_iHistoryIndex = 0;
    m_strSavedCommand[0] = NULLBYTE;
}
ConsoleManager::~ConsoleManager()
{
    // nothing to do
}

Void ConsoleManager::Initialize()
{
    // Create models
    SelectMemory( TEXT("Scratch") );
    m_pLogModel = New ConsoleLogModel();
    m_pInputModel = New ConsoleInputModel( m_pLogModel );
    UnSelectMemory();

    // Create window
    m_pWindow = GUIFn->CreateWindow( TEXT("Console"),
                                     Rectangle2( Point2(50, 50), CONSOLE_WINDOW_WIDTH, CONSOLE_WINDOW_HEIGHT ),
                                     GUIWINDOW_STYLE_DEFAULT );

    // Build window
    GUIWidget * pClientRoot = m_pWindow->GetClientRoot();
    const Rectangle2 & rectClient = pClientRoot->GetClientRect();

    GUIGridLayout consoleLayout;
    consoleLayout.FillX = GUILAYOUT_FILL_FIXED;
    consoleLayout.FillY = GUILAYOUT_FILL_FIXED;
    consoleLayout.Anchor = GUILAYOUT_ANCHOR_FIXED;

    consoleLayout.FixedSize.X = rectClient.Width();
    consoleLayout.FixedSize.Y = rectClient.Height() - CONSOLE_INPUTLINE_HEIGHT;
    consoleLayout.FixedPos = Point2::Null;
    GUIFn->CreateWidget( GUIWIDGET_TEXTLOG, m_pLogModel, pClientRoot, consoleLayout, TEXT("Console_Log") );

    consoleLayout.FixedSize.X = rectClient.Width();
    consoleLayout.FixedSize.Y = CONSOLE_INPUTLINE_HEIGHT;
    consoleLayout.FixedPos.X = 0;
    consoleLayout.FixedPos.Y = rectClient.Height() - CONSOLE_INPUTLINE_HEIGHT;
    GUIFn->CreateWidget( GUIWIDGET_TEXTINPUT, m_pInputModel, pClientRoot, consoleLayout, TEXT("Console_Input") );

    // Register window
    GUIFn->RegisterWindow( m_pWindow );

    // Other windows
    SelectMemory( TEXT("Scratch") );
    m_pVideoSettingsWindow = New VideoSettingsWindow();
    m_pResourceViewerWindow = New ResourceViewerWindow();
    UnSelectMemory();

    // Console environment
    m_cEnvironment.Initialize();

    // Commands database
    for( UInt i = 0; i < CONSOLECOMMAND_CATEGORY_COUNT; ++i ) {
        m_mapCommands[i].UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
        m_mapCommands[i].SetComparator( _Name_Comparator );
        m_mapCommands[i].Create();
    }

    _InstallCommands();
}
Void ConsoleManager::Cleanup()
{
    // Commands database
    SelectMemory( TEXT("Commands") );

    CommandMap::Iterator itCmd;
    for( UInt i = 0; i < CONSOLECOMMAND_CATEGORY_COUNT; ++i ) {
        for( itCmd = m_mapCommands[i].Begin(); !(itCmd.IsNull()); ++itCmd )
            Delete( itCmd.GetItem() );
        m_mapCommands[i].Destroy();
    }

    UnSelectMemory();

    // Console environment
    m_cEnvironment.Cleanup();

    // Other windows
    SelectMemory( TEXT("Scratch") );
    Delete( m_pResourceViewerWindow );
    m_pResourceViewerWindow = NULL;
    Delete( m_pVideoSettingsWindow );
    m_pVideoSettingsWindow = NULL;
    UnSelectMemory();

    // Unregister window
    GUIFn->UnregisterWindow( m_pWindow );

    // Destroy window
    GUIFn->DestroyWindow( m_pWindow );
    m_pWindow = NULL;

    // Destroy models
    SelectMemory( TEXT("Scratch") );
    Delete( m_pLogModel );
    m_pLogModel = NULL;
    Delete( m_pInputModel );
    m_pInputModel = NULL;
    UnSelectMemory();
}

ConsoleCommand * ConsoleManager::GetCommand( ConsoleCommandCategory iCategory, const GChar * strName ) const
{
    CommandMap::Iterator itCmd = m_mapCommands[iCategory].Get( strName );
    if ( itCmd.IsNull() )
        return NULL;
    return itCmd.GetItem();
}
UInt ConsoleManager::LookupCommand( ConsoleCommand ** arrLookupResults, ConsoleCommandCategory iCategory, const GChar * strLookup ) const
{
    UInt iLengthP = StringFn->Length( strLookup );
    UInt iLengthT;
    UInt iFound = INVALID_OFFSET;

    UInt iResultCount = 0;

    CommandMap::Iterator itCmd;
    for( itCmd = m_mapCommands[iCategory].Begin(); !(itCmd.IsNull()); ++itCmd ) {
        iLengthT = StringFn->Length( itCmd.GetKey() );
        iFound = MatchFn->Raita( itCmd.GetKey(), iLengthT, strLookup, iLengthP );
        if ( iFound != INVALID_OFFSET ) {
            arrLookupResults[iResultCount++] = itCmd.GetItem();
            if ( iResultCount == CONSOLE_COMMAND_LOOKUP_MAXRESULTS )
                break;
        }
    }

    return iResultCount;
}

Bool ConsoleManager::Execute( GChar * strOutput, const GChar * strCommand )
{
    // Echo shortcut case
    if ( strCommand[0] != CONSOLECOMMAND_MAGIC_CHAR ) {
        ConsoleCommand * pCmdEcho = GetCommand( CONSOLECOMMAND_GLOBAL, TEXT("echo") );
        return pCmdEcho->Execute( strOutput, strCommand );
    }
    ++strCommand;

    // Parse command category
    GChar strCategory[CONSOLECOMMAND_NAME_LENGTH];
    ConsoleCommandCategory iCategory;

    UInt iLength = 0;
    while( *strCommand != TEXT(' ') ) {
        if ( *strCommand == NULLBYTE )
            break;
        strCategory[iLength++] = *strCommand;
        ++strCommand;
    }
    strCategory[iLength] = NULLBYTE;
    iCategory = ConsoleCommand::ParseCategory( strCategory );

    // Parse command name
    GChar strName[CONSOLECOMMAND_NAME_LENGTH];
    const GChar * strArgList = NULL;

    if ( *strCommand == NULLBYTE ) {
        // Should be global command with no argument
        if ( iCategory != CONSOLECOMMAND_GLOBAL )
            strName[0] = NULLBYTE;
        else
            StringFn->NCopy( strName, strCategory, CONSOLECOMMAND_NAME_LENGTH - 1 );
    } else {
        ++strCommand; // Skip space
        if ( iCategory != CONSOLECOMMAND_GLOBAL ) {
            // Parse command name
            iLength = 0;
            while( *strCommand != TEXT(' ') ) {
                if ( *strCommand == NULLBYTE )
                    break;
                strName[iLength++] = *strCommand;
                ++strCommand;
            }
            strName[iLength] = NULLBYTE;
            if ( *strCommand != NULLBYTE )
                strArgList = strCommand + 1; // Skip space
        } else {
            StringFn->NCopy( strName, strCategory, CONSOLECOMMAND_NAME_LENGTH - 1 );
            strArgList = strCommand;
        }
    }

    // Execute the command
    ConsoleCommand * pCommand = ConsoleFn->GetCommand( iCategory, strName );
    if ( pCommand == NULL ) {
        strOutput = StringFn->Copy( strOutput, TEXT("Invalid Category or Command name ...") );
        return false;
    }

    if ( pCommand->GetLevel() < ConsoleFn->GetLevel() ) {
        strOutput = StringFn->Format( strOutput, TEXT("Forbidden Command, bad security level (%d current, %d needed) ..."),
                                                 ConsoleFn->GetLevel(), pCommand->GetLevel() );
        return false;
    }

    return pCommand->Execute( strOutput, strArgList );
}
Bool ConsoleManager::RunScript( const GChar * strScriptName, Bool bLogOutput )
{
    // Open script file
    GChar strFileName[FILENAME_LENGTH];
    StringFn->Format( strFileName, TEXT("%s/%s.cmd"), CONSOLE_PATH_SCRIPT, strScriptName );

    HFile hFile = SystemFn->OpenFile( strFileName, FILE_READ );
    if ( !hFile.IsValid() ) {
        if ( bLogOutput )
            m_pLogModel->Log( TEXT("Bad parameter : Invalid scriptname ...") );
        return false;
    }

    // Process command script
    GChar strOutput[CONSOLECOMMAND_OUTPUT_LENGTH];
    strOutput[0] = NULLBYTE;

    GChar strCommand[CONSOLECOMMAND_INPUT_LENGTH];
    GChar * pCh = strCommand;

    UInt iCommandCount = 0;
    Bool bContinue;
    Bool bSuccess;

    while( true ) {
        bContinue = hFile.ReadChar( pCh );
        if ( !bContinue ) {
            hFile.Close();
            if ( bLogOutput )
                m_pLogModel->Log( TEXT("Script error : Unexpected end of file (missing ';') ...") );
            return false;
        }

        if ( *pCh != TEXT(';') ) {
            ++pCh;
            continue;
        }

        // Execute current command
        *pCh = NULLBYTE;
        bSuccess = ConsoleFn->Execute( strOutput, strCommand );
        if ( !bSuccess ) {
            hFile.Close();
            if ( bLogOutput ) {
                StringFn->Format( strOutput, TEXT("Script error : Command %d failed :\n  '%s'\n"), iCommandCount, strCommand );
                m_pLogModel->Log( strOutput );
            }
            return false;
        }

        // Log current command output
        if ( bLogOutput && strOutput[0] != NULLBYTE ) {
            m_pLogModel->Log( strOutput );
            strOutput[0] = NULLBYTE;
        }

        // Next command
        pCh = strCommand;
        ++iCommandCount;

        // Skip spaces, tabs & newlines
        *pCh = TEXT('\n');
        while( *pCh == TEXT('\n') || *pCh == TEXT('\r') || *pCh == TEXT('\t') || *pCh == TEXT(' ') ) {
            bContinue = hFile.ReadChar( pCh );
            if ( !bContinue )
                break; // no more command
        }
        if ( !bContinue )
            break; // no more command
        hFile.Seek( FILE_SEEK_CURRENT, -1 );
    }

    if ( bLogOutput ) {
        StringFn->Format( strOutput, TEXT("Runned script %s, %d total commands"), strFileName, iCommandCount );
        m_pLogModel->Log( strOutput );
    }

    // Close script file
    hFile.Close();

    return true;
}

/////////////////////////////////////////////////////////////////////////////////

Void ConsoleManager::_RegisterCommand( ConsoleCommand * pEngineCmd )
{
    ConsoleCommandCategory iCategory = pEngineCmd->GetCategory();

    Bool bInserted = m_mapCommands[iCategory].Insert( pEngineCmd->GetName(), pEngineCmd );
    Assert( bInserted );
}
Void ConsoleManager::_InstallCommands()
{
    SelectMemory( TEXT("Commands") );

    // Create & Register all commands
        // Global commands
    _RegisterCommand( New CommandHelp() );
    _RegisterCommand( New CommandLookup() );
    _RegisterCommand( New CommandEcho() );
    _RegisterCommand( New CommandClear() );
    _RegisterCommand( New CommandDump() );
    _RegisterCommand( New CommandRun() );
    _RegisterCommand( New CommandVar() );
    _RegisterCommand( New CommandVarNew() );
    _RegisterCommand( New CommandVarDel() );
    _RegisterCommand( New CommandVarSet() );

        // GUI commands
    _RegisterCommand( New CommandShowTree() );

        // Resource commands
    _RegisterCommand( New CommandRDB() );
    _RegisterCommand( New CommandOpen() );
    _RegisterCommand( New CommandClose() );
    _RegisterCommand( New CommandCreate() );
    _RegisterCommand( New CommandDestroy() );
    _RegisterCommand( New CommandCWD() );
    _RegisterCommand( New CommandList() );
    _RegisterCommand( New CommandInspect() );
    _RegisterCommand( New CommandRename() );
    _RegisterCommand( New CommandMove() );
    _RegisterCommand( New CommandLoad() );
    _RegisterCommand( New CommandSave() );
    _RegisterCommand( New CommandImport() );
    _RegisterCommand( New CommandExport() );

    UnSelectMemory();
}

Void ConsoleManager::_History_PushCommand( const GChar * strCommandText )
{
    UInt iHistoryIndex = ( m_iHistoryStart + m_iHistoryLength ) & (CONSOLE_COMMAND_HISTORY_SIZE - 1); // % size
    GChar * strHistory = m_arrCommandsHistory + ( iHistoryIndex * CONSOLECOMMAND_INPUT_LENGTH );
    StringFn->NCopy( strHistory, strCommandText, CONSOLECOMMAND_INPUT_LENGTH - 1 );

    if ( m_iHistoryLength ==  CONSOLE_COMMAND_HISTORY_SIZE )
        ++m_iHistoryStart; // Shift
    else
        ++m_iHistoryLength; // Grow

    if ( m_iHistoryIndex != INVALID_OFFSET ) {
        m_iHistoryIndex = INVALID_OFFSET;
        m_strSavedCommand[0] = NULLBYTE;
    }
}
const GChar * ConsoleManager::_History_PrevCommand( const GChar * strCommandText )
{
    if ( m_iHistoryLength == 0 )
        return NULL;

    const GChar * strHistoryCommandText;
    UInt iLastIndex = ( m_iHistoryStart + m_iHistoryLength - 1 ) & (CONSOLE_COMMAND_HISTORY_SIZE - 1);

    if ( m_iHistoryIndex == INVALID_OFFSET ) {
        StringFn->NCopy( m_strSavedCommand, strCommandText, CONSOLECOMMAND_INPUT_LENGTH - 1 );
        m_iHistoryIndex = iLastIndex;
        strHistoryCommandText = m_arrCommandsHistory + ( m_iHistoryIndex * CONSOLECOMMAND_INPUT_LENGTH );
    } else {
        if ( m_iHistoryIndex == m_iHistoryStart ) {
            if ( !m_bHistoryLoop )
                return NULL;
            m_iHistoryIndex = INVALID_OFFSET;
            strHistoryCommandText = m_strSavedCommand;
        } else {
            m_iHistoryIndex = ( m_iHistoryIndex + CONSOLE_COMMAND_HISTORY_SIZE - 1 ) & (CONSOLE_COMMAND_HISTORY_SIZE - 1);
            strHistoryCommandText = m_arrCommandsHistory + ( m_iHistoryIndex * CONSOLECOMMAND_INPUT_LENGTH );
        }
    }
    return strHistoryCommandText;
}
const GChar * ConsoleManager::_History_NextCommand( const GChar * strCommandText )
{
    if ( m_iHistoryLength == 0 )
        return NULL;

    const GChar * strHistoryCommandText;
    UInt iLastIndex = ( m_iHistoryStart + m_iHistoryLength - 1 ) & (CONSOLE_COMMAND_HISTORY_SIZE - 1);

    if ( m_iHistoryIndex == INVALID_OFFSET ) {
        if ( !m_bHistoryLoop )
            return NULL;
        StringFn->NCopy( m_strSavedCommand, strCommandText, CONSOLECOMMAND_INPUT_LENGTH - 1 );
        m_iHistoryIndex = m_iHistoryStart;
        strHistoryCommandText = m_arrCommandsHistory + ( m_iHistoryIndex * CONSOLECOMMAND_INPUT_LENGTH );
    } else {
        if ( m_iHistoryIndex == iLastIndex ) {
            m_iHistoryIndex = INVALID_OFFSET;
            strHistoryCommandText = m_strSavedCommand;
        } else {
            m_iHistoryIndex = ( m_iHistoryIndex + 1 ) & (CONSOLE_COMMAND_HISTORY_SIZE - 1);
            strHistoryCommandText = m_arrCommandsHistory + ( m_iHistoryIndex * CONSOLECOMMAND_INPUT_LENGTH );
        }
    }
    return strHistoryCommandText;
}


