/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Gameplay/Character.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Gameplay : Character
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
#ifndef SCARAB_ENTRYPOINT_SIMPLERPG_GAMEPLAY_CHARACTER_H
#define SCARAB_ENTRYPOINT_SIMPLERPG_GAMEPLAY_CHARACTER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../World/Landscape.h"
#include "../Entities/EntityManager.h"
#include "../Animation/AnimationManager.h"

#include "Statistics.h"
#include "StatusEffects.h"
#include "Skills.h"
#include "Talents.h"
#include "Items.h"
#include "Factions.h"

#include "ThreatTable.h"

#include "Items/ItemClasses.h"
#include "Items/WeaponEmptyFists.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Character Caps
#define CHARACTER_LEVEL_CAP 60

#define CHARACTER_SELECTION_RANGE 60.0f
#define CHARACTER_TARGETLIST_MAX_SIZE 32

#define CHARACTER_MAX_NPC_LINKAGE 32

// Combat IDs
#define COMBATID_INVALID 0xffffffff

typedef UInt CombatID;

// IA state hints
enum NPCBehaviour {
    NPCBEHAVIOUR_INITIALIZE = 0,
    NPCBEHAVIOUR_STANDING,
    NPCBEHAVIOUR_WANDERING,
    NPCBEHAVIOUR_PATROLLING,
    NPCBEHAVIOUR_ATTACKING,
    NPCBEHAVIOUR_DEFENDING,
    NPCBEHAVIOUR_RUNNING,
    NPCBEHAVIOUR_SPECIAL, // Bosses
    NPCBEHAVIOUR_COUNT
};

/////////////////////////////////////////////////////////////////////////////////
// The BaseCharacter class
class BaseCharacter
{
public:
    BaseCharacter( const GChar * strName, CharClass iClass, UInt iLevel, const Vertex3 & vPosition );
    virtual ~BaseCharacter();

    // Getters
    virtual Bool IsPC() const = 0;
    virtual Bool IsNPC() const = 0;

    // Entity
    inline CharEntity * GetEntity() const;

    // Name & class
    inline const GChar * GetName() const;
    inline CharClass GetClass() const;

    // XP & leveling
    inline UInt GetLevel() const;
    inline UInt GetCurrentXP() const;
    inline UInt GetMaxXP() const;

    Void AddXP( UInt iXP );
    Void RemoveXP( UInt iXP );
    Void SetLevel( UInt iLevel );
    inline Void LevelUp();
    inline Void LevelDown();

    // Statistics
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

    // StatusEffects
    inline UInt GetStatusEffectCount() const;

    inline Bool HasStatusEffect( StatusEffectID idStatusEffect ) const;

    inline UInt GetStatusEffectInstanceCount( StatusEffectID idStatusEffect ) const;
    inline StatusEffectInstance * GetStatusEffectInstance( StatusEffectID idStatusEffect, UInt iIndex ) const;

    inline Void EnumStatusEffects() const;
    inline StatusEffectID EnumNextStatusEffect() const;

    inline Void EnumStatusEffectInstances() const;
    inline StatusEffectInstance * EnumNextStatusEffectInstance() const;

    inline Bool AddEffect( StatusEffectID idStatusEffect, BaseCharacter * pSource, UInt iRank, UInt iStacks );
    inline Bool RemoveEffect( StatusEffectID idStatusEffect );

    inline Bool RemoveEffects( StatusEffectType iType );
    inline Bool RemovePositiveEffects();
    inline Bool RemoveNegativeEffects();
    inline Bool RemoveMovementLimiterEffects();
    inline Bool RemoveActionLimiterEffects();
    inline Bool RemoveAllEffects();

    // Skills
    inline Bool HasSkill( SkillID idSkill ) const;
    inline SkillInstance * GetSkill( SkillID idSkill ) const;

    inline Void EnumSkills() const;
    inline SkillInstance * EnumNextSkill() const;

    inline Bool AddSkill( SkillID idSkill );
    inline Bool RemoveSkill( SkillID idSkill );

    inline Bool IsOnCooldown( SkillID idSkill ) const;
    inline Scalar GetCooldown( SkillID idSkill ) const;

    // Talents

