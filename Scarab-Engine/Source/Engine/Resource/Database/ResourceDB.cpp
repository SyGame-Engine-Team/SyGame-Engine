/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Resource/Database/ResourceDB.cpp
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
// Includes
#include "ResourceDB.h"

#include "../ResourceManager.h"

/////////////////////////////////////////////////////////////////////////////////
// ResourceDB implementation
ResourceDB::ResourceDB():
    m_dbStorage(), m_dbTreeOp( &m_dbStorage ), m_stkParentKeys()
{
    // RDB name
    m_strName[0] = NULLBYTE;

    // Root directory
    RDB_ROOT_DIR.keyData.idParent = INVALID_OFFSET;
    StringFn->NCopyA( RDB_ROOT_DIR.keyData.strName, "_ROOT_", RDB_NAME_LENGTH - 1 );
    RDB_ROOT_DIR.iResourceType = RESOURCE_TYPE_DIRECTORY;
    RDB_ROOT_DIR.Content.descDirectory.idDirectory = 0;
    RDB_ROOT_DIR.Content.descDirectory.iFlags = 0;

    // CWD parent chain
    m_stkParentKeys.UseMemoryContext( ResourceFn->GetMemoryContext(), TEXT("Scratch") );

    m_bValidDB = false;
}
ResourceDB::~ResourceDB()
{
    Assert( !m_bValidDB );

    // nothing to do
    // Call Close explicitly
}

Bool ResourceDB::Create( const GChar * strFilename )
{
    Assert( !m_bValidDB );

    // Create full name
    GChar strFullName[RDB_PATH_LENGTH];
    GChar * strCur = StringFn->NCopy( strFullName, RDB_PATH_BASE, RDB_PATH_LENGTH - 1 );
    *strCur++ = TEXT('/');
    strCur = StringFn->NCopy( strCur, strFilename, RDB_PATH_LENGTH - 1 - ( strCur - strFullName ) );
    strCur = StringFn->NCopy( strCur, TEXT(".rdb"), RDB_PATH_LENGTH - 1 - ( strCur - strFullName ) );

    // Create storage
    Bool bCreated = m_dbStorage.Create( strFullName );
    if ( !bCreated )
        return false;

    // Save name
    StringFn->NCopy( m_strName, strFilename, FILENAME_LENGTH - 1 );

    // Allocate root leaves
    RDB_ChunkPtr pLeafStorage;
    RDB_Chunk_BTreeLeaf * pLeafRoot = NULL;
    for( UInt i = 0; i < RDB_CHUNKTABLE_COUNT; ++i ) {
        pLeafRoot = m_dbStorage.AllocLeaf( &pLeafStorage );
        pLeafRoot->pPrevBrother.idBlock = (RDB_BlockID)INVALID_OFFSET;
        pLeafRoot->pPrevBrother.idChunk = (RDB_ChunkID)INVALID_OFFSET;
        pLeafRoot->pPrevBrother.iType = RDB_CHUNK_LEAF;
        pLeafRoot->pNextBrother.idBlock = (RDB_BlockID)INVALID_OFFSET;
        pLeafRoot->pNextBrother.idChunk = (RDB_ChunkID)INVALID_OFFSET;
        pLeafRoot->pNextBrother.iType = RDB_CHUNK_LEAF;
        pLeafRoot->iItemCount = 0;
        m_dbStorage.SetTableChunk( (RDB_ChunkTable)i, pLeafStorage );
    }
    m_dbStorage.Commit();

    // Setup CWD
    m_itCWD = RDB_ROOT_DIR;
    m_stkParentKeys.Create();

    // Done
    m_bValidDB = true;
    return true;
}
Bool ResourceDB::Open( const GChar * strFilename )
{
    Assert( !m_bValidDB );

    // Create full name
    GChar strFullName[RDB_PATH_LENGTH];
    GChar * strCur = StringFn->NCopy( strFullName, RDB_PATH_BASE, RDB_PATH_LENGTH - 1 );
    *strCur++ = TEXT('/');
    strCur = StringFn->NCopy( strCur, strFilename, RDB_PATH_LENGTH - 1 - ( strCur - strFullName ) );
    strCur = StringFn->NCopy( strCur, TEXT(".rdb"), RDB_PATH_LENGTH - 1 - ( strCur - strFullName ) );

    // Open storage
    Bool bOpened = m_dbStorage.Open( strFullName );
    if ( !bOpened )
        return false;

    // Save name
    StringFn->NCopy( m_strName, strFilename, FILENAME_LENGTH - 1 );

    // Setup CWD
    m_itCWD = RDB_ROOT_DIR;
    m_stkParentKeys.Create();

    // Done
    m_bValidDB = true;
    return true;
}
Void ResourceDB::Close()
{
    Assert( m_bValidDB );

    // CWD parent chain
    m_stkParentKeys.Destroy();

    // Destroy name
    m_strName[0] = NULLBYTE;

    // Close storage
    m_dbStorage.Close();

    // Done
    m_bValidDB = false;
}

Void ResourceDB::ListCWD( Array<RDB_DirectoryInfos> * outSubDirectories,
                          Array<RDB_ResourceInfos> * outResources )
{
    Assert( m_bValidDB );

    // Create temp array
    Array<RDB_BTreeItem> tmpList;
    tmpList.UseMemoryContext( ResourceFn->GetMemoryContext(), TEXT("Scratch") );
    Bool bCreated = false;

    // Sub-Directories
    if ( outSubDirectories != NULL ) {
        if ( !bCreated ) {
            tmpList.Create();
            bCreated = true;
        }
        m_dbTreeOp.ListSubDirectories( m_itCWD.Content.descDirectory.idDirectory, &tmpList );

        RDB_DirectoryInfos curInfos;
        for( UInt i = 0; i < tmpList.Count(); ++i ) {
            curInfos.iParentID = tmpList[i].keyData.idParent;
#if defined(UNICODE) || defined(_UNICODE)
            StringFn->AsciiToWideChar( curInfos.strName, tmpList[i].keyData.strName, RDB_NAME_LENGTH - 1 );
#else
            StringFn->NCopyA( curInfos.strName, tmpList[i].keyData.strName, RDB_NAME_LENGTH - 1 );
#endif
            curInfos.iID = tmpList[i].Content.descDirectory.idDirectory;
            curInfos.iFlags = tmpList[i].Content.descDirectory.iFlags;
            outSubDirectories->Push( curInfos );
        }

        tmpList.Clear();
    }

    // Resources
    if ( outResources != NULL ) {
        if ( !bCreated ) {
            tmpList.Create();
            bCreated = true;
        }
        m_dbTreeOp.ListResources( m_itCWD.Content.descDirectory.idDirectory, &tmpList );

        RDB_ResourceInfos curInfos;
        for( UInt i = 0; i < tmpList.Count(); ++i ) {
            curInfos.iParentID = tmpList[i].keyData.idParent;
#if defined(UNICODE) || defined(_UNICODE)
            StringFn->AsciiToWideChar( curInfos.strName, tmpList[i].keyData.strName, RDB_NAME_LENGTH - 1 );
#else
            StringFn->NCopyA( curInfos.strName, tmpList[i].keyData.strName, RDB_NAME_LENGTH - 1 );
#endif
            curInfos.iType = tmpList[i].iResourceType;
            curInfos.iFlags = tmpList[i].Content.descResource.iFlags;
            curInfos.iDataSize = tmpList[i].Content.descResource.iDataSize;
            outResources->Push( curInfos );
        }

        tmpList.Clear();
    }

    // Destroy temp array
    if ( bCreated )
        tmpList.Destroy();
}

