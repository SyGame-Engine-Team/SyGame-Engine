/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Gameplay/Statistics.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Gameplay : Statistics
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
#include "Statistics.h"

#include "GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// CharStatPackage implementation
CharStatPackage::CharStatPackage( BaseCharacter * pOwner )
{
    m_pOwner = pOwner;

    // Initialize base stat functions
    m_arrComputeBaseStat[CHARSTAT_STRENGTH] = _ComputeBaseStat_Strength;
    m_arrComputeBaseStat[CHARSTAT_DEXTERITY] = _ComputeBaseStat_Dexterity;
    m_arrComputeBaseStat[CHARSTAT_INTELLIGENCE] = _ComputeBaseStat_Intelligence;
    m_arrComputeBaseStat[CHARSTAT_WISDOM] = _ComputeBaseStat_Wisdom;
    m_arrComputeBaseStat[CHARSTAT_ENDURANCE] = _ComputeBaseStat_Endurance;
    m_arrComputeBaseStat[CHARSTAT_SPEED] = _ComputeBaseStat_Speed;
    m_arrComputeBaseStat[CHARSTAT_ARMOR] = _ComputeBaseStat_Armor;
    m_arrComputeBaseStat[CHARSTAT_HEALTH] = _ComputeBaseStat_Health;
    m_arrComputeBaseStat[CHARSTAT_MANA] = _ComputeBaseStat_Mana;
    m_arrComputeBaseStat[CHARSTAT_REGEN_HEALTH] = _ComputeBaseStat_RegenHealth;
    m_arrComputeBaseStat[CHARSTAT_REGEN_MANA] = _ComputeBaseStat_RegenMana;
    m_arrComputeBaseStat[CHARSTAT_DODGE] = _ComputeBaseStat_Dodge;
    m_arrComputeBaseStat[CHARSTAT_BLOCK] = _ComputeBaseStat_Block;
    m_arrComputeBaseStat[CHARSTAT_DEFENSE] = _ComputeBaseStat_Defense;
    m_arrComputeBaseStat[CHARSTAT_RESISTANCE] = _ComputeBaseStat_Resistance;
    m_arrComputeBaseStat[CHARSTAT_POWER] = _ComputeBaseStat_Power;
    m_arrComputeBaseStat[CHARSTAT_HIT] = _ComputeBaseStat_Hit;
    m_arrComputeBaseStat[CHARSTAT_HASTE] = _ComputeBaseStat_Haste;
    m_arrComputeBaseStat[CHARSTAT_CRITICALPOWER] = _ComputeBaseStat_CriticalPower;
    m_arrComputeBaseStat[CHARSTAT_CRITICALHIT] = _ComputeBaseStat_CriticalHit;
    m_arrComputeBaseStat[CHARSTAT_POWER_MAGIC] = _ComputeBaseStat_MagicPower;
    m_arrComputeBaseStat[CHARSTAT_HIT_MAGIC] = _ComputeBaseStat_MagicHit;
    m_arrComputeBaseStat[CHARSTAT_HASTE_MAGIC] = _ComputeBaseStat_MagicHaste;
    m_arrComputeBaseStat[CHARSTAT_CRITICALPOWER_MAGIC] = _ComputeBaseStat_MagicCriticalPower;
    m_arrComputeBaseStat[CHARSTAT_CRITICALHIT_MAGIC] = _ComputeBaseStat_MagicCriticalHit;
}
CharStatPackage::~CharStatPackage()
{
    // nothing to do
}

