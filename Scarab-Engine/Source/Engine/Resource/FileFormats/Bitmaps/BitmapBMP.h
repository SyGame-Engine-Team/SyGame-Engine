/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Resource/FileFormats/Bitmaps/BitmapBMP.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Bitmap formats : BMP (MS Win32 bitmap files)
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : Monochrome, 4 & 8 bits formats no more supported ... (no use)
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_RESOURCE_FILEFORMATS_BITMAPS_BITMAPBMP_H
#define SCARAB_ENGINE_RESOURCE_FILEFORMATS_BITMAPS_BITMAPBMP_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "BitmapFile.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#ifndef _FILEFORMAT_BMP_DEFINITIONS
#define _FILEFORMAT_BMP_DEFINITIONS
    #define _BMP_MAGIC           0x4d42 // "BM"
    #define _BMP_FILEHEADER_SIZE 14
    #define _BMP_INFOHEADER_SIZE 40
    #define _BMP_HEADERS_SIZE    54	// hard-coded, don't use sizeof()
    #define _BMP_PALETTE_MAXSIZE 256
    #define _BMP_RGB             0
    #define _BMP_RLE8            1
    #define _BMP_RLE4            2
    #define _BMP_BITFIELD        3

    typedef struct _bitmap_fileheader
    {
        Word Magic;     // Bitmap File Signature
        DWord Size;     // FileSize in bytes
        DWord Reserved; // must be 0
        DWord OffBits;  // offset to pixel data (from the file's begin), unused
    } _BMP_FileHeader;
    typedef struct _bitmap_infoheader
    {
	    DWord ThisSize;         // == sizeof(_BMP_InfoHeader)
        Long Width;             // in pixels
        Long Height;            // in pixels
        Word Planes;            // must be 1
        Word Bpp;               // bit-depth
        DWord Compression;      // RGB, BitField or Run-Lenght-Encoded
        DWord ImageSize;        // image size in bytes, must be rounded up to DWord bound,
                                // if no Compression 0 is a valid value, unused
        Long XPixelsByMeter;    // Horiz resolution
        Long YPixelsByMeter;    // Vert resolution
        DWord PaletteUse;       // colors actually used by the bitmap, if 0 uses max number from Bpp,
                                // else if Bpp < 24, valid value else (Bpp == 24) size of reference col palette
        DWord PaletteImportant; // number of important color indexes, if 0 all are important
    } _BMP_InfoHeader;
    typedef struct _bitmap_rgbquad
    {
        Byte Blue;
        Byte Green;
        Byte Red;
        Byte Alpha;
    } _BMP_RGBQuad;
    typedef struct _bitmap_palette
    {
	    UInt Size;
	    _BMP_RGBQuad Colors[_BMP_PALETTE_MAXSIZE];
    } _BMP_Palette;
#endif // _FILEFORMAT_BMP_DEFINITIONS

/////////////////////////////////////////////////////////////////////////////////
// The BitmapBMP class
class BitmapBMP : public BitmapFile
{
public:
	BitmapBMP();
	virtual ~BitmapBMP();

	inline virtual BitmapFileType GetType() const;

    virtual Bool Load( const GChar * strFilename );
	virtual Bool Save( const GChar * strFilename ) const;

private:
	Void _Load_RGB( const HFile & hFile );
	Void _Save_RGB( const HFile & hFile ) const;

	mutable _BMP_FileHeader m_bmpFileHeader;
	mutable _BMP_InfoHeader m_bmpInfoHeader;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "BitmapBMP.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RESOURCE_FILEFORMATS_BITMAPS_BITMAPBMP_H

