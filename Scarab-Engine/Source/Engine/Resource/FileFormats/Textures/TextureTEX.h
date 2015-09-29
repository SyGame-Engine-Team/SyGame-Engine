/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Resource/FileFormats/Textures/TextureTEX.h
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
// Header prelude
#ifndef SCARAB_ENGINE_RESOURCE_FILEFORMATS_TEXTURES_TEXTURETEX_H
#define SCARAB_ENGINE_RESOURCE_FILEFORMATS_TEXTURES_TEXTURETEX_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "TextureFile.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TextureTEX class
class TextureTEX : public TextureFile
{
public:
	TextureTEX();
	virtual ~TextureTEX();

	inline virtual TextureFileType GetType() const;

    virtual Bool Load( const GChar * strFilename );
	virtual Bool Save( const GChar * strFilename ) const;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "TextureTEX.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RESOURCE_FILEFORMATS_TEXTURES_TEXTURETEX_H

