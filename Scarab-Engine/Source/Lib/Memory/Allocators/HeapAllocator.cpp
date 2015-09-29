/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Memory/Allocators/HeapAllocator.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : The "Full-English-Breakfast" binheap-allocator ...
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "HeapAllocator.h"

/////////////////////////////////////////////////////////////////////////////////
// HeapAllocator implementation
HeapAllocator::HeapAllocator( UInt iContextID, const GChar * strContextName,
                              UInt iAllocatorID, const GChar * strAllocatorName ):
    MemoryAllocator( iContextID, strContextName, iAllocatorID, strAllocatorName ),
    AlignUnit( HEAPMEMORY_ALIGN_NATIVE ), AlignUnitShift( HEAPMEMORY_ALIGN_SHIFT_NATIVE ),
    AUFreeMask( 0x80000000 ), AUSizeMask( 0x7fffffff ),
	ChunkHeadAUSize( _AU_ConvertSize( sizeof(ChunkHead) ) ),
    ChunkHeapNodeAUSize( _AU_ConvertSize( sizeof(ChunkHeapNode) ) ),
	ChunkListNodeAUSize( _AU_ConvertSize( sizeof(ChunkListNode) ) ),
	DummyChunkAUSize( ChunkHeadAUSize ), DummyChunkByteSize( DummyChunkAUSize * AlignUnit ), 
	MinimalChunkAUSize( ChunkHeadAUSize + ChunkHeapNodeAUSize )
{
    m_pBinHeapRoot = NULL;
    m_iHeightChange = BINHEAP_HEIGHT_NOCHANGE;

    m_pLastFreed = NULL;

    m_pHeapMemory = NULL;
    m_iHeapSize = 0;
    m_iTotalFree = 0;
}
HeapAllocator::~HeapAllocator()
{
    if ( m_pHeapMemory != NULL )
        SystemFn->MemFree( m_pHeapMemory );
}

Void HeapAllocator::Initialize( UInt iHeapSize )
{
    Assert( m_pHeapMemory == NULL );

    // Align size, set whole memory to 0
    m_iHeapSize = iHeapSize - ( iHeapSize & (AlignUnit - 1) );
    m_iTotalFree = ( m_iHeapSize - DummyChunkByteSize );
	m_pHeapMemory = (Byte*)SystemFn->MemAlloc( m_iHeapSize );
	MemZero( m_pHeapMemory, m_iHeapSize );

    // Initialize BinHeap
    m_pBinHeapRoot = NULL;
    m_iHeightChange = BINHEAP_HEIGHT_NOCHANGE;

    m_pLastFreed = NULL;

	// Simple & dumb, should make a basic partition with size-hierarchic chunks ...
	// That should need another tricky system to avoid coalescing those ones ...
	ChunkHead * pNewBin = (ChunkHead*)m_pHeapMemory;
	pNewBin->thisAUSize = _AU_ConvertSize( m_iHeapSize - DummyChunkByteSize );
	pNewBin->prevAUSize = 0;
	_Chunk_MarkFree( pNewBin );
    _rec_Insert( &m_pBinHeapRoot, m_iHeightChange, pNewBin );

	// Add Dummy
	ChunkHead * pDummy = (ChunkHead*)( m_pHeapMemory + m_iHeapSize - DummyChunkByteSize );
	pDummy->thisAUSize = DummyChunkAUSize;
	pDummy->prevAUSize = pNewBin->thisAUSize; // Notice this is dependant from above
}
Void HeapAllocator::Cleanup()
{
    Assert( m_pHeapMemory != NULL );

    m_pBinHeapRoot = NULL;
    m_iHeightChange = BINHEAP_HEIGHT_NOCHANGE;

    m_pLastFreed = NULL;

    SystemFn->MemFree( m_pHeapMemory );

    m_pHeapMemory = NULL;
    m_iHeapSize = 0;
    m_iTotalFree = 0;    
}

