/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Resource/Database/RDBTreeOperator.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Resource Database, B*Tree operations
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
#include "RDBTreeOperator.h"

/////////////////////////////////////////////////////////////////////////////////
// RDBTreeOperator implementation
RDBTreeOperator::RDBTreeOperator( RDBPersistentStorage * pStorage )
{
    Assert( pStorage != NULL );
    m_pStorage = pStorage;
    m_iCurRoot = RDB_CHUNKTABLE_DIRECTORIES_ROOT;
}
RDBTreeOperator::~RDBTreeOperator()
{
    // nothing to do
}

Bool RDBTreeOperator::SearchDirectory( const RDB_BTreeKeyData & rKey, RDB_BTreeItem * outItem )
{
    RDB_ChunkPtr pLeafStorage;
    const RDB_Chunk_BTreeLeaf * pLeaf;
    UInt iIndex;
    Bool bFound = _BTree_Search( m_pStorage->GetTableChunk(RDB_CHUNKTABLE_DIRECTORIES_ROOT),
                                 rKey, &pLeafStorage, &pLeaf, &iIndex );
    if ( bFound )
        *outItem = pLeaf->arrItems[iIndex];
    return bFound;
}
Bool RDBTreeOperator::SearchResource( const RDB_BTreeKeyData & rKey, RDB_BTreeItem * outItem )
{
    RDB_ChunkPtr pLeafStorage;
    const RDB_Chunk_BTreeLeaf * pLeaf;
    UInt iIndex;
    Bool bFound = _BTree_Search( m_pStorage->GetTableChunk(RDB_CHUNKTABLE_RESOURCES_ROOT),
                                 rKey, &pLeafStorage, &pLeaf, &iIndex );
    if ( bFound )
        *outItem = pLeaf->arrItems[iIndex];
    return bFound;
}

Void RDBTreeOperator::ListSubDirectories( RDB_DirectoryID idDirectory, Array<RDB_BTreeItem> * outSubDirectories )
{
    RDB_BTreeKeyData keyData;
    keyData.idParent = idDirectory;
    keyData.strName[0] = NULLBYTE;

    RDB_ChunkPtr pLeafStorage;
    const RDB_Chunk_BTreeLeaf * pLeaf;
    UInt iIndex;
    Bool bFound = _BTree_Search( m_pStorage->GetTableChunk(RDB_CHUNKTABLE_DIRECTORIES_ROOT), keyData, &pLeafStorage, &pLeaf, &iIndex );
    Assert( !bFound );

    if ( iIndex >= pLeaf->iItemCount ) {
        if ( pLeaf->pNextBrother.idBlock == (RDB_BlockID)INVALID_OFFSET )
            return;
        pLeaf = m_pStorage->GetConstLeaf( pLeaf->pNextBrother );
        iIndex = 0;
    }

    while( true ) {
        while( pLeaf->arrItems[iIndex].keyData.idParent == idDirectory ) {
            outSubDirectories->Push( pLeaf->arrItems[iIndex] );
            ++iIndex;
            if ( iIndex >= pLeaf->iItemCount )
                break;
        }
        if ( iIndex < pLeaf->iItemCount )
            break;
        if ( pLeaf->pNextBrother.idBlock == (RDB_BlockID)INVALID_OFFSET )
            break;
        pLeaf = m_pStorage->GetConstLeaf( pLeaf->pNextBrother );
        iIndex = 0;
    }
}
Void RDBTreeOperator::ListResources( RDB_DirectoryID idDirectory, Array<RDB_BTreeItem> * outResources )
{
    RDB_BTreeKeyData keyData;
    keyData.idParent = idDirectory;
    keyData.strName[0] = NULLBYTE;

    RDB_ChunkPtr pLeafStorage;
    const RDB_Chunk_BTreeLeaf * pLeaf;
    UInt iIndex;
    Bool bFound = _BTree_Search( m_pStorage->GetTableChunk(RDB_CHUNKTABLE_RESOURCES_ROOT), keyData, &pLeafStorage, &pLeaf, &iIndex );
    Assert( !bFound );

    if ( iIndex >= pLeaf->iItemCount ) {
        if ( pLeaf->pNextBrother.idBlock == (RDB_BlockID)INVALID_OFFSET )
            return;
        pLeaf = m_pStorage->GetConstLeaf( pLeaf->pNextBrother );
        iIndex = 0;
    }

    while( true ) {
        while( pLeaf->arrItems[iIndex].keyData.idParent == idDirectory ) {
            outResources->Push( pLeaf->arrItems[iIndex] );
            ++iIndex;
            if ( iIndex >= pLeaf->iItemCount )
                break;
        }
        if ( iIndex < pLeaf->iItemCount )
            break;
        if ( pLeaf->pNextBrother.idBlock == (RDB_BlockID)INVALID_OFFSET )
            break;
        pLeaf = m_pStorage->GetConstLeaf( pLeaf->pNextBrother );
        iIndex = 0;
    }
}

Bool RDBTreeOperator::UpdateResource( const RDB_BTreeKeyData & rKey, const RDB_ResourceDescriptor & descResource )
{
    RDB_ChunkPtr pLeafStorage;
    const RDB_Chunk_BTreeLeaf * pLeaf;
    UInt iIndex;
    Bool bFound = _BTree_Search( m_pStorage->GetTableChunk(RDB_CHUNKTABLE_RESOURCES_ROOT),
                                 rKey, &pLeafStorage, &pLeaf, &iIndex );
    if ( !bFound )
        return false;

    RDB_Chunk_BTreeLeaf * pWriteLeaf = m_pStorage->GetLeaf( pLeafStorage );
    pWriteLeaf->arrItems[iIndex].Content.descResource = descResource;

    return true;
}

/////////////////////////////////////////////////////////////////////////////////

