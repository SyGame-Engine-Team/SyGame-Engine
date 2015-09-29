/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Animation/SkillAnimations/IceNovaAnimation.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Animation, SkillAnimations : IceNovaAnimation
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
#include "IceNovaAnimation.h"

#include "../../Gameplay/GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// IceNovaAnimation implementation
IceNovaAnimation::IceNovaAnimation():
    SkillAnimation()
{
    m_iType = ANIMTYPE_SKILL;
    m_idAnimation = ANIMID_SKILL_ICENOVA;
}
IceNovaAnimation::~IceNovaAnimation()
{
    // nothing to do
}

Void IceNovaAnimation::OnStart( AnimationInstance * pInstance )
{
    // Animation state
    _AnimationState * pState = (_AnimationState*)( pInstance->GetAnimationState() );

    // Create IceNova entity
    GChar strTmp[64];
    StringFn->Format( strTmp, TEXT("IceNova_%s"), pInstance->GetSource()->GetName() );

    Vertex3 vPosition = pInstance->GetSource()->GetEntity()->GetPosition();
    pState->pIceNova = EntityFn->CreateIceNova( strTmp, vPosition );
}
Void IceNovaAnimation::OnStop( AnimationInstance * pInstance )
{
    // Animation state
    _AnimationState * pState = (_AnimationState*)( pInstance->GetAnimationState() );

    // Destroy FireBolt entity
    EntityFn->DestroyEntity( pState->pIceNova->GetName() );
    pState->pIceNova = NULL;
}
Void IceNovaAnimation::OnUpdate( AnimationInstance * /*pInstance*/ )
{
    // nothing to do
}

