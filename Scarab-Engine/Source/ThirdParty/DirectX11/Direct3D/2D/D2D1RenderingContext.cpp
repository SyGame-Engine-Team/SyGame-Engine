/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/2D/D2D1RenderingContext.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Platform-dependant abstraction for 2D graphics.
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
#include <d3d11.h>
#include <d2d1.h>
#include <dwrite.h>

#undef DebugAssert
#undef DrawText

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "D2D1RenderingContext.h"

/////////////////////////////////////////////////////////////////////////////////
// D2D1RenderingContext implementation
ID2D1Factory * D2D1RenderingContext::sm_pD2D1Factory = NULL;
IDWriteFactory * D2D1RenderingContext::sm_pDWFactory = NULL;

D2D1RenderingContext::D2D1RenderingContext( D3D11Renderer * pRenderer )
{
    m_pRenderer = pRenderer;

    sm_pD2D1Factory = m_pRenderer->m_pD2D1Factory;
    sm_pDWFactory = m_pRenderer->m_pDWFactory;

    m_pRenderTarget = NULL;

    m_fDpiX = 96.0f;
    m_fDpiY = 96.0f;
    m_hDrawingState.antialiasMode = D2D1_ANTIALIAS_MODE_PER_PRIMITIVE;
    m_hDrawingState.transform._11 = 1.0f;
    m_hDrawingState.transform._12 = 0.0f;
    m_hDrawingState.transform._21 = 0.0f;
    m_hDrawingState.transform._22 = 1.0f;
    m_hDrawingState.transform._31 = 0.0f;
    m_hDrawingState.transform._32 = 0.0f;
    m_hDrawingState.tag1 = 0;
    m_hDrawingState.tag2 = 0;
    m_hDrawingState.textAntialiasMode = D2D1_TEXT_ANTIALIAS_MODE_DEFAULT;
    m_pTextRenderingParams = NULL;

    m_pD2D1RenderingContext = NULL;

    m_bTemporaryDestroyed = false;
    m_iBoundToBackBuffer = INVALID_OFFSET;
}
D2D1RenderingContext::~D2D1RenderingContext()
{
    if ( m_pTextRenderingParams != NULL ) {
        m_pTextRenderingParams->Release();
        m_pTextRenderingParams = NULL;
    }

    if ( IsCreated() )
        Destroy();
}

Void D2D1RenderingContext::Create( UInt iBackBuffer )
{
    DebugAssert( !(IsCreated()) );
    DebugAssert( iBackBuffer < m_pRenderer->m_hSwapChainDesc.iBufferCount );

    m_pRenderTarget = NULL;
    m_iBoundToBackBuffer = iBackBuffer;

    _NakedCreate();
}
Void D2D1RenderingContext::Create( D3D11Texture2D * pRenderTarget )
{
    DebugAssert( !(IsCreated()) );
    DebugAssert( pRenderTarget->IsCreated() );

    UInt iBackBuffer = 0;
    if ( pRenderTarget->IsBoundToBackBuffer(&iBackBuffer) ) {
        m_pRenderTarget = NULL;
        m_iBoundToBackBuffer = iBackBuffer;
    } else {
        m_pRenderTarget = pRenderTarget;
        m_iBoundToBackBuffer = INVALID_OFFSET;
    }

    _NakedCreate();
}
Void D2D1RenderingContext::Destroy()
{
    DebugAssert( IsCreated() );

    if ( m_bTemporaryDestroyed )
        m_bTemporaryDestroyed = false;
    else
        _NakedDestroy();

    m_pRenderTarget = NULL;

    m_iBoundToBackBuffer = INVALID_OFFSET;
}

Void D2D1RenderingContext::OnDestroyDevice()
{
    DebugAssert( !m_bTemporaryDestroyed );

    if ( m_pD2D1RenderingContext != NULL ) {
        _NakedDestroy();
        m_bTemporaryDestroyed = true;
    }
}
Void D2D1RenderingContext::OnRestoreDevice()
{
    DebugAssert( m_pD2D1RenderingContext == NULL );

    if ( m_bTemporaryDestroyed ) {
        _NakedCreate();
        m_bTemporaryDestroyed = false;
    }
}

