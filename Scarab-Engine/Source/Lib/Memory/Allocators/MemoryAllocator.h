/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Memory/Allocators/MemoryAllocator.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Base interface for allocators to comply with the manager.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_LIB_MEMORY_ALLOCATORS_MEMORYALLOCATOR_H
#define SCARAB_LIB_MEMORY_ALLOCATORS_MEMORYALLOCATOR_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Error/ErrorManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define MEMORY_MAX_NAMELENGTH 128

enum AllocatorType {
    ALLOCATOR_RESIDENT = 0,
    ALLOCATOR_STACK,
    ALLOCATOR_POOL,
    ALLOCATOR_BREAK,
    ALLOCATOR_HEAP
    // Add new child-classes an ID here ...
};

typedef struct _allocator_report
{
    UInt idMemoryContext;
    const GChar * strContextName;
    UInt idMemoryAllocator;
    const GChar * strAllocatorName;
    // specific data follows ...
} AllocatorReport;

///////////////////////////////////////////////////////////////////////////////
// The MemoryAllocator class
class MemoryAllocator
{
public:
    MemoryAllocator( UInt iContextID, const GChar * strContextName,
                     UInt iAllocatorID, const GChar * strAllocatorName );
	~MemoryAllocator();

    // Getters
    inline UInt GetContextID() const;
    inline const GChar * GetContextName() const;
    inline UInt GetAllocatorID() const;
    inline const GChar * GetAllocatorName() const;

    virtual AllocatorType GetType() const = 0;
    virtual UInt GetBlockSize( Byte * pMemory ) const = 0;

    // Alloc/Free interface
    virtual Byte * Allocate( UInt iSize ) = 0;
    virtual Void Free( Byte * pMemory ) = 0;

    // Reporting
    virtual Void GenerateReport( AllocatorReport * outReport ) const = 0;
    virtual Void LogReport( const AllocatorReport * pReport ) const = 0;

protected:
    UInt m_idContext;
    GChar m_strContextName[MEMORY_MAX_NAMELENGTH];
    UInt m_idAllocator;
    GChar m_strAllocatorName[MEMORY_MAX_NAMELENGTH];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MemoryAllocator.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MEMORY_ALLOCATORS_MEMORYALLOCATOR_H
