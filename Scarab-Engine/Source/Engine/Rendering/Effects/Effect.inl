/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Effects/Effect.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : The fundamental type for the effect system.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Effect implementation
inline UInt Effect::GetTechniqueCount() const {
    return m_iTechniqueCount;
}
inline EffectTechnique * Effect::GetTechnique( UInt iTechnique ) const {
    Assert( iTechnique < m_iTechniqueCount );
    return m_arrTechniques[iTechnique];
}

inline UInt Effect::GetPassCount( UInt iTechnique ) const {
    Assert( iTechnique < m_iTechniqueCount );
    return m_arrTechniques[iTechnique]->GetPassCount();
}
inline EffectPass * Effect::GetPass( UInt iTechnique, UInt iPass ) const {
    Assert( iTechnique < m_iTechniqueCount );
    return m_arrTechniques[iTechnique]->GetPass( iPass );
}

inline GPURasterizerState * Effect::GetRasterizerState( UInt iTechnique, UInt iPass ) const {
    Assert( iTechnique < m_iTechniqueCount );
    return m_arrTechniques[iTechnique]->GetRasterizerState( iPass );
}
inline GPUDepthStencilState * Effect::GetDepthStencilState( UInt iTechnique, UInt iPass ) const {
    Assert( iTechnique < m_iTechniqueCount );
    return m_arrTechniques[iTechnique]->GetDepthStencilState( iPass );
}
inline GPUBlendState * Effect::GetBlendState( UInt iTechnique, UInt iPass ) const {
    Assert( iTechnique < m_iTechniqueCount );
    return m_arrTechniques[iTechnique]->GetBlendState( iPass );
}

inline GPUInputLayout * Effect::GetInputLayout( UInt iTechnique, UInt iPass ) const {
    Assert( iTechnique < m_iTechniqueCount );
    return m_arrTechniques[iTechnique]->GetInputLayout( iPass );
}

inline GPUVertexShader * Effect::GetVertexShader( UInt iTechnique, UInt iPass ) const {
    Assert( iTechnique < m_iTechniqueCount );
    return m_arrTechniques[iTechnique]->GetVertexShader( iPass );
}
inline GPUGeometryShader * Effect::GetGeometryShader( UInt iTechnique, UInt iPass ) const {
    Assert( iTechnique < m_iTechniqueCount );
    return m_arrTechniques[iTechnique]->GetGeometryShader( iPass );
}
inline GPUPixelShader * Effect::GetPixelShader( UInt iTechnique, UInt iPass ) const {
    Assert( iTechnique < m_iTechniqueCount );
    return m_arrTechniques[iTechnique]->GetPixelShader( iPass );
}
inline GPUHullShader * Effect::GetHullShader( UInt iTechnique, UInt iPass ) const {
    Assert( iTechnique < m_iTechniqueCount );
    return m_arrTechniques[iTechnique]->GetHullShader( iPass );
}
inline GPUDomainShader * Effect::GetDomainShader( UInt iTechnique, UInt iPass ) const {
    Assert( iTechnique < m_iTechniqueCount );
    return m_arrTechniques[iTechnique]->GetDomainShader( iPass );
}

