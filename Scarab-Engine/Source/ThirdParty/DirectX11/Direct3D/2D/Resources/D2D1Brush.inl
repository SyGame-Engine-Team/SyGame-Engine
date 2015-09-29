/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/2D/Resources/D2D1Brush.inl
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
// D2D1Brush implementation
inline Bool D2D1Brush::IsCreated() const {
    return ( m_pBrush != NULL || m_bTemporaryDestroyed );
}

inline Float D2D1Brush::GetOpacity() const {
    return m_hBrushProperties.opacity;
}

inline Void D2D1Brush::GetTransform( D2D1Matrix32 * outTransform ) const {
    outTransform->f00 = m_hBrushProperties.transform._11;
    outTransform->f01 = m_hBrushProperties.transform._12;
    outTransform->f10 = m_hBrushProperties.transform._21;
    outTransform->f11 = m_hBrushProperties.transform._22;
    outTransform->f20 = m_hBrushProperties.transform._31;
    outTransform->f21 = m_hBrushProperties.transform._32;
}

inline Void D2D1Brush::GetDesc( D2D1BrushDesc * outDesc ) const {
    outDesc->ConvertFrom( &m_hBrushProperties );
}

/////////////////////////////////////////////////////////////////////////////////
// D2D1SolidColorBrush implementation
inline D2D1BrushType D2D1SolidColorBrush::GetType() const {
    return D2D1BRUSH_SOLID_COLOR;
}

inline const D2D1Color * D2D1SolidColorBrush::GetColor() const {
    return &m_hBrushColor;
}
inline Void D2D1SolidColorBrush::GetColor( D2D1Color * outColor ) const {
    outColor->R = m_hBrushColor.R;
    outColor->G = m_hBrushColor.G;
    outColor->B = m_hBrushColor.B;
    outColor->A = m_hBrushColor.A;
}

/////////////////////////////////////////////////////////////////////////////////
// D2D1GradientBrush implementation
inline D2D1BrushGamma D2D1GradientBrush::GetGradientGammaMode() const {
    DebugAssert( IsCreated() );
    return D2D1BrushGammaFromD2D1[m_iGradientGamma];
}
inline D2D1BrushWrapMode D2D1GradientBrush::GetGradientWrapMode() const {
    DebugAssert( IsCreated() );
    return D2D1BrushWrapModeFromD2D1[m_iGradientExtendMode];
}

inline UInt D2D1GradientBrush::GetGradientStopCount() const {
    DebugAssert( IsCreated() );
    return m_iGradientStopCount;
}
inline const D2D1GradientStop * D2D1GradientBrush::GetGradientStop( UInt iStop ) const {
    DebugAssert( IsCreated() );
    DebugAssert( iStop < m_iGradientStopCount );
    return (const D2D1GradientStop *)( m_arrGradientStops + iStop );
}
inline Void D2D1GradientBrush::GetGradientStop( D2D1GradientStop * outGradientStop, UInt iStop ) const {
    DebugAssert( IsCreated() );
    DebugAssert( iStop < m_iGradientStopCount );
    outGradientStop->fPosition = m_arrGradientStops[iStop].position;
    outGradientStop->fColor.R = m_arrGradientStops[iStop].color.r;
    outGradientStop->fColor.G = m_arrGradientStops[iStop].color.g;
    outGradientStop->fColor.B = m_arrGradientStops[iStop].color.b;
    outGradientStop->fColor.A = m_arrGradientStops[iStop].color.a;
}

/////////////////////////////////////////////////////////////////////////////////
// D2D1LinearGradientBrush implementation
inline D2D1BrushType D2D1LinearGradientBrush::GetType() const {
    return D2D1BRUSH_GRADIENT_LINEAR;
}

inline const D2D1Point * D2D1LinearGradientBrush::GetStartPoint() const {
    return (const D2D1Point *)( &(m_hLinearGradientProperties.startPoint) );
}
inline Void D2D1LinearGradientBrush::GetStartPoint( D2D1Point * outStart ) const {
    outStart->fX = m_hLinearGradientProperties.startPoint.x;
    outStart->fY = m_hLinearGradientProperties.startPoint.y;
}

inline const D2D1Point * D2D1LinearGradientBrush::GetEndPoint() const {
    return (const D2D1Point *)( &(m_hLinearGradientProperties.endPoint) );
}
inline Void D2D1LinearGradientBrush::GetEndPoint( D2D1Point * outEnd ) const {
    outEnd->fX = m_hLinearGradientProperties.endPoint.x;
    outEnd->fY = m_hLinearGradientProperties.endPoint.y;
}

inline Void D2D1LinearGradientBrush::GetLinearGradientDesc( D2D1BrushLinearGradientDesc * outLinearGradientDesc ) const {
    outLinearGradientDesc->ConvertFrom( &m_hLinearGradientProperties );
}

/////////////////////////////////////////////////////////////////////////////////
// D2D1RadialGradientBrush implementation
inline D2D1BrushType D2D1RadialGradientBrush::GetType() const {
    return D2D1BRUSH_GRADIENT_RADIAL;
}

inline const D2D1Point * D2D1RadialGradientBrush::GetCenter() const {
    return (const D2D1Point *)( &(m_hRadialGradientProperties.center) );
}

inline const D2D1Point * D2D1RadialGradientBrush::GetOffset() const {
    return (const D2D1Point *)( &(m_hRadialGradientProperties.gradientOriginOffset) );
}

inline Float D2D1RadialGradientBrush::GetRadiusX() const {
    return m_hRadialGradientProperties.radiusX;
}
inline Float D2D1RadialGradientBrush::GetRadiusY() const {
    return m_hRadialGradientProperties.radiusY;
}

inline Void D2D1RadialGradientBrush::GetRadialGradientDesc( D2D1BrushRadialGradientDesc * outRadialGradientDesc ) const {
    outRadialGradientDesc->ConvertFrom( &m_hRadialGradientProperties );
}

/////////////////////////////////////////////////////////////////////////////////
// D2D1BitmapBrush implementation
inline D2D1BrushType D2D1BitmapBrush::GetType() const {
    return D2D1BRUSH_BITMAP;
}

inline D2D1Bitmap * D2D1BitmapBrush::GetBitmap() const {
    return m_pBitmap;
}

inline D2D1BitmapInterpolationMode D2D1BitmapBrush::GetInterpolationMode() const {
    return D2D1BitmapInterpolationModeFromD2D1[m_hBitmapBrushProperties.interpolationMode];
}

inline D2D1BrushWrapMode D2D1BitmapBrush::GetWrapModeX() const {
    return D2D1BrushWrapModeFromD2D1[m_hBitmapBrushProperties.extendModeX];
}
inline D2D1BrushWrapMode D2D1BitmapBrush::GetWrapModeY() const {
    return D2D1BrushWrapModeFromD2D1[m_hBitmapBrushProperties.extendModeY];
}

inline Void D2D1BitmapBrush::GetBrushBitmapDesc( D2D1BrushBitmapDesc * outBitmapDesc ) const {
    outBitmapDesc->ConvertFrom( &m_hBitmapBrushProperties );
}