Byte * HeapAllocator::Allocate( UInt iSize )
{
    Assert( m_pHeapMemory != NULL );

	UInt iAUSize = _AU_ConvertSize(iSize);
    if ( iAUSize < ChunkHeapNodeAUSize )
        iAUSize = ChunkHeapNodeAUSize;
	iAUSize += ChunkHeadAUSize; // Total Size to alloc in AUs
    ChunkHead * pHead = NULL;

    // Allocation strategy
    if ( m_pLastFreed != NULL ) {
        // Try to re-use last block directly
        if ( _Chunk_Size(m_pLastFreed) >= iAUSize ) {
            pHead = m_pLastFreed;
        } else {
            // Try to re-use the coalesced block
            ChunkHead * pFreed = _PerformCoalescing( m_pLastFreed );
            if ( _Chunk_Size(pFreed) >= iAUSize )
				pHead = pFreed;
            else {
                // Finish coalescing
                _BinHeap_ReleaseChunk( pFreed );
                // Pick another chunk
                pHead = _BinHeap_RequestChunk( iAUSize );
            }
        }
        m_pLastFreed = NULL;
    } else {
        // Pick a chunk
        pHead = _BinHeap_RequestChunk( iAUSize );
    }

    if ( pHead == NULL )
        return NULL;

	// Split chunk to fit requested size
	UInt iRemain = ( _Chunk_Size(pHead) - iAUSize );
	if ( iRemain >= MinimalChunkAUSize ) {
		ChunkHead * pRemain = (ChunkHead*)_AU_Next( (Byte*)pHead, iAUSize );
		ChunkHead * pNext = (ChunkHead*)_AU_Next( (Byte*)pHead, _Chunk_Size(pHead) );
		pHead->thisAUSize = iAUSize;
		pRemain->prevAUSize = iAUSize;
		pRemain->thisAUSize = iRemain;
		pNext->prevAUSize = iRemain;
		_Chunk_MarkFree( pRemain );
        _BinHeap_ReleaseChunk( pRemain );
	}

	m_iTotalFree -= ( _Chunk_Size(pHead) * AlignUnit );
	_Chunk_MarkAllocated( pHead );
	return (Byte*)( _Chunk_GetHeapNode(pHead) );
}
Void HeapAllocator::Free( Byte * pMemory )
{
    Assert( m_pHeapMemory != NULL );
    Assert( pMemory != NULL );

    ChunkListNode * pListNode = (ChunkListNode*)pMemory;
	ChunkHead * pHead = _Chunk_GetHead( pListNode );

    Assert( _Chunk_IsAllocated(pHead) );

    // Use deferred coalescing
	if ( m_pLastFreed != NULL ) {
		ChunkHead * pFreed = _PerformCoalescing( m_pLastFreed );
        _Chunk_MarkFree( pFreed );
		_BinHeap_ReleaseChunk( pFreed );
	}

	m_iTotalFree += ( _Chunk_Size(pHead) * AlignUnit );
    _Chunk_MarkFree( pHead );
	m_pLastFreed = pHead;
}

