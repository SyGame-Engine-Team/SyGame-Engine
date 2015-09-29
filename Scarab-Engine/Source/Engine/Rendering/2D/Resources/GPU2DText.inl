/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/2D/Resources/GPU2DText.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU 2D-Resource : Fonts & Texts
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// GPU2DFontCollection implementation
inline Bool GPU2DFontCollection::IsBound() const {
    return m_hDevice2DFontCollection.IsCreated();
}

inline Void GPU2DFontCollection::Bind() {
    Assert( !(IsBound()) );
    m_hDevice2DFontCollection.Create();
}
inline Void GPU2DFontCollection::UnBind() {
    Assert( IsBound() );
    m_hDevice2DFontCollection.Destroy();
}

inline UInt GPU2DFontCollection::GetLocaleNameMaxLength() {
    return Device2DFontCollection::GetLocaleNameMaxLength();
}
inline Void GPU2DFontCollection::GetLocaleName( GChar * outLocaleName ) {
    Device2DFontCollection::GetLocaleName( outLocaleName );
}

inline UInt GPU2DFontCollection::GetFontFamilyCount() const {
    Assert( IsBound() );
    return m_hDevice2DFontCollection.GetFontFamilyCount();
}
inline Void GPU2DFontCollection::GetFontFamily( GPU2DFontFamily * outFontFamily, UInt iIndex ) const {
    Assert( IsBound() );
    Assert( iIndex < m_hDevice2DFontCollection.GetFontFamilyCount() );
    m_hDevice2DFontCollection.GetFontFamily( &(outFontFamily->m_hDevice2DFontFamily), iIndex );
}

inline Bool GPU2DFontCollection::FindFontFamily( UInt * outIndex, const GChar * strFontFamilyName ) const {
    Assert( IsBound() );
    return m_hDevice2DFontCollection.FindFontFamily( outIndex, strFontFamilyName );
}

