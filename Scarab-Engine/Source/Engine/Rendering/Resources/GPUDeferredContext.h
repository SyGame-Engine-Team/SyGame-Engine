/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Resources/GPUDeferredContext.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU Resource : Deferred Rendering (Command lists)
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_RENDERING_RESOURCES_GPUDEFERREDCONTEXT_H
#define SCARAB_ENGINE_RENDERING_RESOURCES_GPUDEFERREDCONTEXT_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Error/ErrorManager.h"

#include "../DeviceRenderer.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// GPUCommandList definitions
typedef DeviceCommandList GPUCommandList;

// Prototypes
class RenderingManager;

class GPUBuffer;
class GPUVertexBuffer;
class GPUIndexBuffer;
class GPUConstantBuffer;
class GPURawBuffer;
class GPUStructuredBuffer;
class GPUTexture;
class GPUTexture1D;
class GPUTexture2D;
class GPUTexture3D;
class GPUTextureCube;

/////////////////////////////////////////////////////////////////////////////////
// The GPUDeferredContext class
class GPUDeferredContext
{
public:
    GPUDeferredContext();
    ~GPUDeferredContext();

    // Binding
    inline Bool IsBound() const;

    Void Bind();
    Void UnBind();

    // Methods
    inline Void MakeCommandList( GPUCommandList * outCommandList, Bool bRestoreState );

    inline Void Clear(); // Disables everything in this context
    inline Void Reset(); // Reset command list then Clear

private:
    friend class RenderingManager;

    // Access to resources
    friend class GPUAsynchronous;
    friend class GPUBuffer;
    friend class GPUVertexBuffer;
    friend class GPUIndexBuffer;
    friend class GPUConstantBuffer;
    friend class GPURawBuffer;
    friend class GPUStructuredBuffer;
    friend class GPUTexture;
    friend class GPUTexture1D;
    friend class GPUTexture2D;
    friend class GPUTexture3D;
    friend class GPUTextureCube;

    // Device data
    DeviceDeferredContext m_hDeviceDeferredContext;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GPUDeferredContext.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RENDERING_RESOURCES_GPUDEFERREDCONTEXT_H
