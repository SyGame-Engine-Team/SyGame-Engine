/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Resource/Database/RDBPersistentStorage.cpp
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
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "RDBPersistentStorage.h"

#include "../ResourceManager.h"

/////////////////////////////////////////////////////////////////////////////////
// RDBPersistentStorage implementation
RDBPersistentStorage::RDBPersistentStorage():
    m_hFile(), m_cacheMap(), m_cacheQueue(), m_shaTransactionChecksum()
{
    MemZero( m_strFileName, FILENAME_LENGTH * sizeof(GChar) );
    m_bValidFile = false;
    m_bLocked = false;

    MemZero( &m_dbMasterHeader, sizeof(RDB_MasterHeader) );

    m_cacheMap.UseMemoryContext( ResourceFn->GetMemoryContext(), TEXT("Scratch") );
    m_cacheMap.SetComparator( _RDB_FilePtr_Compare, NULL );
    m_cacheQueue.UseMemoryContext( ResourceFn->GetMemoryContext(), TEXT("Scratch") );
    m_cacheQueue.SetComparator( _RDB_FilePtr_Compare, NULL );
}
RDBPersistentStorage::~RDBPersistentStorage()
{
    // nothing to do
    // Call Close explicitly
}

Bool RDBPersistentStorage::Create( const GChar * strFilename )
{
    Assert( !m_bValidFile );

    UInt i;

    // Save filename
    StringFn->NCopy( m_strFileName, strFilename, FILENAME_LENGTH - 1 );

    // Create the file
    m_hFile = SystemFn->CreateFile( m_strFileName, FILE_WRITE );
    m_bValidFile = m_hFile.IsValid();
    if ( !m_bValidFile )
        return false;

    // Write master header
    MemZero( &m_dbMasterHeader, sizeof(RDB_MasterHeader) );
    m_dbMasterHeader.iMagic[0] = 'K'; m_dbMasterHeader.iMagic[1] = 'R';
    m_dbMasterHeader.iMagic[2] = 'D'; m_dbMasterHeader.iMagic[3] = 'B';
    m_dbMasterHeader.iVersion = 0;
    m_dbMasterHeader.iThisSize = sizeof(RDB_MasterHeader);
    m_dbMasterHeader.iJournalSize = RDB_JOURNAL_SIZE;

    m_dbMasterHeader.iBlockSize = sizeof(RDB_BlockHeader) + RDB_BLOCK_SIZE;
    m_dbMasterHeader.iBlockInitCount = RDB_BLOCK_INIT_COUNT;
    m_dbMasterHeader.iBlockGrowCount = RDB_BLOCK_GROW_COUNT;
    m_dbMasterHeader.dbStateData.dbBlockAllocator.idNextFreeBlock = RDB_CHUNK_COUNT;
    m_dbMasterHeader.dbStateData.dbBlockAllocator.iTotalBlocks = RDB_BLOCK_INIT_COUNT;
    m_dbMasterHeader.dbStateData.dbBlockAllocator.iFreeBlocks = RDB_BLOCK_INIT_COUNT - RDB_CHUNK_COUNT;

    for( i = 0; i < RDB_CHUNK_COUNT; ++i ) {
        m_dbMasterHeader.arrChunkSizes[i] = RDB_ChunkSize[i];
        m_dbMasterHeader.arrChunksByBlock[i] = ( RDB_BLOCK_SIZE / RDB_ChunkSize[i] );

        m_dbMasterHeader.dbStateData.dbChunkAllocator[i].idFirstBlock = (RDB_BlockID)i;
        m_dbMasterHeader.dbStateData.dbChunkAllocator[i].idLastBlock = (RDB_BlockID)i;
        m_dbMasterHeader.dbStateData.dbChunkAllocator[i].iBlockCount = 1;

        m_dbMasterHeader.dbStateData.dbChunkAllocator[i].pNextFreeChunk.idBlock = (RDB_BlockID)i;
        m_dbMasterHeader.dbStateData.dbChunkAllocator[i].pNextFreeChunk.idChunk = (RDB_ChunkID)0;
        m_dbMasterHeader.dbStateData.dbChunkAllocator[i].pNextFreeChunk.iType = (RDB_ChunkType)i;
        m_dbMasterHeader.dbStateData.dbChunkAllocator[i].iTotalChunks = m_dbMasterHeader.arrChunksByBlock[i];
        m_dbMasterHeader.dbStateData.dbChunkAllocator[i].iFreeChunks = m_dbMasterHeader.arrChunksByBlock[i];
    }

    for( i = 0; i < RDB_CHUNKTABLE_COUNT; ++i ) {
        m_dbMasterHeader.dbStateData.dbChunkTable[i].idBlock = (RDB_BlockID)INVALID_OFFSET;
        m_dbMasterHeader.dbStateData.dbChunkTable[i].idChunk = (RDB_ChunkID)INVALID_OFFSET;
        m_dbMasterHeader.dbStateData.dbChunkTable[i].iType = RDB_CHUNK_COUNT;
    }

    m_dbMasterHeader.dbStateData.dbNextDirectoryID = (RDB_DirectoryID)1;

    m_bValidFile = m_hFile.WriteBytes( (const Byte *)(&m_dbMasterHeader), sizeof(RDB_MasterHeader) );
    Assert( m_bValidFile );

    // Write journal area
    Byte arrJournalArea[4096];
    MemZero( arrJournalArea, 4096 );
    UInt iJournalAreaCount = ( RDB_JOURNAL_SIZE >> 12 );
    for( i = 0; i < iJournalAreaCount; ++i ) {
        m_bValidFile = m_hFile.WriteBytes( arrJournalArea, 4096 );
        Assert( m_bValidFile );
    }

    // Create temp block
    ResourceFn->SelectMemory( TEXT("Scratch") );
    Byte *  pBlockData = New Byte[m_dbMasterHeader.iBlockSize];

    // Write pre-allocated blocks
    Byte * pTmp;
    RDB_BlockHeader * pBlockHeader;
    RDB_ChunkHeader * pChunkHeader;
    for( i = 0; i < RDB_CHUNK_COUNT; ++i ) {
        MemZero( pBlockData, m_dbMasterHeader.iBlockSize );
        // Block header
        pBlockHeader = (RDB_BlockHeader*)pBlockData;
        pBlockHeader->idNextBlock = (RDB_BlockID)INVALID_OFFSET;
        pBlockHeader->idPrevBlock = (RDB_BlockID)INVALID_OFFSET;
        // Free chunks list
        pTmp = ( pBlockData + sizeof(RDB_BlockHeader) );
        for( UInt j = 0; j < (m_dbMasterHeader.arrChunksByBlock[i] - 1); ++j ) {
            pChunkHeader = (RDB_ChunkHeader*)pTmp;
            pChunkHeader->pNextChunk.idBlock = (RDB_BlockID)i;
            pChunkHeader->pNextChunk.idChunk = (RDB_ChunkID)( j + 1 );
            pChunkHeader->pNextChunk.iType = (RDB_ChunkType)i;
            pChunkHeader->pPrevChunk.idBlock = (RDB_BlockID)INVALID_OFFSET;
            pChunkHeader->pPrevChunk.idChunk = (RDB_ChunkID)INVALID_OFFSET;
            pChunkHeader->pPrevChunk.iType = (RDB_ChunkType)i;
            pTmp += m_dbMasterHeader.arrChunkSizes[i];
        }
        pChunkHeader = (RDB_ChunkHeader*)pTmp;
        pChunkHeader->pNextChunk.idBlock = (RDB_BlockID)INVALID_OFFSET;
        pChunkHeader->pNextChunk.idChunk = (RDB_ChunkID)INVALID_OFFSET;
        pChunkHeader->pNextChunk.iType = (RDB_ChunkType)i;
        pChunkHeader->pPrevChunk.idBlock = (RDB_BlockID)INVALID_OFFSET;
        pChunkHeader->pPrevChunk.idChunk = (RDB_ChunkID)INVALID_OFFSET;
        pChunkHeader->pPrevChunk.iType = (RDB_ChunkType)i;
        // Write block
        m_bValidFile = m_hFile.WriteBytes( pBlockData, m_dbMasterHeader.iBlockSize );
        Assert( m_bValidFile );
    }

    // Write free blocks list
    for( i = RDB_CHUNK_COUNT; i < (m_dbMasterHeader.iBlockInitCount - 1); ++i ) {
        MemZero( pBlockData, m_dbMasterHeader.iBlockSize );
        pBlockHeader = (RDB_BlockHeader*)pBlockData;
        pBlockHeader->idNextBlock = (RDB_BlockID)( i + 1 );
        pBlockHeader->idPrevBlock = (RDB_BlockID)INVALID_OFFSET;
        m_bValidFile = m_hFile.WriteBytes( pBlockData, m_dbMasterHeader.iBlockSize );
        Assert( m_bValidFile );
    }
    MemZero( pBlockData, m_dbMasterHeader.iBlockSize );
    pBlockHeader = (RDB_BlockHeader*)pBlockData;
    pBlockHeader->idNextBlock = (RDB_BlockID)INVALID_OFFSET;
    pBlockHeader->idPrevBlock = (RDB_BlockID)INVALID_OFFSET;
    m_bValidFile = m_hFile.WriteBytes( pBlockData, m_dbMasterHeader.iBlockSize );
    Assert( m_bValidFile );

    // Destroy temp block
    DeleteA( pBlockData );
    ResourceFn->UnSelectMemory();

    // Done creating the file
    m_hFile.Close();

    // Create cache
    m_bSyncStateData = true;

    m_cacheMap.Create();
    m_cacheQueue.Create();
    m_iCacheSize = 0;

    // Setup transaction state
    m_iTransactionOffset = (RDB_FilePtr)INVALID_OFFSET;
    m_iTransactionStepCount = 0;
    m_shaTransactionChecksum.Reset();

    // Done
    return true;
}
Bool RDBPersistentStorage::Open( const GChar * strFilename )
{
    Assert( !m_bValidFile );

    UInt i;

    // Save filename
    StringFn->NCopy( m_strFileName, strFilename, FILENAME_LENGTH - 1 );

    // Open the file
    m_hFile = SystemFn->OpenFile( m_strFileName, FILE_READ );
    m_bValidFile = m_hFile.IsValid();
    if ( !m_bValidFile )
        return false;

    // Read master header
    m_bValidFile = m_hFile.ReadBytes( (Byte*)(&m_dbMasterHeader), sizeof(RDB_MasterHeader) );
    Assert( m_bValidFile );

    // Check Magic number
    Assert( m_dbMasterHeader.iMagic[0] == 'K' && m_dbMasterHeader.iMagic[1] == 'R' );
    Assert( m_dbMasterHeader.iMagic[2] == 'D' && m_dbMasterHeader.iMagic[3] == 'B' );

    // Check Version number
    Assert( m_dbMasterHeader.iVersion == 0 );

    // Check Master Header size
    Assert( m_dbMasterHeader.iThisSize == sizeof(RDB_MasterHeader) );

    // Done opening the file
    m_hFile.Close();

    // Create cache
    m_bSyncStateData = true;

    m_cacheMap.Create();
    m_cacheQueue.Create();
    m_iCacheSize = 0;

    // Setup transaction state
    m_iTransactionOffset = (RDB_FilePtr)INVALID_OFFSET;
    m_iTransactionStepCount = 0;
    m_shaTransactionChecksum.Reset();

    // Replay journal
    Bool bHasRecovered = _Journal_Replay();
    if ( bHasRecovered )
        DebugPrint( TEXT("RDB Open : Empty journal / Transaction recovered !") );
    else
        DebugPrint( TEXT("RDB Open : Incomplete / Corrupted transaction discarded !") );

    // Clear journal
    RDB_FilePtr iStorage = _File_GetPtr_Journal();
    UInt iEmpty = RDB_JOURNAL_MARK_EMPTY;
    _File_Lock( RDB_LOCK_WRITE );
    _File_Write( iStorage, sizeof(UInt), (const Byte *)( &iEmpty ) );
    _File_Unlock();

    // Pre-Cache TableChunks
    UInt iChunkSize;
    for( i = 0; i < RDB_CHUNKTABLE_COUNT; ++i ) {
        if ( m_dbMasterHeader.dbStateData.dbChunkTable[i].idBlock != (RDB_BlockID)INVALID_OFFSET ) {
            iStorage = _File_GetPtr_ChunkContent( m_dbMasterHeader.dbStateData.dbChunkTable[i], &iChunkSize );
            _Cache_Load( iStorage, iChunkSize );
        }
    }

    // Done
    return true;
}
Void RDBPersistentStorage::Close()
{
    Assert( m_bValidFile );

    // Commit any remaining data
    Commit();

    // Destroy cache
    m_cacheQueue.Destroy();
    m_cacheMap.Destroy();

    // Mark unopened
    MemZero( m_strFileName, FILENAME_LENGTH * sizeof(GChar) );
    m_bValidFile = false;
    m_bLocked = false;

    MemZero( &m_dbMasterHeader, sizeof(RDB_MasterHeader) );
}

