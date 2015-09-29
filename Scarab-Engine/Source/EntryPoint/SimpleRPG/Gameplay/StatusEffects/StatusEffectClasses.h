/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Gameplay/StatusEffects/StatusEffectClasses.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Gameplay, StatusEffects : StatusEffectClasses
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
#ifndef SCARAB_ENTRYPOINT_SIMPLERPG_GAMEPLAY_STATUSEFFECTS_STATUSEFFECTCLASSES_H
#define SCARAB_ENTRYPOINT_SIMPLERPG_GAMEPLAY_STATUSEFFECTS_STATUSEFFECTCLASSES_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Statistics.h"
#include "../StatusEffects.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The DOTEffect class
class DOTEffect : public StatusEffect
{
public:
    DOTEffect();
    virtual ~DOTEffect();

    // Methods
    virtual Void OnApply( BaseCharacter * pSource, BaseCharacter * pTarget ) const;
    virtual Void OnFade( BaseCharacter * pSource, BaseCharacter * pTarget ) const;
    virtual Void OnTick( BaseCharacter * pSource, BaseCharacter * pTarget ) const;

protected:
    // Parameters
    UInt m_iTickDamageMin, m_iTickDamageMax;
};

/////////////////////////////////////////////////////////////////////////////////
// The HOTEffect class
class HOTEffect : public StatusEffect
{
public:
    HOTEffect();
    virtual ~HOTEffect();

    // Methods
    virtual Void OnApply( BaseCharacter * pSource, BaseCharacter * pTarget ) const;
    virtual Void OnFade( BaseCharacter * pSource, BaseCharacter * pTarget ) const;
    virtual Void OnTick( BaseCharacter * pSource, BaseCharacter * pTarget ) const;

protected:
    // Parameters
    UInt m_iTickHealMin, m_iTickHealMax;
};

/////////////////////////////////////////////////////////////////////////////////
// The StatModifierEffect class
class StatModifierEffect : public StatusEffect
{
public:
    StatModifierEffect();
    virtual ~StatModifierEffect();

    // Getters
    inline const Scalar * GetStatModifier() const;

protected:
    Scalar m_arrStatModifier[CHARSTAT_COUNT];
};

/////////////////////////////////////////////////////////////////////////////////
// The SlowEffect class
class SlowEffect : public StatModifierEffect
{
public:
    SlowEffect();
    virtual ~SlowEffect();

    // Methods
    virtual Void OnApply( BaseCharacter * pSource, BaseCharacter * pTarget ) const;
    virtual Void OnFade( BaseCharacter * pSource, BaseCharacter * pTarget ) const;
    virtual Void OnTick( BaseCharacter * pSource, BaseCharacter * pTarget ) const;

protected:
    // Parameters
    Scalar m_fSlowFactor; // [0,1]
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "StatusEffectClasses.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_SIMPLERPG_GAMEPLAY_STATUSEFFECTS_STATUSEFFECTCLASSES_H

