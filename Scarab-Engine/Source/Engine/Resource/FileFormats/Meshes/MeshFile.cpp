/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Resource/FileFormats/Meshes/MeshFile.cpp
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
// Includes
#include "MeshFile.h"

#include "../../ResourceManager.h"

/////////////////////////////////////////////////////////////////////////////////
// MeshFile implementation
MeshFile::MeshFile():
    ResourceFile()
{
    MemZero( &m_hMeshHeader, sizeof(MeshHeader) );
}
MeshFile::~MeshFile()
{
    // nothing to do
}

const GChar * MeshFile::GetTypeName( MeshFileType iFileType )
{
    switch( iFileType ) {
        case MESHFILE_MESH: return TEXT("mesh");
        case MESHFILE_DAE: return TEXT("dae");
        case MESHFILE_3DS: return TEXT("3ds");
        default: Assert(false); return NULL;
    }
}
MeshFileType MeshFile::GetType( const GChar * strFileTypeName )
{
    // Simple search
    MeshFileType iFileType;
    for( UInt i = 0; i < MESHFILE_COUNT; ++i ) {
        iFileType = (MeshFileType)i;
        if ( StringFn->Cmp(GetTypeName(iFileType), strFileTypeName) == 0 )
            return iFileType;
    }

    // Not found
    return MESHFILE_UNDEFINED;
}

/////////////////////////////////////////////////////////////////////////////////

Bool MeshFile::_OnExport( ResourceDB * pRDB, RDB_ResourceHandle * hResource )
{
    if ( m_pData != NULL )
        return false;

    pRDB->ResourceRead( hResource, sizeof(MeshHeader), (Byte*)(&m_hMeshHeader) );

    _CreateData( (ResourceFileHeader*)&m_hMeshHeader );
    pRDB->ResourceRead( hResource, m_hMeshHeader.dwDataSize, m_pData );

    return true;
}
Bool MeshFile::_OnImport( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) const
{
    if ( m_pData == NULL )
        return false;

    pRDB->ResourceAppend( hResource, sizeof(MeshHeader), (const Byte*)(&m_hMeshHeader) );

    pRDB->ResourceAppend( hResource, m_hMeshHeader.dwDataSize, m_pData );

    return true;
}


