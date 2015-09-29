/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/D3D11Mappings.h
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
// Header prelude
#ifndef SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_D3D11MAPPINGS_H
#define SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_D3D11MAPPINGS_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../System/System.h"

/////////////////////////////////////////////////////////////////////////////////
// Win32/GDI Decalarations

// Forwards /////////////////////////////////////////////////////////////////////
#define CALLBACK __stdcall

typedef __w64 unsigned int WPARAM;
typedef __w64 long LPARAM;
typedef __w64 long LRESULT;

struct HINSTANCE__; typedef HINSTANCE__ * HINSTANCE;
struct HWND__;      typedef HWND__      * HWND;
struct HMONITOR__;  typedef HMONITOR__  * HMONITOR;
struct HMENU__;     typedef HMENU__     * HMENU;
struct HICON__;     typedef HICON__     * HICON;
typedef HICON HCURSOR;

// Definitions //////////////////////////////////////////////////////////////////
extern KeyCode KeyCodeFromWin32[256];

DWord _D3D11ConvertFlags32( Byte * arrConvert, DWord iFlags );

/////////////////////////////////////////////////////////////////////////////////
// D3D11Window Declarations

// Mapped ///////////////////////////////////////////////////////////////////////
typedef struct _d3d11_point2 {
    UInt iX;
    UInt iY;
} D3D11Point2;
typedef struct _d3d11_point3 {
    UInt iX;
    UInt iY;
    UInt iZ;
} D3D11Point3;
typedef struct _d3d11_rectangle {
    Long iLeft;
    Long iTop;
    Long iRight;
    Long iBottom;
} D3D11Rectangle;
typedef struct _d3d11_box {
    UInt iLeft;
    UInt iTop;
    UInt iFront;
    UInt iRight;
    UInt iBottom;
    UInt iBack;
} D3D11Box;

typedef struct _d3d11_viewport {
    Float fTopLeftX;
    Float fTopLeftY;
    Float fWidth;
    Float fHeight;
    Float fMinDepth;
    Float fMaxDepth;
} D3D11Viewport;

// Forwards /////////////////////////////////////////////////////////////////////
enum DXGI_FORMAT;
enum D3D_FEATURE_LEVEL;

enum DXGI_GRAPHICS_PREEMPTION_GRANULARITY;
enum DXGI_COMPUTE_PREEMPTION_GRANULARITY;
struct DXGI_ADAPTER_DESC;
struct IDXGIAdapter;

enum DXGI_MODE_ROTATION;
struct DXGI_OUTPUT_DESC;
struct DXGI_GAMMA_CONTROL_CAPABILITIES;
struct DXGI_GAMMA_CONTROL;
struct IDXGIOutput;

enum DXGI_MODE_SCANLINE_ORDER;
enum DXGI_MODE_SCALING;
struct DXGI_MODE_DESC;

// Proxies //////////////////////////////////////////////////////////////////////
typedef struct __DXGI_ADAPTER_DESC {
    wchar_t Description[128];
    unsigned int VendorId;
    unsigned int DeviceId;
    unsigned int SubSysId;
    unsigned int Revision;
    __w64 unsigned long DedicatedVideoMemory;
    __w64 unsigned long DedicatedSystemMemory;
    __w64 unsigned long SharedSystemMemory;
    struct __LUID {
        unsigned long LowPart;
        long HighPart;
    } AdapterLuid;

    UInt iEnumIndex;
} __DXGI_ADAPTER_DESC;

typedef struct __DXGI_OUTPUT_DESC {
    wchar_t DeviceName[32];
    struct __RECT {
        long left;
        long top;
        long right;
        long bottom;
    } DesktopCoordinates;
    int AttachedToDesktop;
    DXGI_MODE_ROTATION Rotation;
    HMONITOR Monitor;

    UInt iEnumIndex;
} __DXGI_OUTPUT_DESC;

typedef struct __DXGI_MODE_DESC {
    unsigned int Width;
    unsigned int Height;
    struct __DXGI_RATIONAL {
        unsigned int Numerator;
        unsigned int Denominator;
    } RefreshRate;
    DXGI_FORMAT Format;
    DXGI_MODE_SCANLINE_ORDER ScanlineOrdering;
    DXGI_MODE_SCALING Scaling;
} __DXGI_MODE_DESC;

// Definitions //////////////////////////////////////////////////////////////////
extern PixelFormat PixelFormatFromDXGI[PIXEL_FMT_COUNT];
extern DXGI_FORMAT PixelFormatToDXGI[PIXEL_FMT_COUNT];

typedef struct _d3d11_adapter_desc {
    Void ConvertFrom( const DXGI_ADAPTER_DESC * pDesc, UInt iAdapter );
    Void ConvertTo( DXGI_ADAPTER_DESC * outDesc, UInt * outAdapter ) const;
    inline Void ConvertFrom( const __DXGI_ADAPTER_DESC * pDesc );
    inline Void ConvertTo( __DXGI_ADAPTER_DESC * outDesc ) const;

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
} D3D11AdapterDesc;

enum D3D11OutputRotation {
    D3D11OUTPUT_ROTATION_UNDEFINED = 0,
    D3D11OUTPUT_ROTATION_IDENTITY,
    D3D11OUTPUT_ROTATION_90,
    D3D11OUTPUT_ROTATION_180,
    D3D11OUTPUT_ROTATION_270,
    D3D11OUTPUT_ROTATION_COUNT
};
extern D3D11OutputRotation D3D11OutputRotationFromDXGI[D3D11OUTPUT_ROTATION_COUNT];
extern DXGI_MODE_ROTATION D3D11OutputRotationToDXGI[D3D11OUTPUT_ROTATION_COUNT];

typedef struct _d3d11_output_desc {
    Void ConvertFrom( const DXGI_OUTPUT_DESC * pDesc, UInt iOutput );
    Void ConvertTo( DXGI_OUTPUT_DESC * outDesc, UInt * outOutput ) const;
    inline Void ConvertFrom( const __DXGI_OUTPUT_DESC * pDesc );
    inline Void ConvertTo( __DXGI_OUTPUT_DESC * outDesc ) const;

    UInt iIndex;
    
    GChar strDeviceName[32];
    Void * pMonitor;

    D3D11OutputRotation iRotation;

    Bool bAttachedToDesktop;
    Long iDesktopLeft;
    Long iDesktopRight;
    Long iDesktopTop;
    Long iDesktopBottom;
} D3D11OutputDesc;

enum D3D11DisplayModeScanlineOrdering {
    D3D11DISPLAYMODE_SCANLINE_UNDEFINED = 0,
    D3D11DISPLAYMODE_SCANLINE_PROGRESSIVE,
    D3D11DISPLAYMODE_SCANLINE_UPPER_FIELD_FIRST,
    D3D11DISPLAYMODE_SCANLINE_LOWER_FIELD_FIRST,
    D3D11DISPLAYMODE_SCANLINE_COUNT
};
extern D3D11DisplayModeScanlineOrdering D3D11DisplayModeScanlineOrderingFromDXGI[D3D11DISPLAYMODE_SCANLINE_COUNT];
extern DXGI_MODE_SCANLINE_ORDER D3D11DisplayModeScanlineOrderingToDXGI[D3D11DISPLAYMODE_SCANLINE_COUNT];

enum D3D11DisplayModeScaling {
    D3D11DISPLAYMODE_SCALING_UNDEFINED = 0,
    D3D11DISPLAYMODE_SCALING_CENTERED,
    D3D11DISPLAYMODE_SCALING_STRETCHED,
    D3D11DISPLAYMODE_SCALING_COUNT
};
extern D3D11DisplayModeScaling D3D11DisplayModeScalingFromDXGI[D3D11DISPLAYMODE_SCALING_COUNT];
extern DXGI_MODE_SCALING D3D11DisplayModeScalingToDXGI[D3D11DISPLAYMODE_SCALING_COUNT];

typedef struct _d3d11_displaymode_desc {
    Void ConvertFrom( const DXGI_MODE_DESC * pDesc, UInt iDisplayMode );
    Void ConvertTo( DXGI_MODE_DESC * outDesc, UInt * outDisplayMode ) const;
    inline Void ConvertFrom( const __DXGI_MODE_DESC * pDesc, UInt iDisplayMode );
    inline Void ConvertTo( __DXGI_MODE_DESC * outDesc, UInt * outDisplayMode ) const;

    UInt iIndex;

    UInt iWidth;
    UInt iHeight;
    PixelFormat iFormat;
    UInt iRefreshRateNumerator;
    UInt iRefreshRateDenominator;

    D3D11DisplayModeScanlineOrdering iScanlineOrdering;
    D3D11DisplayModeScaling iScaling;
} D3D11DisplayModeDesc;

typedef struct _d3d11_gamma_caps {
    Void ConvertFrom( const DXGI_GAMMA_CONTROL_CAPABILITIES * pDesc );
    Void ConvertTo( DXGI_GAMMA_CONTROL_CAPABILITIES * outDesc ) const;

    Bool bScaleAndOffsetSupported;
    Float fMaxConvertedValue;
    Float fMinConvertedValue;

    UInt iControlPointCount;
    Float arrControlPoints[1025];
} D3D11GammaCaps;

typedef struct _d3d11_gamma_control {
    Void ConvertFrom( const DXGI_GAMMA_CONTROL * pDesc );
    Void ConvertTo( DXGI_GAMMA_CONTROL * outDesc ) const;

    struct _RGB { Float R; Float G; Float B; };
    _RGB vScale;
    _RGB vOffset;
    _RGB arrGammaCurve[1025];
} D3D11GammaControl;

/////////////////////////////////////////////////////////////////////////////////
// D3D11Renderer Declarations

// Forwards /////////////////////////////////////////////////////////////////////
struct IDXGIFactory;
struct ID2D1Factory;
struct IDWriteFactory;
struct IUnknown;

struct D3D11_FEATURE_DATA_THREADING;
struct D3D11_FEATURE_DATA_ARCHITECTURE_INFO;
struct D3D11_FEATURE_DATA_DOUBLES;
struct D3D11_FEATURE_DATA_SHADER_MIN_PRECISION_SUPPORT;
struct D3D11_FEATURE_DATA_D3D9_OPTIONS;
struct D3D11_FEATURE_DATA_D3D9_SHADOW_SUPPORT;
struct D3D11_FEATURE_DATA_D3D10_X_HARDWARE_OPTIONS;
struct D3D11_FEATURE_DATA_D3D11_OPTIONS;
struct D3D11_COUNTER_INFO;
struct ID3D11Device;
struct ID3D11DeviceChild;

enum DXGI_SCALING;
enum DXGI_ALPHA_MODE;
enum DXGI_SWAP_EFFECT;
struct DXGI_SWAP_CHAIN_DESC;
struct DXGI_FRAME_STATISTICS;
struct IDXGISwapChain;

// Proxies //////////////////////////////////////////////////////////////////////

// Definitions //////////////////////////////////////////////////////////////////
#define D3D11RENDERER_MAX_VERTEXBUFFER_SLOTS     32  // D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT
#define D3D11RENDERER_MAX_CONSTANTBUFFER_SLOTS   14  // D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT
#define D3D11RENDERER_MAX_SHADERSAMPLER_SLOTS    16  // D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT
#define D3D11RENDERER_MAX_SHADERRESOURCE_SLOTS   128 // D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT
#define D3D11RENDERER_MAX_STREAMOUTPUT_SLOTS     4   // D3D11_SO_BUFFER_SLOT_COUNT
#define D3D11RENDERER_MAX_VIEWPORT_SCISSOR_SLOTS 16  // D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE
#define D3D11RENDERER_MAX_RENDERTARGET_SLOTS     8   // D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT
#define D3D11RENDERER_MAX_SHADERUAV_SLOTS        8   // D3D11_PS_CS_UAV_REGISTER_COUNT
#define D3D11RENDERER_MAX_OUTPUTUAV_SLOTS        8   // D3D11_PS_CS_UAV_REGISTER_COUNT

#define D3D11RENDERER_MAX_SWAPCHAIN_BUFFERS         16
#define D3D11RENDERER_MAX_STREAMOUTPUT_DECLARATIONS 512 // (D3D11_SO_STREAM_COUNT * D3D11_SO_OUTPUT_COMPONENT_COUNT)

enum D3D11PixelFormatSupportFlags1 {
    D3D11PIXELFORMAT_SUPPORT1_NONE = 0,
    D3D11PIXELFORMAT_SUPPORT1_BUFFER                      = (1 << 0),
    D3D11PIXELFORMAT_SUPPORT1_IA_VERTEX_BUFFER            = (1 << 1),
    D3D11PIXELFORMAT_SUPPORT1_IA_INDEX_BUFFER             = (1 << 2),
    D3D11PIXELFORMAT_SUPPORT1_SO_BUFFER                   = (1 << 3),
    D3D11PIXELFORMAT_SUPPORT1_TEXTURE1D                   = (1 << 4),
    D3D11PIXELFORMAT_SUPPORT1_TEXTURE2D                   = (1 << 5),
    D3D11PIXELFORMAT_SUPPORT1_TEXTURE3D                   = (1 << 6),
    D3D11PIXELFORMAT_SUPPORT1_TEXTURECUBE                 = (1 << 7),
    D3D11PIXELFORMAT_SUPPORT1_SHADER_LOAD                 = (1 << 8),
    D3D11PIXELFORMAT_SUPPORT1_SHADER_SAMPLE               = (1 << 9),
    D3D11PIXELFORMAT_SUPPORT1_SHADER_SAMPLE_COMPARISON    = (1 << 10),
    D3D11PIXELFORMAT_SUPPORT1_SHADER_SAMPLE_MONO_TEXT     = (1 << 11),
    D3D11PIXELFORMAT_SUPPORT1_MIP                         = (1 << 12),
    D3D11PIXELFORMAT_SUPPORT1_MIP_AUTOGEN                 = (1 << 13),
    D3D11PIXELFORMAT_SUPPORT1_RENDER_TARGET               = (1 << 14),
    D3D11PIXELFORMAT_SUPPORT1_BLENDABLE                   = (1 << 15),
    D3D11PIXELFORMAT_SUPPORT1_DEPTH_STENCIL               = (1 << 16),
    D3D11PIXELFORMAT_SUPPORT1_CPU_LOCKABLE                = (1 << 17),
    D3D11PIXELFORMAT_SUPPORT1_MULTISAMPLE_RESOLVE         = (1 << 18),
    D3D11PIXELFORMAT_SUPPORT1_DISPLAY                     = (1 << 19),
    D3D11PIXELFORMAT_SUPPORT1_CAST_WITHIN_BIT_LAYOUT      = (1 << 20),
    D3D11PIXELFORMAT_SUPPORT1_MULTISAMPLE_RENDERTARGET    = (1 << 21),
    D3D11PIXELFORMAT_SUPPORT1_MULTISAMPLE_LOAD            = (1 << 22),
    D3D11PIXELFORMAT_SUPPORT1_SHADER_GATHER               = (1 << 23),
    D3D11PIXELFORMAT_SUPPORT1_BACK_BUFFER_CAST            = (1 << 24),
    D3D11PIXELFORMAT_SUPPORT1_TYPED_UNORDERED_ACCESS_VIEW = (1 << 25),
    D3D11PIXELFORMAT_SUPPORT1_SHADER_GATHER_COMPARISON    = (1 << 26),
    D3D11PIXELFORMAT_SUPPORT1_DECODER_OUTPUT              = (1 << 27),
    D3D11PIXELFORMAT_SUPPORT1_VIDEO_PROCESSOR_OUTPUT      = (1 << 28),
    D3D11PIXELFORMAT_SUPPORT1_VIDEO_PROCESSOR_INPUT       = (1 << 29),
    D3D11PIXELFORMAT_SUPPORT1_VIDEO_ENCODER               = (1 << 30),
    D3D11PIXELFORMAT_SUPPORT1_COUNT = 31
};
extern Byte D3D11PixelFormatSupportFlags1FromD3D11[D3D11PIXELFORMAT_SUPPORT1_COUNT];
extern Byte D3D11PixelFormatSupportFlags1ToD3D11[D3D11PIXELFORMAT_SUPPORT1_COUNT];

