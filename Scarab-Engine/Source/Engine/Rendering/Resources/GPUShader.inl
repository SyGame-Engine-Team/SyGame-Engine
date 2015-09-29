/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Resources/GPUShader.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU Shaders interface
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// GPUShaderCompiler implementation
inline Bool GPUShaderCompiler::IsCompiled() const {
    return m_hDeviceShaderCompiler.IsCompiled();
}

inline Void GPUShaderCompiler::Compile( const AChar * strHLSLCode, UInt iHLSLCodeLength, const GPUShaderCompilationData * pCompilationData ) {
    Assert( !(IsCompiled()) );
    m_hDeviceShaderCompiler.Compile( strHLSLCode, iHLSLCodeLength, (const DeviceShaderCompilationData *)pCompilationData );
}
inline Void GPUShaderCompiler::Compile( const GChar * strFilename, const GPUShaderCompilationData * pCompilationData ) {
    Assert( !(IsCompiled()) );
    m_hDeviceShaderCompiler.Compile( strFilename, (const DeviceShaderCompilationData *)pCompilationData );
}
inline Void GPUShaderCompiler::Clear() {
    Assert( IsCompiled() );
    m_hDeviceShaderCompiler.Destroy();
}

inline const Byte * GPUShaderCompiler::GetByteCode() const {
    Assert( IsCompiled() );
    return m_hDeviceShaderCompiler.GetByteCode();
}
inline UInt GPUShaderCompiler::GetByteCodeLength() const {
    Assert( IsCompiled() );
    return m_hDeviceShaderCompiler.GetByteCodeLength();
}

/////////////////////////////////////////////////////////////////////////////////
// GPUShaderReflectConstantBuffer implementation
inline Void GPUShaderReflectConstantBuffer::GetDesc( GPUShaderConstantBufferDesc * outDesc ) const {
    m_hDeviceShaderReflectConstantBuffer.GetDesc( (DeviceShaderConstantBufferDesc*)outDesc );
}

inline UInt GPUShaderReflectConstantBuffer::GetVariableCount() const {
    return m_hDeviceShaderReflectConstantBuffer.GetVariableCount();
}
inline Void GPUShaderReflectConstantBuffer::GetVariable( GPUShaderReflectVariable * outVariable, UInt iIndex ) const {
    m_hDeviceShaderReflectConstantBuffer.GetVariable( &(outVariable->m_hDeviceShaderReflectVariable), iIndex );
}
inline Void GPUShaderReflectConstantBuffer::GetVariable( GPUShaderReflectVariable * outVariable, const AChar * strName ) const {
    m_hDeviceShaderReflectConstantBuffer.GetVariable( &(outVariable->m_hDeviceShaderReflectVariable), strName );
}

/////////////////////////////////////////////////////////////////////////////////
// GPUShaderReflectVariable implementation
inline Void GPUShaderReflectVariable::GetDesc( GPUShaderVariableDesc * outDesc ) const {
    m_hDeviceShaderReflectVariable.GetDesc( (DeviceShaderVariableDesc*)outDesc );
}

inline UInt GPUShaderReflectVariable::GetInterfaceSlot( UInt iArrayIndex ) const {
    return m_hDeviceShaderReflectVariable.GetInterfaceSlot( iArrayIndex );
}

inline Void GPUShaderReflectVariable::GetConstantBuffer( GPUShaderReflectConstantBuffer * outConstantBuffer ) const {
    m_hDeviceShaderReflectVariable.GetConstantBuffer( &(outConstantBuffer->m_hDeviceShaderReflectConstantBuffer) );
}
inline Void GPUShaderReflectVariable::GetType( GPUShaderReflectType * outType ) const {
    m_hDeviceShaderReflectVariable.GetType( &(outType->m_hDeviceShaderReflectType) );
}

/////////////////////////////////////////////////////////////////////////////////
// GPUShaderReflectType implementation
inline Void GPUShaderReflectType::GetDesc( GPUShaderTypeDesc * outDesc ) const {
    m_hDeviceShaderReflectType.GetDesc( (DeviceShaderTypeDesc*)outDesc );
}

inline Bool GPUShaderReflectType::IsEqual( const GPUShaderReflectType * pType ) const {
    return m_hDeviceShaderReflectType.IsEqual( &(pType->m_hDeviceShaderReflectType) );
}
inline Bool GPUShaderReflectType::InheritsFrom( const GPUShaderReflectType * pBaseType ) const {
    return m_hDeviceShaderReflectType.InheritsFrom( &(pBaseType->m_hDeviceShaderReflectType) );
}
inline Bool GPUShaderReflectType::ImplementsInterface( const GPUShaderReflectType * pInterface ) const {
    return m_hDeviceShaderReflectType.ImplementsInterface( &(pInterface->m_hDeviceShaderReflectType) );
}