Bool ResourceDB::GetDirectoryInfo( RDB_DirectoryInfos * outInfos, const GChar * strDirPath )
{
    Assert( m_bValidDB );

    // Resolve directory path
    GChar strAbsPath[RDB_PATH_LENGTH];
    if ( strDirPath[0] == RDB_PATH_SEPARATOR )
        StringFn->NCopy( strAbsPath, strDirPath, RDB_PATH_LENGTH - 1 );
    else
        _CWD_ResolvePath( strAbsPath, strDirPath );

    // Search directory
    RDB_BTreeItem getinfoDir;
    Bool bOk = _Path_DirectorySearch( strAbsPath, &getinfoDir );
    if ( !bOk )
        return false; // wrong path

    outInfos->iParentID = getinfoDir.keyData.idParent;
#if defined(UNICODE) || defined(_UNICODE)
    StringFn->AsciiToWideChar( outInfos->strName, getinfoDir.keyData.strName, RDB_NAME_LENGTH - 1 );
#else
    StringFn->NCopyA( outInfos->strName, getinfoDir.keyData.strName, RDB_NAME_LENGTH - 1 );
#endif
    outInfos->iID = getinfoDir.Content.descDirectory.idDirectory;
    outInfos->iFlags = getinfoDir.Content.descDirectory.iFlags;

    return true;
}
Bool ResourceDB::GetResourceInfo( RDB_ResourceInfos * outInfos, const GChar * strResPath )
{
    Assert( m_bValidDB );

    // Resolve resource path
    GChar strAbsPath[RDB_PATH_LENGTH];
    if ( strResPath[0] == RDB_PATH_SEPARATOR )
        StringFn->NCopy( strAbsPath, strResPath, RDB_PATH_LENGTH - 1 );
    else
        _CWD_ResolvePath( strAbsPath, strResPath );

    // Search resource
    RDB_BTreeItem getinfoRes;
    Bool bOk = _Path_ResourceSearch( strAbsPath, &getinfoRes );
    if ( !bOk )
        return false; // wrong path

    outInfos->iType = getinfoRes.iResourceType;
    outInfos->iParentID = getinfoRes.keyData.idParent;
#if defined(UNICODE) || defined(_UNICODE)
    StringFn->AsciiToWideChar( outInfos->strName, getinfoRes.keyData.strName, RDB_NAME_LENGTH - 1 );
#else
    StringFn->NCopyA( outInfos->strName, getinfoRes.keyData.strName, RDB_NAME_LENGTH - 1 );
#endif
    outInfos->iFlags = getinfoRes.Content.descResource.iFlags;
    outInfos->iDataSize = getinfoRes.Content.descResource.iDataSize;

    return true;
}
Void ResourceDB::GetResourceInfo( RDB_ResourceInfos * outInfos, RDB_ResourceHandle * hResource )
{
    Assert( m_bValidDB );

    outInfos->iType = hResource->iType;
    outInfos->iParentID = hResource->keyData.idParent;
#if defined(UNICODE) || defined(_UNICODE)
    StringFn->AsciiToWideChar( outInfos->strName, hResource->keyData.strName, RDB_NAME_LENGTH - 1 );
#else
    StringFn->NCopyA( outInfos->strName, hResource->keyData.strName, RDB_NAME_LENGTH - 1 );
#endif
    outInfos->iFlags = hResource->resDescriptor.iFlags;
    outInfos->iDataSize = hResource->resDescriptor.iDataSize;
}

Bool ResourceDB::CreateDirectory( const GChar * strDirPath, const GChar * strNewName )
{
    Assert( m_bValidDB );

    // Check name validity
    Bool bOk = _Path_IsValidName( strNewName );
    if ( !bOk )
        return false; // invalid name

    // Resolve path for parent directory
    GChar strAbsPath[RDB_PATH_LENGTH];
    if ( strDirPath[0] == RDB_PATH_SEPARATOR )
        StringFn->NCopy( strAbsPath, strDirPath, RDB_PATH_LENGTH - 1 );
    else
        _CWD_ResolvePath( strAbsPath, strDirPath );

    // Search parent directory
    RDB_BTreeItem parentDir;
    bOk = _Path_DirectorySearch( strAbsPath, &parentDir );
    if ( !bOk )
        return false; // wrong path

    // Get next directory ID
    RDB_DirectoryID idNewDir = m_dbStorage.GetNextDirectoryID();

    // Build new directory
    RDB_BTreeItem newDir;
    newDir.keyData.idParent = parentDir.Content.descDirectory.idDirectory;
#if defined(UNICODE) || defined(_UNICODE)
    StringFn->WideCharToMultiByte( newDir.keyData.strName, strNewName, RDB_NAME_LENGTH - 1 );
#else
    StringFn->NCopyA( newDir.keyData.strName, strNewName, RDB_NAME_LENGTH - 1 );
#endif
    newDir.iResourceType = RESOURCE_TYPE_DIRECTORY;
    newDir.Content.descDirectory.idDirectory = idNewDir;
    newDir.Content.descDirectory.iFlags = 0;

    // Insert
    bOk = m_dbTreeOp.InsertDirectory( newDir );
    if ( !bOk )
        return false; // already exists

    // Update next directory ID
    ++idNewDir;
    if ( idNewDir == INVALID_OFFSET ) {
        idNewDir = 1; // Very-Most-Likely this is safe ...
                      // but might not and will not, that's the rule !
                      // Anyway 4 billions directory in a single file
                      // is almost impossible to reach ...
        // Safe way, reallocate all IDs from 0 :
        // Walk through all directories in-order and update,
        // use dynamic algorithm, then update resources parents.
        // Beware to keep new IDs allocated in the same order ...
        //idNewDir = _ReallocateDirectoryIDs();
        Assert( false );
        m_dbStorage.Discard();
        return false; // should not happen
    }
    m_dbStorage.SetNextDirectoryID( idNewDir );

    // Done
    m_dbStorage.Commit();
    return true;
}
Bool ResourceDB::CreateResource( const GChar * strDirPath, const GChar * strNewName, ResourceType iType )
{
    Assert( m_bValidDB );
    Assert( iType < RESOURCE_TYPE_COUNT );

    // Check name validity
    Bool bOk = _Path_IsValidName( strNewName );
    if ( !bOk )
        return false;

    // Resolve path for parent directory
    GChar strAbsPath[RDB_PATH_LENGTH];
    if ( strDirPath[0] == RDB_PATH_SEPARATOR )
        StringFn->NCopy( strAbsPath, strDirPath, RDB_PATH_LENGTH - 1 );
    else
        _CWD_ResolvePath( strAbsPath, strDirPath );

    // Search parent directory
    RDB_BTreeItem parentDir;
    bOk = _Path_DirectorySearch( strAbsPath, &parentDir );
    if ( !bOk )
        return false; // wrong path

    // Allocate an empty data block
    RDB_ChunkPtr pDataStorage;
    RDB_Chunk_Data * pData = m_dbStorage.AllocData( &pDataStorage );
    pData->pPrevChunk.idBlock = (RDB_BlockID)INVALID_OFFSET;
    pData->pPrevChunk.idChunk = (RDB_ChunkID)INVALID_OFFSET;
    pData->pPrevChunk.iType = RDB_CHUNK_DATA;
    pData->pNextChunk.idBlock = (RDB_BlockID)INVALID_OFFSET;
    pData->pNextChunk.idChunk = (RDB_ChunkID)INVALID_OFFSET;
    pData->pNextChunk.iType = RDB_CHUNK_DATA;
    pData->iContentSize = 0;
    MemZero( pData->arrContent, RDB_CHUNK_DATA_SIZE );

    // Build new resource
    RDB_BTreeItem newRes;
    newRes.keyData.idParent = parentDir.Content.descDirectory.idDirectory;
#if defined(UNICODE) || defined(_UNICODE)
    StringFn->WideCharToMultiByte( newRes.keyData.strName, strNewName, RDB_NAME_LENGTH - 1 );
#else
    StringFn->NCopyA( newRes.keyData.strName, strNewName, RDB_NAME_LENGTH - 1 );
#endif
    newRes.iResourceType = iType;
    newRes.Content.descResource.iDataSize = 0;
    newRes.Content.descResource.iFlags = 0;
    newRes.Content.descResource.pFirstChunk = pDataStorage;
    newRes.Content.descResource.pLastChunk = pDataStorage;
    newRes.Content.descResource.iChunkCount = 1;

    // Insert
    bOk = m_dbTreeOp.InsertResource( newRes );
    if ( !bOk ) {
        m_dbStorage.Discard();
        return false; // Already exists
    }

    // Done
    m_dbStorage.Commit();
    return true;
}

