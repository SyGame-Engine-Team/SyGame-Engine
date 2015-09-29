/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/GUIWindow.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GUIWindow is the root dynamic container for GUI layouts.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// GUIWindow implementation
inline const GChar * GUIWindow::GetName() const {
    return m_strName;
}
inline const Rectangle2 & GUIWindow::GetWindowRect() const {
    return m_rectWindow;
}
inline UInt GUIWindow::GetDepth() const {
    return m_iDepth;
}
inline GUIWindowStyle GUIWindow::GetStyle() const {
    return m_iWinStyle;
}

inline Bool GUIWindow::IsVisible() const {
    return m_bIsVisible;
}

inline Bool GUIWindow::HasFocus() const {
    return m_bHasFocus;
}
inline GUIWidget * GUIWindow::GetFocus() const {
    return m_pFocus;
}
inline Bool GUIWindow::HasMouseOver() const {
    return m_bHasMouseOver;
}
inline GUIWidget * GUIWindow::GetMouseOver() const {
    return m_pMousedOver;
}

inline Bool GUIWindow::IsEventBlocking() const {
    return m_bEventBlocking;
}
inline Void GUIWindow::SetEventBlocking( Bool bEventBlocking ) {
    m_bEventBlocking = bEventBlocking;
}

inline GUIWidget * GUIWindow::GetClientRoot() const {
    return m_pClientRoot;
}

inline Void GUIWindow::ScreenToWindow( Point2 * outWindowPos, const Point2 & ptScreenPos ) const {
    outWindowPos->X = ( ptScreenPos.X - m_rectWindow.TopLeft.X );
    outWindowPos->Y = ( ptScreenPos.Y - m_rectWindow.TopLeft.Y );
}
inline Void GUIWindow::WindowToScreen( Point2 * outScreenPos, const Point2 & ptWindowPos ) const {
    outScreenPos->X = ( m_rectWindow.TopLeft.X + ptWindowPos.X );
    outScreenPos->Y = ( m_rectWindow.TopLeft.Y + ptWindowPos.Y );
}

inline GUIWidget * GUIWindow::PickContainer( const Point2 & ptScreenPos ) const {
    return m_pWindowRoot->PickChild( ptScreenPos );
}

inline Bool GUIWindow::HasContextMenu() const {
    if ( m_pTitleBarModel == NULL )
        return false;
    return m_pTitleBarModel->HasContextMenu();
}
inline GUIMenuModel * GUIWindow::GetContextMenu() const {
    if ( m_pTitleBarModel == NULL )
        return NULL;
    return m_pTitleBarModel->GetContextMenu();
}
inline Void GUIWindow::SetContextMenu( GUIMenuModel * pContextMenuModel ) {
    Assert( m_pTitleBarModel != NULL );
    m_pTitleBarModel->SetContextMenu( pContextMenuModel );
}

/////////////////////////////////////////////////////////////////////////////////

inline Void GUIWindow::_PostEvent_Resize() {
    m_pWindowRoot->_FloodResizeEvent();
}
inline Void GUIWindow::_PostEvent_Exit() {
    m_pWindowRoot->_FloodExitEvent();
}

inline Bool GUIWindow::_ProcessNextEvent() {
    return m_pWindowRoot->_ProcessNextEvent();
}

//inline Void GUIWindow::SetTitleBar( GUITitleBar * pTitleBar ) {
//    Assert(!m_bFinalized);
//    m_pTitleBar = pTitleBar;
//}
//inline Void GUIWindow::SetMenuBar( GUIMenuBar * pMenuBar ) {
//    Assert(!m_bFinalized);
//    m_pMenuBar = pMenuBar;
//}
//inline Void GUIWindow::SetStateBar( GUIStateBar * pStateBar ) {
//    Assert(!m_bFinalized);
//    m_pStateBar = pStateBar;
//}