const GChar * CharStatPackage::GetStatName( CharStatistic iStat )
{
    switch(iStat) {
        case CHARSTAT_STRENGTH:            return TEXT("Strength");
        case CHARSTAT_DEXTERITY:           return TEXT("Dexterity");
        case CHARSTAT_INTELLIGENCE:        return TEXT("Intelligence");
        case CHARSTAT_WISDOM:              return TEXT("Wisdom");
        case CHARSTAT_ENDURANCE:           return TEXT("Endurance");
        case CHARSTAT_SPEED:               return TEXT("Speed");
        case CHARSTAT_ARMOR:               return TEXT("Armor");
        case CHARSTAT_HEALTH:              return TEXT("Health");
        case CHARSTAT_MANA:                return TEXT("Mana");
        case CHARSTAT_REGEN_HEALTH:        return TEXT("HealthRegen");
        case CHARSTAT_REGEN_MANA:          return TEXT("ManaRegen");
        case CHARSTAT_DODGE:               return TEXT("Dodge");
        case CHARSTAT_BLOCK:               return TEXT("Block");
        case CHARSTAT_DEFENSE:             return TEXT("Defense");
        case CHARSTAT_RESISTANCE:          return TEXT("Resistance");
        case CHARSTAT_POWER:               return TEXT("PhysicPower");
        case CHARSTAT_HIT:                 return TEXT("PhysicHit");
        case CHARSTAT_HASTE:               return TEXT("PhysicHaste");
        case CHARSTAT_CRITICALPOWER:       return TEXT("PhysicCriticalPower");
        case CHARSTAT_CRITICALHIT:         return TEXT("PhysicCriticalHit");
        case CHARSTAT_POWER_MAGIC:         return TEXT("MagicPower");
        case CHARSTAT_HIT_MAGIC:           return TEXT("MagicHit");
        case CHARSTAT_HASTE_MAGIC:         return TEXT("MagicHaste");
        case CHARSTAT_CRITICALPOWER_MAGIC: return TEXT("MagicCriticalPower");
        case CHARSTAT_CRITICALHIT_MAGIC:   return TEXT("MagicCriticalHit");
        default: Assert(false); return NULL;
    }
}
const GChar * CharStatPackage::GetClassName( CharClass iClass )
{
    switch( iClass ) {
        case CHARCLASS_WARRIOR: return TEXT("Warrior");
        case CHARCLASS_THIEF:   return TEXT("Thief");
        case CHARCLASS_HUNTER:  return TEXT("Hunter");
        case CHARCLASS_MAGE:    return TEXT("Mage");
        case CHARCLASS_WARLOCK: return TEXT("Warlock");
        case CHARCLASS_PRIEST:  return TEXT("Priest");
        case CHARCLASS_PALADIN: return TEXT("Paladin");
        case CHARCLASS_DRUID:   return TEXT("Druid");
        default: return NULL;
    }
}

/////////////////////////////////////////////////////////////////////////////////

