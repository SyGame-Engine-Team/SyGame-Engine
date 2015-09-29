/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Resources/GPUTexture.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU Resource : Textures
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// GPUTexture implementation
inline UInt GPUTexture::GetDimensions( GPUResourceType iTextureType ) {
    Assert( iTextureType >= GPURESOURCE_TEXTURE_1D );
    return sm_arrDimensions[iTextureType - GPURESOURCE_TEXTURE_1D];
}

inline Bool GPUTexture::IsBound() const {
    return m_pDeviceTexture->IsCreated();
}

inline Bool GPUTexture::HasCPUData() const {
    return ( m_arrTexture != NULL );
}

inline ResourceType GPUTexture::GetResourceType() const {
    return RESOURCE_TYPE_TEXTURE;
}

inline Bool GPUTexture::IsMipMapable() const {
    return PixelFormatIsMipMapable( m_iFormat );
}
inline Bool GPUTexture::IsCompressed() const {
    return PixelFormatIsCompressed( m_iFormat );
}

inline PixelFormat GPUTexture::GetFormat() const {
    return m_iFormat;
}
inline UInt GPUTexture::GetStride() const {
    return m_iStride;
}

inline UInt GPUTexture::GetDimensions() const {
    return m_iDimensions;
}

inline Bool GPUTexture::HasMipMap() const {
    return ( m_iMipLevelCount > 1 );
}
inline UInt GPUTexture::GetMipMapSize() const {
    return m_iMipMapSize;
}
inline UInt GPUTexture::GetMipLevelCount() const {
    return m_iMipLevelCount;
}
inline UInt GPUTexture::GetMipLevelBound( UInt iDimension, UInt iMipLevel ) const {
    Assert( iDimension < m_iDimensions );
    Assert( iMipLevel < m_iMipLevelCount );
    return m_arrMipLevelBounds[iMipLevel][iDimension];
}
inline UInt GPUTexture::GetMipLevelOffset( UInt iMipLevel ) const {
    Assert( iMipLevel < m_iMipLevelCount );
    return m_arrMipLevelOffsets[iMipLevel];
}
inline UInt GPUTexture::GetMipLevelSize( UInt iMipLevel ) const {
    Assert( iMipLevel < m_iMipLevelCount );
    return m_arrMipLevelSizes[iMipLevel];
}

inline Bool GPUTexture::IsArray() const {
    return ( m_iArrayCount > 1 );
}
inline UInt GPUTexture::GetArrayCount() const {
    return m_iArrayCount;
}

inline UInt GPUTexture::GetTextureCount() const {
    return m_iSubResourceCount;
}
inline UInt GPUTexture::GetSize() const {
    return m_iSize;
}

inline const Byte * GPUTexture::GetTexture( const GPUTextureIndex * pIndex ) const {
    Assert( HasCPUData() );
    return _GetSubResource( pIndex );
}
inline Byte * GPUTexture::GetTexture( const GPUTextureIndex * pIndex ) {
    Assert( HasCPUData() );
    return _GetSubResource( pIndex );
}
inline Void GPUTexture::SetTexture( const Byte * pTexture, const GPUTextureIndex * pIndex ) {
    Assert( HasCPUData() );
    MemCopy( _GetSubResource(pIndex), pTexture, _GetSubResourceSize(pIndex) );
}

/////////////////////////////////////////////////////////////////////////////////

