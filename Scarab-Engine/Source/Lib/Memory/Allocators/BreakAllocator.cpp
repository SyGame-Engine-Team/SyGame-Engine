/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Memory/Allocators/BreakAllocator.cpp
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
// Includes
#include "BreakAllocator.h"

///////////////////////////////////////////////////////////////////////////////
// BreakAllocator implementation
BreakAllocator::BreakAllocator( UInt iContextID, const GChar * strContextName,
                                UInt iAllocatorID, const GChar * strAllocatorName ):
    MemoryAllocator( iContextID, strContextName, iAllocatorID, strAllocatorName )
{
    m_pMemoryRange = NULL;
}
BreakAllocator::~BreakAllocator()
{
    if ( m_pMemoryRange != NULL )
        SystemFn->MemFree( m_pMemoryRange );
}

Void BreakAllocator::Initialize( UInt /*iBlockSize*/, UInt /*iRangeSize*/ )
{
    ////////////////////////////
}
Void BreakAllocator::Cleanup()
{
    ////////////////////////////
}

Byte * BreakAllocator::Allocate( UInt /*iSize*/ )
{
    ////////////////////////////
    return NULL;
}
Void BreakAllocator::Free( Byte * /*pMemory*/ )
{
    ////////////////////////////
}

Void BreakAllocator::GenerateReport( AllocatorReport * /*outReport*/ ) const
{
    ////////////////////////////
}
Void BreakAllocator::LogReport( const AllocatorReport * /*pReport*/ ) const
{
    ////////////////////////////
}

