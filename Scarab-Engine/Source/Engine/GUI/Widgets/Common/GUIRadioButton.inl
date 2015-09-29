/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Widgets/Common/GUIRadioButton.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Widget : Common controls, Radio buttons
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// GUIRadioButtonGroup implementation
inline UInt GUIRadioButtonGroup::GetModelCount() const {
    return m_arrModels.Count();
}
inline GUIRadioButtonModel * GUIRadioButtonGroup::GetModel( UInt iModel ) const {
    Assert( iModel < m_arrModels.Count() );
    return m_arrModels[iModel];
}

inline Bool GUIRadioButtonGroup::IsTicked( UInt iModel ) const {
    Assert( iModel < m_arrModels.Count() );
    return ( iModel == m_iTicked );
}
inline UInt GUIRadioButtonGroup::GetTickedIndex() const {
    return m_iTicked;
}
inline GUIRadioButtonModel * GUIRadioButtonGroup::GetTickedModel() const {
    if ( m_iTicked == INVALID_OFFSET )
        return NULL;
    return m_arrModels[m_iTicked];
}

/////////////////////////////////////////////////////////////////////////////////
// GUIRadioButtonModel implementation
inline GUIRadioButtonGroup * GUIRadioButtonModel::GetGroup() const {
    return m_pGroup;
}

inline Bool GUIRadioButtonModel::IsCleared() const {
    return !m_bTicked;
}
inline Bool GUIRadioButtonModel::IsTicked() const {
    return m_bTicked;
}

/////////////////////////////////////////////////////////////////////////////////
// GUIRadioButton implementation
inline GUIWidgetType GUIRadioButton::GetType() const {
    return GUIWIDGET_RADIOBUTTON;
}
