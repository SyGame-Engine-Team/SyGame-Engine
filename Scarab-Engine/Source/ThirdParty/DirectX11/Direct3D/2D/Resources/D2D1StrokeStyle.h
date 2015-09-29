/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/2D/Resources/D2D1StrokeStyle.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : D2D1 Dev-Ind Resource : Stroke Styles.
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
#ifndef SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_2D_RESOURCES_D2D1STROKESTYLE_H
#define SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_2D_RESOURCES_D2D1STROKESTYLE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../D2D1Mappings.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Prototypes
class D2D1RenderingContext;
class D2D1Geometry;

/////////////////////////////////////////////////////////////////////////////////
// The D2D1StrokeStyle class
class D2D1StrokeStyle
{
public:
    D2D1StrokeStyle();
    ~D2D1StrokeStyle();

    // Deferred construction
    inline Bool IsCreated() const;

    Void Create( const D2D1StrokeStyleDesc * pDesc, const Float * arrDashes = NULL, UInt iDashCount = 0 );
    Void Destroy();

    // Methods
    D2D1StrokeCapStyle GetStartCap() const;
    D2D1StrokeCapStyle GetEndCap() const;
    D2D1StrokeCapStyle GetDashCap() const;

    D2D1StrokeLineJoin GetLineJoin() const;
    Float GetMiterLimit() const;

    D2D1StrokeDashStyle GetDashStyle() const;
    Float GetDashOffset() const;

    Void GetDesc( D2D1StrokeStyleDesc * outDesc ) const;

    UInt GetDashCount() const;
    Void GetDashes( Float * outDashes, UInt iMaxDashes ) const;

private:
    friend class D2D1Geometry;
    friend class D2D1RenderingContext;

    ID2D1StrokeStyle * m_pStrokeStyle;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "D2D1StrokeStyle.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_2D_RESOURCES_D2D1STROKESTYLE_H

