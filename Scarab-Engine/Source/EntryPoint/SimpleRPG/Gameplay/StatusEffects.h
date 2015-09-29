/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Gameplay/StatusEffects.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Gameplay : StatusEffects
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
#ifndef SCARAB_ENTRYPOINT_SIMPLERPG_GAMEPLAY_STATUSEFFECTS_H
#define SCARAB_ENTRYPOINT_SIMPLERPG_GAMEPLAY_STATUSEFFECTS_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Animation/AnimationManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define STATUSEFFECT_MAX_INSTANCES 8

// StatusEffect types
enum StatusEffectType {
    STATUSEFFECT_UNDEFINED = 0,

    STATUSEFFECT_ENCHANT,
    STATUSEFFECT_BLESSING,
    STATUSEFFECT_SEAL,
    STATUSEFFECT_AURA,

    STATUSEFFECT_BLEED,
    STATUSEFFECT_POISON,
    STATUSEFFECT_DISEASE,
    STATUSEFFECT_CURSE,
    STATUSEFFECT_AFFLICT,

    STATUSEFFECT_COUNT
};

// StatusEffect list
enum StatusEffectID {
    STATUSEFFECTID_UNDEFINED = 0,

    STATUSEFFECTID_CHILL,

    STATUSEFFECTID_DOT,
    STATUSEFFECTID_HOT,

    STATUSEFFECTID_SLOW,
    STATUSEFFECTID_ROOT,
    STATUSEFFECTID_STUN,
    STATUSEFFECTID_FEAR,
    STATUSEFFECTID_JAIL,

    STATUSEFFECTID_COUNT
};

// Prototypes
class BaseCharacter;

class StatusEffectInstance;

/////////////////////////////////////////////////////////////////////////////////
// The StatusEffect class
class StatusEffect
{
public:
    StatusEffect();
    virtual ~StatusEffect();

    // Getters
    static StatusEffect * GetInstance( StatusEffectID idStatusEffect );

    inline StatusEffectType GetType() const;
    inline StatusEffectID GetID() const;
    inline const GChar * GetName() const;

    inline UInt GetMaxRank() const;

    inline Bool IsPositive() const;
    inline Bool IsNegative() const;
    inline Bool IsMovementLimiter() const;
    inline Bool IsActionLimiter() const;
    inline Bool IsStatModifier() const;

    inline Scalar GetDuration() const;
    inline Scalar GetTickPeriod() const;
    inline Scalar GetTickFrequency() const;

    inline UInt GetMaxStacks() const;

    inline StatusEffectAnimation * GetAnimation() const;

    // Methods
    Void GenerateInstance( StatusEffectInstance * outStatusEffectInstance, BaseCharacter * pOwner, BaseCharacter * pSource, UInt iRank, UInt iStacks );

    virtual Void OnApply( StatusEffectInstance * pInstance ) const = 0;
    virtual Void OnFade( StatusEffectInstance * pInstance ) const = 0;
    virtual Void OnTick( StatusEffectInstance * pInstance ) const = 0;

protected:
    StatusEffectType m_iType;
    StatusEffectID m_idStatusEffect;
    GChar m_strName[64];

    UInt m_iMaxRank;

    Bool m_bPositive; // else negative
    Bool m_bMovementLimiter, m_bActionLimiter;
    Bool m_bStatModifier;

    // Timing
    Scalar m_fDuration;
    Scalar m_fTickPeriod, m_fTickFrequency;

    // Stacking
    UInt m_iMaxStacks;

    // Animation
    StatusEffectAnimation * m_pAnimation;
};

/////////////////////////////////////////////////////////////////////////////////
// The StatusEffectInstance class
class StatusEffectInstance
{
public:
    StatusEffectInstance();
    virtual ~StatusEffectInstance();

    inline StatusEffectInstance & operator=( const StatusEffectInstance & rhs );

    // Getters
    inline StatusEffect * GetStatusEffect() const;
    inline BaseCharacter * GetOwner() const;
    inline BaseCharacter * GetSource() const;

    inline StatusEffectType GetType() const;
    inline StatusEffectID GetID() const;
    inline const GChar * GetName() const;

    inline UInt GetRank() const;
    inline UInt GetMaxRank() const;

    inline Bool IsPositive() const;
    inline Bool IsNegative() const;
    inline Bool IsMovementLimiter() const;
    inline Bool IsActionLimiter() const;
    inline Bool IsStatModifier() const;

    inline Scalar GetDuration() const;
    inline Scalar GetTickPeriod() const;
    inline Scalar GetTickFrequency() const;

    inline Bool HasFade() const;
    inline Scalar GetElapsedTime() const;
    inline Scalar GetRemainingTime() const;

    inline UInt GetStacks() const;
    inline UInt GetMaxStacks() const;

    inline StatusEffectAnimation * GetAnimation() const;

    // Methods
    inline Void AddStacks( UInt iStacks );
    inline Void Fade();
    inline Void Refresh();

    // Update routine
    Bool Update( Scalar fTimeStep );

protected:
    friend class StatusEffect;

    StatusEffect * m_pStatusEffect;
    BaseCharacter * m_pOwner;
    BaseCharacter * m_pSource;

    UInt m_iRank;

    // Timing
    Scalar m_fElapsedTime;
    Scalar m_fLastTickElapsedTime;

    // Stacking
    UInt m_iStacks;

    // Animation
    AnimationInstance m_hAnimationInstance;
};
/////////////////////////////////////////////////////////////////////////////////
// The StatusEffectBuffer class
class StatusEffectBuffer
{
public:
    StatusEffectBuffer( BaseCharacter * pOwner );
    virtual ~StatusEffectBuffer();

    // Getters
    inline UInt GetStatusEffectCount() const;

    inline Bool HasStatusEffect( StatusEffectID idStatusEffect ) const;

    inline UInt GetStatusEffectInstanceCount( StatusEffectID idStatusEffect ) const;
    inline StatusEffectInstance * GetStatusEffectInstance( StatusEffectID idStatusEffect, UInt iIndex ) const;

    inline Void EnumStatusEffects() const;
    inline StatusEffectID EnumNextStatusEffect() const;

    inline Void EnumStatusEffectInstances() const;
    inline StatusEffectInstance * EnumNextStatusEffectInstance() const;

    // Methods
    Bool AddEffect( StatusEffectID idStatusEffect, BaseCharacter * pSource, UInt iRank, UInt iStacks );
    Bool RemoveEffect( StatusEffectID idStatusEffect );

    Bool RemoveEffects( StatusEffectType iType );
    Bool RemovePositiveEffects();
    Bool RemoveNegativeEffects();
    Bool RemoveMovementLimiterEffects();
    Bool RemoveActionLimiterEffects();
    Bool RemoveAllEffects();

    // Update routine
    Bool Update( Scalar fTimeStep );

protected:
    BaseCharacter * m_pOwner;

    typedef struct _statuseffect_data {
        inline struct _statuseffect_data & operator=( const struct _statuseffect_data & rhs );

        UInt iInstanceCount;
        StatusEffectInstance arrInstances[STATUSEFFECT_MAX_INSTANCES];
    } _StatusEffectData;

    static Int _Compare_StatusEffectIDs( const StatusEffectID & rLeft, const StatusEffectID & rRight, Void * pUserData );

    typedef TreeMap<StatusEffectID,_StatusEffectData> _StatusEffectMap;
    _StatusEffectMap m_mapStatusEffects;

    mutable _StatusEffectMap::Iterator m_itEnumerate;
    mutable UInt m_iEnumerate;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "StatusEffects.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_SIMPLERPG_GAMEPLAY_STATUSEFFECTS_H