enum D3D11PixelFormatSupportFlags2 {
    D3D11PIXELFORMAT_SUPPORT2_NONE = 0,
    D3D11PIXELFORMAT_SUPPORT2_UAV_ATOMIC_ADD                               = (1 << 0),
    D3D11PIXELFORMAT_SUPPORT2_UAV_ATOMIC_BITWISE_OPS                       = (1 << 1),
    D3D11PIXELFORMAT_SUPPORT2_UAV_ATOMIC_COMPARE_STORE_OR_COMPARE_EXCHANGE = (1 << 2),
    D3D11PIXELFORMAT_SUPPORT2_UAV_ATOMIC_EXCHANGE                          = (1 << 3),
    D3D11PIXELFORMAT_SUPPORT2_UAV_ATOMIC_SIGNED_MIN_OR_MAX                 = (1 << 4),
    D3D11PIXELFORMAT_SUPPORT2_UAV_ATOMIC_UNSIGNED_MIN_OR_MAX               = (1 << 5),
    D3D11PIXELFORMAT_SUPPORT2_UAV_TYPED_LOAD                               = (1 << 6),
    D3D11PIXELFORMAT_SUPPORT2_UAV_TYPED_STORE                              = (1 << 7),
    D3D11PIXELFORMAT_SUPPORT2_OUTPUT_MERGER_LOGIC_OP                       = (1 << 8),
    //D3D11PIXELFORMAT_SUPPORT2_TILED     = D3D11_FORMAT_SUPPORT2_TILED,    // D3D11.2
    //D3D11PIXELFORMAT_SUPPORT2_SHAREABLE = D3D11_FORMAT_SUPPORT2_SHAREABLE // D3D11.2
    D3D11PIXELFORMAT_SUPPORT2_COUNT = 9
};
extern Byte D3D11PixelFormatSupportFlags2FromD3D11[D3D11PIXELFORMAT_SUPPORT2_COUNT];
extern Byte D3D11PixelFormatSupportFlags2ToD3D11[D3D11PIXELFORMAT_SUPPORT2_COUNT];

typedef struct _d3d11_pixelformat_support {
    Void ConvertFrom( UInt iD3D11Flags1, UInt iD3D11Flags2 );

    UInt iFlags1; // D3D11PixelFormatSupportFlags1
    UInt iFlags2; // D3D11PixelFormatSupportFlags2
} D3D11PixelFormatSupport;

typedef struct _d3d11_counter_support {
    Void ConvertFrom( const D3D11_COUNTER_INFO * pDesc );

    UInt iMaxCounters;
    UInt iMaxParallelCounters;
    UInt iMaxParallelUnitsDetection;
} D3D11CounterSupport;

typedef struct _d3d11_device_features {
    Void ConvertFrom( const D3D11_FEATURE_DATA_THREADING * pDescThreading, const D3D11_FEATURE_DATA_ARCHITECTURE_INFO * pDescArch,
                      const D3D11_FEATURE_DATA_DOUBLES * pDescDoubles, const D3D11_FEATURE_DATA_SHADER_MIN_PRECISION_SUPPORT * pDescMinPrecision,
                      const D3D11_FEATURE_DATA_D3D9_OPTIONS * pDescD3D9, const D3D11_FEATURE_DATA_D3D9_SHADOW_SUPPORT * pDescD3D9Shadows,
                      const D3D11_FEATURE_DATA_D3D10_X_HARDWARE_OPTIONS * pDescD3D10, const D3D11_FEATURE_DATA_D3D11_OPTIONS * pDescD3D11 );

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
} D3D11DeviceFeatures;

enum D3D11SwapChainSwapEffect {
    D3D11SWAPCHAIN_SWAPEFFECT_DISCARD = 0,
    D3D11SWAPCHAIN_SWAPEFFECT_SEQUENTIAL,
    D3D11SWAPCHAIN_SWAPEFFECT_FLIP, // Unsupported yet !
    D3D11SWAPCHAIN_SWAPEFFECT_FLIP_SEQUENTIAL,
    D3D11SWAPCHAIN_SWAPEFFECT_COUNT
};
extern D3D11SwapChainSwapEffect D3D11SwapChainSwapEffectFromDXGI[D3D11SWAPCHAIN_SWAPEFFECT_COUNT];
extern DXGI_SWAP_EFFECT D3D11SwapChainSwapEffectToDXGI[D3D11SWAPCHAIN_SWAPEFFECT_COUNT];

enum D3D11SwapChainBufferUsageFlags {
    D3D11SWAPCHAIN_BUFFERUSAGE_NONE = 0,
    D3D11SWAPCHAIN_BUFFERUSAGE_SHADER_INPUT         = (1 << 0),
    D3D11SWAPCHAIN_BUFFERUSAGE_RENDER_TARGET_OUTPUT = (1 << 1),
    D3D11SWAPCHAIN_BUFFERUSAGE_BACK_BUFFER          = (1 << 2),
    D3D11SWAPCHAIN_BUFFERUSAGE_SHARED               = (1 << 3),
    D3D11SWAPCHAIN_BUFFERUSAGE_READ_ONLY            = (1 << 4),
    D3D11SWAPCHAIN_BUFFERUSAGE_DISCARD_ON_PRESENT   = (1 << 5),
    D3D11SWAPCHAIN_BUFFERUSAGE_UNORDERED_ACCESS     = (1 << 6),
    D3D11SWAPCHAIN_BUFFERUSAGE_COUNT = 7
};
extern Byte D3D11SwapChainBufferUsageFlagsFromDXGI[D3D11SWAPCHAIN_BUFFERUSAGE_COUNT + 4];
extern Byte D3D11SwapChainBufferUsageFlagsToDXGI[D3D11SWAPCHAIN_BUFFERUSAGE_COUNT];

enum D3D11SwapChainFlags {
    D3D11SWAPCHAIN_FLAG_NONE = 0,
    D3D11SWAPCHAIN_FLAG_NONPREROTATED                   = (1 << 0),
    D3D11SWAPCHAIN_FLAG_ALLOW_MODE_SWITCH               = (1 << 1),
    D3D11SWAPCHAIN_FLAG_GDI_COMPATIBLE                  = (1 << 2),
    D3D11SWAPCHAIN_FLAG_RESTRICTED_CONTENT              = (1 << 3),
    D3D11SWAPCHAIN_FLAG_RESTRICT_SHARED_RESOURCE_DRIVER = (1 << 4),
    D3D11SWAPCHAIN_FLAG_DISPLAY_ONLY                    = (1 << 5),
    D3D11SWAPCHAIN_FLAG_COUNT = 6
};
extern Byte D3D11SwapChainFlagsFromDXGI[D3D11SWAPCHAIN_FLAG_COUNT];
extern Byte D3D11SwapChainFlagsToDXGI[D3D11SWAPCHAIN_FLAG_COUNT];

enum D3D11PresentFlags {
    D3D11PRESENT_FLAG_NONE = 0,
//  D3D11PRESENT_FLAG_TEST                = (1 << 0), // Never expose to the app, we manage this internally
    D3D11PRESENT_FLAG_DONT_SEQUENCE       = (1 << 1),
    D3D11PRESENT_FLAG_RESTART             = (1 << 2),
    D3D11PRESENT_FLAG_DONT_WAIT           = (1 << 3),
    D3D11PRESENT_FLAG_STEREO_PREFER_RIGHT = (1 << 4),
    D3D11PRESENT_FLAG_STEREO_TEMP_MONO    = (1 << 5),
    D3D11PRESENT_FLAG_RESTRICT_TO_OUTPUT  = (1 << 6),
    D3D11PRESENT_FLAG_COUNT = 7
};
extern Byte D3D11PresentFlagsFromDXGI[D3D11PRESENT_FLAG_COUNT];
extern Byte D3D11PresentFlagsToDXGI[D3D11PRESENT_FLAG_COUNT];

typedef struct _d3d11_swapchain_desc {
    Void ConvertFrom( const DXGI_SWAP_CHAIN_DESC * pDesc );
    Void ConvertTo( DXGI_SWAP_CHAIN_DESC * outDesc ) const;

    Void * pOutputWindow; // HWND
    Bool bWindowed;

    UInt iRefreshRateNumerator;
    UInt iRefreshRateDenominator;

    UInt iBufferCount;
    UInt iBufferUsageFlags; // D3D11SwapChainBufferUsageFlags

    UInt iWidth;
    UInt iHeight;

    PixelFormat iFormat;
    UInt iSampleCount;
    UInt iSampleQuality;

    D3D11DisplayModeScanlineOrdering iScanlineOrdering;
    D3D11DisplayModeScaling iScaling;

    D3D11SwapChainSwapEffect iSwapEffect;
    UInt iFlags; // D3D11SwapChainFlags
} D3D11SwapChainDesc;

typedef struct _d3d11_frame_stats {
    Void ConvertFrom( const DXGI_FRAME_STATISTICS * pDesc, UInt iLastPresentCount );
    Void ConvertTo( DXGI_FRAME_STATISTICS * outDesc, UInt * outLastPresentCount ) const;

    UInt iLastPresentCount;

    UInt iPresentCount;
    UInt iPresentRefreshCount;
    UInt iSyncRefreshCount;
    Int64 iSyncQPCTime;
    Int64 iSyncGPUTime;
} D3D11FrameStats;

/////////////////////////////////////////////////////////////////////////////////
// D3D11DeferredContext Declarations

// Forwards /////////////////////////////////////////////////////////////////////
struct ID3D11DeviceContext;
struct ID3D11CommandList;

// Proxies //////////////////////////////////////////////////////////////////////

// Definitions //////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// D3D11InputLayout Declarations

// Forwards /////////////////////////////////////////////////////////////////////
enum D3D11_INPUT_CLASSIFICATION;
struct D3D11_INPUT_ELEMENT_DESC;
struct ID3D11InputLayout;

// Proxies //////////////////////////////////////////////////////////////////////
typedef struct __D3D11_INPUT_ELEMENT_DESC {
    const char * SemanticName;
    unsigned int SemanticIndex;
    DXGI_FORMAT Format;
    unsigned int InputSlot;
    unsigned int AlignedByteOffset;
    D3D11_INPUT_CLASSIFICATION InputSlotClass;
    unsigned int InstanceDataStepRate;
} __D3D11_INPUT_ELEMENT_DESC;

// Definitions //////////////////////////////////////////////////////////////////
enum D3D11InputFieldType {
    D3D11INPUTFIELD_FLOAT  = PIXEL_FMT_R32F,
    D3D11INPUTFIELD_FLOAT2 = PIXEL_FMT_RG32F,
    D3D11INPUTFIELD_FLOAT3 = PIXEL_FMT_RGB32F,
    D3D11INPUTFIELD_FLOAT4 = PIXEL_FMT_RGBA32F,

    D3D11INPUTFIELD_BYTE  = PIXEL_FMT_R8UI,    D3D11INPUTFIELD_BYTEN  = PIXEL_FMT_R8UN,
    D3D11INPUTFIELD_BYTE2 = PIXEL_FMT_RG8UI,   D3D11INPUTFIELD_BYTE2N = PIXEL_FMT_RG8UN,
    D3D11INPUTFIELD_BYTE4 = PIXEL_FMT_RGBA8UI, D3D11INPUTFIELD_BYTE4N = PIXEL_FMT_RGBA8UN,

    D3D11INPUTFIELD_SSHORT  = PIXEL_FMT_R16SI,    D3D11INPUTFIELD_SSHORTN  = PIXEL_FMT_R16SN,
    D3D11INPUTFIELD_SSHORT2 = PIXEL_FMT_RG16SI,   D3D11INPUTFIELD_SSHORT2N = PIXEL_FMT_RG16SN,
    D3D11INPUTFIELD_SSHORT4 = PIXEL_FMT_RGBA16SI, D3D11INPUTFIELD_SSHORT4N = PIXEL_FMT_RGBA16SN,

    D3D11INPUTFIELD_USHORT  = PIXEL_FMT_R16UI,    D3D11INPUTFIELD_USHORTN  = PIXEL_FMT_R16UN,
    D3D11INPUTFIELD_USHORT2 = PIXEL_FMT_RG16UI,   D3D11INPUTFIELD_USHORT2N = PIXEL_FMT_RG16UN,
    D3D11INPUTFIELD_USHORT4 = PIXEL_FMT_RGBA16UI, D3D11INPUTFIELD_USHORT4N = PIXEL_FMT_RGBA16UN,

    D3D11INPUTFIELD_SINT  = PIXEL_FMT_R32SI,
    D3D11INPUTFIELD_SINT2 = PIXEL_FMT_RG32SI,
    D3D11INPUTFIELD_SINT3 = PIXEL_FMT_RGB32SI,
    D3D11INPUTFIELD_SINT4 = PIXEL_FMT_RGBA32SI,

    D3D11INPUTFIELD_UINT  = PIXEL_FMT_R32UI,
    D3D11INPUTFIELD_UINT2 = PIXEL_FMT_RG32UI,
    D3D11INPUTFIELD_UINT3 = PIXEL_FMT_RGB32UI,
    D3D11INPUTFIELD_UINT4 = PIXEL_FMT_RGBA32UI,

    D3D11INPUTFIELD_COUNT = 30
};
inline D3D11InputFieldType D3D11InputFieldTypeFromD3D11( DXGI_FORMAT iDXGIFormat );
inline DXGI_FORMAT D3D11InputFieldTypeToD3D11( D3D11InputFieldType iInputFieldType );

enum D3D11InputFieldSlotClass {
    D3D11INPUTFIELD_SLOTCLASS_PER_VERTEX = 0,
    D3D11INPUTFIELD_SLOTCLASS_PER_INSTANCE,
    D3D11INPUTFIELD_SLOTCLASS_COUNT
};
extern D3D11InputFieldSlotClass D3D11InputFieldSlotClassFromD3D11[D3D11INPUTFIELD_SLOTCLASS_COUNT];
extern D3D11_INPUT_CLASSIFICATION D3D11InputFieldSlotClassToD3D11[D3D11INPUTFIELD_SLOTCLASS_COUNT];

#define D3D11INPUTFIELD_SEMANTIC_POSITION      "POSITION"     // (Float4) Vertex positions [n]
#define D3D11INPUTFIELD_SEMANTIC_TPOSITION     "POSITIONT"    // (Float4) Vertex transformed position
#define D3D11INPUTFIELD_SEMANTIC_NORMAL        "NORMAL"       // (Float4) Vertex normals [n]
#define D3D11INPUTFIELD_SEMANTIC_TANGENT       "TANGENT"      // (Float4) Vertex tangents [n]
#define D3D11INPUTFIELD_SEMANTIC_BINORMAL      "BINORMAL"     // (Float4) Vertex binormals [n]
#define D3D11INPUTFIELD_SEMANTIC_COLOR         "COLOR"        // (Float4) Vertex diffuse/specular colors [n]
#define D3D11INPUTFIELD_SEMANTIC_TEXCOORD      "TEXCOORD"     // (Float4) Vertex texture coords [n]
#define D3D11INPUTFIELD_SEMANTIC_BLEND_INDICES "BLENDINDICES" // (UInt)  Vertex blend indices [n]
#define D3D11INPUTFIELD_SEMANTIC_BLEND_WEIGHTS "BLENDWEIGHT"  // (Float) Vertex blend weights [n]
#define D3D11INPUTFIELD_SEMANTIC_POINTSIZE     "PSIZE"        // (Float) Vertex point sizes [n]

