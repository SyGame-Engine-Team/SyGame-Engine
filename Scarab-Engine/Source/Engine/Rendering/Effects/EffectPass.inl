/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Effects/EffectPass.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Elementary shader-pass abstraction for the effect system.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// EffectPass implementation
inline Void EffectPass::SetRasterizerState( GPURasterizerState * pState ) {
    m_pRasterizerState = pState;
}
inline Void EffectPass::SetDepthStencilState( GPUDepthStencilState * pState ) {
    m_pDepthStencilState = pState;
}
inline Void EffectPass::SetBlendState( GPUBlendState * pState ) {
    m_pBlendState = pState;
}

inline Void EffectPass::SetInputLayout( GPUInputLayout * pIL ) {
    m_pIL = pIL;
}

inline Void EffectPass::SetVertexShader( GPUVertexShader * pVS ) {
    m_pVS = pVS;
}
inline Void EffectPass::SetGeometryShader( GPUGeometryShader * pGS ) {
    m_pGS = pGS;
}
inline Void EffectPass::SetPixelShader( GPUPixelShader * pPS ) {
    m_pPS = pPS;
}
inline Void EffectPass::SetHullShader( GPUHullShader * pHS ) {
    m_pHS = pHS;
}
inline Void EffectPass::SetDomainShader( GPUDomainShader * pDS ) {
    m_pDS = pDS;
}

inline GPURasterizerState * EffectPass::GetRasterizerState() const {
    return m_pRasterizerState;
}
inline GPUDepthStencilState * EffectPass::GetDepthStencilState() const {
    return m_pDepthStencilState;
}
inline GPUBlendState * EffectPass::GetBlendState() const {
    return m_pBlendState;
}

inline GPUInputLayout * EffectPass::GetInputLayout() const {
    return m_pIL;
}

inline GPUVertexShader * EffectPass::GetVertexShader() const {
    return m_pVS;
}
inline GPUGeometryShader * EffectPass::GetGeometryShader() const {
    return m_pGS;
}
inline GPUPixelShader * EffectPass::GetPixelShader() const {
    return m_pPS;
}
inline GPUHullShader * EffectPass::GetHullShader() const {
    return m_pHS;
}
inline GPUDomainShader * EffectPass::GetDomainShader() const {
    return m_pDS;
}


