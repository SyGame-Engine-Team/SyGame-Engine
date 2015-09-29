/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Effects/EffectInstance.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Instanciation for rendering with the effect system.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "EffectInstance.h"

#include "../RenderingManager.h"

/////////////////////////////////////////////////////////////////////////////////
// EffectShaderParameters implementation
EffectShaderParameters::EffectShaderParameters( GPUShader * pShader )
{
    m_pShader = pShader;

    UInt i;

    m_iConstantBufferCount = 0;
    for( i = 0; i < DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS; ++i )
        m_arrConstantBuffers[i] = NULL;

    m_iSamplerCount = 0;
    for( i = 0; i < DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS; ++i )
        m_arrSamplers[i] = NULL;

    m_iResourceCount = 0;
    for( i = 0; i < DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS; ++i )
        m_arrResources[i] = NULL;
}
EffectShaderParameters::~EffectShaderParameters()
{
    // nothing to do
}

Void EffectShaderParameters::AddConstantBuffer( GPUConstantBuffer * pConstantBuffer )
{
    Assert( m_iConstantBufferCount < DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS );
    Assert( pConstantBuffer != NULL );

    m_arrConstantBuffers[m_iConstantBufferCount] = pConstantBuffer;
    ++m_iConstantBufferCount;
}
GPUConstantBuffer * EffectShaderParameters::RemoveConstantBuffer( UInt iConstantBuffer )
{
    Assert( iConstantBuffer < m_iConstantBufferCount );

    GPUConstantBuffer * pOldConstantBuffer = m_arrConstantBuffers[iConstantBuffer];

    --m_iConstantBufferCount;
    for( UInt i = iConstantBuffer; i < m_iConstantBufferCount; ++i )
        m_arrConstantBuffers[i] = m_arrConstantBuffers[i+1];
    m_arrConstantBuffers[m_iConstantBufferCount] = NULL;

    return pOldConstantBuffer;
}
GPUConstantBuffer * EffectShaderParameters::SetConstantBuffer( UInt iConstantBuffer, GPUConstantBuffer * pConstantBuffer )
{
    Assert( iConstantBuffer < m_iConstantBufferCount );
    Assert( pConstantBuffer != NULL );

    GPUConstantBuffer * pOldConstantBuffer = m_arrConstantBuffers[iConstantBuffer];

    m_arrConstantBuffers[iConstantBuffer] = pConstantBuffer;

    return pOldConstantBuffer;
}

Void EffectShaderParameters::AddSampler( GPUSamplerState * pSampler )
{
    Assert( m_iSamplerCount < DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS );
    Assert( pSampler != NULL );

    m_arrSamplers[m_iSamplerCount] = pSampler;
    ++m_iSamplerCount;
}
GPUSamplerState * EffectShaderParameters::RemoveSampler( UInt iSampler )
{
    Assert( iSampler < m_iSamplerCount );

    GPUSamplerState * pOldSampler = m_arrSamplers[iSampler];

    --m_iSamplerCount;
    for( UInt i = iSampler; i < m_iSamplerCount; ++i )
        m_arrSamplers[i] = m_arrSamplers[i+1];
    m_arrSamplers[m_iSamplerCount] = NULL;

    return pOldSampler;
}
GPUSamplerState * EffectShaderParameters::SetSampler( UInt iSampler, GPUSamplerState * pSampler )
{
    Assert( iSampler < m_iSamplerCount );
    Assert( pSampler != NULL );

    GPUSamplerState * pOldSampler = m_arrSamplers[iSampler];

    m_arrSamplers[iSampler] = pSampler;

    return pOldSampler;
}

Void EffectShaderParameters::AddResource( GPUShaderView * pResource )
{
    Assert( m_iResourceCount < DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS );
    Assert( pResource != NULL );

    m_arrResources[m_iResourceCount] = pResource;
    ++m_iResourceCount;
}
GPUShaderView * EffectShaderParameters::RemoveResource( UInt iResource )
{
    Assert( iResource < m_iResourceCount );

    GPUShaderView * pOldResource = m_arrResources[iResource];

    --m_iResourceCount;
    for( UInt i = iResource; i < m_iResourceCount; ++i )
        m_arrResources[i] = m_arrResources[i+1];
    m_arrResources[m_iResourceCount] = NULL;

    return pOldResource;
}
GPUShaderView * EffectShaderParameters::SetResource( UInt iResource, GPUShaderView * pResource )
{
    Assert( iResource < m_iResourceCount );
    Assert( pResource != NULL );

    GPUShaderView * pOldResource = m_arrResources[iResource];

    m_arrResources[iResource] = pResource;

    return pOldResource;
}

