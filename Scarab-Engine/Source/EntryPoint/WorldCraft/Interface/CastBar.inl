/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Interface/CastBar.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Interface : CastBar
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// CastBarModel implementation
inline Bool CastBarModel::ShowName() const {
    return m_bShowName;
}
inline Bool CastBarModel::ShowValues() const {
    return m_bShowValues;
}

inline Void CastBarModel::SetShowName( Bool bShowName ) {
    m_bShowName = bShowName;
}
inline Void CastBarModel::SetShowValues( Bool bShowValues ) {
    m_bShowValues = bShowValues;
}

inline Bool CastBarModel::IsCasting() const {
    return m_pCharacter->IsCasting();
}
inline SkillID CastBarModel::GetCastingState( Scalar * outCastingTime ) const {
    Assert( outCastingTime != NULL );
    if ( !(m_pCharacter->IsCasting()) ) {
        *outCastingTime = 0.0f;
        return SKILLID_UNDEFINED;
    }
    *outCastingTime = m_pCharacter->GetCastingTime();
    return m_pCharacter->GetCastingSkill();
}

/////////////////////////////////////////////////////////////////////////////////
// CastBar implementation

