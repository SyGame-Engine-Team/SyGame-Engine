/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/Resources/D3D11Texture.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU Resources : Textures.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Third-Party Includes
#pragma warning(disable:4005)

#define WIN32_LEAN_AND_MEAN
#include <d3d11.h>

#undef DebugAssert

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "D3D11Texture.h"

#include "../D3D11Renderer.h"

/////////////////////////////////////////////////////////////////////////////////
// D3D11Texture implementation
D3D11Texture::D3D11Texture( D3D11Renderer * pRenderer ):
    D3D11Resource()
{
    m_pRenderer = pRenderer;

    m_bTemporaryDestroyed = false;
    m_hCreationParameters.pData = NULL;
}
D3D11Texture::~D3D11Texture()
{
    // nothing to do
}

Void D3D11Texture::Destroy()
{
    DebugAssert( IsCreated() );
    DebugAssert( !m_bLocked );

    if ( m_bTemporaryDestroyed )
        m_bTemporaryDestroyed = false;
    else
        _NakedDestroy();

    m_iBinds = D3D11RESOURCE_BIND_NONE;

    m_hCreationParameters.pData = NULL;
}

Void D3D11Texture::OnDestroyDevice()
{
    DebugAssert( !m_bTemporaryDestroyed );

    if ( m_pResource != NULL ) {
        _NakedDestroy();
        m_bTemporaryDestroyed = true;
    }
}
Void D3D11Texture::OnRestoreDevice()
{
    DebugAssert( m_pResource == NULL );

    if ( m_bTemporaryDestroyed ) {
        _NakedCreate();
        m_bTemporaryDestroyed = false;
    }
}