inline Void GPU2DFontCollection::EnumerateFontFamilies( GChar ** outFamilyNames, UInt * outNameLengths, UInt iMaxNames, const GChar * strLocaleName ) const {
    Assert( IsBound() );
    m_hDevice2DFontCollection.EnumerateFontFamilies( outFamilyNames, outNameLengths, iMaxNames, strLocaleName );
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DFontFamily implementation
inline Bool GPU2DFontFamily::IsBound() const {
    return m_hDevice2DFontFamily.IsCreated();
}

inline Void GPU2DFontFamily::UnBind() {
    Assert( IsBound() );
    m_hDevice2DFontFamily.Destroy();
}

inline Bool GPU2DFontFamily::GetFamilyName( GChar * outFamilyName, UInt * pNameLength, const GChar * strLocaleName ) const {
    Assert( IsBound() );
    return m_hDevice2DFontFamily.GetFamilyName( outFamilyName, pNameLength, strLocaleName );
}

inline Void GPU2DFontFamily::GetFontCollection( GPU2DFontCollection * outFontCollection ) const {
    Assert( IsBound() );
    m_hDevice2DFontFamily.GetFontCollection( &(outFontCollection->m_hDevice2DFontCollection) );
}

inline UInt GPU2DFontFamily::GetFontCount() const {
    Assert( IsBound() );
    return m_hDevice2DFontFamily.GetFontCount();
}
inline Void GPU2DFontFamily::GetFont( GPU2DFont * outFont, UInt iIndex ) const {
    Assert( IsBound() );
    m_hDevice2DFontFamily.GetFont( &(outFont->m_hDevice2DFont), iIndex );
}

inline Void GPU2DFontFamily::MatchClosestFont( GPU2DFont * outFont, GPU2DFontWeight iWeight, GPU2DFontStyle iStyle, GPU2DFontStretch iStretch ) const {
    Assert( IsBound() );
    m_hDevice2DFontFamily.MatchClosestFont( &(outFont->m_hDevice2DFont), (Device2DFontWeight)iWeight, (Device2DFontStyle)iStyle, (Device2DFontStretch)iStretch );
}
inline Void GPU2DFontFamily::MatchFonts( GPU2DFontList * outFontList, GPU2DFontWeight iWeight, GPU2DFontStyle iStyle, GPU2DFontStretch iStretch ) const {
    Assert( IsBound() );
    m_hDevice2DFontFamily.MatchFonts( &(outFontList->m_hDevice2DFontList), (Device2DFontWeight)iWeight, (Device2DFontStyle)iStyle, (Device2DFontStretch)iStretch );
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DFont implementation
inline Bool GPU2DFont::IsBound() const {
    return m_hDevice2DFont.IsCreated();
}

inline Void GPU2DFont::UnBind() {
    Assert( IsBound() );
    m_hDevice2DFont.Destroy();
}

inline Bool GPU2DFont::IsSymbolFont() const {
    Assert( IsBound() );
    return m_hDevice2DFont.IsSymbolFont();
}
inline Bool GPU2DFont::HasCharacter( UInt iCharacter ) const {
    Assert( IsBound() );
    return m_hDevice2DFont.HasCharacter( iCharacter );
}

inline Bool GPU2DFont::GetFaceName( GChar * outFaceName, UInt * pNameLength, const GChar * strLocaleName ) const {
    Assert( IsBound() );
    return m_hDevice2DFont.GetFaceName( outFaceName, pNameLength, strLocaleName );
}
inline Bool GPU2DFont::GetInfoString( GChar * outInfoString, UInt * pStringLength, GPU2DFontInfoStringID idInfoString, const GChar * strLocaleName ) const {
    Assert( IsBound() );
    return m_hDevice2DFont.GetInfoString( outInfoString, pStringLength, (Device2DFontInfoStringID)idInfoString, strLocaleName );
}

inline GPU2DFontWeight GPU2DFont::GetWeight() const {
    Assert( IsBound() );
    return (GPU2DFontWeight)( m_hDevice2DFont.GetWeight() );
}
inline GPU2DFontStyle GPU2DFont::GetStyle() const {
    Assert( IsBound() );
    return (GPU2DFontStyle)( m_hDevice2DFont.GetStyle() );
}
inline GPU2DFontStretch GPU2DFont::GetStretch() const {
    Assert( IsBound() );
    return (GPU2DFontStretch)( m_hDevice2DFont.GetStretch() );
}

inline GPU2DFontSimulation GPU2DFont::GetSimulation() const {
    Assert( IsBound() );
    return (GPU2DFontSimulation)( m_hDevice2DFont.GetSimulation() );
}
inline Void GPU2DFont::GetMetrics( GPU2DFontMetrics * outFontMetrics ) const {
    Assert( IsBound() );
    m_hDevice2DFont.GetMetrics( (Device2DFontMetrics*)outFontMetrics );
}

inline Void GPU2DFont::GetFontFamily( GPU2DFontFamily * outFontFamily ) const {
    Assert( IsBound() );
    m_hDevice2DFont.GetFontFamily( &(outFontFamily->m_hDevice2DFontFamily) );
}

inline Void GPU2DFont::CreateFontFace( GPU2DFontFace * outFontFace ) const {
    Assert( IsBound() );
    m_hDevice2DFont.CreateFontFace( &(outFontFace->m_hDevice2DFontFace) );
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DFontList implementation
inline Bool GPU2DFontList::IsBound() const {
    return m_hDevice2DFontList.IsCreated();
}

inline Void GPU2DFontList::UnBind() {
    Assert( IsBound() );
    m_hDevice2DFontList.Destroy();
}

inline Void GPU2DFontList::GetFontCollection( GPU2DFontCollection * outFontCollection ) const {
    Assert( IsBound() );
    m_hDevice2DFontList.GetFontCollection( &(outFontCollection->m_hDevice2DFontCollection) );
}

inline UInt GPU2DFontList::GetFontCount() const {
    Assert( IsBound() );
    return m_hDevice2DFontList.GetFontCount();
}
inline Void GPU2DFontList::GetFont( GPU2DFont * outFont, UInt iIndex ) const {
    Assert( IsBound() );
    m_hDevice2DFontList.GetFont( &(outFont->m_hDevice2DFont), iIndex );
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DFontFace implementation
inline Bool GPU2DFontFace::IsBound() const {
    return m_hDevice2DFontFace.IsCreated();
}

inline Void GPU2DFontFace::UnBind() {
    Assert( IsBound() );
    m_hDevice2DFontFace.Destroy();
}

inline Bool GPU2DFontFace::IsSymbolFont() const {
    Assert( IsBound() );
    return m_hDevice2DFontFace.IsSymbolFont();
}

inline UInt GPU2DFontFace::GetIndex() const {
    Assert( IsBound() );
    return m_hDevice2DFontFace.GetIndex();
}
inline GPU2DFontFaceType GPU2DFontFace::GetType() const {
    Assert( IsBound() );
    return (GPU2DFontFaceType)( m_hDevice2DFontFace.GetType() );
}

inline GPU2DFontSimulation GPU2DFontFace::GetSimulation() const {
    Assert( IsBound() );
    return (GPU2DFontSimulation)( m_hDevice2DFontFace.GetSimulation() );
}
inline Void GPU2DFontFace::GetMetrics( GPU2DFontMetrics * outFontMetrics ) const {
    Assert( IsBound() );
    m_hDevice2DFontFace.GetMetrics( (Device2DFontMetrics*)outFontMetrics );
}
inline Void GPU2DFontFace::GetGdiCompatibleMetrics( GPU2DFontMetrics * outFontMetrics, Float fSizeDIP, Float fPixelsPerDIP, const GPU2DMatrix32 * pTransform ) const {
    Assert( IsBound() );
    m_hDevice2DFontFace.GetGdiCompatibleMetrics( (Device2DFontMetrics*)outFontMetrics, fSizeDIP, fPixelsPerDIP, (const Device2DMatrix32 *)pTransform );
}

inline Void GPU2DFontFace::GetRecommendedRenderingMode( GPU2DTextRenderingMode * outMode, Float fSizeDIP, Float fPixelsPerDIP, GPU2DTextMeasuringMode iMeasuringMode, const GPU2DTextRenderState * pTextRenderState ) const {
    Assert( IsBound() );
    m_hDevice2DFontFace.GetRecommendedRenderingMode( (Device2DTextRenderingMode*)outMode, fSizeDIP, fPixelsPerDIP, (Device2DTextMeasuringMode)iMeasuringMode, &(pTextRenderState->m_hDevice2DTextRenderState) );
}

inline UInt GPU2DFontFace::GetGlyphCount() const {
    Assert( IsBound() );
    return m_hDevice2DFontFace.GetGlyphCount();
}
inline Void GPU2DFontFace::GetGlyphIndices( UShort * outGlyphIndices, const UInt * arrCodePointsUSC4, UInt iCodePointCount ) const {
    Assert( IsBound() );
    m_hDevice2DFontFace.GetGlyphIndices( outGlyphIndices, arrCodePointsUSC4, iCodePointCount );
}

inline Void GPU2DFontFace::GetDesignGlyphMetrics( GPU2DFontGlyphMetrics * outGlyphMetrics, const UShort * arrGlyphIndices, UInt iGlyphCount, Bool bIsSideways ) const {
    Assert( IsBound() );
    m_hDevice2DFontFace.GetDesignGlyphMetrics( (Device2DFontGlyphMetrics*)outGlyphMetrics, arrGlyphIndices, iGlyphCount, bIsSideways );
}
inline Void GPU2DFontFace::GetGdiCompatibleGlyphMetrics( GPU2DFontGlyphMetrics * outGlyphMetrics, const UShort * arrGlyphIndices, UInt iGlyphCount, Bool bIsSideways,
                                                                                   Float fSizeDIP, Float fPixelsPerDIP, Bool bUseGdiNatural, const GPU2DMatrix32 * pTransform ) const {
    Assert( IsBound() );
    m_hDevice2DFontFace.GetGdiCompatibleGlyphMetrics( (Device2DFontGlyphMetrics*)outGlyphMetrics, arrGlyphIndices, iGlyphCount, bIsSideways, fSizeDIP, fPixelsPerDIP, bUseGdiNatural, (const Device2DMatrix32 *)pTransform );
}

inline Void GPU2DFontFace::GetGlyphRunOutline( GPU2DPathGeometry * outGeometry, Float fSizeDIP, const UShort * arrGlyphIndices, UInt iGlyphCount, Bool bIsSideways, Bool bIsRightToLeft,
                                                                                const Float * arrGlyphAdvances, const GPU2DFontGlyphOffset * arrGlyphOffsets ) const {
    Assert( IsBound() );
    Assert( outGeometry->IsBound() );
    Assert( outGeometry->IsOpened() );
    m_hDevice2DFontFace.GetGlyphRunOutline( &(outGeometry->m_hDevice2DPathGeometry), fSizeDIP, arrGlyphIndices, iGlyphCount, bIsSideways, bIsRightToLeft, arrGlyphAdvances, (const Device2DFontGlyphOffset *)arrGlyphOffsets );
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DTextFormat implementation
inline Bool GPU2DTextFormat::IsBound() const {
    return m_hDevice2DTextFormat.IsCreated();
}

inline Void GPU2DTextFormat::Bind( const GChar * strLocaleName, const GChar * strFontFamilyName, GPU2DFontWeight iWeight, GPU2DFontStyle iStyle, GPU2DFontStretch iStretch, Float fSize, GPU2DFontCollection * pFontCollection ) {
    Assert( !(IsBound()) );
    m_hDevice2DTextFormat.Create( strLocaleName, strFontFamilyName, (Device2DFontWeight)iWeight, (Device2DFontStyle)iStyle, (Device2DFontStretch)iStretch, fSize,
                                  (pFontCollection != NULL) ? &(pFontCollection->m_hDevice2DFontCollection) : NULL );
}
inline Void GPU2DTextFormat::UnBind() {
    Assert( IsBound() );
    m_hDevice2DTextFormat.Destroy();
}

inline Void GPU2DTextFormat::GetFontCollection( GPU2DFontCollection * outFontCollection ) const {
    Assert( IsBound() );
    m_hDevice2DTextFormat.GetFontCollection( &(outFontCollection->m_hDevice2DFontCollection) );
}

inline UInt GPU2DTextFormat::GetLocaleNameLength() const {
    Assert( IsBound() );
    return m_hDevice2DTextFormat.GetLocaleNameLength();
}
inline Void GPU2DTextFormat::GetLocaleName( GChar * outLocaleName, UInt iMaxLength ) const {
    Assert( IsBound() );
    m_hDevice2DTextFormat.GetLocaleName( outLocaleName, iMaxLength );
}
inline UInt GPU2DTextFormat::GetFontFamilyNameLength() const {
    Assert( IsBound() );
    return m_hDevice2DTextFormat.GetFontFamilyNameLength();
}
inline Void GPU2DTextFormat::GetFontFamilyName( GChar * outFontFamilyName, UInt iMaxLength ) const {
    Assert( IsBound() );
    m_hDevice2DTextFormat.GetFontFamilyName( outFontFamilyName, iMaxLength );
}

inline GPU2DFontWeight GPU2DTextFormat::GetFontWeight() const {
    Assert( IsBound() );
    return (GPU2DFontWeight)( m_hDevice2DTextFormat.GetFontWeight() );
}
inline GPU2DFontStyle GPU2DTextFormat::GetFontStyle() const {
    Assert( IsBound() );
    return (GPU2DFontStyle)( m_hDevice2DTextFormat.GetFontStyle() );
}
inline GPU2DFontStretch GPU2DTextFormat::GetFontStretch() const {
    Assert( IsBound() );
    return (GPU2DFontStretch)( m_hDevice2DTextFormat.GetFontStretch() );
}
inline Float GPU2DTextFormat::GetFontSize() const {
    Assert( IsBound() );
    return m_hDevice2DTextFormat.GetFontSize();
}

inline GPU2DTextFlowDirection GPU2DTextFormat::GetFlowDirection() const {
    Assert( IsBound() );
    return (GPU2DTextFlowDirection)( m_hDevice2DTextFormat.GetFlowDirection() );
}
inline Void GPU2DTextFormat::SetFlowDirection( GPU2DTextFlowDirection iFlowDirection ) {
    Assert( IsBound() );
    m_hDevice2DTextFormat.SetFlowDirection( (Device2DTextFlowDirection)iFlowDirection );
}

inline GPU2DTextReadingDirection GPU2DTextFormat::GetReadingDirection() const {
    Assert( IsBound() );
    return (GPU2DTextReadingDirection)( m_hDevice2DTextFormat.GetReadingDirection() );
}
inline Void GPU2DTextFormat::SetReadingDirection( GPU2DTextReadingDirection iReadingDirection ) {
    Assert( IsBound() );
    m_hDevice2DTextFormat.SetReadingDirection( (Device2DTextReadingDirection)iReadingDirection );
}

inline GPU2DTextLineSpacingMethod GPU2DTextFormat::GetLineSpacing( Float * outLineSpacing, Float * outBaseline ) const {
    Assert( IsBound() );
    return (GPU2DTextLineSpacingMethod)( m_hDevice2DTextFormat.GetLineSpacing( outLineSpacing, outBaseline ) );
}
inline Void GPU2DTextFormat::SetLineSpacing( GPU2DTextLineSpacingMethod iLineSpacingMethod, Float fLineSpacing, Float fBaseline ) {
    Assert( IsBound() );
    m_hDevice2DTextFormat.SetLineSpacing( (Device2DTextLineSpacingMethod)iLineSpacingMethod, fLineSpacing, fBaseline );
}

inline GPU2DTextAlignment GPU2DTextFormat::GetTextAlignment() const {
    Assert( IsBound() );
    return (GPU2DTextAlignment)( m_hDevice2DTextFormat.GetTextAlignment() );
}
inline Void GPU2DTextFormat::SetTextAlignment( GPU2DTextAlignment iTextAlignment ) {
    Assert( IsBound() );
    m_hDevice2DTextFormat.SetTextAlignment( (Device2DTextAlignment)iTextAlignment );
}

inline GPU2DTextParagraphAlignment GPU2DTextFormat::GetParagraphAlignment() const {
    Assert( IsBound() );
    return (GPU2DTextParagraphAlignment)( m_hDevice2DTextFormat.GetParagraphAlignment() );
}
inline Void GPU2DTextFormat::SetParagraphAlignment( GPU2DTextParagraphAlignment iParagraphAlignment ) {
    Assert( IsBound() );
    m_hDevice2DTextFormat.SetParagraphAlignment( (Device2DTextParagraphAlignment)iParagraphAlignment );
}

inline Float GPU2DTextFormat::GetIncrementalTabStop() const {
    Assert( IsBound() );
    return m_hDevice2DTextFormat.GetIncrementalTabStop();
}
inline Void GPU2DTextFormat::SetIncrementalTabStop( Float fIncrementalTabStop ) {
    Assert( IsBound() );
    m_hDevice2DTextFormat.SetIncrementalTabStop( fIncrementalTabStop );
}

inline GPU2DTextTrimmingGranularity GPU2DTextFormat::GetTrimming( GChar * outDelimiter, UInt * outDelimiterCount ) const {
    Assert( IsBound() );
    return (GPU2DTextTrimmingGranularity)( m_hDevice2DTextFormat.GetTrimming( outDelimiter, outDelimiterCount ) );
}
inline Void GPU2DTextFormat::SetTrimming( GPU2DTextTrimmingGranularity iTrimmingGranularity, GChar chDelimiter, UInt iDelimiterCount ) {
    Assert( IsBound() );
    m_hDevice2DTextFormat.SetTrimming( (Device2DTextTrimmingGranularity)iTrimmingGranularity, chDelimiter, iDelimiterCount );
}

inline GPU2DTextWordWrapping GPU2DTextFormat::GetWordWrapping() const {
    Assert( IsBound() );
    return (GPU2DTextWordWrapping)( m_hDevice2DTextFormat.GetWordWrapping() );
}
inline Void GPU2DTextFormat::SetWordWrapping( GPU2DTextWordWrapping iWordWrapping ) {
    Assert( IsBound() );
    m_hDevice2DTextFormat.SetWordWrapping( (Device2DTextWordWrapping)iWordWrapping );
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DTextInlineObject implementation
inline Void GPU2DTextInlineObject::GetMetrics( GPU2DTextInlineObjectMetrics * outMetrics ) const {
    m_hTextInlineObject.GetMetrics( (Device2DTextInlineObjectMetrics*)outMetrics );
}
inline Void GPU2DTextInlineObject::GetOverhangMetrics( GPU2DTextOverhangMetrics * outOverhangMetrics ) const {
    m_hTextInlineObject.GetOverhangMetrics( (Device2DTextOverhangMetrics*)outOverhangMetrics );
}
inline GPU2DTextBreakCondition GPU2DTextInlineObject::GetBreakConditionBefore() const {
    return (GPU2DTextBreakCondition)( m_hTextInlineObject.GetBreakConditionBefore() );
}
inline GPU2DTextBreakCondition GPU2DTextInlineObject::GetBreakConditionAfter() const {
    return (GPU2DTextBreakCondition)( m_hTextInlineObject.GetBreakConditionAfter() );
}

inline Void GPU2DTextInlineObject::SetMetrics( const GPU2DTextInlineObjectMetrics * pMetrics ) {
    m_hTextInlineObject.SetMetrics( (const Device2DTextInlineObjectMetrics *)pMetrics );
}
inline Void GPU2DTextInlineObject::SetOverhangMetrics( const GPU2DTextOverhangMetrics * pOverhangMetrics ) {
    m_hTextInlineObject.SetOverhangMetrics( (const Device2DTextOverhangMetrics *)pOverhangMetrics );
}
inline Void GPU2DTextInlineObject::SetBreakConditionBefore( GPU2DTextBreakCondition iBreakCondition ) {
    m_hTextInlineObject.SetBreakConditionBefore( (Device2DTextBreakCondition)iBreakCondition );
}
inline Void GPU2DTextInlineObject::SetBreakConditionAfter( GPU2DTextBreakCondition iBreakCondition ) {
    m_hTextInlineObject.SetBreakConditionAfter( (Device2DTextBreakCondition)iBreakCondition );
}

inline Void GPU2DTextInlineObject::_TextInlineObject::Draw( const Device2DPoint * pDstPoint, Bool bIsSideways, Bool bIsRightToleft ) const {
    m_pParentHandler->Draw( (const GPU2DPoint *)pDstPoint, bIsSideways, bIsRightToleft );
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DTextLayout implementation
inline Bool GPU2DTextLayout::IsBound() const {
    return m_hDevice2DTextLayout.IsCreated();
}

inline Void GPU2DTextLayout::Bind( GPU2DTextFormat * pTextFormat, Float fMaxWidth, Float fMaxHeight, const GChar * strText, UInt iTextLength ) {
    Assert( !(IsBound()) );
    m_hDevice2DTextLayout.Create( &(pTextFormat->m_hDevice2DTextFormat), fMaxWidth, fMaxHeight, strText, iTextLength );
}
inline Void GPU2DTextLayout::UnBind() {
    Assert( IsBound() );
    m_hDevice2DTextLayout.Destroy();
}

inline Float GPU2DTextLayout::GetMaxWidth() const {
    Assert( IsBound() );
    return m_hDevice2DTextLayout.GetMaxWidth();
}
inline Void GPU2DTextLayout::SetMaxWidth( Float fMaxWidth ) {
    Assert( IsBound() );
    m_hDevice2DTextLayout.SetMaxWidth( fMaxWidth );
}

inline Float GPU2DTextLayout::GetMaxHeight() const {
    Assert( IsBound() );
    return m_hDevice2DTextLayout.GetMaxHeight();
}
inline Void GPU2DTextLayout::SetMaxHeight( Float fMaxHeight ) {
    Assert( IsBound() );
    m_hDevice2DTextLayout.SetMaxHeight( fMaxHeight );
}

inline Void GPU2DTextLayout::GetMetrics( GPU2DTextMetrics * outTextMetrics ) const {
    Assert( IsBound() );
    m_hDevice2DTextLayout.GetMetrics( (Device2DTextMetrics*)outTextMetrics );
}
inline Void GPU2DTextLayout::GetOverhangMetrics( GPU2DTextOverhangMetrics * outOverhangMetrics ) const {
    Assert( IsBound() );
    m_hDevice2DTextLayout.GetOverhangMetrics( (Device2DTextOverhangMetrics*)outOverhangMetrics );
}

inline UInt GPU2DTextLayout::GetLineMetricCount() const {
    Assert( IsBound() );
    return m_hDevice2DTextLayout.GetLineMetricCount();
}
inline Void GPU2DTextLayout::GetLineMetrics( GPU2DTextLineMetrics * outLineMetrics, UInt iMaxLines ) const {
    Assert( IsBound() );
    m_hDevice2DTextLayout.GetLineMetrics( (Device2DTextLineMetrics*)outLineMetrics, iMaxLines );
}

inline UInt GPU2DTextLayout::GetClusterMetricCount() const {
    Assert( IsBound() );
    return m_hDevice2DTextLayout.GetClusterMetricCount();
}
inline Void GPU2DTextLayout::GetClusterMetrics( GPU2DTextClusterMetrics * outClusterMetrics, UInt iMaxClusters ) const {
    Assert( IsBound() );
    m_hDevice2DTextLayout.GetClusterMetrics( (Device2DTextClusterMetrics*)outClusterMetrics, iMaxClusters );
}

inline Float GPU2DTextLayout::ComputeMinWidth() const {
    Assert( IsBound() );
    return m_hDevice2DTextLayout.ComputeMinWidth();
}

inline Void GPU2DTextLayout::HitTestPoint( GPU2DTextHitTestMetrics * outHitTestMetrics, Bool * outIsInside, Bool * outIsTrailingHit, Float fX, Float fY ) const {
    Assert( IsBound() );
    m_hDevice2DTextLayout.HitTestPoint( (Device2DTextHitTestMetrics*)outHitTestMetrics, outIsInside, outIsTrailingHit, fX, fY );
}

inline Void GPU2DTextLayout::HitTestTextPosition( GPU2DTextHitTestMetrics * outHitTestMetrics, Float * outX, Float * outY, UInt iPosition, Bool bIsTrailingHit ) const {
    Assert( IsBound() );
    m_hDevice2DTextLayout.HitTestTextPosition( (Device2DTextHitTestMetrics*)outHitTestMetrics, outX, outY, iPosition, bIsTrailingHit );
}

inline UInt GPU2DTextLayout::HitTestTextRangeCount( const GPU2DTextRange * pRange, Float fOriginX, Float fOriginY ) const {
    Assert( IsBound() );
    return m_hDevice2DTextLayout.HitTestTextRangeCount( (const Device2DTextRange *)pRange, fOriginX, fOriginY );
}
inline Void GPU2DTextLayout::HitTestTextRange( GPU2DTextHitTestMetrics * outHitTestMetrics, UInt iMaxHitTests, const GPU2DTextRange * pRange, Float fOriginX, Float fOriginY ) const {
    Assert( IsBound() );
    m_hDevice2DTextLayout.HitTestTextRange( (Device2DTextHitTestMetrics*)outHitTestMetrics, iMaxHitTests, (const Device2DTextRange *)pRange, fOriginX, fOriginY );
}

inline UInt GPU2DTextLayout::GetLocaleNameLength( UInt iPosition, GPU2DTextRange * outRange ) const {
    Assert( IsBound() );
    return m_hDevice2DTextLayout.GetLocaleNameLength( iPosition, (Device2DTextRange*)outRange );
}
inline Void GPU2DTextLayout::GetLocaleName( GChar * outLocaleName, UInt iNameSize, UInt iPosition, GPU2DTextRange * outRange ) const {
    Assert( IsBound() );
    m_hDevice2DTextLayout.GetLocaleName( outLocaleName, iNameSize, iPosition, (Device2DTextRange*)outRange );
}
inline Void GPU2DTextLayout::SetLocaleName( const GChar * strLocaleName, const GPU2DTextRange * pRange ) {
    Assert( IsBound() );
    m_hDevice2DTextLayout.SetLocaleName( strLocaleName, (const Device2DTextRange *)pRange );
}

inline Void GPU2DTextLayout::GetFontCollection( GPU2DFontCollection * outFontCollection, UInt iPosition, GPU2DTextRange * outRange ) const {
    Assert( IsBound() );
    m_hDevice2DTextLayout.GetFontCollection( &(outFontCollection->m_hDevice2DFontCollection), iPosition, (Device2DTextRange*)outRange );
}
inline Void GPU2DTextLayout::SetFontCollection( GPU2DFontCollection * pFontCollection, const GPU2DTextRange * pRange ) {
    Assert( IsBound() );
    m_hDevice2DTextLayout.SetFontCollection( &(pFontCollection->m_hDevice2DFontCollection), (const Device2DTextRange *)pRange );
}

inline UInt GPU2DTextLayout::GetFontFamilyNameLength( UInt iPosition, GPU2DTextRange * outRange ) const {
    Assert( IsBound() );
    return m_hDevice2DTextLayout.GetFontFamilyNameLength( iPosition, (Device2DTextRange*)outRange );
}
inline Void GPU2DTextLayout::GetFontFamilyName( GChar * outFamilyName, UInt iNameSize, UInt iPosition, GPU2DTextRange * outRange ) const {
    Assert( IsBound() );
    m_hDevice2DTextLayout.GetFontFamilyName( outFamilyName, iNameSize, iPosition, (Device2DTextRange*)outRange );
}
inline Void GPU2DTextLayout::SetFontFamilyName( const GChar * strFontFamilyName, const GPU2DTextRange * pRange ) {
    Assert( IsBound() );
    m_hDevice2DTextLayout.SetFontFamilyName( strFontFamilyName, (const Device2DTextRange *)pRange );
}

inline GPU2DFontWeight GPU2DTextLayout::GetFontWeight( UInt iPosition, GPU2DTextRange * outRange ) const {
    Assert( IsBound() );
    return (GPU2DFontWeight)( m_hDevice2DTextLayout.GetFontWeight( iPosition, (Device2DTextRange*)outRange ) );
}
inline Void GPU2DTextLayout::SetFontWeight( GPU2DFontWeight iWeight, const GPU2DTextRange * pRange ) {
    Assert( IsBound() );
    m_hDevice2DTextLayout.SetFontWeight( (Device2DFontWeight)iWeight, (const Device2DTextRange *)pRange );
}

inline GPU2DFontStyle GPU2DTextLayout::GetFontStyle( UInt iPosition, GPU2DTextRange * outRange ) const {
    Assert( IsBound() );
    return (GPU2DFontStyle)( m_hDevice2DTextLayout.GetFontStyle( iPosition, (Device2DTextRange*)outRange ) );
}
inline Void GPU2DTextLayout::SetFontStyle( GPU2DFontStyle iStyle, const GPU2DTextRange * pRange ) {
    Assert( IsBound() );
    m_hDevice2DTextLayout.SetFontStyle( (Device2DFontStyle)iStyle, (const Device2DTextRange *)pRange );
}

inline GPU2DFontStretch GPU2DTextLayout::GetFontStretch( UInt iPosition, GPU2DTextRange * outRange ) const {
    Assert( IsBound() );
    return (GPU2DFontStretch)( m_hDevice2DTextLayout.GetFontStretch( iPosition, (Device2DTextRange*)outRange ) );
}
inline Void GPU2DTextLayout::SetFontStretch( GPU2DFontStretch iStretch, const GPU2DTextRange * pRange ) {
    Assert( IsBound() );
    m_hDevice2DTextLayout.SetFontStretch( (Device2DFontStretch)iStretch, (const Device2DTextRange *)pRange );
}

inline Float GPU2DTextLayout::GetFontSize( UInt iPosition, GPU2DTextRange * outRange ) const {
    Assert( IsBound() );
    return m_hDevice2DTextLayout.GetFontSize( iPosition, (Device2DTextRange*)outRange );
}
inline Void GPU2DTextLayout::SetFontSize( Float fSize, const GPU2DTextRange * pRange ) {
    Assert( IsBound() );
    m_hDevice2DTextLayout.SetFontSize( fSize, (const Device2DTextRange *)pRange );
}

inline Bool GPU2DTextLayout::HasUnderline( UInt iPosition, GPU2DTextRange * outRange ) const {
    Assert( IsBound() );
    return m_hDevice2DTextLayout.HasUnderline( iPosition, (Device2DTextRange*)outRange );
}
inline Void GPU2DTextLayout::SetUnderline( Bool bUnderline, const GPU2DTextRange * pRange ) {
    Assert( IsBound() );
    m_hDevice2DTextLayout.SetUnderline( bUnderline, (const Device2DTextRange *)pRange );
}

inline Bool GPU2DTextLayout::HasStrikethrough( UInt iPosition, GPU2DTextRange * outRange ) const {
    Assert( IsBound() );
    return m_hDevice2DTextLayout.HasStrikethrough( iPosition, (Device2DTextRange*)outRange );
}
inline Void GPU2DTextLayout::SetStrikethrough( Bool bStrikethrough, const GPU2DTextRange * pRange ) {
    Assert( IsBound() );
    m_hDevice2DTextLayout.SetStrikethrough( bStrikethrough, (const Device2DTextRange *)pRange );
}

inline Void GPU2DTextLayout::GetForegroundBrush( GPU2DSolidColorBrush * outBrush, UInt iPosition, GPU2DTextRange * outRange ) const {
    Assert( IsBound() );
    m_hDevice2DTextLayout.GetForegroundBrush( &(outBrush->m_hDevice2DSolidColorBrush), iPosition, (Device2DTextRange*)outRange );
}
inline Void GPU2DTextLayout::SetForegroundBrush( GPU2DSolidColorBrush * pBrush, const GPU2DTextRange * pRange ) {
    Assert( IsBound() );
    m_hDevice2DTextLayout.SetForegroundBrush( &(pBrush->m_hDevice2DSolidColorBrush), (const Device2DTextRange *)pRange );
}

inline GPU2DTextInlineObject * GPU2DTextLayout::GetInlineObject( UInt iPosition, GPU2DTextRange * outRange ) const {
    Assert( IsBound() );
    GPU2DTextInlineObject::_TextInlineObject * pObject = (GPU2DTextInlineObject::_TextInlineObject*)( m_hDevice2DTextLayout.GetInlineObject( iPosition, (Device2DTextRange*)outRange ) );
    return pObject->m_pParentHandler;
}
inline Void GPU2DTextLayout::SetInlineObject( GPU2DTextInlineObject * pInlineObject, const GPU2DTextRange * pRange ) {
    Assert( IsBound() );
    m_hDevice2DTextLayout.SetInlineObject( &(pInlineObject->m_hTextInlineObject), (const Device2DTextRange *)pRange );
}