Int RDBTreeOperator::_BTree_Compare( const RDB_BTreeKeyData & rLeft, const RDB_BTreeKeyData & rRight, UInt iPrefixSize )
{
    // Parent directory ordering
    if ( rLeft.idParent < rRight.idParent )
        return +1;
    if ( rLeft.idParent > rRight.idParent )
        return -1;

    // Name ordering
    const AChar * pL = ( rLeft.strName + iPrefixSize );
    const AChar * pR = ( rRight.strName + iPrefixSize );
    while( true ) {
        if ( *pL < *pR )
            return +1;
        if ( *pL > *pR )
            return -1;
        if ( *pL == ANULLBYTE )
            return 0;
        ++pL;
        ++pR;
    }
}

/////////////////////////////////////////////////////////////////////////////////

Bool RDBTreeOperator::_BTree_SearchKey( const RDB_Chunk_BTreeNode * pNode, const RDB_BTreeKeyData & rKey, UInt * outIndex ) const
{
    // Empty node
    if ( pNode->iKeyCount == 0 ) {
        *outIndex = 0;
        return false;
    }

    // Iterative Dichotomic Search
    Int iCmp;
    UInt iLeftBound = 0;
    UInt iRightBound = ( pNode->iKeyCount - 1 );
    UInt iPivot = ( (iLeftBound + iRightBound) >> 1 ); // /2
    while( true ) {
        iCmp = _BTree_Compare( pNode->arrKeys[iPivot].keyData, rKey, 0 );
        if ( iCmp == 0 ) {
            *outIndex = iPivot;
            return true;
        }
        if ( iCmp < 0 ) {
            if ( iPivot == iLeftBound ) {
                *outIndex = iPivot;
                return false;
            }
            iRightBound = iPivot - 1;
        } else { // ( iCmp > 0 )
            if ( iPivot == iRightBound ) {
                *outIndex = iPivot + 1;
                return false;
            }
            iLeftBound = iPivot + 1;
        }
        iPivot = ( (iLeftBound + iRightBound) >> 1 ); // /2
    }

    // Never reach here
    *outIndex = INVALID_OFFSET;
    return false;
}
Bool RDBTreeOperator::_BTree_SearchItem( const RDB_Chunk_BTreeLeaf * pLeaf, const RDB_BTreeKeyData & rKey, UInt * outIndex ) const
{
    // Empty leaf
    if ( pLeaf->iItemCount == 0 ) {
        *outIndex = 0;
        return false;
    }

    // Iterative Dichotomic Search
    Int iCmp;
    UInt iLeftBound = 0;
    UInt iRightBound = ( pLeaf->iItemCount - 1 );
    UInt iPivot = ( (iLeftBound + iRightBound) >> 1 ); // /2
    while( true ) {
        iCmp = _BTree_Compare( pLeaf->arrItems[iPivot].keyData, rKey, 0 );
        if ( iCmp == 0 ) {
            *outIndex = iPivot;
            return true;
        }
        if ( iCmp < 0 ) {
            if ( iPivot == iLeftBound ) {
                *outIndex = iPivot;
                return false;
            }
            iRightBound = iPivot - 1;
        } else { // ( iCmp > 0 )
            if ( iPivot == iRightBound ) {
                *outIndex = iPivot + 1;
                return false;
            }
            iLeftBound = iPivot + 1;
        }
        iPivot = ( (iLeftBound + iRightBound) >> 1 ); // /2
    }

    // Never reach here
    *outIndex = INVALID_OFFSET;
    return false;
}

Void RDBTreeOperator::_BTree_InsertKey( RDB_Chunk_BTreeNode * pNode, UInt iIndex, const RDB_BTreeKey & rKey )
{
    Assert( pNode->iKeyCount < RDB_BTREE_MAX_KEYS );
    Assert( iIndex <= pNode->iKeyCount );
    for( UInt i = pNode->iKeyCount; i > iIndex; --i )
        pNode->arrKeys[i] = pNode->arrKeys[i - 1];
    pNode->arrKeys[iIndex] = rKey;
    ++(pNode->iKeyCount);
}
Void RDBTreeOperator::_BTree_InsertItem( RDB_Chunk_BTreeLeaf * pLeaf, UInt iIndex, const RDB_BTreeItem & rItem )
{
    Assert( pLeaf->iItemCount < RDB_BTREE_MAX_KEYS );
    Assert( iIndex <= pLeaf->iItemCount );
    for( UInt i = pLeaf->iItemCount; i > iIndex; --i )
        pLeaf->arrItems[i] = pLeaf->arrItems[i - 1];
    pLeaf->arrItems[iIndex] = rItem;
    ++(pLeaf->iItemCount);
}

Void RDBTreeOperator::_BTree_RemoveKey( RDB_Chunk_BTreeNode * pNode, UInt iIndex )
{
    Assert( pNode->iKeyCount > 0 );
    Assert( iIndex < pNode->iKeyCount );
    for( UInt i = iIndex + 1; i < pNode->iKeyCount; ++i )
        pNode->arrKeys[i - 1] = pNode->arrKeys[i];
    --(pNode->iKeyCount);
}
Void RDBTreeOperator::_BTree_RemoveItem( RDB_Chunk_BTreeLeaf * pLeaf, UInt iIndex )
{
    Assert( pLeaf->iItemCount > 0 );
    Assert( iIndex < pLeaf->iItemCount );
    for( UInt i = iIndex + 1; i < pLeaf->iItemCount; ++i )
        pLeaf->arrItems[i - 1] = pLeaf->arrItems[i];
    --(pLeaf->iItemCount);
}

