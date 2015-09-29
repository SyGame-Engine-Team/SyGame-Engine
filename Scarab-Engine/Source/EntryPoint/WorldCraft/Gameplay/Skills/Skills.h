/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/Gameplay/Skills/Skills.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : WorldCraft, Gameplay : Skills
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
#ifndef SCARAB_ENTRYPOINT_WORLDCRAFT_GAMEPLAY_SKILLS_SKILLS_H
#define SCARAB_ENTRYPOINT_WORLDCRAFT_GAMEPLAY_SKILLS_SKILLS_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Graphics/GraphicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define SKILL_NAME_LENGTH 64
#define SKILL_RANK_MAX    10

// Skill classification
enum SkillType {
    SKILLTYPE_UNDEFINED = 0,

    SKILLTYPE_TARGET_SELF,   // Skills acting on user directly
    SKILLTYPE_TARGET_SINGLE, // Skills acting on a single target
    SKILLTYPE_TARGET_MULTI,  // Skills acting on multiple targets
    SKILLTYPE_AREA_SELF,     // Skills acting on an area centered on user
    SKILLTYPE_AREA_TARGET,   // Skills acting on an area centered on a target
    SKILLTYPE_AREA_GROUND,   // Skills acting on an area centered on a ground spot

    SKILLTYPE_COUNT
};
enum SkillClass {
    SKILLCLASS_UNDEFINED = 0,

        // Target-Self type
    SKILLCLASS_PASSIVE, // Passive skills with permanent effects
    SKILLCLASS_TRIGGER, // Skills that trigger effects based on conditions (Contingency chains !)
    SKILLCLASS_BUFFER,  // Buffer skills with long/short durations

        // Target-Single type
    SKILLCLASS_BULLET, // Projectile skills, fast / weak impact
    SKILLCLASS_BOLT,   // Projectile skills, slow / strong impact

        // Target-Multi type
    SKILLCLASS_BALL,   // Single projectile, passes throught targets, long trajectory
    SKILLCLASS_BEAM,   // Continuous channeled beam, passes throught targets, short trajectory
    SKILLCLASS_SHARDS, // Multiple projectiles (may hit same target multiple times), spread in a fan / cone

        // Area-Self type
    SKILLCLASS_NOVA,  // Single area discharge
    SKILLCLASS_QUAKE, // Persistent effect area, stationary
    SKILLCLASS_AURA,  // Persistent effect area, follows user

        // Area-Target type
    SKILLCLASS_ROCKET,   // Single projectile, explosive
    SKILLCLASS_MISSILES, // Multiple projectiles, explosives

        // Area-Ground type
    SKILLCLASS_STORM,  // Persistent effect area, stationary
    SKILLCLASS_WALL,   // Persistent blocking area, stationary
    SKILLCLASS_MINE,   // Persistent trigger area, stationary
    SKILLCLASS_SUMMON, // Summoning skills

    SKILLCLASS_COUNT
};

// Skill identifiers
typedef DWord SkillID;
#define SKILLID_UNDEFINED 0

//SKILLID_DEBUG_

// Prototypes
class BaseCharacter;

class Skill;
class SkillInstance;

/////////////////////////////////////////////////////////////////////////////////
// The Skill class
class Skill
{
public:
    Skill();
    virtual ~Skill();

    // Skill IDs
    inline static DWord GetTypeBaseID( SkillType iType );
    inline static DWord GetTypeDomainCapacity( SkillType iType );
    inline static DWord GetClassBaseID( SkillClass iClass );
    inline static DWord GetClassDomainCapacity( SkillClass iClass );

    inline static SkillID MakeID( SkillClass iClass, DWord dwLocalID );

    static SkillType GetTypeFromID( SkillID iSkillID );
    static SkillClass GetClassFromID( SkillID iSkillID );
    static DWord GetLocalID( SkillID iSkillID, UInt * outClassDomainCapacity = NULL );

    // Getters
    inline SkillType GetType() const;
    inline SkillClass GetClass() const;
    inline SkillID GetID() const;

    inline Bool IsActive() const;
    inline Bool IsPassive() const;
    inline Bool RequiresTarget() const;

    inline const GChar * GetName() const;
    inline UInt GetMaxRank() const;

