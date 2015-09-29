/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/2D/GPU2DContext.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU Resource : Core-Class for 2D rendering.
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
#ifndef SCARAB_ENGINE_RENDERING_2D_GPU2DCONTEXT_H
#define SCARAB_ENGINE_RENDERING_2D_GPU2DCONTEXT_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Resources/GPUTexture.h"

#include "Resources/GPU2DRenderState.h"
#include "Resources/GPU2DStrokeStyle.h"

#include "Resources/GPU2DBitmap.h"
#include "Resources/GPU2DBrush.h"

#include "Resources/GPU2DMesh.h"
#include "Resources/GPU2DGeometry.h"

#include "Resources/GPU2DLayer.h"

#include "Resources/GPU2DText.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// GPU2DContext definitions
enum GPU2DRenderTargetType {
    GPU2D_RENDERTARGET_DEFAULT  = DEVICE2D_RENDERTARGET_DEFAULT,
    GPU2D_RENDERTARGET_SOFTWARE = DEVICE2D_RENDERTARGET_SOFTWARE,
    GPU2D_RENDERTARGET_HARDWARE = DEVICE2D_RENDERTARGET_HARDWARE
};

enum GPU2DRenderTargetUsage {
    GPU2D_RENDERTARGETUSAGE_NONE                  = DEVICE2D_RENDERTARGETUSAGE_NONE,
    GPU2D_RENDERTARGETUSAGE_FORCE_BITMAP_REMOTING = DEVICE2D_RENDERTARGETUSAGE_FORCE_BITMAP_REMOTING,
    GPU2D_RENDERTARGETUSAGE_GDI_COMPATIBLE        = DEVICE2D_RENDERTARGETUSAGE_GDI_COMPATIBLE
};

typedef struct _gpu2d_rendertarget_desc {
    GPU2DRenderTargetType iType;
    GPU2DRenderTargetUsage iUsage;
    PixelFormat iFormat;
    GPU2DBitmapAlphaMode iAlphaMode;
    Float fDpiX;
    Float fDpiY;
} GPU2DRenderTargetDesc;

enum GPU2DTextDrawOptions {
    GPU2DTEXT_DRAWOPTION_NONE   = DEVICE2DTEXT_DRAWOPTION_NONE,
    GPU2DTEXT_DRAWOPTION_NOSNAP = DEVICE2DTEXT_DRAWOPTION_NOSNAP,
    GPU2DTEXT_DRAWOPTION_CLIP   = DEVICE2DTEXT_DRAWOPTION_CLIP
};

// Prototypes

/////////////////////////////////////////////////////////////////////////////////
// The GPU2DContext class
class GPU2DContext
{
public:
    GPU2DContext();
    ~GPU2DContext();

    // Binding
    inline Bool IsBound() const;
    inline Bool IsBoundToBackBuffer( UInt * outBackBuffer = NULL ) const;

    Void BindToBackBuffer( UInt iBackBuffer );
    Void BindToTexture( GPUTexture2D * pRenderTarget );
    Void UnBind();

    // Resource Factory        
    GPU2DBitmap * CreateBitmap( PixelFormat iFormat, UInt iWidth, UInt iHeight, const Byte * arrBitmap = NULL, Bool bCreateCPUData = true );
    Void DestroyBitmap( GPU2DBitmap * pBitmap ) const;
    
    GPU2DMesh * CreateMesh();
    Void DestroyMesh( GPU2DMesh * pMesh ) const;
    
    // Render target properties
    inline GPUTexture2D * GetRenderTarget() const;

    inline UInt GetMaxBitmapDimension() const;
    inline Bool IsSupported( const GPU2DRenderTargetDesc * pDesc ) const;
    inline Void GetDesktopDPI( Float * outDpiX, Float * outDpiY ) const;

    inline Void GetPixelFormat( PixelFormat * outFormat, GPU2DBitmapAlphaMode * outAlphaMode ) const;
    inline Void GetPixelSize( UInt * outWidth, UInt * outHeight ) const;
    inline Void GetDIPSize( Float * outWidth, Float * outHeight ) const;

