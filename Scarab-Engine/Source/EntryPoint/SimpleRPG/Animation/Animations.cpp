/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Animation/SkillAnimations.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Animation : SkillAnimations
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
#include "Animations.h"

#include "../Gameplay/GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Animation implementation
Animation::Animation()
{
    m_iType = ANIMTYPE_UNDEFINED;
    m_idAnimation = ANIMID_UNDEFINED;
}
Animation::~Animation()
{
    // nothing to do
}

Animation * Animation::GetInstance( AnimationID idAnimation )
{
    return AnimationFn->GetAnimation( idAnimation );
}

Void Animation::GenerateInstance( AnimationInstance * outAnimationInstance, Scalar fDuration, BaseCharacter * pSource, BaseCharacter * pTarget )
{
    outAnimationInstance->m_pAnimation = this;
    outAnimationInstance->m_pSource = pSource;
    outAnimationInstance->m_pTarget = pTarget;
    outAnimationInstance->m_vTarget = Vertex3::Null;

    outAnimationInstance->m_fDuration = fDuration;
    outAnimationInstance->m_fInvDuration = MathFn->Invert( fDuration );

    outAnimationInstance->m_fElapsedTime = 0.0f;
    outAnimationInstance->m_bPlaying = true;

    MemZero( outAnimationInstance->m_arrAnimationState, SKILLANIMATION_STATE_BYTES );

    OnStart( outAnimationInstance );
}
Void Animation::GenerateInstance( AnimationInstance * outAnimationInstance, Scalar fDuration, BaseCharacter * pSource, const Vertex3 & vTarget )
{
    outAnimationInstance->m_pAnimation = this;
    outAnimationInstance->m_pSource = pSource;
    outAnimationInstance->m_pTarget = NULL;
    outAnimationInstance->m_vTarget = vTarget;

    outAnimationInstance->m_fDuration = fDuration;
    outAnimationInstance->m_fInvDuration = MathFn->Invert( fDuration );

    outAnimationInstance->m_bPlaying = true;
    outAnimationInstance->m_fElapsedTime = 0.0f;

    MemZero( outAnimationInstance->m_arrAnimationState, SKILLANIMATION_STATE_BYTES );

    OnStart( outAnimationInstance );
}

/////////////////////////////////////////////////////////////////////////////////
// AnimationInstance implementation
AnimationInstance::AnimationInstance()
{
    m_pAnimation = NULL;
    m_pSource = NULL;
    m_pTarget = NULL;
    m_vTarget = Vertex3::Null;

    m_fDuration = 0.0f;
    m_fInvDuration = 0.0f;

    m_bPlaying = false;
    m_fElapsedTime = 0.0f;

    MemZero( m_arrAnimationState, SKILLANIMATION_STATE_BYTES );
}
AnimationInstance::~AnimationInstance()
{
    // nothing to do
}

AnimationInstance & AnimationInstance::operator=( const AnimationInstance & rhs )
{
    m_pAnimation = rhs.m_pAnimation;
    m_pSource = rhs.m_pSource;
    m_pTarget = rhs.m_pTarget;
    m_vTarget = rhs.m_vTarget;

    m_fDuration = rhs.m_fDuration;
    m_fInvDuration = rhs.m_fInvDuration;

    m_bPlaying = rhs.m_bPlaying;
    m_fElapsedTime = rhs.m_fElapsedTime;

    MemCopy( m_arrAnimationState, rhs.m_arrAnimationState, SKILLANIMATION_STATE_BYTES );

    return (*this);
}

Void AnimationInstance::Interrupt()
{
    if ( !m_bPlaying )
        return;

    m_bPlaying = false;
    m_fElapsedTime = 0.0f; // Interrupt marker

    m_pAnimation->OnStop( this );
}
Void AnimationInstance::Restart()
{
    if ( !m_bPlaying )
        return;

    Interrupt();

    m_bPlaying = true;
    m_fElapsedTime = 0.0f;

    MemZero( m_arrAnimationState, SKILLANIMATION_STATE_BYTES );

    m_pAnimation->OnStart( this );
}
Bool AnimationInstance::Update( Scalar fTimeStep )
{
    // Zombie animations (should not happen)
    if ( !m_bPlaying )
        return false;

    m_fElapsedTime += fTimeStep;

    // Alive animation
    if ( m_fElapsedTime < m_fDuration ) {
        m_pAnimation->OnUpdate( this );

        return true;
    }

    // Dying animation
    m_bPlaying = false;
    m_fElapsedTime = m_fDuration;

    m_pAnimation->OnStop( this );

    return false; // true => defer anim deletion to next frame (cascade effects ?)
}


