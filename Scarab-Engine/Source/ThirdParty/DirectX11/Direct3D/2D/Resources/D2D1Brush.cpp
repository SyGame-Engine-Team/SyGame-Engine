/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/2D/Resources/D2D1Brush.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : D2D1 Dev-Dep Resource : Brushes.
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
#include "D2D1Brush.h"

#include "../D2D1RenderingContext.h"

/////////////////////////////////////////////////////////////////////////////////
// D2D1Brush implementation
D2D1Brush::D2D1Brush( D2D1RenderingContext * pContext2D )
{
    m_pContext2D = pContext2D;

    m_hBrushProperties.opacity = 1.0f;
    m_hBrushProperties.transform._11 = 1.0f;
    m_hBrushProperties.transform._12 = 0.0f;
    m_hBrushProperties.transform._21 = 0.0f;
    m_hBrushProperties.transform._22 = 1.0f;
    m_hBrushProperties.transform._31 = 0.0f;
    m_hBrushProperties.transform._32 = 0.0f;

    m_pBrush = NULL;

    m_bTemporaryDestroyed = false;
}
D2D1Brush::~D2D1Brush()
{
    // nothing to do
}

Void D2D1Brush::Destroy()
{
    DebugAssert( IsCreated() );

    if ( m_bTemporaryDestroyed )
        m_bTemporaryDestroyed = false;
    else
        _NakedDestroy();
}

Void D2D1Brush::OnDestroyDevice()
{
    DebugAssert( !m_bTemporaryDestroyed );

    if ( m_pBrush != NULL ) {
        _NakedDestroy();
        m_bTemporaryDestroyed = true;
    }
}
Void D2D1Brush::OnRestoreDevice()
{
    DebugAssert( m_pBrush == NULL );

    if ( m_bTemporaryDestroyed ) {
        _NakedCreate();
        m_bTemporaryDestroyed = false;
    }
}

Void D2D1Brush::SetOpacity( Float fOpacity )
{
    m_hBrushProperties.opacity = fOpacity;

    if ( IsCreated() )
        m_pBrush->SetOpacity( m_hBrushProperties.opacity );
}

Void D2D1Brush::SetTransform( const D2D1Matrix32 * pTransform )
{
    m_hBrushProperties.transform._11 = pTransform->f00;
    m_hBrushProperties.transform._12 = pTransform->f01;
    m_hBrushProperties.transform._21 = pTransform->f10;
    m_hBrushProperties.transform._22 = pTransform->f11;
    m_hBrushProperties.transform._31 = pTransform->f20;
    m_hBrushProperties.transform._32 = pTransform->f21;

    if ( IsCreated() )
        m_pBrush->SetTransform( (const D2D1_MATRIX_3X2_F *)&(m_hBrushProperties.transform) );
}

Void D2D1Brush::SetDesc( const D2D1BrushDesc * pDesc )
{
    pDesc->ConvertTo( &m_hBrushProperties );

    if ( IsCreated() ) {
        m_pBrush->SetOpacity( m_hBrushProperties.opacity );
        m_pBrush->SetTransform( (const D2D1_MATRIX_3X2_F *)&(m_hBrushProperties.transform) );
    }
}

/////////////////////////////////////////////////////////////////////////////////
// D2D1SolidColorBrush implementation
D2D1SolidColorBrush::D2D1SolidColorBrush( D2D1RenderingContext * pContext2D ):
    D2D1Brush( pContext2D )
{
    m_hBrushColor.R = 0.0f;
    m_hBrushColor.G = 0.0f;
    m_hBrushColor.B = 0.0f;
    m_hBrushColor.A = 1.0f;

    m_pSolidColorBrush = NULL;
}
D2D1SolidColorBrush::~D2D1SolidColorBrush()
{
    if ( IsCreated() )
        Destroy();
}

Void D2D1SolidColorBrush::Create()
{
    DebugAssert( !(IsCreated()) );

    _NakedCreate();
}

