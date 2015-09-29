/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Widgets/Extended/GUISwitchTabs.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Widget : Extended controls, Switching Tabbed containers
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// GUISwitchTabsButtonModel implementation

/////////////////////////////////////////////////////////////////////////////////
// GUISwitchTabsModel implementation
inline UInt GUISwitchTabsModel::GetTabCount() const {
    return m_iTabCount;
}
inline GUISectionModel * GUISwitchTabsModel::GetTab( UInt iTab ) const {
    Assert( iTab < m_iTabCount );
    return m_arrTabModels[iTab];
}

inline GUISectionModel * GUISwitchTabsModel::GetButtonBar() const {
    return m_pButtonBarModel;
}
inline GUISwitchTabsButtonModel * GUISwitchTabsModel::GetButton( UInt iTab ) const {
    Assert( iTab < m_iTabCount );
    return m_arrButtonModels[iTab];
}

inline UInt GUISwitchTabsModel::GetSelectedTab() const {
    return m_iSelectedTab;
}

/////////////////////////////////////////////////////////////////////////////////
// GUISwitchTabs implementation
inline GUIWidgetType GUISwitchTabs::GetType() const {
    return GUIWIDGET_SWITCHTABS;
}

inline UInt GUISwitchTabs::GetTabCount() const {
    return m_iTabCount;
}
inline GUISection * GUISwitchTabs::GetTab( UInt iTab ) const {
    Assert( iTab < m_iTabCount );
    return m_arrTabs[iTab];
}