    inline Float GetDpiX() const;
    inline Float GetDpiY() const;
    inline Void SetDPI( Float fDpiX, Float fDpiY );

    // RenderState methods
    inline GPU2DAntialiasingMode GetAntialiasingMode() const;
    inline Void SetAntialiasingMode( GPU2DAntialiasingMode iAntialiasingMode );

    inline const GPU2DMatrix32 * GetTransform() const;
    inline Void SetTransform( const GPU2DMatrix32 * pTransform );

    inline GPU2DTag GetTag1() const;
    inline GPU2DTag GetTag2() const;
    inline Void SetTags( GPU2DTag iTag1, GPU2DTag iTag2 );

    inline GPU2DTextAntialiasingMode GetTextAntialiasingMode() const;
    inline Void SetTextAntialiasingMode( GPU2DTextAntialiasingMode iTextAntialiasingMode );

    inline Void GetTextRenderState( GPU2DTextRenderState * outTextRenderState ) const;
    inline Void SetTextRenderState( GPU2DTextRenderState * pTextRenderState = NULL );

    // Clearing methods
    inline Void Clear( const GPU2DColor * pColor );

    // Layering methods
    inline Void PushLayer( GPU2DLayer * pLayer, const GPU2DLayerParameters * pParameters );
    inline Void PopLayer();

    // Clipping methods
    inline Void PushClip( const GPU2DRectangle * pRect, GPU2DAntialiasingMode iAntialiasingMode );
    inline Void PopClip();

    // Rendering methods
    inline Void Begin();
    inline Void End();

    inline Void DrawLine( const GPU2DPoint * pStart, const GPU2DPoint * pEnd, const GPU2DBrush * pBrush, Float fStrokeWidth = 1.0f, const GPU2DStrokeStyle * pStrokeStyle = NULL );
    inline Void DrawRectangle( const GPU2DRectangle * pRectangle, const GPU2DBrush * pBrush, Float fStrokeWidth = 1.0f, const GPU2DStrokeStyle * pStrokeStyle = NULL );
    inline Void FillRectangle( const GPU2DRectangle * pRectangle, const GPU2DBrush * pBrush );
    inline Void DrawRoundedRectangle( const GPU2DRoundedRectangle * pRoundedRectangle, const GPU2DBrush * pBrush, Float fStrokeWidth = 1.0f, const GPU2DStrokeStyle * pStrokeStyle = NULL );
    inline Void FillRoundedRectangle( const GPU2DRoundedRectangle * pRoundedRectangle, const GPU2DBrush * pBrush );
    inline Void DrawCircle( const GPU2DPoint * pCenter, Float fRadius, const GPU2DBrush * pBrush, Float fStrokeWidth = 1.0f, const GPU2DStrokeStyle * pStrokeStyle = NULL );
    inline Void FillCircle( const GPU2DPoint * pCenter, Float fRadius, const GPU2DBrush * pBrush );
    inline Void DrawEllipse( const GPU2DEllipse * pEllipse, const GPU2DBrush * pBrush, Float fStrokeWidth = 1.0f, const GPU2DStrokeStyle * pStrokeStyle = NULL );
    inline Void FillEllipse( const GPU2DEllipse * pEllipse, const GPU2DBrush * pBrush );

    inline Void DrawBitmap( const GPU2DBitmap * pBitmap, const GPU2DRectangle * pDstRect = NULL, const GPU2DRectangle * pSrcRect = NULL, Float fOpacity = 1.0f, GPU2DBitmapInterpolationMode iInterpolationMode = GPU2DBITMAP_INTERPOLATION_LINEAR );
    inline Void FillOpacityMask( const GPU2DBitmap * pOpacityMask, GPU2DBitmapOpacityMaskContent iOpacityMaskContent, const GPU2DBrush * pBrush, const GPU2DRectangle * pDstRect = NULL, const GPU2DRectangle * pSrcRect = NULL );