Bool ResourceDB::DestroyDirectory( const GChar * strDirPath )
{
    Assert( m_bValidDB );

    // Resolve path
    GChar strAbsPath[RDB_PATH_LENGTH];
    if ( strDirPath[0] == RDB_PATH_SEPARATOR )
        StringFn->NCopy( strAbsPath, strDirPath, RDB_PATH_LENGTH - 1 );
    else
        _CWD_ResolvePath( strAbsPath, strDirPath );

    // Search
    RDB_BTreeItem foundDir;
    Bool bOk = _Path_DirectorySearch( strAbsPath, &foundDir );
    if ( !bOk )
        return false; // wrong path

    // Destroy
    _Destroy_Directory( foundDir );

    // Done
    m_dbStorage.Commit();
    return true;
}
Bool ResourceDB::DestroyResource( const GChar * strResPath )
{
    Assert( m_bValidDB );

    // Resolve path
    GChar strAbsPath[RDB_PATH_LENGTH];
    if ( strResPath[0] == RDB_PATH_SEPARATOR )
        StringFn->NCopy( strAbsPath, strResPath, RDB_PATH_LENGTH - 1 );
    else
        _CWD_ResolvePath( strAbsPath, strResPath );

    // Search
    RDB_BTreeItem foundRes;
    Bool bOk = _Path_ResourceSearch( strAbsPath, &foundRes );
    if ( !bOk )
        return false; // wrong path

    // Destroy
    _Destroy_Resource( foundRes );

    // Done
    m_dbStorage.Commit();
    return true;
}

Bool ResourceDB::DirectoryExists( const GChar * strDirPath )
{
    Assert( m_bValidDB );

    // Resolve directory path
    GChar strAbsPath[RDB_PATH_LENGTH];
    if ( strDirPath[0] == RDB_PATH_SEPARATOR )
        StringFn->NCopy( strAbsPath, strDirPath, RDB_PATH_LENGTH - 1 );
    else
        _CWD_ResolvePath( strAbsPath, strDirPath );

    // Test existence
    RDB_BTreeItem tmpDir;
    return _Path_DirectorySearch( strAbsPath, &tmpDir );
}
Bool ResourceDB::ResourceExists( const GChar * strResPath )
{
    Assert( m_bValidDB );

    // Resolve resource path
    GChar strAbsPath[RDB_PATH_LENGTH];
    if ( strResPath[0] == RDB_PATH_SEPARATOR )
        StringFn->NCopy( strAbsPath, strResPath, RDB_PATH_LENGTH - 1 );
    else
        _CWD_ResolvePath( strAbsPath, strResPath );

    // Test existence
    RDB_BTreeItem tmpRes;
    return _Path_ResourceSearch( strAbsPath, &tmpRes );
}

Bool ResourceDB::RenameDirectory( const GChar * strDirPath, const GChar * strNewName )
{
    Assert( m_bValidDB );

    // Check name validity
    Bool bOk = _Path_IsValidName( strNewName );
    if ( !bOk )
        return false; // invalid name

    // Resolve directory path
    GChar strAbsPath[RDB_PATH_LENGTH];
    if ( strDirPath[0] == RDB_PATH_SEPARATOR )
        StringFn->NCopy( strAbsPath, strDirPath, RDB_PATH_LENGTH - 1 );
    else
        _CWD_ResolvePath( strAbsPath, strDirPath );

    // Search directory
    RDB_BTreeItem oldDir;
    bOk = _Path_DirectorySearch( strAbsPath, &oldDir );
    if ( !bOk )
        return false; // wrong path

    // Build renamed directory
    RDB_BTreeItem newDir;
    newDir.keyData.idParent = oldDir.keyData.idParent;
#if defined(UNICODE) || defined(_UNICODE)
    StringFn->WideCharToMultiByte( newDir.keyData.strName, strNewName, RDB_NAME_LENGTH - 1 );
#else
    StringFn->NCopyA( newDir.keyData.strName, strNewName, RDB_NAME_LENGTH - 1 );
#endif
    newDir.iResourceType = RESOURCE_TYPE_DIRECTORY;
    newDir.Content.descDirectory = oldDir.Content.descDirectory;

    // Insert new
    bOk = m_dbTreeOp.InsertDirectory( newDir );
    if ( !bOk )
        return false; // new name already exists

    // Remove old
    bOk = m_dbTreeOp.RemoveDirectory( oldDir.keyData );
    Assert( bOk );

    // Done
    m_dbStorage.Commit();
    return true;
}
Bool ResourceDB::RenameResource( const GChar * strResPath, const GChar * strNewName )
{
    Assert( m_bValidDB );

    // Check name validity
    Bool bOk = _Path_IsValidName( strNewName );
    if ( !bOk )
        return false; // invalid name

    // Resolve resource path
    GChar strAbsPath[RDB_PATH_LENGTH];
    if ( strResPath[0] == RDB_PATH_SEPARATOR )
        StringFn->NCopy( strAbsPath, strResPath, RDB_PATH_LENGTH - 1 );
    else
        _CWD_ResolvePath( strAbsPath, strResPath );

    // Search resource
    RDB_BTreeItem oldRes;
    bOk = _Path_ResourceSearch( strAbsPath, &oldRes );
    if ( !bOk )
        return false; // wrong path

    // Build renamed resource
    RDB_BTreeItem newRes;
    newRes.keyData.idParent = oldRes.keyData.idParent;
#if defined(UNICODE) || defined(_UNICODE)
    StringFn->WideCharToMultiByte( newRes.keyData.strName, strNewName, RDB_NAME_LENGTH - 1 );
#else
    StringFn->NCopyA( newRes.keyData.strName, strNewName, RDB_NAME_LENGTH - 1 );
#endif
    newRes.iResourceType = oldRes.iResourceType;
    newRes.Content.descResource = oldRes.Content.descResource;

    // Insert new
    bOk = m_dbTreeOp.InsertResource( newRes );
    if ( !bOk )
        return false; // new name already exists

    // Remove old
    bOk = m_dbTreeOp.RemoveResource( oldRes.keyData );
    Assert( bOk );

    // Done
    m_dbStorage.Commit();
    return true;
}

