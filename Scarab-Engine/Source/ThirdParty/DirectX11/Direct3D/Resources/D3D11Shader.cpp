/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/Resources/D3D11Shader.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU Shaders.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Third-Party Includes
#pragma warning(disable:4005)

#define WIN32_LEAN_AND_MEAN
#define INITGUID // Trick : Avoid D3DX11 dependency to resolve IID_ID3D11ShaderReflection
#include <d3d11.h>
#include <d3dcompiler.h>

#undef DebugAssert

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "D3D11Shader.h"

#include "../D3D11Renderer.h"

/////////////////////////////////////////////////////////////////////////////////
// D3D11ShaderCompiler implementation
const AChar * D3D11ShaderCompiler::sm_arrTargets[D3D11SHADER_COUNT] = { "vs_5_0", "gs_5_0", "ps_5_0", "hs_5_0", "ds_5_0", "cs_5_0" };

D3D11ShaderCompiler::D3D11ShaderCompiler()
{
    m_pByteCode = NULL;
}
D3D11ShaderCompiler::~D3D11ShaderCompiler()
{
    if ( IsCompiled() )
        Destroy();
}

Void D3D11ShaderCompiler::Compile( const AChar * strHLSLCode, UInt iHLSLCodeLength, const D3D11ShaderCompilationData * pCompilationData )
{
    DebugAssert( !(IsCompiled()) );

    m_pByteCode = NULL;
    ID3DBlob * pErrors = NULL;

    HRESULT hRes = D3DCompile2( strHLSLCode, iHLSLCodeLength, pCompilationData->strName, (const D3D_SHADER_MACRO *)( pCompilationData->arrMacros ), D3D_COMPILE_STANDARD_FILE_INCLUDE,
                                pCompilationData->strEntryPoint, sm_arrTargets[pCompilationData->iTarget], pCompilationData->iFlags, pCompilationData->iEffectFlags,
                                0, NULL, 0, &m_pByteCode, &pErrors );
    DebugAssert( hRes == S_OK && m_pByteCode != NULL );

    if ( pErrors != NULL ) {
        // Lookup errors here ...
        AChar strErrorBuffer[4096];
        UInt iSize = pErrors->GetBufferSize();
        DebugAssert( iSize < 4096 );
        MemCopy( strErrorBuffer, pErrors->GetBufferPointer(), iSize );
        strErrorBuffer[iSize] = ANULLBYTE;

        DebugAssert( false );

        pErrors->Release();
        pErrors = NULL;
    }
}
Void D3D11ShaderCompiler::Compile( const GChar * strFilename, const D3D11ShaderCompilationData * pCompilationData )
{
    DebugAssert( !(IsCompiled()) );

    m_pByteCode = NULL;
    ID3DBlob * pErrors = NULL;

    HRESULT hRes = D3DCompileFromFile( strFilename, (const D3D_SHADER_MACRO *)( pCompilationData->arrMacros ), D3D_COMPILE_STANDARD_FILE_INCLUDE,
                                       pCompilationData->strEntryPoint, sm_arrTargets[pCompilationData->iTarget], pCompilationData->iFlags, pCompilationData->iEffectFlags,
                                       &m_pByteCode, &pErrors );
    DebugAssert( hRes == S_OK && m_pByteCode != NULL );

    if ( pErrors != NULL ) {
        // Lookup errors here ...
        DebugAssert( false );

        pErrors->Release();
        pErrors = NULL;
    }
}
Void D3D11ShaderCompiler::Destroy()
{
    DebugAssert( IsCompiled() );

    m_pByteCode->Release();
    m_pByteCode = NULL;
}

