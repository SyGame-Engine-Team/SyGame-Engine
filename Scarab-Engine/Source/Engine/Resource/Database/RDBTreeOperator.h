/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Resource/Database/RDBTreeOperator.h
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
// Header prelude
#ifndef SCARAB_ENGINE_RESOURCE_DATABASE_RDBTREEOPERATOR_H
#define SCARAB_ENGINE_RESOURCE_DATABASE_RDBTREEOPERATOR_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "RDBPersistentStorage.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
    // Recursive parameters
typedef struct _rdb_btree_insert_parameters
{
    RDB_Chunk_BTreeNode * pParent;
    UInt iParentIndex;

    Bool bUpdateLeafHeadKey;

    RDB_BTreeKeyData upwardKey;
    RDB_ChunkPtr pStorage;
} RDB_BTreeInsertParameters;

typedef struct _rdb_btree_remove_parameters
{
    RDB_Chunk_BTreeNode * pParent;
    UInt iParentIndex;

    RDB_BTreeKeyData * pLeafHeadKey;

    UInt iRemoveIndex;
    RDB_ChunkPtr pStorage;
} RDB_BTreeRemoveParameters;

/////////////////////////////////////////////////////////////////////////////////
// The RDBTreeOperator class
class RDBTreeOperator
{
public:
    RDBTreeOperator( RDBPersistentStorage * pStorage );
    ~RDBTreeOperator();

    // Search operations
    Bool SearchDirectory( const RDB_BTreeKeyData & rKey, RDB_BTreeItem * outItem );
    Bool SearchResource( const RDB_BTreeKeyData & rKey, RDB_BTreeItem * outItem );

    // List operations
    Void ListSubDirectories( RDB_DirectoryID idDirectory, Array<RDB_BTreeItem> * outSubDirectories );
    Void ListResources( RDB_DirectoryID idDirectory, Array<RDB_BTreeItem> * outResources );

    // Insert operations
    inline Bool InsertDirectory( const RDB_BTreeItem & rItem );
    inline Bool InsertResource( const RDB_BTreeItem & rItem );

    // Remove operations
    inline Bool RemoveDirectory( const RDB_BTreeKeyData & rKey );
    inline Bool RemoveResource( const RDB_BTreeKeyData & rKey );

    // Update operation
    Bool UpdateResource( const RDB_BTreeKeyData & rKey, const RDB_ResourceDescriptor & descResource );

private:
    // Helpers
    static Int _BTree_Compare( const RDB_BTreeKeyData & rLeft, const RDB_BTreeKeyData & rRight, UInt iPrefixSize = 0 );

    // Node routines
    Bool _BTree_SearchKey( const RDB_Chunk_BTreeNode * pNode, const RDB_BTreeKeyData & rKey, UInt * outIndex ) const;
    Bool _BTree_SearchItem( const RDB_Chunk_BTreeLeaf * pLeaf, const RDB_BTreeKeyData & rKey, UInt * outIndex ) const;

    Void _BTree_InsertKey( RDB_Chunk_BTreeNode * pNode, UInt iIndex, const RDB_BTreeKey & rKey );
    Void _BTree_InsertItem( RDB_Chunk_BTreeLeaf * pLeaf, UInt iIndex, const RDB_BTreeItem & rItem );

    Void _BTree_RemoveKey( RDB_Chunk_BTreeNode * pNode, UInt iIndex );
    Void _BTree_RemoveItem( RDB_Chunk_BTreeLeaf * pLeaf, UInt iIndex );

    Void _BTree_SelectBrotherNode( const RDB_Chunk_BTreeNode * pParentNode, UInt iParentIndex, Bool bMinElseMax,
                                   Bool * outSelectPrev, RDB_ChunkPtr * outBrotherStorage, RDB_Chunk_BTreeNode ** outBrotherNode );
    Void _BTree_SelectBrotherLeaf( const RDB_Chunk_BTreeNode * pParentNode, UInt iParentIndex,
                                   const RDB_Chunk_BTreeLeaf * pLeaf, Bool bMinElseMax, Bool * outSelectPrev,
                                   RDB_ChunkPtr * outBrotherStorage, RDB_Chunk_BTreeLeaf ** outBrotherLeaf );

    // Balance routines
    Void _BTree_BalanceLeft_Nodes( RDB_BTreeKeyData * pParentKey,
                                   RDB_Chunk_BTreeNode * pLeftNode, RDB_Chunk_BTreeNode * pRightNode );
    Void _BTree_BalanceRight_Nodes( RDB_BTreeKeyData * pParentKey,
                                    RDB_Chunk_BTreeNode * pLeftNode, RDB_Chunk_BTreeNode * pRightNode );
    Void _BTree_BalanceLeft_Leaves( RDB_BTreeKeyData * pParentKey,
                                    RDB_Chunk_BTreeLeaf * pLeftLeaf, RDB_Chunk_BTreeLeaf * pRightLeaf );
    Void _BTree_BalanceRight_Leaves( RDB_BTreeKeyData * pParentKey,
                                    RDB_Chunk_BTreeLeaf * pLeftLeaf, RDB_Chunk_BTreeLeaf * pRightLeaf );

    // Search algorithm
    Bool _BTree_Search( const RDB_ChunkPtr & pStorage, const RDB_BTreeKeyData & rKey,
                        RDB_ChunkPtr * outLeafStorage, const RDB_Chunk_BTreeLeaf ** outLeaf, UInt * outIndex );

    // Insert algorithm
    Void _BTree_SplitNode( RDB_Chunk_BTreeNode * pNode, UInt iInsertIndex,
                           const RDB_BTreeKeyData & rKeyData, const RDB_ChunkPtr & rRightChild,
                           RDB_BTreeKeyData * outUpwardKey, RDB_ChunkPtr * outNewStorage );
    Void _BTree_SplitLeaf( const RDB_ChunkPtr & pStorage, RDB_Chunk_BTreeLeaf * pLeaf,
                           UInt iInsertIndex, const RDB_BTreeItem & rItem,
                           RDB_BTreeKeyData * outUpwardKey, RDB_ChunkPtr * outNewStorage );

    Bool _BTree_Insert( const RDB_ChunkPtr & pStorage, const RDB_BTreeItem & rItem,
                        RDB_BTreeInsertParameters * outParameters = NULL );

    // Remove algorithm
    Void _BTree_MergeNodes( const RDB_BTreeKeyData * pParentKey,
                            RDB_Chunk_BTreeNode * pLeftNode, RDB_Chunk_BTreeNode * pRightNode );
    Void _BTree_MergeLeaves( const RDB_ChunkPtr & pLeftStorage,
                             RDB_Chunk_BTreeLeaf * pLeftLeaf, RDB_Chunk_BTreeLeaf * pRightLeaf );

    Bool _BTree_Remove( const RDB_ChunkPtr & pStorage, const RDB_BTreeKeyData & rKey,
                        RDB_BTreeRemoveParameters * outParameters = NULL );

    // Storage instance
    RDBPersistentStorage * m_pStorage;
    RDB_ChunkTable m_iCurRoot;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "RDBTreeOperator.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RESOURCE_DATABASE_RDBTREEOPERATOR_H

