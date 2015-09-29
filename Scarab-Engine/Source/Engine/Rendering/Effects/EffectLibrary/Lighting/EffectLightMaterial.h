/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Effects/EffectLibrary/Lighting/EffectLightMaterial.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Lighting effect : Lighting + Material
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
#ifndef SCARAB_ENGINE_RENDERING_EFFECTS_EFFECTLIBRARY_LIGHTING_EFFECTLIGHTMATERIAL_H
#define SCARAB_ENGINE_RENDERING_EFFECTS_EFFECTLIBRARY_LIGHTING_EFFECTLIGHTMATERIAL_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../EffectInstance.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Lighting techniques
#ifndef _EFFECTLIGHTTECHNIQUE_DEFINED
#define _EFFECTLIGHTTECHNIQUE_DEFINED
enum EffectLightTechnique {
    EFFECTLIGHT_TECHNIQUE_POINT_AMBIANT = 0,
    EFFECTLIGHT_TECHNIQUE_POINT_GOURAUD,
    EFFECTLIGHT_TECHNIQUE_POINT_PHONG,

    EFFECTLIGHT_TECHNIQUE_COUNT
};
#endif // _EFFECTLIGHTTECHNIQUE_DEFINED

// Input layout
typedef struct _effect_lightmaterialambiant_vertex {
    Vertex4 vPosition;
} EffectLightMaterialAmbiantVertex;

typedef struct _effect_lightmaterialgouraud_vertex {
    Vertex4 vPosition;
    Vector4 vNormal;
} EffectLightMaterialGouraudVertex;

typedef struct _effect_lightmaterialphong_vertex {
    Vertex4 vPosition;
    Vector4 vNormal;
} EffectLightMaterialPhongVertex;

// Prototypes
class EffectLightMaterial;
class EffectLightMaterialInstance;

/////////////////////////////////////////////////////////////////////////////////
// The EffectLightMaterial class
class EffectLightMaterial : public Effect
{
public:
    EffectLightMaterial();
    virtual ~EffectLightMaterial();

    EffectLightMaterialInstance * CreateInstance( EffectLightTechnique iTechnique, Light * pLight, Material * pMaterial ) const;

protected:
    static const AChar * sm_strHLSLCode[EFFECTLIGHT_TECHNIQUE_COUNT];
    static const UInt sm_iHLSLCodeLength[EFFECTLIGHT_TECHNIQUE_COUNT];

    GPURasterizerState m_hRasterizerState;
    GPUDepthStencilState m_hDepthStencilState;
    GPUBlendState m_hBlendState;
    
    GPUInputLayout m_hIL[EFFECTLIGHT_TECHNIQUE_COUNT];

    GPUShaderCompiler m_hCompiledVS[EFFECTLIGHT_TECHNIQUE_COUNT];
    GPUShaderCompiler m_hCompiledPS[EFFECTLIGHT_TECHNIQUE_COUNT];

    GPUVertexShader m_hVS[EFFECTLIGHT_TECHNIQUE_COUNT];
    GPUPixelShader m_hPS[EFFECTLIGHT_TECHNIQUE_COUNT];
    
    EffectPass m_hPass[EFFECTLIGHT_TECHNIQUE_COUNT];
    EffectTechnique m_hTechnique[EFFECTLIGHT_TECHNIQUE_COUNT];
};

/////////////////////////////////////////////////////////////////////////////////
// The EffectLightMaterialInstance class
class EffectLightMaterialInstance : public EffectInstance
{
protected:
    friend class Effect;
    friend class EffectLightMaterial;
    EffectLightMaterialInstance( const EffectLightMaterial * pEffectLightMaterial, EffectLightTechnique iTechnique, Light * pLight, Material * pMaterial );
    virtual ~EffectLightMaterialInstance();

public:
    // Methods
    virtual Void UpdateConstants( UInt iPass, const Transform3 * pWorldTransform, const Camera * pCamera, GPUDeferredContext * pContext = NULL ) const;

protected:
    Light * m_pLight;
    Material * m_pMaterial;

    EffectParameters m_hParameters;
    EffectShaderParameters m_hVSParameters;
    EffectShaderParameters m_hPSParameters;

    GPUConstantBuffer * m_pVSConstants;
    GPUConstantBuffer * m_pPSConstants;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "EffectLightMaterial.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RENDERING_EFFECTS_EFFECTLIBRARY_LIGHTING_EFFECTLIGHTMATERIAL_H