typedef struct _d3d11_inputfield_desc {
    Void ConvertFrom( const D3D11_INPUT_ELEMENT_DESC * pDesc );
    Void ConvertTo( D3D11_INPUT_ELEMENT_DESC * outDesc ) const;
    inline Void ConvertFrom( const __D3D11_INPUT_ELEMENT_DESC * pDesc );
    inline Void ConvertTo( __D3D11_INPUT_ELEMENT_DESC * outDesc ) const;

    D3D11InputFieldType iType;

    const AChar * strSemantic;
    UInt iSemanticIndex;

    UInt iSlot; // 0-15
    D3D11InputFieldSlotClass iSlotClass;

    UInt iInstanceDataStepRate; // Per-Instance fields only, number of instances
                                // to pass with a given per-instance data before
                                // advancing to next per-instance data
} D3D11InputFieldDesc;

/////////////////////////////////////////////////////////////////////////////////
// D3D11RenderState Declarations

// Forwards /////////////////////////////////////////////////////////////////////
enum D3D11_FILTER;
enum D3D11_TEXTURE_ADDRESS_MODE;
enum D3D11_COMPARISON_FUNC;
struct D3D11_SAMPLER_DESC;
struct ID3D11SamplerState;

enum D3D11_FILL_MODE;
enum D3D11_CULL_MODE;
struct D3D11_RASTERIZER_DESC;
struct ID3D11RasterizerState;

enum D3D11_DEPTH_WRITE_MASK;
enum D3D11_STENCIL_OP;
struct D3D11_DEPTH_STENCIL_DESC;
struct ID3D11DepthStencilState;

enum D3D11_BLEND;
enum D3D11_BLEND_OP;
struct D3D11_BLEND_DESC;
struct ID3D11BlendState;

// Proxies //////////////////////////////////////////////////////////////////////
typedef struct __D3D11_SAMPLER_DESC {
    D3D11_FILTER Filter;
    D3D11_TEXTURE_ADDRESS_MODE AddressU;
    D3D11_TEXTURE_ADDRESS_MODE AddressV;
    D3D11_TEXTURE_ADDRESS_MODE AddressW;
    float MipLODBias;
    unsigned int MaxAnisotropy;
    D3D11_COMPARISON_FUNC ComparisonFunc;
    float BorderColor[ 4 ];
    float MinLOD;
    float MaxLOD;
} __D3D11_SAMPLER_DESC;

typedef struct __D3D11_RASTERIZER_DESC {
    D3D11_FILL_MODE FillMode;
    D3D11_CULL_MODE CullMode;
    int FrontCounterClockwise;
    int DepthBias;
    float DepthBiasClamp;
    float SlopeScaledDepthBias;
    int DepthClipEnable;
    int ScissorEnable;
    int MultisampleEnable;
    int AntialiasedLineEnable;
} __D3D11_RASTERIZER_DESC;

typedef struct __D3D11_DEPTH_STENCIL_DESC {
    int DepthEnable;
    D3D11_DEPTH_WRITE_MASK DepthWriteMask;
    D3D11_COMPARISON_FUNC DepthFunc;
    int StencilEnable;
    unsigned char StencilReadMask;
    unsigned char StencilWriteMask;
    struct __D3D11_DEPTH_STENCILOP_DESC {
        D3D11_STENCIL_OP StencilFailOp;
        D3D11_STENCIL_OP StencilDepthFailOp;
        D3D11_STENCIL_OP StencilPassOp;
        D3D11_COMPARISON_FUNC StencilFunc;
    } FrontFace, BackFace;
} __D3D11_DEPTH_STENCIL_DESC;

typedef struct __D3D11_BLEND_DESC {
    int AlphaToCoverageEnable;
    int IndependentBlendEnable;
    struct __D3D11_RENDER_TARGET_BLEND_DESC {
        int BlendEnable;
        D3D11_BLEND SrcBlend;
        D3D11_BLEND DestBlend;
        D3D11_BLEND_OP BlendOp;
        D3D11_BLEND SrcBlendAlpha;
        D3D11_BLEND DestBlendAlpha;
        D3D11_BLEND_OP BlendOpAlpha;
        unsigned char RenderTargetWriteMask;
    } RenderTarget[8];
} __D3D11_BLEND_DESC;

// Definitions //////////////////////////////////////////////////////////////////
enum D3D11SamplerFilterFlags {
    D3D11SAMPLER_FILTERFLAG_NONE = 0,
    D3D11SAMPLER_FILTERFLAG_MIP_LINEAR  = (1 << 0),
    D3D11SAMPLER_FILTERFLAG_MAG_LINEAR  = (1 << 1),
    D3D11SAMPLER_FILTERFLAG_MIN_LINEAR  = (1 << 2),
    D3D11SAMPLER_FILTERFLAG_ANISOTROPIC = (1 << 3),
    D3D11SAMPLER_FILTERFLAG_CMP         = (1 << 4),
    D3D11SAMPLER_FILTERFLAG_COUNT = 5
};
extern Byte D3D11SamplerFilterFlagsFromD3D11[D3D11SAMPLER_FILTERFLAG_COUNT + 3];
extern Byte D3D11SamplerFilterFlagsToD3D11[D3D11SAMPLER_FILTERFLAG_COUNT];

enum D3D11SamplerFilterMode {
    D3D11SAMPLER_FILTER_MIN_MAG_MIP_PPP    = D3D11SAMPLER_FILTERFLAG_NONE,
    D3D11SAMPLER_FILTER_MIN_MAG_MIP_PPL    = D3D11SAMPLER_FILTERFLAG_MIP_LINEAR,
    D3D11SAMPLER_FILTER_MIN_MAG_MIP_PLP    = D3D11SAMPLER_FILTERFLAG_MAG_LINEAR,
    D3D11SAMPLER_FILTER_MIN_MAG_MIP_PLL    = ( D3D11SAMPLER_FILTERFLAG_MAG_LINEAR | D3D11SAMPLER_FILTERFLAG_MIP_LINEAR ),
    D3D11SAMPLER_FILTER_MIN_MAG_MIP_LPP    = D3D11SAMPLER_FILTERFLAG_MIN_LINEAR,
    D3D11SAMPLER_FILTER_MIN_MAG_MIP_LPL    = ( D3D11SAMPLER_FILTERFLAG_MIN_LINEAR | D3D11SAMPLER_FILTERFLAG_MIP_LINEAR ),
    D3D11SAMPLER_FILTER_MIN_MAG_MIP_LLP    = ( D3D11SAMPLER_FILTERFLAG_MIN_LINEAR | D3D11SAMPLER_FILTERFLAG_MAG_LINEAR ),
    D3D11SAMPLER_FILTER_MIN_MAG_MIP_LLL    = ( D3D11SAMPLER_FILTERFLAG_MIN_LINEAR | D3D11SAMPLER_FILTERFLAG_MAG_LINEAR | D3D11SAMPLER_FILTERFLAG_MIP_LINEAR ),
    D3D11SAMPLER_FILTER_ANISOTROPIC        = ( D3D11SAMPLER_FILTERFLAG_ANISOTROPIC | D3D11SAMPLER_FILTER_MIN_MAG_MIP_LLL ),
    D3D11SAMPLER_FILTERCMP_MIN_MAG_MIP_PPP = ( D3D11SAMPLER_FILTERFLAG_CMP | D3D11SAMPLER_FILTER_MIN_MAG_MIP_PPP ),
    D3D11SAMPLER_FILTERCMP_MIN_MAG_MIP_PPL = ( D3D11SAMPLER_FILTERFLAG_CMP | D3D11SAMPLER_FILTER_MIN_MAG_MIP_PPL ),
    D3D11SAMPLER_FILTERCMP_MIN_MAG_MIP_PLP = ( D3D11SAMPLER_FILTERFLAG_CMP | D3D11SAMPLER_FILTER_MIN_MAG_MIP_PLP ),
    D3D11SAMPLER_FILTERCMP_MIN_MAG_MIP_PLL = ( D3D11SAMPLER_FILTERFLAG_CMP | D3D11SAMPLER_FILTER_MIN_MAG_MIP_PLL ),
    D3D11SAMPLER_FILTERCMP_MIN_MAG_MIP_LPP = ( D3D11SAMPLER_FILTERFLAG_CMP | D3D11SAMPLER_FILTER_MIN_MAG_MIP_LPP ),
    D3D11SAMPLER_FILTERCMP_MIN_MAG_MIP_LPL = ( D3D11SAMPLER_FILTERFLAG_CMP | D3D11SAMPLER_FILTER_MIN_MAG_MIP_LPL ),
    D3D11SAMPLER_FILTERCMP_MIN_MAG_MIP_LLP = ( D3D11SAMPLER_FILTERFLAG_CMP | D3D11SAMPLER_FILTER_MIN_MAG_MIP_LLP ),
    D3D11SAMPLER_FILTERCMP_MIN_MAG_MIP_LLL = ( D3D11SAMPLER_FILTERFLAG_CMP | D3D11SAMPLER_FILTER_MIN_MAG_MIP_LLL ),
    D3D11SAMPLER_FILTERCMP_ANISOTROPIC     = ( D3D11SAMPLER_FILTERFLAG_CMP | D3D11SAMPLER_FILTER_ANISOTROPIC ),
};
inline D3D11SamplerFilterMode D3D11SamplerFilterModeFromD3D11( D3D11_FILTER iD3D11Filter );
inline D3D11_FILTER D3D11SamplerFilterModeToD3D11( D3D11SamplerFilterMode iFilterMode );

enum D3D11SamplerWrapMode {
    D3D11SAMPLER_WRAP_CLAMP = 0,
    D3D11SAMPLER_WRAP_BORDER,
    D3D11SAMPLER_WRAP_REPEAT,
    D3D11SAMPLER_WRAP_MIRROR_ONCE,
    D3D11SAMPLER_WRAP_MIRROR_REPEAT,
    D3D11SAMPLER_WRAP_COUNT
};
extern D3D11SamplerWrapMode D3D11SamplerWrapModeFromD3D11[D3D11SAMPLER_WRAP_COUNT + 1];
extern D3D11_TEXTURE_ADDRESS_MODE D3D11SamplerWrapModeToD3D11[D3D11SAMPLER_WRAP_COUNT];

enum D3D11SamplerCompareFunction {
    D3D11SAMPLER_COMPARE_NEVER = 0,
    D3D11SAMPLER_COMPARE_ALLWAYS,
    D3D11SAMPLER_COMPARE_EQUAL,
    D3D11SAMPLER_COMPARE_NOT_EQUAL,
    D3D11SAMPLER_COMPARE_LESSER,
    D3D11SAMPLER_COMPARE_LESSER_EQUAL,
    D3D11SAMPLER_COMPARE_GREATER,
    D3D11SAMPLER_COMPARE_GREATER_EQUAL,
    D3D11SAMPLER_COMPARE_COUNT
};
extern D3D11SamplerCompareFunction D3D11SamplerCompareFunctionFromD3D11[D3D11SAMPLER_COMPARE_COUNT + 1];
extern D3D11_COMPARISON_FUNC D3D11SamplerCompareFunctionToD3D11[D3D11SAMPLER_COMPARE_COUNT];

typedef struct _d3d11_samplerstate_desc {
    Void ConvertFrom( const D3D11_SAMPLER_DESC * pDesc );
    Void ConvertTo( D3D11_SAMPLER_DESC * outDesc ) const;
    inline Void ConvertFrom( const __D3D11_SAMPLER_DESC * pDesc );
    inline Void ConvertTo( __D3D11_SAMPLER_DESC * outDesc ) const;

    D3D11SamplerFilterMode iFilterMode;

    D3D11SamplerWrapMode iWrapModeU;
    D3D11SamplerWrapMode iWrapModeV;
    D3D11SamplerWrapMode iWrapModeW;

    Float arrBorderColor[4];

    Float fMinLOD;
    Float fMaxLOD;
    Float fLODBias;

    UInt iMaxAnisotropy;

    D3D11SamplerCompareFunction iCompareFunction;
} D3D11SamplerStateDesc;

enum D3D11RasterizerFillMode {
    D3D11RASTERIZER_FILL_WIREFRAME = 0,
    D3D11RASTERIZER_FILL_SOLID,
    D3D11RASTERIZER_FILL_COUNT
};
extern D3D11RasterizerFillMode D3D11RasterizerFillModeFromD3D11[D3D11RASTERIZER_FILL_COUNT + 2];
extern D3D11_FILL_MODE D3D11RasterizerFillModeToD3D11[D3D11RASTERIZER_FILL_COUNT];

enum D3D11RasterizerCullMode {
    D3D11RASTERIZER_CULL_NONE = 0,
    D3D11RASTERIZER_CULL_FRONT,
    D3D11RASTERIZER_CULL_BACK,
    D3D11RASTERIZER_CULL_COUNT
};
extern D3D11RasterizerCullMode D3D11RasterizerCullModeFromD3D11[D3D11RASTERIZER_CULL_COUNT + 1];
extern D3D11_CULL_MODE D3D11RasterizerCullModeToD3D11[D3D11RASTERIZER_CULL_COUNT];

typedef struct _d3d11_rasterizerstate_desc {
    Void ConvertFrom( const D3D11_RASTERIZER_DESC * pDesc );
    Void ConvertTo( D3D11_RASTERIZER_DESC * outDesc ) const;
    inline Void ConvertFrom( const __D3D11_RASTERIZER_DESC * pDesc );
    inline Void ConvertTo( __D3D11_RASTERIZER_DESC * outDesc ) const;

    D3D11RasterizerFillMode iFillMode;
    D3D11RasterizerCullMode iCullMode;
    Bool bFrontCounterClockwise;
    
    Int iDepthBias;
    Float fDepthBiasClamp;
    Float fSlopeScaledDepthBias;

    Bool bDepthClipEnabled;

    Bool bScissorEnabled;

    Bool bMultisampleEnabled;
    Bool bAntialiasedLineEnabled;
} D3D11RasterizerStateDesc;

enum D3D11DepthWriteMask {
    D3D11DEPTH_WRITEMASK_ZERO = 0,
    D3D11DEPTH_WRITEMASK_ALL,
    D3D11DEPTH_WRITEMASK_COUNT
};
extern D3D11DepthWriteMask D3D11DepthWriteMaskFromD3D11[D3D11DEPTH_WRITEMASK_COUNT];
extern D3D11_DEPTH_WRITE_MASK D3D11DepthWriteMaskToD3D11[D3D11DEPTH_WRITEMASK_COUNT];

enum D3D11StencilOperation {
    D3D11STENCIL_OP_KEEP = 0,
    D3D11STENCIL_OP_ZERO,
    D3D11STENCIL_OP_INVERT,
    D3D11STENCIL_OP_REPLACE,
    D3D11STENCIL_OP_INCREMENT,
    D3D11STENCIL_OP_INCREMENT_SAT,
    D3D11STENCIL_OP_DECREMENT,
    D3D11STENCIL_OP_DECREMENT_SAT,
    D3D11STENCIL_OP_COUNT
};
extern D3D11StencilOperation D3D11StencilOperationFromD3D11[D3D11STENCIL_OP_COUNT + 1];
extern D3D11_STENCIL_OP D3D11StencilOperationToD3D11[D3D11STENCIL_OP_COUNT];

