/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/RenderingManager.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : The Core-Class of the Rendering system.
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
#ifndef SCARAB_ENGINE_RENDERING_RENDERINGMANAGER_H
#define SCARAB_ENGINE_RENDERING_RENDERINGMANAGER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Lib/Object/Patterns/Manager.h"

#include "../../Lib/Datastruct/Tree/RedBlackTree.h"

#include "../Resource/ResourceManager.h"

#include "DeviceRenderer.h"

#include "Resources/GPUDeferredContext.h"
#include "Resources/GPUInputLayout.h"
#include "Resources/GPURenderState.h"
#include "Resources/GPUAsynchronous.h"

#include "Resources/GPUResource.h"
#include "Resources/GPUBuffer.h"
#include "Resources/GPUTexture.h"
#include "Resources/GPUResourceView.h"
#include "Resources/GPUShader.h"

#include "2D/GPU2DContext.h"

#include "Resources/Mesh.h"
#include "Resources/Camera.h"

#include "Resources/Light.h"
#include "Resources/Material.h"

#include "Effects/Effect.h"
#include "Effects/EffectInstance.h"

#include "Effects/EffectLibrary/EffectDefault.h"

#include "Effects/EffectLibrary/Basic/EffectColor.h"
#include "Effects/EffectLibrary/Basic/EffectTexture.h"

#include "Effects/EffectLibrary/Lighting/EffectMaterial.h"
#include "Effects/EffectLibrary/Lighting/EffectMaterialTexture.h"
#include "Effects/EffectLibrary/Lighting/EffectLightMaterial.h"
#include "Effects/EffectLibrary/Lighting/EffectLightMaterialTexture.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define RenderingFn RenderingManager::GetInstancePtr()

#define GPURENDERER_MAX_VERTEXBUFFER_SLOTS     DEVICERENDERER_MAX_VERTEXBUFFER_SLOTS
#define GPURENDERER_MAX_CONSTANTBUFFER_SLOTS   DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS
#define GPURENDERER_MAX_SHADERSAMPLER_SLOTS    DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS
#define GPURENDERER_MAX_SHADERRESOURCE_SLOTS   DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS
#define GPURENDERER_MAX_STREAMOUTPUT_SLOTS     DEVICERENDERER_MAX_STREAMOUTPUT_SLOTS
#define GPURENDERER_MAX_VIEWPORT_SCISSOR_SLOTS DEVICERENDERER_MAX_VIEWPORT_SCISSOR_SLOTS
#define GPURENDERER_MAX_RENDERTARGET_SLOTS     DEVICERENDERER_MAX_RENDERTARGET_SLOTS
#define GPURENDERER_MAX_SHADERUAV_SLOTS        DEVICERENDERER_MAX_SHADERUAV_SLOTS
#define GPURENDERER_MAX_OUTPUTUAV_SLOTS        DEVICERENDERER_MAX_OUTPUTUAV_SLOTS

#define GPURENDERER_MAX_SWAPCHAIN_BUFFERS         DEVICERENDERER_MAX_SWAPCHAIN_BUFFERS
#define GPURENDERER_MAX_STREAMOUTPUT_DECLARATIONS DEVICERENDERER_MAX_STREAMOUTPUT_DECLARATIONS

// Display definitions
typedef struct _gpu_adapter_desc {
    UInt iIndex;
    
    Int64 iAdapterUID;
    GChar strDescription[128];
    UInt iVendorId;
    UInt iDeviceId;
    UInt iSubSysId;
    UInt iRevision;

    UInt iDedicatedVideoMemory;
    UInt iDedicatedSystemMemory;
    UInt iSharedSystemMemory;
} GPUAdapterDesc;

enum GPUOutputRotation {
    GPUOUTPUT_ROTATION_IDENTITY = DEVICEOUTPUT_ROTATION_IDENTITY,
    GPUOUTPUT_ROTATION_90       = DEVICEOUTPUT_ROTATION_90,
    GPUOUTPUT_ROTATION_180      = DEVICEOUTPUT_ROTATION_180,
    GPUOUTPUT_ROTATION_270      = DEVICEOUTPUT_ROTATION_270
};

typedef struct _gpu_output_desc {
    UInt iIndex;
    
    GChar strDeviceName[32];
    Void * pMonitor;

    GPUOutputRotation iRotation;

    Bool bAttachedToDesktop;
    Long iDesktopLeft;
    Long iDesktopRight;
    Long iDesktopTop;
    Long iDesktopBottom;
} GPUOutputDesc;

enum GPUDisplayModeScanlineOrdering {
    GPUDISPLAYMODE_SCANLINE_UNDEFINED         = DEVICEDISPLAYMODE_SCANLINE_UNDEFINED,
    GPUDISPLAYMODE_SCANLINE_PROGRESSIVE       = DEVICEDISPLAYMODE_SCANLINE_PROGRESSIVE,
    GPUDISPLAYMODE_SCANLINE_UPPER_FIELD_FIRST = DEVICEDISPLAYMODE_SCANLINE_UPPER_FIELD_FIRST,
    GPUDISPLAYMODE_SCANLINE_LOWER_FIELD_FIRST = DEVICEDISPLAYMODE_SCANLINE_LOWER_FIELD_FIRST
};
enum GPUDisplayModeScaling {
    GPUDISPLAYMODE_SCALING_UNDEFINED = DEVICEDISPLAYMODE_SCALING_UNDEFINED,
    GPUDISPLAYMODE_SCALING_CENTERED  = DEVICEDISPLAYMODE_SCALING_CENTERED,
    GPUDISPLAYMODE_SCALING_STRETCHED = DEVICEDISPLAYMODE_SCALING_STRETCHED
};

typedef struct _gpu_displaymode_desc {
    UInt iIndex;

    UInt iWidth;
    UInt iHeight;
    PixelFormat iFormat;
    UInt iRefreshRateNumerator;
    UInt iRefreshRateDenominator;

    GPUDisplayModeScanlineOrdering iScanlineOrdering;
    GPUDisplayModeScaling iScaling;
} GPUDisplayModeDesc;

typedef struct _gpu_gamma_caps {
    Bool bScaleAndOffsetSupported;
    Float fMaxConvertedValue;
    Float fMinConvertedValue;

    UInt iControlPointCount;
    Float arrControlPoints[1025];
} GPUGammaCaps;

typedef struct _gpu_gamma_control {
    struct _RGB { Float R; Float G; Float B; };
    _RGB vScale;
    _RGB vOffset;
    _RGB arrGammaCurve[1025];
} GPUGammaControl;

// Device definitions
typedef struct _gpu_device_features {
    // Threading
    Bool bDriverConcurrentCreates;
    Bool bDriverCommandLists;

    // Batching
    Bool bTileBasedDeferredRenderer;

    // Doubles
    Bool bDoublePrecisionShaderOps;

    // MinPrecision support
    Bool bPixelShaderMinPrecision10Bits;
    Bool bPixelShaderMinPrecision16Bits;
    Bool bAllOtherShaderStagesMinPrecision10Bits;
    Bool bAllOtherShaderStagesMinPrecision16Bits;

    // D3D9 hardware options
    Bool bFullNonPow2TextureSupport;
    Bool bSupportsDepthAsTextureWithLessEqualComparisonFilter;

    // D3D10 hardware options
    Bool bComputeShaders_Plus_RawAndStructuredBuffers_Via_Shader_4_x;

    // D3D11 hardware options
    Bool bOutputMergerLogicOp;
    Bool bUAVOnlyRenderingForcedSampleCount;
    Bool bDiscardAPIsSeenByDriver;
    Bool bFlagsForUpdateAndCopySeenByDriver;
    Bool bClearView;
    Bool bCopyWithOverlap;
    Bool bConstantBufferPartialUpdate;
    Bool bConstantBufferOffsetting;
    Bool bMapNoOverwriteOnDynamicConstantBuffer;
    Bool bMapNoOverwriteOnDynamicBufferSRV;
    Bool bMultisampleRTVWithForcedSampleCountOne;
    Bool bSAD4ShaderInstructions;
    Bool bExtendedDoublesShaderInstructions;
    Bool bExtendedResourceSharing;
} GPUDeviceFeatures;

