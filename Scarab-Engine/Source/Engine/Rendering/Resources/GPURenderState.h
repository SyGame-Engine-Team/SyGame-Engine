/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Resources/GPURenderState.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU Resource : Render States
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
#ifndef SCARAB_ENGINE_RENDERING_RESOURCES_GPURENDERSTATE_H
#define SCARAB_ENGINE_RENDERING_RESOURCES_GPURENDERSTATE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "GPUDeferredContext.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// GPURenderState definitions
enum GPURenderStateType {
    GPURENDERSTATE_SAMPLER      = DEVICERENDERSTATE_SAMPLER,
    GPURENDERSTATE_RASTERIZER   = DEVICERENDERSTATE_RASTERIZER,
    GPURENDERSTATE_DEPTHSTENCIL = DEVICERENDERSTATE_DEPTHSTENCIL,
    GPURENDERSTATE_BLEND        = DEVICERENDERSTATE_BLEND
};

// GPUSamplerState definitions
enum GPUSamplerFilterMode {
    GPUSAMPLER_FILTER_MINMAGMIP_PPP            = DEVICESAMPLER_FILTER_MIN_MAG_MIP_PPP,
    GPUSAMPLER_FILTER_MINMAGMIP_PPL            = DEVICESAMPLER_FILTER_MIN_MAG_MIP_PPL,
    GPUSAMPLER_FILTER_MINMAGMIP_PLP            = DEVICESAMPLER_FILTER_MIN_MAG_MIP_PLP,
    GPUSAMPLER_FILTER_MINMAGMIP_PLL            = DEVICESAMPLER_FILTER_MIN_MAG_MIP_PLL,
    GPUSAMPLER_FILTER_MINMAGMIP_LPP            = DEVICESAMPLER_FILTER_MIN_MAG_MIP_LPP,
    GPUSAMPLER_FILTER_MINMAGMIP_LPL            = DEVICESAMPLER_FILTER_MIN_MAG_MIP_LPL,
    GPUSAMPLER_FILTER_MINMAGMIP_LLP            = DEVICESAMPLER_FILTER_MIN_MAG_MIP_LLP,
    GPUSAMPLER_FILTER_MINMAGMIP_LLL            = DEVICESAMPLER_FILTER_MIN_MAG_MIP_LLL,
    GPUSAMPLER_FILTER_MINMAGMIP_ANISOTROPIC    = DEVICESAMPLER_FILTER_ANISOTROPIC,
    GPUSAMPLER_FILTERCMP_MINMAGMIP_PPP         = DEVICESAMPLER_FILTERCMP_MIN_MAG_MIP_PPP,
    GPUSAMPLER_FILTERCMP_MINMAGMIP_PPL         = DEVICESAMPLER_FILTERCMP_MIN_MAG_MIP_PPL,
    GPUSAMPLER_FILTERCMP_MINMAGMIP_PLP         = DEVICESAMPLER_FILTERCMP_MIN_MAG_MIP_PLP,
    GPUSAMPLER_FILTERCMP_MINMAGMIP_PLL         = DEVICESAMPLER_FILTERCMP_MIN_MAG_MIP_PLL,
    GPUSAMPLER_FILTERCMP_MINMAGMIP_LPP         = DEVICESAMPLER_FILTERCMP_MIN_MAG_MIP_LPP,
    GPUSAMPLER_FILTERCMP_MINMAGMIP_LPL         = DEVICESAMPLER_FILTERCMP_MIN_MAG_MIP_LPL,
    GPUSAMPLER_FILTERCMP_MINMAGMIP_LLP         = DEVICESAMPLER_FILTERCMP_MIN_MAG_MIP_LLP,
    GPUSAMPLER_FILTERCMP_MINMAGMIP_LLL         = DEVICESAMPLER_FILTERCMP_MIN_MAG_MIP_LLL,
    GPUSAMPLER_FILTERCMP_MINMAGMIP_ANISOTROPIC = DEVICESAMPLER_FILTERCMP_ANISOTROPIC
};

enum GPUSamplerWrapMode {
    GPUSAMPLER_WRAP_CLAMP         = DEVICESAMPLER_WRAP_CLAMP,
    GPUSAMPLER_WRAP_BORDER        = DEVICESAMPLER_WRAP_BORDER,
    GPUSAMPLER_WRAP_REPEAT        = DEVICESAMPLER_WRAP_REPEAT,
    GPUSAMPLER_WRAP_MIRROR_ONCE   = DEVICESAMPLER_WRAP_MIRROR_ONCE,
    GPUSAMPLER_WRAP_MIRROR_REPEAT = DEVICESAMPLER_WRAP_MIRROR_REPEAT
};

