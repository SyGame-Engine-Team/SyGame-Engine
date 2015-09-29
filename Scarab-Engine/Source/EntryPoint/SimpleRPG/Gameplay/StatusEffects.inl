/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Gameplay/StatusEffects.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Gameplay : StatusEffects
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// StatusEffect implementation
inline StatusEffectType StatusEffect::GetType() const {
    return m_iType;
}
inline StatusEffectID StatusEffect::GetID() const {
    return m_idStatusEffect;
}
inline const GChar * StatusEffect::GetName() const {
    return m_strName;
}

inline UInt StatusEffect::GetMaxRank() const {
    return m_iMaxRank;
}

inline Bool StatusEffect::IsPositive() const {
    return m_bPositive;
}
inline Bool StatusEffect::IsNegative() const {
    return !m_bPositive;
}
inline Bool StatusEffect::IsMovementLimiter() const {
    return m_bMovementLimiter;
}
inline Bool StatusEffect::IsActionLimiter() const {
    return m_bActionLimiter;
}
inline Bool StatusEffect::IsStatModifier() const {
    return m_bStatModifier;
}

inline Scalar StatusEffect::GetDuration() const {
    return m_fDuration;
}
inline Scalar StatusEffect::GetTickPeriod() const {
    return m_fTickPeriod;
}
inline Scalar StatusEffect::GetTickFrequency() const {
    return m_fTickFrequency;
}

inline UInt StatusEffect::GetMaxStacks() const {
    return m_iMaxStacks;
}

inline StatusEffectAnimation * StatusEffect::GetAnimation() const {
    return m_pAnimation;
}

/////////////////////////////////////////////////////////////////////////////////
// StatusEffectInstance implementation
inline StatusEffectInstance & StatusEffectInstance::operator=( const StatusEffectInstance & rhs ) {
    m_pStatusEffect = rhs.m_pStatusEffect;
    m_pOwner = rhs.m_pOwner;
    m_pSource = rhs.m_pSource;
    m_iRank = rhs.m_iRank;
    m_fElapsedTime = rhs.m_fElapsedTime;
    m_fLastTickElapsedTime = rhs.m_fLastTickElapsedTime;
    m_iStacks = rhs.m_iStacks;
    return (*this);
}

inline StatusEffect * StatusEffectInstance::GetStatusEffect() const {
    return m_pStatusEffect;
}
inline BaseCharacter * StatusEffectInstance::GetOwner() const {
    return m_pOwner;
}
inline BaseCharacter * StatusEffectInstance::GetSource() const {
    return m_pSource;
}

inline StatusEffectType StatusEffectInstance::GetType() const {
    return m_pStatusEffect->GetType();
}
inline StatusEffectID StatusEffectInstance::GetID() const {
    return m_pStatusEffect->GetID();
}
inline const GChar * StatusEffectInstance::GetName() const {
    return m_pStatusEffect->GetName();
}

inline UInt StatusEffectInstance::GetRank() const {
    return m_iRank;
}
inline UInt StatusEffectInstance::GetMaxRank() const {
    return m_pStatusEffect->GetMaxRank();
}

inline Bool StatusEffectInstance::IsPositive() const {
    return m_pStatusEffect->IsPositive();
}
inline Bool StatusEffectInstance::IsNegative() const {
    return m_pStatusEffect->IsNegative();
}
inline Bool StatusEffectInstance::IsMovementLimiter() const {
    return m_pStatusEffect->IsMovementLimiter();
}
inline Bool StatusEffectInstance::IsActionLimiter() const {
    return m_pStatusEffect->IsActionLimiter();
}
inline Bool StatusEffectInstance::IsStatModifier() const {
    return m_pStatusEffect->IsStatModifier();
}

inline Scalar StatusEffectInstance::GetDuration() const {
    return m_pStatusEffect->GetDuration();
}
inline Scalar StatusEffectInstance::GetTickPeriod() const {
    return m_pStatusEffect->GetTickPeriod();
}
inline Scalar StatusEffectInstance::GetTickFrequency() const {
    return m_pStatusEffect->GetTickFrequency();
}

inline Bool StatusEffectInstance::HasFade() const {
    return ( m_fElapsedTime >= m_pStatusEffect->GetDuration() );
}
inline Scalar StatusEffectInstance::GetElapsedTime() const {
    return m_fElapsedTime;
}
inline Scalar StatusEffectInstance::GetRemainingTime() const {
    return ( m_pStatusEffect->GetDuration() - m_fElapsedTime );
}

