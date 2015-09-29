/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Datastruct/Tree/RedBlackTree.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : The smart partial-balanced tree ...
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// RedBlackTree implementation
template<typename T>
typename const RedBlackTree<T>::Iterator RedBlackTree<T>::Iterator::Null = Iterator();

template<typename T>
typename RedBlackTree<T>::RedBlackNode RedBlackTree<T>::sm_SentinelNode = { NULL, { NULL, NULL }, NULL, NULL, RBTREE_COLOR_BLACK, T() };
template<typename T>
typename RedBlackTree<T>::RedBlackNode * RedBlackTree<T>::RBTREE_LEAF = &sm_SentinelNode;

template<typename T>
RedBlackTree<T>::RedBlackTree()
{
    m_pfCompare = NULL;
    m_pUserData = NULL;

    m_iNodeCount = 0;
    m_pRoot = NULL;

    m_pHead = NULL;
    m_pEnd = NULL;
}
template<typename T>
RedBlackTree<T>::~RedBlackTree()
{
    // nothing to do
    // Call Destroy explicitly
}

template<typename T>
inline Void RedBlackTree<T>::SetComparator( Comparator pfCompare, Void * pUserData ) {
    Assert( m_pRoot == NULL );
    m_pfCompare = pfCompare;
    m_pUserData = pUserData;
}

template<typename T>
Void RedBlackTree<T>::Create()
{
    Assert( m_pfCompare != NULL );
    Assert( m_pRoot == NULL );

    m_iNodeCount = 0;
    m_pRoot = RBTREE_LEAF;

    m_pHead = NULL;
    m_pEnd = NULL;
}
template<typename T>
Void RedBlackTree<T>::Destroy()
{
    Assert( m_pRoot != NULL );

    Clear();

    m_iNodeCount = 0;
    m_pRoot = NULL;

    m_pHead = NULL;
    m_pEnd = NULL;
}

template<typename T>
inline UInt RedBlackTree<T>::MemorySize() const {
    Assert( m_pRoot != NULL );
    return ( m_iNodeCount * sizeof(RedBlackNode) );
}
template<typename T>
inline UInt RedBlackTree<T>::Count() const {
    Assert( m_pRoot != NULL );
    return m_iNodeCount;
}

template<typename T>
Void RedBlackTree<T>::Clear()
{
    Assert( m_pRoot != NULL );

    _DeleteAll( m_pRoot );

    m_iNodeCount = 0;
    m_pRoot = RBTREE_LEAF;

    m_pHead = NULL;
    m_pEnd = NULL;
}

template<typename T>
Bool RedBlackTree<T>::Search( const T & rItem, Iterator * outItem ) const
{
    Assert( m_pRoot != NULL );

    // Search
    Int iCmp;
    RBTreeChild searchDir;
    RedBlackNode * pNode = m_pRoot;
    while( pNode != RBTREE_LEAF ) {
        iCmp = m_pfCompare( pNode->Item, rItem, m_pUserData );
        if ( iCmp == 0 ) {
            // Found
            if ( outItem != NULL )
                *outItem = Iterator( pNode );
            return true;
        }
        searchDir = (iCmp < 0) ? RBTREE_CHILD_LEFT : RBTREE_CHILD_RIGHT;
        pNode = pNode->pChild[searchDir];
    }

    // Not Found
    if ( outItem != NULL )
        *outItem = Iterator();
    return false;
}
template<typename T>
inline Bool RedBlackTree<T>::Contains( const T & rItem ) const {
    return Search( rItem );
}

