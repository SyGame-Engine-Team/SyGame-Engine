/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Resource/FileFormats/Textures/TextureFile.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : File formats : Textures
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
#ifndef SCARAB_ENGINE_RESOURCE_FILEFORMATS_TEXTURES_TEXTUREFILE_H
#define SCARAB_ENGINE_RESOURCE_FILEFORMATS_TEXTURES_TEXTUREFILE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../ResourceFile.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
enum TextureFileType
{
    TEXTUREFILE_UNDEFINED = INVALID_OFFSET,

	TEXTUREFILE_TEX = 0, // Native format

    // Image formats (2D only, no mipmap)
	TEXTUREFILE_BMP,   // the most simple one, uncompressed
	//TEXTUREFILE_TGA, // the most texture-friendly one
	//TEXTUREFILE_JPG, // the most known & compression-efficient one
    //TEXTUREFILE_PNG, // the most portable one
	//TEXTUREFILE_GIF  // animation capabilities, web, ...

    // Texture formats
    //////////////////////////////

    TEXTUREFILE_COUNT
};

// Texture header
typedef struct _texture_header
{
    DWord dwDataSize;

    DWord dwType;
    DWord dwFormat;
    DWord dwSampleCount;

    DWord dwMipLevelCount;
    DWord dwBounds[3];
} TextureHeader;

/////////////////////////////////////////////////////////////////////////////////
// The TextureFile interface
class TextureFile : public ResourceFile
{
public:
	TextureFile();
	virtual ~TextureFile();

    inline ResourceType GetResourceType() const;

    static const GChar * GetTypeName( TextureFileType iFileType );
    static TextureFileType GetType( const GChar * strFileTypeName );

	virtual TextureFileType GetType() const = 0;

	virtual Bool Load( const GChar * strFilename ) = 0;
	virtual Bool Save( const GChar * strFilename ) const = 0;

protected:
    virtual Bool _OnExport( ResourceDB * pRDB, RDB_ResourceHandle * hResource );
    virtual Bool _OnImport( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) const;

    TextureHeader m_hTextureHeader;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "TextureFile.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RESOURCE_FILEFORMATS_TEXTURES_TEXTUREFILE_H

