/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Gameplay/Statistics.h
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
// Known Bugs : Here is the heart of the game system ... all stat formulas !
//              Be sure to understand the maths if you change anything ...
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENTRYPOINT_SIMPLERPG_GAMEPLAY_STATISTICS_H
#define SCARAB_ENTRYPOINT_SIMPLERPG_GAMEPLAY_STATISTICS_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Engine/Application/ApplicationManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Statistics
enum CharStatistic {
	// Primary
	CHARSTAT_STRENGTH = 0,
	CHARSTAT_DEXTERITY,

	CHARSTAT_INTELLIGENCE,
	CHARSTAT_WISDOM,

	CHARSTAT_ENDURANCE,
	CHARSTAT_SPEED,          // Impacts on movement speed

    CHARSTAT_ARMOR,

	// Secondary
	CHARSTAT_HEALTH,              //                                           <= CHARSTAT_ENDURANCE
	CHARSTAT_MANA,                //                                           <= CHARSTAT_WISDOM

	CHARSTAT_REGEN_HEALTH,        // Health regen (HP/sec)                     <= CHARSTAT_ENDURANCE
	CHARSTAT_REGEN_MANA,          // Mana regen (MP/sec)                       <= CHARSTAT_WISDOM

    CHARSTAT_DODGE,               // Dodge rate (%)                            <= CHARSTAT_DEXTERITY, CHARSTAT_SPEED
    CHARSTAT_BLOCK,               // Block rate (%)                            <= CHARSTAT_STRENGTH
    CHARSTAT_DEFENSE,             // Physical damage reduction (%)             <= CHARSTAT_ENDURANCE, CHARSTAT_ARMOR
    CHARSTAT_RESISTANCE,          // Magical damage reduction (%)              <= CHARSTAT_ENDURANCE, CHARSTAT_INTELLIGENCE, CHARSTAT_ARMOR

	CHARSTAT_POWER,               // Physical damage                           <= CHARSTAT_STRENGTH
	CHARSTAT_HIT,                 // Physical hit rate (%)                     <= CHARSTAT_DEXTERITY
	CHARSTAT_HASTE,               // Physical strike speed (hit/sec)           <= CHARSTAT_DEXTERITY, CHARSTAT_SPEED
	CHARSTAT_CRITICALPOWER,       // Physical critical power ratio             <= CHARSTAT_STRENGTH
	CHARSTAT_CRITICALHIT,         // Physical critical hit rate (%)            <= CHARSTAT_DEXTERITY

	CHARSTAT_POWER_MAGIC,         // Magical damage                            <= CHARSTAT_INTELLIGENCE
	CHARSTAT_HIT_MAGIC,           // Magical hit rate (%)                      <= CHARSTAT_WISDOM
	CHARSTAT_HASTE_MAGIC,         // Magical cast speed (% casttime reduction) <= CHARSTAT_INTELLIGENCE, CHARSTAT_SPEED
	CHARSTAT_CRITICALPOWER_MAGIC, // Magical critical power ratio              <= CHARSTAT_INTELLIGENCE
	CHARSTAT_CRITICALHIT_MAGIC,   // Magical critical hit rate (%)             <= CHARSTAT_WISDOM

	CHARSTAT_COUNT
};

// Classes
enum CharClass {
	CHARCLASS_WARRIOR = 0,
	CHARCLASS_THIEF,
	CHARCLASS_HUNTER,
	CHARCLASS_MAGE,
	CHARCLASS_WARLOCK,
	CHARCLASS_PRIEST,
	CHARCLASS_PALADIN,
	CHARCLASS_DRUID,

	CHARCLASS_COUNT
};

// Prototypes
class BaseCharacter;

/////////////////////////////////////////////////////////////////////////////////
// The CharStatPackage class
class CharStatPackage
{
public:
    CharStatPackage( BaseCharacter * pOwner );
    virtual ~CharStatPackage();

    // Getters
    static const GChar * GetStatName( CharStatistic iStat );
    static const GChar * GetClassName( CharClass iClass );

    inline Scalar GetBaseStat( CharStatistic iStat ) const;
    inline Scalar GetBaseStatModifier( CharStatistic iStat ) const;
    inline Scalar GetModdedStat( CharStatistic iStat ) const;
    inline Scalar GetPassiveStatModifier( CharStatistic iStat ) const;
    inline Scalar GetItemStatModifier( CharStatistic iStat ) const;
    inline Scalar GetFinalStat( CharStatistic iStat ) const;
    inline Scalar GetActiveStatModifier( CharStatistic iStat ) const;
    inline Scalar GetEffectiveStat( CharStatistic iStat ) const;

    inline UInt GetMaxHP() const;
    inline UInt GetMaxMP() const;
    inline Scalar GetMovementSpeed() const;

