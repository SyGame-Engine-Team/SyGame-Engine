/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Animation/AnimationManager.h
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
// Header prelude
#ifndef SCARAB_ENTRYPOINT_SIMPLERPG_ANIMATION_ANIMATIONMANAGER_H
#define SCARAB_ENTRYPOINT_SIMPLERPG_ANIMATION_ANIMATIONMANAGER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Graphics/GraphicsManager.h"
#include "../Entities/EntityManager.h"

#include "Animations.h"

#include "SkillAnimations/SkillAnimationClasses.h"
#include "SkillAnimations/CastAuraAnimation.h"
#include "SkillAnimations/FireBoltAnimation.h"
#include "SkillAnimations/IceNovaAnimation.h"

#include "StatusEffectAnimations/StatusEffectAnimationClasses.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define AnimationFn AnimationManager::GetInstancePtr()

/////////////////////////////////////////////////////////////////////////////////
// The AnimationManager class
class AnimationManager : public Manager<AnimationManager>
{
public:
    inline static const GChar * GetName();
    inline static const GChar * GetMemoryBudget();

protected:
    friend class Manager<AnimationManager>;
    AnimationManager();
    virtual ~AnimationManager();

public:
    Void Initialize();
    Void Cleanup();

    // Animation database
    inline Animation * GetAnimation( AnimationID idAnimation ) const;

private:
    // Animation database
    Animation * m_arrAnimations[ANIMID_COUNT];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "AnimationManager.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_SIMPLERPG_ANIMATION_ANIMATIONMANAGER_H


