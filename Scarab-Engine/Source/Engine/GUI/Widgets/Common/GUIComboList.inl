/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Widgets/Common/GUIComboList.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Widget : Common controls, ComboList
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// GUIComboListMenuModel implementation

/////////////////////////////////////////////////////////////////////////////////
// GUIComboListModel implementation
inline UInt GUIComboListModel::GetItemCount() const {
    return m_pComboListMenuModel->GetItemCount();
}
inline const GChar * GUIComboListModel::GetItemName( UInt iItem, UInt * outLength ) const {
    return m_pComboListMenuModel->GetItem(iItem)->GetName( outLength );
}
inline GUISkinImage GUIComboListModel::GetItemIcon( UInt iItem ) const {
    return m_pComboListMenuModel->GetItem(iItem)->GetIcon();
}
inline Void * GUIComboListModel::GetItemUserData( UInt iItem ) const {
    return m_pComboListMenuModel->GetItem(iItem)->GetUserData();
}

inline UInt GUIComboListModel::GetSelectedItem() const {
    return m_iSelectedItem;
}
inline const GChar * GUIComboListModel::GetSelectedItemName( UInt * outLength ) const {
    return m_pComboListMenuModel->GetItem(m_iSelectedItem)->GetName( outLength );
}
inline GUISkinImage GUIComboListModel::GetSelectedItemIcon() const {
    return m_pComboListMenuModel->GetItem(m_iSelectedItem)->GetIcon();
}
inline Void * GUIComboListModel::GetSelectedItemUserData() const {
    return m_pComboListMenuModel->GetItem(m_iSelectedItem)->GetUserData();
}

inline Void GUIComboListModel::SelectItem( UInt iItem ) {
    Assert( !m_bOpened );
    Assert( iItem < m_pComboListMenuModel->GetItemCount() );
    m_iSelectedItem = iItem;
}

inline Bool GUIComboListModel::IsOpened() const {
    return m_bOpened;
}
inline Bool GUIComboListModel::IsClosed() const {
    return !m_bOpened;
}

/////////////////////////////////////////////////////////////////////////////////
// GUIComboList implementation
inline GUIWidgetType GUIComboList::GetType() const {
    return GUIWIDGET_COMBOLIST;
}


