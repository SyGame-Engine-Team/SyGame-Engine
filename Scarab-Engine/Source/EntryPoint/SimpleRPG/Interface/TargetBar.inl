/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Interface/TargetBar.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Interface : TargetBar
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TargetBarModel implementation
inline Bool TargetBarModel::ShowName() const {
    return m_bShowName;
}
inline Bool TargetBarModel::ShowClass() const {
    return m_bShowClass;
}
inline Bool TargetBarModel::ShowValues() const {
    return m_bShowValues;
}
inline Bool TargetBarModel::ShowPercentValues() const {
    return m_bShowPercent;
}
inline Bool TargetBarModel::ShowStatusEffectBar() const {
    return m_bShowStatusEffectBar;
}
inline Bool TargetBarModel::ShowCastBar() const {
    return m_bShowCastBar;
}
inline Bool TargetBarModel::ShowTargetOfTargetBar() const {
    return m_bShowTargetOfTargetBar;
}

inline Void TargetBarModel::SetShowName( Bool bShowName ) {
    m_bShowName = bShowName;
}
inline Void TargetBarModel::SetShowClass( Bool bShowClass ) {
    m_bShowClass = bShowClass;
}
inline Void TargetBarModel::SetShowValues( Bool bShowValues, Bool bShowPercent ) {
    m_bShowValues = bShowValues;
    m_bShowPercent = bShowPercent;
}
inline Void TargetBarModel::SetShowStatusEffectBar( Bool bShowStatusEffectBar ) {
    m_bShowStatusEffectBar = bShowStatusEffectBar;
}
inline Void TargetBarModel::SetShowCastBar( Bool bShowCastBar ) {
    m_bShowCastBar = bShowCastBar;
}
inline Void TargetBarModel::SetShowTargetOfTargetBar( Bool bShowTargetOfTargetBar ) {
    m_bShowTargetOfTargetBar = bShowTargetOfTargetBar;
}

inline Scalar TargetBarModel::GetHPBarSize() const {
    return m_fHPBarSize;
}
inline Scalar TargetBarModel::GetMPBarSize() const {
    return m_fMPBarSize;
}
inline Scalar TargetBarModel::GetStatusEffectBarSize() const {
    return m_fStatusEffectBarSize;
}
inline Scalar TargetBarModel::GetCastBarSize() const {
    return m_fCastBarSize;
}
inline Scalar TargetBarModel::GetTargetOfTargetBarSize() const {
    return m_fTargetOfTargetBarSize;
}

inline Void TargetBarModel::SetHPBarSize( Scalar fHPBarSize ) {
    m_fHPBarSize = Clamp<Scalar>( fHPBarSize, 0.0f, 1.0f );
    m_fMPBarSize = ( 1.0f - m_fHPBarSize );
}
inline Void TargetBarModel::SetMPBarSize( Scalar fMPBarSize ) {
    m_fMPBarSize = Clamp<Scalar>( fMPBarSize, 0.0f, 1.0f );
    m_fHPBarSize = ( 1.0f - m_fMPBarSize );
}
inline Void TargetBarModel::SetStatusEffectBarSize( Scalar fStatusEffectBarSize ) {
    m_fStatusEffectBarSize = Clamp<Scalar>( fStatusEffectBarSize, 0.0f, 1.0f );
    m_fCastBarSize = ( 1.0f - m_fStatusEffectBarSize );
}
inline Void TargetBarModel::SetCastBarSize( Scalar fCastBarSize ) {
    m_fCastBarSize = Clamp<Scalar>( fCastBarSize, 0.0f, 1.0f );
    m_fStatusEffectBarSize = ( 1.0f - m_fCastBarSize );
}
inline Void TargetBarModel::SetTargetOfTargetBarSize( Scalar fTargetOfTargetBarSize ) {
    m_fTargetOfTargetBarSize = Clamp<Scalar>( fTargetOfTargetBarSize, 0.0f, 1.0f );
}