Void D2D1SolidColorBrush::SetColor( const D2D1Color * pColor )
{
    m_hBrushColor.R = pColor->R;
    m_hBrushColor.G = pColor->G;
    m_hBrushColor.B = pColor->B;
    m_hBrushColor.A = pColor->A;

    if ( IsCreated() )
        m_pSolidColorBrush->SetColor( (const D2D1_COLOR_F *)&m_hBrushColor );
}

/////////////////////////////////////////////////////////////////////////////////

Void D2D1SolidColorBrush::_NakedCreate()
{
    m_pSolidColorBrush = NULL;
    HRESULT hRes = m_pContext2D->m_pD2D1RenderingContext->CreateSolidColorBrush( (const D2D1_COLOR_F *)&m_hBrushColor,
                                                                                 (const D2D1_BRUSH_PROPERTIES *)&m_hBrushProperties, &m_pSolidColorBrush );
    DebugAssert( hRes == S_OK && m_pSolidColorBrush != NULL );

    m_pBrush = NULL;
    hRes = m_pSolidColorBrush->QueryInterface( __uuidof(ID2D1Brush), (Void**)&m_pBrush );
    DebugAssert( hRes == S_OK && m_pBrush != NULL );
}
Void D2D1SolidColorBrush::_NakedDestroy()
{
    m_pBrush->Release();
    m_pBrush = NULL;

    m_pSolidColorBrush->Release();
    m_pSolidColorBrush = NULL;
}

/////////////////////////////////////////////////////////////////////////////////
// D2D1GradientBrush implementation
D2D1GradientBrush::D2D1GradientBrush( D2D1RenderingContext * pContext2D ):
    D2D1Brush( pContext2D )
{
    m_iGradientGamma = D2D1_GAMMA_2_2;
    m_iGradientExtendMode = D2D1_EXTEND_MODE_CLAMP;
    m_iGradientStopCount = 2;

    m_arrGradientStops[0].position = 0.0f;
    m_arrGradientStops[0].color.r = 0.0f;
    m_arrGradientStops[0].color.g = 0.0f;
    m_arrGradientStops[0].color.b = 0.0f;
    m_arrGradientStops[0].color.a = 1.0f;

    m_arrGradientStops[1].position = 1.0f;
    m_arrGradientStops[1].color.r = 1.0f;
    m_arrGradientStops[1].color.g = 1.0f;
    m_arrGradientStops[1].color.b = 1.0f;
    m_arrGradientStops[1].color.a = 1.0f;
}
D2D1GradientBrush::~D2D1GradientBrush()
{
    // nothing to do
}

Void D2D1GradientBrush::GetGradientStops( D2D1GradientStop * outGradientStops, UInt iMaxStops ) const
{
    DebugAssert( IsCreated() );
    DebugAssert( iMaxStops <= m_iGradientStopCount );

    for( UInt i = 0; i < iMaxStops; ++i ) {
        outGradientStops[i].fPosition = m_arrGradientStops[i].position;
        outGradientStops[i].fColor.R = m_arrGradientStops[i].color.r;
        outGradientStops[i].fColor.G = m_arrGradientStops[i].color.g;
        outGradientStops[i].fColor.B = m_arrGradientStops[i].color.b;
        outGradientStops[i].fColor.A = m_arrGradientStops[i].color.a;
    }
}

Void D2D1GradientBrush::GetGradientDesc( D2D1GradientDesc * outGradientDesc ) const
{
    DebugAssert( IsCreated() );

    outGradientDesc->iGamma = D2D1BrushGammaFromD2D1[m_iGradientGamma];
    outGradientDesc->iWrapMode = D2D1BrushWrapModeFromD2D1[m_iGradientExtendMode];
    outGradientDesc->iStopCount = m_iGradientStopCount;
    for( UInt i = 0; i < m_iGradientStopCount; ++i ) {
        outGradientDesc->arrGradientStops[i].fPosition = m_arrGradientStops[i].position;
        outGradientDesc->arrGradientStops[i].fColor.R = m_arrGradientStops[i].color.r;
        outGradientDesc->arrGradientStops[i].fColor.G = m_arrGradientStops[i].color.g;
        outGradientDesc->arrGradientStops[i].fColor.B = m_arrGradientStops[i].color.b;
        outGradientDesc->arrGradientStops[i].fColor.A = m_arrGradientStops[i].color.a;
    }
}

