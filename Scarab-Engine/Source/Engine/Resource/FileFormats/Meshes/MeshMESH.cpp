/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Resource/FileFormats/Meshes/MeshMESH.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Mesh formats : MESH (native)
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
#include "MeshMESH.h"

#include "../../ResourceManager.h"

/////////////////////////////////////////////////////////////////////////////////
// MeshMESH implementation
MeshMESH::MeshMESH():
    MeshFile()
{
    // nothing to do
}
MeshMESH::~MeshMESH()
{
    // nothing to do
}

Bool MeshMESH::Load( const GChar * strFilename )
{
    Assert( m_pData == NULL );

    // Open file
    HFile hFile = SystemFn->OpenFile( strFilename, FILE_READ );
    if ( !hFile.IsValid() )
        return false;

    // Magic word
    Byte magic[4];
    hFile.ReadBytes( magic, 4 );
    Assert( magic[0] == 'M' );
    Assert( magic[1] == 'E' );
    Assert( magic[2] == 'S' );
    Assert( magic[3] == 'H' );

    // Header
    hFile.ReadBytes( (Byte*)( &(m_hMeshHeader.dwDataSize) ), sizeof(DWord) );

    hFile.ReadBytes( (Byte*)( &(m_hMeshHeader.dwVFFieldCount) ), sizeof(DWord) );
    Assert( m_hMeshHeader.dwVFFieldCount <= 64 );

    for( UInt i = 0; i < m_hMeshHeader.dwVFFieldCount; ++i ) {
        hFile.ReadBytes( (Byte*)( &(m_hMeshHeader.arrVFFields[i].dwType) ), sizeof(DWord) );
        hFile.ReadBytes( (Byte*)( &(m_hMeshHeader.arrVFFields[i].dwUsage) ), sizeof(DWord) );
        hFile.ReadBytes( (Byte*)( &(m_hMeshHeader.arrVFFields[i].dwUsageIndex) ), sizeof(DWord) );
    }
    for( UInt i = m_hMeshHeader.dwVFFieldCount; i < 64; ++i ) {
        DWord arrDummy[3];
        hFile.ReadBytes( (Byte*)arrDummy, 3 * sizeof(DWord) );
        Assert( arrDummy[0] == 0 && arrDummy[1] == 0 && arrDummy[2] == 0 );
    }

    hFile.ReadBytes( (Byte*)( &(m_hMeshHeader.dwVertexSize) ), sizeof(DWord) );
    hFile.ReadBytes( (Byte*)( &(m_hMeshHeader.dwVertexCount) ), sizeof(DWord) );
    hFile.ReadBytes( (Byte*)( &(m_hMeshHeader.dwVertexOffset) ), sizeof(DWord) );

    hFile.ReadBytes( (Byte*)( &(m_hMeshHeader.dwIndexSize) ), sizeof(DWord) );
    hFile.ReadBytes( (Byte*)( &(m_hMeshHeader.dwIndexCount) ), sizeof(DWord) );
    hFile.ReadBytes( (Byte*)( &(m_hMeshHeader.dwIndexOffset) ), sizeof(DWord) );

    Assert( m_hMeshHeader.dwVertexSize > 0 );
    Assert( m_hMeshHeader.dwVertexCount > 0 );
    Assert( m_hMeshHeader.dwDataSize == (m_hMeshHeader.dwVertexSize * m_hMeshHeader.dwVertexCount +
                                         m_hMeshHeader.dwIndexSize * m_hMeshHeader.dwIndexCount) );

    hFile.ReadBytes( (Byte*)( &(m_hMeshHeader.dwMeshType) ), sizeof(DWord) );

    // Content
    _CreateData( (ResourceFileHeader*)&m_hMeshHeader );
    hFile.ReadBytes( m_pData, m_hMeshHeader.dwDataSize );

    // Close file
    hFile.Close();

    return true;
}
Bool MeshMESH::Save( const GChar * strFilename ) const
{
    Assert( m_pData != NULL );
    Assert( m_hMeshHeader.dwVFFieldCount <= 64 );
    Assert( m_hMeshHeader.dwVertexSize > 0 );
    Assert( m_hMeshHeader.dwVertexCount > 0 );
    Assert( m_hMeshHeader.dwDataSize == (m_hMeshHeader.dwVertexSize * m_hMeshHeader.dwVertexCount +
                                         m_hMeshHeader.dwIndexSize * m_hMeshHeader.dwIndexCount) );

    // Create file
    HFile hFile = SystemFn->CreateFile( strFilename, FILE_WRITE );
    if ( !hFile.IsValid() )
        return false;

    // Magic word
    Byte magic[4] = { 'M', 'E', 'S', 'H' };
    hFile.WriteBytes( magic, 4 );

    // Header
    hFile.WriteBytes( (const Byte*)( &(m_hMeshHeader.dwDataSize) ), sizeof(DWord) );
    
    hFile.WriteBytes( (const Byte*)( &(m_hMeshHeader.dwVFFieldCount) ), sizeof(DWord) );

    for( UInt i = 0; i < m_hMeshHeader.dwVFFieldCount; ++i ) {
        hFile.WriteBytes( (const Byte*)( &(m_hMeshHeader.arrVFFields[i].dwType) ), sizeof(DWord) );
        hFile.WriteBytes( (const Byte*)( &(m_hMeshHeader.arrVFFields[i].dwUsage) ), sizeof(DWord) );
        hFile.WriteBytes( (const Byte*)( &(m_hMeshHeader.arrVFFields[i].dwUsageIndex) ), sizeof(DWord) );
    }
    for( UInt i = m_hMeshHeader.dwVFFieldCount; i < 64; ++i ) {
        DWord arrDummy[3] = { 0, 0, 0 };
        hFile.WriteBytes( (const Byte*)arrDummy, 3 * sizeof(DWord) );
    }

    hFile.WriteBytes( (const Byte*)( &(m_hMeshHeader.dwVertexSize) ), sizeof(DWord) );
    hFile.WriteBytes( (const Byte*)( &(m_hMeshHeader.dwVertexCount) ), sizeof(DWord) );
    hFile.WriteBytes( (const Byte*)( &(m_hMeshHeader.dwVertexOffset) ), sizeof(DWord) );

    hFile.WriteBytes( (const Byte*)( &(m_hMeshHeader.dwIndexSize) ), sizeof(DWord) );
    hFile.WriteBytes( (const Byte*)( &(m_hMeshHeader.dwIndexCount) ), sizeof(DWord) );
    hFile.WriteBytes( (const Byte*)( &(m_hMeshHeader.dwIndexOffset) ), sizeof(DWord) );

    hFile.WriteBytes( (const Byte*)( &(m_hMeshHeader.dwMeshType) ), sizeof(DWord) );

    // Content
    hFile.WriteBytes( m_pData, m_hMeshHeader.dwDataSize );

    // Close file
    hFile.Close();

    return true;
}

