/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/D3D11Renderer.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Platform-dependant abstraction for 3D graphics.
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
#include <d2d1.h>
#include <dwrite.h>

#undef DebugAssert

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "D3D11Renderer.h"

/////////////////////////////////////////////////////////////////////////////////
// D3D11RendererCallbacks implementation
D3D11RendererCallbacks::D3D11RendererCallbacks( Void * pUserData )
{
    m_pUserData = pUserData;
}
D3D11RendererCallbacks::~D3D11RendererCallbacks()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11Renderer implementation
Void * D3D11Renderer::sm_arrNullBind[_D3D11_RENDERER_NULL_BINDING_SIZE] = {
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};
UInt D3D11Renderer::sm_arrNullBindData[_D3D11_RENDERER_NULL_BINDING_SIZE] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

D3D11Renderer::D3D11Renderer( D3D11Window * pWindow, D3D11RendererCallbacks * pCallbacks )
{
    DebugAssert( pWindow != NULL && pCallbacks != NULL );

    // Bind to our window
    m_pWindow = pWindow;
    m_pWindow->m_pRenderer = this;

    // Create 2D factories
    m_pD2D1Factory = NULL;
    HRESULT hRes = D2D1CreateFactory( D2D1_FACTORY_TYPE_MULTI_THREADED, __uuidof(ID2D1Factory), (Void**)&m_pD2D1Factory );
    DebugAssert( hRes == S_OK && m_pD2D1Factory != NULL );

    m_pDWFactory = NULL;
    hRes = DWriteCreateFactory( DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&m_pDWFactory );
    DebugAssert( hRes == S_OK && m_pDWFactory != NULL );

    // Create device & swapchain
    m_pCallbacks = pCallbacks;

    m_iFeatureLevel = D3D_FEATURE_LEVEL_11_0;
    m_pDevice = NULL;
    m_pImmediateContext = NULL;

    m_pSwapChain = NULL;
    m_hSwapChainDesc.iSampleCount = 1;

    m_bIdleState = false;

    _CreateDevice( true );
    _CreateSwapChain( true );
}
D3D11Renderer::~D3D11Renderer()
{
    // Destroy device & swapchain
    if ( m_pSwapChain != NULL )
        _DestroySwapChain( true );
    if ( m_pDevice != NULL )
        _DestroyDevice( true );

    // Destroy 2D factories
    m_pDWFactory->Release();
    m_pDWFactory = NULL;

    m_pD2D1Factory->Release();
    m_pD2D1Factory = NULL;

    // Release our window
    m_pWindow->m_pRenderer = NULL;
    m_pWindow = NULL;
}

Void D3D11Renderer::GetDeviceFeatures( D3D11DeviceFeatures * outFeatures ) const
{
    DebugAssert( m_pDevice != NULL );

    D3D11_FEATURE_DATA_THREADING hTreading;
    HRESULT hRes = m_pDevice->CheckFeatureSupport( D3D11_FEATURE_THREADING, &hTreading, sizeof(D3D11_FEATURE_DATA_THREADING) );
    DebugAssert( hRes == S_OK );

    D3D11_FEATURE_DATA_ARCHITECTURE_INFO hArchitecture;
    hRes = m_pDevice->CheckFeatureSupport( D3D11_FEATURE_ARCHITECTURE_INFO, &hArchitecture, sizeof(D3D11_FEATURE_DATA_ARCHITECTURE_INFO) );
    DebugAssert( hRes == S_OK );

    D3D11_FEATURE_DATA_DOUBLES hDoubles;
    hRes = m_pDevice->CheckFeatureSupport( D3D11_FEATURE_DOUBLES, &hDoubles, sizeof(D3D11_FEATURE_DATA_DOUBLES) );
    DebugAssert( hRes == S_OK );

    D3D11_FEATURE_DATA_SHADER_MIN_PRECISION_SUPPORT hShadersMinPrecision;
    hRes = m_pDevice->CheckFeatureSupport( D3D11_FEATURE_SHADER_MIN_PRECISION_SUPPORT, &hShadersMinPrecision,
                                           sizeof(D3D11_FEATURE_DATA_SHADER_MIN_PRECISION_SUPPORT) );
    DebugAssert( hRes == S_OK );

    D3D11_FEATURE_DATA_D3D9_OPTIONS hD3D9;
    hRes = m_pDevice->CheckFeatureSupport( D3D11_FEATURE_D3D9_OPTIONS, &hD3D9, sizeof(D3D11_FEATURE_DATA_D3D9_OPTIONS) );
    DebugAssert( hRes == S_OK );

    D3D11_FEATURE_DATA_D3D9_SHADOW_SUPPORT hD3D9Shadows;
    hRes = m_pDevice->CheckFeatureSupport( D3D11_FEATURE_D3D9_SHADOW_SUPPORT, &hD3D9Shadows, sizeof(D3D11_FEATURE_DATA_D3D9_SHADOW_SUPPORT) );
    DebugAssert( hRes == S_OK );

    D3D11_FEATURE_DATA_D3D10_X_HARDWARE_OPTIONS hD3D10;
    hRes = m_pDevice->CheckFeatureSupport( D3D11_FEATURE_D3D10_X_HARDWARE_OPTIONS, &hD3D10, sizeof(D3D11_FEATURE_DATA_D3D10_X_HARDWARE_OPTIONS) );
    DebugAssert( hRes == S_OK );

    D3D11_FEATURE_DATA_D3D11_OPTIONS hD3D11;
    hRes = m_pDevice->CheckFeatureSupport( D3D11_FEATURE_D3D11_OPTIONS, &hD3D11, sizeof(D3D11_FEATURE_DATA_D3D11_OPTIONS) );
    DebugAssert( hRes == S_OK );

    outFeatures->ConvertFrom( &hTreading, &hArchitecture, &hDoubles, &hShadersMinPrecision, &hD3D9, &hD3D9Shadows, &hD3D10, &hD3D11 );
}
Void D3D11Renderer::GetFormatSupport( D3D11PixelFormatSupport * outFormatSupport, PixelFormat iFormat ) const
{
    DebugAssert( m_pDevice != NULL );

    D3D11_FEATURE_DATA_FORMAT_SUPPORT hFormat1;
    hFormat1.InFormat = PixelFormatToDXGI[iFormat];

    HRESULT hRes = m_pDevice->CheckFeatureSupport( D3D11_FEATURE_FORMAT_SUPPORT, &hFormat1, sizeof(D3D11_FEATURE_DATA_FORMAT_SUPPORT) );
    DebugAssert( hRes == S_OK );

    D3D11_FEATURE_DATA_FORMAT_SUPPORT2 hFormat2;
    hFormat2.InFormat = PixelFormatToDXGI[iFormat];

    hRes = m_pDevice->CheckFeatureSupport( D3D11_FEATURE_FORMAT_SUPPORT2, &hFormat2, sizeof(D3D11_FEATURE_DATA_FORMAT_SUPPORT2) );
    DebugAssert( hRes == S_OK );

    outFormatSupport->ConvertFrom( hFormat1.OutFormatSupport, hFormat2.OutFormatSupport2 );
}
Void D3D11Renderer::GetCounterSupport( D3D11CounterSupport * outCounterSupport ) const
{
    DebugAssert( m_pDevice != NULL );

    D3D11_COUNTER_INFO hInfos;
    m_pDevice->CheckCounterInfo( &hInfos );

    outCounterSupport->ConvertFrom( &hInfos );
}

