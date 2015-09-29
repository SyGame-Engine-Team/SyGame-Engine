/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/2D/Resources/D2D1Bitmap.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : D2D1 Dev-Dep Resource : Bitmaps.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Third-Party Includes
#pragma warning(disable:4005)

#define WIN32_LEAN_AND_MEAN
#include <d2d1.h>

#undef DebugAssert

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "D2D1Bitmap.h"

#include "../D2D1RenderingContext.h"

/////////////////////////////////////////////////////////////////////////////////
// D2D1Bitmaps implementation
D2D1Bitmap::D2D1Bitmap( D2D1RenderingContext * pContext2D )
{
    m_pContext2D = pContext2D;

    m_hBitmapProperties.pixelFormat.format = DXGI_FORMAT_UNKNOWN;
    m_hBitmapProperties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_UNKNOWN;
    m_hBitmapProperties.dpiX = 0.0f;
    m_hBitmapProperties.dpiY = 0.0f;
    m_hSize.width = 0;
    m_hSize.height = 0;
    m_hDIPSize.width = 0.0f;
    m_hDIPSize.height = 0.0f;

    m_pBitmap = NULL;

    m_bTemporaryDestroyed = false;
    m_hCreationParameters.pData = NULL;
    m_hCreationParameters.iDataPitch = 0;
}
D2D1Bitmap::~D2D1Bitmap()
{
    if ( IsCreated() )
        Destroy();
}

Void D2D1Bitmap::Create( const D2D1BitmapDesc * pDesc, const Void * pData, UInt iDataPitch )
{
    DebugAssert( !(IsCreated()) );

    static const Float s_fInv96 = 1.0f / 96.0f;
    const Float fConvertDIPX = ( pDesc->fDpiX * s_fInv96 );
    const Float fConvertDIPY = ( pDesc->fDpiY * s_fInv96 );
    const Float fConvertInvDIPX = 1.0f / fConvertDIPX;
    const Float fConvertInvDIPY = 1.0f / fConvertDIPY;

    // Use pixel or DIP dimensions, pick the non-zero one
    // Both zero or none zero is an error (no implicit preference)
    pDesc->ConvertTo( &m_hBitmapProperties, &m_hSize, &m_hDIPSize );
    if ( m_hSize.width != 0 ) {
        DebugAssert( m_hDIPSize.width == 0.0f );
        m_hDIPSize.width = ( fConvertInvDIPX * (Float)(m_hSize.width) );
    } else {
        DebugAssert( m_hDIPSize.width != 0.0f );
        m_hSize.width = (UInt)( fConvertDIPX * m_hDIPSize.width );
    }
    if ( m_hSize.height != 0 ) {
        DebugAssert( m_hDIPSize.height == 0.0f );
        m_hDIPSize.height = ( fConvertInvDIPY * (Float)(m_hSize.height) );
    } else {
        DebugAssert( m_hDIPSize.height != 0.0f );
        m_hSize.height = (UInt)( fConvertDIPY * m_hDIPSize.height );
    }

    m_hCreationParameters.pData = pData;
    m_hCreationParameters.iDataPitch = iDataPitch;

    _NakedCreate();
}
Void D2D1Bitmap::Destroy()
{
    DebugAssert( IsCreated() );

    if ( m_bTemporaryDestroyed )
        m_bTemporaryDestroyed = false;
    else
        _NakedDestroy();

    m_hBitmapProperties.pixelFormat.format = DXGI_FORMAT_UNKNOWN;
    m_hBitmapProperties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_UNKNOWN;
    m_hBitmapProperties.dpiX = 0.0f;
    m_hBitmapProperties.dpiY = 0.0f;
    m_hSize.width = 0;
    m_hSize.height = 0;
    m_hDIPSize.width = 0.0f;
    m_hDIPSize.height = 0.0f;

    m_hCreationParameters.pData = NULL;
    m_hCreationParameters.iDataPitch = 0;
}

Void D2D1Bitmap::OnDestroyDevice()
{
    DebugAssert( !m_bTemporaryDestroyed );

    if ( m_pBitmap != NULL ) {
        _NakedDestroy();
        m_bTemporaryDestroyed = true;
    }
}
Void D2D1Bitmap::OnRestoreDevice()
{
    DebugAssert( m_pBitmap == NULL );

    if ( m_bTemporaryDestroyed ) {
        _NakedCreate();
        m_bTemporaryDestroyed = false;
    }
}