Void RDBPersistentStorage::SetTableChunk( RDB_ChunkTable iChunkTableSlot, const RDB_ChunkPtr & pChunk )
{
    Assert( m_bValidFile );
    Assert( iChunkTableSlot < RDB_CHUNKTABLE_COUNT );

    RDB_FilePtr iStorage;
    RDB_CacheMap::Iterator itMap;
    UInt iChunkSize;

    // Remove keep flag for previous chunk
    if ( m_dbMasterHeader.dbStateData.dbChunkTable[iChunkTableSlot].idBlock != (RDB_BlockID)INVALID_OFFSET ) {
        iStorage = _File_GetPtr_ChunkContent( m_dbMasterHeader.dbStateData.dbChunkTable[iChunkTableSlot], &iChunkSize );
        itMap = m_cacheMap.Get( iStorage );
        if ( !(itMap.IsNull()) )
            itMap.GetItem().iFlags &= (~RDB_CACHE_FLAG_KEEP);
    }

    // Update chunk table
    m_bSyncStateData = false;
    m_dbMasterHeader.dbStateData.dbChunkTable[iChunkTableSlot].idBlock = pChunk.idBlock;
    m_dbMasterHeader.dbStateData.dbChunkTable[iChunkTableSlot].idChunk = pChunk.idChunk;
    m_dbMasterHeader.dbStateData.dbChunkTable[iChunkTableSlot].iType = pChunk.iType;

    // Load new chunk if needed, Add keep flag
    iStorage = _File_GetPtr_ChunkContent( pChunk, &iChunkSize );
    itMap = m_cacheMap.Get( iStorage );
    if ( itMap.IsNull() )
        _Cache_Load( iStorage, iChunkSize ); // Sets Keep flag auto
    else
        itMap.GetItem().iFlags |= RDB_CACHE_FLAG_KEEP;
}

