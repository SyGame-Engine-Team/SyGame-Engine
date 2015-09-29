/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Effects/EffectLibrary/Basic/EffectTexture.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Basic effect : Texture
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
#include "EffectTexture.h"

#include "../../../RenderingManager.h"

/////////////////////////////////////////////////////////////////////////////////
// EffectTexture implementation
const AChar * EffectTexture::sm_strHLSLCode = (
    // Constants
    "matrix matWVP;"

    // Samplers
    "SamplerState hSampler;"

    // IO layouts
    "struct _VS_Input {"
    "    float4 vPosition : POSITION0;"
    "    float2 vTexCoord : TEXCOORD0;"
    "};"
    "struct _VS_Output {"
    "    float4 vClipPosition : SV_POSITION0;"
    "    float2 vTexCoord : TEXCOORD0;"
    "};"

    // Vertex shader implementation
    "_VS_Output _VS_Main( _VS_Input hInput ) {"
    "    _VS_Output hOutput;"
    "    hOutput.vClipPosition = mul( hInput.vPosition, matWVP );"
    "    hOutput.vTexCoord = hInput.vTexCoord;"
    "    return hOutput;"
    "}"

    // Pixel shader implementation
    "float4 _PS_Main( _VS_Output hInput ) : SV_TARGET {"
    "    return tex2D( hSampler, hInput.vTexCoord );"
    "}"
);
const UInt EffectTexture::sm_iHLSLCodeLength = StringFn->LengthA( sm_strHLSLCode );

EffectTexture::EffectTexture():
    Effect(),
    m_hRasterizerState(), m_hDepthStencilState(), m_hBlendState(), m_hIL(),
    m_hPass(), m_hTechnique()
{
    // Create render states
    m_hRasterizerState.Bind();
    m_hDepthStencilState.Bind();
    m_hBlendState.Bind();

    // Create shaders
    GPUShaderCompilationData hCompilationData;
    hCompilationData.arrMacros = NULL;
    hCompilationData.iFlags = ( GPUSHADER_COMPILE_STRICT | GPUSHADER_COMPILE_PACK_MATRIX_COLUMN_MAJOR | GPUSHADER_COMPILE_OPTIMIZATION_LEVEL1 );
    hCompilationData.iEffectFlags = 0;

    hCompilationData.iTarget = GPUSHADER_VERTEX;
    hCompilationData.strName = "EffectLib.Texture.VS";
    hCompilationData.strEntryPoint = "_VS_Main";

    m_hCompiledVS.Compile( sm_strHLSLCode, sm_iHLSLCodeLength, &hCompilationData );
    m_hVS.Bind( &m_hCompiledVS, false, false );

    hCompilationData.iTarget = GPUSHADER_PIXEL;
    hCompilationData.strName = "EffectLib.Texture.PS";
    hCompilationData.strEntryPoint = "_PS_Main";

    m_hCompiledPS.Compile( sm_strHLSLCode, sm_iHLSLCodeLength, &hCompilationData );
    m_hPS.Bind( &m_hCompiledPS, false, false );

    // Create input layout
    GPUInputFieldDesc hField;
    hField.iSlot = 0;
    hField.iSlotClass = GPUINPUTFIELD_SLOTCLASS_PER_VERTEX;
    hField.iInstanceDataStepRate = 0;

    hField.iType = GPUINPUTFIELD_TYPE_FLOAT4;
    hField.strSemantic = GPUINPUTFIELD_SEMANTIC_POSITION;
    hField.iSemanticIndex = 0;
    m_hIL.PushField( &hField );

    hField.iType = GPUINPUTFIELD_TYPE_FLOAT2;
    hField.strSemantic = GPUINPUTFIELD_SEMANTIC_TEXCOORD;
    hField.iSemanticIndex = 0;
    m_hIL.PushField( &hField );

    m_hIL.Finalize();
    m_hIL.Bind( &m_hCompiledVS );

    // Build effect
    m_hPass.SetRasterizerState( &m_hRasterizerState );
    m_hPass.SetDepthStencilState( &m_hDepthStencilState );
    m_hPass.SetBlendState( &m_hBlendState );

    m_hPass.SetInputLayout( &m_hIL );

    m_hPass.SetVertexShader( &m_hVS );
    m_hPass.SetPixelShader( &m_hPS );

    m_hTechnique.AddPass( &m_hPass );
    AddTechnique( &m_hTechnique );
}
EffectTexture::~EffectTexture()
{
    // Destroy input layout
    m_hIL.UnBind();

    // Destroy shaders
    m_hVS.UnBind();
    m_hPS.UnBind();
    m_hCompiledVS.Clear();
    m_hCompiledPS.Clear();

    // Destroy render states
    m_hRasterizerState.UnBind();
    m_hDepthStencilState.UnBind();
    m_hBlendState.UnBind();
}

