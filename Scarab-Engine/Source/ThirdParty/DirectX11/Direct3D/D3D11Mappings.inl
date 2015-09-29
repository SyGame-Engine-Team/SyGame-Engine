/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/D3D11Mappings.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : API-dependant mappings for Win32, GDI, DXGI & Direct3D
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Win32/GDI Definitions

/////////////////////////////////////////////////////////////////////////////////
// D3D11Window Definitions
inline Void D3D11AdapterDesc::ConvertFrom( const __DXGI_ADAPTER_DESC * pDesc ) {
    ConvertFrom( (const DXGI_ADAPTER_DESC *)pDesc, pDesc->iEnumIndex );
}
inline Void D3D11AdapterDesc::ConvertTo( __DXGI_ADAPTER_DESC * outDesc ) const {
    ConvertTo( (DXGI_ADAPTER_DESC*)outDesc, &(outDesc->iEnumIndex) );
}

inline Void D3D11OutputDesc::ConvertFrom( const __DXGI_OUTPUT_DESC * pDesc ) {
    ConvertFrom( (const DXGI_OUTPUT_DESC *)pDesc, pDesc->iEnumIndex );
}
inline Void D3D11OutputDesc::ConvertTo( __DXGI_OUTPUT_DESC * outDesc ) const {
    ConvertTo( (DXGI_OUTPUT_DESC*)outDesc, &(outDesc->iEnumIndex) );
}

