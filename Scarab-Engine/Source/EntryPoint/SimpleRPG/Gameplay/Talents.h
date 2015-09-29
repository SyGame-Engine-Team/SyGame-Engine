/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Gameplay/Talents.h
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
// Header prelude
#ifndef SCARAB_ENTRYPOINT_SIMPLERPG_GAMEPLAY_TALENTS_H
#define SCARAB_ENTRYPOINT_SIMPLERPG_GAMEPLAY_TALENTS_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Skills.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define TALENTTREE_ROW_COUNT    4
#define TALENTTREE_ROW_SIZE     4
#define TALENTTREE_BRANCH_COUNT 3
#define TALENTTREE_BRANCH_SIZE  ( TALENTTREE_ROW_COUNT * TALENTTREE_ROW_SIZE )

#define TALENTTREE_MAX_DEPENDENCY 4

#define TALENTTREE_ROW_ACCESS_POINTS 5

// Prototypes
enum CharClass;

class TalentTreeInstance;

/////////////////////////////////////////////////////////////////////////////////
// The Talent class
class Talent
{
public:
    Talent();
    virtual ~Talent();

    // Getters
    inline UInt GetBranch() const;
    inline UInt GetRow() const;
    inline UInt GetIndex() const;

    inline SkillID GetSkillID() const;
    inline UInt GetMaxPoints() const;

    inline UInt GetDependencyCount() const;
    inline Talent * GetDependency( UInt iIndex, UInt * outRequiredPoints ) const;

    // Methods
    inline Void SetSlot( UInt iBranch, UInt iRow, UInt iIndex );
    inline Void Setup( SkillID idSkill, UInt iMaxPoints );
    inline Void MakeDependency( Talent * pRequired, UInt iRequiredPoints );

protected:
    UInt m_iBranch;
    UInt m_iRow;
    UInt m_iIndex;

    SkillID m_idSkill;
    UInt m_iMaxPoints;

    typedef struct _dependency {
        Talent * pRequired;
        UInt iRequiredPoints;
    } Dependency;

    UInt m_iDependencyCount;
    Dependency m_arrDependency[TALENTTREE_MAX_DEPENDENCY];
};

/////////////////////////////////////////////////////////////////////////////////
// The TalentTree class
class TalentTree
{
public:
    TalentTree( CharClass iClass );
    virtual ~TalentTree();

    // Getters
    inline CharClass GetClass() const;
    virtual const GChar * GetBranchName( UInt iBranch ) const = 0;

    inline Talent * GetTalent( UInt iBranch, UInt iRow, UInt iIndex );

    // Methods
    Talent * MakeTalent( UInt iBranch, UInt iRow, UInt iIndex, SkillID idSkill, UInt iMaxPoints );

    Void GenerateInstance( TalentTreeInstance * outTalentTreeInstance, BaseCharacter * pOwner, UInt iPoints );

protected:
    CharClass m_iClass;

    Talent m_arrTalentTree[TALENTTREE_BRANCH_COUNT * TALENTTREE_BRANCH_SIZE];
};

/////////////////////////////////////////////////////////////////////////////////
// The TalentTreeInstance class
class TalentTreeInstance
{
public:
    TalentTreeInstance();
    virtual ~TalentTreeInstance();

    // Getters
    inline TalentTree * GetTalentTree() const;
    inline BaseCharacter * GetOwner() const;

    inline CharClass GetClass() const;
    inline const GChar * GetBranchName( UInt iBranch ) const;

    inline const Talent * GetTalent( UInt iBranch, UInt iRow, UInt iIndex ) const;

    inline UInt GetRemainingPoints() const;
    inline UInt GetSpentPoints() const;
    inline UInt GetSpentPoints( UInt iBranch, UInt iRow, UInt iIndex ) const;

    // Methods
    inline Void AddPoint();
    Bool SpendPoint( UInt iBranch, UInt iRow, UInt iIndex );
    Void ResetPoints();

protected:
    friend class TalentTree;

    TalentTree * m_pTalentTree;
    BaseCharacter * m_pOwner;

    UInt m_iRemainingPoints;
    UInt m_iSpentPoints;
    UInt m_arrSpentPoints[TALENTTREE_BRANCH_COUNT * TALENTTREE_BRANCH_SIZE];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Talents.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_SIMPLERPG_GAMEPLAY_TALENTS_H

