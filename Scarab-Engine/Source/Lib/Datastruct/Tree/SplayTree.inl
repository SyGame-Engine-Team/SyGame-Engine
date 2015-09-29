/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Datastruct/Tree/SplayTree.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : The heuristic self-optimizing tree ...
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// SplayTree implementation
template<typename T>
typename const SplayTree<T>::Iterator SplayTree<T>::Iterator::Null = Iterator();

template<typename T>
SplayTree<T>::SplayTree()
{
    m_pfCompare = NULL;
    m_pUserData = NULL;

    m_iNodeCount = 0;
    m_pRoot = NULL;
}
template<typename T>
SplayTree<T>::~SplayTree()
{
    // nothing to do
    // Call Destroy explicitly
}

template<typename T>
inline Void SplayTree<T>::SetComparator( Comparator pfCompare, Void * pUserData ) {
    Assert( m_pRoot == NULL );
    m_pfCompare = pfCompare;
    m_pUserData = pUserData;
}

template<typename T>
Void SplayTree<T>::Create()
{
    Assert( m_pfCompare != NULL );
    Assert( m_pRoot == NULL );

    m_iNodeCount = 0;
    m_pRoot = NULL;
}
template<typename T>
Void SplayTree<T>::Destroy()
{
    Clear();
}

template<typename T>
inline UInt SplayTree<T>::MemorySize() const {
    return ( m_iNodeCount * sizeof(SplayNode) );
}
template<typename T>
inline UInt SplayTree<T>::Count() const {
    return m_iNodeCount;
}

template<typename T>
Void SplayTree<T>::Clear()
{
    _DeleteAll( m_pRoot );

    m_iNodeCount = 0;
    m_pRoot = NULL;
}

template<typename T>
Bool SplayTree<T>::Search( const T & rItem, Iterator * outItem ) const
{
    // Search
    Int iCmp;
    SplayTreeChild searchDir;
    SplayNode * pNode = m_pRoot;
    while( pNode != NULL ) {
        iCmp = m_pfCompare( pNode->Item, rItem, m_pUserData );
        if ( iCmp == 0 ) {
            // Found
            if ( outItem != NULL )
                *outItem = Iterator( pNode );
            _Splay( pNode );
            return true;
        }
        searchDir = (iCmp < 0) ? SPLAYTREE_CHILD_LEFT : SPLAYTREE_CHILD_RIGHT;
        pNode = pNode->pChild[searchDir];
    }

    // Not Found
    if ( outItem != NULL )
        *outItem = Iterator();
    return false;
}

template<typename T>
Bool SplayTree<T>::Insert( const T & rItem, Iterator * outItem )
{
    // Empty tree case
    if ( m_pRoot == NULL ) {
        // Build new node
        _SelectMemory();
        SplayNode * pNewNode = New SplayNode();
        _UnSelectMemory();

        pNewNode->pParent = NULL;
        pNewNode->pChild[SPLAYTREE_CHILD_LEFT] = NULL;
        pNewNode->pChild[SPLAYTREE_CHILD_RIGHT] = NULL;
        pNewNode->Item = rItem;

        // Insert
        m_pRoot = pNewNode;

        // Return item
        if ( outItem != NULL )
            *outItem = Iterator( pNewNode );

        ++m_iNodeCount;
        return true;
    }

    // Search
    Int iCmp;
    SplayTreeChild searchDir;
    SplayNode * pParentNode = m_pRoot;
    while( true ) {
        iCmp = m_pfCompare( pParentNode->Item, rItem, m_pUserData );
        if ( iCmp == 0 ) {
            // Found
            if ( outItem != NULL )
                *outItem = Iterator( pParentNode );
            return false;
        }
        searchDir = (iCmp < 0) ? SPLAYTREE_CHILD_LEFT : SPLAYTREE_CHILD_RIGHT;
        if ( pParentNode->pChild[searchDir] == NULL )
            break;
        pParentNode = pParentNode->pChild[searchDir];
    }

    // Build new node
    _SelectMemory();
    SplayNode * pNewNode = New SplayNode();
    _UnSelectMemory();

    pNewNode->pParent = pParentNode;
    pNewNode->pChild[SPLAYTREE_CHILD_LEFT] = NULL;
    pNewNode->pChild[SPLAYTREE_CHILD_RIGHT] = NULL;
    pNewNode->Item = rItem;

    // Return item
    if ( outItem != NULL )
        *outItem = Iterator( pNewNode );

    // Insert
    pParentNode->pChild[searchDir] = pNewNode;

    // Splay
    _Splay( pNewNode );

    ++m_iNodeCount;
    return true;
}