/////////////////////////////////////////////////////////////////////////////////

Void D2D1GradientBrush::_CreateGradient( const D2D1GradientDesc * pGradientDesc )
{
    DebugAssert( m_pGradientStopCollection == NULL );
    DebugAssert( pGradientDesc->iStopCount <= D2D1GRADIENT_MAX_STOPS );

    m_iGradientGamma = D2D1BrushGammaToD2D1[pGradientDesc->iGamma];
    m_iGradientExtendMode = D2D1BrushWrapModeToD2D1[pGradientDesc->iWrapMode];
    m_iGradientStopCount = pGradientDesc->iStopCount;
    for( UInt i = 0; i < m_iGradientStopCount; ++i ) {
        m_arrGradientStops[i].position = pGradientDesc->arrGradientStops[i].fPosition;
        m_arrGradientStops[i].color.r = pGradientDesc->arrGradientStops[i].fColor.R;
        m_arrGradientStops[i].color.g = pGradientDesc->arrGradientStops[i].fColor.G;
        m_arrGradientStops[i].color.b = pGradientDesc->arrGradientStops[i].fColor.B;
        m_arrGradientStops[i].color.a = pGradientDesc->arrGradientStops[i].fColor.A;
    }
}
Void D2D1GradientBrush::_DestroyGradient()
{
    DebugAssert( m_pGradientStopCollection != NULL );

    m_iGradientGamma = D2D1_GAMMA_2_2;
    m_iGradientExtendMode = D2D1_EXTEND_MODE_CLAMP;
    m_iGradientStopCount = 2;

    m_arrGradientStops[0].position = 0.0f;
    m_arrGradientStops[0].color.r = 0.0f;
    m_arrGradientStops[0].color.g = 0.0f;
    m_arrGradientStops[0].color.b = 0.0f;
    m_arrGradientStops[0].color.a = 1.0f;

    m_arrGradientStops[1].position = 1.0f;
    m_arrGradientStops[0].color.r = 1.0f;
    m_arrGradientStops[0].color.g = 1.0f;
    m_arrGradientStops[0].color.b = 1.0f;
    m_arrGradientStops[0].color.a = 1.0f;
}
Void D2D1GradientBrush::_NakedCreateGradient()
{
    m_pGradientStopCollection = NULL;
    HRESULT hRes = m_pContext2D->m_pD2D1RenderingContext->CreateGradientStopCollection( (const D2D1_GRADIENT_STOP *)m_arrGradientStops, m_iGradientStopCount,
                                                                                         m_iGradientGamma, m_iGradientExtendMode, &m_pGradientStopCollection );
    DebugAssert( hRes == S_OK && m_pGradientStopCollection != NULL );
}
Void D2D1GradientBrush::_NakedDestroyGradient()
{
    m_pGradientStopCollection->Release();
    m_pGradientStopCollection = NULL;
}

/////////////////////////////////////////////////////////////////////////////////
// D2D1LinearGradientBrush implementation
D2D1LinearGradientBrush::D2D1LinearGradientBrush( D2D1RenderingContext * pContext2D ):
    D2D1GradientBrush( pContext2D )
{
    m_hLinearGradientProperties.startPoint.x = 0.0f;
    m_hLinearGradientProperties.startPoint.y = 0.0f;
    m_hLinearGradientProperties.endPoint.x = 100.0f;
    m_hLinearGradientProperties.endPoint.y = 0.0f;

    m_pLinearGradientBrush = NULL;
}
D2D1LinearGradientBrush::~D2D1LinearGradientBrush()
{
    if ( IsCreated() )
        Destroy();
}

