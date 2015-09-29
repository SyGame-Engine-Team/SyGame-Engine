/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Resources/GPUTexture.cpp
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
// Includes
#include "GPUTexture.h"

#include "../RenderingManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GPUTexture implementation
UInt GPUTexture::sm_arrDimensions[4] = {
    1, 2, 3, 2
};

GPUTexture::GPUTexture( PixelFormat iFormat, UInt iDimensions, UInt arrBounds[GPUTEXTURE_MAX_DIMENSIONS], UInt iMipLevelCount, UInt iArrayCount, UInt iSampleCount, const Byte * arrTexels, Bool bCreateCPUData ):
    GPUResource()
{
    Assert( iFormat != PIXEL_FMT_UNKNOWN && iFormat < PIXEL_FMT_COUNT );
    Assert( 0 < iDimensions && iDimensions <= GPUTEXTURE_MAX_DIMENSIONS );
    Assert( iMipLevelCount < GPUTEXTURE_MAX_MIPMAP_LEVELS );
    Assert( iArrayCount > 0 );
    Assert( iSampleCount == 1 || iSampleCount == 2 || iSampleCount == 4 || iSampleCount == 8 || iSampleCount == 16 );

    m_iFormat = iFormat;
    m_iStride = ( iSampleCount * PixelFormatBytes(m_iFormat) );

    m_iDimensions = iDimensions;

    m_iMipLevelCount = iMipLevelCount;
    for( UInt i = 0; i < m_iDimensions; ++i ) {
        Assert( arrBounds[i] > 0 );
        if ( m_iMipLevelCount != 1 ) {
            Assert( (arrBounds[i] & (arrBounds[i]-1)) == 0 );
        }
        m_arrMipLevelBounds[0][i] = arrBounds[i];
    }

    m_iArrayCount = iArrayCount;

    _ComputeTextureLayout();

    m_arrTexture = NULL;
    if ( bCreateCPUData ) {
        m_arrTexture = New Byte[m_iSize];

        if ( arrTexels != NULL )
            MemCopy( m_arrTexture, arrTexels, m_iSize );
    }

    m_pDeviceTexture = NULL;
}
GPUTexture::~GPUTexture()
{
    if ( m_arrTexture != NULL )
        DeleteA( m_arrTexture );
}

Void GPUTexture::UnBind()
{
    Assert( IsBound() );

    RenderingFn->_UnRegisterTexture( this );

    m_pDeviceTexture->Destroy();
}

Void GPUTexture::DestroyCPUData()
{
    Assert( HasCPUData() );

    m_pDeviceTexture->UpdateDataSource( NULL );

    RenderingFn->SelectMemory( TEXT("Textures") );
    DeleteA( m_arrTexture );
    m_arrTexture = NULL;
    RenderingFn->UnSelectMemory();
}
Void GPUTexture::RestoreCPUData()
{
    Assert( !(HasCPUData()) );

    RenderingFn->SelectMemory( TEXT("Textures") );
    m_arrTexture = New Byte[m_iSize];
    RenderingFn->UnSelectMemory();

    m_pDeviceTexture->UpdateDataSource( m_arrTexture );
}

Float GPUTexture::GetMinLOD( GPUDeferredContext * pContext ) const
{
    Assert( IsBound() );

    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    return m_pDeviceTexture->GetMinLOD( pDeviceContext );
}
Void GPUTexture::SetMinLOD( Float fMinLOD, GPUDeferredContext * pContext ) const
{
    Assert( IsBound() );

    Float fMaxLOD = (Float)( m_iMipLevelCount - 1 );
    if ( fMinLOD < 0.0f )
        fMinLOD = 0.0f;
    if ( fMinLOD > fMaxLOD )
        fMinLOD = fMaxLOD;

    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceTexture->SetMinLOD( fMinLOD, pDeviceContext );
}

/////////////////////////////////////////////////////////////////////////////////

Void GPUTexture::_ComputeTextureLayout()
{
    // No MipMap case
    if ( m_iMipLevelCount == GPUTEXTURE_MIPMAP_NONE ) {
        // Just compute offset & size of mip level 0
        m_arrMipLevelOffsets[0] = 0;
        m_arrMipLevelSizes[0] = m_arrMipLevelBounds[0][0];
        for( UInt i = 1; i < m_iDimensions; ++i )
            m_arrMipLevelSizes[0] *= m_arrMipLevelBounds[0][i];
        m_arrMipLevelSizes[0] *= m_iStride;

        // MipMap size
        m_iMipMapSize = m_arrMipLevelSizes[0];

        // Texture total size
        m_iSubResourceCount = m_iArrayCount;
        m_iSize = ( m_iArrayCount * m_iMipMapSize );

        return;
    }

    Assert( PixelFormatIsMipMapable(m_iFormat) );

    // Compute Mip Levels for automatic mode
    if ( m_iMipLevelCount == GPUTEXTURE_MIPMAP_AUTO ) {
        UInt iMaxBound = m_arrMipLevelBounds[0][0];
        for( UInt i = 1; i < m_iDimensions; ++i ) {
            if ( iMaxBound < m_arrMipLevelBounds[0][i] )
                iMaxBound = m_arrMipLevelBounds[0][i];
        }
        m_iMipLevelCount = ( MathFn->Log2OfPower2(iMaxBound) + 1 );
    }

    // Compute bounds for all levels
    for( UInt i = 0; i < m_iDimensions; ++i ) {
        for( UInt iMip = 1; iMip < m_iMipLevelCount; ++iMip ) {
            if ( m_arrMipLevelBounds[iMip-1][i] > 1 )
                m_arrMipLevelBounds[iMip][i] = ( m_arrMipLevelBounds[iMip-1][i] >> 1 );
            else
                m_arrMipLevelBounds[iMip][i] = 1;
        }
    }

    // Compute Offsets and Sizes for all levels
    m_arrMipLevelOffsets[0] = 0;
    for( UInt iMip = 0; iMip < m_iMipLevelCount; ++iMip ) {
        m_arrMipLevelSizes[iMip] = m_arrMipLevelBounds[iMip][0];
        for( UInt i = 1; i < m_iDimensions; ++i )
            m_arrMipLevelSizes[iMip] *= m_arrMipLevelBounds[iMip][i];
        m_arrMipLevelSizes[iMip] *= m_iStride;
        if ( iMip > 0 )
            m_arrMipLevelOffsets[iMip] = ( m_arrMipLevelOffsets[iMip-1] + m_arrMipLevelSizes[iMip-1] );
    }

    // MipMap size
    m_iMipMapSize = m_arrMipLevelSizes[0];
    for( UInt i = 1; i < m_iMipLevelCount; ++i )
        m_iMipMapSize += m_arrMipLevelSizes[i];

    // Texture total size
    m_iSubResourceCount = ( m_iMipLevelCount * m_iArrayCount );
    m_iSize = ( m_iArrayCount * m_iMipMapSize );
}

/////////////////////////////////////////////////////////////////////////////////
// GPUTexture1D implementation
GPUTexture1D::GPUTexture1D( PixelFormat iFormat, UInt iWidth, UInt iMipLevelCount, UInt iArrayCount, const Byte * arrTexels, Bool bCreateCPUData ):
    GPUTexture( iFormat, 1, _MakeBoundArray(iWidth), iMipLevelCount, iArrayCount, 1, arrTexels, bCreateCPUData ), m_hDeviceTexture1D( RenderingFn->m_pDeviceRenderer )
{
    m_pDeviceTexture = &m_hDeviceTexture1D;
    m_pDeviceResource = m_pDeviceTexture;
}
GPUTexture1D::~GPUTexture1D()
{
    if ( IsBound() )
        UnBind();
}

Void GPUTexture1D::Bind( UInt iGPUResourceBinds )
{
    Assert( !(IsBound()) );

    m_hDeviceTexture1D.Create( iGPUResourceBinds, m_iFormat, m_arrMipLevelBounds[0][0], m_iMipLevelCount, m_iArrayCount, m_arrTexture );

    RenderingFn->_RegisterTexture( this );
}

Void GPUTexture1D::SetTexels( const Byte * arrTexels, const GPUSegment * pSeg, const GPUTextureIndex * pIndex )
{
    Assert( HasCPUData() );

    if ( pSeg == NULL ) {
        MemCopy( _GetSubResource(pIndex), arrTexels, _GetSubResourceSize(pIndex) );
        return;
    }

    UInt iWidth = _GetSubResourceBound( 0, pIndex );
    _CopyTexels( _GetSubResource(pIndex), iWidth, arrTexels, (pSeg->iRight - pSeg->iLeft), m_iStride, pSeg->iLeft, NULL );
}

Void GPUTexture1D::GenerateMipMap()
{
    Assert( HasCPUData() );
    Assert( PixelFormatIsMipMapable(m_iFormat) );

    if ( m_iMipLevelCount <= 1 )
        return;

    Byte * arrLERP[2];
    
    GPUTextureIndex hIndex;
    for( hIndex.iArrayIndex = 0; hIndex.iArrayIndex < m_iArrayCount; ++(hIndex.iArrayIndex) ) {
        hIndex.iMipIndex = 0;
        Byte * pPrevSubResource = _GetSubResource( &hIndex );

        for( hIndex.iMipIndex = 1; hIndex.iMipIndex < m_iMipLevelCount; ++(hIndex.iMipIndex) ) {
            Byte * pSubResource = _GetSubResource( &hIndex );
            UInt iWidth = _GetSubResourceBound( 0, &hIndex );
            Byte *pSrc, *pDst;

            for( UInt iX = 0; iX < iWidth; ++iX ) {
                pSrc = pPrevSubResource + ((iX << 1) * m_iStride);
                pDst = pSubResource + (iX * m_iStride);

                arrLERP[0] = pSrc;
                arrLERP[1] = pSrc + m_iStride;
                PixelFormatLERP( pDst, (const Byte **)arrLERP, 2, m_iFormat );
            }
            
            pPrevSubResource = pSubResource;
        }
    }
}

Void * GPUTexture1D::Lock( const GPUTextureIndex * pIndex, GPUResourceLock iLockType, UInt iGPUResourceLockFlags, GPUDeferredContext * pContext )
{
    Assert( IsBound() );
    Assert( CanLock() );
    Assert( !(IsLocked()) );

    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    return m_hDeviceTexture1D.Lock( (const DeviceSubResourceIndex *)pIndex, (DeviceResourceLock)iLockType, iGPUResourceLockFlags, pDeviceContext );
}
Void GPUTexture1D::UnLock( const GPUTextureIndex * pIndex, GPUDeferredContext * pContext )
{
    Assert( IsBound() );
    Assert( CanLock() );
    Assert( IsLocked() );

    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_hDeviceTexture1D.UnLock( (const DeviceSubResourceIndex *)pIndex, pDeviceContext );
}

Void GPUTexture1D::Update( const GPUTextureIndex * pIndex, const GPUSegment * pSeg, GPUDeferredContext * pContext )
{
    Assert( HasCPUData() && IsBound() );
    Assert( CanUpdate() );
    Assert( !(IsLocked()) );

    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    UInt iWidth = _GetSubResourceBound( 0, pIndex );
    GPUSegment hSeg;
    _ValidateCopy( &hSeg, iWidth, iWidth, (pSeg != NULL) ? pSeg->iLeft : 0, pSeg );
    UInt iCopyWidth = ( hSeg.iRight - hSeg.iLeft );

    m_hDeviceTexture1D.Update( (const DeviceSubResourceIndex *)pIndex, hSeg.iLeft, iCopyWidth, m_arrTexture, pDeviceContext );
}
Void GPUTexture1D::UpdateBack( const GPUTextureIndex * pIndex, const GPUSegment * pSeg, GPUDeferredContext * pContext )
{
    Assert( HasCPUData() && IsBound() );
    Assert( CanLock() && CanCPURead() );
    Assert( !(IsLocked()) );

    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    Byte * pData = NULL;

    pData = (Byte*)( m_hDeviceTexture1D.Lock( (const DeviceSubResourceIndex *)pIndex, DEVICERESOURCE_LOCK_READ, DEVICERESOURCE_LOCKFLAG_NONE, pDeviceContext ) );

    UInt iWidth = _GetSubResourceBound( 0, pIndex );
    _CopyTexels( _GetSubResource(pIndex), iWidth, pData, iWidth, m_iStride, (pSeg != NULL) ? pSeg->iLeft : 0, pSeg );

    m_hDeviceTexture1D.UnLock( (const DeviceSubResourceIndex *)pIndex, pDeviceContext );
}

