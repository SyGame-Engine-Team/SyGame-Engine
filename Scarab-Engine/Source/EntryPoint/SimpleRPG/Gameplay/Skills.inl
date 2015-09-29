/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Gameplay/Skills.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Gameplay : Skills
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
inline SkillID Skill::GetID() const {
    return m_idSkill;
}
inline const GChar * Skill::GetName() const {
    return m_strName;
}

inline UInt Skill::GetMaxRank() const {
    return m_iMaxRank;
}

inline Bool Skill::IsActive() const {
    return !m_bPassive;
}
inline Bool Skill::IsPassive() const {
    return m_bPassive;
}
inline Bool Skill::IsPhysical() const {
    return !m_bMagic;
}
inline Bool Skill::IsMagical() const {
    return m_bMagic;
}
inline Bool Skill::RequiresTarget() const {
    return m_bRequiresTarget;
}

inline UInt Skill::GetManaCost() const {
    return m_iManaCost;
}

inline Scalar Skill::GetRange() const {
    return m_fRange;
}
inline Scalar Skill::GetRangeAtTarget() const {
    return m_fRangeAtTarget;
}

inline Bool Skill::IsInstant() const {
    return ( m_fCastingTime == 0.0f );
}
inline Scalar Skill::GetCastingTime() const {
    return m_fCastingTime;
}
inline Scalar Skill::GetCooldownTime() const {
    return m_fCooldownTime;
}

inline SkillAnimation * Skill::GetAnimation() const {
    return m_pAnimation;
}

/////////////////////////////////////////////////////////////////////////////////
// SkillInstance implementation
inline Skill * SkillInstance::GetSkill() const {
    return m_pSkill;
}
inline BaseCharacter * SkillInstance::GetOwner() const {
    return m_pOwner;
}

inline SkillID SkillInstance::GetID() const {
    return m_pSkill->GetID();
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

inline Bool SkillInstance::IsActive() const {
    return m_pSkill->IsActive();
}
inline Bool SkillInstance::IsPassive() const {
    return m_pSkill->IsPassive();
}
inline Bool SkillInstance::IsPhysical() const {
    return m_pSkill->IsPhysical();
}
inline Bool SkillInstance::IsMagical() const {
    return m_pSkill->IsMagical();
}
inline Bool SkillInstance::RequiresTarget() const {
    return m_pSkill->RequiresTarget();
}

inline UInt SkillInstance::GetManaCost() const {
    return m_pSkill->GetManaCost();
}

inline Scalar SkillInstance::GetRange() const {
    return m_pSkill->GetRange();
}
inline Scalar SkillInstance::GetRangeAtTarget() const {
    return m_pSkill->GetRangeAtTarget();
}

inline Bool SkillInstance::IsInstant() const {
    return m_pSkill->IsInstant();
}
inline Scalar SkillInstance::GetCastingTime() const {
    return m_pSkill->GetCastingTime();
}
inline Scalar SkillInstance::GetCooldownTime() const {
    return m_pSkill->GetCooldownTime();
}

inline Bool SkillInstance::IsOnCooldown() const {
    return ( m_fCooldown > 0.0f );
}
inline Scalar SkillInstance::GetCooldown() const {
    return m_fCooldown;
}
inline Void SkillInstance::StartCooldown() {
    Assert( m_fCooldown == 0.0f );
    m_fCooldown = m_pSkill->GetCooldownTime();
}

inline SkillAnimation * SkillInstance::GetAnimation() const {
    return m_pSkill->GetAnimation();
}

/////////////////////////////////////////////////////////////////////////////////
// SkillBook implementation
inline Bool SkillBook::HasSkill( SkillID idSkill ) const {
    return m_mapSkills.Contains( idSkill );
}
inline SkillInstance * SkillBook::GetSkill( SkillID idSkill ) const {
    _SkillMap::Iterator itSkill = m_mapSkills.Get( idSkill );
    if ( itSkill.IsNull() )
        return NULL;
    return itSkill.GetItemPtr();
}

inline Void SkillBook::EnumSkills() const {
    Assert( m_itEnumerate.IsNull() );
    m_itEnumerate = m_mapSkills.Begin();
}
inline SkillInstance * SkillBook::EnumNextSkill() const {
    if ( m_itEnumerate.IsNull() )
        return NULL;
    SkillInstance * pSkillInstance = m_itEnumerate.GetItemPtr();
    ++m_itEnumerate;
    return pSkillInstance;
}


