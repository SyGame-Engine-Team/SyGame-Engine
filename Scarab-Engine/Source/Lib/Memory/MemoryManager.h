/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Memory/MemoryManager.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : The VERY important and fine-tuned memory manager ...
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
#ifndef SCARAB_LIB_MEMORY_MEMORYMANAGER_H
#define SCARAB_LIB_MEMORY_MEMORYMANAGER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Math/Hash/Hashing.h"
#include "../Math/Functions/MathFunction.h" // Log2OfPower2

#include "../Error/ErrorManager.h"

#include "Allocators/StackAllocator.h"
#include "Allocators/PoolAllocator.h"
#include "Allocators/BreakAllocator.h"
#include "Allocators/HeapAllocator.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define MemoryFn MemoryManager::GetInstancePtr()

// Macro interface
#define New \
    new( TEXT(__FILE__), __LINE__ )

#define Delete( _pointer ) { \
    _dat datSaved; Bool bValid = _dat_delete_save( &datSaved, TEXT(__FILE__), __LINE__ ); \
    delete _pointer; _dat_delete_restore( &datSaved, bValid ); \
}
#define DeleteA( _pointer ) { \
    _dat datSaved; Bool bValid = _dat_delete_save( &datSaved, TEXT(__FILE__), __LINE__ ); \
    delete[] _pointer; _dat_delete_restore( &datSaved, bValid ); \
}

// Would take too long to explain why we need this weird stuff in details ...
// Let's just say compilers do very bad things with new/delete, arrays
// and memory tracking (implicit data translation, hidden headers) ...
// using new/delete functions without the operators would mean reproducing
// the compiler's choices without compile-time data which is not feasable ;)
// (For those hackers around here : try to figure out how the compiler
// tracks array sizes & virtual destructors with builtins, classes, pointers
// and templates ... you'll be very surprised ... then figure the same
// with pointer-to-members and derived/base pointers ... commit suicide)
// Added the painful missing of a full-featured placement delete, passing
// arguments to delete ends up being impossible in the very general case.
// Again, one more clue telling us C++ compilers really are one of the most
// challenging piece of code on earth ...
typedef struct _delete_argument_trick {
    const GChar * strFile;
    UInt iLine;
    Bool bIsValid;
} _dat;
inline _dat * _dat_get_ptr();
Bool _dat_delete_save( _dat * pSaved, const GChar * strFile, UInt iLine );
Void _dat_delete_restore( _dat * pSaved, Bool bValid );

// new/new[]/delete/delete[] wrappers
Byte * _managed_new( Bool bIsArray, UInt iSize, const GChar * strFile, UInt iLine );
Void _managed_delete( Bool bIsArray, Byte * pAllocated, const GChar * strFile, UInt iLine );
    // Pre-allocated versions
inline Void * operator new ( UInt iSize, Void * pAddress );
inline Void * operator new[] ( UInt iSize, Void * pAddress );
inline Void operator delete ( Void * pAddress, Void * );
inline Void operator delete[] ( Void * pAddress, Void * );
    // Full-fledged versions (delete/delete[] only called when a constructor throws an exception)
inline Void * operator new ( UInt iSize, const GChar * strFile, UInt iLine );
inline Void * operator new[] ( UInt iSize, const GChar * strFile, UInt iLine );
inline Void operator delete ( Void * pAddress , const GChar * strFile, UInt iLine );
inline Void operator delete[] ( Void * pAddress , const GChar * strFile, UInt iLine );
    // Wrapped default versions (new/new[] only called by the allocators to access system heap)
Void * operator new ( UInt iSize );
Void * operator new[] ( UInt iSize );
Void operator delete ( Void * pAddress );
Void operator delete[] ( Void * pAddress );

// Memory Manager definitions
#define MEMORY_TABLE_SIZE     1024 // MUST be power of 2
#define MEMORY_MAX_ALLOCATORS 1024 // max # in the pool for each type

#define MEMORY_CONTEXT_SHARED      (MEMORY_TABLE_SIZE + 1)
#define MEMORY_SHARED_SCRATCH_SIZE 4194304 // 4 mb

#define MEMORY_CONTEXT_TABLE_SIZE  1024 // MUST be power of 2