enum GPUSamplerCompareFunction {
    GPUSAMPLER_COMPARE_NEVER         = DEVICESAMPLER_COMPARE_NEVER,
    GPUSAMPLER_COMPARE_ALLWAYS       = DEVICESAMPLER_COMPARE_ALLWAYS,
    GPUSAMPLER_COMPARE_EQUAL         = DEVICESAMPLER_COMPARE_EQUAL,
    GPUSAMPLER_COMPARE_NOT_EQUAL     = DEVICESAMPLER_COMPARE_NOT_EQUAL,
    GPUSAMPLER_COMPARE_LESSER        = DEVICESAMPLER_COMPARE_LESSER,
    GPUSAMPLER_COMPARE_LESSER_EQUAL  = DEVICESAMPLER_COMPARE_LESSER_EQUAL,
    GPUSAMPLER_COMPARE_GREATER       = DEVICESAMPLER_COMPARE_GREATER,
    GPUSAMPLER_COMPARE_GREATER_EQUAL = DEVICESAMPLER_COMPARE_GREATER_EQUAL
};

typedef struct _gpu_samplerstate_desc {
    GPUSamplerFilterMode iFilterMode; // Default = GPUSAMPLER_FILTER_MINMAGMIP_LLL

    GPUSamplerWrapMode iWrapModeU; //
    GPUSamplerWrapMode iWrapModeV; // Default = Clamp
    GPUSamplerWrapMode iWrapModeW; //

    Float arrBorderColor[4]; // Default = (1,1,1,1)

    Float fMinLOD;  // Default = -FLOAT_INFINITE
    Float fMaxLOD;  // Default = FLOAT_INFINITE
    Float fLODBias; // Default = 0.0f

    UInt iMaxAnisotropy; // Default = 1

    GPUSamplerCompareFunction iCompareFunction; // Default = GPUSAMPLER_COMPARE_NEVER
} GPUSamplerStateDesc;

// GPURasterizerState definitions
enum GPURasterizerFillMode {
    GPURASTERIZER_FILL_WIREFRAME = DEVICERASTERIZER_FILL_WIREFRAME,
    GPURASTERIZER_FILL_SOLID     = DEVICERASTERIZER_FILL_SOLID
};

enum GPURasterizerCullMode {
    GPURASTERIZER_CULL_NONE  = DEVICERASTERIZER_CULL_NONE,
    GPURASTERIZER_CULL_FRONT = DEVICERASTERIZER_CULL_FRONT,
    GPURASTERIZER_CULL_BACK  = DEVICERASTERIZER_CULL_BACK
};

typedef struct _gpu_rasterizerstate_desc {
    GPURasterizerFillMode iFillMode; // Default = GPURASTERIZER_FILL_SOLID
    GPURasterizerCullMode iCullMode; // Default = GPURASTERIZER_CULL_BACK
    Bool bFrontCounterClockwise; // Default = true
    
    Int iDepthBias;              // Default = 0
    Float fDepthBiasClamp;       // Default = 0.0f
    Float fSlopeScaledDepthBias; // Default = 0.0f

    Bool bDepthClipEnabled; // Default = true

    Bool bScissorEnabled; // Default = false

    Bool bMultisampleEnabled;     // Default = false
    Bool bAntialiasedLineEnabled; // Default = false
} GPURasterizerStateDesc;

// GPUDepthStencilState definitions
enum GPUDepthWriteMask {
    GPUDEPTH_WRITEMASK_ZERO = DEVICEDEPTH_WRITEMASK_ZERO,
    GPUDEPTH_WRITEMASK_ALL  = DEVICEDEPTH_WRITEMASK_ALL
};

enum GPUStencilOperation {
    GPUSTENCIL_OP_KEEP          = DEVICESTENCIL_OP_KEEP,
    GPUSTENCIL_OP_ZERO          = DEVICESTENCIL_OP_ZERO,
    GPUSTENCIL_OP_INVERT        = DEVICESTENCIL_OP_INVERT,
    GPUSTENCIL_OP_REPLACE       = DEVICESTENCIL_OP_REPLACE,
    GPUSTENCIL_OP_INCREMENT     = DEVICESTENCIL_OP_INCREMENT,
    GPUSTENCIL_OP_INCREMENT_SAT = DEVICESTENCIL_OP_INCREMENT_SAT,
    GPUSTENCIL_OP_DECREMENT     = DEVICESTENCIL_OP_DECREMENT,
    GPUSTENCIL_OP_DECREMENT_SAT = DEVICESTENCIL_OP_DECREMENT_SAT
};

