/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/D3D11Renderer.h
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
// Header prelude
#ifndef SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_D3D11RENDERER_H
#define SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_D3D11RENDERER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "D3D11Window.h"

#include "Resources/D3D11DeferredContext.h"

#include "Resources/D3D11Buffer.h"
#include "Resources/D3D11Texture.h"
#include "Resources/D3D11ResourceView.h"

#include "Resources/D3D11InputLayout.h"

#include "Resources/D3D11RenderState.h"

#include "Resources/D3D11Asynchronous.h"

#include "Resources/D3D11Shader.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The D3D11RendererCallbacks class
class D3D11RendererCallbacks
{
public:
    D3D11RendererCallbacks( Void * pUserData = NULL );
    virtual ~D3D11RendererCallbacks();

    inline Void SetUserData( Void * pUserData );

    // Device
    virtual Void OnDestroyDevice() = 0;
    virtual Void OnRestoreDevice() = 0;

    // SwapChain
    virtual Void OnDestroySwapChain() = 0;
    virtual Void OnRestoreSwapChain() = 0;

protected:
    Void * m_pUserData;
};

/////////////////////////////////////////////////////////////////////////////////
// The D3D11Renderer class
class D3D11Renderer
{
public:
    D3D11Renderer( D3D11Window * pWindow, D3D11RendererCallbacks * pCallbacks );
    ~D3D11Renderer();

    // Window access
    inline D3D11Window * GetWindow() const;

    // Device caps methods
    Void GetDeviceFeatures( D3D11DeviceFeatures * outFeatures ) const;
    Void GetFormatSupport( D3D11PixelFormatSupport * outFormatSupport, PixelFormat iFormat ) const;
    Void GetCounterSupport( D3D11CounterSupport * outCounterSupport ) const;

    // SwapChain methods
    inline const D3D11SwapChainDesc * GetSwapChainDesc() const;
    Void GetFrameStats( D3D11FrameStats * outFrameStats ) const;

    Void SetMultisampling( UInt iSampleCount );

    inline Bool IsIdle() const;
    Bool Present( UInt iVSyncInterval = 1, UInt iPresentFlags = 0 );

    // InputAssembler stage (VBs = 32 slots)
    Void IASetInputLayout( D3D11InputLayout * pInputLayout, D3D11DeferredContext * pContext = NULL );
    Void IASetPrimitiveTopology( D3D11ShaderPrimitiveTopology iTopology, D3D11DeferredContext * pContext = NULL );

    Void IASetVertexBuffers( UInt iBindSlot, UInt iBufferCount, D3D11VertexBuffer ** arrBuffers, const UInt * arrStrides, const UInt * arrOffsets = NULL, D3D11DeferredContext * pContext = NULL );
    Void IASetIndexBuffer( D3D11IndexBuffer * pIndexBuffer, Bool bUseShorts = false, UInt iOffset = 0, D3D11DeferredContext * pContext = NULL );
    
    // VertexShader stage (Samplers = 16 slots, ConstantBuffers = 14 slots, Resources = 128 slots)
    Void VSSetShader( D3D11VertexShader * pVertexShader, D3D11DeferredContext * pContext = NULL );
    Void VSSetConstantBuffers( UInt iBindSlot, UInt iBufferCount, D3D11ConstantBuffer ** arrBuffers, D3D11DeferredContext * pContext = NULL );
    Void VSSetSamplers( UInt iBindSlot, UInt iSamplerCount, D3D11SamplerState ** arrSamplers, D3D11DeferredContext * pContext = NULL );
    Void VSSetResources( UInt iBindSlot, UInt iResourceCount, D3D11ShaderView ** arrResources, D3D11DeferredContext * pContext = NULL );

