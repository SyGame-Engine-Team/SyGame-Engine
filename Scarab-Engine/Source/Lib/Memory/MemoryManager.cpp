/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Memory/MemoryManager.cpp
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
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "MemoryManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Wrappers
Bool _dat_delete_save( _dat * pSaved, const GChar * strFile, UInt iLine )
{
    _dat * pDAT = _dat_get_ptr();
    Bool bValid = pDAT->bIsValid;
    if (bValid) {
        pSaved->strFile = pDAT->strFile;
        pSaved->iLine = pDAT->iLine;
        pSaved->bIsValid = pDAT->bIsValid;
    }
    pDAT->strFile = strFile;
    pDAT->iLine = iLine;
    pDAT->bIsValid = true;
    return bValid;
}
Void _dat_delete_restore( _dat * pSaved, Bool bValid )
{
    _dat * pDAT = _dat_get_ptr();
    if (bValid) {
        pDAT->strFile = pSaved->strFile;
        pDAT->iLine = pSaved->iLine;
        pDAT->bIsValid = pSaved->bIsValid;
    } else
        pDAT->bIsValid = false;
}

Byte * _managed_new( Bool bIsArray, UInt iSize, const GChar * strFile, UInt iLine )
{
    MemoryManager * pMemoryManager = MemoryManager::GetInstancePtr();
    MemoryAllocator * pAllocator = pMemoryManager->_GetAllocator();

    Byte * pAllocated = pAllocator->Allocate( iSize );
    Assert(pAllocated != NULL);

    if ( pMemoryManager->m_bTracing ) {
        MemoryContextID idContext = pMemoryManager->GetSelectedContextID();
        MemoryAllocatorID idAllocator = pMemoryManager->GetSelectedAllocatorID();

        Bool bTrace = false;
        if ( pMemoryManager->m_iTraceFilter == MEMORY_TRACE_CONTEXT ) {
            bTrace = (pMemoryManager->m_idTraceContext == idContext);
        } else if ( pMemoryManager->m_iTraceFilter == MEMORY_TRACE_ALLOCATOR ) {
            bTrace = ( (pMemoryManager->m_idTraceContext == idContext)
                       && (pMemoryManager->m_idTraceAllocator == idAllocator) );
        } else // MEMORY_TRACE_ALL
            bTrace = true;

        if (bTrace) {
            pMemoryManager->_Tracing_Record( true, idContext, idAllocator, pAllocated, iSize, bIsArray, strFile, iLine );
        }
    }

    return pAllocated;
}
Void _managed_delete( Bool bIsArray, Byte * pAllocated, const GChar * strFile, UInt iLine )
{
    MemoryManager * pMemoryManager = MemoryManager::GetInstancePtr();
    MemoryAllocator * pAllocator = pMemoryManager->_GetAllocator();

    if ( pMemoryManager->m_bTracing ) {
        MemoryContextID idContext = pMemoryManager->GetSelectedContextID();
        MemoryAllocatorID idAllocator = pMemoryManager->GetSelectedAllocatorID();

        Bool bTrace = false;
        if ( pMemoryManager->m_iTraceFilter == MEMORY_TRACE_CONTEXT ) {
            bTrace = (pMemoryManager->m_idTraceContext == idContext);
        } else if ( pMemoryManager->m_iTraceFilter == MEMORY_TRACE_ALLOCATOR ) {
            bTrace = ( (pMemoryManager->m_idTraceContext == idContext)
                       && (pMemoryManager->m_idTraceAllocator == idAllocator) );
        } else // MEMORY_TRACE_ALL
            bTrace = true;

        if (bTrace) {
            UInt iSize = pAllocator->GetBlockSize(pAllocated);
            pMemoryManager->_Tracing_Record( false, idContext, idAllocator, pAllocated, iSize, bIsArray, strFile, iLine );
        }
    }

    pAllocator->Free( pAllocated );
}

Void * operator new ( UInt iSize ) {
    return SystemFn->MemAlloc( iSize );
}
Void * operator new[] ( UInt iSize ) {
    return SystemFn->MemAlloc( iSize );
}
Void operator delete ( Void * pAddress ) {
    _dat * pDAT = _dat_get_ptr();
    if ( pDAT->bIsValid )
        _managed_delete( false, (Byte*)pAddress, pDAT->strFile, pDAT->iLine );
    else
        SystemFn->MemFree( pAddress );
}
Void operator delete[] ( Void * pAddress ) {
    _dat * pDAT = _dat_get_ptr();
    if ( pDAT->bIsValid )
        _managed_delete( true, (Byte*)pAddress, pDAT->strFile, pDAT->iLine );
    else
        SystemFn->MemFree( pAddress );
}

