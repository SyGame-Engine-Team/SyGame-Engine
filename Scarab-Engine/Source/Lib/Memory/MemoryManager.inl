/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Memory/MemoryManager.inl
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
// Wrappers
inline _dat * _dat_get_ptr() {
    static _dat _dat_values = { NULL, 0, false };
    return &(_dat_values);
}

inline Void * operator new ( UInt /*iSize*/, Void * pAddress ) {
    return pAddress;
}
inline Void * operator new[] ( UInt /*iSize*/, Void * pAddress ) {
    return pAddress;
}
inline Void operator delete ( Void * /*pAddress*/, Void * ) {
    // nothing to do
}
inline Void operator delete[] ( Void * /*pAddress*/, Void * ) {
    // nothing to do
}

inline Void * operator new ( UInt iSize, const GChar * strFile, UInt iLine ) {
    return (Void*)_managed_new( false, iSize, strFile, iLine );
}
inline Void * operator new[] ( UInt iSize, const GChar * strFile, UInt iLine ) {
    return (Void*)_managed_new( true, iSize, strFile, iLine );
}
inline Void operator delete ( Void * pAddress , const GChar * strFile, UInt iLine ) {
    _managed_delete( false, (Byte*)pAddress, strFile, iLine );
}
inline Void operator delete[] ( Void * pAddress , const GChar * strFile, UInt iLine ) {
    _managed_delete( true, (Byte*)pAddress, strFile, iLine );
}

/////////////////////////////////////////////////////////////////////////////////
// MemoryManager implementation
inline Void MemoryManager::Create() {
    Assert(sm_pInstance == NULL);
    sm_pInstance = new MemoryManager();
}
inline Void MemoryManager::Destroy() {
    Assert(sm_pInstance != NULL);
    delete sm_pInstance;
    sm_pInstance = NULL;
}
inline MemoryManager & MemoryManager::GetInstance() {
    Assert(sm_pInstance != NULL);
    return (*sm_pInstance);
}
inline MemoryManager * MemoryManager::GetInstancePtr() {
    Assert(sm_pInstance != NULL);
    return (sm_pInstance);
}

inline MemoryContextID MemoryManager::GetContextID( const GChar * strName ) const {
    MemoryContextID idContext = _HashName( strName, MEMORY_TABLE_SIZE );
    Assert( m_arrContexts[idContext].idContext == idContext );
    return idContext;
}
inline const GChar * MemoryManager::GetContextName( MemoryContextID idContext ) const {
    if ( idContext == MEMORY_CONTEXT_SHARED )
        return m_SharedContext.strName;
    else {
        Assert( idContext < MEMORY_TABLE_SIZE );
        Assert( m_arrContexts[idContext].idContext == idContext );
        return m_arrContexts[idContext].strName;
    }
}
inline MemoryContextID MemoryManager::GetSelectedContextID() const {
    return m_idSelected;
}
inline const GChar * MemoryManager::GetSelectedContextName() const {
    if ( m_idSelected == MEMORY_CONTEXT_SHARED )
        return m_SharedContext.strName;
    else
        return m_arrContexts[m_idSelected].strName;
}

inline MemoryAllocatorID MemoryManager::GetAllocatorID( const GChar * strName, MemoryContextID idContext ) {
    if ( idContext == INVALID_OFFSET )
        idContext = m_idSelected;
    MemoryContext * pContext = _MemoryContext_Get(idContext);
    MemoryAllocatorID idAllocator = _HashName( strName, MEMORY_CONTEXT_TABLE_SIZE );
    Assert( pContext->arrAllocators[idAllocator].idAlloc == idAllocator );
    return idAllocator;
}
inline const GChar * MemoryManager::GetAllocatorName( MemoryAllocatorID idAllocator, MemoryContextID idContext ) {
    if ( idContext == INVALID_OFFSET )
        idContext = m_idSelected;
    MemoryContext * pContext = _MemoryContext_Get(idContext);
    Assert( idAllocator < MEMORY_CONTEXT_TABLE_SIZE );
    Assert( pContext->arrAllocators[idAllocator].idAlloc == idAllocator );
    return pContext->arrAllocators[idAllocator].strName;
}
inline MemoryAllocatorID MemoryManager::GetSelectedAllocatorID( MemoryContextID idContext ) {
    if ( idContext == INVALID_OFFSET )
        idContext = m_idSelected;
    MemoryContext * pContext = _MemoryContext_Get(idContext);
    Assert( pContext->idSelected != INVALID_OFFSET );
    return pContext->idSelected;
}
inline const GChar * MemoryManager::GetSelectedAllocatorName( MemoryContextID idContext ) {
    if ( idContext == INVALID_OFFSET )
        idContext = m_idSelected;
    MemoryContext * pContext = _MemoryContext_Get(idContext);
    Assert( pContext->idSelected != INVALID_OFFSET );
    return pContext->arrAllocators[pContext->idSelected].strName;
}

