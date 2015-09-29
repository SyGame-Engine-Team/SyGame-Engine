/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Gameplay/Talents.inl
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
// Talent implementation
inline UInt Talent::GetBranch() const {
    return m_iBranch;
}
inline UInt Talent::GetRow() const {
    return m_iRow;
}
inline UInt Talent::GetIndex() const {
    return m_iIndex;
}

inline SkillID Talent::GetSkillID() const {
    return m_idSkill;
}
inline UInt Talent::GetMaxPoints() const {
    return m_iMaxPoints;
}

inline UInt Talent::GetDependencyCount() const {
    return m_iDependencyCount;
}
inline Talent * Talent::GetDependency( UInt iIndex, UInt * outRequiredPoints ) const {
    Assert( iIndex < m_iDependencyCount );
    if ( outRequiredPoints != NULL )
        *outRequiredPoints = m_arrDependency[iIndex].iRequiredPoints;
    return m_arrDependency[iIndex].pRequired;
}

inline Void Talent::SetSlot( UInt iBranch, UInt iRow, UInt iIndex ) {
    m_iBranch = iBranch;
    m_iRow = iRow;
    m_iIndex = iIndex;
}
inline Void Talent::Setup( SkillID idSkill, UInt iMaxPoints ) {
    m_idSkill = idSkill;
    m_iMaxPoints = iMaxPoints;
}
inline Void Talent::MakeDependency( Talent * pRequired, UInt iRequiredPoints ) {
    Assert( m_iDependencyCount < TALENTTREE_MAX_DEPENDENCY );
    m_arrDependency[m_iDependencyCount].pRequired = pRequired;
    m_arrDependency[m_iDependencyCount].iRequiredPoints = iRequiredPoints;
}

/////////////////////////////////////////////////////////////////////////////////
// TalentTree implementation
inline CharClass TalentTree::GetClass() const {
    return m_iClass;
}

inline Talent * TalentTree::GetTalent( UInt iBranch, UInt iRow, UInt iIndex ) {
    Assert( iBranch < TALENTTREE_BRANCH_COUNT );
    Assert( iRow < TALENTTREE_ROW_COUNT );
    Assert( iIndex < TALENTTREE_ROW_SIZE );
    return ( m_arrTalentTree + (iBranch * TALENTTREE_BRANCH_SIZE) + (iRow * TALENTTREE_ROW_SIZE) + iIndex );
}

/////////////////////////////////////////////////////////////////////////////////
// TalentTreeInstance implementation
inline TalentTree * TalentTreeInstance::GetTalentTree() const {
    return m_pTalentTree;
}
inline BaseCharacter * TalentTreeInstance::GetOwner() const {
    return m_pOwner;
}

inline CharClass TalentTreeInstance::GetClass() const {
    return m_pTalentTree->GetClass();
}
inline const GChar * TalentTreeInstance::GetBranchName( UInt iBranch ) const {
    return m_pTalentTree->GetBranchName( iBranch );
}

inline const Talent * TalentTreeInstance::GetTalent( UInt iBranch, UInt iRow, UInt iIndex ) const {
    return m_pTalentTree->GetTalent( iBranch, iRow, iIndex );
}

inline UInt TalentTreeInstance::GetRemainingPoints() const {
    return m_iRemainingPoints;
}
inline UInt TalentTreeInstance::GetSpentPoints() const {
    return m_iSpentPoints;
}
inline UInt TalentTreeInstance::GetSpentPoints( UInt iBranch, UInt iRow, UInt iIndex ) const {
    Assert( iBranch < TALENTTREE_BRANCH_COUNT );
    Assert( iRow < TALENTTREE_ROW_COUNT );
    Assert( iIndex < TALENTTREE_ROW_SIZE );
    return *( m_arrSpentPoints + (iBranch * TALENTTREE_BRANCH_SIZE) + (iRow * TALENTTREE_ROW_SIZE) + iIndex );
}

inline Void TalentTreeInstance::AddPoint() {
    ++m_iRemainingPoints;
}