/////////////////////////////////////////////////////////////////////////////////
// MemoryManager implementation
MemoryManager * MemoryManager::sm_pInstance = NULL;

MemoryManager::MemoryManager():
    m_StackFactory( INVALID_OFFSET, TEXT("_Manager_Factories_"), INVALID_OFFSET, TEXT("_ManagerFactory_Stack_") ),
    m_PoolFactory( INVALID_OFFSET, TEXT("_Manager_Factories_"), INVALID_OFFSET, TEXT("_ManagerFactory_Pool_") ),
    m_BreakFactory( INVALID_OFFSET, TEXT("_Manager_Factories_"), INVALID_OFFSET, TEXT("_ManagerFactory_Break_") ),
    m_HeapFactory( INVALID_OFFSET, TEXT("_Manager_Factories_"), INVALID_OFFSET, TEXT("_ManagerFactory_Heap_") )
{
    UInt i;

    // Init Factories
    m_StackFactory.Initialize( sizeof(StackAllocator), MEMORY_MAX_ALLOCATORS );
    m_PoolFactory.Initialize( sizeof(PoolAllocator), MEMORY_MAX_ALLOCATORS );
    m_BreakFactory.Initialize( sizeof(BreakAllocator), MEMORY_MAX_ALLOCATORS );
    m_HeapFactory.Initialize( sizeof(HeapAllocator), MEMORY_MAX_ALLOCATORS );

    // Init contexts
    m_iContextCount = 0;
    for( i = 0; i < MEMORY_TABLE_SIZE; ++i ) {
        m_arrContexts[i].idContext = INVALID_OFFSET;
        m_arrContexts[i].strName[0] = NULLBYTE;
    }
    m_idSelected = MEMORY_CONTEXT_SHARED;

    // Init shared context
    m_SharedContext.idContext = MEMORY_CONTEXT_SHARED;
    StringFn->Copy( m_SharedContext.strName, TEXT("Shared") );
    _MemoryContext_Initialize( &(m_SharedContext), 0 );
    m_SharedContext.idSelected = CreateHeap( TEXT("Scratch"), MEMORY_SHARED_SCRATCH_SIZE, MEMORY_CONTEXT_SHARED );

    // Init SwitchingStack
    m_arrSwitchingStack[0].idContext = MEMORY_CONTEXT_SHARED;
    m_arrSwitchingStack[0].idAlloc = m_SharedContext.idSelected;
    m_iSwitchingStackTop = 1;
    for( i = 1; i < MEMORYSWITCH_MAX; ++i ) {
        m_arrSwitchingStack[i].idContext = INVALID_OFFSET;
        m_arrSwitchingStack[i].idAlloc = INVALID_OFFSET;
    }

    // Init tracing
    m_iTraceCount = 0;
    m_iTraceFilter = MEMORY_TRACE_ALL;
    m_idTraceContext = INVALID_OFFSET;
    m_idTraceAllocator = INVALID_OFFSET;
    m_bTracing = false;

    // Init directory for logging
    SystemFn->CreateDirectory( TEXT("Logs/Memory") );
}
MemoryManager::~MemoryManager()
{
    // Destroy tracing
    // nothing to do

    // Check SwitchingStack
    Assert( m_iSwitchingStackTop == 1 );

    // Destroy shared context
    DestroyAllocator( TEXT("Scratch"), MEMORY_CONTEXT_SHARED );
    _MemoryContext_Cleanup( &(m_SharedContext) );

    // This should not happen and denotes bad code behaviour ...
    // All allocators inside a context should have been cleaned
    // before the context itself ...
    if ( m_iContextCount > 0 ) {
        for( UInt i = 0; i < MEMORY_TABLE_SIZE; ++i ) {
            if ( m_arrContexts[i].idContext == i )
                _MemoryContext_Cleanup( &(m_arrContexts[i]) );
        }
    }

    // Destroy Factories
    m_StackFactory.Cleanup();
    m_PoolFactory.Cleanup();
    m_BreakFactory.Cleanup();
    m_HeapFactory.Cleanup();
}