Void RDBPersistentStorage::Commit()
{
    Assert( m_bValidFile );

    // Journalize cache
    _Cache_Journalize();

    // Commit cache
    _Cache_Commit();
    
    // Clear journal
    RDB_FilePtr iJournalStorage = _File_GetPtr_Journal();
    UInt iEmpty = RDB_JOURNAL_MARK_EMPTY;
    _File_Lock( RDB_LOCK_WRITE );
    _File_Write( iJournalStorage, sizeof(UInt), (const Byte *)( &iEmpty ) );
    _File_Unlock();

    // Release cache
    if ( m_iCacheSize >= RDB_CACHE_MAX_SIZE ) {
        while ( m_iCacheSize > (RDB_CACHE_MAX_SIZE >> 1) )
            _Cache_Release();
    }
}
Void RDBPersistentStorage::Discard()
{
    Assert( m_bValidFile );

    // Update cache
    _Cache_Update();

    // Release cache
    if ( m_iCacheSize >= RDB_CACHE_MAX_SIZE ) {
        while ( m_iCacheSize > (RDB_CACHE_MAX_SIZE >> 1) )
            _Cache_Release();
    }
}

/////////////////////////////////////////////////////////////////////////////////

Void RDBPersistentStorage::_File_Lock( RDB_LockType iLock )
{
    Assert( !m_bLocked );

    // Select mode
    FileMode iMode = 0;
    switch(iLock) {
        case RDB_LOCK_READ:  iMode |= FILE_READ; break;
        case RDB_LOCK_WRITE: iMode |= FILE_WRITE; break;
        case RDB_LOCK_BOTH:  iMode |= (FILE_READ | FILE_WRITE); break;
        default: Assert(false); break;
    }

    // Open the file
    m_hFile = SystemFn->OpenFile( m_strFileName, iMode );
    m_bValidFile = m_hFile.IsValid();
    Assert( m_bValidFile );

    m_bLocked = true;
    m_iLockType = iLock;
}
Void RDBPersistentStorage::_File_Unlock()
{
    Assert( m_bLocked );

    // Close the file
    m_hFile.Close();

    m_bLocked = false;
}

Void RDBPersistentStorage::_File_Read( RDB_FilePtr iStorage, UInt iSize, Byte * outData )
{
    // Require read-lock
    Assert( m_bLocked );
    Assert( (m_iLockType & RDB_LOCK_READ) != 0 );

    // Seek
    m_bValidFile = m_hFile.Seek( iStorage );
    Assert( m_bValidFile );

    // Read
    m_bValidFile = m_hFile.ReadBytes( outData, iSize );
    Assert( m_bValidFile );
}
Void RDBPersistentStorage::_File_Write( RDB_FilePtr iStorage, UInt iSize, const Byte * pData )
{
    // Require Write lock
    Assert( m_bLocked );
    Assert( (m_iLockType & RDB_LOCK_WRITE) != 0 );

    // Seek
    m_bValidFile = m_hFile.Seek( iStorage );
    Assert( m_bValidFile );

    // Write
    m_bValidFile = m_hFile.WriteBytes( pData, iSize );
    Assert( m_bValidFile );
}

