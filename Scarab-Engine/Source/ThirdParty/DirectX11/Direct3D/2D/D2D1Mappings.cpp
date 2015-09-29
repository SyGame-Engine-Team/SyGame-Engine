/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/2D/D2D1Mappings.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : API-dependant mappings for Direct2D and DirectWrite.
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
#include <dwrite.h>

#undef DebugAssert

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "D2D1Mappings.h"

/////////////////////////////////////////////////////////////////////////////////
// D2D1RenderingContext definitions
D2D1RenderTargetType D2D1RenderTargetTypeFromD2D1[D2D1_RENDERTARGET_COUNT] = {
    D2D1_RENDERTARGET_DEFAULT,
    D2D1_RENDERTARGET_SOFTWARE,
    D2D1_RENDERTARGET_HARDWARE
};
D2D1_RENDER_TARGET_TYPE D2D1RenderTargetTypeToD2D1[D2D1_RENDERTARGET_COUNT] = {
    D2D1_RENDER_TARGET_TYPE_DEFAULT,
    D2D1_RENDER_TARGET_TYPE_SOFTWARE,
    D2D1_RENDER_TARGET_TYPE_HARDWARE
};

D2D1RenderTargetUsage D2D1RenderTargetUsageFromD2D1[D2D1_RENDERTARGETUSAGE_COUNT] = {
    D2D1_RENDERTARGETUSAGE_NONE,
    D2D1_RENDERTARGETUSAGE_FORCE_BITMAP_REMOTING,
    D2D1_RENDERTARGETUSAGE_GDI_COMPATIBLE
};
D2D1_RENDER_TARGET_USAGE D2D1RenderTargetUsageToD2D1[D2D1_RENDERTARGETUSAGE_COUNT] = {
    D2D1_RENDER_TARGET_USAGE_NONE,
    D2D1_RENDER_TARGET_USAGE_FORCE_BITMAP_REMOTING,
    D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE
};

D2D1BitmapAlphaMode D2D1BitmapAlphaModeFromD2D1[D2D1BITMAP_ALPHAMODE_COUNT] = {
    D2D1BITMAP_ALPHAMODE_UNKNOWN,
    D2D1BITMAP_ALPHAMODE_PREMULTIPLIED,
    D2D1BITMAP_ALPHAMODE_STRAIGHT,
    D2D1BITMAP_ALPHAMODE_IGNORE
};
D2D1_ALPHA_MODE D2D1BitmapAlphaModeToD2D1[D2D1BITMAP_ALPHAMODE_COUNT] = {
    D2D1_ALPHA_MODE_UNKNOWN,
    D2D1_ALPHA_MODE_PREMULTIPLIED,
    D2D1_ALPHA_MODE_STRAIGHT,
    D2D1_ALPHA_MODE_IGNORE
};

Void D2D1RenderTargetDesc::ConvertFrom( const D2D1_RENDER_TARGET_PROPERTIES * pDesc )
{
    iType = D2D1RenderTargetTypeFromD2D1[pDesc->type];
    iUsage = D2D1RenderTargetUsageFromD2D1[pDesc->usage];
    iFormat = PixelFormatFromDXGI[pDesc->pixelFormat.format];
    iAlphaMode = D2D1BitmapAlphaModeFromD2D1[pDesc->pixelFormat.alphaMode];
    fDpiX = pDesc->dpiX;
    fDpiY = pDesc->dpiY;
    DebugAssert( pDesc->minLevel >= D2D1_FEATURE_LEVEL_10 );
}
Void D2D1RenderTargetDesc::ConvertTo( D2D1_RENDER_TARGET_PROPERTIES * outDesc ) const
{
    outDesc->type = D2D1RenderTargetTypeToD2D1[iType];
    outDesc->usage = D2D1RenderTargetUsageToD2D1[iUsage];
    outDesc->pixelFormat.format = PixelFormatToDXGI[iFormat];
    outDesc->pixelFormat.alphaMode = D2D1BitmapAlphaModeToD2D1[iAlphaMode];
    outDesc->dpiX = fDpiX;
    outDesc->dpiY = fDpiY;
    outDesc->minLevel = D2D1_FEATURE_LEVEL_10;
}

D2D1TextDrawOptions D2D1TextDrawOptionsFromD2D1[D2D1TEXT_DRAWOPTION_COUNT] = {
    D2D1TEXT_DRAWOPTION_NONE,
    D2D1TEXT_DRAWOPTION_NOSNAP,
    D2D1TEXT_DRAWOPTION_CLIP
};
D2D1_DRAW_TEXT_OPTIONS D2D1TextDrawOptionsToD2D1[D2D1TEXT_DRAWOPTION_COUNT] = {
    D2D1_DRAW_TEXT_OPTIONS_NONE,
    D2D1_DRAW_TEXT_OPTIONS_NO_SNAP,
    D2D1_DRAW_TEXT_OPTIONS_CLIP
};

/////////////////////////////////////////////////////////////////////////////////
// D2D1RenderState definitions
D2D1AntialiasingMode D2D1AntialiasingModeFromD2D1[D2D1_ANTIALIASING_COUNT] = {
    D2D1_ANTIALIASING_PER_PRIMITIVE,
    D2D1_ANTIALIASING_ALIASED
};
D2D1_ANTIALIAS_MODE D2D1AntialiasingModeToD2D1[D2D1_ANTIALIASING_COUNT] = {
    D2D1_ANTIALIAS_MODE_PER_PRIMITIVE,
    D2D1_ANTIALIAS_MODE_ALIASED
};