MemoryContextID MemoryManager::CreateContext( const GChar * strName, UInt iResidentSize, Byte ** outResidentPointer )
{
    Assert( m_iContextCount < MEMORY_TABLE_SIZE );

    MemoryContextID idContext = _MemoryContext_Add( strName );
    MemoryContext * pContext = _MemoryContext_Get( idContext );
    _MemoryContext_Initialize( pContext, iResidentSize );

    *outResidentPointer = m_arrContexts[idContext].pResidentMemory;
    return idContext;
}
Void MemoryManager::DestroyContext( MemoryContextID idContext )
{
    Assert( idContext != MEMORY_CONTEXT_SHARED );

    // This should not happen and denotes bad code behaviour ...
    // Select another context before destroying else fall-back
    // to Shared context by default ...
    if ( m_idSelected == idContext )
        m_idSelected = MEMORY_CONTEXT_SHARED;

    MemoryContext * pContext = _MemoryContext_Get( idContext );
    _MemoryContext_Cleanup( pContext );
    _MemoryContext_Remove( idContext );
}
Void MemoryManager::DestroyContext( const GChar * strName )
{
    MemoryContextID idContext = _HashName( strName, MEMORY_TABLE_SIZE );
    Assert( m_arrContexts[idContext].idContext == idContext );

    // This should not happen and denotes bad code behaviour ...
    // Select another context before destroying else fall-back
    // to Shared context by default ...
    if ( m_idSelected == idContext )
        m_idSelected = MEMORY_CONTEXT_SHARED;

    MemoryContext * pContext = _MemoryContext_Get( idContext );
    _MemoryContext_Cleanup( pContext );
    _MemoryContext_Remove( idContext );
}

MemoryAllocatorID MemoryManager::CreateStack( const GChar * strName, UInt iStackSize, MemoryContextID idContext )
{
    if ( idContext == INVALID_OFFSET )
        idContext = m_idSelected;
    MemoryContext * pContext = _MemoryContext_Get(idContext);
    Assert( pContext->iAllocatorCount < MEMORY_CONTEXT_TABLE_SIZE );

    MemoryAllocatorID idStack = _MemoryAllocator_Add( pContext, strName );

    Byte * pMemory = m_StackFactory.Allocate();
    StackAllocator * pStack = new(pMemory) StackAllocator( idContext, pContext->strName, idStack, strName );
    pStack->Initialize( iStackSize );
    pContext->arrAllocators[idStack].pAllocator = (MemoryAllocator*)pStack;

    return idStack;
}
MemoryAllocatorID MemoryManager::CreatePool( const GChar * strName, UInt iChunkSize, UInt iTotalChunks, MemoryContextID idContext )
{
    if ( idContext == INVALID_OFFSET )
        idContext = m_idSelected;
    MemoryContext * pContext = _MemoryContext_Get(idContext);
    Assert( pContext->iAllocatorCount < MEMORY_CONTEXT_TABLE_SIZE );

    MemoryAllocatorID idPool = _MemoryAllocator_Add( pContext, strName );

    Byte * pMemory = m_PoolFactory.Allocate();
    PoolAllocator * pPool = new(pMemory) PoolAllocator( idContext, pContext->strName, idPool, strName );
    pPool->Initialize( iChunkSize, iTotalChunks );
    pContext->arrAllocators[idPool].pAllocator = (MemoryAllocator*)pPool;

    return idPool;
}
MemoryAllocatorID MemoryManager::CreateBreak( const GChar * strName, UInt iBlockSize, UInt iRangeSize, MemoryContextID idContext )
{
    if ( idContext == INVALID_OFFSET )
        idContext = m_idSelected;
    MemoryContext * pContext = _MemoryContext_Get(idContext);
    Assert( pContext->iAllocatorCount < MEMORY_CONTEXT_TABLE_SIZE );

    MemoryAllocatorID idBreak = _MemoryAllocator_Add( pContext, strName );

    Byte * pMemory = m_BreakFactory.Allocate();
    BreakAllocator * pBreak = new(pMemory) BreakAllocator( idContext, pContext->strName, idBreak, strName );
    pBreak->Initialize( iBlockSize, iRangeSize );
    pContext->arrAllocators[idBreak].pAllocator = (MemoryAllocator*)pBreak;

    return idBreak;
}
MemoryAllocatorID MemoryManager::CreateHeap( const GChar * strName, UInt iHeapSize, MemoryContextID idContext )
{
    if ( idContext == INVALID_OFFSET )
        idContext = m_idSelected;
    MemoryContext * pContext = _MemoryContext_Get(idContext);
    Assert( pContext->iAllocatorCount < MEMORY_CONTEXT_TABLE_SIZE );

    MemoryAllocatorID idHeap = _MemoryAllocator_Add( pContext, strName );

    Byte * pMemory = m_HeapFactory.Allocate();
    HeapAllocator * pHeap = new(pMemory) HeapAllocator( idContext, pContext->strName, idHeap, strName );
    pHeap->Initialize( iHeapSize );
    pContext->arrAllocators[idHeap].pAllocator = (MemoryAllocator*)pHeap;

    return idHeap;
}
Void MemoryManager::DestroyAllocator( MemoryAllocatorID idAllocator, MemoryContextID idContext )
{
    if ( idContext == INVALID_OFFSET )
        idContext = m_idSelected;
    MemoryContext * pContext = _MemoryContext_Get(idContext);

    AllocatorData * pAllocator = _MemoryAllocator_Get( pContext, idAllocator );
    _MemoryAllocator_Cleanup( pAllocator );
    _MemoryAllocator_Remove( pContext, idAllocator );
}
Void MemoryManager::DestroyAllocator( const GChar * strName, MemoryContextID idContext )
{
    if ( idContext == INVALID_OFFSET )
        idContext = m_idSelected;
    MemoryContext * pContext = _MemoryContext_Get(idContext);

    MemoryAllocatorID idAllocator = _HashName( strName, MEMORY_CONTEXT_TABLE_SIZE );
    Assert( pContext->arrAllocators[idAllocator].idAlloc == idAllocator );

    AllocatorData * pAllocator = _MemoryAllocator_Get( pContext, idAllocator );
    _MemoryAllocator_Cleanup( pAllocator );
    _MemoryAllocator_Remove( pContext, idAllocator );
}