Void RDBTreeOperator::_BTree_SelectBrotherNode( const RDB_Chunk_BTreeNode * pParentNode, UInt iParentIndex, Bool bMinElseMax,
                                                Bool * outSelectPrev, RDB_ChunkPtr * outBrotherStorage, RDB_Chunk_BTreeNode ** outBrotherNode )
{
    if ( iParentIndex == 0 ) {
        *outSelectPrev = false;
        *outBrotherStorage = pParentNode->arrKeys[iParentIndex + 1].pLeftChild;
        *outBrotherNode = m_pStorage->GetNode( *outBrotherStorage );
        return;
    }
    if ( iParentIndex == pParentNode->iKeyCount ) {
        *outSelectPrev = true;
        *outBrotherStorage = pParentNode->arrKeys[iParentIndex - 1].pLeftChild;
        *outBrotherNode = m_pStorage->GetNode( *outBrotherStorage );
        return;
    }

    const RDB_ChunkPtr pPrevStorage = pParentNode->arrKeys[iParentIndex - 1].pLeftChild;
    RDB_ChunkPtr pNextStorage;
    if ( iParentIndex == (pParentNode->iKeyCount - 1) )
        pNextStorage = pParentNode->pRightChild;
    else
        pNextStorage = pParentNode->arrKeys[iParentIndex + 1].pLeftChild;

    const RDB_Chunk_BTreeNode * pPrevNode = m_pStorage->GetConstNode( pPrevStorage );
    const RDB_Chunk_BTreeNode * pNextNode = m_pStorage->GetConstNode( pNextStorage );

    if ( bMinElseMax )
        *outSelectPrev = ( pPrevNode->iKeyCount < pNextNode->iKeyCount );
    else
        *outSelectPrev = ( pPrevNode->iKeyCount > pNextNode->iKeyCount );

    if ( *outSelectPrev ) {
        *outBrotherStorage = pPrevStorage;
        *outBrotherNode = m_pStorage->GetNode( pPrevStorage );
    } else {
        *outBrotherStorage = pNextStorage;
        *outBrotherNode = m_pStorage->GetNode( pNextStorage );
    }
}
Void RDBTreeOperator::_BTree_SelectBrotherLeaf( const RDB_Chunk_BTreeNode * pParentNode, UInt iParentIndex,
                                                const RDB_Chunk_BTreeLeaf * pLeaf, Bool bMinElseMax, Bool * outSelectPrev,
                                                RDB_ChunkPtr * outBrotherStorage, RDB_Chunk_BTreeLeaf ** outBrotherLeaf )
{
    if ( iParentIndex == 0 ) {
        *outSelectPrev = false;
        *outBrotherStorage = pLeaf->pNextBrother;
        *outBrotherLeaf = m_pStorage->GetLeaf( *outBrotherStorage );
        return;
    }
    if ( iParentIndex == pParentNode->iKeyCount ) {
        *outSelectPrev = true;
        *outBrotherStorage = pLeaf->pPrevBrother;
        *outBrotherLeaf = m_pStorage->GetLeaf( *outBrotherStorage );
        return;
    }

    const RDB_Chunk_BTreeLeaf * pPrevLeaf = m_pStorage->GetConstLeaf( pLeaf->pPrevBrother );
    const RDB_Chunk_BTreeLeaf * pNextLeaf = m_pStorage->GetConstLeaf( pLeaf->pNextBrother );

    if ( bMinElseMax )
        *outSelectPrev = ( pPrevLeaf->iItemCount < pNextLeaf->iItemCount );
    else
        *outSelectPrev = ( pPrevLeaf->iItemCount > pNextLeaf->iItemCount );

    if ( *outSelectPrev ) {
        *outBrotherStorage = pLeaf->pPrevBrother;
        *outBrotherLeaf = m_pStorage->GetLeaf( pLeaf->pPrevBrother );
    } else {
        *outBrotherStorage = pLeaf->pNextBrother;
        *outBrotherLeaf = m_pStorage->GetLeaf( pLeaf->pNextBrother );
    }
}

/////////////////////////////////////////////////////////////////////////////////

