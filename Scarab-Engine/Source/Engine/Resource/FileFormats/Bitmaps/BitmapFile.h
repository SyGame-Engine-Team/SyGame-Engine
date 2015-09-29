/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Resource/FileFormats/Bitmaps/BitmapFile.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : File formats : Bitmaps
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
#ifndef SCARAB_ENGINE_RESOURCE_FILEFORMATS_BITMAPS_BITMAPFILE_H
#define SCARAB_ENGINE_RESOURCE_FILEFORMATS_BITMAPS_BITMAPFILE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../ResourceFile.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
enum BitmapFileType
{
    BITMAPFILE_UNDEFINED = INVALID_OFFSET,

	BITMAPFILE_BMAP = 0, // Native format

    // Bitmap formats
    BITMAPFILE_BMP, // Microsoft Standard BMP format

    BITMAPFILE_COUNT
};

// Bitmap header
typedef struct _bitmap_header
{
    DWord dwDataSize;

    DWord dwFormat;
    DWord dwAlphaMode;
    DWord dwWidth;
    DWord dwHeight;
    Float fDpiX;
    Float fDpiY;
} BitmapHeader;

/////////////////////////////////////////////////////////////////////////////////
// The BitmapFile interface
class BitmapFile : public ResourceFile
{
public:
	BitmapFile();
	virtual ~BitmapFile();

    inline ResourceType GetResourceType() const;

    static const GChar * GetTypeName( BitmapFileType iFileType );
    static BitmapFileType GetType( const GChar * strFileTypeName );

	virtual BitmapFileType GetType() const = 0;

	virtual Bool Load( const GChar * strFilename ) = 0;
	virtual Bool Save( const GChar * strFilename ) const = 0;

protected:
    virtual Bool _OnExport( ResourceDB * pRDB, RDB_ResourceHandle * hResource );
    virtual Bool _OnImport( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) const;

    BitmapHeader m_hBitmapHeader;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "BitmapFile.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RESOURCE_FILEFORMATS_BITMAPS_BITMAPFILE_H