enum GPUPixelFormatSupportFlags1 {
    GPUPIXELFORMAT_SUPPORT1_NONE                        = DEVICEPIXELFORMAT_SUPPORT1_NONE,
    GPUPIXELFORMAT_SUPPORT1_BUFFER                      = DEVICEPIXELFORMAT_SUPPORT1_BUFFER,
    GPUPIXELFORMAT_SUPPORT1_IA_VERTEX_BUFFER            = DEVICEPIXELFORMAT_SUPPORT1_IA_VERTEX_BUFFER,
    GPUPIXELFORMAT_SUPPORT1_IA_INDEX_BUFFER             = DEVICEPIXELFORMAT_SUPPORT1_IA_INDEX_BUFFER,
    GPUPIXELFORMAT_SUPPORT1_SO_BUFFER                   = DEVICEPIXELFORMAT_SUPPORT1_SO_BUFFER,
    GPUPIXELFORMAT_SUPPORT1_TEXTURE1D                   = DEVICEPIXELFORMAT_SUPPORT1_TEXTURE1D,
    GPUPIXELFORMAT_SUPPORT1_TEXTURE2D                   = DEVICEPIXELFORMAT_SUPPORT1_TEXTURE2D,
    GPUPIXELFORMAT_SUPPORT1_TEXTURE3D                   = DEVICEPIXELFORMAT_SUPPORT1_TEXTURE3D,
    GPUPIXELFORMAT_SUPPORT1_TEXTURECUBE                 = DEVICEPIXELFORMAT_SUPPORT1_TEXTURECUBE,
    GPUPIXELFORMAT_SUPPORT1_SHADER_LOAD                 = DEVICEPIXELFORMAT_SUPPORT1_SHADER_LOAD,
    GPUPIXELFORMAT_SUPPORT1_SHADER_SAMPLE               = DEVICEPIXELFORMAT_SUPPORT1_SHADER_SAMPLE,
    GPUPIXELFORMAT_SUPPORT1_SHADER_SAMPLE_COMPARISON    = DEVICEPIXELFORMAT_SUPPORT1_SHADER_SAMPLE_COMPARISON,
    GPUPIXELFORMAT_SUPPORT1_SHADER_SAMPLE_MONO_TEXT     = DEVICEPIXELFORMAT_SUPPORT1_SHADER_SAMPLE_MONO_TEXT,
    GPUPIXELFORMAT_SUPPORT1_MIP                         = DEVICEPIXELFORMAT_SUPPORT1_MIP,
    GPUPIXELFORMAT_SUPPORT1_MIP_AUTOGEN                 = DEVICEPIXELFORMAT_SUPPORT1_MIP_AUTOGEN,
    GPUPIXELFORMAT_SUPPORT1_RENDER_TARGET               = DEVICEPIXELFORMAT_SUPPORT1_RENDER_TARGET,
    GPUPIXELFORMAT_SUPPORT1_BLENDABLE                   = DEVICEPIXELFORMAT_SUPPORT1_BLENDABLE,
    GPUPIXELFORMAT_SUPPORT1_DEPTH_STENCIL               = DEVICEPIXELFORMAT_SUPPORT1_DEPTH_STENCIL,
    GPUPIXELFORMAT_SUPPORT1_CPU_LOCKABLE                = DEVICEPIXELFORMAT_SUPPORT1_CPU_LOCKABLE,
    GPUPIXELFORMAT_SUPPORT1_MULTISAMPLE_RESOLVE         = DEVICEPIXELFORMAT_SUPPORT1_MULTISAMPLE_RESOLVE,
    GPUPIXELFORMAT_SUPPORT1_DISPLAY                     = DEVICEPIXELFORMAT_SUPPORT1_DISPLAY,
    GPUPIXELFORMAT_SUPPORT1_CAST_WITHIN_BIT_LAYOUT      = DEVICEPIXELFORMAT_SUPPORT1_CAST_WITHIN_BIT_LAYOUT,
    GPUPIXELFORMAT_SUPPORT1_MULTISAMPLE_RENDERTARGET    = DEVICEPIXELFORMAT_SUPPORT1_MULTISAMPLE_RENDERTARGET,
    GPUPIXELFORMAT_SUPPORT1_MULTISAMPLE_LOAD            = DEVICEPIXELFORMAT_SUPPORT1_MULTISAMPLE_LOAD,
    GPUPIXELFORMAT_SUPPORT1_SHADER_GATHER               = DEVICEPIXELFORMAT_SUPPORT1_SHADER_GATHER,
    GPUPIXELFORMAT_SUPPORT1_BACK_BUFFER_CAST            = DEVICEPIXELFORMAT_SUPPORT1_BACK_BUFFER_CAST,
    GPUPIXELFORMAT_SUPPORT1_TYPED_UNORDERED_ACCESS_VIEW = DEVICEPIXELFORMAT_SUPPORT1_TYPED_UNORDERED_ACCESS_VIEW,
    GPUPIXELFORMAT_SUPPORT1_SHADER_GATHER_COMPARISON    = DEVICEPIXELFORMAT_SUPPORT1_SHADER_GATHER_COMPARISON,
    GPUPIXELFORMAT_SUPPORT1_DECODER_OUTPUT              = DEVICEPIXELFORMAT_SUPPORT1_DECODER_OUTPUT,
    GPUPIXELFORMAT_SUPPORT1_VIDEO_PROCESSOR_OUTPUT      = DEVICEPIXELFORMAT_SUPPORT1_VIDEO_PROCESSOR_OUTPUT,
    GPUPIXELFORMAT_SUPPORT1_VIDEO_PROCESSOR_INPUT       = DEVICEPIXELFORMAT_SUPPORT1_VIDEO_PROCESSOR_INPUT,
    GPUPIXELFORMAT_SUPPORT1_VIDEO_ENCODER               = DEVICEPIXELFORMAT_SUPPORT1_VIDEO_ENCODER
};
enum GPUPixelFormatSupportFlags2 {
    GPUPIXELFORMAT_SUPPORT2_NONE                                         = DEVICEPIXELFORMAT_SUPPORT2_NONE,
    GPUPIXELFORMAT_SUPPORT2_UAV_ATOMIC_ADD                               = DEVICEPIXELFORMAT_SUPPORT2_UAV_ATOMIC_ADD,
    GPUPIXELFORMAT_SUPPORT2_UAV_ATOMIC_BITWISE_OPS                       = DEVICEPIXELFORMAT_SUPPORT2_UAV_ATOMIC_BITWISE_OPS,
    GPUPIXELFORMAT_SUPPORT2_UAV_ATOMIC_COMPARE_STORE_OR_COMPARE_EXCHANGE = DEVICEPIXELFORMAT_SUPPORT2_UAV_ATOMIC_COMPARE_STORE_OR_COMPARE_EXCHANGE,
    GPUPIXELFORMAT_SUPPORT2_UAV_ATOMIC_EXCHANGE                          = DEVICEPIXELFORMAT_SUPPORT2_UAV_ATOMIC_EXCHANGE,
    GPUPIXELFORMAT_SUPPORT2_UAV_ATOMIC_SIGNED_MIN_OR_MAX                 = DEVICEPIXELFORMAT_SUPPORT2_UAV_ATOMIC_SIGNED_MIN_OR_MAX,
    GPUPIXELFORMAT_SUPPORT2_UAV_ATOMIC_UNSIGNED_MIN_OR_MAX               = DEVICEPIXELFORMAT_SUPPORT2_UAV_ATOMIC_UNSIGNED_MIN_OR_MAX,
    GPUPIXELFORMAT_SUPPORT2_UAV_TYPED_LOAD                               = DEVICEPIXELFORMAT_SUPPORT2_UAV_TYPED_LOAD,
    GPUPIXELFORMAT_SUPPORT2_UAV_TYPED_STORE                              = DEVICEPIXELFORMAT_SUPPORT2_UAV_TYPED_STORE,
    GPUPIXELFORMAT_SUPPORT2_OUTPUT_MERGER_LOGIC_OP                       = DEVICEPIXELFORMAT_SUPPORT2_OUTPUT_MERGER_LOGIC_OP
};

