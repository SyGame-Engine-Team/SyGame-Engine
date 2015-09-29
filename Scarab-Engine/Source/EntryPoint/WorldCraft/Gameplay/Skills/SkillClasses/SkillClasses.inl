/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Gameplay/Skills/SkillClasses.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Gameplay, Skills : SkillClasses
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// PassiveSkill implementation
inline const Scalar * PassiveSkill::GetStatBonus() const {
    return m_arrStatBonus;
}

/////////////////////////////////////////////////////////////////////////////////
// ActiveSkill implementation
inline UInt ActiveSkill::GetDamageMin() const {
    return m_iDamageMin;
}
inline UInt ActiveSkill::GetDamageMax() const {
    return m_iDamageMax;
}

inline StatusEffectID ActiveSkill::GetOnHitEffect() const {
    return m_idOnHitEffect;
}
inline Scalar ActiveSkill::GetOnHitEffectScore() const {
    return m_fOnHitEffectScore;
}
inline UInt ActiveSkill::GetOnHitEffectRank() const {
    return m_iOnHitEffectRank;
}
inline UInt ActiveSkill::GetOnHitEffectStacks() const {
    return m_iOnHitEffectStacks;
}

inline StatusEffectID ActiveSkill::GetOnCriticalEffect() const {
    return m_idOnCriticalEffect;
}
inline Scalar ActiveSkill::GetOnCriticalEffectScore() const {
    return m_fOnCriticalEffectScore;
}
inline UInt ActiveSkill::GetOnCriticalEffectRank() const {
    return m_iOnCriticalEffectRank;
}
inline UInt ActiveSkill::GetOnCriticalEffectStacks() const {
    return m_iOnCriticalEffectStacks;
}

/////////////////////////////////////////////////////////////////////////////////
// DistanceSkill implementation

/////////////////////////////////////////////////////////////////////////////////
// TargetSkill implementation

/////////////////////////////////////////////////////////////////////////////////
// AOESelfSkill implementation

