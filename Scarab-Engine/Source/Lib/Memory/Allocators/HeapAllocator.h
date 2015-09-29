/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Memory/Allocators/HeapAllocator.h
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
// Known Bugs : Improvement, use worst-fit when lowly fragmented, first-fit
//              when uniformly fragmented (stable), and best-fit when heavyly
//              fragmented ... or something like that (!)
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_LIB_MEMORY_ALLOCATORS_HEAPALLOCATOR_H
#define SCARAB_LIB_MEMORY_ALLOCATORS_HEAPALLOCATOR_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "MemoryAllocator.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define HEAPMEMORY_ALIGN_NATIVE       4 // 4 = sizeof(void*)
#define HEAPMEMORY_ALIGN_SHIFT_NATIVE 2 // (a << 2) = a * 4 = a * sizeof(void*)

// A chunk in memory :
// | prevAUSize(4) | AUSize(4) | User Data ....................................................... | (allocated)
// |       "       |     "     | pNext(4) | pLeftChild(4) | pRightChild(4) | iBalance(4) | ....... | (free, binheapnode)
// |       "       |     "     | pNext(4) |   pPrev(4)    |     Padding with '1' (8)     | ....... | (free, binlistnode)
typedef struct _chunk_head
{
	UInt prevAUSize;
	UInt thisAUSize;
} ChunkHead;
typedef struct _chunk_heapnode
{
    struct _chunk_listnode * pNext;
	struct _chunk_heapnode * pChild[2];
	Int iBalanceFactor;
} ChunkHeapNode;
typedef struct _chunk_listnode
{
    struct _chunk_listnode * pNext;
	struct _chunk_listnode * pPrev;
    Int Padding;     // Allways set to BINHEAP_FAKEBALANCE so we have
    Int FakeBalance; // invalid balance value to distinguish nodes
} ChunkListNode;
#define BINHEAP_FAKEBALANCE ( (Int)0xdeadbeef )

// Height changes for the AVL-Tree :
enum BinHeapHeightChange
{
    BINHEAP_HEIGHT_NOCHANGE = 0,
    BINHEAP_HEIGHT_CHANGE = 1
};

// Childs indices :
enum BinHeapChild
{
    BINHEAP_CHILD_LEFT = 0,
    BINHEAP_CHILD_RIGHT = 1
};
#define BINHEAP_INVERTDIR(_dir) ( (BinHeapChild)(1-(_dir)) )

// Balance factors for the AVL-Tree :
#define BINHEAP_LEFT_HEAVY  (-1)
#define BINHEAP_BALANCED    0
#define BINHEAP_RIGHT_HEAVY 1
#define BINHEAP_IMBALANCE_LEFT(_bal) ( (_bal) < BINHEAP_LEFT_HEAVY )
#define BINHEAP_IMBALANCE_RIGHT(_bal) ( (_bal) > BINHEAP_RIGHT_HEAVY )

// Reporting
#define HEAPREPORT_LOGFILE   TEXT("Logs/Memory/HeapReports.log")
#define HEAPREPORT_MAX_TREESPAN 256
#define HEAPREPORT_MAX_LISTSIZE 16
#define HEAPREPORT_MAX_CHUNKS   1024
typedef struct _heap_report : public AllocatorReport
{
    Byte * pBaseAddress;
    UInt iTotalSize;
    UInt iAllocatedSize;
    UInt iFreeSize;
    Byte * pLastFreed;

    UInt iBinHeapSize;    
    Byte ** arrHeapNodes;  //
    Int * arrBalances;     // size = iBinHeapSize
    UInt * arrListSizes;   //
    Byte *** arrListNodes; // <= size[i] = arrFreeListSizes[i]

    UInt iChunkMapSize;
    Byte ** arrChunkMap; //
    UInt * arrPrevSizes; //  size = iChunkMapSize
    UInt * arrSizes;     //
} HeapReport;

///////////////////////////////////////////////////////////////////////////////
// The HeapAllocator class
class HeapAllocator : public MemoryAllocator
{
public:
    HeapAllocator( UInt iContextID, const GChar * strContextName,
                   UInt iAllocatorID, const GChar * strAllocatorName );
	~HeapAllocator();

    // Deferred initialization
    Void Initialize( UInt iHeapSize );
    Void Cleanup();