enum D3D11DepthStencilCompareFunction {
    D3D11DEPTHSTENCIL_COMPARE_NEVER = 0,
    D3D11DEPTHSTENCIL_COMPARE_ALLWAYS,
    D3D11DEPTHSTENCIL_COMPARE_EQUAL,
    D3D11DEPTHSTENCIL_COMPARE_NOT_EQUAL,
    D3D11DEPTHSTENCIL_COMPARE_LESSER,
    D3D11DEPTHSTENCIL_COMPARE_LESSER_EQUAL,
    D3D11DEPTHSTENCIL_COMPARE_GREATER,
    D3D11DEPTHSTENCIL_COMPARE_GREATER_EQUAL,
    D3D11DEPTHSTENCIL_COMPARE_COUNT
};
extern D3D11DepthStencilCompareFunction D3D11DepthStencilCompareFunctionFromD3D11[D3D11DEPTHSTENCIL_COMPARE_COUNT + 1];
extern D3D11_COMPARISON_FUNC D3D11DepthStencilCompareFunctionToD3D11[D3D11DEPTHSTENCIL_COMPARE_COUNT];

typedef struct _d3d11_depthstencilstate_desc {
    Void ConvertFrom( const D3D11_DEPTH_STENCIL_DESC * pDesc );
    Void ConvertTo( D3D11_DEPTH_STENCIL_DESC * outDesc ) const;
    inline Void ConvertFrom( const __D3D11_DEPTH_STENCIL_DESC * pDesc );
    inline Void ConvertTo( __D3D11_DEPTH_STENCIL_DESC * outDesc ) const;

    Bool bDepthEnabled;
    D3D11DepthWriteMask iDepthWriteMask;
    D3D11DepthStencilCompareFunction iDepthFunction;

    Bool bStencilEnabled;
    Byte iStencilReadMask;
    Byte iStencilWriteMask;

    struct {
        D3D11StencilOperation iOnStencilFail;
        D3D11StencilOperation iOnStencilDepthFail;
        D3D11StencilOperation iOnStencilPass;
        D3D11DepthStencilCompareFunction iStencilFunction;
    } hFrontFace, hBackFace;
} D3D11DepthStencilStateDesc;

enum D3D11BlendParameter {
    D3D11BLEND_PARAM_ZERO = 0,
    D3D11BLEND_PARAM_ONE,
    D3D11BLEND_PARAM_SRC_COLOR,
    D3D11BLEND_PARAM_SRC_COLOR_INV,
    D3D11BLEND_PARAM_SRC_ALPHA,
    D3D11BLEND_PARAM_SRC_ALPHA_INV,
    D3D11BLEND_PARAM_SRC_ALPHA_SAT,
    D3D11BLEND_PARAM_DST_COLOR,
    D3D11BLEND_PARAM_DST_COLOR_INV,
    D3D11BLEND_PARAM_DST_ALPHA,
    D3D11BLEND_PARAM_DST_ALPHA_INV,
    D3D11BLEND_PARAM_BLENDFACTOR,
    D3D11BLEND_PARAM_BLENDFACTOR_INV,
    D3D11BLEND_PARAM_SRC1_COLOR,
    D3D11BLEND_PARAM_SRC1_COLOR_INV,
    D3D11BLEND_PARAM_SRC1_ALPHA,
    D3D11BLEND_PARAM_SRC1_ALPHA_INV,
    D3D11BLEND_PARAM_COUNT
};
extern D3D11BlendParameter D3D11BlendParameterFromD3D11[D3D11BLEND_PARAM_COUNT + 3];
extern D3D11_BLEND D3D11BlendParameterToD3D11[D3D11BLEND_PARAM_COUNT];

enum D3D11BlendOperation {
    D3D11BLEND_OP_ADD = 0,
    D3D11BLEND_OP_SUB,
    D3D11BLEND_OP_SUB_REV,
    D3D11BLEND_OP_MIN,
    D3D11BLEND_OP_MAX,
    D3D11BLEND_OP_COUNT
};
extern D3D11BlendOperation D3D11BlendOperationFromD3D11[D3D11BLEND_OP_COUNT + 1];
extern D3D11_BLEND_OP D3D11BlendOperationToD3D11[D3D11BLEND_OP_COUNT];

enum D3D11BlendColorWriteMask {
    D3D11BLEND_COLORWRITEMASK_NONE  = 0,
    D3D11BLEND_COLORWRITEMASK_RED   = (1 << 0),
    D3D11BLEND_COLORWRITEMASK_GREEN = (1 << 1),
    D3D11BLEND_COLORWRITEMASK_BLUE  = (1 << 2),
    D3D11BLEND_COLORWRITEMASK_ALPHA = (1 << 3),
    D3D11BLEND_COLORWRITEMASK_ALL   = ( D3D11BLEND_COLORWRITEMASK_RED | D3D11BLEND_COLORWRITEMASK_GREEN |
                                        D3D11BLEND_COLORWRITEMASK_BLUE | D3D11BLEND_COLORWRITEMASK_ALPHA ),
    D3D11BLEND_COLORWRITEMASK_COUNT = 4
};
extern Byte D3D11BlendColorWriteMaskFromD3D11[D3D11BLEND_COLORWRITEMASK_COUNT];
extern Byte D3D11BlendColorWriteMaskToD3D11[D3D11BLEND_COLORWRITEMASK_COUNT];

typedef struct _d3d11_blendstate_desc {
    Void ConvertFrom( const D3D11_BLEND_DESC * pDesc );
    Void ConvertTo( D3D11_BLEND_DESC * outDesc ) const;
    inline Void ConvertFrom( const __D3D11_BLEND_DESC * pDesc );
    inline Void ConvertTo( __D3D11_BLEND_DESC * outDesc ) const;

    Bool bAlphaToCoverageEnabled;
    Bool bIndependentBlendEnabled;

    struct {
        Bool bBlendEnabled;

        D3D11BlendParameter iBlendSrc;
        D3D11BlendParameter iBlendSrcAlpha;
        D3D11BlendParameter iBlendDst;
        D3D11BlendParameter iBlendDstAlpha;
        D3D11BlendOperation iBlendOp;
        D3D11BlendOperation iBlendOpAlpha;

        Byte iColorWriteMask; // D3D11BlendColorWriteMask
    } arrRenderTargets[D3D11RENDERER_MAX_RENDERTARGET_SLOTS];
} D3D11BlendStateDesc;

/////////////////////////////////////////////////////////////////////////////////
// D3D11Asynchronous Declarations

// Forwards /////////////////////////////////////////////////////////////////////
struct ID3D11Asynchronous;

enum D3D11_QUERY;
struct ID3D11Query;
struct ID3D11Predicate;

enum D3D11_COUNTER;
enum D3D11_COUNTER_TYPE;
struct ID3D11Counter;

// Proxies //////////////////////////////////////////////////////////////////////
typedef struct __D3D11_QUERY_DESC {
    D3D11_QUERY Query;
    unsigned int MiscFlags;
} __D3D11_QUERY_DESC;

typedef struct __D3D11_COUNTER_DESC {
    D3D11_COUNTER Counter;
    unsigned int MiscFlags;
} __D3D11_COUNTER_DESC;

// Definitions //////////////////////////////////////////////////////////////////
enum D3D11QueryType {
    D3D11QUERY_EVENT = 0,
    D3D11QUERY_OCCLUSION,
    D3D11QUERY_TIMESTAMP_FREQUENCY,
    D3D11QUERY_TIMESTAMP,
    D3D11QUERY_STATS_PIPELINE,
    D3D11QUERY_STATS_STREAMOUTPUT,
    D3D11QUERY_STATS_STREAMOUTPUT_0,
    D3D11QUERY_STATS_STREAMOUTPUT_1,
    D3D11QUERY_STATS_STREAMOUTPUT_2,
    D3D11QUERY_STATS_STREAMOUTPUT_3,
    D3D11QUERY_COUNT
};
extern D3D11QueryType D3D11QueryTypeFromD3D11[D3D11QUERY_COUNT + 5];
extern D3D11_QUERY D3D11QueryTypeToD3D11[D3D11QUERY_COUNT];

enum D3D11PredicateType {
    D3D11PREDICATE_OCCLUSION = 0,
    D3D11PREDICATE_OVERFLOW_STREAMOUTPUT,
    D3D11PREDICATE_OVERFLOW_STREAMOUTPUT_0,
    D3D11PREDICATE_OVERFLOW_STREAMOUTPUT_1,
    D3D11PREDICATE_OVERFLOW_STREAMOUTPUT_2,
    D3D11PREDICATE_OVERFLOW_STREAMOUTPUT_3,
    D3D11PREDICATE_COUNT
};
extern D3D11PredicateType D3D11PredicateTypeFromD3D11[D3D11PREDICATE_COUNT + 10];
extern D3D11_QUERY D3D11PredicateTypeToD3D11[D3D11PREDICATE_COUNT];

enum D3D11CounterType {
    D3D11COUNTER_FLOAT = 0,
    D3D11COUNTER_WORD,
    D3D11COUNTER_DWORD,
    D3D11COUNTER_QWORD,
    D3D11COUNTER_COUNT
};
extern D3D11CounterType D3D11CounterTypeFromD3D11[D3D11COUNTER_COUNT];
extern D3D11_COUNTER_TYPE D3D11CounterTypeToD3D11[D3D11COUNTER_COUNT];

typedef struct _d3d11_counter_desc {
    UInt iCounterID;
    UInt iFlags; // Reserved : 0

    D3D11CounterType iType;
    UInt iActiveHardwareCounters;

    UInt iCounterNameLength;
    AChar strCounterName[64];

    UInt iUnitNameLength;
    AChar strUnitName[64];

    UInt iDescriptionLength;
    AChar strDescription[128];
} D3D11CounterDesc;

/////////////////////////////////////////////////////////////////////////////////
// D3D11Resource Declarations

// Forwards /////////////////////////////////////////////////////////////////////
enum D3D11_RESOURCE_DIMENSION;
enum D3D11_USAGE;
enum D3D11_MAP;
struct ID3D11Resource;

// Proxies //////////////////////////////////////////////////////////////////////

// Definitions //////////////////////////////////////////////////////////////////
enum D3D11ResourcePriority {
    D3D11RESOURCE_PRIORITY_MINIMUM = 0,
    D3D11RESOURCE_PRIORITY_LOW,
    D3D11RESOURCE_PRIORITY_NORMAL,
    D3D11RESOURCE_PRIORITY_HIGH,
    D3D11RESOURCE_PRIORITY_MAXIMUM,
    D3D11RESOURCE_PRIORITY_COUNT
};
inline D3D11ResourcePriority D3D11ResourcePriorityFromD3D11( DWord iD3D11ResourcePriority );
extern DWord D3D11ResourcePriorityToD3D11[D3D11RESOURCE_PRIORITY_COUNT];

enum D3D11ResourceUsage {
    // CPU ==> GPU operations = UpdateSubResource
    // CPU <=> GPU operations = Map / Unmap
    // GPU <=> GPU operations = CopyResource/CopySubResourceRegion
    D3D11RESOURCE_USAGE_DEFAULT = 0, // CPU : UpdateSubResource, Write-Only | GPU : Read/Write
    D3D11RESOURCE_USAGE_CONST,       // CPU : Initialize only               | GPU : Read-Only
    D3D11RESOURCE_USAGE_DYNAMIC,     // CPU : Map/Unmap, Write-Only         | GPU : Read-Only
    D3D11RESOURCE_USAGE_STAGING,     // CPU : Any, Read/Write               | GPU : Read/Write
    D3D11RESOURCE_USAGE_COUNT
};
extern D3D11ResourceUsage D3D11ResourceUsageFromD3D11[D3D11RESOURCE_USAGE_COUNT];
extern D3D11_USAGE D3D11ResourceUsageToD3D11[D3D11RESOURCE_USAGE_COUNT];

enum D3D11ResourceBind {
    D3D11RESOURCE_BIND_NONE = 0,
    D3D11RESOURCE_BIND_VERTEX_BUFFER    = (1 << 0), // Buffers only
    D3D11RESOURCE_BIND_INDEX_BUFFER     = (1 << 1), // Buffers only
    D3D11RESOURCE_BIND_CONSTANT_BUFFER  = (1 << 2), // Buffers only
    D3D11RESOURCE_BIND_SHADER_INPUT     = (1 << 3), // Buffers/Textures
    D3D11RESOURCE_BIND_STREAM_OUTPUT    = (1 << 4), // Buffers only
    D3D11RESOURCE_BIND_RENDER_TARGET    = (1 << 5), // Textures only
    D3D11RESOURCE_BIND_DEPTH_STENCIL    = (1 << 6), // Textures only
    D3D11RESOURCE_BIND_UNORDERED_ACCESS = (1 << 7), // Buffers/Textures, Pixel/Compute shaders only
    // D3D11_BIND_VIDEO_ENCODER, D3D11_BIND_DECODER // Video Codec APIs
    D3D11RESOURCE_BIND_COUNT = 8
};
extern Byte D3D11ResourceBindFromD3D11[D3D11RESOURCE_BIND_COUNT];
extern Byte D3D11ResourceBindToD3D11[D3D11RESOURCE_BIND_COUNT];

enum D3D11ResourceDimension {
    D3D11RESOURCE_DIMENSION_UNKNOWN = 0,
    D3D11RESOURCE_DIMENSION_BUFFER,
    D3D11RESOURCE_DIMENSION_1D,
    D3D11RESOURCE_DIMENSION_2D,
    D3D11RESOURCE_DIMENSION_3D,
    D3D11RESOURCE_DIMENSION_COUNT
};
extern D3D11ResourceDimension D3D11ResourceDimensionFromD3D11[D3D11RESOURCE_DIMENSION_COUNT];
extern D3D11_RESOURCE_DIMENSION D3D11ResourceDimensionToD3D11[D3D11RESOURCE_DIMENSION_COUNT];

enum D3D11ResourceLock {
    D3D11RESOURCE_LOCK_NONE = 0,
    D3D11RESOURCE_LOCK_READ,
    D3D11RESOURCE_LOCK_WRITE,
    D3D11RESOURCE_LOCK_READ_WRITE,
    D3D11RESOURCE_LOCK_WRITE_DISCARD,      // Start updating whole buffer
    D3D11RESOURCE_LOCK_WRITE_NO_OVERWRITE, // Sub-sequent buffer updates
    D3D11RESOURCE_LOCK_COUNT
};
extern D3D11ResourceLock D3D11ResourceLockFromD3D11[D3D11RESOURCE_LOCK_COUNT];
extern D3D11_MAP D3D11ResourceLockToD3D11[D3D11RESOURCE_LOCK_COUNT];

enum D3D11ResourceLockFlags {
    D3D11RESOURCE_LOCKFLAG_NONE = 0,
    D3D11RESOURCE_LOCKFLAG_DONT_WAIT = (1 << 0),
    D3D11RESOURCE_LOCKFLAG_COUNT = 1
};
extern Byte D3D11ResourceLockFlagsFromD3D11[D3D11RESOURCE_LOCKFLAG_COUNT + 20];
extern Byte D3D11ResourceLockFlagsToD3D11[D3D11RESOURCE_LOCKFLAG_COUNT];

