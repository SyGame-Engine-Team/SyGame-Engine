/////////////////////////////////////////////////////////////////////////////////
// File : Lib/String/PatternMatching.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Almost all pattern-matching algorithms ...
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// PatternMatching implementation
inline PatternMatching * PatternMatching::GetInstancePtr() {
    static PatternMatching s_Instance;
    return &s_Instance;
}

inline Void PatternMatching::EnterChain() {
    if (m_bKeepMemory)
        return;
    m_bKeepMemory = true;
}
inline Void PatternMatching::LeaveChain() {
    if ( !m_bKeepMemory )
        return;
    _Free(); // "if (m_bMemoryFilled)" not needed,
             // double-free is harmless for us
    m_bMemoryFilled = false;
    m_bKeepMemory = false;
}

/////////////////////////////////////////////////////////////////////////////////

inline Byte * PatternMatching::_Break(UInt iSize) {
    Byte * pAllocated = m_pBreak;
    m_pBreak += iSize;
    if ( m_pBreak > m_pLastBreak ) {
        m_pBreak -= iSize;
        return NULL; // must never happen, tweak memory size !
    }
    return pAllocated;
}
inline Void PatternMatching::_Unbreak(UInt iSize) {
    m_pBreak -= iSize;
    if (m_pBreak < m_pFirstBreak)
        m_pBreak = m_pFirstBreak;
}
inline Void PatternMatching::_Free() {
    m_pBreak = m_pFirstBreak;
}