Void RDBTreeOperator::_BTree_BalanceLeft_Nodes( RDB_BTreeKeyData * pParentKey,
                                                RDB_Chunk_BTreeNode * pLeftNode, RDB_Chunk_BTreeNode * pRightNode )
{
    Assert( pLeftNode->iKeyCount < pRightNode->iKeyCount );

    UInt i;

    UInt iShiftAmount = ( (pRightNode->iKeyCount - pLeftNode->iKeyCount) >> 1 );
    if ( iShiftAmount == 0 )
        iShiftAmount = 1;

    // Rotate left
    UInt iTmp = pLeftNode->iKeyCount;
    for( i = 0; i < iShiftAmount; ++i ) {
        pLeftNode->arrKeys[iTmp].keyData = *pParentKey;
        pLeftNode->arrKeys[iTmp].pLeftChild = pLeftNode->pRightChild;
        *pParentKey = pRightNode->arrKeys[i].keyData;
        pLeftNode->pRightChild = pRightNode->arrKeys[i].pLeftChild;
        ++iTmp;
    }
    pLeftNode->iKeyCount += iShiftAmount;

    // Shift left
    iTmp = ( pRightNode->iKeyCount - iShiftAmount );
    for( i = 0; i < iTmp; ++i )
        pRightNode->arrKeys[i] = pRightNode->arrKeys[iShiftAmount + i];
    pRightNode->iKeyCount -= iShiftAmount;
}
Void RDBTreeOperator::_BTree_BalanceRight_Nodes( RDB_BTreeKeyData * pParentKey,
                                                 RDB_Chunk_BTreeNode * pLeftNode, RDB_Chunk_BTreeNode * pRightNode )
{
    Assert( pLeftNode->iKeyCount > pRightNode->iKeyCount );

    Int i;

    UInt iShiftAmount = ( (pLeftNode->iKeyCount - pRightNode->iKeyCount) >> 1 );
    if ( iShiftAmount == 0 )
        iShiftAmount = 1;

    // Shift right
    for( i = (pRightNode->iKeyCount - 1); i >= 0; --i )
        pRightNode->arrKeys[i + iShiftAmount] = pRightNode->arrKeys[i];
    pRightNode->iKeyCount += iShiftAmount;

    // Rotate right
    UInt iTmp = ( pLeftNode->iKeyCount - 1 );
    for( i = (iShiftAmount - 1); i >= 0; --i ) {
        pRightNode->arrKeys[i].keyData = *pParentKey;
        pRightNode->arrKeys[i].pLeftChild = pLeftNode->pRightChild;
        *pParentKey = pLeftNode->arrKeys[iTmp].keyData;
        pLeftNode->pRightChild = pLeftNode->arrKeys[iTmp].pLeftChild;
        --iTmp;
    }
    pLeftNode->iKeyCount -= iShiftAmount;
}
Void RDBTreeOperator::_BTree_BalanceLeft_Leaves( RDB_BTreeKeyData * pParentKey,
                                                 RDB_Chunk_BTreeLeaf * pLeftLeaf, RDB_Chunk_BTreeLeaf * pRightLeaf )
{
    Assert( pLeftLeaf->iItemCount < pRightLeaf->iItemCount );

    UInt i;

    UInt iShiftAmount = ( ( pRightLeaf->iItemCount - pLeftLeaf->iItemCount ) >> 1 );
    if ( iShiftAmount == 0 )
        iShiftAmount = 1;

    // Rotate left
    UInt iTmp = pLeftLeaf->iItemCount;
    for( i = 0; i < iShiftAmount; ++i )
        pLeftLeaf->arrItems[iTmp++] = pRightLeaf->arrItems[i];
    pLeftLeaf->iItemCount += iShiftAmount;

    // Shift left
    iTmp = ( pRightLeaf->iItemCount - iShiftAmount );
    for( i = 0; i < iTmp; ++i )
        pRightLeaf->arrItems[i] = pRightLeaf->arrItems[iShiftAmount + i];
    pRightLeaf->iItemCount -= iShiftAmount;

    // Update parent key
    *pParentKey = pRightLeaf->arrItems[0].keyData;
}
Void RDBTreeOperator::_BTree_BalanceRight_Leaves( RDB_BTreeKeyData * pParentKey,
                                                  RDB_Chunk_BTreeLeaf * pLeftLeaf, RDB_Chunk_BTreeLeaf * pRightLeaf )
{
    Assert( pLeftLeaf->iItemCount > pRightLeaf->iItemCount );

    Int i;

    UInt iShiftAmount = ( ( pLeftLeaf->iItemCount - pRightLeaf->iItemCount ) >> 1 );
    if ( iShiftAmount == 0 )
        iShiftAmount = 1;

    // Shift right
    for( i = (pRightLeaf->iItemCount - 1); i >= 0; --i )
        pRightLeaf->arrItems[i + iShiftAmount] = pRightLeaf->arrItems[i];
    pRightLeaf->iItemCount += iShiftAmount;

    // Rotate right
    UInt iTmp = ( pLeftLeaf->iItemCount - 1 );
    for( i = (iShiftAmount - 1); i >= 0; --i )
        pRightLeaf->arrItems[i] = pLeftLeaf->arrItems[iTmp--];
    pLeftLeaf->iItemCount -= iShiftAmount;

    // Update parent key
    *pParentKey = pRightLeaf->arrItems[0].keyData;
}

/////////////////////////////////////////////////////////////////////////////////

Bool RDBTreeOperator::_BTree_Search( const RDB_ChunkPtr & pStorage, const RDB_BTreeKeyData & rKey, 
                                     RDB_ChunkPtr * outLeafStorage, const RDB_Chunk_BTreeLeaf ** outLeaf, UInt * outIndex )
{
    Bool bFound;
    UInt iIndex = INVALID_OFFSET;

    // Leaf case
    if ( pStorage.iType == RDB_CHUNK_LEAF ) {
        const RDB_Chunk_BTreeLeaf * pLeaf = m_pStorage->GetConstLeaf( pStorage );

        bFound = _BTree_SearchItem( pLeaf, rKey, &iIndex );
        *outLeafStorage = pStorage;
        *outLeaf = pLeaf;
        *outIndex = iIndex;
        return bFound;
    }

    // Node case
    Assert( pStorage.iType == RDB_CHUNK_NODE );
    const RDB_Chunk_BTreeNode * pNode = m_pStorage->GetConstNode( pStorage );

    bFound = _BTree_SearchKey( pNode, rKey, &iIndex );
    if ( bFound ) //   K   => L has elements < K
        ++iIndex; // L   R => R has elements >= K

    RDB_ChunkPtr pChildStorage;
    if ( iIndex == pNode->iKeyCount )
        pChildStorage = pNode->pRightChild;
    else
        pChildStorage = pNode->arrKeys[iIndex].pLeftChild;

    // Recurse
    return _BTree_Search( pChildStorage, rKey, outLeafStorage, outLeaf, outIndex );
}

/////////////////////////////////////////////////////////////////////////////////