D2D1TextAntialiasingMode D2D1TextAntialiasingModeFromD2D1[D2D1TEXT_ANTIALIASING_COUNT] = {
    D2D1TEXT_ANTIALIASING_DEFAULT,
    D2D1TEXT_ANTIALIASING_CLEARTYPE,
    D2D1TEXT_ANTIALIASING_GRAYSCALE,
    D2D1TEXT_ANTIALIASING_ALIASED
};
D2D1_TEXT_ANTIALIAS_MODE D2D1TextAntialiasingModeToD2D1[D2D1TEXT_ANTIALIASING_COUNT] = {
    D2D1_TEXT_ANTIALIAS_MODE_DEFAULT,
    D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE,
    D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE,
    D2D1_TEXT_ANTIALIAS_MODE_ALIASED
};

Void D2D1RenderStateDesc::ConvertFrom( const D2D1_DRAWING_STATE_DESCRIPTION * pDesc )
{
    iAntialiasingMode = D2D1AntialiasingModeFromD2D1[pDesc->antialiasMode];
    iTextAntialiasingMode = D2D1TextAntialiasingModeFromD2D1[pDesc->textAntialiasMode];
    iTag1 = (D2D1Tag)( pDesc->tag1 );
    iTag2 = (D2D1Tag)( pDesc->tag2 );
    matTransform.f00 = pDesc->transform._11;
    matTransform.f01 = pDesc->transform._12;
    matTransform.f10 = pDesc->transform._21;
    matTransform.f11 = pDesc->transform._22;
    matTransform.f20 = pDesc->transform._31;
    matTransform.f21 = pDesc->transform._32;
}
Void D2D1RenderStateDesc::ConvertTo( D2D1_DRAWING_STATE_DESCRIPTION * outDesc ) const
{
    outDesc->antialiasMode = D2D1AntialiasingModeToD2D1[iAntialiasingMode];
    outDesc->textAntialiasMode = D2D1TextAntialiasingModeToD2D1[iTextAntialiasingMode];
    outDesc->tag1 = (D2D1_TAG)iTag1;
    outDesc->tag2 = (D2D1_TAG)iTag2;
    outDesc->transform._11 = matTransform.f00;
    outDesc->transform._12 = matTransform.f01;
    outDesc->transform._21 = matTransform.f10;
    outDesc->transform._22 = matTransform.f11;
    outDesc->transform._31 = matTransform.f20;
    outDesc->transform._32 = matTransform.f21;
}

D2D1TextPixelGeometry D2D1TextPixelGeometryFromD2D1[D2D1TEXT_PIXELGEOMETRY_COUNT] = {
    D2D1TEXT_PIXELGEOMETRY_FLAT,
    D2D1TEXT_PIXELGEOMETRY_RGB,
    D2D1TEXT_PIXELGEOMETRY_BGR
};
DWRITE_PIXEL_GEOMETRY D2D1TextPixelGeometryToD2D1[D2D1TEXT_PIXELGEOMETRY_COUNT] = {
    DWRITE_PIXEL_GEOMETRY_FLAT,
    DWRITE_PIXEL_GEOMETRY_RGB,
    DWRITE_PIXEL_GEOMETRY_BGR
};

D2D1TextRenderingMode D2D1TextRenderingModeFromD2D1[D2D1TEXT_RENDERINGMODE_COUNT] = {
    D2D1TEXT_RENDERINGMODE_DEFAULT,
    D2D1TEXT_RENDERINGMODE_ALIASED,
    D2D1TEXT_RENDERINGMODE_CLEARTYPE_GDI_CLASSIC,
    D2D1TEXT_RENDERINGMODE_CLEARTYPE_GDI_NATURAL,
    D2D1TEXT_RENDERINGMODE_CLEARTYPE_NATURAL,
    D2D1TEXT_RENDERINGMODE_CLEARTYPE_NATURAL_SYMMETRIC,
    D2D1TEXT_RENDERINGMODE_OUTLINE
};
DWRITE_RENDERING_MODE D2D1TextRenderingModeToD2D1[D2D1TEXT_RENDERINGMODE_COUNT] = {
    DWRITE_RENDERING_MODE_DEFAULT,
    DWRITE_RENDERING_MODE_ALIASED,
    DWRITE_RENDERING_MODE_OUTLINE,
    DWRITE_RENDERING_MODE_CLEARTYPE_GDI_CLASSIC,
    DWRITE_RENDERING_MODE_CLEARTYPE_GDI_NATURAL,
    DWRITE_RENDERING_MODE_CLEARTYPE_NATURAL,
    DWRITE_RENDERING_MODE_CLEARTYPE_NATURAL_SYMMETRIC
};

/////////////////////////////////////////////////////////////////////////////////
// D2D1StrokeStyle definitions
D2D1StrokeCapStyle D2D1StrokeCapStyleFromD2D1[D2D1STROKE_CAPSTYLE_COUNT] = {
    D2D1STROKE_CAPSTYLE_FLAT,
    D2D1STROKE_CAPSTYLE_SQUARE,
    D2D1STROKE_CAPSTYLE_ROUND,
    D2D1STROKE_CAPSTYLE_TRIANGLE
};
D2D1_CAP_STYLE D2D1StrokeCapStyleToD2D1[D2D1STROKE_CAPSTYLE_COUNT] = {
    D2D1_CAP_STYLE_FLAT,
    D2D1_CAP_STYLE_SQUARE,
    D2D1_CAP_STYLE_ROUND,
    D2D1_CAP_STYLE_TRIANGLE
};