inline UInt * GPUTexture::_MakeBoundArray( UInt iBound0, UInt iBound1, UInt iBound2 ) {
    static UInt s_arrBounds[GPUTEXTURE_MAX_DIMENSIONS] = { 0, 0, 0 };
    s_arrBounds[0] = iBound0;
    s_arrBounds[1] = iBound1;
    s_arrBounds[2] = iBound2;
    return s_arrBounds;
}
inline Byte * GPUTexture::_GetSubResource( const GPUTextureIndex * pIndex ) const {
    Assert( HasCPUData() );
    if ( pIndex == NULL )
        return m_arrTexture;
    Assert( pIndex->iArrayIndex < m_iArrayCount );
    Assert( pIndex->iMipIndex < m_iMipLevelCount );
    return ( m_arrTexture + (pIndex->iArrayIndex * m_iMipMapSize) + m_arrMipLevelOffsets[pIndex->iMipIndex] );
}
inline Byte * GPUTexture::_GetSubResource( const GPUTextureIndex * pIndex, GPUTextureCubeFace iFace ) const {
    Assert( HasCPUData() );
    if ( pIndex == NULL )
        return ( m_arrTexture + (iFace * m_iMipMapSize) );
    Assert( pIndex->iArrayIndex < m_iArrayCount );
    Assert( pIndex->iMipIndex < m_iMipLevelCount );
    return ( m_arrTexture + ((pIndex->iArrayIndex * 6 + iFace) * m_iMipMapSize) + m_arrMipLevelOffsets[pIndex->iMipIndex] );
}
inline UInt GPUTexture::_GetSubResourceBound( UInt iDimension, const GPUTextureIndex * pIndex ) const {
    Assert( iDimension < m_iDimensions );
    if ( pIndex == NULL )
        return m_arrMipLevelBounds[0][iDimension];
    Assert( pIndex->iMipIndex < m_iMipLevelCount );
    return m_arrMipLevelBounds[pIndex->iMipIndex][iDimension];
}
inline UInt GPUTexture::_GetSubResourcePitch( const GPUTextureIndex * pIndex ) const {
    return ( m_iStride * _GetSubResourceBound(0, pIndex) );
}
inline UInt GPUTexture::_GetSubResourceSlice( const GPUTextureIndex * pIndex ) const {
    return ( _GetSubResourcePitch(pIndex) * _GetSubResourceBound(1, pIndex) );
}
inline UInt GPUTexture::_GetSubResourceOffset( const GPUTextureIndex * pIndex ) const {
    if ( pIndex == NULL )
        return m_arrMipLevelOffsets[0];
    Assert( pIndex->iMipIndex < m_iMipLevelCount );
    return m_arrMipLevelOffsets[pIndex->iMipIndex];
}
inline UInt GPUTexture::_GetSubResourceSize( const GPUTextureIndex * pIndex ) const {
    if ( pIndex == NULL )
        return m_arrMipLevelSizes[0];
    Assert( pIndex->iMipIndex < m_iMipLevelCount );
    return m_arrMipLevelSizes[pIndex->iMipIndex];
}

/////////////////////////////////////////////////////////////////////////////////
// GPUTexture1D implementation
inline UInt GPUTexture1D::GetWidth( UInt iMipLevel ) const {
    return GetMipLevelBound( 0, iMipLevel );
}

inline const Byte * GPUTexture1D::GetTexel( UInt iPoint, const GPUTextureIndex * pIndex ) const {
    Assert( HasCPUData() );
    Assert( iPoint < _GetSubResourceBound(0, pIndex) );
    return ( _GetSubResource(pIndex) + (iPoint * m_iStride) );
}
inline Byte * GPUTexture1D::GetTexel( UInt iPoint, const GPUTextureIndex * pIndex ) {
    Assert( HasCPUData() );
    Assert( iPoint < _GetSubResourceBound(0, pIndex) );
    return ( _GetSubResource(pIndex) + (iPoint * m_iStride) );
}
inline Void GPUTexture1D::SetTexel( const Byte * pTexel, UInt iPoint, const GPUTextureIndex * pIndex ) {
    Assert( HasCPUData() );
    Assert( iPoint < _GetSubResourceBound(0, pIndex) );
    MemCopy( _GetSubResource(pIndex) + (iPoint * m_iStride), pTexel, m_iStride );
}

template<typename T>
inline const T * GPUTexture1D::GetTexel( UInt iPoint, const GPUTextureIndex * pIndex ) const {
    Assert( m_iStride == sizeof(T) );
    return (const T *)GetTexel( iPoint, pIndex );
}
template<typename T>
inline T * GPUTexture1D::GetTexel( UInt iPoint, const GPUTextureIndex * pIndex ) {
    Assert( m_iStride == sizeof(T) );
    return (T *)GetTexel( iPoint, pIndex );
}
template<typename T>
inline Void GPUTexture1D::SetTexel( const T * pTexel, UInt iPoint, const GPUTextureIndex * pIndex ) {
    Assert( m_iStride == sizeof(T) );
    SetTexel( (const Byte *)pTexel, iPoint, pIndex );
}
template<typename T>
inline Void GPUTexture1D::SetTexels( const T * arrTexels, const GPUSegment * pSeg, const GPUTextureIndex * pIndex ) {
    Assert( m_iStride == sizeof(T) );
    SetTexels( (const Byte *)arrTexels, pSeg, pIndex );
}