Void D2D1LinearGradientBrush::Create( const D2D1GradientDesc * pGradientDesc )
{
    DebugAssert( !(IsCreated()) );

    _CreateGradient( pGradientDesc );
    _NakedCreate();
}
Void D2D1LinearGradientBrush::Destroy()
{
    D2D1Brush::Destroy();
    _DestroyGradient();
}

Void D2D1LinearGradientBrush::SetStartPoint( const D2D1Point * pStart )
{
    m_hLinearGradientProperties.startPoint.x = pStart->fX;
    m_hLinearGradientProperties.startPoint.y = pStart->fY;

    if ( IsCreated() )
        m_pLinearGradientBrush->SetStartPoint( *( (D2D1_POINT_2F*)&(m_hLinearGradientProperties.startPoint) ) );
}

Void D2D1LinearGradientBrush::SetEndPoint( const D2D1Point * pEnd )
{
    m_hLinearGradientProperties.endPoint.x = pEnd->fX;
    m_hLinearGradientProperties.endPoint.y = pEnd->fY;

    if ( IsCreated() )
        m_pLinearGradientBrush->SetEndPoint( *( (D2D1_POINT_2F*)&(m_hLinearGradientProperties.endPoint) ) );
}

Void D2D1LinearGradientBrush::SetLinearGradientDesc( const D2D1BrushLinearGradientDesc * pLinearGradientDesc )
{
    pLinearGradientDesc->ConvertTo( &m_hLinearGradientProperties );

    if ( IsCreated() ) {
        m_pLinearGradientBrush->SetStartPoint( *( (D2D1_POINT_2F*)&(m_hLinearGradientProperties.startPoint) ) );
        m_pLinearGradientBrush->SetEndPoint( *( (D2D1_POINT_2F*)&(m_hLinearGradientProperties.endPoint) ) );
    }
}

/////////////////////////////////////////////////////////////////////////////////

Void D2D1LinearGradientBrush::_NakedCreate()
{
    _NakedCreateGradient();

    m_pLinearGradientBrush = NULL;
    HRESULT hRes = m_pContext2D->m_pD2D1RenderingContext->CreateLinearGradientBrush( (const D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES *)&m_hLinearGradientProperties,
                                                                                     (const D2D1_BRUSH_PROPERTIES *)&m_hBrushProperties, m_pGradientStopCollection, &m_pLinearGradientBrush );
    DebugAssert( hRes == S_OK && m_pLinearGradientBrush != NULL );

    m_pBrush = NULL;
    hRes = m_pLinearGradientBrush->QueryInterface( __uuidof(ID2D1Brush), (Void**)&m_pBrush );
    DebugAssert( hRes == S_OK && m_pBrush != NULL );
}
Void D2D1LinearGradientBrush::_NakedDestroy()
{
    m_pBrush->Release();
    m_pBrush = NULL;

    m_pLinearGradientBrush->Release();
    m_pLinearGradientBrush = NULL;

    _NakedDestroyGradient();
}

/////////////////////////////////////////////////////////////////////////////////
// D2D1RadialGradientBrush implementation
D2D1RadialGradientBrush::D2D1RadialGradientBrush( D2D1RenderingContext * pContext2D ):
    D2D1GradientBrush( pContext2D )
{
    m_hRadialGradientProperties.center.x = 0.0f;
    m_hRadialGradientProperties.center.y = 0.0f;
    m_hRadialGradientProperties.gradientOriginOffset.x = 0.0f;
    m_hRadialGradientProperties.gradientOriginOffset.y = 0.0f;
    m_hRadialGradientProperties.radiusX = 1.0f;
    m_hRadialGradientProperties.radiusY = 1.0f;

    m_pRadialGradientBrush = NULL;
}
D2D1RadialGradientBrush::~D2D1RadialGradientBrush()
{
    if ( IsCreated() )
        Destroy();
}

