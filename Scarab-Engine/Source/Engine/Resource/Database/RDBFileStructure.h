/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Resource/Database/RDBFileStructure.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Resource Database, file structure descriptors
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_RESOURCE_DATABASE_RDBFILESTRUCTURE_H
#define SCARAB_ENGINE_RESOURCE_DATABASE_RDBFILESTRUCTURE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Error/ErrorManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
    // Header parameters
#define RDB_HEADER_STATES_OFFSET ( sizeof(RDB_MasterHeader) - sizeof(RDB_StateData) )
#define RDB_HEADER_JOURNAL_OFFSET ( sizeof(RDB_MasterHeader) )

    // Journal parameters
#define RDB_JOURNAL_SIZE 67108864 // 64Mb

#define RDB_JOURNAL_MARK_NOT_COMPLETE 0xaaaaaaaa
#define RDB_JOURNAL_MARK_COMPLETE     0xffffffff
#define RDB_JOURNAL_MARK_EMPTY        0x00000000

    // Block parameters
#define RDB_BLOCK_INIT_COUNT 32
#define RDB_BLOCK_GROW_COUNT 8

#define RDB_BLOCK_SIZE 1108080 // See Comment in RDBFileStructure.cpp

    // Chunk parameters
enum RDB_ChunkType
{
    RDB_CHUNK_NODE = 0,
    RDB_CHUNK_LEAF,
    RDB_CHUNK_DATA,

    RDB_CHUNK_COUNT
};

#define RDB_CHUNK_DATA_SIZE 30728 // See Comment in RDBFileStructure.cpp
extern UInt RDB_ChunkSize[RDB_CHUNK_COUNT];

enum RDB_ChunkTable
{
    RDB_CHUNKTABLE_DIRECTORIES_ROOT = 0,
    RDB_CHUNKTABLE_RESOURCES_ROOT,

    RDB_CHUNKTABLE_COUNT
};

    // BTree parameters
#define RDB_BTREE_NAME_LENGTH 64

#define RDB_BTREE_MAX_KEYS 256
#define RDB_BTREE_MIN_KEYS ( RDB_BTREE_MAX_KEYS >> 1 )

    // Resource types
enum ResourceType
{
    // These are fixed, don't change or you'll have to recreate all your RDBs from scratch/import !
    // A smarter solution could be to write a patching program gathering involved resources
    // and updating their type value ... but this of course would be a mess to maintain ...
    // NEVER CHANGE DIRECTORY VALUE !!!
    RESOURCE_TYPE_DIRECTORY = 0x80000000, // Forces a DWord size too ...
    RESOURCE_TYPE_UNDEFINED = 0,

    // Resource types
        // Rendering types
    RESOURCE_TYPE_BUFFER,
    RESOURCE_TYPE_BITMAP,
    RESOURCE_TYPE_TEXTURE,
    RESOURCE_TYPE_MESH,
    //RESOURCE_TYPE_LIGHT,
    //RESOURCE_TYPE_MATERIAL,
    //RESOURCE_TYPE_CAMERA,
    //RESOURCE_TYPE_EFFECT,
        // Physics types
    //RESOURCE_TYPE_SHAPE,
    //RESOURCE_TYPE_BODY,
    //RESOURCE_TYPE_JOINT,
    //RESOURCE_TYPE_CONTROLLER,
        // World types
    //RESOURCE_TYPE_WORLDCAMERA,
    //RESOURCE_TYPE_WORLDLEAF,
    //RESOURCE_TYPE_WORLDNODE,

        // Reserved values
    //RESOURCE_TYPE_RESERVED_0,
    //RESOURCE_TYPE_RESERVED_1,
    //RESOURCE_TYPE_RESERVED_2,

    //RESOURCE_TYPE_RESERVED_3,
    //RESOURCE_TYPE_RESERVED_4,
    //RESOURCE_TYPE_RESERVED_5,
    //RESOURCE_TYPE_RESERVED_6,
    //RESOURCE_TYPE_RESERVED_7,
    //RESOURCE_TYPE_RESERVED_8,
    //RESOURCE_TYPE_RESERVED_9,
    //RESOURCE_TYPE_RESERVED_10,
    //RESOURCE_TYPE_RESERVED_11,

    RESOURCE_TYPE_COUNT
};

/////////////////////////////////////////////////////////////////////////////////
// Begin : RDB Structure
#pragma pack(push, 1)

/////////////////////////////////////////////////////////////////////////////////
// File pointer
typedef UInt64 RDB_FilePtr; // Size = 8 bytes

inline Int _RDB_FilePtr_Compare( const RDB_FilePtr & rLeft, const RDB_FilePtr & rRight, Void * pUserData );

/////////////////////////////////////////////////////////////////////////////////
// Block layer
typedef UInt RDB_BlockID; // Size = 4 bytes

typedef struct _rdb_block_header
{
    RDB_BlockID idNextBlock;
    RDB_BlockID idPrevBlock;
} RDB_BlockHeader; // Size = 8 bytes

typedef struct _rdb_block_allocator
{
    RDB_BlockID idNextFreeBlock;
    UInt iTotalBlocks;
    UInt iFreeBlocks;
} RDB_BlockAllocator; // Size = 12 bytes

/////////////////////////////////////////////////////////////////////////////////
// Chunk layer
typedef UInt RDB_ChunkID; // Size = 4 bytes

