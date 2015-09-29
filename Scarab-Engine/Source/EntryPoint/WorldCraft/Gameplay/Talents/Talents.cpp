/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Gameplay/Talents.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Gameplay : Talents
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
#include "Talents.h"

#include "GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Talent implementation
Talent::Talent()
{
    m_iBranch = 0;
    m_iRow = 0;
    m_iIndex = 0;

    m_idSkill = SKILLID_UNDEFINED;
    m_iMaxPoints = 0;

    m_iDependencyCount = 0;
    for( UInt i = 0; i < TALENTTREE_MAX_DEPENDENCY; ++i ) {
        m_arrDependency[i].pRequired = NULL;
        m_arrDependency[i].iRequiredPoints = 0;
    }
}
Talent::~Talent()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// TalentTree implementation
TalentTree::TalentTree( CharClass iClass )
{
    m_iClass = iClass;

    for( UInt i = 0; i < TALENTTREE_BRANCH_COUNT; ++i ) {
        Talent * pBranch = ( m_arrTalentTree + (i * TALENTTREE_BRANCH_SIZE) );
        for( UInt j = 0; j < TALENTTREE_ROW_COUNT; ++j ) {
            Talent * pRow = ( pBranch + (j * TALENTTREE_ROW_SIZE) );
            for( UInt k = 0; k < TALENTTREE_ROW_SIZE; ++k ) {
                Talent * pTalent = ( pRow + k );
                pTalent->SetSlot( i, j, k );
            }
        }
    }
}
TalentTree::~TalentTree()
{
    // nothing to do
}

Talent * TalentTree::MakeTalent( UInt iBranch, UInt iRow, UInt iIndex, SkillID idSkill, UInt iMaxPoints )
{
    Assert( iBranch < TALENTTREE_BRANCH_COUNT );
    Assert( iRow < TALENTTREE_ROW_COUNT );
    Assert( iIndex < TALENTTREE_ROW_SIZE );
    Talent * pTalent = ( m_arrTalentTree + (iBranch * TALENTTREE_BRANCH_SIZE) + (iRow * TALENTTREE_ROW_SIZE) + iIndex );

    pTalent->Setup( idSkill, iMaxPoints );

    return pTalent;
}

Void TalentTree::GenerateInstance( TalentTreeInstance * outTalentTreeInstance, BaseCharacter * pOwner, UInt iPoints )
{
    outTalentTreeInstance->m_pTalentTree = this;
    outTalentTreeInstance->m_pOwner = pOwner;

    outTalentTreeInstance->m_iRemainingPoints = iPoints;
    outTalentTreeInstance->m_iSpentPoints = 0;

    UInt iCount = ( TALENTTREE_BRANCH_COUNT * TALENTTREE_BRANCH_SIZE );
    for( UInt i = 0; i < iCount; ++i )
        outTalentTreeInstance->m_arrSpentPoints[i] = 0;
}

/////////////////////////////////////////////////////////////////////////////////
// TalentTreeInstance implementation
TalentTreeInstance::TalentTreeInstance()
{
    m_pTalentTree = NULL;
    m_pOwner = NULL;

    m_iRemainingPoints = 0;
    m_iSpentPoints = 0;

    UInt iCount = ( TALENTTREE_BRANCH_COUNT * TALENTTREE_BRANCH_SIZE );
    for( UInt i = 0; i < iCount; ++i )
        m_arrSpentPoints[i] = 0;
}
TalentTreeInstance::~TalentTreeInstance()
{
    // nothing to do
}

Bool TalentTreeInstance::SpendPoint( UInt iBranch, UInt iRow, UInt iIndex )
{
    Assert( iBranch < TALENTTREE_BRANCH_COUNT );
    Assert( iRow < TALENTTREE_ROW_COUNT );
    Assert( iIndex < TALENTTREE_ROW_SIZE );

    Talent * pTalent = m_pTalentTree->GetTalent( iBranch, iRow, iIndex );
    UInt * pSpent = ( m_arrSpentPoints + (iBranch * TALENTTREE_BRANCH_SIZE) + (iRow * TALENTTREE_ROW_SIZE) + iIndex );

    // Check talent existence
    if ( pTalent->GetSkillID() == SKILLID_UNDEFINED )
        return false;

    // Check if maxed already
    if ( *pSpent >= pTalent->GetMaxPoints() )
        return false;

    // Check available points
    if ( m_iRemainingPoints == 0 )
        return false;

    // Check row access
    if ( iRow > 0 ) {
        UInt * pPrevRow = ( m_arrSpentPoints + (iBranch * TALENTTREE_BRANCH_SIZE) + ((iRow - 1) * TALENTTREE_ROW_SIZE) );
        UInt iPointsSpent = 0;
        for( UInt i = 0; i < TALENTTREE_ROW_SIZE; ++i )
            iPointsSpent += pPrevRow[i];
        if ( iPointsSpent < TALENTTREE_ROW_ACCESS_POINTS )
            return false;
    }

    // Check dependencies
    UInt iDependencyCount = pTalent->GetDependencyCount();
    for( UInt i = 0; i < iDependencyCount; ++i ) {
        UInt iRequiredPoints = 0;
        Talent * pRequiredTalent = pTalent->GetDependency( i, &iRequiredPoints );
        UInt iPointsSpent = GetSpentPoints( pRequiredTalent->GetBranch(), pRequiredTalent->GetRow(), pRequiredTalent->GetIndex() );
        if ( iPointsSpent < iRequiredPoints )
            return false;
    }

    // Spend the point
    --m_iRemainingPoints;
    ++m_iSpentPoints;
    ++(*pSpent);
    return true;
}
Void TalentTreeInstance::ResetPoints()
{
    m_iRemainingPoints += m_iSpentPoints;
    m_iSpentPoints = 0;

    UInt iCount = ( TALENTTREE_BRANCH_COUNT * TALENTTREE_BRANCH_SIZE );
    for( UInt i = 0; i < iCount; ++i )
        m_arrSpentPoints[i] = 0;
}