    // Inventory
    inline ItemInstance * GetItem( InventoryBagID idBag, UInt iSlot );
    inline ItemInstance * GetItem( EquipmentSoltID idEquipSlot );

    inline UInt PickItem( Item * pItem, UInt iAmount = 1 );
    inline Void DropItem( InventoryBagID idBag, UInt iSlot );
    inline ItemInstance * EquipItem( EquipmentSoltID idEquipSlot, InventoryBagID idBag, UInt iSlot );
    inline ItemInstance * UnEquipItem( EquipmentSoltID idEquipSlot );

    inline ItemInstance * UseItem( BaseCharacter * pTarget, InventoryBagID idBag, UInt iSlot );
    inline ItemInstance * UseItem( BaseCharacter * pTarget, EquipmentSoltID idEquipSlot );

    inline Void SortBag( InventoryBagID idBag );
    inline Void SortAllBags();
    inline Void SortBufferBagItems();

    // Factions
    inline Bool IsHostile( BaseCharacter * pTarget ) const;

    // Environment checking
    inline Scalar GetDistance( BaseCharacter * pTarget ) const;
    inline Bool HasLineOfSight( BaseCharacter * pTarget ) const;
    inline const Array<SimpleEntity*> & GetEntitiesInRange( EntityType iType, Scalar fRange, Bool bSort ) const;

    // Movement tracking
    inline Vertex3 GetPosition() const;

    inline Bool IsMovingForward() const;
    inline Bool IsMovingBackward() const;
    inline Bool IsMovingLeft() const;
    inline Bool IsMovingRight() const;
    inline Bool IsMovingUp() const;
    inline Bool IsMovingDown() const;
    inline Bool IsMoving() const;

    inline Void SetMovingForward( Bool bMoving );
    inline Void SetMovingBackward( Bool bMoving );
    inline Void SetMovingLeft( Bool bMoving );
    inline Void SetMovingRight( Bool bMoving );
    inline Void SetMovingUp( Bool bMoving );
    inline Void SetMovingDown( Bool bMoving );
    inline Void SetStanding();

    inline const Vector3 & GetMovementDirection() const;

    Void Teleport( const Vertex3 & vPosition, Bool bFar, Bool bKeepInertia = false );

    inline Bool IsPathTracking() const;

    inline Void StartPathTracking( const Vertex3 & vTarget );
    inline Void EndPathTracking();

    // Target selection
    inline BaseCharacter * GetSelectedTarget() const;
    inline BaseCharacter * GetFocusedTarget() const;

    inline Void SelectTarget( BaseCharacter * pTarget );
    inline Void FocusTarget( BaseCharacter * pTarget );
    inline Void SwapFocusTarget();

    Void SelectNextNearestTarget();

    // Continuous attack
    inline Bool IsAttacking() const;
    inline BaseCharacter * GetAttackingTarget() const;

    Bool Attack( BaseCharacter * pTarget, Bool bContinuous, Bool bUseThrow );
    Bool InterruptAttack();
    Void WeaponAttack( ItemInstance * pWeaponInstance, BaseCharacter * pTarget );

    // Casting sequence
    inline Bool IsCasting() const;
    inline Scalar GetCastingTime() const; // Remaining time
    inline SkillID GetCastingSkill() const;
    inline BaseCharacter * GetCastingTarget() const;

    Bool Cast( SkillID idSkill, BaseCharacter * pTarget );
    Bool InterruptCast();
    Void SkillAttack( SkillInstance * pSkillInstance, BaseCharacter * pTarget );

    // Global cooldown
    inline Bool IsOnGlobalCooldown() const;
    inline Scalar GetGlobalCooldown() const; // Remaining time

    // State
    inline UInt GetCurrentHP() const;
    inline UInt GetCurrentMP() const;

    inline Bool IsInCombat() const;
    inline CombatID GetCombatID() const;

    inline Bool IsDead() const;

    // Methods
    Void Damage( BaseCharacter * pSource, UInt iDamage );
    Void Heal( BaseCharacter * pSource, UInt iHeal );

    Void Kill();
    Void Resurrect( UInt iHP );

    // Update routine
    virtual Void Update( Scalar fTimeStep );

protected:
    // Helpers
    static Int _Compare_CharDistance( BaseCharacter * const & rLeft, BaseCharacter * const & rRight, Void * pUserData );