Float D3D11Texture::GetMinLOD( D3D11DeferredContext * pContext ) const
{
    DebugAssert( IsCreated() );

    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    return pDeviceContext->GetResourceMinLOD( m_pResource );
}
Void D3D11Texture::SetMinLOD( Float fMinLOD, D3D11DeferredContext * pContext ) const
{
    DebugAssert( IsCreated() );

    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->SetResourceMinLOD( m_pResource, fMinLOD );
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11Texture1D implementation
D3D11Texture1D::D3D11Texture1D( D3D11Renderer * pRenderer ):
    D3D11Texture( pRenderer )
{
    m_pTexture = NULL;

    m_hTextureDesc.MiscFlags = 0;
}
D3D11Texture1D::~D3D11Texture1D()
{
    if ( IsCreated() )
        Destroy();
}

Void D3D11Texture1D::Create( UInt iResourceBinds, PixelFormat iFormat, UInt iWidth, UInt iMipLevelCount, UInt iArrayCount, const Void * pData )
{
    DebugAssert( !(IsCreated()) );
    DebugAssert( m_iUsage != D3D11RESOURCE_USAGE_CONST || pData != NULL );
    
    m_iBinds = iResourceBinds;

    m_hTextureDesc.Usage = D3D11ResourceUsageToD3D11[m_iUsage];
    m_hTextureDesc.CPUAccessFlags = _GetCPUAccessFlags();
    m_hTextureDesc.BindFlags = _D3D11ConvertFlags32( D3D11ResourceBindToD3D11, m_iBinds );
    m_hTextureDesc.Format = PixelFormatToDXGI[iFormat];
    m_hTextureDesc.Width = iWidth;
    m_hTextureDesc.MipLevels = iMipLevelCount;
    m_hTextureDesc.ArraySize = iArrayCount;

    m_hCreationParameters.pData = pData;
    
    _NakedCreate();
}

Void * D3D11Texture1D::Lock( const D3D11SubResourceIndex * pIndex, D3D11ResourceLock iLockType, UInt iResourceLockFlags, D3D11DeferredContext * pContext )
{
    DebugAssert( IsCreated() );
    DebugAssert( CanLock() );
    DebugAssert( !m_bLocked );

    if ( iLockType == D3D11RESOURCE_LOCK_WRITE_DISCARD ) {
        DebugAssert( m_iUsage == D3D11RESOURCE_USAGE_DYNAMIC );
    } else if ( iLockType == D3D11RESOURCE_LOCK_WRITE_NO_OVERWRITE ) {
        DebugAssert( m_iUsage == D3D11RESOURCE_USAGE_DYNAMIC );
        DebugAssert( (m_iBinds & D3D11RESOURCE_BIND_SHADER_INPUT) == 0 );
    } else {
        if ( iLockType & D3D11RESOURCE_LOCK_READ ) {
            DebugAssert( CanCPURead() );
        } else if ( iLockType & D3D11RESOURCE_LOCK_WRITE ) {
            DebugAssert( CanCPUWrite() );
        }
    }

    UInt iSubResource = _GetSubResourceIndex( pIndex, m_hTextureDesc.MipLevels, m_hTextureDesc.ArraySize );

    D3D11_MAPPED_SUBRESOURCE hMappedSubResource;
    hMappedSubResource.pData = NULL;
    hMappedSubResource.RowPitch = 0;
    hMappedSubResource.DepthPitch = 0;

    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    HRESULT hRes = pDeviceContext->Map( m_pTexture, iSubResource, D3D11ResourceLockToD3D11[iLockType], _D3D11ConvertFlags32(D3D11ResourceLockFlagsToD3D11, iResourceLockFlags), &hMappedSubResource );
    DebugAssert( hRes == S_OK && hMappedSubResource.pData != NULL );

    m_bLocked = true;

    return hMappedSubResource.pData;
}
Void D3D11Texture1D::UnLock( const D3D11SubResourceIndex * pIndex, D3D11DeferredContext * pContext )
{
    DebugAssert( IsCreated() );
    DebugAssert( CanLock() );
    DebugAssert( m_bLocked );

    UInt iSubResource = _GetSubResourceIndex( pIndex, m_hTextureDesc.MipLevels, m_hTextureDesc.ArraySize );

    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->Unmap( m_pTexture, iSubResource );

    m_bLocked = false;
}

Void D3D11Texture1D::Update( const D3D11SubResourceIndex * pIndex, UInt iX, UInt iWidth, const Void * pSrcData, D3D11DeferredContext * pContext ) const
{
    DebugAssert( IsCreated() );
    DebugAssert( CanUpdate() );
    DebugAssert( !m_bLocked );

    UInt iSubResource = _GetSubResourceIndex( pIndex, m_hTextureDesc.MipLevels, m_hTextureDesc.ArraySize );
    DebugAssert( iX + iWidth <= _GetBound(m_hTextureDesc.Width, pIndex->iMipSlice) );

    D3D11_BOX hBox;
    hBox.left = iX;
    hBox.right = iX + iWidth;
    hBox.top = 0;
    hBox.bottom = 1;
    hBox.front = 0;
    hBox.back = 1;

    UInt iStride = _GetStride( m_hTextureDesc.Format );
    UInt iPitch = iStride * _GetBound( m_hTextureDesc.Width, pIndex->iMipSlice );
    
    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->UpdateSubresource( m_pTexture, iSubResource, &hBox, pSrcData, iPitch, 0 );
}

Void D3D11Texture1D::Copy( D3D11Texture1D * pDstTexture, D3D11DeferredContext * pContext ) const
{
    DebugAssert( IsCreated() && pDstTexture->IsCreated() );
    DebugAssert( !m_bLocked && !(pDstTexture->m_bLocked) );
    DebugAssert( pDstTexture->CanGPUWrite() );

    DebugAssert( pDstTexture != this );

    DebugAssert( m_hTextureDesc.Format == pDstTexture->m_hTextureDesc.Format );
    DebugAssert( m_hTextureDesc.Width == pDstTexture->m_hTextureDesc.Width );
    DebugAssert( m_hTextureDesc.MipLevels == pDstTexture->m_hTextureDesc.MipLevels );
    DebugAssert( m_hTextureDesc.ArraySize == pDstTexture->m_hTextureDesc.ArraySize );

    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->CopyResource( pDstTexture->m_pTexture, m_pTexture );
}
Void D3D11Texture1D::Copy( D3D11Texture1D * pDstTexture, const D3D11SubResourceIndex * pDstIndex, UInt iDstX, const D3D11SubResourceIndex * pSrcIndex, UInt iSrcX, UInt iSrcWidth, D3D11DeferredContext * pContext ) const
{
    DebugAssert( IsCreated() && pDstTexture->IsCreated() );
    DebugAssert( !m_bLocked && !(pDstTexture->m_bLocked) );
    DebugAssert( pDstTexture->CanGPUWrite() );

    DebugAssert( m_hTextureDesc.Format == pDstTexture->m_hTextureDesc.Format );

    UInt iSrcSubResource = _GetSubResourceIndex( pSrcIndex, m_hTextureDesc.MipLevels, m_hTextureDesc.ArraySize );
    DebugAssert( iSrcX + iSrcWidth <= _GetBound(m_hTextureDesc.Width,pSrcIndex->iMipSlice) );

    UInt iDstSubResource = _GetSubResourceIndex( pDstIndex, pDstTexture->m_hTextureDesc.MipLevels, pDstTexture->m_hTextureDesc.ArraySize );
    DebugAssert( iDstX + iSrcWidth <= _GetBound(pDstTexture->m_hTextureDesc.Width,pDstIndex->iMipSlice) );

    D3D11_BOX hSrcBox;
    hSrcBox.left = iSrcX;
    hSrcBox.right = iSrcX + iSrcWidth;
    hSrcBox.top = 0;
    hSrcBox.bottom = 1;
    hSrcBox.front = 0;
    hSrcBox.back = 1;

    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->CopySubresourceRegion( pDstTexture->m_pTexture, iDstSubResource, iDstX, 0, 0, m_pTexture, iSrcSubResource, &hSrcBox );
}

/////////////////////////////////////////////////////////////////////////////////

Void D3D11Texture1D::_NakedCreate()
{
    D3D11_SUBRESOURCE_DATA hInitialization;
    hInitialization.pSysMem = m_hCreationParameters.pData;
    hInitialization.SysMemPitch = 0;
    hInitialization.SysMemSlicePitch = 0;

    m_pTexture = NULL;
    HRESULT hRes = m_pRenderer->m_pDevice->CreateTexture1D( (const D3D11_TEXTURE1D_DESC *)&m_hTextureDesc, (m_hCreationParameters.pData != NULL) ? &hInitialization : NULL, &m_pTexture );
    DebugAssert( hRes == S_OK && m_pTexture != NULL );

    m_pResource = NULL;
    hRes = m_pTexture->QueryInterface( __uuidof(ID3D11Resource), (Void**)&m_pResource );
    DebugAssert( hRes == S_OK && m_pResource != NULL );
}
Void D3D11Texture1D::_NakedDestroy()
{
    m_pResource->Release();
    m_pResource = NULL;

    m_pTexture->Release();
    m_pTexture = NULL;
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11Texture2D implementation
D3D11Texture2D::D3D11Texture2D( D3D11Renderer * pRenderer, _BackBufferResizeCallback pfCallback, Void * pUserData ):
    D3D11Texture( pRenderer )
{
    m_pTexture = NULL;

    m_hTextureDesc.SampleDesc.Count = 1;
    m_hTextureDesc.SampleDesc.Quality = 0;
    m_hTextureDesc.MiscFlags = 0;

    m_iBoundToBackBuffer = INVALID_OFFSET;
    m_pfBackBufferResizeCallback = pfCallback;
    m_pBackBufferResizeUserData = pUserData;
}
D3D11Texture2D::~D3D11Texture2D()
{
    if ( IsCreated() )
        Destroy();
}

Void D3D11Texture2D::AttachToBackBuffer( UInt iBackBuffer )
{
    DebugAssert( !(IsCreated()) );
    DebugAssert( iBackBuffer < m_pRenderer->m_hSwapChainDesc.iBufferCount );

    m_hCreationParameters.pData = NULL;

    m_iBoundToBackBuffer = iBackBuffer;
    _NakedCreate();
}
Void D3D11Texture2D::Create( UInt iResourceBinds, PixelFormat iFormat, UInt iWidth, UInt iHeight, UInt iMipLevelCount, UInt iArrayCount, const Void * pData )
{
    DebugAssert( !(IsCreated()) );
    DebugAssert( m_iUsage != D3D11RESOURCE_USAGE_CONST || pData != NULL );

    m_iBinds = iResourceBinds;

    m_hTextureDesc.Usage = D3D11ResourceUsageToD3D11[m_iUsage];
    m_hTextureDesc.CPUAccessFlags = _GetCPUAccessFlags();
    m_hTextureDesc.BindFlags = _D3D11ConvertFlags32( D3D11ResourceBindToD3D11, m_iBinds );
    m_hTextureDesc.Format = PixelFormatToDXGI[iFormat];
    m_hTextureDesc.Width = iWidth;
    m_hTextureDesc.Height = iHeight;
    m_hTextureDesc.MipLevels = iMipLevelCount;
    m_hTextureDesc.ArraySize = iArrayCount;
    
    m_hCreationParameters.pData = pData;

    m_iBoundToBackBuffer = INVALID_OFFSET;
    _NakedCreate();
}
Void D3D11Texture2D::Destroy()
{
    D3D11Texture::Destroy();

    m_hTextureDesc.SampleDesc.Count = 1;
    m_hTextureDesc.SampleDesc.Quality = 0;

    m_iBoundToBackBuffer = INVALID_OFFSET;
}

Void D3D11Texture2D::SetSampleCount( UInt iSampleCount )
{
    DebugAssert( !(IsCreated()) );
    DebugAssert( iSampleCount == 1 || iSampleCount == 2 || iSampleCount == 4 || iSampleCount == 8 || iSampleCount == 16 );

    m_hTextureDesc.SampleDesc.Count = 1;
    m_hTextureDesc.SampleDesc.Quality = 0;
    if ( iSampleCount > 1 ) {
        UInt iFmtSupport = 0;
        HRESULT hRes = m_pRenderer->m_pDevice->CheckFormatSupport( m_hTextureDesc.Format, &iFmtSupport );
        DebugAssert( iFmtSupport & D3D11_FORMAT_SUPPORT_MULTISAMPLE_LOAD );

        UInt iMaxSamplingQuality = 0;
        hRes = m_pRenderer->m_pDevice->CheckMultisampleQualityLevels( m_hTextureDesc.Format, iSampleCount, &iMaxSamplingQuality );
        DebugAssert( hRes == S_OK );
        DebugAssert( iMaxSamplingQuality > 0 );

        m_hTextureDesc.SampleDesc.Count = iSampleCount;
        m_hTextureDesc.SampleDesc.Quality = (UINT)D3D11_STANDARD_MULTISAMPLE_PATTERN;
    }
}

Void * D3D11Texture2D::Lock( const D3D11SubResourceIndex * pIndex, D3D11ResourceLock iLockType, UInt iResourceLockFlags, UInt * outPitch, D3D11DeferredContext * pContext )
{
    DebugAssert( IsCreated() );
    DebugAssert( CanLock() );
    DebugAssert( !m_bLocked );

    if ( iLockType == D3D11RESOURCE_LOCK_WRITE_DISCARD ) {
        DebugAssert( m_iUsage == D3D11RESOURCE_USAGE_DYNAMIC );
    } else if ( iLockType == D3D11RESOURCE_LOCK_WRITE_NO_OVERWRITE ) {
        DebugAssert( m_iUsage == D3D11RESOURCE_USAGE_DYNAMIC );
        DebugAssert( (m_iBinds & D3D11RESOURCE_BIND_SHADER_INPUT) == 0 );
    } else {
        if ( iLockType & D3D11RESOURCE_LOCK_READ ) {
            DebugAssert( CanCPURead() );
        } else if ( iLockType & D3D11RESOURCE_LOCK_WRITE ) {
            DebugAssert( CanCPUWrite() );
        }
    }

    UInt iSubResource = _GetSubResourceIndex( pIndex, m_hTextureDesc.MipLevels, m_hTextureDesc.ArraySize );

    D3D11_MAPPED_SUBRESOURCE hMappedSubResource;
    hMappedSubResource.pData = NULL;
    hMappedSubResource.RowPitch = 0;
    hMappedSubResource.DepthPitch = 0;

    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    HRESULT hRes = pDeviceContext->Map( m_pTexture, iSubResource, D3D11ResourceLockToD3D11[iLockType], _D3D11ConvertFlags32(D3D11ResourceLockFlagsToD3D11, iResourceLockFlags), &hMappedSubResource );
    DebugAssert( hRes == S_OK && hMappedSubResource.pData != NULL );
    
    *outPitch = hMappedSubResource.RowPitch;

    m_bLocked = true;

    return hMappedSubResource.pData;
}
Void D3D11Texture2D::UnLock( const D3D11SubResourceIndex * pIndex, D3D11DeferredContext * pContext )
{
    DebugAssert( IsCreated() );
    DebugAssert( CanLock() );
    DebugAssert( m_bLocked );

    UInt iSubResource = _GetSubResourceIndex( pIndex, m_hTextureDesc.MipLevels, m_hTextureDesc.ArraySize );

    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->Unmap( m_pTexture, iSubResource );

    m_bLocked = false;
}

Void D3D11Texture2D::Update( const D3D11SubResourceIndex * pIndex, const D3D11Rectangle * pRect, const Void * pSrcData, D3D11DeferredContext * pContext ) const
{
    DebugAssert( IsCreated() );
    DebugAssert( CanUpdate() );
    DebugAssert( !m_bLocked );

    UInt iSubResource = _GetSubResourceIndex( pIndex, m_hTextureDesc.MipLevels, m_hTextureDesc.ArraySize );
    DebugAssert( pRect->iLeft >= 0 && pRect->iLeft < pRect->iRight && pRect->iRight <= (signed)_GetBound(m_hTextureDesc.Width,pIndex->iMipSlice) );
    DebugAssert( pRect->iTop >= 0 && pRect->iTop < pRect->iBottom && pRect->iBottom <= (signed)_GetBound(m_hTextureDesc.Height,pIndex->iMipSlice) );

    D3D11_BOX hBox;
    hBox.left = pRect->iLeft;
    hBox.right = pRect->iRight;
    hBox.top = pRect->iTop;
    hBox.bottom = pRect->iBottom;
    hBox.front = 0;
    hBox.back = 1;

    UInt iStride = _GetStride( m_hTextureDesc.Format );
    UInt iPitch = iStride * _GetBound( m_hTextureDesc.Width, pIndex->iMipSlice );
    UInt iSlice = iPitch * _GetBound( m_hTextureDesc.Height, pIndex->iMipSlice );

    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->UpdateSubresource( m_pTexture, iSubResource, &hBox, pSrcData, iPitch, iSlice );
}

Void D3D11Texture2D::Copy( D3D11Texture2D * pDstTexture, D3D11DeferredContext * pContext ) const
{
    DebugAssert( IsCreated() && pDstTexture->IsCreated() );
    DebugAssert( !m_bLocked && !(pDstTexture->m_bLocked) );
    DebugAssert( pDstTexture->CanGPUWrite() );

    DebugAssert( pDstTexture != this );

    DebugAssert( pDstTexture->m_hTextureDesc.Format == m_hTextureDesc.Format );
    DebugAssert( pDstTexture->m_hTextureDesc.SampleDesc.Count == m_hTextureDesc.SampleDesc.Count );
    DebugAssert( pDstTexture->m_hTextureDesc.Width == m_hTextureDesc.Width );
    DebugAssert( pDstTexture->m_hTextureDesc.Height == m_hTextureDesc.Height );
    DebugAssert( pDstTexture->m_hTextureDesc.MipLevels == m_hTextureDesc.MipLevels );
    DebugAssert( pDstTexture->m_hTextureDesc.ArraySize == m_hTextureDesc.ArraySize );
    
    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->CopyResource( pDstTexture->m_pTexture, m_pTexture );
}
Void D3D11Texture2D::Copy( D3D11Texture2D * pDstTexture, const D3D11SubResourceIndex * pDstIndex, const D3D11Point2 * pDstPoint, const D3D11SubResourceIndex * pSrcIndex, const D3D11Rectangle * pSrcRect,
                           D3D11DeferredContext * pContext ) const
{
    DebugAssert( IsCreated() && pDstTexture->IsCreated() );
    DebugAssert( !m_bLocked && !(pDstTexture->m_bLocked) );
    DebugAssert( pDstTexture->CanGPUWrite() );
    
    DebugAssert( pDstTexture->m_hTextureDesc.Format == m_hTextureDesc.Format );
    DebugAssert( pDstTexture->m_hTextureDesc.SampleDesc.Count == m_hTextureDesc.SampleDesc.Count );

    UInt iSrcSubResource = _GetSubResourceIndex( pSrcIndex, m_hTextureDesc.MipLevels, m_hTextureDesc.ArraySize );
    DebugAssert( pSrcRect->iLeft >= 0 && pSrcRect->iLeft < pSrcRect->iRight && pSrcRect->iRight <= (signed)_GetBound(m_hTextureDesc.Width,pSrcIndex->iMipSlice) );
    DebugAssert( pSrcRect->iTop >= 0 && pSrcRect->iTop < pSrcRect->iBottom && pSrcRect->iBottom <= (signed)_GetBound(m_hTextureDesc.Height,pSrcIndex->iMipSlice) );

    UInt iDstSubResource = _GetSubResourceIndex( pDstIndex, pDstTexture->m_hTextureDesc.MipLevels, pDstTexture->m_hTextureDesc.ArraySize );
    DebugAssert( pDstPoint->iX + (pSrcRect->iRight - pSrcRect->iLeft) <= _GetBound(pDstTexture->m_hTextureDesc.Width,pDstIndex->iMipSlice) );
    DebugAssert( pDstPoint->iY + (pSrcRect->iBottom - pSrcRect->iTop) <= _GetBound(pDstTexture->m_hTextureDesc.Height,pDstIndex->iMipSlice) );

    D3D11_BOX hSrcBox;
    hSrcBox.left = pSrcRect->iLeft;
    hSrcBox.right = pSrcRect->iRight;
    hSrcBox.top = pSrcRect->iTop;
    hSrcBox.bottom = pSrcRect->iBottom;
    hSrcBox.front = 0;
    hSrcBox.back = 1;
    
    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->CopySubresourceRegion( pDstTexture->m_pTexture, iDstSubResource, pDstPoint->iX, pDstPoint->iY, 0, m_pTexture, iSrcSubResource, &hSrcBox );
}

/////////////////////////////////////////////////////////////////////////////////

Void D3D11Texture2D::_NakedCreate()
{
    HRESULT hRes;

    if ( m_iBoundToBackBuffer == INVALID_OFFSET ) {
        D3D11_SUBRESOURCE_DATA hInitialization;
        hInitialization.pSysMem = m_hCreationParameters.pData;
        hInitialization.SysMemPitch = m_hTextureDesc.Width * PixelFormatBytes( PixelFormatFromDXGI[m_hTextureDesc.Format] );
        hInitialization.SysMemSlicePitch = 0;

        m_pTexture = NULL;
        hRes = m_pRenderer->m_pDevice->CreateTexture2D( (const D3D11_TEXTURE2D_DESC *)&m_hTextureDesc, (m_hCreationParameters.pData != NULL) ? &hInitialization : NULL, &m_pTexture );
        DebugAssert( hRes == S_OK && m_pTexture != NULL );
    } else {
        m_pTexture = NULL;
        hRes = m_pRenderer->m_pSwapChain->GetBuffer( m_iBoundToBackBuffer, __uuidof(ID3D11Texture2D), (Void**)&m_pTexture );
        DebugAssert( hRes == S_OK && m_pTexture != NULL );

        m_pTexture->GetDesc( (D3D11_TEXTURE2D_DESC*)&m_hTextureDesc );

        m_iUsage = D3D11ResourceUsageFromD3D11[m_hTextureDesc.Usage];
        m_iBinds = _D3D11ConvertFlags32( D3D11ResourceBindFromD3D11, m_hTextureDesc.BindFlags );

        // Give owner a chance to resize accordingly
        m_pfBackBufferResizeCallback( PixelFormatFromDXGI[m_hTextureDesc.Format], m_hTextureDesc.Width, m_hTextureDesc.Height, m_hTextureDesc.SampleDesc.Count, m_pBackBufferResizeUserData );
    }

    m_pResource = NULL;
    hRes = m_pTexture->QueryInterface( __uuidof(ID3D11Resource), (Void**)&m_pResource );
    DebugAssert( hRes == S_OK && m_pResource != NULL );
}
Void D3D11Texture2D::_NakedDestroy()
{
    m_pResource->Release();
    m_pResource = NULL;

    m_pTexture->Release();
    m_pTexture = NULL;
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11Texture3D implementation
D3D11Texture3D::D3D11Texture3D( D3D11Renderer * pRenderer ):
    D3D11Texture( pRenderer )
{
    m_pTexture = NULL;

    m_hTextureDesc.MiscFlags = 0;
}
D3D11Texture3D::~D3D11Texture3D()
{
    if ( IsCreated() )
        Destroy();
}

Void D3D11Texture3D::Create( UInt iResourceBinds, PixelFormat iFormat, UInt iWidth, UInt iHeight, UInt iDepth, UInt iMipLevelCount, const Void * pData )
{
    DebugAssert( !(IsCreated()) );
    DebugAssert( m_iUsage != D3D11RESOURCE_USAGE_CONST || pData != NULL );

    m_iBinds = iResourceBinds;

    m_hTextureDesc.Usage = D3D11ResourceUsageToD3D11[m_iUsage];
    m_hTextureDesc.CPUAccessFlags = _GetCPUAccessFlags();
    m_hTextureDesc.BindFlags = _D3D11ConvertFlags32( D3D11ResourceBindToD3D11, m_iBinds );
    m_hTextureDesc.Format = PixelFormatToDXGI[iFormat];
    m_hTextureDesc.Width = iWidth;
    m_hTextureDesc.Height = iHeight;
    m_hTextureDesc.Depth = iDepth;
    m_hTextureDesc.MipLevels = iMipLevelCount;

    m_hCreationParameters.pData = pData;

    _NakedCreate();
}

Void * D3D11Texture3D::Lock( const D3D11SubResourceIndex * pIndex, D3D11ResourceLock iLockType, UInt iResourceLockFlags, UInt * outPitch, UInt * outSlice, D3D11DeferredContext * pContext )
{
    DebugAssert( IsCreated() );
    DebugAssert( CanLock() );
    DebugAssert( !m_bLocked );

    if ( iLockType == D3D11RESOURCE_LOCK_WRITE_DISCARD ) {
        DebugAssert( m_iUsage == D3D11RESOURCE_USAGE_DYNAMIC );
    } else if ( iLockType == D3D11RESOURCE_LOCK_WRITE_NO_OVERWRITE ) {
        DebugAssert( m_iUsage == D3D11RESOURCE_USAGE_DYNAMIC );
        DebugAssert( (m_iBinds & D3D11RESOURCE_BIND_SHADER_INPUT) == 0 );
    } else {
        if ( iLockType & D3D11RESOURCE_LOCK_READ ) {
            DebugAssert( CanCPURead() );
        } else if ( iLockType & D3D11RESOURCE_LOCK_WRITE ) {
            DebugAssert( CanCPUWrite() );
        }
    }

    UInt iSubResource = _GetSubResourceIndex( pIndex, m_hTextureDesc.MipLevels, 1 );

    D3D11_MAPPED_SUBRESOURCE hMappedSubResource;
    hMappedSubResource.pData = NULL;
    hMappedSubResource.RowPitch = 0;
    hMappedSubResource.DepthPitch = 0;

    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    HRESULT hRes = pDeviceContext->Map( m_pTexture, iSubResource, D3D11ResourceLockToD3D11[iLockType], _D3D11ConvertFlags32(D3D11ResourceLockFlagsToD3D11, iResourceLockFlags), &hMappedSubResource );
    DebugAssert( hRes == S_OK && hMappedSubResource.pData != NULL );
    
    *outPitch = hMappedSubResource.RowPitch;
    *outSlice = hMappedSubResource.DepthPitch;

    m_bLocked = true;

    return hMappedSubResource.pData;
}
Void D3D11Texture3D::UnLock( const D3D11SubResourceIndex * pIndex, D3D11DeferredContext * pContext )
{
    DebugAssert( IsCreated() );
    DebugAssert( CanLock() );
    DebugAssert( m_bLocked );

    UInt iSubResource = _GetSubResourceIndex( pIndex, m_hTextureDesc.MipLevels, 1 );

    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->Unmap( m_pTexture, iSubResource );

    m_bLocked = false;
}

Void D3D11Texture3D::Update( const D3D11SubResourceIndex * pIndex, const D3D11Box * pBox, const Void * pSrcData, D3D11DeferredContext * pContext ) const
{
    DebugAssert( IsCreated() );
    DebugAssert( CanUpdate() );
    DebugAssert( !m_bLocked );

    UInt iSubResource = _GetSubResourceIndex( pIndex, m_hTextureDesc.MipLevels, 1 );
    DebugAssert( pBox->iLeft < pBox->iRight && pBox->iRight <= _GetBound(m_hTextureDesc.Width,pIndex->iMipSlice) );
    DebugAssert( pBox->iTop < pBox->iBottom && pBox->iBottom <= _GetBound(m_hTextureDesc.Height,pIndex->iMipSlice) );
    DebugAssert( pBox->iFront < pBox->iBack && pBox->iBack <= _GetBound(m_hTextureDesc.Depth,pIndex->iMipSlice) );

    D3D11_BOX hBox;
    hBox.left = pBox->iLeft;
    hBox.right = pBox->iRight;
    hBox.top = pBox->iTop;
    hBox.bottom = pBox->iBottom;
    hBox.front = pBox->iFront;
    hBox.back = pBox->iBack;

    UInt iStride = _GetStride( m_hTextureDesc.Format );
    UInt iPitch = iStride * _GetBound( m_hTextureDesc.Width, pIndex->iMipSlice );
    UInt iSlice = iPitch * _GetBound( m_hTextureDesc.Height, pIndex->iMipSlice );

    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->UpdateSubresource( m_pTexture, iSubResource, &hBox, pSrcData, iPitch, iSlice );
}

Void D3D11Texture3D::Copy( D3D11Texture3D * pDstTexture, D3D11DeferredContext * pContext ) const
{
    DebugAssert( IsCreated() && pDstTexture->IsCreated() );
    DebugAssert( !m_bLocked && !(pDstTexture->m_bLocked) );
    DebugAssert( pDstTexture->CanGPUWrite() );

    DebugAssert( pDstTexture != this );

    DebugAssert( pDstTexture->m_hTextureDesc.Format == m_hTextureDesc.Format );
    DebugAssert( pDstTexture->m_hTextureDesc.Width == m_hTextureDesc.Width );
    DebugAssert( pDstTexture->m_hTextureDesc.Height == m_hTextureDesc.Height );
    DebugAssert( pDstTexture->m_hTextureDesc.Depth == m_hTextureDesc.Depth );
    DebugAssert( pDstTexture->m_hTextureDesc.MipLevels == m_hTextureDesc.MipLevels );
    
    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->CopyResource( pDstTexture->m_pTexture, m_pTexture );
}
Void D3D11Texture3D::Copy( D3D11Texture3D * pDstTexture, const D3D11SubResourceIndex * pDstIndex, const D3D11Point3 * pDstPoint, const D3D11SubResourceIndex * pSrcIndex, const D3D11Box * pSrcBox,
                           D3D11DeferredContext * pContext ) const
{
    DebugAssert( IsCreated() && pDstTexture->IsCreated() );
    DebugAssert( !m_bLocked && !(pDstTexture->m_bLocked) );
    DebugAssert( pDstTexture->CanGPUWrite() );

    DebugAssert( pDstTexture->m_hTextureDesc.Format == m_hTextureDesc.Format );

    UInt iSrcSubResource = _GetSubResourceIndex( pSrcIndex, m_hTextureDesc.MipLevels, 1 );
    DebugAssert( pSrcBox->iLeft < pSrcBox->iRight && pSrcBox->iRight <= _GetBound(m_hTextureDesc.Width,pSrcIndex->iMipSlice) );
    DebugAssert( pSrcBox->iTop < pSrcBox->iBottom && pSrcBox->iBottom <= _GetBound(m_hTextureDesc.Height,pSrcIndex->iMipSlice) );
    DebugAssert( pSrcBox->iFront < pSrcBox->iBack && pSrcBox->iBack <= _GetBound(m_hTextureDesc.Depth,pSrcIndex->iMipSlice) );

    UInt iDstSubResource = _GetSubResourceIndex( pDstIndex, pDstTexture->m_hTextureDesc.MipLevels, 1 );
    DebugAssert( pDstPoint->iX + (pSrcBox->iRight - pSrcBox->iLeft) <= _GetBound(pDstTexture->m_hTextureDesc.Width,pDstIndex->iMipSlice) );
    DebugAssert( pDstPoint->iY + (pSrcBox->iBottom - pSrcBox->iTop) <= _GetBound(pDstTexture->m_hTextureDesc.Height,pDstIndex->iMipSlice) );
    DebugAssert( pDstPoint->iZ + (pSrcBox->iBack - pSrcBox->iFront) <= _GetBound(pDstTexture->m_hTextureDesc.Depth,pDstIndex->iMipSlice) );

    D3D11_BOX hSrcBox;
    hSrcBox.left = pSrcBox->iLeft;
    hSrcBox.right = pSrcBox->iRight;
    hSrcBox.top = pSrcBox->iTop;
    hSrcBox.bottom = pSrcBox->iBottom;
    hSrcBox.front = pSrcBox->iFront;
    hSrcBox.back = pSrcBox->iBack;
    
    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->CopySubresourceRegion( pDstTexture->m_pTexture, iDstSubResource, pDstPoint->iX, pDstPoint->iY, pDstPoint->iZ, m_pTexture, iSrcSubResource, &hSrcBox );
}

/////////////////////////////////////////////////////////////////////////////////

Void D3D11Texture3D::_NakedCreate()
{
    D3D11_SUBRESOURCE_DATA hInitialization;
    hInitialization.pSysMem = m_hCreationParameters.pData;
    hInitialization.SysMemPitch = m_hTextureDesc.Width * PixelFormatBytes( PixelFormatFromDXGI[m_hTextureDesc.Format] );
    hInitialization.SysMemSlicePitch = m_hTextureDesc.Height * hInitialization.SysMemPitch;

    m_pTexture = NULL;
    HRESULT hRes = m_pRenderer->m_pDevice->CreateTexture3D( (const D3D11_TEXTURE3D_DESC *)&m_hTextureDesc, (m_hCreationParameters.pData != NULL) ? &hInitialization : NULL, &m_pTexture );
    DebugAssert( hRes == S_OK && m_pTexture != NULL );

    m_pResource = NULL;
    hRes = m_pTexture->QueryInterface( __uuidof(ID3D11Resource), (Void**)&m_pResource );
    DebugAssert( hRes == S_OK && m_pResource != NULL );
}
Void D3D11Texture3D::_NakedDestroy()
{
    m_pResource->Release();
    m_pResource = NULL;

    m_pTexture->Release();
    m_pTexture = NULL;
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11TextureCube implementation
D3D11TextureCube::D3D11TextureCube( D3D11Renderer * pRenderer ):
    D3D11Texture( pRenderer )
{
    m_pTexture = NULL;

    m_hTextureDesc.SampleDesc.Count = 1;
    m_hTextureDesc.SampleDesc.Quality = 0;
    m_hTextureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
}
D3D11TextureCube::~D3D11TextureCube()
{
    if ( IsCreated() )
        Destroy();
}

Void D3D11TextureCube::Create( UInt iResourceBinds, PixelFormat iFormat, UInt iEdgeSize, UInt iMipLevelCount, UInt iArrayCount, const Void * pData )
{
    DebugAssert( !(IsCreated()) );
    DebugAssert( m_iUsage != D3D11RESOURCE_USAGE_CONST || pData != NULL );

    m_iBinds = iResourceBinds;

    m_hTextureDesc.Usage = D3D11ResourceUsageToD3D11[m_iUsage];
    m_hTextureDesc.CPUAccessFlags = _GetCPUAccessFlags();
    m_hTextureDesc.BindFlags = _D3D11ConvertFlags32( D3D11ResourceBindToD3D11, m_iBinds );
    m_hTextureDesc.Format = PixelFormatToDXGI[iFormat];
    m_hTextureDesc.Width = iEdgeSize;
    m_hTextureDesc.Height = iEdgeSize;
    m_hTextureDesc.MipLevels = iMipLevelCount;
    m_hTextureDesc.ArraySize = iArrayCount;
    
    m_hCreationParameters.pData = pData;

    _NakedCreate();
}

Void * D3D11TextureCube::Lock( const D3D11SubResourceIndex * pIndex, D3D11TextureCubeFace iFace, D3D11ResourceLock iLockType, UInt iResourceLockFlags, UInt * outPitch,
                               D3D11DeferredContext * pContext )
{
    DebugAssert( IsCreated() );
    DebugAssert( CanLock() );
    DebugAssert( !m_bLocked );

    if ( iLockType == D3D11RESOURCE_LOCK_WRITE_DISCARD ) {
        DebugAssert( m_iUsage == D3D11RESOURCE_USAGE_DYNAMIC );
    } else if ( iLockType == D3D11RESOURCE_LOCK_WRITE_NO_OVERWRITE ) {
        DebugAssert( m_iUsage == D3D11RESOURCE_USAGE_DYNAMIC );
        DebugAssert( (m_iBinds & D3D11RESOURCE_BIND_SHADER_INPUT) == 0 );
    } else {
        if ( iLockType & D3D11RESOURCE_LOCK_READ ) {
            DebugAssert( CanCPURead() );
        } else if ( iLockType & D3D11RESOURCE_LOCK_WRITE ) {
            DebugAssert( CanCPUWrite() );
        }
    }

    UInt iSubResource = _GetSubResourceIndex( pIndex, iFace, m_hTextureDesc.MipLevels, m_hTextureDesc.ArraySize );

    D3D11_MAPPED_SUBRESOURCE hMappedSubResource;
    hMappedSubResource.pData = NULL;
    hMappedSubResource.RowPitch = 0;
    hMappedSubResource.DepthPitch = 0;

    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    HRESULT hRes = pDeviceContext->Map( m_pTexture, iSubResource, D3D11ResourceLockToD3D11[iLockType], _D3D11ConvertFlags32(D3D11ResourceLockFlagsToD3D11, iResourceLockFlags), &hMappedSubResource );
    DebugAssert( hRes == S_OK && hMappedSubResource.pData != NULL );
    
    *outPitch = hMappedSubResource.RowPitch;

    m_bLocked = true;

    return hMappedSubResource.pData;
}
Void D3D11TextureCube::UnLock( const D3D11SubResourceIndex * pIndex, D3D11TextureCubeFace iFace, D3D11DeferredContext * pContext )
{
    DebugAssert( IsCreated() );
    DebugAssert( CanLock() );
    DebugAssert( m_bLocked );

    UInt iSubResource = _GetSubResourceIndex( pIndex, iFace, m_hTextureDesc.MipLevels, m_hTextureDesc.ArraySize );

    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->Unmap( m_pTexture, iSubResource );

    m_bLocked = false;
}

Void D3D11TextureCube::Update( const D3D11SubResourceIndex * pIndex, D3D11TextureCubeFace iFace, const D3D11Rectangle * pRect, const Void * pSrcData, D3D11DeferredContext * pContext ) const
{
    DebugAssert( IsCreated() );
    DebugAssert( CanUpdate() );
    DebugAssert( !m_bLocked );

    UInt iSubResource = _GetSubResourceIndex( pIndex, iFace, m_hTextureDesc.MipLevels, m_hTextureDesc.ArraySize );
    DebugAssert( pRect->iLeft >= 0 && pRect->iLeft < pRect->iRight && pRect->iRight <= (signed)_GetBound(m_hTextureDesc.Width,pIndex->iMipSlice) );
    DebugAssert( pRect->iTop >= 0 && pRect->iTop < pRect->iBottom && pRect->iBottom <= (signed)_GetBound(m_hTextureDesc.Height,pIndex->iMipSlice) );

    D3D11_BOX hBox;
    hBox.left = pRect->iLeft;
    hBox.right = pRect->iRight;
    hBox.top = pRect->iTop;
    hBox.bottom = pRect->iBottom;
    hBox.front = 0;
    hBox.back = 1;

    UInt iStride = _GetStride( m_hTextureDesc.Format );
    UInt iPitch = iStride * _GetBound( m_hTextureDesc.Width, pIndex->iMipSlice );
    UInt iSlice = iPitch * _GetBound( m_hTextureDesc.Height, pIndex->iMipSlice );

    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->UpdateSubresource( m_pTexture, iSubResource, &hBox, pSrcData, iPitch, iSlice );
}

Void D3D11TextureCube::Copy( D3D11TextureCube * pDstTexture, D3D11DeferredContext * pContext ) const
{
    DebugAssert( IsCreated() && pDstTexture->IsCreated() );
    DebugAssert( !m_bLocked && !(pDstTexture->m_bLocked) );
    DebugAssert( pDstTexture->CanGPUWrite() );

    DebugAssert( pDstTexture != this );

    DebugAssert( pDstTexture->m_hTextureDesc.Format == m_hTextureDesc.Format );
    DebugAssert( pDstTexture->m_hTextureDesc.Width == m_hTextureDesc.Width );
    DebugAssert( pDstTexture->m_hTextureDesc.Height == m_hTextureDesc.Height );
    DebugAssert( pDstTexture->m_hTextureDesc.MipLevels == m_hTextureDesc.MipLevels );
    DebugAssert( pDstTexture->m_hTextureDesc.ArraySize == m_hTextureDesc.ArraySize );
    
    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->CopyResource( pDstTexture->m_pTexture, m_pTexture );
}
Void D3D11TextureCube::Copy( D3D11TextureCube * pDstTexture, const D3D11SubResourceIndex * pDstIndex, D3D11TextureCubeFace iDstFace, const D3D11Point2 * pDstPoint,
                             const D3D11SubResourceIndex * pSrcIndex, D3D11TextureCubeFace iSrcFace, const D3D11Rectangle * pSrcRect, D3D11DeferredContext * pContext ) const
{
    DebugAssert( IsCreated() && pDstTexture->IsCreated() );
    DebugAssert( !m_bLocked && !(pDstTexture->m_bLocked) );
    DebugAssert( pDstTexture->CanGPUWrite() );

    DebugAssert( pDstTexture->m_hTextureDesc.Format == m_hTextureDesc.Format );

    UInt iSrcSubResource = _GetSubResourceIndex( pSrcIndex, iSrcFace, m_hTextureDesc.MipLevels, m_hTextureDesc.ArraySize );
    DebugAssert( pSrcRect->iLeft >= 0 && pSrcRect->iLeft < pSrcRect->iRight && pSrcRect->iRight <= (signed)_GetBound(m_hTextureDesc.Width,pSrcIndex->iMipSlice) );
    DebugAssert( pSrcRect->iTop >= 0 && pSrcRect->iTop < pSrcRect->iBottom && pSrcRect->iBottom <= (signed)_GetBound(m_hTextureDesc.Height,pSrcIndex->iMipSlice) );

    UInt iDstSubResource = _GetSubResourceIndex( pDstIndex, iDstFace, pDstTexture->m_hTextureDesc.MipLevels, pDstTexture->m_hTextureDesc.ArraySize );
    DebugAssert( pDstPoint->iX + (pSrcRect->iRight - pSrcRect->iLeft) <= _GetBound(pDstTexture->m_hTextureDesc.Width,pDstIndex->iMipSlice) );
    DebugAssert( pDstPoint->iY + (pSrcRect->iBottom - pSrcRect->iTop) <= _GetBound(pDstTexture->m_hTextureDesc.Height,pDstIndex->iMipSlice) );

    D3D11_BOX hSrcBox;
    hSrcBox.left = pSrcRect->iLeft;
    hSrcBox.right = pSrcRect->iRight;
    hSrcBox.top = pSrcRect->iTop;
    hSrcBox.bottom = pSrcRect->iBottom;
    hSrcBox.front = 0;
    hSrcBox.back = 1;
    
    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->CopySubresourceRegion( pDstTexture->m_pTexture, iDstSubResource, pDstPoint->iX, pDstPoint->iY, 0, m_pTexture, iSrcSubResource, &hSrcBox );
}

/////////////////////////////////////////////////////////////////////////////////

Void D3D11TextureCube::_NakedCreate()
{
    D3D11_SUBRESOURCE_DATA hInitialization;
    hInitialization.pSysMem = m_hCreationParameters.pData;
    hInitialization.SysMemPitch = m_hTextureDesc.Width * PixelFormatBytes( PixelFormatFromDXGI[m_hTextureDesc.Format] );
    hInitialization.SysMemSlicePitch = 0;

    m_pTexture = NULL;
    HRESULT hRes = m_pRenderer->m_pDevice->CreateTexture2D( (const D3D11_TEXTURE2D_DESC *)&m_hTextureDesc, (m_hCreationParameters.pData != NULL) ? &hInitialization : NULL, &m_pTexture );
    DebugAssert( hRes == S_OK && m_pTexture != NULL );

    m_pResource = NULL;
    hRes = m_pTexture->QueryInterface( __uuidof(ID3D11Resource), (Void**)&m_pResource );
    DebugAssert( hRes == S_OK && m_pResource != NULL );
}
Void D3D11TextureCube::_NakedDestroy()
{
    m_pResource->Release();
    m_pResource = NULL;

    m_pTexture->Release();
    m_pTexture = NULL;
}