typedef struct _gpu_pixelformat_support {
    UInt iFlags1; // GPUPixelFormatSupportFlags1
    UInt iFlags2; // GPUPixelFormatSupportFlags2
} GPUPixelFormatSupport;

typedef struct _gpu_counter_support {
    UInt iMaxCounters;
    UInt iMaxParallelCounters;
    UInt iMaxParallelUnitsDetection;
} GPUCounterSupport;

// SwapChain definitions
enum GPUSwapChainSwapEffect {
    GPUSWAPCHAIN_SWAPEFFECT_DISCARD         = DEVICESWAPCHAIN_SWAPEFFECT_DISCARD,
    GPUSWAPCHAIN_SWAPEFFECT_SEQUENTIAL      = DEVICESWAPCHAIN_SWAPEFFECT_SEQUENTIAL,
    GPUSWAPCHAIN_SWAPEFFECT_FLIP            = DEVICESWAPCHAIN_SWAPEFFECT_FLIP,
    GPUSWAPCHAIN_SWAPEFFECT_FLIP_SEQUENTIAL = DEVICESWAPCHAIN_SWAPEFFECT_FLIP_SEQUENTIAL
};
enum GPUSwapChainBufferUsageFlags {
    GPUSWAPCHAIN_BUFFERUSAGE_NONE                 = DEVICESWAPCHAIN_BUFFERUSAGE_NONE,
    GPUSWAPCHAIN_BUFFERUSAGE_SHADER_INPUT         = DEVICESWAPCHAIN_BUFFERUSAGE_SHADER_INPUT,
    GPUSWAPCHAIN_BUFFERUSAGE_RENDER_TARGET_OUTPUT = DEVICESWAPCHAIN_BUFFERUSAGE_RENDER_TARGET_OUTPUT,
    GPUSWAPCHAIN_BUFFERUSAGE_BACK_BUFFER          = DEVICESWAPCHAIN_BUFFERUSAGE_BACK_BUFFER,
    GPUSWAPCHAIN_BUFFERUSAGE_SHARED               = DEVICESWAPCHAIN_BUFFERUSAGE_SHARED,
    GPUSWAPCHAIN_BUFFERUSAGE_READ_ONLY            = DEVICESWAPCHAIN_BUFFERUSAGE_READ_ONLY,
    GPUSWAPCHAIN_BUFFERUSAGE_DISCARD_ON_PRESENT   = DEVICESWAPCHAIN_BUFFERUSAGE_DISCARD_ON_PRESENT,
    GPUSWAPCHAIN_BUFFERUSAGE_UNORDERED_ACCESS     = DEVICESWAPCHAIN_BUFFERUSAGE_UNORDERED_ACCESS
};
enum GPUSwapChainFlags {
    GPUSWAPCHAIN_FLAG_NONE                            = DEVICESWAPCHAIN_FLAG_NONE,
    GPUSWAPCHAIN_FLAG_NONPREROTATED                   = DEVICESWAPCHAIN_FLAG_NONPREROTATED,
    GPUSWAPCHAIN_FLAG_ALLOW_MODE_SWITCH               = DEVICESWAPCHAIN_FLAG_ALLOW_MODE_SWITCH,
    GPUSWAPCHAIN_FLAG_GDI_COMPATIBLE                  = DEVICESWAPCHAIN_FLAG_GDI_COMPATIBLE,
    GPUSWAPCHAIN_FLAG_RESTRICTED_CONTENT              = DEVICESWAPCHAIN_FLAG_RESTRICTED_CONTENT,
    GPUSWAPCHAIN_FLAG_RESTRICT_SHARED_RESOURCE_DRIVER = DEVICESWAPCHAIN_FLAG_RESTRICT_SHARED_RESOURCE_DRIVER,
    GPUSWAPCHAIN_FLAG_DISPLAY_ONLY                    = DEVICESWAPCHAIN_FLAG_DISPLAY_ONLY
};

enum GPUPresentFlags {
    GPUPRESENT_FLAG_DONT_SEQUENCE       = DEVICEPRESENT_FLAG_DONT_SEQUENCE,
    GPUPRESENT_FLAG_RESTART             = DEVICEPRESENT_FLAG_RESTART,
    GPUPRESENT_FLAG_DONT_WAIT           = DEVICEPRESENT_FLAG_DONT_WAIT,
    GPUPRESENT_FLAG_STEREO_PREFER_RIGHT = DEVICEPRESENT_FLAG_STEREO_PREFER_RIGHT,
    GPUPRESENT_FLAG_STEREO_TEMP_MONO    = DEVICEPRESENT_FLAG_STEREO_TEMP_MONO,
    GPUPRESENT_FLAG_RESTRICT_TO_OUTPUT  = DEVICEPRESENT_FLAG_RESTRICT_TO_OUTPUT
};

typedef struct _gpu_swapchain_desc {
    Void * pOutputWindow;
    Bool bWindowed;

    UInt iRefreshRateNumerator;
    UInt iRefreshRateDenominator;

    UInt iBufferCount;
    UInt iBufferUsageFlags; // GPUSwapChainBufferUsageFlags

    UInt iWidth;
    UInt iHeight;

    PixelFormat iFormat;
    UInt iSampleCount;
    UInt iSampleQuality;

    GPUDisplayModeScanlineOrdering iScanlineOrdering;
    GPUDisplayModeScaling iScaling;

    GPUSwapChainSwapEffect iSwapEffect;
    UInt iFlags; // GPUSwapChainFlags
} GPUSwapChainDesc;

typedef struct _gpu_frame_stats {
    UInt iLastPresentCount;

    UInt iPresentCount;
    UInt iPresentRefreshCount;
    UInt iSyncRefreshCount;
    Int64 iSyncQPCTime;
    Int64 iSyncGPUTime;
} GPUFrameStats;

typedef struct _gpu_viewport {
    Float fTopLeftX;
    Float fTopLeftY;
    Float fWidth;
    Float fHeight;
    Float fMinDepth;
    Float fMaxDepth;
} GPUViewport;

// Batched drawing
#define RENDERING_BATCH_COUNT        128
#define RENDERING_BATCH_MAX_VERTICES 32768
//
//#define RENDERING_BATCH2D_COUNT    8
//#define RENDERING_BATCHTEXT_COUNT  8
//#define RENDERING_BATCHDEBUG_COUNT 4
//
enum RenderingBatchSortMode
{
    RENDERING_BATCH_SORT_FRONT_TO_BACK = 0, // Opaque geometry
    RENDERING_BATCH_SORT_BACK_TO_FRONT      // Transparent geometry
};