Void HeapAllocator::GenerateReport( AllocatorReport * outReport ) const
{
    static ChunkHeapNode * s_arrFIFO[HEAPREPORT_MAX_TREESPAN];
    static Byte * s_ScratchMemory1[HEAPREPORT_MAX_TREESPAN];
    static Int s_ScratchMemory2[HEAPREPORT_MAX_TREESPAN];
    static UInt s_ScratchMemory3[HEAPREPORT_MAX_TREESPAN];
    static Byte ** s_ScratchMemory4[HEAPREPORT_MAX_TREESPAN * HEAPREPORT_MAX_LISTSIZE];
    static Byte * s_ScratchMemory5[HEAPREPORT_MAX_CHUNKS];
    static UInt s_ScratchMemory6[HEAPREPORT_MAX_CHUNKS];
    static UInt s_ScratchMemory7[HEAPREPORT_MAX_CHUNKS];

    Assert( outReport != NULL );
    HeapReport * outHeapReport = (HeapReport*)outReport;
    UInt iIndex;

    outHeapReport->idMemoryContext = m_idContext;
    outHeapReport->strContextName = m_strContextName;
    outHeapReport->idMemoryAllocator = m_idAllocator;
    outHeapReport->strAllocatorName = m_strAllocatorName;
    outHeapReport->pBaseAddress = m_pHeapMemory;
    outHeapReport->iTotalSize = m_iHeapSize;
    outHeapReport->iAllocatedSize = ( m_iHeapSize - m_iTotalFree );
    outHeapReport->iFreeSize = m_iTotalFree;
    outHeapReport->pLastFreed = (Byte*)m_pLastFreed;

    // Extract BinHeap's Structure : Breadth-First traversal
    outHeapReport->iBinHeapSize = 0;
    outHeapReport->arrHeapNodes = s_ScratchMemory1;
    outHeapReport->arrBalances = s_ScratchMemory2;
    outHeapReport->arrListSizes = s_ScratchMemory3;
    outHeapReport->arrListNodes = s_ScratchMemory4;
    s_arrFIFO[0] = m_pBinHeapRoot;
    UInt iRead = 0, iWrite = 1;
    ChunkHeapNode * pNode;
    iIndex = 0;
    while( iRead != iWrite ) {
        pNode = s_arrFIFO[iRead++];
        if ( iRead >= HEAPREPORT_MAX_TREESPAN )
            iRead -= HEAPREPORT_MAX_TREESPAN;

        // Update binheap size
        ++(outHeapReport->iBinHeapSize);

        // leave holes for empty leaves (at the deepest level)
        if ( pNode == NULL ) {
            outHeapReport->arrHeapNodes[iIndex] = NULL;
            outHeapReport->arrBalances[iIndex] = 0;
            outHeapReport->arrListSizes[iIndex] = 0;
            ++iIndex;
            continue;
        }

        // record heap node
        outHeapReport->arrHeapNodes[iIndex] = (Byte*)( _Chunk_GetHead(pNode) );
        outHeapReport->arrBalances[iIndex] = pNode->iBalanceFactor;

        // record list nodes
        UInt iCount = 0;
        ChunkListNode * pListNode = pNode->pNext;
        while ( pListNode != NULL ) {
            outHeapReport->arrListNodes[iIndex][iCount] = (Byte*)( _Chunk_GetHead(pListNode) );
            ++iCount;
            pListNode = pListNode->pNext;
        }
        outHeapReport->arrListSizes[iIndex] = iCount;

        ++iIndex;
        s_arrFIFO[iWrite++] = pNode->pChild[BINHEAP_CHILD_LEFT];
        if ( iWrite >= HEAPREPORT_MAX_TREESPAN )
            iWrite -= HEAPREPORT_MAX_TREESPAN;
        s_arrFIFO[iWrite++] = pNode->pChild[BINHEAP_CHILD_RIGHT];
        if ( iWrite >= HEAPREPORT_MAX_TREESPAN )
            iWrite -= HEAPREPORT_MAX_TREESPAN;
    }

    // Extract ChunkMap's Structure
    outHeapReport->iChunkMapSize = 0;
    outHeapReport->arrChunkMap = s_ScratchMemory5;
    outHeapReport->arrPrevSizes = s_ScratchMemory6;
    outHeapReport->arrSizes = s_ScratchMemory7;
    ChunkHead * pChunk = (ChunkHead*)m_pHeapMemory;
    ChunkHead * pHeapEnd = (ChunkHead*)( m_pHeapMemory + m_iHeapSize );
    iIndex = 0;
    while( pChunk < pHeapEnd ) {
        // Update binheap size
        ++(outHeapReport->iChunkMapSize);

        // record chunk
        outHeapReport->arrChunkMap[iIndex] = (Byte*)pChunk;
        outHeapReport->arrPrevSizes[iIndex] = _Chunk_PrevSize( pChunk ) * AlignUnit;
        outHeapReport->arrSizes[iIndex] = _Chunk_Size( pChunk ) * AlignUnit;

        ++iIndex;
        pChunk = (ChunkHead*)_AU_Next( (Byte*)pChunk, _Chunk_Size(pChunk) );
    }

}
Void HeapAllocator::LogReport( const AllocatorReport * pReport ) const
{
    Assert( pReport != NULL );
    const HeapReport * pHeapReport = (const HeapReport*)pReport;
    UInt i, j;

    HFile logFile = SystemFn->OpenFile( HEAPREPORT_LOGFILE, FILE_WRITE );
    Assert( logFile.IsValid() );
    Bool bOk = logFile.Seek( FILE_SEEK_END, 0 );
    Assert( bOk );

    ErrorFn->Log( logFile, TEXT("Heap Report :") ); // Add Timestamps ? maybe usefull later ...

    ErrorFn->Log( logFile, TEXT("\n => Memory Context ID     : %ud"),  pHeapReport->idMemoryContext );
    ErrorFn->Log( logFile, TEXT("\n => Memory Context Name   : %s"),   pHeapReport->strContextName );
    ErrorFn->Log( logFile, TEXT("\n => Memory Allocator ID   : %ud"),  pHeapReport->idMemoryAllocator );
    ErrorFn->Log( logFile, TEXT("\n => Memory Allocator Name : %s"),   pHeapReport->strAllocatorName );
    ErrorFn->Log( logFile, TEXT("\n => Base Address          : %u8x"), pHeapReport->pBaseAddress );
    ErrorFn->Log( logFile, TEXT("\n => Total size            : %ud"),  pHeapReport->iTotalSize );
    ErrorFn->Log( logFile, TEXT("\n => Allocated size        : %ud"),  pHeapReport->iAllocatedSize );
    ErrorFn->Log( logFile, TEXT("\n => Free size             : %ud"),  pHeapReport->iFreeSize );
    ErrorFn->Log( logFile, TEXT("\n => LastFreed pointer     : %u8x"), pHeapReport->pLastFreed );

    // BinHeap
    ErrorFn->Log( logFile, TEXT("\n => BinHeap Structure (Breadth-First, Span size = %ud) :"), pHeapReport->iBinHeapSize );
    for( i = 0; i < pHeapReport->iBinHeapSize; ++i ) {
        ErrorFn->Log( logFile, TEXT("\n\t - HeapNode (%u8x) : Balance = %d, ListSize = %ud"),
                      pHeapReport->arrHeapNodes[i], pHeapReport->arrBalances[i], pHeapReport->arrListSizes[i] );
        ErrorFn->Log( logFile, TEXT("\n\t   -> ListNodes :") );
        for( j = 0; j < pHeapReport->arrListSizes[i]; ++j )
            ErrorFn->Log( logFile, TEXT(" %u8x"), pHeapReport->arrListNodes[i][j] );
    }

    // ChunkMap
    ErrorFn->Log( logFile, TEXT("\n => ChunkMap Structure (Starting from BaseAddress, Chunk count = %ud) :"), pHeapReport->iChunkMapSize );
    for( i = 0; i < pHeapReport->iChunkMapSize; ++i ) {
        ErrorFn->Log ( logFile, TEXT("\n\t - Chunk (%u8x) : PrevSize = %ud, Size = %ud"),
                       pHeapReport->arrChunkMap[i], pHeapReport->arrPrevSizes[i], pHeapReport->arrSizes[i] );
        if ( pHeapReport->arrChunkMap[i] == pHeapReport->pLastFreed )
            ErrorFn->Log( logFile, TEXT(" (LastFreed)") );
    }

    ErrorFn->Log( logFile, TEXT("\n\n") );

    logFile.Close();
}