Void D3D11Renderer::GetFrameStats( D3D11FrameStats * outFrameStats ) const
{
    DebugAssert( m_pSwapChain != NULL );
    DebugAssert( !(m_hSwapChainDesc.bWindowed) );

    DXGI_FRAME_STATISTICS dxgiStats;
    HRESULT hRes = m_pSwapChain->GetFrameStatistics( &dxgiStats );
    DebugAssert( hRes == S_OK );

    UInt iLastPresentCount = 0;
    hRes = m_pSwapChain->GetLastPresentCount( &iLastPresentCount );
    DebugAssert( hRes == S_OK );

    outFrameStats->ConvertFrom( &dxgiStats, iLastPresentCount );
}

Void D3D11Renderer::SetMultisampling( UInt iSampleCount )
{
    DebugAssert( m_pSwapChain != NULL );
    DebugAssert( iSampleCount == 1 || iSampleCount == 2 || iSampleCount == 4 || iSampleCount == 8 || iSampleCount == 16 );

    // Don't do anything when messing with window / display modes
    if ( m_pWindow->m_bIsInSizeMove )
        return;
    if ( m_pWindow->m_bInDisplayModeSwitchTransition )
        return;

    // Re-create SwapChain
    _DestroySwapChain();
    m_hSwapChainDesc.iSampleCount = iSampleCount;
    _CreateSwapChain();
}

Bool D3D11Renderer::Present( UInt iVSyncInterval, UInt iPresentFlags )
{
    DebugAssert( m_pSwapChain != NULL );
    DebugAssert( iVSyncInterval < 5 );

    // Don't do anything when messing with window / display modes
    if ( m_pWindow->m_bIsInSizeMove )
        return false;
    if ( m_pWindow->m_bInDisplayModeSwitchTransition )
        return false;

    HRESULT hRes;

    // Idle state case
    if ( m_bIdleState ) {
        // Test presentation
        hRes = m_pSwapChain->Present( iVSyncInterval, iPresentFlags | DXGI_PRESENT_TEST );
    
        // Check device status
        if ( hRes != S_OK ) {
            if ( hRes == DXGI_STATUS_OCCLUDED )
                return false;

            DebugAssert( false );
            return false;
        }

        // Exit idle state
        m_bIdleState = false;
    }

    // Present current frame
    hRes = m_pSwapChain->Present( iVSyncInterval, iPresentFlags );
    
    // Check device status
    if ( hRes != S_OK ) {
        if ( hRes == DXGI_STATUS_OCCLUDED ) {
            // Enter idle state
            m_bIdleState = true;
            return false;
        }

        // Enter idle state
        m_bIdleState = true;

        DebugAssert( false );
        return false;
    }

    // Present succeeded
    return true;
}

Void D3D11Renderer::IASetInputLayout( D3D11InputLayout * pInputLayout, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );
    
    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    if ( pInputLayout == NULL )
        pDeviceContext->IASetInputLayout( NULL );
    else {
        DebugAssert( pInputLayout->m_pInputLayout != NULL );
        pDeviceContext->IASetInputLayout( pInputLayout->m_pInputLayout );
    }
}
Void D3D11Renderer::IASetPrimitiveTopology( D3D11ShaderPrimitiveTopology iTopology, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->IASetPrimitiveTopology( D3D11ShaderPrimitiveTopologyToD3D11[iTopology] );
}

Void D3D11Renderer::IASetVertexBuffers( UInt iBindSlot, UInt iBufferCount, D3D11VertexBuffer ** arrBuffers, const UInt * arrStrides, const UInt * arrOffsets, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );
    DebugAssert( iBindSlot + iBufferCount <= D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;


    if ( arrBuffers == NULL )
        pDeviceContext->IASetVertexBuffers( iBindSlot, iBufferCount, (ID3D11Buffer**)sm_arrNullBind, sm_arrNullBindData, sm_arrNullBindData );
    else {
        DebugAssert( arrStrides != NULL );
        ID3D11Buffer * arrInstances[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];
        for( UInt i = 0; i < iBufferCount; ++i ) {
            arrInstances[i] = NULL;
            if ( arrBuffers[i] != NULL ) {
                DebugAssert( arrBuffers[i]->m_pBuffer != NULL );
                arrInstances[i] = arrBuffers[i]->m_pBuffer;
            }
        }
        if ( arrOffsets == NULL )
            arrOffsets = sm_arrNullBindData;
        pDeviceContext->IASetVertexBuffers( iBindSlot, iBufferCount, arrInstances, arrStrides, arrOffsets );
    }
}
Void D3D11Renderer::IASetIndexBuffer( D3D11IndexBuffer * pIndexBuffer, Bool bUseShorts, UInt iOffset, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    if ( pIndexBuffer == NULL )
        pDeviceContext->IASetIndexBuffer( NULL, DXGI_FORMAT_R32_UINT, 0 );
    else {
        DebugAssert( pIndexBuffer->m_pBuffer != NULL );
        pDeviceContext->IASetIndexBuffer( pIndexBuffer->m_pBuffer, (bUseShorts) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, iOffset );
    }
}