Void MemoryManager::SelectMemory( MemoryAllocatorID idAllocator, MemoryContextID idContext )
{
    if ( idContext == INVALID_OFFSET )
        idContext = m_idSelected;
    MemoryContext * pContext = _MemoryContext_Get(idContext);
    Assert( idAllocator < MEMORY_CONTEXT_TABLE_SIZE );
    Assert( pContext->arrAllocators[idAllocator].idAlloc == idAllocator );

    MemoryContext * pSelectedContext = _MemoryContext_Get(m_idSelected);
    _SwitchingStack_Push( m_idSelected, pSelectedContext->idSelected );

    m_idSelected = idContext;
    pContext->idSelected = idAllocator;
}
Void MemoryManager::SelectMemory( const GChar * strAllocatorName, MemoryContextID idContext )
{
    if ( idContext == INVALID_OFFSET )
        idContext = m_idSelected;
    MemoryContext * pContext = _MemoryContext_Get(idContext);
    MemoryAllocatorID idAllocator = _HashName( strAllocatorName, MEMORY_CONTEXT_TABLE_SIZE );
    Assert( pContext->arrAllocators[idAllocator].idAlloc == idAllocator );

    MemoryContext * pSelectedContext = _MemoryContext_Get(m_idSelected);
    _SwitchingStack_Push( m_idSelected, pSelectedContext->idSelected );

    m_idSelected = idContext;
    pContext->idSelected = idAllocator;
}
Void MemoryManager::SelectMemory( MemoryAllocatorID idAllocator, const GChar * strContextName )
{
    MemoryContextID idContext;
    if ( strContextName == NULL )
        idContext = m_idSelected;
    else {
        idContext = _HashName( strContextName, MEMORY_TABLE_SIZE );
        Assert( m_arrContexts[idContext].idContext == idContext );
    }
    MemoryContext * pContext = _MemoryContext_Get(idContext);
    Assert( idAllocator < MEMORY_CONTEXT_TABLE_SIZE );
    Assert( pContext->arrAllocators[idAllocator].idAlloc == idAllocator );

    MemoryContext * pSelectedContext = _MemoryContext_Get(m_idSelected);
    _SwitchingStack_Push( m_idSelected, pSelectedContext->idSelected );

    m_idSelected = idContext;
    pContext->idSelected = idAllocator;
}
Void MemoryManager::SelectMemory( const GChar * strAllocatorName, const GChar * strContextName )
{
    MemoryContextID idContext;
    if ( strContextName == NULL )
        idContext = m_idSelected;
    else {
        idContext = _HashName( strContextName, MEMORY_TABLE_SIZE );
        Assert( m_arrContexts[idContext].idContext == idContext );
    }
    MemoryContext * pContext = _MemoryContext_Get(idContext);
    MemoryAllocatorID idAllocator = _HashName( strAllocatorName, MEMORY_CONTEXT_TABLE_SIZE );
    Assert( pContext->arrAllocators[idAllocator].idAlloc == idAllocator );

    MemoryContext * pSelectedContext = _MemoryContext_Get(m_idSelected);
    _SwitchingStack_Push( m_idSelected, pSelectedContext->idSelected );

    m_idSelected = idContext;
    pContext->idSelected = idAllocator;
}
Void MemoryManager::UnSelectMemory()
{
    MemoryAllocatorID idAllocator;
    _SwitchingStack_Pop( &m_idSelected, &idAllocator );

    MemoryContext * pSelectedContext = _MemoryContext_Get(m_idSelected);
    pSelectedContext->idSelected = idAllocator;
}

