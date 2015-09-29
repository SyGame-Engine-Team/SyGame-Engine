/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Resource/ResourceFile.h
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
// Header prelude
#ifndef SCARAB_ENGINE_RESOURCE_RESOURCEFILE_H
#define SCARAB_ENGINE_RESOURCE_RESOURCEFILE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Database/ResourceDB.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
typedef struct _resource_file_header
{
    DWord dwDataSize;
    // Specific data follows ...
} ResourceFileHeader;

/////////////////////////////////////////////////////////////////////////////////
// The ResourceFile interface
class ResourceFile
{
public:
	ResourceFile();
	virtual ~ResourceFile();

	virtual ResourceType GetResourceType() const = 0;

    inline Bool HasData() const;
    inline const ResourceFileHeader * GetHeader() const;
    inline Byte * GetData() const;

    virtual Bool Load( const GChar * strFilename ) = 0;
	virtual Bool Save( const GChar * strFilename ) const = 0;

    Bool Export( const GChar * strDBName, const GChar * strResPath );
    Bool Import( const GChar * strDBName, const GChar * strResPath ) const;

protected:
    virtual Bool _OnExport( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) = 0;
    virtual Bool _OnImport( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) const = 0;

    Void _CreateData( ResourceFileHeader * pHeader );
    Void _DestroyData();

    ResourceFileHeader * m_pHeader;
    Byte * m_pData;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ResourceFile.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RESOURCE_RESOURCEFILE_H