typedef UInt MemoryContextID;
typedef UInt MemoryAllocatorID;

typedef struct _allocator_data
{
    MemoryAllocatorID idAlloc;
    GChar strName[MEMORY_MAX_NAMELENGTH];

    MemoryAllocator * pAllocator;
} AllocatorData;

typedef struct _memory_context
{
    MemoryContextID idContext;
    GChar strName[MEMORY_MAX_NAMELENGTH];

    Byte * pResidentMemory;
    UInt iResidentSize;

    UInt iAllocatorCount;
    AllocatorData arrAllocators[MEMORY_CONTEXT_TABLE_SIZE];

    MemoryAllocatorID idSelected;
} MemoryContext;

// Switching system
#define MEMORYSWITCH_LOGFILE TEXT("Logs/Memory/Switches.log")
#define MEMORYSWITCH_MAX     1024

typedef struct _switchstack_item
{
    MemoryContextID idContext;
    MemoryAllocatorID idAlloc;
} SwitchStackItem;

// Tracing system
#define MEMORYTRACE_LOGFILE TEXT("Logs/Memory/Traces.log")
#define MEMORYTRACE_MAX     256

enum TraceFilter
{
    MEMORY_TRACE_ALL = 0,
    MEMORY_TRACE_CONTEXT,
    MEMORY_TRACE_ALLOCATOR
};

typedef struct _trace_record
{
    Bool bIsAlloc;                  // Alloc or Free
    MemoryContextID idContext;      // ID of the Context
    const GChar * strContextName;   // Context name
    MemoryAllocatorID idAllocator;  // ID of the Allocator
    const GChar * strAllocatorName; // Allocator name
    AllocatorType iAllocatorType;   // Type of the Allocator
    Byte * pAddress;                // Address of memory block
    UInt iSize;                     // Size of memory block
    Bool bIsArray;                  // scalar or vector allocation
    const GChar * strFileName;      // File where the trace was initiated
    UInt iFileLine;                 // Line number in this file
} TraceRecord;

/////////////////////////////////////////////////////////////////////////////////
// The MemoryManager class
class MemoryManager
{
    // Manual singleton interface since the manager
    // template actually depends on this !
public:
    inline static Void Create();
    inline static Void Destroy();
    inline static MemoryManager & GetInstance();
    inline static MemoryManager * GetInstancePtr();

private:
    MemoryManager();
    ~MemoryManager();

    static MemoryManager * sm_pInstance;

public:
    // Contexts management
    MemoryContextID CreateContext( const GChar * strName, UInt iResidentSize, Byte ** outResidentPointer );
    Void DestroyContext( MemoryContextID idContext );
    Void DestroyContext( const GChar * strName );

    inline MemoryContextID GetContextID( const GChar * strName ) const;
    inline const GChar * GetContextName( MemoryContextID idContext ) const;
    inline MemoryContextID GetSelectedContextID() const;
    inline const GChar * GetSelectedContextName() const;

    // Allocators management
    MemoryAllocatorID CreateStack( const GChar * strName, UInt iStackSize,                   MemoryContextID idContext = INVALID_OFFSET );
    MemoryAllocatorID CreatePool( const GChar * strName, UInt iChunkSize, UInt iTotalChunks, MemoryContextID idContext = INVALID_OFFSET );
    MemoryAllocatorID CreateBreak( const GChar * strName, UInt iBlockSize, UInt iRangeSize,  MemoryContextID idContext = INVALID_OFFSET );
    MemoryAllocatorID CreateHeap( const GChar * strName, UInt iHeapSize,                     MemoryContextID idContext = INVALID_OFFSET );
    Void DestroyAllocator( MemoryAllocatorID idAllocator, MemoryContextID idContext = INVALID_OFFSET );
    Void DestroyAllocator( const GChar * strName,         MemoryContextID idContext = INVALID_OFFSET );

    inline MemoryAllocatorID GetAllocatorID( const GChar * strName,       MemoryContextID idContext = INVALID_OFFSET );
    inline const GChar * GetAllocatorName( MemoryAllocatorID idAllocator, MemoryContextID idContext = INVALID_OFFSET );
    inline MemoryAllocatorID GetSelectedAllocatorID( MemoryContextID idContext = INVALID_OFFSET );
    inline const GChar * GetSelectedAllocatorName( MemoryContextID idContext = INVALID_OFFSET );