D2D1StrokeLineJoin D2D1StrokeLineJoinFromD2D1[D2D1STROKE_LINEJOIN_COUNT] = {
    D2D1STROKE_LINEJOIN_MITER,
    D2D1STROKE_LINEJOIN_BEVEL,
    D2D1STROKE_LINEJOIN_ROUND,
    D2D1STROKE_LINEJOIN_MITER_OR_BEVEL
};
D2D1_LINE_JOIN D2D1StrokeLineJoinToD2D1[D2D1STROKE_LINEJOIN_COUNT] = {
    D2D1_LINE_JOIN_MITER,
    D2D1_LINE_JOIN_BEVEL,
    D2D1_LINE_JOIN_ROUND,
    D2D1_LINE_JOIN_MITER_OR_BEVEL
};

D2D1StrokeDashStyle D2D1StrokeDashStyleFromD2D1[D2D1STROKE_DASHSTYLE_COUNT] = {
    D2D1STROKE_DASHSTYLE_SOLID,
    D2D1STROKE_DASHSTYLE_DASH,
    D2D1STROKE_DASHSTYLE_DOT,
    D2D1STROKE_DASHSTYLE_DASHDOT,
    D2D1STROKE_DASHSTYLE_DASHDOTDOT,
    D2D1STROKE_DASHSTYLE_CUSTOM
};
D2D1_DASH_STYLE D2D1StrokeDashStyleToD2D1[D2D1STROKE_DASHSTYLE_COUNT] = {
    D2D1_DASH_STYLE_SOLID,
    D2D1_DASH_STYLE_DASH,
    D2D1_DASH_STYLE_DOT,
    D2D1_DASH_STYLE_DASH_DOT,
    D2D1_DASH_STYLE_DASH_DOT_DOT,
    D2D1_DASH_STYLE_CUSTOM
};

Void D2D1StrokeStyleDesc::ConvertFrom( const D2D1_STROKE_STYLE_PROPERTIES * pDesc )
{
    iStartCap = D2D1StrokeCapStyleFromD2D1[pDesc->startCap];
    iEndCap = D2D1StrokeCapStyleFromD2D1[pDesc->endCap];
    iDashCap = D2D1StrokeCapStyleFromD2D1[pDesc->dashCap];
    iLineJoin = D2D1StrokeLineJoinFromD2D1[pDesc->lineJoin];
    fMiterLimit = pDesc->miterLimit;
    iDashStyle = D2D1StrokeDashStyleFromD2D1[pDesc->dashStyle];
    fDashOffset = pDesc->dashOffset;
}
Void D2D1StrokeStyleDesc::ConvertTo( D2D1_STROKE_STYLE_PROPERTIES * outDesc ) const
{
    outDesc->startCap = D2D1StrokeCapStyleToD2D1[iStartCap];
    outDesc->endCap = D2D1StrokeCapStyleToD2D1[iEndCap];
    outDesc->dashCap = D2D1StrokeCapStyleToD2D1[iDashCap];
    outDesc->lineJoin = D2D1StrokeLineJoinToD2D1[iLineJoin];
    outDesc->miterLimit = fMiterLimit;
    outDesc->dashStyle = D2D1StrokeDashStyleToD2D1[iDashStyle];
    outDesc->dashOffset = fDashOffset;
}

/////////////////////////////////////////////////////////////////////////////////
// D2D1Bitmap definitions
D2D1BitmapInterpolationMode D2D1BitmapInterpolationModeFromD2D1[D2D1BITMAP_INTERPOLATION_COUNT] = {
    D2D1BITMAP_INTERPOLATION_NEAREST,
    D2D1BITMAP_INTERPOLATION_LINEAR
};
D2D1_BITMAP_INTERPOLATION_MODE D2D1BitmapInterpolationModeToD2D1[D2D1BITMAP_INTERPOLATION_COUNT] = {
    D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
    D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
};

D2D1BitmapOpacityMaskContent D2D1BitmapOpacityMaskContentFromD2D1[D2D1BITMAP_OPACITYMASK_COUNT] = {
    D2D1BITMAP_OPACITYMASK_GRAPHICS,
    D2D1BITMAP_OPACITYMASK_TEXT_NATURAL,
    D2D1BITMAP_OPACITYMASK_TEXT_GDICOMPATIBLE
};
D2D1_OPACITY_MASK_CONTENT D2D1BitmapOpacityMaskContentToD2D1[D2D1BITMAP_OPACITYMASK_COUNT] = {
    D2D1_OPACITY_MASK_CONTENT_GRAPHICS,
    D2D1_OPACITY_MASK_CONTENT_TEXT_NATURAL,
    D2D1_OPACITY_MASK_CONTENT_TEXT_GDI_COMPATIBLE
};