/////////////////////////////////////////////////////////////////////////////////
// EffectParameters implementation
EffectParameters::EffectParameters()
{
    m_pVSParameters = NULL;
    m_pGSParameters = NULL;
    m_pPSParameters = NULL;
    m_pHSParameters = NULL;
    m_pDSParameters = NULL;

    m_iStencilRef = 0;
    m_arrBlendFactors[0] = 1.0f;
    m_arrBlendFactors[1] = 1.0f;
    m_arrBlendFactors[2] = 1.0f;
    m_arrBlendFactors[3] = 1.0f;
    m_iBlendSampleMask = 0xffffffff;

    m_iOutputBufferCount = 0;
    for( UInt i = 0; i < DEVICERENDERER_MAX_STREAMOUTPUT_SLOTS; ++i ) {
        m_arrOutputBuffers[i] = NULL;
        m_arrOutputBufferOffsets[i] = 0;
    }
}
EffectParameters::~EffectParameters()
{
    // nothing to do
}

EffectShaderParameters * EffectParameters::SetShaderParameters( GPUShaderType iType, EffectShaderParameters * pShaderParameters )
{
    Assert( pShaderParameters != NULL );

    EffectShaderParameters * pOldShaderParameters;

    switch( iType ) {
        case GPUSHADER_VERTEX:   pOldShaderParameters = m_pVSParameters; m_pVSParameters = pShaderParameters; break;
        case GPUSHADER_GEOMETRY: pOldShaderParameters = m_pGSParameters; m_pGSParameters = pShaderParameters; break;
        case GPUSHADER_PIXEL:    pOldShaderParameters = m_pPSParameters; m_pPSParameters = pShaderParameters; break;
        case GPUSHADER_HULL:     pOldShaderParameters = m_pHSParameters; m_pHSParameters = pShaderParameters; break;
        case GPUSHADER_DOMAIN:   pOldShaderParameters = m_pDSParameters; m_pDSParameters = pShaderParameters; break;
        default: Assert( false ); break;
    }

    return pOldShaderParameters;
}

Void EffectParameters::AddOutputBuffer( GPUBuffer * pOutputBuffer, UInt iOffset )
{
    Assert( m_iOutputBufferCount < DEVICERENDERER_MAX_STREAMOUTPUT_SLOTS );
    Assert( pOutputBuffer != NULL );

    m_arrOutputBuffers[m_iOutputBufferCount] = pOutputBuffer;
    m_arrOutputBufferOffsets[m_iOutputBufferCount] = iOffset;
    ++m_iOutputBufferCount;
}
GPUBuffer * EffectParameters::RemoveOutputBuffer( UInt iOutputBuffer, UInt * outOffset )
{
    Assert( iOutputBuffer < m_iOutputBufferCount );

    GPUBuffer * pOldOutputBuffer = m_arrOutputBuffers[iOutputBuffer];
    if ( outOffset != NULL )
        *outOffset = m_arrOutputBufferOffsets[iOutputBuffer];

    --m_iOutputBufferCount;
    for( UInt i = iOutputBuffer; i < m_iOutputBufferCount; ++i ) {
        m_arrOutputBuffers[i] = m_arrOutputBuffers[i+1];
        m_arrOutputBufferOffsets[i] = m_arrOutputBufferOffsets[i+1];
    }
    m_arrOutputBuffers[m_iOutputBufferCount] = NULL;
    m_arrOutputBufferOffsets[m_iOutputBufferCount] = 0;

    return pOldOutputBuffer;
}
GPUBuffer * EffectParameters::SetOutputBuffer( UInt iOutputBuffer, GPUBuffer * pOutputBuffer, UInt iOffset, UInt * outOldOffset )
{
    Assert( iOutputBuffer < m_iOutputBufferCount );
    Assert( pOutputBuffer != NULL );

    GPUBuffer * pOldOutputBuffer = m_arrOutputBuffers[iOutputBuffer];
    if ( outOldOffset != NULL )
        *outOldOffset = m_arrOutputBufferOffsets[iOutputBuffer];

    m_arrOutputBuffers[iOutputBuffer] = pOutputBuffer;
    m_arrOutputBufferOffsets[iOutputBuffer] = iOffset;

    return pOldOutputBuffer;
}

/////////////////////////////////////////////////////////////////////////////////
// EffectInstance implementation
EffectInstance::EffectInstance( const Effect * pEffect, UInt iTechniqueIndex )
{
    Assert( pEffect != NULL );

    m_pEffect = pEffect;

    m_iTechniqueIndex = iTechniqueIndex;
    m_pTechnique = m_pEffect->GetTechnique( m_iTechniqueIndex );

    m_iPassCount = m_pTechnique->GetPassCount();
    for( UInt i = 0; i < m_iPassCount; ++i )
        m_arrParameters[i] = NULL;
}
EffectInstance::~EffectInstance()
{
    // nothing to do
}

