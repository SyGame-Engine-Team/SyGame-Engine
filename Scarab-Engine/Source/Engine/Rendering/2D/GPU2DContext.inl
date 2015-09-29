/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/2D/GPU2DContext.inl
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
// GPU2DContext implementation
inline Bool GPU2DContext::IsBound() const {
    return m_hDevice2DContext.IsCreated();
}
inline Bool GPU2DContext::IsBoundToBackBuffer( UInt * outBackBuffer ) const {
    Assert( IsBound() );
    return m_hDevice2DContext.IsBoundToBackBuffer( outBackBuffer );
}

inline GPUTexture2D * GPU2DContext::GetRenderTarget() const {
    Assert( IsBound() );
    return m_pRenderTarget;
}

inline UInt GPU2DContext::GetMaxBitmapDimension() const {
    Assert( IsBound() );
    return m_hDevice2DContext.GetMaxBitmapDimension();
}
inline Bool GPU2DContext::IsSupported( const GPU2DRenderTargetDesc * pDesc ) const {
    Assert( IsBound() );
    return m_hDevice2DContext.IsSupported( (const Device2DRenderTargetDesc *)pDesc );
}
inline Void GPU2DContext::GetDesktopDPI( Float * outDpiX, Float * outDpiY ) const {
    Assert( IsBound() );
    m_hDevice2DContext.GetDesktopDPI( outDpiX, outDpiY );
}

inline Void GPU2DContext::GetPixelFormat( PixelFormat * outFormat, GPU2DBitmapAlphaMode * outAlphaMode ) const {
    Assert( IsBound() );
    m_hDevice2DContext.GetPixelFormat( outFormat, (Device2DBitmapAlphaMode*)outAlphaMode );
}
inline Void GPU2DContext::GetPixelSize( UInt * outWidth, UInt * outHeight ) const {
    Assert( IsBound() );
    m_hDevice2DContext.GetPixelSize( outWidth, outHeight );
}
inline Void GPU2DContext::GetDIPSize( Float * outWidth, Float * outHeight ) const {
    Assert( IsBound() );
    m_hDevice2DContext.GetDIPSize( outWidth, outHeight );
}

inline Float GPU2DContext::GetDpiX() const {
    return m_hDevice2DContext.GetDpiX();
}
inline Float GPU2DContext::GetDpiY() const {
    return m_hDevice2DContext.GetDpiY();
}
inline Void GPU2DContext::SetDPI( Float fDpiX, Float fDpiY ) {
    m_hDevice2DContext.SetDPI( fDpiX, fDpiY );
}

inline GPU2DAntialiasingMode GPU2DContext::GetAntialiasingMode() const {
    return (GPU2DAntialiasingMode)( m_hDevice2DContext.GetAntialiasingMode() );
}
inline Void GPU2DContext::SetAntialiasingMode( GPU2DAntialiasingMode iAntialiasingMode ) {
    m_hDevice2DContext.SetAntialiasingMode( (Device2DAntialiasingMode)iAntialiasingMode );
}

inline const GPU2DMatrix32 * GPU2DContext::GetTransform() const {
    return (const GPU2DMatrix32 *)( m_hDevice2DContext.GetTransform() );
}
inline Void GPU2DContext::SetTransform( const GPU2DMatrix32 * pTransform ) {
    m_hDevice2DContext.SetTransform( (const Device2DMatrix32 *)pTransform );
}

inline GPU2DTag GPU2DContext::GetTag1() const {
    return (GPU2DTag)( m_hDevice2DContext.GetTag1() );
}
inline GPU2DTag GPU2DContext::GetTag2() const {
    return (GPU2DTag)( m_hDevice2DContext.GetTag2() );
}
inline Void GPU2DContext::SetTags( GPU2DTag iTag1, GPU2DTag iTag2 ) {
    m_hDevice2DContext.SetTags( (Device2DTag)iTag1, (Device2DTag)iTag2 );
}

