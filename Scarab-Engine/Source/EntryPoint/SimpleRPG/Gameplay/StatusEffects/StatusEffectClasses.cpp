/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Gameplay/StatusEffects/StatusEffectClasses.cpp
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
// Includes
#include "StatusEffectClasses.h"

#include "../GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// DOTEffect implementation
DOTEffect::DOTEffect():
    StatusEffect()
{
    m_bStatModifier = false;

    m_iTickDamageMin = 0;
    m_iTickDamageMax = 0;
}
DOTEffect::~DOTEffect()
{
    // nothing to do
}

Void DOTEffect::OnApply( BaseCharacter * /*pSource*/, BaseCharacter * /*pTarget*/ ) const
{
    // nothing to do
}
Void DOTEffect::OnFade( BaseCharacter * /*pSource*/, BaseCharacter * /*pTarget*/ ) const
{
    // nothing to do
}
Void DOTEffect::OnTick( BaseCharacter * pSource, BaseCharacter * pTarget ) const
{
    UInt iDamage = GameplayFn->RandRange( m_iTickDamageMin, m_iTickDamageMax );
    pTarget->Damage( pSource, iDamage );
}

/////////////////////////////////////////////////////////////////////////////////
// HOTEffect implementation
HOTEffect::HOTEffect():
    StatusEffect()
{
    m_bStatModifier = false;

    m_iTickHealMin = 0;
    m_iTickHealMax = 0;
}
HOTEffect::~HOTEffect()
{
    // nothing to do
}

Void HOTEffect::OnApply( BaseCharacter * /*pSource*/, BaseCharacter * /*pTarget*/ ) const
{
    // nothing to do
}
Void HOTEffect::OnFade( BaseCharacter * /*pSource*/, BaseCharacter * /*pTarget*/ ) const
{
    // nothing to do
}
Void HOTEffect::OnTick( BaseCharacter * pSource, BaseCharacter * pTarget ) const
{
    UInt iHeal = GameplayFn->RandRange( m_iTickHealMin, m_iTickHealMax );
    pTarget->Heal( pSource, iHeal );
}

/////////////////////////////////////////////////////////////////////////////////
// StatModifierEffect implementation
StatModifierEffect::StatModifierEffect():
    StatusEffect()
{
    m_bStatModifier = true;
}
StatModifierEffect::~StatModifierEffect()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// SlowEffect implementation
SlowEffect::SlowEffect():
    StatModifierEffect()
{
    m_fSlowFactor = 1.0f;
}
SlowEffect::~SlowEffect()
{
    // nothing to do
}

Void SlowEffect::OnApply( BaseCharacter * /*pSource*/, BaseCharacter * /*pTarget*/ ) const
{
    //pTarget->ApplyMovementReduction( m_fSlowFactor );
}
Void SlowEffect::OnFade( BaseCharacter * /*pSource*/, BaseCharacter * /*pTarget*/ ) const
{
    //pTarget->FadeMovementReduction( m_fSlowFactor );
}
Void SlowEffect::OnTick( BaseCharacter * /*pSource*/, BaseCharacter * /*pTarget*/ ) const
{
    // nothing to do
}

