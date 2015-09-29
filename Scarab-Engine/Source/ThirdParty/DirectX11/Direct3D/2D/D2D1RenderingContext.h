/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/2D/D2D1RenderingContext.h
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
// Header prelude
#ifndef SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_2D_D2D1RENDERINGCONTEXT_H
#define SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_2D_D2D1RENDERINGCONTEXT_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../D3D11Renderer.h"

#include "D2D1Mappings.h"

#include "Resources/D2D1RenderState.h"
#include "Resources/D2D1StrokeStyle.h"

#include "Resources/D2D1Bitmap.h"
#include "Resources/D2D1Brush.h"

#include "Resources/D2D1Mesh.h"
#include "Resources/D2D1Geometry.h"

#include "Resources/D2D1Layer.h"

#include "Resources/D2D1Text.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The D2D1RenderingContext class
class D2D1RenderingContext
{
public:
    D2D1RenderingContext( D3D11Renderer * pRenderer );
    virtual ~D2D1RenderingContext();

    // Deferred construction
    inline Bool IsCreated() const;
    inline Bool IsBoundToBackBuffer( UInt * outBackBuffer = NULL ) const;

    Void Create( UInt iBackBuffer = 0 );
    Void Create( D3D11Texture2D * pRenderTarget );
    Void Destroy();

    Void OnDestroyDevice();
    Void OnRestoreDevice();

    // Render target properties
    UInt GetMaxBitmapDimension() const;
    Bool IsSupported( const D2D1RenderTargetDesc * pDesc ) const;
    Void GetDesktopDPI( Float * outDpiX, Float * outDpiY ) const;

    Void GetPixelFormat( PixelFormat * outFormat, D2D1BitmapAlphaMode * outAlphaMode ) const;
    Void GetPixelSize( UInt * outWidth, UInt * outHeight ) const;
    Void GetDIPSize( Float * outWidth, Float * outHeight ) const;

    inline Float GetDpiX() const;
    inline Float GetDpiY() const;
    Void SetDPI( Float fDpiX, Float fDpiY );

    // RenderState methods
    inline D2D1AntialiasingMode GetAntialiasingMode() const;
    Void SetAntialiasingMode( D2D1AntialiasingMode iAntialiasingMode );

    inline const D2D1Matrix32 * GetTransform() const;
    Void SetTransform( const D2D1Matrix32 * pTransform );

    inline D2D1Tag GetTag1() const;
    inline D2D1Tag GetTag2() const;
    Void SetTags( D2D1Tag iTag1, D2D1Tag iTag2 );

    inline D2D1TextAntialiasingMode GetTextAntialiasingMode() const;
    Void SetTextAntialiasingMode( D2D1TextAntialiasingMode iTextAntialiasingMode );

    inline Void GetTextRenderState( D2D1TextRenderState * outTextRenderState ) const;
    Void SetTextRenderState( D2D1TextRenderState * pTextRenderState = NULL );

    // Clearing methods
    Void Clear( const D2D1Color * pColor );

    // Layering methods
    Void PushLayer( D2D1Layer * pLayer, const D2D1LayerParameters * pParameters );
    Void PopLayer();

    // Clipping methods
    Void PushClip( const D2D1Rectangle * pRect, D2D1AntialiasingMode iAntialiasingMode );
    Void PopClip();

    // Rendering methods
    Void Begin();
    Void End();

    Void DrawLine( const D2D1Point * pStart, const D2D1Point * pEnd, const D2D1Brush * pBrush, Float fStrokeWidth = 1.0f, const D2D1StrokeStyle * pStrokeStyle = NULL );
    Void DrawRectangle( const D2D1Rectangle * pRectangle, const D2D1Brush * pBrush, Float fStrokeWidth = 1.0f, const D2D1StrokeStyle * pStrokeStyle = NULL );
    Void FillRectangle( const D2D1Rectangle * pRectangle, const D2D1Brush * pBrush );
    Void DrawRoundedRectangle( const D2D1RoundedRectangle * pRoundedRectangle, const D2D1Brush * pBrush, Float fStrokeWidth = 1.0f, const D2D1StrokeStyle * pStrokeStyle = NULL );
    Void FillRoundedRectangle( const D2D1RoundedRectangle * pRoundedRectangle, const D2D1Brush * pBrush );
    inline Void DrawCircle( const D2D1Point * pCenter, Float fRadius, const D2D1Brush * pBrush, Float fStrokeWidth = 1.0f, const D2D1StrokeStyle * pStrokeStyle = NULL );
    inline Void FillCircle( const D2D1Point * pCenter, Float fRadius, const D2D1Brush * pBrush );
    Void DrawEllipse( const D2D1Ellipse * pEllipse, const D2D1Brush * pBrush, Float fStrokeWidth = 1.0f, const D2D1StrokeStyle * pStrokeStyle = NULL );
    Void FillEllipse( const D2D1Ellipse * pEllipse, const D2D1Brush * pBrush );