Void RDBTreeOperator::_BTree_SplitNode( RDB_Chunk_BTreeNode * pNode, UInt iInsertIndex,
                                        const RDB_BTreeKeyData & rKeyData, const RDB_ChunkPtr & rRightChild,
                                        RDB_BTreeKeyData * outUpwardKey, RDB_ChunkPtr * outNewStorage )
{
    Assert( pNode->iKeyCount == RDB_BTREE_MAX_KEYS );

    UInt i, iCur;

    // Allocate right node
    RDB_Chunk_BTreeNode * pNewNode = m_pStorage->AllocNode( outNewStorage );

    // Mid-Insert case
    if ( iInsertIndex == RDB_BTREE_MIN_KEYS ) {
        // Fill right node
        iCur = 0;
        for( i = RDB_BTREE_MIN_KEYS; i < RDB_BTREE_MAX_KEYS; ++i )
            pNewNode->arrKeys[iCur++] = pNode->arrKeys[i];
        Assert( iCur == RDB_BTREE_MIN_KEYS );
        pNode->iKeyCount = RDB_BTREE_MIN_KEYS;
        pNewNode->iKeyCount = RDB_BTREE_MIN_KEYS;

        // Update links
        pNewNode->pRightChild = pNode->pRightChild;
        pNode->pRightChild = pNewNode->arrKeys[0].pLeftChild;
        pNewNode->arrKeys[0].pLeftChild = rRightChild;

        // Extract upward key
        *outUpwardKey = rKeyData;

        return;
    }

    // Left-Insert case
    if ( iInsertIndex < RDB_BTREE_MIN_KEYS ) {
        // Fill right node
        iCur = 0;
        for( i = RDB_BTREE_MIN_KEYS; i < RDB_BTREE_MAX_KEYS; ++i )
            pNewNode->arrKeys[iCur++] = pNode->arrKeys[i];
        Assert( iCur == RDB_BTREE_MIN_KEYS );
        pNode->iKeyCount = RDB_BTREE_MIN_KEYS;
        pNewNode->iKeyCount = RDB_BTREE_MIN_KEYS;

        // Insert left
        RDB_BTreeKey newKey;
        newKey.keyData = rKeyData;
        newKey.pLeftChild = pNode->arrKeys[iInsertIndex].pLeftChild;
        pNode->arrKeys[iInsertIndex].pLeftChild = rRightChild;
        _BTree_InsertKey( pNode, iInsertIndex, newKey );

        // Extract upward key
        UInt iLastKey = pNode->iKeyCount - 1;
        pNewNode->pRightChild = pNode->pRightChild;
        pNode->pRightChild = pNode->arrKeys[iLastKey].pLeftChild;
        *outUpwardKey = pNode->arrKeys[iLastKey].keyData;
        --(pNode->iKeyCount);

        return;
    }

    // Right-Insert case
    Assert( iInsertIndex > RDB_BTREE_MIN_KEYS );

        // Fill right node (begin)
    iCur = 0;
    for( i = RDB_BTREE_MIN_KEYS + 1; i < iInsertIndex; ++i )
        pNewNode->arrKeys[iCur++] = pNode->arrKeys[i];

        // Insert right
    if ( iInsertIndex == RDB_BTREE_MAX_KEYS ) {
        pNewNode->arrKeys[iCur].keyData = rKeyData;
        pNewNode->arrKeys[iCur].pLeftChild = pNode->pRightChild;
        ++iCur;
    } else {
        pNewNode->arrKeys[iCur].keyData = rKeyData;
        pNewNode->arrKeys[iCur].pLeftChild = pNode->arrKeys[iInsertIndex].pLeftChild;
        ++iCur;
        pNewNode->arrKeys[iCur].keyData = pNode->arrKeys[iInsertIndex].keyData;
        pNewNode->arrKeys[iCur].pLeftChild = rRightChild;
        ++iCur;
    }

        // Fill right node (end)
    for( i = iInsertIndex + 1; i < RDB_BTREE_MAX_KEYS; ++i )
        pNewNode->arrKeys[iCur++] = pNode->arrKeys[i];
    pNewNode->iKeyCount = RDB_BTREE_MIN_KEYS;

        // Extract upward key
    if ( iInsertIndex == RDB_BTREE_MAX_KEYS )
        pNewNode->pRightChild = rRightChild;
    else
        pNewNode->pRightChild = pNode->pRightChild;
    pNode->pRightChild = pNode->arrKeys[RDB_BTREE_MIN_KEYS].pLeftChild;
    *outUpwardKey = pNode->arrKeys[RDB_BTREE_MIN_KEYS].keyData;

        // Truncate left node
    pNode->iKeyCount = RDB_BTREE_MIN_KEYS;
}
Void RDBTreeOperator::_BTree_SplitLeaf( const RDB_ChunkPtr & pStorage, RDB_Chunk_BTreeLeaf * pLeaf,
                                        UInt iInsertIndex, const RDB_BTreeItem & rItem,
                                        RDB_BTreeKeyData * outUpwardKey, RDB_ChunkPtr * outNewStorage )
{
    Assert( pLeaf->iItemCount == RDB_BTREE_MAX_KEYS );

    UInt i, iCur;

    // Allocate right leaf
    RDB_Chunk_BTreeLeaf * pNewLeaf = m_pStorage->AllocLeaf( outNewStorage );

    // Update brother links
    pNewLeaf->pPrevBrother = pStorage;
    pNewLeaf->pNextBrother = pLeaf->pNextBrother;
    if ( pLeaf->pNextBrother.idBlock != (RDB_BlockID)INVALID_OFFSET ) {
        RDB_Chunk_BTreeLeaf * pRightLeaf = m_pStorage->GetLeaf( pLeaf->pNextBrother );
        pRightLeaf->pPrevBrother = *outNewStorage;
    }
    pLeaf->pNextBrother = *outNewStorage;

    // Left-Insert case
    if ( iInsertIndex < RDB_BTREE_MIN_KEYS ) {
        // Fill right leaf
        iCur = 0;
        for( i = RDB_BTREE_MIN_KEYS; i < RDB_BTREE_MAX_KEYS; ++i )
            pNewLeaf->arrItems[iCur++] = pLeaf->arrItems[i];
        Assert( iCur == RDB_BTREE_MIN_KEYS );
        pNewLeaf->iItemCount = RDB_BTREE_MIN_KEYS;
        pLeaf->iItemCount = RDB_BTREE_MIN_KEYS;

        // Insert left
        _BTree_InsertItem( pLeaf, iInsertIndex, rItem );

        // Extract upward key
        *outUpwardKey = pNewLeaf->arrItems[0].keyData;

        return;
    }

    // Right-Insert case
        // Fill right leaf (begin)
    iCur = 0;
    for( i = RDB_BTREE_MIN_KEYS; i < iInsertIndex; ++i )
        pNewLeaf->arrItems[iCur++] = pLeaf->arrItems[i];

        // Insert right
    pNewLeaf->arrItems[iCur++] = rItem;

        // Fill right leaf (end)
    for( i = iInsertIndex; i < RDB_BTREE_MAX_KEYS; ++i )
        pNewLeaf->arrItems[iCur++] = pLeaf->arrItems[i];
    Assert( iCur == (RDB_BTREE_MIN_KEYS + 1) );
    pNewLeaf->iItemCount = RDB_BTREE_MIN_KEYS + 1;
    pLeaf->iItemCount = RDB_BTREE_MIN_KEYS;

        // Extract upward key
    *outUpwardKey = pNewLeaf->arrItems[0].keyData;
}