EffectTextureInstance * EffectTexture::CreateInstance( GPUTexture2D * pTexture ) const
{
    Assert( pTexture->IsBound() );
    Assert( pTexture->GetBinds() & GPURESOURCE_BIND_SHADER_INPUT );

    RenderingFn->SelectMemory( TEXT("Effects") );
    EffectTextureInstance * pInstance = New EffectTextureInstance( this, pTexture );
	RenderingFn->UnSelectMemory();

    return pInstance;
}

/////////////////////////////////////////////////////////////////////////////////
// EffectTextureInstance implementation
EffectTextureInstance::EffectTextureInstance( const EffectTexture * pEffectTexture, GPUTexture2D * pTexture ):
    EffectInstance( pEffectTexture, 0 ), m_hParameters(),
    m_hVSParameters( pEffectTexture->GetVertexShader(0,0) ),
    m_hPSParameters( pEffectTexture->GetPixelShader(0,0) ),
    m_hPSSampler()
{
    m_pTexture = pTexture;

    // Create constant buffers
    m_pVSConstants = RenderingFn->CreateConstantBuffer( 4, NULL );
    m_pVSConstants->SetUsage( GPURESOURCE_USAGE_DEFAULT );
    m_pVSConstants->Bind();

    // Create samplers & textures
    m_hPSTextureView.Bind( m_pTexture, 0, 1, 0, 1 );

    GPUSamplerStateDesc hDesc;
    hDesc.iFilterMode = GPUSAMPLER_FILTER_MINMAGMIP_LLL;
    hDesc.iWrapModeU = GPUSAMPLER_WRAP_CLAMP;
    hDesc.iWrapModeV = GPUSAMPLER_WRAP_CLAMP;
    hDesc.iWrapModeW = GPUSAMPLER_WRAP_CLAMP;
    hDesc.arrBorderColor[0] = 1.0f;
    hDesc.arrBorderColor[1] = 1.0f;
    hDesc.arrBorderColor[2] = 1.0f;
    hDesc.arrBorderColor[3] = 1.0f;
    hDesc.fMinLOD = -FLOAT_INFINITE;
    hDesc.fMaxLOD = FLOAT_INFINITE;
    hDesc.fLODBias = 0.0f;
    hDesc.iMaxAnisotropy = 1;
    hDesc.iCompareFunction = GPUSAMPLER_COMPARE_NEVER;

    m_hPSSampler.Bind( &hDesc );

    // Build effect instance
    m_hVSParameters.AddConstantBuffer( m_pVSConstants );
    m_hPSParameters.AddSampler( &m_hPSSampler );
    m_hPSParameters.AddResource( &m_hPSTextureView );

    m_hParameters.SetShaderParameters( GPUSHADER_VERTEX, &m_hVSParameters );
    m_hParameters.SetShaderParameters( GPUSHADER_PIXEL, &m_hPSParameters );
	SetParameters( 0, &m_hParameters );
}
EffectTextureInstance::~EffectTextureInstance()
{
    // Destroy samplers & textures
    m_hPSSampler.UnBind();
    m_hPSTextureView.UnBind();

    // Destroy constant buffers
    m_pVSConstants->UnBind();
    RenderingFn->DestroyResource( m_pVSConstants );
}

Void EffectTextureInstance::UpdateConstants( UInt iPass, const Transform3 * pWorldTransform, const Camera * pCamera, GPUDeferredContext * pContext ) const
{
    Assert( iPass < m_iPassCount );

    // Compute PVW matrix
    Matrix4 * pPVW = (Matrix4*)( m_pVSConstants->GetElement<GPUConstant>(0) );

    *pPVW = ( pCamera->GetProjectionMatrix() * pCamera->GetViewMatrix() );
    if ( pCamera->HasPreView() )
        *pPVW *= pCamera->GetPreViewMatrix();
    if ( pCamera->HasPostProjection() )
        *pPVW = ( pCamera->GetPostProjectionMatrix() * (*pPVW) );
    *pPVW *= pWorldTransform->GetHMatrix();

    // Update
    m_pVSConstants->Update( 0, INVALID_OFFSET, pContext );
}