template<typename T>
Bool SplayTree<T>::Remove( const T & rItem, T * outItem )
{
    // Search
    Int iCmp;
    SplayTreeChild searchDir;
    SplayNode * pNode = m_pRoot;
    while( pNode != NULL ) {
        iCmp = m_pfCompare( pNode->Item, rItem, m_pUserData );
        if ( iCmp == 0 )
            break;
        searchDir = (iCmp < 0) ? SPLAYTREE_CHILD_LEFT : SPLAYTREE_CHILD_RIGHT;
        pNode = pNode->pChild[searchDir];
    }

    // Not found
    if ( pNode == NULL )
        return false;

    // Return item
    if ( outItem != NULL )
        *outItem = pNode->Item;

    // Splay the node
    _Splay( pNode );

    // Split in 2 trees
    SplayNode * pLeft = m_pRoot->pChild[SPLAYTREE_CHILD_LEFT];
    SplayNode * pRight = m_pRoot->pChild[SPLAYTREE_CHILD_RIGHT];
    
    // NULL left case
    if ( pLeft == NULL ) {
        if ( pRight != NULL )
            pRight->pParent = NULL;

        _SelectMemory();
        Delete( m_pRoot );
        _UnSelectMemory();

        m_pRoot = pRight;
        --m_iNodeCount;
        return true;
    }
    pLeft->pParent = NULL;

    // NULL right case
    if ( pRight == NULL ) {
        _SelectMemory();
        Delete( m_pRoot );
        _UnSelectMemory();

        m_pRoot = pLeft;
        --m_iNodeCount;
        return true;
    }
    pRight->pParent = NULL;

    // Replace root with left tree
    _SelectMemory();
    Delete( m_pRoot );
    _UnSelectMemory();

    m_pRoot = pLeft;

    // Splay the max
    SplayNode * pMax = _Enumerate_First( SPLAYTREE_CHILD_RIGHT );
    _Splay( pMax );

    // Insert back right tree
    Assert( m_pRoot->pChild[SPLAYTREE_CHILD_RIGHT] == NULL );
    m_pRoot->pChild[SPLAYTREE_CHILD_RIGHT] = pRight;
    pRight->pParent = m_pRoot;

    --m_iNodeCount;
    return true;
}

template<typename T>
Bool SplayTree<T>::Raise( const T & rItem )
{
    // Search
    Int iCmp;
    SplayTreeChild searchDir;
    SplayNode * pNode = m_pRoot;
    while( pNode != NULL ) {
        iCmp = m_pfCompare( pNode->Item, rItem, m_pUserData );
        if ( iCmp == 0 )
            break;
        searchDir = (iCmp < 0) ? SPLAYTREE_CHILD_LEFT : SPLAYTREE_CHILD_RIGHT;
        pNode = pNode->pChild[searchDir];
    }

    // Not found
    if ( pNode == NULL )
        return false;

    // Splay
    _Splay( pNode );

    return true;
}

template<typename T>
Bool SplayTree<T>::Extract( T * outItem )
{
    // Empty tree case
    if ( m_pRoot == NULL )
        return false;

    // Return Item
    if ( outItem != NULL )
        *outItem = m_pRoot->Item;

    // Split in 2 trees
    SplayNode * pLeft = m_pRoot->pChild[SPLAYTREE_CHILD_LEFT];
    SplayNode * pRight = m_pRoot->pChild[SPLAYTREE_CHILD_RIGHT];

    // NULL left case
    if ( pLeft == NULL ) {
        if ( pRight != NULL )
            pRight->pParent = NULL;

        _SelectMemory();
        Delete( m_pRoot );
        _UnSelectMemory();

        m_pRoot = pRight;
        --m_iNodeCount;
        return true;
    }
    pLeft->pParent = NULL;

    // NULL right case
    if ( pRight == NULL ) {
        _SelectMemory();
        Delete( m_pRoot );
        _UnSelectMemory();

        m_pRoot = pLeft;
        --m_iNodeCount;
        return true;
    }
    pRight->pParent = NULL;

    // Replace root with left tree
    _SelectMemory();
    Delete( m_pRoot );
    _UnSelectMemory();

    m_pRoot = pLeft;

    // Splay the max
    SplayNode * pMax = _Enumerate_First( SPLAYTREE_CHILD_RIGHT );
    _Splay( pMax );

    // Insert back right tree
    Assert( m_pRoot->pChild[SPLAYTREE_CHILD_RIGHT] == NULL );
    m_pRoot->pChild[SPLAYTREE_CHILD_RIGHT] = pRight;
    pRight->pParent = m_pRoot;

    --m_iNodeCount;
    return true;
}