Bool ResourceDB::MoveDirectory( const GChar * strDirPath, const GChar * strNewPath )
{
    Assert( m_bValidDB );

    // Resolve directory path
    UInt iAbsDirPathLength;
    GChar strAbsDirPath[RDB_PATH_LENGTH];
    if ( strDirPath[0] == RDB_PATH_SEPARATOR ) {
        GChar * strTmp = StringFn->NCopy( strAbsDirPath, strDirPath, RDB_PATH_LENGTH - 1 );
        iAbsDirPathLength = ( strTmp - strAbsDirPath );
    } else
        iAbsDirPathLength = _CWD_ResolvePath( strAbsDirPath, strDirPath );

    // Resolve new parent path
    UInt iAbsNewPathLength;
    GChar strAbsNewPath[RDB_PATH_LENGTH];
    if ( strNewPath[0] == RDB_PATH_SEPARATOR ) {
        GChar * strTmp = StringFn->NCopy( strAbsNewPath, strNewPath, RDB_PATH_LENGTH - 1 );
        iAbsNewPathLength = ( strTmp - strAbsNewPath );
    } else
        iAbsNewPathLength = _CWD_ResolvePath( strAbsNewPath, strNewPath );

    // Avoid circular linking problem, new path cannot be inside moved dir !
    UInt iIndex = MatchFn->Raita( strAbsNewPath, iAbsNewPathLength, strAbsDirPath, iAbsDirPathLength );
    if ( iIndex != INVALID_OFFSET )
        return false; // circular linkage

    // Search directory
    RDB_BTreeItem oldDir;
    Bool bOk = _Path_DirectorySearch( strAbsDirPath, &oldDir );
    if ( !bOk )
        return false; // wrong path

    // Search new parent
    RDB_BTreeItem parentDir;
    bOk = _Path_DirectorySearch( strAbsNewPath, &parentDir );
    if ( !bOk )
        return false; // wrong path

    // Build moved directory
    RDB_BTreeItem newDir;
    newDir.keyData.idParent = parentDir.Content.descDirectory.idDirectory;
    StringFn->NCopyA( newDir.keyData.strName, oldDir.keyData.strName, RDB_NAME_LENGTH - 1 );
    newDir.iResourceType = RESOURCE_TYPE_DIRECTORY;
    newDir.Content.descDirectory = oldDir.Content.descDirectory;

    // Insert new
    bOk = m_dbTreeOp.InsertDirectory( newDir );
    if ( !bOk )
        return false; // new name already exists

    // Remove old
    bOk = m_dbTreeOp.RemoveDirectory( oldDir.keyData );
    Assert( bOk );

    // Done
    m_dbStorage.Commit();
    return true;
}
Bool ResourceDB::MoveResource( const GChar * strResPath, const GChar * strNewPath )
{
    Assert( m_bValidDB );

    // Resolve resource path
    GChar strAbsPath[RDB_PATH_LENGTH];
    if ( strResPath[0] == RDB_PATH_SEPARATOR )
        StringFn->NCopy( strAbsPath, strResPath, RDB_PATH_LENGTH - 1 );
    else
        _CWD_ResolvePath( strAbsPath, strResPath );

    // Search resource
    RDB_BTreeItem oldRes;
    Bool bOk = _Path_ResourceSearch( strAbsPath, &oldRes );
    if ( !bOk )
        return false; // wrong path

    // Resolve new parent path
    if ( strNewPath[0] == RDB_PATH_SEPARATOR )
        StringFn->NCopy( strAbsPath, strNewPath, RDB_PATH_LENGTH - 1 );
    else
        _CWD_ResolvePath( strAbsPath, strNewPath );

    // Search new parent
    RDB_BTreeItem parentDir;
    bOk = _Path_DirectorySearch( strAbsPath, &parentDir );
    if ( !bOk )
        return false; // wrong path

    // Build moved resource
    RDB_BTreeItem newRes;
    newRes.keyData.idParent = parentDir.Content.descDirectory.idDirectory;
    StringFn->NCopyA( newRes.keyData.strName, oldRes.keyData.strName, RDB_NAME_LENGTH - 1 );
    newRes.iResourceType = oldRes.iResourceType;
    newRes.Content.descResource = oldRes.Content.descResource;

    // Insert new
    bOk = m_dbTreeOp.InsertResource( newRes );
    if ( !bOk )
        return false; // new name already exists

    // Remove old
    bOk = m_dbTreeOp.RemoveResource( oldRes.keyData );
    Assert( bOk );

    // Done
    m_dbStorage.Commit();
    return true;
}

Bool ResourceDB::ListDirectory( const GChar * strDirPath, Array<RDB_DirectoryInfos> * outSubDirectories,
                                                          Array<RDB_ResourceInfos> * outResources )
{
    Assert( m_bValidDB );

    // Resolve directory path
    GChar strAbsPath[RDB_PATH_LENGTH];
    if ( strDirPath[0] == RDB_PATH_SEPARATOR )
        StringFn->NCopy( strAbsPath, strDirPath, RDB_PATH_LENGTH - 1 );
    else
        _CWD_ResolvePath( strAbsPath, strDirPath );

    // Search directory
    RDB_BTreeItem listDir;
    Bool bOk = _Path_DirectorySearch( strAbsPath, &listDir );
    if ( !bOk )
        return false; // wrong path

    // Create temp array
    Array<RDB_BTreeItem> tmpList;
    tmpList.UseMemoryContext( ResourceFn->GetMemoryContext(), TEXT("Scratch") );
    Bool bCreated = false;

    // Sub-Directories
    if ( outSubDirectories != NULL ) {
        if ( !bCreated ) {
            tmpList.Create();
            bCreated = true;
        }
        m_dbTreeOp.ListSubDirectories( listDir.Content.descDirectory.idDirectory, &tmpList );

        RDB_DirectoryInfos curInfos;
        for( UInt i = 0; i < tmpList.Count(); ++i ) {
            curInfos.iParentID = tmpList[i].keyData.idParent;

#if defined(UNICODE) || defined(_UNICODE)
            StringFn->AsciiToWideChar( curInfos.strName, tmpList[i].keyData.strName, RDB_NAME_LENGTH - 1 );
#else
            StringFn->NCopyA( curInfos.strName, tmpList[i].keyData.strName, RDB_NAME_LENGTH - 1 );
#endif
            curInfos.iID = tmpList[i].Content.descDirectory.idDirectory;
            curInfos.iFlags = tmpList[i].Content.descDirectory.iFlags;
            outSubDirectories->Push( curInfos );
        }

        tmpList.Clear();
    }

    // Resources
    if ( outResources != NULL ) {
        if ( !bCreated ) {
            tmpList.Create();
            bCreated = true;
        }
        m_dbTreeOp.ListResources( listDir.Content.descDirectory.idDirectory, &tmpList );

        RDB_ResourceInfos curInfos;
        for( UInt i = 0; i < tmpList.Count(); ++i ) {
            curInfos.iParentID = tmpList[i].keyData.idParent;
#if defined(UNICODE) || defined(_UNICODE)
            StringFn->AsciiToWideChar( curInfos.strName, tmpList[i].keyData.strName, RDB_NAME_LENGTH - 1 );
#else
            StringFn->NCopyA( curInfos.strName, tmpList[i].keyData.strName, RDB_NAME_LENGTH - 1 );
#endif
            curInfos.iType = tmpList[i].iResourceType;
            curInfos.iFlags = tmpList[i].Content.descResource.iFlags;
            curInfos.iDataSize = tmpList[i].Content.descResource.iDataSize;
            outResources->Push( curInfos );
        }

        tmpList.Clear();
    }

    // Destroy temp array
    if ( bCreated )
        tmpList.Destroy();

    // Done
    return true;
}

/////////////////////////////////////////////////////////////////////////////////

Bool ResourceDB::ResourceOpen( RDB_ResourceHandle * hResource, const GChar * strResPath, Bool bReadOnly )
{
    Assert( m_bValidDB );
    Assert( hResource != NULL );

    // Resolve resource path
    GChar strAbsPath[RDB_PATH_LENGTH];
    if ( strResPath[0] == RDB_PATH_SEPARATOR )
        StringFn->NCopy( strAbsPath, strResPath, RDB_PATH_LENGTH - 1 );
    else
        _CWD_ResolvePath( strAbsPath, strResPath );

    // Search resource
    RDB_BTreeItem openRes;
    Bool bOk = _Path_ResourceSearch( strAbsPath, &openRes );
    if ( !bOk )
        return false; // wrong path

    // Build handle
    hResource->keyData = openRes.keyData;
    hResource->iType = openRes.iResourceType;
    hResource->resDescriptor = openRes.Content.descResource;
    hResource->bAllowWrite = !bReadOnly;
    hResource->pChunk = openRes.Content.descResource.pFirstChunk;
    hResource->pContent = m_dbStorage.GetConstData( hResource->pChunk );
    hResource->iChunkIndex = 0;
    hResource->iContentIndex = 0;

    // Done
    return true;
}
Void ResourceDB::ResourceClose( RDB_ResourceHandle * hResource )
{
    Assert( m_bValidDB );
    Assert( hResource != NULL );

    // nothing to do
    // Release cache ?

    // Destroy handle
    MemZero( hResource, sizeof(RDB_ResourceHandle) );
}

