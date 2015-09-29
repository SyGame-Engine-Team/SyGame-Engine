/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Effects/EffectLibrary/Lighting/EffectLightMaterialTexture.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Lighting effect : Lighting + Material + Texture
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
#include "EffectLightMaterialTexture.h"

#include "../../../RenderingManager.h"

/////////////////////////////////////////////////////////////////////////////////
// EffectLightMaterialTexture implementation
const AChar * EffectLightMaterialTexture::sm_strHLSLCode[EFFECTLIGHT_TECHNIQUE_COUNT] = {

( // GPULIGHT_TECHNIQUE_POINT_AMBIANT
    // Constants
    "matrix matW;"
    "matrix matVP;"
    "float4 vLightWorldPosition;"
    "float4 vLightAttenuation;"
    "float4 vLightAmbiant;"
    "float4 vMaterialEmissive;"
    "float4 vMaterialAmbiant;"

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
    "    float4 vColor : COLOR0;"
    "};"

    // Vertex shader implementation
    "_VS_Output _VS_Main( _VS_Input hInput ) {"
    "    float4 vWorldPosition = mul( hInput.vPosition, matW );"
    "    float fDistance = length( vWorldPosition - vLightWorldPosition );"
    "    float fIntensity = vLightAttenuation.a / ( vLightAttenuation.r + fDistance * (vLightAttenuation.g + (fDistance * vLightAttenuation.b)) );"
    "    float4 vAmbiant = vLightAmbiant * vMaterialAmbiant * fIntensity;"

    "    _VS_Output hOutput;"
    "    hOutput.vClipPosition = mul( vWorldPosition, matVP );"
    "    hOutput.vTexCoord = hInput.vTexCoord;"
    "    hOutput.vColor = saturate( vAmbiant + vMaterialEmissive );"
    "    return hOutput;"
    "}"

    // Pixel shader implementation
    "float4 _PS_Main( _VS_Output hInput ) : SV_TARGET {"
    "    return saturate( hInput.vColor + tex2D(hSampler, hInput.vTexCoord) );"
    "}"
),

( // GPULIGHT_TECHNIQUE_POINT_GOURAUD
    // Constants
    "matrix matW;"
    "matrix matVP;"
    "float4 vCameraWorldPosition;"
    "float4 vLightWorldPosition;"
    "float4 vLightAttenuation;"
    "float4 vLightAmbiant;"
    "float4 vLightDiffuse;"
    "float4 vLightSpecular;"
    "float4 vMaterialEmissive;"
    "float4 vMaterialAmbiant;"
    "float4 vMaterialDiffuse;"
    "float4 vMaterialSpecular;"

    // Samplers
    "SamplerState hSampler;"

    // IO layouts
    "struct _VS_Input {"
    "    float4 vPosition : POSITION0;"
    "    float4 vNormal : NORMAL0;"
    "    float2 vTexCoord : TEXCOORD0;"
    "};"
    "struct _VS_Output {"
    "    float4 vClipPosition : SV_POSITION0;"
    "    float2 vTexCoord : TEXCOORD0;"
    "    float4 vColor : COLOR0;"
    "};"

    // Vertex shader implementation
    "_VS_Output _VS_Main( _VS_Input hInput ) {"
    "    float4 vWorldPosition = mul( hInput.vPosition, matW );"
    "    float4 vWorldNormal = normalize( mul( hInput.vNormal, matW ) );"

    "    float4 vLightN = ( vLightWorldPosition - vWorldPosition );"
    "    float fDistLight = length( vLightN );"
    "    vLightN = ( vLightN / fDistLight );"
    "    float4 vViewPtLightN = normalize( vCameraWorldPosition - vWorldPosition );"
    "    vViewPtLightN = normalize( vViewPtLightN + vLightN );"

    "    float fIntensity = vLightAttenuation.a / ( vLightAttenuation.r + fDistLight * (vLightAttenuation.g + (fDistLight * vLightAttenuation.b)) );"
    "    float4 vSpecular = vLightSpecular * vMaterialSpecular * pow( dot(vViewPtLightN, vWorldNormal), vMaterialSpecular.a );"
    "    float4 vDiffuse = vLightDiffuse * vMaterialDiffuse * dot( vLightN, vWorldNormal );"
    "    float4 vAmbiant = vLightAmbiant * vMaterialAmbiant;"

    "    _VS_Output hOutput;"
    "    hOutput.vClipPosition = mul( vWorldPosition, matVP );"
    "    hOutput.vTexCoord = hInput.vTexCoord;"
    "    hOutput.vColor = saturate( ((vAmbiant + vDiffuse + vSpecular) * fIntensity) + vMaterialEmissive );"
    "    return hOutput;"
    "}"

    // Pixel shader implementation
    "float4 _PS_Main( _VS_Output hInput ) : SV_TARGET {"
    "    return saturate( hInput.vColor + tex2D(hSampler, hInput.vTexCoord) );"
    "}"
),


( // GPULIGHT_TECHNIQUE_POINT_PHONG
    // Constants
    "cbuffer vsbuffer {"
    "    matrix matW;"
    "    matrix matVP;"
    "}"
    "cbuffer psbuffer {"
    "    float4 vCameraWorldPosition;"
    "    float4 vLightWorldPosition;"
    "    float4 vLightAttenuation;"
    "    float4 vLightAmbiant;"
    "    float4 vLightDiffuse;"
    "    float4 vLightSpecular;"
    "    float4 vMaterialEmissive;"
    "    float4 vMaterialAmbiant;"
    "    float4 vMaterialDiffuse;"
    "    float4 vMaterialSpecular;"
    "}"

    // Samplers
    "SamplerState hSampler;"

    // IO layouts
    "struct _VS_Input {"
    "    float4 vPosition : POSITION0;"
    "    float4 vNormal : NORMAL0;"
    "    float2 vTexCoord : TEXCOORD0;"
    "};"
    "struct _VS_Output {"
    "    float4 vClipPosition : SV_POSITION0;"
    "    float2 vTexCoord : TEXCOORD0;"
    "    float4 vWorldPosition : POSITION1;"
    "    float4 vWorldNormal : NORMAL0;"
    "};"

    // Vertex shader implementation
    "_VS_Output _VS_Main( _VS_Input hInput ) {"
    "    _VS_Output hOutput;"
    "    hOutput.vTexCoord = hInput.vTexCoord;"
    "    hOutput.vWorldPosition = mul( hInput.vPosition, matW );"
    "    hOutput.vWorldNormal = normalize( mul( hInput.vNormal, matW ) );"
    "    hOutput.vClipPosition = mul( hOutput.vWorldPosition, matVP );"
    "    return hOutput;"
    "}"

    // Pixel shader implementation
    "float4 _PS_Main( _VS_Output hInput ) : SV_TARGET {"
    "    float4 vLightN = ( vLightWorldPosition - hInput.vWorldPosition );"
    "    float fDistLight = length( vLightN );"
    "    vLightN = ( vLightN / fDistLight );"
    "    float4 vViewPtLightN = normalize( vCameraWorldPosition - hInput.vWorldPosition );"
    "    vViewPtLightN = normalize( vViewPtLightN + vLightN );"

    "    float fIntensity = vLightAttenuation.a / ( vLightAttenuation.r + fDistLight * (vLightAttenuation.g + (fDistLight * vLightAttenuation.b)) );"
    "    float4 vSpecular = vLightSpecular * vMaterialSpecular * pow( dot(vViewPtLightN, hInput.vWorldNormal), vMaterialSpecular.a );"
    "    float4 vDiffuse = vLightDiffuse * vMaterialDiffuse * dot( vLightN, hInput.vWorldNormal );"
    "    float4 vAmbiant = vLightAmbiant * vMaterialAmbiant;"

    "    return saturate( ((vAmbiant + vDiffuse + vSpecular) * fIntensity) + vMaterialEmissive + tex2D(hSampler, hInput.vTexCoord) );"
    "}"
)

};
const UInt EffectLightMaterialTexture::sm_iHLSLCodeLength[EFFECTLIGHT_TECHNIQUE_COUNT] = {
    StringFn->LengthA( sm_strHLSLCode[EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT] ),
    StringFn->LengthA( sm_strHLSLCode[EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD] ),
    StringFn->LengthA( sm_strHLSLCode[EFFECTLIGHT_TECHNIQUE_POINT_PHONG] )
};