/////////////////////////////////////////////////////////////////////////////////

ChunkHead * HeapAllocator::_BinHeap_RequestChunk( UInt iMinSize )
{
    ChunkHeapNode * pHeapNode = _Search( iMinSize );
    if ( pHeapNode == NULL )
        return NULL;

    ChunkHead * pChunk = _Chunk_GetHead( pHeapNode );

    if ( pHeapNode->pNext == NULL )
		_rec_Remove( &m_pBinHeapRoot, m_iHeightChange, pChunk );
	else {
        ChunkListNode * pListNode = pHeapNode->pNext;
        pChunk = _Chunk_GetHead( pListNode);
        pHeapNode->pNext = pListNode->pNext;
        if ( pListNode->pNext != NULL )
            pListNode->pNext->pPrev = (ChunkListNode*)pHeapNode;
    }

    return pChunk;
}
Void HeapAllocator::_BinHeap_ReleaseChunk( ChunkHead * pChunk )
{
	ChunkHeapNode * pHeapNode = _Match( pChunk );
    if ( pHeapNode == NULL )
		_rec_Insert( &m_pBinHeapRoot, m_iHeightChange, pChunk );
    else {
        ChunkListNode * pListNode = _Chunk_GetListNode( pChunk );
        pListNode->pNext = pHeapNode->pNext;
        pListNode->pPrev = (ChunkListNode*)pHeapNode;
        pListNode->Padding = -1;
        pListNode->FakeBalance = BINHEAP_FAKEBALANCE;
        if ( pHeapNode->pNext != NULL )
            pHeapNode->pNext->pPrev = pListNode;
        pHeapNode->pNext = pListNode;
	}
}

