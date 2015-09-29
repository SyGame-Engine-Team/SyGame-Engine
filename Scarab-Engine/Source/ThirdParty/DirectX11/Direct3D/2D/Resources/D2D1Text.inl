/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/2D/Resources/D2D1Text.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : D2D1 Dev-Ind Resource : Fonts & Texts.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// D2D1FontCollection implementation
inline Bool D2D1FontCollection::IsCreated() const {
    return ( m_pFontCollection != NULL );
}

/////////////////////////////////////////////////////////////////////////////////
// D2D1FontFamily implementation
inline Bool D2D1FontFamily::IsCreated() const {
    return ( m_pFontFamily != NULL );
}

/////////////////////////////////////////////////////////////////////////////////
// D2D1Font implementation
inline Bool D2D1Font::IsCreated() const {
    return ( m_pFont != NULL );
}

/////////////////////////////////////////////////////////////////////////////////
// D2D1FontList implementation
inline Bool D2D1FontList::IsCreated() const {
    return ( m_pFontList != NULL );
}

/////////////////////////////////////////////////////////////////////////////////
// D2D1FontFace implementation
inline Bool D2D1FontFace::IsCreated() const {
    return ( m_pFontFace != NULL );
}

/////////////////////////////////////////////////////////////////////////////////
// D2D1TextFormat implementation
inline Bool D2D1TextFormat::IsCreated() const {
    return ( m_pTextFormat != NULL );
}

/////////////////////////////////////////////////////////////////////////////////
// D2D1TextInlineObject implementation
inline Void D2D1TextInlineObject::GetMetrics( D2D1TextInlineObjectMetrics * outMetrics ) const {
    outMetrics->fWidth = m_hInlineObjectMetrics.fWidth;
    outMetrics->fHeight = m_hInlineObjectMetrics.fHeight;
    outMetrics->fBaseline = m_hInlineObjectMetrics.fBaseline;
    outMetrics->bSupportSideways = m_hInlineObjectMetrics.bSupportSideways;
}
inline Void D2D1TextInlineObject::SetMetrics( const D2D1TextInlineObjectMetrics * pMetrics ) {
    m_hInlineObjectMetrics.fWidth = pMetrics->fWidth;
    m_hInlineObjectMetrics.fHeight = pMetrics->fHeight;
    m_hInlineObjectMetrics.fBaseline = pMetrics->fBaseline;
    m_hInlineObjectMetrics.bSupportSideways = pMetrics->bSupportSideways;
}

inline Void D2D1TextInlineObject::GetOverhangMetrics( D2D1TextOverhangMetrics * outOverhangMetrics ) const {
    outOverhangMetrics->fLeft = m_hOverhangMetrics.fLeft;
    outOverhangMetrics->fTop = m_hOverhangMetrics.fTop;
    outOverhangMetrics->fRight = m_hOverhangMetrics.fRight;
    outOverhangMetrics->fBottom = m_hOverhangMetrics.fBottom;
}
inline Void D2D1TextInlineObject::SetOverhangMetrics( const D2D1TextOverhangMetrics * pOverhangMetrics ) {
    m_hOverhangMetrics.fLeft = pOverhangMetrics->fLeft;
    m_hOverhangMetrics.fTop = pOverhangMetrics->fTop;
    m_hOverhangMetrics.fRight = pOverhangMetrics->fRight;
    m_hOverhangMetrics.fBottom = pOverhangMetrics->fBottom;
}

inline D2D1TextBreakCondition D2D1TextInlineObject::GetBreakConditionBefore() const {
    return m_iBreakBefore;
}
inline D2D1TextBreakCondition D2D1TextInlineObject::GetBreakConditionAfter() const {
    return m_iBreakAfter;
}
inline Void D2D1TextInlineObject::SetBreakConditionBefore( D2D1TextBreakCondition iBreakCondition ) {
    m_iBreakBefore = iBreakCondition;
}
inline Void D2D1TextInlineObject::SetBreakConditionAfter( D2D1TextBreakCondition iBreakCondition ) {
    m_iBreakAfter = iBreakCondition;
}

/////////////////////////////////////////////////////////////////////////////////
// D2D1TextLayout implementation
inline Bool D2D1TextLayout::IsCreated() const {
    return ( m_pTextLayout != NULL );
}


