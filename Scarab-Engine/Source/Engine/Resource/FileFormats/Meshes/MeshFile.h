/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Resource/FileFormats/Meshes/MeshFile.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : File formats : Meshes
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
#ifndef SCARAB_ENGINE_RESOURCE_FILEFORMATS_MESHES_MESHFILE_H
#define SCARAB_ENGINE_RESOURCE_FILEFORMATS_MESHES_MESHFILE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../ResourceFile.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
enum MeshFileType
{
    MESHFILE_UNDEFINED = INVALID_OFFSET,

	MESHFILE_MESH = 0, // Native format

    // Mesh formats
    MESHFILE_DAE, // Standard XML-based COLLADA format
	MESHFILE_3DS, // Legacy 3DS Max format

    MESHFILE_COUNT
};

// Mesh header
typedef struct _mesh_header
{
    DWord dwDataSize;

    DWord dwVFFieldCount;
    struct {
        DWord dwType;
        DWord dwUsage;
        DWord dwUsageIndex;
    } arrVFFields[64];

    DWord dwVertexSize;
    DWord dwVertexCount;
    DWord dwVertexOffset;

    DWord dwIndexSize;
    DWord dwIndexCount;
    DWord dwIndexOffset;

    DWord dwMeshType;
} MeshHeader;

/////////////////////////////////////////////////////////////////////////////////
// The MeshFile interface
class MeshFile : public ResourceFile
{
public:
	MeshFile();
	virtual ~MeshFile();

    inline ResourceType GetResourceType() const;

    static const GChar * GetTypeName( MeshFileType iFileType );
    static MeshFileType GetType( const GChar * strFileTypeName );

	virtual MeshFileType GetType() const = 0;

	virtual Bool Load( const GChar * strFilename ) = 0;
	virtual Bool Save( const GChar * strFilename ) const = 0;

protected:
    virtual Bool _OnExport( ResourceDB * pRDB, RDB_ResourceHandle * hResource );
    virtual Bool _OnImport( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) const;

    MeshHeader m_hMeshHeader;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "MeshFile.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RESOURCE_FILEFORMATS_MESHES_MESHFILE_H

