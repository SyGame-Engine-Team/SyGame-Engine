/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Effects/EffectPass.h
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
// Header prelude
#ifndef SCARAB_ENGINE_RENDERING_EFFECTS_EFFECTPASS_H
#define SCARAB_ENGINE_RENDERING_EFFECTS_EFFECTPASS_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Resources/GPURenderState.h"
#include "../Resources/GPUInputLayout.h"

#include "../Resources/GPUShader.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The EffectPass class
class EffectPass
{
public:
    EffectPass();
    virtual ~EffectPass();

    // Construction
    inline Void SetRasterizerState( GPURasterizerState * pState );
    inline Void SetDepthStencilState( GPUDepthStencilState * pState );
    inline Void SetBlendState( GPUBlendState * pState );

    inline Void SetInputLayout( GPUInputLayout * pIL );

    inline Void SetVertexShader( GPUVertexShader * pVS );
    inline Void SetGeometryShader( GPUGeometryShader * pGS );
    inline Void SetPixelShader( GPUPixelShader * pPS );
    inline Void SetHullShader( GPUHullShader * pHS );
    inline Void SetDomainShader( GPUDomainShader * pDS );

    // Getters
    inline GPURasterizerState * GetRasterizerState() const;
    inline GPUDepthStencilState * GetDepthStencilState() const;
    inline GPUBlendState * GetBlendState() const;

    inline GPUInputLayout * GetInputLayout() const;

    inline GPUVertexShader * GetVertexShader() const;
    inline GPUGeometryShader * GetGeometryShader() const;
    inline GPUPixelShader * GetPixelShader() const;
    inline GPUHullShader * GetHullShader() const;
    inline GPUDomainShader * GetDomainShader() const;

protected:
    GPURasterizerState * m_pRasterizerState;
    GPUDepthStencilState * m_pDepthStencilState;
    GPUBlendState * m_pBlendState;

    GPUInputLayout * m_pIL;

    GPUVertexShader * m_pVS;
    GPUGeometryShader * m_pGS;
    GPUPixelShader * m_pPS;
    GPUHullShader * m_pHS;
    GPUDomainShader * m_pDS;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "EffectPass.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RENDERING_EFFECTS_EFFECTPASS_H