Scalar CharStatPackage::_ComputeBaseStat_Strength( CharClass iClass, UInt iLevel )
{
    switch( iClass ) {
        case CHARCLASS_WARRIOR: return ( iLevel * 0.0f );
        case CHARCLASS_THIEF:   return ( iLevel * 0.0f );
        case CHARCLASS_HUNTER:  return ( iLevel * 0.0f );
        case CHARCLASS_MAGE:    return ( iLevel * 1.0f );
        case CHARCLASS_WARLOCK: return ( iLevel * 0.0f );
        case CHARCLASS_PRIEST:  return ( iLevel * 0.0f );
        case CHARCLASS_PALADIN: return ( iLevel * 0.0f );
        case CHARCLASS_DRUID:   return ( iLevel * 0.0f );
        default: Assert(false); return 0.0f;
    }
}
Scalar CharStatPackage::_ComputeBaseStat_Dexterity( CharClass iClass, UInt iLevel )
{
    switch( iClass ) {
        case CHARCLASS_WARRIOR: return ( iLevel * 0.0f );
        case CHARCLASS_THIEF:   return ( iLevel * 0.0f );
        case CHARCLASS_HUNTER:  return ( iLevel * 0.0f );
        case CHARCLASS_MAGE:    return ( iLevel * 1.0f );
        case CHARCLASS_WARLOCK: return ( iLevel * 0.0f );
        case CHARCLASS_PRIEST:  return ( iLevel * 0.0f );
        case CHARCLASS_PALADIN: return ( iLevel * 0.0f );
        case CHARCLASS_DRUID:   return ( iLevel * 0.0f );
        default: Assert(false); return 0.0f;
    }
}
Scalar CharStatPackage::_ComputeBaseStat_Intelligence( CharClass iClass, UInt iLevel )
{
    switch( iClass ) {
        case CHARCLASS_WARRIOR: return ( iLevel * 0.0f );
        case CHARCLASS_THIEF:   return ( iLevel * 0.0f );
        case CHARCLASS_HUNTER:  return ( iLevel * 0.0f );
        case CHARCLASS_MAGE:    return ( iLevel * 100.0f );
        case CHARCLASS_WARLOCK: return ( iLevel * 0.0f );
        case CHARCLASS_PRIEST:  return ( iLevel * 0.0f );
        case CHARCLASS_PALADIN: return ( iLevel * 0.0f );
        case CHARCLASS_DRUID:   return ( iLevel * 0.0f );
        default: Assert(false); return 0.0f;
    }
}
Scalar CharStatPackage::_ComputeBaseStat_Wisdom( CharClass iClass, UInt iLevel )
{
    switch( iClass ) {
        case CHARCLASS_WARRIOR: return ( iLevel * 0.0f );
        case CHARCLASS_THIEF:   return ( iLevel * 0.0f );
        case CHARCLASS_HUNTER:  return ( iLevel * 0.0f );
        case CHARCLASS_MAGE:    return ( iLevel * 100.0f );
        case CHARCLASS_WARLOCK: return ( iLevel * 0.0f );
        case CHARCLASS_PRIEST:  return ( iLevel * 0.0f );
        case CHARCLASS_PALADIN: return ( iLevel * 0.0f );
        case CHARCLASS_DRUID:   return ( iLevel * 0.0f );
        default: Assert(false); return 0.0f;
    }
}
Scalar CharStatPackage::_ComputeBaseStat_Endurance( CharClass iClass, UInt iLevel )
{
    switch( iClass ) {
        case CHARCLASS_WARRIOR: return ( iLevel * 0.0f );
        case CHARCLASS_THIEF:   return ( iLevel * 0.0f );
        case CHARCLASS_HUNTER:  return ( iLevel * 0.0f );
        case CHARCLASS_MAGE:    return ( iLevel * 50.0f );
        case CHARCLASS_WARLOCK: return ( iLevel * 0.0f );
        case CHARCLASS_PRIEST:  return ( iLevel * 0.0f );
        case CHARCLASS_PALADIN: return ( iLevel * 0.0f );
        case CHARCLASS_DRUID:   return ( iLevel * 0.0f );
        default: Assert(false); return 0.0f;
    }
}
Scalar CharStatPackage::_ComputeBaseStat_Speed( CharClass iClass, UInt iLevel )
{
    switch( iClass ) {
        case CHARCLASS_WARRIOR: return ( iLevel * 0.0f );
        case CHARCLASS_THIEF:   return ( iLevel * 0.0f );
        case CHARCLASS_HUNTER:  return ( iLevel * 0.0f );
        case CHARCLASS_MAGE:    return ( iLevel * 75.0f );
        case CHARCLASS_WARLOCK: return ( iLevel * 0.0f );
        case CHARCLASS_PRIEST:  return ( iLevel * 0.0f );
        case CHARCLASS_PALADIN: return ( iLevel * 0.0f );
        case CHARCLASS_DRUID:   return ( iLevel * 0.0f );
        default: Assert(false); return 0.0f;
    }
}
Scalar CharStatPackage::_ComputeBaseStat_Armor( CharClass iClass, UInt iLevel )
{
    switch( iClass ) {
        case CHARCLASS_WARRIOR: return ( iLevel * 0.0f );
        case CHARCLASS_THIEF:   return ( iLevel * 0.0f );
        case CHARCLASS_HUNTER:  return ( iLevel * 0.0f );
        case CHARCLASS_MAGE:    return ( iLevel * 50.0f );
        case CHARCLASS_WARLOCK: return ( iLevel * 0.0f );
        case CHARCLASS_PRIEST:  return ( iLevel * 0.0f );
        case CHARCLASS_PALADIN: return ( iLevel * 0.0f );
        case CHARCLASS_DRUID:   return ( iLevel * 0.0f );
        default: Assert(false); return 0.0f;
    }
}
Scalar CharStatPackage::_ComputeBaseStat_Health( CharClass iClass, UInt iLevel )
{
    switch( iClass ) {
        case CHARCLASS_WARRIOR: return ( iLevel * 0.0f );
        case CHARCLASS_THIEF:   return ( iLevel * 0.0f );
        case CHARCLASS_HUNTER:  return ( iLevel * 0.0f );
        case CHARCLASS_MAGE:    return ( iLevel * 50.0f );
        case CHARCLASS_WARLOCK: return ( iLevel * 0.0f );
        case CHARCLASS_PRIEST:  return ( iLevel * 0.0f );
        case CHARCLASS_PALADIN: return ( iLevel * 0.0f );
        case CHARCLASS_DRUID:   return ( iLevel * 0.0f );
        default: Assert(false); return 0.0f;
    }
}
Scalar CharStatPackage::_ComputeBaseStat_Mana( CharClass iClass, UInt iLevel )
{
    switch( iClass ) {
        case CHARCLASS_WARRIOR: return ( iLevel * 0.0f );
        case CHARCLASS_THIEF:   return ( iLevel * 0.0f );
        case CHARCLASS_HUNTER:  return ( iLevel * 0.0f );
        case CHARCLASS_MAGE:    return ( iLevel * 100.0f );
        case CHARCLASS_WARLOCK: return ( iLevel * 0.0f );
        case CHARCLASS_PRIEST:  return ( iLevel * 0.0f );
        case CHARCLASS_PALADIN: return ( iLevel * 0.0f );
        case CHARCLASS_DRUID:   return ( iLevel * 0.0f );
        default: Assert(false); return 0.0f;
    }
}
Scalar CharStatPackage::_ComputeBaseStat_RegenHealth( CharClass iClass, UInt iLevel )
{
    switch( iClass ) {
        case CHARCLASS_WARRIOR: return ( iLevel * 0.0f );
        case CHARCLASS_THIEF:   return ( iLevel * 0.0f );
        case CHARCLASS_HUNTER:  return ( iLevel * 0.0f );
        case CHARCLASS_MAGE:    return ( iLevel * 50.0f );
        case CHARCLASS_WARLOCK: return ( iLevel * 0.0f );
        case CHARCLASS_PRIEST:  return ( iLevel * 0.0f );
        case CHARCLASS_PALADIN: return ( iLevel * 0.0f );
        case CHARCLASS_DRUID:   return ( iLevel * 0.0f );
        default: Assert(false); return 0.0f;
    }
}
Scalar CharStatPackage::_ComputeBaseStat_RegenMana( CharClass iClass, UInt iLevel )
{
    switch( iClass ) {
        case CHARCLASS_WARRIOR: return ( iLevel * 0.0f );
        case CHARCLASS_THIEF:   return ( iLevel * 0.0f );
        case CHARCLASS_HUNTER:  return ( iLevel * 0.0f );
        case CHARCLASS_MAGE:    return ( iLevel * 100.0f );
        case CHARCLASS_WARLOCK: return ( iLevel * 0.0f );
        case CHARCLASS_PRIEST:  return ( iLevel * 0.0f );
        case CHARCLASS_PALADIN: return ( iLevel * 0.0f );
        case CHARCLASS_DRUID:   return ( iLevel * 0.0f );
        default: Assert(false); return 0.0f;
    }
}
Scalar CharStatPackage::_ComputeBaseStat_Dodge( CharClass iClass, UInt iLevel )
{
    switch( iClass ) {
        case CHARCLASS_WARRIOR: return ( iLevel * 0.0f );
        case CHARCLASS_THIEF:   return ( iLevel * 0.0f );
        case CHARCLASS_HUNTER:  return ( iLevel * 0.0f );
        case CHARCLASS_MAGE:    return ( iLevel * 0.1f );
        case CHARCLASS_WARLOCK: return ( iLevel * 0.0f );
        case CHARCLASS_PRIEST:  return ( iLevel * 0.0f );
        case CHARCLASS_PALADIN: return ( iLevel * 0.0f );
        case CHARCLASS_DRUID:   return ( iLevel * 0.0f );
        default: Assert(false); return 0.0f;
    }
}
Scalar CharStatPackage::_ComputeBaseStat_Block( CharClass iClass, UInt iLevel )
{
    switch( iClass ) {
        case CHARCLASS_WARRIOR: return ( iLevel * 0.0f );
        case CHARCLASS_THIEF:   return ( iLevel * 0.0f );
        case CHARCLASS_HUNTER:  return ( iLevel * 0.0f );
        case CHARCLASS_MAGE:    return ( iLevel * 0.1f );
        case CHARCLASS_WARLOCK: return ( iLevel * 0.0f );
        case CHARCLASS_PRIEST:  return ( iLevel * 0.0f );
        case CHARCLASS_PALADIN: return ( iLevel * 0.0f );
        case CHARCLASS_DRUID:   return ( iLevel * 0.0f );
        default: Assert(false); return 0.0f;
    }
}
Scalar CharStatPackage::_ComputeBaseStat_Defense( CharClass iClass, UInt iLevel )
{
    switch( iClass ) {
        case CHARCLASS_WARRIOR: return ( iLevel * 0.0f );
        case CHARCLASS_THIEF:   return ( iLevel * 0.0f );
        case CHARCLASS_HUNTER:  return ( iLevel * 0.0f );
        case CHARCLASS_MAGE:    return ( iLevel * 25.0f );
        case CHARCLASS_WARLOCK: return ( iLevel * 0.0f );
        case CHARCLASS_PRIEST:  return ( iLevel * 0.0f );
        case CHARCLASS_PALADIN: return ( iLevel * 0.0f );
        case CHARCLASS_DRUID:   return ( iLevel * 0.0f );
        default: Assert(false); return 0.0f;
    }
}
Scalar CharStatPackage::_ComputeBaseStat_Resistance( CharClass iClass, UInt iLevel )
{
    switch( iClass ) {
        case CHARCLASS_WARRIOR: return ( iLevel * 0.0f );
        case CHARCLASS_THIEF:   return ( iLevel * 0.0f );
        case CHARCLASS_HUNTER:  return ( iLevel * 0.0f );
        case CHARCLASS_MAGE:    return ( iLevel * 50.0f );
        case CHARCLASS_WARLOCK: return ( iLevel * 0.0f );
        case CHARCLASS_PRIEST:  return ( iLevel * 0.0f );
        case CHARCLASS_PALADIN: return ( iLevel * 0.0f );
        case CHARCLASS_DRUID:   return ( iLevel * 0.0f );
        default: Assert(false); return 0.0f;
    }
}
Scalar CharStatPackage::_ComputeBaseStat_Power( CharClass iClass, UInt iLevel )
{
    switch( iClass ) {
        case CHARCLASS_WARRIOR: return ( iLevel * 0.0f );
        case CHARCLASS_THIEF:   return ( iLevel * 0.0f );
        case CHARCLASS_HUNTER:  return ( iLevel * 0.0f );
        case CHARCLASS_MAGE:    return ( iLevel * 10.0f );
        case CHARCLASS_WARLOCK: return ( iLevel * 0.0f );
        case CHARCLASS_PRIEST:  return ( iLevel * 0.0f );
        case CHARCLASS_PALADIN: return ( iLevel * 0.0f );
        case CHARCLASS_DRUID:   return ( iLevel * 0.0f );
        default: Assert(false); return 0.0f;
    }
}
Scalar CharStatPackage::_ComputeBaseStat_Hit( CharClass iClass, UInt iLevel )
{
    switch( iClass ) {
        case CHARCLASS_WARRIOR: return ( iLevel * 0.0f );
        case CHARCLASS_THIEF:   return ( iLevel * 0.0f );
        case CHARCLASS_HUNTER:  return ( iLevel * 0.0f );
        case CHARCLASS_MAGE:    return ( iLevel * 0.8f );
        case CHARCLASS_WARLOCK: return ( iLevel * 0.0f );
        case CHARCLASS_PRIEST:  return ( iLevel * 0.0f );
        case CHARCLASS_PALADIN: return ( iLevel * 0.0f );
        case CHARCLASS_DRUID:   return ( iLevel * 0.0f );
        default: Assert(false); return 0.0f;
    }
}
Scalar CharStatPackage::_ComputeBaseStat_Haste( CharClass iClass, UInt iLevel )
{
    switch( iClass ) {
        case CHARCLASS_WARRIOR: return ( iLevel * 0.0f );
        case CHARCLASS_THIEF:   return ( iLevel * 0.0f );
        case CHARCLASS_HUNTER:  return ( iLevel * 0.0f );
        case CHARCLASS_MAGE:    return ( iLevel * 1.0f );
        case CHARCLASS_WARLOCK: return ( iLevel * 0.0f );
        case CHARCLASS_PRIEST:  return ( iLevel * 0.0f );
        case CHARCLASS_PALADIN: return ( iLevel * 0.0f );
        case CHARCLASS_DRUID:   return ( iLevel * 0.0f );
        default: Assert(false); return 0.0f;
    }
}
Scalar CharStatPackage::_ComputeBaseStat_CriticalPower( CharClass iClass, UInt iLevel )
{
    switch( iClass ) {
        case CHARCLASS_WARRIOR: return ( iLevel * 0.0f );
        case CHARCLASS_THIEF:   return ( iLevel * 0.0f );
        case CHARCLASS_HUNTER:  return ( iLevel * 0.0f );
        case CHARCLASS_MAGE:    return ( iLevel * 2.0f );
        case CHARCLASS_WARLOCK: return ( iLevel * 0.0f );
        case CHARCLASS_PRIEST:  return ( iLevel * 0.0f );
        case CHARCLASS_PALADIN: return ( iLevel * 0.0f );
        case CHARCLASS_DRUID:   return ( iLevel * 0.0f );
        default: Assert(false); return 0.0f;
    }
}
Scalar CharStatPackage::_ComputeBaseStat_CriticalHit( CharClass iClass, UInt iLevel )
{
    switch( iClass ) {
        case CHARCLASS_WARRIOR: return ( iLevel * 0.0f );
        case CHARCLASS_THIEF:   return ( iLevel * 0.0f );
        case CHARCLASS_HUNTER:  return ( iLevel * 0.0f );
        case CHARCLASS_MAGE:    return ( iLevel * 0.1f );
        case CHARCLASS_WARLOCK: return ( iLevel * 0.0f );
        case CHARCLASS_PRIEST:  return ( iLevel * 0.0f );
        case CHARCLASS_PALADIN: return ( iLevel * 0.0f );
        case CHARCLASS_DRUID:   return ( iLevel * 0.0f );
        default: Assert(false); return 0.0f;
    }
}
Scalar CharStatPackage::_ComputeBaseStat_MagicPower( CharClass iClass, UInt iLevel )
{
    switch( iClass ) {
        case CHARCLASS_WARRIOR: return ( iLevel * 0.0f );
        case CHARCLASS_THIEF:   return ( iLevel * 0.0f );
        case CHARCLASS_HUNTER:  return ( iLevel * 0.0f );
        case CHARCLASS_MAGE:    return ( iLevel * 100.0f );
        case CHARCLASS_WARLOCK: return ( iLevel * 0.0f );
        case CHARCLASS_PRIEST:  return ( iLevel * 0.0f );
        case CHARCLASS_PALADIN: return ( iLevel * 0.0f );
        case CHARCLASS_DRUID:   return ( iLevel * 0.0f );
        default: Assert(false); return 0.0f;
    }
}
Scalar CharStatPackage::_ComputeBaseStat_MagicHit( CharClass iClass, UInt iLevel )
{
    switch( iClass ) {
        case CHARCLASS_WARRIOR: return ( iLevel * 0.0f );
        case CHARCLASS_THIEF:   return ( iLevel * 0.0f );
        case CHARCLASS_HUNTER:  return ( iLevel * 0.0f );
        case CHARCLASS_MAGE:    return ( iLevel * 1.0f );
        case CHARCLASS_WARLOCK: return ( iLevel * 0.0f );
        case CHARCLASS_PRIEST:  return ( iLevel * 0.0f );
        case CHARCLASS_PALADIN: return ( iLevel * 0.0f );
        case CHARCLASS_DRUID:   return ( iLevel * 0.0f );
        default: Assert(false); return 0.0f;
    }
}
Scalar CharStatPackage::_ComputeBaseStat_MagicHaste( CharClass iClass, UInt iLevel )
{
    switch( iClass ) {
        case CHARCLASS_WARRIOR: return ( iLevel * 0.0f );
        case CHARCLASS_THIEF:   return ( iLevel * 0.0f );
        case CHARCLASS_HUNTER:  return ( iLevel * 0.0f );
        case CHARCLASS_MAGE:    return ( iLevel * 1.0f );
        case CHARCLASS_WARLOCK: return ( iLevel * 0.0f );
        case CHARCLASS_PRIEST:  return ( iLevel * 0.0f );
        case CHARCLASS_PALADIN: return ( iLevel * 0.0f );
        case CHARCLASS_DRUID:   return ( iLevel * 0.0f );
        default: Assert(false); return 0.0f;
    }
}
Scalar CharStatPackage::_ComputeBaseStat_MagicCriticalPower( CharClass iClass, UInt iLevel )
{
    switch( iClass ) {
        case CHARCLASS_WARRIOR: return ( iLevel * 0.0f );
        case CHARCLASS_THIEF:   return ( iLevel * 0.0f );
        case CHARCLASS_HUNTER:  return ( iLevel * 0.0f );
        case CHARCLASS_MAGE:    return ( iLevel * 2.0f );
        case CHARCLASS_WARLOCK: return ( iLevel * 0.0f );
        case CHARCLASS_PRIEST:  return ( iLevel * 0.0f );
        case CHARCLASS_PALADIN: return ( iLevel * 0.0f );
        case CHARCLASS_DRUID:   return ( iLevel * 0.0f );
        default: Assert(false); return 0.0f;
    }
}
Scalar CharStatPackage::_ComputeBaseStat_MagicCriticalHit( CharClass iClass, UInt iLevel )
{
    switch( iClass ) {
        case CHARCLASS_WARRIOR: return ( iLevel * 0.0f );
        case CHARCLASS_THIEF:   return ( iLevel * 0.0f );
        case CHARCLASS_HUNTER:  return ( iLevel * 0.0f );
        case CHARCLASS_MAGE:    return ( iLevel * 0.5f );
        case CHARCLASS_WARLOCK: return ( iLevel * 0.0f );
        case CHARCLASS_PRIEST:  return ( iLevel * 0.0f );
        case CHARCLASS_PALADIN: return ( iLevel * 0.0f );
        case CHARCLASS_DRUID:   return ( iLevel * 0.0f );
        default: Assert(false); return 0.0f;
    }
}