Bool RDBTreeOperator::_BTree_Insert( const RDB_ChunkPtr & pStorage, const RDB_BTreeItem & rItem,
                                     RDB_BTreeInsertParameters * outParameters )
{
    Bool bFound;
    UInt iIndex = INVALID_OFFSET;

    RDB_Chunk_BTreeNode * pParent = NULL;
    UInt iParentIndex = INVALID_OFFSET;

    // Leaf case
    if ( pStorage.iType == RDB_CHUNK_LEAF ) {
        RDB_Chunk_BTreeLeaf * pLeaf = m_pStorage->GetLeaf( pStorage );

        // Downward phase : Search
        bFound = _BTree_SearchItem( pLeaf, rItem.keyData, &iIndex );
        if ( bFound )
            return false; // already exists
        Assert( iIndex != INVALID_OFFSET );

        // Retrieve parent if any
        if ( outParameters != NULL ) {
            pParent = outParameters->pParent;
            iParentIndex = outParameters->iParentIndex;
        }

        // Downward phase : Insert
        if ( pLeaf->iItemCount < RDB_BTREE_MAX_KEYS ) {
            _BTree_InsertItem( pLeaf, iIndex, rItem );
            if ( outParameters != NULL ) {
                if ( iIndex == 0 )
                    outParameters->bUpdateLeafHeadKey = true;
                outParameters->upwardKey.strName[0] = NULLBYTE;
            }
            return true;
        }

        // Downward phase : Balance
        if ( outParameters != NULL ) {
            Bool bSelectPrev;
            RDB_ChunkPtr pBrotherStorage;
            RDB_Chunk_BTreeLeaf * pBrother;
            _BTree_SelectBrotherLeaf( pParent, iParentIndex, pLeaf, true,
                                      &bSelectPrev, &pBrotherStorage, &pBrother );
            if ( pBrother->iItemCount < RDB_BTREE_MAX_KEYS ) {
                if ( bSelectPrev ) {
                    _BTree_BalanceLeft_Leaves( &( pParent->arrKeys[iParentIndex - 1].keyData ),
                                                pBrother, pLeaf );
                } else {
                    _BTree_BalanceRight_Leaves( &( pParent->arrKeys[iParentIndex].keyData ),
                                               pLeaf, pBrother );
                }
                _BTree_InsertItem( pLeaf, iIndex, rItem );
                if ( iIndex == 0 )
                    outParameters->bUpdateLeafHeadKey = true;
                outParameters->upwardKey.strName[0] = NULLBYTE;
                return true;
            }
        }

        // Downward phase : Split
        RDB_BTreeKeyData upwardKey;
        RDB_ChunkPtr pNewStorage;
        _BTree_SplitLeaf( pStorage, pLeaf, iIndex, rItem, &upwardKey, &pNewStorage );

        // Upward Phase : Transmit
        if ( outParameters != NULL ) {
            if ( iIndex == 0 )
                outParameters->bUpdateLeafHeadKey = true;
            outParameters->upwardKey = upwardKey;
            outParameters->pStorage = pNewStorage;
            return true;
        }

        // Upward Phase : Root Update
        RDB_ChunkPtr pRootStorage;
        RDB_Chunk_BTreeNode * pRoot = m_pStorage->AllocNode( &pRootStorage );
        pRoot->iKeyCount = 1;
        pRoot->arrKeys[0].keyData = upwardKey;
        pRoot->arrKeys[0].pLeftChild = pStorage;
        pRoot->pRightChild = pNewStorage;
        m_pStorage->SetTableChunk( m_iCurRoot, pRootStorage );
        return true;
    }

    // Node case
    Assert( pStorage.iType == RDB_CHUNK_NODE );
    RDB_Chunk_BTreeNode * pNode = m_pStorage->GetNode( pStorage );

    // Downward phase : Search
    bFound = _BTree_SearchKey( pNode, rItem.keyData, &iIndex );
    if ( bFound )
        return false; // already exists

    RDB_ChunkPtr * pChildStorage;
    if ( iIndex == pNode->iKeyCount )
        pChildStorage = &( pNode->pRightChild );
    else
        pChildStorage = &( pNode->arrKeys[iIndex].pLeftChild );

    // Recurse
    RDB_BTreeInsertParameters insertParams;
    insertParams.pParent = pNode;
    insertParams.iParentIndex = iIndex;
    Bool bInserted = _BTree_Insert( *pChildStorage, rItem, &insertParams );
    if ( !bInserted )
        return false; // already exists

    // Upward phase : Leaf-Head key
    if ( insertParams.bUpdateLeafHeadKey ) {
        if ( iIndex == 0 ) {
            if ( outParameters != NULL )
                outParameters->bUpdateLeafHeadKey = true;
        } else {
            pNode->arrKeys[iIndex-1].keyData = rItem.keyData;
            if ( outParameters != NULL )
                outParameters->bUpdateLeafHeadKey = false;
        }
    }

    // Upward phase : End case
    if ( insertParams.upwardKey.strName[0] == NULLBYTE ) {
        if ( outParameters != NULL )
            outParameters->upwardKey.strName[0] = NULLBYTE;
        return true;
    }

    // Upward phase : Insert
    if ( pNode->iKeyCount < RDB_BTREE_MAX_KEYS ) {
        RDB_BTreeKey newKey;
        newKey.keyData = insertParams.upwardKey;
        newKey.pLeftChild = *pChildStorage;
        *pChildStorage = insertParams.pStorage;
        _BTree_InsertKey( pNode, iIndex, newKey );
        if ( outParameters != NULL )
            outParameters->upwardKey.strName[0] = NULLBYTE;
        return true;
    }

    // Retrieve parent if any
    if ( outParameters != NULL ) {
        pParent = outParameters->pParent;
        iParentIndex = outParameters->iParentIndex;
    }

    // Upward phase : Balance
    if ( outParameters != NULL ) {
        Bool bSelectPrev;
        RDB_ChunkPtr pBrotherStorage;
        RDB_Chunk_BTreeNode * pBrother;
        _BTree_SelectBrotherNode( pParent, iParentIndex, true,
                                  &bSelectPrev, &pBrotherStorage, &pBrother );
        if ( pBrother->iKeyCount < RDB_BTREE_MAX_KEYS ) {
            if ( bSelectPrev ) {
                _BTree_BalanceLeft_Nodes( &( pParent->arrKeys[iParentIndex - 1].keyData ),
                                          pBrother, pNode );
            } else {
                _BTree_BalanceRight_Nodes( &( pParent->arrKeys[iParentIndex].keyData ),
                                           pNode, pBrother );
            }
            RDB_BTreeKey newKey;
            newKey.keyData = insertParams.upwardKey;
            newKey.pLeftChild = *pChildStorage;
            *pChildStorage = insertParams.pStorage;
            _BTree_InsertKey( pNode, iIndex, newKey );
            outParameters->upwardKey.strName[0] = NULLBYTE;
            return true;
        }
    }

    // Upward phase : Split
    RDB_BTreeKeyData upwardKey;
    RDB_ChunkPtr pNewStorage;
    _BTree_SplitNode( pNode, iIndex, insertParams.upwardKey, insertParams.pStorage,
                      &upwardKey, &pNewStorage );

    // Upward Phase : Transmit
    if ( outParameters != NULL ) {
        outParameters->upwardKey = upwardKey;
        outParameters->pStorage = pNewStorage;
        return true;
    }

    // Upward Phase : Root Update
    RDB_ChunkPtr pRootStorage;
    RDB_Chunk_BTreeNode * pRoot = m_pStorage->AllocNode( &pRootStorage );
    pRoot->iKeyCount = 1;
    pRoot->arrKeys[0].keyData = upwardKey;
    pRoot->arrKeys[0].pLeftChild = pStorage;
    pRoot->pRightChild = pNewStorage;
    m_pStorage->SetTableChunk( m_iCurRoot, pRootStorage );
    return true;
}

