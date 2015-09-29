/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/Resources/D3D11ResourceView.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU Resources : Bind-Views.
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
#include "D3D11ResourceView.h"

#include "../D3D11Renderer.h"

/////////////////////////////////////////////////////////////////////////////////
// D3D11ResourceView implementation
D3D11ResourceView::D3D11ResourceView( D3D11Renderer * pRenderer )
{
    m_pRenderer = pRenderer;

    m_pResource = NULL;

    m_pView = NULL;

    m_bTemporaryDestroyed = false;
    m_iBoundToBackBuffer = INVALID_OFFSET;
}
D3D11ResourceView::~D3D11ResourceView()
{
    // nothing to do
}

Void D3D11ResourceView::Destroy()
{
    DebugAssert( IsCreated() );

    if ( m_bTemporaryDestroyed )
        m_bTemporaryDestroyed = false;
    else
        _NakedDestroy();
}

Void D3D11ResourceView::OnDestroyDevice()
{
    DebugAssert( !m_bTemporaryDestroyed );

    if ( m_pView != NULL ) {
        _NakedDestroy();
        m_bTemporaryDestroyed = true;
    }
}
Void D3D11ResourceView::OnRestoreDevice()
{
    DebugAssert( m_pView == NULL );

    if ( m_bTemporaryDestroyed ) {
        _NakedCreate();
        m_bTemporaryDestroyed = false;
    }
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11RenderTargetView implementation
D3D11RenderTargetView::D3D11RenderTargetView( D3D11Renderer * pRenderer ):
    D3D11ResourceView( pRenderer )
{
    m_hRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_UNKNOWN;
    m_hRTVDesc.Format = DXGI_FORMAT_UNKNOWN;

    m_pRenderTargetView = NULL;
}
D3D11RenderTargetView::~D3D11RenderTargetView()
{
    if ( IsCreated() )
        Destroy();
}

Void D3D11RenderTargetView::AttachToBackBuffer( UInt iBackBuffer )
{
    DebugAssert( !(IsCreated()) );

    DebugAssert( iBackBuffer < m_pRenderer->m_hSwapChainDesc.iBufferCount );

    m_iBoundToBackBuffer = iBackBuffer;
    m_pResource = NULL;

    m_hRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    m_hRTVDesc.Format = PixelFormatToDXGI[m_pRenderer->m_hSwapChainDesc.iFormat];
    m_hRTVDesc.Texture2D.MipSlice = 0;

    _NakedCreate();
}
Void D3D11RenderTargetView::Create( D3D11Buffer * pBuffer, UInt iIndex, UInt iCount )
{
    DebugAssert( !(IsCreated()) );

    DebugAssert( (pBuffer->GetBinds() & D3D11RESOURCE_BIND_RENDER_TARGET) != 0 );

    DebugAssert( pBuffer->IsCreated() );
    DebugAssert( (iIndex + iCount) * pBuffer->GetStride() <= pBuffer->GetByteSize() );

    m_iBoundToBackBuffer = INVALID_OFFSET;
    m_pResource = pBuffer;

    m_hRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_BUFFER;
    m_hRTVDesc.Buffer.FirstElement = iIndex;
    m_hRTVDesc.Buffer.NumElements = iCount;

    switch( pBuffer->GetType() ) {
        case D3D11RESOURCE_BUFFER_RAW:    m_hRTVDesc.Format = DXGI_FORMAT_R32_TYPELESS; break;
        case D3D11RESOURCE_BUFFER_STRUCT: m_hRTVDesc.Format = DXGI_FORMAT_UNKNOWN;      break;
        default: DebugAssert( false ); break;
    }

    _NakedCreate();
}
Void D3D11RenderTargetView::Create( D3D11Texture * pTexture, UInt iMipSlice, UInt iArraySlice, UInt iArraySliceCount )
{
    DebugAssert( !(IsCreated()) );

    DebugAssert( (pTexture->GetBinds() & D3D11RESOURCE_BIND_RENDER_TARGET) != 0 );

    DebugAssert( pTexture->IsCreated() );
    DebugAssert( iMipSlice < pTexture->GetMipLevelCount() );
    DebugAssert( iArraySlice + iArraySliceCount <= pTexture->GetArrayCount() );

    m_iBoundToBackBuffer = INVALID_OFFSET;
    m_pResource = pTexture;

    m_hRTVDesc.Format = PixelFormatToDXGI[pTexture->GetFormat()];

    switch( pTexture->GetType() ) {
        case D3D11RESOURCE_TEXTURE_1D:
            if ( pTexture->IsArray() ) {
                m_hRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE1DARRAY;
                m_hRTVDesc.Texture1DArray.MipSlice = iMipSlice;
                m_hRTVDesc.Texture1DArray.FirstArraySlice = iArraySlice;
                m_hRTVDesc.Texture1DArray.ArraySize = iArraySliceCount;
            } else {
                m_hRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE1D;
                m_hRTVDesc.Texture1D.MipSlice = iMipSlice;
            }
            break;
        case D3D11RESOURCE_TEXTURE_2D:
            if ( pTexture->IsArray() ) {
                if ( ((D3D11Texture2D*)pTexture)->IsMultiSampled() ) {
                    m_hRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY;
                    m_hRTVDesc.Texture2DMSArray.FirstArraySlice = iArraySlice;
                    m_hRTVDesc.Texture2DMSArray.ArraySize = iArraySliceCount;
                } else {
                    m_hRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
                    m_hRTVDesc.Texture2DArray.MipSlice = iMipSlice;
                    m_hRTVDesc.Texture2DArray.FirstArraySlice = iArraySlice;
                    m_hRTVDesc.Texture2DArray.ArraySize = iArraySliceCount;
                }
            } else {
                if ( ((D3D11Texture2D*)pTexture)->IsMultiSampled() ) {
                    m_hRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
                    m_hRTVDesc.Texture2DMS.UnusedField_NothingToDefine = 0;
                } else {
                    m_hRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
                    m_hRTVDesc.Texture2D.MipSlice = iMipSlice;
                }
            }
            break;
        case D3D11RESOURCE_TEXTURE_3D:
            m_hRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE3D;
            m_hRTVDesc.Texture3D.MipSlice = iMipSlice;
            m_hRTVDesc.Texture3D.FirstWSlice = iArraySlice;
            m_hRTVDesc.Texture3D.WSize = iArraySliceCount;
            break;
        default: DebugAssert( false ); break;
    }

    _NakedCreate();
}

/////////////////////////////////////////////////////////////////////////////////

Void D3D11RenderTargetView::_NakedCreate()
{
    HRESULT hRes;

    if ( m_iBoundToBackBuffer == INVALID_OFFSET ) {
        m_pRenderTargetView = NULL;
        hRes = m_pRenderer->m_pDevice->CreateRenderTargetView( m_pResource->m_pResource, (const D3D11_RENDER_TARGET_VIEW_DESC *)&m_hRTVDesc, &m_pRenderTargetView );
        DebugAssert( hRes == S_OK && m_pRenderTargetView != NULL );
    } else {
        ID3D11Texture2D * pBackBuffer = NULL;
        hRes = m_pRenderer->m_pSwapChain->GetBuffer( m_iBoundToBackBuffer, __uuidof(ID3D11Texture2D), (Void**)&pBackBuffer );
        DebugAssert( hRes == S_OK && pBackBuffer != NULL );

        m_pRenderTargetView = NULL;
        hRes = m_pRenderer->m_pDevice->CreateRenderTargetView( pBackBuffer, (const D3D11_RENDER_TARGET_VIEW_DESC *)&m_hRTVDesc, &m_pRenderTargetView );
        DebugAssert( hRes == S_OK && m_pRenderTargetView != NULL );

        pBackBuffer->Release();
        pBackBuffer = NULL;
    }

    m_pView = NULL;
    hRes = m_pRenderTargetView->QueryInterface( __uuidof(ID3D11View), (Void**)&m_pView );
    DebugAssert( hRes == S_OK && m_pView != NULL );
}
Void D3D11RenderTargetView::_NakedDestroy()
{
    m_pView->Release();
    m_pView = NULL;

    m_pRenderTargetView->Release();
    m_pRenderTargetView = NULL;
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11DepthStencilView implementation
D3D11DepthStencilView::D3D11DepthStencilView( D3D11Renderer * pRenderer ):
    D3D11ResourceView( pRenderer )
{
    m_hDSVDesc.ViewDimension = D3D11_DSV_DIMENSION_UNKNOWN;
    m_hDSVDesc.Format = DXGI_FORMAT_UNKNOWN;
    m_hDSVDesc.Flags = 0;

    m_pDepthStencilView = NULL;
}
D3D11DepthStencilView::~D3D11DepthStencilView()
{
    if ( IsCreated() )
        Destroy();
}

Void D3D11DepthStencilView::AttachToBackBuffer( UInt iBackBuffer, Bool bReadOnlyDepth, Bool bReadOnlyStencil )
{
    DebugAssert( !(IsCreated()) );

    DebugAssert( iBackBuffer < m_pRenderer->m_hSwapChainDesc.iBufferCount );

    m_iBoundToBackBuffer = iBackBuffer;
    m_pResource = NULL;

    m_hDSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    m_hDSVDesc.Format = PixelFormatToDXGI[m_pRenderer->m_hSwapChainDesc.iFormat];
    m_hDSVDesc.Flags = 0;
    if ( bReadOnlyDepth )
        m_hDSVDesc.Flags |= D3D11_DSV_READ_ONLY_DEPTH;
    if ( bReadOnlyStencil )
        m_hDSVDesc.Flags |= D3D11_DSV_READ_ONLY_STENCIL;
    m_hDSVDesc.Texture2D.MipSlice = 0;

    _NakedCreate();
}
Void D3D11DepthStencilView::Create( D3D11Texture * pTexture, UInt iMipSlice, UInt iArraySlice, UInt iArraySliceCount, Bool bReadOnlyDepth, Bool bReadOnlyStencil )
{
    DebugAssert( !(IsCreated()) );

    DebugAssert( (pTexture->GetBinds() & D3D11RESOURCE_BIND_DEPTH_STENCIL) != 0 );

    DebugAssert( pTexture->IsCreated() );
    DebugAssert( iMipSlice < pTexture->GetMipLevelCount() );
    DebugAssert( iArraySlice + iArraySliceCount <= pTexture->GetArrayCount() );

    m_iBoundToBackBuffer = INVALID_OFFSET;
    m_pResource = pTexture;

    m_hDSVDesc.Format = PixelFormatToDXGI[pTexture->GetFormat()];
    m_hDSVDesc.Flags = 0;
    if ( bReadOnlyDepth )
        m_hDSVDesc.Flags |= D3D11_DSV_READ_ONLY_DEPTH;
    if ( bReadOnlyStencil )
        m_hDSVDesc.Flags |= D3D11_DSV_READ_ONLY_STENCIL;

    switch( pTexture->GetType() ) {
        case D3D11RESOURCE_TEXTURE_1D:
            if ( pTexture->IsArray() ) {
                m_hDSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE1DARRAY;
                m_hDSVDesc.Texture1DArray.MipSlice = iMipSlice;
                m_hDSVDesc.Texture1DArray.FirstArraySlice = iArraySlice;
                m_hDSVDesc.Texture1DArray.ArraySize = iArraySliceCount;
            } else {
                m_hDSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE1D;
                m_hDSVDesc.Texture1D.MipSlice = iMipSlice;
            }
            break;
        case D3D11RESOURCE_TEXTURE_2D:
            if ( pTexture->IsArray() ) {
                if ( ((D3D11Texture2D*)pTexture)->IsMultiSampled() ) {
                    m_hDSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY;
                    m_hDSVDesc.Texture2DMSArray.FirstArraySlice = iArraySlice;
                    m_hDSVDesc.Texture2DMSArray.ArraySize = iArraySliceCount;
                } else {
                    m_hDSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
                    m_hDSVDesc.Texture2DArray.MipSlice = iMipSlice;
                    m_hDSVDesc.Texture2DArray.FirstArraySlice = iArraySlice;
                    m_hDSVDesc.Texture2DArray.ArraySize = iArraySliceCount;
                }
            } else {
                if ( ((D3D11Texture2D*)pTexture)->IsMultiSampled() ) {
                    m_hDSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
                    m_hDSVDesc.Texture2DMS.UnusedField_NothingToDefine = 0;
                } else {
                    m_hDSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
                    m_hDSVDesc.Texture2D.MipSlice = iMipSlice;
                }
            }
            break;
        default: DebugAssert( false ); break;
    }

    _NakedCreate();
}

/////////////////////////////////////////////////////////////////////////////////

Void D3D11DepthStencilView::_NakedCreate()
{
    HRESULT hRes;

    if ( m_iBoundToBackBuffer == INVALID_OFFSET ) {
        m_pDepthStencilView = NULL;
        hRes = m_pRenderer->m_pDevice->CreateDepthStencilView( m_pResource->m_pResource, (const D3D11_DEPTH_STENCIL_VIEW_DESC *)&m_hDSVDesc, &m_pDepthStencilView );
        DebugAssert( hRes == S_OK && m_pDepthStencilView != NULL );
    } else {
        ID3D11Texture2D * pBackBuffer = NULL;
        hRes = m_pRenderer->m_pSwapChain->GetBuffer( m_iBoundToBackBuffer, __uuidof(ID3D11Texture2D), (Void**)&pBackBuffer );
        DebugAssert( hRes == S_OK && pBackBuffer != NULL );

        m_pDepthStencilView = NULL;
        hRes = m_pRenderer->m_pDevice->CreateDepthStencilView( pBackBuffer, (const D3D11_DEPTH_STENCIL_VIEW_DESC *)&m_hDSVDesc, &m_pDepthStencilView );
        DebugAssert( hRes == S_OK && m_pDepthStencilView != NULL );

        pBackBuffer->Release();
        pBackBuffer = NULL;
    }

    m_pView = NULL;
    hRes = m_pDepthStencilView->QueryInterface( __uuidof(ID3D11View), (Void**)&m_pView );
    DebugAssert( hRes == S_OK && m_pView != NULL );
}
Void D3D11DepthStencilView::_NakedDestroy()
{
    m_pView->Release();
    m_pView = NULL;

    m_pDepthStencilView->Release();
    m_pDepthStencilView = NULL;
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11ShaderView implementation
D3D11ShaderView::D3D11ShaderView( D3D11Renderer * pRenderer ):
    D3D11ResourceView( pRenderer )
{
    m_hSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_UNKNOWN;
    m_hSRVDesc.Format = DXGI_FORMAT_UNKNOWN;

    m_pShaderView = NULL;
}
D3D11ShaderView::~D3D11ShaderView()
{
    if ( IsCreated() )
        Destroy();
}

Void D3D11ShaderView::AttachToBackBuffer( UInt iBackBuffer )
{
    DebugAssert( !(IsCreated()) );

    DebugAssert( iBackBuffer < m_pRenderer->m_hSwapChainDesc.iBufferCount );

    m_iBoundToBackBuffer = iBackBuffer;
    m_pResource = NULL;

    m_hSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    m_hSRVDesc.Format = PixelFormatToDXGI[m_pRenderer->m_hSwapChainDesc.iFormat];
    m_hSRVDesc.Texture2D.MostDetailedMip = 0;
    m_hSRVDesc.Texture2D.MipLevels = 1;

    _NakedCreate();
}
Void D3D11ShaderView::Create( D3D11Buffer * pBuffer, UInt iIndex, UInt iCount )
{
    DebugAssert( !(IsCreated()) );

    DebugAssert( (pBuffer->GetBinds() & D3D11RESOURCE_BIND_SHADER_INPUT) != 0 );

    DebugAssert( pBuffer->IsCreated() );
    DebugAssert( (iIndex + iCount) * pBuffer->GetStride() <= pBuffer->GetByteSize() );

    m_iBoundToBackBuffer = INVALID_OFFSET;
    m_pResource = pBuffer;

    m_hSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
    m_hSRVDesc.BufferEx.FirstElement = iIndex;
    m_hSRVDesc.BufferEx.NumElements = iCount;

    switch( pBuffer->GetType() ) {
        case D3D11RESOURCE_BUFFER_RAW:
            m_hSRVDesc.Format = DXGI_FORMAT_R32_TYPELESS;
            m_hSRVDesc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
            break;
        case D3D11RESOURCE_BUFFER_STRUCT:
            m_hSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
            m_hSRVDesc.BufferEx.Flags = 0;
            break;
        default: DebugAssert( false ); break;
    }

    _NakedCreate();
}
Void D3D11ShaderView::Create( D3D11Texture * pTexture, UInt iMostDetailedMip, UInt iMipLevelCount, UInt iArraySlice, UInt iArraySliceCount )
{
    DebugAssert( !(IsCreated()) );

    DebugAssert( (pTexture->GetBinds() & D3D11RESOURCE_BIND_SHADER_INPUT) != 0 );

    DebugAssert( pTexture->IsCreated() );
    DebugAssert( iMostDetailedMip < pTexture->GetMipLevelCount() );
    if ( iMipLevelCount == INVALID_OFFSET )
        iMipLevelCount = ( pTexture->GetMipLevelCount() - iMostDetailedMip );
    DebugAssert( iMostDetailedMip + iMipLevelCount <= pTexture->GetMipLevelCount() );
    DebugAssert( iArraySlice + iArraySliceCount <= pTexture->GetArrayCount() );

    m_iBoundToBackBuffer = INVALID_OFFSET;
    m_pResource = pTexture;

    m_hSRVDesc.Format = PixelFormatToDXGI[pTexture->GetFormat()];

    switch( pTexture->GetType() ) {
        case D3D11RESOURCE_TEXTURE_1D:
            if ( pTexture->IsArray() ) {
                m_hSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1DARRAY;
                m_hSRVDesc.Texture1DArray.MostDetailedMip = iMostDetailedMip;
                m_hSRVDesc.Texture1DArray.MipLevels = iMipLevelCount;
                m_hSRVDesc.Texture1DArray.FirstArraySlice = iArraySlice;
                m_hSRVDesc.Texture1DArray.ArraySize = iArraySliceCount;
            } else {
                m_hSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
                m_hSRVDesc.Texture1D.MostDetailedMip = iMostDetailedMip;
                m_hSRVDesc.Texture1D.MipLevels = iMipLevelCount;
            }
            break;
        case D3D11RESOURCE_TEXTURE_2D:
            if ( pTexture->IsArray() ) {
                if ( ((D3D11Texture2D*)pTexture)->IsMultiSampled() ) {
                    m_hSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
                    m_hSRVDesc.Texture2DMSArray.FirstArraySlice = iArraySlice;
                    m_hSRVDesc.Texture2DMSArray.ArraySize = iArraySliceCount;
                } else {
                    m_hSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
                    m_hSRVDesc.Texture2DArray.MostDetailedMip = iMostDetailedMip;
                    m_hSRVDesc.Texture2DArray.MipLevels = iMipLevelCount;
                    m_hSRVDesc.Texture2DArray.FirstArraySlice = iArraySlice;
                    m_hSRVDesc.Texture2DArray.ArraySize = iArraySliceCount;
                }
            } else {
                if ( ((D3D11Texture2D*)pTexture)->IsMultiSampled() ) {
                    m_hSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
                    m_hSRVDesc.Texture2DMS.UnusedField_NothingToDefine = 0;
                } else {
                    m_hSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                    m_hSRVDesc.Texture2D.MostDetailedMip = iMostDetailedMip;
                    m_hSRVDesc.Texture2D.MipLevels = iMipLevelCount;
                }
            }
            break;
        case D3D11RESOURCE_TEXTURE_3D:
            m_hSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
            m_hSRVDesc.Texture3D.MostDetailedMip = iMostDetailedMip;
            m_hSRVDesc.Texture3D.MipLevels = iMipLevelCount;
            break;
        case D3D11RESOURCE_TEXTURE_CUBE:
            if ( ((D3D11TextureCube*)pTexture)->IsCubeArray() ) {
                m_hSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
                m_hSRVDesc.TextureCubeArray.MostDetailedMip = iMostDetailedMip;
                m_hSRVDesc.TextureCubeArray.MipLevels = iMipLevelCount;
                m_hSRVDesc.TextureCubeArray.First2DArrayFace = iArraySlice;
                m_hSRVDesc.TextureCubeArray.NumCubes = ( iArraySliceCount / 6 );
            } else {
                m_hSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
                m_hSRVDesc.TextureCube.MostDetailedMip = iMostDetailedMip;
                m_hSRVDesc.TextureCube.MipLevels = iMipLevelCount;
            }
            break;
        default: DebugAssert( false ); break;
    }

    _NakedCreate();
}

/////////////////////////////////////////////////////////////////////////////////

Void D3D11ShaderView::_NakedCreate()
{
    HRESULT hRes;

    if ( m_iBoundToBackBuffer == INVALID_OFFSET ) {
        m_pShaderView = NULL;
        hRes = m_pRenderer->m_pDevice->CreateShaderResourceView( m_pResource->m_pResource, (const D3D11_SHADER_RESOURCE_VIEW_DESC *)&m_hSRVDesc, &m_pShaderView );
        DebugAssert( hRes == S_OK && m_pShaderView != NULL );
    } else {
        ID3D11Texture2D * pBackBuffer = NULL;
        hRes = m_pRenderer->m_pSwapChain->GetBuffer( m_iBoundToBackBuffer, __uuidof(ID3D11Texture2D), (Void**)&pBackBuffer );
        DebugAssert( hRes == S_OK && pBackBuffer != NULL );

        m_pShaderView = NULL;
        hRes = m_pRenderer->m_pDevice->CreateShaderResourceView( pBackBuffer, (const D3D11_SHADER_RESOURCE_VIEW_DESC *)&m_hSRVDesc, &m_pShaderView );
        DebugAssert( hRes == S_OK && m_pShaderView != NULL );

        pBackBuffer->Release();
        pBackBuffer = NULL;
    }

    m_pView = NULL;
    hRes = m_pShaderView->QueryInterface( __uuidof(ID3D11View), (Void**)&m_pView );
    DebugAssert( hRes == S_OK && m_pView != NULL );
}
Void D3D11ShaderView::_NakedDestroy()
{
    m_pView->Release();
    m_pView = NULL;

    m_pShaderView->Release();
    m_pShaderView = NULL;
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11UnorderedAccessView implementation
D3D11UnorderedAccessView::D3D11UnorderedAccessView( D3D11Renderer * pRenderer ):
    D3D11ResourceView( pRenderer )
{
    m_hUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_UNKNOWN;
    m_hUAVDesc.Format = DXGI_FORMAT_UNKNOWN;

    m_pUnorderedAccessView = NULL;
}
D3D11UnorderedAccessView::~D3D11UnorderedAccessView()
{
    if ( IsCreated() )
        Destroy();
}

Void D3D11UnorderedAccessView::AttachToBackBuffer( UInt iBackBuffer )
{
    DebugAssert( !(IsCreated()) );

    DebugAssert( iBackBuffer < m_pRenderer->m_hSwapChainDesc.iBufferCount );

    m_iBoundToBackBuffer = iBackBuffer;
    m_pResource = NULL;

    m_hUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
    m_hUAVDesc.Format = PixelFormatToDXGI[m_pRenderer->m_hSwapChainDesc.iFormat];
    m_hUAVDesc.Texture2D.MipSlice = 0;

    _NakedCreate();
}
Void D3D11UnorderedAccessView::Create( D3D11Buffer * pBuffer, UInt iIndex, UInt iCount, Bool bAppendConsume, Bool bUseCounter )
{
    DebugAssert( !(IsCreated()) );

    DebugAssert( (pBuffer->GetBinds() & D3D11RESOURCE_BIND_UNORDERED_ACCESS) != 0 );

    DebugAssert( pBuffer->IsCreated() );
    DebugAssert( (iIndex + iCount) * pBuffer->GetStride() <= pBuffer->GetByteSize() );

    m_iBoundToBackBuffer = INVALID_OFFSET;
    m_pResource = pBuffer;

    m_hUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
    m_hUAVDesc.Buffer.FirstElement = iIndex;
    m_hUAVDesc.Buffer.NumElements = iCount;

    switch( pBuffer->GetType() ) {
        case D3D11RESOURCE_BUFFER_RAW:
            m_hUAVDesc.Format = DXGI_FORMAT_R32_TYPELESS;
            m_hUAVDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
            break;
        case D3D11RESOURCE_BUFFER_STRUCT:
            m_hUAVDesc.Format = DXGI_FORMAT_UNKNOWN;
            m_hUAVDesc.Buffer.Flags = 0;
            if ( bAppendConsume )
                m_hUAVDesc.Buffer.Flags |= D3D11_BUFFER_UAV_FLAG_APPEND;
            if ( bUseCounter ) {
                DebugAssert( pBuffer->CanGPUWrite() );
                m_hUAVDesc.Buffer.Flags |= D3D11_BUFFER_UAV_FLAG_COUNTER;
            }
            break;
        default: DebugAssert( false ); break;
    }

    _NakedCreate();
}
Void D3D11UnorderedAccessView::Create( D3D11Texture * pTexture, UInt iMipSlice, UInt iArraySlice, UInt iArraySliceCount )
{
    DebugAssert( !(IsCreated()) );

    DebugAssert( (pTexture->GetBinds() & D3D11RESOURCE_BIND_UNORDERED_ACCESS) != 0 );

    DebugAssert( pTexture->IsCreated() );
    DebugAssert( iMipSlice < pTexture->GetMipLevelCount() );
    DebugAssert( iArraySlice + iArraySliceCount <= pTexture->GetArrayCount() );

    m_iBoundToBackBuffer = INVALID_OFFSET;
    m_pResource = pTexture;

    m_hUAVDesc.Format = PixelFormatToDXGI[pTexture->GetFormat()];

    switch( pTexture->GetType() ) {
        case D3D11RESOURCE_TEXTURE_1D:
            if ( pTexture->IsArray() ) {
                m_hUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE1DARRAY;
                m_hUAVDesc.Texture1DArray.MipSlice = iMipSlice;
                m_hUAVDesc.Texture1DArray.FirstArraySlice = iArraySlice;
                m_hUAVDesc.Texture1DArray.ArraySize = iArraySliceCount;
            } else {
                m_hUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE1D;
                m_hUAVDesc.Texture1D.MipSlice = iMipSlice;
            }
            break;
        case D3D11RESOURCE_TEXTURE_2D:
            DebugAssert( !(((D3D11Texture2D*)pTexture)->IsMultiSampled()) );
            if ( pTexture->IsArray() ) {
                m_hUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
                m_hUAVDesc.Texture2DArray.MipSlice = iMipSlice;
                m_hUAVDesc.Texture2DArray.FirstArraySlice = iArraySlice;
                m_hUAVDesc.Texture2DArray.ArraySize = iArraySliceCount;
            } else {
                m_hUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
                m_hUAVDesc.Texture2D.MipSlice = iMipSlice;
            }
            break;
        case D3D11RESOURCE_TEXTURE_3D:
            m_hUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE3D;
            m_hUAVDesc.Texture3D.MipSlice = iMipSlice;
            m_hUAVDesc.Texture3D.FirstWSlice = iArraySlice;
            m_hUAVDesc.Texture3D.WSize = iArraySliceCount;
            break;
        default: DebugAssert( false ); break;
    }

    _NakedCreate();
}

/////////////////////////////////////////////////////////////////////////////////

Void D3D11UnorderedAccessView::_NakedCreate()
{
    HRESULT hRes;

    if ( m_iBoundToBackBuffer == INVALID_OFFSET ) {
        m_pUnorderedAccessView = NULL;
        hRes = m_pRenderer->m_pDevice->CreateUnorderedAccessView( m_pResource->m_pResource, (const D3D11_UNORDERED_ACCESS_VIEW_DESC *)&m_hUAVDesc, &m_pUnorderedAccessView );
        DebugAssert( hRes == S_OK && m_pUnorderedAccessView != NULL );
    } else {
        ID3D11Texture2D * pBackBuffer = NULL;
        hRes = m_pRenderer->m_pSwapChain->GetBuffer( m_iBoundToBackBuffer, __uuidof(ID3D11Texture2D), (Void**)&pBackBuffer );
        DebugAssert( hRes == S_OK && pBackBuffer != NULL );

        m_pUnorderedAccessView = NULL;
        hRes = m_pRenderer->m_pDevice->CreateUnorderedAccessView( pBackBuffer, (const D3D11_UNORDERED_ACCESS_VIEW_DESC *)&m_hUAVDesc, &m_pUnorderedAccessView );
        DebugAssert( hRes == S_OK && m_pUnorderedAccessView != NULL );

        pBackBuffer->Release();
        pBackBuffer = NULL;
    }

    m_pView = NULL;
    hRes = m_pUnorderedAccessView->QueryInterface( __uuidof(ID3D11View), (Void**)&m_pView );
    DebugAssert( hRes == S_OK && m_pView != NULL );
}
Void D3D11UnorderedAccessView::_NakedDestroy()
{
    m_pView->Release();
    m_pView = NULL;

    m_pUnorderedAccessView->Release();
    m_pUnorderedAccessView = NULL;
}