Void GPUTexture1D::CopyCPUData( GPUTexture1D * pDstTexture, Bool bUpdateGPUData, GPUDeferredContext * pContext ) const
{
    Assert( HasCPUData() && pDstTexture->HasCPUData() );
    Assert( m_iFormat == pDstTexture->m_iFormat );
    Assert( m_arrMipLevelBounds[0][0] == pDstTexture->m_arrMipLevelBounds[0][0] );
    Assert( m_iMipLevelCount == pDstTexture->m_iMipLevelCount );
    Assert( m_iArrayCount == pDstTexture->m_iArrayCount );
    Assert( m_iSize == pDstTexture->m_iSize );

    MemCopy( pDstTexture->m_arrTexture, m_arrTexture, m_iSize );

    if ( bUpdateGPUData ) {
        GPUTextureIndex hIndex;
        for( hIndex.iArrayIndex = 0; hIndex.iArrayIndex < pDstTexture->m_iArrayCount; ++(hIndex.iArrayIndex) ) {
            for( hIndex.iMipIndex = 0; hIndex.iMipIndex < pDstTexture->m_iMipLevelCount; ++(hIndex.iMipIndex) )
                pDstTexture->Update( &hIndex, NULL, pContext );
        }
    }
}
Void GPUTexture1D::CopyCPUData( GPUTexture1D * pDstTexture, const GPUTextureIndex * pDstIndex, UInt iDstPoint, const GPUTextureIndex * pSrcIndex, const GPUSegment * pSrcSeg, Bool bUpdateGPUData, GPUDeferredContext * pContext ) const
{
    Assert( HasCPUData() && pDstTexture->HasCPUData() );
    Assert( m_iFormat == pDstTexture->m_iFormat );

    UInt iSrcWidth = _GetSubResourceBound( 0, pSrcIndex );
    UInt iDstWidth = pDstTexture->_GetSubResourceBound( 0, pDstIndex );
    _CopyTexels( pDstTexture->_GetSubResource(pDstIndex), iDstWidth, _GetSubResource(pSrcIndex), iSrcWidth, m_iStride, iDstPoint, pSrcSeg );

    if ( bUpdateGPUData ) {
        GPUSegment hDstSeg;
        hDstSeg.iLeft = iDstPoint;
        hDstSeg.iRight = hDstSeg.iLeft + ( (pSrcSeg != NULL) ? (pSrcSeg->iRight - pSrcSeg->iLeft) : iSrcWidth );
        pDstTexture->Update( pDstIndex, &hDstSeg, pContext );
    }
}
Void GPUTexture1D::CopyGPUData( GPUTexture1D * pDstTexture, Bool bUpdateCPUData, GPUDeferredContext * pContext ) const
{
    Assert( IsBound() && pDstTexture->IsBound() );
    Assert( !(IsLocked()) && !(pDstTexture->IsLocked()) );
    Assert( pDstTexture->CanGPUWrite() );

    Assert( pDstTexture != this );

    Assert( m_iFormat == pDstTexture->m_iFormat );
    Assert( m_arrMipLevelBounds[0][0] == pDstTexture->m_arrMipLevelBounds[0][0] );
    Assert( m_iMipLevelCount == pDstTexture->m_iMipLevelCount );
    Assert( m_iArrayCount == pDstTexture->m_iArrayCount );
    Assert( m_iSize == pDstTexture->m_iSize );

    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_hDeviceTexture1D.Copy( &(pDstTexture->m_hDeviceTexture1D), pDeviceContext );

    if ( bUpdateCPUData ) {
        GPUTextureIndex hIndex;
        for( hIndex.iArrayIndex = 0; hIndex.iArrayIndex < pDstTexture->m_iArrayCount; ++(hIndex.iArrayIndex) ) {
            for( hIndex.iMipIndex = 0; hIndex.iMipIndex < pDstTexture->m_iMipLevelCount; ++(hIndex.iMipIndex) )
                pDstTexture->UpdateBack( &hIndex, NULL, pContext );
        }
    }
}
Void GPUTexture1D::CopyGPUData( GPUTexture1D * pDstTexture, const GPUTextureIndex * pDstIndex, UInt iDstPoint, const GPUTextureIndex * pSrcIndex, const GPUSegment * pSrcSeg, Bool bUpdateCPUData, GPUDeferredContext * pContext ) const
{
    Assert( IsBound() && pDstTexture->IsBound() );
    Assert( !(IsLocked()) && !(pDstTexture->IsLocked()) );
    Assert( pDstTexture->CanGPUWrite() );

    Assert( m_iFormat == pDstTexture->m_iFormat );

    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    UInt iSrcWidth = _GetSubResourceBound( 0, pSrcIndex );
    UInt iDstWidth = pDstTexture->_GetSubResourceBound( 0, pDstIndex );
    GPUSegment hSeg;
    _ValidateCopy( &hSeg, iDstWidth, iSrcWidth, iDstPoint, pSrcSeg );
    UInt iCopyWidth = ( hSeg.iRight - hSeg.iLeft );

    m_hDeviceTexture1D.Copy( &(pDstTexture->m_hDeviceTexture1D), (const DeviceSubResourceIndex *)pDstIndex, iDstPoint, (const DeviceSubResourceIndex *)pSrcIndex, hSeg.iLeft, iCopyWidth, pDeviceContext );

    if ( bUpdateCPUData ) {
        GPUSegment hDstSeg;
        hDstSeg.iLeft = iDstPoint;
        hDstSeg.iRight = hDstSeg.iLeft + iCopyWidth;
        pDstTexture->UpdateBack( pDstIndex, &hDstSeg, pContext );
    }
}

/////////////////////////////////////////////////////////////////////////////////

Void GPUTexture1D::_ValidateCopy( GPUSegment * outSrcSeg, UInt iDstWidth, UInt iSrcWidth, UInt iDstPoint, const GPUSegment * pSrcSeg )
{
    outSrcSeg->iLeft = 0;
    outSrcSeg->iRight = iSrcWidth;
    if ( pSrcSeg != NULL ) {
        Assert( pSrcSeg->iLeft < pSrcSeg->iRight && pSrcSeg->iRight <= iSrcWidth );
        outSrcSeg->iLeft = pSrcSeg->iLeft;
        outSrcSeg->iRight = pSrcSeg->iRight;
    }

    Assert( iDstPoint < iDstWidth );
    Assert( iDstPoint + (outSrcSeg->iRight - outSrcSeg->iLeft) <= iDstWidth );
}
Void GPUTexture1D::_CopyTexels( Byte * pDst, UInt iDstWidth, const Byte * pSrc, UInt iSrcWidth, UInt iStride, UInt iDstPoint, const GPUSegment * pSrcSeg )
{
    GPUSegment hSrcSeg;
    _ValidateCopy( &hSrcSeg, iDstWidth, iSrcWidth, iDstPoint, pSrcSeg );

    MemCopy( pDst + (iDstPoint * iStride), pSrc + (hSrcSeg.iLeft * iStride), (hSrcSeg.iRight - hSrcSeg.iLeft) * iStride );
}

Bool GPUTexture1D::_OnLoad( ResourceDB * pRDB, RDB_ResourceHandle * hResource )
{
    Assert( !IsBound() );

    TextureHeader hTextureHeader;
    pRDB->ResourceRead( hResource, sizeof(TextureHeader), (Byte*)( &hTextureHeader ) );

    Assert( hTextureHeader.dwType == GPURESOURCE_TEXTURE_1D );
    Assert( hTextureHeader.dwSampleCount == 1 );

    m_iFormat = (PixelFormat)( hTextureHeader.dwFormat );
    m_iStride = ( hTextureHeader.dwSampleCount * PixelFormatBytes(m_iFormat) );

    Assert( m_iDimensions == 1 );

    m_iMipLevelCount = hTextureHeader.dwMipLevelCount;

    Assert( hTextureHeader.dwBounds[0] > 0 );
    if ( m_iMipLevelCount > 1 ) {
        Assert( ( hTextureHeader.dwBounds[0] & (hTextureHeader.dwBounds[0] - 1) ) == 0 );
    }
    m_arrMipLevelBounds[0][0] = hTextureHeader.dwBounds[0];
    m_arrMipLevelBounds[0][1] = 0;
    m_arrMipLevelBounds[0][2] = 0;

    m_iArrayCount = 1;

    _ComputeTextureLayout();
    Assert( m_iSize == hTextureHeader.dwDataSize );

    RenderingFn->SelectMemory( TEXT("Textures") );

    if ( m_arrTexture != NULL ) {
        DeleteA( m_arrTexture );
        m_arrTexture = NULL;
    }
    m_arrTexture = New Byte[m_iSize];

    RenderingFn->UnSelectMemory();

    pRDB->ResourceRead( hResource, m_iSize, m_arrTexture );

    return true;
}
Bool GPUTexture1D::_OnSave( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) const
{
    Assert( HasCPUData() );
    Assert( m_iArrayCount == 1 );

    TextureHeader hTextureHeader;
    hTextureHeader.dwDataSize = m_iSize;
    hTextureHeader.dwType = GPURESOURCE_TEXTURE_1D;
    hTextureHeader.dwFormat = m_iFormat;
    hTextureHeader.dwSampleCount = 1;
    hTextureHeader.dwMipLevelCount = m_iMipLevelCount;
    hTextureHeader.dwBounds[0] = m_arrMipLevelBounds[0][0];
    hTextureHeader.dwBounds[1] = 0;
    hTextureHeader.dwBounds[2] = 0;

    pRDB->ResourceAppend( hResource, sizeof(TextureHeader), (const Byte *)( &hTextureHeader ) );

    pRDB->ResourceAppend( hResource, m_iSize, m_arrTexture );

    return true;
}

/////////////////////////////////////////////////////////////////////////////////
// GPUTexture2D implementation
GPUTexture2D::GPUTexture2D( PixelFormat iFormat, UInt iWidth, UInt iHeight, UInt iMipLevelCount, UInt iArrayCount, UInt iSampleCount, const Byte * arrTexels, Bool bCreateCPUData ):
    GPUTexture( iFormat, 2, _MakeBoundArray(iWidth, iHeight), iMipLevelCount, iArrayCount, iSampleCount, arrTexels, bCreateCPUData ), m_hDeviceTexture2D( RenderingFn->m_pDeviceRenderer, _OnBackBufferResize, this )
{
    m_pDeviceTexture = &m_hDeviceTexture2D;
    m_pDeviceResource = m_pDeviceTexture;

    m_iSampleCount = iSampleCount;

    m_hDeviceTexture2D.SetSampleCount( m_iSampleCount );
}
GPUTexture2D::~GPUTexture2D()
{
    if ( IsBound() )
        UnBind();
}

Void GPUTexture2D::BindToBackBuffer( UInt iBackBuffer )
{
    Assert( !(IsBound()) );

    const GPUSwapChainDesc * pSwapChainDesc = RenderingFn->GetSwapChainDesc();

    Assert( iBackBuffer < pSwapChainDesc->iBufferCount );
    Assert( m_iFormat == pSwapChainDesc->iFormat );
    Assert( m_arrMipLevelBounds[0][0] == pSwapChainDesc->iWidth );
    Assert( m_arrMipLevelBounds[0][1] == pSwapChainDesc->iHeight );
    Assert( m_iMipLevelCount == 1 );
    Assert( m_iArrayCount == 1 );
    Assert( m_iSampleCount == pSwapChainDesc->iSampleCount );

    m_hDeviceTexture2D.AttachToBackBuffer( iBackBuffer );

    RenderingFn->_RegisterTexture( this );
}
Void GPUTexture2D::Bind( UInt iGPUResourceBinds )
{
    Assert( !(IsBound()) );

    m_hDeviceTexture2D.Create( iGPUResourceBinds, m_iFormat, m_arrMipLevelBounds[0][0], m_arrMipLevelBounds[0][1], m_iMipLevelCount, m_iArrayCount, m_arrTexture );

    RenderingFn->_RegisterTexture( this );
}