/////////////////////////////////////////////////////////////////////////////////

Void RDBTreeOperator::_BTree_MergeNodes( const RDB_BTreeKeyData * pParentKey,
                                         RDB_Chunk_BTreeNode * pLeftNode, RDB_Chunk_BTreeNode * pRightNode )
{
    UInt i;

    // Parent goes down
    UInt iTmp = pLeftNode->iKeyCount;
    pLeftNode->arrKeys[iTmp].keyData = *pParentKey;
    pLeftNode->arrKeys[iTmp].pLeftChild = pLeftNode->pRightChild;
    ++iTmp;

    // Merge
    for( i = 0; i < pRightNode->iKeyCount; ++i )
        pLeftNode->arrKeys[iTmp++] = pRightNode->arrKeys[i];
    pLeftNode->pRightChild = pRightNode->pRightChild;
    pLeftNode->iKeyCount += pRightNode->iKeyCount;
}
Void RDBTreeOperator::_BTree_MergeLeaves( const RDB_ChunkPtr & pLeftStorage,
                                          RDB_Chunk_BTreeLeaf * pLeftLeaf, RDB_Chunk_BTreeLeaf * pRightLeaf )
{
    // Update brother links
    pLeftLeaf->pNextBrother = pRightLeaf->pNextBrother;
    if ( pRightLeaf->pNextBrother.idBlock != (RDB_BlockID)INVALID_OFFSET ) {
        RDB_Chunk_BTreeLeaf * pTmpLeaf = m_pStorage->GetLeaf( pRightLeaf->pNextBrother );
        pTmpLeaf->pPrevBrother = pLeftStorage;
    }

    // Merge
    UInt iTmp = pLeftLeaf->iItemCount;
    for( UInt i = 0; i < pRightLeaf->iItemCount; ++i )
        pLeftLeaf->arrItems[iTmp++] = pRightLeaf->arrItems[i];
    pLeftLeaf->iItemCount += pRightLeaf->iItemCount;
}