Void D2D1RadialGradientBrush::Create( const D2D1GradientDesc * pGradientDesc )
{
    DebugAssert( !(IsCreated()) );

    _CreateGradient( pGradientDesc );
    _NakedCreate();
}
Void D2D1RadialGradientBrush::Destroy()
{
    D2D1Brush::Destroy();
    _DestroyGradient();
}

Void D2D1RadialGradientBrush::SetCenter( const D2D1Point * pCenter )
{
    m_hRadialGradientProperties.center.x = pCenter->fX;
    m_hRadialGradientProperties.center.y = pCenter->fY;

    if ( IsCreated() )
        m_pRadialGradientBrush->SetCenter( *( (const D2D1_POINT_2F *)&(m_hRadialGradientProperties.center) ) );
}

Void D2D1RadialGradientBrush::SetOffset( const D2D1Point * pOffset )
{
    m_hRadialGradientProperties.gradientOriginOffset.x = pOffset->fX;
    m_hRadialGradientProperties.gradientOriginOffset.y = pOffset->fY;

    if ( IsCreated() )
        m_pRadialGradientBrush->SetGradientOriginOffset( *( (const D2D1_POINT_2F *)&(m_hRadialGradientProperties.gradientOriginOffset) ) );
}

Void D2D1RadialGradientBrush::SetRadiusX( Float fRadiusX )
{
    m_hRadialGradientProperties.radiusX = fRadiusX;

    if ( IsCreated() )
        m_pRadialGradientBrush->SetRadiusX( m_hRadialGradientProperties.radiusX );
}
Void D2D1RadialGradientBrush::SetRadiusY( Float fRadiusY )
{
    m_hRadialGradientProperties.radiusY = fRadiusY;

    if ( IsCreated() )
        m_pRadialGradientBrush->SetRadiusY( m_hRadialGradientProperties.radiusY );
}

Void D2D1RadialGradientBrush::SetRadialGradientDesc( const D2D1BrushRadialGradientDesc * pRadialGradientDesc )
{
    pRadialGradientDesc->ConvertTo( &m_hRadialGradientProperties );

    if ( IsCreated() ) {
        m_pRadialGradientBrush->SetCenter( *( (const D2D1_POINT_2F *)&(m_hRadialGradientProperties.center) ) );
        m_pRadialGradientBrush->SetGradientOriginOffset( *( (const D2D1_POINT_2F *)&(m_hRadialGradientProperties.gradientOriginOffset) ) );
        m_pRadialGradientBrush->SetRadiusX( m_hRadialGradientProperties.radiusX );
        m_pRadialGradientBrush->SetRadiusY( m_hRadialGradientProperties.radiusY );
    }
}

/////////////////////////////////////////////////////////////////////////////////

Void D2D1RadialGradientBrush::_NakedCreate()
{
    _NakedCreateGradient();

    m_pRadialGradientBrush = NULL;
    HRESULT hRes = m_pContext2D->m_pD2D1RenderingContext->CreateRadialGradientBrush( (const D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES *)&m_hRadialGradientProperties,
                                                                                     (const D2D1_BRUSH_PROPERTIES *)&m_hBrushProperties, m_pGradientStopCollection, &m_pRadialGradientBrush );
    DebugAssert( hRes == S_OK && m_pRadialGradientBrush != NULL );

    m_pBrush = NULL;
    hRes = m_pRadialGradientBrush->QueryInterface( __uuidof(ID2D1Brush), (Void**)&m_pBrush );
    DebugAssert( hRes == S_OK && m_pBrush != NULL );
}
Void D2D1RadialGradientBrush::_NakedDestroy()
{
    m_pBrush->Release();
    m_pBrush = NULL;

    m_pRadialGradientBrush->Release();
    m_pRadialGradientBrush = NULL;

    _NakedDestroyGradient();
}