inline GPU2DTextAntialiasingMode GPU2DContext::GetTextAntialiasingMode() const {
    return (GPU2DTextAntialiasingMode)( m_hDevice2DContext.GetTextAntialiasingMode() );
}
inline Void GPU2DContext::SetTextAntialiasingMode( GPU2DTextAntialiasingMode iTextAntialiasingMode ) {
    m_hDevice2DContext.SetTextAntialiasingMode( (Device2DTextAntialiasingMode)iTextAntialiasingMode );
}

inline Void GPU2DContext::GetTextRenderState( GPU2DTextRenderState * outTextRenderState ) const {
    Assert( !(outTextRenderState->IsBound()) );
    m_hDevice2DContext.GetTextRenderState( &(outTextRenderState->m_hDevice2DTextRenderState) );
}
inline Void GPU2DContext::SetTextRenderState( GPU2DTextRenderState * pTextRenderState ) {
    Assert( pTextRenderState == NULL || pTextRenderState->IsBound() );
    m_hDevice2DContext.SetTextRenderState( (pTextRenderState != NULL) ? &(pTextRenderState->m_hDevice2DTextRenderState) : NULL );
}

inline Void GPU2DContext::Clear( const GPU2DColor * pColor ) {
    Assert( IsBound() );
    m_hDevice2DContext.Clear( (const Device2DColor *)pColor );
}

inline Void GPU2DContext::PushLayer( GPU2DLayer * pLayer, const GPU2DLayerParameters * pParameters ) {
    Assert( IsBound() );
    Assert( pLayer->IsBound() );
    m_hDevice2DContext.PushLayer( &(pLayer->m_hDevice2DLayer), (const Device2DLayerParameters *)pParameters );
}
inline Void GPU2DContext::PopLayer() {
    Assert( IsBound() );
    m_hDevice2DContext.PopLayer();
}

inline Void GPU2DContext::PushClip( const GPU2DRectangle * pRect, GPU2DAntialiasingMode iAntialiasingMode ) {
    Assert( IsBound() );
    m_hDevice2DContext.PushClip( (const Device2DRectangle *)pRect, (Device2DAntialiasingMode)iAntialiasingMode );
}
inline Void GPU2DContext::PopClip() {
    Assert( IsBound() );
    m_hDevice2DContext.PopClip();
}

inline Void GPU2DContext::Begin() {
    Assert( IsBound() );
    m_hDevice2DContext.Begin();
}
inline Void GPU2DContext::End() {
    Assert( IsBound() );
    m_hDevice2DContext.End();
}