ChunkHead * HeapAllocator::_PerformCoalescing( ChunkHead * pChunk )
{
	ChunkHead * pHead = pChunk;
	ChunkHead * pTmp = NULL;

	// Coalescing left side
	ChunkHead * pPrev = (ChunkHead*)_AU_Prev( (Byte*)pHead, _Chunk_PrevSize(pHead) );
	if ( pPrev == pHead )
		pPrev = NULL;
    if ( pPrev != NULL && _Chunk_IsFree(pPrev) ) { // coalesce left
		// Remove previous chunk from binheap
        ChunkHeapNode * pHeapNode = _Chunk_GetHeapNode( pPrev );
        if ( pHeapNode->iBalanceFactor == BINHEAP_FAKEBALANCE ) {
            // Extract from binlist
            ChunkListNode * pListNode = _Chunk_GetListNode( pPrev );
            pListNode->pPrev->pNext = pListNode->pNext;
            if ( pListNode->pNext != NULL )
                pListNode->pNext->pPrev = pListNode->pPrev;
            pListNode->pNext = NULL;
            pListNode->pPrev = NULL;
        } else {
            // Replace list's head or remove
            if ( pHeapNode->pNext == NULL )
                _rec_Remove( &m_pBinHeapRoot, m_iHeightChange, pPrev );
            else {
                _Replace( _Chunk_GetHead(pHeapNode->pNext) );
                pHeapNode->pNext = NULL;
            }
        }

        // Merge chunks
		pPrev->thisAUSize += _Chunk_Size( pHead );
		pTmp = (ChunkHead*)_AU_Next( (Byte*)pPrev, _Chunk_Size(pPrev) );
		pTmp->prevAUSize = _Chunk_Size( pPrev );
		pHead = pPrev;
	}

	// Coalescing right side
	ChunkHead * pNext = (ChunkHead*)_AU_Next( (Byte*)pHead, _Chunk_Size(pHead) );
	if ( _Chunk_Size(pNext) == DummyChunkAUSize )
		pNext = NULL;
    if ( pNext != NULL && _Chunk_IsFree(pNext) ) { // coalesce right
		// Remove next chunk from binheap
        ChunkHeapNode * pHeapNode = _Chunk_GetHeapNode( pNext );
        if ( pHeapNode->iBalanceFactor == BINHEAP_FAKEBALANCE ) {
            // Extract from binlist
            ChunkListNode * pListNode = _Chunk_GetListNode( pNext );
            pListNode->pPrev->pNext = pListNode->pNext;
            if ( pListNode->pNext != NULL )
                pListNode->pNext->pPrev = pListNode->pPrev;
            pListNode->pNext = NULL;
            pListNode->pPrev = NULL;
        } else {
            // Replace list's head or remove
            if ( pHeapNode->pNext == NULL )
                _rec_Remove( &m_pBinHeapRoot, m_iHeightChange, pNext );
            else {
                _Replace( _Chunk_GetHead(pHeapNode->pNext) );
                pHeapNode->pNext = NULL;
            }
        }

		// Merge chunks
		pHead->thisAUSize += _Chunk_Size( pNext );
		pTmp = (ChunkHead*)_AU_Next( (Byte*)pHead, _Chunk_Size(pHead) );
		pTmp->prevAUSize = _Chunk_Size( pHead );
	}

	return pHead;
}