const Byte * D3D11ShaderCompiler::GetByteCode() const
{
    DebugAssert( IsCompiled() );

    return (const Byte *)( m_pByteCode->GetBufferPointer() );
}
UInt D3D11ShaderCompiler::GetByteCodeLength() const
{
    DebugAssert( IsCompiled() );

    return m_pByteCode->GetBufferSize();
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11ShaderReflectConstantBuffer implementation
D3D11ShaderReflectConstantBuffer::D3D11ShaderReflectConstantBuffer()
{
    m_pConstantBuffer = NULL;
}
D3D11ShaderReflectConstantBuffer::~D3D11ShaderReflectConstantBuffer()
{
    // nothing to do
}

Void D3D11ShaderReflectConstantBuffer::GetDesc( D3D11ShaderConstantBufferDesc * outDesc ) const
{
    DebugAssert( m_pConstantBuffer != NULL );

    D3D11_SHADER_BUFFER_DESC hDescCB;
    HRESULT hRes = m_pConstantBuffer->GetDesc( &hDescCB );
    DebugAssert( hRes == S_OK );

    outDesc->ConvertFrom( &hDescCB );
}

UInt D3D11ShaderReflectConstantBuffer::GetVariableCount() const
{
    DebugAssert( m_pConstantBuffer != NULL );

    D3D11_SHADER_BUFFER_DESC hDescCB;
    HRESULT hRes = m_pConstantBuffer->GetDesc( &hDescCB );
    DebugAssert( hRes == S_OK );

    return hDescCB.Variables;
}
Void D3D11ShaderReflectConstantBuffer::GetVariable( D3D11ShaderReflectVariable * outVariable, UInt iIndex ) const
{
    DebugAssert( m_pConstantBuffer != NULL );

    ID3D11ShaderReflectionVariable * pVariable = m_pConstantBuffer->GetVariableByIndex( iIndex );
    DebugAssert( pVariable != NULL );

    outVariable->m_pVariable = pVariable;
}
Void D3D11ShaderReflectConstantBuffer::GetVariable( D3D11ShaderReflectVariable * outVariable, const AChar * strName ) const
{
    DebugAssert( m_pConstantBuffer != NULL );

    ID3D11ShaderReflectionVariable * pVariable = m_pConstantBuffer->GetVariableByName( strName );
    DebugAssert( pVariable != NULL );

    outVariable->m_pVariable = pVariable;
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11ShaderReflectVariable implementation
D3D11ShaderReflectVariable::D3D11ShaderReflectVariable()
{
    m_pVariable = NULL;
}
D3D11ShaderReflectVariable::~D3D11ShaderReflectVariable()
{
    // nothing to do
}

Void D3D11ShaderReflectVariable::GetDesc( D3D11ShaderVariableDesc * outDesc ) const
{
    DebugAssert( m_pVariable != NULL );

    D3D11_SHADER_VARIABLE_DESC hDescVar;
    HRESULT hRes = m_pVariable->GetDesc( &hDescVar );
    DebugAssert( hRes == S_OK );

    outDesc->ConvertFrom( &hDescVar );
}

UInt D3D11ShaderReflectVariable::GetInterfaceSlot( UInt iArrayIndex ) const
{
    DebugAssert( m_pVariable != NULL );

    return m_pVariable->GetInterfaceSlot( iArrayIndex );
}

Void D3D11ShaderReflectVariable::GetConstantBuffer( D3D11ShaderReflectConstantBuffer * outConstantBuffer ) const
{
    DebugAssert( m_pVariable != NULL );

    ID3D11ShaderReflectionConstantBuffer * pConstantBuffer = m_pVariable->GetBuffer();
    DebugAssert( pConstantBuffer != NULL );

    outConstantBuffer->m_pConstantBuffer =  pConstantBuffer;
}
Void D3D11ShaderReflectVariable::GetType( D3D11ShaderReflectType * outType ) const
{
    DebugAssert( m_pVariable != NULL );

    ID3D11ShaderReflectionType * pType = m_pVariable->GetType();
    DebugAssert( pType != NULL );

    outType->m_pType = pType;
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11ShaderReflectType implementation
D3D11ShaderReflectType::D3D11ShaderReflectType()
{
    m_pType = NULL;
}
D3D11ShaderReflectType::~D3D11ShaderReflectType()
{
    // nothing to do
}

Void D3D11ShaderReflectType::GetDesc( D3D11ShaderTypeDesc * outDesc ) const
{
    DebugAssert( m_pType != NULL );

    D3D11_SHADER_TYPE_DESC hDescType;
    HRESULT hRes = m_pType->GetDesc( &hDescType );
    DebugAssert( hRes == S_OK );

    outDesc->ConvertFrom( &hDescType, m_pType->GetNumInterfaces() );
}

Bool D3D11ShaderReflectType::IsEqual( const D3D11ShaderReflectType * pType ) const
{
    DebugAssert( m_pType != NULL && pType->m_pType != NULL );

    return ( m_pType->IsEqual(pType->m_pType) == S_OK );
}
Bool D3D11ShaderReflectType::InheritsFrom( const D3D11ShaderReflectType * pBaseType ) const
{
    DebugAssert( m_pType != NULL && pBaseType->m_pType != NULL );

    return ( m_pType->IsOfType(pBaseType->m_pType) == S_OK );
}
Bool D3D11ShaderReflectType::ImplementsInterface( const D3D11ShaderReflectType * pInterface ) const
{
    DebugAssert( m_pType != NULL && pInterface->m_pType != NULL );

    return ( m_pType->ImplementsInterface(pInterface->m_pType) == S_OK );
}

Void D3D11ShaderReflectType::GetBaseClass( D3D11ShaderReflectType * outBaseClass ) const
{
    DebugAssert( m_pType != NULL );

    ID3D11ShaderReflectionType * pType = m_pType->GetSubType();
    DebugAssert( pType != NULL );

    outBaseClass->m_pType = pType;
}
Void D3D11ShaderReflectType::GetVariableBaseClass( D3D11ShaderReflectType * outBaseClass ) const
{
    DebugAssert( m_pType != NULL );

    ID3D11ShaderReflectionType * pType = m_pType->GetBaseClass();
    DebugAssert( pType != NULL );

    outBaseClass->m_pType = pType;
}

UInt D3D11ShaderReflectType::GetInterfaceCount() const
{
    DebugAssert( m_pType != NULL );

    return m_pType->GetNumInterfaces();
}
Void D3D11ShaderReflectType::GetInterface( D3D11ShaderReflectType * outInterface, UInt iIndex ) const
{
    DebugAssert( m_pType != NULL );

    ID3D11ShaderReflectionType * pType = m_pType->GetInterfaceByIndex( iIndex );
    DebugAssert( pType != NULL );

    outInterface->m_pType = pType;
}

UInt D3D11ShaderReflectType::GetMemberTypeCount() const
{
    DebugAssert( m_pType != NULL );

    D3D11_SHADER_TYPE_DESC hDescType;
    HRESULT hRes = m_pType->GetDesc( &hDescType );
    DebugAssert( hRes == S_OK );

    return hDescType.Members;
}
const AChar * D3D11ShaderReflectType::GetMemberTypeName( UInt iIndex ) const
{
    DebugAssert( m_pType != NULL );

    return m_pType->GetMemberTypeName( iIndex );
}
Void D3D11ShaderReflectType::GetMemberType( D3D11ShaderReflectType * outMember, UInt iIndex ) const
{
    DebugAssert( m_pType != NULL );

    ID3D11ShaderReflectionType * pType = m_pType->GetMemberTypeByIndex( iIndex );
    DebugAssert( pType != NULL );

    outMember->m_pType = pType;
}
Void D3D11ShaderReflectType::GetMemberType( D3D11ShaderReflectType * outMember, const AChar * strName ) const
{
    DebugAssert( m_pType != NULL );

    ID3D11ShaderReflectionType * pType = m_pType->GetMemberTypeByName( strName );
    DebugAssert( pType != NULL );

    outMember->m_pType = pType;
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11Shader implementation
D3D11Shader::D3D11Shader( D3D11Renderer * pRenderer )
{
    m_pRenderer = pRenderer;

    m_pCompiledShader = NULL;

    m_pShader = NULL;

    // Reflection
    m_bHasReflection = false;
    m_pReflector = NULL;

    // Dynamic linkage
    m_bHasDynamicLinkage = false;
    m_pClassLinkage = NULL;
    m_iClassInstanceCount = 0;
    m_arrClassInstances = NULL;

    // Done
    m_bTemporaryDestroyed = false;
}
D3D11Shader::~D3D11Shader()
{
    // nothing to do
}

Void D3D11Shader::Create( const D3D11ShaderCompiler * pCompiledShader, Bool bReflect, Bool bDynamicLinking )
{
    if ( GetType() == D3D11SHADER_GEOMETRY ) {
        DebugAssert( ((D3D11GeometryShader*)this)->IsFinalized() );
    }
    DebugAssert( !(IsCreated()) );
    DebugAssert( bReflect || !bDynamicLinking );

    m_pCompiledShader = pCompiledShader;

    m_bHasReflection = bReflect;

    m_bHasDynamicLinkage = bDynamicLinking;

    _NakedCreate();
}
Void D3D11Shader::Destroy()
{
    DebugAssert( IsCreated() );

    if ( m_bTemporaryDestroyed )
        m_bTemporaryDestroyed = false;
    else
        _NakedDestroy();

    m_pCompiledShader = NULL;

    m_bHasReflection = false;

    m_bHasDynamicLinkage = false;
}

Void D3D11Shader::OnDestroyDevice()
{
    DebugAssert( !m_bTemporaryDestroyed );

    if ( m_pShader != NULL ) {
        _NakedDestroy();
        m_bTemporaryDestroyed = true;
    }
}
Void D3D11Shader::OnRestoreDevice()
{
    DebugAssert( m_pShader == NULL );

    if ( m_bTemporaryDestroyed ) {
        _NakedCreate();
        m_bTemporaryDestroyed = false;
    }
}

Void D3D11Shader::GetDesc( D3D11ShaderDesc * outDesc ) const
{
    DebugAssert( HasReflection() );

    D3D11_SHADER_DESC hDesc;
    HRESULT hRes = m_pReflector->GetDesc( &hDesc );
    DebugAssert( hRes == S_OK );

    outDesc->ConvertFrom( &hDesc, ( m_pReflector->IsSampleFrequencyShader() == TRUE ), (UInt)( m_pReflector->GetRequiresFlags() ), m_pReflector->GetNumInterfaceSlots(),
                          m_pReflector->GetBitwiseInstructionCount(), m_pReflector->GetConversionInstructionCount(), m_pReflector->GetMovInstructionCount(),
                          m_pReflector->GetMovcInstructionCount() );
}

UInt D3D11Shader::GetInputParameterCount() const
{
    DebugAssert( HasReflection() );

    D3D11_SHADER_DESC hDesc;
    HRESULT hRes = m_pReflector->GetDesc( &hDesc );
    DebugAssert( hRes == S_OK );

    return hDesc.InputParameters;
}
Void D3D11Shader::GetInputParameterDesc( D3D11ShaderParameterDesc * outInputParameterDesc, UInt iIndex ) const
{
    DebugAssert( HasReflection() );

    D3D11_SIGNATURE_PARAMETER_DESC hDesc;
    HRESULT hRes = m_pReflector->GetInputParameterDesc( iIndex, &hDesc );
    DebugAssert( hRes == S_OK );

    outInputParameterDesc->ConvertFrom( &hDesc );
}

UInt D3D11Shader::GetOutputParameterCount() const
{
    DebugAssert( HasReflection() );

    D3D11_SHADER_DESC hDesc;
    HRESULT hRes = m_pReflector->GetDesc( &hDesc );
    DebugAssert( hRes == S_OK );

    return hDesc.OutputParameters;
}
Void D3D11Shader::GetOutputParameterDesc( D3D11ShaderParameterDesc * outOutputParameterDesc, UInt iIndex ) const
{
    DebugAssert( HasReflection() );

    D3D11_SIGNATURE_PARAMETER_DESC hDesc;
    HRESULT hRes = m_pReflector->GetOutputParameterDesc( iIndex, &hDesc );
    DebugAssert( hRes == S_OK );

    outOutputParameterDesc->ConvertFrom( &hDesc );
}

UInt D3D11Shader::GetPatchConstantParameterCount() const
{
    DebugAssert( HasReflection() );

    D3D11_SHADER_DESC hDesc;
    HRESULT hRes = m_pReflector->GetDesc( &hDesc );
    DebugAssert( hRes == S_OK );

    return hDesc.PatchConstantParameters;
}
Void D3D11Shader::GetPatchConstantParameterDesc( D3D11ShaderParameterDesc * outPatchConstantParameterDesc, UInt iIndex ) const
{
    DebugAssert( HasReflection() );

    D3D11_SIGNATURE_PARAMETER_DESC hDesc;
    HRESULT hRes = m_pReflector->GetPatchConstantParameterDesc( iIndex, &hDesc );
    DebugAssert( hRes == S_OK );

    outPatchConstantParameterDesc->ConvertFrom( &hDesc );
}

UInt D3D11Shader::GetBindingCount() const
{
    DebugAssert( HasReflection() );

    D3D11_SHADER_DESC hDesc;
    HRESULT hRes = m_pReflector->GetDesc( &hDesc );
    DebugAssert( hRes == S_OK );

    return hDesc.BoundResources;
}
Void D3D11Shader::GetBindingDesc( D3D11ShaderBindingDesc * outBindingDesc, UInt iIndex ) const
{
    DebugAssert( HasReflection() );

    D3D11_SHADER_INPUT_BIND_DESC hDesc;
    HRESULT hRes = m_pReflector->GetResourceBindingDesc( iIndex, &hDesc );
    DebugAssert( hRes == S_OK );

    outBindingDesc->ConvertFrom( &hDesc );
}
Void D3D11Shader::GetBindingDesc( D3D11ShaderBindingDesc * outBindingDesc, const AChar * strName ) const
{
    DebugAssert( HasReflection() );

    D3D11_SHADER_INPUT_BIND_DESC hDesc;
    HRESULT hRes = m_pReflector->GetResourceBindingDescByName( strName, &hDesc );
    DebugAssert( hRes == S_OK );

    outBindingDesc->ConvertFrom( &hDesc );
}

UInt D3D11Shader::GetConstantBufferCount() const
{
    DebugAssert( HasReflection() );

    D3D11_SHADER_DESC hDesc;
    HRESULT hRes = m_pReflector->GetDesc( &hDesc );
    DebugAssert( hRes == S_OK );

    return hDesc.ConstantBuffers;
}
Void D3D11Shader::GetConstantBuffer( D3D11ShaderReflectConstantBuffer * outConstantBuffer, UInt iIndex ) const
{
    DebugAssert( HasReflection() );

    ID3D11ShaderReflectionConstantBuffer * pConstantBuffer = m_pReflector->GetConstantBufferByIndex( iIndex );
    DebugAssert( pConstantBuffer != NULL );

    outConstantBuffer->m_pConstantBuffer = pConstantBuffer;
}
Void D3D11Shader::GetConstantBuffer( D3D11ShaderReflectConstantBuffer * outConstantBuffer, const AChar * strName ) const
{
    DebugAssert( HasReflection() );

    ID3D11ShaderReflectionConstantBuffer * pConstantBuffer = m_pReflector->GetConstantBufferByName( strName );
    DebugAssert( pConstantBuffer != NULL );

    outConstantBuffer->m_pConstantBuffer = pConstantBuffer;
}

Void D3D11Shader::GetVariable( D3D11ShaderReflectVariable * outVariable, const AChar * strName ) const
{
    DebugAssert( HasReflection() );

    ID3D11ShaderReflectionVariable * pVariable = m_pReflector->GetVariableByName( strName );
    DebugAssert( pVariable != NULL );

    outVariable->m_pVariable = pVariable;
}

UInt D3D11Shader::GetInterfaceSlotCount() const
{
    DebugAssert( HasReflection() );

    return m_pReflector->GetNumInterfaceSlots();
}

Void D3D11Shader::GetThreadGroupSize( UInt * outSizeX, UInt * outSizeY, UInt * outSizeZ ) const
{
    DebugAssert( HasReflection() );

    HRESULT hRes = m_pReflector->GetThreadGroupSize( outSizeX, outSizeY, outSizeZ );
    DebugAssert( hRes == S_OK );
}

Void D3D11Shader::AddDynamicLink( const AChar * strInterfaceInstance, const AChar * strClassInstance )
{
    DebugAssert( HasDynamicLinkage() );

    D3D11ShaderReflectVariable hVariable;
    GetVariable( &hVariable, strInterfaceInstance );

    UInt iSlot = hVariable.GetInterfaceSlot( 0 );
    DebugAssert( m_arrClassInstances[iSlot] == NULL );

    ID3D11ClassInstance * pClassInstance = NULL;
    HRESULT hRes = m_pClassLinkage->GetClassInstance( strClassInstance, 0, &pClassInstance );
    DebugAssert( hRes == S_OK );

    m_arrClassInstances[iSlot] = pClassInstance;
}

/////////////////////////////////////////////////////////////////////////////////

Void D3D11Shader::_InitializeReflection()
{
    m_pReflector = NULL;
    HRESULT hRes = D3DReflect( m_pCompiledShader->GetByteCode(), m_pCompiledShader->GetByteCodeLength(), IID_ID3D11ShaderReflection, (Void**)&m_pReflector );
    DebugAssert( hRes == S_OK && m_pReflector != NULL );

    D3D_FEATURE_LEVEL iFeatureLevel;
    hRes = m_pReflector->GetMinFeatureLevel( &iFeatureLevel );
    DebugAssert( hRes == S_OK );
    DebugAssert( iFeatureLevel <= m_pRenderer->m_iFeatureLevel );
}
Void D3D11Shader::_CleanupReflection()
{
    m_pReflector->Release();
    m_pReflector = NULL;
}

Void D3D11Shader::_InitializeDynamicLinkage()
{
    m_iClassInstanceCount = m_pReflector->GetNumInterfaceSlots();
    m_arrClassInstances = (ID3D11ClassInstance**)( SystemFn->MemAlloc(m_iClassInstanceCount * sizeof(ID3D11ClassInstance*)) );

    for( UInt i = 0; i < m_iClassInstanceCount; ++i )
        m_arrClassInstances[i] = NULL;
}
Void D3D11Shader::_CleanupDynamicLinkage()
{
    m_iClassInstanceCount = 0;

    SystemFn->MemFree( m_arrClassInstances );
    m_arrClassInstances = NULL;
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11VertexShader implementation
D3D11VertexShader::D3D11VertexShader( D3D11Renderer * pRenderer ):
    D3D11Shader( pRenderer )
{
    m_pVertexShader = NULL;
}
D3D11VertexShader::~D3D11VertexShader()
{
    if ( IsCreated() )
        Destroy();
}

/////////////////////////////////////////////////////////////////////////////////

Void D3D11VertexShader::_NakedCreate()
{
    HRESULT hRes;

    m_pClassLinkage = NULL;
    if ( m_bHasDynamicLinkage ) {
        hRes = m_pRenderer->m_pDevice->CreateClassLinkage( &m_pClassLinkage );
        DebugAssert( hRes == S_OK && m_pClassLinkage != NULL );
    }

    m_pVertexShader = NULL;
    hRes = m_pRenderer->m_pDevice->CreateVertexShader( m_pCompiledShader->GetByteCode(), m_pCompiledShader->GetByteCodeLength(), m_pClassLinkage, &m_pVertexShader );
    DebugAssert( hRes == S_OK && m_pVertexShader != NULL );

    m_pShader = NULL;
    hRes = m_pVertexShader->QueryInterface( __uuidof(ID3D11DeviceChild), (Void**)&m_pShader );
    DebugAssert( hRes == S_OK && m_pShader != NULL );

    if ( m_bHasReflection ) {
        _InitializeReflection();
        if ( m_bHasDynamicLinkage )
            _InitializeDynamicLinkage();
    }
}
Void D3D11VertexShader::_NakedDestroy()
{
    if ( m_bHasReflection ) {
        if ( m_bHasDynamicLinkage )
            _CleanupDynamicLinkage();
        _CleanupReflection();
    }

    m_pShader->Release();
    m_pShader = NULL;

    m_pVertexShader->Release();
    m_pVertexShader = NULL;

    if ( m_bHasDynamicLinkage ) {
        m_pClassLinkage->Release();
        m_pClassLinkage = NULL;
    }
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11GeometryShader implementation
D3D11GeometryShader::D3D11GeometryShader( D3D11Renderer * pRenderer ):
    D3D11Shader( pRenderer )
{
    m_iSODeclarationCount = 0;

    m_iStrideCount = 0;

    m_iRasterStream = INVALID_OFFSET;

    m_bFinalized = false;
    m_pGeometryShader = NULL;
}
D3D11GeometryShader::~D3D11GeometryShader()
{
    if ( IsCreated() )
        Destroy();
}

Void D3D11GeometryShader::PushSODeclaration( const D3D11StreamOutputDeclaration * pSODeclaration )
{
    DebugAssert( !m_bFinalized );
    DebugAssert( m_iSODeclarationCount < (D3D11_SO_STREAM_COUNT * D3D11_SO_OUTPUT_COMPONENT_COUNT) );

    DebugAssert( pSODeclaration->iStartComponent + pSODeclaration->iComponentCount <= 4 );
    DebugAssert( pSODeclaration->iOutputSlot < D3D11_SO_BUFFER_SLOT_COUNT );

    pSODeclaration->ConvertTo( m_arrSODeclarations + m_iSODeclarationCount );

    ++m_iSODeclarationCount;
}
Void D3D11GeometryShader::Finalize( const UInt * arrStrides, UInt iStrideCount, UInt iRasterStream )
{
    DebugAssert( !m_bFinalized );

    if ( m_iSODeclarationCount > 0 ) {
        DebugAssert( arrStrides != NULL );
        DebugAssert( iStrideCount <= D3D11_SO_BUFFER_SLOT_COUNT );
        m_iStrideCount = iStrideCount;
        for( UInt i = 0; i < m_iStrideCount; ++i )
            m_arrStrides[i] = arrStrides[i];
        if ( iRasterStream != INVALID_OFFSET ) {
            DebugAssert( iRasterStream < D3D11_SO_STREAM_COUNT );
        }
        m_iRasterStream = iRasterStream;
    } else {
        DebugAssert( arrStrides == NULL );
    }

    m_bFinalized = true;
}

/////////////////////////////////////////////////////////////////////////////////

Void D3D11GeometryShader::_NakedCreate()
{
    HRESULT hRes;

    m_pClassLinkage = NULL;
    if ( m_bHasDynamicLinkage ) {
        hRes = m_pRenderer->m_pDevice->CreateClassLinkage( &m_pClassLinkage );
        DebugAssert( hRes == S_OK && m_pClassLinkage != NULL );
    }

    if ( m_iSODeclarationCount > 0 ) {
        m_pGeometryShader = NULL;
        hRes = m_pRenderer->m_pDevice->CreateGeometryShaderWithStreamOutput( m_pCompiledShader->GetByteCode(), m_pCompiledShader->GetByteCodeLength(),
                                                                             (const D3D11_SO_DECLARATION_ENTRY *)m_arrSODeclarations, m_iSODeclarationCount, m_arrStrides, m_iStrideCount,
                                                                             (m_iRasterStream != INVALID_OFFSET) ? m_iRasterStream : D3D11_SO_NO_RASTERIZED_STREAM, m_pClassLinkage, &m_pGeometryShader );
        DebugAssert( hRes == S_OK && m_pGeometryShader != NULL );
    } else {
        m_pGeometryShader = NULL;
        hRes = m_pRenderer->m_pDevice->CreateGeometryShader( m_pCompiledShader->GetByteCode(), m_pCompiledShader->GetByteCodeLength(), m_pClassLinkage, &m_pGeometryShader );
        DebugAssert( hRes == S_OK && m_pGeometryShader != NULL );
    }

    m_pShader = NULL;
    hRes = m_pGeometryShader->QueryInterface( __uuidof(ID3D11DeviceChild), (Void**)&m_pShader );
    DebugAssert( hRes == S_OK && m_pShader != NULL );

    if ( m_bHasReflection ) {
        _InitializeReflection();
        if ( m_bHasDynamicLinkage )
            _InitializeDynamicLinkage();
    }
}
Void D3D11GeometryShader::_NakedDestroy()
{
    if ( m_bHasReflection ) {
        if ( m_bHasDynamicLinkage )
            _CleanupDynamicLinkage();
        _CleanupReflection();
    }

    m_pShader->Release();
    m_pShader = NULL;

    m_pGeometryShader->Release();
    m_pGeometryShader = NULL;

    if ( m_bHasDynamicLinkage ) {
        m_pClassLinkage->Release();
        m_pClassLinkage = NULL;
    }
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11PixelShader implementation
D3D11PixelShader::D3D11PixelShader( D3D11Renderer * pRenderer ):
    D3D11Shader( pRenderer )
{
    m_pPixelShader = NULL;
}
D3D11PixelShader::~D3D11PixelShader()
{
    if ( IsCreated() )
        Destroy();
}

/////////////////////////////////////////////////////////////////////////////////

Void D3D11PixelShader::_NakedCreate()
{
    HRESULT hRes;

    m_pClassLinkage = NULL;
    if ( m_bHasDynamicLinkage ) {
        hRes = m_pRenderer->m_pDevice->CreateClassLinkage( &m_pClassLinkage );
        DebugAssert( hRes == S_OK && m_pClassLinkage != NULL );
    }

    m_pPixelShader = NULL;
    hRes = m_pRenderer->m_pDevice->CreatePixelShader( m_pCompiledShader->GetByteCode(), m_pCompiledShader->GetByteCodeLength(), m_pClassLinkage, &m_pPixelShader );
    DebugAssert( hRes == S_OK && m_pPixelShader != NULL );

    m_pShader = NULL;
    hRes = m_pPixelShader->QueryInterface( __uuidof(ID3D11DeviceChild), (Void**)&m_pShader );
    DebugAssert( hRes == S_OK && m_pShader != NULL );

    if ( m_bHasReflection ) {
        _InitializeReflection();
        if ( m_bHasDynamicLinkage )
            _InitializeDynamicLinkage();
    }
}
Void D3D11PixelShader::_NakedDestroy()
{
    if ( m_bHasReflection ) {
        if ( m_bHasDynamicLinkage )
            _CleanupDynamicLinkage();
        _CleanupReflection();
    }

    m_pShader->Release();
    m_pShader = NULL;

    m_pPixelShader->Release();
    m_pPixelShader = NULL;

    if ( m_bHasDynamicLinkage ) {
        m_pClassLinkage->Release();
        m_pClassLinkage = NULL;
    }
}


/////////////////////////////////////////////////////////////////////////////////
// D3D11HullShader implementation
D3D11HullShader::D3D11HullShader( D3D11Renderer * pRenderer ):
    D3D11Shader( pRenderer )
{
    m_pHullShader = NULL;
}
D3D11HullShader::~D3D11HullShader()
{
    if ( IsCreated() )
        Destroy();
}

/////////////////////////////////////////////////////////////////////////////////

Void D3D11HullShader::_NakedCreate()
{
    HRESULT hRes;

    m_pClassLinkage = NULL;
    if ( m_bHasDynamicLinkage ) {
        hRes = m_pRenderer->m_pDevice->CreateClassLinkage( &m_pClassLinkage );
        DebugAssert( hRes == S_OK && m_pClassLinkage != NULL );
    }

    m_pHullShader = NULL;
    hRes = m_pRenderer->m_pDevice->CreateHullShader( m_pCompiledShader->GetByteCode(), m_pCompiledShader->GetByteCodeLength(), m_pClassLinkage, &m_pHullShader );
    DebugAssert( hRes == S_OK && m_pHullShader != NULL );

    m_pShader = NULL;
    hRes = m_pHullShader->QueryInterface( __uuidof(ID3D11DeviceChild), (Void**)&m_pShader );
    DebugAssert( hRes == S_OK && m_pShader != NULL );

    if ( m_bHasReflection ) {
        _InitializeReflection();
        if ( m_bHasDynamicLinkage )
            _InitializeDynamicLinkage();
    }
}
Void D3D11HullShader::_NakedDestroy()
{
    if ( m_bHasReflection ) {
        if ( m_bHasDynamicLinkage )
            _CleanupDynamicLinkage();
        _CleanupReflection();
    }

    m_pShader->Release();
    m_pShader = NULL;

    m_pHullShader->Release();
    m_pHullShader = NULL;

    if ( m_bHasDynamicLinkage ) {
        m_pClassLinkage->Release();
        m_pClassLinkage = NULL;
    }
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11DomainShader implementation
D3D11DomainShader::D3D11DomainShader( D3D11Renderer * pRenderer ):
    D3D11Shader( pRenderer )
{
    m_pDomainShader = NULL;
}
D3D11DomainShader::~D3D11DomainShader()
{
    if ( IsCreated() )
        Destroy();
}

/////////////////////////////////////////////////////////////////////////////////

Void D3D11DomainShader::_NakedCreate()
{
    HRESULT hRes;

    m_pClassLinkage = NULL;
    if ( m_bHasDynamicLinkage ) {
        hRes = m_pRenderer->m_pDevice->CreateClassLinkage( &m_pClassLinkage );
        DebugAssert( hRes == S_OK && m_pClassLinkage != NULL );
    }

    m_pDomainShader = NULL;
    hRes = m_pRenderer->m_pDevice->CreateDomainShader( m_pCompiledShader->GetByteCode(), m_pCompiledShader->GetByteCodeLength(), m_pClassLinkage, &m_pDomainShader );
    DebugAssert( hRes == S_OK && m_pDomainShader != NULL );

    m_pShader = NULL;
    hRes = m_pDomainShader->QueryInterface( __uuidof(ID3D11DeviceChild), (Void**)&m_pShader );
    DebugAssert( hRes == S_OK && m_pShader != NULL );

    if ( m_bHasReflection ) {
        _InitializeReflection();
        if ( m_bHasDynamicLinkage )
            _InitializeDynamicLinkage();
    }
}
Void D3D11DomainShader::_NakedDestroy()
{
    if ( m_bHasReflection ) {
        if ( m_bHasDynamicLinkage )
            _CleanupDynamicLinkage();
        _CleanupReflection();
    }

    m_pShader->Release();
    m_pShader = NULL;

    m_pDomainShader->Release();
    m_pDomainShader = NULL;

    if ( m_bHasDynamicLinkage ) {
        m_pClassLinkage->Release();
        m_pClassLinkage = NULL;
    }
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11ComputeShader implementation
D3D11ComputeShader::D3D11ComputeShader( D3D11Renderer * pRenderer ):
    D3D11Shader( pRenderer )
{
    m_pComputeShader = NULL;
}
D3D11ComputeShader::~D3D11ComputeShader()
{
    if ( IsCreated() )
        Destroy();
}

/////////////////////////////////////////////////////////////////////////////////

Void D3D11ComputeShader::_NakedCreate()
{
    HRESULT hRes;

    m_pClassLinkage = NULL;
    if ( m_bHasDynamicLinkage ) {
        hRes = m_pRenderer->m_pDevice->CreateClassLinkage( &m_pClassLinkage );
        DebugAssert( hRes == S_OK && m_pClassLinkage != NULL );
    }

    m_pComputeShader = NULL;
    hRes = m_pRenderer->m_pDevice->CreateComputeShader( m_pCompiledShader->GetByteCode(), m_pCompiledShader->GetByteCodeLength(), m_pClassLinkage, &m_pComputeShader );
    DebugAssert( hRes == S_OK && m_pComputeShader != NULL );

    m_pShader = NULL;
    hRes = m_pComputeShader->QueryInterface( __uuidof(ID3D11DeviceChild), (Void**)&m_pShader );
    DebugAssert( hRes == S_OK && m_pShader != NULL );

    if ( m_bHasReflection ) {
        _InitializeReflection();
        if ( m_bHasDynamicLinkage )
            _InitializeDynamicLinkage();
    }
}
Void D3D11ComputeShader::_NakedDestroy()
{
    if ( m_bHasReflection ) {
        if ( m_bHasDynamicLinkage )
            _CleanupDynamicLinkage();
        _CleanupReflection();
    }

    m_pShader->Release();
    m_pShader = NULL;

    m_pComputeShader->Release();
    m_pComputeShader = NULL;

    if ( m_bHasDynamicLinkage ) {
        m_pClassLinkage->Release();
        m_pClassLinkage = NULL;
    }
}

