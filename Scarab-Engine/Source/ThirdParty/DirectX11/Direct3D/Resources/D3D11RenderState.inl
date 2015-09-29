/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/Resources/D3D11RenderState.inl
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
// D3D11RenderState implementation
inline Bool D3D11RenderState::IsCreated() const {
    return ( m_pRenderState != NULL || m_bTemporaryDestroyed );
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11SamplerState implementation
inline D3D11RenderStateType D3D11SamplerState::GetType() const {
    return D3D11RENDERSTATE_SAMPLER;
}

inline Void D3D11SamplerState::GetDesc( D3D11SamplerStateDesc * outSamplerStateDesc ) const {
    outSamplerStateDesc->ConvertFrom( &m_hSamplerDesc );
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11RasterizerState implementation
inline D3D11RenderStateType D3D11RasterizerState::GetType() const {
    return D3D11RENDERSTATE_RASTERIZER;
}

inline Void D3D11RasterizerState::GetDesc( D3D11RasterizerStateDesc * outRasterizerDesc ) const {
    outRasterizerDesc->ConvertFrom( &m_hRasterizerDesc );
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11DepthStencilState implementation
inline D3D11RenderStateType D3D11DepthStencilState::GetType() const {
    return D3D11RENDERSTATE_DEPTHSTENCIL;
}

inline Void D3D11DepthStencilState::GetDesc( D3D11DepthStencilStateDesc * outDepthStencilDesc ) const {
    outDepthStencilDesc->ConvertFrom( &m_hDepthStencilDesc );
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11BlendState implementation
inline D3D11RenderStateType D3D11BlendState::GetType() const {
    return D3D11RENDERSTATE_BLEND;
}

inline Void D3D11BlendState::GetDesc( D3D11BlendStateDesc * outBlendDesc ) const {
    outBlendDesc->ConvertFrom( &m_hBlendDesc );
}

