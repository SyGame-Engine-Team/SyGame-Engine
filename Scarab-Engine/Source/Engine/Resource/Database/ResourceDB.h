/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Resource/Database/ResourceDB.h
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
// Header prelude
#ifndef SCARAB_ENGINE_RESOURCE_DATABASE_RESOURCEDB_H
#define SCARAB_ENGINE_RESOURCE_DATABASE_RESOURCEDB_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/String/PatternMatching.h"

#include "RDBPersistentStorage.h"
#include "RDBTreeOperator.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
    // Paths & Names
#define RDB_NAME_LENGTH RDB_BTREE_NAME_LENGTH
#define RDB_PATH_LENGTH 1024

#define RDB_PATH_SEPARATOR TEXT('/')

#define RDB_ROOT_ID     0
#define RDB_ROOT_PATH   TEXT("/")
#define RDB_PARENT_PATH TEXT("..")
#define RDB_SELF_PATH   TEXT(".")

// <target> = <dir> | <res>
// <rdir> = ".." | <dir>
// <rtarget> = ".." | <target>
// Absolute Path = "/<dir0>/<dir1>/ ... /<dirN>/<target>"
// Relative Path = "<rdir0>/<rdir1>/ .. /<rdirN>/<rtarget>"

    // Info structures
typedef struct _rdb_directory_infos
{
    RDB_DirectoryID iParentID;
    GChar strName[RDB_NAME_LENGTH];

    RDB_DirectoryID iID;
    UInt iFlags;
} RDB_DirectoryInfos;

typedef struct _rdb_resource_infos
{
    RDB_DirectoryID iParentID;
    GChar strName[RDB_NAME_LENGTH];

    ResourceType iType;
    UInt iFlags;

    UInt iDataSize;
} RDB_ResourceInfos;

    // Resource handle
#define RDB_MAX_OPEN_HANDLES 128

typedef struct _rdb_resource_handle
{
    RDB_BTreeKeyData keyData;

    ResourceType iType;
    RDB_ResourceDescriptor resDescriptor;

    Bool bAllowWrite;

    RDB_ChunkPtr pChunk;
    const RDB_Chunk_Data * pContent;
    UInt iChunkIndex;
    UInt iContentIndex;
} RDB_ResourceHandle;

/////////////////////////////////////////////////////////////////////////////////
// The ResourceDB class
class ResourceDB
{
public:
    ResourceDB();
    ~ResourceDB();

    // Name access
    inline const GChar * GetName() const;
    inline const GChar * GetFileName() const;

    // Construction
    Bool Create( const GChar * strFilename );
    Bool Open( const GChar * strFilename );
    Void Close();

    // Current Working Directory interface
    inline UInt GetAbsolutePath( GChar * outAbsolutePath, const GChar * strRelativePath ) const;

    inline UInt GetCWD( GChar * outCWDPath ) const;
    inline Bool ChangeCWD( const GChar * strRelativePath );
    Void ListCWD( Array<RDB_DirectoryInfos> * outSubDirectories,
                  Array<RDB_ResourceInfos> * outResources );

    // DB operations
    Bool GetDirectoryInfo( RDB_DirectoryInfos * outInfos, const GChar * strDirPath );
    Bool GetResourceInfo( RDB_ResourceInfos * outInfos, const GChar * strResPath );
    Void GetResourceInfo( RDB_ResourceInfos * outInfos, RDB_ResourceHandle * hResource );

    Bool CreateDirectory( const GChar * strDirPath, const GChar * strNewName );
    Bool CreateResource( const GChar * strDirPath, const GChar * strNewName, ResourceType iType );

    Bool DestroyDirectory( const GChar * strDirPath );
    Bool DestroyResource( const GChar * strResPath );

    Bool DirectoryExists( const GChar * strDirPath );
    Bool ResourceExists( const GChar * strResPath );

    Bool RenameDirectory( const GChar * strDirPath, const GChar * strNewName );
    Bool RenameResource( const GChar * strResPath, const GChar * strNewName );

    Bool MoveDirectory( const GChar * strDirPath, const GChar * strNewPath );
    Bool MoveResource( const GChar * strResPath, const GChar * strNewPath );

    Bool ListDirectory( const GChar * strDirPath, Array<RDB_DirectoryInfos> * outSubDirectories,
                                                  Array<RDB_ResourceInfos> * outResources );

    // Resource operations
    Bool ResourceOpen( RDB_ResourceHandle * hResource, const GChar * strResPath, Bool bReadOnly );
    Void ResourceClose( RDB_ResourceHandle * hResource );

    Void ResourceSeekBegin( RDB_ResourceHandle * hResource );
    Void ResourceSeekEnd( RDB_ResourceHandle * hResource );
    Void ResourceSeekAbs( RDB_ResourceHandle * hResource, UInt iOffset );
    Void ResourceSeekRel( RDB_ResourceHandle * hResource, Int iOffset );
    inline UInt ResourceTell( RDB_ResourceHandle * hResource );

    Void ResourceRead( RDB_ResourceHandle * hResource, UInt iSize, Byte * outData );
    Void ResourceWrite( RDB_ResourceHandle * hResource, UInt iSize, const Byte * pData );

    Void ResourceAppend( RDB_ResourceHandle * hResource, UInt iSize, const Byte * pData );
    Void ResourceTruncate( RDB_ResourceHandle * hResource, UInt iSize );
    Void ResourceErase( RDB_ResourceHandle * hResource );

    inline UInt64 ResourceGetSize( RDB_ResourceHandle * hResource ) const;
    inline ResourceType ResourceGetType( RDB_ResourceHandle * hResource ) const;

private:
    GChar m_strName[FILENAME_LENGTH];

    RDB_BTreeItem RDB_ROOT_DIR;

    // Recursive destruction
    Void _Destroy_Directory( const RDB_BTreeItem & itDirectory );
    Void _Destroy_Resource( const RDB_BTreeItem & itResource );

    // Path system
    static Bool _Path_IsValidChar( GChar ch );
    static Bool _Path_IsValidName( const GChar * strName );

    Bool _Path_DirectorySearch( const GChar * strPath, RDB_BTreeItem * outDirectory );
    Bool _Path_ResourceSearch( const GChar * strPath, RDB_BTreeItem * outResource );

    // CWD system
    UInt _CWD_GetPathString( GChar * outPath ) const;
    UInt _CWD_ResolvePath( GChar * outAbsolutePath, const GChar * strRelativePath ) const;

    Bool _CWD_WalkUp();
    Bool _CWD_WalkDown( const GChar * strName );
    Bool _CWD_WalkThrough( const GChar * strRelativePath );

    RDB_BTreeItem m_itCWD;
    Array<RDB_BTreeKeyData> m_stkParentKeys;

    // RDB instances
    RDBPersistentStorage m_dbStorage;
    RDBTreeOperator m_dbTreeOp;
    Bool m_bValidDB;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ResourceDB.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RESOURCE_DATABASE_RESOURCEDB_H

