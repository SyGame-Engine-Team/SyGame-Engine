/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Resource/FileFormats/Bitmaps/BitmapBMAP.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Bitmap formats : BMAP (native)
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
#include "BitmapBMAP.h"

#include "../../ResourceManager.h"

/////////////////////////////////////////////////////////////////////////////////
// BitmapBMAP implementation
BitmapBMAP::BitmapBMAP():
    BitmapFile()
{
    // nothing to do
}
BitmapBMAP::~BitmapBMAP()
{
    // nothing to do
}

Bool BitmapBMAP::Load( const GChar * strFilename )
{
    Assert( m_pData == NULL );

    // Open file
    HFile hFile = SystemFn->OpenFile( strFilename, FILE_READ );
    if ( !hFile.IsValid() )
        return false;

    // Magic word
    Byte magic[4];
    hFile.ReadBytes( magic, 4 );
    Assert( magic[0] == 'B' );
    Assert( magic[1] == 'M' );
    Assert( magic[2] == 'A' );
    Assert( magic[3] == 'P' );

    // Header
    hFile.ReadBytes( (Byte*)( &(m_hBitmapHeader.dwDataSize) ), sizeof(DWord) );

    hFile.ReadBytes( (Byte*)( &(m_hBitmapHeader.dwFormat) ), sizeof(DWord) );
    hFile.ReadBytes( (Byte*)( &(m_hBitmapHeader.dwAlphaMode) ), sizeof(DWord) );

    hFile.ReadBytes( (Byte*)( &(m_hBitmapHeader.dwWidth) ), sizeof(DWord) );
    hFile.ReadBytes( (Byte*)( &(m_hBitmapHeader.dwHeight) ), sizeof(DWord) );

    hFile.ReadBytes( (Byte*)( &(m_hBitmapHeader.fDpiX) ), sizeof(Float) );
    hFile.ReadBytes( (Byte*)( &(m_hBitmapHeader.fDpiY) ), sizeof(Float) );

    // Content
    _CreateData( (ResourceFileHeader *)&m_hBitmapHeader );
    hFile.ReadBytes( m_pData, m_hBitmapHeader.dwDataSize );

    // Close file
    hFile.Close();

    return true;
}
Bool BitmapBMAP::Save( const GChar * strFilename ) const
{
    Assert( m_pData != NULL );

    // Create file
    HFile hFile = SystemFn->CreateFile( strFilename, FILE_WRITE );
    if ( !hFile.IsValid() )
        return false;

    // Magic word
    Byte magic[4] = { 'B', 'M', 'A', 'P' };
    hFile.WriteBytes( magic, 4 );

    // Header
    hFile.WriteBytes( (const Byte*)( &(m_hBitmapHeader.dwDataSize) ), sizeof(DWord) );

    hFile.WriteBytes( (const Byte*)( &(m_hBitmapHeader.dwFormat) ), sizeof(DWord) );
    hFile.WriteBytes( (const Byte*)( &(m_hBitmapHeader.dwAlphaMode) ), sizeof(DWord) );

    hFile.WriteBytes( (const Byte*)( &(m_hBitmapHeader.dwWidth) ), sizeof(DWord) );
    hFile.WriteBytes( (const Byte*)( &(m_hBitmapHeader.dwHeight) ), sizeof(DWord) );

    hFile.WriteBytes( (const Byte*)( &(m_hBitmapHeader.fDpiX) ), sizeof(Float) );
    hFile.WriteBytes( (const Byte*)( &(m_hBitmapHeader.fDpiY) ), sizeof(Float) );

    // Content
    hFile.WriteBytes( m_pData, m_hBitmapHeader.dwDataSize );

    // Close file
    hFile.Close();

    return true;
}