/////////////////////////////////////////////////////////////////////////////////
// GPUTexture2D implementation
inline Bool GPUTexture2D::IsBoundToBackBuffer( UInt * outBackBuffer ) const {
    Assert( IsBound() );
    return m_hDeviceTexture2D.IsBoundToBackBuffer( outBackBuffer );
}

inline Bool GPUTexture2D::IsMultiSampled() const {
    return ( m_iSampleCount > 1 );
}
inline UInt GPUTexture2D::GetSampleCount() const {
    return m_iSampleCount;
}

inline UInt GPUTexture2D::GetWidth( UInt iMipLevel ) const {
    return GetMipLevelBound( 0, iMipLevel );
}
inline UInt GPUTexture2D::GetHeight( UInt iMipLevel ) const {
    return GetMipLevelBound( 1, iMipLevel );
}

inline UInt GPUTexture2D::GetPitch( UInt iMipLevel ) const {
    return ( m_iStride * GetWidth(iMipLevel) );
}

inline const Byte * GPUTexture2D::GetTexel( const GPUPoint2 * pPoint, const GPUTextureIndex * pIndex ) const {
    Assert( HasCPUData() );
    if ( pPoint == NULL )
        return _GetSubResource( pIndex );
    Assert( pPoint->iX < _GetSubResourceBound(0, pIndex) );
    Assert( pPoint->iY < _GetSubResourceBound(1, pIndex) );
    return ( _GetSubResource(pIndex) + (pPoint->iY * _GetSubResourcePitch(pIndex)) + (pPoint->iX * m_iStride) );
}
inline Byte * GPUTexture2D::GetTexel( const GPUPoint2 * pPoint, const GPUTextureIndex * pIndex ) {
    Assert( HasCPUData() );
    if ( pPoint == NULL )
        return _GetSubResource( pIndex );
    Assert( pPoint->iX < _GetSubResourceBound(0, pIndex) );
    Assert( pPoint->iY < _GetSubResourceBound(1, pIndex) );
    return ( _GetSubResource(pIndex) + (pPoint->iY * _GetSubResourcePitch(pIndex)) + (pPoint->iX * m_iStride) );
}
inline Void GPUTexture2D::SetTexel( const Byte * pTexel, const GPUPoint2 * pPoint, const GPUTextureIndex * pIndex ) {
    Assert( HasCPUData() );
    if ( pPoint == NULL ) {
        MemCopy( _GetSubResource(pIndex), pTexel, m_iStride );
        return;
    }
    Assert( pPoint->iX < _GetSubResourceBound(0, pIndex) );
    Assert( pPoint->iY < _GetSubResourceBound(1, pIndex) );
    MemCopy( _GetSubResource(pIndex) + (pPoint->iY * _GetSubResourcePitch(pIndex)) + (pPoint->iX * m_iStride), pTexel, m_iStride );
}

template<typename T>
inline const T * GPUTexture2D::GetTexel( const GPUPoint2 * pPoint, const GPUTextureIndex * pIndex ) const {
    Assert( m_iStride == sizeof(T) );
    return (const T *)GetTexel( pPoint, pIndex );
}
template<typename T>
inline T * GPUTexture2D::GetTexel( const GPUPoint2 * pPoint, const GPUTextureIndex * pIndex ) {
    Assert( m_iStride == sizeof(T) );
    return (T *)GetTexel( pPoint, pIndex );
}
template<typename T>
inline Void GPUTexture2D::SetTexel( const T * pTexel, const GPUPoint2 * pPoint, const GPUTextureIndex * pIndex ) {
    Assert( m_iStride == sizeof(T) );
    SetTexel( (const Byte *)pTexel, pPoint, pIndex );
}
template<typename T>
inline Void GPUTexture2D::SetTexels( const T * arrTexels, UInt iTexelsPitch, const GPURectangle * pRect, const GPUTextureIndex * pIndex ) {
    Assert( m_iStride == sizeof(T) );
    SetTexels( (const Byte *)arrTexels, iTexelsPitch, pRect, pIndex );
}

/////////////////////////////////////////////////////////////////////////////////
// GPUTexture3D implementation
inline UInt GPUTexture3D::GetWidth( UInt iMipLevel ) const {
    return GetMipLevelBound( 0, iMipLevel );
}
inline UInt GPUTexture3D::GetHeight( UInt iMipLevel ) const {
    return GetMipLevelBound( 1, iMipLevel );
}
inline UInt GPUTexture3D::GetDepth( UInt iMipLevel ) const {
    return GetMipLevelBound( 2, iMipLevel );
}

