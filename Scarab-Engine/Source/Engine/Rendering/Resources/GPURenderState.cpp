/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Resources/GPURenderState.cpp
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
// Includes
#include "GPURenderState.h"

#include "../RenderingManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GPURenderState implementation
GPURenderState::GPURenderState()
{
    m_pDeviceRenderState = NULL;
}
GPURenderState::~GPURenderState()
{
    // nothing to do
}

Void GPURenderState::UnBind()
{
    Assert( IsBound() );

    RenderingFn->_UnRegisterRenderState( this );

    m_pDeviceRenderState->Destroy();
}

/////////////////////////////////////////////////////////////////////////////////
// GPUSamplerState implementation
GPUSamplerState::GPUSamplerState():
    GPURenderState(), m_hDeviceSamplerState( RenderingFn->m_pDeviceRenderer )
{
    m_pDeviceRenderState = &m_hDeviceSamplerState;
}
GPUSamplerState::~GPUSamplerState()
{
    if ( IsBound() )
        UnBind();
}

Void GPUSamplerState::Bind( const GPUSamplerStateDesc * pDesc )
{
    Assert( !(IsBound()) );

    if ( pDesc != NULL )
        m_hDeviceSamplerState.Create( (const DeviceSamplerStateDesc *)pDesc );
    else {
        DeviceSamplerStateDesc hDesc;
        hDesc.iFilterMode = DEVICESAMPLER_FILTER_MIN_MAG_MIP_LLL;
        hDesc.iWrapModeU = DEVICESAMPLER_WRAP_CLAMP;
        hDesc.iWrapModeV = DEVICESAMPLER_WRAP_CLAMP;
        hDesc.iWrapModeW = DEVICESAMPLER_WRAP_CLAMP;

        hDesc.arrBorderColor[0] = 1.0f;
        hDesc.arrBorderColor[1] = 1.0f;
        hDesc.arrBorderColor[2] = 1.0f;
        hDesc.arrBorderColor[3] = 1.0f;

        hDesc.fMinLOD = -FLOAT_INFINITE;
        hDesc.fMaxLOD = FLOAT_INFINITE;
        hDesc.fLODBias = 0.0f;
        hDesc.iMaxAnisotropy = 1;

        hDesc.iCompareFunction = DEVICESAMPLER_COMPARE_NEVER;

        m_hDeviceSamplerState.Create( &hDesc );
    }

    RenderingFn->_RegisterRenderState( this );
}

/////////////////////////////////////////////////////////////////////////////////
// GPURasterizerState implementation
GPURasterizerState::GPURasterizerState():
    GPURenderState(), m_hDeviceRasterizerState( RenderingFn->m_pDeviceRenderer )
{
    m_pDeviceRenderState = &m_hDeviceRasterizerState;
}
GPURasterizerState::~GPURasterizerState()
{
    if ( IsBound() )
        UnBind();
}

Void GPURasterizerState::Bind( const GPURasterizerStateDesc * pDesc )
{
    Assert( !(IsBound()) );

    if ( pDesc != NULL )
        m_hDeviceRasterizerState.Create( (const DeviceRasterizerStateDesc *)pDesc );
    else {
        DeviceRasterizerStateDesc hDesc;
        hDesc.iFillMode = DEVICERASTERIZER_FILL_SOLID;
        hDesc.iCullMode = DEVICERASTERIZER_CULL_BACK;
        hDesc.bFrontCounterClockwise = true;

        hDesc.iDepthBias = 0;
        hDesc.fDepthBiasClamp = 0.0f;
        hDesc.fSlopeScaledDepthBias = 0.0f;

        hDesc.bDepthClipEnabled = true;

        hDesc.bScissorEnabled = false;

        hDesc.bMultisampleEnabled = false;
        hDesc.bAntialiasedLineEnabled = false;

        m_hDeviceRasterizerState.Create( &hDesc );
    }

    RenderingFn->_RegisterRenderState( this );
}