RDB_FilePtr RDBPersistentStorage::_File_GetPtr_BlockHeader( RDB_BlockID idBlock )
{
    RDB_FilePtr iStorage = (RDB_FilePtr)( m_dbMasterHeader.iThisSize + m_dbMasterHeader.iJournalSize );

    RDB_FilePtr iTemp = (RDB_FilePtr)( idBlock );
    iTemp *= (RDB_FilePtr)( m_dbMasterHeader.iBlockSize );
    iStorage += iTemp;

    return iStorage;
}
RDB_FilePtr RDBPersistentStorage::_File_GetPtr_ChunkHeader( const RDB_ChunkPtr & pChunk )
{
    RDB_FilePtr iStorage = (RDB_FilePtr)( m_dbMasterHeader.iThisSize + m_dbMasterHeader.iJournalSize );

    RDB_FilePtr iTemp = (RDB_FilePtr)( pChunk.idBlock );
    iTemp *= (RDB_FilePtr)( m_dbMasterHeader.iBlockSize );
    iStorage += iTemp;

    iStorage += (RDB_FilePtr)( sizeof(RDB_BlockHeader) );

    iTemp = (RDB_FilePtr)( pChunk.idChunk );
    iTemp *= (RDB_FilePtr)( m_dbMasterHeader.arrChunkSizes[pChunk.iType] );
    iStorage += iTemp;

    return iStorage;
}
RDB_FilePtr RDBPersistentStorage::_File_GetPtr_ChunkContent( const RDB_ChunkPtr & pChunk, UInt * outSize )
{
    RDB_FilePtr iStorage = (RDB_FilePtr)( m_dbMasterHeader.iThisSize + m_dbMasterHeader.iJournalSize );

    RDB_FilePtr iTemp = (RDB_FilePtr)( pChunk.idBlock );
    iTemp *= (RDB_FilePtr)( m_dbMasterHeader.iBlockSize );
    iStorage += iTemp;

    iStorage += (RDB_FilePtr)( sizeof(RDB_BlockHeader) );

    iTemp = (RDB_FilePtr)( pChunk.idChunk );
    iTemp *= (RDB_FilePtr)( m_dbMasterHeader.arrChunkSizes[pChunk.iType] );
    iStorage += iTemp;

    iStorage += (RDB_FilePtr)( sizeof(RDB_ChunkHeader) );

    *outSize = ( m_dbMasterHeader.arrChunkSizes[pChunk.iType] - sizeof(RDB_ChunkHeader) );
    return iStorage;
}

/////////////////////////////////////////////////////////////////////////////////