inline UInt GPUTexture3D::GetPitch( UInt iMipLevel ) const {
    return ( m_iStride * GetWidth(iMipLevel) );
}
inline UInt GPUTexture3D::GetSlice( UInt iMipLevel ) const {
    return ( GetPitch(iMipLevel) * GetHeight(iMipLevel) );
}

inline const Byte * GPUTexture3D::GetTexel( const GPUPoint3 * pPoint, const GPUTextureIndex * pIndex ) const {
    Assert( HasCPUData() );
    if ( pPoint == NULL )
        return _GetSubResource( pIndex );
    Assert( pPoint->iX < _GetSubResourceBound(0, pIndex) );
    Assert( pPoint->iY < _GetSubResourceBound(1, pIndex) );
    Assert( pPoint->iZ < _GetSubResourceBound(2, pIndex) );
    return ( _GetSubResource(pIndex) + (pPoint->iZ * _GetSubResourceSlice(pIndex)) + (pPoint->iY * _GetSubResourcePitch(pIndex)) + (pPoint->iX * m_iStride) );
}
inline Byte * GPUTexture3D::GetTexel( const GPUPoint3 * pPoint, const GPUTextureIndex * pIndex ) {
    Assert( HasCPUData() );
    if ( pPoint == NULL )
        return _GetSubResource( pIndex );
    Assert( pPoint->iX < _GetSubResourceBound(0, pIndex) );
    Assert( pPoint->iY < _GetSubResourceBound(1, pIndex) );
    Assert( pPoint->iZ < _GetSubResourceBound(2, pIndex) );
    return ( _GetSubResource(pIndex) + (pPoint->iZ * _GetSubResourceSlice(pIndex)) + (pPoint->iY * _GetSubResourcePitch(pIndex)) + (pPoint->iX * m_iStride) );
}
inline Void GPUTexture3D::SetTexel( const Byte * pTexel, const GPUPoint3 * pPoint, const GPUTextureIndex * pIndex ) {
    Assert( HasCPUData() );
    if ( pPoint == NULL ) {
        MemCopy( _GetSubResource(pIndex), pTexel, m_iStride );
        return;
    }
    Assert( pPoint->iX < _GetSubResourceBound(0, pIndex) );
    Assert( pPoint->iY < _GetSubResourceBound(1, pIndex) );
    Assert( pPoint->iZ < _GetSubResourceBound(2, pIndex) );
    MemCopy( _GetSubResource(pIndex) + (pPoint->iZ * _GetSubResourceSlice(pIndex)) + (pPoint->iY * _GetSubResourcePitch(pIndex)) + (pPoint->iX * m_iStride), pTexel, m_iStride );
}

template<typename T>
inline const T * GPUTexture3D::GetTexel( const GPUPoint3 * pPoint, const GPUTextureIndex * pIndex ) const {
    Assert( m_iStride == sizeof(T) );
    return (const T *)GetTexel( pPoint, pIndex );
}
template<typename T>
inline T * GPUTexture3D::GetTexel( const GPUPoint3 * pPoint, const GPUTextureIndex * pIndex ) {
    Assert( m_iStride == sizeof(T) );
    return (T *)GetTexel( pPoint, pIndex );
}
template<typename T>
inline Void GPUTexture3D::SetTexel( const T * pTexel, const GPUPoint3 * pPoint, const GPUTextureIndex * pIndex ) {
    Assert( m_iStride == sizeof(T) );
    SetTexel( (const Byte *)pTexel, pPoint, pIndex );
}
template<typename T>
inline Void GPUTexture3D::SetTexels( const T * arrTexels, UInt iTexelsPitch, UInt iTexelsSlice, const GPUBox * pBox, const GPUTextureIndex * pIndex ) {
    Assert( m_iStride == sizeof(T) );
    SetTexels( (const Byte *)arrTexels, iTexelsPitch, iTexelsSlice, pBox, pIndex );
}

/////////////////////////////////////////////////////////////////////////////////
// GPUTextureCube implementation
inline Bool GPUTextureCube::IsCubeArray() const {
    return ( m_iArrayCount > 6 );
}
inline UInt GPUTextureCube::GetCubeCount() const {
    return ( m_iArrayCount / 6 );
}

