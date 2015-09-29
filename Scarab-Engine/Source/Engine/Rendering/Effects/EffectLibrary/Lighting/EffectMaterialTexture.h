/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Effects/EffectLibrary/Lighting/EffectMaterialTexture.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Lighting effect : Material + Texture
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
#ifndef SCARAB_ENGINE_RENDERING_EFFECTS_EFFECTLIBRARY_LIGHTING_EFFECTMATERIALTEXTURE_H
#define SCARAB_ENGINE_RENDERING_EFFECTS_EFFECTLIBRARY_LIGHTING_EFFECTMATERIALTEXTURE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../EffectInstance.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Input layout
typedef struct _effect_materialtexture_vertex {
    Vertex4 vPosition;
    TextureCoord2 vTexCoord;
} EffectMaterialTextureVertex;

// Prototypes
class EffectMaterialTexture;
class EffectMaterialTextureInstance;

/////////////////////////////////////////////////////////////////////////////////
// The EffectMaterialTexture class
class EffectMaterialTexture : public Effect
{
public:
    EffectMaterialTexture();
    virtual ~EffectMaterialTexture();

    EffectMaterialTextureInstance * CreateInstance( Material * pMaterial, GPUTexture2D * pTexture ) const;

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
// The EffectMaterialTextureInstance class
class EffectMaterialTextureInstance : public EffectInstance
{
protected:
    friend class Effect;
    friend class EffectMaterialTexture;
    EffectMaterialTextureInstance( const EffectMaterialTexture * pEffectMaterial, Material * pMaterial, GPUTexture2D * pTexture );
    virtual ~EffectMaterialTextureInstance();

public:
    // Methods
    virtual Void UpdateConstants( UInt iPass, const Transform3 * pWorldTransform, const Camera * pCamera, GPUDeferredContext * pContext = NULL ) const;

protected:
    Material * m_pMaterial;
    GPUTexture2D * m_pTexture;

    EffectParameters m_hParameters;
    EffectShaderParameters m_hVSParameters;
    EffectShaderParameters m_hPSParameters;

    GPUConstantBuffer * m_pVSConstants;

    GPUShaderView m_hPSTextureView;
    GPUSamplerState m_hPSSampler;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "EffectMaterialTexture.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RENDERING_EFFECTS_EFFECTLIBRARY_LIGHTING_EFFECTMATERIALTEXTURE_H
