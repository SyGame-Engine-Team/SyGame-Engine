/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Resource/FileFormats/Textures/TextureBMP.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Texture formats : BMP (Win32 bitmap files)
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
#include "TextureBMP.h"

#include "../../ResourceManager.h"

/////////////////////////////////////////////////////////////////////////////////
// TextureBMP implementation
TextureBMP::TextureBMP():
	TextureFile()
{
	MemZero( &m_bmpFileHeader, sizeof(_BMP_FileHeader) );
	MemZero( &m_bmpInfoHeader, sizeof(_BMP_InfoHeader) );
}
TextureBMP::~TextureBMP()
{
    // nothing to do
}

Bool TextureBMP::Load( const GChar * strFilename )
{
    Assert( m_pData == NULL );

    // Open file
    HFile hFile = SystemFn->OpenFile( strFilename, FILE_READ );
    if( !hFile.IsValid() )
        return false;

    // Header
	hFile.ReadBytes( (Byte*)( &(m_bmpFileHeader.Magic) ), sizeof(Word) );
	hFile.ReadBytes( (Byte*)( &(m_bmpFileHeader.Size) ), sizeof(DWord) );
	hFile.ReadBytes( (Byte*)( &(m_bmpFileHeader.Reserved) ), sizeof(DWord) );
	hFile.ReadBytes( (Byte*)( &(m_bmpFileHeader.OffBits) ), sizeof(DWord) );
    Assert( m_bmpFileHeader.Magic == _BMP_MAGIC );
    Assert( m_bmpFileHeader.OffBits == _BMP_HEADERS_SIZE );

	hFile.ReadBytes( (Byte*)( &(m_bmpInfoHeader.ThisSize) ), sizeof(DWord) );
	hFile.ReadBytes( (Byte*)( &(m_bmpInfoHeader.Width) ), sizeof(Long) );
	hFile.ReadBytes( (Byte*)( &(m_bmpInfoHeader.Height) ), sizeof(Long) );
	hFile.ReadBytes( (Byte*)( &(m_bmpInfoHeader.Planes) ), sizeof(Word) );
	hFile.ReadBytes( (Byte*)( &(m_bmpInfoHeader.Bpp) ), sizeof(Word) );
	hFile.ReadBytes( (Byte*)( &(m_bmpInfoHeader.Compression) ), sizeof(DWord) );
	hFile.ReadBytes( (Byte*)( &(m_bmpInfoHeader.ImageSize) ), sizeof(DWord) );
	hFile.ReadBytes( (Byte*)( &(m_bmpInfoHeader.XPixelsByMeter) ), sizeof(Long) );
	hFile.ReadBytes( (Byte*)( &(m_bmpInfoHeader.YPixelsByMeter) ), sizeof(Long) );
	hFile.ReadBytes( (Byte*)( &(m_bmpInfoHeader.PaletteUse) ), sizeof(DWord) );
	hFile.ReadBytes( (Byte*)( &(m_bmpInfoHeader.PaletteImportant) ), sizeof(DWord) );
    Assert( m_bmpInfoHeader.ThisSize == _BMP_INFOHEADER_SIZE );
    Assert( m_bmpInfoHeader.Width > 0 );
    Assert( m_bmpInfoHeader.Height > 0 );
    Assert( m_bmpInfoHeader.Planes == 1 );
    Assert( m_bmpInfoHeader.Compression == _BMP_RGB );

    m_hTextureHeader.dwDataSize = ( m_bmpInfoHeader.Width * m_bmpInfoHeader.Height * PixelFormatBytes(PIXEL_FMT_RGBA8UN) );
    m_hTextureHeader.dwType = 6;
    m_hTextureHeader.dwFormat = (DWord)PIXEL_FMT_RGBA8UN;
    m_hTextureHeader.dwSampleCount = 1;
    m_hTextureHeader.dwMipLevelCount = 1;
    m_hTextureHeader.dwBounds[0] = m_bmpInfoHeader.Width;
    m_hTextureHeader.dwBounds[1] = m_bmpInfoHeader.Height;
    m_hTextureHeader.dwBounds[2] = 0;

    // Content
    _CreateData( (ResourceFileHeader *)&m_hTextureHeader );
    _Load_RGB( hFile );

    // Close file
    hFile.Close();

    return true;
}
Bool TextureBMP::Save( const GChar * strFilename ) const
{
    Assert( m_pData != NULL );
    Assert( m_hTextureHeader.dwType == 6 );
    Assert( m_hTextureHeader.dwFormat == PIXEL_FMT_RGBA8UN );
    Assert( m_hTextureHeader.dwMipLevelCount == 1 );

    // Create file
    HFile hFile = SystemFn->CreateFile( strFilename, FILE_WRITE );
    if( !hFile.IsValid() )
        return false;

    // Header
    m_bmpFileHeader.Magic = _BMP_MAGIC;
	m_bmpFileHeader.Size = _BMP_HEADERS_SIZE + m_hTextureHeader.dwDataSize;
	m_bmpFileHeader.Reserved = 0;
	m_bmpFileHeader.OffBits = _BMP_HEADERS_SIZE;
	hFile.WriteBytes( (const Byte*)( &(m_bmpFileHeader.Magic) ), sizeof(Word) );
	hFile.WriteBytes( (const Byte*)( &(m_bmpFileHeader.Size) ), sizeof(DWord) );
	hFile.WriteBytes( (const Byte*)( &(m_bmpFileHeader.Reserved) ), sizeof(DWord) );
	hFile.WriteBytes( (const Byte*)( &(m_bmpFileHeader.OffBits) ), sizeof(DWord) );

    m_bmpInfoHeader.ThisSize = _BMP_INFOHEADER_SIZE;
	m_bmpInfoHeader.Width = m_hTextureHeader.dwBounds[0];
	m_bmpInfoHeader.Height = m_hTextureHeader.dwBounds[1];
	m_bmpInfoHeader.Planes = 1;
	m_bmpInfoHeader.Bpp = 32;
	m_bmpInfoHeader.Compression = _BMP_RGB;
    m_bmpInfoHeader.ImageSize = m_hTextureHeader.dwDataSize;
	m_bmpInfoHeader.XPixelsByMeter = 0;
	m_bmpInfoHeader.YPixelsByMeter = 0;
	m_bmpInfoHeader.PaletteUse = 0;
	m_bmpInfoHeader.PaletteImportant = 0;
	hFile.WriteBytes( (const Byte*)( &(m_bmpInfoHeader.ThisSize) ), sizeof(DWord) );
	hFile.WriteBytes( (const Byte*)( &(m_bmpInfoHeader.Width) ), sizeof(Long) );
	hFile.WriteBytes( (const Byte*)( &(m_bmpInfoHeader.Height) ), sizeof(Long) );
	hFile.WriteBytes( (const Byte*)( &(m_bmpInfoHeader.Planes) ), sizeof(Word) );
	hFile.WriteBytes( (const Byte*)( &(m_bmpInfoHeader.Bpp) ), sizeof(Word) );
	hFile.WriteBytes( (const Byte*)( &(m_bmpInfoHeader.Compression) ), sizeof(DWord) );
	hFile.WriteBytes( (const Byte*)( &(m_bmpInfoHeader.ImageSize) ), sizeof(DWord) );
	hFile.WriteBytes( (const Byte*)( &(m_bmpInfoHeader.XPixelsByMeter) ), sizeof(Long) );
	hFile.WriteBytes( (const Byte*)( &(m_bmpInfoHeader.YPixelsByMeter) ), sizeof(Long) );
	hFile.WriteBytes( (const Byte*)( &(m_bmpInfoHeader.PaletteUse) ), sizeof(DWord) );
	hFile.WriteBytes( (const Byte*)( &(m_bmpInfoHeader.PaletteImportant) ), sizeof(DWord) );

    // Content
    _Save_RGB( hFile );

    // Close file
    hFile.Close();

    return true;
}