enum GPUDepthStencilCompareFunction {
    GPUDEPTHSTENCIL_COMPARE_NEVER         = DEVICEDEPTHSTENCIL_COMPARE_NEVER,
    GPUDEPTHSTENCIL_COMPARE_ALLWAYS       = DEVICEDEPTHSTENCIL_COMPARE_ALLWAYS,
    GPUDEPTHSTENCIL_COMPARE_EQUAL         = DEVICEDEPTHSTENCIL_COMPARE_EQUAL,
    GPUDEPTHSTENCIL_COMPARE_NOT_EQUAL     = DEVICEDEPTHSTENCIL_COMPARE_NOT_EQUAL,
    GPUDEPTHSTENCIL_COMPARE_LESSER        = DEVICEDEPTHSTENCIL_COMPARE_LESSER,
    GPUDEPTHSTENCIL_COMPARE_LESSER_EQUAL  = DEVICEDEPTHSTENCIL_COMPARE_LESSER_EQUAL,
    GPUDEPTHSTENCIL_COMPARE_GREATER_      = DEVICEDEPTHSTENCIL_COMPARE_GREATER,
    GPUDEPTHSTENCIL_COMPARE_GREATER_EQUAL = DEVICEDEPTHSTENCIL_COMPARE_GREATER_EQUAL
};

typedef struct _gpu_depthstencilstate_desc {
    Bool bDepthEnabled;                            // Default = true
    GPUDepthWriteMask iDepthWriteMask;             // Default = GPUDEPTH_WRITEMASK_ALL
    GPUDepthStencilCompareFunction iDepthFunction; // Default = GPUDEPTHSTENCIL_COMPARE_LESSER

    Bool bStencilEnabled;   // Default = false
    Byte iStencilReadMask;  // Default = 0xff
    Byte iStencilWriteMask; // Default = 0xff

    struct {
        GPUStencilOperation iOnStencilFail;              // Default = GPUSTENCIL_OP_KEEP
        GPUStencilOperation iOnStencilDepthFail;         // Default = GPUSTENCIL_OP_KEEP
        GPUStencilOperation iOnStencilPass;              // Default = GPUSTENCIL_OP_KEEP
        GPUDepthStencilCompareFunction iStencilFunction; // Default = GPUDEPTHSTENCIL_COMPARE_ALLWAYS
    } hFrontFace, hBackFace;
} GPUDepthStencilStateDesc;

// GPUBlendState definitions
#define GPUSHADER_MAX_RENDERTARGET_SLOTS DEVICERENDERER_MAX_RENDERTARGET_SLOTS

enum GPUBlendParameter {
    GPUBLEND_PARAM_ZERO            = DEVICEBLEND_PARAM_ZERO,
    GPUBLEND_PARAM_ONE             = DEVICEBLEND_PARAM_ONE,
    GPUBLEND_PARAM_SRC_COLOR       = DEVICEBLEND_PARAM_SRC_COLOR,
    GPUBLEND_PARAM_SRC_COLOR_INV   = DEVICEBLEND_PARAM_SRC_COLOR_INV,
    GPUBLEND_PARAM_SRC_ALPHA       = DEVICEBLEND_PARAM_SRC_ALPHA,
    GPUBLEND_PARAM_SRC_ALPHA_INV   = DEVICEBLEND_PARAM_SRC_ALPHA_INV,
    GPUBLEND_PARAM_SRC_ALPHA_SAT   = DEVICEBLEND_PARAM_SRC_ALPHA_SAT,
    GPUBLEND_PARAM_DST_COLOR       = DEVICEBLEND_PARAM_DST_COLOR,
    GPUBLEND_PARAM_DST_COLOR_INV   = DEVICEBLEND_PARAM_DST_COLOR_INV,
    GPUBLEND_PARAM_DST_ALPHA       = DEVICEBLEND_PARAM_DST_ALPHA,
    GPUBLEND_PARAM_DST_ALPHA_INV   = DEVICEBLEND_PARAM_DST_ALPHA_INV,
    GPUBLEND_PARAM_BLENDFACTOR     = DEVICEBLEND_PARAM_BLENDFACTOR,
    GPUBLEND_PARAM_BLENDFACTOR_INV = DEVICEBLEND_PARAM_BLENDFACTOR_INV,
    GPUBLEND_PARAM_SRC1_COLOR      = DEVICEBLEND_PARAM_SRC1_COLOR,
    GPUBLEND_PARAM_SRC1_COLOR_INV  = DEVICEBLEND_PARAM_SRC1_COLOR_INV,
    GPUBLEND_PARAM_SRC1_ALPHA      = DEVICEBLEND_PARAM_SRC1_ALPHA,
    GPUBLEND_PARAM_SRC1_ALPHA_INV  = DEVICEBLEND_PARAM_SRC1_ALPHA_INV
};

