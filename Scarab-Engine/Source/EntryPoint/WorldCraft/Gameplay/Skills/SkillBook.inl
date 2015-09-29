/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/Gameplay/Skills/SkillBook.inl
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
// SkillBook implementation
inline Bool SkillBook::HasSkill( SkillID iSkillID ) const {
    return m_mapSkills.Contains( iSkillID );
}
inline SkillInstance * SkillBook::GetSkill( SkillID iSkillID ) const {
    _SkillMap::Iterator itSkill = m_mapSkills.Get( iSkillID );
    if ( itSkill.IsNull() )
        return NULL;
    return itSkill.GetItem();
}

inline Void SkillBook::EnumSkills() const {
    Assert( m_itEnumerate.IsNull() );
    m_itEnumerate = m_mapSkills.Begin();
}
inline SkillInstance * SkillBook::EnumNextSkill() const {
    if ( m_itEnumerate.IsNull() )
        return NULL;
    SkillInstance * pSkillInstance = m_itEnumerate.GetItem();
    ++m_itEnumerate;
    return pSkillInstance;
}