Void D2D1BitmapDesc::ConvertFrom( const __D2D1_BITMAP_PROPERTIES * pDesc, const __D2D1_SIZE_U * pSize, const __D2D1_SIZE_F * pDIPSize )
{
    iFormat = PixelFormatFromDXGI[pDesc->pixelFormat.format];
    iAlphaMode = D2D1BitmapAlphaModeFromD2D1[pDesc->pixelFormat.alphaMode];
    fDpiX = pDesc->dpiX;
    fDpiY = pDesc->dpiY;
    iWidth = pSize->width;
    iHeight = pSize->height;
    fDIPWidth = pDIPSize->width;
    fDIPHeight = pDIPSize->height;
}
Void D2D1BitmapDesc::ConvertTo( __D2D1_BITMAP_PROPERTIES * outDesc, __D2D1_SIZE_U * outSize, __D2D1_SIZE_F * outDIPSize ) const
{
    outDesc->pixelFormat.format = PixelFormatToDXGI[iFormat];
    outDesc->pixelFormat.alphaMode = D2D1BitmapAlphaModeToD2D1[iAlphaMode];
    outDesc->dpiX = fDpiX;
    outDesc->dpiY = fDpiY;
    outSize->width = iWidth;
    outSize->height = iHeight;
    outDIPSize->width = fDIPWidth;
    outDIPSize->height = fDIPHeight;
}

/////////////////////////////////////////////////////////////////////////////////
// D2D1Brush definitions
D2D1BrushGamma D2D1BrushGammaFromD2D1[D2D1BRUSH_GAMMA_COUNT] = {
    D2D1BRUSH_GAMMA_2_2,
    D2D1BRUSH_GAMMA_1_0
};
D2D1_GAMMA D2D1BrushGammaToD2D1[D2D1BRUSH_GAMMA_COUNT] = {
    D2D1_GAMMA_2_2,
    D2D1_GAMMA_1_0
};

D2D1BrushWrapMode D2D1BrushWrapModeFromD2D1[D2D1BRUSH_WRAPMODE_COUNT] = {
    D2D1BRUSH_WRAPMODE_CLAMP,
    D2D1BRUSH_WRAPMODE_REPEAT,
    D2D1BRUSH_WRAPMODE_MIRROR
};
D2D1_EXTEND_MODE D2D1BrushWrapModeToD2D1[D2D1BRUSH_WRAPMODE_COUNT] = {
    D2D1_EXTEND_MODE_CLAMP,
    D2D1_EXTEND_MODE_WRAP,
    D2D1_EXTEND_MODE_MIRROR
};

Void D2D1BrushDesc::ConvertFrom( const __D2D1_BRUSH_PROPERTIES * pDesc )
{
    fOpacity = pDesc->opacity;
    matTransform.f00 = pDesc->transform._11;
    matTransform.f01 = pDesc->transform._12;
    matTransform.f10 = pDesc->transform._21;
    matTransform.f11 = pDesc->transform._22;
    matTransform.f20 = pDesc->transform._31;
    matTransform.f21 = pDesc->transform._32;
}
Void D2D1BrushDesc::ConvertTo( __D2D1_BRUSH_PROPERTIES * outDesc ) const
{
    outDesc->opacity = fOpacity;
    outDesc->transform._11 = matTransform.f00;
    outDesc->transform._12 = matTransform.f01;
    outDesc->transform._21 = matTransform.f10;
    outDesc->transform._22 = matTransform.f11;
    outDesc->transform._31 = matTransform.f20;
    outDesc->transform._32 = matTransform.f21;
}

Void D2D1BrushLinearGradientDesc::ConvertFrom( const __D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES * pDesc )
{
    ptStart.fX = pDesc->startPoint.x;
    ptStart.fY = pDesc->startPoint.y;
    ptEnd.fX = pDesc->endPoint.x;
    ptEnd.fY = pDesc->endPoint.y;
}
Void D2D1BrushLinearGradientDesc::ConvertTo( __D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES * outDesc ) const
{
    outDesc->startPoint.x = ptStart.fX;
    outDesc->startPoint.y = ptStart.fY;
    outDesc->endPoint.x = ptEnd.fX;
    outDesc->endPoint.y = ptEnd.fY;
}

Void D2D1BrushRadialGradientDesc::ConvertFrom( const __D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES * pDesc )
{
    ptCenter.fX = pDesc->center.x;
    ptCenter.fY = pDesc->center.y;
    ptOffset.fX = pDesc->gradientOriginOffset.x;
    ptOffset.fY = pDesc->gradientOriginOffset.y;
    fRadiusX = pDesc->radiusX;
    fRadiusY = pDesc->radiusY;
}
Void D2D1BrushRadialGradientDesc::ConvertTo( __D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES * outDesc ) const
{
    outDesc->center.x = ptCenter.fX;
    outDesc->center.y = ptCenter.fY;
    outDesc->gradientOriginOffset.x = ptOffset.fX;
    outDesc->gradientOriginOffset.y = ptOffset.fY;
    outDesc->radiusX = fRadiusX;
    outDesc->radiusY = fRadiusY;
}

Void D2D1BrushBitmapDesc::ConvertFrom( const __D2D1_BITMAP_BRUSH_PROPERTIES * pDesc )
{
    iInterpolationMode = D2D1BitmapInterpolationModeFromD2D1[pDesc->interpolationMode];
    iWrapModeX = D2D1BrushWrapModeFromD2D1[pDesc->extendModeX];
    iWrapModeY = D2D1BrushWrapModeFromD2D1[pDesc->extendModeY];
}
Void D2D1BrushBitmapDesc::ConvertTo( __D2D1_BITMAP_BRUSH_PROPERTIES * outDesc ) const
{
    outDesc->interpolationMode = D2D1BitmapInterpolationModeToD2D1[iInterpolationMode];
    outDesc->extendModeX = D2D1BrushWrapModeToD2D1[iWrapModeX];
    outDesc->extendModeY = D2D1BrushWrapModeToD2D1[iWrapModeY];
}

/////////////////////////////////////////////////////////////////////////////////
// D2D1Mesh definitions

