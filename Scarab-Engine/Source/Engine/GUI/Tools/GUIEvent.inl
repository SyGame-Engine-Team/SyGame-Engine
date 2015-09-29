/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Tools/GUIEvent.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Messaging support for the GUI system.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// GUIEvent implementation
inline GUIEventType GUIEvent::Type() const {
    return m_iType;
}
inline GUIEventFlag GUIEvent::Flags() const {
    return m_iFlags;
}
inline const Byte * GUIEvent::Parameter( UInt iParameter ) const {
    Assert( iParameter < m_iParamCount );
	return ( m_arrParameters + m_arrParamOffsets[iParameter] );
}
inline UInt GUIEvent::ParamCount() const {
    return m_iParamCount;
}
