/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Resource/ResourceManager.h
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
// Header prelude
#ifndef SCARAB_ENGINE_RESOURCE_RESOURCEMANAGER_H
#define SCARAB_ENGINE_RESOURCE_RESOURCEMANAGER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Lib/Object/Patterns/Manager.h"

// Resource Database
#include "Database/ResourceDB.h"
#include "ResourceFile.h"
#include "Resource.h"

// XML support
#include "XML/XML.h"

// File formats
    // Bitmaps
#include "FileFormats/Bitmaps/BitmapBMAP.h"
#include "FileFormats/Bitmaps/BitmapBMP.h"
    // Textures
#include "FileFormats/Textures/TextureTEX.h"
#include "FileFormats/Textures/TextureBMP.h"
    // Meshes
#include "FileFormats/Meshes/MeshMESH.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define ResourceFn ResourceManager::GetInstancePtr()

// Databases
#define RDB_PATH_BASE    TEXT("Data/Resources")
#define RDB_PATH_IMPORTS TEXT("Data/Resources/Imports")
#define RDB_PATH_EXPORTS TEXT("Data/Resources/Exports")

#define RDB_MASTER       TEXT("MasterDB")

// Packages
#define RDB_PACKAGE_NULL TEXT("/Package_Null")

// Locations
#define RDB_LOCATION_GUI       TEXT("/GUI")
#define RDB_LOCATION_GUI_SKINS TEXT("/GUI/Skins")

/////////////////////////////////////////////////////////////////////////////////
// The ResourceManager class
class ResourceManager : public Manager<ResourceManager>
{
public:
    inline static const GChar * GetName();
    inline static const GChar * GetMemoryBudget();

protected:
    friend class Manager<ResourceManager>;
    ResourceManager();
    virtual ~ResourceManager();

public:
    Void Initialize();
    Void Cleanup();

    // RDB management
    ResourceDB * CreateRDB( const GChar * strName );
    ResourceDB * OpenRDB( const GChar * strName );
    Void CloseRDB( const GChar * strName );

    inline ResourceDB * GetRDB( const GChar * strName ) const;

    inline Void EnumRDBs() const;
    inline ResourceDB * EnumNextRDB() const;

private:
    // Helpers
    typedef TreeMap<const GChar *, ResourceDB *> ResourceDBMap;
    inline static Int _Name_Comparator( const GChar* const & rLeft, const GChar* const & rRight, Void * pUserData );

    // Loaded Databases
    ResourceDBMap m_mapResourceDBs;
    mutable ResourceDBMap::Iterator m_itEnumerate;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ResourceManager.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RESOURCE_RESOURCEMANAGER_H