    // Methods
    inline Void OnLevelChange();
    inline Void OnPassiveChange();
    inline Void OnItemChange();
    inline Void OnActiveChange();

protected:
    // Helpers
    static Scalar _ComputeBaseStat_Strength( CharClass iClass, UInt iLevel );
    static Scalar _ComputeBaseStat_Dexterity( CharClass iClass, UInt iLevel );
    static Scalar _ComputeBaseStat_Intelligence( CharClass iClass, UInt iLevel );
    static Scalar _ComputeBaseStat_Wisdom( CharClass iClass, UInt iLevel );
    static Scalar _ComputeBaseStat_Endurance( CharClass iClass, UInt iLevel );
    static Scalar _ComputeBaseStat_Speed( CharClass iClass, UInt iLevel );
    static Scalar _ComputeBaseStat_Armor( CharClass iClass, UInt iLevel );
    static Scalar _ComputeBaseStat_Health( CharClass iClass, UInt iLevel );
    static Scalar _ComputeBaseStat_Mana( CharClass iClass, UInt iLevel );
    static Scalar _ComputeBaseStat_RegenHealth( CharClass iClass, UInt iLevel );
    static Scalar _ComputeBaseStat_RegenMana( CharClass iClass, UInt iLevel );
    static Scalar _ComputeBaseStat_Dodge( CharClass iClass, UInt iLevel );
    static Scalar _ComputeBaseStat_Block( CharClass iClass, UInt iLevel );
    static Scalar _ComputeBaseStat_Defense( CharClass iClass, UInt iLevel );
    static Scalar _ComputeBaseStat_Resistance( CharClass iClass, UInt iLevel );
    static Scalar _ComputeBaseStat_Power( CharClass iClass, UInt iLevel );
    static Scalar _ComputeBaseStat_Hit( CharClass iClass, UInt iLevel );
    static Scalar _ComputeBaseStat_Haste( CharClass iClass, UInt iLevel );
    static Scalar _ComputeBaseStat_CriticalPower( CharClass iClass, UInt iLevel );
    static Scalar _ComputeBaseStat_CriticalHit( CharClass iClass, UInt iLevel );
    static Scalar _ComputeBaseStat_MagicPower( CharClass iClass, UInt iLevel );
    static Scalar _ComputeBaseStat_MagicHit( CharClass iClass, UInt iLevel );
    static Scalar _ComputeBaseStat_MagicHaste( CharClass iClass, UInt iLevel );
    static Scalar _ComputeBaseStat_MagicCriticalPower( CharClass iClass, UInt iLevel );
    static Scalar _ComputeBaseStat_MagicCriticalHit( CharClass iClass, UInt iLevel );

    typedef Scalar (*_ComputeBaseStatFunc)( CharClass, UInt );
    _ComputeBaseStatFunc m_arrComputeBaseStat[CHARSTAT_COUNT];

    Scalar _ComputeBaseStat( CharStatistic iStat ) const;

    inline static Scalar _ComputeBaseStatModifier_Health( Scalar fEndurance );
    inline static Scalar _ComputeBaseStatModifier_Mana( Scalar fWisdom );
    inline static Scalar _ComputeBaseStatModifier_RegenHealth( Scalar fEndurance );
    inline static Scalar _ComputeBaseStatModifier_RegenMana( Scalar fWisdom );
    inline static Scalar _ComputeBaseStatModifier_Dodge( Scalar fDexterity, Scalar fSpeed );
    inline static Scalar _ComputeBaseStatModifier_Block( Scalar fStrength );
    inline static Scalar _ComputeBaseStatModifier_Defense( Scalar fEndurance, Scalar fArmor );
    inline static Scalar _ComputeBaseStatModifier_Resistance( Scalar fEndurance, Scalar fArmor, Scalar fIntelligence );
    inline static Scalar _ComputeBaseStatModifier_Power( Scalar fStrength );
    inline static Scalar _ComputeBaseStatModifier_Hit( Scalar fDexterity );
    inline static Scalar _ComputeBaseStatModifier_Haste( Scalar fDexterity, Scalar fSpeed );
    inline static Scalar _ComputeBaseStatModifier_CriticalPower( Scalar fStrength );
    inline static Scalar _ComputeBaseStatModifier_CriticalHit( Scalar fDexterity );
    inline static Scalar _ComputeBaseStatModifier_MagicPower( Scalar fIntelligence );
    inline static Scalar _ComputeBaseStatModifier_MagicHit( Scalar fWisdom );
    inline static Scalar _ComputeBaseStatModifier_MagicHaste( Scalar fIntelligence, Scalar fSpeed );
    inline static Scalar _ComputeBaseStatModifier_MagicCriticalPower( Scalar fIntelligence );
    inline static Scalar _ComputeBaseStatModifier_MagicCriticalHit( Scalar fWisdom );

    inline static UInt _ComputeSecondaryStat_HP( Scalar fHealth );
    inline static UInt _ComputeSecondaryStat_MP( Scalar fMana );
    inline static Scalar _ComputeSecondaryStat_MovementSpeed( Scalar fSpeed );

    Void _ComputeBaseStats();
    Void _ComputeBaseStatModifiers();
    Void _ComputePersistentStatModifiers();
    Void _ComputeTemporaryStatModifiers();
    Void _ComputeSecondaryStats();

    BaseCharacter * m_pOwner;

    Scalar m_arrBaseStat[CHARSTAT_COUNT];
    Scalar m_arrBaseStatModifier[CHARSTAT_COUNT];
    Scalar m_arrModdedStat[CHARSTAT_COUNT];

    Scalar m_arrPassiveStatModifier[CHARSTAT_COUNT]; // Passive skills
    Scalar m_arrItemStatModifier[CHARSTAT_COUNT];    // Equipped items
    Scalar m_arrFinalStat[CHARSTAT_COUNT];

    Scalar m_arrActiveStatModifier[CHARSTAT_COUNT]; // Status effects
    Scalar m_arrEffectiveStat[CHARSTAT_COUNT];

    UInt m_iMaxHP, m_iMaxMP;
    Scalar m_fMovementSpeed;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Statistics.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_SIMPLERPG_GAMEPLAY_STATISTICS_H

