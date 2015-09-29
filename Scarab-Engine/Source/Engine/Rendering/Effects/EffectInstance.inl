/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Effects/EffectInstance.inl
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
// EffectShaderParameters implementation
inline GPUShader * EffectShaderParameters::GetShader() const {
    return m_pShader;
}

inline UInt EffectShaderParameters::GetConstantBufferCount() const {
    return m_iConstantBufferCount;
}
inline GPUConstantBuffer * EffectShaderParameters::GetConstantBuffer( UInt iConstantBuffer ) const {
    Assert( iConstantBuffer < m_iConstantBufferCount );
    return m_arrConstantBuffers[iConstantBuffer];
}
inline GPUConstantBuffer ** EffectShaderParameters::GetConstantBuffers( UInt iConstantBuffer ) {
    Assert( iConstantBuffer < m_iConstantBufferCount );
    return ( m_arrConstantBuffers + iConstantBuffer );
}

inline UInt EffectShaderParameters::GetSamplerCount() const {
    return m_iSamplerCount;
}
inline GPUSamplerState * EffectShaderParameters::GetSampler( UInt iSampler ) const {
    Assert( iSampler < m_iSamplerCount );
    return m_arrSamplers[iSampler];
}
inline GPUSamplerState ** EffectShaderParameters::GetSamplers( UInt iSampler ) {
    Assert( iSampler < m_iSamplerCount );
    return ( m_arrSamplers + iSampler );
}

inline UInt EffectShaderParameters::GetResourceCount() const {
    return m_iResourceCount;
}
inline GPUShaderView * EffectShaderParameters::GetResource( UInt iResource ) const {
    Assert( iResource < m_iResourceCount );
    return m_arrResources[iResource];
}
inline GPUShaderView ** EffectShaderParameters::GetResources( UInt iResource ) {
    Assert( iResource < m_iResourceCount );
    return ( m_arrResources + iResource );
}

/////////////////////////////////////////////////////////////////////////////////
// EffectParameters implementation
inline EffectShaderParameters * EffectParameters::GetShaderParameters( GPUShaderType iType ) const {
    switch( iType ) {
        case GPUSHADER_VERTEX:   return m_pVSParameters;
        case GPUSHADER_GEOMETRY: return m_pGSParameters;
        case GPUSHADER_PIXEL:    return m_pPSParameters;
        case GPUSHADER_HULL:     return m_pHSParameters;
        case GPUSHADER_DOMAIN:   return m_pDSParameters;
        default: Assert( false ); break;
    }
}

inline DWord EffectParameters::GetStencilRef() const {
    return m_iStencilRef;
}
inline Void EffectParameters::SetStencilRef( DWord iStencilRef ) {
    m_iStencilRef = iStencilRef;
}

inline const Float * EffectParameters::GetBlendFactors() const {
    return m_arrBlendFactors;
}
inline Void EffectParameters::SetBlendFactors( const Float arrBlendFactors[4] ) {
    m_arrBlendFactors[0] = arrBlendFactors[0];
    m_arrBlendFactors[1] = arrBlendFactors[1];
    m_arrBlendFactors[2] = arrBlendFactors[2];
    m_arrBlendFactors[3] = arrBlendFactors[3];
}

inline DWord EffectParameters::GetBlendSampleMask() const {
    return m_iBlendSampleMask;
}
inline Void EffectParameters::SetBlendSampleMask( DWord iBlendSampleMask ) {
    m_iBlendSampleMask = iBlendSampleMask;
}

inline UInt EffectParameters::GetOutputBufferCount() const {
    return m_iOutputBufferCount;
}
inline GPUBuffer * EffectParameters::GetOutputBuffer( UInt iOutputBuffer ) const {
    Assert( iOutputBuffer < m_iOutputBufferCount );
    return m_arrOutputBuffers[iOutputBuffer];
}
inline UInt EffectParameters::GetOutputBufferOffset( UInt iOutputBuffer ) const {
    Assert( iOutputBuffer < m_iOutputBufferCount );
    return m_arrOutputBufferOffsets[iOutputBuffer];
}
inline GPUBuffer ** EffectParameters::GetOutputBuffers( UInt iOutputBuffer ) {
    Assert( iOutputBuffer < m_iOutputBufferCount );
    return ( m_arrOutputBuffers + iOutputBuffer );
}
inline UInt * EffectParameters::GetOutputBufferOffsets( UInt iOutputBuffer ) {
    Assert( iOutputBuffer < m_iOutputBufferCount );
    return ( m_arrOutputBufferOffsets + iOutputBuffer );
}

/////////////////////////////////////////////////////////////////////////////////
// EffectInstance implementation
inline const Effect * EffectInstance::GetEffect() const {
    return m_pEffect;
}

inline UInt EffectInstance::GetTechniqueIndex() const {
    return m_iTechniqueIndex;
}
inline const EffectTechnique * EffectInstance::GetTechnique() const {
    return m_pTechnique;
}

inline UInt EffectInstance::GetPassCount() const {
    return m_iPassCount;
}
inline const EffectPass * EffectInstance::GetPass( UInt iPass ) const {
    return m_pTechnique->GetPass( iPass );
}

inline EffectParameters * EffectInstance::GetParameters( UInt iPass ) const {
    Assert( iPass < m_iPassCount );
    return m_arrParameters[iPass];
}

