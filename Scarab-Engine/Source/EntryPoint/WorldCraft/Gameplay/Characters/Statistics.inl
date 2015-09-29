/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Gameplay/Statistics.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Gameplay : Statistics
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// CharStatPackage implementation
inline Scalar CharStatPackage::GetBaseStat( CharStatistic iStat ) const {
    Assert( iStat < CHARSTAT_COUNT );
    return m_arrBaseStat[iStat];
}
inline Scalar CharStatPackage::GetBaseStatModifier( CharStatistic iStat ) const {
    Assert( iStat < CHARSTAT_COUNT );
    return m_arrBaseStatModifier[iStat];
}
inline Scalar CharStatPackage::GetModdedStat( CharStatistic iStat ) const {
    Assert( iStat < CHARSTAT_COUNT );
    return m_arrModdedStat[iStat];
}
inline Scalar CharStatPackage::GetPassiveStatModifier( CharStatistic iStat ) const {
    Assert( iStat < CHARSTAT_COUNT );
    return m_arrPassiveStatModifier[iStat];
}
inline Scalar CharStatPackage::GetItemStatModifier( CharStatistic iStat ) const {
    Assert( iStat < CHARSTAT_COUNT );
    return m_arrItemStatModifier[iStat];
}
inline Scalar CharStatPackage::GetFinalStat( CharStatistic iStat ) const {
    Assert( iStat < CHARSTAT_COUNT );
    return m_arrFinalStat[iStat];
}
inline Scalar CharStatPackage::GetActiveStatModifier( CharStatistic iStat ) const {
    Assert( iStat < CHARSTAT_COUNT );
    return m_arrActiveStatModifier[iStat];
}
inline Scalar CharStatPackage::GetEffectiveStat( CharStatistic iStat ) const {
    Assert( iStat < CHARSTAT_COUNT );
    return m_arrEffectiveStat[iStat];
}

inline UInt CharStatPackage::GetMaxHP() const {
    return m_iMaxHP;
}
inline UInt CharStatPackage::GetMaxMP() const {
    return m_iMaxMP;
}
inline Scalar CharStatPackage::GetMovementSpeed() const {
    return m_fMovementSpeed;
}

inline Void CharStatPackage::OnLevelChange() {
    _ComputeBaseStats();
    _ComputeBaseStatModifiers();
    _ComputePersistentStatModifiers();
    _ComputeTemporaryStatModifiers();
    _ComputeSecondaryStats();
}
inline Void CharStatPackage::OnPassiveChange() {
    _ComputePersistentStatModifiers();
    _ComputeTemporaryStatModifiers();
    _ComputeSecondaryStats();
}
inline Void CharStatPackage::OnItemChange() {
    _ComputePersistentStatModifiers();
    _ComputeTemporaryStatModifiers();
    _ComputeSecondaryStats();
}
inline Void CharStatPackage::OnActiveChange() {
    _ComputeTemporaryStatModifiers();
    _ComputeSecondaryStats();
}

/////////////////////////////////////////////////////////////////////////////////

inline Scalar CharStatPackage::_ComputeBaseStatModifier_Health( Scalar fEndurance ) {
    return ( fEndurance * 0.0f );
}
inline Scalar CharStatPackage::_ComputeBaseStatModifier_Mana( Scalar fWisdom ) {
    return ( fWisdom * 0.0f );
}
inline Scalar CharStatPackage::_ComputeBaseStatModifier_RegenHealth( Scalar fEndurance ) {
    return ( fEndurance * 0.0f );
}
inline Scalar CharStatPackage::_ComputeBaseStatModifier_RegenMana( Scalar fWisdom ) {
    return ( fWisdom * 0.0f );
}
inline Scalar CharStatPackage::_ComputeBaseStatModifier_Dodge( Scalar fDexterity, Scalar fSpeed ) {
    return ( ( fDexterity * 0.0f ) + ( fSpeed * 0.0f ) );
}
inline Scalar CharStatPackage::_ComputeBaseStatModifier_Block( Scalar fStrength ) {
    return ( fStrength * 0.0f );
}
inline Scalar CharStatPackage::_ComputeBaseStatModifier_Defense( Scalar fEndurance, Scalar fArmor ) {
    return ( ( fEndurance * 0.0f ) + ( fArmor * 0.0f ) );
}
inline Scalar CharStatPackage::_ComputeBaseStatModifier_Resistance( Scalar fEndurance, Scalar fArmor, Scalar fIntelligence ) {
    return ( ( fEndurance * 0.0f ) + ( fArmor * 0.0f ) + ( fIntelligence * 0.0f ) );
}
inline Scalar CharStatPackage::_ComputeBaseStatModifier_Power( Scalar fStrength ) {
    return ( fStrength * 0.0f );
}
inline Scalar CharStatPackage::_ComputeBaseStatModifier_Hit( Scalar fDexterity ) {
    return ( fDexterity * 0.0f );
}
inline Scalar CharStatPackage::_ComputeBaseStatModifier_Haste( Scalar fDexterity, Scalar fSpeed ) {
    return ( ( fDexterity * 0.0f ) + ( fSpeed * 0.0f ) );
}
inline Scalar CharStatPackage::_ComputeBaseStatModifier_CriticalPower( Scalar fStrength ) {
    return ( fStrength * 0.0f );
}
inline Scalar CharStatPackage::_ComputeBaseStatModifier_CriticalHit( Scalar fDexterity ) {
    return ( fDexterity * 0.0f );
}
inline Scalar CharStatPackage::_ComputeBaseStatModifier_MagicPower( Scalar fIntelligence ) {
    return ( fIntelligence * 0.0f );
}
inline Scalar CharStatPackage::_ComputeBaseStatModifier_MagicHit( Scalar fWisdom ) {
    return ( fWisdom * 0.0f );
}
inline Scalar CharStatPackage::_ComputeBaseStatModifier_MagicHaste( Scalar fIntelligence, Scalar fSpeed ) {
    return ( ( fIntelligence * 0.0f ) + ( fSpeed * 0.0f ) );
}
inline Scalar CharStatPackage::_ComputeBaseStatModifier_MagicCriticalPower( Scalar fIntelligence ) {
    return ( fIntelligence * 0.0f );
}
inline Scalar CharStatPackage::_ComputeBaseStatModifier_MagicCriticalHit( Scalar fWisdom ) {
    return ( fWisdom * 0.0f );
}

inline UInt CharStatPackage::_ComputeSecondaryStat_HP( Scalar fHealth ) {
    return (UInt)( MathFn->Floor( fHealth * 0.0f ) ) + 10000;
}
inline UInt CharStatPackage::_ComputeSecondaryStat_MP( Scalar fMana ) {
    return (UInt)( MathFn->Floor( fMana * 0.0f ) ) + 1000;
}
inline Scalar CharStatPackage::_ComputeSecondaryStat_MovementSpeed( Scalar fSpeed ) {
    return ( fSpeed * 0.0f ) + 10.0f;
}