inline Void GPUShaderReflectType::GetBaseClass( GPUShaderReflectType * outBaseClass ) const {
    m_hDeviceShaderReflectType.GetBaseClass( &(outBaseClass->m_hDeviceShaderReflectType) );
}
inline Void GPUShaderReflectType::GetVariableBaseClass( GPUShaderReflectType * outBaseClass ) const {
    m_hDeviceShaderReflectType.GetVariableBaseClass( &(outBaseClass->m_hDeviceShaderReflectType) );
}

inline UInt GPUShaderReflectType::GetInterfaceCount() const {
    return m_hDeviceShaderReflectType.GetInterfaceCount();
}
inline Void GPUShaderReflectType::GetInterface( GPUShaderReflectType * outInterface, UInt iIndex ) const {
    m_hDeviceShaderReflectType.GetInterface( &(outInterface->m_hDeviceShaderReflectType), iIndex );
}

inline UInt GPUShaderReflectType::GetMemberTypeCount() const {
    return m_hDeviceShaderReflectType.GetMemberTypeCount();
}
inline const AChar * GPUShaderReflectType::GetMemberTypeName( UInt iIndex ) const {
    return m_hDeviceShaderReflectType.GetMemberTypeName( iIndex );
}
inline Void GPUShaderReflectType::GetMemberType( GPUShaderReflectType * outMember, UInt iIndex ) const {
    m_hDeviceShaderReflectType.GetMemberType( &(outMember->m_hDeviceShaderReflectType), iIndex );
}
inline Void GPUShaderReflectType::GetMemberType( GPUShaderReflectType * outMember, const AChar * strName ) const {
    m_hDeviceShaderReflectType.GetMemberType( &(outMember->m_hDeviceShaderReflectType), strName );
}

/////////////////////////////////////////////////////////////////////////////////
// GPUShader implementation
inline Bool GPUShader::IsBound() const {
    return m_pDeviceShader->IsCreated();
}

inline GPUShaderType GPUShader::GetType() const {
    return (GPUShaderType)( m_pDeviceShader->GetType() );
}

inline Bool GPUShader::HasReflection() const {
    Assert( IsBound() );
    return m_pDeviceShader->HasReflection();
}

inline Void GPUShader::GetDesc( GPUShaderDesc * outDesc ) const {
    Assert( HasReflection() );
    m_pDeviceShader->GetDesc( (DeviceShaderDesc*)outDesc );
}

inline UInt GPUShader::GetInputParameterCount() const {
    Assert( HasReflection() );
    return m_pDeviceShader->GetInputParameterCount();
}
inline Void GPUShader::GetInputParameterDesc( GPUShaderParameterDesc * outInputParameterDesc, UInt iIndex ) const {
    Assert( HasReflection() );
    m_pDeviceShader->GetInputParameterDesc( (DeviceShaderParameterDesc*)outInputParameterDesc, iIndex );
}

inline UInt GPUShader::GetOutputParameterCount() const {
    Assert( HasReflection() );
    return m_pDeviceShader->GetOutputParameterCount();
}
inline Void GPUShader::GetOutputParameterDesc( GPUShaderParameterDesc * outOutputParameterDesc, UInt iIndex ) const {
    Assert( HasReflection() );
    m_pDeviceShader->GetOutputParameterDesc( (DeviceShaderParameterDesc*)outOutputParameterDesc, iIndex );
}

inline UInt GPUShader::GetPatchConstantParameterCount() const {
    Assert( HasReflection() );
    return m_pDeviceShader->GetPatchConstantParameterCount();
}
inline Void GPUShader::GetPatchConstantParameterDesc( GPUShaderParameterDesc * outPatchConstantParameterDesc, UInt iIndex ) const {
    Assert( HasReflection() );
    m_pDeviceShader->GetPatchConstantParameterDesc( (DeviceShaderParameterDesc*)outPatchConstantParameterDesc, iIndex );
}