typedef UInt RenderingBatchID;

// Text drawing
//#define RENDERING_FONT_COUNT 32
//
//#define RENDERING_FONT_MAX_SIZE 64
//#define RENDERING_FONT_SPACING 1
//
//typedef struct _font_cell
//{
//    UInt iCharX, iCharY;
//    UInt iCharWidth, iCharHeight;
//} FontCell;
//typedef struct _font_descriptor
//{
//    GChar strName[32];
//    UInt iCharCount;
//    UInt iCellSize, iCellBaseline;
//    UInt iCharMaxWidth, iCharMaxHeight;
//    FontCell arrCells[256];
//} FontDescriptor;

// Prototypes
class WindowApplication;

/////////////////////////////////////////////////////////////////////////////////
// The RenderingManagerCallbacks class
class RenderingManagerCallbacks : public DeviceRendererCallbacks
{
public:
    RenderingManagerCallbacks();
    virtual ~RenderingManagerCallbacks();

    // Device
    virtual Void OnDestroyDevice();
    virtual Void OnRestoreDevice();

    // SwapChain
    virtual Void OnDestroySwapChain();
    virtual Void OnRestoreSwapChain();
};

/////////////////////////////////////////////////////////////////////////////////
// The RenderingManager class
class RenderingManager : public Manager<RenderingManager>
{
public:
    inline static const GChar * GetName();
    inline static const GChar * GetMemoryBudget();

protected:
    friend class Manager<RenderingManager>;
    RenderingManager();
    virtual ~RenderingManager();

public:
    Void Initialize( DeviceWindow * pDeviceWindow );
    Void Cleanup();

    // Resource Factory
    GPUVertexBuffer * CreateVertexBuffer( UInt iVertexSize, UInt iVertexCount, const Void * arrVertices = NULL, Bool bCreateCPUData = true ) const;
    GPUIndexBuffer * CreateIndexBuffer( UInt iIndexCount, const Void * arrIndices = NULL, Bool bUseShorts = false, Bool bCreateCPUData = true ) const;
    GPUConstantBuffer * CreateConstantBuffer( UInt iConstantCount, const GPUConstant * arrConstants = NULL, Bool bCreateCPUData = true ) const;
    GPURawBuffer * CreateRawBuffer( UInt iDWordCount, const DWord * arrDWords = NULL, Bool bCreateCPUData = true ) const;
    GPUStructuredBuffer * CreateStructuredBuffer( UInt iStructSize, UInt iStructCount, const Void * arrStructs = NULL, Bool bCreateCPUData = true ) const;

    GPUTexture1D * CreateTexture1D( PixelFormat iFormat, UInt iWidth, UInt iMipLevelCount = GPUTEXTURE_MIPMAP_NONE, UInt iArrayCount = 1, const Byte * arrTexels = NULL, Bool bCreateCPUData = true ) const;
    GPUTexture2D * CreateTexture2D( PixelFormat iFormat, UInt iWidth, UInt iHeight, UInt iMipLevelCount = GPUTEXTURE_MIPMAP_NONE, UInt iArrayCount = 1, UInt iSampleCount = 1, const Byte * arrTexels = NULL, Bool bCreateCPUData = true ) const;
    GPUTexture3D * CreateTexture3D( PixelFormat iFormat, UInt iWidth, UInt iHeight, UInt iDepth, UInt iMipLevelCount = GPUTEXTURE_MIPMAP_NONE, const Byte * arrTexels = NULL, Bool bCreateCPUData = true ) const;
    GPUTextureCube * CreateTextureCube( PixelFormat iFormat, UInt iEdgeLength, UInt iMipLevelCount = GPUTEXTURE_MIPMAP_NONE, UInt iCubeCount = 1, const Byte * arrTexels = NULL, Bool bCreateCPUData = true ) const;

    Void DestroyResource( GPUResource * pResource ) const;

    // Screen methods
    inline Void GetScreenSize( UInt * outWidth, UInt * outHeight ) const;

    // Display methods
    inline UInt GetAdapterCount() const;
    inline Void GetAdapterDesc( GPUAdapterDesc * outAdapterDesc, UInt iAdapter ) const;

    inline UInt GetCurrentAdapter() const;
    inline const GPUAdapterDesc * GetCurrentAdapterDesc() const;

    inline UInt GetOutputCount( UInt iAdapter = INVALID_OFFSET ) const;
    inline Void GetOutputDesc( GPUOutputDesc * outOutputDesc, UInt iOutput, UInt iAdapter = INVALID_OFFSET ) const;

    inline UInt GetCurrentOutput() const;
    inline const GPUOutputDesc * GetCurrentOutputDesc() const;

    inline UInt GetDisplayModeCount( UInt iOutput = INVALID_OFFSET, UInt iAdapter = INVALID_OFFSET ) const;
    inline Void GetDisplayModeDesc( GPUDisplayModeDesc * outDisplayModeDesc, UInt iDisplayMode, UInt iOutput = INVALID_OFFSET, UInt iAdapter = INVALID_OFFSET ) const;

    inline UInt GetCurrentDisplayMode() const;
    inline const GPUDisplayModeDesc * GetCurrentDisplayModeDesc() const;

    inline UInt MatchDisplayMode( GPUDisplayModeDesc * outMatchedDisplayMode, const GPUDisplayModeDesc * pDisplayMode, UInt iOutput = INVALID_OFFSET, UInt iAdapter = INVALID_OFFSET );

    inline Void SwitchAdapter( UInt iAdapter, UInt iOutput = INVALID_OFFSET, UInt iDisplayMode = INVALID_OFFSET );
    inline Void SwitchOutput( UInt iOutput, UInt iDisplayMode = INVALID_OFFSET );
    inline Void SwitchDisplayMode( UInt iDisplayMode );

    inline Bool IsFullScreen( UInt * outOutput = NULL ) const;
    inline Void SetFullScreen( Bool bFullScreen, UInt iOutput = INVALID_OFFSET, UInt iDisplayMode = INVALID_OFFSET );
    inline Void SetFullScreenWindowed( Bool bFullScreenWindowed );

    inline Void MoveWindow( UInt iX, UInt iY ) const;

    inline Void GetGammaCaps( GPUGammaCaps * outGammaCaps ) const;
    inline Void GetGammaControl( GPUGammaControl * outGammaControl ) const;
    inline Void SetGammaControl( const GPUGammaControl * pGammaControl ) const;

    inline Void WaitForVSync() const;

    // Cursor methods
    inline Void ClipCursor( Bool bClip, Bool bClipToClientRect ) const;
    inline Void ShowCursor( Bool bShow ) const;
    inline Void WarpCursor( UInt iX, UInt iY ) const;
    inline Void SetCursor( const DWord * pCursorRGBA = NULL, UInt iWidth = 0, UInt iHeight = 0, DWord dwKeyColor = 0, UInt iHotspotX = 0, UInt iHotspotY = 0 );

    // Clipboard methods
    inline Bool GetClipboardData( GChar * outText, UInt iMaxLength ) const;
    inline Bool SetClipboardData( const GChar * strText, UInt iLength = INVALID_OFFSET ) const;

    // Device caps methods
    inline Void GetDeviceFeatures( GPUDeviceFeatures * outFeatures ) const;
    inline Void GetFormatSupport( GPUPixelFormatSupport * outFormatSupport, PixelFormat iFormat ) const;
    inline Void GetCounterSupport( GPUCounterSupport * outCounterSupport ) const;