enum GPUBlendOperation {
    GPUBLEND_OP_ADD     = DEVICEBLEND_OP_ADD,
    GPUBLEND_OP_SUB     = DEVICEBLEND_OP_SUB,
    GPUBLEND_OP_SUB_REV = DEVICEBLEND_OP_SUB_REV,
    GPUBLEND_OP_MIN     = DEVICEBLEND_OP_MIN,
    GPUBLEND_OP_MAX     = DEVICEBLEND_OP_MAX
};

enum GPUBlendColorWriteMask {
    GPUBLEND_COLORWRITEMASK_RED   = DEVICEBLEND_COLORWRITEMASK_RED,
    GPUBLEND_COLORWRITEMASK_GREEN = DEVICEBLEND_COLORWRITEMASK_GREEN,
    GPUBLEND_COLORWRITEMASK_BLUE  = DEVICEBLEND_COLORWRITEMASK_BLUE,
    GPUBLEND_COLORWRITEMASK_ALPHA = DEVICEBLEND_COLORWRITEMASK_ALPHA,
    GPUBLEND_COLORWRITEMASK_ALL   = DEVICEBLEND_COLORWRITEMASK_ALL
};

typedef struct _gpu_blendstate_desc {
    Bool bAlphaToCoverageEnabled;  // Default = false
    Bool bIndependentBlendEnabled; // Default = false

    struct {
        Bool bBlendEnabled;   // Default = false

        GPUBlendParameter iBlendSrc;      // Default = GPUBLEND_PARAM_ONE
        GPUBlendParameter iBlendSrcAlpha; // Default = GPUBLEND_PARAM_ONE
        GPUBlendParameter iBlendDst;      // Default = GPUBLEND_PARAM_ZERO
        GPUBlendParameter iBlendDstAlpha; // Default = GPUBLEND_PARAM_ZERO
        GPUBlendOperation iBlendOp;       // Default = GPUBLEND_OP_ADD
        GPUBlendOperation iBlendOpAlpha;  // Default = GPUBLEND_OP_ADD

        Byte iColorWriteMask; // Default = GPUBLEND_COLORWRITEMASK_ALL
    } arrRenderTargets[GPUSHADER_MAX_RENDERTARGET_SLOTS];
} GPUBlendStateDesc;

// Prototypes
class RenderingManager;

/////////////////////////////////////////////////////////////////////////////////
// The GPURenderState class
class GPURenderState
{
protected:
    GPURenderState();
public:
    virtual ~GPURenderState();

    // Binding
    inline Bool IsBound() const;

    Void UnBind();

    // Getters
    inline GPURenderStateType GetType() const;

protected:
    friend class RenderingManager;

    // Device data
    DeviceRenderState * m_pDeviceRenderState;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPUSamplerState class
class GPUSamplerState : public GPURenderState
{
public:
    GPUSamplerState();
    virtual ~GPUSamplerState();

    // Binding
    Void Bind( const GPUSamplerStateDesc * pDesc = NULL );

    // Getters
    inline Void GetDesc( GPUSamplerStateDesc * outDesc ) const;

protected:
    friend class RenderingManager;

    // Device data
    DeviceSamplerState m_hDeviceSamplerState;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPURasterizerState class
class GPURasterizerState : public GPURenderState
{
public:
    GPURasterizerState();
    virtual ~GPURasterizerState();

    // Binding
    Void Bind( const GPURasterizerStateDesc * pDesc = NULL );

    // Getters
    inline Void GetDesc( GPURasterizerStateDesc * outDesc ) const;

protected:
    friend class RenderingManager;

    // Device data
    DeviceRasterizerState m_hDeviceRasterizerState;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPUDepthStencilState class
class GPUDepthStencilState : public GPURenderState
{
public:
    GPUDepthStencilState();
    virtual ~GPUDepthStencilState();

    // Binding
    Void Bind( const GPUDepthStencilStateDesc * pDesc = NULL );

    // Getters
    inline Void GetDesc( GPUDepthStencilStateDesc * outDesc ) const;

protected:
    friend class RenderingManager;

    // Device data
    DeviceDepthStencilState m_hDeviceDepthStencilState;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPUBlendState class
class GPUBlendState : public GPURenderState
{
public:
    GPUBlendState();
    virtual ~GPUBlendState();

    // Binding
    Void Bind( const GPUBlendStateDesc * pDesc = NULL );

    // Getters
    inline Void GetDesc( GPUBlendStateDesc * outDesc ) const;

protected:
    friend class RenderingManager;

    // Device data
    DeviceBlendState m_hDeviceBlendState;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GPURenderState.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RENDERING_RESOURCES_GPURENDERSTATE_H
