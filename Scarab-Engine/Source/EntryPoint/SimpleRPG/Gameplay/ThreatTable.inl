/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Gameplay/ThreatTable.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : SimpleRPG, Gameplay : ThreatTable
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// ThreatTable implementation
inline UInt ThreatTable::GetThreatRecordCount() const {
    return m_iRecordCount;
}
inline BaseCharacter * ThreatTable::GetThreatRecord( UInt iIndex, UInt * outThreat, Bool * outMark ) const {
    Assert( iIndex < m_iRecordCount );
    _ThreatRecord * pThreatRecord = m_arrRecords[THREATTABLE_ROOT + iIndex];
    if ( outThreat != NULL )
        *outThreat = pThreatRecord->iThreat;
    if ( outMark != NULL )
        *outMark = pThreatRecord->bMark;
    return pThreatRecord->pTarget;
}

inline Bool ThreatTable::HasThreat( BaseCharacter * pTarget ) const {
    return m_hThreatMap.Contains( pTarget );
}
inline UInt ThreatTable::GetThreat( BaseCharacter * pTarget ) const {
    _ThreatMap::Iterator itThreatRecord = m_hThreatMap.Get( pTarget );
    if ( itThreatRecord.IsNull() )
        return 0;
    return itThreatRecord.GetItem().iThreat;
}

inline BaseCharacter * ThreatTable::GetBiggestThreat( UInt * outThreat ) const {
    _ThreatRecord * pThreatRecord = m_arrRecords[THREATTABLE_ROOT];
    if ( pThreatRecord == NULL )
        return NULL;
    if ( outThreat != NULL )
        *outThreat = pThreatRecord->iThreat;
    return pThreatRecord->pTarget;
}

inline Void ThreatTable::ResetThreat() {
    m_iRecordCount = 0;
    m_arrRecords[THREATTABLE_ROOT] = NULL;
    m_hThreatMap.Clear();
}

inline Bool ThreatTable::HasMark( BaseCharacter * pCharacter ) const {
    _ThreatMap::Iterator itThreatRecord = m_hThreatMap.Get( pCharacter );
    if ( itThreatRecord.IsNull() )
        return false;
    return itThreatRecord.GetItem().bMark;
}

/////////////////////////////////////////////////////////////////////////////////

inline ThreatTable::_ThreatRecord & ThreatTable::_ThreatRecord::operator=( const _ThreatRecord & rhs ) {
    pTarget = rhs.pTarget;
    iThreat = rhs.iThreat;
    iNode = rhs.iNode;
    return (*this);
}

inline Int ThreatTable::_Compare_BaseChars( BaseCharacter * const & rLeft, BaseCharacter * const & rRight, Void * /*pUserData*/ ) {
    if ( rLeft < rRight )
        return +1;
    if ( rLeft > rRight )
        return -1;
    return 0;
}