/////////////////////////////////////////////////////////////////////////////////
// D2D1Geometry definitions
D2D1GeometryRelation D2D1GeometryRelationFromD2D1[D2D1GEOMETRY_RELATION_COUNT] = {
    D2D1GEOMETRY_RELATION_UNKNOWN,
    D2D1GEOMETRY_RELATION_DISJOINT,
    D2D1GEOMETRY_RELATION_ISCONTAINED,
    D2D1GEOMETRY_RELATION_CONTAINS,
    D2D1GEOMETRY_RELATION_OVERLAP
};
D2D1_GEOMETRY_RELATION D2D1GeometryRelationToD2D1[D2D1GEOMETRY_RELATION_COUNT] = {
    D2D1_GEOMETRY_RELATION_UNKNOWN,
    D2D1_GEOMETRY_RELATION_DISJOINT,
    D2D1_GEOMETRY_RELATION_IS_CONTAINED,
    D2D1_GEOMETRY_RELATION_CONTAINS,
    D2D1_GEOMETRY_RELATION_OVERLAP
};

D2D1GeometryCombineMode D2D1GeometryCombineModeFromD2D1[D2D1GEOMETRY_COMBINE_COUNT] = {
    D2D1GEOMETRY_COMBINE_UNION,
    D2D1GEOMETRY_COMBINE_INTERSECT,
    D2D1GEOMETRY_COMBINE_XOR,
    D2D1GEOMETRY_COMBINE_EXCLUDE
};
D2D1_COMBINE_MODE D2D1GeometryCombineModeToD2D1[D2D1GEOMETRY_COMBINE_COUNT] = {
    D2D1_COMBINE_MODE_UNION,
    D2D1_COMBINE_MODE_INTERSECT,
    D2D1_COMBINE_MODE_EXCLUDE,
    D2D1_COMBINE_MODE_XOR
};

D2D1GeometrySimplifyOption D2D1GeometrySimplifyOptionFromD2D1[D2D1GEOMETRY_SIMPLIFY_COUNT] = {
    D2D1GEOMETRY_SIMPLIFY_CUBICS_AND_LINES,
    D2D1GEOMETRY_SIMPLIFY_LINES_ONLY
};
D2D1_GEOMETRY_SIMPLIFICATION_OPTION D2D1GeometrySimplifyOptionToD2D1[D2D1GEOMETRY_SIMPLIFY_COUNT] = {
    D2D1_GEOMETRY_SIMPLIFICATION_OPTION_CUBICS_AND_LINES,
    D2D1_GEOMETRY_SIMPLIFICATION_OPTION_LINES
};

D2D1GeometryFillMode D2D1GeometryFillModeFromD2D1[D2D1GEOMETRY_FILL_COUNT] = {
    D2D1GEOMETRY_FILL_ALTERNATE,
    D2D1GEOMETRY_FILL_WINDING
};
D2D1_FILL_MODE D2D1GeometryFillModeToD2D1[D2D1GEOMETRY_FILL_COUNT] = {
    D2D1_FILL_MODE_ALTERNATE,
    D2D1_FILL_MODE_WINDING
};

D2D1GeometrySegmentFlag D2D1GeometrySegmentFlagFromD2D1[D2D1GEOMETRY_SEGMENT_COUNT] = {
    D2D1GEOMETRY_SEGMENT_NONE,
    D2D1GEOMETRY_SEGMENT_FORCE_UNSTROKED,
    D2D1GEOMETRY_SEGMENT_FORCE_ROUND_LINE_JOIN
};
D2D1_PATH_SEGMENT D2D1GeometrySegmentFlagToD2D1[D2D1GEOMETRY_SEGMENT_COUNT] = {
    D2D1_PATH_SEGMENT_NONE,
    D2D1_PATH_SEGMENT_FORCE_UNSTROKED,
    D2D1_PATH_SEGMENT_FORCE_ROUND_LINE_JOIN
};

/////////////////////////////////////////////////////////////////////////////////
// D2D1Layer definitions
D2D1LayerOptions D2D1LayerOptionsFromD2D1[D2D1LAYER_OPTION_COUNT] = {
    D2D1LAYER_OPTION_NONE,
    D2D1LAYER_OPTION_CLEARTYPE_INIT
};
D2D1_LAYER_OPTIONS D2D1LayerOptionsToD2D1[D2D1LAYER_OPTION_COUNT] = {
    D2D1_LAYER_OPTIONS_NONE,
    D2D1_LAYER_OPTIONS_INITIALIZE_FOR_CLEARTYPE
};

/////////////////////////////////////////////////////////////////////////////////
// D2D1Text definitions
D2D1FontWeight D2D1FontWeightFromD2D1( DWRITE_FONT_WEIGHT iD2D1FontWeight ) {
    if ( iD2D1FontWeight == DWRITE_FONT_WEIGHT_EXTRA_BLACK )
        return D2D1FONT_WEIGHT_EXTRA_BLACK;
    return (D2D1FontWeight)( (iD2D1FontWeight / 100) - 1 );
}
DWRITE_FONT_WEIGHT D2D1FontWeightToD2D1[D2D1FONT_WEIGHT_COUNT] = {
    DWRITE_FONT_WEIGHT_THIN,
    DWRITE_FONT_WEIGHT_EXTRA_LIGHT,
    DWRITE_FONT_WEIGHT_LIGHT,
    DWRITE_FONT_WEIGHT_NORMAL,
    DWRITE_FONT_WEIGHT_MEDIUM,
    DWRITE_FONT_WEIGHT_SEMI_BOLD,
    DWRITE_FONT_WEIGHT_BOLD,
    DWRITE_FONT_WEIGHT_EXTRA_BOLD,
    DWRITE_FONT_WEIGHT_BLACK,
    DWRITE_FONT_WEIGHT_EXTRA_BLACK,
};