    // Entity
    CharEntity * m_pEntity;

    // Name & class
    GChar m_strName[64];
    CharClass m_iClass;

    // XP & leveling
    inline static UInt _ComputeMaxXP( UInt iLevel );

    UInt m_iLevel;
    UInt m_iCurrentXP, m_iMaxXP;

    // Statistics
    CharStatPackage m_hStatistics;

    // StatusEffects
    StatusEffectBuffer m_hStatusEffectBuffer;

    // Skills
    SkillBook m_hSkillBook;

    // Talents
    TalentTreeInstance m_hTalentTree;

    // Inventory
    Inventory m_hInventory;

    // Factions
    FactionRegistry m_hFactionRegistry;

    // Target selection
    BaseCharacter * m_pSelectedTarget;
    BaseCharacter * m_pFocusedTarget;

    Scalar m_fTargetListLastRefresh;
    UInt m_iCurrentTarget;

    UInt m_iTargetCount;
    BaseCharacter * m_arrTargetList[CHARACTER_TARGETLIST_MAX_SIZE];

    SelectionMarkerEntity * m_pSelectionMarker;

    // Continuous attack
    Void _UpdateAttacking( Scalar fTimeStep );

    Bool m_bAttacking, m_bContinuous;
    Scalar m_fLastAttackElapsedTimeRH;
    WeaponRightItem * m_pAttackingWeaponRH;
    Scalar m_fLastAttackElapsedTimeLH;
    WeaponLeftItem * m_pAttackingWeaponLH;
    Scalar m_fLastAttackElapsedTimeT;
    WeaponThrowItem * m_pAttackingWeaponT;
    Scalar m_fLastAttackElapsedTimeF;
    WeaponEmptyFists * m_pAttackingWeaponF;
    BaseCharacter * m_pAttackingTarget;

    // Casting sequence
    Void _UpdateCasting( Scalar fTimeStep );

    SkillID m_idCastingSkill;
    Scalar m_fCastingTime;
    BaseCharacter * m_pCastingTarget;
    AnimationInstance m_hCastingAnimation;

    // Global cooldown
    Scalar m_fGlobalCooldown;

    // Character state
    Scalar m_fLastRegenTickElapsedTime;
    UInt m_iCurrentHP, m_iCurrentMP;

    CombatID m_idCombat;
};

/////////////////////////////////////////////////////////////////////////////////
// The PCharacter class
class PCharacter : public BaseCharacter
{
public:
    PCharacter( const GChar * strName, CharClass iClass, UInt iLevel, const Vertex3 & vPosition );
    virtual ~PCharacter();

    // Getters
    inline virtual Bool IsPC() const;
    inline virtual Bool IsNPC() const;

    // Update routine
    virtual Void Update( Scalar fTimeStep );

protected:
};

/////////////////////////////////////////////////////////////////////////////////
// The NPCharacter class
class NPCharacter : public BaseCharacter
{
public:
    NPCharacter( const GChar * strName, CharClass iClass, UInt iLevel, const Vertex3 & vPosition );
    virtual ~NPCharacter();

    // Getters
    inline virtual Bool IsPC() const;
    inline virtual Bool IsNPC() const;

    // Threat Table
    inline Bool HasThreat( BaseCharacter * pTarget ) const;
    inline UInt GetThreat( BaseCharacter * pTarget ) const;

    inline BaseCharacter * GetBiggestThreat( UInt * outThreat = NULL ) const;

    inline UInt AddThreat( BaseCharacter * pTarget, UInt iThreat );
    inline Void ResetThreat();

    // Update routine
    virtual Void Update( Scalar fTimeStep );

protected:
    // Threat distance & linkage
    Scalar m_fThreatDistance;
    NPCharacter * m_arrLinkedNPCs[CHARACTER_MAX_NPC_LINKAGE];

    // Threat Table
    ThreatTable m_hThreatTable;

    // IA Behaviour
    virtual Void _BehaviourScript( Scalar fTimeStep );

    NPCBehaviour m_iBehaviour;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Character.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_SIMPLERPG_GAMEPLAY_CHARACTER_H

