/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Gameplay/GameplayManager.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Gameplay : GameplayManager
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// GameplayManager implementation
inline const GChar * GameplayManager::GetName() {
    return TEXT("GameplayManager");
}
inline const GChar * GameplayManager::GetMemoryBudget() {
    return TEXT (
"Heap:Scratch(4194304);"
    );
}

inline Scalar GameplayManager::RandUnit() {
    return ( _INV_RANDMAX * (Scalar)( RandFn->XorShift128() ) );
}
inline UInt GameplayManager::RandRange( UInt iMin, UInt iMax ) {
    Scalar fRatio = ( _INV_RANDMAX * (Scalar)( RandFn->XorShift128() ) );
    return iMin + (UInt)( MathFn->Floor( fRatio * (Scalar)(iMax - iMin) ) );
}

inline StatusEffect * GameplayManager::GetStatusEffect( StatusEffectID idStatusEffect ) const {
    Assert( idStatusEffect < STATUSEFFECTID_COUNT );
    return m_arrStatusEffects[idStatusEffect];
}

inline Skill * GameplayManager::GetSkill( SkillID idSkill ) const {
    Assert( idSkill < SKILLID_COUNT );
    return m_arrSkills[idSkill];
}


inline TalentTree * GameplayManager::GetTalentTree( CharClass iClass ) const {
    Assert( iClass < CHARCLASS_COUNT );
    return m_arrTalentTrees[iClass];
}

inline Item * GameplayManager::GetItem( ItemID idItem ) const {
    Assert( idItem < ITEMID_COUNT );
    return m_arrItems[idItem];
}

inline BaseCharacter * GameplayManager::GetCharacter( const GChar * strName ) const {
    _CharacterMap::Iterator itCharacter = m_mapCharacters.Get( strName );
    if ( itCharacter.IsNull() )
        return NULL;
    return itCharacter.GetItem();
}

inline Void GameplayManager::EnumCombatIDs() const {
    Assert( m_itEnumerate.IsNull() );
    m_itEnumerate = m_mapCombats.Begin();
}
inline CombatID GameplayManager::EnumNextCombatID() const {
    if ( m_itEnumerate.IsNull() )
        return COMBATID_INVALID;
    CombatID idCombat = m_itEnumerate.GetKey();
    ++m_itEnumerate;
    return idCombat;
}

inline UInt GameplayManager::GetCombatCharacterCount( CombatID idCombat ) const {
    _CombatMap::Iterator itCombat = m_mapCombats.Get( idCombat );
    if ( itCombat.IsNull() )
        return 0;
    return itCombat.GetItem().iCharacterCount;
}
inline BaseCharacter * GameplayManager::GetCombatCharacter( CombatID idCombat, UInt iIndex ) const {
    _CombatMap::Iterator itCombat = m_mapCombats.Get( idCombat );
    if ( itCombat.IsNull() )
        return NULL;
    Assert( iIndex < itCombat.GetItem().iCharacterCount );
    return itCombat.GetItem().arrCharacters[iIndex];
}

/////////////////////////////////////////////////////////////////////////////////

inline Int GameplayManager::_Compare_Strings( const GChar * const & rLeft, const GChar * const & rRight, Void * /*pUserData*/ ) {
    return StringFn->Cmp( rLeft, rRight );
}

inline Int GameplayManager::_Compare_CombatIDs( const CombatID & rLeft, const CombatID & rRight, Void * /*pUserData*/ ) {
    if ( rLeft < rRight )
        return +1;
    if ( rLeft > rRight )
        return -1;
    return 0;
}

inline CombatID GameplayManager::_GenerateCombatID() const {
    static CombatID s_idNextFree = 0;
    if ( s_idNextFree == COMBATID_INVALID )
        s_idNextFree = 0;
    return s_idNextFree++;
}