Void GPUTexture2D::SetTexels( const Byte * arrTexels, UInt iTexelsPitch, const GPURectangle * pRect, const GPUTextureIndex * pIndex )
{
    Assert( HasCPUData() );

    if ( pRect == NULL ) {
        MemCopy( _GetSubResource(pIndex), arrTexels, _GetSubResourceSize(pIndex) );
        return;
    }

    GPUPoint2 hDstPoint;
    hDstPoint.iX = pRect->iLeft;
    hDstPoint.iY = pRect->iTop;

    UInt iWidth = _GetSubResourceBound( 0, pIndex );
    UInt iHeight = _GetSubResourceBound( 1, pIndex );
    _CopyTexels( _GetSubResource(pIndex), iWidth, iHeight, _GetSubResourcePitch(pIndex), arrTexels, (pRect->iRight - pRect->iLeft), (pRect->iBottom - pRect->iTop), iTexelsPitch,
                 m_iStride, &hDstPoint, NULL );
}

Void GPUTexture2D::GenerateMipMap()
{
    Assert( HasCPUData() );
    Assert( PixelFormatIsMipMapable(m_iFormat) );

    if ( m_iMipLevelCount <= 1 )
        return;

    static UInt s_arrMultisampling_X1_PitchFactor[4]  = { 0, 0, 1, 1 };
    static UInt s_arrMultisampling_X1_StrideFactor[4] = { 0, 1, 0, 1 };
    static UInt s_arrMultisampling_X1_SampleFactor[4] = { 0, 0, 0, 0 };

    static UInt s_arrMultisampling_X2_PitchFactor[8]  = { 0, 1, 1, 1,   0, 0, 0, 1 };
    static UInt s_arrMultisampling_X2_StrideFactor[8] = { 1, 0, 1, 1,   0, 0, 1, 0 };
    static UInt s_arrMultisampling_X2_SampleFactor[8] = { 0, 0, 0, 1,   0, 1, 1, 1 };

    static UInt s_arrMultisampling_X4_PitchFactor[16]  = { 0, 0, 0, 0,   0, 0, 1, 1,   0, 0, 1, 1,   1, 1, 1, 1 };
    static UInt s_arrMultisampling_X4_StrideFactor[16] = { 0, 0, 1, 1,   1, 1, 1, 1,   0, 0, 0, 0,   0, 0, 1, 1 };
    static UInt s_arrMultisampling_X4_SampleFactor[16] = { 0, 1, 0, 2,   1, 3, 0, 1,   2, 3, 0, 2,   1, 3, 2, 3 };

    static UInt s_arrMultisampling_X8_PitchFactor[32]  = { 0, 0, 0, 0,   1, 1, 1, 1,   0, 1, 1, 1,   0, 0, 0, 0,   1, 1, 1, 1,   0, 0, 0, 1,   1, 1, 1, 1,   0, 0, 0, 0 };
    static UInt s_arrMultisampling_X8_StrideFactor[32] = { 0, 1, 1, 1,   0, 0, 0, 1,   1, 1, 1, 1,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   1, 1, 1, 1,   1, 1, 1, 1 };
    static UInt s_arrMultisampling_X8_SampleFactor[32] = { 2, 1, 4, 5,   0, 2, 7, 5,   6, 0, 3, 7,   0, 3, 5, 7,   1, 4, 5, 6,   1, 4, 6, 3,   1, 2, 4, 6,   0, 2, 3, 7 };

    static UInt s_arrMultisampling_X16_PitchFactor[64]  = {  1, 1, 1, 1,   0, 0, 0, 0,   1, 1, 1, 1,   0, 0, 0, 1,   0, 0, 0, 0,   1, 1, 1, 1,   1, 1, 1, 1,   0, 0, 0, 0,
                                                             1, 1, 1, 1,   0, 0, 0, 0,   0, 0, 0, 0,   1, 1, 1, 1,   0, 1, 1, 1,   0, 0, 0, 0,   1, 1, 1, 1,   0, 0, 0, 0 };
    static UInt s_arrMultisampling_X16_StrideFactor[64] = {  0, 1, 1, 1,   0, 0, 1, 1,   0, 0, 0, 0,   1, 1, 1, 1,   0, 0, 0, 0,   1, 1, 1, 1,   1, 1, 1, 1,   1, 1, 1, 1,
                                                             0, 0, 0, 0,   0, 1, 1, 1,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   1, 1, 1, 1,   1, 1, 1, 1,   0, 0, 0, 0 };
    static UInt s_arrMultisampling_X16_SampleFactor[64] = { 13, 4, 10, 15,    6, 14, 11, 12,   1, 3, 7, 9,   5, 8, 14,  9,    0, 2,  5,  8,   2, 8, 11, 12,   1, 3, 7, 13,   0,  1,  2,  4,
                                                             0, 5,  6, 14,   13,  9, 10, 15,   1, 3, 7, 9,   2, 8, 11, 12,   11, 4, 10, 15,   3, 6,  7, 13,   0, 5, 6, 14,   4, 10, 12, 15 };

    UInt iSampleStride = ( m_iStride / m_iSampleCount );
    UInt * arrPitchFactor = NULL;
    UInt * arrStrideFactor = NULL;
    UInt * arrSampleFactor = NULL;
    switch( m_iSampleCount ) {
        case 1:  arrPitchFactor = (UInt*)s_arrMultisampling_X1_PitchFactor;  arrStrideFactor = (UInt*)s_arrMultisampling_X1_StrideFactor;  arrSampleFactor = (UInt*)s_arrMultisampling_X1_SampleFactor;  break;
        case 2:  arrPitchFactor = (UInt*)s_arrMultisampling_X2_PitchFactor;  arrStrideFactor = (UInt*)s_arrMultisampling_X2_StrideFactor;  arrSampleFactor = (UInt*)s_arrMultisampling_X2_SampleFactor;  break;
        case 4:  arrPitchFactor = (UInt*)s_arrMultisampling_X4_PitchFactor;  arrStrideFactor = (UInt*)s_arrMultisampling_X4_StrideFactor;  arrSampleFactor = (UInt*)s_arrMultisampling_X4_SampleFactor;  break;
        case 8:  arrPitchFactor = (UInt*)s_arrMultisampling_X8_PitchFactor;  arrStrideFactor = (UInt*)s_arrMultisampling_X8_StrideFactor;  arrSampleFactor = (UInt*)s_arrMultisampling_X8_SampleFactor;  break;
        case 16: arrPitchFactor = (UInt*)s_arrMultisampling_X16_PitchFactor; arrStrideFactor = (UInt*)s_arrMultisampling_X16_StrideFactor; arrSampleFactor = (UInt*)s_arrMultisampling_X16_SampleFactor; break;
        default: Assert( false ); break;
    }

    Byte * arrLERP[4];

    GPUTextureIndex hIndex;
    for( hIndex.iArrayIndex = 0; hIndex.iArrayIndex < m_iArrayCount; ++(hIndex.iArrayIndex) ) {
        hIndex.iMipIndex = 0;
        Byte * pPrevSubResource = _GetSubResource( &hIndex );
        UInt iPrevPitch = _GetSubResourcePitch( &hIndex );

        for( hIndex.iMipIndex = 1; hIndex.iMipIndex < m_iMipLevelCount; ++(hIndex.iMipIndex) ) {
            Byte * pSubResource = _GetSubResource( &hIndex );
            UInt iWidth = _GetSubResourceBound( 0, &hIndex );
            UInt iHeight = _GetSubResourceBound( 1, &hIndex );
            UInt iPitch = ( iWidth * m_iStride );
            Byte *pSrc, *pDst;

            for( UInt iY = 0; iY < iHeight; ++iY ) {
                for( UInt iX = 0; iX < iWidth; ++iX ) {
                    pSrc = pPrevSubResource + ((iY << 1) * iPrevPitch) + ((iX << 1) * m_iStride);
                    pDst = pSubResource + (iY * iPitch) + (iX * m_iStride);

                    for( UInt k = 0; k < m_iSampleCount; ++k ) {
                        UInt iOffset = ( k << 2 );
                        
                        arrLERP[0] = pSrc + (arrPitchFactor[iOffset]     * iPrevPitch) + (arrStrideFactor[iOffset]     * m_iStride) + (arrSampleFactor[iOffset]     * iSampleStride);
                        arrLERP[1] = pSrc + (arrPitchFactor[iOffset + 1] * iPrevPitch) + (arrStrideFactor[iOffset + 1] * m_iStride) + (arrSampleFactor[iOffset + 1] * iSampleStride);
                        arrLERP[2] = pSrc + (arrPitchFactor[iOffset + 2] * iPrevPitch) + (arrStrideFactor[iOffset + 2] * m_iStride) + (arrSampleFactor[iOffset + 2] * iSampleStride);
                        arrLERP[3] = pSrc + (arrPitchFactor[iOffset + 3] * iPrevPitch) + (arrStrideFactor[iOffset + 3] * m_iStride) + (arrSampleFactor[iOffset + 3] * iSampleStride);
                        PixelFormatLERP( pDst + (k * iSampleStride), (const Byte **)arrLERP, 4, m_iFormat );
                    }
                }
            }
            
            pPrevSubResource = pSubResource;
            iPrevPitch = iPitch;
        }
    }
}

Void * GPUTexture2D::Lock( const GPUTextureIndex * pIndex, GPUResourceLock iLockType, UInt iGPUResourceLockFlags, UInt * outPitch, GPUDeferredContext * pContext )
{
    Assert( IsBound() );
    Assert( CanLock() );
    Assert( !(IsLocked()) );

    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    return m_hDeviceTexture2D.Lock( (const DeviceSubResourceIndex *)pIndex, (DeviceResourceLock)iLockType, iGPUResourceLockFlags, outPitch, pDeviceContext );
}
Void GPUTexture2D::UnLock( const GPUTextureIndex * pIndex, GPUDeferredContext * pContext )
{
    Assert( IsBound() );
    Assert( CanLock() );
    Assert( IsLocked() );

    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_hDeviceTexture2D.UnLock( (const DeviceSubResourceIndex *)pIndex, pDeviceContext );
}

Void GPUTexture2D::Update( const GPUTextureIndex * pIndex, const GPURectangle * pRect, GPUDeferredContext * pContext )
{
    Assert( HasCPUData() && IsBound() );
    Assert( CanUpdate() );
    Assert( !(IsLocked()) );

    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    UInt iWidth = _GetSubResourceBound( 0, pIndex );
    UInt iHeight = _GetSubResourceBound( 1, pIndex );
    GPUPoint2 hDstPoint;
    GPURectangle hRect;
    hDstPoint.iX = 0;
    hDstPoint.iY = 0;
    if ( pRect != NULL ) {
        hDstPoint.iX = pRect->iLeft;
        hDstPoint.iY = pRect->iTop;
    }
    _ValidateCopy( &hDstPoint, &hRect, iWidth, iHeight, iWidth, iHeight, &hDstPoint, pRect );

    m_hDeviceTexture2D.Update( (const DeviceSubResourceIndex *)pIndex, (const DeviceRectangle *)&hRect, m_arrTexture, pDeviceContext );
}
Void GPUTexture2D::UpdateBack( const GPUTextureIndex * pIndex, const GPURectangle * pRect, GPUDeferredContext * pContext )
{
    Assert( HasCPUData() && IsBound() );
    Assert( CanLock() && CanCPURead() );
    Assert( !(IsLocked()) );

    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    Byte * pData = NULL;
    UInt iLockPitch = 0;

    pData = (Byte*)( m_hDeviceTexture2D.Lock( (const DeviceSubResourceIndex *)pIndex, DEVICERESOURCE_LOCK_READ, DEVICERESOURCE_LOCKFLAG_NONE, &iLockPitch, pDeviceContext ) );

    UInt iWidth = _GetSubResourceBound( 0, pIndex );
    UInt iHeight = _GetSubResourceBound( 1, pIndex );
    GPUPoint2 hDstPoint;
    hDstPoint.iX = 0;
    hDstPoint.iY = 0;
    if ( pRect != NULL ) {
        hDstPoint.iX = pRect->iLeft;
        hDstPoint.iY = pRect->iTop;
    }
    _CopyTexels( _GetSubResource(pIndex), iWidth, iHeight, _GetSubResourcePitch(pIndex), pData, iWidth, iHeight, iLockPitch, m_iStride, &hDstPoint, pRect );

    m_hDeviceTexture2D.UnLock( (const DeviceSubResourceIndex *)pIndex, pDeviceContext );
}