Byte * RDBPersistentStorage::_Cache_Query( RDB_FilePtr iStorage, UInt iSize, Bool bWillWrite )
{
    RDB_CacheEntry * pCacheEntry;

    // Cache query
    RDB_CacheMap::Iterator itMap = m_cacheMap.Get( iStorage );
    if ( itMap.IsNull() )
        pCacheEntry = _Cache_Load( iStorage, iSize );
    else
        pCacheEntry = itMap.GetItemPtr();
    Assert( pCacheEntry->pData != NULL );
    Assert( pCacheEntry->iSize == iSize );

    // Update Queue
    Bool bRaised = m_cacheQueue.Raise( iStorage );
    Assert( bRaised );

    // Update flag
    if ( bWillWrite )
        pCacheEntry->iFlags &= (~RDB_CACHE_FLAG_SYNC);

    return pCacheEntry->pData;
}
RDB_CacheEntry * RDBPersistentStorage::_Cache_Load( RDB_FilePtr iStorage, UInt iSize )
{
    // Allocate memory
    ResourceFn->SelectMemory( TEXT("CacheMemory") );
    Byte * pCacheData = New Byte[iSize];
    m_iCacheSize += iSize;
    ResourceFn->UnSelectMemory();

    // Load data from storage
    _File_Lock( RDB_LOCK_READ );
    _File_Read( iStorage, iSize, pCacheData );
    _File_Unlock();

    // Create cache entry
    RDB_CacheEntry newCacheEntry;
    newCacheEntry.pData = pCacheData;
    newCacheEntry.iSize = iSize;
    newCacheEntry.iFlags = RDB_CACHE_FLAG_SYNC;

    // Check presence in ChunkTable, set keep flag
    RDB_FilePtr iTableChunkStorage;
    UInt iTableChunkSize;
    for( UInt i = 0; i < RDB_CHUNKTABLE_COUNT; ++i ) {
        if ( m_dbMasterHeader.dbStateData.dbChunkTable[i].idBlock != (RDB_BlockID)INVALID_OFFSET ) {
            iTableChunkStorage = _File_GetPtr_ChunkContent( m_dbMasterHeader.dbStateData.dbChunkTable[i], &iTableChunkSize );
            if ( iTableChunkStorage == iStorage ) {
                newCacheEntry.iFlags |= RDB_CACHE_FLAG_KEEP;
                break;
            }
        }
    }

    // Insert into cache map
    RDB_CacheMap::Iterator itMap;
    Bool bInserted = m_cacheMap.Insert( iStorage, newCacheEntry, &itMap );
    Assert( bInserted && ( !(itMap.IsNull()) ) );

    // Insert into cache queue
    bInserted = m_cacheQueue.Insert( iStorage );
    Assert( bInserted );

    return itMap.GetItemPtr();
}
Void RDBPersistentStorage::_Cache_Journalize()
{
    // Begin transaction
    Bool bJournalizing = false;

    // Content data
    RDB_CacheEntry * pCacheEntry;
    RDB_CacheMap::Iterator itMap = m_cacheMap.Begin();
    while( !( itMap.IsNull() ) ) {
        pCacheEntry = itMap.GetItemPtr();
        if ( (pCacheEntry->iFlags & RDB_CACHE_FLAG_SYNC) == 0 ) {
            if ( !bJournalizing ) {
                _Journal_TransactionBegin();
                bJournalizing = true;
            }
            _Journal_TransactionStep( itMap.GetKey(), pCacheEntry->iSize, pCacheEntry->pData );
        }
        ++itMap;
    }

    // State data
    if ( !m_bSyncStateData ) {
        if ( !bJournalizing ) {
            _Journal_TransactionBegin();
            bJournalizing = true;
        }
        _Journal_TransactionStep (
            _File_GetPtr_HeaderState(), sizeof(RDB_StateData),
            (const Byte *)( &(m_dbMasterHeader.dbStateData) )
        );
    }

    // End transaction
    if ( bJournalizing )
        _Journal_TransactionEnd();
}
Void RDBPersistentStorage::_Cache_Commit()
{
    // Begin committing
    Bool bCommitting = false;

    // Content data
    RDB_CacheEntry * pCacheEntry;
    RDB_CacheMap::Iterator itMap = m_cacheMap.Begin();
    while( !( itMap.IsNull() ) ) {
        pCacheEntry = itMap.GetItemPtr();
        if ( (pCacheEntry->iFlags & RDB_CACHE_FLAG_SYNC) == 0 ) {
            if ( !bCommitting ) {
                _File_Lock( RDB_LOCK_WRITE );
                bCommitting = true;
            }
            _File_Write( itMap.GetKey(), pCacheEntry->iSize, pCacheEntry->pData );
        }
        ++itMap;
    }

    // State data
    if ( !m_bSyncStateData ) {
        if ( !bCommitting ) {
            _File_Lock( RDB_LOCK_WRITE );
            bCommitting = true;
        }
        _File_Write (
            _File_GetPtr_HeaderState(), sizeof(RDB_StateData),
            (const Byte *)( &(m_dbMasterHeader.dbStateData) )
        );
    }

    // End committing
    if ( bCommitting )
        _File_Unlock();

    // Sync content data
    itMap = m_cacheMap.Begin();
    while( !( itMap.IsNull() ) ) {
        pCacheEntry = itMap.GetItemPtr();
        pCacheEntry->iFlags |= RDB_CACHE_FLAG_SYNC;
        ++itMap;
    }

    // Sync state data
    m_bSyncStateData = true;
}
Void RDBPersistentStorage::_Cache_Update()
{
    // Begin updating
    Bool bUpdating = false;

    // Content data
    RDB_CacheEntry * pCacheEntry;
    RDB_CacheMap::Iterator itMap = m_cacheMap.Begin();
    while( !( itMap.IsNull() ) ) {
        pCacheEntry = itMap.GetItemPtr();
        if ( (pCacheEntry->iFlags & RDB_CACHE_FLAG_SYNC) == 0 ) {
            if ( !bUpdating ) {
                _File_Lock( RDB_LOCK_READ );
                bUpdating = true;
            }
            _File_Read( itMap.GetKey(), pCacheEntry->iSize, pCacheEntry->pData );
        }
        ++itMap;
    }

    // State data
    if ( !m_bSyncStateData ) {
        if ( !bUpdating ) {
            _File_Lock( RDB_LOCK_READ );
            bUpdating = true;
        }
        _File_Read (
            _File_GetPtr_HeaderState(), sizeof(RDB_StateData),
            (Byte *)( &(m_dbMasterHeader.dbStateData) )
        );
    }

    // End updating
    if ( bUpdating )
        _File_Unlock();

    // Sync content data
    itMap = m_cacheMap.Begin();
    while( !( itMap.IsNull() ) ) {
        pCacheEntry = itMap.GetItemPtr();
        pCacheEntry->iFlags |= RDB_CACHE_FLAG_SYNC;
        ++itMap;
    }

    // Sync meta data
    m_bSyncStateData = true;
}
Void RDBPersistentStorage::_Cache_Release()
{
    UInt iKeepCount = ( m_cacheMap.Count() >> 1 );

    RDB_FilePtr iStorage;
    UInt i;
    Bool bOk;

    // Create temp storage
    Array<RDB_FilePtr> arrKeptItems;
    arrKeptItems.UseMemoryContext( ResourceFn->GetMemoryContext(), TEXT("Scratch") );
    arrKeptItems.Create();
    bOk = arrKeptItems.EnsureCapacity( iKeepCount + RDB_CHUNKTABLE_COUNT );
    Assert( bOk );

    // Get iKeepCount most recent items
    for( i = 0; i < iKeepCount; ++i ) {
        bOk = m_cacheQueue.Extract( &iStorage );
        Assert( bOk );
        arrKeptItems.Push( iStorage );
    }

    ResourceFn->SelectMemory( TEXT("CacheMemory") );

    // Discard all remaining
    while( !(m_cacheQueue.IsEmpty()) ) {
        bOk = m_cacheQueue.Extract( &iStorage );
        Assert( bOk );

        const RDB_CacheEntry & rCacheEntry = m_cacheMap[iStorage];
        Assert( rCacheEntry.pData != NULL );
        Assert( (rCacheEntry.iFlags & RDB_CACHE_FLAG_SYNC) != 0 );

        if ( (rCacheEntry.iFlags & RDB_CACHE_FLAG_KEEP) != 0 )
            arrKeptItems.Push( iStorage );
        else {
            DeleteA( rCacheEntry.pData );
            m_iCacheSize -= rCacheEntry.iSize;
            bOk = m_cacheMap.Remove( iStorage );
            Assert( bOk );
        }
    }

    ResourceFn->UnSelectMemory();

    // Rebuild queue
    UInt iCount = arrKeptItems.Count();
    for( i = 0; i < iCount; ++i ) {
        arrKeptItems.Pop( iStorage );
        bOk = m_cacheQueue.Insert( iStorage );
        Assert( bOk );
    }

    // Destroy temp storage
    arrKeptItems.Destroy();
}

/////////////////////////////////////////////////////////////////////////////////

