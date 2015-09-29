/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Gameplay/Skills.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Gameplay : Skills
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
#ifndef SCARAB_ENTRYPOINT_SIMPLERPG_GAMEPLAY_SKILLS_H
#define SCARAB_ENTRYPOINT_SIMPLERPG_GAMEPLAY_SKILLS_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Animation/AnimationManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define SKILL_MAX_ANIMATIONS 4

// Skill list
enum SkillID {
    SKILLID_UNDEFINED = 0,

    // Basic skills
    SKILLID_BASICATTACK,
    SKILLID_BASICCAST, // Wand

    SKILLID_WARPHOME,
    SKILLID_REST,

    // Human skills
    SKILLID_HUMAN_SCIENCE,   // passive, INT +10
    SKILLID_HUMAN_CHARISM,   // passive, Reputation rate +10%

    // Mage skills
    SKILLID_MAGE_FIREBOLT,   // target 30m, 2s cast, no cd, 100mp
    SKILLID_MAGE_ICEARROW,   // target 30m, 2s cast, 5s cd, 125mp, slow 30% 5s
    SKILLID_MAGE_BLAZE,      // target 20m, instant, 10s cd, 175mp
    SKILLID_MAGE_ICENOVA,    // aoe-self 10m, instant, 12s cd, 150mp, freeze 3s
    SKILLID_MAGE_MAGICBLAST, // aoe-self 15m, instant, no cd, 200mp
    SKILLID_MAGE_EARTHQUAKE, // aoe-target 25m/10m, 2s cast, 5s cd, 125mp
    SKILLID_MAGE_BLIZZARD,   // aoe-target 30m/10m, channeled, 10s cd, 100mp/s
    SKILLID_MAGE_BLINK,      // self, instant, 15s cd, 150mp, teleport backward 20m

    SKILLID_COUNT
};

// Prototypes
class BaseCharacter;

class SkillInstance;

/////////////////////////////////////////////////////////////////////////////////
// The Skill class
class Skill
{
public:
    Skill();
    virtual ~Skill();

    // Getters
    static Skill * GetInstance( SkillID idSkill );

    inline SkillID GetID() const;
    inline const GChar * GetName() const;

    inline UInt GetMaxRank() const;

    inline Bool IsActive() const;
    inline Bool IsPassive() const;
    inline Bool IsPhysical() const;
    inline Bool IsMagical() const;
    inline Bool RequiresTarget() const;

    inline UInt GetManaCost() const;

    inline Scalar GetRange() const;
    inline Scalar GetRangeAtTarget() const;

    inline Bool IsInstant() const;
    inline Scalar GetCastingTime() const;
    inline Scalar GetCooldownTime() const;

    inline SkillAnimation * GetAnimation() const;

    // Methods
    Void GenerateInstance( SkillInstance * outSkillInstance, BaseCharacter * pOwner );

    virtual Void OnSkillEffect( SkillInstance * pInstance, BaseCharacter * pTarget ) const = 0;

protected:
    SkillID m_idSkill;
    GChar m_strName[64];

    UInt m_iMaxRank;

    // Flags
    Bool m_bPassive; // else Active
    Bool m_bMagic;   // else Physic
    Bool m_bRequiresTarget;

    // Mana
    UInt m_iManaCost;
    
    // Range
    Scalar m_fRange;
    Scalar m_fRangeAtTarget;

    // Timing
    Scalar m_fCastingTime; // 0.0f => Instant
    Scalar m_fCooldownTime;

    // Animation
    SkillAnimation * m_pAnimation;

    // IA hints
    //Scalar m_fIAHint_Priority;
    //Bool m_bIAHint_Aggressive;
    //Bool m_bIAHint_Defensive;
    //Bool m_bIAHint_Healing;
    //Bool m_bIAHint_MultiTarget;
    //Bool m_bIAHint_AOE;
};

/////////////////////////////////////////////////////////////////////////////////
// The SkillInstance class
class SkillInstance
{
public:
    SkillInstance();
    virtual ~SkillInstance();

    SkillInstance & operator=( const SkillInstance & rhs );

    // Getters
    inline Skill * GetSkill() const;
    inline BaseCharacter * GetOwner() const;

    inline SkillID GetID() const;
    inline const GChar * GetName() const;

    inline UInt GetRank() const;
    inline UInt GetMaxRank() const;

    inline Bool IsActive() const;
    inline Bool IsPassive() const;
    inline Bool IsPhysical() const;
    inline Bool IsMagical() const;
    inline Bool RequiresTarget() const;

    inline UInt GetManaCost() const;

    inline Scalar GetRange() const;
    inline Scalar GetRangeAtTarget() const;

    inline Bool IsInstant() const;
    inline Scalar GetCastingTime() const;
    inline Scalar GetCooldownTime() const;

    inline Bool IsOnCooldown() const;
    inline Scalar GetCooldown() const;
    inline Void StartCooldown();

    inline SkillAnimation * GetAnimation() const;

    // Methods
    Void Cast( BaseCharacter * pTarget );

    // Update routine
    Void Update( Scalar fTimeStep );

protected:
    friend class Skill;

    Skill * m_pSkill;
    BaseCharacter * m_pOwner;

    UInt m_iRank;

    // Timing
    Scalar m_fCooldown;

    // Animation
    UInt m_iAnimationCount;
    AnimationInstance m_arrAnimationInstance[SKILL_MAX_ANIMATIONS];
};

/////////////////////////////////////////////////////////////////////////////////
// The SkillBook class
class SkillBook
{
public:
    SkillBook( BaseCharacter * pOwner );
    virtual ~SkillBook();

    // Getters
    inline Bool HasSkill( SkillID idSkill ) const;
    inline SkillInstance * GetSkill( SkillID idSkill ) const;

    inline Void EnumSkills() const;
    inline SkillInstance * EnumNextSkill() const;

    // Methods
    Bool AddSkill( SkillID idSkill );
    Bool RemoveSkill( SkillID idSkill );

    // Update routine
    Void Update( Scalar fTimeStep );

protected:
    BaseCharacter * m_pOwner;

    static Int _Compare_SkillIDs( const SkillID & rLeft, const SkillID & rRight, Void * pUserData );

    typedef TreeMap<SkillID,SkillInstance> _SkillMap;
    _SkillMap m_mapSkills;

    mutable _SkillMap::Iterator m_itEnumerate;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Skills.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_SIMPLERPG_GAMEPLAY_SKILLS_H