Void GPUTexture2D::CopyCPUData( GPUTexture2D * pDstTexture, Bool bUpdateGPUData, GPUDeferredContext * pContext ) const
{
    Assert( HasCPUData() && pDstTexture->HasCPUData() );
    Assert( m_iFormat == pDstTexture->m_iFormat );
    Assert( m_iSampleCount == pDstTexture->m_iSampleCount );
    Assert( m_arrMipLevelBounds[0][0] == pDstTexture->m_arrMipLevelBounds[0][0] );
    Assert( m_arrMipLevelBounds[0][1] == pDstTexture->m_arrMipLevelBounds[0][1] );
    Assert( m_iMipLevelCount == pDstTexture->m_iMipLevelCount );
    Assert( m_iArrayCount == pDstTexture->m_iArrayCount );
    Assert( m_iSize == pDstTexture->m_iSize );

    MemCopy( pDstTexture->m_arrTexture, m_arrTexture, m_iSize );

    if ( bUpdateGPUData ) {
        GPUTextureIndex hIndex;
        for( hIndex.iArrayIndex = 0; hIndex.iArrayIndex < pDstTexture->m_iArrayCount; ++(hIndex.iArrayIndex) ) {
            for( hIndex.iMipIndex = 0; hIndex.iMipIndex < pDstTexture->m_iMipLevelCount; ++(hIndex.iMipIndex) )
                pDstTexture->Update( &hIndex, NULL, pContext );
        }
    }
}
Void GPUTexture2D::CopyCPUData( GPUTexture2D * pDstTexture, const GPUTextureIndex * pDstIndex, const GPUPoint2 * pDstPoint, const GPUTextureIndex * pSrcIndex, const GPURectangle * pSrcRect,
                                Bool bUpdateGPUData, GPUDeferredContext * pContext ) const
{
    Assert( HasCPUData() && pDstTexture->HasCPUData() );
    Assert( m_iFormat == pDstTexture->m_iFormat );
    Assert( m_iSampleCount == pDstTexture->m_iSampleCount );

    UInt iSrcWidth = _GetSubResourceBound( 0, pSrcIndex );
    UInt iSrcHeight = _GetSubResourceBound( 1, pSrcIndex );
    UInt iDstWidth = pDstTexture->_GetSubResourceBound( 0, pDstIndex );
    UInt iDstHeight = pDstTexture->_GetSubResourceBound( 1, pDstIndex );
    _CopyTexels( pDstTexture->_GetSubResource(pDstIndex), iDstWidth, iDstHeight, pDstTexture->_GetSubResourcePitch(pDstIndex),
                 _GetSubResource(pSrcIndex), iSrcWidth, iSrcHeight, _GetSubResourcePitch(pSrcIndex), m_iStride, pDstPoint, pSrcRect );

    if ( bUpdateGPUData ) {
        GPURectangle hDstRect;
        hDstRect.iLeft = (pDstPoint != NULL) ? pDstPoint->iX : 0;
        hDstRect.iRight = hDstRect.iLeft + ( (pSrcRect != NULL) ? (pSrcRect->iRight - pSrcRect->iLeft) : iSrcWidth );
        hDstRect.iTop = (pDstPoint != NULL) ? pDstPoint->iY : 0;
        hDstRect.iBottom = hDstRect.iTop + ( (pSrcRect != NULL) ? (pSrcRect->iBottom - pSrcRect->iTop) : iSrcHeight );
        pDstTexture->Update( pDstIndex, &hDstRect, pContext );
    }
}
Void GPUTexture2D::CopyGPUData( GPUTexture2D * pDstTexture, Bool bUpdateCPUData, GPUDeferredContext * pContext ) const
{
    Assert( IsBound() && pDstTexture->IsBound() );
    Assert( !(IsLocked()) && !(pDstTexture->IsLocked()) );
    Assert( pDstTexture->CanGPUWrite() );

    Assert( pDstTexture != this );

    Assert( m_iFormat == pDstTexture->m_iFormat );
    Assert( m_iSampleCount == pDstTexture->m_iSampleCount );
    Assert( m_arrMipLevelBounds[0][0] == pDstTexture->m_arrMipLevelBounds[0][0] );
    Assert( m_arrMipLevelBounds[0][1] == pDstTexture->m_arrMipLevelBounds[0][1] );
    Assert( m_iMipLevelCount == pDstTexture->m_iMipLevelCount );
    Assert( m_iArrayCount == pDstTexture->m_iArrayCount );
    Assert( m_iSize == pDstTexture->m_iSize );
    
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_hDeviceTexture2D.Copy( &(pDstTexture->m_hDeviceTexture2D), pDeviceContext );

    if ( bUpdateCPUData ) {
        GPUTextureIndex hIndex;
        for( hIndex.iArrayIndex = 0; hIndex.iArrayIndex < pDstTexture->m_iArrayCount; ++(hIndex.iArrayIndex) ) {
            for( hIndex.iMipIndex = 0; hIndex.iMipIndex < pDstTexture->m_iMipLevelCount; ++(hIndex.iMipIndex) )
                pDstTexture->UpdateBack( &hIndex, NULL, pContext );
        }
    }
}
Void GPUTexture2D::CopyGPUData( GPUTexture2D * pDstTexture, const GPUTextureIndex * pDstIndex, const GPUPoint2 * pDstPoint, const GPUTextureIndex * pSrcIndex, const GPURectangle * pSrcRect,
                                Bool bUpdateCPUData, GPUDeferredContext * pContext ) const
{
    Assert( IsBound() && pDstTexture->IsBound() );
    Assert( !(IsLocked()) && !(pDstTexture->IsLocked()) );
    Assert( pDstTexture->CanGPUWrite() );

    Assert( m_iFormat == pDstTexture->m_iFormat );
    Assert( m_iSampleCount == pDstTexture->m_iSampleCount );

    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    UInt iSrcWidth = _GetSubResourceBound( 0, pSrcIndex );
    UInt iSrcHeight = _GetSubResourceBound( 1, pSrcIndex );
    UInt iDstWidth = pDstTexture->_GetSubResourceBound( 0, pDstIndex );
    UInt iDstHeight = pDstTexture->_GetSubResourceBound( 1, pDstIndex );
    GPUPoint2 hDstPoint;
    GPURectangle hSrcRect;
    _ValidateCopy( &hDstPoint, &hSrcRect, iDstWidth, iDstHeight, iSrcWidth, iSrcHeight, pDstPoint, pSrcRect );

    m_hDeviceTexture2D.Copy( &(pDstTexture->m_hDeviceTexture2D), (const DeviceSubResourceIndex *)pDstIndex, (const DevicePoint2 *)&hDstPoint,
                             (const DeviceSubResourceIndex *)pSrcIndex, (const DeviceRectangle*)&hSrcRect, pDeviceContext );

    if ( bUpdateCPUData ) {
        GPURectangle hDstRect;
        hDstRect.iLeft = (pDstPoint != NULL) ? pDstPoint->iX : 0;
        hDstRect.iRight = hDstRect.iLeft + ( hSrcRect.iRight - hSrcRect.iLeft );
        hDstRect.iTop = (pDstPoint != NULL) ? pDstPoint->iY : 0;
        hDstRect.iBottom = hDstRect.iTop + ( hSrcRect.iBottom - hSrcRect.iTop );
        pDstTexture->UpdateBack( pDstIndex, &hDstRect, pContext );
    }
}

/////////////////////////////////////////////////////////////////////////////////

Void GPUTexture2D::_ValidateCopy( GPUPoint2 * outDstPoint, GPURectangle * outSrcRect, UInt iDstWidth, UInt iDstHeight, UInt iSrcWidth, UInt iSrcHeight,
                                  const GPUPoint2 * pDstPoint, const GPURectangle * pSrcRect )
{
    outSrcRect->iLeft = 0;
    outSrcRect->iRight = iSrcWidth;
    outSrcRect->iTop = 0;
    outSrcRect->iBottom = iSrcHeight;
    if ( pSrcRect != NULL ) {
        Assert( pSrcRect->iLeft < pSrcRect->iRight && pSrcRect->iRight <= (Long)iSrcWidth );
        Assert( pSrcRect->iTop < pSrcRect->iBottom && pSrcRect->iBottom <= (Long)iSrcHeight );
        outSrcRect->iLeft = pSrcRect->iLeft;
        outSrcRect->iRight = pSrcRect->iRight;
        outSrcRect->iTop = pSrcRect->iTop;
        outSrcRect->iBottom = pSrcRect->iBottom;
    }

    outDstPoint->iX = 0;
    outDstPoint->iY = 0;
    if ( pDstPoint != NULL ) {
        Assert( pDstPoint->iX < iDstWidth );
        Assert( pDstPoint->iY < iDstHeight );
        outDstPoint->iX = pDstPoint->iX;
        outDstPoint->iY = pDstPoint->iY;
    }
    Assert( outDstPoint->iX + (outSrcRect->iRight - outSrcRect->iLeft) <= iDstWidth );
    Assert( outDstPoint->iY + (outSrcRect->iBottom - outSrcRect->iTop) <= iDstHeight );
}
Void GPUTexture2D::_CopyTexels( Byte * pDst, UInt iDstWidth, UInt iDstHeight, UInt iDstPitch, const Byte * pSrc, UInt iSrcWidth, UInt iSrcHeight, UInt iSrcPitch, UInt iStride,
                                const GPUPoint2 * pDstPoint, const GPURectangle * pSrcRect )
{
    GPUPoint2 hDstPoint;
    GPURectangle hSrcRect;
    _ValidateCopy( &hDstPoint, &hSrcRect, iDstWidth, iDstHeight, iSrcWidth, iSrcHeight, pDstPoint, pSrcRect );

    pDst += ( (hDstPoint.iY * iDstPitch) + (hDstPoint.iX * iStride) );
    pSrc += ( (hSrcRect.iTop * iSrcPitch) + (hSrcRect.iLeft * iStride) );
    UInt iCopySize = ( (hSrcRect.iRight - hSrcRect.iLeft) * iStride );

    for( Long iY = hSrcRect.iTop; iY < hSrcRect.iBottom; ++iY ) {
        MemCopy( pDst, pSrc, iCopySize );
        pDst += iDstPitch;
        pSrc += iSrcPitch;
    }
}

Bool GPUTexture2D::_OnLoad( ResourceDB * pRDB, RDB_ResourceHandle * hResource )
{
    Assert( !IsBound() );

    TextureHeader hTextureHeader;
    pRDB->ResourceRead( hResource, sizeof(TextureHeader), (Byte*)( &hTextureHeader ) );

    Assert( hTextureHeader.dwType == GPURESOURCE_TEXTURE_2D );
    Assert( hTextureHeader.dwSampleCount == 1 || hTextureHeader.dwSampleCount == 2 || hTextureHeader.dwSampleCount == 4 || hTextureHeader.dwSampleCount == 8 || hTextureHeader.dwSampleCount == 16 );

    m_iFormat = (PixelFormat)( hTextureHeader.dwFormat );
    m_iSampleCount = hTextureHeader.dwSampleCount;
    m_iStride = ( m_iSampleCount * PixelFormatBytes(m_iFormat) );

    Assert( m_iDimensions == 2 );

    m_iMipLevelCount = hTextureHeader.dwMipLevelCount;

    Assert( hTextureHeader.dwBounds[0] > 0 );
    Assert( hTextureHeader.dwBounds[1] > 0 );
    if ( m_iMipLevelCount > 1 ) {
        Assert( ( hTextureHeader.dwBounds[0] & (hTextureHeader.dwBounds[0] - 1) ) == 0 );
        Assert( ( hTextureHeader.dwBounds[1] & (hTextureHeader.dwBounds[1] - 1) ) == 0 );
    }
    m_arrMipLevelBounds[0][0] = hTextureHeader.dwBounds[0];
    m_arrMipLevelBounds[0][1] = hTextureHeader.dwBounds[1];
    m_arrMipLevelBounds[0][2] = 0;

    m_iArrayCount = 1;

    _ComputeTextureLayout();
    Assert( m_iSize == hTextureHeader.dwDataSize );

    RenderingFn->SelectMemory( TEXT("Textures") );

    if ( m_arrTexture != NULL ) {
        DeleteA( m_arrTexture );
        m_arrTexture = NULL;
    }
    m_arrTexture = New Byte[m_iSize];

    RenderingFn->UnSelectMemory();

    pRDB->ResourceRead( hResource, m_iSize, m_arrTexture );

    return true;
}
Bool GPUTexture2D::_OnSave( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) const
{
    Assert( HasCPUData() );
    Assert( m_iArrayCount == 1 );

    TextureHeader hTextureHeader;
    hTextureHeader.dwDataSize = m_iSize;
    hTextureHeader.dwType = GPURESOURCE_TEXTURE_2D;
    hTextureHeader.dwFormat = m_iFormat;
    hTextureHeader.dwSampleCount = m_iSampleCount;
    hTextureHeader.dwMipLevelCount = m_iMipLevelCount;
    hTextureHeader.dwBounds[0] = m_arrMipLevelBounds[0][0];
    hTextureHeader.dwBounds[1] = m_arrMipLevelBounds[0][1];
    hTextureHeader.dwBounds[2] = 0;

    pRDB->ResourceAppend( hResource, sizeof(TextureHeader), (const Byte *)( &hTextureHeader ) );

    pRDB->ResourceAppend( hResource, m_iSize, m_arrTexture );

    return true;
}

