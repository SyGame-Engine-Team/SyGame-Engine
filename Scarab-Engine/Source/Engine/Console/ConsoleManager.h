/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/ConsoleManager.h
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
// Known Bugs : Command syntax =
//                  <command> = /<category> <name> (<arglist>)
//                            | <string_max>
//                  <category> = <string_name>
//                  <name> = <string_name>
//                  <arglist> = <arg> <arglist>
//                            | <arg>
//                  <arg> = <int> | <scalar> | <string_arg>
//                  <int> = integer value, standard string format
//                  <scalar> = float value, standard string format
//                  <string_name> = string | "string", length = CONSOLECOMMAND_NAME_LENGTH
//                  <string_arg>  = string | "string", length = CONSOLECOMMAND_ARGUMENT_LENGTH
//                  <string_max>  = string | "string", length = GUITEXTINPUT_MAX_LENGTH
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_CONSOLE_CONSOLEMANAGER_H
#define SCARAB_ENGINE_CONSOLE_CONSOLEMANAGER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Lib/Object/Patterns/Manager.h"

#include "../../Lib/String/PatternMatching.h"

#include "../GUI/GUIManager.h"

#include "ConsoleCommand.h"
#include "ConsoleEnvironment.h"

#include "GlobalCommands/CommandHelp.h"
#include "GlobalCommands/CommandLookup.h"
#include "GlobalCommands/CommandEcho.h"
#include "GlobalCommands/CommandClear.h"
#include "GlobalCommands/CommandDump.h"
#include "GlobalCommands/CommandRun.h"
#include "GlobalCommands/CommandVar.h"
#include "GlobalCommands/CommandVarNew.h"
#include "GlobalCommands/CommandVarDel.h"
#include "GlobalCommands/CommandVarSet.h"

#include "GUICommands/CommandShowTree.h"

#include "ResourceCommands/CommandRDB.h"
#include "ResourceCommands/CommandOpen.h"
#include "ResourceCommands/CommandClose.h"
#include "ResourceCommands/CommandCreate.h"
#include "ResourceCommands/CommandDestroy.h"
#include "ResourceCommands/CommandCWD.h"
#include "ResourceCommands/CommandList.h"
#include "ResourceCommands/CommandInspect.h"
#include "ResourceCommands/CommandRename.h"
#include "ResourceCommands/CommandMove.h"
#include "ResourceCommands/CommandLoad.h"
#include "ResourceCommands/CommandSave.h"
#include "ResourceCommands/CommandImport.h"
#include "ResourceCommands/CommandExport.h"

#include "Interfaces/VideoSettings.h"
#include "Interfaces/ResourceViewer.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define ConsoleFn ConsoleManager::GetInstancePtr()

#define CONSOLE_WINDOW_WIDTH  600
#define CONSOLE_WINDOW_HEIGHT 320

#define CONSOLE_LOGLINE_HEIGHT 20
#define CONSOLE_INPUTLINE_HEIGHT 20

#define CONSOLE_COMMAND_HISTORY_SIZE 64 // Must be power of 2

#define CONSOLE_COMMAND_LOOKUP_MAXRESULTS 32

#define CONSOLE_PATH_SCRIPT TEXT("Data/Scripts/Console")
#define CONSOLE_PATH_LOG    TEXT("Logs/Console")

/////////////////////////////////////////////////////////////////////////////////
// The ConsoleLogModel class
class ConsoleLogModel : public GUITextLogModel
{
public:
    ConsoleLogModel();
    virtual ~ConsoleLogModel();

    // Controller
    virtual Void OnTextLog();
};

/////////////////////////////////////////////////////////////////////////////////
// The ConsoleInputModel class
class ConsoleInputModel : public GUITextInputModel
{
public:
    ConsoleInputModel( ConsoleLogModel * pLogModel );
    virtual ~ConsoleInputModel();

    // Controller
    virtual Void OnKeyPress( KeyCode iKey, GUIEventFlag iFlags );

    virtual Void OnTextChange();
    virtual Void OnCursorChange();
    virtual Void OnSelectionChange();

private:
    ConsoleLogModel * m_pLogModel;
};

/////////////////////////////////////////////////////////////////////////////////
// The ConsoleManager class
class ConsoleManager : public Manager<ConsoleManager>
{
public:
    inline static const GChar * GetName();
    inline static const GChar * GetMemoryBudget();

protected:
    friend class Manager<ConsoleManager>;
    ConsoleManager();
    virtual ~ConsoleManager();

public:
    Void Initialize();
    Void Cleanup();

    // Models access
    inline ConsoleInputModel * GetInputModel() const;
    inline ConsoleLogModel * GetLogModel() const;

    // Show/Hide console window
    inline Void Show();
    inline Void Hide();
    inline Void Toggle();

    // Show/Hide other windows
    inline Void ShowVideoSettings();
    inline Void HideVideoSettings();
    inline Void ToggleVideoSettings();

    inline Void ShowResourceView();
    inline Void HideResourceView();
    inline Void ToggleResourceView();

    // Console's security level
    inline ConsoleCommandLevel GetLevel() const;
    inline Void SetLevel( ConsoleCommandLevel iLevel );

    // Console environment
    inline ConsoleEnvironment * GetEnvironment();

    // Commands database
    ConsoleCommand * GetCommand( ConsoleCommandCategory iCategory, const GChar * strName ) const;
    UInt LookupCommand( ConsoleCommand ** arrLookupResults, ConsoleCommandCategory iCategory, const GChar * strLookup ) const;

    inline Void EnumCommands( ConsoleCommandCategory iCategory ) const;
    inline ConsoleCommand * EnumNextCommand() const;

    // Commands history
    inline Void SetHistoryLoop( Bool bLoop );

    // Command execution
    Bool Execute( GChar * strOutput, const GChar * strCommand );
    Bool RunScript( const GChar * strScriptName, Bool bLogOutput = true );

private:
    // Give access to models
    friend class ConsoleLogModel;
    friend class ConsoleInputModel;

    // Helpers
    typedef TreeMap<const GChar*, ConsoleCommand*> CommandMap;
    inline static Int _Name_Comparator( const GChar* const & rLeft, const GChar* const & rRight, Void * pUserData );

    // GUI interface
    ConsoleInputModel * m_pInputModel;
    ConsoleLogModel * m_pLogModel;

    GUIWindow * m_pWindow;

    VideoSettingsWindow * m_pVideoSettingsWindow;
    ResourceViewerWindow * m_pResourceViewerWindow;

    // Console's security level
    ConsoleCommandLevel m_iConsoleLevel;

    // Console environment
    ConsoleEnvironment m_cEnvironment;

    // Commands database
    Void _RegisterCommand( ConsoleCommand * pEngineCmd );
    Void _InstallCommands();

    CommandMap m_mapCommands[CONSOLECOMMAND_CATEGORY_COUNT];
    mutable CommandMap::Iterator m_itEnumerate;

    // Commands history
    Void _History_PushCommand( const GChar * strCommandText );
    const GChar * _History_PrevCommand( const GChar * strCommandText );
    const GChar * _History_NextCommand( const GChar * strCommandText );

    Bool m_bHistoryLoop;
    UInt m_iHistoryStart, m_iHistoryLength;
    UInt m_iHistoryIndex;
    GChar m_strSavedCommand[GUITEXTINPUT_MAX_LENGTH];
    GChar m_arrCommandsHistory[CONSOLE_COMMAND_HISTORY_SIZE * CONSOLECOMMAND_INPUT_LENGTH];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ConsoleManager.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_CONSOLE_CONSOLEMANAGER_H

