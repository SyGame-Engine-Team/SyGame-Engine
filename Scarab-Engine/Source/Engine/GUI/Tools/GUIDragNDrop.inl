/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Tools/GUIDragNDrop.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Drag & Drop state management
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// GUIDragNDrop implementation
inline Bool GUIDragNDrop::IsPrepared( GUIDragChannel iChannel ) const {
    return m_arrDragData[iChannel].bPrepared;
}
inline Bool GUIDragNDrop::IsConfirmed( GUIDragChannel iChannel ) const {
    return m_arrDragData[iChannel].bConfirmed;
}

inline GUIEventProcessor * GUIDragNDrop::GetSource( GUIDragChannel iChannel ) const {
    return m_arrDragData[iChannel].pSource;
}
inline const Point2 & GUIDragNDrop::GetSourcePos( GUIDragChannel iChannel ) const {
    return m_arrDragData[iChannel].ptSourcePos;
}

inline GUIEventProcessor * GUIDragNDrop::GetDest( GUIDragChannel iChannel ) const {
    return m_arrDragData[iChannel].pDest;
}
inline const Point2 & GUIDragNDrop::GetDestPos( GUIDragChannel iChannel ) const {
    return m_arrDragData[iChannel].ptDestPos;
}

inline Void * GUIDragNDrop::GetUserData( GUIDragChannel iChannel ) const {
    return m_arrDragData[iChannel].pUserData;
}
inline Void GUIDragNDrop::SetUserData( GUIDragChannel iChannel, Void * pUserData ) {
    m_arrDragData[iChannel].pUserData = pUserData;
}

