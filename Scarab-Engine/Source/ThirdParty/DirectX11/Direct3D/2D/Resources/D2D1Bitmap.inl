/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/2D/Resources/D2D1Bitmap.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : D2D1 Dev-Dep Resource : Bitmaps.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// D2D1Bitmap implementation
inline Bool D2D1Bitmap::IsCreated() const {
    return ( m_pBitmap != NULL || m_bTemporaryDestroyed );
}
inline Void D2D1Bitmap::UpdateDataSource( const Void * pData ) {
    m_hCreationParameters.pData = pData;
}

inline PixelFormat D2D1Bitmap::GetPixelFormat() const {
    DebugAssert( IsCreated() );
    return PixelFormatFromDXGI[m_hBitmapProperties.pixelFormat.format];
}
inline D2D1BitmapAlphaMode D2D1Bitmap::GetAlphaMode() const {
    DebugAssert( IsCreated() );
    return D2D1BitmapAlphaModeFromD2D1[m_hBitmapProperties.pixelFormat.alphaMode];
}

inline Float D2D1Bitmap::GetDpiX() const {
    DebugAssert( IsCreated() );
    return m_hBitmapProperties.dpiX;
}
inline Float D2D1Bitmap::GetDpiY() const {
    DebugAssert( IsCreated() );
    return m_hBitmapProperties.dpiY;
}

inline UInt D2D1Bitmap::GetWidth() const {
    DebugAssert( IsCreated() );
    return m_hSize.width;
}
inline UInt D2D1Bitmap::GetHeight() const {
    DebugAssert( IsCreated() );
    return m_hSize.height;
}

inline Float D2D1Bitmap::GetDIPWidth() const {
    DebugAssert( IsCreated() );
    return m_hDIPSize.width;
}
inline Float D2D1Bitmap::GetDIPHeight() const {
    DebugAssert( IsCreated() );
    return m_hDIPSize.height;
}

inline Void D2D1Bitmap::GetDesc( D2D1BitmapDesc * outDesc ) const {
    DebugAssert( IsCreated() );
    outDesc->ConvertFrom( &m_hBitmapProperties, &m_hSize, &m_hDIPSize );
}