inline Void GPU2DContext::DrawLine( const GPU2DPoint * pStart, const GPU2DPoint * pEnd, const GPU2DBrush * pBrush, Float fStrokeWidth, const GPU2DStrokeStyle * pStrokeStyle ) {
    Assert( IsBound() );
    Assert( pBrush->IsBound() );
    Assert( pStrokeStyle == NULL || pStrokeStyle->IsBound() );
    m_hDevice2DContext.DrawLine( (const Device2DPoint *)pStart, (const Device2DPoint *)pEnd, pBrush->m_pDevice2DBrush, fStrokeWidth, (pStrokeStyle != NULL) ? &(pStrokeStyle->m_hDevice2DStrokeStyle) : NULL );
}
inline Void GPU2DContext::DrawRectangle( const GPU2DRectangle * pRectangle, const GPU2DBrush * pBrush, Float fStrokeWidth, const GPU2DStrokeStyle * pStrokeStyle ) {
    Assert( IsBound() );
    Assert( pBrush->IsBound() );
    Assert( pStrokeStyle == NULL || pStrokeStyle->IsBound() );
    m_hDevice2DContext.DrawRectangle( (const Device2DRectangle *)pRectangle, pBrush->m_pDevice2DBrush, fStrokeWidth, (pStrokeStyle != NULL) ? &(pStrokeStyle->m_hDevice2DStrokeStyle) : NULL );
}
inline Void GPU2DContext::FillRectangle( const GPU2DRectangle * pRectangle, const GPU2DBrush * pBrush ) {
    Assert( IsBound() );
    Assert( pBrush->IsBound() );
    m_hDevice2DContext.FillRectangle( (const Device2DRectangle *)pRectangle, pBrush->m_pDevice2DBrush );
}
inline Void GPU2DContext::DrawRoundedRectangle( const GPU2DRoundedRectangle * pRoundedRectangle, const GPU2DBrush * pBrush, Float fStrokeWidth, const GPU2DStrokeStyle * pStrokeStyle ) {
    Assert( IsBound() );
    Assert( pBrush->IsBound() );
    Assert( pStrokeStyle == NULL || pStrokeStyle->IsBound() );
    m_hDevice2DContext.DrawRoundedRectangle( (const Device2DRoundedRectangle *)pRoundedRectangle, pBrush->m_pDevice2DBrush, fStrokeWidth, (pStrokeStyle != NULL) ? &(pStrokeStyle->m_hDevice2DStrokeStyle) : NULL );
}
inline Void GPU2DContext::FillRoundedRectangle( const GPU2DRoundedRectangle * pRoundedRectangle, const GPU2DBrush * pBrush ) {
    Assert( IsBound() );
    Assert( pBrush->IsBound() );
    m_hDevice2DContext.FillRoundedRectangle( (const Device2DRoundedRectangle *)pRoundedRectangle, pBrush->m_pDevice2DBrush );
}
inline Void GPU2DContext::DrawCircle( const GPU2DPoint * pCenter, Float fRadius, const GPU2DBrush * pBrush, Float fStrokeWidth, const GPU2DStrokeStyle * pStrokeStyle ) {
    Assert( IsBound() );
    Assert( pBrush->IsBound() );
    Assert( pStrokeStyle == NULL || pStrokeStyle->IsBound() );
    m_hDevice2DContext.DrawCircle( (const Device2DPoint *)pCenter, fRadius, pBrush->m_pDevice2DBrush, fStrokeWidth, (pStrokeStyle != NULL) ? &(pStrokeStyle->m_hDevice2DStrokeStyle) : NULL );
}
inline Void GPU2DContext::FillCircle( const GPU2DPoint * pCenter, Float fRadius, const GPU2DBrush * pBrush ) {
    Assert( IsBound() );
    Assert( pBrush->IsBound() );
    m_hDevice2DContext.FillCircle( (const Device2DPoint *)pCenter, fRadius, pBrush->m_pDevice2DBrush );
}
inline Void GPU2DContext::DrawEllipse( const GPU2DEllipse * pEllipse, const GPU2DBrush * pBrush, Float fStrokeWidth, const GPU2DStrokeStyle * pStrokeStyle ) {
    Assert( IsBound() );
    Assert( pBrush->IsBound() );
    Assert( pStrokeStyle == NULL || pStrokeStyle->IsBound() );
    m_hDevice2DContext.DrawEllipse( (const Device2DEllipse *)pEllipse, pBrush->m_pDevice2DBrush, fStrokeWidth, (pStrokeStyle != NULL) ? &(pStrokeStyle->m_hDevice2DStrokeStyle) : NULL );
}
inline Void GPU2DContext::FillEllipse( const GPU2DEllipse * pEllipse, const GPU2DBrush * pBrush ) {
    Assert( IsBound() );
    Assert( pBrush->IsBound() );
    m_hDevice2DContext.FillEllipse( (const Device2DEllipse *)pEllipse, pBrush->m_pDevice2DBrush );
}