UInt D2D1RenderingContext::GetMaxBitmapDimension() const
{
    DebugAssert( IsCreated() );

    return m_pD2D1RenderingContext->GetMaximumBitmapSize();
}
Bool D2D1RenderingContext::IsSupported( const D2D1RenderTargetDesc * pDesc ) const
{
    DebugAssert( IsCreated() );

    D2D1_RENDER_TARGET_PROPERTIES hProperties;
    pDesc->ConvertTo( &hProperties );

    BOOL bSupported = m_pD2D1RenderingContext->IsSupported( &hProperties );
    return ( bSupported != FALSE );
}
Void D2D1RenderingContext::GetDesktopDPI( Float * outDpiX, Float * outDpiY ) const
{
    DebugAssert( IsCreated() );

    sm_pD2D1Factory->GetDesktopDpi( outDpiX, outDpiY );
}

Void D2D1RenderingContext::GetPixelFormat( PixelFormat * outFormat, D2D1BitmapAlphaMode * outAlphaMode ) const
{
    DebugAssert( IsCreated() );

    D2D1_PIXEL_FORMAT iFormat = m_pD2D1RenderingContext->GetPixelFormat();
    *outFormat = PixelFormatFromDXGI[iFormat.format];
    *outAlphaMode = D2D1BitmapAlphaModeFromD2D1[iFormat.alphaMode];
}
Void D2D1RenderingContext::GetPixelSize( UInt * outWidth, UInt * outHeight ) const
{
    DebugAssert( IsCreated() );

    D2D1_SIZE_U hSize = m_pD2D1RenderingContext->GetPixelSize();
    *outWidth = hSize.width;
    *outHeight = hSize.height;
}
Void D2D1RenderingContext::GetDIPSize( Float * outWidth, Float * outHeight ) const
{
    DebugAssert( IsCreated() );

    D2D1_SIZE_F hSize = m_pD2D1RenderingContext->GetSize();
    *outWidth = hSize.width;
    *outHeight = hSize.height;
}

Void D2D1RenderingContext::SetDPI( Float fDpiX, Float fDpiY )
{
    m_fDpiX = fDpiX;
    m_fDpiY = fDpiY;

    if ( IsCreated() )
        m_pD2D1RenderingContext->SetDpi( m_fDpiX, m_fDpiY );
}

Void D2D1RenderingContext::SetAntialiasingMode( D2D1AntialiasingMode iAntialiasingMode )
{
    m_hDrawingState.antialiasMode = D2D1AntialiasingModeToD2D1[iAntialiasingMode];

    if ( IsCreated() )
        m_pD2D1RenderingContext->SetAntialiasMode( m_hDrawingState.antialiasMode );
}

Void D2D1RenderingContext::SetTransform( const D2D1Matrix32 * pTransform )
{
    m_hDrawingState.transform._11 = pTransform->f00;
    m_hDrawingState.transform._12 = pTransform->f01;
    m_hDrawingState.transform._21 = pTransform->f10;
    m_hDrawingState.transform._22 = pTransform->f11;
    m_hDrawingState.transform._31 = pTransform->f20;
    m_hDrawingState.transform._32 = pTransform->f21;

    if ( IsCreated() )
        m_pD2D1RenderingContext->SetTransform( (const D2D1_MATRIX_3X2_F *)&(m_hDrawingState.transform) );
}

Void D2D1RenderingContext::SetTags( D2D1Tag iTag1, D2D1Tag iTag2 )
{
    m_hDrawingState.tag1 = (D2D1_TAG)iTag1;
    m_hDrawingState.tag2 = (D2D1_TAG)iTag2;

    if ( IsCreated() )
        m_pD2D1RenderingContext->SetTags( m_hDrawingState.tag1, m_hDrawingState.tag2 );
}

Void D2D1RenderingContext::SetTextAntialiasingMode( D2D1TextAntialiasingMode iTextAntialiasingMode )
{
    m_hDrawingState.textAntialiasMode = D2D1TextAntialiasingModeToD2D1[iTextAntialiasingMode];

    if ( IsCreated() )
        m_pD2D1RenderingContext->SetTextAntialiasMode( m_hDrawingState.textAntialiasMode );
}

