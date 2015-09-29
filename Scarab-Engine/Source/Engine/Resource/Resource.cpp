/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Resource/Resource.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Base interface for all resource-managed objects.
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
#include "Resource.h"

#include "ResourceManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Resource implementation
Resource::Resource()
{
    // nothing to do
}
Resource::~Resource()
{
    // nothing to do
}

const GChar * Resource::GetResourceTypeName( ResourceType iType )
{
    switch( iType ) {
        case RESOURCE_TYPE_DIRECTORY: return TEXT("<Directory>");
        case RESOURCE_TYPE_UNDEFINED: return TEXT("<Undefined>");
            // Rendering types
        case RESOURCE_TYPE_MESH:      return TEXT("Mesh");
        case RESOURCE_TYPE_TEXTURE:   return TEXT("Texture");
        default: Assert(false); return NULL;
    }
}
ResourceType Resource::GetResourceType( const GChar * strResourceTypeName )
{
    // Directory case
    if ( StringFn->Cmp(strResourceTypeName, TEXT("Directory")) == 0 )
        return RESOURCE_TYPE_DIRECTORY;

    // Simple search
    ResourceType iType;
    for( UInt i = 1; i < RESOURCE_TYPE_COUNT; ++i ) {
        iType = (ResourceType)i;
        if ( StringFn->Cmp(GetResourceTypeName(iType), strResourceTypeName) == 0 )
            return iType;
    }

    // Not found
    return RESOURCE_TYPE_UNDEFINED;
}

Bool Resource::Load( const GChar * strDBName, const GChar * strResPath )
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

    // Parse & Load content
    bOk = _OnLoad( pRDB, &hResource );

    // Close the resource
    pRDB->ResourceClose( &hResource );

    // Done
    return bOk;
}
Bool Resource::Save( const GChar * strDBName, const GChar * strResPath ) const
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

    // Parse & Save content
    bOk = _OnSave( pRDB, &hResource );

    // Close the resource
    pRDB->ResourceClose( &hResource );

    // Done
    return bOk;
}