Void D3D11Renderer::VSSetShader( D3D11VertexShader * pVertexShader, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    if ( pVertexShader == NULL )
        pDeviceContext->VSSetShader( NULL, NULL, 0 );
    else {
        DebugAssert( pVertexShader->m_pVertexShader != NULL );
        pDeviceContext->VSSetShader( pVertexShader->m_pVertexShader, pVertexShader->m_arrClassInstances, pVertexShader->m_iClassInstanceCount );
    }
}
Void D3D11Renderer::VSSetConstantBuffers( UInt iBindSlot, UInt iBufferCount, D3D11ConstantBuffer ** arrBuffers, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );
    DebugAssert( iBindSlot + iBufferCount <= D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    if ( arrBuffers == NULL )
        pDeviceContext->VSSetConstantBuffers( iBindSlot, iBufferCount, (ID3D11Buffer**)sm_arrNullBind );
    else {
        ID3D11Buffer * arrInstances[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
        for( UInt i = 0; i < iBufferCount; ++i ) {
            arrInstances[i] = NULL;
            if ( arrBuffers[i] != NULL ) {
                DebugAssert( arrBuffers[i]->m_pBuffer != NULL );
                arrInstances[i] = arrBuffers[i]->m_pBuffer;
            }
        }
        pDeviceContext->VSSetConstantBuffers( iBindSlot, iBufferCount, arrInstances );
    }
}
Void D3D11Renderer::VSSetSamplers( UInt iBindSlot, UInt iSamplerCount, D3D11SamplerState ** arrSamplers, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );
    DebugAssert( iBindSlot + iSamplerCount <= D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    if ( arrSamplers == NULL )
        pDeviceContext->VSSetSamplers( iBindSlot, iSamplerCount, (ID3D11SamplerState**)sm_arrNullBind );
    else {
        ID3D11SamplerState * arrInstances[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];
        for( UInt i = 0; i < iSamplerCount; ++i ) {
            arrInstances[i] = NULL;
            if ( arrSamplers[i] != NULL ) {
                DebugAssert( arrSamplers[i]->m_pSamplerState != NULL );
                arrInstances[i] = arrSamplers[i]->m_pSamplerState;
            }
        }
        pDeviceContext->VSSetSamplers( iBindSlot, iSamplerCount, arrInstances );
    }
}
Void D3D11Renderer::VSSetResources( UInt iBindSlot, UInt iResourceCount, D3D11ShaderView ** arrResources, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );
    DebugAssert( iBindSlot + iResourceCount <= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    if ( arrResources == NULL )
        pDeviceContext->VSSetShaderResources( iBindSlot, iResourceCount, (ID3D11ShaderResourceView**)sm_arrNullBind );
    else {
        ID3D11ShaderResourceView * arrInstances[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
        for( UInt i = 0; i < iResourceCount; ++i ) {
            arrInstances[i] = NULL;
            if ( arrResources[i] != NULL ) {
                DebugAssert( arrResources[i]->m_pShaderView != NULL );
                arrInstances[i] = arrResources[i]->m_pShaderView;
            }
        }
        pDeviceContext->VSSetShaderResources( iBindSlot, iResourceCount, arrInstances );
    }
}

Void D3D11Renderer::GSSetShader( D3D11GeometryShader * pGeometryShader, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    if ( pGeometryShader == NULL )
        pDeviceContext->GSSetShader( NULL, NULL, 0 );
    else {
        DebugAssert( pGeometryShader->m_pGeometryShader != NULL );
        pDeviceContext->GSSetShader( pGeometryShader->m_pGeometryShader, pGeometryShader->m_arrClassInstances, pGeometryShader->m_iClassInstanceCount );
    }
}
Void D3D11Renderer::GSSetConstantBuffers( UInt iBindSlot, UInt iBufferCount, D3D11ConstantBuffer ** arrBuffers, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );
    DebugAssert( iBindSlot + iBufferCount <= D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    if ( arrBuffers == NULL )
        pDeviceContext->GSSetConstantBuffers( iBindSlot, iBufferCount, (ID3D11Buffer**)sm_arrNullBind );
    else {
        ID3D11Buffer * arrInstances[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
        for( UInt i = 0; i < iBufferCount; ++i ) {
            arrInstances[i] = NULL;
            if ( arrBuffers[i] != NULL ) {
                DebugAssert( arrBuffers[i]->m_pBuffer != NULL );
                arrInstances[i] = arrBuffers[i]->m_pBuffer;
            }
        }
        pDeviceContext->GSSetConstantBuffers( iBindSlot, iBufferCount, arrInstances );
    }
}
Void D3D11Renderer::GSSetSamplers( UInt iBindSlot, UInt iSamplerCount, D3D11SamplerState ** arrSamplers, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );
    DebugAssert( iBindSlot + iSamplerCount <= D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    if ( arrSamplers == NULL )
        pDeviceContext->GSSetSamplers( iBindSlot, iSamplerCount, (ID3D11SamplerState**)sm_arrNullBind );
    else {
        ID3D11SamplerState * arrInstances[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];
        for( UInt i = 0; i < iSamplerCount; ++i ) {
            arrInstances[i] = NULL;
            if ( arrSamplers[i] != NULL ) {
                DebugAssert( arrSamplers[i]->m_pSamplerState != NULL );
                arrInstances[i] = arrSamplers[i]->m_pSamplerState;
            }
        }
        pDeviceContext->GSSetSamplers( iBindSlot, iSamplerCount, arrInstances );
    }
}
Void D3D11Renderer::GSSetResources( UInt iBindSlot, UInt iResourceCount, D3D11ShaderView ** arrResources, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );
    DebugAssert( iBindSlot + iResourceCount <= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    if ( arrResources == NULL )
        pDeviceContext->GSSetShaderResources( iBindSlot, iResourceCount, (ID3D11ShaderResourceView**)sm_arrNullBind );
    else {
        ID3D11ShaderResourceView * arrInstances[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
        for( UInt i = 0; i < iResourceCount; ++i ) {
            arrInstances[i] = NULL;
            if ( arrResources[i] != NULL ) {
                DebugAssert( arrResources[i]->m_pShaderView != NULL );
                arrInstances[i] = arrResources[i]->m_pShaderView;
            }
        }
        pDeviceContext->GSSetShaderResources( iBindSlot, iResourceCount, arrInstances );
    }
}

Void D3D11Renderer::PSSetShader( D3D11PixelShader * pPixelShader, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    if ( pPixelShader == NULL )
        pDeviceContext->PSSetShader( NULL, NULL, 0 );
    else {
        DebugAssert( pPixelShader->m_pPixelShader != NULL );
        pDeviceContext->PSSetShader( pPixelShader->m_pPixelShader, pPixelShader->m_arrClassInstances, pPixelShader->m_iClassInstanceCount );
    }
}
Void D3D11Renderer::PSSetConstantBuffers( UInt iBindSlot, UInt iBufferCount, D3D11ConstantBuffer ** arrBuffers, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );
    DebugAssert( iBindSlot + iBufferCount <= D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    if ( arrBuffers == NULL )
        pDeviceContext->PSSetConstantBuffers( iBindSlot, iBufferCount, (ID3D11Buffer**)sm_arrNullBind );
    else {
        ID3D11Buffer * arrInstances[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
        for( UInt i = 0; i < iBufferCount; ++i ) {
            arrInstances[i] = NULL;
            if ( arrBuffers[i] != NULL ) {
                DebugAssert( arrBuffers[i]->m_pBuffer != NULL );
                arrInstances[i] = arrBuffers[i]->m_pBuffer;
            }
        }
        pDeviceContext->PSSetConstantBuffers( iBindSlot, iBufferCount, arrInstances );
    }
}
Void D3D11Renderer::PSSetSamplers( UInt iBindSlot, UInt iSamplerCount, D3D11SamplerState ** arrSamplers, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );
    DebugAssert( iBindSlot + iSamplerCount <= D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    if ( arrSamplers == NULL )
        pDeviceContext->PSSetSamplers( iBindSlot, iSamplerCount, (ID3D11SamplerState**)sm_arrNullBind );
    else {
        ID3D11SamplerState * arrInstances[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];
        for( UInt i = 0; i < iSamplerCount; ++i ) {
            arrInstances[i] = NULL;
            if ( arrSamplers[i] != NULL ) {
                DebugAssert( arrSamplers[i]->m_pSamplerState != NULL );
                arrInstances[i] = arrSamplers[i]->m_pSamplerState;
            }
        }
        pDeviceContext->PSSetSamplers( iBindSlot, iSamplerCount, arrInstances );
    }
}
Void D3D11Renderer::PSSetResources( UInt iBindSlot, UInt iResourceCount, D3D11ShaderView ** arrResources, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );
    DebugAssert( iBindSlot + iResourceCount <= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    if ( arrResources == NULL )
        pDeviceContext->PSSetShaderResources( iBindSlot, iResourceCount, (ID3D11ShaderResourceView**)sm_arrNullBind );
    else {
        ID3D11ShaderResourceView * arrInstances[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
        for( UInt i = 0; i < iResourceCount; ++i ) {
            arrInstances[i] = NULL;
            if ( arrResources[i] != NULL ) {
                DebugAssert( arrResources[i]->m_pShaderView != NULL );
                arrInstances[i] = arrResources[i]->m_pShaderView;
            }
        }
        pDeviceContext->PSSetShaderResources( iBindSlot, iResourceCount, arrInstances );
    }
}

Void D3D11Renderer::HSSetShader( D3D11HullShader * pHullShader, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    if ( pHullShader == NULL )
        pDeviceContext->HSSetShader( NULL, NULL, 0 );
    else {
        DebugAssert( pHullShader->m_pHullShader != NULL );
        pDeviceContext->HSSetShader( pHullShader->m_pHullShader, pHullShader->m_arrClassInstances, pHullShader->m_iClassInstanceCount );
    }
}
Void D3D11Renderer::HSSetConstantBuffers( UInt iBindSlot, UInt iBufferCount, D3D11ConstantBuffer ** arrBuffers, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );
    DebugAssert( iBindSlot + iBufferCount <= D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    if ( arrBuffers == NULL )
        pDeviceContext->HSSetConstantBuffers( iBindSlot, iBufferCount, (ID3D11Buffer**)sm_arrNullBind );
    else {
        ID3D11Buffer * arrInstances[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
        for( UInt i = 0; i < iBufferCount; ++i ) {
            arrInstances[i] = NULL;
            if ( arrBuffers[i] != NULL ) {
                DebugAssert( arrBuffers[i]->m_pBuffer != NULL );
                arrInstances[i] = arrBuffers[i]->m_pBuffer;
            }
        }
        pDeviceContext->HSSetConstantBuffers( iBindSlot, iBufferCount, arrInstances );
    }
}
Void D3D11Renderer::HSSetSamplers( UInt iBindSlot, UInt iSamplerCount, D3D11SamplerState ** arrSamplers, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );
    DebugAssert( iBindSlot + iSamplerCount <= D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    if ( arrSamplers == NULL )
        pDeviceContext->HSSetSamplers( iBindSlot, iSamplerCount, (ID3D11SamplerState**)sm_arrNullBind );
    else {
        ID3D11SamplerState * arrInstances[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];
        for( UInt i = 0; i < iSamplerCount; ++i ) {
            arrInstances[i] = NULL;
            if ( arrSamplers[i] != NULL ) {
                DebugAssert( arrSamplers[i]->m_pSamplerState != NULL );
                arrInstances[i] = arrSamplers[i]->m_pSamplerState;
            }
        }
        pDeviceContext->HSSetSamplers( iBindSlot, iSamplerCount, arrInstances );
    }
}
Void D3D11Renderer::HSSetResources( UInt iBindSlot, UInt iResourceCount, D3D11ShaderView ** arrResources, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );
    DebugAssert( iBindSlot + iResourceCount <= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    if ( arrResources == NULL )
        pDeviceContext->HSSetShaderResources( iBindSlot, iResourceCount, (ID3D11ShaderResourceView**)sm_arrNullBind );
    else {
        ID3D11ShaderResourceView * arrInstances[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
        for( UInt i = 0; i < iResourceCount; ++i ) {
            arrInstances[i] = NULL;
            if ( arrResources[i] != NULL ) {
                DebugAssert( arrResources[i]->m_pShaderView != NULL );
                arrInstances[i] = arrResources[i]->m_pShaderView;
            }
        }
        pDeviceContext->HSSetShaderResources( iBindSlot, iResourceCount, arrInstances );
    }
}

Void D3D11Renderer::DSSetShader( D3D11DomainShader * pDomainShader, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    if ( pDomainShader == NULL )
        pDeviceContext->DSSetShader( NULL, NULL, 0 );
    else {
        DebugAssert( pDomainShader->m_pDomainShader != NULL );
        pDeviceContext->DSSetShader( pDomainShader->m_pDomainShader, pDomainShader->m_arrClassInstances, pDomainShader->m_iClassInstanceCount );
    }
}
Void D3D11Renderer::DSSetConstantBuffers( UInt iBindSlot, UInt iBufferCount, D3D11ConstantBuffer ** arrBuffers, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );
    DebugAssert( iBindSlot + iBufferCount <= D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    if ( arrBuffers == NULL )
        pDeviceContext->DSSetConstantBuffers( iBindSlot, iBufferCount, (ID3D11Buffer**)sm_arrNullBind );
    else {
        ID3D11Buffer * arrInstances[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
        for( UInt i = 0; i < iBufferCount; ++i ) {
            arrInstances[i] = NULL;
            if ( arrBuffers[i] != NULL ) {
                DebugAssert( arrBuffers[i]->m_pBuffer != NULL );
                arrInstances[i] = arrBuffers[i]->m_pBuffer;
            }
        }
        pDeviceContext->DSSetConstantBuffers( iBindSlot, iBufferCount, arrInstances );
    }
}
Void D3D11Renderer::DSSetSamplers( UInt iBindSlot, UInt iSamplerCount, D3D11SamplerState ** arrSamplers, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );
    DebugAssert( iBindSlot + iSamplerCount <= D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    if ( arrSamplers == NULL )
        pDeviceContext->DSSetSamplers( iBindSlot, iSamplerCount, (ID3D11SamplerState**)sm_arrNullBind );
    else {
        ID3D11SamplerState * arrInstances[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];
        for( UInt i = 0; i < iSamplerCount; ++i ) {
            arrInstances[i] = NULL;
            if ( arrSamplers[i] != NULL ) {
                DebugAssert( arrSamplers[i]->m_pSamplerState != NULL );
                arrInstances[i] = arrSamplers[i]->m_pSamplerState;
            }
        }
        pDeviceContext->DSSetSamplers( iBindSlot, iSamplerCount, arrInstances );
    }
}
Void D3D11Renderer::DSSetResources( UInt iBindSlot, UInt iResourceCount, D3D11ShaderView ** arrResources, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );
    DebugAssert( iBindSlot + iResourceCount <= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    if ( arrResources == NULL )
        pDeviceContext->DSSetShaderResources( iBindSlot, iResourceCount, (ID3D11ShaderResourceView**)sm_arrNullBind );
    else {
        ID3D11ShaderResourceView * arrInstances[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
        for( UInt i = 0; i < iResourceCount; ++i ) {
            arrInstances[i] = NULL;
            if ( arrResources[i] != NULL ) {
                DebugAssert( arrResources[i]->m_pShaderView != NULL );
                arrInstances[i] = arrResources[i]->m_pShaderView;
            }
        }
        pDeviceContext->DSSetShaderResources( iBindSlot, iResourceCount, arrInstances );
    }
}

Void D3D11Renderer::CSSetShader( D3D11ComputeShader * pComputeShader, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    if ( pComputeShader == NULL )
        pDeviceContext->CSSetShader( NULL, NULL, 0 );
    else {
        DebugAssert( pComputeShader->m_pComputeShader != NULL );
        pDeviceContext->CSSetShader( pComputeShader->m_pComputeShader, pComputeShader->m_arrClassInstances, pComputeShader->m_iClassInstanceCount );
    }
}
Void D3D11Renderer::CSSetConstantBuffers( UInt iBindSlot, UInt iBufferCount, D3D11ConstantBuffer ** arrBuffers, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );
    DebugAssert( iBindSlot + iBufferCount <= D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    if ( arrBuffers == NULL )
        pDeviceContext->CSSetConstantBuffers( iBindSlot, iBufferCount, (ID3D11Buffer**)sm_arrNullBind );
    else {
        ID3D11Buffer * arrInstances[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT];
        for( UInt i = 0; i < iBufferCount; ++i ) {
            arrInstances[i] = NULL;
            if ( arrBuffers[i] != NULL ) {
                DebugAssert( arrBuffers[i]->m_pBuffer != NULL );
                arrInstances[i] = arrBuffers[i]->m_pBuffer;
            }
        }
        pDeviceContext->CSSetConstantBuffers( iBindSlot, iBufferCount, arrInstances );
    }
}
Void D3D11Renderer::CSSetSamplers( UInt iBindSlot, UInt iSamplerCount, D3D11SamplerState ** arrSamplers, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );
    DebugAssert( iBindSlot + iSamplerCount <= D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    if ( arrSamplers == NULL )
        pDeviceContext->CSSetSamplers( iBindSlot, iSamplerCount, (ID3D11SamplerState**)sm_arrNullBind );
    else {
        ID3D11SamplerState * arrInstances[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT];
        for( UInt i = 0; i < iSamplerCount; ++i ) {
            arrInstances[i] = NULL;
            if ( arrSamplers[i] != NULL ) {
                DebugAssert( arrSamplers[i]->m_pSamplerState != NULL );
                arrInstances[i] = arrSamplers[i]->m_pSamplerState;
            }
        }
        pDeviceContext->CSSetSamplers( iBindSlot, iSamplerCount, arrInstances );
    }
}
Void D3D11Renderer::CSSetResources( UInt iBindSlot, UInt iResourceCount, D3D11ShaderView ** arrResources, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );
    DebugAssert( iBindSlot + iResourceCount <= D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    if ( arrResources == NULL )
        pDeviceContext->CSSetShaderResources( iBindSlot, iResourceCount, (ID3D11ShaderResourceView**)sm_arrNullBind );
    else {
        ID3D11ShaderResourceView * arrInstances[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
        for( UInt i = 0; i < iResourceCount; ++i ) {
            arrInstances[i] = NULL;
            if ( arrResources[i] != NULL ) {
                DebugAssert( arrResources[i]->m_pShaderView != NULL );
                arrInstances[i] = arrResources[i]->m_pShaderView;
            }
        }
        pDeviceContext->CSSetShaderResources( iBindSlot, iResourceCount, arrInstances );
    }
}
Void D3D11Renderer::CSSetUAVs( UInt iUAVSlot, UInt iUAVCount, D3D11UnorderedAccessView ** arrUAVs, const UInt * arrAppendConsumeInitCounts, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );
    DebugAssert( iUAVSlot + iUAVCount <= D3D11_PS_CS_UAV_REGISTER_COUNT );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    if ( arrUAVs == NULL )
        pDeviceContext->CSSetUnorderedAccessViews( iUAVSlot, iUAVCount, (ID3D11UnorderedAccessView**)sm_arrNullBind, NULL );
    else {
        ID3D11UnorderedAccessView * arrInstances[D3D11_PS_CS_UAV_REGISTER_COUNT];
        for( UInt i = 0; i < iUAVCount; ++i ) {
            arrInstances[i] = NULL;
            if ( arrUAVs[i] != NULL ) {
                DebugAssert( arrUAVs[i]->m_pUnorderedAccessView != NULL );
                arrInstances[i] = arrUAVs[i]->m_pUnorderedAccessView;
            }
        }
        pDeviceContext->CSSetUnorderedAccessViews( iUAVSlot, iUAVCount, arrInstances, arrAppendConsumeInitCounts );
    }
}

Void D3D11Renderer::SOSetBuffers( UInt iBufferCount, D3D11Buffer ** arrBuffers, const UInt * arrOffsets, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );
    DebugAssert( iBufferCount <= D3D11_SO_BUFFER_SLOT_COUNT );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    if ( arrBuffers == NULL )
        pDeviceContext->SOSetTargets( iBufferCount, (ID3D11Buffer**)sm_arrNullBind, sm_arrNullBindData );
    else {
        ID3D11Buffer * arrInstances[D3D11_SO_BUFFER_SLOT_COUNT];
        for( UInt i = 0; i < iBufferCount; ++i ) {
            arrInstances[i] = NULL;
            if ( arrBuffers[i] != NULL ) {
                DebugAssert( arrBuffers[i]->m_pBuffer != NULL );
                arrInstances[i] = arrBuffers[i]->m_pBuffer;
            }
        }
        if ( arrOffsets == NULL )
            arrOffsets = sm_arrNullBindData;
        pDeviceContext->SOSetTargets( iBufferCount, arrInstances, arrOffsets );
    }
}

Void D3D11Renderer::RSSetState( D3D11RasterizerState * pRasterizerState, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    if ( pRasterizerState == NULL )
        pDeviceContext->RSSetState( NULL );
    else {
        DebugAssert( pRasterizerState->m_pRasterizerState != NULL );
        pDeviceContext->RSSetState( pRasterizerState->m_pRasterizerState );
    }
}

Void D3D11Renderer::RSSetViewports( UInt iViewportCount, const D3D11Viewport * arrViewports, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );
    DebugAssert( iViewportCount <= D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    if ( iViewportCount == 0 ) {
        pDeviceContext->RSSetViewports( 0, NULL );
    } else {
        DebugAssert( arrViewports != NULL );
        pDeviceContext->RSSetViewports( iViewportCount, (const D3D11_VIEWPORT *)arrViewports );
    }
}
Void D3D11Renderer::RSSetScissors( UInt iScissorCount, const D3D11Rectangle * arrScissors, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );
    DebugAssert( iScissorCount <= D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    if ( iScissorCount == 0 ) {
        pDeviceContext->RSSetScissorRects( 0, NULL );
    } else {
        DebugAssert( arrScissors != NULL );
        pDeviceContext->RSSetScissorRects( iScissorCount, (const D3D11_RECT *)arrScissors );
    }
}

Void D3D11Renderer::OMSetDepthStencilState( D3D11DepthStencilState * pDepthStencilState, DWord iStencilRef, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    if ( pDepthStencilState == NULL )
        pDeviceContext->OMSetDepthStencilState( NULL, 0 );
    else {
        DebugAssert( pDepthStencilState->m_pDepthStencilState != NULL );
        pDeviceContext->OMSetDepthStencilState( pDepthStencilState->m_pDepthStencilState, iStencilRef );
    }
}
Void D3D11Renderer::OMSetBlendState( D3D11BlendState * pBlendState, const Float arrBlendFactors[4], DWord iSampleMask, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    if ( pBlendState == NULL ) {
        pDeviceContext->OMSetBlendState( NULL, NULL, 0xffffffff );
    } else {
        DebugAssert( pBlendState->m_pBlendState != NULL );
        pDeviceContext->OMSetBlendState( pBlendState->m_pBlendState, arrBlendFactors, iSampleMask );
    }
}

Void D3D11Renderer::OMSetRenderTargets( UInt iRenderTargetCount, D3D11RenderTargetView ** arrRenderTargets, D3D11DepthStencilView * pDepthStencilView, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );
    DebugAssert( iRenderTargetCount <= D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    if ( iRenderTargetCount == 0 )
        pDeviceContext->OMSetRenderTargets( 0, NULL, NULL );
    else {
        DebugAssert( arrRenderTargets != NULL );
        ID3D11RenderTargetView * arrInstances[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
        for( UInt i = 0; i < iRenderTargetCount; ++i ) {
            DebugAssert( arrRenderTargets[i] != NULL );
            DebugAssert( arrRenderTargets[i]->m_pRenderTargetView != NULL );
            arrInstances[i] = arrRenderTargets[i]->m_pRenderTargetView;
        }
        DebugAssert( pDepthStencilView != NULL );
        DebugAssert( pDepthStencilView->m_pDepthStencilView != NULL );
        pDeviceContext->OMSetRenderTargets( iRenderTargetCount, arrInstances, pDepthStencilView->m_pDepthStencilView );
    }
}
Void D3D11Renderer::OMSetUAVs( UInt iUAVSlot, UInt iUAVCount, D3D11UnorderedAccessView ** arrUAVs, const UInt * arrAppendConsumeInitCounts, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );
    DebugAssert( iUAVSlot + iUAVCount <= D3D11_PS_CS_UAV_REGISTER_COUNT );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    if ( arrUAVs == NULL )
        pDeviceContext->OMSetRenderTargetsAndUnorderedAccessViews( D3D11_KEEP_RENDER_TARGETS_AND_DEPTH_STENCIL, NULL, NULL, iUAVSlot, iUAVCount, (ID3D11UnorderedAccessView**)sm_arrNullBind, NULL );
    else {
        ID3D11UnorderedAccessView * arrInstances[D3D11_PS_CS_UAV_REGISTER_COUNT];
        for( UInt i = 0; i < iUAVCount; ++i ) {
            arrInstances[i] = NULL;
            if ( arrUAVs[i] != NULL ) {
                DebugAssert( arrUAVs[i]->m_pUnorderedAccessView != NULL );
                arrInstances[i] = arrUAVs[i]->m_pUnorderedAccessView;
            }
        }
        pDeviceContext->OMSetRenderTargetsAndUnorderedAccessViews( D3D11_KEEP_RENDER_TARGETS_AND_DEPTH_STENCIL, NULL, NULL, iUAVSlot, iUAVCount, arrInstances, arrAppendConsumeInitCounts );
    }
}

Void D3D11Renderer::Clear( D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->ClearState();
}
Void D3D11Renderer::Flush( D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->Flush();
}
Void D3D11Renderer::DestroyObjects( D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->ClearState();
    pDeviceContext->Flush();
}

Void D3D11Renderer::ClearDepthStencil( D3D11DepthStencilView * pDepthStencil, Bool bClearDepth, Bool bClearStencil, Float fDepth, Byte iStencil, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );

    UInt iFlags = 0;
    if ( bClearDepth )
        iFlags |= D3D11_CLEAR_DEPTH;
    if ( bClearStencil )
        iFlags |= D3D11_CLEAR_STENCIL;

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->ClearDepthStencilView( pDepthStencil->m_pDepthStencilView, iFlags, fDepth, iStencil );
}
Void D3D11Renderer::ClearRenderTarget( D3D11RenderTargetView * pRenderTarget, const Float arrRGBA[4], D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->ClearRenderTargetView( pRenderTarget->m_pRenderTargetView, arrRGBA );
}

Void D3D11Renderer::ClearUAVUInt( D3D11UnorderedAccessView * pUAV, const UInt arrValues[4], D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->ClearUnorderedAccessViewUint( pUAV->m_pUnorderedAccessView, arrValues );
}
Void D3D11Renderer::ClearUAVFloat( D3D11UnorderedAccessView * pUAV, const Float arrValues[4], D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->ClearUnorderedAccessViewFloat( pUAV->m_pUnorderedAccessView, arrValues );
}

Bool D3D11Renderer::GetPredication( D3D11Predicate * outPredicate, Bool * pPredicateValue, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );
    DebugAssert( outPredicate->m_pPredicate == NULL && outPredicate->m_pPredicateValue == NULL );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    outPredicate->m_pPredicate = NULL;
    outPredicate->m_pPredicateValue = pPredicateValue;
    BOOL bPredicate = FALSE;
    pDeviceContext->GetPredication( &(outPredicate->m_pPredicate), &bPredicate );
    *(outPredicate->m_pPredicateValue) = ( bPredicate != FALSE );
    outPredicate->m_pPredicate->GetDesc( (D3D11_QUERY_DESC*)&(outPredicate->m_hPredicateDesc) );

    outPredicate->m_pAsynchronous = NULL;
    HRESULT hRes = outPredicate->m_pPredicate->QueryInterface( __uuidof(ID3D11Asynchronous), (Void**)&(outPredicate->m_pAsynchronous) );
    DebugAssert( hRes == S_OK && outPredicate->m_pAsynchronous != NULL );

    return *(outPredicate->m_pPredicateValue);
}
Void D3D11Renderer::SetPredication( D3D11Predicate * pPredicate, Bool bPredicateReference, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->SetPredication( pPredicate->m_pPredicate, (bPredicateReference) ? TRUE : FALSE );
}

Void D3D11Renderer::AsyncBegin( D3D11Asynchronous * pAsynchronous, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );
    DebugAssert( pAsynchronous->IsCreated() );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->Begin( pAsynchronous->m_pAsynchronous );
}
Void D3D11Renderer::AsyncEnd( D3D11Asynchronous * pAsynchronous, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );
    DebugAssert( pAsynchronous->IsCreated() );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->End( pAsynchronous->m_pAsynchronous );
}

Void D3D11Renderer::Draw( UInt iVertexCount, UInt iBaseVertex, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->Draw( iVertexCount, iBaseVertex );
}
Void D3D11Renderer::DrawIndexed( UInt iIndexCount, UInt iBaseIndex, Int iBaseVertex, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->DrawIndexed( iIndexCount, iBaseIndex, iBaseVertex );
}

Void D3D11Renderer::DrawInstanced( UInt iInstanceCount, UInt iVerticesPerInstance, UInt iBaseInstance, UInt iBaseVertex, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->DrawInstanced( iVerticesPerInstance, iInstanceCount, iBaseVertex, iBaseInstance );
}
Void D3D11Renderer::DrawInstancedIndexed( UInt iInstanceCount, UInt iIndicesPerInstance, UInt iBaseInstance, UInt iBaseIndex, Int iBaseVertex, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->DrawIndexedInstanced( iIndicesPerInstance, iInstanceCount, iBaseIndex, iBaseVertex, iBaseInstance );
}

Void D3D11Renderer::DrawInstancedIndirect( D3D11VertexBuffer * pGeneratedPrimitives, UInt iBaseVertex, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->DrawInstancedIndirect( pGeneratedPrimitives->m_pBuffer, iBaseVertex );
}
Void D3D11Renderer::DrawInstancedIndirectIndexed( D3D11IndexBuffer * pGeneratedPrimitives, UInt iBaseIndex, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->DrawIndexedInstancedIndirect( pGeneratedPrimitives->m_pBuffer, iBaseIndex );
}

Void D3D11Renderer::DrawAuto( D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->DrawAuto();
}

Void D3D11Renderer::ExecuteCommandList( D3D11CommandList * pCommandList, Bool bRestoreContextState )
{
    DebugAssert( m_pImmediateContext != NULL );
    DebugAssert( pCommandList->m_pCommandList != NULL );

    m_pImmediateContext->ExecuteCommandList( pCommandList->m_pCommandList, (bRestoreContextState) ? TRUE : FALSE );
}

Void D3D11Renderer::Dispatch( UInt iThreadGroupX, UInt iThreadGroupY, UInt iThreadGroupZ, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->Dispatch( iThreadGroupX, iThreadGroupY, iThreadGroupZ );
}
Void D3D11Renderer::DispatchIndirect( D3D11Buffer * pGeneratedDispatch, UInt iByteOffset, D3D11DeferredContext * pContext )
{
    DebugAssert( m_pImmediateContext != NULL );

    ID3D11DeviceContext * pDeviceContext = m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->DispatchIndirect( pGeneratedDispatch->m_pBuffer, iByteOffset );
}

/////////////////////////////////////////////////////////////////////////////////

Void D3D11Renderer::_CreateDevice( Bool bFirstTime )
{
    DebugAssert( m_pDevice == NULL );

    UInt iFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT; // Needed for Direct2D operations
#ifdef _DEBUG
    iFlags |= ( D3D11_CREATE_DEVICE_DEBUG );
#endif // _DEBUG

    D3D_FEATURE_LEVEL arrFeatureLevels[2] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };

    m_pDevice = NULL;
    m_pImmediateContext = NULL;
    HRESULT hRes = D3D11CreateDevice( m_pWindow->m_pAdapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, iFlags, arrFeatureLevels, 2,
                                      D3D11_SDK_VERSION, &m_pDevice, &m_iFeatureLevel, &m_pImmediateContext );
    DebugAssert( hRes == S_OK && m_pDevice != NULL && m_pImmediateContext != NULL );
    DebugAssert( m_iFeatureLevel >= D3D_FEATURE_LEVEL_11_0 );

    if ( !bFirstTime )
        m_pCallbacks->OnRestoreDevice();
}
Void D3D11Renderer::_DestroyDevice( Bool bLastTime )
{
    DebugAssert( m_pDevice != NULL );
    DebugAssert( m_pSwapChain == NULL );

    if ( !bLastTime )
        m_pCallbacks->OnDestroyDevice();

    m_pImmediateContext->Release();
    m_pImmediateContext = NULL;

    m_pDevice->Release();
    m_pDevice = NULL;
}
Void D3D11Renderer::_CreateSwapChain( Bool bFirstTime )
{
    DebugAssert( m_pDevice != NULL );
    DebugAssert( m_pSwapChain == NULL );

    UInt iAdapterIndex = m_pWindow->m_iAdapter;
    UInt iOutputIndex = ( iAdapterIndex * D3D11WINDOW_MAX_OUTPUTS ) + m_pWindow->m_iOutput;
    UInt iDisplayModeIndex = ( iOutputIndex * D3D11WINDOW_MAX_DISPLAYMODES ) + m_pWindow->m_iDisplayMode;
    const DXGI_MODE_DESC * pDisplayMode = (const DXGI_MODE_DESC *)( m_pWindow->m_arrDisplayModes + iDisplayModeIndex );

    HRESULT hRes;

    DXGI_SWAP_CHAIN_DESC hSwapChainDesc;
    hSwapChainDesc.BufferDesc.Width = pDisplayMode->Width;
    hSwapChainDesc.BufferDesc.Height = pDisplayMode->Height;
    hSwapChainDesc.BufferDesc.RefreshRate.Numerator = pDisplayMode->RefreshRate.Numerator;
    hSwapChainDesc.BufferDesc.RefreshRate.Denominator = pDisplayMode->RefreshRate.Denominator;
    hSwapChainDesc.BufferDesc.Format = pDisplayMode->Format;
    hSwapChainDesc.BufferDesc.ScanlineOrdering = pDisplayMode->ScanlineOrdering;
    hSwapChainDesc.BufferDesc.Scaling = pDisplayMode->Scaling;

    hSwapChainDesc.SampleDesc.Count = 1;
    hSwapChainDesc.SampleDesc.Quality = 0;
    if ( m_hSwapChainDesc.iSampleCount > 1 ) {
        UInt iFmtSupport = 0;
        hRes = m_pDevice->CheckFormatSupport( pDisplayMode->Format, &iFmtSupport );
        DebugAssert( iFmtSupport & D3D11_FORMAT_SUPPORT_MULTISAMPLE_RENDERTARGET );

        UInt iMaxSamplingQuality = 0;
        hRes = m_pDevice->CheckMultisampleQualityLevels( pDisplayMode->Format, m_hSwapChainDesc.iSampleCount, &iMaxSamplingQuality );
        DebugAssert( hRes == S_OK );
        DebugAssert( iMaxSamplingQuality > 0 );

        hSwapChainDesc.SampleDesc.Count = m_hSwapChainDesc.iSampleCount;
        hSwapChainDesc.SampleDesc.Quality = (UINT)D3D11_STANDARD_MULTISAMPLE_PATTERN;
    }

    hSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    hSwapChainDesc.BufferCount = 2;

    hSwapChainDesc.OutputWindow = m_pWindow->m_hWindow;
    hSwapChainDesc.Windowed = TRUE; // Allways create windowed, switch fullscreen afterward
    
    hSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    hSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    m_pSwapChain = NULL;
    hRes = m_pWindow->m_pDXGIFactory->CreateSwapChain( m_pDevice, &hSwapChainDesc, &m_pSwapChain );
    DebugAssert( hRes == S_OK && m_pSwapChain != NULL );

    hRes = m_pWindow->m_pDXGIFactory->MakeWindowAssociation( m_pWindow->m_hWindow, DXGI_MWA_NO_ALT_ENTER | DXGI_MWA_NO_PRINT_SCREEN );
    DebugAssert( hRes == S_OK );

    _UpdateSwapChainDesc();

    if ( !bFirstTime )
        m_pCallbacks->OnRestoreSwapChain();
}
Void D3D11Renderer::_DestroySwapChain( Bool bLastTime )
{
    DebugAssert( m_pSwapChain != NULL );

    if ( !bLastTime )
        m_pCallbacks->OnDestroySwapChain();

    HRESULT hRes = m_pSwapChain->SetFullscreenState( FALSE, NULL );
    DebugAssert( hRes == S_OK );

    m_pSwapChain->Release();
    m_pSwapChain = NULL;
}
Void D3D11Renderer::_UpdateSwapChainDesc()
{
    DebugAssert( m_pSwapChain != NULL );

    DXGI_SWAP_CHAIN_DESC hSwapChainDesc;
    HRESULT hRes = m_pSwapChain->GetDesc( (DXGI_SWAP_CHAIN_DESC*)&hSwapChainDesc );
    DebugAssert( hRes == S_OK );

    m_hSwapChainDesc.ConvertFrom( &hSwapChainDesc );
}

