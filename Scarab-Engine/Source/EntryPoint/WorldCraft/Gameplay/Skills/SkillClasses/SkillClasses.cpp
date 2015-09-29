/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Gameplay/Skills/SkillClasses.cpp
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
// Includes
#include "SkillClasses.h"

#include "../GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// PassiveSkill implementation
PassiveSkill::PassiveSkill():
    Skill()
{
    m_bPassive = true;

    m_iManaCost = 0;

    m_fCastingTime = 0.0f;
    m_fCooldownTime = 0.0f;

    m_pAnimation = NULL;
}
PassiveSkill::~PassiveSkill()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// ActiveSkill implementation
ActiveSkill::ActiveSkill():
    Skill()
{
    m_bPassive = false;

    m_iDamageMin = 0;
    m_iDamageMax = 0;

    m_idOnHitEffect = STATUSEFFECTID_UNDEFINED;
    m_fOnHitEffectScore = 0.0f;
    m_iOnHitEffectRank = 0;
    m_iOnHitEffectStacks = 0;

    m_idOnCriticalEffect = STATUSEFFECTID_UNDEFINED;
    m_fOnCriticalEffectScore = 0.0f;
    m_iOnCriticalEffectRank = 0;
    m_iOnCriticalEffectStacks = 0;
}
ActiveSkill::~ActiveSkill()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// DistanceSkill implementation
DistanceSkill::DistanceSkill():
    ActiveSkill()
{
    // nothing to do
}
DistanceSkill::~DistanceSkill()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// TargetSkill implementation
TargetSkill::TargetSkill():
    DistanceSkill()
{
    // nothing to do
}
TargetSkill::~TargetSkill()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// AOESelfSkill implementation
AOESelfSkill::AOESelfSkill():
    DistanceSkill()
{
    // nothing to do
}
AOESelfSkill::~AOESelfSkill()
{
    // nothing to do
}

