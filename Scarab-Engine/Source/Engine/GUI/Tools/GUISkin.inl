/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Tools/GUISkin.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Container for all resources needed for GUI rendering
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// GUISkin implementation
inline GPU2DBitmap * GUISkin::GetSkinBitmap() const {
    return m_pSkinBitmap;
}
inline const Rectangle2 & GUISkin::GetLocation( GUISkinImage iImageID ) const {
    Assert( iImageID < GUISKIN_IMAGE_COUNT );
    return m_arrLocations[iImageID];
}

inline GPU2DSolidColorBrush * GUISkin::GetSkinColorBrush() const {
    return &m_hColorBrush;
}
inline const Color4 & GUISkin::GetColor( GUISkinColor iColorID ) const {
    Assert( iColorID < GUISKIN_COLOR_COUNT );
    return m_arrColors[iColorID];
}

inline GPU2DSolidColorBrush * GUISkin::GetSkinTextBrush() const {
    return &m_hTextBrush;
}
inline GPU2DTextFormat * GUISkin::GetSkinTextFormat( GUISkinFont iFontID ) const {
    Assert( iFontID < GUISKIN_FONT_COUNT );
    return m_arrTextFormats[iFontID];
}