Void RDBPersistentStorage::_Journal_TransactionBegin()
{
    Assert( m_iTransactionOffset == (RDB_FilePtr)INVALID_OFFSET );

    RDB_FilePtr iJournalStorage = _File_GetPtr_Journal();

    // Start transaction
    m_iTransactionOffset = (RDB_FilePtr)0;
    m_iTransactionStepCount = 0;
    m_shaTransactionChecksum.Reset();
    _File_Lock( RDB_LOCK_WRITE );

    // Write completion mark
    UInt iNotCompleted = RDB_JOURNAL_MARK_NOT_COMPLETE;
    _File_Write( iJournalStorage + m_iTransactionOffset, sizeof(UInt), (const Byte *)( &iNotCompleted ) );
    m_iTransactionOffset += sizeof(UInt);

    // Write null checksum
    HashSHA1 shaChecksum;
    MemZero( shaChecksum, sizeof(HashSHA1) );
    _File_Write( iJournalStorage + m_iTransactionOffset, sizeof(HashSHA1), shaChecksum );
    m_iTransactionOffset += sizeof(HashSHA1);

    // Write step count
    _File_Write( iJournalStorage + m_iTransactionOffset, sizeof(UInt), (const Byte *)( &m_iTransactionStepCount ) );
    m_iTransactionOffset += sizeof(UInt);    
}
Void RDBPersistentStorage::_Journal_TransactionStep( RDB_FilePtr iStorage, UInt iSize, const Byte * pData )
{
    Assert( m_iTransactionOffset != (RDB_FilePtr)INVALID_OFFSET );

    RDB_FilePtr iJournalStorage = _File_GetPtr_Journal();

    // Step transaction
    ++m_iTransactionStepCount;
    m_shaTransactionChecksum.Update( &iStorage, sizeof(RDB_FilePtr) );
    m_shaTransactionChecksum.Update( &iSize, sizeof(UInt) );
    m_shaTransactionChecksum.Update( pData, iSize );

    // Write step data
    _File_Write( iJournalStorage + m_iTransactionOffset, sizeof(RDB_FilePtr), (const Byte *)( &iStorage ) );
    m_iTransactionOffset += sizeof(RDB_FilePtr);
    _File_Write( iJournalStorage + m_iTransactionOffset, sizeof(UInt), (const Byte *)( &iSize ) );
    m_iTransactionOffset += sizeof(UInt);
    _File_Write( iJournalStorage + m_iTransactionOffset, iSize, pData );
    m_iTransactionOffset += iSize;

    // Write step count
    RDB_FilePtr iStepCountOffset = (RDB_FilePtr)( sizeof(UInt) + sizeof(HashSHA1) );
    _File_Write( iJournalStorage + iStepCountOffset, sizeof(UInt), (const Byte *)( &m_iTransactionStepCount ) );
}
Void RDBPersistentStorage::_Journal_TransactionEnd()
{
    Assert( m_iTransactionOffset != (RDB_FilePtr)INVALID_OFFSET );

    RDB_FilePtr iJournalStorage = _File_GetPtr_Journal();

    // Finalize checksum
    HashSHA1 shaChecksum;
    m_shaTransactionChecksum.Update( &m_iTransactionStepCount, sizeof(UInt) );
    m_shaTransactionChecksum.GetHash( shaChecksum );

    // Write checksum
    _File_Write( iJournalStorage + sizeof(UInt), sizeof(HashSHA1), shaChecksum );

    // Write completion mark
    UInt iCompleted = RDB_JOURNAL_MARK_COMPLETE;
    _File_Write( iJournalStorage, sizeof(UInt), (const Byte *)( &iCompleted ) );

    // End transaction
    _File_Unlock();
    m_iTransactionOffset = (RDB_FilePtr)INVALID_OFFSET;
}

Bool RDBPersistentStorage::_Journal_Replay()
{
    RDB_FilePtr iJournalStorage = _File_GetPtr_Journal();
    RDB_FilePtr iJournalOffset = 0;
    UInt i;

    _File_Lock( RDB_LOCK_READ );

    // Read completion mark
    UInt iCompleteMark;
    _File_Read( iJournalStorage + iJournalOffset, sizeof(UInt), (Byte *)( &iCompleteMark ) );
    iJournalOffset += sizeof(UInt);

    // Verify emptyness
    if ( iCompleteMark == RDB_JOURNAL_MARK_EMPTY ) {
        _File_Unlock();

        // No transaction
        return true;
    }

    // Verify completion
    if ( iCompleteMark != RDB_JOURNAL_MARK_COMPLETE ) {
        _File_Unlock();

        // Incomplete/Corrupted transaction
        return false;
    }

    // Read reference checksum
    HashSHA1 shaRefChecksum;
    _File_Read( iJournalStorage + iJournalOffset, sizeof(HashSHA1), shaRefChecksum );
    iJournalOffset += sizeof(HashSHA1);

    // Read step count
    UInt iStepCount;
    _File_Read( iJournalStorage + iJournalOffset, sizeof(UInt), (Byte *)( &iStepCount ) );
    iJournalOffset += sizeof(UInt);

    // Create transaction data
    ResourceFn->SelectMemory( TEXT("Scratch") );
    RDB_FilePtr * arrStorages = New RDB_FilePtr[iStepCount];
    UInt * arrSizes = New UInt[iStepCount];
    Byte ** arrData = New Byte*[iStepCount];

    // Read transaction data
    for( i = 0; i < iStepCount; ++i ) {
        // Read file ptr
        _File_Read( iJournalStorage + iJournalOffset, sizeof(RDB_FilePtr), (Byte *)( arrStorages + i ) );
        iJournalOffset += sizeof(RDB_FilePtr);

        // Read size
        _File_Read( iJournalStorage + iJournalOffset, sizeof(UInt), (Byte *)( arrSizes + i ) );
        iJournalOffset += sizeof(UInt);

        arrData[i] = New Byte[ arrSizes[i] ];

        // Read data
        _File_Read( iJournalStorage + iJournalOffset, arrSizes[i], arrData[i] );
        iJournalOffset += arrSizes[i];
    }

    _File_Unlock();

    // Compute test checksum
    HashSHA1 shaTestChecksum;
    m_shaTransactionChecksum.Reset();
    for( i = 0; i < iStepCount; ++i ) {
        m_shaTransactionChecksum.Update( (arrStorages + i), sizeof(RDB_FilePtr) );
        m_shaTransactionChecksum.Update( (arrSizes + i), sizeof(UInt) );
        m_shaTransactionChecksum.Update( arrData[i], arrSizes[i] );
    }
    m_shaTransactionChecksum.Update( &iStepCount, sizeof(UInt) );
    m_shaTransactionChecksum.GetHash( shaTestChecksum );

    // Verify integrity
    if ( MemCmp( shaTestChecksum, shaRefChecksum, sizeof(HashSHA1) ) != 0 ) {
        // Destroy transaction data
        for( i = 0; i < iStepCount; ++i )
            DeleteA( arrData[i] );
        DeleteA( arrData );
        DeleteA( arrSizes );
        DeleteA( arrStorages );
        ResourceFn->UnSelectMemory();

        // Corrupted transaction
        return false;
    }

    // Execute transaction
    _File_Lock( RDB_LOCK_WRITE );
    for( i = 0; i < iStepCount; ++i )
        _File_Write( arrStorages[i], arrSizes[i], arrData[i] );
    _File_Unlock();

    // Destroy transaction data
    for( i = 0; i < iStepCount; ++i )
        DeleteA( arrData[i] );
    DeleteA( arrData );
    DeleteA( arrSizes );
    DeleteA( arrStorages );
    ResourceFn->UnSelectMemory();

    // Recovered transaction
    return true;
}