Scalar CharStatPackage::_ComputeBaseStat( CharStatistic iStat ) const
{
    Assert( iStat < CHARSTAT_COUNT );
    return m_arrComputeBaseStat[iStat]( m_pOwner->GetClass(), m_pOwner->GetLevel() );
}

Void CharStatPackage::_ComputeBaseStats()
{
    for( UInt i = 0; i < CHARSTAT_COUNT; ++i )
        m_arrBaseStat[i] = _ComputeBaseStat( (CharStatistic)i );
}
Void CharStatPackage::_ComputeBaseStatModifiers()
{
    m_arrBaseStatModifier[CHARSTAT_STRENGTH] = 0.0f;
    m_arrBaseStatModifier[CHARSTAT_DEXTERITY] = 0.0f;
        
    m_arrBaseStatModifier[CHARSTAT_INTELLIGENCE] = 0.0f;
    m_arrBaseStatModifier[CHARSTAT_WISDOM] = 0.0f;
        
    m_arrBaseStatModifier[CHARSTAT_ENDURANCE] = 0.0f;
    m_arrBaseStatModifier[CHARSTAT_SPEED] = 0.0f;

    m_arrBaseStatModifier[CHARSTAT_ARMOR] = 0.0f;

    m_arrBaseStatModifier[CHARSTAT_HEALTH] = _ComputeBaseStatModifier_Health( m_arrBaseStat[CHARSTAT_ENDURANCE] );
    m_arrBaseStatModifier[CHARSTAT_MANA] = _ComputeBaseStatModifier_Mana( m_arrBaseStat[CHARSTAT_WISDOM] );

    m_arrBaseStatModifier[CHARSTAT_REGEN_HEALTH] = _ComputeBaseStatModifier_RegenHealth( m_arrBaseStat[CHARSTAT_ENDURANCE] );
    m_arrBaseStatModifier[CHARSTAT_REGEN_MANA] = _ComputeBaseStatModifier_RegenMana( m_arrBaseStat[CHARSTAT_WISDOM] );

    m_arrBaseStatModifier[CHARSTAT_DODGE] = _ComputeBaseStatModifier_Dodge( m_arrBaseStat[CHARSTAT_DEXTERITY], m_arrBaseStat[CHARSTAT_SPEED] );
    m_arrBaseStatModifier[CHARSTAT_BLOCK] = _ComputeBaseStatModifier_Block( m_arrBaseStat[CHARSTAT_STRENGTH] );
    m_arrBaseStatModifier[CHARSTAT_DEFENSE] = _ComputeBaseStatModifier_Defense( m_arrBaseStat[CHARSTAT_ENDURANCE], m_arrBaseStat[CHARSTAT_ARMOR] );
    m_arrBaseStatModifier[CHARSTAT_RESISTANCE] = _ComputeBaseStatModifier_Resistance( m_arrBaseStat[CHARSTAT_ENDURANCE], m_arrBaseStat[CHARSTAT_ARMOR],
                                                                                      m_arrBaseStat[CHARSTAT_INTELLIGENCE] );

    m_arrBaseStatModifier[CHARSTAT_POWER] = _ComputeBaseStatModifier_Power( m_arrBaseStat[CHARSTAT_STRENGTH] );
    m_arrBaseStatModifier[CHARSTAT_HIT] = _ComputeBaseStatModifier_Hit( m_arrBaseStat[CHARSTAT_DEXTERITY] );
    m_arrBaseStatModifier[CHARSTAT_HASTE] = _ComputeBaseStatModifier_Haste( m_arrBaseStat[CHARSTAT_DEXTERITY], m_arrBaseStat[CHARSTAT_SPEED] );
    m_arrBaseStatModifier[CHARSTAT_CRITICALPOWER] = _ComputeBaseStatModifier_CriticalPower( m_arrBaseStat[CHARSTAT_STRENGTH] );
    m_arrBaseStatModifier[CHARSTAT_CRITICALHIT] = _ComputeBaseStatModifier_CriticalHit( m_arrBaseStat[CHARSTAT_DEXTERITY] );

    m_arrBaseStatModifier[CHARSTAT_POWER_MAGIC] = _ComputeBaseStatModifier_MagicPower( m_arrBaseStat[CHARSTAT_INTELLIGENCE] );
    m_arrBaseStatModifier[CHARSTAT_HIT_MAGIC] = _ComputeBaseStatModifier_MagicHit( m_arrBaseStat[CHARSTAT_WISDOM] );
    m_arrBaseStatModifier[CHARSTAT_HASTE_MAGIC] = _ComputeBaseStatModifier_MagicHaste( m_arrBaseStat[CHARSTAT_INTELLIGENCE], m_arrBaseStat[CHARSTAT_SPEED] );
    m_arrBaseStatModifier[CHARSTAT_CRITICALPOWER_MAGIC] = _ComputeBaseStatModifier_MagicCriticalPower( m_arrBaseStat[CHARSTAT_INTELLIGENCE] );
    m_arrBaseStatModifier[CHARSTAT_CRITICALHIT_MAGIC] = _ComputeBaseStatModifier_MagicCriticalHit( m_arrBaseStat[CHARSTAT_WISDOM] );

    // Modded stats
    for( UInt i = 0; i < CHARSTAT_COUNT; ++i )
        m_arrModdedStat[i] = ( m_arrBaseStat[i] + m_arrBaseStatModifier[i] );
}
Void CharStatPackage::_ComputePersistentStatModifiers()
{
    // Modifiers from passives
    for( UInt i = 0; i < CHARSTAT_COUNT; ++i )
        m_arrPassiveStatModifier[i] = 0.0f;

    m_pOwner->EnumSkills();
    SkillInstance * pSkillInstance = m_pOwner->EnumNextSkill();
    while( pSkillInstance != NULL ) {
        if ( pSkillInstance->IsPassive() ) {
            const Scalar * arrBonus = ( (PassiveSkill*)(pSkillInstance->GetSkill()) )->GetStatBonus();
            for( UInt i = 0; i < CHARSTAT_COUNT; ++i )
                m_arrPassiveStatModifier[i] += arrBonus[i];
        }
        pSkillInstance = m_pOwner->EnumNextSkill();
    }

    // Modifiers from items
    for( UInt i = 0; i < CHARSTAT_COUNT; ++i )
        m_arrItemStatModifier[i] = 0.0f;

    for( UInt i = 0; i < EQUIPSLOTID_COUNT; ++i ) {
        ItemInstance * pItemInstance = m_pOwner->GetItem( (EquipmentSoltID)i );
        if ( pItemInstance->IsEmpty() )
            continue;
        Assert( pItemInstance->GetItem()->GetType() == ITEMTYPE_EQUIPMENT );
        EquipmentItem * pItem = (EquipmentItem*)( pItemInstance->GetItem() );
        const Scalar * arrBonus = pItem->GetStatBonus();
        for( UInt j = 0; j < CHARSTAT_COUNT; ++j )
            m_arrItemStatModifier[j] += arrBonus[j];
    }

    // Final stats
    for( UInt i = 0; i < CHARSTAT_COUNT; ++i )
        m_arrFinalStat[i] = ( m_arrModdedStat[i] + m_arrPassiveStatModifier[i] + m_arrItemStatModifier[i] );
}
Void CharStatPackage::_ComputeTemporaryStatModifiers()
{
    // Modifiers from Status effects
    for( UInt i = 0; i < CHARSTAT_COUNT; ++i )
        m_arrActiveStatModifier[i] = 0.0f;

    m_pOwner->EnumStatusEffects();
    StatusEffectID idStatusEffect = m_pOwner->EnumNextStatusEffect();
    while( idStatusEffect != STATUSEFFECTID_UNDEFINED ) {
        m_pOwner->EnumStatusEffectInstances();
        StatusEffectInstance * pStatusEffectInstance = m_pOwner->EnumNextStatusEffectInstance();
        while( pStatusEffectInstance != NULL ) {
            if ( pStatusEffectInstance->IsStatModifier() ) {
                const Scalar * arrModifier = ( (StatModifierEffect*)(pStatusEffectInstance->GetStatusEffect()) )->GetStatModifier();
                for( UInt i = 0; i < CHARSTAT_COUNT; ++i )
                    m_arrActiveStatModifier[i] += arrModifier[i];
            }
            pStatusEffectInstance = m_pOwner->EnumNextStatusEffectInstance();
        }
        idStatusEffect = m_pOwner->EnumNextStatusEffect();
    }

    // Effective stats
    for( UInt i = 0; i < CHARSTAT_COUNT; ++i )
        m_arrEffectiveStat[i] = ( m_arrFinalStat[i] + m_arrActiveStatModifier[i] );
}
Void CharStatPackage::_ComputeSecondaryStats()
{
    m_iMaxHP = _ComputeSecondaryStat_HP( m_arrEffectiveStat[CHARSTAT_HEALTH] );
    m_iMaxMP = _ComputeSecondaryStat_MP( m_arrEffectiveStat[CHARSTAT_MANA] );
    m_fMovementSpeed = _ComputeSecondaryStat_MovementSpeed( m_arrEffectiveStat[CHARSTAT_SPEED] );
}