inline Void GPU2DContext::DrawBitmap( const GPU2DBitmap * pBitmap, const GPU2DRectangle * pDstRect, const GPU2DRectangle * pSrcRect, Float fOpacity, GPU2DBitmapInterpolationMode iInterpolationMode ) {
    Assert( IsBound() );
    Assert( pBitmap->IsBound() );
    m_hDevice2DContext.DrawBitmap( &(pBitmap->m_hDevice2DBitmap), (const Device2DRectangle *)pDstRect, (const Device2DRectangle *)pSrcRect, fOpacity, (Device2DBitmapInterpolationMode)iInterpolationMode );
}
inline Void GPU2DContext::FillOpacityMask( const GPU2DBitmap * pOpacityMask, GPU2DBitmapOpacityMaskContent iOpacityMaskContent, const GPU2DBrush * pBrush, const GPU2DRectangle * pDstRect, const GPU2DRectangle * pSrcRect ) {
    Assert( IsBound() );
    Assert( pOpacityMask->IsBound() );
    Assert( pBrush->IsBound() );
    m_hDevice2DContext.FillOpacityMask( &(pOpacityMask->m_hDevice2DBitmap), (Device2DBitmapOpacityMaskContent)iOpacityMaskContent, pBrush->m_pDevice2DBrush, (const Device2DRectangle *)pDstRect, (const Device2DRectangle *)pSrcRect );
}

inline Void GPU2DContext::FillMesh( const GPU2DMesh * pMesh, const GPU2DBrush * pBrush ) {
    Assert( IsBound() );
    Assert( pMesh->IsBound() );
    Assert( pBrush->IsBound() );
    m_hDevice2DContext.FillMesh( &(pMesh->m_hDevice2DMesh), pBrush->m_pDevice2DBrush );
}
inline Void GPU2DContext::DrawGeometry( const GPU2DGeometry * pGeometry, const GPU2DBrush * pBrush, Float fStrokeWidth, const GPU2DStrokeStyle * pStrokeStyle ) {
    Assert( IsBound() );
    Assert( pGeometry->IsBound() );
    Assert( pBrush->IsBound() );
    Assert( pStrokeStyle == NULL || pStrokeStyle->IsBound() );
    m_hDevice2DContext.DrawGeometry( pGeometry->m_pDevice2DGeometry, pBrush->m_pDevice2DBrush, fStrokeWidth, (pStrokeStyle != NULL) ? &(pStrokeStyle->m_hDevice2DStrokeStyle) : NULL );
}
inline Void GPU2DContext::FillGeometry( const GPU2DGeometry * pGeometry, const GPU2DBrush * pBrush, const GPU2DBrush * pOpacityBrush ) {
    Assert( IsBound() );
    Assert( pGeometry->IsBound() );
    Assert( pBrush->IsBound() );
    Assert( pOpacityBrush == NULL || pOpacityBrush->IsBound() );
    m_hDevice2DContext.FillGeometry( pGeometry->m_pDevice2DGeometry, pBrush->m_pDevice2DBrush, (pOpacityBrush != NULL) ? pOpacityBrush->m_pDevice2DBrush : NULL );
}

inline Void GPU2DContext::DrawText( const GChar * strText, UInt iLength, const GPU2DTextFormat * pTextFormat, const GPU2DRectangle * pLayoutRect, const GPU2DBrush * pForegroundBrush, UInt iDrawingOptions, GPU2DTextMeasuringMode iMeasuringMode ) {
    Assert( IsBound() );
    Assert( pTextFormat->IsBound() );
    Assert( pForegroundBrush->IsBound() );
    m_hDevice2DContext.DrawText( strText, iLength, &(pTextFormat->m_hDevice2DTextFormat), (const Device2DRectangle *)pLayoutRect, pForegroundBrush->m_pDevice2DBrush, iDrawingOptions, (Device2DTextMeasuringMode)iMeasuringMode );
}
inline Void GPU2DContext::DrawTextLayout( const GPU2DPoint * pTopLeft, const GPU2DTextLayout * pTextLayout, const GPU2DBrush * pForegroundBrush, UInt iDrawingOptions ) {
    Assert( IsBound() );
    Assert( pTextLayout->IsBound() );
    Assert( pForegroundBrush->IsBound() );
    m_hDevice2DContext.DrawTextLayout( (const Device2DPoint *)pTopLeft, &(pTextLayout->m_hDevice2DTextLayout), pForegroundBrush->m_pDevice2DBrush, iDrawingOptions );
}