D2D1FontStyle D2D1FontStyleFromD2D1[D2D1FONT_STYLE_COUNT] = {
    D2D1FONT_STYLE_NORMAL,
    D2D1FONT_STYLE_OBLIQUE,
    D2D1FONT_STYLE_ITALIC
};
DWRITE_FONT_STYLE D2D1FontStyleToD2D1[D2D1FONT_STYLE_COUNT] = {
    DWRITE_FONT_STYLE_NORMAL,
    DWRITE_FONT_STYLE_OBLIQUE,
    DWRITE_FONT_STYLE_ITALIC
};

D2D1FontStretch D2D1FontStretchFromD2D1[D2D1FONT_STRETCH_COUNT] = {
    D2D1FONT_STRETCH_UNDEFINED,
    D2D1FONT_STRETCH_ULTRACONDENSED,
    D2D1FONT_STRETCH_EXTRACONDENSED,
    D2D1FONT_STRETCH_CONDENSED,
    D2D1FONT_STRETCH_SEMICONDENSED,
    D2D1FONT_STRETCH_NORMAL,
    D2D1FONT_STRETCH_SEMIEXPANDED,
    D2D1FONT_STRETCH_EXPANDED,
    D2D1FONT_STRETCH_EXTRAEXPANDED,
    D2D1FONT_STRETCH_ULTRAEXPANDED
};
DWRITE_FONT_STRETCH D2D1FontStretchToD2D1[D2D1FONT_STRETCH_COUNT] = {
    DWRITE_FONT_STRETCH_UNDEFINED,
    DWRITE_FONT_STRETCH_ULTRA_CONDENSED,
    DWRITE_FONT_STRETCH_EXTRA_CONDENSED,
    DWRITE_FONT_STRETCH_CONDENSED,
    DWRITE_FONT_STRETCH_SEMI_CONDENSED,
    DWRITE_FONT_STRETCH_NORMAL,
    DWRITE_FONT_STRETCH_SEMI_EXPANDED,
    DWRITE_FONT_STRETCH_EXPANDED,
    DWRITE_FONT_STRETCH_EXTRA_EXPANDED,
    DWRITE_FONT_STRETCH_ULTRA_EXPANDED
};

D2D1FontInfoStringID D2D1FontInfoStringIDFromD2D1[D2D1FONT_INFOSTRING_COUNT] = {
    D2D1FONT_INFOSTRING_NONE,
    D2D1FONT_INFOSTRING_COPYRIGHT,
    D2D1FONT_INFOSTRING_VERSION,
    D2D1FONT_INFOSTRING_TRADEMARK,
    D2D1FONT_INFOSTRING_MANUFACTURER,
    D2D1FONT_INFOSTRING_DESIGNER,
    D2D1FONT_INFOSTRING_DESIGNER_URL,
    D2D1FONT_INFOSTRING_DESCRIPTION,
    D2D1FONT_INFOSTRING_VENDOR_URL,
    D2D1FONT_INFOSTRING_LICENSE_DESCRIPTION,
    D2D1FONT_INFOSTRING_LICENSE_URL,
    D2D1FONT_INFOSTRING_WIN32_FAMILYNAMES,
    D2D1FONT_INFOSTRING_WIN32_SUBFAMILYNAMES,
    D2D1FONT_INFOSTRING_PREF_FAMILYNAMES,
    D2D1FONT_INFOSTRING_PREF_SUBFAMILYNAMES,
    D2D1FONT_INFOSTRING_SAMPLE_TEXT
};
DWRITE_INFORMATIONAL_STRING_ID D2D1FontInfoStringIDToD2D1[D2D1FONT_INFOSTRING_COUNT] = {
    DWRITE_INFORMATIONAL_STRING_NONE,
    DWRITE_INFORMATIONAL_STRING_COPYRIGHT_NOTICE,
    DWRITE_INFORMATIONAL_STRING_VERSION_STRINGS,
    DWRITE_INFORMATIONAL_STRING_TRADEMARK,
    DWRITE_INFORMATIONAL_STRING_MANUFACTURER,
    DWRITE_INFORMATIONAL_STRING_DESIGNER,
    DWRITE_INFORMATIONAL_STRING_DESIGNER_URL,
    DWRITE_INFORMATIONAL_STRING_DESCRIPTION,
    DWRITE_INFORMATIONAL_STRING_FONT_VENDOR_URL,
    DWRITE_INFORMATIONAL_STRING_LICENSE_DESCRIPTION,
    DWRITE_INFORMATIONAL_STRING_LICENSE_INFO_URL,
    DWRITE_INFORMATIONAL_STRING_WIN32_FAMILY_NAMES,
    DWRITE_INFORMATIONAL_STRING_WIN32_SUBFAMILY_NAMES,
    DWRITE_INFORMATIONAL_STRING_PREFERRED_FAMILY_NAMES,
    DWRITE_INFORMATIONAL_STRING_PREFERRED_SUBFAMILY_NAMES,
    DWRITE_INFORMATIONAL_STRING_SAMPLE_TEXT
};

