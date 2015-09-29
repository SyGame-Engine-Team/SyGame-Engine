/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Interface/CharacterView.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Interface : CharacterView
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// CharacterViewModel implementation
inline const GChar * CharacterViewModel::GetName() const {
    return m_pCharacter->GetName();
}
inline CharClass CharacterViewModel::GetClass() const {
    return m_pCharacter->GetClass();
}

inline UInt CharacterViewModel::GetLevel() const {
    return m_pCharacter->GetLevel();
}
inline UInt CharacterViewModel::GetCurrentXP() const {
    return m_pCharacter->GetCurrentXP();
}
inline UInt CharacterViewModel::GetMaxXP() const {
    return m_pCharacter->GetMaxXP();
}

inline UInt CharacterViewModel::GetCurrentHP() const {
    return m_pCharacter->GetCurrentHP();
}
inline UInt CharacterViewModel::GetMaxHP() const {
    return m_pCharacter->GetMaxHP();
}
inline UInt CharacterViewModel::GetCurrentMP() const {
    return m_pCharacter->GetCurrentMP();
}
inline UInt CharacterViewModel::GetMaxMP() const {
    return m_pCharacter->GetMaxMP();
}
inline Scalar CharacterViewModel::GetMovementSpeed() const {
    return m_pCharacter->GetMovementSpeed();
}

inline Scalar CharacterViewModel::GetStat( CharStatistic iStat ) const {
    return m_pCharacter->GetFinalStat( iStat );
}
inline Scalar CharacterViewModel::GetStatBonus( CharStatistic iStat ) const {
    return m_pCharacter->GetActiveStatModifier( iStat );
}

/////////////////////////////////////////////////////////////////////////////////
// CharacterView implementation
