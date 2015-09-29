/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Effects/EffectLibrary/Lighting/EffectLightMaterialTexture.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Lighting effect : Lighting + Material + Texture
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
#ifndef SCARAB_ENGINE_RENDERING_EFFECTS_EFFECTLIBRARY_LIGHTING_EFFECTLIGHTMATERIALTEXTURE_H
#define SCARAB_ENGINE_RENDERING_EFFECTS_EFFECTLIBRARY_LIGHTING_EFFECTLIGHTMATERIALTEXTURE_H

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
typedef struct _effect_lightmaterialtextureambiant_vertex {
    Vertex4 vPosition;
    TextureCoord2 vTexCoord;
} EffectLightMaterialTextureAmbiantVertex;

typedef struct _effect_lightmaterialtexturegouraud_vertex {
    Vertex4 vPosition;
    Vector4 vNormal;
    TextureCoord2 vTexCoord;
} EffectLightMaterialTextureGouraudVertex;

typedef struct _effect_lightmaterialtexturephong_vertex {
    Vertex4 vPosition;
    Vector4 vNormal;
    TextureCoord2 vTexCoord;
} EffectLightMaterialTexturePhongVertex;

// Prototypes
class EffectLightMaterialTexture;
class EffectLightMaterialTextureInstance;

/////////////////////////////////////////////////////////////////////////////////
// The EffectLightMaterialTexture class
class EffectLightMaterialTexture : public Effect
{
public:
    EffectLightMaterialTexture();
    virtual ~EffectLightMaterialTexture();

    EffectLightMaterialTextureInstance * CreateInstance( EffectLightTechnique iTechnique, Light * pLight, Material * pMaterial, GPUTexture2D * pTexture ) const;

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
// The EffectLightMaterialTextureInstance class
class EffectLightMaterialTextureInstance : public EffectInstance
{
protected:
    friend class Effect;
    friend class EffectLightMaterialTexture;
    EffectLightMaterialTextureInstance( const EffectLightMaterialTexture * pEffectLightMaterialTexture, EffectLightTechnique iTechnique, Light * pLight, Material * pMaterial, GPUTexture2D * pTexture );
    virtual ~EffectLightMaterialTextureInstance();

public:
    // Methods
    virtual Void UpdateConstants( UInt iPass, const Transform3 * pWorldTransform, const Camera * pCamera, GPUDeferredContext * pContext = NULL ) const;

protected:
    Light * m_pLight;
    Material * m_pMaterial;
    GPUTexture2D * m_pTexture;

    EffectParameters m_hParameters;
    EffectShaderParameters m_hVSParameters;
    EffectShaderParameters m_hPSParameters;

    GPUConstantBuffer * m_pVSConstants;

    GPUConstantBuffer * m_pPSConstants;
    GPUShaderView m_hPSTextureView;
    GPUSamplerState m_hPSSampler;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "EffectLightMaterialTexture.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RENDERING_EFFECTS_EFFECTLIBRARY_LIGHTING_EFFECTLIGHTMATERIALTEXTURE_H
