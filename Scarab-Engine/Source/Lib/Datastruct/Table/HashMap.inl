/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Datastruct/Table/HashMap.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : The standard HashMap for generic indexing ...
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs :
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// HashMap implementation
template<typename Key, typename T>
HashMap<Key,T>::HashMap( UInt iPageSize ):
    Datastruct()
{
    m_iPageSize = iPageSize;

    m_iTableSize = 0;
    m_arrTable = NULL;

	m_iEntryCount = 0;
    m_pHead = NULL;
    m_pEnd = NULL;
}
template<typename Key, typename T>
HashMap<Key,T>::~HashMap()
{
	// nothing to do
    // Call Destroy explicitly
}

template<typename Key, typename T>
Void HashMap<Key,T>::Create()
{
    Assert( m_arrTable == NULL );

    m_iTableSize = m_iPageSize;

    _SelectMemory();
    m_arrTable = New HashMapNode[m_iTableSize];
    _UnSelectMemory();

    for( UInt i = 0; i < m_iTableSize; ++i ) {
        m_arrTable[i].bAllocated = false;
        m_arrTable[i].pPrev = NULL;
        m_arrTable[i].pNext = NULL;
    }
    m_iEntryCount = 0;
    m_pHead = NULL;
    m_pEnd = NULL;
}
template<typename Key, typename T>
Void HashMap<Key,T>::Destroy()
{
    Assert( m_arrTable != NULL );

    _SelectMemory();
    DeleteA( m_arrTable );
    _UnSelectMemory();

	m_iTableSize = 0;
    m_arrTable = NULL;

    m_iEntryCount = 0;
    m_pHead = NULL;
    m_pEnd = NULL;
}

template<typename Key, typename T>
inline UInt HashMap<Key,T>::MemorySize() const {
    Assert( m_arrTable != NULL );
    return ( m_iTableSize * sizeof(HashMapNode) );
}
template<typename Key, typename T>
inline UInt HashMap<Key,T>::Count() const {
    Assert( m_arrTable != NULL );
    return m_iEntryCount;
}

template<typename Key, typename T>
Void HashMap<Key,T>::Clear()
{
	Assert( m_arrTable != NULL );

    for( UInt i = 0; i < m_iTableSize; ++i ) {
        m_arrTable[i].bAllocated = false;
        m_arrTable[i].pPrev = NULL;
        m_arrTable[i].pNext = NULL;
    }
    m_iEntryCount = 0;
    m_pHead = NULL;
    m_pEnd = NULL;
}

template<typename Key, typename T>
T & HashMap<Key,T>::operator[]( const Key & rhs )
{
    Assert( m_arrTable != NULL );

	UInt iIndex = _Hash( rhs );
    HashMapNode * pNode = ( m_arrTable + iIndex );

    if ( !(pNode->bAllocated) ) { // auto-vivify
        if ( m_pHead == NULL ) {
            pNode->pPrev = NULL;
            pNode->pNext = NULL;
            m_pHead = pNode;
            m_pEnd = pNode;
        } else {
            pNode->pPrev = m_pEnd;
            pNode->pNext = NULL;
            m_pEnd->pNext = pNode;
            m_pEnd = pNode;
        }

		pNode->bAllocated = true;
		pNode->nodeKey = rhs;
		++m_iEntryCount;

        if ( m_iEntryCount >= ((m_iTableSize * 3) >> 2) ) { // 75% = 3/4
		    _GrowPage();
            iIndex = _Hash( rhs );
            pNode = (m_arrTable + iIndex);
        }
	}
	return pNode->Item;
}
template<typename Key, typename T>
const T & HashMap<Key,T>::operator[]( const Key & rhs ) const
{
    Assert( m_arrTable != NULL );

	UInt iIndex = _Hash( rhs );
    Assert( m_arrTable[iIndex].bAllocated );
	return m_arrTable[iIndex].Item;
}

template<typename Key, typename T>
inline Bool HashMap<Key,T>::Contains( const Key & rKey ) const {
    Assert( m_arrTable != NULL );
    UInt iIndex = _Hash( rKey );
	return m_arrTable[iIndex].bAllocated;
}
template<typename Key, typename T>
inline typename HashMap<Key,T>::Iterator HashMap<Key,T>::Begin() const {
    Assert( m_arrTable != NULL );
    return Iterator(m_pHead);
}
template<typename Key, typename T>
inline typename HashMap<Key,T>::Iterator HashMap<Key,T>::End() const {
    Assert( m_arrTable != NULL );
    return Iterator(m_pEnd);
}

