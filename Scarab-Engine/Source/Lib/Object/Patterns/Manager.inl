/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Object/Patterns/Manager.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Generic singleton abstractor for the Manager pattern.
//               Integrated memory management.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Manager implementation
template<class T>
T * Manager<T>::sm_pInstance = NULL;

template<class T>
MemoryContextID Manager<T>::sm_idMemoryContext = INVALID_OFFSET;

template<class T>
Void Manager<T>::Create()
{
    Assert(sm_pInstance == NULL);

    Byte * pResident = NULL;
    sm_idMemoryContext = MemoryFn->CreateContext( T::GetName(), sizeof(T), &pResident );
    sm_pInstance = new(pResident) T();
}
template<class T>
Void Manager<T>::Destroy()
{
    Assert(sm_pInstance != NULL);

    sm_pInstance->~T();
    sm_pInstance = NULL;
    MemoryFn->DestroyContext( sm_idMemoryContext );
    sm_idMemoryContext = INVALID_OFFSET;
}

template<class T>
inline T & Manager<T>::GetInstance() {
    Assert(sm_pInstance != NULL);
    return (*sm_pInstance);
}
template<class T>
inline T * Manager<T>::GetInstancePtr() {
    Assert(sm_pInstance != NULL);
    return sm_pInstance;
}

template<class T>
inline Void Manager<T>::SelectMemory( const GChar * strName ) const {
    MemoryFn->SelectMemory( strName, sm_idMemoryContext );
}
template<class T>
inline Void Manager<T>::UnSelectMemory() const {
    MemoryFn->UnSelectMemory();
}

template<class T>
inline MemoryContextID Manager<T>::GetMemoryContext() {
    return sm_idMemoryContext;
}

template<class T>
Manager<T>::Manager()
{
    m_iAllocatorCount = 0;
    for( UInt i = 0; i < MANAGER_MAX_ALLOCATORS; ++i )
        m_arrAllocators[i] = INVALID_OFFSET;

    _CreateMemoryBudget( T::GetMemoryBudget() );
}
template<class T>
Manager<T>::~Manager()
{
    _DestroyMemoryBudget();
}

/////////////////////////////////////////////////////////////////////////////////

template<class T>
Void Manager<T>::_CreateMemoryBudget( const GChar * strMemoryBudget )
{
    // Parse budget string
    const GChar * pBegin = strMemoryBudget;
    const GChar * pCh = strMemoryBudget;
    while( *pCh != NULLBYTE ) {
        if ( *pCh == TEXT(';') ) {
            Assert( *(pCh-1) == TEXT(')') );
            _ParseBudgetDeclaration( pBegin );
            ++pCh;
            pBegin = pCh;
        }
        ++pCh;
    }
}
template<class T>
Void Manager<T>::_ParseBudgetDeclaration( const GChar * strDecl )
{
    const GChar * pCh = strDecl;

    // Parse Allocator Type
    const GChar * strType = pCh;
    UInt iTypeLen;
    while( *pCh != TEXT(':') )
        ++pCh;
    iTypeLen = (UInt)(pCh - strType);
    ++pCh;

    // Parse Allocator Name
    GChar strName[MEMORY_MAX_NAMELENGTH];
    GChar * pWrite = strName;
    UInt iNameLen = 0;
    while( *pCh != TEXT('(') ) {
        *pWrite++ = *pCh++; ++iNameLen;
    }
    *pWrite = NULLBYTE;
    Assert( iNameLen < MEMORY_MAX_NAMELENGTH );
    ++pCh;

    // Parse Allocator Arguments
    UInt iArgCount = 0;
    UInt iArgs[8]; // never need more than 2
    while( true ) {
        iArgs[iArgCount++] = (UInt)StringFn->ToUInt( pCh, &pCh );
        if ( *pCh == TEXT(')') )
            break;
        Assert( *pCh == TEXT(',') );
        ++pCh;
    }

    // Switch with type
    if ( StringFn->NCmp(strType, TEXT("Stack"), iTypeLen) == 0 ) {
        Assert(iArgCount == 1);
        m_arrAllocators[m_iAllocatorCount++] =
            MemoryFn->CreateStack( strName, iArgs[0], sm_idMemoryContext );
    } else if ( StringFn->NCmp(strType, TEXT("Heap"), iTypeLen) == 0 ) {
        Assert(iArgCount == 1);
        m_arrAllocators[m_iAllocatorCount++] =
            MemoryFn->CreateHeap( strName, iArgs[0], sm_idMemoryContext );
    } else if ( StringFn->NCmp(strType, TEXT("Pool"), iTypeLen) == 0 ) {
        Assert(iArgCount == 2);
        m_arrAllocators[m_iAllocatorCount++] =
            MemoryFn->CreatePool( strName, iArgs[0], iArgs[1], sm_idMemoryContext );
    } else if ( StringFn->NCmp(strType, TEXT("Break"), iTypeLen) == 0 ) {
        Assert(iArgCount == 2);
        m_arrAllocators[m_iAllocatorCount++] =
            MemoryFn->CreateBreak( strName, iArgs[0], iArgs[1], sm_idMemoryContext );
    } else
        Assert(false);
}
template<class T>
Void Manager<T>::_DestroyMemoryBudget()
{
    for( UInt i = 0; i < m_iAllocatorCount; ++i ) {
        MemoryFn->DestroyAllocator( m_arrAllocators[i], sm_idMemoryContext );
        m_arrAllocators[i] = INVALID_OFFSET;
    }
    m_iAllocatorCount = 0;
}