    // GeometryShader stage (Same)
    Void GSSetShader( D3D11GeometryShader * pGeometryShader, D3D11DeferredContext * pContext = NULL );
    Void GSSetConstantBuffers( UInt iBindSlot, UInt iBufferCount, D3D11ConstantBuffer ** arrBuffers, D3D11DeferredContext * pContext = NULL );
    Void GSSetSamplers( UInt iBindSlot, UInt iSamplerCount, D3D11SamplerState ** arrSamplers, D3D11DeferredContext * pContext = NULL );
    Void GSSetResources( UInt iBindSlot, UInt iResourceCount, D3D11ShaderView ** arrResources, D3D11DeferredContext * pContext = NULL );

    // PixelShader stage (Same)
    Void PSSetShader( D3D11PixelShader * pPixelShader, D3D11DeferredContext * pContext = NULL );
    Void PSSetConstantBuffers( UInt iBindSlot, UInt iBufferCount, D3D11ConstantBuffer ** arrBuffers, D3D11DeferredContext * pContext = NULL );
    Void PSSetSamplers( UInt iBindSlot, UInt iSamplerCount, D3D11SamplerState ** arrSamplers, D3D11DeferredContext * pContext = NULL );
    Void PSSetResources( UInt iBindSlot, UInt iResourceCount, D3D11ShaderView ** arrResources, D3D11DeferredContext * pContext = NULL );

    // HullShader stage (Same)
    Void HSSetShader( D3D11HullShader * pHullShader, D3D11DeferredContext * pContext = NULL );
    Void HSSetConstantBuffers( UInt iBindSlot, UInt iBufferCount, D3D11ConstantBuffer ** arrBuffers, D3D11DeferredContext * pContext = NULL );
    Void HSSetSamplers( UInt iBindSlot, UInt iSamplerCount, D3D11SamplerState ** arrSamplers, D3D11DeferredContext * pContext = NULL );
    Void HSSetResources( UInt iBindSlot, UInt iResourceCount, D3D11ShaderView ** arrResources, D3D11DeferredContext * pContext = NULL );

    // DomainShader stage (Same)
    Void DSSetShader( D3D11DomainShader * pDomainShader, D3D11DeferredContext * pContext = NULL );
    Void DSSetConstantBuffers( UInt iBindSlot, UInt iBufferCount, D3D11ConstantBuffer ** arrBuffers, D3D11DeferredContext * pContext = NULL );
    Void DSSetSamplers( UInt iBindSlot, UInt iSamplerCount, D3D11SamplerState ** arrSamplers, D3D11DeferredContext * pContext = NULL );
    Void DSSetResources( UInt iBindSlot, UInt iResourceCount, D3D11ShaderView ** arrResources, D3D11DeferredContext * pContext = NULL );

    // ComputeShader stage (Same, UAVs = 64 slots)
    Void CSSetShader( D3D11ComputeShader * pComputeShader, D3D11DeferredContext * pContext = NULL );
    Void CSSetConstantBuffers( UInt iBindSlot, UInt iBufferCount, D3D11ConstantBuffer ** arrBuffers, D3D11DeferredContext * pContext = NULL );
    Void CSSetSamplers( UInt iBindSlot, UInt iSamplerCount, D3D11SamplerState ** arrSamplers, D3D11DeferredContext * pContext = NULL );
    Void CSSetResources( UInt iBindSlot, UInt iResourceCount, D3D11ShaderView ** arrResources, D3D11DeferredContext * pContext = NULL );
    Void CSSetUAVs( UInt iUAVSlot, UInt iUAVCount, D3D11UnorderedAccessView ** arrUAVs, const UInt * arrAppendConsumeInitCounts = NULL, D3D11DeferredContext * pContext = NULL );

    // StreamOutput stage
    Void SOSetBuffers( UInt iBufferCount, D3D11Buffer ** arrBuffers, const UInt * arrOffsets = NULL, D3D11DeferredContext * pContext = NULL );
    
    // Rasterizer stage
    Void RSSetState( D3D11RasterizerState * pRasterizerState, D3D11DeferredContext * pContext = NULL );

    Void RSSetViewports( UInt iViewportCount, const D3D11Viewport * arrViewports, D3D11DeferredContext * pContext = NULL );
    Void RSSetScissors( UInt iScissorCount, const D3D11Rectangle * arrScissors, D3D11DeferredContext * pContext = NULL );