EffectParameters * EffectInstance::SetParameters( UInt iPass, EffectParameters * pParameters )
{
    Assert( iPass < m_iPassCount );
    Assert( pParameters != NULL );

    EffectParameters * pOldParameters = m_arrParameters[iPass];

    m_arrParameters[iPass] = pParameters;

    return pOldParameters;
}

Void EffectInstance::Enable( UInt iPass, GPUDeferredContext * pContext ) const
{
    Assert( iPass < m_iPassCount );

    EffectPass * pPass = m_pTechnique->GetPass( iPass );
    EffectParameters * pParameters = m_arrParameters[iPass];
    EffectShaderParameters * pShaderParameters;

    GPUVertexShader * pVS = pPass->GetVertexShader();
    GPUGeometryShader * pGS = pPass->GetGeometryShader();
    GPUPixelShader * pPS = pPass->GetPixelShader();
    GPUHullShader * pHS = pPass->GetHullShader();
    GPUDomainShader * pDS = pPass->GetDomainShader();

    // Enable render states
    RenderingFn->RSEnableState( pPass->GetRasterizerState(), pContext );
    RenderingFn->OMEnableDepthStencilState( pPass->GetDepthStencilState(), pParameters->GetStencilRef(), pContext );
    RenderingFn->OMEnableBlendState( pPass->GetBlendState(), pParameters->GetBlendFactors(), pParameters->GetBlendSampleMask(), pContext );

    // Enable shaders and parameters
    if ( pVS != NULL ) {
        RenderingFn->VSEnable( pVS, pContext );

        pShaderParameters = pParameters->GetShaderParameters( GPUSHADER_VERTEX );
        if ( pShaderParameters != NULL ) {
            if ( pShaderParameters->GetConstantBufferCount() > 0 )
                RenderingFn->VSEnableCBs( 0, pShaderParameters->GetConstantBufferCount(), pShaderParameters->GetConstantBuffers(0), pContext );
            if ( pShaderParameters->GetSamplerCount() > 0 )
                RenderingFn->VSEnableSamplers( 0, pShaderParameters->GetSamplerCount(), pShaderParameters->GetSamplers(0), pContext );
            if ( pShaderParameters->GetResourceCount() > 0 )
                RenderingFn->VSEnableResources( 0, pShaderParameters->GetResourceCount(), pShaderParameters->GetResources(0), pContext );
        }
    }
    if ( pGS != NULL ) {
        RenderingFn->GSEnable( pGS, pContext );

        pShaderParameters = pParameters->GetShaderParameters( GPUSHADER_GEOMETRY );
        if ( pShaderParameters != NULL ) {
            if ( pShaderParameters->GetConstantBufferCount() > 0 )
                RenderingFn->GSEnableCBs( 0, pShaderParameters->GetConstantBufferCount(), pShaderParameters->GetConstantBuffers(0), pContext );
            if ( pShaderParameters->GetSamplerCount() > 0 )
                RenderingFn->GSEnableSamplers( 0, pShaderParameters->GetSamplerCount(), pShaderParameters->GetSamplers(0), pContext );
            if ( pShaderParameters->GetResourceCount() > 0 )
                RenderingFn->GSEnableResources( 0, pShaderParameters->GetResourceCount(), pShaderParameters->GetResources(0), pContext );
        }
    }
    if ( pPS != NULL ) {
        RenderingFn->PSEnable( pPS, pContext );

        pShaderParameters = pParameters->GetShaderParameters( GPUSHADER_PIXEL );
        if ( pShaderParameters != NULL ) {
            if ( pShaderParameters->GetConstantBufferCount() > 0 )
                RenderingFn->PSEnableCBs( 0, pShaderParameters->GetConstantBufferCount(), pShaderParameters->GetConstantBuffers(0), pContext );
            if ( pShaderParameters->GetSamplerCount() > 0 )
                RenderingFn->PSEnableSamplers( 0, pShaderParameters->GetSamplerCount(), pShaderParameters->GetSamplers(0), pContext );
            if ( pShaderParameters->GetResourceCount() > 0 )
                RenderingFn->PSEnableResources( 0, pShaderParameters->GetResourceCount(), pShaderParameters->GetResources(0), pContext );
        }
    }
    if ( pHS != NULL ) {
        RenderingFn->HSEnable( pHS, pContext );

        pShaderParameters = pParameters->GetShaderParameters( GPUSHADER_HULL );
        if ( pShaderParameters != NULL ) {
            if ( pShaderParameters->GetConstantBufferCount() > 0 )
                RenderingFn->HSEnableCBs( 0, pShaderParameters->GetConstantBufferCount(), pShaderParameters->GetConstantBuffers(0), pContext );
            if ( pShaderParameters->GetSamplerCount() > 0 )
                RenderingFn->HSEnableSamplers( 0, pShaderParameters->GetSamplerCount(), pShaderParameters->GetSamplers(0), pContext );
            if ( pShaderParameters->GetResourceCount() > 0 )
                RenderingFn->HSEnableResources( 0, pShaderParameters->GetResourceCount(), pShaderParameters->GetResources(0), pContext );
        }
    }
    if ( pDS != NULL ) {
        RenderingFn->DSEnable( pDS, pContext );

        pShaderParameters = pParameters->GetShaderParameters( GPUSHADER_DOMAIN );
        if ( pShaderParameters != NULL ) {
            if ( pShaderParameters->GetConstantBufferCount() > 0 )
                RenderingFn->DSEnableCBs( 0, pShaderParameters->GetConstantBufferCount(), pShaderParameters->GetConstantBuffers(0), pContext );
            if ( pShaderParameters->GetSamplerCount() > 0 )
                RenderingFn->DSEnableSamplers( 0, pShaderParameters->GetSamplerCount(), pShaderParameters->GetSamplers(0), pContext );
            if ( pShaderParameters->GetResourceCount() > 0 )
                RenderingFn->DSEnableResources( 0, pShaderParameters->GetResourceCount(), pShaderParameters->GetResources(0), pContext );
        }
    }

    // Enable input layout
    RenderingFn->IAEnableIL( pPass->GetInputLayout(), pContext );

    // Enable output stream buffers
    if ( pParameters->GetOutputBufferCount() > 0 )
        RenderingFn->SOEnable( pParameters->GetOutputBufferCount(), pParameters->GetOutputBuffers(0), pParameters->GetOutputBufferOffsets(0), pContext );
}
Void EffectInstance::Disable( UInt iPass, GPUDeferredContext * pContext ) const
{
    Assert( iPass < m_iPassCount );

    EffectPass * pPass = m_pTechnique->GetPass( iPass );
    EffectParameters * pParameters = m_arrParameters[iPass];
    EffectShaderParameters * pShaderParameters;

    GPUVertexShader * pVS = pPass->GetVertexShader();
    GPUGeometryShader * pGS = pPass->GetGeometryShader();
    GPUPixelShader * pPS = pPass->GetPixelShader();
    GPUHullShader * pHS = pPass->GetHullShader();
    GPUDomainShader * pDS = pPass->GetDomainShader();

    // Disable output stream buffers
    RenderingFn->SODisable( pContext );

    // Disable input layout
    RenderingFn->IADisableIL( pContext );

    // Disable shaders and parameters
    if ( pVS != NULL ) {
        pShaderParameters = pParameters->GetShaderParameters( GPUSHADER_VERTEX );
        if ( pShaderParameters != NULL ) {
            RenderingFn->VSDisableAllCBs();
            RenderingFn->VSDisableAllSamplers();
            RenderingFn->VSDisableAllResources();
        }

        RenderingFn->VSDisable( pContext );
    }
    if ( pGS != NULL ) {
        pShaderParameters = pParameters->GetShaderParameters( GPUSHADER_GEOMETRY );
        if ( pShaderParameters != NULL ) {
            RenderingFn->GSDisableAllCBs();
            RenderingFn->GSDisableAllSamplers();
            RenderingFn->GSDisableAllResources();
        }

        RenderingFn->GSDisable( pContext );
    }
    if ( pPS != NULL ) {
        pShaderParameters = pParameters->GetShaderParameters( GPUSHADER_PIXEL );
        if ( pShaderParameters != NULL ) {
            RenderingFn->PSDisableAllCBs();
            RenderingFn->PSDisableAllSamplers();
            RenderingFn->PSDisableAllResources();
        }

        RenderingFn->PSDisable( pContext );
    }
    if ( pHS != NULL ) {
        pShaderParameters = pParameters->GetShaderParameters( GPUSHADER_HULL );
        if ( pShaderParameters != NULL ) {
            RenderingFn->HSDisableAllCBs();
            RenderingFn->HSDisableAllSamplers();
            RenderingFn->HSDisableAllResources();
        }

        RenderingFn->HSDisable( pContext );
    }
    if ( pDS != NULL ) {
        pShaderParameters = pParameters->GetShaderParameters( GPUSHADER_DOMAIN );
        if ( pShaderParameters != NULL ) {
            RenderingFn->DSDisableAllCBs();
            RenderingFn->DSDisableAllSamplers();
            RenderingFn->DSDisableAllResources();
        }

        RenderingFn->DSDisable( pContext );
    }

    // Disable render states
    RenderingFn->RSDisableState( pContext );
    RenderingFn->OMDisableDepthStencilState( pContext );
    RenderingFn->OMDisableBlendState( pContext );
}