Void D2D1Bitmap::CopyFrom( const Void * pSrcData, UInt iSrcPitch, const D2D1RectangleI * pDstRect )
{
    DebugAssert( IsCreated() );

    // Destination rectangle
    D2D1_RECT_U hRect;

    if ( pDstRect != NULL ) {
        DebugAssert( pDstRect->iLeft < pDstRect->iRight && pDstRect->iRight < m_hSize.width );
        DebugAssert( pDstRect->iTop < pDstRect->iBottom && pDstRect->iBottom < m_hSize.height );
        hRect.left = pDstRect->iLeft;
        hRect.top = pDstRect->iTop;
        hRect.right = pDstRect->iRight;
        hRect.bottom = pDstRect->iBottom;
    } else {
        hRect.left = 0;
        hRect.top = 0;
        hRect.right = m_hSize.width - 1;
        hRect.bottom = m_hSize.height - 1;
    }

    // Perform Copy
    HRESULT hRes = m_pBitmap->CopyFromMemory( &hRect, pSrcData, iSrcPitch );
    DebugAssert( hRes == S_OK );
}
Void D2D1Bitmap::CopyFrom( D2D1Bitmap * pSrcBitmap, const D2D1PointI * pDstPoint, const D2D1RectangleI * pSrcRect )
{
    DebugAssert( IsCreated() );
    DebugAssert( pSrcBitmap->IsCreated() );

    DebugAssert( m_hBitmapProperties.pixelFormat.format == pSrcBitmap->m_hBitmapProperties.pixelFormat.format );
    DebugAssert( m_hBitmapProperties.pixelFormat.alphaMode == pSrcBitmap->m_hBitmapProperties.pixelFormat.alphaMode );

    // Destination point
    D2D1_POINT_2U hDstPoint;
    if ( pDstPoint != NULL ) {
        DebugAssert( pDstPoint->iX < m_hSize.width );
        DebugAssert( pDstPoint->iY < m_hSize.height );
        hDstPoint.x = pDstPoint->iX;
        hDstPoint.y = pDstPoint->iY;
    } else {
        hDstPoint.x = 0;
        hDstPoint.y = 0;
    }

    // Source rectangle
    D2D1_RECT_U hSrcRect;
    if ( pSrcRect != NULL ) {
        DebugAssert( pSrcRect->iLeft < pSrcRect->iRight && pSrcRect->iRight < pSrcBitmap->m_hSize.width );
        DebugAssert( pSrcRect->iTop < pSrcRect->iBottom && pSrcRect->iBottom < pSrcBitmap->m_hSize.height );
        hSrcRect.left = pSrcRect->iLeft;
        hSrcRect.top = pSrcRect->iTop;
        hSrcRect.right = pSrcRect->iRight;
        hSrcRect.bottom = pSrcRect->iBottom;
    } else {
        hSrcRect.left = 0;
        hSrcRect.top = 0;
        hSrcRect.right = pSrcBitmap->m_hSize.width - 1;
        hSrcRect.bottom = pSrcBitmap->m_hSize.height - 1;
    }

    // Destination rectangle
    UInt iCopyWidth = ( hSrcRect.right + 1 - hSrcRect.left );
    UInt iCopyHeight = ( hSrcRect.bottom + 1 - hSrcRect.top );
    DebugAssert( hDstPoint.x + iCopyWidth <= m_hSize.width );
    DebugAssert( hDstPoint.y + iCopyHeight <= m_hSize.height );

    // Perform Copy
    HRESULT hRes = m_pBitmap->CopyFromBitmap( &hDstPoint, pSrcBitmap->m_pBitmap, &hSrcRect );
    DebugAssert( hRes == S_OK );
}

/////////////////////////////////////////////////////////////////////////////////

Void D2D1Bitmap::_NakedCreate()
{
    m_pBitmap = NULL;
    HRESULT hRes = m_pContext2D->m_pD2D1RenderingContext->CreateBitmap( *((D2D1_SIZE_U*)(&m_hSize)), m_hCreationParameters.pData, m_hCreationParameters.iDataPitch,
                                                                        (const D2D1_BITMAP_PROPERTIES *)&m_hBitmapProperties, &m_pBitmap );
    DebugAssert( hRes == S_OK && m_pBitmap != NULL );
}
Void D2D1Bitmap::_NakedDestroy()
{
    m_pBitmap->Release();
    m_pBitmap = NULL;
}
