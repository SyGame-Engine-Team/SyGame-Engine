/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/2D/Resources/GPU2DBitmap.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU 2D-Resource : Bitmaps
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
#include "GPU2DBitmap.h"

#include "../../RenderingManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GPU2DBitmap implementation
GPU2DBitmap::GPU2DBitmap( GPU2DContext * pContext2D, PixelFormat iFormat, UInt iWidth, UInt iHeight, const Byte * arrBitmap, Bool bCreateCPUData ):
    m_hDevice2DBitmap( &(pContext2D->m_hDevice2DContext) )
{
    m_pContext2D = pContext2D;

    Assert( iFormat != PIXEL_FMT_UNKNOWN && iFormat < PIXEL_FMT_COUNT );

    m_iFormat = iFormat;
    m_iAlphaMode = GPU2DBITMAP_ALPHAMODE_IGNORE;
    m_iStride = PixelFormatBytes( m_iFormat );

    m_iWidth = iWidth;
    m_iHeight = iHeight;
    m_iPitch = ( m_iWidth * m_iStride );

    m_fDpiX = 96.0f;
    m_fDpiY = 96.0f;

    m_iSize = ( m_iHeight * m_iPitch );
    m_arrBitmap = NULL;
    if ( bCreateCPUData ) {
        m_arrBitmap = New Byte[m_iSize];

        if ( arrBitmap != NULL )
            MemCopy( m_arrBitmap, arrBitmap, m_iSize );
    }
}
GPU2DBitmap::~GPU2DBitmap()
{
    if ( IsBound() )
        UnBind();

    if ( m_arrBitmap != NULL )
        DeleteA( m_arrBitmap );
}

Void GPU2DBitmap::Bind()
{
    Assert( !(IsBound()) );

    GPU2DBitmapDesc hDesc;
    hDesc.iFormat = m_iFormat;
    hDesc.iAlphaMode = m_iAlphaMode;
    hDesc.fDpiX = m_fDpiX;
    hDesc.fDpiY = m_fDpiY;
    hDesc.iWidth = m_iWidth;
    hDesc.iHeight = m_iHeight;
    hDesc.fDIPWidth = 0.0f;
    hDesc.fDIPHeight = 0.0f;

    m_hDevice2DBitmap.Create( (const Device2DBitmapDesc *)&hDesc, m_arrBitmap, m_iPitch );

    m_pContext2D->_RegisterBitmap( this );
}
Void GPU2DBitmap::UnBind()
{
    Assert( IsBound() );

    m_pContext2D->_UnRegisterBitmap( this );

    m_hDevice2DBitmap.Destroy();
}

Void GPU2DBitmap::DestroyCPUData()
{
    Assert( HasCPUData() );

    m_hDevice2DBitmap.UpdateDataSource( NULL );

    RenderingFn->SelectMemory( TEXT("Bitmaps") );
    DeleteA( m_arrBitmap );
    m_arrBitmap = NULL;
    RenderingFn->UnSelectMemory();
}
Void GPU2DBitmap::RestoreCPUData()
{
    Assert( !(HasCPUData()) );

    RenderingFn->SelectMemory( TEXT("Bitmaps") );
    m_arrBitmap = New Byte[m_iSize];
    RenderingFn->UnSelectMemory();

    m_hDevice2DBitmap.UpdateDataSource( m_arrBitmap );
}

Void GPU2DBitmap::SetPixels( const Byte * arrPixels, UInt iPixelsPitch, const GPU2DRectangleI * pRect )
{
    Assert( HasCPUData() );

    UInt iLeft = 0;
    UInt iTop = 0;
    if ( pRect != NULL ) {
        Assert( pRect->iLeft < m_iWidth );
        Assert( pRect->iTop < m_iHeight );
        iLeft = pRect->iLeft;
        iTop = pRect->iTop;
    }

    UInt iWidth = m_iWidth;
    UInt iHeight = m_iHeight;
    if ( pRect != NULL ) {
        Assert( pRect->iLeft < pRect->iRight && pRect->iRight <= m_iWidth );
        Assert( pRect->iTop < pRect->iBottom && pRect->iBottom <= m_iHeight );
        iWidth = ( pRect->iRight - iLeft );
        iHeight = ( pRect->iBottom - iTop );
    }

    for( ; iTop < m_iHeight; ++iTop ) {
        MemCopy( m_arrBitmap + (iTop * m_iPitch) + (iLeft * m_iStride), arrPixels, iWidth * m_iStride );
        arrPixels += iPixelsPitch;
    }
}

/////////////////////////////////////////////////////////////////////////////////

Bool GPU2DBitmap::_OnLoad( ResourceDB * pRDB, RDB_ResourceHandle * hResource )
{
    Assert( !IsBound() );

    BitmapHeader hBitmapHeader;
    pRDB->ResourceRead( hResource, sizeof(BitmapHeader), (Byte*)( &hBitmapHeader ) );

    m_iFormat = (PixelFormat)( hBitmapHeader.dwFormat );
    m_iAlphaMode = (GPU2DBitmapAlphaMode)( hBitmapHeader.dwAlphaMode );
    m_iStride = PixelFormatBytes( m_iFormat );

    m_iWidth = hBitmapHeader.dwWidth;
    m_iHeight = hBitmapHeader.dwHeight;
    m_iPitch = ( m_iWidth * m_iStride );

    m_fDpiX = hBitmapHeader.fDpiX;
    m_fDpiY = hBitmapHeader.fDpiY;

    m_iSize = ( m_iHeight * m_iPitch );
    Assert( m_iSize == hBitmapHeader.dwDataSize );

    RenderingFn->SelectMemory( TEXT("Bitmaps") );

    if ( m_arrBitmap != NULL ) {
        DeleteA( m_arrBitmap );
        m_arrBitmap = NULL;
    }
    m_arrBitmap = New Byte[m_iSize];

    RenderingFn->UnSelectMemory();

    pRDB->ResourceRead( hResource, m_iSize, m_arrBitmap );

    return true;
}
Bool GPU2DBitmap::_OnSave( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) const
{
    Assert( HasCPUData() );

    BitmapHeader hBitmapHeader;
    hBitmapHeader.dwDataSize = m_iSize;
    hBitmapHeader.dwFormat = m_iFormat;
    hBitmapHeader.dwAlphaMode = m_iAlphaMode;
    hBitmapHeader.dwWidth = m_iWidth;
    hBitmapHeader.dwHeight = m_iHeight;
    hBitmapHeader.fDpiX = m_fDpiX;
    hBitmapHeader.fDpiY = m_fDpiY;

    pRDB->ResourceAppend( hResource, sizeof(BitmapHeader), (const Byte *)( &hBitmapHeader ) );

    pRDB->ResourceAppend( hResource, m_iSize, m_arrBitmap );

    return true;
}