D2D1FontSimulation D2D1FontSimulationFromD2D1[D2D1FONT_SIMULATION_COUNT] = {
    D2D1FONT_SIMULATION_NONE,
    D2D1FONT_SIMULATION_BOLD,
    D2D1FONT_SIMULATION_OBLIQUE
};
DWRITE_FONT_SIMULATIONS D2D1FontSimulationToD2D1[D2D1FONT_SIMULATION_COUNT] = {
    DWRITE_FONT_SIMULATIONS_NONE,
    DWRITE_FONT_SIMULATIONS_BOLD,
    DWRITE_FONT_SIMULATIONS_OBLIQUE
};

Void D2D1FontMetrics::ConvertFrom( const DWRITE_FONT_METRICS * pDesc )
{
    iDesignUnitsPerEM = pDesc->designUnitsPerEm;
    iAscent = pDesc->ascent;
    iDescent = pDesc->descent;
    iLineGap = pDesc->lineGap;
    iCapHeight = pDesc->capHeight;
    iXHeight = pDesc->xHeight;
    iUnderlinePosition = pDesc->underlinePosition;
    iUnderlineThickness = pDesc->underlineThickness;
    iStrikethroughPosition = pDesc->strikethroughPosition;
    iStrikethroughThickness = pDesc->strikethroughThickness;
}

D2D1FontFaceType D2D1FontFaceTypeFromD2D1[D2D1FONT_FACE_COUNT] = {
    D2D1FONT_FACE_CFF,
    D2D1FONT_FACE_TRUETYPE,
    D2D1FONT_FACE_TRUETYPE_COLLECTION,
    D2D1FONT_FACE_TYPE1,
    D2D1FONT_FACE_VECTOR,
    D2D1FONT_FACE_BITMAP,
    D2D1FONT_FACE_UNKNOWN
};
DWRITE_FONT_FACE_TYPE D2D1FontFaceTypeToD2D1[D2D1FONT_FACE_COUNT] = {
    DWRITE_FONT_FACE_TYPE_CFF,
    DWRITE_FONT_FACE_TYPE_TRUETYPE,
    DWRITE_FONT_FACE_TYPE_TRUETYPE_COLLECTION,
    DWRITE_FONT_FACE_TYPE_TYPE1,
    DWRITE_FONT_FACE_TYPE_VECTOR,
    DWRITE_FONT_FACE_TYPE_BITMAP,
    DWRITE_FONT_FACE_TYPE_UNKNOWN
};

D2D1TextMeasuringMode D2D1TextMeasuringModeFromD2D1[D2D1TEXT_MEASURING_COUNT] = {
    D2D1TEXT_MEASURING_NATURAL,
    D2D1TEXT_MEASURING_GDI_CLASSIC,
    D2D1TEXT_MEASURING_GDI_NATURAL
};
DWRITE_MEASURING_MODE D2D1TextMeasuringModeToD2D1[D2D1TEXT_MEASURING_COUNT] = {
    DWRITE_MEASURING_MODE_NATURAL,
    DWRITE_MEASURING_MODE_GDI_CLASSIC,
    DWRITE_MEASURING_MODE_GDI_NATURAL
};

Void D2D1FontGlyphMetrics::ConvertFrom( const DWRITE_GLYPH_METRICS * pDesc )
{
    iVerticalOriginY = pDesc->verticalOriginY;
    iLeftSideBearing = pDesc->leftSideBearing;
    iTopSideBearing = pDesc->topSideBearing;
    iRightSideBearing = pDesc->rightSideBearing;
    iBottomSideBearing = pDesc->bottomSideBearing;
    iAdvanceWidth = pDesc->advanceWidth;
    iAdvanceHeight = pDesc->advanceHeight;
}

D2D1TextFlowDirection D2D1TextFlowDirectionFromD2D1[D2D1TEXT_FLOWDIRECTION_COUNT] = {
    D2D1TEXT_FLOWDIRECTION_TOP_BOTTOM
};
DWRITE_FLOW_DIRECTION D2D1TextFlowDirectionToD2D1[D2D1TEXT_FLOWDIRECTION_COUNT] = {
    DWRITE_FLOW_DIRECTION_TOP_TO_BOTTOM
};

D2D1TextReadingDirection D2D1TextReadingDirectionFromD2D1[D2D1TEXT_READINGDIRECTION_COUNT] = {
    D2D1TEXT_READINGDIRECTION_LEFT_RIGHT,
    D2D1TEXT_READINGDIRECTION_RIGHT_LEFT
};
DWRITE_READING_DIRECTION D2D1TextReadingDirectionToD2D1[D2D1TEXT_READINGDIRECTION_COUNT] = {
    DWRITE_READING_DIRECTION_LEFT_TO_RIGHT,
    DWRITE_READING_DIRECTION_RIGHT_TO_LEFT
};

D2D1TextLineSpacingMethod D2D1TextLineSpacingMethodFromD2D1[D2D1TEXT_LINESPACING_COUNT] = {
    D2D1TEXT_LINESPACING_DEFAULT,
    D2D1TEXT_LINESPACING_UNIFORM
};
DWRITE_LINE_SPACING_METHOD D2D1TextLineSpacingMethodToD2D1[D2D1TEXT_LINESPACING_COUNT] = {
    DWRITE_LINE_SPACING_METHOD_DEFAULT,
    DWRITE_LINE_SPACING_METHOD_UNIFORM
};

