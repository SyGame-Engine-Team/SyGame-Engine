/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Resource/Database/ResourceDB.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Resource Database, final interface.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// ResourceDB implementation
inline const GChar * ResourceDB::GetName() const {
    Assert( m_bValidDB );
    return m_strName;
}
inline const GChar * ResourceDB::GetFileName() const {
    Assert( m_bValidDB );
    return m_dbStorage.GetFileName();
}

inline UInt ResourceDB::GetAbsolutePath( GChar * outAbsolutePath, const GChar * strRelativePath ) const {
    Assert( m_bValidDB );
    return _CWD_ResolvePath( outAbsolutePath, strRelativePath );
}

inline UInt ResourceDB::GetCWD( GChar * outCWDPath ) const {
    Assert( m_bValidDB );
    return _CWD_GetPathString( outCWDPath );
}
inline Bool ResourceDB::ChangeCWD( const GChar * strRelativePath ) {
    Assert( m_bValidDB );
    return _CWD_WalkThrough( strRelativePath );
}

inline UInt ResourceDB::ResourceTell( RDB_ResourceHandle * hResource ) {
    return ( hResource->iChunkIndex * RDB_CHUNK_DATA_SIZE ) + hResource->iContentIndex;
}

inline UInt64 ResourceDB::ResourceGetSize( RDB_ResourceHandle * hResource ) const {
    return hResource->resDescriptor.iDataSize;
}
inline ResourceType ResourceDB::ResourceGetType( RDB_ResourceHandle * hResource ) const {
    return hResource->iType;
}