inline Void GPU2DContext::Flush( GPU2DTag * outTag1, GPU2DTag * outTag2 ) {
    Assert( IsBound() );
    m_hDevice2DContext.Flush( (Device2DTag*)outTag1, (Device2DTag*)outTag2 );
}

inline Void GPU2DContext::SaveDrawingState( GPU2DRenderState * pRenderState ) const {
    Assert( IsBound() );
    Assert( pRenderState->IsBound() );
    m_hDevice2DContext.SaveDrawingState( &(pRenderState->m_hDevice2DRenderState) );
}
inline Void GPU2DContext::RestoreDrawingState( GPU2DRenderState * pRenderState ) {
    Assert( IsBound() );
    Assert( pRenderState->IsBound() );
    m_hDevice2DContext.RestoreDrawingState( &(pRenderState->m_hDevice2DRenderState) );
}

/////////////////////////////////////////////////////////////////////////////////

inline Int GPU2DContext::_Compare_GPU2DBitmap( GPU2DBitmap * const & rLeft, GPU2DBitmap * const & rRight, Void * /*pUserData*/ ) {
    if ( rLeft < rRight )
        return +1;
    if ( rLeft > rRight )
        return -1;
    return 0;
}
inline Int GPU2DContext::_Compare_GPU2DBrush( GPU2DBrush * const & rLeft, GPU2DBrush * const & rRight, Void * /*pUserData*/ ) {
    if ( rLeft < rRight )
        return +1;
    if ( rLeft > rRight )
        return -1;
    return 0;
}
inline Int GPU2DContext::_Compare_GPU2DMesh( GPU2DMesh * const & rLeft, GPU2DMesh * const & rRight, Void * /*pUserData*/ ) {
    if ( rLeft < rRight )
        return +1;
    if ( rLeft > rRight )
        return -1;
    return 0;
}
inline Int GPU2DContext::_Compare_GPU2DLayer( GPU2DLayer * const & rLeft, GPU2DLayer * const & rRight, Void * /*pUserData*/ ) {
    if ( rLeft < rRight )
        return +1;
    if ( rLeft > rRight )
        return -1;
    return 0;
}

inline Void GPU2DContext::_RegisterBitmap( GPU2DBitmap * pBitmap ) {
    Bool bInserted = m_lstBitmaps.Insert( pBitmap );
    Assert( bInserted );
}
inline Void GPU2DContext::_RegisterBrush( GPU2DBrush * pBrush ) {
    Bool bInserted = m_lstBrushes.Insert( pBrush );
    Assert( bInserted );
}
inline Void GPU2DContext::_RegisterMesh( GPU2DMesh * pMesh ) {
    Bool bInserted = m_lstMeshes.Insert( pMesh );
    Assert( bInserted );
}
inline Void GPU2DContext::_RegisterLayer( GPU2DLayer * pLayer ) {
    Bool bInserted = m_lstLayers.Insert( pLayer );
    Assert( bInserted );
}

inline Void GPU2DContext::_UnRegisterBitmap( GPU2DBitmap * pBitmap ) {
    Bool bRemoved = m_lstBitmaps.Remove( pBitmap );
    Assert( bRemoved );
}
inline Void GPU2DContext::_UnRegisterBrush( GPU2DBrush * pBrush ) {
    Bool bRemoved = m_lstBrushes.Remove( pBrush );
    Assert( bRemoved );
}
inline Void GPU2DContext::_UnRegisterMesh( GPU2DMesh * pMesh ) {
    Bool bRemoved = m_lstMeshes.Remove( pMesh );
    Assert( bRemoved );
}
inline Void GPU2DContext::_UnRegisterLayer( GPU2DLayer * pLayer ) {
    Bool bRemoved = m_lstLayers.Remove( pLayer );
    Assert( bRemoved );
}