template<typename T>
inline typename SplayTree<T>::Iterator SplayTree<T>::Begin() const {
    return Iterator( _Enumerate_First(SPLAYTREE_CHILD_LEFT) );
}
template<typename T>
inline typename SplayTree<T>::Iterator SplayTree<T>::End() const {
    return Iterator( _Enumerate_First(SPLAYTREE_CHILD_RIGHT) );
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
Void SplayTree<T>::_DeleteAll( SplayNode * pNode )
{
    if ( pNode == NULL )
        return;
    _DeleteAll( pNode->pChild[SPLAYTREE_CHILD_LEFT] );
    _DeleteAll( pNode->pChild[SPLAYTREE_CHILD_RIGHT] );

    _SelectMemory();
    Delete( pNode );
    _UnSelectMemory();
}
template<typename T>
Void SplayTree<T>::_RotateOnce( SplayNode * pNode, SplayTreeChild rotDir )
{
    SplayTreeChild invertDir = SPLAYTREE_INVERTDIR(rotDir);
    Assert( pNode != NULL );
    Assert( pNode->pChild[invertDir] != NULL );

    // Retrieve holding pointer
    SplayNode ** ppNode;
    if ( pNode->pParent == NULL ) {
        Assert( pNode == m_pRoot );
        ppNode = &m_pRoot;
    } else {
        if ( pNode->pParent->pChild[SPLAYTREE_CHILD_LEFT] == pNode )
            ppNode = &( pNode->pParent->pChild[SPLAYTREE_CHILD_LEFT] );
        else
            ppNode = &( pNode->pParent->pChild[SPLAYTREE_CHILD_RIGHT] );
    }

    // Update parent links
    if ( pNode->pChild[invertDir]->pChild[rotDir] != NULL )
        pNode->pChild[invertDir]->pChild[rotDir]->pParent = pNode;
    pNode->pChild[invertDir]->pParent = pNode->pParent;
    pNode->pParent = pNode->pChild[invertDir];

    // Update child links
    *ppNode = pNode->pChild[invertDir];
	pNode->pChild[invertDir] = (*ppNode)->pChild[rotDir];
	(*ppNode)->pChild[rotDir] = pNode;
}
template<typename T>
Void SplayTree<T>::_Splay( SplayNode * pNode )
{
    SplayNode * pParent;
    SplayNode * pGrandParent;
    SplayTreeChild iNodeDir, iParentDir;

    // Iterative Recurse Loop
    while( true ) {
        // End case
        if ( pNode->pParent == NULL ) {
            m_pRoot = pNode;
            return;
        }

        // Pick parent & node dir
        pParent = pNode->pParent;
        if ( pParent->pChild[SPLAYTREE_CHILD_LEFT] == pNode )
            iNodeDir = SPLAYTREE_CHILD_LEFT;
        else
            iNodeDir = SPLAYTREE_CHILD_RIGHT;

        // End case, Zig step
        if ( pParent->pParent == NULL ) {
            _RotateOnce( pParent, SPLAYTREE_INVERTDIR(iNodeDir) );
            m_pRoot = pNode;
            return;
        }

        // Pick grandparent & parent dir
        pGrandParent = pParent->pParent;
        if ( pGrandParent->pChild[SPLAYTREE_CHILD_LEFT] == pParent )
            iParentDir = SPLAYTREE_CHILD_LEFT;
        else
            iParentDir = SPLAYTREE_CHILD_RIGHT;

        // Splay upward
        if ( iNodeDir == iParentDir ) {
            // Zig-Zig step
            _RotateOnce( pGrandParent, SPLAYTREE_INVERTDIR(iParentDir) );
            _RotateOnce( pParent, SPLAYTREE_INVERTDIR(iNodeDir) );
        } else {
            // Zig-Zag step
            _RotateOnce( pParent, iParentDir );
            _RotateOnce( pGrandParent, iNodeDir );
        }
    }
}

template<typename T>
typename SplayTree<T>::SplayNode * SplayTree<T>::_Enumerate_First( SplayTreeChild iEnumDir ) const
{
    // Empty tree case
    if ( m_pRoot == NULL )
        return NULL;

    // Downward along iEnumDir
    SplayNode * pFirstNode = m_pRoot;
    while( pFirstNode->pChild[iEnumDir] != NULL )
        pFirstNode = pFirstNode->pChild[iEnumDir];

    return pFirstNode;
}
template<typename T>
typename SplayTree<T>::SplayNode * SplayTree<T>::_Enumerate_Step( SplayNode * pNode, SplayTreeChild iEnumDir )
{
    // In-Order traversal stepping using edge orientation
    SplayNode * pNextNode = NULL;
    SplayTreeChild iInvEnumDir = SPLAYTREE_INVERTDIR(iEnumDir);

    // Inverse direction's node is leaf case
    if ( pNode->pChild[iInvEnumDir] == NULL ) {
        // Upward loop
        while( true ) {
            pNextNode = pNode->pParent;
            if ( pNextNode == NULL )
                return NULL;
            // Upward along iEnumDir ( Front-Enum phase )
            if ( pNextNode->pChild[iEnumDir] == pNode )
                return pNextNode;
            // Upward along iInvEnumDir ( Back-Skip phase )
            pNode = pNextNode;
        }
    }

    // Downward along iInvEnumDir ( Back-Enum phase )
    pNextNode = pNode->pChild[iInvEnumDir];

    // Downward along iEnumDir ( Front-Skip phase )
    while( pNextNode->pChild[iEnumDir] != NULL )
        pNextNode = pNextNode->pChild[iEnumDir];

    return pNextNode;
}