BinHeapHeightChange HeapAllocator::_RotateOnce( ChunkHeapNode ** ppNode, BinHeapChild rotDir )
{
    BinHeapChild invertDir = BINHEAP_INVERTDIR( rotDir );
	ChunkHeapNode * pOldNode = *ppNode;
    BinHeapHeightChange heightChange = ( pOldNode->pChild[invertDir]->iBalanceFactor == BINHEAP_BALANCED ) ?
									     BINHEAP_HEIGHT_NOCHANGE : BINHEAP_HEIGHT_CHANGE;

    *ppNode = pOldNode->pChild[invertDir];
    pOldNode->pChild[invertDir] = (*ppNode)->pChild[rotDir];
    (*ppNode)->pChild[rotDir] = pOldNode;

    if ( rotDir == BINHEAP_CHILD_LEFT )
		--( (*ppNode)->iBalanceFactor );
	else
		++( (*ppNode)->iBalanceFactor );
	pOldNode->iBalanceFactor = -( (*ppNode)->iBalanceFactor );

	return heightChange;
}
BinHeapHeightChange HeapAllocator::_RotateTwice( ChunkHeapNode ** ppNode, BinHeapChild rotDir )
{
    BinHeapChild invertDir = BINHEAP_INVERTDIR( rotDir );
	ChunkHeapNode * pOldNode = *ppNode;
	ChunkHeapNode * pOldInvertDir = pOldNode->pChild[invertDir];

	*ppNode = pOldNode->pChild[invertDir]->pChild[rotDir];
	pOldNode->pChild[invertDir] = (*ppNode)->pChild[rotDir];
	(*ppNode)->pChild[rotDir] = pOldNode;
	pOldInvertDir->pChild[rotDir] = (*ppNode)->pChild[invertDir];
	(*ppNode)->pChild[invertDir] = pOldInvertDir;

	(*ppNode)->pChild[BINHEAP_CHILD_LEFT]->iBalanceFactor = - Max<Int>( (*ppNode)->iBalanceFactor, BINHEAP_BALANCED );
	(*ppNode)->pChild[BINHEAP_CHILD_RIGHT]->iBalanceFactor = - Min<Int>( (*ppNode)->iBalanceFactor, BINHEAP_BALANCED );
	(*ppNode)->iBalanceFactor = BINHEAP_BALANCED;

	return BINHEAP_HEIGHT_CHANGE; // height is always shortened
}
BinHeapHeightChange HeapAllocator::_ReBalance( ChunkHeapNode ** ppNode )
{
    BinHeapHeightChange heightChange = BINHEAP_HEIGHT_NOCHANGE;

    if ( BINHEAP_IMBALANCE_LEFT((*ppNode)->iBalanceFactor) ) {
		// Need a right rotation
		if ( (*ppNode)->pChild[BINHEAP_CHILD_LEFT]->iBalanceFactor == BINHEAP_RIGHT_HEAVY )
			heightChange = _RotateTwice( ppNode, BINHEAP_CHILD_RIGHT ); // RL rotation needed
		else
			heightChange = _RotateOnce( ppNode, BINHEAP_CHILD_RIGHT ); // R rotation needed
	} else if ( BINHEAP_IMBALANCE_RIGHT((*ppNode)->iBalanceFactor) ) {
		// Need a left rotation
		if ( (*ppNode)->pChild[BINHEAP_CHILD_RIGHT]->iBalanceFactor == BINHEAP_LEFT_HEAVY )
			heightChange = _RotateTwice( ppNode, BINHEAP_CHILD_LEFT ); // LR rotation needed
		else
			heightChange = _RotateOnce( ppNode, BINHEAP_CHILD_LEFT ); // L rotation needed
	}

	return heightChange;
}
ChunkHeapNode * HeapAllocator::_rec_Insert( ChunkHeapNode ** ppNode, BinHeapHeightChange & heightChange, ChunkHead * pChunk )
{
    // End of sub-tree, insert here
    if ( *ppNode == NULL ) {
		*ppNode = _Chunk_GetHeapNode( pChunk );
        (*ppNode)->pNext = NULL;
        (*ppNode)->pChild[BINHEAP_CHILD_LEFT] = NULL;
        (*ppNode)->pChild[BINHEAP_CHILD_RIGHT] = NULL;
        (*ppNode)->iBalanceFactor = BINHEAP_BALANCED;
		heightChange = BINHEAP_HEIGHT_CHANGE;
		return NULL;
	}

    // Compare
    ChunkHead * pNodeChunk = _Chunk_GetHead( *ppNode );
    Int cmp = _Compare( pNodeChunk, pChunk );

    // Key already exist, transmit up
    if ( cmp == 0 ) {
        heightChange = BINHEAP_HEIGHT_NOCHANGE;
		return (*ppNode);
    }

    // Search in correct sub-tree
    BinHeapChild dir = ( cmp < 0 ) ? BINHEAP_CHILD_LEFT : BINHEAP_CHILD_RIGHT;
    ChunkHeapNode * pFound = _rec_Insert( &((*ppNode)->pChild[dir]), heightChange, pChunk );

    // Key exists in sub-tree, transmit up
    if ( pFound != NULL )
        return pFound;

    // Adjust for re-balance
    Int inc = cmp * heightChange;

    // Key was inserted, re-balance if needed
    (*ppNode)->iBalanceFactor += inc;
    if ( (inc != 0) && ((*ppNode)->iBalanceFactor != BINHEAP_BALANCED) )
        heightChange = (BinHeapHeightChange)( 1 - _ReBalance(ppNode) );
    else
        heightChange = BINHEAP_HEIGHT_NOCHANGE;
	return NULL;
}
Bool HeapAllocator::_rec_Remove( ChunkHeapNode ** ppNode, BinHeapHeightChange & heightChange, ChunkHead * pChunk )
{
    // End of sub-tree, key not found
    if ( *ppNode == NULL ) {
		heightChange = BINHEAP_HEIGHT_NOCHANGE;
		return false;
	}

    // Compare
    ChunkHead * pNodeChunk = _Chunk_GetHead( *ppNode );
    Int cmp = _Compare( pNodeChunk, pChunk );
    Bool bRemoved = false;
    Int dec = 0;

    if ( cmp != 0 ) {
        // Search in correct sub-tree
        BinHeapChild dir = ( cmp < 0 ) ? BINHEAP_CHILD_LEFT : BINHEAP_CHILD_RIGHT;
	    bRemoved = _rec_Remove( &((*ppNode)->pChild[dir]), heightChange, pChunk );

        // Key doesn't exist, can't remove
        if ( !bRemoved )
            return false;

        // Adjust for re-balance
        dec = cmp * heightChange;
    } else {
        // Key found at this node
        ChunkHeapNode * pRemove = (*ppNode);

        // Leaf case
        if ( ((*ppNode)->pChild[BINHEAP_CHILD_LEFT] == NULL) &&
             ((*ppNode)->pChild[BINHEAP_CHILD_RIGHT] == NULL) ) {
            *ppNode = NULL;
            pRemove->pChild[BINHEAP_CHILD_LEFT] = NULL;
            pRemove->pChild[BINHEAP_CHILD_RIGHT] = NULL;
            pRemove->iBalanceFactor = BINHEAP_FAKEBALANCE;
            heightChange = BINHEAP_HEIGHT_CHANGE;
		    return true;
        }

        // One-slot cases
        if ( (*ppNode)->pChild[BINHEAP_CHILD_LEFT] == NULL ) {
            *ppNode = (*ppNode)->pChild[BINHEAP_CHILD_RIGHT];
            pRemove->pChild[BINHEAP_CHILD_LEFT] = NULL;
            pRemove->pChild[BINHEAP_CHILD_RIGHT] = NULL;
            pRemove->iBalanceFactor = BINHEAP_FAKEBALANCE;
            heightChange = BINHEAP_HEIGHT_CHANGE;
		    return true;
        }
        if ( (*ppNode)->pChild[BINHEAP_CHILD_RIGHT] == NULL ) {
            *ppNode = (*ppNode)->pChild[BINHEAP_CHILD_LEFT];
            pRemove->pChild[BINHEAP_CHILD_LEFT] = NULL;
            pRemove->pChild[BINHEAP_CHILD_RIGHT] = NULL;
            pRemove->iBalanceFactor = BINHEAP_FAKEBALANCE;
            heightChange = BINHEAP_HEIGHT_CHANGE;
		    return true;
        }

        // Two-children case : Swap with successor
        ChunkHeapNode ** ppSwapNode = &( (*ppNode)->pChild[BINHEAP_CHILD_RIGHT] );
        while( (*ppSwapNode)->pChild[BINHEAP_CHILD_LEFT] != NULL )
            ppSwapNode = &( (*ppSwapNode)->pChild[BINHEAP_CHILD_LEFT] );

        // Here is the tricky part, can't move data, we can alter only links & factors !
            // Swap left branches & balances
        Swap<Int>( &( (*ppNode)->iBalanceFactor ), &( (*ppSwapNode)->iBalanceFactor ) );
        (*ppSwapNode)->pChild[BINHEAP_CHILD_LEFT] = (*ppNode)->pChild[BINHEAP_CHILD_LEFT];
        (*ppNode)->pChild[BINHEAP_CHILD_LEFT] = NULL;
            // Swap right branches & parent links
        ChunkHeapNode * pSwapNode = *ppSwapNode;
        ChunkHeapNode * pTemp = (*ppNode)->pChild[BINHEAP_CHILD_RIGHT];
        (*ppNode)->pChild[BINHEAP_CHILD_RIGHT] = pSwapNode->pChild[BINHEAP_CHILD_RIGHT];
        if ( pTemp == pSwapNode ) {
            // Circular permutation case
            pSwapNode->pChild[BINHEAP_CHILD_RIGHT] = *ppNode;
        } else {
            // Swap case
            pSwapNode->pChild[BINHEAP_CHILD_RIGHT] = pTemp;
            *ppSwapNode = *ppNode;
        }
        *ppNode = pSwapNode;

        bRemoved = _rec_Remove( &( (*ppNode)->pChild[BINHEAP_CHILD_RIGHT] ), heightChange, pChunk );
        Assert( bRemoved );

        // Adjust for re-balance
        dec = heightChange;
    }
    
    // Key was deleted, re-balance if needed
    (*ppNode)->iBalanceFactor -= dec;
	if ( dec != 0 ) {
		if ( (*ppNode)->iBalanceFactor != BINHEAP_BALANCED )
			heightChange = _ReBalance( ppNode );
		else
			heightChange = BINHEAP_HEIGHT_CHANGE;
	} else
		heightChange = BINHEAP_HEIGHT_NOCHANGE;
	return true;
}
ChunkHeapNode * HeapAllocator::_Search( UInt iMinSize ) const
{
    ChunkHeapNode * pNode = m_pBinHeapRoot;
    ChunkHeapNode * pBestNode = NULL;

    while( pNode != NULL ) {
        ChunkHead * pNodeChunk = _Chunk_GetHead( pNode );
        UInt iNodeSize = _Chunk_Size( pNodeChunk );
        // Perfect fit, return ASAP !
        if ( iNodeSize == iMinSize )
            return pNode;
        // Doesn't fit, go right
        if ( iNodeSize < iMinSize ) {
            pNode = pNode->pChild[BINHEAP_CHILD_RIGHT];
            continue;
        }
        // First/Best fit, save and go left
        pBestNode = pNode;
        pNode = pNode->pChild[BINHEAP_CHILD_LEFT];
    }
    // We have the best node available
    return pBestNode;
}
ChunkHeapNode * HeapAllocator::_Match( const ChunkHead * pChunk ) const
{
    ChunkHeapNode * pNode = m_pBinHeapRoot;
    while( pNode != NULL ) {
        // Compare
        ChunkHead * pNodeChunk = _Chunk_GetHead( pNode );
        Int cmp = _Compare( pNodeChunk, pChunk );

        // Found
        if ( cmp == 0 )
            return pNode;

        // Search in correct sub-tree
        BinHeapChild dir = ( cmp < 0 ) ? BINHEAP_CHILD_LEFT : BINHEAP_CHILD_RIGHT;
        pNode = pNode->pChild[dir];
    }
    return NULL;
}
ChunkHead * HeapAllocator::_Replace( ChunkHead * pNewChunk )
{
    ChunkHeapNode ** ppNode = &(m_pBinHeapRoot);
    while( (*ppNode) != NULL ) {
        // Compare
        ChunkHead * pNodeChunk = _Chunk_GetHead( *ppNode );
        Int cmp = _Compare( pNodeChunk, pNewChunk );

        // Found
        if ( cmp == 0 ) {
            ChunkHeapNode * pNewNode = _Chunk_GetHeapNode( pNewChunk );
            pNewNode->pChild[BINHEAP_CHILD_LEFT] = (*ppNode)->pChild[BINHEAP_CHILD_LEFT];
            pNewNode->pChild[BINHEAP_CHILD_RIGHT] = (*ppNode)->pChild[BINHEAP_CHILD_RIGHT];
            pNewNode->iBalanceFactor = (*ppNode)->iBalanceFactor;
            (*ppNode)->pChild[BINHEAP_CHILD_LEFT] = NULL;
            (*ppNode)->pChild[BINHEAP_CHILD_RIGHT] = NULL;
            (*ppNode)->iBalanceFactor = BINHEAP_FAKEBALANCE;
            *ppNode = pNewNode;
            return pNodeChunk;
        }

        // Search in correct sub-tree
        BinHeapChild dir = ( cmp < 0 ) ? BINHEAP_CHILD_LEFT : BINHEAP_CHILD_RIGHT;
        ppNode = &( (*ppNode)->pChild[dir] );
    }
    return NULL;
}