Void GPUTexture2D::_OnBackBufferResize( PixelFormat iFormat, UInt iWidth, UInt iHeight, UInt iSampleCount, Void * pUserData )
{
    GPUTexture2D * pThis = (GPUTexture2D*)pUserData;

    if ( pThis->m_iFormat == iFormat && pThis->m_arrMipLevelBounds[0][0] == iWidth && pThis->m_arrMipLevelBounds[0][1] == iHeight &&
         pThis->m_iMipLevelCount == 1 && pThis->m_iArrayCount == 1 && pThis->m_iSampleCount == iSampleCount )
        return;

    Bool bHasCPUData = pThis->HasCPUData();

    if ( bHasCPUData ) {
        RenderingFn->SelectMemory( TEXT("Textures") );
        DeleteA( pThis->m_arrTexture );
        pThis->m_arrTexture = NULL;
        RenderingFn->UnSelectMemory();
    }

    Assert( iFormat != PIXEL_FMT_UNKNOWN && iFormat < PIXEL_FMT_COUNT );
    Assert( iSampleCount == 1 || iSampleCount == 2 || iSampleCount == 4 || iSampleCount == 8 || iSampleCount == 16 );

    pThis->m_iFormat = iFormat;
    pThis->m_iStride = ( iSampleCount * PixelFormatBytes(pThis->m_iFormat) );

    pThis->m_iDimensions = 2;
    pThis->m_arrMipLevelBounds[0][0] = iWidth;
    pThis->m_arrMipLevelBounds[0][1] = iHeight;
    pThis->m_arrMipLevelBounds[0][2] = 0;

    pThis->m_iMipLevelCount = 1;
    pThis->m_iArrayCount = 1;

    pThis->_ComputeTextureLayout();

    pThis->m_iSampleCount = iSampleCount;

    if ( bHasCPUData ) {
        RenderingFn->SelectMemory( TEXT("Textures") );
        pThis->m_arrTexture = New Byte[pThis->m_iSize];
        RenderingFn->UnSelectMemory();
    }
}

/////////////////////////////////////////////////////////////////////////////////
// GPUTexture3D implementation
GPUTexture3D::GPUTexture3D( PixelFormat iFormat, UInt iWidth, UInt iHeight, UInt iDepth, UInt iMipLevelCount, const Byte * arrTexels, Bool bCreateCPUData ):
    GPUTexture( iFormat, 3, _MakeBoundArray(iWidth, iHeight, iDepth), iMipLevelCount, 1, 1, arrTexels, bCreateCPUData ), m_hDeviceTexture3D( RenderingFn->m_pDeviceRenderer )
{
    m_pDeviceTexture = &m_hDeviceTexture3D;
    m_pDeviceResource = m_pDeviceTexture;
}
GPUTexture3D::~GPUTexture3D()
{
    if ( IsBound() )
        UnBind();
}

Void GPUTexture3D::Bind( UInt iGPUResourceBinds )
{
    Assert( !(IsBound()) );

    m_hDeviceTexture3D.Create( iGPUResourceBinds, m_iFormat, m_arrMipLevelBounds[0][0], m_arrMipLevelBounds[0][1], m_arrMipLevelBounds[0][2], m_iMipLevelCount, m_arrTexture );

    RenderingFn->_RegisterTexture( this );
}

Void GPUTexture3D::SetTexels( const Byte * arrTexels, UInt iTexelsPitch, UInt iTexelsSlice, const GPUBox * pBox, const GPUTextureIndex * pIndex )
{
    Assert( HasCPUData() );

    if ( pBox == NULL ) {
        MemCopy( _GetSubResource(pIndex), arrTexels, _GetSubResourceSize(pIndex) );
        return;
    }

    GPUPoint3 hDstPoint;
    hDstPoint.iX = pBox->iLeft;
    hDstPoint.iY = pBox->iTop;
    hDstPoint.iZ = pBox->iFront;

    UInt iWidth = _GetSubResourceBound( 0, pIndex );
    UInt iHeight = _GetSubResourceBound( 1, pIndex );
    UInt iDepth = _GetSubResourceBound( 2, pIndex );
    _CopyTexels( _GetSubResource(pIndex), iWidth, iHeight, iDepth, _GetSubResourcePitch(pIndex), _GetSubResourceSlice(pIndex),
                 arrTexels, (pBox->iRight - pBox->iLeft), (pBox->iBottom - pBox->iTop), (pBox->iBack - pBox->iFront), iTexelsPitch, iTexelsSlice,
                 m_iStride, &hDstPoint, NULL );
}

Void GPUTexture3D::GenerateMipMap()
{
    Assert( HasCPUData() );
    Assert( PixelFormatIsMipMapable(m_iFormat) );

    if ( m_iMipLevelCount <= 1 )
        return;

    Byte * arrLERP[8];

    GPUTextureIndex hIndex;
    for( hIndex.iArrayIndex = 0; hIndex.iArrayIndex < m_iArrayCount; ++(hIndex.iArrayIndex) ) {
        hIndex.iMipIndex = 0;
        Byte * pPrevSubResource = _GetSubResource( &hIndex );
        UInt iPrevPitch = _GetSubResourcePitch( &hIndex );
        UInt iPrevSlice = iPrevPitch * _GetSubResourceBound( 1, &hIndex ); // Pitch * Height

        for( hIndex.iMipIndex = 1; hIndex.iMipIndex < m_iMipLevelCount; ++(hIndex.iMipIndex) ) {
            Byte * pSubResource = _GetSubResource( &hIndex );
            UInt iWidth = _GetSubResourceBound( 0, &hIndex );
            UInt iHeight = _GetSubResourceBound( 1, &hIndex );
            UInt iDepth = _GetSubResourceBound( 2, &hIndex );
            UInt iPitch = ( iWidth * m_iStride );
            UInt iSlice = ( iHeight * iPitch );
            Byte *pSrc, *pDst;

            for( UInt iZ = 0; iZ < iDepth; ++iZ ) {
                for( UInt iY = 0; iY < iHeight; ++iY ) {
                    for( UInt iX = 0; iX < iWidth; ++iX ) {
                        pSrc = pPrevSubResource + ((iZ << 1) * iPrevSlice) + ((iY << 1) * iPrevPitch) + ((iX << 1) * m_iStride);
                        pDst = pSubResource + (iZ * iSlice) + (iY * iPitch) + (iX * m_iStride);

                        arrLERP[0] = pSrc;
                        arrLERP[1] = pSrc + m_iStride;
                        arrLERP[2] = pSrc + iPrevPitch;
                        arrLERP[3] = pSrc + iPrevPitch + m_iStride;
                        arrLERP[4] = pSrc + iPrevSlice;
                        arrLERP[5] = pSrc + iPrevSlice + m_iStride;
                        arrLERP[6] = pSrc + iPrevSlice + iPrevPitch;
                        arrLERP[7] = pSrc + iPrevSlice + iPrevPitch + m_iStride;
                        PixelFormatLERP( pDst, (const Byte **)arrLERP, 8, m_iFormat );
                    }
                }
            }
            
            pPrevSubResource = pSubResource;
            iPrevPitch = iPitch;
            iPrevSlice = iSlice;
        }
    }
}

Void * GPUTexture3D::Lock( const GPUTextureIndex * pIndex, GPUResourceLock iLockType, UInt iGPUResourceLockFlags, UInt * outPitch, UInt * outSlice, GPUDeferredContext * pContext )
{
    Assert( IsBound() );
    Assert( CanLock() );
    Assert( !(IsLocked()) );

    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    return m_hDeviceTexture3D.Lock( (const DeviceSubResourceIndex *)pIndex, (DeviceResourceLock)iLockType, iGPUResourceLockFlags, outPitch, outSlice, pDeviceContext );
}
Void GPUTexture3D::UnLock( const GPUTextureIndex * pIndex, GPUDeferredContext * pContext )
{
    Assert( IsBound() );
    Assert( CanLock() );
    Assert( IsLocked() );

    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_hDeviceTexture3D.UnLock( (const DeviceSubResourceIndex *)pIndex, pDeviceContext );
}

Void GPUTexture3D::Update( const GPUTextureIndex * pIndex, const GPUBox * pBox, GPUDeferredContext * pContext )
{
    Assert( HasCPUData() && IsBound() );
    Assert( CanUpdate() );
    Assert( !(IsLocked()) );

    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    UInt iWidth = _GetSubResourceBound( 0, pIndex );
    UInt iHeight = _GetSubResourceBound( 1, pIndex );
    UInt iDepth = _GetSubResourceBound( 2, pIndex );
    GPUPoint3 hDstPoint;
    GPUBox hBox;
    hDstPoint.iX = 0;
    hDstPoint.iY = 0;
    hDstPoint.iZ = 0;
    if ( pBox != NULL ) {
        hDstPoint.iX = pBox->iLeft;
        hDstPoint.iY = pBox->iTop;
        hDstPoint.iZ = pBox->iFront;
    }
    _ValidateCopy( &hDstPoint, &hBox, iWidth, iHeight, iDepth, iWidth, iHeight, iDepth, &hDstPoint, pBox );

    m_hDeviceTexture3D.Update( (const DeviceSubResourceIndex *)pIndex, (const DeviceBox *)&hBox, m_arrTexture, pDeviceContext );
}
Void GPUTexture3D::UpdateBack( const GPUTextureIndex * pIndex, const GPUBox * pBox, GPUDeferredContext * pContext )
{
    Assert( HasCPUData() && IsBound() );
    Assert( CanLock() && CanCPURead() );
    Assert( !(IsLocked()) );

    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    Byte * pData = NULL;
    UInt iLockPitch = 0;
    UInt iLockSlice = 0;

    pData = (Byte*)( m_hDeviceTexture3D.Lock( (const DeviceSubResourceIndex *)pIndex, DEVICERESOURCE_LOCK_READ, DEVICERESOURCE_LOCKFLAG_NONE, &iLockPitch, &iLockSlice, pDeviceContext ) );

    UInt iWidth = _GetSubResourceBound( 0, pIndex );
    UInt iHeight = _GetSubResourceBound( 1, pIndex );
    UInt iDepth = _GetSubResourceBound( 2, pIndex );
    GPUPoint3 hDstPoint;
    hDstPoint.iX = 0;
    hDstPoint.iY = 0;
    hDstPoint.iZ = 0;
    if ( pBox != NULL ) {
        hDstPoint.iX = pBox->iLeft;
        hDstPoint.iY = pBox->iTop;
        hDstPoint.iZ = pBox->iFront;
    }
    _CopyTexels( _GetSubResource(pIndex), iWidth, iHeight, iDepth, _GetSubResourcePitch(pIndex), _GetSubResourceSlice(pIndex),
                 pData, iWidth, iHeight, iDepth, iLockPitch, iLockSlice, m_iStride, &hDstPoint, pBox );

    m_hDeviceTexture3D.UnLock( (const DeviceSubResourceIndex *)pIndex, pDeviceContext );
}