/////////////////////////////////////////////////////////////////////////////////

Void TextureBMP::_Load_RGB( const HFile & hFile )
{
    UInt iWidth = m_hTextureHeader.dwBounds[0];
    UInt iHeight = m_hTextureHeader.dwBounds[1];

    PixelFormat iSrcFormat;
    switch( m_bmpInfoHeader.Bpp ) {
        case 16: iSrcFormat = PIXEL_FMT_BGR5A1UN; break;
        case 24: iSrcFormat = PIXEL_FMT_BGRX8UN;  break;
        case 32: iSrcFormat = PIXEL_FMT_BGRA8UN; break;
        default: Assert(false); break;
    }
    UInt iSrcStride = ( m_bmpInfoHeader.Bpp >> 3 );

    PixelFormat iDstFormat = PIXEL_FMT_RGBA8UN;
    UInt iDstStride =  PixelFormatBytes( iDstFormat );
    UInt iDstPitch = ( iWidth * iDstStride );

    // Invert scanlines and convert format "on-the-fly"
    Byte * pScanLine = m_pData + ( iDstPitch * (iHeight - 1) );
    Byte * pPixel;
    Byte arrTmp[4];
    Float fColor[4];

    while( pScanLine >= m_pData ) {
        pPixel = pScanLine;
        for( UInt i = 0; i < iWidth; ++i ) {
            hFile.ReadBytes( arrTmp, iSrcStride );
            PixelFormatConvertFrom( fColor, arrTmp, iSrcFormat, 1 );
            PixelFormatConvertTo( pPixel, fColor, PIXEL_FMT_RGBA8UN, 1 );
            pPixel += iDstStride;
        }
        pScanLine -= iDstPitch;
    }
}
Void TextureBMP::_Save_RGB( const HFile & hFile ) const
{
    UInt iWidth = m_hTextureHeader.dwBounds[0];
    UInt iHeight = m_hTextureHeader.dwBounds[1];

    PixelFormat iSrcFormat = PIXEL_FMT_RGBA8UN;
    UInt iSrcStride = PixelFormatBytes( iSrcFormat );
    UInt iSrcPitch = ( iWidth * iSrcStride );

    PixelFormat iDstFormat = PIXEL_FMT_BGRA8UN;
    UInt iDstStride =  PixelFormatBytes( iDstFormat );

    // Invert scanlines and convert format "on-the-fly"
    const Byte * pScanLine = m_pData + ( iSrcPitch * (iHeight - 1) );
    const Byte * pPixel;
    Byte arrTmp[4];
    Float fColor[4];

    while( pScanLine >= m_pData ) {
        pPixel = pScanLine;
        for( UInt i = 0; i < iWidth; ++i ) {
            PixelFormatConvertFrom( fColor, pPixel, iSrcFormat, 1 );
            PixelFormatConvertTo( arrTmp, fColor, iDstFormat, 1 );
            hFile.WriteBytes( arrTmp, iDstStride );
            pPixel += iSrcStride;
        }
        pScanLine -= iSrcPitch;
    }
}

