/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Gameplay/GameplayManager.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Gameplay : GameplayManager
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
#ifndef SCARAB_ENTRYPOINT_SIMPLERPG_GAMEPLAY_GAMEPLAYMANAGER_H
#define SCARAB_ENTRYPOINT_SIMPLERPG_GAMEPLAY_GAMEPLAYMANAGER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Graphics/GraphicsManager.h"
#include "../Entities/EntityManager.h"
#include "../Animation/AnimationManager.h"

#include "Statistics.h"
#include "StatusEffects.h"
#include "Skills.h"
#include "Talents.h"
#include "Items.h"
#include "Factions.h"
#include "ThreatTable.h"
#include "Character.h"

#include "StatusEffects/StatusEffectClasses.h"

#include "Skills/SkillClasses.h"
#include "Skills/MageSkills.h"

#include "Talents/TalentTreeClasses.h"

#include "Items/ItemClasses.h"
#include "Items/WeaponEmptyFists.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define GameplayFn GameplayManager::GetInstancePtr()

// Combats
#define COMBAT_MAX_CHARACTERS 1024

// Game settings
#define GAMEPLAY_GLOBAL_COOLDOWN 0.5f

/////////////////////////////////////////////////////////////////////////////////
// The GameplayManager class
class GameplayManager : public Manager<GameplayManager>
{
public:
    inline static const GChar * GetName();
    inline static const GChar * GetMemoryBudget();

protected:
    friend class Manager<GameplayManager>;
    GameplayManager();
    virtual ~GameplayManager();

public:
    Void Initialize();
    Void Cleanup();

    // Randomness provider
    inline static Scalar RandUnit();
    inline static UInt RandRange( UInt iMin, UInt iMax );

    // StatusEffect database
    inline StatusEffect * GetStatusEffect( StatusEffectID idStatusEffect ) const;

    // Skill database
    inline Skill * GetSkill( SkillID idSkill ) const;

    // Talent database
    inline TalentTree * GetTalentTree( CharClass iClass ) const;

    // Item database
    inline Item * GetItem( ItemID idItem ) const;

    // Quest database

    // Characters access
    inline BaseCharacter * GetCharacter( const GChar * strName ) const;

    BaseCharacter * CreateCharacter( const GChar * strName, CharClass iClass, UInt iLevel, Bool bNPC, const Vertex3 & vPosition );
    Void DestroyCharacter( const GChar * strName );

    // Combat management
    inline Void EnumCombatIDs() const;
    inline CombatID EnumNextCombatID() const;

    inline UInt GetCombatCharacterCount( CombatID idCombat ) const;
    inline BaseCharacter * GetCombatCharacter( CombatID idCombat, UInt iIndex ) const;

    CombatID CreateCombat( BaseCharacter * pSource, BaseCharacter * pTarget );
    CombatID MergeCombats( CombatID idSourceCombat, CombatID idTargetCombat );

    Void JoinCombat( CombatID idCombat, BaseCharacter * pTarget );
    Void LeaveCombat( CombatID idCombat, BaseCharacter * pTarget );

    // Update routine
    Void Update( Scalar fTimeStep );

private:
    static const Scalar _INV_RANDMAX;

    // StatusEffect database
    StatusEffect * m_arrStatusEffects[STATUSEFFECTID_COUNT];

    // Skill database
    Skill * m_arrSkills[SKILLID_COUNT];

    // Talent database
    TalentTree * m_arrTalentTrees[CHARCLASS_COUNT];

    // Item database
    Item * m_arrItems[ITEMID_COUNT];

    // Quest database

    // Characters
    inline static Int _Compare_Strings( const GChar * const & rLeft, const GChar * const & rRight, Void * pUserData );

    typedef TreeMap<const GChar*,BaseCharacter*> _CharacterMap;
    _CharacterMap m_mapCharacters;

    // Combats
    class _CombatData { public:
        _CombatData();
        ~_CombatData();

        _CombatData & operator=( const _CombatData & rhs );

        UInt iCharacterCount;
        BaseCharacter * arrCharacters[COMBAT_MAX_CHARACTERS];
    };

    inline static Int _Compare_CombatIDs( const CombatID & rLeft, const CombatID & rRight, Void * pUserData );

    inline CombatID _GenerateCombatID() const;

    typedef TreeMap<CombatID,_CombatData> _CombatMap;
    _CombatMap m_mapCombats;

    mutable _CombatMap::Iterator m_itEnumerate;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GameplayManager.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_SIMPLERPG_GAMEPLAY_GAMEPLAYMANAGER_H