    inline UInt GetManaCost( UInt iRank ) const;

    inline Scalar GetRange( UInt iRank ) const;
    inline Scalar GetRangeAtTarget( UInt iRank ) const;

    inline Bool TriggersGCD( UInt iRank ) const;
    inline Bool IsInstant( UInt iRank ) const;
    inline Scalar GetCastingTime( UInt iRank ) const;
    inline Scalar GetCooldownTime( UInt iRank ) const;

    // Instanciation
    SkillInstance * CreateInstance( BaseCharacter * pOwner, UInt iRank ) const;

    // Methods
    virtual Void OnCastBegin( SkillInstance * pInstance, BaseCharacter * pTarget ) const = 0; // Called when cast sequence begins => store target, create models, animations, ... (allways called)
    virtual Void OnCastSuccess( SkillInstance * pInstance ) const = 0;                        // Called when casting is successful => throw skill effects, ...
    virtual Void OnCastFail( SkillInstance * pInstance ) const = 0;                           // Called when casting failed/was aborted => adjust animations, ...
    virtual Void OnCastEnd( SkillInstance * pInstance ) const = 0;                            // Called when cast sequence ends => cleanup models, animations, ... (allways called)

protected:
    // Skill IDs
    typedef struct _skillid_domain {
        DWord dwBaseID;
        DWord dwCapacity;
    } _SkillIDDomain;
    static _SkillIDDomain sm_arrTypeIDDomains[SKILLTYPE_COUNT];
    static _SkillIDDomain sm_arrClassIDDomains[SKILLCLASS_COUNT];

    // Skill data
    SkillType m_iType;
    SkillClass m_iClass;
    SkillID m_iSkillID;

    GChar m_strName[64];
    UInt m_iMaxRank;

    UInt m_iManaCost[SKILL_RANK_MAX];
    Scalar m_fRange[SKILL_RANK_MAX];
    Scalar m_fRangeAtTarget[SKILL_RANK_MAX];
    Bool m_bTriggersGCD[SKILL_RANK_MAX];
    Scalar m_fCastingTime[SKILL_RANK_MAX]; // 0.0f => Instant
    Scalar m_fCooldownTime[SKILL_RANK_MAX];
};

/////////////////////////////////////////////////////////////////////////////////
// The SkillInstance class
class SkillInstance
{
public:
    SkillInstance( const Skill * pSkill, BaseCharacter * pOwner, UInt iRank );
    SkillInstance( const SkillInstance & rhs );
    virtual ~SkillInstance();

    inline SkillInstance & operator=( const SkillInstance & rhs );

    // Getters
    inline const Skill * GetSkill() const;

    inline SkillType GetType() const;
    inline SkillClass GetClass() const;
    inline SkillID GetID() const;

    inline Bool IsActive() const;
    inline Bool IsPassive() const;
    inline Bool RequiresTarget() const;

    inline const GChar * GetName() const;
    inline UInt GetRank() const;
    inline UInt GetMaxRank() const;

    inline BaseCharacter * GetOwner() const;

    inline UInt GetManaCost( UInt iRank ) const;

    inline Scalar GetRange( UInt iRank ) const;
    inline Scalar GetRangeAtTarget( UInt iRank ) const;

    inline Bool TriggersGCD( UInt iRank ) const;
    inline Bool IsInstant( UInt iRank ) const;
    inline Scalar GetCastingTime( UInt iRank ) const;
    inline Scalar GetCooldownTime( UInt iRank ) const;

    // Methods
    inline Bool IsCastInProgress() const;
    inline Bool IsOnCooldown() const;
    inline Scalar GetCooldown() const;
    inline Void StartCooldown();
    inline Void ResetCooldown();
    inline Void DecreaseCooldown( Scalar fAmount );

    Bool Cast( BaseCharacter * pTarget );

    // Update routine
    Bool Update( Scalar fTimeStep );

protected:
    const Skill * m_pSkill;
    BaseCharacter * m_pOwner;

    // Rank
    UInt m_iRank;

    // Cooldown
    Bool m_bCastInProgress;
    Scalar m_fCooldown;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Skills.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_WORLDCRAFT_GAMEPLAY_SKILLS_SKILLS_H