/////////////////////////////////////////////////////////////////////////////////
// D2D1BitmapBrush implementation
D2D1BitmapBrush::D2D1BitmapBrush( D2D1RenderingContext * pContext2D ):
    D2D1Brush( pContext2D )
{
    m_pBitmap = NULL;
    m_hBitmapBrushProperties.interpolationMode = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR;
    m_hBitmapBrushProperties.extendModeX = D2D1_EXTEND_MODE_CLAMP;
    m_hBitmapBrushProperties.extendModeY = D2D1_EXTEND_MODE_CLAMP;

    m_pBitmapBrush = NULL;
}
D2D1BitmapBrush::~D2D1BitmapBrush()
{
    if ( IsCreated() )
        Destroy();
}

Void D2D1BitmapBrush::Create()
{
    DebugAssert( !(IsCreated()) );
    DebugAssert( m_pBitmap != NULL && m_pBitmap->IsCreated() );

    _NakedCreate();
}

Void D2D1BitmapBrush::SetBitmap( D2D1Bitmap * pBitmap )
{
    DebugAssert( m_pBitmap != NULL && m_pBitmap->IsCreated() );

    m_pBitmap = pBitmap;

    if ( IsCreated() )
        m_pBitmapBrush->SetBitmap( m_pBitmap->m_pBitmap );
}

Void D2D1BitmapBrush::SetInterpolationMode( D2D1BitmapInterpolationMode iInterpolationMode )
{
    m_hBitmapBrushProperties.interpolationMode = D2D1BitmapInterpolationModeToD2D1[iInterpolationMode];

    if ( IsCreated() )
        m_pBitmapBrush->SetInterpolationMode( m_hBitmapBrushProperties.interpolationMode );
}

Void D2D1BitmapBrush::SetWrapModeX( D2D1BrushWrapMode iWrapModeX )
{
    m_hBitmapBrushProperties.extendModeX = D2D1BrushWrapModeToD2D1[iWrapModeX];

    if ( IsCreated() )
        m_pBitmapBrush->SetExtendModeX( m_hBitmapBrushProperties.extendModeX );
}
Void D2D1BitmapBrush::SetWrapModeY( D2D1BrushWrapMode iWrapModeY )
{
    m_hBitmapBrushProperties.extendModeY = D2D1BrushWrapModeToD2D1[iWrapModeY];

    if ( IsCreated() )
        m_pBitmapBrush->SetExtendModeY( m_hBitmapBrushProperties.extendModeY );
}

Void D2D1BitmapBrush::SetBrushBitmapDesc( const D2D1BrushBitmapDesc * pBitmapDesc )
{
    pBitmapDesc->ConvertTo( &m_hBitmapBrushProperties );

    if ( IsCreated() ) {
        m_pBitmapBrush->SetInterpolationMode( m_hBitmapBrushProperties.interpolationMode );
        m_pBitmapBrush->SetExtendModeX( m_hBitmapBrushProperties.extendModeX );
        m_pBitmapBrush->SetExtendModeY( m_hBitmapBrushProperties.extendModeY );
    }
}

/////////////////////////////////////////////////////////////////////////////////

Void D2D1BitmapBrush::_NakedCreate()
{
    m_pBitmapBrush = NULL;
    HRESULT hRes = m_pContext2D->m_pD2D1RenderingContext->CreateBitmapBrush( m_pBitmap->m_pBitmap, (const D2D1_BITMAP_BRUSH_PROPERTIES *)&m_hBitmapBrushProperties,
                                                                             (const D2D1_BRUSH_PROPERTIES *)&m_hBrushProperties, &m_pBitmapBrush );
    DebugAssert( hRes == S_OK && m_pBitmapBrush != NULL );

    m_pBrush = NULL;
    hRes = m_pBitmapBrush->QueryInterface( __uuidof(ID2D1Brush), (Void**)&m_pBrush );
    DebugAssert( hRes == S_OK && m_pBrush != NULL );
}
Void D2D1BitmapBrush::_NakedDestroy()
{
    m_pBrush->Release();
    m_pBrush = NULL;

    m_pBitmapBrush->Release();
    m_pBitmapBrush = NULL;
}