Void GPUTexture3D::CopyCPUData( GPUTexture3D * pDstTexture, Bool bUpdateGPUData, GPUDeferredContext * pContext ) const
{
    Assert( HasCPUData() && pDstTexture->HasCPUData() );
    Assert( m_iFormat == pDstTexture->m_iFormat );
    Assert( m_arrMipLevelBounds[0][0] == pDstTexture->m_arrMipLevelBounds[0][0] );
    Assert( m_arrMipLevelBounds[0][1] == pDstTexture->m_arrMipLevelBounds[0][1] );
    Assert( m_arrMipLevelBounds[0][2] == pDstTexture->m_arrMipLevelBounds[0][2] );
    Assert( m_iMipLevelCount == pDstTexture->m_iMipLevelCount );
    Assert( m_iArrayCount == pDstTexture->m_iArrayCount );
    Assert( m_iSize == pDstTexture->m_iSize );

    MemCopy( pDstTexture->m_arrTexture, m_arrTexture, m_iSize );

    if ( bUpdateGPUData ) {
        GPUTextureIndex hIndex;
        for( hIndex.iArrayIndex = 0; hIndex.iArrayIndex < pDstTexture->m_iArrayCount; ++(hIndex.iArrayIndex) ) {
            for( hIndex.iMipIndex = 0; hIndex.iMipIndex < pDstTexture->m_iMipLevelCount; ++(hIndex.iMipIndex) )
                pDstTexture->Update( &hIndex, NULL, pContext );
        }
    }
}
Void GPUTexture3D::CopyCPUData( GPUTexture3D * pDstTexture, const GPUTextureIndex * pDstIndex, const GPUPoint3 * pDstPoint, const GPUTextureIndex * pSrcIndex, const GPUBox * pSrcBox,
                                Bool bUpdateGPUData, GPUDeferredContext * pContext ) const
{
    Assert( HasCPUData() && pDstTexture->HasCPUData() );
    Assert( m_iFormat == pDstTexture->m_iFormat );

    UInt iSrcWidth = _GetSubResourceBound( 0, pSrcIndex );
    UInt iSrcHeight = _GetSubResourceBound( 1, pSrcIndex );
    UInt iSrcDepth = _GetSubResourceBound( 2, pSrcIndex );
    UInt iDstWidth = pDstTexture->_GetSubResourceBound( 0, pDstIndex );
    UInt iDstHeight = pDstTexture->_GetSubResourceBound( 1, pDstIndex );
    UInt iDstDepth = pDstTexture->_GetSubResourceBound( 2, pDstIndex );
    _CopyTexels( pDstTexture->_GetSubResource(pDstIndex), iDstWidth, iDstHeight, iDstDepth, pDstTexture->_GetSubResourcePitch(pDstIndex), pDstTexture->_GetSubResourceSlice(pDstIndex),
                 _GetSubResource(pSrcIndex), iSrcWidth, iSrcHeight, iSrcDepth, _GetSubResourcePitch(pSrcIndex), _GetSubResourceSlice(pSrcIndex), m_iStride, pDstPoint, pSrcBox );

    if ( bUpdateGPUData ) {
        GPUBox hDstBox;
        hDstBox.iLeft = (pDstPoint != NULL) ? pDstPoint->iX : 0;
        hDstBox.iRight = hDstBox.iLeft + ( (pSrcBox != NULL) ? (pSrcBox->iRight - pSrcBox->iLeft) : iSrcWidth );
        hDstBox.iTop = (pDstPoint != NULL) ? pDstPoint->iY : 0;
        hDstBox.iBottom = hDstBox.iTop + ( (pSrcBox != NULL) ? (pSrcBox->iBottom - pSrcBox->iTop) : iSrcHeight );
        hDstBox.iFront = (pDstPoint != NULL) ? pDstPoint->iZ : 0;
        hDstBox.iBack = hDstBox.iFront + ( (pSrcBox != NULL) ? (pSrcBox->iBack - pSrcBox->iFront) : iSrcDepth );
        pDstTexture->Update( pDstIndex, &hDstBox, pContext );
    }
}
Void GPUTexture3D::CopyGPUData( GPUTexture3D * pDstTexture, Bool bUpdateCPUData, GPUDeferredContext * pContext ) const
{
    Assert( IsBound() && pDstTexture->IsBound() );
    Assert( !(IsLocked()) && !(pDstTexture->IsLocked()) );
    Assert( pDstTexture->CanGPUWrite() );

    Assert( pDstTexture != this );

    Assert( m_iFormat == pDstTexture->m_iFormat );
    Assert( m_arrMipLevelBounds[0][0] == pDstTexture->m_arrMipLevelBounds[0][0] );
    Assert( m_arrMipLevelBounds[0][1] == pDstTexture->m_arrMipLevelBounds[0][1] );
    Assert( m_arrMipLevelBounds[0][2] == pDstTexture->m_arrMipLevelBounds[0][2] );
    Assert( m_iMipLevelCount == pDstTexture->m_iMipLevelCount );
    Assert( m_iArrayCount == pDstTexture->m_iArrayCount );
    Assert( m_iSize == pDstTexture->m_iSize );
    
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_hDeviceTexture3D.Copy( &(pDstTexture->m_hDeviceTexture3D), pDeviceContext );

    if ( bUpdateCPUData ) {
        GPUTextureIndex hIndex;
        for( hIndex.iArrayIndex = 0; hIndex.iArrayIndex < pDstTexture->m_iArrayCount; ++(hIndex.iArrayIndex) ) {
            for( hIndex.iMipIndex = 0; hIndex.iMipIndex < pDstTexture->m_iMipLevelCount; ++(hIndex.iMipIndex) )
                pDstTexture->UpdateBack( &hIndex, NULL, pContext );
        }
    }
}
Void GPUTexture3D::CopyGPUData( GPUTexture3D * pDstTexture, const GPUTextureIndex * pDstIndex, const GPUPoint3 * pDstPoint, const GPUTextureIndex * pSrcIndex, const GPUBox * pSrcBox,
                                Bool bUpdateCPUData, GPUDeferredContext * pContext ) const
{
    Assert( IsBound() && pDstTexture->IsBound() );
    Assert( !(IsLocked()) && !(pDstTexture->IsLocked()) );
    Assert( pDstTexture->CanGPUWrite() );

    Assert( m_iFormat == pDstTexture->m_iFormat );

    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    UInt iSrcWidth = _GetSubResourceBound( 0, pSrcIndex );
    UInt iSrcHeight = _GetSubResourceBound( 1, pSrcIndex );
    UInt iSrcDepth = _GetSubResourceBound( 2, pSrcIndex );
    UInt iDstWidth = pDstTexture->_GetSubResourceBound( 0, pDstIndex );
    UInt iDstHeight = pDstTexture->_GetSubResourceBound( 1, pDstIndex );
    UInt iDstDepth = pDstTexture->_GetSubResourceBound( 2, pDstIndex );
    GPUPoint3 hDstPoint;
    GPUBox hSrcBox;
    _ValidateCopy( &hDstPoint, &hSrcBox, iDstWidth, iDstHeight, iDstDepth, iSrcWidth, iSrcHeight, iSrcDepth, pDstPoint, pSrcBox );

    m_hDeviceTexture3D.Copy( &(pDstTexture->m_hDeviceTexture3D), (const DeviceSubResourceIndex *)pDstIndex, (const DevicePoint3 *)&hDstPoint,
                             (const DeviceSubResourceIndex *)pSrcIndex, (const DeviceBox *)&hSrcBox, pDeviceContext );

    if ( bUpdateCPUData ) {
        GPUBox hDstBox;
        hDstBox.iLeft = (pDstPoint != NULL) ? pDstPoint->iX : 0;
        hDstBox.iRight = hDstBox.iLeft + ( hSrcBox.iRight - hSrcBox.iLeft );
        hDstBox.iTop = (pDstPoint != NULL) ? pDstPoint->iY : 0;
        hDstBox.iBottom = hDstBox.iTop + ( hSrcBox.iBottom - hSrcBox.iTop );
        hDstBox.iFront = (pDstPoint != NULL) ? pDstPoint->iZ : 0;
        hDstBox.iBack = hDstBox.iFront + ( hSrcBox.iBack - hSrcBox.iFront );
        pDstTexture->Update( pDstIndex, &hDstBox, pContext );
    }
}

/////////////////////////////////////////////////////////////////////////////////

Void GPUTexture3D::_ValidateCopy( GPUPoint3 * outDstPoint, GPUBox * outSrcBox, UInt iDstWidth, UInt iDstHeight, UInt iDstDepth, UInt iSrcWidth, UInt iSrcHeight, UInt iSrcDepth,
                                  const GPUPoint3 * pDstPoint, const GPUBox * pSrcBox )
{
    outSrcBox->iLeft = 0;
    outSrcBox->iRight = iSrcWidth;
    outSrcBox->iTop = 0;
    outSrcBox->iBottom = iSrcHeight;
    outSrcBox->iFront = 0;
    outSrcBox->iBack = iSrcDepth;
    if ( pSrcBox != NULL ) {
        Assert( pSrcBox->iLeft < pSrcBox->iRight && pSrcBox->iRight <= iSrcWidth );
        Assert( pSrcBox->iTop < pSrcBox->iBottom && pSrcBox->iBottom <= iSrcHeight );
        Assert( pSrcBox->iFront < pSrcBox->iBack && pSrcBox->iBack <= iSrcDepth );
        outSrcBox->iLeft = pSrcBox->iLeft;
        outSrcBox->iRight = pSrcBox->iRight;
        outSrcBox->iTop = pSrcBox->iTop;
        outSrcBox->iBottom = pSrcBox->iBottom;
        outSrcBox->iFront = pSrcBox->iFront;
        outSrcBox->iBack = pSrcBox->iBack;
    }

    outDstPoint->iX = 0;
    outDstPoint->iY = 0;
    outDstPoint->iZ = 0;
    if ( pDstPoint != NULL ) {
        Assert( pDstPoint->iX < iDstWidth );
        Assert( pDstPoint->iY < iDstHeight );
        Assert( pDstPoint->iZ < iDstDepth );
        outDstPoint->iX = pDstPoint->iX;
        outDstPoint->iY = pDstPoint->iY;
        outDstPoint->iZ = pDstPoint->iZ;
    }
    Assert( outDstPoint->iX + (outSrcBox->iRight - outSrcBox->iLeft) <= iDstWidth );
    Assert( outDstPoint->iY + (outSrcBox->iBottom - outSrcBox->iTop) <= iDstHeight );
    Assert( outDstPoint->iZ + (outSrcBox->iBack - outSrcBox->iFront) <= iDstDepth );
}
Void GPUTexture3D::_CopyTexels( Byte * pDst, UInt iDstWidth, UInt iDstHeight, UInt iDstDepth, UInt iDstPitch, UInt iDstSlice,
                                const Byte * pSrc, UInt iSrcWidth, UInt iSrcHeight, UInt iSrcDepth, UInt iSrcPitch, UInt iSrcSlice, UInt iStride,
                                const GPUPoint3 * pDstPoint, const GPUBox * pSrcBox )
{
    GPUPoint3 hDstPoint;
    GPUBox hSrcBox;
    _ValidateCopy( &hDstPoint, &hSrcBox, iDstWidth, iDstHeight, iDstDepth, iSrcWidth, iSrcHeight, iSrcDepth, pDstPoint, pSrcBox );

    pDst += ( (hDstPoint.iZ * iDstSlice) + (hDstPoint.iY * iDstPitch) + (hDstPoint.iX * iStride) );
    pSrc += ( (hSrcBox.iFront * iSrcSlice) + (hSrcBox.iTop * iSrcPitch) + (hSrcBox.iLeft * iStride) );
    UInt iCopySize = ( (hSrcBox.iRight - hSrcBox.iLeft) * iStride );

    for( UInt iZ = hSrcBox.iFront; iZ < hSrcBox.iBack; ++iZ ) {
        Byte * pTmpDst = pDst;
        const Byte * pTmpSrc = pSrc;
        for( UInt iY = hSrcBox.iTop; iY < hSrcBox.iBottom; ++iY ) {
            MemCopy( pTmpDst, pTmpSrc, iCopySize );
            pTmpDst += iDstPitch;
            pTmpSrc += iSrcPitch;
        }
        pDst += iDstSlice;
        pSrc += iSrcSlice;
    }
}

