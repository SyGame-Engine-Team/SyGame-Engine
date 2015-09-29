/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/2D/Resources/GPU2DBitmap.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU 2D-Resource : Bitmaps
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// GPU2DBitmap implementation
inline Bool GPU2DBitmap::IsBound() const {
    return m_hDevice2DBitmap.IsCreated();
}

inline Bool GPU2DBitmap::HasCPUData() const {
    return ( m_arrBitmap != NULL );
}

inline ResourceType GPU2DBitmap::GetResourceType() const {
    return RESOURCE_TYPE_BITMAP;
}

inline PixelFormat GPU2DBitmap::GetFormat() const {
    return m_iFormat;
}
inline GPU2DBitmapAlphaMode GPU2DBitmap::GetAlphaMode() const {
    return m_iAlphaMode;
}
inline UInt GPU2DBitmap::GetStride() const {
    return m_iStride;
}

inline UInt GPU2DBitmap::GetWidth() const {
    return m_iWidth;
}
inline UInt GPU2DBitmap::GetHeight() const {
    return m_iHeight;
}
inline UInt GPU2DBitmap::GetPitch() const {
    return m_iPitch;
}

inline Float GPU2DBitmap::GetDpiX() const {
    return m_fDpiX;
}
inline Float GPU2DBitmap::GetDpiY() const {
    return m_fDpiY;
}

inline Float GPU2DBitmap::GetDIPWidth() const {
    return ( m_iWidth / (m_fDpiX / 96.0f) );
}
inline Float GPU2DBitmap::GetDIPHeight() const {
    return ( m_iHeight / (m_fDpiY / 96.0f) );
}

inline Void GPU2DBitmap::GetDesc( GPU2DBitmapDesc * outDesc ) const {
    outDesc->iFormat = m_iFormat;
    outDesc->iAlphaMode = m_iAlphaMode;
    outDesc->fDpiX = m_fDpiX;
    outDesc->fDpiY = m_fDpiY;
    outDesc->iWidth = m_iWidth;
    outDesc->iHeight = m_iHeight;
    outDesc->fDIPWidth = ( m_iWidth / (m_fDpiX / 96.0f) );
    outDesc->fDIPHeight = ( m_iHeight / (m_fDpiY / 96.0f) );
}

inline const Byte * GPU2DBitmap::GetBitmap() const {
    Assert( HasCPUData() );
    return m_arrBitmap;
}
inline Byte * GPU2DBitmap::GetBitmap() {
    Assert( HasCPUData() );
    return m_arrBitmap;
}

inline const Byte * GPU2DBitmap::GetPixel( const GPU2DPointI * pPoint ) const {
    Assert( HasCPUData() );
    Assert( pPoint->iX < m_iWidth );
    Assert( pPoint->iY < m_iHeight );
    return ( m_arrBitmap + (pPoint->iY * m_iPitch) + (pPoint->iX * m_iStride) );
}
inline Byte * GPU2DBitmap::GetPixel( const GPU2DPointI * pPoint ) {
    Assert( HasCPUData() );
    Assert( pPoint->iX < m_iWidth );
    Assert( pPoint->iY < m_iHeight );
    return ( m_arrBitmap + (pPoint->iY * m_iPitch) + (pPoint->iX * m_iStride) );
}
inline Void GPU2DBitmap::SetPixel( const Byte * pPixel, const GPU2DPointI * pPoint ) {
    Assert( HasCPUData() );
    Assert( pPoint->iX < m_iWidth );
    Assert( pPoint->iY < m_iHeight );
    MemCopy( m_arrBitmap + (pPoint->iY * m_iPitch) + (pPoint->iX * m_iStride), pPixel, m_iStride );
}

template<typename T>
inline const T * GPU2DBitmap::GetPixel( const GPU2DPointI * pPoint ) const {
    Assert( m_iStride == sizeof(T) );
    return (const T *)GetPixel( pPoint );
}
template<typename T>
inline T * GPU2DBitmap::GetPixel( const GPU2DPointI * pPoint ) {
    Assert( m_iStride == sizeof(T) );
    return (T *)GetPixel( pPoint );
}
template<typename T>
inline Void GPU2DBitmap::SetPixel( const T * pPixel, const GPU2DPointI * pPoint ) {
    Assert( m_iStride == sizeof(T) );
    SetPixel( (const Byte *)pPixel, pPoint );
}
template<typename T>
inline Void GPU2DBitmap::SetPixels( const T * arrPixels, UInt iPixelsPitch, const GPU2DRectangleI * pRect ) {
    Assert( m_iStride == sizeof(T) );
    SetPixels( (const Byte *)arrPixels, iPixelsPitch, pRect );
}

inline Void GPU2DBitmap::Update( const GPU2DRectangleI * pRect ) {
    Assert( HasCPUData() && IsBound() );
    m_hDevice2DBitmap.CopyFrom( m_arrBitmap, m_iPitch, (const Device2DRectangleI *)pRect );
}

inline Void GPU2DBitmap::CopyFrom( const Void * pSrcData, UInt iSrcPitch, const GPU2DRectangleI * pDstRect ) {
    Assert( IsBound() );
    m_hDevice2DBitmap.CopyFrom( pSrcData, iSrcPitch, (const Device2DRectangleI *)pDstRect );
}
inline Void GPU2DBitmap::CopyFrom( GPU2DBitmap * pSrcBitmap, const GPU2DPointI * pDstPoint, const GPU2DRectangleI * pSrcRect ) {
    Assert( IsBound() );
    Assert( pSrcBitmap->IsBound() );
    m_hDevice2DBitmap.CopyFrom( &(pSrcBitmap->m_hDevice2DBitmap), (const Device2DPointI *)pDstPoint, (const Device2DRectangleI *)pSrcRect );
}