    // SwapChain methods
    inline const GPUSwapChainDesc * GetSwapChainDesc() const;
    inline Void GetFrameStats( GPUFrameStats * outFrameStats ) const;

    inline Void SetMultisampling( UInt iSampleCount );

    inline Bool IsIdle() const;
    inline Bool Present( UInt iVSyncInterval = 1, UInt iPresentFlags = 0 );

    // InputAssembler stage methods
    Void IAEnableIL( GPUInputLayout * pIL, GPUDeferredContext * pContext = NULL );
    Void IADisableIL( GPUDeferredContext * pContext = NULL );

    Void IAEnablePT( GPUShaderPrimitiveTopology iPrimitiveTopology, GPUDeferredContext * pContext = NULL );
    Void IADisablePT( GPUDeferredContext * pContext = NULL );

    Void IAEnableVB( UInt iSlot, GPUVertexBuffer * pVB, UInt iOffset = 0, GPUDeferredContext * pContext = NULL );
    Void IAEnableVBs( UInt iSlot, UInt iVBCount, GPUVertexBuffer ** arrVBs, const UInt * arrOffsets = NULL, GPUDeferredContext * pContext = NULL );
    Void IADisableVB( UInt iSlot, GPUDeferredContext * pContext = NULL );
    Void IADisableVBs( UInt iSlot, UInt iVBCount, GPUDeferredContext * pContext = NULL );
    Void IADisableAllVBs( GPUDeferredContext * pContext = NULL );

    Void IAEnableIB( GPUIndexBuffer * pIB, UInt iOffset = 0, GPUDeferredContext * pContext = NULL );
    Void IADisableIB( GPUDeferredContext * pContext = NULL );

    // VertexShader stage methods
    Void VSEnable( GPUVertexShader * pVS, GPUDeferredContext * pContext = NULL );
    Void VSDisable( GPUDeferredContext * pContext = NULL );

    Void VSEnableCB( UInt iSlot, GPUConstantBuffer * pCB, GPUDeferredContext * pContext = NULL );
    Void VSEnableCBs( UInt iSlot, UInt iCBCount, GPUConstantBuffer ** arrCBs, GPUDeferredContext * pContext = NULL );
    Void VSDisableCB( UInt iSlot, GPUDeferredContext * pContext = NULL );
    Void VSDisableCBs( UInt iSlot, UInt iCBCount, GPUDeferredContext * pContext = NULL );
    Void VSDisableAllCBs( GPUDeferredContext * pContext = NULL );

    Void VSEnableSampler( UInt iSlot, GPUSamplerState * pSampler, GPUDeferredContext * pContext = NULL );
    Void VSEnableSamplers( UInt iSlot, UInt iSamplerCount, GPUSamplerState ** arrSamplers, GPUDeferredContext * pContext = NULL );
    Void VSDisableSampler( UInt iSlot, GPUDeferredContext * pContext = NULL );
    Void VSDisableSamplers( UInt iSlot, UInt iSamplerCount, GPUDeferredContext * pContext = NULL );
    Void VSDisableAllSamplers( GPUDeferredContext * pContext = NULL );

    Void VSEnableResource( UInt iSlot, GPUShaderView * pResource, GPUDeferredContext * pContext = NULL );
    Void VSEnableResources( UInt iSlot, UInt iResourceCount, GPUShaderView ** arrResources, GPUDeferredContext * pContext = NULL );
    Void VSDisableResource( UInt iSlot, GPUDeferredContext * pContext = NULL );
    Void VSDisableResources( UInt iSlot, UInt iResourceCount, GPUDeferredContext * pContext = NULL );
    Void VSDisableAllResources( GPUDeferredContext * pContext = NULL );

    // GeometryShader stage methods
    Void GSEnable( GPUGeometryShader * pGS, GPUDeferredContext * pContext = NULL );
    Void GSDisable( GPUDeferredContext * pContext = NULL );

    Void GSEnableCB( UInt iSlot, GPUConstantBuffer * pCB, GPUDeferredContext * pContext = NULL );
    Void GSEnableCBs( UInt iSlot, UInt iCBCount, GPUConstantBuffer ** arrCBs, GPUDeferredContext * pContext = NULL );
    Void GSDisableCB( UInt iSlot, GPUDeferredContext * pContext = NULL );
    Void GSDisableCBs( UInt iSlot, UInt iCBCount, GPUDeferredContext * pContext = NULL );
    Void GSDisableAllCBs( GPUDeferredContext * pContext = NULL );

    Void GSEnableSampler( UInt iSlot, GPUSamplerState * pSampler, GPUDeferredContext * pContext = NULL );
    Void GSEnableSamplers( UInt iSlot, UInt iSamplerCount, GPUSamplerState ** arrSamplers, GPUDeferredContext * pContext = NULL );
    Void GSDisableSampler( UInt iSlot, GPUDeferredContext * pContext = NULL );
    Void GSDisableSamplers( UInt iSlot, UInt iSamplerCount, GPUDeferredContext * pContext = NULL );
    Void GSDisableAllSamplers( GPUDeferredContext * pContext = NULL );

    Void GSEnableResource( UInt iSlot, GPUShaderView * pResource, GPUDeferredContext * pContext = NULL );
    Void GSEnableResources( UInt iSlot, UInt iResourceCount, GPUShaderView ** arrResources, GPUDeferredContext * pContext = NULL );
    Void GSDisableResource( UInt iSlot, GPUDeferredContext * pContext = NULL );
    Void GSDisableResources( UInt iSlot, UInt iResourceCount, GPUDeferredContext * pContext = NULL );
    Void GSDisableAllResources( GPUDeferredContext * pContext = NULL );

    // PixelShader stage methods
    Void PSEnable( GPUPixelShader * pPS, GPUDeferredContext * pContext = NULL );
    Void PSDisable( GPUDeferredContext * pContext = NULL );

    Void PSEnableCB( UInt iSlot, GPUConstantBuffer * pCB, GPUDeferredContext * pContext = NULL );
    Void PSEnableCBs( UInt iSlot, UInt iCBCount, GPUConstantBuffer ** arrCBs, GPUDeferredContext * pContext = NULL );
    Void PSDisableCB( UInt iSlot, GPUDeferredContext * pContext = NULL );
    Void PSDisableCBs( UInt iSlot, UInt iCBCount, GPUDeferredContext * pContext = NULL );
    Void PSDisableAllCBs( GPUDeferredContext * pContext = NULL );

    Void PSEnableSampler( UInt iSlot, GPUSamplerState * pSampler, GPUDeferredContext * pContext = NULL );
    Void PSEnableSamplers( UInt iSlot, UInt iSamplerCount, GPUSamplerState ** arrSamplers, GPUDeferredContext * pContext = NULL );
    Void PSDisableSampler( UInt iSlot, GPUDeferredContext * pContext = NULL );
    Void PSDisableSamplers( UInt iSlot, UInt iSamplerCount, GPUDeferredContext * pContext = NULL );
    Void PSDisableAllSamplers( GPUDeferredContext * pContext = NULL );

    Void PSEnableResource( UInt iSlot, GPUShaderView * pResource, GPUDeferredContext * pContext = NULL );
    Void PSEnableResources( UInt iSlot, UInt iResourceCount, GPUShaderView ** arrResources, GPUDeferredContext * pContext = NULL );
    Void PSDisableResource( UInt iSlot, GPUDeferredContext * pContext = NULL );
    Void PSDisableResources( UInt iSlot, UInt iResourceCount, GPUDeferredContext * pContext = NULL );
    Void PSDisableAllResources( GPUDeferredContext * pContext = NULL );

