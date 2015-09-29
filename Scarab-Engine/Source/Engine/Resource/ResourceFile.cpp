/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Resource/ResourceFile.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Base interface for all resource-managed files.
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
#include "ResourceFile.h"

#include "ResourceManager.h"

/////////////////////////////////////////////////////////////////////////////////
// ResourceFile implementation
ResourceFile::ResourceFile()
{
    m_pHeader = NULL;
    m_pData = NULL;
}
ResourceFile::~ResourceFile()
{
    if ( m_pData != NULL )
        _DestroyData();
}

Bool ResourceFile::Export( const GChar * strDBName, const GChar * strResPath )
{
    // Pick the registered RDB
    ResourceDB * pRDB = ResourceFn->GetRDB( strDBName );
    if ( pRDB == NULL )
        return false;

    // Open the resource
    RDB_ResourceHandle hResource;
    Bool bOk = pRDB->ResourceOpen( &hResource, strResPath, true );
    if ( !bOk )
        return false;

    // Check resource type
    if ( pRDB->ResourceGetType(&hResource) != GetResourceType() ) {
        pRDB->ResourceClose( &hResource );
        return false;
    }

    // Export content
    bOk = _OnExport( pRDB, &hResource );
    
    // Close the resource
    pRDB->ResourceClose( &hResource );

    // Done
    return bOk;
}
Bool ResourceFile::Import( const GChar * strDBName, const GChar * strResPath ) const
{
    // Pick the registered RDB
    ResourceDB * pRDB = ResourceFn->GetRDB( strDBName );
    if ( pRDB == NULL )
        return false;

    // Open the resource
    RDB_ResourceHandle hResource;
    Bool bOk = pRDB->ResourceOpen( &hResource, strResPath, false );
    if ( !bOk ) {
        // Not existing, create
        GChar strParentDir[RDB_PATH_LENGTH + 1];
        GChar strName[RDB_NAME_LENGTH + 1];
        GChar * strTmp = StringFn->NCopy( strParentDir, strResPath, RDB_PATH_LENGTH );
        while( strTmp > strParentDir && *strTmp != TEXT('/') )
            --strTmp;
        if ( strTmp == strParentDir ) {
            StringFn->NCopy( strName, strParentDir, RDB_NAME_LENGTH );
            StringFn->Copy( strParentDir, RDB_SELF_PATH );
        } else {
            StringFn->NCopy( strName, strTmp + 1, RDB_NAME_LENGTH );
            *strTmp = NULLBYTE;
        }

        bOk = pRDB->CreateResource( strParentDir, strName, GetResourceType() );
        if ( !bOk )
            return false;
        bOk = pRDB->ResourceOpen( &hResource, strResPath, false );
        if ( !bOk )
            return false;
    } else {
        // Existing, erase content
        pRDB->ResourceErase( &hResource );
    }

    // Check resource type
    if ( pRDB->ResourceGetType(&hResource) != GetResourceType() ) {
        pRDB->ResourceClose( &hResource );
        return false;
    }

    // Import content
    bOk = _OnImport( pRDB, &hResource );
    
    // Close the resource
    pRDB->ResourceClose( &hResource );

    // Done
    return bOk;
}

/////////////////////////////////////////////////////////////////////////////////

Void ResourceFile::_CreateData( ResourceFileHeader * pHeader )
{
    Assert( m_pData == NULL );

    m_pHeader = pHeader;

    ResourceFn->SelectMemory( TEXT("Scratch") );
    m_pData = New Byte[m_pHeader->dwDataSize];
    ResourceFn->UnSelectMemory();
}
Void ResourceFile::_DestroyData()
{
    Assert( m_pData != NULL );

    ResourceFn->SelectMemory( TEXT("Scratch") );
    DeleteA( m_pData );
    ResourceFn->UnSelectMemory();

    m_pHeader = NULL;
    m_pData = NULL;
}

