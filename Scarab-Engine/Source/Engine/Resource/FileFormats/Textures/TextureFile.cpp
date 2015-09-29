/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Resource/FileFormats/Textures/TextureFile.cpp
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
// Includes
#include "TextureFile.h"

#include "../../ResourceManager.h"

/////////////////////////////////////////////////////////////////////////////////
// TextureFile implementation
TextureFile::TextureFile():
    ResourceFile()
{
    MemZero( &m_hTextureHeader, sizeof(TextureHeader) );
}
TextureFile::~TextureFile()
{
    // nothing to do
}

const GChar * TextureFile::GetTypeName( TextureFileType iFileType )
{
    switch( iFileType ) {
        case TEXTUREFILE_TEX: return TEXT("tex");
        case TEXTUREFILE_BMP: return TEXT("bmp");
        default: Assert(false); return NULL;
    }
}
TextureFileType TextureFile::GetType( const GChar * strFileTypeName )
{
    // Simple search
    TextureFileType iFileType;
    for( UInt i = 0; i < TEXTUREFILE_COUNT; ++i ) {
        iFileType = (TextureFileType)i;
        if ( StringFn->Cmp(GetTypeName(iFileType), strFileTypeName) == 0 )
            return iFileType;
    }

    // Not found
    return TEXTUREFILE_UNDEFINED;
}

/////////////////////////////////////////////////////////////////////////////////

Bool TextureFile::_OnExport( ResourceDB * pRDB, RDB_ResourceHandle * hResource )
{
    if ( m_pData != NULL )
        return false;

    pRDB->ResourceRead( hResource, sizeof(TextureHeader), (Byte*)(&m_hTextureHeader) );

    _CreateData( (ResourceFileHeader*)&m_hTextureHeader );
    pRDB->ResourceRead( hResource, m_hTextureHeader.dwDataSize, m_pData );

    return true;
}
Bool TextureFile::_OnImport( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) const
{
    if ( m_pData == NULL )
        return false;

    pRDB->ResourceAppend( hResource, sizeof(TextureHeader), (const Byte*)(&m_hTextureHeader) );

    pRDB->ResourceAppend( hResource, m_hTextureHeader.dwDataSize, m_pData );

    return true;
}

