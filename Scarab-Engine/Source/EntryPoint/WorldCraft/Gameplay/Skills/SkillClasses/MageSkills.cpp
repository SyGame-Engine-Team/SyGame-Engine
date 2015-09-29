/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Gameplay/Skills/MageSkills.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Gameplay, Skills : MageSkills
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
#include "MageSkills.h"

#include "../GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// MageSkillFireBolt implementation
MageSkillFireBolt::MageSkillFireBolt():
    TargetSkill()
{
    m_idSkill = SKILLID_MAGE_FIREBOLT;
    StringFn->Copy( m_strName, TEXT("FireBolt") );

    m_iMaxRank = 1;

    m_bMagic = true;
    m_bRequiresTarget = true;

    m_iManaCost = 5;
    m_fRange = 30.0f;
    m_fRangeAtTarget = 0.0f;
    m_fCastingTime = 2.0f;
    m_fCooldownTime = 1.5f;

    m_pAnimation = (SkillAnimation*)( Animation::GetInstance(ANIMID_SKILL_FIREBOLT) );

    m_iDamageMin = 100;
    m_iDamageMax = 1000;
}
MageSkillFireBolt::~MageSkillFireBolt()
{
    // nothing to do
}

Void MageSkillFireBolt::OnSkillEffect( SkillInstance * pInstance, BaseCharacter * pTarget ) const
{
    BaseCharacter * pCaster = pInstance->GetOwner();
    pCaster->SkillAttack( pInstance, pTarget );
}

/////////////////////////////////////////////////////////////////////////////////
// MageSkillIceNova implementation
MageSkillIceNova::MageSkillIceNova():
    AOESelfSkill()
{
    m_idSkill = SKILLID_MAGE_ICENOVA;
    StringFn->Copy( m_strName, TEXT("IceNova") );

    m_iMaxRank = 1;

    m_bMagic = true;
    m_bRequiresTarget = false;

    m_iManaCost = 5;
    m_fRange = 15.0f;
    m_fRangeAtTarget = 0.0f;
    m_fCastingTime = 0.0f;
    m_fCooldownTime = 5.0f;

    m_pAnimation = (SkillAnimation*)( Animation::GetInstance(ANIMID_SKILL_ICENOVA) );

    m_iDamageMin = 100;
    m_iDamageMax = 1000;

    //m_idOnHitEffect = STATUSEFFECTID_FREEZE;
    //m_fOnHitEffectScore = 1.0f;
    //m_iOnHitEffectRank = 1;
    //m_iOnHitEffectStacks = 1;
}
MageSkillIceNova::~MageSkillIceNova()
{
    // nothing to do
}

Void MageSkillIceNova::OnSkillEffect( SkillInstance * pInstance, BaseCharacter * /*pTarget*/ ) const
{
    BaseCharacter * pCaster = pInstance->GetOwner();

    const Array<SimpleEntity*> & arrCharacters = pCaster->GetEntity()->GetEntitiesInRange( ENTITYTYPE_CHARACTER, m_fRange, false );
    for( UInt i = 0; i < arrCharacters.Count(); ++i )
        pCaster->SkillAttack( pInstance, ((CharEntity*)arrCharacters[i])->GetCharacter() );
}

