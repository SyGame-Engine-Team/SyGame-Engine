/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Datastruct/Tree/AVLTree.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : The Adelson-Velskii-Landis Tree.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// AVLTree implementation
template<typename T>
AVLTree<T>::AVLTree():
    Datastruct()
{
    m_pfCompare = NULL;

    m_iNodeCount = INVALID_OFFSET;
    m_pRoot = NULL;

    m_iHeightChange = AVLTREE_HEIGHT_NOCHANGE;
}
template<typename T>
AVLTree<T>::~AVLTree()
{
    // nothing to do
    // Call Destroy explicitly
}

template<typename T>
inline Void AVLTree<T>::SetComparator( Comparator pfCompare ) {
    Assert( m_iNodeCount == INVALID_OFFSET );
    m_pfCompare = pfCompare;
}

template<typename T>
Void AVLTree<T>::Create()
{
    Assert( m_pfCompare != NULL );
    Assert( m_iNodeCount == INVALID_OFFSET );

    m_iNodeCount = 0;
    m_pRoot = NULL;

    m_iHeightChange = AVLTREE_HEIGHT_NOCHANGE;
}
template<typename T>
Void AVLTree<T>::Destroy()
{
    Assert( m_iNodeCount != INVALID_OFFSET );

    Clear();

    m_iNodeCount = INVALID_OFFSET;
    m_pRoot = NULL;

    m_iHeightChange = AVLTREE_HEIGHT_NOCHANGE;
}

template<typename T>
inline UInt AVLTree<T>::MemorySize() const {
    Assert( m_iNodeCount != INVALID_OFFSET );
    return ( m_iNodeCount * sizeof(AVLNode) );
}
template<typename T>
inline UInt AVLTree<T>::Count() const {
    Assert( m_iNodeCount != INVALID_OFFSET );
    return m_iNodeCount;
}

template<typename T>
inline Void AVLTree<T>::Clear() {
    Assert( m_iNodeCount != INVALID_OFFSET );
    if ( m_pRoot != NULL ) {
        _rec_Destroy( m_pRoot );
        m_iNodeCount = 0;
        m_pRoot = NULL;
        m_iHeightChange = AVLTREE_HEIGHT_NOCHANGE;
    }    
}

template<typename T>
inline Void AVLTree<T>::Insert(const T & rItem) {
    Assert( m_iNodeCount != INVALID_OFFSET );
    _rec_Insert( &m_pRoot, m_iHeightChange, rItem );
}
template<typename T>
inline Bool AVLTree<T>::Remove(const T & rItem) {
    Assert( m_iNodeCount != INVALID_OFFSET );
    return _rec_Remove( &m_pRoot, m_iHeightChange, rItem );
}

