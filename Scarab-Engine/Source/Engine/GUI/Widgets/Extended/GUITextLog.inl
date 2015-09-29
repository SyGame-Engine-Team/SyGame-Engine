/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Widgets/Extended/GUITextLog.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Widget : Extended controls, Multiline text view
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// GUITextLogLine implementation
inline UInt GUITextLogLine::GetLineCount() const {
    return m_iLineCount;
}
inline UInt GUITextLogLine::GetSplit( UInt iSplit ) const {
    Assert( iSplit < m_iLineCount );
    return m_arrSplits[iSplit];
}

inline UInt GUITextLogLine::GetLength() const {
    return m_iLength;
}
inline const GChar * GUITextLogLine::GetText( UInt * outLength ) const {
    if ( outLength != NULL )
        *outLength = m_iLength;
    return m_strText;
}

/////////////////////////////////////////////////////////////////////////////////
// GUITextLogModel implementation
inline UInt GUITextLogModel::GetDisplayBottom() const {
    return m_iDisplayBottom;
}
inline UInt GUITextLogModel::GetDisplayBottomInner() const {
    return m_iDisplayBottomInner;
}
inline UInt GUITextLogModel::GetDisplayLineCount() const {
    return m_iDisplayLineCount;
}

inline UInt GUITextLogModel::GetLineHeight() const {
    return m_iLineHeight;
}

inline UInt GUITextLogModel::GetLineCount() const {
    return m_arrLines.Count();
}
inline GUITextLogLine * GUITextLogModel::GetLine( UInt iLine ) const {
    Assert( iLine < m_arrLines.Count() );
    return (GUITextLogLine*)( &(m_arrLines[iLine]) );
}

/////////////////////////////////////////////////////////////////////////////////
// GUITextLog implementation
inline GUIWidgetType GUITextLog::GetType() const {
    return GUIWIDGET_TEXTLOG;
}
