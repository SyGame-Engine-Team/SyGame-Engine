/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/Resources/D3D11Shader.inl
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
// D3D11ShaderCompiler implementation
inline Bool D3D11ShaderCompiler::IsCompiled() const {
    return ( m_pByteCode != NULL );
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11ShaderReflectConstantBuffer implementation

/////////////////////////////////////////////////////////////////////////////////
// D3D11ShaderReflectVariable implementation

/////////////////////////////////////////////////////////////////////////////////
// D3D11ShaderReflectType implementation

/////////////////////////////////////////////////////////////////////////////////
// D3D11Shader implementation
inline Bool D3D11Shader::IsCreated() const {
    return ( m_pShader != NULL || m_bTemporaryDestroyed );
}

inline const D3D11ShaderCompiler * D3D11Shader::GetCompiledShader() const {
    DebugAssert( IsCreated() );
    return m_pCompiledShader;
}

inline Bool D3D11Shader::HasReflection() const {
    DebugAssert( IsCreated() );
    return m_bHasReflection;
}

inline Bool D3D11Shader::HasDynamicLinkage() const {
    DebugAssert( HasReflection() );
    return m_bHasDynamicLinkage;
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11VertexShader implementation
inline D3D11ShaderType D3D11VertexShader::GetType() const {
    return D3D11SHADER_VERTEX;
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11GeometryShader implementation
inline Bool D3D11GeometryShader::IsFinalized() const {
    return m_bFinalized;
}

inline D3D11ShaderType D3D11GeometryShader::GetType() const {
    return D3D11SHADER_GEOMETRY;
}

inline UInt D3D11GeometryShader::GetSODeclarationCount() const {
    return m_iSODeclarationCount;
}
inline Void D3D11GeometryShader::GetSODeclaration( D3D11StreamOutputDeclaration * outSODeclaration, UInt iSODeclaration ) const {
    DebugAssert( iSODeclaration < m_iSODeclarationCount );
    outSODeclaration->ConvertFrom( m_arrSODeclarations + iSODeclaration );
}

inline UInt D3D11GeometryShader::GetStrideCount() const {
    return m_iStrideCount;
}
inline UInt D3D11GeometryShader::GetStride( UInt iStreamOutputSlot ) const {
    DebugAssert( iStreamOutputSlot < m_iStrideCount );
    return m_arrStrides[iStreamOutputSlot];
}

inline UInt D3D11GeometryShader::GetRasterStream() const {
    return m_iRasterStream;
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11PixelShader implementation
inline D3D11ShaderType D3D11PixelShader::GetType() const {
    return D3D11SHADER_PIXEL;
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11HullShader implementation
inline D3D11ShaderType D3D11HullShader::GetType() const {
    return D3D11SHADER_HULL;
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11DomainShader implementation
inline D3D11ShaderType D3D11DomainShader::GetType() const {
    return D3D11SHADER_DOMAIN;
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11ComputeShader implementation
inline D3D11ShaderType D3D11ComputeShader::GetType() const {
    return D3D11SHADER_COMPUTE;
}

