/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/Gameplay/Skills/Skills.cpp
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
// Includes
#include "Skills.h"

#include "../GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Skill implementation
Skill::_SkillIDDomain Skill::sm_arrTypeIDDomains[SKILLTYPE_COUNT] = {
    { 0, 0 }, // SKILLTYPE_UNDEFINED
    { 0x00000000, 0x20000000 }, // SKILLTYPE_TARGET_SELF
    { 0x20000000, 0x20000000 }, // SKILLTYPE_TARGET_SINGLE
    { 0x40000000, 0x30000000 }, // SKILLTYPE_TARGET_MULTI
    { 0x70000000, 0x30000000 }, // SKILLTYPE_AREA_SELF
    { 0xa0000000, 0x20000000 }, // SKILLTYPE_AREA_TARGET
    { 0xc0000000, 0x40000000 }  // SKILLTYPE_AREA_GROUND
};
Skill::_SkillIDDomain Skill::sm_arrClassIDDomains[SKILLCLASS_COUNT] = {
    { 0, 0 }, // SKILLCLASS_UNDEFINED
    // Target-Self type
    { 0x00000000, 0x08000000 }, // SKILLCLASS_PASSIVE
    { 0x08000000, 0x08000000 }, // SKILLCLASS_TRIGGER
    { 0x10000000, 0x10000000 }, // SKILLCLASS_BUFFER
    // Target-Single type
    { 0x20000000, 0x10000000 }, // SKILLCLASS_BULLET
    { 0x30000000, 0x10000000 }, // SKILLCLASS_BOLT
    // Target-Multi type
    { 0x40000000, 0x10000000 }, // SKILLCLASS_BALL
    { 0x50000000, 0x10000000 }, // SKILLCLASS_BEAM
    { 0x60000000, 0x10000000 }, // SKILLCLASS_SHARDS
    // Area-Self type
    { 0x70000000, 0x10000000 }, // SKILLCLASS_NOVA
    { 0x80000000, 0x10000000 }, // SKILLCLASS_QUAKE
    { 0x90000000, 0x10000000 }, // SKILLCLASS_AURA
    // Area-Target type
    { 0xa0000000, 0x10000000 }, // SKILLCLASS_ROCKET
    { 0xb0000000, 0x10000000 }, // SKILLCLASS_MISSILES
    // Area-Ground type
    { 0xc0000000, 0x10000000 }, // SKILLCLASS_STORM
    { 0xd0000000, 0x10000000 }, // SKILLCLASS_WALL
    { 0xe0000000, 0x10000000 }, // SKILLCLASS_MINE
    { 0xf0000000, 0x10000000 }  // SKILLCLASS_SUMMON
};

Skill::Skill()
{
    m_iType = SKILLTYPE_UNDEFINED;
    m_iClass = SKILLCLASS_UNDEFINED;
    m_iSkillID = SKILLID_UNDEFINED;

    m_strName[0] = NULLBYTE;
    m_iMaxRank = 0;

    m_iManaCost[0] = 0;
    m_fRange[0] = 0.0f;
    m_fRangeAtTarget[0] = 0.0f;
    m_bTriggersGCD[0] = false;
    m_fCastingTime[0] = 0.0f;
    m_fCooldownTime[0] = 0.0f;
}
Skill::~Skill()
{
    // nothing to do
}

SkillType Skill::GetTypeFromID( SkillID iSkillID )
{
    for( UInt i = 1; i < SKILLTYPE_COUNT; ++i ) {
        if ( iSkillID < sm_arrTypeIDDomains[i].dwBaseID )
            return (SkillType)( i - 1 );
    }

    return (SkillType)( SKILLTYPE_COUNT - 1 );
}
SkillClass Skill::GetClassFromID( SkillID iSkillID )
{
    for( UInt i = 1; i < SKILLCLASS_COUNT; ++i ) {
        if ( iSkillID < sm_arrClassIDDomains[i].dwBaseID )
            return (SkillClass)( i - 1 );
    }

    return (SkillClass)( SKILLCLASS_COUNT - 1 );
}
DWord Skill::GetLocalID( SkillID iSkillID, UInt * outClassDomainCapacity )
{
    for( UInt i = 1; i < SKILLCLASS_COUNT; ++i ) {
        if ( iSkillID < sm_arrClassIDDomains[i].dwBaseID ) {
            if ( outClassDomainCapacity != NULL )
                *outClassDomainCapacity = sm_arrClassIDDomains[i-1].dwCapacity;
            return ( iSkillID - sm_arrClassIDDomains[i-1].dwBaseID );
        }
    }

    if ( outClassDomainCapacity != NULL )
        *outClassDomainCapacity = sm_arrClassIDDomains[SKILLCLASS_COUNT-1].dwCapacity;
    return ( iSkillID - sm_arrClassIDDomains[SKILLCLASS_COUNT-1].dwBaseID );
}

SkillInstance * Skill::CreateInstance( BaseCharacter * pOwner, UInt iRank ) const
{
    Assert( iRank < m_iMaxRank );

    GameplayFn->SelectMemory( TEXT("SkillInstances") );
    SkillInstance * pSkillInstance = New SkillInstance( this, pOwner, iRank );
    GameplayFn->UnSelectMemory();

    return pSkillInstance;
}

/////////////////////////////////////////////////////////////////////////////////
// SkillInstance implementation
SkillInstance::SkillInstance( const Skill * pSkill, BaseCharacter * pOwner, UInt iRank )
{
    m_pSkill = pSkill;
    m_pOwner = pOwner;

    m_iRank = iRank;

    m_bCastInProgress = false;
    m_fCooldown = 0.0f;
}
SkillInstance::SkillInstance( const SkillInstance & rhs )
{
    m_pSkill = rhs.m_pSkill;
    m_pOwner = rhs.m_pOwner;
    m_iRank = rhs.m_iRank;
    m_bCastInProgress = rhs.m_bCastInProgress;
    m_fCooldown = rhs.m_fCooldown;
}
SkillInstance::~SkillInstance()
{
    // nothing to do
}

Bool SkillInstance::Cast( BaseCharacter * pTarget )
{
    // Check if we're already casting
    if ( m_bCastInProgress )
        return false;

    // Check if we're on cooldown
    if ( m_fCooldown > 0.0f )
        return false;

    // Start casting
    m_bCastInProgress = true;
    m_fCooldown = m_pSkill->GetCooldownTime( m_iRank );

    m_pSkill->OnCastBegin( this, pTarget );

    return true;
}

Bool SkillInstance::Update( Scalar fTimeStep )
{
    Assert( m_pSkill != NULL );

    // Update cooldown
    if ( m_fCooldown > 0.0f ) {
        m_fCooldown -= fTimeStep;
        if ( m_fCooldown < 0.0f )
            m_fCooldown = 0.0f;
    }

    // Check if a cast is in progress
    if ( !m_bCastInProgress )
        return false;

    ////////////////////////////////////
}




