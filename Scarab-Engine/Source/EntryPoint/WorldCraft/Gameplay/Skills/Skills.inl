/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/Gameplay/Skills/Skills.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : WorldCraft, Gameplay : Skills
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Skill implementation
inline DWord Skill::GetTypeBaseID( SkillType iType ) {
    Assert( iType < SKILLTYPE_COUNT );
    return sm_arrTypeIDDomains[iType].dwBaseID;
}
inline DWord Skill::GetTypeDomainCapacity( SkillType iType ) {
    Assert( iType < SKILLTYPE_COUNT );
    return sm_arrTypeIDDomains[iType].dwCapacity;
}
inline DWord Skill::GetClassBaseID( SkillClass iClass ) {
    Assert( iClass < SKILLCLASS_COUNT );
    return sm_arrClassIDDomains[iClass].dwBaseID;
}
inline DWord Skill::GetClassDomainCapacity( SkillClass iClass ) {
    Assert( iClass < SKILLCLASS_COUNT );
    return sm_arrClassIDDomains[iClass].dwCapacity;
}

inline SkillID Skill::MakeID( SkillClass iClass, DWord dwLocalID ) {
    Assert( iClass < SKILLCLASS_COUNT );
    DWord dwBaseID = sm_arrClassIDDomains[iClass].dwBaseID;
    Assert( dwLocalID < sm_arrClassIDDomains[iClass].dwCapacity );
    return (SkillID)( dwBaseID | dwLocalID );
}

inline SkillType Skill::GetType() const {
    return m_iType;
}
inline SkillClass Skill::GetClass() const {
    return m_iClass;
}
inline SkillID Skill::GetID() const {
    return m_iSkillID;
}

inline Bool Skill::IsActive() const {
    return ( m_iClass != SKILLCLASS_PASSIVE );
}
inline Bool Skill::IsPassive() const {
    return ( m_iClass == SKILLCLASS_PASSIVE );
}
inline Bool Skill::RequiresTarget() const {
    return ( m_iType != SKILLTYPE_TARGET_SELF && m_iType != SKILLTYPE_AREA_SELF );
}

inline const GChar * Skill::GetName() const {
    return m_strName;
}
inline UInt Skill::GetMaxRank() const {
    return m_iMaxRank;
}

inline UInt Skill::GetManaCost( UInt iRank ) const {
    Assert( iRank < m_iMaxRank );
    return m_iManaCost[iRank];
}

inline Scalar Skill::GetRange( UInt iRank ) const {
    Assert( iRank < m_iMaxRank );
    return m_fRange[iRank];
}
inline Scalar Skill::GetRangeAtTarget( UInt iRank ) const {
    Assert( iRank < m_iMaxRank );
    return m_fRangeAtTarget[iRank];
}

inline Bool Skill::TriggersGCD( UInt iRank ) const {
    Assert( iRank < m_iMaxRank );
    return m_bTriggersGCD[iRank];
}
inline Bool Skill::IsInstant( UInt iRank ) const {
    Assert( iRank < m_iMaxRank );
    return ( m_fCastingTime[iRank] == 0.0f );
}
inline Scalar Skill::GetCastingTime( UInt iRank ) const {
    Assert( iRank < m_iMaxRank );
    return m_fCastingTime[iRank];
}
inline Scalar Skill::GetCooldownTime( UInt iRank ) const {
    Assert( iRank < m_iMaxRank );
    return m_fCooldownTime[iRank];
}

/////////////////////////////////////////////////////////////////////////////////
// SkillInstance implementation
inline SkillInstance & SkillInstance::operator=( const SkillInstance & rhs ) {
    m_pSkill = rhs.m_pSkill;
    m_pOwner = rhs.m_pOwner;
    m_iRank = rhs.m_iRank;
    m_bCastInProgress = rhs.m_bCastInProgress;
    m_fCooldown = rhs.m_fCooldown;
    return (*this);
}

inline const Skill * SkillInstance::GetSkill() const {
    return m_pSkill;
}

inline SkillType SkillInstance::GetType() const {
    return m_pSkill->GetType();
}
inline SkillClass SkillInstance::GetClass() const {
    return m_pSkill->GetClass();
}
inline SkillID SkillInstance::GetID() const {
    return m_pSkill->GetID();
}

inline Bool SkillInstance::IsActive() const {
    return m_pSkill->IsActive();
}
inline Bool SkillInstance::IsPassive() const {
    return m_pSkill->IsPassive();
}
inline Bool SkillInstance::RequiresTarget() const {
    return m_pSkill->RequiresTarget();
}

inline const GChar * SkillInstance::GetName() const {
    return m_pSkill->GetName();
}
inline UInt SkillInstance::GetRank() const {
    return m_iRank;
}
inline UInt SkillInstance::GetMaxRank() const {
    return m_pSkill->GetMaxRank();
}

inline BaseCharacter * SkillInstance::GetOwner() const {
    return m_pOwner;
}

inline UInt SkillInstance::GetManaCost( UInt iRank ) const {
    return m_pSkill->GetManaCost( iRank );
}

inline Scalar SkillInstance::GetRange( UInt iRank ) const {
    return m_pSkill->GetRange( iRank );
}
inline Scalar SkillInstance::GetRangeAtTarget( UInt iRank ) const {
    return m_pSkill->GetRangeAtTarget( iRank );
}

inline Bool SkillInstance::IsInstant( UInt iRank ) const {
    return m_pSkill->IsInstant( iRank );
}
inline Scalar SkillInstance::GetCastingTime( UInt iRank ) const {
    return m_pSkill->GetCastingTime( iRank );
}
inline Scalar SkillInstance::GetCooldownTime( UInt iRank ) const {
    return m_pSkill->GetCooldownTime( iRank );
}

inline Bool SkillInstance::IsCastInProgress() const {
    return m_bCastInProgress;
}
inline Bool SkillInstance::IsOnCooldown() const {
    return ( m_fCooldown > 0.0f );
}
inline Scalar SkillInstance::GetCooldown() const {
    return m_fCooldown;
}
inline Void SkillInstance::StartCooldown() {
    Assert( m_fCooldown == 0.0f );
    m_fCooldown = m_pSkill->GetCooldownTime( m_iRank );
}
inline Void SkillInstance::ResetCooldown() {
    m_fCooldown = 0.0f;
}
inline Void SkillInstance::DecreaseCooldown( Scalar fAmount ) {
    Assert( fAmount >= 0.0f );
    m_fCooldown -= fAmount;
    if ( m_fCooldown < 0.0f )
        m_fCooldown = 0.0f;
}



