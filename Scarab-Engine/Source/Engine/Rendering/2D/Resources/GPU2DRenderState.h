/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/2D/Resources/GPU2DRenderState.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU 2D-Resource : Render States
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
#ifndef SCARAB_ENGINE_RENDERING_2D_RESOURCES_GPU2DRENDERSTATE_H
#define SCARAB_ENGINE_RENDERING_2D_RESOURCES_GPU2DRENDERSTATE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../Lib/Error/ErrorManager.h"
#include "../../../../Lib/Math/Types/Matrix/Matrix2.h"
#include "../../../../Lib/Math/Types/Matrix/Matrix3.h"
#include "../../../../Lib/Math/Types/Discrete/Rectangle2.h"
#include "../../../../Lib/Math/Types/Color/Color3.h"
#include "../../../../Lib/Math/Types/Color/Color4.h"

#include "../../DeviceRenderer.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// General definitions
typedef struct _gpu2d_point {
    inline struct _gpu2d_point()                                  { fX = 0.0f; fY = 0.0f; }
    inline struct _gpu2d_point( const Point2 & rhs )              { fX = (Float)(rhs.X); fY = (Float)(rhs.Y); }
    inline struct _gpu2d_point( const Offset2 & rhs )             { fX = (Float)(rhs.X); fY = (Float)(rhs.Y); }
    inline struct _gpu2d_point( const Vertex2 & rhs )             { fX = rhs.X; fY = rhs.Y; }
    inline struct _gpu2d_point( const Vector2 & rhs )             { fX = rhs.X; fY = rhs.Y; }
    inline struct _gpu2d_point( const struct _gpu2d_point & rhs ) { fX = rhs.fX; fY = rhs.fY; }

    inline struct _gpu2d_point & operator=( const Point2 & rhs )              { fX = (Float)(rhs.X); fY = (Float)(rhs.Y); return (*this); }
    inline struct _gpu2d_point & operator=( const Offset2 & rhs )             { fX = (Float)(rhs.X); fY = (Float)(rhs.Y); return (*this); }
    inline struct _gpu2d_point & operator=( const Vertex2 & rhs )             { fX = rhs.X; fY = rhs.Y; return (*this); }
    inline struct _gpu2d_point & operator=( const Vector2 & rhs )             { fX = rhs.X; fY = rhs.Y; return (*this); }
    inline struct _gpu2d_point & operator=( const struct _gpu2d_point & rhs ) { fX = rhs.fX; fY = rhs.fY; return (*this); }

    inline operator Point2() const { return Point2((Int)fX, (Int)fY); }
    inline operator Offset2() const { return Offset2((Int)fX, (Int)fY); }
    inline operator Vertex2() const { return Vertex2(fX, fY); }
    inline operator Vector2() const { return Vector2(fX, fY); }

    Float fX;
    Float fY;
} GPU2DPoint;

typedef struct _gpu2d_rectangle {
    inline struct _gpu2d_rectangle()                                      { fLeft = 0.0f; fTop = 0.0f; fRight = 1.0f; fBottom = 1.0f; }
    inline struct _gpu2d_rectangle( const Rectangle2 & rhs )              { fLeft = (Float)(rhs.TopLeft.X); fTop = (Float)(rhs.TopLeft.Y); fRight = (Float)(rhs.DownRight.X); fBottom = (Float)(rhs.DownRight.Y); }
    inline struct _gpu2d_rectangle( const struct _gpu2d_rectangle & rhs ) { fLeft = rhs.fLeft; fTop = rhs.fTop; fRight = rhs.fRight; fBottom = rhs.fBottom; }

    inline struct _gpu2d_rectangle & operator=( const Rectangle2 & rhs )              { fLeft = (Float)(rhs.TopLeft.X); fTop = (Float)(rhs.TopLeft.Y); fRight = (Float)(rhs.DownRight.X); fBottom = (Float)(rhs.DownRight.Y); return (*this); }
    inline struct _gpu2d_rectangle & operator=( const struct _gpu2d_rectangle & rhs ) { fLeft = rhs.fLeft; fTop = rhs.fTop; fRight = rhs.fRight; fBottom = rhs.fBottom; return (*this); }

    inline operator Rectangle2() const { return Rectangle2((Int)fLeft, (Int)fTop, (Int)fRight, (Int)fBottom); }

    Float fLeft;
    Float fTop;
    Float fRight;
    Float fBottom;
} GPU2DRectangle;

