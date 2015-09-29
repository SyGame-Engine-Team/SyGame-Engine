/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Effects/EffectTechnique.h
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
// Header prelude
#ifndef SCARAB_ENGINE_RENDERING_EFFECTS_EFFECTTECHNIQUE_H
#define SCARAB_ENGINE_RENDERING_EFFECTS_EFFECTTECHNIQUE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "EffectPass.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define EFFECT_TECHNIQUE_MAX_PASSES 64

/////////////////////////////////////////////////////////////////////////////////
// The EffectTechnique class
class EffectTechnique
{
public:
    EffectTechnique();
    virtual ~EffectTechnique();

    // Construction
    Void AddPass( EffectPass * pPass );
    EffectPass * RemovePass( UInt iPass );
    EffectPass * SetPass( UInt iPass, EffectPass * pPass );

    // Getters
    inline UInt GetPassCount() const;
    inline EffectPass * GetPass( UInt iPass ) const;

    inline GPURasterizerState * GetRasterizerState( UInt iPass ) const;
    inline GPUDepthStencilState * GetDepthStencilState( UInt iPass ) const;
    inline GPUBlendState * GetBlendState( UInt iPass ) const;

    inline GPUInputLayout * GetInputLayout( UInt iPass ) const;

    inline GPUVertexShader * GetVertexShader( UInt iPass ) const;
    inline GPUGeometryShader * GetGeometryShader( UInt iPass ) const;
    inline GPUPixelShader * GetPixelShader( UInt iPass ) const;
    inline GPUHullShader * GetHullShader( UInt iPass ) const;
    inline GPUDomainShader * GetDomainShader( UInt iPass ) const;

protected:
    UInt m_iPassCount;
    EffectPass * m_arrPasses[EFFECT_TECHNIQUE_MAX_PASSES];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "EffectTechnique.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RENDERING_EFFECTS_EFFECTTECHNIQUE_H
