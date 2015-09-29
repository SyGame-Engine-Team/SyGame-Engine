/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Overlays/GUIMenu.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Overlay : Generic menu interface
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// GUIMenuItem implementation
inline Bool GUIMenuItem::IsActive() const {
    return m_bActive;
}

inline Bool GUIMenuItem::HasSubMenu() const {
    return (m_pSubMenu != NULL);
}
inline GUIMenuModel * GUIMenuItem::GetSubMenu() const {
    return m_pSubMenu;
}

inline const GChar * GUIMenuItem::GetName( UInt * outLentgh ) const {
    if ( outLentgh != NULL )
        *outLentgh = m_iNameLength;
    return m_strName;
}
inline GUISkinImage GUIMenuItem::GetIcon() const {
    return m_idIcon;
}
inline Void * GUIMenuItem::GetUserData() const {
    return m_pUserData;
}

/////////////////////////////////////////////////////////////////////////////////
// GUIMenuModel implementation
inline UInt GUIMenuModel::GetItemHeight() const {
    return m_iItemHeight;
}

inline UInt GUIMenuModel::GetItemCount() const {
    return m_iItemCount;
}
inline const GUIMenuItem * GUIMenuModel::GetItem( UInt iItem ) const {
    Assert( iItem < m_iItemCount );
    return ( m_arrItems + iItem );
}

inline Void GUIMenuModel::SetActive( UInt iItem, Bool bActive ) {
    Assert( iItem < m_iItemCount );
    m_arrItems[iItem].m_bActive = bActive;
}

inline UInt GUIMenuModel::GetSelectedItem() const {
    return m_iSelectedItem;
}
inline Void GUIMenuModel::SelectItem( UInt iItem ) {
    Assert( iItem < m_iItemCount );
    if ( m_arrItems[iItem].m_bActive )
        m_iSelectedItem = iItem;
    else
        m_iSelectedItem = INVALID_OFFSET;
}

/////////////////////////////////////////////////////////////////////////////////
// GUIMenu implementation
inline GUIOverlayType GUIMenu::GetType() const {
    return GUIOVERLAY_MENU;
}

inline UInt GUIMenu::GetItemCount() const {
    return m_iItemCount;
}
inline GUIMenu * GUIMenu::GetSubMenu( UInt iItem ) const {
    Assert( iItem < m_iItemCount );
    return m_arrSubMenus[iItem];
}

