/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Resource/ResourceManager.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Core-Class for the resource management system.
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
#include "ResourceManager.h"

/////////////////////////////////////////////////////////////////////////////////
// ResourceManager implementation
ResourceManager::ResourceManager():
    Manager<ResourceManager>(), m_mapResourceDBs(), m_itEnumerate()
{
    // nothing to do
}
ResourceManager::~ResourceManager()
{
    // nothing to do
}

Void ResourceManager::Initialize()
{
    // Allocate RDBs map
    m_mapResourceDBs.UseMemoryContext( GetMemoryContext(), TEXT("Internal") );
    m_mapResourceDBs.SetComparator( _Name_Comparator, NULL );
    m_mapResourceDBs.Create();

    // Open master DB
    OpenRDB( RDB_MASTER );
}
Void ResourceManager::Cleanup()
{
    // Close all RDBs
    ResourceDBMap::Iterator itRDB;
    while( !(m_mapResourceDBs.IsEmpty()) ) {
        itRDB = m_mapResourceDBs.Begin();
        CloseRDB( itRDB.GetKey() );
    }

    // Destroy RDBs map
    m_mapResourceDBs.Destroy();
}

ResourceDB * ResourceManager::CreateRDB( const GChar * strName )
{
    if ( m_mapResourceDBs.Contains(strName) )
        return NULL;

    SelectMemory( TEXT("Internal") );
    ResourceDB * pRDB = New ResourceDB();
    UnSelectMemory();

    Bool bOk = pRDB->Create( strName );
    if ( !bOk ) {
        SelectMemory( TEXT("Internal") );
        Delete( pRDB );
        UnSelectMemory();
        return NULL;
    }

    bOk = m_mapResourceDBs.Insert( pRDB->GetName(), pRDB );
    Assert( bOk );

    return pRDB;
}
ResourceDB * ResourceManager::OpenRDB( const GChar * strName )
{
    if ( m_mapResourceDBs.Contains(strName) )
        return NULL;

    SelectMemory( TEXT("Internal") );
    ResourceDB * pRDB = New ResourceDB();
    UnSelectMemory();

    Bool bOk = pRDB->Open( strName );
    if ( !bOk ) {
        SelectMemory( TEXT("Internal") );
        Delete( pRDB );
        UnSelectMemory();
        return NULL;
    }

    bOk = m_mapResourceDBs.Insert( pRDB->GetName(), pRDB );
    Assert( bOk );

    return pRDB;
}
Void ResourceManager::CloseRDB( const GChar * strName )
{
    ResourceDB * pRDB = NULL;
    Bool bRemoved = m_mapResourceDBs.Remove( strName, &pRDB );
    if ( !bRemoved )
        return;

    Assert( pRDB != NULL );
    pRDB->Close();

    SelectMemory( TEXT("Internal") );
    Delete( pRDB );
    UnSelectMemory();
}


