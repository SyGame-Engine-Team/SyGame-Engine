/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Widgets/Common/GUITextInput.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Widget : Common controls, Text Input (monoline)
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// GUITextInputModel implementation
inline UInt GUITextInputModel::GetDisplayStart() const {
    return m_iDisplayStart;
}
inline UInt GUITextInputModel::GetDisplayEnd() const {
    return m_iDisplayEnd;
}
inline UInt GUITextInputModel::GetDisplayText( GChar * outDisplayText ) const {
    return ( StringFn->NCopy( outDisplayText, m_strText + m_iDisplayStart, (m_iDisplayEnd + 1 - m_iDisplayStart) ) - outDisplayText );
}

inline UInt GUITextInputModel::GetCursor() const {
    return m_iCursor;
}
inline Void GUITextInputModel::SetCursor( UInt iCursorPos ) {
    if ( iCursorPos < m_iCursor )
        CursorDecrease( m_iCursor - iCursorPos );
    else if ( iCursorPos > m_iCursor )
        CursorIncrease( iCursorPos - m_iCursor );
}

inline Bool GUITextInputModel::HasSelection() const {
    return ( m_iSelectionStart != INVALID_OFFSET );
}
inline UInt GUITextInputModel::GetSelectionStart() const {
    return m_iSelectionStart;
}
inline UInt GUITextInputModel::GetSelectionEnd() const {
    return m_iSelectionEnd;
}
inline UInt GUITextInputModel::GetSelectionLength() const {
    return ( m_iSelectionEnd + 1 - m_iSelectionStart );
}

inline Bool GUITextInputModel::IsInsertMode() const {
    return m_bInsertMode;
}

inline UInt GUITextInputModel::GetTextLength() const {
    return m_iLength;
}
inline const GChar * GUITextInputModel::GetText() const {
    return m_strText;
}

/////////////////////////////////////////////////////////////////////////////////
// GUITextInput implementation
inline GUIWidgetType GUITextInput::GetType() const {
    return GUIWIDGET_TEXTINPUT;
}
