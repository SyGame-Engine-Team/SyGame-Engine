/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/2D/Resources/GPU2DStrokeStyle.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU 2D-Resource : StrokeStyles
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
#ifndef SCARAB_ENGINE_RENDERING_2D_RESOURCES_GPU2DSTROKESTYLE_H
#define SCARAB_ENGINE_RENDERING_2D_RESOURCES_GPU2DSTROKESTYLE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../Lib/Error/ErrorManager.h"

#include "../../DeviceRenderer.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
enum GPU2DStrokeCapStyle {
    GPU2DSTROKE_CAPSTYLE_FLAT     = DEVICE2DSTROKE_CAPSTYLE_FLAT,
    GPU2DSTROKE_CAPSTYLE_SQUARE   = DEVICE2DSTROKE_CAPSTYLE_SQUARE,
    GPU2DSTROKE_CAPSTYLE_ROUND    = DEVICE2DSTROKE_CAPSTYLE_ROUND,
    GPU2DSTROKE_CAPSTYLE_TRIANGLE = DEVICE2DSTROKE_CAPSTYLE_TRIANGLE
};

enum GPU2DStrokeLineJoin {
    GPU2DSTROKE_LINEJOIN_MITER          = DEVICE2DSTROKE_LINEJOIN_MITER,
    GPU2DSTROKE_LINEJOIN_BEVEL          = DEVICE2DSTROKE_LINEJOIN_BEVEL,
    GPU2DSTROKE_LINEJOIN_ROUND          = DEVICE2DSTROKE_LINEJOIN_ROUND,
    GPU2DSTROKE_LINEJOIN_MITER_OR_BEVEL = DEVICE2DSTROKE_LINEJOIN_MITER_OR_BEVEL
};

enum GPU2DStrokeDashStyle {
    GPU2DSTROKE_DASHSTYLE_SOLID      = DEVICE2DSTROKE_DASHSTYLE_SOLID,
    GPU2DSTROKE_DASHSTYLE_DASH       = DEVICE2DSTROKE_DASHSTYLE_DASH,
    GPU2DSTROKE_DASHSTYLE_DOT        = DEVICE2DSTROKE_DASHSTYLE_DOT,
    GPU2DSTROKE_DASHSTYLE_DASHDOT    = DEVICE2DSTROKE_DASHSTYLE_DASHDOT,
    GPU2DSTROKE_DASHSTYLE_DASHDOTDOT = DEVICE2DSTROKE_DASHSTYLE_DASHDOTDOT,
    GPU2DSTROKE_DASHSTYLE_CUSTOM     = DEVICE2DSTROKE_DASHSTYLE_CUSTOM
};

typedef struct _gpu2d_strokestyle_desc {
    GPU2DStrokeCapStyle iStartCap;
    GPU2DStrokeCapStyle iEndCap;
    GPU2DStrokeCapStyle iDashCap;
    GPU2DStrokeLineJoin iLineJoin;
    Float fMiterLimit;
    GPU2DStrokeDashStyle iDashStyle;
    Float fDashOffset;
} GPU2DStrokeStyleDesc;

// Prototypes
class GPU2DContext;
class GPU2DGeometry;

/////////////////////////////////////////////////////////////////////////////////
// The GPU2DStrokeStyle class
class GPU2DStrokeStyle
{
public:
    GPU2DStrokeStyle();
    ~GPU2DStrokeStyle();

    // Binding
    inline Bool IsBound() const;

    inline Void Bind( const GPU2DStrokeStyleDesc * pDesc, const Float * arrDashes = NULL, UInt iDashCount = 0 );
    inline Void UnBind();

    // Methods
    inline GPU2DStrokeCapStyle GetStartCap() const;
    inline GPU2DStrokeCapStyle GetEndCap() const;
    inline GPU2DStrokeCapStyle GetDashCap() const;

    inline GPU2DStrokeLineJoin GetLineJoin() const;
    inline Float GetMiterLimit() const;

    inline GPU2DStrokeDashStyle GetDashStyle() const;
    inline Float GetDashOffset() const;

    inline Void GetDesc( GPU2DStrokeStyleDesc * outDesc ) const;

    inline UInt GetDashCount() const;
    inline Void GetDashes( Float * outDashes, UInt iMaxDashes ) const;

private:
    friend class GPU2DContext;
    friend class GPU2DGeometry;
    
    // Device data
    Device2DStrokeStyle m_hDevice2DStrokeStyle;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GPU2DStrokeStyle.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RENDERING_2D_RESOURCES_GPU2DSTROKESTYLE_H