    // HullShader stage methods
    Void HSEnable( GPUHullShader * pHS, GPUDeferredContext * pContext = NULL );
    Void HSDisable( GPUDeferredContext * pContext = NULL );

    Void HSEnableCB( UInt iSlot, GPUConstantBuffer * pCB, GPUDeferredContext * pContext = NULL );
    Void HSEnableCBs( UInt iSlot, UInt iCBCount, GPUConstantBuffer ** arrCBs, GPUDeferredContext * pContext = NULL );
    Void HSDisableCB( UInt iSlot, GPUDeferredContext * pContext = NULL );
    Void HSDisableCBs( UInt iSlot, UInt iCBCount, GPUDeferredContext * pContext = NULL );
    Void HSDisableAllCBs( GPUDeferredContext * pContext = NULL );

    Void HSEnableSampler( UInt iSlot, GPUSamplerState * pSampler, GPUDeferredContext * pContext = NULL );
    Void HSEnableSamplers( UInt iSlot, UInt iSamplerCount, GPUSamplerState ** arrSamplers, GPUDeferredContext * pContext = NULL );
    Void HSDisableSampler( UInt iSlot, GPUDeferredContext * pContext = NULL );
    Void HSDisableSamplers( UInt iSlot, UInt iSamplerCount, GPUDeferredContext * pContext = NULL );
    Void HSDisableAllSamplers( GPUDeferredContext * pContext = NULL );

    Void HSEnableResource( UInt iSlot, GPUShaderView * pResource, GPUDeferredContext * pContext = NULL );
    Void HSEnableResources( UInt iSlot, UInt iResourceCount, GPUShaderView ** arrResources, GPUDeferredContext * pContext = NULL );
    Void HSDisableResource( UInt iSlot, GPUDeferredContext * pContext = NULL );
    Void HSDisableResources( UInt iSlot, UInt iResourceCount, GPUDeferredContext * pContext = NULL );
    Void HSDisableAllResources( GPUDeferredContext * pContext = NULL );

    // DomainShader stage methods
    Void DSEnable( GPUDomainShader * pDS, GPUDeferredContext * pContext = NULL );
    Void DSDisable( GPUDeferredContext * pContext = NULL );

    Void DSEnableCB( UInt iSlot, GPUConstantBuffer * pCB, GPUDeferredContext * pContext = NULL );
    Void DSEnableCBs( UInt iSlot, UInt iCBCount, GPUConstantBuffer ** arrCBs, GPUDeferredContext * pContext = NULL );
    Void DSDisableCB( UInt iSlot, GPUDeferredContext * pContext = NULL );
    Void DSDisableCBs( UInt iSlot, UInt iCBCount, GPUDeferredContext * pContext = NULL );
    Void DSDisableAllCBs( GPUDeferredContext * pContext = NULL );

    Void DSEnableSampler( UInt iSlot, GPUSamplerState * pSampler, GPUDeferredContext * pContext = NULL );
    Void DSEnableSamplers( UInt iSlot, UInt iSamplerCount, GPUSamplerState ** arrSamplers, GPUDeferredContext * pContext = NULL );
    Void DSDisableSampler( UInt iSlot, GPUDeferredContext * pContext = NULL );
    Void DSDisableSamplers( UInt iSlot, UInt iSamplerCount, GPUDeferredContext * pContext = NULL );
    Void DSDisableAllSamplers( GPUDeferredContext * pContext = NULL );

    Void DSEnableResource( UInt iSlot, GPUShaderView * pResource, GPUDeferredContext * pContext = NULL );
    Void DSEnableResources( UInt iSlot, UInt iResourceCount, GPUShaderView ** arrResources, GPUDeferredContext * pContext = NULL );
    Void DSDisableResource( UInt iSlot, GPUDeferredContext * pContext = NULL );
    Void DSDisableResources( UInt iSlot, UInt iResourceCount, GPUDeferredContext * pContext = NULL );
    Void DSDisableAllResources( GPUDeferredContext * pContext = NULL );

    // ComputeShader stage methods
    Void CSEnable( GPUComputeShader * pCS, GPUDeferredContext * pContext = NULL );
    Void CSDisable( GPUDeferredContext * pContext = NULL );

    Void CSEnableCB( UInt iSlot, GPUConstantBuffer * pCB, GPUDeferredContext * pContext = NULL );
    Void CSEnableCBs( UInt iSlot, UInt iCBCount, GPUConstantBuffer ** arrCBs, GPUDeferredContext * pContext = NULL );
    Void CSDisableCB( UInt iSlot, GPUDeferredContext * pContext = NULL );
    Void CSDisableCBs( UInt iSlot, UInt iCBCount, GPUDeferredContext * pContext = NULL );
    Void CSDisableAllCBs( GPUDeferredContext * pContext = NULL );

    Void CSEnableSampler( UInt iSlot, GPUSamplerState * pSampler, GPUDeferredContext * pContext = NULL );
    Void CSEnableSamplers( UInt iSlot, UInt iSamplerCount, GPUSamplerState ** arrSamplers, GPUDeferredContext * pContext = NULL );
    Void CSDisableSampler( UInt iSlot, GPUDeferredContext * pContext = NULL );
    Void CSDisableSamplers( UInt iSlot, UInt iSamplerCount, GPUDeferredContext * pContext = NULL );
    Void CSDisableAllSamplers( GPUDeferredContext * pContext = NULL );

    Void CSEnableResource( UInt iSlot, GPUShaderView * pResource, GPUDeferredContext * pContext = NULL );
    Void CSEnableResources( UInt iSlot, UInt iResourceCount, GPUShaderView ** arrResources, GPUDeferredContext * pContext = NULL );
    Void CSDisableResource( UInt iSlot, GPUDeferredContext * pContext = NULL );
    Void CSDisableResources( UInt iSlot, UInt iResourceCount, GPUDeferredContext * pContext = NULL );
    Void CSDisableAllResources( GPUDeferredContext * pContext = NULL );

    Void CSEnableUAV( UInt iSlot, GPUUnorderedAccessView * pUAV, UInt iAppendConsumeInitCount = INVALID_OFFSET, GPUDeferredContext * pContext = NULL );
    Void CSEnableUAVs( UInt iSlot, UInt iUAVCount, GPUUnorderedAccessView ** arrUAVs, const UInt * arrAppendConsumeInitCounts = NULL, GPUDeferredContext * pContext = NULL );
    Void CSDisableUAV( UInt iSlot, GPUDeferredContext * pContext = NULL );
    Void CSDisableUAVs( UInt iSlot, UInt iUAVCount, GPUDeferredContext * pContext = NULL );
    Void CSDisableAllUAVs( GPUDeferredContext * pContext = NULL );

    // StreamOutput stage methods
    Void SOEnable( UInt iBufferCount, GPUBuffer ** arrBuffers, const UInt * arrOffsets = NULL, GPUDeferredContext * pContext = NULL );
    Void SODisable( GPUDeferredContext * pContext = NULL );

    // Rasterizer stage methods
    Void RSEnableState( GPURasterizerState * pRasterizerState, GPUDeferredContext * pContext = NULL );
    Void RSDisableState( GPUDeferredContext * pContext = NULL );

    Void RSEnableViewports( UInt iViewportCount, const GPUViewport * arrViewports, GPUDeferredContext * pContext = NULL );
    Void RSDisableViewports( GPUDeferredContext * pContext = NULL );

    Void RSEnableScissors( UInt iScissorCount, const GPURectangle * arrScissors, GPUDeferredContext * pContext = NULL );
    Void RSDisableScissors( GPUDeferredContext * pContext = NULL );