typedef struct _gpu2d_matrix32 {
    inline struct _gpu2d_matrix32()                                     { f00 = 1.0f; f01 = 0.0f; f10 = 0.0f; f11 = 1.0f; f20 = 0.0f; f21 = 0.0f; }
    inline struct _gpu2d_matrix32( const Matrix2 & rhs )                { f00 = rhs.m00; f01 = rhs.m01; f10 = rhs.m10; f11 = rhs.m11; f20 = 0.0f; f21 = 0.0f; }
    inline struct _gpu2d_matrix32( const Matrix3 & rhs )                { f00 = rhs.m00; f01 = rhs.m01; f10 = rhs.m10; f11 = rhs.m11; f20 = rhs.m20; f21 = rhs.m21; }
    inline struct _gpu2d_matrix32( const struct _gpu2d_matrix32 & rhs ) { f00 = rhs.f00; f01 = rhs.f01; f10 = rhs.f10; f11 = rhs.f11; f20 = rhs.f20; f21 = rhs.f21; }

    inline struct _gpu2d_matrix32 & operator=( const Matrix2 & rhs )                { f00 = rhs.m00; f01 = rhs.m01; f10 = rhs.m10; f11 = rhs.m11; f20 = 0.0f; f21 = 0.0f; return (*this); }
    inline struct _gpu2d_matrix32 & operator=( const Matrix3 & rhs )                { f00 = rhs.m00; f01 = rhs.m01; f10 = rhs.m10; f11 = rhs.m11; f20 = rhs.m20; f21 = rhs.m21; return (*this); }
    inline struct _gpu2d_matrix32 & operator=( const struct _gpu2d_matrix32 & rhs ) { f00 = rhs.f00; f01 = rhs.f01; f10 = rhs.f10; f11 = rhs.f11; f20 = rhs.f20; f21 = rhs.f21; return (*this); }

    inline operator Matrix2() const { return Matrix2(f00, f01, f10, f11); }
    inline operator Matrix3() const { return Matrix3(f00, f01, 0.0f, f10, f11, 0.0f, f20, f21, 1.0f); }

    Float f00; Float f01;
    Float f10; Float f11;
    Float f20; Float f21;
} GPU2DMatrix32;

typedef struct _gpu2d_color {
    inline struct _gpu2d_color()                                  { R = 0.0f; G = 0.0f; B = 0.0f; A = 1.0f; }
    inline struct _gpu2d_color( const Color3 & rhs )              { R = rhs.R; G = rhs.G; B = rhs.B; A = 1.0f; }
    inline struct _gpu2d_color( const Color4 & rhs )              { R = rhs.R; G = rhs.G; B = rhs.B; A = rhs.A; }
    inline struct _gpu2d_color( const struct _gpu2d_color & rhs ) { R = rhs.R; G = rhs.G; B = rhs.B; A = rhs.A; }

    inline struct _gpu2d_color & operator=( const Color3 & rhs )              { R = rhs.R; G = rhs.G; B = rhs.B; A = 1.0f; return (*this); }
    inline struct _gpu2d_color & operator=( const Color4 & rhs )              { R = rhs.R; G = rhs.G; B = rhs.B; A = rhs.A; return (*this); }
    inline struct _gpu2d_color & operator=( const struct _gpu2d_color & rhs ) { R = rhs.R; G = rhs.G; B = rhs.B; A = rhs.A; return (*this); }

    inline operator Color3() const { return Color3(R, G, B); }
    inline operator Color4() const { return Color4(R, G, B, A); }

    Float R;
    Float G;
    Float B;
    Float A;
} GPU2DColor;

// GPU2DRenderState definitions
enum GPU2DAntialiasingMode {
    GPU2D_ANTIALIASING_PER_PRIMITIVE = DEVICE2D_ANTIALIASING_PER_PRIMITIVE,
    GPU2D_ANTIALIASING_ALIASED       = DEVICE2D_ANTIALIASING_ALIASED
};

enum GPU2DTextAntialiasingMode {
    GPU2DTEXT_ANTIALIASING_DEFAULT   = DEVICE2DTEXT_ANTIALIASING_DEFAULT,
    GPU2DTEXT_ANTIALIASING_CLEARTYPE = DEVICE2DTEXT_ANTIALIASING_CLEARTYPE,
    GPU2DTEXT_ANTIALIASING_GRAYSCALE = DEVICE2DTEXT_ANTIALIASING_GRAYSCALE,
    GPU2DTEXT_ANTIALIASING_ALIASED   = DEVICE2DTEXT_ANTIALIASING_ALIASED
};

typedef Device2DTag GPU2DTag;