/////////////////////////////////////////////////////////////////////////////////
// D3D11Buffer Declarations

// Forwards /////////////////////////////////////////////////////////////////////
struct ID3D11Buffer;

// Proxies //////////////////////////////////////////////////////////////////////
typedef struct __D3D11_BUFFER_DESC {
    unsigned int ByteWidth;
    D3D11_USAGE Usage;
    unsigned int BindFlags;
    unsigned int CPUAccessFlags;
    unsigned int MiscFlags;
    unsigned int StructureByteStride;
} __D3D11_BUFFER_DESC;

// Definitions //////////////////////////////////////////////////////////////////
#define D3D11CONSTANTBUFFER_REGISTER_SIZE            16
#define D3D11CONSTANTBUFFER_REGISTER_COMPONENT_SIZE  4
#define D3D11CONSTANTBUFFER_REGISTER_COMPONENT_COUNT 4

/////////////////////////////////////////////////////////////////////////////////
// D3D11Texture Declarations

// Forwards /////////////////////////////////////////////////////////////////////
enum D3D11_TEXTURECUBE_FACE;
struct ID3D11Texture1D;
struct ID3D11Texture2D;
struct ID3D11Texture3D;

// Proxies //////////////////////////////////////////////////////////////////////
typedef struct __D3D11_TEXTURE1D_DESC {
    unsigned int Width;
    unsigned int MipLevels;
    unsigned int ArraySize;
    DXGI_FORMAT Format;
    D3D11_USAGE Usage;
    unsigned int BindFlags;
    unsigned int CPUAccessFlags;
    unsigned int MiscFlags;
} __D3D11_TEXTURE1D_DESC;

typedef struct __D3D11_TEXTURE2D_DESC {
    unsigned int Width;
    unsigned int Height;
    unsigned int MipLevels;
    unsigned int ArraySize;
    DXGI_FORMAT Format;
    struct __DXGI_SAMPLE_DESC {
        unsigned int Count;
        unsigned int Quality;
    } SampleDesc;
    D3D11_USAGE Usage;
    unsigned int BindFlags;
    unsigned int CPUAccessFlags;
    unsigned int MiscFlags;
} __D3D11_TEXTURE2D_DESC;

typedef struct __D3D11_TEXTURE3D_DESC {
    unsigned int Width;
    unsigned int Height;
    unsigned int Depth;
    unsigned int MipLevels;
    DXGI_FORMAT Format;
    D3D11_USAGE Usage;
    unsigned int BindFlags;
    unsigned int CPUAccessFlags;
    unsigned int MiscFlags;
} __D3D11_TEXTURE3D_DESC;

// Definitions //////////////////////////////////////////////////////////////////
typedef struct _d3d11_subresource_index {
    UInt iArraySlice, iMipSlice;
} D3D11SubResourceIndex;

typedef struct _d3d11_subresource_region {
    UInt iArraySlice, iMipSlice;
    UInt iArrayWidth, iMipHeight;
} D3D11SubResourceRegion;

enum D3D11TextureCubeFace {
    D3D11TEXTURE_CUBEFACE_X_POS = 0,
    D3D11TEXTURE_CUBEFACE_X_NEG,
    D3D11TEXTURE_CUBEFACE_Y_POS,
    D3D11TEXTURE_CUBEFACE_Y_NEG,
    D3D11TEXTURE_CUBEFACE_Z_POS,
    D3D11TEXTURE_CUBEFACE_Z_NEG,
    D3D11TEXTURE_CUBEFACE_COUNT
};
extern D3D11TextureCubeFace D3D11TextureCubeFaceFromD3D11[D3D11TEXTURE_CUBEFACE_COUNT];
extern D3D11_TEXTURECUBE_FACE D3D11TextureCubeFaceToD3D11[D3D11TEXTURE_CUBEFACE_COUNT];

/////////////////////////////////////////////////////////////////////////////////
// D3D11ResourceView Declarations

// Forwards /////////////////////////////////////////////////////////////////////
struct ID3D11View;

enum D3D11_RTV_DIMENSION;
struct D3D11_RENDER_TARGET_VIEW_DESC;
struct ID3D11RenderTargetView;

enum D3D11_DSV_DIMENSION;
struct D3D11_DEPTH_STENCIL_VIEW_DESC;
struct ID3D11DepthStencilView;

enum D3D_SRV_DIMENSION;
struct D3D11_SHADER_RESOURCE_VIEW_DESC;
struct ID3D11ShaderResourceView;

enum D3D11_UAV_DIMENSION;
struct D3D11_UNORDERED_ACCESS_VIEW_DESC;
struct ID3D11UnorderedAccessView;

// Proxies //////////////////////////////////////////////////////////////////////
typedef struct __D3D11_RENDER_TARGET_VIEW_DESC {
    DXGI_FORMAT Format;
    D3D11_RTV_DIMENSION ViewDimension;
    union {
        struct __D3D11_BUFFER_RTV {
            union { unsigned int FirstElement; unsigned int ElementOffset; };
            union { unsigned int NumElements; unsigned int ElementWidth; };
        } Buffer;
        struct __D3D11_TEX1D_RTV { unsigned int MipSlice; } Texture1D;
        struct __D3D11_TEX1D_ARRAY_RTV { unsigned int MipSlice; unsigned int FirstArraySlice; unsigned int ArraySize; } Texture1DArray;
        struct __D3D11_TEX2D_RTV { unsigned int MipSlice; } Texture2D;
        struct __D3D11_TEX2D_ARRAY_RTV { unsigned int MipSlice; unsigned int FirstArraySlice; unsigned int ArraySize; } Texture2DArray;
        struct __D3D11_TEX2DMS_RTV { unsigned int UnusedField_NothingToDefine; } Texture2DMS;
        struct __D3D11_TEX2DMS_ARRAY_RTV { unsigned int FirstArraySlice; unsigned int ArraySize; } Texture2DMSArray;
        struct __D3D11_TEX3D_RTV { unsigned int MipSlice; unsigned int FirstWSlice; unsigned int WSize; } Texture3D;
    };
} __D3D11_RENDER_TARGET_VIEW_DESC;

typedef struct __D3D11_DEPTH_STENCIL_VIEW_DESC {
    DXGI_FORMAT Format;
    D3D11_DSV_DIMENSION ViewDimension;
    unsigned int Flags;
    union {
        struct __D3D11_TEX1D_DSV { unsigned int MipSlice; } Texture1D;
        struct __D3D11_TEX1D_ARRAY_DSV { unsigned int MipSlice; unsigned int FirstArraySlice; unsigned int ArraySize; } Texture1DArray;
        struct __D3D11_TEX2D_DSV { unsigned int MipSlice; } Texture2D;
        struct __D3D11_TEX2D_ARRAY_DSV { unsigned int MipSlice; unsigned int FirstArraySlice; unsigned int ArraySize; } Texture2DArray;
        struct __D3D11_TEX2DMS_DSV { unsigned int UnusedField_NothingToDefine; } Texture2DMS;
        struct __D3D11_TEX2DMS_ARRAY_DSV { unsigned int FirstArraySlice; unsigned int ArraySize; } Texture2DMSArray;
    };
} __D3D11_DEPTH_STENCIL_VIEW_DESC;

typedef struct __D3D11_SHADER_RESOURCE_VIEW_DESC {
    DXGI_FORMAT Format;
    D3D_SRV_DIMENSION ViewDimension;
    union {
        struct __D3D11_BUFFER_SRV {
            union { unsigned int FirstElement; unsigned int ElementOffset; };
            union { unsigned int NumElements; unsigned int ElementWidth; };
        } Buffer;
        struct __D3D11_TEX1D_SRV { unsigned int MostDetailedMip; unsigned int MipLevels; } Texture1D;
        struct __D3D11_TEX1D_ARRAY_SRV { unsigned int MostDetailedMip; unsigned int MipLevels; unsigned int FirstArraySlice; unsigned int ArraySize; } Texture1DArray;
        struct __D3D11_TEX2D_SRV { unsigned int MostDetailedMip; unsigned int MipLevels; } Texture2D;
        struct __D3D11_TEX2D_ARRAY_SRV { unsigned int MostDetailedMip; unsigned int MipLevels; unsigned int FirstArraySlice; unsigned int ArraySize; } Texture2DArray;
        struct __D3D11_TEX2DMS_SRV { unsigned int UnusedField_NothingToDefine; } Texture2DMS;
        struct __D3D11_TEX2DMS_ARRAY_SRV { unsigned int FirstArraySlice; unsigned int ArraySize; } Texture2DMSArray;
        struct __D3D11_TEX3D_SRV { unsigned int MostDetailedMip; unsigned int MipLevels; }Texture3D;
        struct __D3D11_TEXCUBE_SRV { unsigned int MostDetailedMip; unsigned int MipLevels; } TextureCube;
        struct __D3D11_TEXCUBE_ARRAY_SRV { unsigned int MostDetailedMip; unsigned int MipLevels; unsigned int First2DArrayFace; unsigned int NumCubes; } TextureCubeArray;
        struct __D3D11_BUFFEREX_SRV { unsigned int FirstElement; unsigned int NumElements; unsigned int Flags; } BufferEx;
    };
} __D3D11_SHADER_RESOURCE_VIEW_DESC;

typedef struct __D3D11_UNORDERED_ACCESS_VIEW_DESC {
    DXGI_FORMAT Format;
    D3D11_UAV_DIMENSION ViewDimension;
    union {
        struct __D3D11_BUFFER_UAV { unsigned int FirstElement; unsigned int NumElements; unsigned int Flags; } Buffer;
        struct __D3D11_TEX1D_UAV { unsigned int MipSlice; } Texture1D;
        struct __D3D11_TEX1D_ARRAY_UAV { unsigned int MipSlice; unsigned int FirstArraySlice; unsigned int ArraySize; } Texture1DArray;
        struct __D3D11_TEX2D_UAV { unsigned int MipSlice; } Texture2D;
        struct __D3D11_TEX2D_ARRAY_UAV { unsigned int MipSlice; unsigned int FirstArraySlice; unsigned int ArraySize; } Texture2DArray;
        struct __D3D11_TEX3D_UAV { unsigned int MipSlice; unsigned int FirstWSlice; unsigned int WSize; } Texture3D;
    };
} __D3D11_UNORDERED_ACCESS_VIEW_DESC;

// Definitions //////////////////////////////////////////////////////////////////
enum D3D11RenderTargetViewDimension {
    D3D11RENDERTARGETVIEW_DIM_UNKNOWN = 0,
    D3D11RENDERTARGETVIEW_DIM_BUFFER,
    D3D11RENDERTARGETVIEW_DIM_TEXTURE1D,
    D3D11RENDERTARGETVIEW_DIM_TEXTURE1DARRAY,
    D3D11RENDERTARGETVIEW_DIM_TEXTURE2D,
    D3D11RENDERTARGETVIEW_DIM_TEXTURE2DARRAY,
    D3D11RENDERTARGETVIEW_DIM_TEXTURE2DMS,
    D3D11RENDERTARGETVIEW_DIM_TEXTURE2DMSARRAY,
    D3D11RENDERTARGETVIEW_DIM_TEXTURE3D,
    D3D11RENDERTARGETVIEW_DIM_COUNT
};
extern D3D11RenderTargetViewDimension D3D11RenderTargetViewDimensionFromD3D11[D3D11RENDERTARGETVIEW_DIM_COUNT];
extern D3D11_RTV_DIMENSION D3D11RenderTargetViewDimensionToD3D11[D3D11RENDERTARGETVIEW_DIM_COUNT];

typedef struct _d3d11_rendertarget_view_desc {
    Void ConvertFrom( const D3D11_RENDER_TARGET_VIEW_DESC * pDesc );
    Void ConvertTo( D3D11_RENDER_TARGET_VIEW_DESC * outDesc ) const;
    inline Void ConvertFrom( const __D3D11_RENDER_TARGET_VIEW_DESC * pDesc );
    inline Void ConvertTo( __D3D11_RENDER_TARGET_VIEW_DESC * outDesc ) const;

    PixelFormat iFormat;

    D3D11RenderTargetViewDimension iViewDimension;
    union {
        struct { UInt iOffset; UInt iSize; } hBuffer;
        struct { UInt iMipSlice; } hTexture1D;
        struct { UInt iMipSlice; UInt iArraySlice; UInt iArraySliceCount; } hTexture1DArray;
        struct { UInt iMipSlice; } hTexture2D;
        struct { UInt iMipSlice; UInt iArraySlice; UInt iArraySliceCount; } hTexture2DArray;
        struct { UInt _reserved; } hTexture2DMS;
        struct { UInt iArraySlice; UInt iArraySliceCount; } hTexture2DMSArray;
        struct { UInt iMipSlice; UInt iDepthSlice; UInt iDepthSliceCount; } hTexture3D;
    };
} D3D11RenderTargetViewDesc;

enum D3D11DepthStencilViewDimension {
    D3D11DEPTHSTENCILVIEW_DIM_UNKNOWN = 0,
    D3D11DEPTHSTENCILVIEW_DIM_TEXTURE1D,
    D3D11DEPTHSTENCILVIEW_DIM_TEXTURE1DARRAY,
    D3D11DEPTHSTENCILVIEW_DIM_TEXTURE2D,
    D3D11DEPTHSTENCILVIEW_DIM_TEXTURE2DARRAY,
    D3D11DEPTHSTENCILVIEW_DIM_TEXTURE2DMS,
    D3D11DEPTHSTENCILVIEW_DIM_TEXTURE2DMSARRAY,
    D3D11DEPTHSTENCILVIEW_DIM_COUNT
};
extern D3D11DepthStencilViewDimension D3D11DepthStencilViewDimensionFromD3D11[D3D11DEPTHSTENCILVIEW_DIM_COUNT];
extern D3D11_DSV_DIMENSION D3D11DepthStencilViewDimensionToD3D11[D3D11DEPTHSTENCILVIEW_DIM_COUNT];

enum D3D11DepthStencilViewFlags {
    D3D11DEPTHSTENCILVIEW_FLAG_NONE = 0,
    D3D11DEPTHSTENCILVIEW_FLAG_READONLY_DEPTH   = (1 << 0),
    D3D11DEPTHSTENCILVIEW_FLAG_READONLY_STENCIL = (1 << 1),
    D3D11DEPTHSTENCILVIEW_FLAG_COUNT = 2
};
extern Byte D3D11DepthStencilViewFlagsFromD3D11[D3D11DEPTHSTENCILVIEW_FLAG_COUNT];
extern Byte D3D11DepthStencilViewFlagsToD3D11[D3D11DEPTHSTENCILVIEW_FLAG_COUNT];

typedef struct _d3d11_depthstencil_view_desc {
    Void ConvertFrom( const D3D11_DEPTH_STENCIL_VIEW_DESC * pDesc );
    Void ConvertTo( D3D11_DEPTH_STENCIL_VIEW_DESC * outDesc ) const;
    inline Void ConvertFrom( const __D3D11_DEPTH_STENCIL_VIEW_DESC * pDesc );
    inline Void ConvertTo( __D3D11_DEPTH_STENCIL_VIEW_DESC * outDesc ) const;

    PixelFormat iFormat;
    UInt iFlags; // D3D11DepthStencilViewFlags

    D3D11DepthStencilViewDimension iViewDimension;
    union {
        struct { UInt iMipSlice; } hTexture1D;
        struct { UInt iMipSlice; UInt iArraySlice; UInt iArraySliceCount; } hTexture1DArray;
        struct { UInt iMipSlice; } hTexture2D;
        struct { UInt iMipSlice; UInt iArraySlice; UInt iArraySliceCount; } hTexture2DArray;
        struct { UInt _reserved; } hTexture2DMS;
        struct { UInt iArraySlice; UInt iArraySliceCount; } hTexture2DMSArray;
    };
} D3D11DepthStencilViewDesc;