Void ResourceDB::ResourceSeekBegin( RDB_ResourceHandle * hResource )
{
    Assert( m_bValidDB );
    Assert( hResource != NULL );

    if ( hResource->pChunk != hResource->resDescriptor.pFirstChunk ) {
        hResource->pChunk = hResource->resDescriptor.pFirstChunk;
        hResource->pContent = m_dbStorage.GetConstData( hResource->pChunk );
    }
    hResource->iChunkIndex = 0;
    hResource->iContentIndex = 0;
}
Void ResourceDB::ResourceSeekEnd( RDB_ResourceHandle * hResource )
{
    Assert( m_bValidDB );
    Assert( hResource != NULL );

    if ( hResource->pChunk != hResource->resDescriptor.pLastChunk ) {
        hResource->pChunk = hResource->resDescriptor.pLastChunk;
        hResource->pContent = m_dbStorage.GetConstData( hResource->pChunk );
    }
    hResource->iChunkIndex = (hResource->resDescriptor.iChunkCount - 1);
    hResource->iContentIndex = (hResource->pContent->iContentSize - 1);
}
Void ResourceDB::ResourceSeekAbs( RDB_ResourceHandle * hResource, UInt iOffset )
{
    Assert( m_bValidDB );
    Assert( hResource != NULL );
    Assert( hResource->iChunkIndex != INVALID_OFFSET );
    Assert( hResource->iContentIndex != INVALID_OFFSET );
    Assert( iOffset < hResource->resDescriptor.iDataSize );

    UInt iTargetChunkIndex = ( iOffset / RDB_CHUNK_DATA_SIZE );
    UInt iTargetContentIndex = ( iOffset % RDB_CHUNK_DATA_SIZE );

    while( hResource->iChunkIndex > iTargetChunkIndex ) {
        hResource->pChunk = hResource->pContent->pPrevChunk;
        hResource->pContent = m_dbStorage.GetConstData( hResource->pChunk );
        --(hResource->iChunkIndex);
    }
    while( hResource->iChunkIndex < iTargetChunkIndex ) {
        hResource->pChunk = hResource->pContent->pNextChunk;
        hResource->pContent = m_dbStorage.GetConstData( hResource->pChunk );
        ++(hResource->iChunkIndex);
    }
    hResource->iContentIndex = iTargetContentIndex;
}
Void ResourceDB::ResourceSeekRel( RDB_ResourceHandle * hResource, Int iOffset )
{
    Assert( m_bValidDB );
    Assert( hResource != NULL );
    Assert( hResource->iChunkIndex != INVALID_OFFSET );
    Assert( hResource->iContentIndex != INVALID_OFFSET );

    UInt iCurrentOffset = ( hResource->iChunkIndex * RDB_CHUNK_DATA_SIZE ) + hResource->iContentIndex;
    Int iTargetOffset = ((Int)iCurrentOffset) + iOffset;
    Assert( (iTargetOffset >= 0) && (iTargetOffset < (Int)(hResource->resDescriptor.iDataSize)) );

    ResourceSeekAbs( hResource, (UInt)iTargetOffset );
}

Void ResourceDB::ResourceRead( RDB_ResourceHandle * hResource, UInt iSize, Byte * outData )
{
    Assert( m_bValidDB );
    Assert( hResource != NULL );
    Assert( hResource->iChunkIndex != INVALID_OFFSET );
    Assert( hResource->iContentIndex != INVALID_OFFSET );

    // Verify available data
    UInt iCurrentOffset = ( hResource->iChunkIndex * RDB_CHUNK_DATA_SIZE ) + hResource->iContentIndex;
    Assert( iSize <= (hResource->resDescriptor.iDataSize - iCurrentOffset) );

    // Remaining of current chunk
    UInt iRemainingSize = ( hResource->pContent->iContentSize - hResource->iContentIndex );
    if ( iSize < iRemainingSize ) {
        MemCopy( outData, hResource->pContent->arrContent + hResource->iContentIndex, iSize );
        hResource->iContentIndex += iSize;
        return;
    }
    MemCopy( outData, hResource->pContent->arrContent + hResource->iContentIndex, iRemainingSize );
    outData += iRemainingSize;
    iSize -= iRemainingSize;

    // Switch to next chunk
    if ( hResource->pContent->pNextChunk.idBlock == (RDB_BlockID)INVALID_OFFSET ) {
        Assert( iSize == 0 );
        hResource->iChunkIndex = INVALID_OFFSET;
        hResource->iContentIndex = INVALID_OFFSET;
        return; // End of Resource reached
    }
    hResource->pChunk = hResource->pContent->pNextChunk;
    hResource->pContent = m_dbStorage.GetConstData( hResource->pChunk );
    ++(hResource->iChunkIndex);
    hResource->iContentIndex = 0;

    // Whole chunks
    while( iSize >= RDB_CHUNK_DATA_SIZE ) {
        Assert( hResource->pContent->iContentSize == RDB_CHUNK_DATA_SIZE );
        MemCopy( outData, hResource->pContent->arrContent, RDB_CHUNK_DATA_SIZE );
        outData += RDB_CHUNK_DATA_SIZE;
        iSize -= RDB_CHUNK_DATA_SIZE;

        if ( hResource->pContent->pNextChunk.idBlock == (RDB_BlockID)INVALID_OFFSET ) {
            Assert( iSize == 0 );
            hResource->iChunkIndex = INVALID_OFFSET;
            hResource->iContentIndex = INVALID_OFFSET;
            return; // End of Resource reached
        }
        hResource->pChunk = hResource->pContent->pNextChunk;
        hResource->pContent = m_dbStorage.GetConstData( hResource->pChunk );
        ++(hResource->iChunkIndex);
    }

    // Begining of last chunk
    if ( iSize > 0 ) {
        Assert( iSize <= hResource->pContent->iContentSize );
        MemCopy( outData, hResource->pContent->arrContent, iSize );
        if ( iSize == hResource->pContent->iContentSize ) {
            hResource->iChunkIndex = INVALID_OFFSET;
            hResource->iContentIndex = INVALID_OFFSET;
            return; // End of Resource reached
        }
        hResource->iContentIndex = iSize;
    }
}
Void ResourceDB::ResourceWrite( RDB_ResourceHandle * hResource, UInt iSize, const Byte * pData )
{
    Assert( m_bValidDB );
    Assert( hResource != NULL );
    Assert( hResource->iChunkIndex != INVALID_OFFSET );
    Assert( hResource->iContentIndex != INVALID_OFFSET );
    Assert( hResource->bAllowWrite );

    // Verify available data
    UInt iCurrentOffset = ( hResource->iChunkIndex * RDB_CHUNK_DATA_SIZE ) + hResource->iContentIndex;
    Assert( iSize <= (hResource->resDescriptor.iDataSize - iCurrentOffset) );

    RDB_Chunk_Data * pWriteContent;

    // Remaining of current chunk
    UInt iRemainingSize = ( hResource->pContent->iContentSize - hResource->iContentIndex );
    if ( iSize < iRemainingSize ) {
        pWriteContent = m_dbStorage.GetData( hResource->pChunk );
        MemCopy( pWriteContent->arrContent + hResource->iContentIndex, pData, iSize );
        hResource->iContentIndex += iSize;

        // Done
        m_dbStorage.Commit();
        return;
    }
    pWriteContent = m_dbStorage.GetData( hResource->pChunk );
    MemCopy( pWriteContent->arrContent + hResource->iContentIndex, pData, iRemainingSize );
    pData += iRemainingSize;
    iSize -= iRemainingSize;

    // Switch to next chunk
    if ( hResource->pContent->pNextChunk.idBlock == (RDB_BlockID)INVALID_OFFSET ) {
        Assert( iSize == 0 );
        hResource->iChunkIndex = INVALID_OFFSET;
        hResource->iContentIndex = INVALID_OFFSET;

        m_dbStorage.Commit();
        return; // End of Resource reached
    }
    hResource->pChunk = hResource->pContent->pNextChunk;
    hResource->pContent = m_dbStorage.GetConstData( hResource->pChunk );
    ++(hResource->iChunkIndex);
    hResource->iContentIndex = 0;

    // Whole chunks
    while( iSize >= RDB_CHUNK_DATA_SIZE ) {
        Assert( hResource->pContent->iContentSize == RDB_CHUNK_DATA_SIZE );
        pWriteContent = m_dbStorage.GetData( hResource->pChunk );
        MemCopy( pWriteContent->arrContent, pData, RDB_CHUNK_DATA_SIZE );
        pData += RDB_CHUNK_DATA_SIZE;
        iSize -= RDB_CHUNK_DATA_SIZE;

        if ( hResource->pContent->pNextChunk.idBlock == (RDB_BlockID)INVALID_OFFSET ) {
            Assert( iSize == 0 );
            hResource->iChunkIndex = INVALID_OFFSET;
            hResource->iContentIndex = INVALID_OFFSET;

            m_dbStorage.Commit();
            return; // End of Resource reached
        }
        hResource->pChunk = hResource->pContent->pNextChunk;
        hResource->pContent = m_dbStorage.GetConstData( hResource->pChunk );
        ++(hResource->iChunkIndex);
    }

    // Begining of last chunk
    if ( iSize > 0 ) {
        Assert( iSize <= hResource->pContent->iContentSize );
        pWriteContent = m_dbStorage.GetData( hResource->pChunk );
        MemCopy( pWriteContent->arrContent, pData, iSize );
        if ( iSize == hResource->pContent->iContentSize ) {
            hResource->iChunkIndex = INVALID_OFFSET;
            hResource->iContentIndex = INVALID_OFFSET;

            m_dbStorage.Commit();
            return; // End of Resource reached
        }
        hResource->iContentIndex = iSize;
    }

    // Done
    m_dbStorage.Commit();
}

