/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Effects/EffectLibrary/EffectDefault.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : The default effect to fallback to if another can't be used.
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
#include "EffectDefault.h"

#include "../../RenderingManager.h"

/////////////////////////////////////////////////////////////////////////////////
// EffectDefault implementation
const AChar * EffectDefault::sm_strHLSLCode = (
    // Constants
    "matrix matWVP;"

    // IO layouts
    "struct _VS_Input {"
    "    float4 vPosition : POSITION0;"
    "};"
    "struct _VS_Output {"
    "    float4 vClipPosition : SV_POSITION0;"
    "};"

    // Vertex shader implementation
    "_VS_Output _VS_Main( _VS_Input hInput ) {"
    "    _VS_Output hOutput;"
    "    hOutput.vClipPosition = mul( hInput.vPosition, matWVP );"
    "    return hOutput;"
    "}"

    // Pixel shader implementation
    "float4 _PS_Main( _VS_Output hInput ) : SV_TARGET {"
    "    return float4( 1.0, 0.0, 1.0, 1.0 );"
    "}"
);
const UInt EffectDefault::sm_iHLSLCodeLength = StringFn->LengthA( sm_strHLSLCode );

EffectDefault::EffectDefault():
    Effect(),
    m_hRasterizerState(), m_hDepthStencilState(), m_hBlendState(), m_hIL(),
    m_hCompiledVS(), m_hCompiledPS(), m_hVS(), m_hPS(),
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
    hCompilationData.strName = "EffectLib.Default.VS";
    hCompilationData.strEntryPoint = "_VS_Main";

    m_hCompiledVS.Compile( sm_strHLSLCode, sm_iHLSLCodeLength, &hCompilationData );
    m_hVS.Bind( &m_hCompiledVS, false, false );

    hCompilationData.iTarget = GPUSHADER_PIXEL;
    hCompilationData.strName = "EffectLib.Default.PS";
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
EffectDefault::~EffectDefault()
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

EffectDefaultInstance * EffectDefault::CreateInstance() const
{
    RenderingFn->SelectMemory( TEXT("Effects") );
    EffectDefaultInstance * pInstance = New EffectDefaultInstance( this );
	RenderingFn->UnSelectMemory();

    return pInstance;
}

/////////////////////////////////////////////////////////////////////////////////
// EffectDefaultInstance implementation
EffectDefaultInstance::EffectDefaultInstance( const EffectDefault * pEffectDefault ):
    EffectInstance( pEffectDefault, 0 ), m_hParameters(),
    m_hVSParameters( pEffectDefault->GetVertexShader(0,0) )
{
    // Create constant buffers
    m_pVSConstants = RenderingFn->CreateConstantBuffer( 4, NULL );
    m_pVSConstants->SetUsage( GPURESOURCE_USAGE_DEFAULT );
    m_pVSConstants->Bind();

    // Build effect instance
    m_hVSParameters.AddConstantBuffer( m_pVSConstants );

    m_hParameters.SetShaderParameters( GPUSHADER_VERTEX, &m_hVSParameters );
	SetParameters( 0, &m_hParameters );
}
EffectDefaultInstance::~EffectDefaultInstance()
{
    // Destroy constant buffers
    m_pVSConstants->UnBind();
    RenderingFn->DestroyResource( m_pVSConstants );
}

Void EffectDefaultInstance::UpdateConstants( UInt iPass, const Transform3 * pWorldTransform, const Camera * pCamera, GPUDeferredContext * pContext ) const
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