enum D3D11ShaderViewDimension {
    D3D11SHADERVIEW_DIM_UNKNOWN = 0,
    D3D11SHADERVIEW_DIM_BUFFER,
    D3D11SHADERVIEW_DIM_BUFFEREX,
    D3D11SHADERVIEW_DIM_TEXTURE1D,
    D3D11SHADERVIEW_DIM_TEXTURE1DARRAY,
    D3D11SHADERVIEW_DIM_TEXTURE2D,
    D3D11SHADERVIEW_DIM_TEXTURE2DARRAY,
    D3D11SHADERVIEW_DIM_TEXTURE2DMS,
    D3D11SHADERVIEW_DIM_TEXTURE2DMSARRAY,
    D3D11SHADERVIEW_DIM_TEXTURE3D,
    D3D11SHADERVIEW_DIM_TEXTURECUBE,
    D3D11SHADERVIEW_DIM_TEXTURECUBEARRAY,
    D3D11SHADERVIEW_DIM_COUNT
};
extern D3D11ShaderViewDimension D3D11ShaderViewDimensionFromD3D11[D3D11SHADERVIEW_DIM_COUNT];
extern D3D_SRV_DIMENSION D3D11ShaderViewDimensionToD3D11[D3D11SHADERVIEW_DIM_COUNT];

enum D3D11ShaderViewBufferExFlags {
    D3D11SHADERVIEW_BUFFEREXFLAG_NONE = 0,
    D3D11SHADERVIEW_BUFFEREXFLAG_RAW = (1 << 0),
    D3D11SHADERVIEW_BUFFEREXFLAG_COUNT = 1
};
extern Byte D3D11ShaderViewBufferExFlagsFromD3D11[D3D11SHADERVIEW_BUFFEREXFLAG_COUNT];
extern Byte D3D11ShaderViewBufferExFlagsToD3D11[D3D11SHADERVIEW_BUFFEREXFLAG_COUNT];

typedef struct _d3d11_shader_view_desc {
    Void ConvertFrom( const D3D11_SHADER_RESOURCE_VIEW_DESC * pDesc );
    Void ConvertTo( D3D11_SHADER_RESOURCE_VIEW_DESC * outDesc ) const;
    inline Void ConvertFrom( const __D3D11_SHADER_RESOURCE_VIEW_DESC * pDesc );
    inline Void ConvertTo( __D3D11_SHADER_RESOURCE_VIEW_DESC * outDesc ) const;

    PixelFormat iFormat;

    D3D11ShaderViewDimension iViewDimension;
    union {
        struct { UInt iOffset; UInt iSize; } hBuffer;
        struct { UInt iOffset; UInt iSize; UInt iFlags; } hBufferEx; // D3D11ShaderViewBufferExFlags
        struct { UInt iMostDetailedMip; UInt iMipLevels; } hTexture1D;
        struct { UInt iMostDetailedMip; UInt iMipLevels; UInt iArraySlice; UInt iArraySliceCount; } hTexture1DArray;
        struct { UInt iMostDetailedMip; UInt iMipLevels; } hTexture2D;
        struct { UInt iMostDetailedMip; UInt iMipLevels; UInt iArraySlice; UInt iArraySliceCount; } hTexture2DArray;
        struct { UInt _reserved; } hTexture2DMS;
        struct { UInt iArraySlice; UInt iArraySliceCount; } hTexture2DMSArray;
        struct { UInt iMostDetailedMip; UInt iMipLevels; } hTexture3D;
        struct { UInt iMostDetailedMip; UInt iMipLevels; } hTextureCube;
        struct { UInt iMostDetailedMip; UInt iMipLevels; UInt iFirstFaceIndex; UInt iCubeCount; } hTextureCubeArray;
    };
} D3D11ShaderViewDesc;

enum D3D11UnorderedAccessViewDimension {
    D3D11UNORDEREDACCESSVIEW_DIM_UNKNOWN = 0,
    D3D11UNORDEREDACCESSVIEW_DIM_BUFFER,
    D3D11UNORDEREDACCESSVIEW_DIM_TEXTURE1D,
    D3D11UNORDEREDACCESSVIEW_DIM_TEXTURE1DARRAY,
    D3D11UNORDEREDACCESSVIEW_DIM_TEXTURE2D,
    D3D11UNORDEREDACCESSVIEW_DIM_TEXTURE2DARRAY,
    D3D11UNORDEREDACCESSVIEW_DIM_TEXTURE3D,
    D3D11UNORDEREDACCESSVIEW_DIM_COUNT
};
extern D3D11UnorderedAccessViewDimension D3D11UnorderedAccessViewDimensionFromD3D11[D3D11UNORDEREDACCESSVIEW_DIM_COUNT + 2];
extern D3D11_UAV_DIMENSION D3D11UnorderedAccessViewDimensionToD3D11[D3D11UNORDEREDACCESSVIEW_DIM_COUNT];

enum D3D11UnorderedAccessViewBufferFlags {
    D3D11UNORDEREDACCESSVIEW_BUFFERFLAG_NONE = 0,
    D3D11UNORDEREDACCESSVIEW_BUFFERFLAG_RAW     = (1 << 0),
    D3D11UNORDEREDACCESSVIEW_BUFFERFLAG_APPEND  = (1 << 1),
    D3D11UNORDEREDACCESSVIEW_BUFFERFLAG_COUNTER = (1 << 2),
    D3D11UNORDEREDACCESSVIEW_BUFFERFLAG_COUNT = 3
};
extern Byte D3D11UnorderedAccessViewBufferFlagsFromD3D11[D3D11UNORDEREDACCESSVIEW_BUFFERFLAG_COUNT];
extern Byte D3D11UnorderedAccessViewBufferFlagsToD3D11[D3D11UNORDEREDACCESSVIEW_BUFFERFLAG_COUNT];

typedef struct _d3d11_unorderedaccess_view_desc {
    Void ConvertFrom( const D3D11_UNORDERED_ACCESS_VIEW_DESC * pDesc );
    Void ConvertTo( D3D11_UNORDERED_ACCESS_VIEW_DESC * outDesc ) const;
    inline Void ConvertFrom( const __D3D11_UNORDERED_ACCESS_VIEW_DESC * pDesc );
    inline Void ConvertTo( __D3D11_UNORDERED_ACCESS_VIEW_DESC * outDesc ) const;

    PixelFormat iFormat;

    D3D11UnorderedAccessViewDimension iViewDimension;
    union {
        struct { UInt iOffset; UInt iSize; UInt iFlags; } hBuffer; // D3D11UnorderedAccessViewBufferFlags
        struct { UInt iMipSlice; } hTexture1D;
        struct { UInt iMipSlice; UInt iArraySlice; UInt iArraySliceCount; } hTexture1DArray;
        struct { UInt iMipSlice; } hTexture2D;
        struct { UInt iMipSlice; UInt iArraySlice; UInt iArraySliceCount; } hTexture2DArray;
        struct { UInt iMipSlice; UInt iDepthSlice; UInt iDepthSliceCount; } hTexture3D;
    };
} D3D11UnorderedAccessViewDesc;

/////////////////////////////////////////////////////////////////////////////////
// D3D11Shader Declarations

// Forwards /////////////////////////////////////////////////////////////////////
struct D3D11_SO_DECLARATION_ENTRY;

enum D3D_PRIMITIVE;
enum D3D_PRIMITIVE_TOPOLOGY;
enum D3D_TESSELLATOR_OUTPUT_PRIMITIVE;
enum D3D_TESSELLATOR_PARTITIONING;
enum D3D_TESSELLATOR_DOMAIN;
enum D3D_NAME;
enum D3D_REGISTER_COMPONENT_TYPE;
enum D3D_MIN_PRECISION;
enum _D3D_SHADER_INPUT_TYPE;     typedef enum _D3D_SHADER_INPUT_TYPE D3D_SHADER_INPUT_TYPE;
enum D3D_RESOURCE_RETURN_TYPE;
enum _D3D_CBUFFER_TYPE;          typedef enum _D3D_CBUFFER_TYPE          D3D_CBUFFER_TYPE;
enum _D3D_SHADER_VARIABLE_CLASS; typedef enum _D3D_SHADER_VARIABLE_CLASS D3D_SHADER_VARIABLE_CLASS;
enum _D3D_SHADER_VARIABLE_TYPE;  typedef enum _D3D_SHADER_VARIABLE_TYPE  D3D_SHADER_VARIABLE_TYPE;

struct _D3D11_SHADER_DESC;              typedef struct _D3D11_SHADER_DESC              D3D11_SHADER_DESC;
struct _D3D11_SIGNATURE_PARAMETER_DESC; typedef struct _D3D11_SIGNATURE_PARAMETER_DESC D3D11_SIGNATURE_PARAMETER_DESC;
struct _D3D11_SHADER_INPUT_BIND_DESC;   typedef struct _D3D11_SHADER_INPUT_BIND_DESC   D3D11_SHADER_INPUT_BIND_DESC;
struct _D3D11_SHADER_BUFFER_DESC;       typedef struct _D3D11_SHADER_BUFFER_DESC       D3D11_SHADER_BUFFER_DESC;
struct _D3D11_SHADER_TYPE_DESC;         typedef struct _D3D11_SHADER_TYPE_DESC         D3D11_SHADER_TYPE_DESC;
struct _D3D11_SHADER_VARIABLE_DESC;     typedef struct _D3D11_SHADER_VARIABLE_DESC     D3D11_SHADER_VARIABLE_DESC;

struct ID3D10Blob; typedef ID3D10Blob ID3DBlob;

struct ID3D11ShaderReflectionConstantBuffer;
struct ID3D11ShaderReflectionVariable;
struct ID3D11ShaderReflectionType;
struct ID3D11ShaderReflection;

struct ID3D11ClassLinkage;
struct ID3D11ClassInstance;

struct ID3D11VertexShader;
struct ID3D11GeometryShader;
struct ID3D11PixelShader;
struct ID3D11HullShader;
struct ID3D11DomainShader;
struct ID3D11ComputeShader;

// Proxies //////////////////////////////////////////////////////////////////////
typedef struct __D3D11_SO_DECLARATION_ENTRY {
    unsigned int Stream;
    const char * SemanticName;
    unsigned int SemanticIndex;
    unsigned char StartComponent;
    unsigned char ComponentCount;
    unsigned char OutputSlot;
} __D3D11_SO_DECLARATION_ENTRY;

// Definitions //////////////////////////////////////////////////////////////////
typedef struct _d3d11_streamoutput_declaration {
    Void ConvertFrom( const D3D11_SO_DECLARATION_ENTRY * pDesc );
    Void ConvertTo( D3D11_SO_DECLARATION_ENTRY * outDesc ) const;
    inline Void ConvertFrom( const __D3D11_SO_DECLARATION_ENTRY * pDesc );
    inline Void ConvertTo( __D3D11_SO_DECLARATION_ENTRY * outDesc ) const;

    UInt iStream;

    const AChar * strSemanticName; // Set to NULL to create a gap in the stream output with no written data
                                   // In this case, iComponentCount can be greater than 4
    UInt iSemanticIndex;

    Byte iStartComponent; // First component to write out, 0-3
    Byte iComponentCount; // Components to write out, 1-4
    Byte iOutputSlot;     // Associated stream output buffer, 0-3
} D3D11StreamOutputDeclaration;

enum D3D11ShaderCompilationFlags {
    D3D11SHADER_COMPILE_NONE = 0,

    D3D11SHADER_COMPILE_DEBUG                   = (1 << 0), // Add debug information to the output code.
    D3D11SHADER_COMPILE_WARNINGS_AS_ERRORS      = (1 << 1), // Treat warnings as errors
    D3D11SHADER_COMPILE_STRICT                  = (1 << 2), // Forces strict compile, might eject legacy syntax. Disabled on deprecated syntax by default.
    D3D11SHADER_COMPILE_STRICT_IEEE             = (1 << 3), // Forces IEEE strict compile.
    D3D11SHADER_COMPILE_BACKWARDS_COMPATIBILITY = (1 << 4), // Enable older shaders to compile to 5_0 targets.
    D3D11SHADER_COMPILE_SKIP_OPTIMIZATION       = (1 << 5), // Skip optimization steps during code generation. Debug purposes only.
    D3D11SHADER_COMPILE_SKIP_VALIDATION         = (1 << 6), // Don't validate the generated code. Trusted shaders only (tested & working).
    D3D11SHADER_COMPILE_VS_SOFTWARE_NO_OPT      = (1 << 7), // Compile vertexshader for next highest shader profile. Turns debug on / optimizations off.
    D3D11SHADER_COMPILE_PS_SOFTWARE_NO_OPT      = (1 << 8), // Compile pixelshader for next highest shader profile. Turns debug on / optimizations off.
    D3D11SHADER_COMPILE_NO_PRESHADER            = (1 << 9), // Disable Preshaders, the compiler does not pull out static expressions for evaluation.
    
    D3D11SHADER_COMPILE_PARTIAL_PRECISION        = (1 << 10), // Use partial precision. The compiled code might run faster on some hardware.
    D3D11SHADER_COMPILE_PACK_MATRIX_ROW_MAJOR    = (1 << 11), // Pack matrices in row-major order on input and output from the shader.
    D3D11SHADER_COMPILE_PACK_MATRIX_COLUMN_MAJOR = (1 << 12), // Pack matrices in column-major order on input and output from the shader (generally better for vector-matrix multiplication).
    D3D11SHADER_COMPILE_AVOID_FLOW_CONTROL       = (1 << 13), // Avoid flow-control constructs where possible.
    D3D11SHADER_COMPILE_PREFER_FLOW_CONTROL      = (1 << 14), // Prefer flow-control constructs where possible.

    D3D11SHADER_COMPILE_OPTIMIZATION_COMPILESPEED = (1 << 15), // The compiler might produce slower code but produces the code more quickly.
    D3D11SHADER_COMPILE_OPTIMIZATION_EXECUTESPEED = (1 << 16), // The compiler produces the best possible code but might take longer to do so.
    D3D11SHADER_COMPILE_OPTIMIZATION_LEVEL0       = D3D11SHADER_COMPILE_OPTIMIZATION_COMPILESPEED,
    D3D11SHADER_COMPILE_OPTIMIZATION_LEVEL1       = 0, // Default optimization level.
    D3D11SHADER_COMPILE_OPTIMIZATION_LEVEL2       = ( D3D11SHADER_COMPILE_OPTIMIZATION_COMPILESPEED | D3D11SHADER_COMPILE_OPTIMIZATION_EXECUTESPEED ),
    D3D11SHADER_COMPILE_OPTIMIZATION_LEVEL3       = D3D11SHADER_COMPILE_OPTIMIZATION_EXECUTESPEED,

    D3D11SHADER_COMPILE_COUNT = 17
};
extern Byte D3D11ShaderCompilationFlagsFromD3D11[D3D11SHADER_COMPILE_COUNT + 2];
extern Byte D3D11ShaderCompilationFlagsToD3D11[D3D11SHADER_COMPILE_COUNT];