/////////////////////////////////////////////////////////////////////////////////

Void RDBPersistentStorage::_BlockArray_Grow()
{
    // Require unlocked
    Assert( !m_bLocked );

    // Create temp block
    ResourceFn->SelectMemory( TEXT("Scratch") );
    Byte * pBlock = New Byte[m_dbMasterHeader.iBlockSize];
    MemZero( pBlock, m_dbMasterHeader.iBlockSize );
    RDB_BlockHeader * pBlockHeader = (RDB_BlockHeader*)pBlock;

    // Begin grow sequence
    m_hFile = SystemFn->OpenFile( m_strFileName, FILE_WRITE );
    m_bValidFile = m_hFile.IsValid();
    Assert( m_bValidFile );
    m_bValidFile = m_hFile.Seek( FILE_SEEK_END, 0 );
    Assert( m_bValidFile );

    // Append free blocks
    UInt iTotalBlocks = m_dbMasterHeader.dbStateData.dbBlockAllocator.iTotalBlocks;
    for( UInt i = 0; i < (m_dbMasterHeader.iBlockGrowCount - 1); ++i ) {
        pBlockHeader->idNextBlock = (RDB_BlockID)( iTotalBlocks + (i + 1) );
        pBlockHeader->idPrevBlock = (RDB_BlockID)INVALID_OFFSET;
        m_bValidFile = m_hFile.WriteBytes( pBlock, m_dbMasterHeader.iBlockSize );
        Assert( m_bValidFile );
    }
    pBlockHeader->idNextBlock = m_dbMasterHeader.dbStateData.dbBlockAllocator.idNextFreeBlock;
    pBlockHeader->idPrevBlock = (RDB_BlockID)INVALID_OFFSET;
    m_bValidFile = m_hFile.WriteBytes( pBlock, m_dbMasterHeader.iBlockSize );
    Assert( m_bValidFile );

    // End grow sequence
    m_hFile.Close();

    // Destroy temp block
    DeleteA( pBlock );
    ResourceFn->UnSelectMemory();

    // Update Block allocator
    m_bSyncStateData = false;
    m_dbMasterHeader.dbStateData.dbBlockAllocator.idNextFreeBlock = iTotalBlocks;
    m_dbMasterHeader.dbStateData.dbBlockAllocator.iTotalBlocks += m_dbMasterHeader.iBlockGrowCount;
    m_dbMasterHeader.dbStateData.dbBlockAllocator.iFreeBlocks += m_dbMasterHeader.iBlockGrowCount;
}