typedef struct _gpu2d_renderstate_desc {
    GPU2DAntialiasingMode iAntialiasingMode;
    GPU2DTextAntialiasingMode iTextAntialiasingMode;
    GPU2DTag iTag1;
    GPU2DTag iTag2;
    GPU2DMatrix32 matTransform;
} GPU2DRenderStateDesc;

// GPU2DTextRenderState definitions
enum GPU2DTextPixelGeometry {
    GPU2DTEXT_PIXELGEOMETRY_FLAT  = DEVICE2DTEXT_PIXELGEOMETRY_FLAT,
    GPU2DTEXT_PIXELGEOMETRY_RGB   = DEVICE2DTEXT_PIXELGEOMETRY_RGB,
    GPU2DTEXT_PIXELGEOMETRY_BGR   = DEVICE2DTEXT_PIXELGEOMETRY_BGR
};

enum GPU2DTextRenderingMode {
    GPU2DTEXT_RENDERINGMODE_DEFAULT                     = DEVICE2DTEXT_RENDERINGMODE_DEFAULT,
    GPU2DTEXT_RENDERINGMODE_ALIASED                     = DEVICE2DTEXT_RENDERINGMODE_ALIASED,
    GPU2DTEXT_RENDERINGMODE_OUTLINE                     = DEVICE2DTEXT_RENDERINGMODE_OUTLINE,
    GPU2DTEXT_RENDERINGMODE_CLEARTYPE_GDI_CLASSIC       = DEVICE2DTEXT_RENDERINGMODE_CLEARTYPE_GDI_CLASSIC,
    GPU2DTEXT_RENDERINGMODE_CLEARTYPE_GDI_NATURAL       = DEVICE2DTEXT_RENDERINGMODE_CLEARTYPE_GDI_NATURAL,
    GPU2DTEXT_RENDERINGMODE_CLEARTYPE_NATURAL           = DEVICE2DTEXT_RENDERINGMODE_CLEARTYPE_NATURAL,
    GPU2DTEXT_RENDERINGMODE_CLEARTYPE_NATURAL_SYMMETRIC = DEVICE2DTEXT_RENDERINGMODE_CLEARTYPE_NATURAL_SYMMETRIC
};

typedef struct _gpu2d_text_renderstate_desc {
    Float fGamma;            // ]0;256]
    Float fEnhancedContrast; // [0;+inf]
    Float fClearTypeLevel;   // [0;1]
    GPU2DTextPixelGeometry iPixelGeometry;
    GPU2DTextRenderingMode iRenderingMode;
} GPU2DTextRenderStateDesc;

// Prototypes
class GPU2DContext;
class GPU2DFontFace;

class GPU2DRenderState;
class GPU2DTextRenderState;

/////////////////////////////////////////////////////////////////////////////////
// The GPU2DRenderState class
class GPU2DRenderState
{
public:
    GPU2DRenderState();
    ~GPU2DRenderState();

    // Binding
    inline Bool IsBound() const;

    inline Void Bind( const GPU2DRenderStateDesc * pDesc );
    inline Void UnBind();

    // Methods
    inline Void GetDesc( GPU2DRenderStateDesc * outDesc ) const;
    inline Void SetDesc( const GPU2DRenderStateDesc * pDesc );

    inline Void GetTextRenderState( GPU2DTextRenderState * outTextRenderState ) const;
    inline Void SetTextRenderState( GPU2DTextRenderState * pTextRenderState = NULL );

private:
    friend class GPU2DContext;
    
    // Device data
    Device2DRenderState m_hDevice2DRenderState;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPU2DTextRenderState class
class GPU2DTextRenderState
{
public:
    GPU2DTextRenderState();
    ~GPU2DTextRenderState();

    // Binding
    inline Bool IsBound() const;

    inline Void Bind( Void * pMonitor = NULL );
    inline Void Bind( const GPU2DTextRenderStateDesc * pDesc );
    inline Void UnBind();

    // Methods
    inline Float GetGamma() const;
    inline Float GetEnhancedContrast() const;
    inline Float GetClearTypeLevel() const;
    inline GPU2DTextPixelGeometry GetPixelGeometry() const;
    inline GPU2DTextRenderingMode GetRenderingMode() const;

    inline Void GetDesc( GPU2DTextRenderStateDesc * outDesc ) const;

private:
    friend class GPU2DRenderState;
    friend class GPU2DContext;
    friend class GPU2DFontFace;

    // Device data
    Device2DTextRenderState m_hDevice2DTextRenderState;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GPU2DRenderState.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RENDERING_2D_RESOURCES_GPU2DRENDERSTATE_H