Void ResourceDB::ResourceAppend( RDB_ResourceHandle * hResource, UInt iSize, const Byte * pData )
{
    Assert( m_bValidDB );
    Assert( hResource != NULL );
    Assert( hResource->iChunkIndex != INVALID_OFFSET );
    Assert( hResource->iContentIndex != INVALID_OFFSET );
    Assert( hResource->bAllowWrite );

    RDB_ChunkPtr pChunk, pNewChunk;
    RDB_Chunk_Data *pChunkContent, *pNewChunkContent;

    // Remaining of last chunk
    pChunk = hResource->resDescriptor.pLastChunk;
    pChunkContent = m_dbStorage.GetData( pChunk );
    UInt iRemainingSize = ( RDB_CHUNK_DATA_SIZE - pChunkContent->iContentSize );
    if ( iSize <= iRemainingSize ) {
        MemCopy( pChunkContent->arrContent + pChunkContent->iContentSize, pData, iSize );
        pChunkContent->iContentSize += iSize;
        hResource->resDescriptor.iDataSize += iSize;

        // Done
        m_dbTreeOp.UpdateResource( hResource->keyData, hResource->resDescriptor );
        m_dbStorage.Commit();
        return;
    }
    MemCopy( pChunkContent->arrContent + pChunkContent->iContentSize, pData, iRemainingSize );
    pChunkContent->iContentSize += iRemainingSize;
    hResource->resDescriptor.iDataSize += iRemainingSize;
    pData += iRemainingSize;
    iSize -= iRemainingSize;

    // Whole chunks
    while( iSize >= RDB_CHUNK_DATA_SIZE ) {
        pNewChunkContent = m_dbStorage.AllocData( &pNewChunk );
        pChunkContent->pNextChunk = pNewChunk;
        pNewChunkContent->pPrevChunk = pChunk;
        pNewChunkContent->pNextChunk.idBlock = (RDB_BlockID)INVALID_OFFSET;
        pNewChunkContent->pNextChunk.idChunk = (RDB_ChunkID)INVALID_OFFSET;
        pNewChunkContent->pNextChunk.iType = RDB_CHUNK_DATA;

        pNewChunkContent->iContentSize = RDB_CHUNK_DATA_SIZE;
        MemCopy( pNewChunkContent->arrContent, pData, RDB_CHUNK_DATA_SIZE );
        pData += RDB_CHUNK_DATA_SIZE;
        iSize -= RDB_CHUNK_DATA_SIZE;

        hResource->resDescriptor.pLastChunk = pNewChunk;
        ++(hResource->resDescriptor.iChunkCount);
        hResource->resDescriptor.iDataSize += RDB_CHUNK_DATA_SIZE;

        pChunk = pNewChunk;
        pChunkContent = pNewChunkContent;
    }

    // Begining of last chunk
    if ( iSize > 0 ) {
        pNewChunkContent = m_dbStorage.AllocData( &pNewChunk );
        pChunkContent->pNextChunk = pNewChunk;
        pNewChunkContent->pPrevChunk = pChunk;
        pNewChunkContent->pNextChunk.idBlock = (RDB_BlockID)INVALID_OFFSET;
        pNewChunkContent->pNextChunk.idChunk = (RDB_ChunkID)INVALID_OFFSET;
        pNewChunkContent->pNextChunk.iType = RDB_CHUNK_DATA;

        pNewChunkContent->iContentSize = iSize;
        MemCopy( pNewChunkContent->arrContent, pData, iSize );

        hResource->resDescriptor.pLastChunk = pNewChunk;
        ++(hResource->resDescriptor.iChunkCount);
        hResource->resDescriptor.iDataSize += iSize;
    }

    // Done
    m_dbTreeOp.UpdateResource( hResource->keyData, hResource->resDescriptor );
    m_dbStorage.Commit();
}
Void ResourceDB::ResourceTruncate( RDB_ResourceHandle * hResource, UInt iSize )
{
    Assert( m_bValidDB );
    Assert( hResource != NULL );
    Assert( hResource->iChunkIndex != INVALID_OFFSET );
    Assert( hResource->iContentIndex != INVALID_OFFSET );
    Assert( hResource->bAllowWrite );

    if ( iSize > hResource->resDescriptor.iDataSize )
         iSize = hResource->resDescriptor.iDataSize;

    RDB_ChunkPtr pChunk, pTmpChunk;
    RDB_Chunk_Data * pChunkContent;

    // Begining of last chunk
    pChunk = hResource->resDescriptor.pLastChunk;
    pChunkContent = m_dbStorage.GetData( pChunk );
    if ( iSize <= pChunkContent->iContentSize ) {
        pChunkContent->iContentSize -= iSize;
        hResource->resDescriptor.iDataSize -= iSize;

        // Done
        m_dbTreeOp.UpdateResource( hResource->keyData, hResource->resDescriptor );
        m_dbStorage.Commit();
        return;
    }

    iSize -= pChunkContent->iContentSize;

    pTmpChunk = pChunk;
    pChunk = pChunkContent->pPrevChunk;
    pChunkContent = m_dbStorage.GetData( pChunk );
    pChunkContent->pNextChunk.idBlock = (RDB_BlockID)INVALID_OFFSET;
    pChunkContent->pNextChunk.idChunk = (RDB_ChunkID)INVALID_OFFSET;
    pChunkContent->pNextChunk.iType = RDB_CHUNK_DATA;
    m_dbStorage.Free( pTmpChunk );

    hResource->resDescriptor.pLastChunk = pChunk;
    --(hResource->resDescriptor.iChunkCount);
    hResource->resDescriptor.iDataSize -= pChunkContent->iContentSize;

    // Whole chunks
    while( iSize >= RDB_CHUNK_DATA_SIZE ) {
        iSize -= RDB_CHUNK_DATA_SIZE;

        pTmpChunk = pChunk;
        pChunk = pChunkContent->pPrevChunk;
        pChunkContent = m_dbStorage.GetData( pChunk );
        pChunkContent->pNextChunk.idBlock = (RDB_BlockID)INVALID_OFFSET;
        pChunkContent->pNextChunk.idChunk = (RDB_ChunkID)INVALID_OFFSET;
        pChunkContent->pNextChunk.iType = RDB_CHUNK_DATA;
        m_dbStorage.Free( pTmpChunk );

        hResource->resDescriptor.pLastChunk = pChunk;
        --(hResource->resDescriptor.iChunkCount);
        hResource->resDescriptor.iDataSize -= RDB_CHUNK_DATA_SIZE;
    }

    // Remaining of last chunk
    if ( iSize > 0 ) {
        pChunkContent->iContentSize -= iSize;
        hResource->resDescriptor.iDataSize -= iSize;
    }

    // Done
    m_dbTreeOp.UpdateResource( hResource->keyData, hResource->resDescriptor );
    m_dbStorage.Commit();
}
Void ResourceDB::ResourceErase( RDB_ResourceHandle * hResource )
{
    Assert( m_bValidDB );
    Assert( hResource != NULL );
    Assert( hResource->iChunkIndex != INVALID_OFFSET );
    Assert( hResource->iContentIndex != INVALID_OFFSET );
    Assert( hResource->bAllowWrite );

    // Destroy all chunks but one
    RDB_ChunkPtr pFirstChunk = hResource->resDescriptor.pFirstChunk;
    RDB_ChunkPtr pChunk = hResource->resDescriptor.pLastChunk;
    RDB_ChunkPtr pPrevChunk;
    const RDB_Chunk_Data * pChunkContent;
    while( pChunk != pFirstChunk ) {
        pChunkContent = m_dbStorage.GetConstData( pChunk );
        pPrevChunk = pChunkContent->pPrevChunk;

        m_dbStorage.Free( pChunk );

        pChunk = pPrevChunk;
    }

    // Erase first chunk
    RDB_Chunk_Data * pFirstChunkContent = m_dbStorage.GetData( pFirstChunk );
    pFirstChunkContent->iContentSize = 0;
    pFirstChunkContent->pNextChunk.idBlock = (RDB_BlockID)INVALID_OFFSET;
    pFirstChunkContent->pNextChunk.idChunk = (RDB_ChunkID)INVALID_OFFSET;
    pFirstChunkContent->pNextChunk.iType = RDB_CHUNK_DATA;

    // Update descriptor
    hResource->resDescriptor.pLastChunk = pFirstChunk;
    hResource->resDescriptor.iChunkCount = 1;
    hResource->resDescriptor.iDataSize = 0;

    // Update handle
    hResource->pChunk = pFirstChunk;
    hResource->pContent = m_dbStorage.GetConstData( pFirstChunk );
    hResource->iChunkIndex = 0;
    hResource->iContentIndex = 0;

    // Done
    m_dbTreeOp.UpdateResource( hResource->keyData, hResource->resDescriptor );
    m_dbStorage.Commit();
}