D2D1TextAlignment D2D1TextAlignmentFromD2D1[D2D1TEXT_ALIGNMENT_COUNT] = {
    D2D1TEXT_ALIGNMENT_LEADING,
    D2D1TEXT_ALIGNMENT_TRAILING,
    D2D1TEXT_ALIGNMENT_CENTER
};
DWRITE_TEXT_ALIGNMENT D2D1TextAlignmentToD2D1[D2D1TEXT_ALIGNMENT_COUNT] = {
    DWRITE_TEXT_ALIGNMENT_LEADING,
    DWRITE_TEXT_ALIGNMENT_TRAILING,
    DWRITE_TEXT_ALIGNMENT_CENTER
};

D2D1TextParagraphAlignment D2D1TextParagraphAlignmentFromD2D1[D2D1TEXT_PARAGRAPHALIGNMENT_COUNT] = {
    D2D1TEXT_PARAGRAPHALIGNMENT_NEAR,
    D2D1TEXT_PARAGRAPHALIGNMENT_FAR,
    D2D1TEXT_PARAGRAPHALIGNMENT_CENTER
};
DWRITE_PARAGRAPH_ALIGNMENT D2D1TextParagraphAlignmentToD2D1[D2D1TEXT_PARAGRAPHALIGNMENT_COUNT] = {
    DWRITE_PARAGRAPH_ALIGNMENT_NEAR,
    DWRITE_PARAGRAPH_ALIGNMENT_FAR,
    DWRITE_PARAGRAPH_ALIGNMENT_CENTER
};

D2D1TextTrimmingGranularity D2D1TextTrimmingGranularityFromD2D1[D2D1TEXT_TRIMMINGGRANULARITY_COUNT] = {
    D2D1TEXT_TRIMMINGGRANULARITY_NONE,
    D2D1TEXT_TRIMMINGGRANULARITY_CHARACTER,
    D2D1TEXT_TRIMMINGGRANULARITY_WORD
};
DWRITE_TRIMMING_GRANULARITY D2D1TextTrimmingGranularityToD2D1[D2D1TEXT_TRIMMINGGRANULARITY_COUNT] = {
    DWRITE_TRIMMING_GRANULARITY_NONE,
    DWRITE_TRIMMING_GRANULARITY_CHARACTER,
    DWRITE_TRIMMING_GRANULARITY_WORD
};

D2D1TextWordWrapping D2D1TextWordWrappingFromD2D1[D2D1TEXT_WORDWRAPPING_COUNT] = {
    D2D1TEXT_WORDWRAPPING_WRAP,
    D2D1TEXT_WORDWRAPPING_NONE
};
DWRITE_WORD_WRAPPING D2D1TextWordWrappingToD2D1[D2D1TEXT_WORDWRAPPING_COUNT] = {
    DWRITE_WORD_WRAPPING_NO_WRAP,
    DWRITE_WORD_WRAPPING_WRAP
};

D2D1TextBreakCondition D2D1TextBreakConditionFromD2D1[D2D1TEXT_BREAKCONDITION_COUNT] = {
    D2D1TEXT_BREAKCONDITION_NEUTRAL,
    D2D1TEXT_BREAKCONDITION_CAN_BREAK,
    D2D1TEXT_BREAKCONDITION_MAY_NOT_BREAK,
    D2D1TEXT_BREAKCONDITION_MUST_BREAK
};
DWRITE_BREAK_CONDITION D2D1TextBreakConditionToD2D1[D2D1TEXT_BREAKCONDITION_COUNT] = {
    DWRITE_BREAK_CONDITION_NEUTRAL,
    DWRITE_BREAK_CONDITION_CAN_BREAK,
    DWRITE_BREAK_CONDITION_MAY_NOT_BREAK,
    DWRITE_BREAK_CONDITION_MUST_BREAK
};

Void D2D1TextInlineObjectMetrics::ConvertFrom( const DWRITE_INLINE_OBJECT_METRICS * pDesc )
{
    fWidth = pDesc->width;
    fHeight = pDesc->height;
    fBaseline = pDesc->baseline;
    bSupportSideways = (pDesc->supportsSideways != FALSE);
}
Void D2D1TextInlineObjectMetrics::ConvertTo( DWRITE_INLINE_OBJECT_METRICS * outDesc )
{
    outDesc->width = fWidth;
    outDesc->height = fHeight;
    outDesc->baseline = fBaseline;
    outDesc->supportsSideways = (bSupportSideways) ? TRUE : FALSE;
}

Void D2D1TextOverhangMetrics::ConvertFrom( const DWRITE_OVERHANG_METRICS * pDesc )
{
    fLeft = pDesc->left;
    fTop = pDesc->top;
    fRight = pDesc->right;
    fBottom = pDesc->bottom;
}
Void D2D1TextOverhangMetrics::ConvertTo( DWRITE_OVERHANG_METRICS * outDesc )
{
    outDesc->left = fLeft;
    outDesc->top = fTop;
    outDesc->right = fRight;
    outDesc->bottom = fBottom;
}

Void D2D1TextMetrics::ConvertFrom( const DWRITE_TEXT_METRICS * pDesc )
{
    fLeft = pDesc->left;
    fTop = pDesc->top;
    fWidth = pDesc->width;
    fWidthIncludingTrailingWhitespaces = pDesc->widthIncludingTrailingWhitespace;
    fHeight = pDesc->height;
    fLayoutWidth = pDesc->layoutWidth;
    fLayoutHeight = pDesc->layoutHeight;
    iMaxBidiReorderingDepth = pDesc->maxBidiReorderingDepth;
    iLineCount = pDesc->lineCount;
}
