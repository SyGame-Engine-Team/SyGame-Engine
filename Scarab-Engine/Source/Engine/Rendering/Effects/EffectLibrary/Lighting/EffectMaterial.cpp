/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Effects/EffectLibrary/Lighting/EffectMaterial.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Lighting effect : Material, diffuse only
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
#include "EffectMaterial.h"

#include "../../../RenderingManager.h"

/////////////////////////////////////////////////////////////////////////////////
// EffectMaterial implementation
const AChar * EffectMaterial::sm_strHLSLCode = (
    // Constants
    "matrix matWVP;"
    "float4 vMaterialEmissive;"

    // IO layouts
    "struct _VS_Input {"
    "    float4 vPosition : POSITION0;"
    "};"
    "struct _VS_Output {"
    "    float4 vClipPosition : SV_POSITION0;"
    "    float4 vColor : COLOR0;"
    "};"

    // Vertex shader implementation
    "_VS_Output _VS_Main( _VS_Input hInput ) {"
    "    _VS_Output hOutput;"
    "    hOutput.vClipPosition = mul( hInput.vPosition, matWVP );"
    "    hOutput.vColor = vMaterialEmissive;"
    "    return hOutput;"
    "}"

    // Pixel shader implementation
    "float4 _PS_Main( _VS_Output hInput ) : SV_TARGET {"
    "    return hInput.vColor;"
    "}"
);
const UInt EffectMaterial::sm_iHLSLCodeLength = StringFn->LengthA( sm_strHLSLCode );

EffectMaterial::EffectMaterial():
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
    hCompilationData.strName = "EffectLib.Material.VS";
    hCompilationData.strEntryPoint = "_VS_Main";

    m_hCompiledVS.Compile( sm_strHLSLCode, sm_iHLSLCodeLength, &hCompilationData );
    m_hVS.Bind( &m_hCompiledVS, false, false );

    hCompilationData.iTarget = GPUSHADER_PIXEL;
    hCompilationData.strName = "EffectLib.Material.PS";
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
EffectMaterial::~EffectMaterial()
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

EffectMaterialInstance * EffectMaterial::CreateInstance( Material * pMaterial ) const
{
    RenderingFn->SelectMemory( TEXT("Effects") );
    EffectMaterialInstance * pInstance = New EffectMaterialInstance( this, pMaterial );
	RenderingFn->UnSelectMemory();

    return pInstance;
}

/////////////////////////////////////////////////////////////////////////////////
// EffectMaterialInstance implementation
EffectMaterialInstance::EffectMaterialInstance( const EffectMaterial * pEffectMaterial, Material * pMaterial ):
    EffectInstance( pEffectMaterial, 0 ), m_hParameters(),
    m_hVSParameters( pEffectMaterial->GetVertexShader(0,0) )
{
    m_pMaterial = pMaterial;

    // Create constant buffers
    m_pVSConstants = RenderingFn->CreateConstantBuffer( 5, NULL );
    m_pVSConstants->SetUsage( GPURESOURCE_USAGE_DEFAULT );
    m_pVSConstants->Bind();

    // Build effect instance
    m_hVSParameters.AddConstantBuffer( m_pVSConstants );

    m_hParameters.SetShaderParameters( GPUSHADER_VERTEX, &m_hVSParameters );
	SetParameters( 0, &m_hParameters );
}
EffectMaterialInstance::~EffectMaterialInstance()
{
    // Destroy constant buffers
    m_pVSConstants->UnBind();
    RenderingFn->DestroyResource( m_pVSConstants );
}

Void EffectMaterialInstance::UpdateConstants( UInt iPass, const Transform3 * pWorldTransform, const Camera * pCamera, GPUDeferredContext * pContext ) const
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

    // Material emissive color
    Color4 * pMaterialEmissive = (Color4*)( m_pVSConstants->GetElement<GPUConstant>(4) );
    *pMaterialEmissive = m_pMaterial->Emissive;

    // Update
    m_pVSConstants->Update( 0, INVALID_OFFSET, pContext );
}