Void D2D1RenderingContext::SetTextRenderState( D2D1TextRenderState * pTextRenderState )
{
    DebugAssert( pTextRenderState == NULL || pTextRenderState->IsCreated() );

    IDWriteRenderingParams * pTextRenderingParams = (pTextRenderState != NULL) ? pTextRenderState->m_pTextRenderingParams : NULL;
    if ( m_pTextRenderingParams == pTextRenderingParams )
        return;
    
    if ( m_pTextRenderingParams != NULL ) {
        m_pTextRenderingParams->Release();
        m_pTextRenderingParams = NULL;
    }
    m_pTextRenderingParams = pTextRenderingParams;

    if ( IsCreated() )
        m_pD2D1RenderingContext->SetTextRenderingParams( m_pTextRenderingParams );
}

Void D2D1RenderingContext::Clear( const D2D1Color * pColor )
{
    DebugAssert( IsCreated() );

    m_pD2D1RenderingContext->Clear( (const D2D1_COLOR_F *)pColor );
}

Void D2D1RenderingContext::PushLayer( D2D1Layer * pLayer, const D2D1LayerParameters * pParameters )
{
    DebugAssert( IsCreated() );

    D2D1_LAYER_PARAMETERS hLayerParameters;
    hLayerParameters.contentBounds.left = pParameters->rectContentBounds.fLeft;
    hLayerParameters.contentBounds.top = pParameters->rectContentBounds.fTop;
    hLayerParameters.contentBounds.right = pParameters->rectContentBounds.fRight;
    hLayerParameters.contentBounds.bottom = pParameters->rectContentBounds.fBottom;

    hLayerParameters.geometricMask = pParameters->pMaskGeometry->m_pGeometry;
    hLayerParameters.maskAntialiasMode = D2D1AntialiasingModeToD2D1[pParameters->iMaskAntialiasingMode];
    hLayerParameters.maskTransform._11 = pParameters->matMaskTransform.f00;
    hLayerParameters.maskTransform._12 = pParameters->matMaskTransform.f01;
    hLayerParameters.maskTransform._21 = pParameters->matMaskTransform.f10;
    hLayerParameters.maskTransform._22 = pParameters->matMaskTransform.f11;
    hLayerParameters.maskTransform._31 = pParameters->matMaskTransform.f20;
    hLayerParameters.maskTransform._32 = pParameters->matMaskTransform.f21;

    hLayerParameters.opacityBrush = pParameters->pOpacityBrush->m_pBrush;

    hLayerParameters.opacity = pParameters->fOpacity;

    hLayerParameters.layerOptions = D2D1LayerOptionsToD2D1[pParameters->iLayerOptions];

    m_pD2D1RenderingContext->PushLayer( &hLayerParameters, pLayer->m_pLayer );
}
Void D2D1RenderingContext::PopLayer()
{
    DebugAssert( IsCreated() );

    m_pD2D1RenderingContext->PopLayer();
}

Void D2D1RenderingContext::PushClip( const D2D1Rectangle * pRect, D2D1AntialiasingMode iAntialiasingMode )
{
    DebugAssert( IsCreated() );

    m_pD2D1RenderingContext->PushAxisAlignedClip( (const D2D1_RECT_F *)pRect, D2D1AntialiasingModeToD2D1[iAntialiasingMode] );
}
Void D2D1RenderingContext::PopClip()
{
    DebugAssert( IsCreated() );

    m_pD2D1RenderingContext->PopAxisAlignedClip();
}

Void D2D1RenderingContext::Begin()
{
    DebugAssert( IsCreated() );

    m_pD2D1RenderingContext->BeginDraw();
}
Void D2D1RenderingContext::End()
{
    DebugAssert( IsCreated() );

    HRESULT hRes = m_pD2D1RenderingContext->EndDraw();
    DebugAssert( hRes == S_OK );
}

