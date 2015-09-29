/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Interface/SkillBar.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Interface : SkillBar
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// SkillBarModel implementation
inline UInt SkillBarModel::GetRowCount() const {
    return m_iRowCount;
}
inline UInt SkillBarModel::GetSlotsPerRow() const {
    return m_iSlotsPerRow;
}
inline UInt SkillBarModel::GetSlotCount() const {
    return m_iSlotCount;
}

inline SkillID SkillBarModel::GetSlot( UInt iSlot ) const {
    Assert( iSlot < m_iSlotCount );
    return m_arrSlots[iSlot];
}
inline Void SkillBarModel::SetSlot( UInt iSlot, SkillID idSkill ) {
    Assert( iSlot < m_iSlotCount );
    m_arrSlots[iSlot] = idSkill;
}

inline Bool SkillBarModel::IsOnGlobalCooldown() const {
    return m_pCharacter->IsOnGlobalCooldown();
}
inline Scalar SkillBarModel::GetGlobalCooldown() const {
    return m_pCharacter->GetGlobalCooldown();
}
inline Bool SkillBarModel::IsOnCooldown( SkillID idSkill ) const {
    return m_pCharacter->IsOnCooldown( idSkill );
}
inline Scalar SkillBarModel::GetCooldown( SkillID idSkill ) const {
    return m_pCharacter->GetCooldown( idSkill );
}

inline Bool SkillBarModel::ShowKeyBindings() const {
    return m_bShowKeyBindings;
}
inline Void SkillBarModel::SetShowKeyBindings( Bool bShowKeyBindings ) {
    m_bShowKeyBindings = bShowKeyBindings;
}

/////////////////////////////////////////////////////////////////////////////////
// SkillBar implementation