    // OutputMerger stage methods
    Void OMEnableDepthStencilState( GPUDepthStencilState * pDepthStencilState, DWord iStencilRef, GPUDeferredContext * pContext = NULL );
    Void OMDisableDepthStencilState( GPUDeferredContext * pContext = NULL );

    Void OMEnableBlendState( GPUBlendState * pBlendState, const Float arrBlendFactors[4] = NULL, DWord iSampleMask = 0xffffffff, GPUDeferredContext * pContext = NULL );
    Void OMDisableBlendState( GPUDeferredContext * pContext = NULL );

    Void OMEnableRenderTarget( GPURenderTargetView * pRTV, GPUDepthStencilView * pDSV, GPUDeferredContext * pContext = NULL );
    Void OMEnableRenderTargets( UInt iRTVCount, GPURenderTargetView ** arrRTVs, GPUDepthStencilView * pDSV, GPUDeferredContext * pContext = NULL );
    Void OMDisableAllRenderTargets( GPUDeferredContext * pContext = NULL );

    Void OMEnableUAV( UInt iSlot, GPUUnorderedAccessView * pUAV, UInt iAppendConsumeInitCount = INVALID_OFFSET, GPUDeferredContext * pContext = NULL );
    Void OMEnableUAVs( UInt iSlot, UInt iUAVCount, GPUUnorderedAccessView ** arrUAVs, const UInt * arrAppendConsumeInitCounts = NULL, GPUDeferredContext * pContext = NULL );
    Void OMDisableUAV( UInt iSlot, GPUDeferredContext * pContext = NULL );
    Void OMDisableUAVs( UInt iSlot, UInt iUAVCount, GPUDeferredContext * pContext = NULL );
    Void OMDisableAllUAVs( GPUDeferredContext * pContext = NULL );

    // Clearing methods
    Void Clear( GPUDeferredContext * pContext = NULL ); // Disables everything in the given context
    Void Flush( GPUDeferredContext * pContext = NULL ); // Flushes GPU Command Queue for the given context (NOT recommended, induces performance penalty)
    Void DestroyObjects( GPUDeferredContext * pContext = NULL ); // Ensures synchronous destruction after unbinding resources (Clear + Flush)

    Void ClearDepthStencil( GPUDepthStencilView * pDepthStencil, Bool bClearDepth = true, Bool bClearStencil = true, Float fDepth = FLOAT_INFINITE, Byte iStencil = 0, GPUDeferredContext * pContext = NULL );
    Void ClearRenderTarget( GPURenderTargetView * pRenderTarget, const Float arrRGBA[4], GPUDeferredContext * pContext = NULL );

    Void ClearUAVUInt( GPUUnorderedAccessView * pUAV, const UInt arrValues[4], GPUDeferredContext * pContext = NULL );
    Void ClearUAVFloat( GPUUnorderedAccessView * pUAV, const Float arrValues[4], GPUDeferredContext * pContext = NULL );

    // Camera methods
    inline const Camera * GetDefaultCamera() const;
    inline Camera * GetDefault2DCamera() const;
    inline Camera * GetCamera() const;

    inline Void EnableDefaultCamera();
    inline Void EnableDefault2DCamera();
    inline Void EnableCamera( Camera * pCamera );

    Bool GetPickRay( Vertex3 * outRayPos, Vector3 * outRayDir, UInt iX, UInt iY ) const;

    // Screen resources
    inline const GPUViewport * GetScreenViewport() const;
    inline Void SetScreenViewport( const GPUViewport * pViewport );

    inline GPUDepthStencilView * GetScreenDepthStencil();
    inline GPURenderTargetView * GetScreenRenderTarget();

    inline Void EnableScreenResources( GPUDeferredContext * pContext = NULL );

    inline GPU2DContext * GetScreen2DContext();

    // Basic rendering methods
    Bool GetPredication( GPUPredicate * outPredicate, Bool * outValue, GPUDeferredContext * pContext = NULL );
    Void SetPredication( GPUPredicate * pPredicate, Bool bPredicateReference, GPUDeferredContext * pContext = NULL );

    Void AsyncBegin( GPUAsynchronous * pAsynchronous = NULL, GPUDeferredContext * pContext = NULL );
    Void AsyncEnd( GPUAsynchronous * pAsynchronous = NULL, GPUDeferredContext * pContext = NULL );

    Void Draw( const Mesh * pMesh, const Transform3 * pWorldTransform, const EffectInstance * pEffectInstance, UInt iSlot = 0, GPUDeferredContext * pContext = NULL );

    //Void DrawInstanced( const Mesh * pMesh, const Transform3 * pWorldTransform, const EffectInstance * pEffectInstance,
    //                    UInt iElementsPerInstance, UInt iInstanceCount, UInt iBaseInstance = 0, GPUDeferredContext * pContext = NULL );

    //Void DrawInstancedIndirect( GPUVertexBuffer * outGeneratedPrimitives, UInt iBaseVertex = 0, GPUDeferredContext * pContext = NULL );
    //Void DrawInstancedIndirect( GPUIndexBuffer * outGeneratedPrimitives, UInt iBaseIndex = 0, GPUDeferredContext * pContext = NULL );

    //Void DrawStreamOutput( const Transform3 * pWorldTransform, const EffectInstance * pEffectInstance, GPUDeferredContext * pContext = NULL );

    Void ExecuteCommandList( GPUCommandList * pCommandList, Bool bRestoreContextState );

    Void Dispatch( UInt iThreadGroupX, UInt iThreadGroupY, UInt iThreadGroupZ, GPUDeferredContext * pContext = NULL );
    Void DispatchIndirect( GPUBuffer * pGeneratedDispatch, UInt iByteOffset = 0, GPUDeferredContext * pContext = NULL );

    // Batched rendering methods
    inline UInt GetBatchCount() const;
    inline UInt GetFreeBatchesCount() const;

    inline RenderingBatchID BatchCreate( const EffectInstance * pEffectInstance, GPUShaderPrimitiveTopology iPrimitiveTopology, GPUDeferredContext * pContext = NULL );
    inline Void BatchDestroy( RenderingBatchID idBatch );

    inline Void BatchSetWorldTransform( RenderingBatchID idBatch, const Transform3 & trWorldTransform );

    inline Void BatchStart( RenderingBatchID idBatch );
    inline Void BatchFinish( RenderingBatchID idBatch );
    inline Void BatchAbort( RenderingBatchID idBatch );

    inline Void BatchFlush( RenderingBatchID idBatch );
    inline Void BatchReset( RenderingBatchID idBatch );

    inline Void BatchPrimitive( RenderingBatchID idBatch, const Byte * arrVertices );

    Void BatchSort( RenderingBatchID idBatch, RenderingBatchSortMode iSortMode );

private:
    // Access to resources
    friend class GPUDeferredContext;
    friend class GPUInputLayout;
    friend class GPURenderState;
    friend class GPUSamplerState;
    friend class GPURasterizerState;
    friend class GPUDepthStencilState;
    friend class GPUBlendState;
    friend class GPUAsynchronous;
    friend class GPUQuery;
    friend class GPUQueryCommandProcessing;
    friend class GPUQueryOcclusion;
    friend class GPUQueryTimeStampFrequency;
    friend class GPUQueryTimeStamp;
    friend class GPUQueryStatsPipeline;
    friend class GPUQueryStatsStreamOutput;
    friend class GPUPredicate;
    friend class GPUPredicateOcclusion;
    friend class GPUPredicateStreamOutputOverflow;
    friend class GPUCounter;
    friend class GPUBuffer;
    friend class GPUVertexBuffer;
    friend class GPUIndexBuffer;
    friend class GPUConstantBuffer;
    friend class GPURawBuffer;
    friend class GPUStructuredBuffer;
    friend class GPUTexture;
    friend class GPUTexture1D;
    friend class GPUTexture2D;
    friend class GPUTexture3D;
    friend class GPUTextureCube;
    friend class GPUResourceView;
    friend class GPURenderTargetView;
    friend class GPUDepthStencilView;
    friend class GPUShaderView;
    friend class GPUUnorderedAccessView;
    friend class GPUShader;
    friend class GPUVertexShader;
    friend class GPUGeometryShader;
    friend class GPUPixelShader;
    friend class GPUHullShader;
    friend class GPUDomainShader;
    friend class GPUComputeShader;
    friend class GPU2DContext;