inline UInt GPUShader::GetBindingCount() const {
    Assert( HasReflection() );
    return m_pDeviceShader->GetBindingCount();
}
inline Void GPUShader::GetBindingDesc( GPUShaderBindingDesc * outBindingDesc, UInt iIndex ) const {
    Assert( HasReflection() );
    m_pDeviceShader->GetBindingDesc( (DeviceShaderBindingDesc*)outBindingDesc, iIndex );
}
inline Void GPUShader::GetBindingDesc( GPUShaderBindingDesc * outBindingDesc, const AChar * strName ) const {
    Assert( HasReflection() );
    m_pDeviceShader->GetBindingDesc( (DeviceShaderBindingDesc*)outBindingDesc, strName );
}

inline UInt GPUShader::GetConstantBufferCount() const {
    Assert( HasReflection() );
    return m_pDeviceShader->GetConstantBufferCount();
}
inline Void GPUShader::GetConstantBuffer( GPUShaderReflectConstantBuffer * outConstantBuffer, UInt iIndex ) const {
    Assert( HasReflection() );
    m_pDeviceShader->GetConstantBuffer( &(outConstantBuffer->m_hDeviceShaderReflectConstantBuffer), iIndex );
}
inline Void GPUShader::GetConstantBuffer( GPUShaderReflectConstantBuffer * outConstantBuffer, const AChar * strName ) const {
    Assert( HasReflection() );
    m_pDeviceShader->GetConstantBuffer( &(outConstantBuffer->m_hDeviceShaderReflectConstantBuffer), strName );
}

inline Void GPUShader::GetVariable( GPUShaderReflectVariable * outVariable, const AChar * strName ) const {
    Assert( HasReflection() );
    m_pDeviceShader->GetVariable( &(outVariable->m_hDeviceShaderReflectVariable), strName );
}

inline UInt GPUShader::GetInterfaceSlotCount() const {
    Assert( HasReflection() );
    return m_pDeviceShader->GetInterfaceSlotCount();
}

inline Void GPUShader::GetThreadGroupSize( UInt * outSizeX, UInt * outSizeY, UInt * outSizeZ ) const {
    Assert( HasReflection() );
    m_pDeviceShader->GetThreadGroupSize( outSizeX, outSizeY, outSizeZ );
}

inline Bool GPUShader::HasDynamicLinkage() const {
    Assert( HasReflection() );
    return m_pDeviceShader->HasDynamicLinkage();
}
inline Void GPUShader::AddDynamicLink( const AChar * strInterfaceInstance, const AChar * strClassInstance ) {
    Assert( HasDynamicLinkage() );
    m_pDeviceShader->AddDynamicLink( strInterfaceInstance, strClassInstance );
}

/////////////////////////////////////////////////////////////////////////////////
// GPUVertexShader implementation

/////////////////////////////////////////////////////////////////////////////////
// GPUGeometryShader implementation
inline Bool GPUGeometryShader::IsFinalized() const {
    return m_hDeviceGeometryShader.IsFinalized();
}

inline Void GPUGeometryShader::PushSODeclaration( const GPUStreamOutputDeclaration * pSODeclaration ) {
    Assert( !(IsFinalized()) );
    m_hDeviceGeometryShader.PushSODeclaration( (const DeviceStreamOutputDeclaration *)pSODeclaration );
}
inline Void GPUGeometryShader::Finalize( const UInt * arrStrides, UInt iStrideCount, UInt iRasterStream ) {
    Assert( !(IsFinalized()) );
    m_hDeviceGeometryShader.Finalize( arrStrides, iStrideCount, iRasterStream );
}

inline UInt GPUGeometryShader::GetSODeclarationCount() const {
    return m_hDeviceGeometryShader.GetSODeclarationCount();
}
inline Void GPUGeometryShader::GetSODeclaration( GPUStreamOutputDeclaration * outSODeclaration, UInt iSODeclaration ) const {
    m_hDeviceGeometryShader.GetSODeclaration( (DeviceStreamOutputDeclaration*)outSODeclaration, iSODeclaration );
}

inline UInt GPUGeometryShader::GetStrideCount() const {
    return m_hDeviceGeometryShader.GetStrideCount();
}
inline UInt GPUGeometryShader::GetStride( UInt iStreamOutputSlot ) const {
    return m_hDeviceGeometryShader.GetStride( iStreamOutputSlot );
}

inline UInt GPUGeometryShader::GetRasterStream() const {
    return m_hDeviceGeometryShader.GetRasterStream();
}

/////////////////////////////////////////////////////////////////////////////////
// GPUPixelShader implementation

/////////////////////////////////////////////////////////////////////////////////
// GPUHullShader implementation

/////////////////////////////////////////////////////////////////////////////////
// GPUDomainShader implementation

/////////////////////////////////////////////////////////////////////////////////
// GPUComputeShader implementation

