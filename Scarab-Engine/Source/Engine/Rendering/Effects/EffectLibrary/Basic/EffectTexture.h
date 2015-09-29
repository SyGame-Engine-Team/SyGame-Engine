/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Effects/EffectLibrary/Basic/EffectTexture.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Basic effect : Texture
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
#ifndef SCARAB_ENGINE_RENDERING_EFFECTS_EFFECTLIBRARY_BASIC_EFFECTTEXTURE_H
#define SCARAB_ENGINE_RENDERING_EFFECTS_EFFECTLIBRARY_BASIC_EFFECTTEXTURE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../EffectInstance.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Input layout
typedef struct _effect_texture_vertex {
    Vertex4 vPosition;
    TextureCoord2 vTexCoord;
} EffectTextureVertex;

// Prototypes
class EffectTexture;
class EffectTextureInstance;

/////////////////////////////////////////////////////////////////////////////////
// The EffectTexture class
class EffectTexture : public Effect
{
public:
    EffectTexture();
    virtual ~EffectTexture();

    EffectTextureInstance * CreateInstance( GPUTexture2D * pTexture ) const;

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
// The EffectTextureInstance class
class EffectTextureInstance : public EffectInstance
{
protected:
    friend class Effect;
    friend class EffectTexture;
    EffectTextureInstance( const EffectTexture * pEffectTexture, GPUTexture2D * pTexture );
    virtual ~EffectTextureInstance();

public:
    // Methods
    virtual Void UpdateConstants( UInt iPass, const Transform3 * pWorldTransform, const Camera * pCamera, GPUDeferredContext * pContext = NULL ) const;

protected:
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
#include "EffectTexture.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RENDERING_EFFECTS_EFFECTLIBRARY_BASIC_EFFECTTEXTURE_H
