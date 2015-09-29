/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Datastruct/Table/TreeMap.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : The standard red/black-tree based map implementation
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TreeMap implementation
template<typename Key, typename T>
TreeMap<Key,T>::TreeMap():
    Datastruct(), m_rbTreeMap()
{
    m_pfCompare = NULL;
    m_pUserData = NULL;
}
template<typename Key, typename T>
TreeMap<Key,T>::~TreeMap()
{
    // nothing to do
    // Call Destroy explicitly
}

template<typename Key, typename T>
inline Void TreeMap<Key,T>::UseMemoryContext( MemoryContextID idMemoryContext, const GChar * strAllocatorName ) {
    Datastruct::UseMemoryContext( idMemoryContext, strAllocatorName );
    m_rbTreeMap.UseMemoryContext( idMemoryContext, strAllocatorName );
}

template<typename Key, typename T>
inline Void TreeMap<Key,T>::SetComparator( Comparator pfCompare, Void * pUserData = NULL ) {
    m_pfCompare = pfCompare;
    m_pUserData = pUserData;
    m_rbTreeMap.SetComparator( _NodeCompare, this );
}

template<typename Key, typename T>
inline Void TreeMap<Key,T>::Create() {
    Assert( m_pfCompare != NULL );
    m_rbTreeMap.Create();
}
template<typename Key, typename T>
inline Void TreeMap<Key,T>::Destroy() {
    m_rbTreeMap.Destroy();
}

template<typename Key, typename T>
inline UInt TreeMap<Key,T>::MemorySize() const {
    return m_rbTreeMap.MemorySize();
}
template<typename Key, typename T>
inline UInt TreeMap<Key,T>::Count() const {
    return m_rbTreeMap.Count();
}

template<typename Key, typename T>
inline Void TreeMap<Key,T>::Clear() {
    m_rbTreeMap.Clear();
}

template<typename Key, typename T>
T & TreeMap<Key,T>::operator[]( const Key & rhs )
{
    RedBlackTree<TreeMapNode>::Iterator itFoundNode;

    // Build search node
    TreeMapNode searchNode;
    searchNode.nodeKey = rhs;

    // Search existing key
    Bool bFound = m_rbTreeMap.Search( searchNode, &itFoundNode );
    if ( bFound ) {
        Assert( itFoundNode.Get().nodeKey == rhs );
        return itFoundNode.Get().nodeItem;
    }

    // Auto-Vivification
    Bool bInserted = m_rbTreeMap.Insert( searchNode, &itFoundNode );
    Assert( bInserted );
    Assert( itFoundNode.Get().nodeKey == rhs );
    return itFoundNode.Get().nodeItem;
}
template<typename Key, typename T>
const T & TreeMap<Key,T>::operator[]( const Key & rhs ) const
{
    RedBlackTree<TreeMapNode>::Iterator itFoundNode;

    // Build search node
    TreeMapNode searchNode;
    searchNode.nodeKey = rhs;

    // Search existing key
    Bool bFound = m_rbTreeMap.Search( searchNode, &itFoundNode );
    Assert( bFound );
    Assert( m_pfCompare(itFoundNode.Get().nodeKey, rhs, m_pUserData) == 0 );
    return itFoundNode.Get().nodeItem;
}

template<typename Key, typename T>
inline typename TreeMap<Key,T>::Iterator TreeMap<Key,T>::Begin() const {
    return Iterator( m_rbTreeMap.Begin() );
}
template<typename Key, typename T>
inline typename TreeMap<Key,T>::Iterator TreeMap<Key,T>::End() const {
    return Iterator( m_rbTreeMap.End() );
}
template<typename Key, typename T>
typename TreeMap<Key,T>::Iterator TreeMap<Key,T>::Get( const Key & rKey ) const
{
    RedBlackTree<TreeMapNode>::Iterator itFoundNode;

    // Build search node
    TreeMapNode searchNode;
    searchNode.nodeKey = rKey;

    // Search existing key
    Bool bFound = m_rbTreeMap.Search( searchNode, &itFoundNode );
    if ( !bFound )
        return Iterator();
    Assert( m_pfCompare(rKey, itFoundNode.Get().nodeKey, m_pUserData) == 0 );

    return Iterator( itFoundNode );
}

template<typename Key, typename T>
inline Bool TreeMap<Key,T>::Contains( const Key & rKey ) const {
    TreeMapNode searchNode;
    searchNode.nodeKey = rKey;
    return m_rbTreeMap.Search( searchNode );
}

template<typename Key, typename T>
Bool TreeMap<Key,T>::Insert( const Key & rKey, const T & rItem, Iterator * outItem )
{
    RedBlackTree<TreeMapNode>::Iterator itFoundNode;

    // Build new node
    TreeMapNode newNode;
    newNode.nodeKey = rKey;

    // Replace existing key
    Bool bFound = m_rbTreeMap.Search( newNode, &itFoundNode );
    if ( bFound ) {
        Assert( m_pfCompare(rKey, itFoundNode.Get().nodeKey, m_pUserData) == 0 );

        itFoundNode.Get().nodeItem = rItem;
        if ( outItem != NULL )
            *outItem = Iterator( itFoundNode );
        return false;
    }

    // Insert into map
    Bool bInserted = m_rbTreeMap.Insert( newNode, &itFoundNode );
    Assert( bInserted );
    Assert( m_pfCompare(rKey, itFoundNode.Get().nodeKey, m_pUserData) == 0 );

    itFoundNode.Get().nodeItem = rItem;
    if ( outItem != NULL )
        *outItem = Iterator( itFoundNode );
    return true;
}
template<typename Key, typename T>
Bool TreeMap<Key,T>::Remove( const Key & rKey, T * outItem )
{
    // Build search node
    TreeMapNode searchNode;
    searchNode.nodeKey = rKey;

    // Remove from map
    TreeMapNode removedNode;
    Bool bRemoved = m_rbTreeMap.Remove( searchNode, &removedNode );
    if ( !bRemoved )
        return false;
    Assert( m_pfCompare(rKey, removedNode.nodeKey, m_pUserData) == 0 );

    if ( outItem != NULL )
        *outItem = removedNode.nodeItem;
    return true;
}

/////////////////////////////////////////////////////////////////////////////////

template<typename Key, typename T>
inline Int TreeMap<Key,T>::_NodeCompare( const TreeMapNode & rLeft, const TreeMapNode & rRight, Void * pUserData ) {
    Assert( pUserData != NULL );
    TreeMap<Key,T> * pThis = (TreeMap<Key,T>*)( pUserData );
    return pThis->m_pfCompare( rLeft.nodeKey, rRight.nodeKey, pThis->m_pUserData );
}

