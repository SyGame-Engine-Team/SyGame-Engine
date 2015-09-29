/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Resource/FileFormats/Bitmaps/BitmapFile.cpp
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
// Includes
#include "BitmapFile.h"

#include "../../ResourceManager.h"

/////////////////////////////////////////////////////////////////////////////////
// BitmapFile implementation
BitmapFile::BitmapFile():
    ResourceFile()
{
    MemZero( &m_hBitmapHeader, sizeof(BitmapHeader) );
}
BitmapFile::~BitmapFile()
{
    // nothing to do
}

const GChar * BitmapFile::GetTypeName( BitmapFileType iFileType )
{
    switch( iFileType ) {
        case BITMAPFILE_BMAP: return TEXT("bmap");
        case BITMAPFILE_BMP:  return TEXT("bmp");
        default: Assert(false); return NULL;
    }
}
BitmapFileType BitmapFile::GetType( const GChar * strFileTypeName )
{
    // Simple search
    BitmapFileType iFileType;
    for( UInt i = 0; i < BITMAPFILE_COUNT; ++i ) {
        iFileType = (BitmapFileType)i;
        if ( StringFn->Cmp(GetTypeName(iFileType), strFileTypeName) == 0 )
            return iFileType;
    }

    // Not found
    return BITMAPFILE_UNDEFINED;
}

/////////////////////////////////////////////////////////////////////////////////

Bool BitmapFile::_OnExport( ResourceDB * pRDB, RDB_ResourceHandle * hResource )
{
    if ( m_pData != NULL )
        return false;

    pRDB->ResourceRead( hResource, sizeof(BitmapHeader), (Byte*)(&m_hBitmapHeader) );

    _CreateData( (ResourceFileHeader*)&m_hBitmapHeader );
    pRDB->ResourceRead( hResource, m_hBitmapHeader.dwDataSize, m_pData );

    return true;
}
Bool BitmapFile::_OnImport( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) const
{
    if ( m_pData == NULL )
        return false;

    pRDB->ResourceAppend( hResource, sizeof(BitmapHeader), (const Byte*)(&m_hBitmapHeader) );

    pRDB->ResourceAppend( hResource, m_hBitmapHeader.dwDataSize, m_pData );

    return true;
}

