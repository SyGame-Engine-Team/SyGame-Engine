/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/2D/Resources/GPU2DBrush.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU 2D-Resource : Brushes
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
#ifndef SCARAB_ENGINE_RENDERING_2D_RESOURCES_GPU2DBRUSH_H
#define SCARAB_ENGINE_RENDERING_2D_RESOURCES_GPU2DBRUSH_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "GPU2DRenderState.h"
#include "GPU2DBitmap.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Brush types
enum GPU2DBrushType {
    GPU2DBRUSH_SOLID_COLOR     = DEVICE2DBRUSH_SOLID_COLOR,
    GPU2DBRUSH_GRADIENT_LINEAR = DEVICE2DBRUSH_GRADIENT_LINEAR,
    GPU2DBRUSH_GRADIENT_RADIAL = DEVICE2DBRUSH_GRADIENT_RADIAL,
    GPU2DBRUSH_BITMAP          = DEVICE2DBRUSH_BITMAP
};

// GPU2DBrush definitions
typedef struct _gpu2d_brush_desc {
    Float fOpacity;
    GPU2DMatrix32 matTransform;
} GPU2DBrushDesc;

// GPU2DGradientBrush definitions
enum GPU2DBrushGamma {
    GPU2DBRUSH_GAMMA_2_2 = DEVICE2DBRUSH_GAMMA_2_2,
    GPU2DBRUSH_GAMMA_1_0 = DEVICE2DBRUSH_GAMMA_1_0
};
enum GPU2DBrushWrapMode {
    GPU2DBRUSH_WRAPMODE_CLAMP  = DEVICE2DBRUSH_WRAPMODE_CLAMP,
    GPU2DBRUSH_WRAPMODE_REPEAT = DEVICE2DBRUSH_WRAPMODE_REPEAT,
    GPU2DBRUSH_WRAPMODE_MIRROR = DEVICE2DBRUSH_WRAPMODE_MIRROR
};

#define GPU2DGRADIENT_MAX_STOPS DEVICE2DGRADIENT_MAX_STOPS

typedef struct _gpu2d_gradient_stop {
    Float fPosition;
    GPU2DColor fColor;
} GPU2DGradientStop;
typedef struct _gpu2d_gradient_desc {
    GPU2DBrushGamma iGamma;
    GPU2DBrushWrapMode iWrapMode;
    UInt iStopCount;
    GPU2DGradientStop arrGradientStops[GPU2DGRADIENT_MAX_STOPS];
} GPU2DGradientDesc;

// GPU2DLinearGradientBrush definitions
typedef struct _gpu2d_brush_lineargradient_desc {
    GPU2DPoint ptStart;
    GPU2DPoint ptEnd;
} GPU2DBrushLinearGradientDesc;

// GPU2DRadialGradientBrush definitions
typedef struct _gpu2d_brush_radialgradient_desc {
    GPU2DPoint ptCenter;
    GPU2DPoint ptOffset;
    Float fRadiusX;
    Float fRadiusY;
} GPU2DBrushRadialGradientDesc;

// GPU2DBitmapBrush definitions
typedef struct _gpu2d_brush_bitmap_desc {
    GPU2DBitmapInterpolationMode iInterpolationMode;
    GPU2DBrushWrapMode iWrapModeX;
    GPU2DBrushWrapMode iWrapModeY;
} GPU2DBrushBitmapDesc;

// Prototypes
class GPU2DContext;
class GPU2DTextLayout;

/////////////////////////////////////////////////////////////////////////////////
// The GPU2DBrush class
class GPU2DBrush
{
protected:
    GPU2DBrush( GPU2DContext * pContext2D );
public:
    virtual ~GPU2DBrush();

    // Binding
    inline Bool IsBound() const;

    Void UnBind();

    // Getters
    inline GPU2DBrushType GetType() const;

    // Methods
    inline Float GetOpacity() const;
    inline Void SetOpacity( Float fOpacity );

    inline Void GetTransform( GPU2DMatrix32 * outTransform ) const;
    inline Void SetTransform( const GPU2DMatrix32 * pTransform );

    inline Void GetDesc( GPU2DBrushDesc * outDesc ) const;
    inline Void SetDesc( const GPU2DBrushDesc * pDesc );

protected:
    friend class GPU2DContext;
    GPU2DContext * m_pContext2D;

    // Device data
    Device2DBrush * m_pDevice2DBrush;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPU2DSolidColorBrush class
class GPU2DSolidColorBrush : public GPU2DBrush
{
public:
    GPU2DSolidColorBrush( GPU2DContext * pContext2D );
    virtual ~GPU2DSolidColorBrush();

    // Binding
    Void Bind();

    // Methods
    inline const GPU2DColor * GetColor() const;
    inline Void GetColor( GPU2DColor * outColor ) const;
    inline Void SetColor( const GPU2DColor * pColor );

protected:
    friend class GPU2DContext;
    friend class GPU2DTextLayout;