template<typename T>
Bool RedBlackTree<T>::Insert( const T & rItem, Iterator * outItem )
{
    Assert( m_pRoot != NULL );

    // Search
    RedBlackNode * pParentNode;
    RBTreeChild iLeafChild;
    Bool bInsert = _Insert_Search( &pParentNode, &iLeafChild, rItem );
    if ( !bInsert ) {
        // Return found value
        if ( outItem != NULL )
            *outItem = Iterator( pParentNode );
        return false;
    }

    // Build new node
    _SelectMemory();
    RedBlackNode * pNewNode = New RedBlackNode();
    _UnSelectMemory();

    pNewNode->pParent = pParentNode;
    pNewNode->pChild[RBTREE_CHILD_LEFT] = RBTREE_LEAF;
    pNewNode->pChild[RBTREE_CHILD_RIGHT] = RBTREE_LEAF;
    pNewNode->pPrev = NULL;
    pNewNode->pNext = NULL;
    pNewNode->iColor = RBTREE_COLOR_RED;
    pNewNode->Item = rItem;

    // Return new value
    if ( outItem != NULL )
        *outItem = Iterator( pNewNode );

    // Root creation case
    if ( pParentNode == NULL ) {
        m_pRoot = pNewNode;
        m_pRoot->iColor = RBTREE_COLOR_BLACK;
        m_pHead = pNewNode;
        m_pEnd = pNewNode;
        ++m_iNodeCount;
        return true;
    }

    // Insert
    pParentNode->pChild[iLeafChild] = pNewNode;

    // Insert in linked list
    RedBlackNode *pPredecessor, *pSuccessor;
    if ( iLeafChild == RBTREE_CHILD_LEFT ) {
        pPredecessor = pParentNode->pPrev;
        pSuccessor = pParentNode;
    } else {
        Assert( iLeafChild == RBTREE_CHILD_RIGHT );
        pPredecessor = pParentNode;
        pSuccessor = pParentNode->pNext;
    }
    pNewNode->pPrev = pPredecessor;
    pNewNode->pNext = pSuccessor;
    if ( pPredecessor != NULL )
        pPredecessor->pNext = pNewNode;
    else
        m_pHead = pNewNode;
    if ( pSuccessor != NULL )
        pSuccessor->pPrev = pNewNode;
    else
        m_pEnd = pNewNode;

    // Rebalance
    _Insert_Rebalance( pNewNode );

    // Done
    ++m_iNodeCount;
    return true;
}

template<typename T>
Bool RedBlackTree<T>::Remove( const T & rItem, T * outItem )
{
    Assert( m_pRoot != NULL );

    // Search
    RedBlackNode * pNode = NULL;
    RBTreeChild iNodeDir;
    Bool bFound = _Remove_Search( &pNode, &iNodeDir, rItem );
    if ( !bFound )
        return false;

    // Return found value
    if ( outItem != NULL )
        *outItem = pNode->Item;

    // Swap with predecessor/successor Node
    RedBlackNode * pSwappedNode = pNode;
    _Remove_Swap( &pNode, iNodeDir );

    // Remove from linked list
    if ( pNode->pPrev != NULL )
        pNode->pPrev->pNext = pNode->pNext;
    else
        m_pHead = pNode->pNext;
    if ( pNode->pNext != NULL )
        pNode->pNext->pPrev = pNode->pPrev;
    else
        m_pEnd = pNode->pPrev;
    pNode->pPrev = NULL;
    pNode->pNext = NULL;

    // Pick Child (non-leaf one, if any)
    RedBlackNode * pChild;
    if ( pNode->pChild[RBTREE_CHILD_RIGHT] == RBTREE_LEAF )
        pChild = pNode->pChild[RBTREE_CHILD_LEFT];
    else
        pChild = pNode->pChild[RBTREE_CHILD_RIGHT];

    // Pick Parent
    RedBlackNode * pParent = pNode->pParent;
    if ( pParent == pSwappedNode )
        iNodeDir = RBTREE_INVERTDIR(iNodeDir);

    // Replace Node with its Child
    if ( pChild != RBTREE_LEAF )
        pChild->pParent = pParent;
    if ( pParent != NULL )
        pParent->pChild[iNodeDir] = pChild;
    else
        m_pRoot = pChild;

    // Rebalance
    if ( pNode->iColor == RBTREE_COLOR_BLACK ) {
        if ( pChild->iColor == RBTREE_COLOR_RED )
            pChild->iColor = RBTREE_COLOR_BLACK;
        else {
            Assert( pChild == RBTREE_LEAF );
            _Remove_Rebalance( pParent, iNodeDir );
        }
    }

    // Delete Node
    _SelectMemory();
    Delete( pNode );
    _UnSelectMemory();

    // Done
    --m_iNodeCount;
    return true;
}

