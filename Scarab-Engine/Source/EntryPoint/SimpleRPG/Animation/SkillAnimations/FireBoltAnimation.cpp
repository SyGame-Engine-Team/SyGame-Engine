/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Animation/SkillAnimations/FireBoltAnimation.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Animation, SkillAnimations : FireBoltAnimation
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
#include "FireBoltAnimation.h"

#include "../../Gameplay/GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// FireBoltAnimation implementation
FireBoltAnimation::FireBoltAnimation():
    SkillAnimation()
{
    m_iType = ANIMTYPE_SKILL;
    m_idAnimation = ANIMID_SKILL_FIREBOLT;
}
FireBoltAnimation::~FireBoltAnimation()
{
    // nothing to do
}

Void FireBoltAnimation::OnStart( AnimationInstance * pInstance )
{
    // Animation state
    _AnimationState * pState = (_AnimationState*)( pInstance->GetAnimationState() );

    // Create FireBolt entity
    GChar strTmp[64];
    StringFn->Format( strTmp, TEXT("FireBolt_%s"), pInstance->GetSource()->GetName() );

    Vertex3 vPosition = pInstance->GetSource()->GetEntity()->GetPosition();
    Vertex3 vTarget = pInstance->GetTarget()->GetEntity()->GetPosition();
    pState->pFireBolt = EntityFn->CreateFireBolt( strTmp, vPosition, vTarget );
}
Void FireBoltAnimation::OnStop( AnimationInstance * pInstance )
{
    // Animation state
    _AnimationState * pState = (_AnimationState*)( pInstance->GetAnimationState() );

    // Destroy FireBolt entity
    EntityFn->DestroyEntity( pState->pFireBolt->GetName() );
    pState->pFireBolt = NULL;
}
Void FireBoltAnimation::OnUpdate( AnimationInstance * pInstance )
{
    // Animation state
    _AnimationState * pState = (_AnimationState*)( pInstance->GetAnimationState() );

    // Homing behaviour : update target position
    Vertex3 vTarget = pInstance->GetTarget()->GetEntity()->GetPosition();
    pState->pFireBolt->UpdateTarget( vTarget );
}

