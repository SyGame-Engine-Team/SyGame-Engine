/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Animation/AnimationManager.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Animation : AnimationManager
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
#include "AnimationManager.h"

/////////////////////////////////////////////////////////////////////////////////
// AnimationManager implementation
AnimationManager::AnimationManager():
    Manager<AnimationManager>()
{
    // nothing to do
}
AnimationManager::~AnimationManager()
{
    // nothing to do
}

Void AnimationManager::Initialize()
{
    SelectMemory( TEXT("Scratch") );

    // Animation database
    m_arrAnimations[ANIMID_UNDEFINED] = NULL;

    m_arrAnimations[ANIMID_STATUSEFFECT_KINDLE] = NULL;

    m_arrAnimations[ANIMID_SKILL_CASTAURA] = New CastAuraAnimation();

    m_arrAnimations[ANIMID_SKILL_FIREBOLT] = New FireBoltAnimation();

    m_arrAnimations[ANIMID_SKILL_ICENOVA] = New IceNovaAnimation();

    UnSelectMemory();
}
Void AnimationManager::Cleanup()
{
    SelectMemory( TEXT("Scratch") );
    
    // Animation database
    for( UInt i = 0; i < ANIMID_COUNT; ++i ) {
        if ( m_arrAnimations[i] != NULL ) {
            Delete( m_arrAnimations[i] );
            m_arrAnimations[i] = NULL;
        }
    }

    UnSelectMemory();
}