inline const Byte * GPUTextureCube::GetCubeFace( const GPUTextureIndex * pIndex, GPUTextureCubeFace iFace ) const {
    Assert( HasCPUData() );
    return _GetSubResource( pIndex, iFace );
}
inline Byte * GPUTextureCube::GetCubeFace( const GPUTextureIndex * pIndex, GPUTextureCubeFace iFace ) {
    Assert( HasCPUData() );
    return _GetSubResource( pIndex, iFace );
}
inline Void GPUTextureCube::SetCubeFace( const Byte * pCubeFace, const GPUTextureIndex * pIndex, GPUTextureCubeFace iFace ) {
    Assert( HasCPUData() );
    MemCopy( _GetSubResource(pIndex, iFace), pCubeFace, _GetSubResourceSize(pIndex) );
}

inline UInt GPUTextureCube::GetWidth( UInt iMipLevel ) const {
    return GetMipLevelBound( 0, iMipLevel );
}
inline UInt GPUTextureCube::GetHeight( UInt iMipLevel ) const {
    return GetMipLevelBound( 1, iMipLevel );
}

inline UInt GPUTextureCube::GetPitch( UInt iMipLevel ) const {
    return ( m_iStride * GetWidth(iMipLevel) );
}

inline const Byte * GPUTextureCube::GetTexel( const GPUPoint2 * pPoint, const GPUTextureIndex * pIndex, GPUTextureCubeFace iFace ) const {
    Assert( HasCPUData() );
    if ( pPoint == NULL )
        return _GetSubResource( pIndex, iFace );
    Assert( pPoint->iX < _GetSubResourceBound(0, pIndex) );
    Assert( pPoint->iY < _GetSubResourceBound(1, pIndex) );
    return ( _GetSubResource(pIndex, iFace) + (pPoint->iY * _GetSubResourcePitch(pIndex)) + (pPoint->iX * m_iStride) );
}
inline Byte * GPUTextureCube::GetTexel( const GPUPoint2 * pPoint, const GPUTextureIndex * pIndex, GPUTextureCubeFace iFace ) {
    Assert( HasCPUData() );
    if ( pPoint == NULL )
        return _GetSubResource( pIndex, iFace );
    Assert( pPoint->iX < _GetSubResourceBound(0, pIndex) );
    Assert( pPoint->iY < _GetSubResourceBound(1, pIndex) );
    return ( _GetSubResource(pIndex, iFace) + (pPoint->iY * _GetSubResourcePitch(pIndex)) + (pPoint->iX * m_iStride) );
}
inline Void GPUTextureCube::SetTexel( const Byte * pTexel, const GPUPoint2 * pPoint, const GPUTextureIndex * pIndex, GPUTextureCubeFace iFace ) {
    Assert( HasCPUData() );
    if ( pPoint == NULL ) {
        MemCopy( _GetSubResource(pIndex, iFace), pTexel, m_iStride );
        return;
    }
    Assert( pPoint->iX < _GetSubResourceBound(0, pIndex) );
    Assert( pPoint->iY < _GetSubResourceBound(1, pIndex) );
    MemCopy( _GetSubResource(pIndex, iFace) + (pPoint->iY * _GetSubResourcePitch(pIndex)) + (pPoint->iX * m_iStride), pTexel, m_iStride );
}

template<typename T>
inline const T * GPUTextureCube::GetTexel( const GPUPoint2 * pPoint, const GPUTextureIndex * pIndex, GPUTextureCubeFace iFace ) const {
    Assert( m_iStride == sizeof(T) );
    return (const T *)GetTexel( pPoint, pIndex, iFace );
}
template<typename T>
inline T * GPUTextureCube::GetTexel( const GPUPoint2 * pPoint, const GPUTextureIndex * pIndex, GPUTextureCubeFace iFace ) {
    Assert( m_iStride == sizeof(T) );
    return (T *)GetTexel( pPoint, pIndex, iFace );
}
template<typename T>
inline Void GPUTextureCube::SetTexel( const T * pTexel, const GPUPoint2 * pPoint, const GPUTextureIndex * pIndex, GPUTextureCubeFace iFace ) {
    Assert( m_iStride == sizeof(T) );
    SetTexel( (const Byte *)pTexel, pPoint, pIndex, iFace );
}
template<typename T>
inline Void GPUTextureCube::SetTexels( const T * arrTexels, UInt iTexelsPitch, const GPURectangle * pRect, const GPUTextureIndex * pIndex, GPUTextureCubeFace iFace ) {
    Assert( m_iStride == sizeof(T) );
    SetTexels( (const Byte *)arrTexels, iTexelsPitch, pRect, pIndex, iFace );
}