    Void DrawBitmap( const D2D1Bitmap * pSrcBitmap, const D2D1Rectangle * pDstRect = NULL, const D2D1Rectangle * pSrcRect = NULL, Float fOpacity = 1.0f, D2D1BitmapInterpolationMode iInterpolationMode = D2D1BITMAP_INTERPOLATION_LINEAR );
    Void FillOpacityMask( const D2D1Bitmap * pOpacityMask, D2D1BitmapOpacityMaskContent iOpacityMaskContent, const D2D1Brush * pBrush, const D2D1Rectangle * pDstRect = NULL, const D2D1Rectangle * pSrcRect = NULL );

    Void FillMesh( const D2D1Mesh * pMesh, const D2D1Brush * pBrush );
    Void DrawGeometry( const D2D1Geometry * pGeometry, const D2D1Brush * pBrush, Float fStrokeWidth = 1.0f, const D2D1StrokeStyle * pStrokeStyle = NULL );
    Void FillGeometry( const D2D1Geometry * pGeometry, const D2D1Brush * pBrush, const D2D1Brush * pOpacityBrush = NULL );

    Void DrawText( const GChar * strText, UInt iLength, const D2D1TextFormat * pTextFormat, const D2D1Rectangle * pLayoutRect, const D2D1Brush * pForegroundBrush,
                   UInt iDrawingOptions = D2D1TEXT_DRAWOPTION_NONE, D2D1TextMeasuringMode iMeasuringMode = D2D1TEXT_MEASURING_NATURAL );
    Void DrawTextLayout( const D2D1Point * pTopLeft, const D2D1TextLayout * pTextLayout, const D2D1Brush * pForegroundBrush,
                         UInt iDrawingOptions = D2D1TEXT_DRAWOPTION_NONE );
    //Void DrawGlyphRun( const D2D1Point * pBaselineOrigin, const D2D1GlyphRun * pGlyphRun, const D2D1Brush * pForegroundBrush,
    //                   D2D1TextMeasuringMode iMeasuringMode = D2D1TEXT_MEASURINGMODE_NATURAL );

    Void Flush( D2D1Tag * outTag1 = NULL, D2D1Tag * outTag2 = NULL );

    Void SaveDrawingState( D2D1RenderState * outRenderState ) const;
    Void RestoreDrawingState( const D2D1RenderState * pRenderState );

    // Remaining ... //////////////////////////////////////////////////////////////
    // CreateSharedBitmap / Creates an ID2D1Bitmap whose data is shared with another resource.
    // CreateBitmapFromWicBitmap / Creates an ID2D1Bitmap by copying the specified Microsoft Windows Imaging Component (WIC) bitmap.
    // CreateCompatibleRenderTarget / Creates a new bitmap render target for use during intermediate offscreen drawing that is compatible with the current render target
    ///////////////////////////////////////////////////////////////////////////////

private:
    // 3D Renderer bind
    D3D11Renderer * m_pRenderer;

    static ID2D1Factory * sm_pD2D1Factory;
    static IDWriteFactory * sm_pDWFactory;

    // Resources access
    friend class D2D1RenderState;
    friend class D2D1TextRenderState;

    friend class D2D1StrokeStyle;

    friend class D2D1Bitmap;

    friend class D2D1SolidColorBrush;
    friend class D2D1GradientBrush;
    friend class D2D1LinearGradientBrush;
    friend class D2D1RadialGradientBrush;
    friend class D2D1BitmapBrush;

    friend class D2D1Mesh;

    friend class D2D1RectangleGeometry;
    friend class D2D1RoundedRectangleGeometry;
    friend class D2D1EllipseGeometry;
    friend class D2D1PathGeometry;
    friend class D2D1TransformedGeometry;
    friend class D2D1GeometryGroup;

    friend class D2D1Layer;

    friend class D2D1FontCollection;
    friend class D2D1TextFormat;
    friend class D2D1TextLayout;

    // Direct2D data
    D3D11Texture2D * m_pRenderTarget;

    Float m_fDpiX;
    Float m_fDpiY;
    __D2D1_DRAWING_STATE_DESCRIPTION m_hDrawingState;
    IDWriteRenderingParams * m_pTextRenderingParams;

    ID2D1RenderTarget * m_pD2D1RenderingContext;

    // Auto-Regen system
    Void _NakedCreate();
    Void _NakedDestroy();

    Bool m_bTemporaryDestroyed;
    UInt m_iBoundToBackBuffer;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "D2D1RenderingContext.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_2D_D2D1RENDERINGCONTEXT_H