inline UInt StatusEffectInstance::GetStacks() const {
    return m_iStacks;
}
inline UInt StatusEffectInstance::GetMaxStacks() const {
    return m_pStatusEffect->GetMaxStacks();
}

inline StatusEffectAnimation * StatusEffectInstance::GetAnimation() const {
    return m_pStatusEffect->GetAnimation();
}


inline Void StatusEffectInstance::AddStacks( UInt iStacks ) {
    m_iStacks += iStacks;
    if ( m_iStacks > m_pStatusEffect->GetMaxStacks() )
        m_iStacks = m_pStatusEffect->GetMaxStacks();
    Refresh();
}
inline Void StatusEffectInstance::Fade() {
    m_fElapsedTime = m_pStatusEffect->GetDuration();
    m_fLastTickElapsedTime = 0;
    m_pStatusEffect->OnFade( this );
    m_hAnimationInstance.Interrupt();
    m_hAnimationInstance = AnimationInstance();
}
inline Void StatusEffectInstance::Refresh() {
    m_fElapsedTime = 0.0f;
    m_pStatusEffect->OnApply( this );
    m_hAnimationInstance.Restart();
}

/////////////////////////////////////////////////////////////////////////////////
// StatusEffectBuffer implementation
inline UInt StatusEffectBuffer::GetStatusEffectCount() const {
    return m_mapStatusEffects.Count();
}

inline Bool StatusEffectBuffer::HasStatusEffect( StatusEffectID idStatusEffect ) const {
    return m_mapStatusEffects.Contains( idStatusEffect );
}

inline UInt StatusEffectBuffer::GetStatusEffectInstanceCount( StatusEffectID idStatusEffect ) const {
    _StatusEffectMap::Iterator itStatusEffect = m_mapStatusEffects.Get( idStatusEffect );
    if ( itStatusEffect.IsNull() )
        return 0;
    return itStatusEffect.GetItem().iInstanceCount;
}
inline StatusEffectInstance * StatusEffectBuffer::GetStatusEffectInstance( StatusEffectID idStatusEffect, UInt iIndex ) const {
    _StatusEffectMap::Iterator itStatusEffect = m_mapStatusEffects.Get( idStatusEffect );
    if ( itStatusEffect.IsNull() )
        return NULL;
    _StatusEffectData * pData = itStatusEffect.GetItemPtr();
    Assert( iIndex < pData->iInstanceCount );
    return ( pData->arrInstances + iIndex );
}

inline Void StatusEffectBuffer::EnumStatusEffects() const {
    Assert( m_itEnumerate.IsNull() );
    m_itEnumerate = m_mapStatusEffects.Begin();
}
inline StatusEffectID StatusEffectBuffer::EnumNextStatusEffect() const {
    if ( m_itEnumerate.IsNull() )
        return STATUSEFFECTID_UNDEFINED;
    StatusEffectID idStatusEffect = m_itEnumerate.GetKey();
    ++m_itEnumerate;
    return idStatusEffect;
}

inline Void StatusEffectBuffer::EnumStatusEffectInstances() const {
    Assert( !(m_itEnumerate.IsNull()) && m_iEnumerate == INVALID_OFFSET );
    m_iEnumerate = 0;
}
inline StatusEffectInstance * StatusEffectBuffer::EnumNextStatusEffectInstance() const {
    Assert( !(m_itEnumerate.IsNull()) );
    if ( m_iEnumerate == INVALID_OFFSET )
        return NULL;
    StatusEffectInstance * pStatusEffectInstance = ( m_itEnumerate.GetItem().arrInstances + m_iEnumerate );
    ++m_iEnumerate;
    if ( m_iEnumerate >= m_itEnumerate.GetItem().iInstanceCount )
        m_iEnumerate = INVALID_OFFSET;
    return pStatusEffectInstance;
}

/////////////////////////////////////////////////////////////////////////////////

inline StatusEffectBuffer::_StatusEffectData & StatusEffectBuffer::_StatusEffectData::operator=( const _StatusEffectData & rhs ) {
    iInstanceCount = rhs.iInstanceCount;
    for( UInt i = 0; i < iInstanceCount; ++i )
        arrInstances[i] = rhs.arrInstances[i];
    return (*this);
}