inline Void MemoryManager::TraceStart( TraceFilter iFilter,
                                       MemoryContextID idContext, MemoryAllocatorID idAllocator ) {
    m_iTraceFilter = iFilter;
    if ( m_iTraceFilter == MEMORY_TRACE_CONTEXT ) {
        m_idTraceContext = idContext;
        m_idTraceAllocator = INVALID_OFFSET;
    } else if ( m_iTraceFilter == MEMORY_TRACE_ALLOCATOR ) {
        m_idTraceContext = idContext;
        m_idTraceAllocator = idAllocator;
    } else { // MEMORY_TRACE_ALL
        m_idTraceContext = INVALID_OFFSET;
        m_idTraceAllocator = INVALID_OFFSET;
    }
    m_bTracing = true;
}
inline Void MemoryManager::TraceStop() {
    m_iTraceFilter = MEMORY_TRACE_ALL;
    m_idTraceContext = INVALID_OFFSET;
    m_idTraceAllocator = INVALID_OFFSET;
    m_bTracing = false;
}
inline Void MemoryManager::TraceReset() {
    _Tracing_LogAndFlush();
}

inline UInt MemoryManager::TraceCount() const {
    return m_iTraceCount;
}
inline const TraceRecord * MemoryManager::TracePick( UInt iIndex ) const {
    Assert( iIndex < m_iTraceCount );
    return &(m_arrTraceRecords[iIndex]);
}

inline Void MemoryManager::GenerateReport( AllocatorReport * outReport,
                                           MemoryAllocatorID idAllocator, MemoryContextID idContext ) {
    if ( idContext == INVALID_OFFSET )
        idContext = m_idSelected;
    MemoryContext * pContext = _MemoryContext_Get(idContext);
    if ( idAllocator == INVALID_OFFSET )
        idAllocator = pContext->idSelected;
    Assert( idAllocator != INVALID_OFFSET );
    AllocatorData * pAllocatorData = _MemoryAllocator_Get( pContext, idAllocator );
    pAllocatorData->pAllocator->GenerateReport( outReport );
}
inline Void MemoryManager::LogReport( const AllocatorReport * pReport,
                                      MemoryAllocatorID idAllocator, MemoryContextID idContext ) {
    if ( idContext == INVALID_OFFSET )
        idContext = m_idSelected;
    MemoryContext * pContext = _MemoryContext_Get(idContext);
    if ( idAllocator == INVALID_OFFSET )
        idAllocator = pContext->idSelected;
    Assert( idAllocator != INVALID_OFFSET );
    AllocatorData * pAllocatorData = _MemoryAllocator_Get( pContext, idAllocator );
    pAllocatorData->pAllocator->LogReport( pReport );
}

/////////////////////////////////////////////////////////////////////////////////

inline MemoryAllocator * MemoryManager::_GetAllocator() {
    MemoryContext * pContext = _MemoryContext_Get(m_idSelected);
    Assert( pContext->idSelected != INVALID_OFFSET );
    return _MemoryAllocator_Get( pContext, pContext->idSelected )->pAllocator;
}