RDB_BlockID RDBPersistentStorage::_Block_Allocate()
{
    // Grow if needed
    if ( m_dbMasterHeader.dbStateData.dbBlockAllocator.idNextFreeBlock == (RDB_BlockID)INVALID_OFFSET )
        _BlockArray_Grow();

    // Allocate the new block
    RDB_BlockID idNewBlock = m_dbMasterHeader.dbStateData.dbBlockAllocator.idNextFreeBlock;
    Assert( idNewBlock != (RDB_BlockID)INVALID_OFFSET );

    // Retrieve this block's header
    RDB_FilePtr iStorage = _File_GetPtr_BlockHeader( idNewBlock );
    const RDB_BlockHeader * pBlockHeader = (const RDB_BlockHeader*)(
        _Cache_Query( iStorage, sizeof(RDB_BlockHeader), false )
    );

    // Update block allocator
    m_bSyncStateData = false;
    m_dbMasterHeader.dbStateData.dbBlockAllocator.idNextFreeBlock = pBlockHeader->idNextBlock;
    --(m_dbMasterHeader.dbStateData.dbBlockAllocator.iFreeBlocks);

    return idNewBlock;
}
Void RDBPersistentStorage::_Block_Free( RDB_BlockID idBlock )
{
    // Retrieve this block's header
    RDB_FilePtr iStorage = _File_GetPtr_BlockHeader( idBlock );
    RDB_BlockHeader * pBlockHeader = (RDB_BlockHeader*)(
        _Cache_Query( iStorage, sizeof(RDB_BlockHeader), true )
    );

    // Update free blocks list
    pBlockHeader->idNextBlock = m_dbMasterHeader.dbStateData.dbBlockAllocator.idNextFreeBlock;

    // Update block allocator
    m_bSyncStateData = false;
    m_dbMasterHeader.dbStateData.dbBlockAllocator.idNextFreeBlock = idBlock;
    ++(m_dbMasterHeader.dbStateData.dbBlockAllocator.iFreeBlocks);

    // Shrink if possible
    ////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////

Void RDBPersistentStorage::_ChunkArray_Grow( RDB_ChunkType iChunkType )
{
    // Selection
    UInt iIndex = (UInt)( iChunkType );
    RDB_ChunkAllocator * pChunkAllocator = ( m_dbMasterHeader.dbStateData.dbChunkAllocator + iIndex );
    UInt iChunksByBlock = m_dbMasterHeader.arrChunksByBlock[iIndex];

    // Allocate a new block
    RDB_BlockID idNewBlock = _Block_Allocate();

    // Retrieve new block's header
    RDB_FilePtr iNewBlockStorage = _File_GetPtr_BlockHeader( idNewBlock );
    RDB_BlockHeader * pNewBlockHeader = (RDB_BlockHeader*)(
        _Cache_Query( iNewBlockStorage, sizeof(RDB_BlockHeader), true )
    );

    // Retrieve last block's header
    RDB_FilePtr iLastBlockStorage = _File_GetPtr_BlockHeader( pChunkAllocator->idLastBlock );
    RDB_BlockHeader * pLastBlockHeader = (RDB_BlockHeader*)(
        _Cache_Query( iLastBlockStorage, sizeof(RDB_BlockHeader), true )
    );

    // Update block list
    pNewBlockHeader->idNextBlock = (RDB_BlockID)INVALID_OFFSET;
    pNewBlockHeader->idPrevBlock = pChunkAllocator->idLastBlock;
    pLastBlockHeader->idNextBlock = idNewBlock;

    // Initialize free chunks list
    RDB_FilePtr iChunkStorage;
    RDB_ChunkHeader * pChunkHeader;
    RDB_ChunkPtr pChunk;
    pChunk.idBlock = idNewBlock;
    pChunk.iType = iChunkType;
    for( UInt i = 0; i < (iChunksByBlock - 1); ++i ) {
        pChunk.idChunk = i;
        iChunkStorage = _File_GetPtr_ChunkHeader( pChunk );
        pChunkHeader = (RDB_ChunkHeader*)(
            _Cache_Query( iChunkStorage, sizeof(RDB_ChunkHeader), true )
        );
        pChunkHeader->pNextChunk.idBlock = idNewBlock;
        pChunkHeader->pNextChunk.idChunk = (RDB_ChunkID)( i + 1 );
        pChunkHeader->pNextChunk.iType = iChunkType;
    }
    pChunk.idChunk = (iChunksByBlock - 1);
    iChunkStorage = _File_GetPtr_ChunkHeader( pChunk );
    pChunkHeader = (RDB_ChunkHeader*)(
        _Cache_Query( iChunkStorage, sizeof(RDB_ChunkHeader), true )
    );
    pChunkHeader->pNextChunk.idBlock = (RDB_BlockID)INVALID_OFFSET;
    pChunkHeader->pNextChunk.idChunk = (RDB_ChunkID)INVALID_OFFSET;
    pChunkHeader->pNextChunk.iType = iChunkType;

    // Update chunk allocator
    m_bSyncStateData = false;
    pChunkAllocator->pNextFreeChunk.idBlock = idNewBlock;
    pChunkAllocator->pNextFreeChunk.idChunk = 0;
    pChunkAllocator->pNextFreeChunk.iType = iChunkType;
    pChunkAllocator->iTotalChunks += iChunksByBlock;
    pChunkAllocator->iFreeChunks += iChunksByBlock;
    pChunkAllocator->idLastBlock = idNewBlock;
    ++(pChunkAllocator->iBlockCount);
}

Void RDBPersistentStorage::_Chunk_Allocate( RDB_ChunkPtr * outNewChunk, RDB_ChunkType iChunkType )
{
    // Selection
    UInt iIndex = (UInt)( iChunkType );
    RDB_ChunkAllocator * pChunkAllocator = ( m_dbMasterHeader.dbStateData.dbChunkAllocator + iIndex );

    // Grow if needed
    if ( pChunkAllocator->pNextFreeChunk.idBlock == (RDB_BlockID)INVALID_OFFSET )
        _ChunkArray_Grow( iChunkType );

    // Allocate the new chunk
    outNewChunk->idBlock = pChunkAllocator->pNextFreeChunk.idBlock;
    outNewChunk->idChunk = pChunkAllocator->pNextFreeChunk.idChunk;
    outNewChunk->iType = iChunkType;

    // Retrieve this chunk's header
    RDB_FilePtr iStorage = _File_GetPtr_ChunkHeader( *outNewChunk );
    const RDB_ChunkHeader * pChunkHeader = (const RDB_ChunkHeader*)(
        _Cache_Query( iStorage, sizeof(RDB_ChunkHeader), false )
    );

    // Update chunk allocator
    m_bSyncStateData = false;
    pChunkAllocator->pNextFreeChunk.idBlock = pChunkHeader->pNextChunk.idBlock;
    pChunkAllocator->pNextFreeChunk.idChunk = pChunkHeader->pNextChunk.idChunk;
    pChunkAllocator->pNextFreeChunk.iType = pChunkHeader->pNextChunk.iType;
    --(pChunkAllocator->iFreeChunks);
}
Void RDBPersistentStorage::_Chunk_Free( const RDB_ChunkPtr & pChunk )
{
    // Selection
    UInt iIndex = (UInt)( pChunk.iType );
    RDB_ChunkAllocator * pChunkAllocator = ( m_dbMasterHeader.dbStateData.dbChunkAllocator + iIndex );

    // Retrieve this chunk's header
    RDB_FilePtr iStorage = _File_GetPtr_ChunkHeader( pChunk );
    RDB_ChunkHeader * pChunkHeader = (RDB_ChunkHeader*)(
        _Cache_Query( iStorage, sizeof(RDB_ChunkHeader), true )
    );

    // Update free chunks list
    pChunkHeader->pNextChunk.idBlock = pChunkAllocator->pNextFreeChunk.idBlock;
    pChunkHeader->pNextChunk.idChunk = pChunkAllocator->pNextFreeChunk.idChunk;
    pChunkHeader->pNextChunk.iType = pChunkAllocator->pNextFreeChunk.iType;

    // Update chunk allocator
    m_bSyncStateData = false;
    pChunkAllocator->pNextFreeChunk.idBlock = pChunk.idBlock;
    pChunkAllocator->pNextFreeChunk.idChunk = pChunk.idChunk;
    pChunkAllocator->pNextFreeChunk.iType = pChunk.iType;
    ++(pChunkAllocator->iFreeChunks);

    // Shrink if possible
    ////////////////////////////////
}

