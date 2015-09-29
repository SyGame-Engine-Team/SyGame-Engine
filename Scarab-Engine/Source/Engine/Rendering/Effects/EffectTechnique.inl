/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Effects/EffectTechnique.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Multi-pass chaining for the effect system.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// EffectTechnique implementation
inline UInt EffectTechnique::GetPassCount() const {
    return m_iPassCount;
}
inline EffectPass * EffectTechnique::GetPass( UInt iPass ) const {
    Assert( iPass < m_iPassCount );
    return m_arrPasses[iPass];
}

inline GPURasterizerState * EffectTechnique::GetRasterizerState( UInt iPass ) const {
    Assert( iPass < m_iPassCount );
    return m_arrPasses[iPass]->GetRasterizerState();
}
inline GPUDepthStencilState * EffectTechnique::GetDepthStencilState( UInt iPass ) const {
    Assert( iPass < m_iPassCount );
    return m_arrPasses[iPass]->GetDepthStencilState();
}
inline GPUBlendState * EffectTechnique::GetBlendState( UInt iPass ) const {
    Assert( iPass < m_iPassCount );
    return m_arrPasses[iPass]->GetBlendState();
}

inline GPUInputLayout * EffectTechnique::GetInputLayout( UInt iPass ) const {
    Assert( iPass < m_iPassCount );
    return m_arrPasses[iPass]->GetInputLayout();
}

inline GPUVertexShader * EffectTechnique::GetVertexShader( UInt iPass ) const {
    Assert( iPass < m_iPassCount );
    return m_arrPasses[iPass]->GetVertexShader();
}
inline GPUGeometryShader * EffectTechnique::GetGeometryShader( UInt iPass ) const {
    Assert( iPass < m_iPassCount );
    return m_arrPasses[iPass]->GetGeometryShader();
}
inline GPUPixelShader * EffectTechnique::GetPixelShader( UInt iPass ) const {
    Assert( iPass < m_iPassCount );
    return m_arrPasses[iPass]->GetPixelShader();
}
inline GPUHullShader * EffectTechnique::GetHullShader( UInt iPass ) const {
    Assert( iPass < m_iPassCount );
    return m_arrPasses[iPass]->GetHullShader();
}
inline GPUDomainShader * EffectTechnique::GetDomainShader( UInt iPass ) const {
    Assert( iPass < m_iPassCount );
    return m_arrPasses[iPass]->GetDomainShader();
}