inline UInt MemoryManager::_HashName( const GChar * strName, UInt iTableSize ) const {
    DWord dwHash = HashFn->BobJenkinsRot( strName );
    //UInt iBitCount = MathFn->Log2OfPower2( iTableSize );
    //UInt iIndex = 0, i = 0; Int iBits = 32;
    //while( iBits > 0 ) {
    //    iIndex ^= (UInt)( dwHash >> (iBitCount * i) );
    //    ++i; iBits -= iBitCount;
    //}
    //iIndex &= (iTableSize - 1);
    //return iIndex;
    return ( dwHash & (iTableSize - 1) );
}

inline MemoryContext * MemoryManager::_MemoryContext_Get( MemoryContextID idContext ) {
    if ( idContext == MEMORY_CONTEXT_SHARED )
        return &(m_SharedContext);
    else {
        Assert( idContext < MEMORY_TABLE_SIZE );
        Assert( m_arrContexts[idContext].idContext == idContext );
        return &(m_arrContexts[idContext]);
    }
}
inline AllocatorData * MemoryManager::_MemoryAllocator_Get( MemoryContext * pContext, MemoryAllocatorID idAllocator ) {
    Assert( idAllocator < MEMORY_CONTEXT_TABLE_SIZE );
    Assert( pContext->arrAllocators[idAllocator].idAlloc == idAllocator );
    return &(pContext->arrAllocators[idAllocator]);
}

inline MemoryContextID MemoryManager::_MemoryContext_Add( const GChar * strName ) {
    MemoryContextID idContext = _HashName( strName, MEMORY_TABLE_SIZE );
    Assert( m_arrContexts[idContext].idContext == INVALID_OFFSET );

    m_arrContexts[idContext].idContext = idContext;
    StringFn->NCopy( m_arrContexts[idContext].strName, strName, MEMORY_MAX_NAMELENGTH - 1 );
    ++m_iContextCount;
    return idContext;
}
inline MemoryAllocatorID MemoryManager::_MemoryAllocator_Add( MemoryContext * pContext, const GChar * strName ) {
    MemoryAllocatorID idAllocator = _HashName( strName, MEMORY_CONTEXT_TABLE_SIZE );
    Assert( pContext->arrAllocators[idAllocator].idAlloc == INVALID_OFFSET );

    pContext->arrAllocators[idAllocator].idAlloc = idAllocator;
    StringFn->NCopy( pContext->arrAllocators[idAllocator].strName, strName, MEMORY_MAX_NAMELENGTH - 1 );
    ++(pContext->iAllocatorCount);
    return idAllocator;
}
inline Void MemoryManager::_MemoryContext_Remove( MemoryContextID idContext ) {
    m_arrContexts[idContext].idContext = INVALID_OFFSET;
    m_arrContexts[idContext].strName[0] = NULLBYTE;
    --m_iContextCount;
}
inline Void MemoryManager::_MemoryAllocator_Remove( MemoryContext * pContext, MemoryAllocatorID idAllocator ) {
    pContext->arrAllocators[idAllocator].idAlloc = INVALID_OFFSET;
    pContext->arrAllocators[idAllocator].strName[0] = NULLBYTE;
    --(pContext->iAllocatorCount);
}

inline Void MemoryManager::_SwitchingStack_Push( MemoryContextID idContext, MemoryAllocatorID idAllocator ) {
    Assert( m_iSwitchingStackTop < MEMORYSWITCH_MAX );

    m_arrSwitchingStack[m_iSwitchingStackTop].idContext = idContext;
    m_arrSwitchingStack[m_iSwitchingStackTop].idAlloc = idAllocator;
    ++m_iSwitchingStackTop;
}
inline Void MemoryManager::_SwitchingStack_Pop( MemoryContextID * outContext, MemoryAllocatorID * outAllocator ) {
    Assert( m_iSwitchingStackTop > 1 );

    --m_iSwitchingStackTop;
    *outContext = m_arrSwitchingStack[m_iSwitchingStackTop].idContext;
    *outAllocator = m_arrSwitchingStack[m_iSwitchingStackTop].idAlloc;
    m_arrSwitchingStack[m_iSwitchingStackTop].idContext = INVALID_OFFSET;
    m_arrSwitchingStack[m_iSwitchingStackTop].idAlloc = INVALID_OFFSET;
}