Bool GPUTexture3D::_OnLoad( ResourceDB * pRDB, RDB_ResourceHandle * hResource )
{
    Assert( !IsBound() );

    TextureHeader hTextureHeader;
    pRDB->ResourceRead( hResource, sizeof(TextureHeader), (Byte*)( &hTextureHeader ) );

    Assert( hTextureHeader.dwType == GPURESOURCE_TEXTURE_3D );
    Assert( hTextureHeader.dwSampleCount == 1 );

    m_iFormat = (PixelFormat)( hTextureHeader.dwFormat );
    m_iStride = ( hTextureHeader.dwSampleCount * PixelFormatBytes(m_iFormat) );

    Assert( m_iDimensions == 3 );

    m_iMipLevelCount = hTextureHeader.dwMipLevelCount;

    Assert( hTextureHeader.dwBounds[0] > 0 );
    Assert( hTextureHeader.dwBounds[1] > 0 );
    Assert( hTextureHeader.dwBounds[2] > 0 );
    if ( m_iMipLevelCount > 1 ) {
        Assert( ( hTextureHeader.dwBounds[0] & (hTextureHeader.dwBounds[0] - 1) ) == 0 );
        Assert( ( hTextureHeader.dwBounds[1] & (hTextureHeader.dwBounds[1] - 1) ) == 0 );
        Assert( ( hTextureHeader.dwBounds[2] & (hTextureHeader.dwBounds[2] - 1) ) == 0 );
    }
    m_arrMipLevelBounds[0][0] = hTextureHeader.dwBounds[0];
    m_arrMipLevelBounds[0][1] = hTextureHeader.dwBounds[1];
    m_arrMipLevelBounds[0][2] = hTextureHeader.dwBounds[2];

    m_iArrayCount = 1;

    _ComputeTextureLayout();
    Assert( m_iSize == hTextureHeader.dwDataSize );

    RenderingFn->SelectMemory( TEXT("Textures") );

    if ( m_arrTexture != NULL ) {
        DeleteA( m_arrTexture );
        m_arrTexture = NULL;
    }
    m_arrTexture = New Byte[m_iSize];

    RenderingFn->UnSelectMemory();

    pRDB->ResourceRead( hResource, m_iSize, m_arrTexture );

    return true;
}
Bool GPUTexture3D::_OnSave( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) const
{
    Assert( HasCPUData() );
    Assert( m_iArrayCount == 1 );

    TextureHeader hTextureHeader;
    hTextureHeader.dwDataSize = m_iSize;
    hTextureHeader.dwType = GPURESOURCE_TEXTURE_3D;
    hTextureHeader.dwFormat = m_iFormat;
    hTextureHeader.dwSampleCount = 1;
    hTextureHeader.dwMipLevelCount = m_iMipLevelCount;
    hTextureHeader.dwBounds[0] = m_arrMipLevelBounds[0][0];
    hTextureHeader.dwBounds[1] = m_arrMipLevelBounds[0][1];
    hTextureHeader.dwBounds[2] = m_arrMipLevelBounds[0][2];

    pRDB->ResourceAppend( hResource, sizeof(TextureHeader), (const Byte *)( &hTextureHeader ) );

    pRDB->ResourceAppend( hResource, m_iSize, m_arrTexture );

    return true;
}

/////////////////////////////////////////////////////////////////////////////////
// GPUTextureCube implementation
GPUTextureCube::GPUTextureCube( PixelFormat iFormat, UInt iEdgeLength, UInt iMipLevelCount, UInt iCubeCount, const Byte * arrTexels, Bool bCreateCPUData ):
    GPUTexture( iFormat, 2, _MakeBoundArray(iEdgeLength, iEdgeLength), iMipLevelCount, iCubeCount * 6, 1, arrTexels, bCreateCPUData ), m_hDeviceTextureCube( RenderingFn->m_pDeviceRenderer )
{
    m_pDeviceTexture = &m_hDeviceTextureCube;
    m_pDeviceResource = m_pDeviceTexture;
}
GPUTextureCube::~GPUTextureCube()
{
    if ( IsBound() )
        UnBind();
}

Void GPUTextureCube::Bind( UInt iGPUResourceBinds )
{
    Assert( !(IsBound()) );

    m_hDeviceTextureCube.Create( iGPUResourceBinds, m_iFormat, m_arrMipLevelBounds[0][0], m_iMipLevelCount, m_iArrayCount, m_arrTexture );

    RenderingFn->_RegisterTexture( this );
}

Void GPUTextureCube::SetTexels( const Byte * arrTexels, UInt iTexelsPitch, const GPURectangle * pRect, const GPUTextureIndex * pIndex, GPUTextureCubeFace iFace )
{
    Assert( HasCPUData() );

    if ( pRect == NULL ) {
        MemCopy( _GetSubResource(pIndex, iFace), arrTexels, _GetSubResourceSize(pIndex) );
        return;
    }

    GPUPoint2 hDstPoint;
    hDstPoint.iX = pRect->iLeft;
    hDstPoint.iY = pRect->iTop;

    UInt iWidth = _GetSubResourceBound( 0, pIndex );
    UInt iHeight = _GetSubResourceBound( 1, pIndex );
    _CopyTexels( _GetSubResource(pIndex, iFace), iWidth, iHeight, _GetSubResourcePitch(pIndex), arrTexels, (pRect->iRight - pRect->iLeft), (pRect->iBottom - pRect->iTop), iTexelsPitch,
                 m_iStride, &hDstPoint, NULL );
}

Void GPUTextureCube::GenerateMipMap()
{
    Assert( HasCPUData() );
    Assert( PixelFormatIsMipMapable(m_iFormat) );

    if ( m_iMipLevelCount <= 1 )
        return;

    Byte * arrLERP[4];

    GPUTextureIndex hIndex;
    for( hIndex.iArrayIndex = 0; hIndex.iArrayIndex < m_iArrayCount; ++(hIndex.iArrayIndex) ) {
        hIndex.iMipIndex = 0;
        Byte * pPrevSubResource = _GetSubResource( &hIndex );
        UInt iPrevPitch = _GetSubResourcePitch( &hIndex );

        for( hIndex.iMipIndex = 1; hIndex.iMipIndex < m_iMipLevelCount; ++(hIndex.iMipIndex) ) {
            Byte * pSubResource = _GetSubResource( &hIndex );
            UInt iWidth = _GetSubResourceBound( 0, &hIndex );
            UInt iHeight = _GetSubResourceBound( 1, &hIndex );
            UInt iPitch = ( iWidth * m_iStride );
            Byte *pSrc, *pDst;

            for( UInt iY = 0; iY < iHeight; ++iY ) {
                for( UInt iX = 0; iX < iWidth; ++iX ) {
                    pSrc = pPrevSubResource + ((iY << 1) * iPrevPitch) + ((iX << 1) * m_iStride);
                    pDst = pSubResource + (iY * iPitch) + (iX * m_iStride);

                    arrLERP[0] = pSrc;
                    arrLERP[1] = pSrc + m_iStride;
                    arrLERP[2] = pSrc + iPrevPitch;
                    arrLERP[3] = pSrc + iPrevPitch + m_iStride;
                    PixelFormatLERP( pDst, (const Byte **)arrLERP, 4, m_iFormat );
                }
            }
            
            pPrevSubResource = pSubResource;
            iPrevPitch = iPitch;
        }
    }
}

Void * GPUTextureCube::Lock( const GPUTextureIndex * pIndex, GPUTextureCubeFace iFace, GPUResourceLock iLockType, UInt iGPUResourceLockFlags, UInt * outPitch, GPUDeferredContext * pContext )
{
    Assert( IsBound() );
    Assert( CanLock() );
    Assert( !(IsLocked()) );

    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    return m_hDeviceTextureCube.Lock( (const DeviceSubResourceIndex *)pIndex, (DeviceTextureCubeFace)iFace, (DeviceResourceLock)iLockType, iGPUResourceLockFlags, outPitch, pDeviceContext );
}
Void GPUTextureCube::UnLock( const GPUTextureIndex * pIndex, GPUTextureCubeFace iFace, GPUDeferredContext * pContext )
{
    Assert( IsBound() );
    Assert( CanLock() );
    Assert( IsLocked() );

    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_hDeviceTextureCube.UnLock( (const DeviceSubResourceIndex *)pIndex, (DeviceTextureCubeFace)iFace, pDeviceContext );
}

Void GPUTextureCube::Update( const GPUTextureIndex * pIndex, GPUTextureCubeFace iFace, const GPURectangle * pRect, GPUDeferredContext * pContext )
{
    Assert( HasCPUData() && IsBound() );
    Assert( CanUpdate() );
    Assert( !(IsLocked()) );

    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    UInt iWidth = _GetSubResourceBound( 0, pIndex );
    UInt iHeight = _GetSubResourceBound( 1, pIndex );
    GPUPoint2 hDstPoint;
    GPURectangle hRect;
    hDstPoint.iX = 0;
    hDstPoint.iY = 0;
    if ( pRect != NULL ) {
        hDstPoint.iX = pRect->iLeft;
        hDstPoint.iY = pRect->iTop;
    }
    _ValidateCopy( &hDstPoint, &hRect, iWidth, iHeight, iWidth, iHeight, &hDstPoint, pRect );

    m_hDeviceTextureCube.Update( (const DeviceSubResourceIndex *)pIndex, (DeviceTextureCubeFace)iFace, (const DeviceRectangle *)&hRect, m_arrTexture, pDeviceContext );
}
Void GPUTextureCube::UpdateBack( const GPUTextureIndex * pIndex, GPUTextureCubeFace iFace, const GPURectangle * pRect, GPUDeferredContext * pContext )
{
    Assert( HasCPUData() && IsBound() );
    Assert( CanLock() && CanCPURead() );
    Assert( !(IsLocked()) );

    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    Byte * pData = NULL;
    UInt iLockPitch = 0;

    pData = (Byte*)( m_hDeviceTextureCube.Lock( (const DeviceSubResourceIndex *)pIndex, (DeviceTextureCubeFace)iFace, DEVICERESOURCE_LOCK_READ, DEVICERESOURCE_LOCKFLAG_NONE, &iLockPitch, pDeviceContext ) );

    UInt iWidth = _GetSubResourceBound( 0, pIndex );
    UInt iHeight = _GetSubResourceBound( 1, pIndex );
    GPUPoint2 hDstPoint;
    hDstPoint.iX = 0;
    hDstPoint.iY = 0;
    if ( pRect != NULL ) {
        hDstPoint.iX = pRect->iLeft;
        hDstPoint.iY = pRect->iTop;
    }
    _CopyTexels( _GetSubResource(pIndex, iFace), iWidth, iHeight, _GetSubResourcePitch(pIndex), pData, iWidth, iHeight, iLockPitch, m_iStride, &hDstPoint, pRect );

    m_hDeviceTextureCube.UnLock( (const DeviceSubResourceIndex *)pIndex, (DeviceTextureCubeFace)iFace, pDeviceContext );
}