    // OutputMerger stage (RTs = 8 slots, UAVs = 64 slots)
    Void OMSetDepthStencilState( D3D11DepthStencilState * pDepthStencilState, DWord iStencilRef = 0, D3D11DeferredContext * pContext = NULL );
    Void OMSetBlendState( D3D11BlendState * pBlendState, const Float arrBlendFactors[4] = NULL, DWord iSampleMask = 0xffffffff, D3D11DeferredContext * pContext = NULL );

    Void OMSetRenderTargets( UInt iRenderTargetCount, D3D11RenderTargetView ** arrRenderTargets, D3D11DepthStencilView * pDepthStencilView, D3D11DeferredContext * pContext = NULL );
    Void OMSetUAVs( UInt iUAVSlot, UInt iUAVCount, D3D11UnorderedAccessView ** arrUAVs, const UInt * arrAppendConsumeInitCounts = NULL, D3D11DeferredContext * pContext = NULL );

    // Clearing methods
    Void Clear( D3D11DeferredContext * pContext = NULL );
    Void Flush( D3D11DeferredContext * pContext = NULL );
    Void DestroyObjects( D3D11DeferredContext * pContext = NULL );

    Void ClearDepthStencil( D3D11DepthStencilView * pDepthStencil, Bool bClearDepth = true, Bool bClearStencil = true, Float fDepth = FLOAT_INFINITE, Byte iStencil = 0, D3D11DeferredContext * pContext = NULL );
    Void ClearRenderTarget( D3D11RenderTargetView * pRenderTarget, const Float arrRGBA[4], D3D11DeferredContext * pContext = NULL );

    Void ClearUAVUInt( D3D11UnorderedAccessView * pUAV, const UInt arrValues[4], D3D11DeferredContext * pContext = NULL );
    Void ClearUAVFloat( D3D11UnorderedAccessView * pUAV, const Float arrValues[4], D3D11DeferredContext * pContext = NULL );

    // Rendering methods
    Bool GetPredication( D3D11Predicate * outPredicate, Bool * pPredicateValue, D3D11DeferredContext * pContext = NULL );
    Void SetPredication( D3D11Predicate * pPredicate, Bool bPredicateReference, D3D11DeferredContext * pContext = NULL );

    Void AsyncBegin( D3D11Asynchronous * pAsynchronous = NULL, D3D11DeferredContext * pContext = NULL );
    Void AsyncEnd( D3D11Asynchronous * pAsynchronous = NULL, D3D11DeferredContext * pContext = NULL );

    Void Draw( UInt iVertexCount, UInt iBaseVertex = 0, D3D11DeferredContext * pContext = NULL );
    Void DrawIndexed( UInt iIndexCount, UInt iBaseIndex = 0, Int iBaseVertex = 0, D3D11DeferredContext * pContext = NULL );

    Void DrawInstanced( UInt iInstanceCount, UInt iVerticesPerInstance, UInt iBaseInstance = 0, UInt iBaseVertex = 0, D3D11DeferredContext * pContext = NULL );
    Void DrawInstancedIndexed( UInt iInstanceCount, UInt iIndicesPerInstance, UInt iBaseInstance = 0, UInt iBaseIndex = 0, Int iBaseVertex = 0, D3D11DeferredContext * pContext = NULL );

    Void DrawInstancedIndirect( D3D11VertexBuffer * pGeneratedPrimitives, UInt iBaseVertex = 0, D3D11DeferredContext * pContext = NULL );
    Void DrawInstancedIndirectIndexed( D3D11IndexBuffer * pGeneratedPrimitives, UInt iBaseIndex = 0, D3D11DeferredContext * pContext = NULL );

    Void DrawAuto( D3D11DeferredContext * pContext = NULL ); // For geometry coming from StreamOutput stage (proper InputLayout still needed)

        // Note that the immediate context is cleared before & after command list execution
    Void ExecuteCommandList( D3D11CommandList * pCommandList, Bool bRestoreContextState );

