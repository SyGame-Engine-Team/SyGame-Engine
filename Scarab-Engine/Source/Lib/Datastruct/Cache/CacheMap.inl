/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Datastruct/Cache/CacheMap.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : CacheMap template definition.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// CacheMap implementation
template<typename Key, typename T>
CacheMap<Key,T>::CacheMap( UInt iCacheSize ):
    Datastruct(), m_mapCache()
{
    m_pfLoad = NULL;
    m_pfUnload = NULL;
    m_pUserData = NULL;

    m_iCacheSize = iCacheSize;
    m_iMostRecentAccess = 0;
}
template<typename Key, typename T>
CacheMap<Key,T>::~CacheMap()
{
    // nothing to do
    // Call Destroy explicitly
}

template<typename Key, typename T>
inline Void CacheMap<Key,T>::UseMemoryContext( MemoryContextID idMemoryContext, const GChar * strAllocatorName ) {
    Datastruct::UseMemoryContext( idMemoryContext, strAllocatorName );
    m_mapCache.UseMemoryContext( idMemoryContext, strAllocatorName );
}

template<typename Key, typename T>
inline Void CacheMap<Key,T>::SetComparator( Comparator pfCompare, Void * pUserData ) {
    m_mapCache.SetComparator( pfCompare, pUserData );
}
template<typename Key, typename T>
inline Void CacheMap<Key,T>::SetLoader( Loader pfLoad, Void * pUserData ) {
    m_pfLoad = pfLoad;
    m_pUserData = pUserData;
}
template<typename Key, typename T>
inline Void CacheMap<Key,T>::SetUnloader( Unloader pfUnload ) {
    m_pfUnload = pfUnload;
}

template<typename Key, typename T>
inline Void CacheMap<Key,T>::Create() {
    Assert( m_pfLoad != NULL );
    m_mapCache.Create();
}
template<typename Key, typename T>
inline Void CacheMap<Key,T>::Destroy() {
    if ( m_pfUnload != NULL ) {
        for( TreeMap<Key,CacheMapNode>::Iterator it = m_mapCache.Begin(); !(it.IsNull()); ++it )
            m_pfUnload( it.GetKey(), it.GetItem().nodeItem, m_pUserData );
    }
    m_mapCache.Destroy();
}

template<typename Key, typename T>
inline UInt CacheMap<Key,T>::MemorySize() const {
    return m_mapCache.MemorySize();
}
template<typename Key, typename T>
inline UInt CacheMap<Key,T>::Count() const {
    return m_mapCache.Count();
}

template<typename Key, typename T>
inline Void CacheMap<Key,T>::Clear() {
    if ( m_pfUnload != NULL ) {
        for( TreeMap<Key,CacheMapNode>::Iterator it = m_mapCache.Begin(); !(it.IsNull()); ++it )
            m_pfUnload( it.GetKey(), it.GetItem().nodeItem, m_pUserData );
    }
    m_mapCache.Clear();
}

template<typename Key, typename T>
inline Bool CacheMap<Key,T>::IsFull() const {
    return ( m_mapCache.Count() >= m_iCacheSize );
}

template<typename Key, typename T>
inline Bool CacheMap<Key,T>::Contains( const Key & rKey ) const {
    return m_mapCache.Contains( rKey );
}

template<typename Key, typename T>
const T & CacheMap<Key,T>::Query( const Key & rKey )
{
    // Query
    TreeMap<Key,CacheMapNode>::Iterator it = m_mapCache.Get( rKey );

    // Cache-Miss case
    if ( it.IsNull() )
        return Load( rKey );

    // Cache-Hit case
    CacheMapNode * pCacheNode = it.GetItemPtr();

    // Update access counter
    pCacheNode->iLastAccess = ++m_iMostRecentAccess;

    return pCacheNode->nodeItem;
}
template<typename Key, typename T>
const T & CacheMap<Key,T>::Load( const Key & rKey )
{
    // Call loader
    CacheMapNode newNode;
    m_pfLoad( &(newNode.nodeItem), rKey, m_pUserData );

    // Check if cache is full
    if ( m_mapCache.Count() >= m_iCacheSize )
        Release();

    // Update access counter
    newNode.iLastAccess = ++m_iMostRecentAccess;

    // Add to cache
    TreeMap<Key,CacheMapNode>::Iterator it;
    Bool bInserted = m_mapCache.Insert( rKey, newNode, &it );
    Assert( bInserted && !(it.IsNull()) );

    // Done
    return it.GetItem().nodeItem;
}

template<typename Key, typename T>
Void CacheMap<Key,T>::Update()
{
    // Call loader on all nodes
    TreeMap<Key,CacheMapNode>::Iterator it;
    CacheMapNode * pCacheNode;

    for( it = m_mapCache.Begin(); !(it.IsNull()); ++it ) {
        pCacheNode = it.GetItemPtr();
        m_pfLoad( &(pCacheNode->nodeItem), it.GetKey(), m_pUserData );
    }
}
template<typename Key, typename T>
Void CacheMap<Key,T>::Release()
{
    UInt iHalfSize = ( m_iCacheSize >> 1 );

    // Create temp array
    _SelectMemory();
    Key * arrRemove = New Key[iHalfSize];
    _UnSelectMemory();

    // Pass 1 : Collect old nodes, Reset access counters
    TreeMap<Key,CacheMapNode>::Iterator it;
    CacheMapNode * pCacheNode;
    UInt iRemoveCount = 0;

    for( it = m_mapCache.Begin(); !(it.IsNull()); ++it ) {
        pCacheNode = it.GetItemPtr();

        // Age estimation from access counters
        if ( (m_iMostRecentAccess - pCacheNode->iLastAccess) > iHalfSize ) {
            if ( iRemoveCount < iHalfSize )
                arrRemove[iRemoveCount++] = it.GetKey();
        }

        // Reset access counter
        pCacheNode->iLastAccess = 0;
    }

    // Pass 2 : Delete collected nodes
    Bool bRemoved;
    CacheMapNode removedNode;
    for( UInt i = 0; i < iRemoveCount; ++i ) {
        bRemoved = m_mapCache.Remove( arrRemove[i], &removedNode );
        Assert( bRemoved );
        if ( m_pfUnload != NULL )
            m_pfUnload( arrRemove[i], removedNode.nodeItem, m_pUserData );
    }

    // Reset access counter
    m_iMostRecentAccess = 0;

    // Destroy temp array
    _SelectMemory();
    DeleteA( arrRemove );
    _UnSelectMemory();

    // Check new size
    Assert( m_mapCache.Count() < m_iCacheSize );
}


