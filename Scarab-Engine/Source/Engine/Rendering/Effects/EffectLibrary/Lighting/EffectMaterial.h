/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Effects/EffectLibrary/Lighting/EffectMaterial.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Lighting effect : Material
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
#ifndef SCARAB_ENGINE_RENDERING_EFFECTS_EFFECTLIBRARY_LIGHTING_EFFECTMATERIAL_H
#define SCARAB_ENGINE_RENDERING_EFFECTS_EFFECTLIBRARY_LIGHTING_EFFECTMATERIAL_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../EffectInstance.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Input layout
typedef struct _effect_material_vertex {
    Vertex4 vPosition;
} EffectMaterialVertex;

// Prototypes
class EffectMaterial;
class EffectMaterialInstance;

/////////////////////////////////////////////////////////////////////////////////
// The EffectMaterial class
class EffectMaterial : public Effect
{
public:
    EffectMaterial();
    virtual ~EffectMaterial();

    EffectMaterialInstance * CreateInstance( Material * pMaterial ) const;

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
// The EffectMaterialInstance class
class EffectMaterialInstance : public EffectInstance
{
protected:
    friend class Effect;
    friend class EffectMaterial;
    EffectMaterialInstance( const EffectMaterial * pEffectMaterial, Material * pMaterial );
    virtual ~EffectMaterialInstance();

public:
    // Methods
    virtual Void UpdateConstants( UInt iPass, const Transform3 * pWorldTransform, const Camera * pCamera, GPUDeferredContext * pContext = NULL ) const;

protected:
    Material * m_pMaterial;

    EffectParameters m_hParameters;
    EffectShaderParameters m_hVSParameters;

    GPUConstantBuffer * m_pVSConstants;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "EffectMaterial.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RENDERING_EFFECTS_EFFECTLIBRARY_LIGHTING_EFFECTMATERIAL_H