    // Device data
    Device2DSolidColorBrush m_hDevice2DSolidColorBrush;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPU2DGradientBrush class
class GPU2DGradientBrush : public GPU2DBrush
{
protected:
    GPU2DGradientBrush( GPU2DContext * pContext2D );
public:
    virtual ~GPU2DGradientBrush();

    // Getters
    inline GPU2DBrushGamma GetGradientGammaMode() const;
    inline GPU2DBrushWrapMode GetGradientWrapMode() const;

    // Methods
    inline UInt GetGradientStopCount() const;
    inline const GPU2DGradientStop * GetGradientStop( UInt iStop ) const;
    inline Void GetGradientStop( GPU2DGradientStop * outGradientStop, UInt iStop ) const;
    inline Void GetGradientStops( GPU2DGradientStop * outGradientStops, UInt iMaxStops ) const;

    inline Void GetGradientDesc( GPU2DGradientDesc * outGradientDesc ) const;

protected:
    friend class GPU2DContext;

    // Device data
    Device2DGradientBrush * m_pDevice2DGradientBrush;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPU2DLinearGradientBrush class
class GPU2DLinearGradientBrush : public GPU2DGradientBrush
{
public:
    GPU2DLinearGradientBrush( GPU2DContext * pContext2D );
    virtual ~GPU2DLinearGradientBrush();

    // Binding
    Void Bind( const GPU2DGradientDesc * pGradientDesc );

    // Methods
    inline const GPU2DPoint * GetStartPoint() const;
    inline Void GetStartPoint( GPU2DPoint * outStart ) const;
    inline Void SetStartPoint( const GPU2DPoint * pStart );

    inline const GPU2DPoint * GetEndPoint() const;
    inline Void GetEndPoint( GPU2DPoint * outEnd ) const;
    inline Void SetEndPoint( const GPU2DPoint * pEnd );

    inline Void GetLinearGradientDesc( GPU2DBrushLinearGradientDesc * outLinearGradientDesc ) const;
    inline Void SetLinearGradientDesc( const GPU2DBrushLinearGradientDesc * pLinearGradientDesc );

protected:
    friend class GPU2DContext;

    // Device data
    Device2DLinearGradientBrush m_hDevice2DLinearGradientBrush;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPU2DRadialGradientBrush class
class GPU2DRadialGradientBrush : public GPU2DGradientBrush
{
public:
    GPU2DRadialGradientBrush( GPU2DContext * pContext2D );
    virtual ~GPU2DRadialGradientBrush();

    // Binding
    Void Bind( const GPU2DGradientDesc * pGradientDesc );

    // Methods
    inline const GPU2DPoint * GetCenter() const;
    inline Void SetCenter( const GPU2DPoint * pCenter );

    inline const GPU2DPoint * GetOffset() const;
    inline Void SetOffset( const GPU2DPoint * pOffset );

    inline Float GetRadiusX() const;
    inline Float GetRadiusY() const;
    inline Void SetRadiusX( Float fRadiusX );
    inline Void SetRadiusY( Float fRadiusY );

    inline Void GetRadialGradientDesc( GPU2DBrushRadialGradientDesc * outRadialGradientDesc ) const;
    inline Void SetRadialGradientDesc( const GPU2DBrushRadialGradientDesc * pRadialGradientDesc );

protected:
    friend class GPU2DContext;

    // Device data
    Device2DRadialGradientBrush m_hDevice2DRadialGradientBrush;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPU2DBitmapBrush class
class GPU2DBitmapBrush : public GPU2DBrush
{
public:
    GPU2DBitmapBrush( GPU2DContext * pContext2D );
    virtual ~GPU2DBitmapBrush();

    // Binding
    Void Bind();

    // Methods
    inline GPU2DBitmap * GetBitmap() const;
    inline Void SetBitmap( GPU2DBitmap * pBitmap );

    inline GPU2DBitmapInterpolationMode GetInterpolationMode() const;
    inline Void SetInterpolationMode( GPU2DBitmapInterpolationMode iInterpolationMode );

    inline GPU2DBrushWrapMode GetWrapModeX() const;
    inline GPU2DBrushWrapMode GetWrapModeY() const;
    inline Void SetWrapModeX( GPU2DBrushWrapMode iWrapModeX );
    inline Void SetWrapModeY( GPU2DBrushWrapMode iWrapModeY );

    inline Void GetBrushBitmapDesc( GPU2DBrushBitmapDesc * outBitmapDesc ) const;
    inline Void SetBrushBitmapDesc( const GPU2DBrushBitmapDesc * pBitmapDesc );

protected:
    friend class GPU2DContext;

    GPU2DBitmap * m_pBitmap;

    // Device data
    Device2DBitmapBrush m_hDevice2DBitmapBrush;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GPU2DBrush.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RENDERING_2D_RESOURCES_GPU2DBRUSH_H