    // Renderer callbacks
    friend class RenderingManagerCallbacks;

    Void _OnDestroyDevice();
    Void _OnRestoreDevice();

    Void _OnDestroySwapChain();
    Void _OnRestoreSwapChain();

    RenderingManagerCallbacks m_hRendererCallbacks;

    // Resource database
    typedef RedBlackTree<GPUDeferredContext*> _DeferredContextList;
    typedef RedBlackTree<GPUInputLayout*> _InputLayoutList;
    typedef RedBlackTree<GPURenderState*> _RenderStateList;
    typedef RedBlackTree<GPUAsynchronous*> _AsynchronousList;
    typedef RedBlackTree<GPUBuffer*> _BufferList;
    typedef RedBlackTree<GPUTexture*> _TextureList;
    typedef RedBlackTree<GPUResourceView*> _ResourceViewList;
    typedef RedBlackTree<GPUShader*> _ShaderList;
    typedef RedBlackTree<GPU2DContext*> _2DContextList;

    inline static Int _Compare_GPUDeferredContext( GPUDeferredContext * const & rLeft, GPUDeferredContext * const & rRight, Void * pUserData );
    inline static Int _Compare_GPUInputLayout( GPUInputLayout * const & rLeft, GPUInputLayout * const & rRight, Void * pUserData );
    inline static Int _Compare_GPURenderState( GPURenderState * const & rLeft, GPURenderState * const & rRight, Void * pUserData );
    inline static Int _Compare_GPUAsynchronous( GPUAsynchronous * const & rLeft, GPUAsynchronous * const & rRight, Void * pUserData );
    inline static Int _Compare_GPUBuffer( GPUBuffer * const & rLeft, GPUBuffer * const & rRight, Void * pUserData );
    inline static Int _Compare_GPUTexture( GPUTexture * const & rLeft, GPUTexture * const & rRight, Void * pUserData );
    inline static Int _Compare_GPUResourceView( GPUResourceView * const & rLeft, GPUResourceView * const & rRight, Void * pUserData );
    inline static Int _Compare_GPUShader( GPUShader * const & rLeft, GPUShader * const & rRight, Void * pUserData );
    inline static Int _Compare_GPU2DContext( GPU2DContext * const & rLeft, GPU2DContext * const & rRight, Void * pUserData );

    inline Void _RegisterDeferredContext( GPUDeferredContext * pDeferredContext );
    inline Void _RegisterInputLayout( GPUInputLayout * pInputLayout );
    inline Void _RegisterRenderState( GPURenderState * pRenderState );
    inline Void _RegisterAsynchronous( GPUAsynchronous * pAsynchronous );
    inline Void _RegisterBuffer( GPUBuffer * pBuffer );
    inline Void _RegisterTexture( GPUTexture * pTexture );
    inline Void _RegisterResourceView( GPUResourceView * pResourceView );
    inline Void _RegisterShader( GPUShader * pShader );
    inline Void _Register2DContext( GPU2DContext * p2DContext );

    inline Void _UnRegisterDeferredContext( GPUDeferredContext * pDeferredContext );
    inline Void _UnRegisterInputLayout( GPUInputLayout * pInputLayout );
    inline Void _UnRegisterRenderState( GPURenderState * pRenderState );
    inline Void _UnRegisterAsynchronous( GPUAsynchronous * pAsynchronous );
    inline Void _UnRegisterBuffer( GPUBuffer * pBuffer );
    inline Void _UnRegisterTexture( GPUTexture * pTexture );
    inline Void _UnRegisterResourceView( GPUResourceView * pResourceView );
    inline Void _UnRegisterShader( GPUShader * pShader );
    inline Void _UnRegister2DContext( GPU2DContext * p2DContext );

    _DeferredContextList m_lstDeferredContexts;
    _InputLayoutList m_lstInputLayouts;
    _RenderStateList m_lstRenderStates;
    _AsynchronousList m_lstAsynchronouses;
    _BufferList m_lstBuffers;
    _TextureList m_lstTextures;
    _ResourceViewList m_lstResourceViews;
    _ShaderList m_lstShaders;
    _2DContextList m_lst2DContexts;

    // Device-dependant delegates
    DeviceWindow * m_pDeviceWindow;
    DeviceRenderer * m_pDeviceRenderer;

    // Screen resources
    GPUViewport m_hScreenViewport;
    GPUTexture2D * m_pScreenDepthStencil;
    GPUDepthStencilView * m_pScreenDepthStencilView;
    GPURenderTargetView * m_pScreenRenderTargetView;

    GPU2DContext * m_pScreen2DContext;

    // Camera
    Camera * m_pScreenDefaultCamera;
    Camera * m_pScreenDefault2DCamera;
    Camera * m_pScreenCamera;

    // Batched Rendering
    typedef struct _rendering_batch
    {
        RenderingBatchID iNextFreeID; // same as index => allocated

        const EffectInstance * pEffectInstance;
        GPUShaderPrimitiveTopology iPrimitiveTopology;
        UInt iPrimitiveSize;
        GPUDeferredContext * pContext;

        GPUVertexBuffer * pVB;
        Transform3 trWorldTransform;

        UInt iVertexCount, iPrimitiveCount;
        Byte * arrVertices;
    } _RenderingBatch;

    typedef Int (*_Batch_SortCallback)( Float, Float );
    inline static Int _Batch_Sort_FrontToBack( Float fLeft, Float fRight );
    inline static Int _Batch_Sort_BackToFront( Float fLeft, Float fRight );

    Float _Batch_PrimitiveDepth( RenderingBatchID idBatch, const Byte * pPrimitive );
    Void _Batch_QuickSort( RenderingBatchID idBatch, Byte * arrVertices, UInt iPrimitiveCount, _Batch_SortCallback pfSortCallback );

    RenderingBatchID _Batch_Create( const EffectInstance * pEffectInstance, GPUShaderPrimitiveTopology iPrimitiveTopology, GPUDeferredContext * pContext = NULL );
    Void _Batch_Destroy( RenderingBatchID idBatch );

    Void _Batch_Start( RenderingBatchID idBatch );
    Void _Batch_Finish( RenderingBatchID idBatch );
    Void _Batch_Abort( RenderingBatchID idBatch );

    Void _Batch_Flush( RenderingBatchID idBatch );
    Void _Batch_Reset( RenderingBatchID idBatch );

    Void _Batch_Primitive( RenderingBatchID idBatch, const Byte * arrVertices );

    Void _Batch_DrawPrimitives( RenderingBatchID idBatch );

    UInt m_iFreeBatchesCount;
    RenderingBatchID m_iNextFreeBatchID;
    _RenderingBatch m_arrBatches[RENDERING_BATCH_COUNT];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "RenderingManager.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RENDERING_RENDERINGMANAGER_H
