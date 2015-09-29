/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Animation/Animations.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Animation : Animations
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
#ifndef SCARAB_ENTRYPOINT_SIMPLERPG_ANIMATION_ANIMATIONS_H
#define SCARAB_ENTRYPOINT_SIMPLERPG_ANIMATION_ANIMATIONS_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Entities/EntityManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Animation types
enum AnimationType {
    ANIMTYPE_UNDEFINED = 0,

    ANIMTYPE_STATUSEFFECT,
    ANIMTYPE_SKILL,

    ANIMTYPE_COUNT
};

// Animation list
enum AnimationID {
    ANIMID_UNDEFINED = 0,

    // StatusEffects
    ANIMID_STATUSEFFECT_KINDLE,

    // Skills
    ANIMID_SKILL_CASTAURA,

    ANIMID_SKILL_FIREBOLT,

    ANIMID_SKILL_ICENOVA,

    ANIMID_COUNT
};

// SkillAnimation state
#define SKILLANIMATION_STATE_BYTES 256

// Prototypes
class BaseCharacter;

class AnimationInstance;

/////////////////////////////////////////////////////////////////////////////////
// The Animation class
class Animation
{
public:
    Animation();
    virtual ~Animation();

    // Getters
    static Animation * GetInstance( AnimationID idAnimation );

    inline AnimationType GetType() const;
    inline AnimationID GetID() const;

    // Methods
    Void GenerateInstance( AnimationInstance * outAnimationInstance, Scalar fDuration, BaseCharacter * pSource, BaseCharacter * pTarget );
    Void GenerateInstance( AnimationInstance * outAnimationInstance, Scalar fDuration, BaseCharacter * pSource, const Vertex3 & vTarget );

    virtual Void OnStart( AnimationInstance * pInstance ) = 0;
    virtual Void OnStop( AnimationInstance * pInstance ) = 0;
    virtual Void OnUpdate( AnimationInstance * pInstance ) = 0;

protected:
    AnimationType m_iType;
    AnimationID m_idAnimation;
};

/////////////////////////////////////////////////////////////////////////////////
// The AnimationInstance class
class AnimationInstance
{
public:
    AnimationInstance();
    virtual ~AnimationInstance();

    AnimationInstance & operator=( const AnimationInstance & rhs );

    // Getters
    inline Animation * GetAnimation() const;
    inline BaseCharacter * GetSource() const;
    inline BaseCharacter * GetTarget() const;
    inline const Vertex3 & GetTargetFixed() const;

    inline AnimationType GetType() const;
    inline AnimationID GetID() const;

    inline Scalar GetDuration() const;
    inline Scalar GetInvDuration() const;

    inline Bool IsPlaying() const;
    inline Scalar GetElapsedTime() const;

    inline Byte * GetAnimationState();

    // Update routine
    Void Interrupt();
    Void Restart();
    Bool Update( Scalar fTimeStep );

protected:
    friend class Animation;

    Animation * m_pAnimation;
    BaseCharacter * m_pSource;
    BaseCharacter * m_pTarget;
    Vertex3 m_vTarget;  // Fixed-target skills

    Scalar m_fDuration;
    Scalar m_fInvDuration;

    Bool m_bPlaying;
    Scalar m_fElapsedTime;

    Byte m_arrAnimationState[SKILLANIMATION_STATE_BYTES];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Animations.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_SIMPLERPG_ANIMATION_ANIMATIONS_H

