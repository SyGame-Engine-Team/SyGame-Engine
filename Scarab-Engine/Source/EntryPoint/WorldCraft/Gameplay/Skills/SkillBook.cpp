/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/Gameplay/Skills/SkillBook.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : WorldCraft, Gameplay : SkillBook
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
#include "SkillBook.h"

#include "../GameplayManager.h"

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

Bool SkillBook::AddSkill( SkillInstance * pSkillInstance )
{
    if ( m_mapSkills.Contains(pSkillInstance->GetID()) )
        return false;

    Bool bInserted = m_mapSkills.Insert( pSkillInstance->GetID(), pSkillInstance );
    Assert( bInserted );

    return true;
}
SkillInstance * SkillBook::RemoveSkill( SkillID idSkill )
{
    SkillInstance * pSkillInstance = NULL;
    Bool bRemoved = m_mapSkills.Remove( idSkill, &pSkillInstance );
    if ( !bRemoved )
        return NULL;
    return pSkillInstance;
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