inline Bool TargetBarModel::HasTarget() const {
    BaseCharacter * pTarget = m_bUseFocus ? m_pCharacter->GetFocusedTarget() : m_pCharacter->GetSelectedTarget();
    return ( pTarget != NULL );
}
inline const GChar * TargetBarModel::GetName() const {
    BaseCharacter * pTarget = m_bUseFocus ? m_pCharacter->GetFocusedTarget() : m_pCharacter->GetSelectedTarget();
    if ( pTarget == NULL )
        return NULL;
    return pTarget->GetName();
}
inline CharClass TargetBarModel::GetClass() const {
    BaseCharacter * pTarget = m_bUseFocus ? m_pCharacter->GetFocusedTarget() : m_pCharacter->GetSelectedTarget();
    if ( pTarget == NULL )
        return CHARCLASS_COUNT;
    return pTarget->GetClass();
}
inline UInt TargetBarModel::GetMaxHP() const {
    BaseCharacter * pTarget = m_bUseFocus ? m_pCharacter->GetFocusedTarget() : m_pCharacter->GetSelectedTarget();
    if ( pTarget == NULL )
        return 0;
    return pTarget->GetMaxHP();
}
inline UInt TargetBarModel::GetCurrentHP() const {
    BaseCharacter * pTarget = m_bUseFocus ? m_pCharacter->GetFocusedTarget() : m_pCharacter->GetSelectedTarget();
    if ( pTarget == NULL )
        return 0;
    return pTarget->GetCurrentHP();
}
inline UInt TargetBarModel::GetMaxMP() const {
    BaseCharacter * pTarget = m_bUseFocus ? m_pCharacter->GetFocusedTarget() : m_pCharacter->GetSelectedTarget();
    if ( pTarget == NULL )
        return 0;
    return pTarget->GetMaxMP();
}
inline UInt TargetBarModel::GetCurrentMP() const {
    BaseCharacter * pTarget = m_bUseFocus ? m_pCharacter->GetFocusedTarget() : m_pCharacter->GetSelectedTarget();
    if ( pTarget == NULL )
        return 0;
    return pTarget->GetCurrentMP();
}

inline UInt TargetBarModel::GetStatusEffectCount() const {
    BaseCharacter * pTarget = m_bUseFocus ? m_pCharacter->GetFocusedTarget() : m_pCharacter->GetSelectedTarget();
    if ( pTarget == NULL )
        return 0;
    return pTarget->GetStatusEffectCount();
}
//inline StatusEffectInstance * TargetBarModel::GetStatusEffect( UInt iStatusEffect ) const {
//    BaseCharacter * pTarget = m_bUseFocus ? m_pCharacter->GetFocusedTarget() : m_pCharacter->GetSelectedTarget();
//    if ( pTarget == NULL )
//        return NULL;
//    return pTarget->GetStatusEffectInstance();
//}

inline SkillID TargetBarModel::GetCastingState( Scalar * outCastingTime ) const {
    Assert( outCastingTime != NULL );
    BaseCharacter * pTarget = m_bUseFocus ? m_pCharacter->GetFocusedTarget() : m_pCharacter->GetSelectedTarget();
    if ( pTarget == NULL )
        return SKILLID_UNDEFINED;
    if ( !(pTarget->IsCasting()) ) {
        *outCastingTime = 0.0f;
        return SKILLID_UNDEFINED;
    }
    *outCastingTime = pTarget->GetCastingTime();
    return pTarget->GetCastingSkill();
}

inline Bool TargetBarModel::HasTargetOfTarget() const {
    BaseCharacter * pTarget = m_bUseFocus ? m_pCharacter->GetFocusedTarget() : m_pCharacter->GetSelectedTarget();
    if ( pTarget == NULL )
        return NULL;
    return ( pTarget->GetSelectedTarget() != NULL );
}
inline const GChar * TargetBarModel::GetTargetOfTargetName() const {
    BaseCharacter * pTarget = m_bUseFocus ? m_pCharacter->GetFocusedTarget() : m_pCharacter->GetSelectedTarget();
    if ( pTarget == NULL )
        return NULL;
    pTarget = pTarget->GetSelectedTarget();
    if ( pTarget == NULL )
        return NULL;
    return pTarget->GetName();
}
inline CharClass TargetBarModel::GetTargetOfTargetClass() const {
    BaseCharacter * pTarget = m_bUseFocus ? m_pCharacter->GetFocusedTarget() : m_pCharacter->GetSelectedTarget();
    if ( pTarget == NULL )
        return CHARCLASS_COUNT;
    pTarget = pTarget->GetSelectedTarget();
    if ( pTarget == NULL )
        return CHARCLASS_COUNT;
    return pTarget->GetClass();
}
inline UInt TargetBarModel::GetTargetOfTargetMaxHP() const {
    BaseCharacter * pTarget = m_bUseFocus ? m_pCharacter->GetFocusedTarget() : m_pCharacter->GetSelectedTarget();
    if ( pTarget == NULL )
        return NULL;
    pTarget = pTarget->GetSelectedTarget();
    if ( pTarget == NULL )
        return NULL;
    return pTarget->GetMaxHP();
}
inline UInt TargetBarModel::GetTargetOfTargetCurrentHP() const {
    BaseCharacter * pTarget = m_bUseFocus ? m_pCharacter->GetFocusedTarget() : m_pCharacter->GetSelectedTarget();
    if ( pTarget == NULL )
        return NULL;
    pTarget = pTarget->GetSelectedTarget();
    if ( pTarget == NULL )
        return NULL;
    return pTarget->GetCurrentHP();
}

/////////////////////////////////////////////////////////////////////////////////
// TargetBar implementation