enum D3D11ShaderRequirementFlags {
    D3D11SHADER_REQUIRES_NONE = 0,
    D3D11SHADER_REQUIRES_9_X_SHADOWS            = (1 << 0), // Direct3D 9 shadow support.
    D3D11SHADER_REQUIRES_11_1_DOUBLE_EXTENSIONS = (1 << 1), // Extended double instructions support.
    D3D11SHADER_REQUIRES_11_1_SHADER_EXTENSIONS = (1 << 2), // msad4 intrinsic function support. (SAD4)
    D3D11SHADER_REQUIRES_DOUBLES                = (1 << 3), // Double data type support.
    D3D11SHADER_REQUIRES_MINIMUM_PRECISION      = (1 << 4), // Minimum precision support.
    D3D11SHADER_REQUIRES_EARLY_DEPTH_STENCIL    = (1 << 5), // Shader requires an early depth stencil.
    D3D11SHADER_REQUIRES_UAVS_64                = (1 << 6), // Shader requires 64 UAVs.
    D3D11SHADER_REQUIRES_UAVS_AT_EVERY_STAGE    = (1 << 7), // Shader requires UAVs at every pipeline stage.
    D3D11SHADER_REQUIRES_COUNT = 8
};
extern Byte D3D11ShaderRequirementFlagsFromD3D11[D3D11SHADER_REQUIRES_COUNT];
extern Byte D3D11ShaderRequirementFlagsToD3D11[D3D11SHADER_REQUIRES_COUNT];

enum D3D11ShaderPrimitive {
    D3D11SHADER_PRIMITIVE_UNDEFINED = 0,
    D3D11SHADER_PRIMITIVE_POINT,
    D3D11SHADER_PRIMITIVE_LINE,
    D3D11SHADER_PRIMITIVE_TRIANGLE,
    D3D11SHADER_PRIMITIVE_LINE_ADJ,
    D3D11SHADER_PRIMITIVE_TRIANGLE_ADJ,
    D3D11SHADER_PRIMITIVE_CONTROL_POINT_PATCH_1,
    D3D11SHADER_PRIMITIVE_CONTROL_POINT_PATCH_2,
    D3D11SHADER_PRIMITIVE_CONTROL_POINT_PATCH_3,
    D3D11SHADER_PRIMITIVE_CONTROL_POINT_PATCH_4,
    D3D11SHADER_PRIMITIVE_CONTROL_POINT_PATCH_5,
    D3D11SHADER_PRIMITIVE_CONTROL_POINT_PATCH_6,
    D3D11SHADER_PRIMITIVE_CONTROL_POINT_PATCH_7,
    D3D11SHADER_PRIMITIVE_CONTROL_POINT_PATCH_8,
    D3D11SHADER_PRIMITIVE_CONTROL_POINT_PATCH_9,
    D3D11SHADER_PRIMITIVE_CONTROL_POINT_PATCH_10,
    D3D11SHADER_PRIMITIVE_CONTROL_POINT_PATCH_11,
    D3D11SHADER_PRIMITIVE_CONTROL_POINT_PATCH_12,
    D3D11SHADER_PRIMITIVE_CONTROL_POINT_PATCH_13,
    D3D11SHADER_PRIMITIVE_CONTROL_POINT_PATCH_14,
    D3D11SHADER_PRIMITIVE_CONTROL_POINT_PATCH_15,
    D3D11SHADER_PRIMITIVE_CONTROL_POINT_PATCH_16,
    D3D11SHADER_PRIMITIVE_CONTROL_POINT_PATCH_17,
    D3D11SHADER_PRIMITIVE_CONTROL_POINT_PATCH_18,
    D3D11SHADER_PRIMITIVE_CONTROL_POINT_PATCH_19,
    D3D11SHADER_PRIMITIVE_CONTROL_POINT_PATCH_20,
    D3D11SHADER_PRIMITIVE_CONTROL_POINT_PATCH_21,
    D3D11SHADER_PRIMITIVE_CONTROL_POINT_PATCH_22,
    D3D11SHADER_PRIMITIVE_CONTROL_POINT_PATCH_23,
    D3D11SHADER_PRIMITIVE_CONTROL_POINT_PATCH_24,
    D3D11SHADER_PRIMITIVE_CONTROL_POINT_PATCH_25,
    D3D11SHADER_PRIMITIVE_CONTROL_POINT_PATCH_26,
    D3D11SHADER_PRIMITIVE_CONTROL_POINT_PATCH_27,
    D3D11SHADER_PRIMITIVE_CONTROL_POINT_PATCH_28,
    D3D11SHADER_PRIMITIVE_CONTROL_POINT_PATCH_29,
    D3D11SHADER_PRIMITIVE_CONTROL_POINT_PATCH_30,
    D3D11SHADER_PRIMITIVE_CONTROL_POINT_PATCH_31,
    D3D11SHADER_PRIMITIVE_CONTROL_POINT_PATCH_32,
    D3D11SHADER_PRIMITIVE_COUNT
};
extern D3D11ShaderPrimitive D3D11ShaderPrimitiveFromD3D11[D3D11SHADER_PRIMITIVE_COUNT + 3];
extern D3D_PRIMITIVE D3D11ShaderPrimitiveToD3D11[D3D11SHADER_PRIMITIVE_COUNT];

enum D3D11ShaderPrimitiveTopology {
    D3D11SHADER_PRIMITIVETOPOLOGY_UNDEFINED = 0,
    D3D11SHADER_PRIMITIVETOPOLOGY_POINTLIST,
    D3D11SHADER_PRIMITIVETOPOLOGY_LINELIST,
    D3D11SHADER_PRIMITIVETOPOLOGY_LINESTRIP,
    D3D11SHADER_PRIMITIVETOPOLOGY_TRIANGLELIST,
    D3D11SHADER_PRIMITIVETOPOLOGY_TRIANGLESTRIP,
    D3D11SHADER_PRIMITIVETOPOLOGY_LINELIST_ADJ,
    D3D11SHADER_PRIMITIVETOPOLOGY_LINESTRIP_ADJ,
    D3D11SHADER_PRIMITIVETOPOLOGY_TRIANGLELIST_ADJ,
    D3D11SHADER_PRIMITIVETOPOLOGY_TRIANGLESTRIP_ADJ,
    D3D11SHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_1,
    D3D11SHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_2,
    D3D11SHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_3,
    D3D11SHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_4,
    D3D11SHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_5,
    D3D11SHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_6,
    D3D11SHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_7,
    D3D11SHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_8,
    D3D11SHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_9,
    D3D11SHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_10,
    D3D11SHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_11,
    D3D11SHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_12,
    D3D11SHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_13,
    D3D11SHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_14,
    D3D11SHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_15,
    D3D11SHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_16,
    D3D11SHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_17,
    D3D11SHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_18,
    D3D11SHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_19,
    D3D11SHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_20,
    D3D11SHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_21,
    D3D11SHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_22,
    D3D11SHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_23,
    D3D11SHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_24,
    D3D11SHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_25,
    D3D11SHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_26,
    D3D11SHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_27,
    D3D11SHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_28,
    D3D11SHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_29,
    D3D11SHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_30,
    D3D11SHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_31,
    D3D11SHADER_PRIMITIVETOPOLOGY_CONTROL_POINT_PATCHLIST_32,
    D3D11SHADER_PRIMITIVETOPOLOGY_COUNT
};
extern D3D11ShaderPrimitiveTopology _D3D11ShaderPrimitiveTopologyFromD3D11[D3D11SHADER_PRIMITIVETOPOLOGY_COUNT + 5];
D3D11ShaderPrimitiveTopology D3D11ShaderPrimitiveTopologyFromD3D11( D3D_PRIMITIVE_TOPOLOGY iD3DPrimitiveTopology );
extern D3D_PRIMITIVE_TOPOLOGY D3D11ShaderPrimitiveTopologyToD3D11[D3D11SHADER_PRIMITIVETOPOLOGY_COUNT];

enum D3D11ShaderTesselatorOutput {
    D3D11SHADER_TESSELLATOROUTPUT_UNDEFINED = 0,
    D3D11SHADER_TESSELLATOROUTPUT_POINT,
    D3D11SHADER_TESSELLATOROUTPUT_LINE,
    D3D11SHADER_TESSELLATOROUTPUT_TRIANGLE_CW,
    D3D11SHADER_TESSELLATOROUTPUT_TRIANGLE_CCW,
    D3D11SHADER_TESSELLATOROUTPUT_COUNT
};
extern D3D11ShaderTesselatorOutput D3D11ShaderTesselatorOutputFromD3D11[D3D11SHADER_TESSELLATOROUTPUT_COUNT];
extern D3D_TESSELLATOR_OUTPUT_PRIMITIVE D3D11ShaderTesselatorOutputToD3D11[D3D11SHADER_TESSELLATOROUTPUT_COUNT];

enum D3D11ShaderTesselatorPartitioning {
    D3D11SHADER_TESSELLATORPARTITIONING_UNDEFINED = 0,
    D3D11SHADER_TESSELLATORPARTITIONING_INTEGER,
    D3D11SHADER_TESSELLATORPARTITIONING_POW2,
    D3D11SHADER_TESSELLATORPARTITIONING_FRACTIONAL_ODD,
    D3D11SHADER_TESSELLATORPARTITIONING_FRACTIONAL_EVEN,
    D3D11SHADER_TESSELLATORPARTITIONING_COUNT
};
extern D3D11ShaderTesselatorPartitioning D3D11ShaderTesselatorPartitioningFromD3D11[D3D11SHADER_TESSELLATORPARTITIONING_COUNT];
extern D3D_TESSELLATOR_PARTITIONING D3D11ShaderTesselatorPartitioningToD3D11[D3D11SHADER_TESSELLATORPARTITIONING_COUNT];

enum D3D11ShaderTesselatorDomain {
    D3D11SHADER_TESSELLATORDOMAIN_UNDEFINED = 0,
    D3D11SHADER_TESSELLATORDOMAIN_ISOLINE,
    D3D11SHADER_TESSELLATORDOMAIN_TRIANGLE,
    D3D11SHADER_TESSELLATORDOMAIN_QUAD,
    D3D11SHADER_TESSELLATORDOMAIN_COUNT
};
extern D3D11ShaderTesselatorDomain D3D11ShaderTesselatorDomainFromD3D11[D3D11SHADER_TESSELLATORDOMAIN_COUNT];
extern D3D_TESSELLATOR_DOMAIN D3D11ShaderTesselatorDomainToD3D11[D3D11SHADER_TESSELLATORDOMAIN_COUNT];

typedef struct _d3d11_shader_desc {
    Void ConvertFrom( const D3D11_SHADER_DESC * pDesc, Bool bD3D11IsSampleFrequencyShader, UInt iD3D11RequirementFlags,
                      UInt iD3D11NumInterfaceSlots, UInt iD3D11BitwiseInstructionCount, UInt iD3D11ConversionInstructionCount,
                      UInt iD3D11MOVInstructionCount, UInt iD3D11MOVCInstructionCount );

    UInt iVersion;
    const AChar * strCreator;

    Bool bIsSampleFrequencyShader;

    UInt iCompilationFlags; // ShaderCompilationFlags
    UInt iRequirementFlags; // ShaderRequirementFlags

    UInt iInputParameterCount;
    UInt iOutputParameterCount;
    UInt iPatchConstantParameterCount;
    UInt iBindingCount;
    UInt iConstantBufferCount;

    UInt iInterfaceSlotCount;

    UInt iTempRegisterCount;
    UInt iTempArrayCount;
    
    UInt iInstructionCount;

    UInt iIntInstructionCount;
    UInt iUIntInstructionCount;
    UInt iFloatInstructionCount;
    UInt iArrayInstructionCount;

    UInt iBitwiseInstructionCount;
    UInt iConversionInstructionCount;
    UInt iMOVInstructionCount;
    UInt iMOVCInstructionCount;

    UInt iDefInstructionCount;
    UInt iDclInstructionCount;
    UInt iMacroInstructionCount;

    UInt iTextureNormalInstructionCount;
    UInt iTextureLoadInstructionCount;
    UInt iTextureCompInstructionCount;
    UInt iTextureBiasInstructionCount;
    UInt iTextureGradientInstructionCount;

    UInt iCutInstructionCount;
    UInt iEmitInstructionCount;

    UInt iStaticFlowControlCount;
    UInt iDynamicFlowControlCount;

    D3D11ShaderPrimitive iGSHSInputPrimitive;

    D3D11ShaderPrimitiveTopology iGSOutputTopology;
    UInt iGSMaxOutputVertexCount;
    UInt iGSInstanceCount;

    UInt iTesselatorControlPointCount;
    D3D11ShaderTesselatorDomain iTesselatorDomain;
    D3D11ShaderTesselatorOutput iHSOutputPrimitive;
    D3D11ShaderTesselatorPartitioning iHSPartitioningMode;

    UInt iCSBarrierInstructionCount;
    UInt iCSInterlockedInstructionCount;
    UInt iCSTextureStoreInstructionCount;
} D3D11ShaderDesc;

enum D3D11ShaderParameterType {
    D3D11SHADER_PARAMETER_UNDEFINED = 0,
    D3D11SHADER_PARAMETER_VERTEX_ID,
    D3D11SHADER_PARAMETER_PRIMITIVE_ID,
    D3D11SHADER_PARAMETER_INSTANCE_ID,
    D3D11SHADER_PARAMETER_POSITION,
    D3D11SHADER_PARAMETER_CLIP_DISTANCE,
    D3D11SHADER_PARAMETER_CULL_DISTANCE,
    D3D11SHADER_PARAMETER_IS_FRONT_FACE,
    D3D11SHADER_PARAMETER_SAMPLE_INDEX,
    D3D11SHADER_PARAMETER_TARGET,
    D3D11SHADER_PARAMETER_RENDER_TARGET_ARRAY_INDEX,
    D3D11SHADER_PARAMETER_VIEWPORT_ARRAY_INDEX,
    D3D11SHADER_PARAMETER_DEPTH,
    D3D11SHADER_PARAMETER_DEPTH_GREATER_EQUAL,
    D3D11SHADER_PARAMETER_DEPTH_LESS_EQUAL,
    D3D11SHADER_PARAMETER_COVERAGE,
    D3D11SHADER_PARAMETER_TESSFACTOR_FINAL_LINE_DETAIL,
    D3D11SHADER_PARAMETER_TESSFACTOR_FINAL_LINE_DENSITY,
    D3D11SHADER_PARAMETER_TESSFACTOR_FINAL_TRI_EDGE,
    D3D11SHADER_PARAMETER_TESSFACTOR_FINAL_TRI_INSIDE,
    D3D11SHADER_PARAMETER_TESSFACTOR_FINAL_QUAD_EDGE,
    D3D11SHADER_PARAMETER_TESSFACTOR_FINAL_QUAD_INSIDE,
    D3D11SHADER_PARAMETER_COUNT
};
extern D3D11ShaderParameterType _D3D11ShaderParameterTypeFromD3D11[D3D11SHADER_PARAMETER_COUNT + 1];
D3D11ShaderParameterType D3D11ShaderParameterTypeFromD3D11( D3D_NAME iD3DName );
extern D3D_NAME D3D11ShaderParameterTypeToD3D11[D3D11SHADER_PARAMETER_COUNT];

enum D3D11ShaderRegisterComponentType {
    D3D11SHADER_REGISTERCOMPONENT_UNKNOWN = 0,
    D3D11SHADER_REGISTERCOMPONENT_FLOAT32,
    D3D11SHADER_REGISTERCOMPONENT_UINT32,
    D3D11SHADER_REGISTERCOMPONENT_SINT32,
    D3D11SHADER_REGISTERCOMPONENT_COUNT
};
extern D3D11ShaderRegisterComponentType D3D11ShaderRegisterComponentTypeFromD3D11[D3D11SHADER_REGISTERCOMPONENT_COUNT];
extern D3D_REGISTER_COMPONENT_TYPE D3D11ShaderRegisterComponentTypeToD3D11[D3D11SHADER_REGISTERCOMPONENT_COUNT];