/////////////////////////////////////////////////////////////////////////////////

Void ResourceDB::_Destroy_Directory( const RDB_BTreeItem & itDirectory )
{
    Bool bOk;
    UInt i;

    // Create temp array
    Array<RDB_BTreeItem> arrTmp;
    arrTmp.UseMemoryContext( ResourceFn->GetMemoryContext(), TEXT("Scratch") );
    arrTmp.Create();

    // Remove content
    m_dbTreeOp.ListResources( itDirectory.Content.descDirectory.idDirectory, &arrTmp );    
    for( i = 0; i < arrTmp.Count(); ++i ) {
        Assert( arrTmp[i].iResourceType != RESOURCE_TYPE_DIRECTORY );
        _Destroy_Resource( arrTmp[i] );
    }

    arrTmp.Clear();

    // Remove sub directories
    m_dbTreeOp.ListSubDirectories( itDirectory.Content.descDirectory.idDirectory, &arrTmp );
    for( i = 0; i < arrTmp.Count(); ++i ) {
        Assert( arrTmp[i].iResourceType == RESOURCE_TYPE_DIRECTORY );
        _Destroy_Directory( arrTmp[i] );
    }

    // Destroy temp array
    arrTmp.Destroy();

    // Remove the directory itself
    bOk = m_dbTreeOp.RemoveDirectory( itDirectory.keyData );
    Assert( bOk );
}
Void ResourceDB::_Destroy_Resource( const RDB_BTreeItem & itResource )
{
    RDB_ChunkPtr pCurStorage, pNextStorage;
    const RDB_Chunk_Data * pCurData;

    // Free data chunks
    pCurStorage = itResource.Content.descResource.pFirstChunk;
    for( UInt i = 0; i < itResource.Content.descResource.iChunkCount; ++i ) {
        pCurData = m_dbStorage.GetConstData( pCurStorage );
        pNextStorage = pCurData->pNextChunk;
        m_dbStorage.Free( pCurStorage );
        pCurStorage = pNextStorage;
    }

    // Remove
    Bool bOk = m_dbTreeOp.RemoveResource( itResource.keyData );
    Assert( bOk );
}

/////////////////////////////////////////////////////////////////////////////////

Bool ResourceDB::_Path_IsValidChar( GChar ch )
{
    if ( ch == NULLBYTE || ch == RDB_PATH_SEPARATOR )
        return true;
    if ( ch == TEXT('_') || ch == TEXT('.') || StringFn->IsAlphaNum(ch) )
        return true;
    return false;
}
Bool ResourceDB::_Path_IsValidName( const GChar * strName )
{
    UInt iLength = 0;
    while( *strName != NULLBYTE ) {
        if ( !_Path_IsValidChar(*strName) )
            return false;
        if ( *strName == RDB_PATH_SEPARATOR )
            return false;
        ++iLength;
        ++strName;
        if ( iLength >= RDB_NAME_LENGTH )
            return false;
    }
    return true;
}

