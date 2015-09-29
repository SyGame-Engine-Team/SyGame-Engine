/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/ConsoleManager.inl
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
// ConsoleInputModel implementation

/////////////////////////////////////////////////////////////////////////////////
// ConsoleLogModel implementation

/////////////////////////////////////////////////////////////////////////////////
// ConsoleManager implementation
inline const GChar * ConsoleManager::GetName() {
    return TEXT("ConsoleManager");
}
inline const GChar * ConsoleManager::GetMemoryBudget() {
    return TEXT (
"Heap:Scratch(4194304);Heap:Commands(4194304);Heap:Environment(4194304);Heap:Interfaces(4194304);"
    );
}

inline ConsoleInputModel * ConsoleManager::GetInputModel() const {
    return m_pInputModel;
}
inline ConsoleLogModel * ConsoleManager::GetLogModel() const {
    return m_pLogModel;
}

inline Void ConsoleManager::Show() {
    m_pWindow->SetVisible( true );
}
inline Void ConsoleManager::Hide() {
    m_pWindow->SetVisible( false );
}
inline Void ConsoleManager::Toggle() {
    m_pWindow->SetVisible( !(m_pWindow->IsVisible()) );
}

inline Void ConsoleManager::ShowVideoSettings() {
    m_pVideoSettingsWindow->Show();
}
inline Void ConsoleManager::HideVideoSettings() {
    m_pVideoSettingsWindow->Hide();
}
inline Void ConsoleManager::ToggleVideoSettings() {
    m_pVideoSettingsWindow->Toggle();
}

inline Void ConsoleManager::ShowResourceView() {
    m_pResourceViewerWindow->Show();
}
inline Void ConsoleManager::HideResourceView() {
    m_pResourceViewerWindow->Hide();
}
inline Void ConsoleManager::ToggleResourceView() {
    m_pResourceViewerWindow->Toggle();
}

inline ConsoleCommandLevel ConsoleManager::GetLevel() const {
    return m_iConsoleLevel;
}
inline Void ConsoleManager::SetLevel( ConsoleCommandLevel iLevel ) {
    m_iConsoleLevel = iLevel;
}

inline ConsoleEnvironment * ConsoleManager::GetEnvironment() {
    return &m_cEnvironment;
}

inline Void ConsoleManager::EnumCommands( ConsoleCommandCategory iCategory ) const {
    Assert( iCategory < CONSOLECOMMAND_CATEGORY_COUNT );
    m_itEnumerate = m_mapCommands[iCategory].Begin();
}
inline ConsoleCommand * ConsoleManager::EnumNextCommand() const {
    if ( m_itEnumerate.IsNull() )
        return NULL;
    ConsoleCommand * pCommand = m_itEnumerate.GetItem();
    ++m_itEnumerate;
    return pCommand;
}

inline Void ConsoleManager::SetHistoryLoop( Bool bLoop ) {
    m_bHistoryLoop = bLoop;
}

/////////////////////////////////////////////////////////////////////////////////

inline Int ConsoleManager::_Name_Comparator( const GChar* const & rLeft, const GChar* const & rRight, Void * /*pUserData*/ ) {
    return StringFn->NCmp( rLeft, rRight, CONSOLECOMMAND_NAME_LENGTH - 1 );
}
