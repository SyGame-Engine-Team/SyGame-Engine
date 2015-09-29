/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Widgets/Window/GUITitleBar.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Widget : Window-specific, Title Bar
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// GUITitleBarModel implementation
inline const GChar * GUITitleBarModel::GetTitle( UInt * outLength ) const {
    if ( outLength != NULL )
        *outLength = m_iTitleLength;
    return m_strTitle;
}

/////////////////////////////////////////////////////////////////////////////////
// GUITitleBar implementation
inline GUIWidgetType GUITitleBar::GetType() const {
    return GUIWIDGET_WINDOW_TITLEBAR;
}