/////////////////////////////////////////////////////////////////////////////////
// GPUDepthStencilState implementation
GPUDepthStencilState::GPUDepthStencilState():
    GPURenderState(), m_hDeviceDepthStencilState( RenderingFn->m_pDeviceRenderer )
{
    m_pDeviceRenderState = &m_hDeviceDepthStencilState;
}
GPUDepthStencilState::~GPUDepthStencilState()
{
    if ( IsBound() )
        UnBind();
}

Void GPUDepthStencilState::Bind( const GPUDepthStencilStateDesc * pDesc )
{
    Assert( !(IsBound()) );

    if ( pDesc != NULL )
        m_hDeviceDepthStencilState.Create( (const DeviceDepthStencilStateDesc *)pDesc );
    else {
        DeviceDepthStencilStateDesc hDesc;
        hDesc.bDepthEnabled = true;
        hDesc.iDepthWriteMask = DEVICEDEPTH_WRITEMASK_ALL;
        hDesc.iDepthFunction = DEVICEDEPTHSTENCIL_COMPARE_LESSER;

        hDesc.bStencilEnabled = false;
        hDesc.iStencilReadMask = 0xff;
        hDesc.iStencilWriteMask = 0xff;

        hDesc.hFrontFace.iOnStencilFail = DEVICESTENCIL_OP_KEEP;
        hDesc.hFrontFace.iOnStencilDepthFail = DEVICESTENCIL_OP_KEEP;
        hDesc.hFrontFace.iOnStencilPass = DEVICESTENCIL_OP_KEEP;
        hDesc.hFrontFace.iStencilFunction = DEVICEDEPTHSTENCIL_COMPARE_ALLWAYS;

        hDesc.hBackFace.iOnStencilFail = DEVICESTENCIL_OP_KEEP;
        hDesc.hBackFace.iOnStencilDepthFail = DEVICESTENCIL_OP_KEEP;
        hDesc.hBackFace.iOnStencilPass = DEVICESTENCIL_OP_KEEP;
        hDesc.hBackFace.iStencilFunction = DEVICEDEPTHSTENCIL_COMPARE_ALLWAYS;

        m_hDeviceDepthStencilState.Create( &hDesc );
    }

    RenderingFn->_RegisterRenderState( this );
}

/////////////////////////////////////////////////////////////////////////////////
// GPUBlendState implementation
GPUBlendState::GPUBlendState():
    GPURenderState(), m_hDeviceBlendState( RenderingFn->m_pDeviceRenderer )
{
    m_pDeviceRenderState = &m_hDeviceBlendState;
}
GPUBlendState::~GPUBlendState()
{
    if ( IsBound() )
        UnBind();
}

Void GPUBlendState::Bind( const GPUBlendStateDesc * pDesc )
{
    Assert( !(IsBound()) );

    if ( pDesc != NULL )
        m_hDeviceBlendState.Create( (const DeviceBlendStateDesc *)pDesc );
    else {
        DeviceBlendStateDesc hDesc;
        hDesc.bAlphaToCoverageEnabled = false;
        hDesc.bIndependentBlendEnabled = false;

        for( UInt i = 0; i < GPUSHADER_MAX_RENDERTARGET_SLOTS; ++i ) {
            hDesc.arrRenderTargets[i].bBlendEnabled = false;

            hDesc.arrRenderTargets[i].iBlendSrc = DEVICEBLEND_PARAM_ONE;
            hDesc.arrRenderTargets[i].iBlendSrcAlpha = DEVICEBLEND_PARAM_ONE;
            hDesc.arrRenderTargets[i].iBlendDst = DEVICEBLEND_PARAM_ZERO;
            hDesc.arrRenderTargets[i].iBlendDstAlpha = DEVICEBLEND_PARAM_ZERO;
            hDesc.arrRenderTargets[i].iBlendOp = DEVICEBLEND_OP_ADD;
            hDesc.arrRenderTargets[i].iBlendOpAlpha = DEVICEBLEND_OP_ADD;

            hDesc.arrRenderTargets[i].iColorWriteMask = DEVICEBLEND_COLORWRITEMASK_ALL;
        }

        m_hDeviceBlendState.Create( &hDesc );
    }

    RenderingFn->_RegisterRenderState( this );
}