Void D2D1RenderingContext::DrawLine( const D2D1Point * pStart, const D2D1Point * pEnd, const D2D1Brush * pBrush, Float fStrokeWidth, const D2D1StrokeStyle * pStrokeStyle )
{
    DebugAssert( IsCreated() );
    DebugAssert( pBrush->IsCreated() );
    DebugAssert( pStrokeStyle == NULL || pStrokeStyle->IsCreated() );

    m_pD2D1RenderingContext->DrawLine( *( (const D2D1_POINT_2F *)pStart ), *( (const D2D1_POINT_2F *)pEnd ), pBrush->m_pBrush, fStrokeWidth, (pStrokeStyle == NULL) ? NULL : pStrokeStyle->m_pStrokeStyle );
}
Void D2D1RenderingContext::DrawRectangle( const D2D1Rectangle * pRectangle, const D2D1Brush * pBrush, Float fStrokeWidth, const D2D1StrokeStyle * pStrokeStyle )
{
    DebugAssert( IsCreated() );
    DebugAssert( pBrush->IsCreated() );
    DebugAssert( pStrokeStyle == NULL || pStrokeStyle->IsCreated() );

    m_pD2D1RenderingContext->DrawRectangle( (const D2D1_RECT_F *)pRectangle, pBrush->m_pBrush, fStrokeWidth, (pStrokeStyle == NULL) ? NULL : pStrokeStyle->m_pStrokeStyle );
}
Void D2D1RenderingContext::FillRectangle( const D2D1Rectangle * pRectangle, const D2D1Brush * pBrush )
{
    DebugAssert( IsCreated() );
    DebugAssert( pBrush->IsCreated() );

    m_pD2D1RenderingContext->FillRectangle( (const D2D1_RECT_F *)pRectangle, pBrush->m_pBrush );
}
Void D2D1RenderingContext::DrawRoundedRectangle( const D2D1RoundedRectangle * pRoundedRectangle, const D2D1Brush * pBrush, Float fStrokeWidth, const D2D1StrokeStyle * pStrokeStyle )
{
    DebugAssert( IsCreated() );
    DebugAssert( pBrush->IsCreated() );
    DebugAssert( pStrokeStyle == NULL || pStrokeStyle->IsCreated() );

    m_pD2D1RenderingContext->DrawRoundedRectangle( (const D2D1_ROUNDED_RECT *)pRoundedRectangle, pBrush->m_pBrush, fStrokeWidth, (pStrokeStyle == NULL) ? NULL : pStrokeStyle->m_pStrokeStyle );
}
Void D2D1RenderingContext::FillRoundedRectangle( const D2D1RoundedRectangle * pRoundedRectangle, const D2D1Brush * pBrush )
{
    DebugAssert( IsCreated() );
    DebugAssert( pBrush->IsCreated() );

    m_pD2D1RenderingContext->FillRoundedRectangle( (const D2D1_ROUNDED_RECT *)pRoundedRectangle, pBrush->m_pBrush );
}
Void D2D1RenderingContext::DrawEllipse( const D2D1Ellipse * pEllipse, const D2D1Brush * pBrush, Float fStrokeWidth, const D2D1StrokeStyle * pStrokeStyle )
{
    DebugAssert( IsCreated() );
    DebugAssert( pBrush->IsCreated() );
    DebugAssert( pStrokeStyle == NULL || pStrokeStyle->IsCreated() );

    m_pD2D1RenderingContext->DrawEllipse( (const D2D1_ELLIPSE *)pEllipse, pBrush->m_pBrush, fStrokeWidth, (pStrokeStyle == NULL) ? NULL : pStrokeStyle->m_pStrokeStyle );
}
Void D2D1RenderingContext::FillEllipse( const D2D1Ellipse * pEllipse, const D2D1Brush * pBrush )
{
    DebugAssert( IsCreated() );
    DebugAssert( pBrush->IsCreated() );

    m_pD2D1RenderingContext->FillEllipse( (const D2D1_ELLIPSE *)pEllipse, pBrush->m_pBrush );
}

Void D2D1RenderingContext::DrawBitmap( const D2D1Bitmap * pSrcBitmap, const D2D1Rectangle * pDstRect, const D2D1Rectangle * pSrcRect, Float fOpacity, D2D1BitmapInterpolationMode iInterpolationMode )
{
    DebugAssert( IsCreated() );
    DebugAssert( pSrcBitmap->IsCreated() );

    m_pD2D1RenderingContext->DrawBitmap( pSrcBitmap->m_pBitmap, (const D2D1_RECT_F *)pDstRect, fOpacity, D2D1BitmapInterpolationModeToD2D1[iInterpolationMode], (const D2D1_RECT_F *)pSrcRect );
}
Void D2D1RenderingContext::FillOpacityMask( const D2D1Bitmap * pOpacityMask, D2D1BitmapOpacityMaskContent iOpacityMaskContent, const D2D1Brush * pBrush, const D2D1Rectangle * pDstRect, const D2D1Rectangle * pSrcRect )
{
    DebugAssert( IsCreated() );
    DebugAssert( pOpacityMask->IsCreated() );
    DebugAssert( pBrush->IsCreated() );

    m_pD2D1RenderingContext->FillOpacityMask( pOpacityMask->m_pBitmap, pBrush->m_pBrush, D2D1BitmapOpacityMaskContentToD2D1[iOpacityMaskContent], (const D2D1_RECT_F *)pDstRect, (const D2D1_RECT_F *)pSrcRect );
}