Void MemoryManager::LogSelectionState()
{
    // Log
    HFile logFile = SystemFn->OpenFile( MEMORYSWITCH_LOGFILE, FILE_WRITE );
    Assert( logFile.IsValid() );
    Bool bOk = logFile.Seek( FILE_SEEK_END, 0 );
    Assert( bOk );

    // Add Timestamps ? maybe usefull later ...
    ErrorFn->Log( logFile, TEXT("Switching Stack State :") );

    MemoryContextID idContext;
    MemoryContext * pContext;
    MemoryAllocatorID idAllocator;
    const GChar * strContextName;
    const GChar * strAllocatorName;
    for( UInt i = 0; i < m_iSwitchingStackTop; ++i ) {
        idContext = m_arrSwitchingStack[i].idContext;
        idAllocator = m_arrSwitchingStack[i].idAlloc;
        pContext = _MemoryContext_Get( idContext );
        strContextName = pContext->strName;
        strAllocatorName = pContext->arrAllocators[idAllocator].strName;
        ErrorFn->Log( logFile, TEXT("\n => Level %ud = Context Name : %s (ID = %ud)"), i, strContextName, idContext );
        ErrorFn->Log( logFile, TEXT("\n                Allocator Name : %s (ID = %ud)"), strAllocatorName, idAllocator );
        ErrorFn->Log( logFile, TEXT("\n\n") );
    }

    logFile.Close();
}

/////////////////////////////////////////////////////////////////////////////////

Void MemoryManager::_MemoryContext_Initialize( MemoryContext * pContext, UInt iResidentSize )
{
    pContext->iResidentSize = iResidentSize;
    if (iResidentSize > 0)
        pContext->pResidentMemory = new Byte[iResidentSize];
    else
        pContext->pResidentMemory = NULL;

    pContext->iAllocatorCount = 0;
    for( UInt i = 0; i < MEMORY_CONTEXT_TABLE_SIZE; ++i ) {
        pContext->arrAllocators[i].idAlloc = INVALID_OFFSET;
        pContext->arrAllocators[i].strName[0] = NULLBYTE;
        pContext->arrAllocators[i].pAllocator = NULL;
    }
    pContext->idSelected = INVALID_OFFSET;
}
Void MemoryManager::_MemoryContext_Cleanup( MemoryContext * pContext )
{
    // This should not happen and denotes bad code behaviour ...
    // All allocators inside a context should have been cleaned
    // before the context itself ...
    if ( pContext->iAllocatorCount > 0 ) {
        for( UInt i = 0; i < MEMORY_CONTEXT_TABLE_SIZE; ++i ) {
            if ( pContext->arrAllocators[i].idAlloc == i )
                _MemoryAllocator_Cleanup( &(pContext->arrAllocators[i]) );
        }
    }

    if ( pContext->pResidentMemory != NULL ) {
        delete[] pContext->pResidentMemory;
        pContext->pResidentMemory = NULL;
    }
}
Void MemoryManager::_MemoryAllocator_Cleanup( AllocatorData * pAllocatorData )
{
    MemoryAllocator * pAllocator = pAllocatorData->pAllocator;
    switch( pAllocator->GetType() ) {
        case ALLOCATOR_STACK:
            ((StackAllocator*)pAllocator)->Cleanup();
            ((StackAllocator*)pAllocator)->~StackAllocator();
            m_StackFactory.Free( (Byte*)pAllocator );
            break;
        case ALLOCATOR_POOL:
            ((PoolAllocator*)pAllocator)->Cleanup();
            ((PoolAllocator*)pAllocator)->~PoolAllocator();
            m_PoolFactory.Free( (Byte*)pAllocator );
            break;
        case ALLOCATOR_BREAK:
            ((BreakAllocator*)pAllocator)->Cleanup();
            ((BreakAllocator*)pAllocator)->~BreakAllocator();
            m_BreakFactory.Free( (Byte*)pAllocator );
            break;
        case ALLOCATOR_HEAP:
            ((HeapAllocator*)pAllocator)->Cleanup();
            ((HeapAllocator*)pAllocator)->~HeapAllocator();
            m_HeapFactory.Free( (Byte*)pAllocator );
            break;
        default: Assert(false); return;
    }
    pAllocatorData->pAllocator = NULL;
}