Bool ResourceDB::_Path_DirectorySearch( const GChar * strPath, RDB_BTreeItem * outDirectory )
{
    const GChar * pCh = strPath;

    // Test Leading Root slash
    if ( *pCh != RDB_PATH_SEPARATOR )
        return false;
    ++pCh;

    // Root path is "/"
    if ( *pCh == NULLBYTE ) {
        *outDirectory = RDB_ROOT_DIR;
        return true;
    }

    // Parse loop
    Bool bFound;
    RDB_BTreeKeyData curKeyData;
    curKeyData.idParent = 0;
    AChar * pNameCh = curKeyData.strName;
    while( true ) {
        if ( !_Path_IsValidChar(*pCh) )
            return false;
        if ( *pCh == NULLBYTE ) {
            *pNameCh = ANULLBYTE;
            return m_dbTreeOp.SearchDirectory( curKeyData, outDirectory );
        }
        if ( *pCh == RDB_PATH_SEPARATOR ) {
            *pNameCh = ANULLBYTE;
            bFound = m_dbTreeOp.SearchDirectory( curKeyData, outDirectory );
            if ( !bFound )
                return false;
            if ( outDirectory->iResourceType != RESOURCE_TYPE_DIRECTORY )
                return false;
            curKeyData.idParent = outDirectory->Content.descDirectory.idDirectory;
            pNameCh = curKeyData.strName;
            ++pCh;
            continue;
        }
        *(pNameCh++) = (AChar)( *(pCh++) );
    }
}
Bool ResourceDB::_Path_ResourceSearch( const GChar * strPath, RDB_BTreeItem * outResource )
{
    const GChar * pCh = strPath;

    // Test Leading Root slash
    if ( *pCh != RDB_PATH_SEPARATOR )
        return false;
    ++pCh;

    // Root path is "/"
    if ( *pCh == NULLBYTE )
        return false;

    // Parse loop
    Bool bFound;
    RDB_BTreeKeyData curKeyData;
    curKeyData.idParent = 0;
    AChar * pNameCh = curKeyData.strName;
    while( true ) {
        if ( !_Path_IsValidChar(*pCh) )
            return false;
        if ( *pCh == NULLBYTE ) {
            *pNameCh = ANULLBYTE;
            return m_dbTreeOp.SearchResource( curKeyData, outResource );
        }
        if ( *pCh == RDB_PATH_SEPARATOR ) {
            *pNameCh = ANULLBYTE;
            bFound = m_dbTreeOp.SearchDirectory( curKeyData, outResource );
            if ( !bFound )
                return false;
            if ( outResource->iResourceType != RESOURCE_TYPE_DIRECTORY )
                return false;
            curKeyData.idParent = outResource->Content.descDirectory.idDirectory;
            pNameCh = curKeyData.strName;
            ++pCh;
            continue;
        }
        *(pNameCh++) = (AChar)( *(pCh++) );
    }
}

/////////////////////////////////////////////////////////////////////////////////

UInt ResourceDB::_CWD_GetPathString( GChar * outPath ) const
{
    // Root case
    if ( m_itCWD.keyData.idParent == INVALID_OFFSET ) {
        *(outPath++) = RDB_PATH_SEPARATOR;
        *outPath = NULLBYTE;
        return 1;
    }

    // Build path string
    GChar * pCh = outPath;
    for( UInt i = 0; i < m_stkParentKeys.Count(); ++i ) {
        *pCh++ = RDB_PATH_SEPARATOR;
#if defined(UNICODE) || defined(_UNICODE)
        pCh = StringFn->AsciiToWideChar( pCh, m_stkParentKeys[i].strName, RDB_PATH_LENGTH - 1 - ( pCh - outPath ) );
#else
        pCh = StringFn->NCopyA( pCh, m_stkParentKeys[i].strName, RDB_PATH_LENGTH - 1 - ( pCh - outPath ) );
#endif
    }
    *pCh++ = RDB_PATH_SEPARATOR;
#if defined(UNICODE) || defined(_UNICODE)
    pCh = StringFn->AsciiToWideChar( pCh, m_itCWD.keyData.strName, RDB_PATH_LENGTH - 1 - ( pCh - outPath ) );
#else
    pCh = StringFn->NCopyA( pCh, m_itCWD.keyData.strName, RDB_PATH_LENGTH - 1 - ( pCh - outPath ) );
#endif

    return ( pCh - outPath );
}
UInt ResourceDB::_CWD_ResolvePath( GChar * outAbsolutePath, const GChar * strRelativePath ) const
{
    // CWD path
    UInt iLength = _CWD_GetPathString( outAbsolutePath );
    GChar * pAbs = ( outAbsolutePath + iLength );
    Bool bRootReached = false;

    // Null path case
    if ( *strRelativePath == NULLBYTE )
        return iLength;

    // Self path case
    if ( StringFn->Cmp(strRelativePath, RDB_SELF_PATH) == 0 )
        return iLength;

    // Remove root slash
    if ( iLength == 1 ) {
        pAbs = outAbsolutePath;
        *pAbs = NULLBYTE;
        bRootReached = true;
    }

    // Parse relative path
    const GChar * pRel = strRelativePath;
    GChar tmpName[RDB_NAME_LENGTH];
    GChar * pName = tmpName;
    while( true ) {
        if ( (*pRel == NULLBYTE) || (*pRel == RDB_PATH_SEPARATOR) ) {
            *pName = NULLBYTE;

            if ( StringFn->Cmp(tmpName, RDB_PARENT_PATH) == 0 ) {
                if ( bRootReached )
                    return 0; // already at root
                while( *pAbs != RDB_PATH_SEPARATOR )
                    --pAbs;
                *pAbs = NULLBYTE;
                bRootReached = ( pAbs == outAbsolutePath );
            } else {
                *pAbs++ = RDB_PATH_SEPARATOR;
                pAbs = StringFn->NCopy( pAbs, tmpName, RDB_NAME_LENGTH - 1 );
                bRootReached = false;
            }
        }
        if ( *pRel == NULLBYTE )
            break;
        if ( *pRel == RDB_PATH_SEPARATOR ) {
            ++pRel;
            pName = tmpName;
        } else
            *pName++ = *pRel++;
    }

    // Add root slash back
    if ( bRootReached ) {
        *pAbs++ = RDB_PATH_SEPARATOR;
        *pAbs = NULLBYTE;
    }

    return ( pAbs - outAbsolutePath );
}

Bool ResourceDB::_CWD_WalkUp()
{
    // Root case
    if ( m_itCWD.keyData.idParent == INVALID_OFFSET )
        return false;

    // Before-Root case
    if ( m_itCWD.keyData.idParent == RDB_ROOT_ID ) {
        m_itCWD = RDB_ROOT_DIR;
        return true;
    }

    // Retrieve parent key
    RDB_BTreeKeyData parentKey;
    m_stkParentKeys.Pop( parentKey );

    // Lookup new CWD
    RDB_BTreeItem newCWD;
    Bool bOk = m_dbTreeOp.SearchDirectory( parentKey, &newCWD );
    Assert( bOk );

    // Update CWD state
    m_itCWD = newCWD;

    return true;
}
Bool ResourceDB::_CWD_WalkDown( const GChar * strName )
{
    // Build child key
    RDB_BTreeKeyData childKey;
    childKey.idParent = m_itCWD.Content.descDirectory.idDirectory;
#if defined(UNICODE) || defined(_UNICODE)
    StringFn->WideCharToMultiByte( childKey.strName, strName, RDB_NAME_LENGTH - 1 );
#else
    StringFn->NCopyA( childKey.strName, strName, RDB_NAME_LENGTH - 1 );
#endif

    // Lookup new CWD
    RDB_BTreeItem newCWD;
    Bool bOk = m_dbTreeOp.SearchDirectory( childKey, &newCWD );
    if ( !bOk )
        return false;

    // Update CWD state
    if ( m_itCWD.keyData.idParent != INVALID_OFFSET )
        m_stkParentKeys.Push( m_itCWD.keyData );
    m_itCWD = newCWD;

    return true;
}
Bool ResourceDB::_CWD_WalkThrough( const GChar * strRelativePath )
{
    // Null path case
    if ( *strRelativePath == NULLBYTE )
        return true;

    // Self path case
    if ( StringFn->Cmp(strRelativePath, RDB_SELF_PATH) == 0 )
        return true;

    // Parse relative path
    const GChar * pRel = strRelativePath;
    GChar tmpName[RDB_NAME_LENGTH];
    GChar * pName = tmpName;
    while( true ) {
        if ( (*pRel == NULLBYTE) || (*pRel == RDB_PATH_SEPARATOR) ) {
            *pName = NULLBYTE;

            if ( StringFn->Cmp( tmpName, RDB_PARENT_PATH ) == 0 ) {
                if ( !_CWD_WalkUp() )
                    return false;
            } else {
                if ( !_CWD_WalkDown(tmpName) )
                    return false;
            }
        }
        if ( *pRel == NULLBYTE )
            break;
        if ( *pRel == RDB_PATH_SEPARATOR ) {
            ++pRel;
            pName = tmpName;
        } else
            *(pName++) = *(pRel++);
    }
    return true;
}

