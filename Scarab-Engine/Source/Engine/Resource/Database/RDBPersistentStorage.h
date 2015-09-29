/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Resource/Database/RDBPersistentStorage.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Resource Database, Caching & Journalizing systems
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
#ifndef SCARAB_ENGINE_RESOURCE_DATABASE_RDBPERSISTENTSTORAGE_H
#define SCARAB_ENGINE_RESOURCE_DATABASE_RDBPERSISTENTSTORAGE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Datastruct/Array/Array.h"
#include "../../../Lib/Datastruct/Table/TreeMap.h"
#include "../../../Lib/Datastruct/Tree/SplayTree.h"

#include "../../../Lib/Math/Hash/SHA1.h"

#include "RDBFileStructure.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
    // Locking
enum RDB_LockType
{
    RDB_LOCK_READ = 1,
    RDB_LOCK_WRITE = 2,
    RDB_LOCK_BOTH = 3
};

    // Caching
#define RDB_CACHE_MAX_SIZE 1048576 // 1 Mb

#define RDB_CACHE_FLAG_KEEP 0x80000000
#define RDB_CACHE_FLAG_SYNC 0x00000001

typedef struct _rdb_cache_entry
{
    Byte * pData;
    UInt iSize;
    UInt iFlags; // bit 31 = Keep flag
                 // bit  0 = Sync flag
} RDB_CacheEntry;

typedef TreeMap< RDB_FilePtr, RDB_CacheEntry > RDB_CacheMap;
typedef SplayTree< RDB_FilePtr > RDB_CacheQueue;

/////////////////////////////////////////////////////////////////////////////////
// The RDBPersistentStorage class
class RDBPersistentStorage
{
public:
    RDBPersistentStorage();
    ~RDBPersistentStorage();

    // Name access
    inline const GChar * GetFileName() const;

    // Construction
    Bool Create( const GChar * strFilename );
    Bool Open( const GChar * strFilename );
    Void Close();

    // Chunk interface
    inline RDB_Chunk_BTreeNode * AllocNode( RDB_ChunkPtr * outNodePtr );
    inline RDB_Chunk_BTreeLeaf * AllocLeaf( RDB_ChunkPtr * outLeafPtr );
    inline RDB_Chunk_Data * AllocData( RDB_ChunkPtr * outDataPtr );

    inline Void Free( const RDB_ChunkPtr & pChunkPtr );

    inline RDB_Chunk_BTreeNode * GetNode( const RDB_ChunkPtr & pNodePtr );
    inline RDB_Chunk_BTreeLeaf * GetLeaf( const RDB_ChunkPtr & pLeafPtr );
    inline RDB_Chunk_Data * GetData( const RDB_ChunkPtr & pDataPtr );

    inline const RDB_Chunk_BTreeNode * GetConstNode( const RDB_ChunkPtr & pNodePtr );
    inline const RDB_Chunk_BTreeLeaf * GetConstLeaf( const RDB_ChunkPtr & pLeafPtr );
    inline const RDB_Chunk_Data * GetConstData( const RDB_ChunkPtr & pDataPtr );

    // Chunk table access
    inline const RDB_ChunkPtr & GetTableChunk( RDB_ChunkTable iChunkTableSlot ) const;
    Void SetTableChunk( RDB_ChunkTable iChunkTableSlot, const RDB_ChunkPtr & pChunk );

    // Last Directory ID access
    inline RDB_DirectoryID GetNextDirectoryID() const;
    inline Void SetNextDirectoryID( RDB_DirectoryID iNextDirectoryID );

    // Cache control
    Void Commit();
    Void Discard();

private:
    // File access
    Void _File_Lock( RDB_LockType iLock );
    Void _File_Unlock();

    Void _File_Read( RDB_FilePtr iStorage, UInt iSize, Byte * outData );
    Void _File_Write( RDB_FilePtr iStorage, UInt iSize, const Byte * pData );

    inline RDB_FilePtr _File_GetPtr_HeaderState();
    inline RDB_FilePtr _File_GetPtr_Journal();
    RDB_FilePtr _File_GetPtr_BlockHeader( RDB_BlockID idBlock );
    RDB_FilePtr _File_GetPtr_ChunkHeader( const RDB_ChunkPtr & pChunk );
    RDB_FilePtr _File_GetPtr_ChunkContent( const RDB_ChunkPtr & pChunk, UInt * outSize );

    GChar m_strFileName[FILENAME_LENGTH];
    HFile m_hFile;
    Bool m_bValidFile;
    Bool m_bLocked;
    RDB_LockType m_iLockType;

    // Caching system
    Byte * _Cache_Query( RDB_FilePtr iStorage, UInt iSize, Bool bWillWrite );
    RDB_CacheEntry * _Cache_Load( RDB_FilePtr iStorage, UInt iSize );
    Void _Cache_Journalize();
    Void _Cache_Commit();
    Void _Cache_Update();
    Void _Cache_Release();

    RDB_MasterHeader m_dbMasterHeader;
    Bool m_bSyncStateData;

    RDB_CacheMap m_cacheMap;
    RDB_CacheQueue m_cacheQueue;
    UInt m_iCacheSize;

    // Journalizing system
    Void _Journal_TransactionBegin();
    Void _Journal_TransactionStep( RDB_FilePtr iStorage, UInt iSize, const Byte * pData );
    Void _Journal_TransactionEnd();

    Bool _Journal_Replay();

    RDB_FilePtr m_iTransactionOffset;
    UInt m_iTransactionStepCount;
    SHA1Hasher m_shaTransactionChecksum;

    // Block allocator
    Void _BlockArray_Grow();

    RDB_BlockID _Block_Allocate();
    Void _Block_Free( RDB_BlockID idBlock );

    // Chunk allocator
    Void _ChunkArray_Grow( RDB_ChunkType iChunkType );

    Void _Chunk_Allocate( RDB_ChunkPtr * outNewChunk, RDB_ChunkType iChunkType );
    Void _Chunk_Free( const RDB_ChunkPtr & pChunk );
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "RDBPersistentStorage.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RESOURCE_DATABASE_RDBPERSISTENTSTORAGE_H

