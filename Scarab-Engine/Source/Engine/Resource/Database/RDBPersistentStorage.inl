/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Resource/Database/RDBPersistentStorage.inl
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
// RDBPersistentStorage implementation
inline const GChar * RDBPersistentStorage::GetFileName() const {
    return m_strFileName;
}

inline RDB_Chunk_BTreeNode * RDBPersistentStorage::AllocNode( RDB_ChunkPtr * outNodePtr ) {
    Assert( m_bValidFile );
    _Chunk_Allocate( outNodePtr, RDB_CHUNK_NODE );
    UInt iSize;
    RDB_FilePtr iStorage = _File_GetPtr_ChunkContent( *outNodePtr, &iSize );
    Assert( iSize == sizeof(RDB_Chunk_BTreeNode) );
    return (RDB_Chunk_BTreeNode*)( _Cache_Query( iStorage, iSize, true ) );
}
inline RDB_Chunk_BTreeLeaf * RDBPersistentStorage::AllocLeaf( RDB_ChunkPtr * outLeafPtr ) {
    Assert( m_bValidFile );
    _Chunk_Allocate( outLeafPtr, RDB_CHUNK_LEAF );
    UInt iSize;
    RDB_FilePtr iStorage = _File_GetPtr_ChunkContent( *outLeafPtr, &iSize );
    Assert( iSize == sizeof(RDB_Chunk_BTreeLeaf) );
    return (RDB_Chunk_BTreeLeaf*)( _Cache_Query( iStorage, iSize, true ) );
}
inline RDB_Chunk_Data * RDBPersistentStorage::AllocData( RDB_ChunkPtr * outDataPtr ) {
    Assert( m_bValidFile );
    _Chunk_Allocate( outDataPtr, RDB_CHUNK_DATA );
    UInt iSize;
    RDB_FilePtr iStorage = _File_GetPtr_ChunkContent( *outDataPtr, &iSize );
    Assert( iSize == sizeof(RDB_Chunk_Data) );
    return (RDB_Chunk_Data*)( _Cache_Query( iStorage, iSize, true ) );
}

inline Void RDBPersistentStorage::Free( const RDB_ChunkPtr & pChunkPtr ) {
    Assert( m_bValidFile );
    _Chunk_Free( pChunkPtr );
}

inline RDB_Chunk_BTreeNode * RDBPersistentStorage::GetNode( const RDB_ChunkPtr & pNodePtr ) {
    Assert( m_bValidFile );
    Assert( pNodePtr.iType == RDB_CHUNK_NODE );
    UInt iSize;
    RDB_FilePtr iStorage = _File_GetPtr_ChunkContent( pNodePtr, &iSize );
    Assert( iSize == sizeof(RDB_Chunk_BTreeNode) );
    return (RDB_Chunk_BTreeNode*)( _Cache_Query( iStorage, iSize, true ) );
}
inline RDB_Chunk_BTreeLeaf * RDBPersistentStorage::GetLeaf( const RDB_ChunkPtr & pLeafPtr ) {
    Assert( m_bValidFile );
    Assert( pLeafPtr.iType == RDB_CHUNK_LEAF );
    UInt iSize;
    RDB_FilePtr iStorage = _File_GetPtr_ChunkContent( pLeafPtr, &iSize );
    Assert( iSize == sizeof(RDB_Chunk_BTreeLeaf) );
    return (RDB_Chunk_BTreeLeaf*)( _Cache_Query( iStorage, iSize, true ) );
}
inline RDB_Chunk_Data * RDBPersistentStorage::GetData( const RDB_ChunkPtr & pDataPtr ) {
    Assert( m_bValidFile );
    Assert( pDataPtr.iType == RDB_CHUNK_DATA );
    UInt iSize;
    RDB_FilePtr iStorage = _File_GetPtr_ChunkContent( pDataPtr, &iSize );
    Assert( iSize == sizeof(RDB_Chunk_Data) );
    return (RDB_Chunk_Data*)( _Cache_Query( iStorage, iSize, true ) );
}

inline const RDB_Chunk_BTreeNode * RDBPersistentStorage::GetConstNode( const RDB_ChunkPtr & pNodePtr ) {
    Assert( m_bValidFile );
    Assert( pNodePtr.iType == RDB_CHUNK_NODE );
    UInt iSize;
    RDB_FilePtr iStorage = _File_GetPtr_ChunkContent( pNodePtr, &iSize );
    Assert( iSize == sizeof(RDB_Chunk_BTreeNode) );
    return (const RDB_Chunk_BTreeNode*)( _Cache_Query( iStorage, iSize, false ) );
}
inline const RDB_Chunk_BTreeLeaf * RDBPersistentStorage::GetConstLeaf( const RDB_ChunkPtr & pLeafPtr ) {
    Assert( m_bValidFile );
    Assert( pLeafPtr.iType == RDB_CHUNK_LEAF );
    UInt iSize;
    RDB_FilePtr iStorage = _File_GetPtr_ChunkContent( pLeafPtr, &iSize );
    Assert( iSize == sizeof(RDB_Chunk_BTreeLeaf) );
    return (const RDB_Chunk_BTreeLeaf*)( _Cache_Query( iStorage, iSize, false ) );
}
inline const RDB_Chunk_Data * RDBPersistentStorage::GetConstData( const RDB_ChunkPtr & pDataPtr ) {
    Assert( m_bValidFile );
    Assert( pDataPtr.iType == RDB_CHUNK_DATA );
    UInt iSize;
    RDB_FilePtr iStorage = _File_GetPtr_ChunkContent( pDataPtr, &iSize );
    Assert( iSize == sizeof(RDB_Chunk_Data) );
    return (const RDB_Chunk_Data*)( _Cache_Query( iStorage, iSize, false ) );
}

inline const RDB_ChunkPtr & RDBPersistentStorage::GetTableChunk( RDB_ChunkTable iChunkTableSlot ) const {
    Assert( m_bValidFile );
    Assert( iChunkTableSlot < RDB_CHUNKTABLE_COUNT );
    return m_dbMasterHeader.dbStateData.dbChunkTable[iChunkTableSlot];
}

inline RDB_DirectoryID RDBPersistentStorage::GetNextDirectoryID() const {
    Assert( m_bValidFile );
    return m_dbMasterHeader.dbStateData.dbNextDirectoryID;
}
inline Void RDBPersistentStorage::SetNextDirectoryID( RDB_DirectoryID iNextDirectoryID ) {
    Assert( m_bValidFile );
    m_bSyncStateData = false;
    m_dbMasterHeader.dbStateData.dbNextDirectoryID = iNextDirectoryID;
}

/////////////////////////////////////////////////////////////////////////////////

inline RDB_FilePtr RDBPersistentStorage::_File_GetPtr_HeaderState() {
    return (RDB_FilePtr)( RDB_HEADER_STATES_OFFSET );
}
inline RDB_FilePtr RDBPersistentStorage::_File_GetPtr_Journal() {
    return (RDB_FilePtr)( RDB_HEADER_JOURNAL_OFFSET );
}
