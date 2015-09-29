/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Gameplay/Skills.cpp
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
// Includes
#include "Skills.h"

#include "GameplayManager.h"
#include "../Interface/InterfaceManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Skill implementation
Skill::Skill()
{
    m_idSkill = SKILLID_UNDEFINED;
    m_strName[0] = NULLBYTE;

    m_iMaxRank = 0;

    m_bPassive = false;
    m_bMagic = false;
    m_bRequiresTarget = false;

    m_iManaCost = 0;

    m_fRange = 0.0f;
    m_fRangeAtTarget = 0.0f;

    m_fCastingTime = 0.0f;
    m_fCooldownTime = 0.0f;

    m_pAnimation = NULL;
}
Skill::~Skill()
{
    // nothing to do
}

Skill * Skill::GetInstance( SkillID idSkill )
{
    return GameplayFn->GetSkill( idSkill );
}

Void Skill::GenerateInstance( SkillInstance * outSkillInstance, BaseCharacter * pOwner )
{
    outSkillInstance->m_pSkill = this;
    outSkillInstance->m_pOwner = pOwner;

    outSkillInstance->m_iRank = 1;

    outSkillInstance->m_fCooldown = 0.0f;

    for( UInt i = 0; i < SKILL_MAX_ANIMATIONS; ++i )
        outSkillInstance->m_arrAnimationInstance[i] = AnimationInstance();
}

/////////////////////////////////////////////////////////////////////////////////
// SkillInstance implementation
SkillInstance::SkillInstance()
{
    m_pSkill = NULL;
    m_pOwner = NULL;

    m_iRank = 0;

    m_fCooldown = 0.0f;

    m_iAnimationCount = 0;
}
SkillInstance::~SkillInstance()
{
    for( UInt i = 0; i < m_iAnimationCount; ++i ) {
        if ( m_arrAnimationInstance[i].GetAnimation() != NULL )
            m_arrAnimationInstance[i].Interrupt();
    }
}

SkillInstance & SkillInstance::operator=( const SkillInstance & rhs )
{
    m_pSkill = rhs.m_pSkill;
    m_pOwner = rhs.m_pOwner;

    m_iRank = rhs.m_iRank;

    m_fCooldown = rhs.m_fCooldown;

    m_iAnimationCount = rhs.m_iAnimationCount;
    for( UInt i = 0; i < m_iAnimationCount; ++i )
        m_arrAnimationInstance[i] = rhs.m_arrAnimationInstance[i];

    return (*this);
}

Void SkillInstance::Cast( BaseCharacter * pTarget )
{
    Assert( m_iAnimationCount < SKILL_MAX_ANIMATIONS ); // BEWARE ! Guaranted by timing ...

    Animation * pAnimation = m_pSkill->GetAnimation();

    pAnimation->GenerateInstance( (m_arrAnimationInstance + m_iAnimationCount), 0.5f, m_pOwner, pTarget );
    ++m_iAnimationCount;
}

Void SkillInstance::Update( Scalar fTimeStep )
{
    Assert( m_pSkill != NULL );

    // Update cooldown
    if ( m_fCooldown > 0.0f ) {
        m_fCooldown -= fTimeStep;
        if ( m_fCooldown < 0.0f )
            m_fCooldown = 0.0f;
    }

    // Update skill animations
    UInt i = 0;
    while( i < m_iAnimationCount ) {
        AnimationInstance * pAnimationInstance = ( m_arrAnimationInstance + i );
        Assert( pAnimationInstance->GetAnimation() != NULL );

        Bool bContinue = pAnimationInstance->Update( fTimeStep );
        if ( bContinue ) {
            ++i;
            continue;
        }

        m_pSkill->OnSkillEffect( this, pAnimationInstance->GetTarget() );

        // Remove faded instances
        if ( m_iAnimationCount == 1 ) {
            m_arrAnimationInstance[0] = AnimationInstance();
            m_iAnimationCount = 0;
            break;
        }
        if ( i == (m_iAnimationCount - 1) ) {
            m_arrAnimationInstance[i] = AnimationInstance();
            --m_iAnimationCount;
            break;
        }

        m_arrAnimationInstance[i] = m_arrAnimationInstance[m_iAnimationCount - 1];
        m_arrAnimationInstance[m_iAnimationCount - 1] = AnimationInstance();
        --m_iAnimationCount;
    }
}

/////////////////////////////////////////////////////////////////////////////////
// SkillBook implementation
SkillBook::SkillBook( BaseCharacter * pOwner ):
    m_mapSkills(), m_itEnumerate()
{
    m_pOwner = pOwner;

    m_mapSkills.SetComparator( _Compare_SkillIDs, NULL );
    m_mapSkills.UseMemoryContext( GameplayFn->GetMemoryContext(), TEXT("Scratch") );
    m_mapSkills.Create();
}
SkillBook::~SkillBook()
{
    m_mapSkills.Destroy();
}

Bool SkillBook::AddSkill( SkillID idSkill )
{
    if ( m_mapSkills.Contains(idSkill) )
        return false;

    Skill * pSkill = Skill::GetInstance( idSkill );
    SkillInstance hInstance;
    pSkill->GenerateInstance( &hInstance, m_pOwner );

    Bool bInserted = m_mapSkills.Insert( idSkill, hInstance );
    Assert( bInserted );

    // Needed
    if ( StringFn->Cmp( m_pOwner->GetName(), TEXT("Shiki") ) == 0 )
        InterfaceFn->UpdateSkillBookView();

    return true;
}
Bool SkillBook::RemoveSkill( SkillID idSkill )
{
    Bool bRemoved = m_mapSkills.Remove( idSkill );

    // Needed
    if ( StringFn->Cmp( m_pOwner->GetName(), TEXT("Shiki") ) == 0 )
        InterfaceFn->UpdateSkillBookView();

    return bRemoved;
}

Void SkillBook::Update( Scalar fTimeStep )
{
    for( _SkillMap::Iterator itSkill = m_mapSkills.Begin(); !(itSkill.IsNull()); ++itSkill ) {
        SkillInstance * pSkillInstance = itSkill.GetItemPtr();
        pSkillInstance->Update( fTimeStep );
    }
}

/////////////////////////////////////////////////////////////////////////////////

Int SkillBook::_Compare_SkillIDs( const SkillID & rLeft, const SkillID & rRight, Void * /*pUserData*/ )
{
    if ( rLeft < rRight )
        return +1;
    if ( rLeft > rRight )
        return -1;
    return 0;
}