typedef struct _rdb_chunk_ptr
{
    inline Bool operator==( const struct _rdb_chunk_ptr & rhs ) const;
    inline Bool operator!=( const struct _rdb_chunk_ptr & rhs ) const;
    RDB_BlockID idBlock;
    RDB_ChunkID idChunk;
    RDB_ChunkType iType;
} RDB_ChunkPtr; // Size = 12 bytes

typedef struct _rdb_chunk_header
{
    RDB_ChunkPtr pNextChunk;
    RDB_ChunkPtr pPrevChunk;
} RDB_ChunkHeader; // Size = 24 bytes

typedef struct _rdb_chunk_allocator
{
    RDB_BlockID idFirstBlock;
    RDB_BlockID idLastBlock;
    UInt iBlockCount;

    RDB_ChunkPtr pNextFreeChunk;
    UInt iTotalChunks;
    UInt iFreeChunks;
} RDB_ChunkAllocator; // Size = 32 bytes

/////////////////////////////////////////////////////////////////////////////////
// Content layer
typedef UInt RDB_DirectoryID; // Size = 4 bytes

typedef struct _rdb_directory_descriptor
{
    UInt iFlags; // = 0, Unused for now

    RDB_DirectoryID idDirectory;    
} RDB_DirectoryDescriptor; // Size = 8 bytes

typedef struct _rdb_resource_descriptor
{
    UInt iFlags; // = 0, Unused for now

    RDB_ChunkPtr pFirstChunk;
    RDB_ChunkPtr pLastChunk;

    UInt iChunkCount;
    UInt iDataSize;
} RDB_ResourceDescriptor; // Size = 36 bytes

typedef struct _rdb_chunk_data
{
    RDB_ChunkPtr pPrevChunk;
    RDB_ChunkPtr pNextChunk;

    UInt iContentSize;
    Byte arrContent[RDB_CHUNK_DATA_SIZE];
} RDB_Chunk_Data; // Size = 30756 bytes

/////////////////////////////////////////////////////////////////////////////////
// B+Tree layer
typedef struct _rdb_btree_keydata
{
    inline struct _rdb_btree_keydata & operator=( const struct _rdb_btree_keydata & rhs );
    RDB_DirectoryID idParent;
    AChar strName[RDB_BTREE_NAME_LENGTH];
} RDB_BTreeKeyData; // Size = 68 bytes

typedef struct _rdb_btree_key
{
    inline struct _rdb_btree_key & operator=( const struct _rdb_btree_key & rhs );
    RDB_BTreeKeyData keyData;
    RDB_ChunkPtr pLeftChild;
} RDB_BTreeKey; // Size = 80 bytes

typedef struct _rdb_btree_item
{
    inline struct _rdb_btree_item & operator=( const struct _rdb_btree_item & rhs );
    RDB_BTreeKeyData keyData;
    ResourceType iResourceType;
    union {
        RDB_DirectoryDescriptor descDirectory;
        RDB_ResourceDescriptor descResource;
    } Content;
} RDB_BTreeItem; // Size = 108 bytes

typedef struct _rdb_chunk_btree_node
{
    // Child linkage
    UInt iKeyCount;
    RDB_BTreeKey arrKeys[RDB_BTREE_MAX_KEYS];
    RDB_ChunkPtr pRightChild;
} RDB_Chunk_BTreeNode; // Size = 20496 bytes

typedef struct _rdb_chunk_btree_leaf
{
    // Brother linkage
    RDB_ChunkPtr pPrevBrother; 
    RDB_ChunkPtr pNextBrother;

    // Data linkage
    UInt iItemCount;
    RDB_BTreeItem arrItems[RDB_BTREE_MAX_KEYS];
} RDB_Chunk_BTreeLeaf; // Size = 27676 bytes

/////////////////////////////////////////////////////////////////////////////////
// Master Header
typedef struct _rdb_state_data
{
    // Block Allocator
    RDB_BlockAllocator dbBlockAllocator;

    // Chunk Allocators
    RDB_ChunkAllocator dbChunkAllocator[RDB_CHUNK_COUNT];

    // Chunk Table
    RDB_ChunkPtr dbChunkTable[RDB_CHUNKTABLE_COUNT];

    // Next Directory ID
    RDB_DirectoryID dbNextDirectoryID;
} RDB_StateData; // Size = 136 bytes

typedef struct _rdb_master_header
{
    // Magic Number
    Byte iMagic[4]; // "KRDB"

    // Version Number
    UInt iVersion; // must be 0 (for now)

    // Master Header Size
    UInt iThisSize; // = sizeof(RDB_MasterHeader)

    // Journal parameters
    UInt iJournalSize; // = RDB_JOURNAL_SIZE

    // Block parameters
    UInt iBlockSize;
    UInt iBlockInitCount;
    UInt iBlockGrowCount;

    // Chunk parameters
    UInt arrChunkSizes[RDB_CHUNK_COUNT];
    UInt arrChunksByBlock[RDB_CHUNK_COUNT];

    // RDB states
    RDB_StateData dbStateData;

    // Journal area follows ... (fixed size)
    // Block area follows ...   (growing size)
} RDB_MasterHeader; // Size = 188 bytes

/////////////////////////////////////////////////////////////////////////////////
// End : RDB Structure
#pragma pack(pop)

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "RDBFileStructure.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RESOURCE_DATABASE_RDBFILESTRUCTURE_H

