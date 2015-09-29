/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Interface/SelfBar.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Interface : SelfBar
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// SelfBarModel implementation
inline Bool SelfBarModel::ShowName() const {
    return m_bShowName;
}
inline Bool SelfBarModel::ShowClass() const {
    return m_bShowClass;
}
inline Bool SelfBarModel::ShowValues() const {
    return m_bShowValues;
}
inline Bool SelfBarModel::ShowPercentValues() const {
    return m_bShowPercent;
}

inline Void SelfBarModel::SetShowName( Bool bShowName ) {
    m_bShowName = bShowName;
}
inline Void SelfBarModel::SetShowClass( Bool bShowClass ) {
    m_bShowClass = bShowClass;
}
inline Void SelfBarModel::SetShowValues( Bool bShowValues, Bool bShowPercent ) {
    m_bShowValues = bShowValues;
    m_bShowPercent = bShowPercent;
}

inline Scalar SelfBarModel::GetHPBarSize() const {
    return m_fHPBarSize;
}
inline Scalar SelfBarModel::GetMPBarSize() const {
    return m_fMPBarSize;
}

inline Void SelfBarModel::SetHPBarSize( Scalar fHPBarSize ) {
    m_fHPBarSize = Clamp<Scalar>( fHPBarSize, 0.0f, 1.0f );
    m_fMPBarSize = ( 1.0f - m_fHPBarSize );
}
inline Void SelfBarModel::SetMPBarSize( Scalar fMPBarSize ) {
    m_fMPBarSize = Clamp<Scalar>( fMPBarSize, 0.0f, 1.0f );
    m_fHPBarSize = ( 1.0f - m_fMPBarSize );
}

inline const GChar * SelfBarModel::GetName() const {
    return m_pCharacter->GetName();
}
inline CharClass SelfBarModel::GetClass() const {
    return m_pCharacter->GetClass();
}
inline UInt SelfBarModel::GetMaxHP() const {
    return m_pCharacter->GetMaxHP();
}
inline UInt SelfBarModel::GetCurrentHP() const {
    return m_pCharacter->GetCurrentHP();
}
inline UInt SelfBarModel::GetMaxMP() const {
    return m_pCharacter->GetMaxMP();
}
inline UInt SelfBarModel::GetCurrentMP() const {
    return m_pCharacter->GetCurrentMP();
}

/////////////////////////////////////////////////////////////////////////////////
// SelfBar implementation
inline GUIWidgetType SelfBar::GetType() const {
    return GUIWIDGET_CUSTOM;
}

