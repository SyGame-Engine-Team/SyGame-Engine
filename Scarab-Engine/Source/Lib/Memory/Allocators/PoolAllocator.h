/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Memory/Allocators/PoolAllocator.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Very simple, but very efficient, fixed-size allocator.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : Beware !!! Here is a tricky & subtle thing :
//              If we use a pool allocator with a stack-fashion allocation
//              pattern, we are guaranted to keep our blocks contiguous because
//              the free list acts symmetrically with head add/remove ...
//              This is of course the optimal case for page-growing arrays !
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_LIB_MEMORY_ALLOCATORS_POOLALLOCATOR_H
#define SCARAB_LIB_MEMORY_ALLOCATORS_POOLALLOCATOR_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "MemoryAllocator.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define POOLREPORT_LOGFILE      TEXT("Logs/Memory/PoolReports.log")
#define POOLREPORT_MAX_FREELIST 1024
typedef struct _pool_report : public AllocatorReport
{
    Byte * pBaseAddress;
    UInt iTotalSize;
    UInt iChunkSize;
    UInt iTotalChunks;
    UInt iAllocatedChunks;
    UInt iFreeChunks;
    UInt * arrFreeChunksList; // size = iFreeChunks
} PoolReport;

///////////////////////////////////////////////////////////////////////////////
// The PoolAllocator class
class PoolAllocator : public MemoryAllocator
{
public:
	PoolAllocator( UInt iContextID, const GChar * strContextName,
                   UInt iAllocatorID, const GChar * strAllocatorName );
	~PoolAllocator();

    // Deferred initialization
    Void Initialize( UInt iChunkSize, UInt iTotalChunks );
    Void Cleanup();

    // Getters
    inline AllocatorType GetType() const;
    inline UInt GetBlockSize( Byte * pMemory ) const;

    inline UInt ChunkSize() const;
	inline UInt ChunkCount() const;
    inline UInt ChunkTotal() const;

    // Alloc/Free interface
	Byte * Allocate( UInt iSize );
	Void Free( Byte * pMemory );

    Byte * Allocate();

    // Reporting
    Void GenerateReport( AllocatorReport * outReport ) const;
    Void LogReport( const AllocatorReport * pReport ) const;

private:
	Byte * m_pBuffer;
    UInt m_iChunkSize;
	UInt m_iTotalChunks;
	UInt m_iNextFree;
    UInt m_iChunkCount;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "PoolAllocator.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MEMORY_ALLOCATORS_POOLALLOCATOR_H
