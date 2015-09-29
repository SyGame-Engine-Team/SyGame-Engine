/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/2D/Resources/GPU2DBrush.inl
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
// GPU2DBrush implementation
inline Bool GPU2DBrush::IsBound() const {
    return m_pDevice2DBrush->IsCreated();
}

inline GPU2DBrushType GPU2DBrush::GetType() const {
    return (GPU2DBrushType)( m_pDevice2DBrush->GetType() );
}

inline Float GPU2DBrush::GetOpacity() const {
    return m_pDevice2DBrush->GetOpacity();
}
inline Void GPU2DBrush::SetOpacity( Float fOpacity ) {
    m_pDevice2DBrush->SetOpacity( fOpacity );
}

inline Void GPU2DBrush::GetTransform( GPU2DMatrix32 * outTransform ) const {
    m_pDevice2DBrush->GetTransform( (Device2DMatrix32*)outTransform );
}
inline Void GPU2DBrush::SetTransform( const GPU2DMatrix32 * pTransform ) {
    m_pDevice2DBrush->SetTransform( (const Device2DMatrix32 *)pTransform );
}

inline Void GPU2DBrush::GetDesc( GPU2DBrushDesc * outDesc ) const {
    m_pDevice2DBrush->GetDesc( (Device2DBrushDesc*)outDesc );
}
inline Void GPU2DBrush::SetDesc( const GPU2DBrushDesc * pDesc ) {
    m_pDevice2DBrush->SetDesc( (const Device2DBrushDesc *)pDesc );
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DSolidColorBrush implementation
inline const GPU2DColor * GPU2DSolidColorBrush::GetColor() const {
    return (const GPU2DColor *)( m_hDevice2DSolidColorBrush.GetColor() );
}
inline Void GPU2DSolidColorBrush::GetColor( GPU2DColor * outColor ) const {
    m_hDevice2DSolidColorBrush.GetColor( (Device2DColor*)outColor );
}
inline Void GPU2DSolidColorBrush::SetColor( const GPU2DColor * pColor ) {
    m_hDevice2DSolidColorBrush.SetColor( (const Device2DColor *)pColor );
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DGradientBrush implementation
inline GPU2DBrushGamma GPU2DGradientBrush::GetGradientGammaMode() const {
    Assert( IsBound() );
    return (GPU2DBrushGamma)( m_pDevice2DGradientBrush->GetGradientGammaMode() );
}
inline GPU2DBrushWrapMode GPU2DGradientBrush::GetGradientWrapMode() const {
    Assert( IsBound() );
    return (GPU2DBrushWrapMode)( m_pDevice2DGradientBrush->GetGradientWrapMode() );
}

inline UInt GPU2DGradientBrush::GetGradientStopCount() const {
    Assert( IsBound() );
    return m_pDevice2DGradientBrush->GetGradientStopCount();
}
inline const GPU2DGradientStop * GPU2DGradientBrush::GetGradientStop( UInt iStop ) const {
    Assert( IsBound() );
    return (const GPU2DGradientStop *)( m_pDevice2DGradientBrush->GetGradientStop( iStop ) );
}
inline Void GPU2DGradientBrush::GetGradientStop( GPU2DGradientStop * outGradientStop, UInt iStop ) const {
    Assert( IsBound() );
    m_pDevice2DGradientBrush->GetGradientStop( (Device2DGradientStop*)outGradientStop, iStop );
}
inline Void GPU2DGradientBrush::GetGradientStops( GPU2DGradientStop * outGradientStops, UInt iMaxStops ) const {
    Assert( IsBound() );
    m_pDevice2DGradientBrush->GetGradientStops( (Device2DGradientStop*)outGradientStops, iMaxStops );
}

inline Void GPU2DGradientBrush::GetGradientDesc( GPU2DGradientDesc * outGradientDesc ) const {
    Assert( IsBound() );
    m_pDevice2DGradientBrush->GetGradientDesc( (Device2DGradientDesc*)outGradientDesc );
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DLinearGradientBrush implementation
inline const GPU2DPoint * GPU2DLinearGradientBrush::GetStartPoint() const {
    return (const GPU2DPoint *)( m_hDevice2DLinearGradientBrush.GetStartPoint() );
}
inline Void GPU2DLinearGradientBrush::GetStartPoint( GPU2DPoint * outStart ) const {
    m_hDevice2DLinearGradientBrush.GetStartPoint( (Device2DPoint*)outStart );
}
inline Void GPU2DLinearGradientBrush::SetStartPoint( const GPU2DPoint * pStart ) {
    m_hDevice2DLinearGradientBrush.SetStartPoint( (const Device2DPoint *)pStart );
}

inline const GPU2DPoint * GPU2DLinearGradientBrush::GetEndPoint() const {
    return (const GPU2DPoint *)( m_hDevice2DLinearGradientBrush.GetEndPoint() );
}
inline Void GPU2DLinearGradientBrush::GetEndPoint( GPU2DPoint * outEnd ) const {
    m_hDevice2DLinearGradientBrush.GetEndPoint( (Device2DPoint*)outEnd );
}
inline Void GPU2DLinearGradientBrush::SetEndPoint( const GPU2DPoint * pEnd ) {
    m_hDevice2DLinearGradientBrush.SetEndPoint( (const Device2DPoint *)pEnd );
}

inline Void GPU2DLinearGradientBrush::GetLinearGradientDesc( GPU2DBrushLinearGradientDesc * outLinearGradientDesc ) const {
    m_hDevice2DLinearGradientBrush.GetLinearGradientDesc( (Device2DBrushLinearGradientDesc*)outLinearGradientDesc );
}
inline Void GPU2DLinearGradientBrush::SetLinearGradientDesc( const GPU2DBrushLinearGradientDesc * pLinearGradientDesc ) {
    m_hDevice2DLinearGradientBrush.SetLinearGradientDesc( (const Device2DBrushLinearGradientDesc *)pLinearGradientDesc );
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DRadialGradientBrush implementation
inline const GPU2DPoint * GPU2DRadialGradientBrush::GetCenter() const {
    return (const GPU2DPoint *)( m_hDevice2DRadialGradientBrush.GetCenter() );
}
inline Void GPU2DRadialGradientBrush::SetCenter( const GPU2DPoint * pCenter ) {
    m_hDevice2DRadialGradientBrush.SetCenter( (const Device2DPoint *)pCenter );
}

inline const GPU2DPoint * GPU2DRadialGradientBrush::GetOffset() const {
    return (const GPU2DPoint *)( m_hDevice2DRadialGradientBrush.GetOffset() );
}
inline Void GPU2DRadialGradientBrush::SetOffset( const GPU2DPoint * pOffset ) {
    m_hDevice2DRadialGradientBrush.SetOffset( (const Device2DPoint *)pOffset );
}

inline Float GPU2DRadialGradientBrush::GetRadiusX() const {
    return m_hDevice2DRadialGradientBrush.GetRadiusX();
}
inline Float GPU2DRadialGradientBrush::GetRadiusY() const {
    return m_hDevice2DRadialGradientBrush.GetRadiusY();
}
inline Void GPU2DRadialGradientBrush::SetRadiusX( Float fRadiusX ) {
    m_hDevice2DRadialGradientBrush.SetRadiusX( fRadiusX );
}
inline Void GPU2DRadialGradientBrush::SetRadiusY( Float fRadiusY ) {
    m_hDevice2DRadialGradientBrush.SetRadiusY( fRadiusY );
}

inline Void GPU2DRadialGradientBrush::GetRadialGradientDesc( GPU2DBrushRadialGradientDesc * outRadialGradientDesc ) const {
    m_hDevice2DRadialGradientBrush.GetRadialGradientDesc( (Device2DBrushRadialGradientDesc*)outRadialGradientDesc );
}
inline Void GPU2DRadialGradientBrush::SetRadialGradientDesc( const GPU2DBrushRadialGradientDesc * pRadialGradientDesc ) {
    m_hDevice2DRadialGradientBrush.SetRadialGradientDesc( (const Device2DBrushRadialGradientDesc *)pRadialGradientDesc );
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DBitmapBrush implementation
inline GPU2DBitmap * GPU2DBitmapBrush::GetBitmap() const {
    return m_pBitmap;
}
inline Void GPU2DBitmapBrush::SetBitmap( GPU2DBitmap * pBitmap ) {
    Assert( pBitmap != NULL && pBitmap->IsBound() );
    m_hDevice2DBitmapBrush.SetBitmap( &(pBitmap->m_hDevice2DBitmap) );
    m_pBitmap = pBitmap;
}

inline GPU2DBitmapInterpolationMode GPU2DBitmapBrush::GetInterpolationMode() const {
    return (GPU2DBitmapInterpolationMode)( m_hDevice2DBitmapBrush.GetInterpolationMode() );
}
inline Void GPU2DBitmapBrush::SetInterpolationMode( GPU2DBitmapInterpolationMode iInterpolationMode ) {
    m_hDevice2DBitmapBrush.SetInterpolationMode( (Device2DBitmapInterpolationMode)iInterpolationMode );
}

inline GPU2DBrushWrapMode GPU2DBitmapBrush::GetWrapModeX() const {
    return (GPU2DBrushWrapMode)( m_hDevice2DBitmapBrush.GetWrapModeX() );
}
inline GPU2DBrushWrapMode GPU2DBitmapBrush::GetWrapModeY() const {
    return (GPU2DBrushWrapMode)( m_hDevice2DBitmapBrush.GetWrapModeY() );
}
inline Void GPU2DBitmapBrush::SetWrapModeX( GPU2DBrushWrapMode iWrapModeX ) {
    m_hDevice2DBitmapBrush.SetWrapModeX( (Device2DBrushWrapMode)iWrapModeX );
}
inline Void GPU2DBitmapBrush::SetWrapModeY( GPU2DBrushWrapMode iWrapModeY ) {
    m_hDevice2DBitmapBrush.SetWrapModeY( (Device2DBrushWrapMode)iWrapModeY );
}

inline Void GPU2DBitmapBrush::GetBrushBitmapDesc( GPU2DBrushBitmapDesc * outBitmapDesc ) const {
    m_hDevice2DBitmapBrush.GetBrushBitmapDesc( (Device2DBrushBitmapDesc*)outBitmapDesc );
}
inline Void GPU2DBitmapBrush::SetBrushBitmapDesc( const GPU2DBrushBitmapDesc * pBitmapDesc ) {
    m_hDevice2DBitmapBrush.SetBrushBitmapDesc( (const Device2DBrushBitmapDesc *)pBitmapDesc );
}
