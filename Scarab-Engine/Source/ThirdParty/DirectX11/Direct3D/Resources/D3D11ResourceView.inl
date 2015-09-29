/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/Resources/D3D11ResourceView.inl
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
// D3D11ResourceView implementation
inline Bool D3D11ResourceView::IsCreated() const {
    return ( m_pView != NULL || m_bTemporaryDestroyed );
}
inline Bool D3D11ResourceView::IsBoundToBackBuffer( UInt * outBackBuffer ) const {
    DebugAssert( IsCreated() );
    if ( m_iBoundToBackBuffer == INVALID_OFFSET && m_pResource->GetType() == D3D11RESOURCE_TEXTURE_2D )
        return ((D3D11Texture2D*)m_pResource)->IsBoundToBackBuffer( outBackBuffer );
    if ( outBackBuffer != NULL )
        *outBackBuffer = m_iBoundToBackBuffer;
    return ( m_iBoundToBackBuffer != INVALID_OFFSET );
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11RenderTargetView implementation
inline D3D11ResourceViewType D3D11RenderTargetView::GetType() const {
    return D3D11RESOURCEVIEW_RENDER_TARGET;
}

inline Void D3D11RenderTargetView::GetDesc( D3D11RenderTargetViewDesc * outDesc ) const {
    DebugAssert( IsCreated() );
    outDesc->ConvertFrom( &m_hRTVDesc );
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11DepthStencilView implementation
inline D3D11ResourceViewType D3D11DepthStencilView::GetType() const {
    return D3D11RESOURCEVIEW_DEPTH_STENCIL;
}

inline Void D3D11DepthStencilView::GetDesc( D3D11DepthStencilViewDesc * outDesc ) const {
    DebugAssert( IsCreated() );
    outDesc->ConvertFrom( &m_hDSVDesc );
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11ShaderView implementation
inline D3D11ResourceViewType D3D11ShaderView::GetType() const {
    return D3D11RESOURCEVIEW_SHADER;
}

inline Void D3D11ShaderView::GetDesc( D3D11ShaderViewDesc * outDesc ) const {
    DebugAssert( IsCreated() );
    outDesc->ConvertFrom( &m_hSRVDesc );
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11UnorderedAccessView implementation
inline D3D11ResourceViewType D3D11UnorderedAccessView::GetType() const {
    return D3D11RESOURCEVIEW_UNORDERED_ACCESS;
}

inline Void D3D11UnorderedAccessView::GetDesc( D3D11UnorderedAccessViewDesc * outDesc ) const {
    DebugAssert( IsCreated() );
    outDesc->ConvertFrom( &m_hUAVDesc );
}