EffectLightMaterialTexture::EffectLightMaterialTexture():
    Effect(),
    m_hRasterizerState(), m_hDepthStencilState(), m_hBlendState()
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
    hCompilationData.strName = "EffectLib.LightMaterialTexture_Point_Ambiant.VS";
    hCompilationData.strEntryPoint = "_VS_Main";

    m_hCompiledVS[EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT].Compile( sm_strHLSLCode[EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT], sm_iHLSLCodeLength[EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT], &hCompilationData );
    m_hVS[EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT].Bind( m_hCompiledVS + EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT, false, false );

    hCompilationData.iTarget = GPUSHADER_PIXEL;
    hCompilationData.strName = "EffectLib.LightMaterialTexture_Point_Ambiant.PS";
    hCompilationData.strEntryPoint = "_PS_Main";

    m_hCompiledPS[EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT].Compile( sm_strHLSLCode[EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT], sm_iHLSLCodeLength[EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT], &hCompilationData );
    m_hPS[EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT].Bind( m_hCompiledPS + EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT, false, false );

    hCompilationData.iTarget = GPUSHADER_VERTEX;
    hCompilationData.strName = "EffectLib.LightMaterialTexture_Point_Gouraud.VS";
    hCompilationData.strEntryPoint = "_VS_Main";

    m_hCompiledVS[EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD].Compile( sm_strHLSLCode[EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD], sm_iHLSLCodeLength[EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD], &hCompilationData );
    m_hVS[EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD].Bind( m_hCompiledVS + EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD, false, false );

    hCompilationData.iTarget = GPUSHADER_PIXEL;
    hCompilationData.strName = "EffectLib.LightMaterialTexture_Point_Gouraud.PS";
    hCompilationData.strEntryPoint = "_PS_Main";

    m_hCompiledPS[EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD].Compile( sm_strHLSLCode[EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD], sm_iHLSLCodeLength[EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD], &hCompilationData );
    m_hPS[EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD].Bind( m_hCompiledPS + EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD, false, false );

    hCompilationData.iTarget = GPUSHADER_VERTEX;
    hCompilationData.strName = "EffectLib.LightMaterialTexture_Point_Phong.VS";
    hCompilationData.strEntryPoint = "_VS_Main";

    m_hCompiledVS[EFFECTLIGHT_TECHNIQUE_POINT_PHONG].Compile( sm_strHLSLCode[EFFECTLIGHT_TECHNIQUE_POINT_PHONG], sm_iHLSLCodeLength[EFFECTLIGHT_TECHNIQUE_POINT_PHONG], &hCompilationData );
    m_hVS[EFFECTLIGHT_TECHNIQUE_POINT_PHONG].Bind( m_hCompiledVS + EFFECTLIGHT_TECHNIQUE_POINT_PHONG, false, false );

    hCompilationData.iTarget = GPUSHADER_PIXEL;
    hCompilationData.strName = "EffectLib.LightMaterialTexture_Point_Phong.PS";
    hCompilationData.strEntryPoint = "_PS_Main";

    m_hCompiledPS[EFFECTLIGHT_TECHNIQUE_POINT_PHONG].Compile( sm_strHLSLCode[EFFECTLIGHT_TECHNIQUE_POINT_PHONG], sm_iHLSLCodeLength[EFFECTLIGHT_TECHNIQUE_POINT_PHONG], &hCompilationData );
    m_hPS[EFFECTLIGHT_TECHNIQUE_POINT_PHONG].Bind( m_hCompiledPS + EFFECTLIGHT_TECHNIQUE_POINT_PHONG, false, false );

    // Create input layouts
    GPUInputFieldDesc hField;
    hField.iSlot = 0;
    hField.iSlotClass = GPUINPUTFIELD_SLOTCLASS_PER_VERTEX;
    hField.iInstanceDataStepRate = 0;

    hField.iType = GPUINPUTFIELD_TYPE_FLOAT4;
    hField.strSemantic = GPUINPUTFIELD_SEMANTIC_POSITION;
    hField.iSemanticIndex = 0;
    m_hIL[EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT].PushField( &hField );

    hField.iType = GPUINPUTFIELD_TYPE_FLOAT2;
    hField.strSemantic = GPUINPUTFIELD_SEMANTIC_TEXCOORD;
    hField.iSemanticIndex = 0;
    m_hIL[EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT].PushField( &hField );

    m_hIL[EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT].Finalize();
    m_hIL[EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT].Bind( m_hCompiledVS + EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT );

    hField.iType = GPUINPUTFIELD_TYPE_FLOAT4;
    hField.strSemantic = GPUINPUTFIELD_SEMANTIC_POSITION;
    hField.iSemanticIndex = 0;
    m_hIL[EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD].PushField( &hField );

    hField.iType = GPUINPUTFIELD_TYPE_FLOAT4;
    hField.strSemantic = GPUINPUTFIELD_SEMANTIC_NORMAL;
    hField.iSemanticIndex = 0;
    m_hIL[EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD].PushField( &hField );

    hField.iType = GPUINPUTFIELD_TYPE_FLOAT2;
    hField.strSemantic = GPUINPUTFIELD_SEMANTIC_TEXCOORD;
    hField.iSemanticIndex = 0;
    m_hIL[EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD].PushField( &hField );

    m_hIL[EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD].Finalize();
    m_hIL[EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD].Bind( m_hCompiledVS + EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD );

    hField.iType = GPUINPUTFIELD_TYPE_FLOAT4;
    hField.strSemantic = GPUINPUTFIELD_SEMANTIC_POSITION;
    hField.iSemanticIndex = 0;
    m_hIL[EFFECTLIGHT_TECHNIQUE_POINT_PHONG].PushField( &hField );

    hField.iType = GPUINPUTFIELD_TYPE_FLOAT4;
    hField.strSemantic = GPUINPUTFIELD_SEMANTIC_NORMAL;
    hField.iSemanticIndex = 0;
    m_hIL[EFFECTLIGHT_TECHNIQUE_POINT_PHONG].PushField( &hField );

    hField.iType = GPUINPUTFIELD_TYPE_FLOAT2;
    hField.strSemantic = GPUINPUTFIELD_SEMANTIC_TEXCOORD;
    hField.iSemanticIndex = 0;
    m_hIL[EFFECTLIGHT_TECHNIQUE_POINT_PHONG].PushField( &hField );

    m_hIL[EFFECTLIGHT_TECHNIQUE_POINT_PHONG].Finalize();
    m_hIL[EFFECTLIGHT_TECHNIQUE_POINT_PHONG].Bind( m_hCompiledVS + EFFECTLIGHT_TECHNIQUE_POINT_PHONG );

    // Build effect
    m_hPass[EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT].SetRasterizerState( &m_hRasterizerState );
    m_hPass[EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD].SetRasterizerState( &m_hRasterizerState );
    m_hPass[EFFECTLIGHT_TECHNIQUE_POINT_PHONG].SetRasterizerState( &m_hRasterizerState );

    m_hPass[EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT].SetDepthStencilState( &m_hDepthStencilState );
    m_hPass[EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD].SetDepthStencilState( &m_hDepthStencilState );
    m_hPass[EFFECTLIGHT_TECHNIQUE_POINT_PHONG].SetDepthStencilState( &m_hDepthStencilState );

    m_hPass[EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT].SetBlendState( &m_hBlendState );
    m_hPass[EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD].SetBlendState( &m_hBlendState );
    m_hPass[EFFECTLIGHT_TECHNIQUE_POINT_PHONG].SetBlendState( &m_hBlendState );

    m_hPass[EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT].SetInputLayout( m_hIL + EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT );
    m_hPass[EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD].SetInputLayout( m_hIL + EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD );
    m_hPass[EFFECTLIGHT_TECHNIQUE_POINT_PHONG].SetInputLayout( m_hIL + EFFECTLIGHT_TECHNIQUE_POINT_PHONG );

    m_hPass[EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT].SetVertexShader( m_hVS + EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT );
    m_hPass[EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD].SetVertexShader( m_hVS + EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD );
    m_hPass[EFFECTLIGHT_TECHNIQUE_POINT_PHONG].SetVertexShader( m_hVS + EFFECTLIGHT_TECHNIQUE_POINT_PHONG );

    m_hPass[EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT].SetPixelShader( m_hPS + EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT );
    m_hPass[EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD].SetPixelShader( m_hPS + EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD );
    m_hPass[EFFECTLIGHT_TECHNIQUE_POINT_PHONG].SetPixelShader( m_hPS + EFFECTLIGHT_TECHNIQUE_POINT_PHONG );

    m_hTechnique[EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT].AddPass( m_hPass + EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT );
    m_hTechnique[EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD].AddPass( m_hPass + EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD );
    m_hTechnique[EFFECTLIGHT_TECHNIQUE_POINT_PHONG].AddPass( m_hPass + EFFECTLIGHT_TECHNIQUE_POINT_PHONG );
    AddTechnique( m_hTechnique + EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT );
    AddTechnique( m_hTechnique + EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD );
    AddTechnique( m_hTechnique + EFFECTLIGHT_TECHNIQUE_POINT_PHONG );
}
EffectLightMaterialTexture::~EffectLightMaterialTexture()
{
    // Destroy input layout
    m_hIL[EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT].UnBind();
    m_hIL[EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD].UnBind();
    m_hIL[EFFECTLIGHT_TECHNIQUE_POINT_PHONG].UnBind();

    // Destroy shaders
    m_hVS[EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT].UnBind();
    m_hVS[EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD].UnBind();
    m_hVS[EFFECTLIGHT_TECHNIQUE_POINT_PHONG].UnBind();
    m_hPS[EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT].UnBind();
    m_hPS[EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD].UnBind();
    m_hPS[EFFECTLIGHT_TECHNIQUE_POINT_PHONG].UnBind();
    m_hCompiledVS[EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT].Clear();
    m_hCompiledVS[EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD].Clear();
    m_hCompiledVS[EFFECTLIGHT_TECHNIQUE_POINT_PHONG].Clear();
    m_hCompiledPS[EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT].Clear();
    m_hCompiledPS[EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD].Clear();
    m_hCompiledPS[EFFECTLIGHT_TECHNIQUE_POINT_PHONG].Clear();

    // Destroy render states
    m_hRasterizerState.UnBind();
    m_hDepthStencilState.UnBind();
    m_hBlendState.UnBind();
}