Void MemoryManager::_Tracing_Record( Bool bIsAlloc, MemoryContextID idContext, MemoryAllocatorID idAllocator,
                                     Byte * pAddress, UInt iSize, Bool bIsArray,
                                     const GChar * strFileName, UInt iFileLine )
{
    if ( m_iTraceCount >= MEMORYTRACE_MAX )
        _Tracing_LogAndFlush();
    TraceRecord * pTrace = &(m_arrTraceRecords[m_iTraceCount]);

    MemoryContext * pContext = _MemoryContext_Get(idContext);
    AllocatorData * pAllocatorData = _MemoryAllocator_Get(pContext, idAllocator);
    pTrace->bIsAlloc = bIsAlloc;
    pTrace->idContext = idContext;
    pTrace->strContextName = pContext->strName;
    pTrace->idAllocator = idAllocator;
    pTrace->strAllocatorName = pAllocatorData->strName;
    pTrace->iAllocatorType = pAllocatorData->pAllocator->GetType();
    pTrace->pAddress = pAddress;
    pTrace->iSize = iSize;
    pTrace->bIsArray = bIsArray;
    pTrace->strFileName = strFileName;
    pTrace->iFileLine = iFileLine;

    ++m_iTraceCount;
}
Void MemoryManager::_Tracing_LogAndFlush()
{
    // Log
    HFile logFile = SystemFn->OpenFile( MEMORYTRACE_LOGFILE, FILE_WRITE );
    Assert( logFile.IsValid() );
    Bool bOk = logFile.Seek( FILE_SEEK_END, 0 );
    Assert( bOk );

    GChar * strAllocatorType = NULL;
    for( UInt i = 0; i < m_iTraceCount; ++i ) {
        const TraceRecord * pTrace = &(m_arrTraceRecords[i]);
        switch( pTrace->iAllocatorType ) {
            case ALLOCATOR_STACK: strAllocatorType = TEXT("Stack"); break;
            case ALLOCATOR_POOL:  strAllocatorType = TEXT("Pool"); break;
            case ALLOCATOR_BREAK: strAllocatorType = TEXT("Break"); break;
            case ALLOCATOR_HEAP:  strAllocatorType = TEXT("Heap"); break;
        }

        // Add Timestamps ? maybe usefull later ...
        ErrorFn->Log( logFile, TEXT("Trace Record : %s"), (pTrace->bIsAlloc) ? TEXT("Allocate") : TEXT("Free") );

        ErrorFn->Log( logFile, TEXT("\n => Context ID     : %ud"), pTrace->idContext );
        ErrorFn->Log( logFile, TEXT("\n => Context Name   : %s"), pTrace->strContextName );
        ErrorFn->Log( logFile, TEXT("\n => Allocator ID   : %ud"), pTrace->idAllocator );
        ErrorFn->Log( logFile, TEXT("\n => Allocator Name : %s"), pTrace->strAllocatorName );
        ErrorFn->Log( logFile, TEXT("\n => Allocator Type : %s"), strAllocatorType );
        ErrorFn->Log( logFile, TEXT("\n => Block Address  : %u8x"), pTrace->pAddress );
        ErrorFn->Log( logFile, TEXT("\n => Block Size     : %ud"), pTrace->iSize );
        ErrorFn->Log( logFile, TEXT("\n => Block Is Array : %s"), (pTrace->bIsArray) ? TEXT("Yes") : TEXT("No") );
        ErrorFn->Log( logFile, TEXT("\n => File Name      : %s"), pTrace->strFileName );
        ErrorFn->Log( logFile, TEXT("\n => Line Number    : %ud"), pTrace->iFileLine );

        ErrorFn->Log( logFile, TEXT("\n\n") );
    }

    logFile.Close();

    // Flush
    m_iTraceCount = 0;
}