    // Parallel computing ( Cast compute shader instances )
    Void Dispatch( UInt iThreadGroupX, UInt iThreadGroupY, UInt iThreadGroupZ, D3D11DeferredContext * pContext = NULL );
    Void DispatchIndirect( D3D11Buffer * pGeneratedDispatch, UInt iByteOffset = 0, D3D11DeferredContext * pContext = NULL );

    // Remaining ... //////////////////////////////////////////////////////////////

    // ID3D11DeviceContext1::ResolveSubResource // Tex2DMS => Tex2D
    // ID3D11DeviceContext1::CopyStructureCount // UAV to buffer

    // ID3D11DeviceContext1::GenerateMips // Generate Mips for an SRV containing a RenderTarget resource(D3D11_BIND_RENDER_TARGET)
                                          // and created with D3D11_RESOURCE_MISC_GENERATE_MIPS ... (RT in SRV ?!! why not RTV ??!)

    // ID3D11Device1::GetDeviceRemovedReason (needed ?)

    ///////////////////////////////////////////////////////////////////////////////

private:
    // Resources access
    friend class D3D11DeferredContext;

    friend class D3D11InputLayout;

    friend class D3D11SamplerState;
    friend class D3D11RasterizerState;
    friend class D3D11DepthStencilState;
    friend class D3D11BlendState;

    friend class D3D11Resource;
    friend class D3D11Buffer;
    friend class D3D11VertexBuffer;
    friend class D3D11IndexBuffer;
    friend class D3D11ConstantBuffer;
    friend class D3D11RawBuffer;
    friend class D3D11StructuredBuffer;
    friend class D3D11Texture;
    friend class D3D11Texture1D;
    friend class D3D11Texture2D;
    friend class D3D11Texture3D;
    friend class D3D11TextureCube;

    friend class D3D11RenderTargetView;
    friend class D3D11DepthStencilView;
    friend class D3D11ShaderView;
    friend class D3D11UnorderedAccessView;

    friend class D3D11Query;
    friend class D3D11QueryCommandProcessing;
    friend class D3D11QueryOcclusion;
    friend class D3D11QueryTimeStampFrequency;
    friend class D3D11QueryTimeStamp;
    friend class D3D11QueryStatsPipeline;
    friend class D3D11QueryStatsStreamOutput;
    friend class D3D11Predicate;
    friend class D3D11PredicateOcclusion;
    friend class D3D11PredicateStreamOutputOverflow;
    friend class D3D11Counter;

    friend class D3D11Shader;
    friend class D3D11VertexShader;
    friend class D3D11GeometryShader;
    friend class D3D11PixelShader;
    friend class D3D11HullShader;
    friend class D3D11DomainShader;
    friend class D3D11ComputeShader;

    // Window bind
    friend class D3D11Window;
    D3D11Window * m_pWindow;

    // 2D Renderer bind
    friend class D2D1RenderingContext;

    ID2D1Factory * m_pD2D1Factory;
    IDWriteFactory * m_pDWFactory;

    // Device & SwapChain data
    Void _CreateDevice( Bool bFirstTime = false );
    Void _DestroyDevice( Bool bLastTime = false );
    Void _CreateSwapChain( Bool bFirstTime = false );
    Void _DestroySwapChain( Bool bLastTime = false );
    Void _UpdateSwapChainDesc();

    D3D11RendererCallbacks * m_pCallbacks;

    D3D_FEATURE_LEVEL m_iFeatureLevel;
    ID3D11Device * m_pDevice;
    ID3D11DeviceContext * m_pImmediateContext;

    IDXGISwapChain * m_pSwapChain;
    D3D11SwapChainDesc m_hSwapChainDesc;

    Bool m_bIdleState;

    // Helpers
#define _D3D11_RENDERER_NULL_BINDING_SIZE 256 // Make sure to use a value >= largest binding requirement
    static Void * sm_arrNullBind[_D3D11_RENDERER_NULL_BINDING_SIZE];
    static UInt sm_arrNullBindData[_D3D11_RENDERER_NULL_BINDING_SIZE];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "D3D11Renderer.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_D3D11RENDERER_H