    inline Void FillMesh( const GPU2DMesh * pMesh, const GPU2DBrush * pBrush );
    inline Void DrawGeometry( const GPU2DGeometry * pGeometry, const GPU2DBrush * pBrush, Float fStrokeWidth = 1.0f, const GPU2DStrokeStyle * pStrokeStyle = NULL );
    inline Void FillGeometry( const GPU2DGeometry * pGeometry, const GPU2DBrush * pBrush, const GPU2DBrush * pOpacityBrush = NULL );

    inline Void DrawText( const GChar * strText, UInt iLength, const GPU2DTextFormat * pTextFormat, const GPU2DRectangle * pLayoutRect, const GPU2DBrush * pForegroundBrush,
                          UInt iDrawingOptions = GPU2DTEXT_DRAWOPTION_NONE, GPU2DTextMeasuringMode iMeasuringMode = GPU2DTEXT_MEASURING_NATURAL );
    inline Void DrawTextLayout( const GPU2DPoint * pTopLeft, const GPU2DTextLayout * pTextLayout, const GPU2DBrush * pForegroundBrush,
                                UInt iDrawingOptions = GPU2DTEXT_DRAWOPTION_NONE );

    inline Void Flush( GPU2DTag * outTag1 = NULL, GPU2DTag * outTag2 = NULL );

    inline Void SaveDrawingState( GPU2DRenderState * outRenderState ) const;
    inline Void RestoreDrawingState( GPU2DRenderState * pRenderState );

private:
    friend class RenderingManager;

    // Access to resources
    friend class GPU2DBitmap;
    friend class GPU2DBrush;
    friend class GPU2DSolidColorBrush;
    friend class GPU2DGradientBrush;
    friend class GPU2DLinearGradientBrush;
    friend class GPU2DRadialGradientBrush;
    friend class GPU2DBitmapBrush;
    friend class GPU2DMesh;
    friend class GPU2DLayer;
    friend class GPU2DTextInlineObject;

    // Renderer callbacks
    Void _OnDestroyDevice();
    Void _OnRestoreDevice();

    // Resource database
    typedef RedBlackTree<GPU2DBitmap*> _BitmapList;
    typedef RedBlackTree<GPU2DBrush*> _BrushList;
    typedef RedBlackTree<GPU2DMesh*> _MeshList;
    typedef RedBlackTree<GPU2DLayer*> _LayerList;

    inline static Int _Compare_GPU2DBitmap( GPU2DBitmap * const & rLeft, GPU2DBitmap * const & rRight, Void * pUserData );
    inline static Int _Compare_GPU2DBrush( GPU2DBrush * const & rLeft, GPU2DBrush * const & rRight, Void * pUserData );
    inline static Int _Compare_GPU2DMesh( GPU2DMesh * const & rLeft, GPU2DMesh * const & rRight, Void * pUserData );
    inline static Int _Compare_GPU2DLayer( GPU2DLayer * const & rLeft, GPU2DLayer * const & rRight, Void * pUserData );

    inline Void _RegisterBitmap( GPU2DBitmap * pBitmap );
    inline Void _RegisterBrush( GPU2DBrush * pBrush );
    inline Void _RegisterMesh( GPU2DMesh * pMesh );
    inline Void _RegisterLayer( GPU2DLayer * pLayer );

    inline Void _UnRegisterBitmap( GPU2DBitmap * pBitmap );
    inline Void _UnRegisterBrush( GPU2DBrush * pBrush );
    inline Void _UnRegisterMesh( GPU2DMesh * pMesh );
    inline Void _UnRegisterLayer( GPU2DLayer * pLayer );

    _BitmapList m_lstBitmaps;
    _BrushList m_lstBrushes;
    _MeshList m_lstMeshes;
    _LayerList m_lstLayers;

    // Render target
    GPUTexture2D * m_pRenderTarget; // NULL => render to backbuffer

    // Device data
    Device2DContext m_hDevice2DContext;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GPU2DContext.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RENDERING_2D_GPU2DCONTEXT_H