enum D3D11ShaderMinPrecision {
    D3D11SHADER_MINPRECISION_FLOAT_32 = 0, // 32 bits
    D3D11SHADER_MINPRECISION_FLOAT_16,     // 16 bits
    D3D11SHADER_MINPRECISION_FLOAT_2_8,    // 10 bits
    D3D11SHADER_MINPRECISION_UINT_16,      // 16 bits
    D3D11SHADER_MINPRECISION_SINT_16,      // 16 bits
    D3D11SHADER_MINPRECISION_ANY_16,       // 16 bits
    D3D11SHADER_MINPRECISION_ANY_10,       // 10 bits
    D3D11SHADER_MINPRECISION_COUNT
};
extern D3D11ShaderMinPrecision _D3D11ShaderMinPrecisionFromD3D11[D3D11SHADER_MINPRECISION_COUNT + 2];
D3D11ShaderMinPrecision D3D11ShaderMinPrecisionFromD3D11( D3D_MIN_PRECISION iD3DMinPrecision );
extern D3D_MIN_PRECISION D3D11ShaderMinPrecisionToD3D11[D3D11SHADER_MINPRECISION_COUNT];

typedef struct _d3d11_shaderparameter_desc {
    Void ConvertFrom( const D3D11_SIGNATURE_PARAMETER_DESC * pDesc );

    UInt iStreamIndex;

    const AChar * strSemanticName;
    UInt iSemanticIndex;

    UInt iRegister;

    D3D11ShaderParameterType iSystemValueType;
    D3D11ShaderRegisterComponentType iComponentType;

    Byte iComponentMask; // Components to use
    Byte iReadWriteMask; // Never written (output) / Allways read (input) hints

    D3D11ShaderMinPrecision iMinPrecision; // For interpolation
} D3D11ShaderParameterDesc;

enum D3D11ShaderInputType {
    D3D11SHADER_INPUT_CBUFFER = 0,
    D3D11SHADER_INPUT_TBUFFER,
    D3D11SHADER_INPUT_TEXTURE,
    D3D11SHADER_INPUT_SAMPLER,
    D3D11SHADER_INPUT_BYTEADDRESS,
    D3D11SHADER_INPUT_STRUCTURED,
    D3D11SHADER_INPUT_UAV_RWBYTEADDRESS,
    D3D11SHADER_INPUT_UAV_RWSTRUCTURED,
    D3D11SHADER_INPUT_UAV_RWSTRUCTURED_APPEND,
    D3D11SHADER_INPUT_UAV_RWSTRUCTURED_CONSUME,
    D3D11SHADER_INPUT_UAV_RWSTRUCTURED_WITH_COUNTER,
    D3D11SHADER_INPUT_UAV_RWTYPED,
    D3D11SHADER_INPUT_COUNT
};
extern D3D11ShaderInputType D3D11ShaderInputTypeFromD3D11[D3D11SHADER_INPUT_COUNT];
extern D3D_SHADER_INPUT_TYPE D3D11ShaderInputTypeToD3D11[D3D11SHADER_INPUT_COUNT];

enum D3D11ShaderInputFlags {
    D3D11SHADER_INPUTFLAG_NONE = 0,
    D3D11SHADER_INPUTFLAG_USERPACKED          = (1 << 0),
    D3D11SHADER_INPUTFLAG_COMPARISON_SAMPLER  = (1 << 1),
    D3D11SHADER_INPUTFLAG_TEXTURE_COMPONENT_0 = (1 << 2),
    D3D11SHADER_INPUTFLAG_TEXTURE_COMPONENT_1 = (1 << 3),
    D3D11SHADER_INPUTFLAG_UNUSED              = (1 << 4),
    D3D11SHADER_INPUTFLAG_TEXTURE_COMPONENTS  = ( D3D11SHADER_INPUTFLAG_TEXTURE_COMPONENT_0 | D3D11SHADER_INPUTFLAG_TEXTURE_COMPONENT_1),
    D3D11SHADER_INPUTFLAG_COUNT = 5
};
extern Byte D3D11ShaderInputFlagsFromD3D11[D3D11SHADER_INPUTFLAG_COUNT];
extern Byte D3D11ShaderInputFlagsToD3D11[D3D11SHADER_INPUTFLAG_COUNT];

enum D3D11ShaderReturnType {
    D3D11SHADER_RETURN_UNORM = 0,
    D3D11SHADER_RETURN_SNORM,
    D3D11SHADER_RETURN_UINT,
    D3D11SHADER_RETURN_SINT,
    D3D11SHADER_RETURN_FLOAT,
    D3D11SHADER_RETURN_DOUBLE,
    D3D11SHADER_RETURN_MIXED,
    D3D11SHADER_RETURN_CONTINUED,
    D3D11SHADER_RETURN_COUNT
};
extern D3D11ShaderReturnType D3D11ShaderReturnTypeFromD3D11[D3D11SHADER_RETURN_COUNT + 1];
extern D3D_RESOURCE_RETURN_TYPE D3D11ShaderReturnTypeToD3D11[D3D11SHADER_RETURN_COUNT];

typedef struct _d3d11_shaderbinding_desc {
    Void ConvertFrom( const D3D11_SHADER_INPUT_BIND_DESC * pDesc );

    const AChar * strName;

    D3D11ShaderInputType iType;
    UInt iFlags; // D3D11ShaderInputFlags

    UInt iBindPoint;
    UInt iBindCount;

    D3D11ShaderReturnType iReturnType;
    D3D11ShaderViewDimension iViewDimension;
    UInt iSampleCount; // 0 = Multisampling disabled
} D3D11ShaderBindingDesc;

enum D3D11ShaderConstantBufferType {
    D3D11SHADER_CONSTANTBUFFER_CBUFFER = 0,
    D3D11SHADER_CONSTANTBUFFER_TBUFFER,
    D3D11SHADER_CONSTANTBUFFER_INTERFACE_POINTERS,
    D3D11SHADER_CONSTANTBUFFER_RESOURCE_BIND_INFO,
    D3D11SHADER_CONSTANTBUFFER_COUNT
};
extern D3D11ShaderConstantBufferType D3D11ShaderConstantBufferTypeFromD3D11[D3D11SHADER_CONSTANTBUFFER_COUNT];
extern D3D_CBUFFER_TYPE D3D11ShaderConstantBufferTypeToD3D11[D3D11SHADER_CONSTANTBUFFER_COUNT];

enum D3D11ShaderConstantBufferFlags {
    D3D11SHADER_CONSTANTBUFFERFLAG_NONE = 0,
    D3D11SHADER_CONSTANTBUFFERFLAG_USERPACKED = (1 << 0),
    D3D11SHADER_CONSTANTBUFFERFLAG_COUNT = 1
};
extern Byte D3D11ShaderConstantBufferFlagsFromD3D11[D3D11SHADER_CONSTANTBUFFERFLAG_COUNT];
extern Byte D3D11ShaderConstantBufferFlagsToD3D11[D3D11SHADER_CONSTANTBUFFERFLAG_COUNT];

typedef struct _d3d11_shader_constantbuffer_desc {
    Void ConvertFrom( const D3D11_SHADER_BUFFER_DESC * pDesc );

    const AChar  * strName;
    
    D3D11ShaderConstantBufferType iType;

    UInt iByteSize;
    UInt iVariableCount;

    UInt iFlags; // D3D11ShaderConstantBufferFlags
} D3D11ShaderConstantBufferDesc;

enum D3D11ShaderVariableClass {
    D3D11SHADER_VARIABLECLASS_SCALAR = 0,
    D3D11SHADER_VARIABLECLASS_VECTOR,
    D3D11SHADER_VARIABLECLASS_MATRIX_ROWS,
    D3D11SHADER_VARIABLECLASS_MATRIX_COLUMNS,
    D3D11SHADER_VARIABLECLASS_STRUCT,
    D3D11SHADER_VARIABLECLASS_OBJECT,
    D3D11SHADER_VARIABLECLASS_INTERFACE_CLASS,
    D3D11SHADER_VARIABLECLASS_INTERFACE_POINTER,
    D3D11SHADER_VARIABLECLASS_COUNT
};
extern D3D11ShaderVariableClass D3D11ShaderVariableClassFromD3D11[D3D11SHADER_VARIABLECLASS_COUNT];
extern D3D_SHADER_VARIABLE_CLASS D3D11ShaderVariableClassToD3D11[D3D11SHADER_VARIABLECLASS_COUNT];

enum D3D11ShaderVariableType {
    D3D11SHADER_VARIABLE_VOID = 0,

    D3D11SHADER_VARIABLE_BOOL,

    D3D11SHADER_VARIABLE_BYTE,

    D3D11SHADER_VARIABLE_SHORT,
    D3D11SHADER_VARIABLE_USHORT,
    D3D11SHADER_VARIABLE_INT,
    D3D11SHADER_VARIABLE_UINT,

    D3D11SHADER_VARIABLE_FLOAT,
    D3D11SHADER_VARIABLE_DOUBLE,

    D3D11SHADER_VARIABLE_INT12,
    D3D11SHADER_VARIABLE_FLOAT8,
    D3D11SHADER_VARIABLE_FLOAT10,
    D3D11SHADER_VARIABLE_FLOAT16,

    D3D11SHADER_VARIABLE_INTERFACEPOINTER,

    D3D11SHADER_VARIABLE_STRING,

    D3D11SHADER_VARIABLE_BUFFER,
    D3D11SHADER_VARIABLE_RWBUFFER,

    D3D11SHADER_VARIABLE_CBUFFER,
    D3D11SHADER_VARIABLE_TBUFFER,

    D3D11SHADER_VARIABLE_RAWBUFFER,
    D3D11SHADER_VARIABLE_RWRAWBUFFER,

    D3D11SHADER_VARIABLE_STRUCTUREDBUFFER,
    D3D11SHADER_VARIABLE_RWSTRUCTUREDBUFFER,
    D3D11SHADER_VARIABLE_STRUCTUREDBUFFER_APPEND,
    D3D11SHADER_VARIABLE_STRUCTUREDBUFFER_CONSUME,

    D3D11SHADER_VARIABLE_TEXTURE,

    D3D11SHADER_VARIABLE_TEXTURE1D,
    D3D11SHADER_VARIABLE_TEXTURE1DARRAY,
    D3D11SHADER_VARIABLE_RWTEXTURE1D,
    D3D11SHADER_VARIABLE_RWTEXTURE1DARRAY,

    D3D11SHADER_VARIABLE_TEXTURE2D,
    D3D11SHADER_VARIABLE_TEXTURE2DARRAY,
    D3D11SHADER_VARIABLE_RWTEXTURE2D,
    D3D11SHADER_VARIABLE_RWTEXTURE2DARRAY,

    D3D11SHADER_VARIABLE_TEXTURE2DMS,
    D3D11SHADER_VARIABLE_TEXTURE2DMSARRAY,

    D3D11SHADER_VARIABLE_TEXTURE3D,
    D3D11SHADER_VARIABLE_RWTEXTURE3D,

    D3D11SHADER_VARIABLE_TEXTURECUBE,
    D3D11SHADER_VARIABLE_TEXTURECUBEARRAY,

    D3D11SHADER_VARIABLE_SAMPLER,
    D3D11SHADER_VARIABLE_SAMPLER1D,
    D3D11SHADER_VARIABLE_SAMPLER2D,
    D3D11SHADER_VARIABLE_SAMPLER3D,
    D3D11SHADER_VARIABLE_SAMPLERCUBE,

    D3D11SHADER_VARIABLE_VERTEXSHADER,
    D3D11SHADER_VARIABLE_GEOMETRYSHADER,
    D3D11SHADER_VARIABLE_PIXELSHADER,
    D3D11SHADER_VARIABLE_HULLSHADER,
    D3D11SHADER_VARIABLE_DOMAINSHADER,
    D3D11SHADER_VARIABLE_COMPUTESHADER,

    D3D11SHADER_VARIABLE_VERTEXFRAGMENT,
    D3D11SHADER_VARIABLE_PIXELFRAGMENT,

    D3D11SHADER_VARIABLE_RASTERIZER,
    D3D11SHADER_VARIABLE_DEPTHSTENCIL,
    D3D11SHADER_VARIABLE_BLEND,

    D3D11SHADER_VARIABLE_RENDERTARGETVIEW,
    D3D11SHADER_VARIABLE_DEPTHSTENCILVIEW,

    D3D11SHADER_VARIABLE_COUNT
};
extern D3D11ShaderVariableType D3D11ShaderVariableTypeFromD3D11[D3D11SHADER_VARIABLE_COUNT];
extern D3D_SHADER_VARIABLE_TYPE D3D11ShaderVariableTypeToD3D11[D3D11SHADER_VARIABLE_COUNT];

typedef struct _d3d11_shader_type_desc {
    Void ConvertFrom( const D3D11_SHADER_TYPE_DESC * pDesc, UInt iD3D11InterfaceCount );

    const AChar * strName; // Can be NULL

    D3D11ShaderVariableClass iClass; // Variable class (object, matrix, ...)
    D3D11ShaderVariableType iType;   // Variable type (float, sampler, ...)

    UInt iInterfaceCount;
    UInt iMemberCount;  // 0 if not a structure
    UInt iElementCount; // 0 if not an array

    UInt iColumnCount;  // for vectors & matrices, 1 for other numeric, 0 if not applicable
    UInt iRowCount;     // for matrices, 1 for other numeric, 0 if not applicable

    UInt iOffset;       // Offset from the start of structure (0 if not a structure member)
} D3D11ShaderTypeDesc;

enum D3D11ShaderVariableFlags {
    D3D11SHADER_VARIABLEFLAG_NONE = 0,
    D3D11SHADER_VARIABLEFLAG_USERPACKED          = (1 << 0),
    D3D11SHADER_VARIABLEFLAG_USED                = (1 << 1),
    D3D11SHADER_VARIABLEFLAG_INTERFACE_POINTER   = (1 << 2),
    D3D11SHADER_VARIABLEFLAG_INTERFACE_PARAMETER = (1 << 3),
    D3D11SHADER_VARIABLEFLAG_COUNT = 4
};
extern Byte D3D11ShaderVariableFlagsFromD3D11[D3D11SHADER_VARIABLEFLAG_COUNT];
extern Byte D3D11ShaderVariableFlagsToD3D11[D3D11SHADER_VARIABLEFLAG_COUNT];

typedef struct _d3d11_shader_variable_desc {
    Void ConvertFrom( const D3D11_SHADER_VARIABLE_DESC * pDesc );

    const AChar  * strName;

    UInt iStartOffset; // Offset in constant buffer's backing store
    UInt iByteSize;    // Size of variable (in bytes)

    UInt iStartTextureSlot; // First texture index (or -1 if no textures used)
    UInt iTextureSlotCount; // Number of texture slots possibly used.

    UInt iStartSamplerSlot; // First sampler index (or -1 if no textures used)
    UInt iSamplerSlotCount; // Number of sampler slots possibly used.

    UInt iFlags; // D3D11ShaderVariableFlags

    Void * pDefaultValue;
} D3D11ShaderVariableDesc;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "D3D11Mappings.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_D3D11MAPPINGS_H