template<typename T>
Bool AVLTree<T>::Search( T & rOut, const T & rItem, AVLTreeSearch searchFlags ) const
{
    Assert( m_iNodeCount != INVALID_OFFSET );

	AVLNode * pNode = m_pRoot;
	AVLNode * pBestNode = NULL;
    AVLTreeChild iSearchDir;
    AVLTreeChild iInvertDir;
	Int cmp;
    Bool bTest;

    Bool bSearchEqual = ( (searchFlags & AVLTREE_SEARCH_EQUAL) != 0 );
    if (searchFlags & AVLTREE_SEARCH_LESSER)
        iSearchDir = AVLTREE_CHILD_LEFT;
    else if (searchFlags & AVLTREE_SEARCH_GREATER)
        iSearchDir = AVLTREE_CHILD_RIGHT;
    else {
        // Strict Equal search
        while(pNode != NULL) {
            cmp = m_pfCompare(pNode->Item, rItem);
            if (cmp == 0) {
                rOut = pNode->Item;
                return true;
            }
            iSearchDir = (cmp < 0) ? AVLTREE_CHILD_LEFT : AVLTREE_CHILD_RIGHT;
            pNode = pNode->pChild[iSearchDir];
        }
        return false;
    }
    iInvertDir = AVLTREE_INVERTDIR(iSearchDir);

    // Best/Worst search
    while(pNode != NULL) {
        cmp = m_pfCompare(pNode->Item, rItem);
        bTest = (iSearchDir == AVLTREE_CHILD_LEFT) ? (cmp < 0) : (cmp > 0);
        if (!bSearchEqual)
            bTest = ((cmp == 0) || bTest);
        if (bTest) {
            pNode = pNode->pChild[iSearchDir];
            continue;
        }
        if (searchFlags & AVLTREE_SEARCH_FIT_FIRST) {
            rOut = pNode->Item;
            return true;
        }
        pBestNode = pNode;
        if (searchFlags & AVLTREE_SEARCH_FIT_WORST) {
            pNode = pNode->pChild[iSearchDir];
        } else if (searchFlags & AVLTREE_SEARCH_FIT_BEST) {
            pNode = pNode->pChild[iInvertDir];
        }
    }

    if (pBestNode != NULL) {
        rOut = pBestNode->Item;
        return true;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
AVLTreeHeightChange AVLTree<T>::_RotateOnce( AVLNode ** ppNode, AVLTreeChild rotDir )
{
    AVLTreeChild invertDir = AVLTREE_INVERTDIR(rotDir);
	AVLNode * pOldNode = *ppNode;
    AVLTreeHeightChange heightChange = ( pOldNode->pChild[invertDir]->iBalanceFactor == AVLTREE_BALANCED ) ?
									     AVLTREE_HEIGHT_NOCHANGE : AVLTREE_HEIGHT_CHANGE;

    *ppNode = pOldNode->pChild[invertDir];
	pOldNode->pChild[invertDir] = (*ppNode)->pChild[rotDir];
	(*ppNode)->pChild[rotDir] = pOldNode;

    if ( rotDir == AVLTREE_CHILD_LEFT )
		--( (*ppNode)->iBalanceFactor );
	else
		++( (*ppNode)->iBalanceFactor );
	pOldNode->iBalanceFactor = -( (*ppNode)->iBalanceFactor );

    return heightChange;
}
template<typename T>
AVLTreeHeightChange AVLTree<T>::_RotateTwice( AVLNode ** ppNode, AVLTreeChild rotDir )
{
	AVLTreeChild invertDir = AVLTREE_INVERTDIR(rotDir);
	AVLNode * pOldNode = *ppNode;
	AVLNode * pOldInvertDir = pOldNode->pChild[invertDir];

	*ppNode = pOldNode->pChild[invertDir]->pChild[rotDir];
	pOldNode->pChild[invertDir] = (*ppNode)->pChild[rotDir];
	(*ppNode)->pChild[rotDir] = pOldNode;
	pOldInvertDir->pChild[rotDir] = (*ppNode)->pChild[invertDir];
	(*ppNode)->pChild[invertDir] = pOldInvertDir;

	(*ppNode)->pChild[AVLTREE_CHILD_LEFT]->iBalanceFactor = - Max<Int>( (*ppNode)->iBalanceFactor, AVLTREE_BALANCED );
	(*ppNode)->pChild[AVLTREE_CHILD_RIGHT]->iBalanceFactor = - Min<Int>( (*ppNode)->iBalanceFactor, AVLTREE_BALANCED );
	(*ppNode)->iBalanceFactor = AVLTREE_BALANCED;

	return AVLTREE_HEIGHT_CHANGE; // height is always shortened
}
template<typename T>
AVLTreeHeightChange AVLTree<T>::_ReBalance( AVLNode ** ppNode )
{
    AVLTreeHeightChange heightChange = AVLTREE_HEIGHT_NOCHANGE;

	if ( AVLTREE_IMBALANCE_LEFT((*ppNode)->iBalanceFactor) ) {
		// Need a right rotation
		if ( (*ppNode)->pChild[AVLTREE_CHILD_LEFT]->iBalanceFactor == AVLTREE_RIGHT_HEAVY )
			heightChange = _RotateTwice(ppNode, AVLTREE_CHILD_RIGHT); // RL rotation needed
		else
			heightChange = _RotateOnce(ppNode, AVLTREE_CHILD_RIGHT); // R rotation needed
	} else if ( AVLTREE_IMBALANCE_RIGHT((*ppNode)->iBalanceFactor) ) {
		// Need a left rotation
		if ( (*ppNode)->pChild[AVLTREE_CHILD_RIGHT]->iBalanceFactor == AVLTREE_LEFT_HEAVY )
			heightChange = _RotateTwice(ppNode, AVLTREE_CHILD_LEFT); // LR rotation needed
		else
			heightChange = _RotateOnce(ppNode, AVLTREE_CHILD_LEFT); // L rotation needed
	}

	return heightChange;
}

template<typename T>
Bool AVLTree<T>::_rec_Insert( AVLNode ** ppNode, AVLTreeHeightChange & heightChange, const T & rItem )
{
    // End of sub-tree, insert here
	if ( *ppNode == NULL ) {
        _SelectMemory();
        *ppNode = New AVLNode();
        _UnSelectMemory();

        (*ppNode)->pChild[AVLTREE_CHILD_LEFT] = NULL;
        (*ppNode)->pChild[AVLTREE_CHILD_RIGHT] = NULL;
        (*ppNode)->iBalanceFactor = AVLTREE_BALANCED;
        (*ppNode)->Item = rItem;
        ++m_iNodeCount;
		heightChange = AVLTREE_HEIGHT_CHANGE;
		return true;
	}

    // Compare
    Int cmp = m_pfCompare( (*ppNode)->Item, rItem );

    // Key already exists, transmit up
    if ( cmp == 0 ) {
        heightChange = AVLTREE_HEIGHT_NOCHANGE;
		return false;
    }

    // Search in correct sub-tree
    AVLTreeChild dir = (cmp < 0) ? AVLTREE_CHILD_LEFT : AVLTREE_CHILD_RIGHT;
    Bool bInserted = _rec_Insert( &((*ppNode)->pChild[dir]), heightChange, rItem );

    // Key already exists, transmit up
    if ( !bInserted )
		return false;

    // Adjust for re-balance
    Int inc = cmp * heightChange;

    // Key was inserted, re-balance if needed
	(*ppNode)->iBalanceFactor += inc;
    if ( (inc != 0) && ((*ppNode)->iBalanceFactor != AVLTREE_BALANCED) )
        heightChange = (AVLTreeHeightChange)( 1 - _ReBalance(ppNode) );
    else
        heightChange = AVLTREE_HEIGHT_NOCHANGE;
	return true;
}
template<typename T>
Bool AVLTree<T>::_rec_Remove( AVLNode ** ppNode, AVLTreeHeightChange & heightChange, const T & rItem )
{
	// End of sub-tree, key not found
	if ( *ppNode == NULL ) {
		heightChange = AVLTREE_HEIGHT_NOCHANGE;
		return false;
	}

    // Compare
    Int cmp = m_pfCompare( (*ppNode)->Item, rItem );
    Bool bRemoved = false;
    Int dec = 0;

    if ( cmp != 0 ) {
        // Search in correct sub-tree
        AVLTreeChild dir = (cmp < 0) ? AVLTREE_CHILD_LEFT : AVLTREE_CHILD_RIGHT;
	    bRemoved = _rec_Remove( &( (*ppNode)->pChild[dir] ), heightChange, rItem );

        // Key doesn't exist, can't remove
        if ( !bRemoved )
            return false;

        // Adjust for re-balance
        dec = cmp * heightChange;
    } else {
        // Key found at this node
        AVLNode * pRemove = (*ppNode);

        // Leaf case
        if ( ((*ppNode)->pChild[AVLTREE_CHILD_LEFT] == NULL) &&
             ((*ppNode)->pChild[AVLTREE_CHILD_RIGHT] == NULL) ) {
            *ppNode = NULL;

            _SelectMemory();
            Delete( pRemove );
            _UnSelectMemory();

            --m_iNodeCount;
            heightChange = AVLTREE_HEIGHT_CHANGE;
		    return true;
        }

        // One-slot cases
        if ( (*ppNode)->pChild[AVLTREE_CHILD_LEFT] == NULL ) {
            *ppNode = (*ppNode)->pChild[AVLTREE_CHILD_RIGHT];

            _SelectMemory();
            Delete( pRemove );
            _UnSelectMemory();

            --m_iNodeCount;
            heightChange = AVLTREE_HEIGHT_CHANGE;
		    return true;
        }
        if ( (*ppNode)->pChild[AVLTREE_CHILD_RIGHT] == NULL ) {
            *ppNode = (*ppNode)->pChild[AVLTREE_CHILD_LEFT];

            _SelectMemory();
            Delete( pRemove );
            _UnSelectMemory();

            --m_iNodeCount;
            heightChange = AVLTREE_HEIGHT_CHANGE;
		    return true;
        }

        // Two-children case : Swap with successor
        AVLNode * pSuccessor = (*ppNode)->pChild[AVLTREE_CHILD_RIGHT];
        while( pSuccessor->pChild[AVLTREE_CHILD_LEFT] != NULL )
            pSuccessor = pSuccessor->pChild[AVLTREE_CHILD_LEFT];
        pRemove->Item = pSuccessor->Item;

        bRemoved = _rec_Remove( &( (*ppNode)->pChild[AVLTREE_CHILD_RIGHT] ), heightChange, pSuccessor->Item );
        Assert( bRemoved );

        // Adjust for re-balance
        dec = heightChange;
    }

    // Key was deleted, re-balance if needed
    (*ppNode)->iBalanceFactor -= dec;
	if ( dec != 0 ) {
		if ( (*ppNode)->iBalanceFactor != AVLTREE_BALANCED )
			heightChange = _ReBalance(ppNode);
		else
			heightChange = AVLTREE_HEIGHT_CHANGE;
    } else
		heightChange = AVLTREE_HEIGHT_NOCHANGE;
	return true;
}
template<typename T>
Void AVLTree<T>::_rec_Destroy( AVLNode * pNode )
{
    if ( pNode->pChild[AVLTREE_CHILD_LEFT] != NULL )
        _rec_Destroy( pNode->pChild[AVLTREE_CHILD_LEFT] );
    if ( pNode->pChild[AVLTREE_CHILD_RIGHT] != NULL )
        _rec_Destroy( pNode->pChild[AVLTREE_CHILD_RIGHT] );

    _SelectMemory();
    Delete( pNode );
    _UnSelectMemory();
}
