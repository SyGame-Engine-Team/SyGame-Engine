/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Resource/FileFormats/Textures/TextureTEX.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Texture formats : TEX (native)
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
#include "TextureTEX.h"

#include "../../ResourceManager.h"

/////////////////////////////////////////////////////////////////////////////////
// TextureTEX implementation
TextureTEX::TextureTEX():
    TextureFile()
{
    // nothing to do
}
TextureTEX::~TextureTEX()
{
    // nothing to do
}

Bool TextureTEX::Load( const GChar * strFilename )
{
    Assert( m_pData == NULL );

    // Open file
    HFile hFile = SystemFn->OpenFile( strFilename, FILE_READ );
    if ( !hFile.IsValid() )
        return false;

    // Magic word
    Byte magic[4];
    hFile.ReadBytes( magic, 4 );
    Assert( magic[0] == 'T' );
    Assert( magic[1] == 'E' );
    Assert( magic[2] == 'X' );
    Assert( magic[3] == '_' );

    // Header
    hFile.ReadBytes( (Byte*)( &(m_hTextureHeader.dwDataSize) ), sizeof(DWord) );

    hFile.ReadBytes( (Byte*)( &(m_hTextureHeader.dwType) ), sizeof(DWord) );
    hFile.ReadBytes( (Byte*)( &(m_hTextureHeader.dwFormat) ), sizeof(DWord) );
    hFile.ReadBytes( (Byte*)( &(m_hTextureHeader.dwSampleCount) ), sizeof(DWord) );

    hFile.ReadBytes( (Byte*)( &(m_hTextureHeader.dwMipLevelCount) ), sizeof(DWord) );
    hFile.ReadBytes( (Byte*)( m_hTextureHeader.dwBounds ), 3 * sizeof(DWord) );

    // Content
    _CreateData( (ResourceFileHeader *)&m_hTextureHeader );
    hFile.ReadBytes( m_pData, m_hTextureHeader.dwDataSize );

    // Close file
    hFile.Close();

    return true;
}
Bool TextureTEX::Save( const GChar * strFilename ) const
{
    Assert( m_pData != NULL );

    // Create file
    HFile hFile = SystemFn->CreateFile( strFilename, FILE_WRITE );
    if ( !hFile.IsValid() )
        return false;

    // Magic word
    Byte magic[4] = { 'T', 'E', 'X', '_' };
    hFile.WriteBytes( magic, 4 );

    // Header
    hFile.WriteBytes( (const Byte*)( &(m_hTextureHeader.dwDataSize) ), sizeof(DWord) );

    hFile.WriteBytes( (const Byte*)( &(m_hTextureHeader.dwType) ), sizeof(DWord) );
    hFile.WriteBytes( (const Byte*)( &(m_hTextureHeader.dwFormat) ), sizeof(DWord) );
    hFile.WriteBytes( (const Byte*)( &(m_hTextureHeader.dwSampleCount) ), sizeof(DWord) );

    hFile.WriteBytes( (const Byte*)( &(m_hTextureHeader.dwMipLevelCount) ), sizeof(DWord) );
    hFile.WriteBytes( (const Byte*)( m_hTextureHeader.dwBounds ), 3 * sizeof(DWord) );

    // Content
    hFile.WriteBytes( m_pData, m_hTextureHeader.dwDataSize );

    // Close file
    hFile.Close();

    return true;
}

