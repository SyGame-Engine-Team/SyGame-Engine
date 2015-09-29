/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/SimpleRPG/Gameplay/ThreatTable.cpp
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
// Includes
#include "ThreatTable.h"

#include "GameplayManager.h"

/////////////////////////////////////////////////////////////////////////////////
// ThreatTable implementation
ThreatTable::ThreatTable():
    m_hThreatMap()
{
    m_iRecordCount = 0;

    for( UInt i = 0; i <= THREATTABLE_MAX_RECORDS; ++i )
        m_arrRecords[i] = NULL;

    m_hThreatMap.SetComparator( _Compare_BaseChars, NULL );
    m_hThreatMap.UseMemoryContext( GameplayFn->GetMemoryContext(), TEXT("Scratch") );
    m_hThreatMap.Create();
}
ThreatTable::~ThreatTable()
{
    m_hThreatMap.Destroy();
}

UInt ThreatTable::AddThreat( BaseCharacter * pCharacter, UInt iThreat, Bool bMark )
{
    _ThreatMap::Iterator itThreatRecord = m_hThreatMap.Get( pCharacter );

    // New record case
    if ( itThreatRecord.IsNull() ) {
        _ThreatRecord hNewRecord;
        hNewRecord.pTarget = pCharacter;
        hNewRecord.iThreat = iThreat;
        hNewRecord.iNode = INVALID_OFFSET;
        hNewRecord.bMark = bMark;

        Bool bInserted = m_hThreatMap.Insert( pCharacter, hNewRecord, &itThreatRecord );
        Assert( bInserted && !(itThreatRecord.IsNull()) );

        _Heap_Insert( itThreatRecord.GetItemPtr() );

        return iThreat;
    }

    // Update record case
    _ThreatRecord * pThreatRecord = itThreatRecord.GetItemPtr();

    pThreatRecord->iThreat += iThreat;
    if ( bMark )
        pThreatRecord->bMark = true;
    _Heap_UpdateUp( pThreatRecord->iNode );

    return pThreatRecord->iThreat;
}
UInt ThreatTable::RemoveThreat( BaseCharacter * pCharacter, UInt iThreat, Bool bMark )
{
    _ThreatMap::Iterator itThreatRecord = m_hThreatMap.Get( pCharacter );

    // No record case
    if ( itThreatRecord.IsNull() )
        return 0;

    // Update record case
    _ThreatRecord * pThreatRecord = itThreatRecord.GetItemPtr();

    if ( iThreat < pThreatRecord->iThreat ) {
        pThreatRecord->iThreat -= iThreat;
        pThreatRecord->bMark = bMark;
        _Heap_UpdateDown( pThreatRecord->iNode );

        return pThreatRecord->iThreat;
    }

    // Remove record case
    pThreatRecord->iThreat = 0;
    _Heap_Remove( pThreatRecord->iNode );

    Bool bRemoved = m_hThreatMap.Remove( pThreatRecord->pTarget );
    Assert( bRemoved );

    return 0;
}
Void ThreatTable::ResetMarks()
{
    for( UInt i = THREATTABLE_ROOT; i < m_iRecordCount; ++i )
        m_arrRecords[i]->bMark = false;
}

/////////////////////////////////////////////////////////////////////////////////

Void ThreatTable::_Heap_Insert( _ThreatRecord * pThreatRecord )
{
	++m_iRecordCount;
    pThreatRecord->iNode = m_iRecordCount;
	m_arrRecords[m_iRecordCount] = pThreatRecord;

	_Heap_UpdateUp( m_iRecordCount );
}
Void ThreatTable::_Heap_Remove( UInt iNode )
{
    if ( m_iRecordCount == 1 ) {
        Assert( iNode == THREATTABLE_ROOT );
		m_iRecordCount = 0;
	    m_arrRecords[THREATTABLE_ROOT] = NULL;
		return;
	}

	m_arrRecords[iNode] = m_arrRecords[m_iRecordCount];
    m_arrRecords[iNode]->iNode = iNode;
    m_arrRecords[m_iRecordCount] = NULL;
	--m_iRecordCount;

    _Heap_UpdateDown( iNode );
}
Void ThreatTable::_Heap_UpdateUp( UInt iNode )
{
    if ( iNode == THREATTABLE_ROOT )
        return;

    UInt iParent = ( iNode >> 1 );

    if ( m_arrRecords[iNode]->iThreat <= m_arrRecords[iParent]->iThreat )
        return;

    Swap<_ThreatRecord*>( m_arrRecords + iNode, m_arrRecords + iParent );
    m_arrRecords[iNode]->iNode = iNode;
    m_arrRecords[iParent]->iNode = iParent;

    _Heap_UpdateUp( iParent );
}
Void ThreatTable::_Heap_UpdateDown( UInt iNode )
{
    if ( iNode > (m_iRecordCount >> 1) )
        return;

    UInt iLeftChild = ( iNode << 1 );
    UInt iRightChild = iLeftChild + 1;
    UInt iMaxChild;

    if ( iRightChild > m_iRecordCount )
        iMaxChild = iLeftChild;
    else
        iMaxChild = ( m_arrRecords[iLeftChild]->iThreat >= m_arrRecords[iRightChild]->iThreat ) ? iLeftChild : iRightChild;

    if ( m_arrRecords[iNode]->iThreat >= m_arrRecords[iMaxChild]->iThreat )
        return;

    Swap<_ThreatRecord*>( m_arrRecords + iNode, m_arrRecords + iMaxChild );
    m_arrRecords[iNode]->iNode = iNode;
    m_arrRecords[iMaxChild]->iNode = iMaxChild;

    _Heap_UpdateDown( iMaxChild );
}