//Void MemoryManager::ChunkRequest( UInt iMinFreeSize, MemoryContextID idContext )
//{
//    if (idContext == INVALID_OFFSET)
//        idContext = m_iCurrentContext;
//
//    Assert(idContext != INVALID_OFFSET);
//    MemoryContext * pContext = _GetContext(idContext);
//    Assert(pContext->bHasLocation[MEMORY_CHUNK]);
//
//    Assert( iMinFreeSize <= (pContext->chunkAlloc.ChunkSize() - sizeof(ChunkBreakHeader)) );
//
//    // Create first chunk if needed
//    if ( pContext->pCurrentChunk == NULL ) {
//        pContext->pFirstChunk = (ChunkBreakHeader*)( pContext->chunkAlloc.Allocate() );
//        if ( pContext->pFirstChunk == NULL ) {
//            ErrorFn->Throw(KERROR_OUT_OF_MEMORY);
//            return;
//        }
//        pContext->pFirstChunk->pPrev = NULL;
//        pContext->pFirstChunk->pNext = NULL;
//        pContext->pFirstChunk->iBreak = sizeof(ChunkBreakHeader);
//        pContext->pCurrentChunk = pContext->pFirstChunk;
//        return;
//    }
//
//    // Check if we fit in current chunk
//    if ( pContext->pCurrentChunk->iBreak + iMinFreeSize <= pContext->chunkAlloc.ChunkSize() )
//        return;
//
//    // We don't fit, try to re-use chunks first
//    ChunkBreakHeader * pChunk = pContext->pFirstChunk;
//    while(pChunk != NULL) {
//        if ( pChunk->iBreak + iMinFreeSize <= pContext->chunkAlloc.ChunkSize() ) {
//            pContext->pCurrentChunk = pChunk;
//            return;
//        }
//        if (pChunk->pNext == NULL) // save last chunk
//            pContext->pCurrentChunk = pChunk;
//        pChunk = pChunk->pNext;
//    }
//
//    // No fit found, a new chunk is needed
//    ChunkBreakHeader * pNewChunk = (ChunkBreakHeader*)( pContext->chunkAlloc.Allocate() );
//    if ( pNewChunk == NULL ) {
//        ErrorFn->Throw(KERROR_OUT_OF_MEMORY);
//        return;
//    }
//    pNewChunk->pPrev = pContext->pCurrentChunk;
//    pNewChunk->pNext = NULL;
//    pNewChunk->iBreak = sizeof(ChunkBreakHeader);
//    pContext->pCurrentChunk->pNext = pNewChunk;
//    pContext->pCurrentChunk = pNewChunk;
//}
//Void MemoryManager::ChunkRelease( MemoryContextID idContext )
//{
//    if (idContext == INVALID_OFFSET)
//        idContext = m_iCurrentContext;
//
//    Assert(idContext != INVALID_OFFSET);
//    MemoryContext * pContext = _GetContext(idContext);
//    Assert(pContext->bHasLocation[MEMORY_CHUNK]);
//
//    // Empty list
//    if (pContext->pCurrentChunk == NULL)
//        return;
//
//    // Remove head
//    if (pContext->pCurrentChunk == pContext->pFirstChunk) {
//        if (pContext->pFirstChunk->pNext == NULL) {
//            pContext->chunkAlloc.Free( (Byte*)(pContext->pFirstChunk) );
//            pContext->pFirstChunk = NULL;
//            pContext->pCurrentChunk = NULL;
//        } else {
//            pContext->pFirstChunk = pContext->pFirstChunk->pNext;
//            pContext->chunkAlloc.Free( (Byte*)(pContext->pFirstChunk->pPrev) );
//            pContext->pFirstChunk->pPrev = NULL;
//            pContext->pCurrentChunk = pContext->pFirstChunk;
//        }
//        return;
//    }
//
//    // Remove middle/tail
//    ChunkBreakHeader * pPrev = pContext->pCurrentChunk->pPrev;
//    if (pContext->pCurrentChunk->pNext != NULL)
//        pContext->pCurrentChunk->pNext->pPrev = pPrev;
//    pPrev->pNext = pContext->pCurrentChunk->pNext;
//    pContext->chunkAlloc.Free( (Byte*)(pContext->pCurrentChunk) );
//    pContext->pCurrentChunk = pPrev;
//}
//Void MemoryManager::ChunkFirst( MemoryContextID idContext )
//{
//    if (idContext == INVALID_OFFSET)
//        idContext = m_iCurrentContext;
//
//    Assert(idContext != INVALID_OFFSET);
//    MemoryContext * pContext = _GetContext(idContext);
//    Assert(pContext->bHasLocation[MEMORY_CHUNK]);
//
//    Assert( pContext->pFirstChunk != NULL );
//
//    pContext->pCurrentChunk = pContext->pFirstChunk;
//}
//Void MemoryManager::ChunkPrev( MemoryContextID idContext )
//{
//    if (idContext == INVALID_OFFSET)
//        idContext = m_iCurrentContext;
//
//    Assert(idContext != INVALID_OFFSET);
//    MemoryContext * pContext = _GetContext(idContext);
//    Assert(pContext->bHasLocation[MEMORY_CHUNK]);
//
//    Assert( pContext->pCurrentChunk != NULL );
//
//    if (pContext->pCurrentChunk->pPrev != NULL)
//        pContext->pCurrentChunk = pContext->pCurrentChunk->pPrev;
//}
//Void MemoryManager::ChunkNext( MemoryContextID idContext )
//{
//    if (idContext == INVALID_OFFSET)
//        idContext = m_iCurrentContext;
//
//    Assert(idContext != INVALID_OFFSET);
//    MemoryContext * pContext = _GetContext(idContext);
//    Assert(pContext->bHasLocation[MEMORY_CHUNK]);
//
//    Assert( pContext->pCurrentChunk != NULL );
//
//    if (pContext->pCurrentChunk->pNext != NULL)
//        pContext->pCurrentChunk = pContext->pCurrentChunk->pNext;
//}
//Void MemoryManager::ChunkSelect( UInt iHistoryIndex, MemoryContextID idContext )
//{
//    if (idContext == INVALID_OFFSET)
//        idContext = m_iCurrentContext;
//
//    Assert(idContext != INVALID_OFFSET);
//    MemoryContext * pContext = _GetContext(idContext);
//    Assert(pContext->bHasLocation[MEMORY_CHUNK]);
//
//    Assert( pContext->pFirstChunk != NULL );
//
//    pContext->pCurrentChunk = pContext->pFirstChunk;
//    for(UInt i = 0; i < iHistoryIndex; ++i) {
//        if (pContext->pCurrentChunk->pNext == NULL)
//            return; // stick to last
//        pContext->pCurrentChunk = pContext->pCurrentChunk->pNext;
//    }
//}
//Byte * MemoryManager::ChunkBreak( UInt iSize, MemoryContextID idContext )
//{
//    if (idContext == INVALID_OFFSET)
//        idContext = m_iCurrentContext;
//
//    Assert(idContext != INVALID_OFFSET);
//    MemoryContext * pContext = _GetContext(idContext);
//    Assert(pContext->bHasLocation[MEMORY_CHUNK]);
//
//    Assert( pContext->pCurrentChunk != NULL );
//    if ( pContext->pCurrentChunk->iBreak + iSize > pContext->chunkAlloc.ChunkSize() ) {
//        ErrorFn->Throw(KERROR_OUT_OF_MEMORY);
//        return NULL;
//    }
//
//    Byte * pPtr = ( (Byte*)(pContext->pCurrentChunk) ) + pContext->pCurrentChunk->iBreak;
//    pContext->pCurrentChunk->iBreak += iSize;
//    return pPtr;
//}
//Void MemoryManager::ChunkUnbreak( Byte * pMemory, MemoryContextID idContext )
//{
//    if (idContext == INVALID_OFFSET)
//        idContext = m_iCurrentContext;
//
//    Assert(idContext != INVALID_OFFSET);
//    MemoryContext * pContext = _GetContext(idContext);
//    Assert(pContext->bHasLocation[MEMORY_CHUNK]);
//
//    Assert( pContext->pCurrentChunk != NULL );
//    Assert( pMemory >= ( (Byte*)(pContext->pCurrentChunk) + sizeof(ChunkBreakHeader) ) );
//    Assert( pMemory < ( (Byte*)(pContext->pCurrentChunk) + pContext->pCurrentChunk->iBreak ) );
//
//    pContext->pCurrentChunk->iBreak = (UInt)( pMemory - (Byte*)(pContext->pCurrentChunk) );
//}