Bool RDBTreeOperator::_BTree_Remove( const RDB_ChunkPtr & pStorage, const RDB_BTreeKeyData & rKey,
                                     RDB_BTreeRemoveParameters * outParameters )
{
    Bool bFound;
    UInt iIndex = INVALID_OFFSET;

    // Leaf case
    if ( pStorage.iType == RDB_CHUNK_LEAF ) {
        RDB_Chunk_BTreeLeaf * pLeaf = m_pStorage->GetLeaf( pStorage );

        // Downward phase : Search
        bFound = _BTree_SearchItem( pLeaf, rKey, &iIndex );
        if ( !bFound )
            return false; // doesn't exists
        Assert( iIndex != INVALID_OFFSET );

        // Downward phase : Remove
        _BTree_RemoveItem( pLeaf, iIndex );

        // Downward phase : Root case
        if ( outParameters == NULL )
            return true;

        // Downward phase : No underflow
        if ( pLeaf->iItemCount >= RDB_BTREE_MIN_KEYS ) {
            if ( iIndex == 0 ) {
                if ( outParameters->pLeafHeadKey != NULL )
                    *(outParameters->pLeafHeadKey) = pLeaf->arrItems[0].keyData;
            }
            outParameters->iRemoveIndex = INVALID_OFFSET;
            return true;
        }

        // Select max brother
        RDB_Chunk_BTreeNode * pParent = outParameters->pParent;
        UInt iParentIndex = outParameters->iParentIndex;

        Bool bSelectPrev;
        RDB_ChunkPtr pBrotherStorage;
        RDB_Chunk_BTreeLeaf * pBrother;
        _BTree_SelectBrotherLeaf( pParent, iParentIndex, pLeaf, false,
                                  &bSelectPrev, &pBrotherStorage, &pBrother );

        // Downward phase : Balance
        if ( pBrother->iItemCount > RDB_BTREE_MIN_KEYS ) {
            if ( bSelectPrev ) {
                _BTree_BalanceRight_Leaves( &( pParent->arrKeys[iParentIndex - 1].keyData ),
                                            pBrother, pLeaf );
            } else {
                _BTree_BalanceLeft_Leaves( &( pParent->arrKeys[iParentIndex].keyData ),
                                           pLeaf, pBrother );
                if ( iIndex == 0 ) {
                    if ( outParameters->pLeafHeadKey != NULL )
                        *(outParameters->pLeafHeadKey) = pLeaf->arrItems[0].keyData;
                }
            }
            outParameters->iRemoveIndex = INVALID_OFFSET;
            return true;
        }

        // Downward phase : Merge
        if ( bSelectPrev ) {
            _BTree_MergeLeaves( pBrotherStorage, pBrother, pLeaf );
            m_pStorage->Free( pStorage );
            outParameters->iRemoveIndex = iParentIndex - 1;
            outParameters->pStorage = pBrotherStorage;
        } else {
            _BTree_MergeLeaves( pStorage, pLeaf, pBrother );
            m_pStorage->Free( pBrotherStorage );
            outParameters->iRemoveIndex = iParentIndex;
            outParameters->pStorage = pStorage;
            if ( iIndex == 0 ) {
                if ( outParameters->pLeafHeadKey != NULL )
                    *(outParameters->pLeafHeadKey) = pLeaf->arrItems[0].keyData;
            }
        }

        // Upward Phase : Transmit
        return true;
    }

    // Node case
    Assert( pStorage.iType == RDB_CHUNK_NODE );
    RDB_Chunk_BTreeNode * pNode = m_pStorage->GetNode( pStorage );

    // Downward phase : Search
    bFound = _BTree_SearchKey( pNode, rKey, &iIndex );
    if ( bFound ) //   K   => L has elements < K
        ++iIndex; // L   R => R has elements >= K

    RDB_ChunkPtr pChildStorage;
    if ( iIndex == pNode->iKeyCount )
        pChildStorage = pNode->pRightChild;
    else
        pChildStorage = pNode->arrKeys[iIndex].pLeftChild;

    // Downward phase : Leaf-Head key
    RDB_BTreeKeyData * pLeafHeadKey = NULL;
    if ( bFound ) {
        if ( outParameters != NULL )
            Assert( outParameters->pLeafHeadKey == NULL );
        pLeafHeadKey = &( pNode->arrKeys[iIndex - 1].keyData );
    } else {
        if ( outParameters != NULL )
            pLeafHeadKey = outParameters->pLeafHeadKey;
    }

    // Recurse
    RDB_BTreeRemoveParameters removeParams;
    removeParams.pParent = pNode;
    removeParams.iParentIndex = iIndex;
    removeParams.pLeafHeadKey = pLeafHeadKey;
    Bool bRemoved = _BTree_Remove( pChildStorage, rKey, &removeParams );
    if ( !bRemoved )
        return false; // doesn't exists

    // Upward phase : End case
    if ( removeParams.iRemoveIndex == INVALID_OFFSET ) {
        if ( outParameters != NULL )
            outParameters->iRemoveIndex = INVALID_OFFSET;
        return true;
    }

    UInt iRemoveIndex = removeParams.iRemoveIndex;

    // Upward phase : Root case
    if ( outParameters == NULL ) {
        _BTree_RemoveKey( pNode, iRemoveIndex );
        if ( pNode->iKeyCount == 0 ) {
            m_pStorage->Free( pStorage );
            m_pStorage->SetTableChunk( m_iCurRoot, removeParams.pStorage );
        } else {
            if ( iRemoveIndex == pNode->iKeyCount )
                pNode->pRightChild = removeParams.pStorage;
            else
                pNode->arrKeys[iRemoveIndex].pLeftChild = removeParams.pStorage;
        }
        return true;
    }

    // Upward phase : Remove
    _BTree_RemoveKey( pNode, iRemoveIndex );
    if ( iRemoveIndex == pNode->iKeyCount )
        pNode->pRightChild = removeParams.pStorage;
    else
        pNode->arrKeys[iRemoveIndex].pLeftChild = removeParams.pStorage;

    // Upward phase : No underflow
    if ( pNode->iKeyCount >= RDB_BTREE_MIN_KEYS ) {
        outParameters->iRemoveIndex = INVALID_OFFSET;
        return true;
    }

    // Upward phase : Select max brother
    UInt iParentIndex = outParameters->iParentIndex;
    RDB_Chunk_BTreeNode * pParent = outParameters->pParent;

    Bool bSelectPrev;
    RDB_ChunkPtr pBrotherStorage;
    RDB_Chunk_BTreeNode * pBrother;
    _BTree_SelectBrotherNode( pParent, iParentIndex, false,
                              &bSelectPrev, &pBrotherStorage, &pBrother );

    // Upward phase : Balance
    if ( pBrother->iKeyCount > RDB_BTREE_MIN_KEYS ) {
        if ( bSelectPrev ) {
            _BTree_BalanceRight_Nodes( &( pParent->arrKeys[iParentIndex - 1].keyData ),
                                       pBrother, pNode );
        } else {
            _BTree_BalanceLeft_Nodes( &( pParent->arrKeys[iParentIndex].keyData ),
                                      pNode, pBrother );
        }
        outParameters->iRemoveIndex = INVALID_OFFSET;
        return true;
    }

    // Upward phase : Merge
    if ( bSelectPrev ) {
        _BTree_MergeNodes( &( pParent->arrKeys[iParentIndex - 1].keyData ),
                           pBrother, pNode );
        m_pStorage->Free( pStorage );
        outParameters->iRemoveIndex = iParentIndex - 1;
        outParameters->pStorage = pBrotherStorage;
    } else {
        _BTree_MergeNodes( &( pParent->arrKeys[iParentIndex].keyData ),
                           pNode, pBrother );
        m_pStorage->Free( pBrotherStorage );
        outParameters->iRemoveIndex = iParentIndex;
        outParameters->pStorage = pStorage;
    }

    // Upward Phase : Transmit
    return true;
}

