/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Memory/Allocators/BreakAllocator.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : The old-school dirty-trashy assembly memory block-breaker ...
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_LIB_MEMORY_ALLOCATORS_BREAKALLOCATOR_H
#define SCARAB_LIB_MEMORY_ALLOCATORS_BREAKALLOCATOR_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "MemoryAllocator.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
typedef struct _block_break_header
{
    struct _block_break_header * pPrev;
    struct _block_break_header * pNext;
    UInt iBreak;
} BlockBreakHeader;

// Reporting
#define BREAKREPORT_LOGFILE    TEXT("Logs/Memory/BreakReports.log")
typedef struct _break_report : public AllocatorReport
{
    Byte * pBaseAddress;
    UInt iTotalSize;
    UInt iBlockSize;
    UInt iAllocatedBlocks;
    UInt iFreeBlocks;
    UInt iCurrentBlock;
    UInt * arrBreakList; // size = iAllocatedBlocks
} BreakReport;

///////////////////////////////////////////////////////////////////////////////
// The BreakAllocator class
class BreakAllocator : public MemoryAllocator
{
public:
	BreakAllocator( UInt iContextID, const GChar * strContextName,
                    UInt iAllocatorID, const GChar * strAllocatorName );
	~BreakAllocator();

    // Deferred initialization
    Void Initialize( UInt iBlockSize, UInt iRangeSize );
    Void Cleanup();

    // Getters
    inline AllocatorType GetType() const;
    inline UInt GetBlockSize( Byte * pMemory ) const;

    // Alloc/Free interface
    Byte * Allocate( UInt iSize );
    Void Free( Byte * pMemory );

        // Chunk-Break allocations
    //Void ChunkRequest( UInt iMinFreeSize, MemoryContextID idContext = INVALID_OFFSET );
    //Void ChunkRelease( MemoryContextID idContext = INVALID_OFFSET );
    //Void ChunkFirst( MemoryContextID idContext = INVALID_OFFSET );
    //Void ChunkPrev( MemoryContextID idContext = INVALID_OFFSET );
    //Void ChunkNext( MemoryContextID idContext = INVALID_OFFSET );
    //Void ChunkSelect( UInt iHistoryIndex, MemoryContextID idContext = INVALID_OFFSET );
    //Byte * ChunkBreak( UInt iSize, MemoryContextID idContext = INVALID_OFFSET );
    //Void ChunkUnbreak( Byte * pMemory, MemoryContextID idContext = INVALID_OFFSET );

    // Reporting
    Void GenerateReport( AllocatorReport * outReport ) const;
    Void LogReport( const AllocatorReport * pReport ) const;

private:
    Byte * m_pMemoryRange;
    UInt m_iRangeSize;
    UInt m_iBlockSize;
    BlockBreakHeader * m_pFirstBlock;
    BlockBreakHeader * m_pCurrentBlock;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "BreakAllocator.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MEMORY_ALLOCATORS_BREAKALLOCATOR_H