Void GPUTextureCube::CopyCPUData( GPUTextureCube * pDstTexture, Bool bUpdateGPUData, GPUDeferredContext * pContext ) const
{
    Assert( HasCPUData() && pDstTexture->HasCPUData() );
    Assert( m_iFormat == pDstTexture->m_iFormat );
    Assert( m_arrMipLevelBounds[0][0] == pDstTexture->m_arrMipLevelBounds[0][0] );
    Assert( m_arrMipLevelBounds[0][1] == pDstTexture->m_arrMipLevelBounds[0][1] );
    Assert( m_iMipLevelCount == pDstTexture->m_iMipLevelCount );
    Assert( m_iArrayCount == pDstTexture->m_iArrayCount );
    Assert( m_iSize == pDstTexture->m_iSize );

    MemCopy( pDstTexture->m_arrTexture, m_arrTexture, m_iSize );

    if ( bUpdateGPUData ) {
        UInt iCubeCount = ( pDstTexture->m_iArrayCount / 6 );
        GPUTextureIndex hIndex;
        for( hIndex.iArrayIndex = 0; hIndex.iArrayIndex < iCubeCount; ++(hIndex.iArrayIndex) ) {
            for( UInt iFace = 0; iFace < 6; ++iFace ) {
                GPUTextureCubeFace iCubeFace = (GPUTextureCubeFace)( GPUTEXTURE_CUBEFACE_X_POS + iFace );
                for( hIndex.iMipIndex = 0; hIndex.iMipIndex < pDstTexture->m_iMipLevelCount; ++(hIndex.iMipIndex) )
                    pDstTexture->Update( &hIndex, iCubeFace, NULL, pContext );
            }
        }
    }
}
Void GPUTextureCube::CopyCPUData( GPUTextureCube * pDstTexture, const GPUTextureIndex * pDstIndex, GPUTextureCubeFace iDstFace, const GPUPoint2 * pDstPoint,
                                  const GPUTextureIndex * pSrcIndex, GPUTextureCubeFace iSrcFace, const GPURectangle * pSrcRect, Bool bUpdateGPUData, GPUDeferredContext * pContext ) const
{
    Assert( HasCPUData() && pDstTexture->HasCPUData() );
    Assert( m_iFormat == pDstTexture->m_iFormat );

    UInt iSrcWidth = _GetSubResourceBound( 0, pSrcIndex );
    UInt iSrcHeight = _GetSubResourceBound( 1, pSrcIndex );
    UInt iDstWidth = pDstTexture->_GetSubResourceBound( 0, pDstIndex );
    UInt iDstHeight = pDstTexture->_GetSubResourceBound( 1, pDstIndex );
    _CopyTexels( pDstTexture->_GetSubResource(pDstIndex, iDstFace), iDstWidth, iDstHeight, pDstTexture->_GetSubResourcePitch(pDstIndex),
                 _GetSubResource(pSrcIndex, iSrcFace), iSrcWidth, iSrcHeight, _GetSubResourcePitch(pSrcIndex), m_iStride, pDstPoint, pSrcRect );

    if ( bUpdateGPUData ) {
        GPURectangle hDstRect;
        hDstRect.iLeft = (pDstPoint != NULL) ? pDstPoint->iX : 0;
        hDstRect.iRight = hDstRect.iLeft + ( (pSrcRect != NULL) ? (pSrcRect->iRight - pSrcRect->iLeft) : iSrcWidth );
        hDstRect.iTop = (pDstPoint != NULL) ? pDstPoint->iY : 0;
        hDstRect.iBottom = hDstRect.iTop + ( (pSrcRect != NULL) ? (pSrcRect->iBottom - pSrcRect->iTop) : iSrcHeight );
        pDstTexture->Update( pDstIndex, iDstFace, &hDstRect, pContext );
    }
}
Void GPUTextureCube::CopyGPUData( GPUTextureCube * pDstTexture, Bool bUpdateCPUData, GPUDeferredContext * pContext ) const
{
    Assert( IsBound() && pDstTexture->IsBound() );
    Assert( !(IsLocked()) && !(pDstTexture->IsLocked()) );
    Assert( pDstTexture->CanGPUWrite() );

    Assert( pDstTexture != this );

    Assert( m_iFormat == pDstTexture->m_iFormat );
    Assert( m_arrMipLevelBounds[0][0] == pDstTexture->m_arrMipLevelBounds[0][0] );
    Assert( m_arrMipLevelBounds[0][1] == pDstTexture->m_arrMipLevelBounds[0][1] );
    Assert( m_iMipLevelCount == pDstTexture->m_iMipLevelCount );
    Assert( m_iArrayCount == pDstTexture->m_iArrayCount );
    Assert( m_iSize == pDstTexture->m_iSize );
    
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_hDeviceTextureCube.Copy( &(pDstTexture->m_hDeviceTextureCube), pDeviceContext );

    if ( bUpdateCPUData ) {
        UInt iCubeCount = ( pDstTexture->m_iArrayCount / 6 );
        GPUTextureIndex hIndex;
        for( hIndex.iArrayIndex = 0; hIndex.iArrayIndex < iCubeCount; ++(hIndex.iArrayIndex) ) {
            for( UInt iFace = 0; iFace < 6; ++iFace ) {
                GPUTextureCubeFace iCubeFace = (GPUTextureCubeFace)( GPUTEXTURE_CUBEFACE_X_POS + iFace );
                for( hIndex.iMipIndex = 0; hIndex.iMipIndex < pDstTexture->m_iMipLevelCount; ++(hIndex.iMipIndex) )
                    pDstTexture->UpdateBack( &hIndex, iCubeFace, NULL, pContext );
            }
        }
    }
}
Void GPUTextureCube::CopyGPUData( GPUTextureCube * pDstTexture, const GPUTextureIndex * pDstIndex, GPUTextureCubeFace iDstFace, const GPUPoint2 * pDstPoint,
                                  const GPUTextureIndex * pSrcIndex, GPUTextureCubeFace iSrcFace, const GPURectangle * pSrcRect, Bool bUpdateCPUData, GPUDeferredContext * pContext ) const
{
    Assert( IsBound() && pDstTexture->IsBound() );
    Assert( !(IsLocked()) && !(pDstTexture->IsLocked()) );
    Assert( pDstTexture->CanGPUWrite() );

    Assert( m_iFormat == pDstTexture->m_iFormat );

    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    UInt iSrcWidth = _GetSubResourceBound( 0, pSrcIndex );
    UInt iSrcHeight = _GetSubResourceBound( 1, pSrcIndex );
    UInt iDstWidth = pDstTexture->_GetSubResourceBound( 0, pDstIndex );
    UInt iDstHeight = pDstTexture->_GetSubResourceBound( 1, pDstIndex );
    GPUPoint2 hDstPoint;
    GPURectangle hSrcRect;
    _ValidateCopy( &hDstPoint, &hSrcRect, iDstWidth, iDstHeight, iSrcWidth, iSrcHeight, pDstPoint, pSrcRect );

    m_hDeviceTextureCube.Copy( &(pDstTexture->m_hDeviceTextureCube), (const DeviceSubResourceIndex *)pDstIndex, (DeviceTextureCubeFace)iDstFace, (const DevicePoint2 *)&hDstPoint,
                               (const DeviceSubResourceIndex *)pSrcIndex, (DeviceTextureCubeFace)iSrcFace, (const DeviceRectangle *)&hSrcRect, pDeviceContext );

    if ( bUpdateCPUData ) {
        GPURectangle hDstRect;
        hDstRect.iLeft = (pDstPoint != NULL) ? pDstPoint->iX : 0;
        hDstRect.iRight = hDstRect.iLeft + ( hSrcRect.iRight - hSrcRect.iLeft );
        hDstRect.iTop = (pDstPoint != NULL) ? pDstPoint->iY : 0;
        hDstRect.iBottom = hDstRect.iTop + ( hSrcRect.iBottom - hSrcRect.iTop );
        pDstTexture->UpdateBack( pDstIndex, iDstFace, &hDstRect, pContext );
    }
}

/////////////////////////////////////////////////////////////////////////////////

Void GPUTextureCube::_ValidateCopy( GPUPoint2 * outDstPoint, GPURectangle * outSrcRect, UInt iDstWidth, UInt iDstHeight, UInt iSrcWidth, UInt iSrcHeight,
                                    const GPUPoint2 * pDstPoint, const GPURectangle * pSrcRect )
{
    outSrcRect->iLeft = 0;
    outSrcRect->iRight = iSrcWidth;
    outSrcRect->iTop = 0;
    outSrcRect->iBottom = iSrcHeight;
    if ( pSrcRect != NULL ) {
        Assert( pSrcRect->iLeft < pSrcRect->iRight && pSrcRect->iRight <= (Long)iSrcWidth );
        Assert( pSrcRect->iTop < pSrcRect->iBottom && pSrcRect->iBottom <= (Long)iSrcHeight );
        outSrcRect->iLeft = pSrcRect->iLeft;
        outSrcRect->iRight = pSrcRect->iRight;
        outSrcRect->iTop = pSrcRect->iTop;
        outSrcRect->iBottom = pSrcRect->iBottom;
    }

    outDstPoint->iX = 0;
    outDstPoint->iY = 0;
    if ( pDstPoint != NULL ) {
        Assert( pDstPoint->iX < iDstWidth );
        Assert( pDstPoint->iY < iDstHeight );
        outDstPoint->iX = pDstPoint->iX;
        outDstPoint->iY = pDstPoint->iY;
    }
    Assert( outDstPoint->iX + (outSrcRect->iRight - outSrcRect->iLeft) <= iDstWidth );
    Assert( outDstPoint->iY + (outSrcRect->iBottom - outSrcRect->iTop) <= iDstHeight );
}
Void GPUTextureCube::_CopyTexels( Byte * pDst, UInt iDstWidth, UInt iDstHeight, UInt iDstPitch, const Byte * pSrc, UInt iSrcWidth, UInt iSrcHeight, UInt iSrcPitch, UInt iStride,
                                  const GPUPoint2 * pDstPoint, const GPURectangle * pSrcRect )
{
    GPUPoint2 hDstPoint;
    GPURectangle hSrcRect;
    _ValidateCopy( &hDstPoint, &hSrcRect, iDstWidth, iDstHeight, iSrcWidth, iSrcHeight, pDstPoint, pSrcRect );

    pDst += ( (hDstPoint.iY * iDstPitch) + (hDstPoint.iX * iStride) );
    pSrc += ( (hSrcRect.iTop * iSrcPitch) + (hSrcRect.iLeft * iStride) );
    UInt iCopySize = ( (hSrcRect.iRight - hSrcRect.iLeft) * iStride );

    for( Long iY = hSrcRect.iTop; iY < hSrcRect.iBottom; ++iY ) {
        MemCopy( pDst, pSrc, iCopySize );
        pDst += iDstPitch;
        pSrc += iSrcPitch;
    }
}

Bool GPUTextureCube::_OnLoad( ResourceDB * pRDB, RDB_ResourceHandle * hResource )
{
    Assert( !IsBound() );

    TextureHeader hTextureHeader;
    pRDB->ResourceRead( hResource, sizeof(TextureHeader), (Byte*)( &hTextureHeader ) );

    Assert( hTextureHeader.dwType == GPURESOURCE_TEXTURE_CUBE );
    Assert( hTextureHeader.dwSampleCount == 1 );

    m_iFormat = (PixelFormat)( hTextureHeader.dwFormat );
    m_iStride = ( hTextureHeader.dwSampleCount * PixelFormatBytes(m_iFormat) );

    Assert( m_iDimensions == 2 );

    m_iMipLevelCount = hTextureHeader.dwMipLevelCount;

    Assert( hTextureHeader.dwBounds[0] > 0 );
    Assert( hTextureHeader.dwBounds[0] == hTextureHeader.dwBounds[1] );
    if ( m_iMipLevelCount > 1 ) {
        Assert( ( hTextureHeader.dwBounds[0] & (hTextureHeader.dwBounds[0] - 1) ) == 0 );
    }
    m_arrMipLevelBounds[0][0] = hTextureHeader.dwBounds[0];
    m_arrMipLevelBounds[0][1] = hTextureHeader.dwBounds[1];
    m_arrMipLevelBounds[0][2] = 0;

    m_iArrayCount = 1;

    _ComputeTextureLayout();
    Assert( m_iSize == hTextureHeader.dwDataSize );

    RenderingFn->SelectMemory( TEXT("Textures") );

    if ( m_arrTexture != NULL ) {
        DeleteA( m_arrTexture );
        m_arrTexture = NULL;
    }
    m_arrTexture = New Byte[m_iSize];

    RenderingFn->UnSelectMemory();

    pRDB->ResourceRead( hResource, m_iSize, m_arrTexture );

    return true;
}
Bool GPUTextureCube::_OnSave( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) const
{
    Assert( HasCPUData() );
    Assert( m_iArrayCount == 1 );

    TextureHeader hTextureHeader;
    hTextureHeader.dwDataSize = m_iSize;
    hTextureHeader.dwType = GPURESOURCE_TEXTURE_CUBE;
    hTextureHeader.dwFormat = m_iFormat;
    hTextureHeader.dwSampleCount = 1;
    hTextureHeader.dwMipLevelCount = m_iMipLevelCount;
    hTextureHeader.dwBounds[0] = m_arrMipLevelBounds[0][0];
    hTextureHeader.dwBounds[1] = m_arrMipLevelBounds[0][1];
    hTextureHeader.dwBounds[2] = 0;

    pRDB->ResourceAppend( hResource, sizeof(TextureHeader), (const Byte *)( &hTextureHeader ) );

    pRDB->ResourceAppend( hResource, m_iSize, m_arrTexture );

    return true;
}



