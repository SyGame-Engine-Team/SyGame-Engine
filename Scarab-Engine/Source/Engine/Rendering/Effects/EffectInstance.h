/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Effects/EffectInstance.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Instanciation for rendering with the effect system.
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
#ifndef SCARAB_ENGINE_RENDERING_EFFECTS_EFFECTINSTANCE_H
#define SCARAB_ENGINE_RENDERING_EFFECTS_EFFECTINSTANCE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Math/Types/TextureCoord/TextureCoord1.h"
#include "../../../Lib/Math/Types/TextureCoord/TextureCoord2.h"
#include "../../../Lib/Math/Types/TextureCoord/TextureCoord3.h"
#include "../../../Lib/Math/Types/TextureCoord/TextureCoord4.h"

#include "../Resources/GPUBuffer.h"
#include "../Resources/GPUTexture.h"
#include "../Resources/GPUResourceView.h"

#include "../Resources/Camera.h"
#include "../Resources/Light.h"
#include "../Resources/Material.h"

#include "Effect.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The EffectShaderParameters class
class EffectShaderParameters
{
public:
    EffectShaderParameters( GPUShader * pShader );
    virtual ~EffectShaderParameters();

    // Getters
    inline GPUShader * GetShader() const;

    // Constant buffers
    inline UInt GetConstantBufferCount() const;
    inline GPUConstantBuffer * GetConstantBuffer( UInt iConstantBuffer ) const;
    inline GPUConstantBuffer ** GetConstantBuffers( UInt iConstantBuffer );

    Void AddConstantBuffer( GPUConstantBuffer * pConstantBuffer );
    GPUConstantBuffer * RemoveConstantBuffer( UInt iConstantBuffer );
    GPUConstantBuffer * SetConstantBuffer( UInt iConstantBuffer, GPUConstantBuffer * pConstantBuffer );

    // Samplers
    inline UInt GetSamplerCount() const;
    inline GPUSamplerState * GetSampler( UInt iSampler ) const;
    inline GPUSamplerState ** GetSamplers( UInt iSampler );

    Void AddSampler( GPUSamplerState * pSampler );
    GPUSamplerState * RemoveSampler( UInt iSampler );
    GPUSamplerState * SetSampler( UInt iSampler, GPUSamplerState * pSampler );

    // Resources
    inline UInt GetResourceCount() const;
    inline GPUShaderView * GetResource( UInt iResource ) const;
    inline GPUShaderView ** GetResources( UInt iResource );

    Void AddResource( GPUShaderView * pResource );
    GPUShaderView * RemoveResource( UInt iResource );
    GPUShaderView * SetResource( UInt iResource, GPUShaderView * pResource );

protected:
    GPUShader * m_pShader;

    UInt m_iConstantBufferCount;
    GPUConstantBuffer * m_arrConstantBuffers[DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS];

    UInt m_iSamplerCount;
    GPUSamplerState * m_arrSamplers[DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS];

    UInt m_iResourceCount;
    GPUShaderView * m_arrResources[DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS];
};

/////////////////////////////////////////////////////////////////////////////////
// The EffectParameters class
class EffectParameters
{
public:
    EffectParameters();
    virtual ~EffectParameters();

    // Shader parameters
    inline EffectShaderParameters * GetShaderParameters( GPUShaderType iType ) const;
    EffectShaderParameters * SetShaderParameters( GPUShaderType iType, EffectShaderParameters * pShaderParameters );

    // Stencil parameters
    inline DWord GetStencilRef() const;
    inline Void SetStencilRef( DWord iStencilRef );

    // Blend parameters
    inline const Float * GetBlendFactors() const;
    inline Void SetBlendFactors( const Float arrBlendFactors[4] );

    inline DWord GetBlendSampleMask() const;
    inline Void SetBlendSampleMask( DWord iBlendSampleMask );

    // Output buffers
    inline UInt GetOutputBufferCount() const;
    inline GPUBuffer * GetOutputBuffer( UInt iOutputBuffer ) const;
    inline UInt GetOutputBufferOffset( UInt iOutputBuffer ) const;
    inline GPUBuffer ** GetOutputBuffers( UInt iOutputBuffer );
    inline UInt * GetOutputBufferOffsets( UInt iOutputBuffer );

    Void AddOutputBuffer( GPUBuffer * pOutputBuffer, UInt iOffset = 0 );
    GPUBuffer * RemoveOutputBuffer( UInt iOutputBuffer, UInt * outOffset = NULL );
    GPUBuffer * SetOutputBuffer( UInt iOutputBuffer, GPUBuffer * pOutputBuffer, UInt iOffset = 0, UInt * outOldOffset = NULL );

protected:
    EffectShaderParameters * m_pVSParameters;
    EffectShaderParameters * m_pGSParameters;
    EffectShaderParameters * m_pPSParameters;
    EffectShaderParameters * m_pHSParameters;
    EffectShaderParameters * m_pDSParameters;

    UInt m_iStencilRef;
    Float m_arrBlendFactors[4];
    DWord m_iBlendSampleMask;

    UInt m_iOutputBufferCount;
    GPUBuffer * m_arrOutputBuffers[DEVICERENDERER_MAX_STREAMOUTPUT_DECLARATIONS];
    UInt m_arrOutputBufferOffsets[DEVICERENDERER_MAX_STREAMOUTPUT_DECLARATIONS];
};

/////////////////////////////////////////////////////////////////////////////////
// The EffectInstance class
class EffectInstance
{
protected:
    friend class Effect;
    EffectInstance( const Effect * pEffect, UInt iTechniqueIndex );
    virtual ~EffectInstance();

public:
    // Getters
    inline const Effect * GetEffect() const;

    inline UInt GetTechniqueIndex() const;
    inline const EffectTechnique * GetTechnique() const;

    inline UInt GetPassCount() const;
    inline const EffectPass * GetPass( UInt iPass ) const;

    inline EffectParameters * GetParameters( UInt iPass ) const;
    EffectParameters * SetParameters( UInt iPass, EffectParameters * pParameters );

    // Methods
    Void Enable( UInt iPass, GPUDeferredContext * pContext = NULL ) const;
    Void Disable( UInt iPass, GPUDeferredContext * pContext = NULL ) const;

    virtual Void UpdateConstants( UInt iPass, const Transform3 * pWorldTransform, const Camera * pCamera, GPUDeferredContext * pContext = NULL ) const = 0;

protected:
    const Effect * m_pEffect;

    UInt m_iTechniqueIndex;
    const EffectTechnique * m_pTechnique;

    UInt m_iPassCount;
    EffectParameters * m_arrParameters[EFFECT_TECHNIQUE_MAX_PASSES];
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "EffectInstance.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RENDERING_EFFECTS_EFFECTINSTANCE_H