EffectLightMaterialTextureInstance * EffectLightMaterialTexture::CreateInstance( EffectLightTechnique iTechnique, Light * pLight, Material * pMaterial, GPUTexture2D * pTexture ) const
{
    Assert( pTexture->IsBound() );
    Assert( pTexture->GetBinds() & GPURESOURCE_BIND_SHADER_INPUT );

    RenderingFn->SelectMemory( TEXT("Effects") );
    EffectLightMaterialTextureInstance * pInstance = New EffectLightMaterialTextureInstance( this, iTechnique, pLight, pMaterial, pTexture );
	RenderingFn->UnSelectMemory();

    return pInstance;
}

/////////////////////////////////////////////////////////////////////////////////
// EffectLightMaterialTextureInstance implementation
EffectLightMaterialTextureInstance::EffectLightMaterialTextureInstance( const EffectLightMaterialTexture * pEffectLightMaterialTexture, EffectLightTechnique iTechnique, Light * pLight, Material * pMaterial, GPUTexture2D * pTexture ):
    EffectInstance( pEffectLightMaterialTexture, iTechnique ), m_hParameters(),
    m_hVSParameters( pEffectLightMaterialTexture->GetVertexShader(iTechnique,0) ),
    m_hPSParameters( pEffectLightMaterialTexture->GetPixelShader(iTechnique,0) ),
    m_hPSSampler()
{
    m_pLight = pLight;
    m_pMaterial = pMaterial;
    m_pTexture = pTexture;

    // Create constant buffers
    switch( iTechnique ) {
        case EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT:
            m_pVSConstants = RenderingFn->CreateConstantBuffer( 13, NULL );
            m_pPSConstants = NULL;
            break;
        case EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD:
            m_pVSConstants = RenderingFn->CreateConstantBuffer( 18, NULL );
            m_pPSConstants = NULL;
            break;
        case EFFECTLIGHT_TECHNIQUE_POINT_PHONG:
            m_pVSConstants = RenderingFn->CreateConstantBuffer( 8, NULL );
            m_pPSConstants = RenderingFn->CreateConstantBuffer( 10, NULL );
            break;
        default: Assert( false ); break;
    }
    m_pVSConstants->SetUsage( GPURESOURCE_USAGE_DEFAULT );
    m_pVSConstants->Bind();
    if ( m_pPSConstants != NULL ) {
        m_pPSConstants->SetUsage( GPURESOURCE_USAGE_DEFAULT );
        m_pPSConstants->Bind();
    }

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

    if ( m_pPSConstants != NULL )
        m_hPSParameters.AddConstantBuffer( m_pPSConstants );
    m_hPSParameters.AddSampler( &m_hPSSampler );
    m_hPSParameters.AddResource( &m_hPSTextureView );

    m_hParameters.SetShaderParameters( GPUSHADER_VERTEX, &m_hVSParameters );
    m_hParameters.SetShaderParameters( GPUSHADER_PIXEL, &m_hPSParameters );
	SetParameters( 0, &m_hParameters );
}
EffectLightMaterialTextureInstance::~EffectLightMaterialTextureInstance()
{
    // Destroy samplers & textures
    m_hPSSampler.UnBind();
    m_hPSTextureView.UnBind();

    // Destroy constant buffers
    m_pVSConstants->UnBind();
    RenderingFn->DestroyResource( m_pVSConstants );

    if ( m_pPSConstants != NULL ) {
        m_pPSConstants->UnBind();
        RenderingFn->DestroyResource( m_pPSConstants );
    }
}

