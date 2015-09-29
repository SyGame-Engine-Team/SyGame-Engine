/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Animation/SkillAnimations/CastAuraAnimation.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Animation, SkillAnimations : CastAuraAnimation
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
#include "CastAuraAnimation.h"

#include "../../Gameplay/GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// CastAuraAnimation implementation
CastAuraAnimation::CastAuraAnimation():
    Animation()
{
    m_iType = ANIMTYPE_SKILL;
    m_idAnimation = ANIMID_SKILL_CASTAURA;
}
CastAuraAnimation::~CastAuraAnimation()
{
    // nothing to do
}

Void CastAuraAnimation::OnStart( AnimationInstance * pInstance )
{
    // Animation state
    _AnimationState * pState = (_AnimationState*)( pInstance->GetAnimationState() );

    // Create CastAura entity
    GChar strTmp[64];
    StringFn->Format( strTmp, TEXT("CastAura_%s"), pInstance->GetSource()->GetName() );

    Vertex3 vPosition = pInstance->GetSource()->GetEntity()->GetPosition();
    Skill * pSkill = Skill::GetInstance( pInstance->GetSource()->GetCastingSkill() );

    pState->pCastAura = EntityFn->CreateCastAura( strTmp, pSkill->GetCastingTime(), vPosition );
}
Void CastAuraAnimation::OnStop( AnimationInstance * pInstance )
{
    // Animation state
    _AnimationState * pState = (_AnimationState*)( pInstance->GetAnimationState() );

    // Destroy CastAura entity
    EntityFn->DestroyEntity( pState->pCastAura->GetName() );
    pState->pCastAura = NULL;
}
Void CastAuraAnimation::OnUpdate( AnimationInstance * /*pInstance*/ )
{
    // nothing to do
}