    // Getters
    inline AllocatorType GetType() const;
    inline UInt GetBlockSize( Byte * pMemory ) const;

    // Alloc/Free interface
	Byte * Allocate( UInt iSize );
	Void Free( Byte * pMemory );

    // Reporting
    Void GenerateReport( AllocatorReport * outReport ) const;
    Void LogReport( const AllocatorReport * pReport ) const;

private:
    // Internal constants
    const UInt AlignUnit;      // Size of an AU, must be power of 2
    const UInt AlignUnitShift; // Shift-size of an AU, 2^Shift = Size
	const UInt AUFreeMask; // Mask of the free-flag bit in an AU's size-pointers
	const UInt AUSizeMask; // Mask of the size bits in an AU's size-pointers
    const UInt ChunkHeadAUSize; // Size of ChunkHead, in AUs
    const UInt ChunkHeapNodeAUSize; // Size of ChunkHeapNode, in AUs
    const UInt ChunkListNodeAUSize; // Size of ChunkListNode, in AUs
	const UInt DummyChunkAUSize;
	const UInt DummyChunkByteSize;
	const UInt MinimalChunkAUSize;

    // AU helpers
    inline UInt _AU_ConvertSize( UInt iSize ) const;
    inline Bool _AU_IsAllocated( UInt iAUSize ) const;
	inline Bool _AU_IsFree( UInt iAUSize ) const;
	inline UInt _AU_Size( UInt iAUSize ) const;
	inline Byte * _AU_Next( Byte * pChunk, UInt nAAU ) const;
	inline Byte * _AU_Prev( Byte * pChunk, UInt nAAU ) const;

    // Chunk helpers
    inline Void _Chunk_MarkAllocated( ChunkHead * pChunk ) const;
	inline Void _Chunk_MarkFree( ChunkHead * pChunk ) const;
	inline Bool _Chunk_IsAllocated( const ChunkHead * pChunk ) const;
	inline Bool _Chunk_IsFree( const ChunkHead * pChunk ) const;
	inline UInt _Chunk_PrevSize( const ChunkHead * pChunk ) const;
	inline UInt _Chunk_Size( const ChunkHead * pChunk ) const;
    inline ChunkHeapNode * _Chunk_GetHeapNode( ChunkHead * pChunk ) const;
	inline ChunkListNode * _Chunk_GetListNode( ChunkHead * pChunk ) const;
    inline ChunkHead * _Chunk_GetHead( ChunkHeapNode * pHeapNode ) const;
    inline ChunkHead * _Chunk_GetHead( ChunkListNode * pListNode ) const;
    inline Int _Compare( const ChunkHead * pLHS, const ChunkHead * pRHS ) const;

    // BinHeap interface
    ChunkHead * _BinHeap_RequestChunk( UInt iMinSize );
	Void _BinHeap_ReleaseChunk( ChunkHead * pChunk );

    // Deferred coalescing
	ChunkHead * _PerformCoalescing( ChunkHead * pChunk );
	ChunkHead * m_pLastFreed;

    // AVL-Tree sub-routines & data
	BinHeapHeightChange _RotateOnce( ChunkHeapNode ** ppNode, BinHeapChild rotDir );
	BinHeapHeightChange _RotateTwice( ChunkHeapNode ** ppNode, BinHeapChild rotDir );
	BinHeapHeightChange _ReBalance( ChunkHeapNode ** ppNode );
	ChunkHeapNode * _rec_Insert( ChunkHeapNode ** ppNode, BinHeapHeightChange & heightChange, ChunkHead * pChunk );
	Bool _rec_Remove( ChunkHeapNode ** ppNode, BinHeapHeightChange & heightChange, ChunkHead * pChunk );
    ChunkHeapNode * _Search( UInt iMinSize ) const;
    ChunkHeapNode * _Match( const ChunkHead * pChunk ) const;
    ChunkHead * _Replace( ChunkHead * pNewChunk );

    ChunkHeapNode * m_pBinHeapRoot;
    BinHeapHeightChange m_iHeightChange;

    // Heap memory
	Byte * m_pHeapMemory;
	UInt m_iHeapSize;
    UInt m_iTotalFree;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "HeapAllocator.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MEMORY_ALLOCATORS_HEAPALLOCATOR_H