template<typename T>
inline typename RedBlackTree<T>::Iterator RedBlackTree<T>::Begin() const {
    return Iterator( m_pHead );
}
template<typename T>
inline typename RedBlackTree<T>::Iterator RedBlackTree<T>::End() const {
    return Iterator( m_pEnd );
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
Void RedBlackTree<T>::_DeleteAll( RedBlackNode * pNode )
{
    if ( pNode == RBTREE_LEAF )
        return;
    _DeleteAll( pNode->pChild[RBTREE_CHILD_LEFT] );
    _DeleteAll( pNode->pChild[RBTREE_CHILD_RIGHT] );

    _SelectMemory();
    Delete( pNode );
    _UnSelectMemory();
}
template<typename T>
Void RedBlackTree<T>::_RotateOnce( RedBlackNode * pNode, RBTreeChild rotDir )
{
    RBTreeChild invertDir = RBTREE_INVERTDIR(rotDir);
    Assert( pNode != RBTREE_LEAF );
    Assert( pNode->pChild[invertDir] != RBTREE_LEAF );

    // Retrieve holding pointer
    RedBlackNode ** ppNode;
    if ( pNode->pParent == NULL ) {
        Assert( pNode == m_pRoot );
        ppNode = &m_pRoot;
    } else {
        if ( pNode->pParent->pChild[RBTREE_CHILD_LEFT] == pNode )
            ppNode = &( pNode->pParent->pChild[RBTREE_CHILD_LEFT] );
        else
            ppNode = &( pNode->pParent->pChild[RBTREE_CHILD_RIGHT] );
    }

    // Update parent links
    if ( pNode->pChild[invertDir]->pChild[rotDir] != RBTREE_LEAF )
        pNode->pChild[invertDir]->pChild[rotDir]->pParent = pNode;
    pNode->pChild[invertDir]->pParent = pNode->pParent;
    pNode->pParent = pNode->pChild[invertDir];

    // Update child links
    *ppNode = pNode->pChild[invertDir];
	pNode->pChild[invertDir] = (*ppNode)->pChild[rotDir];
	(*ppNode)->pChild[rotDir] = pNode;
}

template<typename T>
Bool RedBlackTree<T>::_Insert_Search( RedBlackNode ** outParentNode, RBTreeChild * outLeaf, const T & rItem ) const
{
    // Leaf-Root case
    if ( m_pRoot == RBTREE_LEAF ) {
        *outParentNode = NULL;
        return true;
    }

    // Search the tree
    Int iCmp;
    RBTreeChild searchDir;
    RedBlackNode * pNode = m_pRoot;
    while( true ) {
        iCmp = m_pfCompare( pNode->Item, rItem, m_pUserData );
        if ( iCmp == 0 ) {
            *outParentNode = pNode;
            return false; // already exists
        }
        searchDir = (iCmp < 0) ? RBTREE_CHILD_LEFT : RBTREE_CHILD_RIGHT;
        if ( pNode->pChild[searchDir] == RBTREE_LEAF ) {
            *outParentNode = pNode;
            *outLeaf = searchDir;
            return true;
        }
        pNode = pNode->pChild[searchDir];
    }

    // Never goes here
    *outParentNode = NULL;
    return false;
}
template<typename T>
Void RedBlackTree<T>::_Insert_Rebalance( RedBlackNode * pNode )
{
    RedBlackNode * pParent;
    RedBlackNode * pGrandParent;
    RedBlackNode * pUncle;
    RBTreeChild iParentDir, iUncleDir;

    // Iterative Recurse Loop
    while( true ) {
        // Root case
        if ( pNode->pParent == NULL ) {
            pNode->iColor = RBTREE_COLOR_BLACK;
            return;
        }

        // Get Parent
        pParent = pNode->pParent;

        // Black Parent case, nothing to do
        if ( pParent->iColor == RBTREE_COLOR_BLACK )
            return;

        // Get GrandParent
        pGrandParent = pParent->pParent;
        Assert( pGrandParent != NULL );

        // Get Uncle
        if ( pGrandParent->pChild[RBTREE_CHILD_LEFT] == pParent ) {
            iParentDir = RBTREE_CHILD_LEFT;
            iUncleDir = RBTREE_CHILD_RIGHT;
        } else {
            iParentDir = RBTREE_CHILD_RIGHT;
            iUncleDir = RBTREE_CHILD_LEFT;
        }
        pUncle = pGrandParent->pChild[iUncleDir];

        // Black Uncle case, finish with rotations
        if ( pUncle->iColor == RBTREE_COLOR_BLACK )
            break;

        // Red Uncle case, go up
        pParent->iColor = RBTREE_COLOR_BLACK;
        pUncle->iColor = RBTREE_COLOR_BLACK;
        pGrandParent->iColor = RBTREE_COLOR_RED;
        pNode = pGrandParent;
    }

    // Get Node direction
    RBTreeChild iNodeDir;
    if ( pParent->pChild[RBTREE_CHILD_LEFT] == pNode )
        iNodeDir = RBTREE_CHILD_LEFT;
    else
        iNodeDir = RBTREE_CHILD_RIGHT;

    // Alignment rotation
    if ( iNodeDir != iParentDir ) {
        _RotateOnce( pParent, iParentDir );
        pParent = pGrandParent->pChild[iParentDir];
        pNode = pParent->pChild[iParentDir];
        iNodeDir = iParentDir;
    }

    // Balancing rotation
    pParent->iColor = RBTREE_COLOR_BLACK;
    pGrandParent->iColor = RBTREE_COLOR_RED;
    _RotateOnce( pGrandParent, RBTREE_INVERTDIR(iParentDir) );
}

template<typename T>
Bool RedBlackTree<T>::_Remove_Search( RedBlackNode ** outNode, RBTreeChild * outNodeDir, const T & rItem ) const
{
    // Leaf-Root case
    if ( m_pRoot == RBTREE_LEAF ) {
        *outNode = NULL;
        return false;
    }

    // Root case balancing
    static Bool s_bRootDirLeft = false;
    s_bRootDirLeft = !s_bRootDirLeft;

    // Search the tree
    Int iCmp;
    RBTreeChild searchDir = (s_bRootDirLeft) ? RBTREE_CHILD_LEFT : RBTREE_CHILD_RIGHT;
    RedBlackNode * pNode = m_pRoot;
    while( true ) {
        iCmp = m_pfCompare( pNode->Item, rItem, m_pUserData );
        if ( iCmp == 0 ) {
            *outNode = pNode;
            *outNodeDir = searchDir;
            return true;
        }
        searchDir = (iCmp < 0) ? RBTREE_CHILD_LEFT : RBTREE_CHILD_RIGHT;
        if ( pNode->pChild[searchDir] == RBTREE_LEAF ) {
            *outNode = NULL;
            return false;
        }
        pNode = pNode->pChild[searchDir];
    }

    // Never goes here
    *outNode = NULL;
    return false;
}
template<typename T>
Void RedBlackTree<T>::_Remove_Swap( RedBlackNode ** ppNode, RBTreeChild iNodeDir )
{
    RedBlackNode * pNode = *ppNode;

    // At least one leaf child
    if ( (pNode->pChild[RBTREE_CHILD_LEFT] == RBTREE_LEAF) ||
         (pNode->pChild[RBTREE_CHILD_RIGHT] == RBTREE_LEAF) )
        return;

    // No Leaf child, get predecessor/successor
    RedBlackNode * pSwapNode;
    if ( iNodeDir == RBTREE_CHILD_LEFT )
        pSwapNode = pNode->pNext;
    else {
        Assert( iNodeDir == RBTREE_CHILD_RIGHT );
        pSwapNode = pNode->pPrev;
    }
    Assert( pSwapNode != NULL );

    // Swap
    pNode->Item = pSwapNode->Item;
    *ppNode = pSwapNode;
}
template<typename T>
Void RedBlackTree<T>::_Remove_Rebalance( RedBlackNode * pParent, RBTreeChild iNodeDir )
{
    RedBlackNode * pSibbling;
    RBTreeChild iSibblingDir = RBTREE_INVERTDIR(iNodeDir);

    // Iterative Recurse Loop
    while( true ) {
        // Root case
        if ( pParent == NULL )
            return;

        // Pick Sibbling
        pSibbling = pParent->pChild[iSibblingDir];
        Assert( pSibbling != RBTREE_LEAF ); // Cannot be leaf

        // Red Sibbling case, Parent is black
        if ( pSibbling->iColor == RBTREE_COLOR_RED ) {
            Assert( pParent->iColor == RBTREE_COLOR_BLACK );

            // Swap colors
            pParent->iColor = RBTREE_COLOR_RED;
            pSibbling->iColor = RBTREE_COLOR_BLACK;

            // Rotate
            _RotateOnce( pParent, iNodeDir );
            pSibbling = pParent->pChild[iSibblingDir];
        }

        // At least 1 Red around, exit loop
        if ( pParent->iColor == RBTREE_COLOR_RED ||
             pSibbling->iColor == RBTREE_COLOR_RED ||
             pSibbling->pChild[RBTREE_CHILD_LEFT]->iColor == RBTREE_COLOR_RED ||
             pSibbling->pChild[RBTREE_CHILD_RIGHT]->iColor == RBTREE_COLOR_RED )
            break;

        // All-Black case, go up
        pSibbling->iColor = RBTREE_COLOR_RED;
        RedBlackNode * pNode = pParent;
        pParent = pNode->pParent;
        if ( pParent != NULL ) {
            if ( pParent->pChild[RBTREE_CHILD_LEFT] == pNode )
                iNodeDir = RBTREE_CHILD_LEFT;
            else
                iNodeDir = RBTREE_CHILD_RIGHT;
            iSibblingDir = RBTREE_INVERTDIR(iNodeDir);
        }
    }

    // Sibbling is ensured Black by now
    Assert( pSibbling->iColor == RBTREE_COLOR_BLACK );

    // Red parent, Black childs case
    if ( pParent->iColor == RBTREE_COLOR_RED &&
         pSibbling->pChild[RBTREE_CHILD_LEFT]->iColor == RBTREE_COLOR_BLACK &&
         pSibbling->pChild[RBTREE_CHILD_RIGHT]->iColor == RBTREE_COLOR_BLACK ) {
        // Swap colors
        pParent->iColor = RBTREE_COLOR_BLACK;
        pSibbling->iColor = RBTREE_COLOR_RED;
        return;
    }

    // Red/Black childs case
    if ( pSibbling->pChild[iSibblingDir]->iColor == RBTREE_COLOR_BLACK ) {
        Assert( pSibbling->pChild[iNodeDir]->iColor == RBTREE_COLOR_RED );

        // Swap colors
        pSibbling->iColor = RBTREE_COLOR_RED;
        pSibbling->pChild[iNodeDir]->iColor = RBTREE_COLOR_BLACK;

        // Rotate
        _RotateOnce( pSibbling, iSibblingDir );
        pSibbling = pParent->pChild[iSibblingDir];
    }

    // Black Sibbling with Red child in same direction case
    pSibbling->iColor = pParent->iColor;
    pParent->iColor = RBTREE_COLOR_BLACK;
    pSibbling->pChild[iSibblingDir]->iColor = RBTREE_COLOR_BLACK;
    _RotateOnce( pParent, iNodeDir );
}

//template<typename T>
//typename RedBlackTree<T>::RedBlackNode * RedBlackTree<T>::_Enumerate_First( RBTreeChild iEnumDir ) const
//{
//    // Empty tree case
//    if ( m_pRoot == RBTREE_LEAF )
//        return NULL;
//
//    // Downward along iEnumDir
//    RedBlackNode * pFirstNode = m_pRoot;
//    while( pFirstNode->pChild[iEnumDir] != RBTREE_LEAF )
//        pFirstNode = pFirstNode->pChild[iEnumDir];
//
//    return pFirstNode;
//}
//template<typename T>
//typename RedBlackTree<T>::RedBlackNode * RedBlackTree<T>::_Enumerate_Step( RedBlackNode * pNode, RBTreeChild iEnumDir )
//{
//    // In-Order traversal stepping using edge orientation
//    RedBlackNode * pNextNode = NULL;
//    RBTreeChild iInvEnumDir = RBTREE_INVERTDIR(iEnumDir);
//
//    // Inverse direction's node is leaf case
//    if ( pNode->pChild[iInvEnumDir] == RBTREE_LEAF ) {
//        // Upward loop
//        while( true ) {
//            pNextNode = pNode->pParent;
//            if ( pNextNode == NULL )
//                return NULL;
//            // Upward along iEnumDir ( Front-Enum phase )
//            if ( pNextNode->pChild[iEnumDir] == pNode )
//                return pNextNode;
//            // Upward along iInvEnumDir ( Back-Skip phase )
//            pNode = pNextNode;
//        }
//    }
//
//    // Downward along iInvEnumDir ( Back-Enum phase )
//    pNextNode = pNode->pChild[iInvEnumDir];
//
//    // Downward along iEnumDir ( Front-Skip phase )
//    while( pNextNode->pChild[iEnumDir] != RBTREE_LEAF )
//        pNextNode = pNextNode->pChild[iEnumDir];
//
//    return pNextNode;
//}

