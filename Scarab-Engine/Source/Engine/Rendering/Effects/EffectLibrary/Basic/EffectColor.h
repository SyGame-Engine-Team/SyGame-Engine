/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Effects/EffectLibrary/Basic/EffectColor.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Basic effect : Color
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
#ifndef SCARAB_ENGINE_RENDERING_EFFECTS_EFFECTLIBRARY_BASIC_EFFECTCOLOR_H
#define SCARAB_ENGINE_RENDERING_EFFECTS_EFFECTLIBRARY_BASIC_EFFECTCOLOR_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../EffectInstance.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Input layout
typedef struct _effect_color_vertex {
    Vertex4 vPosition;
    Color4 vColor;
} EffectColorVertex;

// Prototypes
class EffectColor;
class EffectColorInstance;

/////////////////////////////////////////////////////////////////////////////////
// The EffectColor class
class EffectColor : public Effect
{
public:
    EffectColor();
    virtual ~EffectColor();

    // Methods
    EffectColorInstance * CreateInstance() const;

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
// The EffectColorInstance class
class EffectColorInstance : public EffectInstance
{
protected:
    friend class Effect;
    friend class EffectColor;
    EffectColorInstance( const EffectColor * pEffectColor );
    virtual ~EffectColorInstance();

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
#include "EffectColor.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RENDERING_EFFECTS_EFFECTLIBRARY_BASIC_EFFECTCOLOR_H
