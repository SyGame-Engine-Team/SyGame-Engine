/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Effects/EffectLibrary/EffectDefault.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : The default effect to fallback to if another can't be used.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : Fallback to this when no effect can apply, renders as hard
//              magenta for you to notice easily ...
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_RENDERING_EFFECTS_EFFECTLIBRARY_EFFECTDEFAULT_H
#define SCARAB_ENGINE_RENDERING_EFFECTS_EFFECTLIBRARY_EFFECTDEFAULT_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../EffectInstance.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Input layout
typedef struct _effect_default_vertex {
    Vertex4 vPosition;
} EffectDefaultVertex;

// Prototypes
class EffectDefault;
class EffectDefaultInstance;

/////////////////////////////////////////////////////////////////////////////////
// The EffectDefault class
class EffectDefault : public Effect
{
public:
    EffectDefault();
    virtual ~EffectDefault();

    // Methods
    EffectDefaultInstance * CreateInstance() const;

protected:
    static const AChar * sm_strHLSLCode;
    static const UInt sm_iHLSLCodeLength;

    GPURasterizerState m_hRasterizerState;
    GPUDepthStencilState m_hDepthStencilState;
    GPUBlendState m_hBlendState;
    
    GPUInputLayout m_hIL;

    GPUShaderCompiler m_hCompiledVS;
    GPUShaderCompiler m_hCompiledPS;

    GPUVertexShader m_hVS;
    GPUPixelShader m_hPS;
    
    EffectPass m_hPass;
    EffectTechnique m_hTechnique;
};

/////////////////////////////////////////////////////////////////////////////////
// The EffectDefaultInstance class
class EffectDefaultInstance : public EffectInstance
{
protected:
    friend class Effect;
    friend class EffectDefault;
    EffectDefaultInstance( const EffectDefault * pEffectDefault );
    virtual ~EffectDefaultInstance();

public:
    // Methods
    virtual Void UpdateConstants( UInt iPass, const Transform3 * pWorldTransform, const Camera * pCamera, GPUDeferredContext * pContext = NULL ) const;

protected:
    EffectParameters m_hParameters;
    EffectShaderParameters m_hVSParameters;

    GPUConstantBuffer * m_pVSConstants;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "EffectDefault.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RENDERING_EFFECTS_EFFECTLIBRARY_EFFECTDEFAULT_H