template<typename Key, typename T>
Void HashMap<Key,T>::Insert( const Key & rKey, const T & rData )
{
    Assert( m_arrTable != NULL );

	UInt iIndex = _Hash( rKey );
    HashMapNode * pNode = ( m_arrTable + iIndex );
	if ( pNode->bAllocated ) {
		pNode->Item = rData;
		return;
	}

    if ( m_pHead == NULL ) {
        pNode->pPrev = NULL;
        pNode->pNext = NULL;
        m_pHead = pNode;
        m_pEnd = pNode;
    } else {
        pNode->pPrev = m_pEnd;
        pNode->pNext = NULL;
        m_pEnd->pNext = pNode;
        m_pEnd = pNode;
    }

	pNode->bAllocated = true;
	pNode->nodeKey = rKey;
	pNode->Item = rData;
	++m_iEntryCount;

    if ( m_iEntryCount >= ((m_iTableSize * 3) >> 2) ) // 75% = 3/4
		_GrowPage();
}
template<typename Key, typename T>
Void HashMap<Key,T>::Remove( const Key & rKey )
{
    Assert( m_arrTable != NULL );
    Assert( m_iEntryCount > 0 );

	UInt iIndex = _Hash( rKey );
    HashMapNode * pNode = ( m_arrTable + iIndex );
	if ( !(pNode->bAllocated) )
		return;

    if ( pNode == m_pHead ) {
        if ( pNode->pNext != NULL )
            pNode->pNext->pPrev = NULL;
        else
            m_pEnd = NULL;
        m_pHead = pNode->pNext;
    } else if ( pNode == m_pEnd ) {
        if ( pNode->pPrev != NULL )
            pNode->pPrev->pNext = NULL;
        else
            m_pHead = NULL;
        m_pEnd = pNode->pPrev;
    } else {
        pNode->pNext->pPrev = pNode->pPrev;
        pNode->pPrev->pNext = pNode->pNext;
    }

	pNode->bAllocated = false;
    pNode->pPrev = NULL;
	pNode->pNext = NULL;
	--m_iEntryCount;
}

/////////////////////////////////////////////////////////////////////////////////

template<typename Key>
inline UInt _Hash_Wrapper( const Key & rKey ) {
    return (UInt)( HashFn->BobJenkinsRot( (const Byte *)(&rKey), sizeof(Key) ) );
}
template<>
inline UInt _Hash_Wrapper<KString>( const KString & rKey ) {
    return (UInt)( HashFn->BobJenkinsRot( (const GChar *)rKey ) );
}
template<>
inline UInt _Hash_Wrapper<KCString>( const KCString & rKey ) {
    return (UInt)( HashFn->BobJenkinsRot( (const GChar *)rKey ) );
}

template<typename Key, typename T>
UInt HashMap<Key,T>::_Hash( const Key & rKey ) const
{
    // Hashing
    Float fHash = (Float)( _Hash_Wrapper<Key>(rKey) );
    UInt iHash = (UInt)( (fHash / (Float)UINT_MAX) * (Float)m_iTableSize );

    // Collisions (if any) : Quadratic probing
    Int iIndex;
    UInt i = 0;
    Int iBias = -1;
    while(true) {
        iIndex = iHash + ( 4 * iBias * i * i );
        if ( iIndex < 0 )
            iIndex += m_iTableSize;
        if ( iIndex >= (Int)m_iTableSize )
            iIndex -= m_iTableSize;

        if ( m_arrTable[iIndex].bAllocated == false )
            return (UInt)iIndex;
        if ( m_arrTable[iIndex].nodeKey == rKey )
            return (UInt)iIndex;

        Assert(false); // collision !!
        iBias = -iBias;
        if (iBias > 0) ++i;
    }
	return INVALID_OFFSET;
}
template<typename Key, typename T>
Void HashMap<Key,T>::_GrowPage()
{
    // Save old table
    UInt iOldTableSize = m_iTableSize;
    HashMapNode * pOldTable = m_arrTable;
    HashMapNode * pOldHead = m_pHead;
    HashMapNode * pOldEnd = m_pEnd;

    // Allocate a new one
    m_iTableSize += m_iPageSize;

    _SelectMemory();
    m_arrTable = New HashMapNode[m_iTableSize];
    _UnSelectMemory();

    for( UInt i = 0; i < m_iTableSize; ++i ) {
        m_arrTable[i].pPrev = NULL;
        m_arrTable[i].pNext = NULL;
        m_arrTable[i].bAllocated = false;
    }
    m_pHead = NULL;
    m_pEnd = NULL;

    // Realloc all elements, rebuilding links on-the-fly
    HashMapNode * pOldNode = pOldHead;
    HashMapNode * pOldNextNode = pOldNode->pNext;
    UInt iIndex = _Hash( pOldNode->nodeKey );
    HashMapNode * pNode = ( m_arrTable + iIndex );
    UInt iNextIndex;
    HashMapNode * pNextNode;

    m_pHead = pNode;
    pNode->pPrev = NULL;
    while( pOldNextNode != NULL ) {
        Assert( pOldNode->bAllocated && pOldNextNode->bAllocated );
        iNextIndex = _Hash( pOldNextNode->nodeKey );
        pNextNode = ( m_arrTable + iNextIndex );

        pNode->pNext = pNextNode;
        pNode->bAllocated = true;
        pNode->nodeKey = pOldNode->nodeKey;
        pNode->Item = pOldNode->Item;

        pNextNode->pPrev = pNode;

        pOldNode = pOldNextNode;
        pOldNextNode = pOldNode->pNext;
        iIndex = iNextIndex;
        pNode = pNextNode;
    }
    pNode->pNext = NULL;
    pNode->bAllocated = true;
    pNode->nodeKey = pOldNode->nodeKey;
    pNode->Item = pOldNode->Item;
    m_pEnd = pNode;

    // Done
    _SelectMemory();
    DeleteA( pOldTable );
    _UnSelectMemory();
}
