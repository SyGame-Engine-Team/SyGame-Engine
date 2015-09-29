/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/Resources/D3D11RenderState.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU resources : Render states.
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
#include "D3D11RenderState.h"

#include "../D3D11Renderer.h"

/////////////////////////////////////////////////////////////////////////////////
// D3D11RenderState implementation
D3D11RenderState::D3D11RenderState( D3D11Renderer * pRenderer )
{
    m_pRenderer = pRenderer;

    m_pRenderState = NULL;

    m_bTemporaryDestroyed = false;
}
D3D11RenderState::~D3D11RenderState()
{
    // nothing to do
}

Void D3D11RenderState::Destroy()
{
    DebugAssert( IsCreated() );

    if ( m_bTemporaryDestroyed )
        m_bTemporaryDestroyed = false;
    else
        _NakedDestroy();
}

Void D3D11RenderState::OnDestroyDevice()
{
    DebugAssert( !m_bTemporaryDestroyed );

    if ( m_pRenderState != NULL ) {
        _NakedDestroy();
        m_bTemporaryDestroyed = true;
    }
}
Void D3D11RenderState::OnRestoreDevice()
{
    DebugAssert( m_pRenderState == NULL );

    if ( m_bTemporaryDestroyed ) {
        _NakedCreate();
        m_bTemporaryDestroyed = false;
    }
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11SamplerState implementation
D3D11SamplerState::D3D11SamplerState( D3D11Renderer * pRenderer ):
    D3D11RenderState( pRenderer )
{
    m_hSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    m_hSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    m_hSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    m_hSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    m_hSamplerDesc.MipLODBias = 0.0f;
    m_hSamplerDesc.MaxAnisotropy = 1;
    m_hSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    m_hSamplerDesc.BorderColor[0] = 1.0f;
    m_hSamplerDesc.BorderColor[1] = 1.0f;
    m_hSamplerDesc.BorderColor[2] = 1.0f;
    m_hSamplerDesc.BorderColor[3] = 1.0f;
    m_hSamplerDesc.MinLOD = -FLOAT_INFINITE;
    m_hSamplerDesc.MaxLOD = FLOAT_INFINITE;

    m_pSamplerState = NULL;
}
D3D11SamplerState::~D3D11SamplerState()
{
    if ( IsCreated() )
        Destroy();
}

Void D3D11SamplerState::Create( const D3D11SamplerStateDesc * pSamplerDesc )
{
    DebugAssert( !(IsCreated()) );

    pSamplerDesc->ConvertTo( &m_hSamplerDesc );

    _NakedCreate();
}
Void D3D11SamplerState::Destroy()
{
    D3D11RenderState::Destroy();

    m_hSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    m_hSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    m_hSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    m_hSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    m_hSamplerDesc.MipLODBias = 0.0f;
    m_hSamplerDesc.MaxAnisotropy = 1;
    m_hSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    m_hSamplerDesc.BorderColor[0] = 1.0f;
    m_hSamplerDesc.BorderColor[1] = 1.0f;
    m_hSamplerDesc.BorderColor[2] = 1.0f;
    m_hSamplerDesc.BorderColor[3] = 1.0f;
    m_hSamplerDesc.MinLOD = -FLOAT_INFINITE;
    m_hSamplerDesc.MaxLOD = FLOAT_INFINITE;
}

/////////////////////////////////////////////////////////////////////////////////

Void D3D11SamplerState::_NakedCreate()
{
    m_pSamplerState = NULL;
    HRESULT hRes = m_pRenderer->m_pDevice->CreateSamplerState( (const D3D11_SAMPLER_DESC *)&m_hSamplerDesc, &m_pSamplerState );
    DebugAssert( hRes == S_OK && m_pSamplerState != NULL );

    m_pRenderState = NULL;
    hRes = m_pSamplerState->QueryInterface( __uuidof(ID3D11DeviceChild), (Void**)&m_pRenderState );
    DebugAssert( hRes == S_OK && m_pRenderState != NULL );
}
Void D3D11SamplerState::_NakedDestroy()
{
    m_pRenderState->Release();
    m_pRenderState = NULL;

    m_pSamplerState->Release();
    m_pSamplerState = NULL;
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11RasterizerState implementation
D3D11RasterizerState::D3D11RasterizerState( D3D11Renderer * pRenderer ):
    D3D11RenderState( pRenderer )
{
    m_hRasterizerDesc.FillMode = D3D11_FILL_SOLID;
    m_hRasterizerDesc.CullMode = D3D11_CULL_BACK;
    m_hRasterizerDesc.FrontCounterClockwise = TRUE;
    m_hRasterizerDesc.DepthBias = 0;
    m_hRasterizerDesc.DepthBiasClamp = 0.0f;
    m_hRasterizerDesc.SlopeScaledDepthBias = 0.0f;
    m_hRasterizerDesc.DepthClipEnable = TRUE;
    m_hRasterizerDesc.ScissorEnable = FALSE;
    m_hRasterizerDesc.MultisampleEnable = FALSE;
    m_hRasterizerDesc.AntialiasedLineEnable = FALSE;

    m_pRasterizerState = NULL;
}
D3D11RasterizerState::~D3D11RasterizerState()
{
    if ( IsCreated() )
        Destroy();
}

Void D3D11RasterizerState::Create( const D3D11RasterizerStateDesc * pRasterizerDesc )
{
    DebugAssert( !(IsCreated()) );

    pRasterizerDesc->ConvertTo( &m_hRasterizerDesc );

    _NakedCreate();
}
Void D3D11RasterizerState::Destroy()
{
    D3D11RenderState::Destroy();

    m_hRasterizerDesc.FillMode = D3D11_FILL_SOLID;
    m_hRasterizerDesc.CullMode = D3D11_CULL_BACK;
    m_hRasterizerDesc.FrontCounterClockwise = TRUE;
    m_hRasterizerDesc.DepthBias = 0;
    m_hRasterizerDesc.DepthBiasClamp = 0.0f;
    m_hRasterizerDesc.SlopeScaledDepthBias = 0.0f;
    m_hRasterizerDesc.DepthClipEnable = TRUE;
    m_hRasterizerDesc.ScissorEnable = FALSE;
    m_hRasterizerDesc.MultisampleEnable = FALSE;
    m_hRasterizerDesc.AntialiasedLineEnable = FALSE;
}

/////////////////////////////////////////////////////////////////////////////////

Void D3D11RasterizerState::_NakedCreate()
{
    m_pRasterizerState = NULL;
    HRESULT hRes = m_pRenderer->m_pDevice->CreateRasterizerState( (const D3D11_RASTERIZER_DESC *)&m_hRasterizerDesc, &m_pRasterizerState );
    DebugAssert( hRes == S_OK && m_pRasterizerState != NULL );

    m_pRenderState = NULL;
    hRes = m_pRasterizerState->QueryInterface( __uuidof(ID3D11DeviceChild), (Void**)&m_pRenderState );
    DebugAssert( hRes == S_OK && m_pRenderState != NULL );
}
Void D3D11RasterizerState::_NakedDestroy()
{
    m_pRenderState->Release();
    m_pRenderState = NULL;

    m_pRasterizerState->Release();
    m_pRasterizerState = NULL;
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11DepthStencilState implementation
D3D11DepthStencilState::D3D11DepthStencilState( D3D11Renderer * pRenderer ):
    D3D11RenderState( pRenderer )
{
    m_hDepthStencilDesc.DepthEnable = TRUE;
    m_hDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    m_hDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
    m_hDepthStencilDesc.StencilEnable = FALSE;
    m_hDepthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
    m_hDepthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
    m_hDepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    m_hDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    m_hDepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    m_hDepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    m_hDepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    m_hDepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    m_hDepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    m_hDepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

    m_pDepthStencilState = NULL;
}
D3D11DepthStencilState::~D3D11DepthStencilState()
{
    if ( IsCreated() )
        Destroy();
}

Void D3D11DepthStencilState::Create( const D3D11DepthStencilStateDesc * pDepthStencilDesc )
{
    DebugAssert( !(IsCreated()) );

    pDepthStencilDesc->ConvertTo( &m_hDepthStencilDesc );

    _NakedCreate();
}
Void D3D11DepthStencilState::Destroy()
{
    D3D11RenderState::Destroy();

    m_hDepthStencilDesc.DepthEnable = TRUE;
    m_hDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    m_hDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
    m_hDepthStencilDesc.StencilEnable = FALSE;
    m_hDepthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
    m_hDepthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
    m_hDepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    m_hDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    m_hDepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    m_hDepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    m_hDepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    m_hDepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    m_hDepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    m_hDepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
}

/////////////////////////////////////////////////////////////////////////////////

Void D3D11DepthStencilState::_NakedCreate()
{
    m_pDepthStencilState = NULL;
    HRESULT hRes = m_pRenderer->m_pDevice->CreateDepthStencilState( (const D3D11_DEPTH_STENCIL_DESC *)&m_hDepthStencilDesc, &m_pDepthStencilState );
    DebugAssert( hRes == S_OK && m_pDepthStencilState != NULL );

    m_pRenderState = NULL;
    hRes = m_pDepthStencilState->QueryInterface( __uuidof(ID3D11DeviceChild), (Void**)&m_pRenderState );
    DebugAssert( hRes == S_OK && m_pRenderState != NULL );
}
Void D3D11DepthStencilState::_NakedDestroy()
{
    m_pRenderState->Release();
    m_pRenderState = NULL;

    m_pDepthStencilState->Release();
    m_pDepthStencilState = NULL;
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11BlendState implementation
D3D11BlendState::D3D11BlendState( D3D11Renderer * pRenderer ):
    D3D11RenderState( pRenderer )
{
    m_hBlendDesc.AlphaToCoverageEnable = FALSE;
    m_hBlendDesc.IndependentBlendEnable = FALSE;
    for( UInt i = 0; i < D3D11RENDERER_MAX_RENDERTARGET_SLOTS; ++i ) {
        m_hBlendDesc.RenderTarget[i].BlendEnable = FALSE;
        m_hBlendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_ONE;
        m_hBlendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_ZERO;
        m_hBlendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
        m_hBlendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
        m_hBlendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
        m_hBlendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        m_hBlendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    }

    m_pBlendState = NULL;
}
D3D11BlendState::~D3D11BlendState()
{
    if ( IsCreated() )
        Destroy();
}

Void D3D11BlendState::Create( const D3D11BlendStateDesc * pBlendDesc )
{
    DebugAssert( !(IsCreated()) );

    pBlendDesc->ConvertTo( &m_hBlendDesc );

    _NakedCreate();
}
Void D3D11BlendState::Destroy()
{
    D3D11RenderState::Destroy();

    m_hBlendDesc.AlphaToCoverageEnable = FALSE;
    m_hBlendDesc.IndependentBlendEnable = FALSE;
    for( UInt i = 0; i < D3D11RENDERER_MAX_RENDERTARGET_SLOTS; ++i ) {
        m_hBlendDesc.RenderTarget[i].BlendEnable = FALSE;
        m_hBlendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_ONE;
        m_hBlendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_ZERO;
        m_hBlendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
        m_hBlendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
        m_hBlendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
        m_hBlendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        m_hBlendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    }
}

/////////////////////////////////////////////////////////////////////////////////

Void D3D11BlendState::_NakedCreate()
{
    m_pBlendState = NULL;
    HRESULT hRes = m_pRenderer->m_pDevice->CreateBlendState( (const D3D11_BLEND_DESC *)&m_hBlendDesc, &m_pBlendState );
    DebugAssert( hRes == S_OK && m_pBlendState != NULL );

    m_pRenderState = NULL;
    hRes = m_pBlendState->QueryInterface( __uuidof(ID3D11DeviceChild), (Void**)&m_pRenderState );
    DebugAssert( hRes == S_OK && m_pRenderState != NULL );
}
Void D3D11BlendState::_NakedDestroy()
{
    m_pRenderState->Release();
    m_pRenderState = NULL;

    m_pBlendState->Release();
    m_pBlendState = NULL;
}