inline Void D3D11DisplayModeDesc::ConvertFrom( const __DXGI_MODE_DESC * pDesc, UInt iDisplayMode ) {
    ConvertFrom( (const DXGI_MODE_DESC *)pDesc, iDisplayMode );
}
inline Void D3D11DisplayModeDesc::ConvertTo( __DXGI_MODE_DESC * outDesc, UInt * outDisplayMode ) const {
    ConvertTo( (DXGI_MODE_DESC*)outDesc, outDisplayMode );
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11Renderer Definitions

/////////////////////////////////////////////////////////////////////////////////
// D3D11DeferredContext Definitions

/////////////////////////////////////////////////////////////////////////////////
// D3D11InputLayout Definitions
inline D3D11InputFieldType D3D11InputFieldTypeFromD3D11( DXGI_FORMAT iDXGIFormat ) {
    return (D3D11InputFieldType)( PixelFormatFromDXGI[iDXGIFormat] );
}
inline DXGI_FORMAT D3D11InputFieldTypeToD3D11( D3D11InputFieldType iInputFieldType ) {
    return PixelFormatToDXGI[ (PixelFormat)iInputFieldType ];
}

inline Void D3D11InputFieldDesc::ConvertFrom( const __D3D11_INPUT_ELEMENT_DESC * pDesc ) {
    ConvertFrom( (const D3D11_INPUT_ELEMENT_DESC *)pDesc );
}
inline Void D3D11InputFieldDesc::ConvertTo( __D3D11_INPUT_ELEMENT_DESC * outDesc ) const {
    ConvertTo( (D3D11_INPUT_ELEMENT_DESC*)outDesc );
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11RenderState Definitions
inline D3D11SamplerFilterMode D3D11SamplerFilterModeFromD3D11( D3D11_FILTER iD3D11Filter ) {
    return (D3D11SamplerFilterMode)( _D3D11ConvertFlags32(D3D11SamplerFilterFlagsFromD3D11, iD3D11Filter) );
}
inline D3D11_FILTER D3D11SamplerFilterModeToD3D11( D3D11SamplerFilterMode iFilterMode ) {
    return (D3D11_FILTER)( _D3D11ConvertFlags32(D3D11SamplerFilterFlagsToD3D11, iFilterMode) );
}

inline Void D3D11SamplerStateDesc::ConvertFrom( const __D3D11_SAMPLER_DESC * pDesc ) {
    ConvertFrom( (const D3D11_SAMPLER_DESC *)pDesc );
}
inline Void D3D11SamplerStateDesc::ConvertTo( __D3D11_SAMPLER_DESC * outDesc ) const {
    ConvertTo( (D3D11_SAMPLER_DESC*)outDesc );
}

inline Void D3D11RasterizerStateDesc::ConvertFrom( const __D3D11_RASTERIZER_DESC * pDesc ) {
    ConvertFrom( (const D3D11_RASTERIZER_DESC *)pDesc );
}
inline Void D3D11RasterizerStateDesc::ConvertTo( __D3D11_RASTERIZER_DESC * outDesc ) const {
    ConvertTo( (D3D11_RASTERIZER_DESC*)outDesc );
}

inline Void D3D11DepthStencilStateDesc::ConvertFrom( const __D3D11_DEPTH_STENCIL_DESC * pDesc ) {
    ConvertFrom( (const D3D11_DEPTH_STENCIL_DESC *)pDesc );
}
inline Void D3D11DepthStencilStateDesc::ConvertTo( __D3D11_DEPTH_STENCIL_DESC * outDesc ) const {
    ConvertTo( (D3D11_DEPTH_STENCIL_DESC*)outDesc );
}

inline Void D3D11BlendStateDesc::ConvertFrom( const __D3D11_BLEND_DESC * pDesc ) {
    ConvertFrom( (const D3D11_BLEND_DESC *)pDesc );
}
inline Void D3D11BlendStateDesc::ConvertTo( __D3D11_BLEND_DESC * outDesc ) const {
    ConvertTo( (D3D11_BLEND_DESC*)outDesc );
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11Asynchronous Definitions

/////////////////////////////////////////////////////////////////////////////////
// D3D11Resource Definitions
inline D3D11ResourcePriority D3D11ResourcePriorityFromD3D11( DWord iD3D11ResourcePriority ) {
    DWord iRes = 0;
    iD3D11ResourcePriority = ( iD3D11ResourcePriority >> 24 );
    if ( iD3D11ResourcePriority & 0x28 ) {
        iD3D11ResourcePriority &= 0xd7; // ~(0x28)
        ++iRes;
    }
    iRes += ( iD3D11ResourcePriority / 0x25 ) - 1;
    return (D3D11ResourcePriority)iRes;
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11Buffer Definitions

/////////////////////////////////////////////////////////////////////////////////
// D3D11Texture Definitions

/////////////////////////////////////////////////////////////////////////////////
// D3D11ResourceView Definitions
inline Void D3D11RenderTargetViewDesc::ConvertFrom( const __D3D11_RENDER_TARGET_VIEW_DESC * pDesc ) {
    ConvertFrom( (const D3D11_RENDER_TARGET_VIEW_DESC *)pDesc );
}
inline Void D3D11RenderTargetViewDesc::ConvertTo( __D3D11_RENDER_TARGET_VIEW_DESC * outDesc ) const {
    ConvertTo( (D3D11_RENDER_TARGET_VIEW_DESC*)outDesc );
}

inline Void D3D11DepthStencilViewDesc::ConvertFrom( const __D3D11_DEPTH_STENCIL_VIEW_DESC * pDesc ) {
    ConvertFrom( (const D3D11_DEPTH_STENCIL_VIEW_DESC *)pDesc );
}
inline Void D3D11DepthStencilViewDesc::ConvertTo( __D3D11_DEPTH_STENCIL_VIEW_DESC * outDesc ) const {
    ConvertTo( (D3D11_DEPTH_STENCIL_VIEW_DESC*)outDesc );
}

inline Void D3D11ShaderViewDesc::ConvertFrom( const __D3D11_SHADER_RESOURCE_VIEW_DESC * pDesc ) {
    ConvertFrom( (const D3D11_SHADER_RESOURCE_VIEW_DESC *)pDesc );
}
inline Void D3D11ShaderViewDesc::ConvertTo( __D3D11_SHADER_RESOURCE_VIEW_DESC * outDesc ) const {
    ConvertTo( (D3D11_SHADER_RESOURCE_VIEW_DESC*)outDesc );
}

inline Void D3D11UnorderedAccessViewDesc::ConvertFrom( const __D3D11_UNORDERED_ACCESS_VIEW_DESC * pDesc ) {
    ConvertFrom( (const D3D11_UNORDERED_ACCESS_VIEW_DESC *)pDesc );
}
inline Void D3D11UnorderedAccessViewDesc::ConvertTo( __D3D11_UNORDERED_ACCESS_VIEW_DESC * outDesc ) const {
    ConvertTo( (D3D11_UNORDERED_ACCESS_VIEW_DESC*)outDesc );
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11Shader Definitions
inline Void D3D11StreamOutputDeclaration::ConvertFrom( const __D3D11_SO_DECLARATION_ENTRY * pDesc ) {
    ConvertFrom( (const D3D11_SO_DECLARATION_ENTRY *)pDesc );
}
inline Void D3D11StreamOutputDeclaration::ConvertTo( __D3D11_SO_DECLARATION_ENTRY * outDesc ) const {
    ConvertTo( (D3D11_SO_DECLARATION_ENTRY*)outDesc );
}