    // Switching system
    Void SelectMemory( MemoryAllocatorID idAllocator, MemoryContextID idContext = INVALID_OFFSET );
    Void SelectMemory( const GChar * strAllocatorName, MemoryContextID idContext = INVALID_OFFSET );
    Void SelectMemory( MemoryAllocatorID idAllocator, const GChar * strContextName = NULL );
    Void SelectMemory( const GChar * strAllocatorName, const GChar * strContextName = NULL );
    Void UnSelectMemory();

    Void LogSelectionState();

    // Tracing system
    inline Void TraceStart( TraceFilter iFilter, MemoryContextID idContext = INVALID_OFFSET, MemoryAllocatorID idAllocator = INVALID_OFFSET );
    inline Void TraceStop();
    inline Void TraceReset();

    inline UInt TraceCount() const;
    inline const TraceRecord * TracePick( UInt iIndex ) const;

    // Reporting system
    inline Void GenerateReport( AllocatorReport * outReport, MemoryAllocatorID idAllocator = INVALID_OFFSET, MemoryContextID idContext = INVALID_OFFSET );
    inline Void LogReport( const AllocatorReport * pReport, MemoryAllocatorID idAllocator = INVALID_OFFSET, MemoryContextID idContext = INVALID_OFFSET );

private:
    // Access to wrappers
    friend Byte * _managed_new( Bool, UInt, const GChar *, UInt );
    friend Void _managed_delete( Bool, Byte *, const GChar *, UInt );

    inline MemoryAllocator * _GetAllocator();

    // Helpers
    inline UInt _HashName( const GChar * strName, UInt iTableSize ) const;

    inline MemoryContext * _MemoryContext_Get( MemoryContextID idContext );
    inline AllocatorData * _MemoryAllocator_Get( MemoryContext * pContext, MemoryAllocatorID idAllocator );

    inline MemoryContextID _MemoryContext_Add( const GChar * strName );
    inline MemoryAllocatorID _MemoryAllocator_Add( MemoryContext * pContext, const GChar * strName );
    inline Void _MemoryContext_Remove( MemoryContextID idContext );
    inline Void _MemoryAllocator_Remove( MemoryContext * pContext, MemoryAllocatorID idAllocator );

    Void _MemoryContext_Initialize( MemoryContext * pContext, UInt iResidentSize );
    Void _MemoryContext_Cleanup( MemoryContext * pContext );
    Void _MemoryAllocator_Cleanup( AllocatorData * pAllocatorData );    

    inline Void _SwitchingStack_Push( MemoryContextID idContext, MemoryAllocatorID idAllocator );
    inline Void _SwitchingStack_Pop( MemoryContextID * outContext, MemoryAllocatorID * outAllocator );

    // Allocators Factories
    PoolAllocator m_StackFactory;
    PoolAllocator m_PoolFactory;
    PoolAllocator m_BreakFactory;
    PoolAllocator m_HeapFactory;

    // Contexts
    MemoryContext m_SharedContext;

    UInt m_iContextCount;
    MemoryContext m_arrContexts[MEMORY_TABLE_SIZE];

    MemoryContextID m_idSelected;

    // Switching stack
    UInt m_iSwitchingStackTop;
    SwitchStackItem m_arrSwitchingStack[MEMORYSWITCH_MAX];

    // Tracing system
    Void _Tracing_Record( Bool bIsAlloc, MemoryContextID idContext, MemoryAllocatorID idAllocator,
                          Byte * pAddress, UInt iSize, Bool bIsArray,
                          const GChar * strFileName, UInt iFileLine );
    Void _Tracing_LogAndFlush();

    TraceRecord m_arrTraceRecords[MEMORYTRACE_MAX];
    UInt m_iTraceCount;
    TraceFilter m_iTraceFilter;
    MemoryContextID m_idTraceContext;
    MemoryAllocatorID m_idTraceAllocator;
    Bool m_bTracing;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MemoryManager.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MEMORY_MEMORYMANAGER_H