Void EffectLightMaterialTextureInstance::UpdateConstants( UInt iPass, const Transform3 * pWorldTransform, const Camera * pCamera, GPUDeferredContext * pContext ) const
{
    Assert( iPass < m_iPassCount );

    // Get W matrix
    Matrix4 * pW = (Matrix4*)( m_pVSConstants->GetElement<GPUConstant>(0) );

    *pW = pWorldTransform->GetHMatrix();

    // Compute PV matrix
    Matrix4 * pPV = (Matrix4*)( m_pVSConstants->GetElement<GPUConstant>(4) );

    *pPV = ( pCamera->GetProjectionMatrix() * pCamera->GetViewMatrix() );
    if ( pCamera->HasPreView() )
        *pPV *= pCamera->GetPreViewMatrix();
    if ( pCamera->HasPostProjection() )
        *pPV = ( pCamera->GetPostProjectionMatrix() * (*pPV) );

    // Update Camera, Light & Material constants
    switch( m_iTechniqueIndex ) {
        case EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT: {
                Vertex4 * pLightWorldPosition = (Vertex4*)( m_pVSConstants->GetElement<GPUConstant>(8) );
                *pLightWorldPosition = m_pLight->Position;

                Color4 * pLightAttenuation = (Color4*)( m_pVSConstants->GetElement<GPUConstant>(9) );
                pLightAttenuation->R = m_pLight->FadeConstant;
                pLightAttenuation->G = m_pLight->FadeLinear;
                pLightAttenuation->B = m_pLight->FadeQuadratic;
                pLightAttenuation->A = m_pLight->Intensity;

                Color4 * pLightAmbiant = (Color4*)( m_pVSConstants->GetElement<GPUConstant>(10) );
                *pLightAmbiant = m_pLight->Ambient;

                Color4 * pMaterialEmissive = (Color4*)( m_pVSConstants->GetElement<GPUConstant>(11) );
                *pMaterialEmissive = m_pMaterial->Emissive;

                Color4 * pMaterialAmbiant = (Color4*)( m_pVSConstants->GetElement<GPUConstant>(12) );
                *pMaterialAmbiant = m_pMaterial->Ambient;
            } break;
        case EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD: {
                Vertex4 * pCameraWorldPosition = (Vertex4*)( m_pVSConstants->GetElement<GPUConstant>(8) );
                *pCameraWorldPosition = pCamera->GetViewEye();

                Vertex4 * pLightWorldPosition = (Vertex4*)( m_pVSConstants->GetElement<GPUConstant>(9) );
                *pLightWorldPosition = m_pLight->Position;

                Color4 * pLightAttenuation = (Color4*)( m_pVSConstants->GetElement<GPUConstant>(10) );
                pLightAttenuation->R = m_pLight->FadeConstant;
                pLightAttenuation->G = m_pLight->FadeLinear;
                pLightAttenuation->B = m_pLight->FadeQuadratic;
                pLightAttenuation->A = m_pLight->Intensity;

                Color4 * pLightAmbiant = (Color4*)( m_pVSConstants->GetElement<GPUConstant>(11) );
                *pLightAmbiant = m_pLight->Ambient;

                Color4 * pLightDiffuse = (Color4*)( m_pVSConstants->GetElement<GPUConstant>(12) );
                *pLightDiffuse = m_pLight->Diffuse;

                Color4 * pLightSpecular = (Color4*)( m_pVSConstants->GetElement<GPUConstant>(13) );
                *pLightSpecular = m_pLight->Specular;

                Color4 * pMaterialEmissive = (Color4*)( m_pVSConstants->GetElement<GPUConstant>(14) );
                *pMaterialEmissive = m_pMaterial->Emissive;

                Color4 * pMaterialAmbiant = (Color4*)( m_pVSConstants->GetElement<GPUConstant>(15) );
                *pMaterialAmbiant = m_pMaterial->Ambient;

                Color4 * pMaterialDiffuse = (Color4*)( m_pVSConstants->GetElement<GPUConstant>(16) );
                *pMaterialDiffuse = m_pMaterial->Diffuse;

                Color4 * pMaterialSpecular = (Color4*)( m_pVSConstants->GetElement<GPUConstant>(17) );
                *pMaterialSpecular = m_pMaterial->Specular;
            } break;
        case EFFECTLIGHT_TECHNIQUE_POINT_PHONG: {
                Vertex4 * pCameraWorldPosition = (Vertex4*)( m_pPSConstants->GetElement<GPUConstant>(0) );
                *pCameraWorldPosition = pCamera->GetViewEye();

                Vertex4 * pLightWorldPosition = (Vertex4*)( m_pPSConstants->GetElement<GPUConstant>(1) );
                *pLightWorldPosition = m_pLight->Position;

                Color4 * pLightAttenuation = (Color4*)( m_pPSConstants->GetElement<GPUConstant>(2) );
                pLightAttenuation->R = m_pLight->FadeConstant;
                pLightAttenuation->G = m_pLight->FadeLinear;
                pLightAttenuation->B = m_pLight->FadeQuadratic;
                pLightAttenuation->A = m_pLight->Intensity;

                Color4 * pLightAmbiant = (Color4*)( m_pPSConstants->GetElement<GPUConstant>(3) );
                *pLightAmbiant = m_pLight->Ambient;

                Color4 * pLightDiffuse = (Color4*)( m_pPSConstants->GetElement<GPUConstant>(4) );
                *pLightDiffuse = m_pLight->Diffuse;

                Color4 * pLightSpecular = (Color4*)( m_pPSConstants->GetElement<GPUConstant>(5) );
                *pLightSpecular = m_pLight->Specular;

                Color4 * pMaterialEmissive = (Color4*)( m_pPSConstants->GetElement<GPUConstant>(6) );
                *pMaterialEmissive = m_pMaterial->Emissive;

                Color4 * pMaterialAmbiant = (Color4*)( m_pPSConstants->GetElement<GPUConstant>(7) );
                *pMaterialAmbiant = m_pMaterial->Ambient;

                Color4 * pMaterialDiffuse = (Color4*)( m_pPSConstants->GetElement<GPUConstant>(8) );
                *pMaterialDiffuse = m_pMaterial->Diffuse;

                Color4 * pMaterialSpecular = (Color4*)( m_pPSConstants->GetElement<GPUConstant>(9) );
                *pMaterialSpecular = m_pMaterial->Specular;
            } break;
        default: Assert( false ); break;
    }

    // Update
    m_pVSConstants->Update( 0, INVALID_OFFSET, pContext );
    if ( m_pPSConstants != NULL )
        m_pPSConstants->Update( 0, INVALID_OFFSET, pContext );
}
