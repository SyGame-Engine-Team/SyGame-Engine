/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/2D/Resources/D2D1StrokeStyle.cpp
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
// Third-Party Includes
#pragma warning(disable:4005)

#define WIN32_LEAN_AND_MEAN
#include <d2d1.h>

#undef DebugAssert

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "D2D1StrokeStyle.h"

#include "../D2D1RenderingContext.h"

/////////////////////////////////////////////////////////////////////////////////
// D2D1StrokeStyle implementation
D2D1StrokeStyle::D2D1StrokeStyle()
{
    m_pStrokeStyle = NULL;
}
D2D1StrokeStyle::~D2D1StrokeStyle()
{
    if ( IsCreated() )
        Destroy();
}

Void D2D1StrokeStyle::Create( const D2D1StrokeStyleDesc * pDesc, const Float * arrDashes, UInt iDashCount )
{
    DebugAssert( !(IsCreated()) );

    D2D1_STROKE_STYLE_PROPERTIES hStrokeStyleProperties;
    pDesc->ConvertTo( &hStrokeStyleProperties );

    if ( hStrokeStyleProperties.dashStyle == D2D1_DASH_STYLE_CUSTOM ) {
        DebugAssert( arrDashes != NULL && iDashCount > 0 );
    } else {
        DebugAssert( arrDashes == NULL && iDashCount == 0 );
    }

    m_pStrokeStyle = NULL;
    HRESULT hRes = D2D1RenderingContext::sm_pD2D1Factory->CreateStrokeStyle( &hStrokeStyleProperties, arrDashes, iDashCount, &m_pStrokeStyle );
    DebugAssert( hRes == S_OK && m_pStrokeStyle != NULL );
}
Void D2D1StrokeStyle::Destroy()
{
    DebugAssert( IsCreated() );

    m_pStrokeStyle->Release();
    m_pStrokeStyle = NULL;
}

D2D1StrokeCapStyle D2D1StrokeStyle::GetStartCap() const
{
    DebugAssert( IsCreated() );

    return D2D1StrokeCapStyleFromD2D1[m_pStrokeStyle->GetStartCap()];
}
D2D1StrokeCapStyle D2D1StrokeStyle::GetEndCap() const
{
    DebugAssert( IsCreated() );

    return D2D1StrokeCapStyleFromD2D1[m_pStrokeStyle->GetEndCap()];
}
D2D1StrokeCapStyle D2D1StrokeStyle::GetDashCap() const
{
    DebugAssert( IsCreated() );

    return D2D1StrokeCapStyleFromD2D1[m_pStrokeStyle->GetDashCap()];
}

D2D1StrokeLineJoin D2D1StrokeStyle::GetLineJoin() const
{
    DebugAssert( IsCreated() );

    return D2D1StrokeLineJoinFromD2D1[m_pStrokeStyle->GetLineJoin()];
}
Float D2D1StrokeStyle::GetMiterLimit() const
{
    DebugAssert( IsCreated() );

    return m_pStrokeStyle->GetMiterLimit();
}

D2D1StrokeDashStyle D2D1StrokeStyle::GetDashStyle() const
{
    DebugAssert( IsCreated() );

    return D2D1StrokeDashStyleFromD2D1[m_pStrokeStyle->GetDashStyle()];
}
Float D2D1StrokeStyle::GetDashOffset() const
{
    DebugAssert( IsCreated() );

    return m_pStrokeStyle->GetDashOffset();
}

Void D2D1StrokeStyle::GetDesc( D2D1StrokeStyleDesc * outDesc ) const
{
    DebugAssert( IsCreated() );

    outDesc->iStartCap = D2D1StrokeCapStyleFromD2D1[m_pStrokeStyle->GetStartCap()];
    outDesc->iEndCap = D2D1StrokeCapStyleFromD2D1[m_pStrokeStyle->GetEndCap()];
    outDesc->iDashCap = D2D1StrokeCapStyleFromD2D1[m_pStrokeStyle->GetDashCap()];

    outDesc->iLineJoin = D2D1StrokeLineJoinFromD2D1[m_pStrokeStyle->GetLineJoin()];
    outDesc->fMiterLimit = m_pStrokeStyle->GetMiterLimit();

    outDesc->iDashStyle = D2D1StrokeDashStyleFromD2D1[m_pStrokeStyle->GetDashStyle()];
    outDesc->fDashOffset = m_pStrokeStyle->GetDashOffset();
}

UInt D2D1StrokeStyle::GetDashCount() const
{
    DebugAssert( IsCreated() );
    DebugAssert( m_pStrokeStyle->GetDashStyle() == D2D1_DASH_STYLE_CUSTOM );

    return m_pStrokeStyle->GetDashesCount();
}
Void D2D1StrokeStyle::GetDashes( Float * outDashes, UInt iMaxDashes ) const
{
    DebugAssert( IsCreated() );
    DebugAssert( m_pStrokeStyle->GetDashStyle() == D2D1_DASH_STYLE_CUSTOM );
    
    m_pStrokeStyle->GetDashes( outDashes, iMaxDashes );
}

