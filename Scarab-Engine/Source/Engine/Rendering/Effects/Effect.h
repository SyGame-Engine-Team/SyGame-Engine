/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Effects/Effect.h
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
// Header prelude
#ifndef SCARAB_ENGINE_RENDERING_EFFECTS_EFFECT_H
#define SCARAB_ENGINE_RENDERING_EFFECTS_EFFECT_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "EffectTechnique.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define EFFECT_MAX_TECHNIQUES 64

// Prototypes
class EffectInstance;

/////////////////////////////////////////////////////////////////////////////////
// The Effect class
class Effect
{
protected:
    Effect();
public:
    virtual ~Effect();

    // Construction
    Void AddTechnique( EffectTechnique * pTechnique );
    EffectTechnique * RemoveTechnique( UInt iTechnique );
    EffectTechnique * SetTechnique( UInt iTechnique, EffectTechnique * pTechnique );

    // Getters
    inline UInt GetTechniqueCount() const;
    inline EffectTechnique * GetTechnique( UInt iTechnique ) const;

    inline UInt GetPassCount( UInt iTechnique ) const;
    inline EffectPass * GetPass( UInt iTechnique, UInt iPass ) const;

    inline GPURasterizerState * GetRasterizerState( UInt iTechnique, UInt iPass ) const;
    inline GPUDepthStencilState * GetDepthStencilState( UInt iTechnique, UInt iPass ) const;
    inline GPUBlendState * GetBlendState( UInt iTechnique, UInt iPass ) const;

    inline GPUInputLayout * GetInputLayout( UInt iTechnique, UInt iPass ) const;

    inline GPUVertexShader * GetVertexShader( UInt iTechnique, UInt iPass ) const;
    inline GPUGeometryShader * GetGeometryShader( UInt iTechnique, UInt iPass ) const;
    inline GPUPixelShader * GetPixelShader( UInt iTechnique, UInt iPass ) const;
    inline GPUHullShader * GetHullShader( UInt iTechnique, UInt iPass ) const;
    inline GPUDomainShader * GetDomainShader( UInt iTechnique, UInt iPass ) const;

    // Methods
    static Void DestroyInstance( EffectInstance * pInstance );

protected:
    UInt m_iTechniqueCount;
    EffectTechnique * m_arrTechniques[EFFECT_MAX_TECHNIQUES];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Effect.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RENDERING_EFFECTS_EFFECT_H