Void D2D1RenderingContext::FillMesh( const D2D1Mesh * pMesh, const D2D1Brush * pBrush )
{
    DebugAssert( IsCreated() );
    DebugAssert( pMesh->IsCreated() );
    DebugAssert( pBrush->IsCreated() );

    m_pD2D1RenderingContext->FillMesh( pMesh->m_pMesh, pBrush->m_pBrush );
}
Void D2D1RenderingContext::DrawGeometry( const D2D1Geometry * pGeometry, const D2D1Brush * pBrush, Float fStrokeWidth, const D2D1StrokeStyle * pStrokeStyle )
{
    DebugAssert( IsCreated() );
    DebugAssert( pGeometry->IsCreated() );
    DebugAssert( pBrush->IsCreated() );
    DebugAssert( pStrokeStyle == NULL || pStrokeStyle->IsCreated() );

    m_pD2D1RenderingContext->DrawGeometry( pGeometry->m_pGeometry, pBrush->m_pBrush, fStrokeWidth, (pStrokeStyle == NULL) ? NULL : pStrokeStyle->m_pStrokeStyle );
}
Void D2D1RenderingContext::FillGeometry( const D2D1Geometry * pGeometry, const D2D1Brush * pBrush, const D2D1Brush * pOpacityBrush )
{
    DebugAssert( IsCreated() );
    DebugAssert( pGeometry->IsCreated() );
    DebugAssert( pBrush->IsCreated() );
    DebugAssert( pOpacityBrush == NULL || pOpacityBrush->IsCreated() );

    m_pD2D1RenderingContext->FillGeometry( pGeometry->m_pGeometry, pBrush->m_pBrush, (pOpacityBrush == NULL) ? NULL : pOpacityBrush->m_pBrush );
}

Void D2D1RenderingContext::DrawText( const GChar * strText, UInt iLength, const D2D1TextFormat * pTextFormat, const D2D1Rectangle * pLayoutRect, const D2D1Brush * pForegroundBrush, UInt iDrawingOptions, D2D1TextMeasuringMode iMeasuringMode )
{
    DebugAssert( IsCreated() );
    DebugAssert( pTextFormat->IsCreated() );
    DebugAssert( pForegroundBrush->IsCreated() );

#if ( defined(UNICODE) || defined(_UNICODE) )
    m_pD2D1RenderingContext->DrawTextW( strText, iLength, pTextFormat->m_pTextFormat, (const D2D1_RECT_F *)pLayoutRect, pForegroundBrush->m_pBrush, D2D1TextDrawOptionsToD2D1[iDrawingOptions], D2D1TextMeasuringModeToD2D1[iMeasuringMode] );
#else
    m_pD2D1RenderingContext->DrawTextA( strText, iLength, pTextFormat->m_pTextFormat, (const D2D1_RECT_F *)pLayoutRect, pForegroundBrush->m_pBrush, D2D1TextDrawOptionsToD2D1[iDrawingOptions], D2D1TextMeasuringModeToD2D1[iMeasuringMode] );
#endif
}
Void D2D1RenderingContext::DrawTextLayout( const D2D1Point * pTopLeft, const D2D1TextLayout * pTextLayout, const D2D1Brush * pForegroundBrush, UInt iDrawingOptions )
{
    DebugAssert( IsCreated() );
    DebugAssert( pTextLayout->IsCreated() );
    DebugAssert( pForegroundBrush->IsCreated() );

    m_pD2D1RenderingContext->DrawTextLayout( *( (const D2D1_POINT_2F *)pTopLeft ), pTextLayout->m_pTextLayout, pForegroundBrush->m_pBrush, D2D1TextDrawOptionsToD2D1[iDrawingOptions] );
}
//Void D2D1RenderingContext::DrawGlyphRun( const D2D1Point * pBaselineOrigin, const D2D1GlyphRun * pGlyphRun, const D2D1Brush * pForegroundBrush, D2D1TextMeasuringMode iMeasuringMode )
//{
//    DebugAssert( IsCreated() );
//    DebugAssert( pGlyphRun->IsCreated() );
//    DebugAssert( pForegroundBrush->IsCreated() );
//
//    m_pD2D1RenderingContext->DrawGlyphRun( *( (const D2D1_POINT_2F *)pBaselineOrigin ), pGlyphRun->m_pGlyphRun, pForegroundBrush->m_pBrush, D2D1TextMeasuringModeToD2D1[iMeasuringMode] );
//}