//Void MemoryManager::GenerateReport( ChunkReport * pReport, Bool bLogToFile, MemoryContextID idContext )
//{
//    Assert(pReport != NULL);
//
//    if (idContext == INVALID_OFFSET)
//        idContext = m_iCurrentContext;
//
//    Assert(idContext != INVALID_OFFSET);
//    MemoryContext * pContext = _GetContext(idContext);
//    Assert(pContext->bHasLocation[MEMORY_CHUNK]);
//
//    pReport->idContext = pContext->idContext;
//    pReport->strContextName = pContext->strName;
//    pReport->iChunkSize = pContext->chunkAlloc.ChunkSize();
//    pReport->iAllocatedChunks = pContext->chunkAlloc.ChunkCount();
//    pReport->iFreeChunks = (pContext->chunkAlloc.ChunkTotal() - pReport->iAllocatedChunks);
//    pReport->arrBreakList = (UInt*)( m_reportAlloc.Allocate(pReport->iAllocatedChunks * sizeof(UInt)) );
//    ChunkBreakHeader * pChunk = pContext->pFirstChunk;
//    UInt iIndex = 0;
//    pReport->iCurrentChunk = INVALID_OFFSET;
//    while(pChunk != NULL) {
//        pReport->arrBreakList[iIndex] = pChunk->iBreak;
//        if (pChunk == pContext->pCurrentChunk)
//            pReport->iCurrentChunk = iIndex;
//        pChunk = pChunk->pNext;
//        ++iIndex;
//    }
//
//    if (bLogToFile)
//        _LogReport(pReport);
//}

//Void MemoryManager::_LogReport( const ChunkReport * pReport )
//{
//    ErrorFn->Log (
//        MEMORY_MANAGER_LOGFILE_REPORTS, sm_strChunkReportFormat,
//        pReport->strContextName,
//        pReport->iChunkSize, pReport->iAllocatedChunks, pReport->iFreeChunks, pReport->iCurrentChunk
//    );
//
//    ErrorFn->Log( MEMORY_MANAGER_LOGFILE_REPORTS, TEXT("\n => Chunks BreakList :") );
//    for(UInt i = 0; i < pReport->iAllocatedChunks; ++i) {
//        ErrorFn->Log( MEMORY_MANAGER_LOGFILE_REPORTS, TEXT(" %ud"), pReport->arrBreakList[i] );
//    }
//
//    ErrorFn->Log( MEMORY_MANAGER_LOGFILE_REPORTS, TEXT("\n\n") );
//}