Void D2D1RenderingContext::Flush( D2D1Tag * outTag1, D2D1Tag * outTag2 )
{
    DebugAssert( IsCreated() );
    
    HRESULT hRes = m_pD2D1RenderingContext->Flush( outTag1, outTag2 );
    DebugAssert( hRes == S_OK );
}

Void D2D1RenderingContext::SaveDrawingState( D2D1RenderState * pRenderState ) const
{
    DebugAssert( IsCreated() );
    DebugAssert( pRenderState->IsCreated() );

    m_pD2D1RenderingContext->SaveDrawingState( pRenderState->m_pStateBlock );
}
Void D2D1RenderingContext::RestoreDrawingState( const D2D1RenderState * pRenderState )
{
    DebugAssert( IsCreated() );
    DebugAssert( pRenderState->IsCreated() );

    m_pD2D1RenderingContext->RestoreDrawingState( pRenderState->m_pStateBlock );
}

/////////////////////////////////////////////////////////////////////////////////

Void D2D1RenderingContext::_NakedCreate()
{
    HRESULT hRes;

    D2D1_RENDER_TARGET_PROPERTIES hProperties;
    hProperties.minLevel = D2D1_FEATURE_LEVEL_10;
    hProperties.type = D2D1_RENDER_TARGET_TYPE_HARDWARE;
    hProperties.usage = D2D1_RENDER_TARGET_USAGE_NONE;
    hProperties.pixelFormat.format = DXGI_FORMAT_UNKNOWN;
    hProperties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
    hProperties.dpiX = m_fDpiX;
    hProperties.dpiY = m_fDpiY;

    IDXGISurface * pSurface2D = NULL;
    if ( m_iBoundToBackBuffer == INVALID_OFFSET ) {
        hRes = m_pRenderTarget->m_pTexture->QueryInterface( __uuidof(IDXGISurface), (Void**)&pSurface2D );
        DebugAssert( hRes == S_OK && pSurface2D != NULL );
    } else {
        hRes = m_pRenderer->m_pSwapChain->GetBuffer( m_iBoundToBackBuffer, __uuidof(IDXGISurface), (Void**)&pSurface2D );
        DebugAssert( hRes == S_OK && pSurface2D != NULL );
    }

    m_pD2D1RenderingContext = NULL;
    hRes = sm_pD2D1Factory->CreateDxgiSurfaceRenderTarget( pSurface2D, &hProperties, &m_pD2D1RenderingContext );
    DebugAssert( hRes == S_OK && m_pD2D1RenderingContext != NULL );

    pSurface2D->Release();
    pSurface2D = NULL;

    m_pD2D1RenderingContext->SetAntialiasMode( m_hDrawingState.antialiasMode );
    m_pD2D1RenderingContext->SetTransform( (const D2D1_MATRIX_3X2_F *)&(m_hDrawingState.transform) );
    m_pD2D1RenderingContext->SetTags( m_hDrawingState.tag1, m_hDrawingState.tag2 );
    m_pD2D1RenderingContext->SetTextAntialiasMode( m_hDrawingState.textAntialiasMode );
    m_pD2D1RenderingContext->SetTextRenderingParams( m_pTextRenderingParams );
}
Void D2D1RenderingContext::_NakedDestroy()
{
    m_pD2D1RenderingContext->Release();
    m_pD2D1RenderingContext = NULL;
}


