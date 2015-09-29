/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Resources/GPUResource.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU Resource : Common interface
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
#ifndef SCARAB_ENGINE_RENDERING_RESOURCES_GPURESOURCE_H
#define SCARAB_ENGINE_RENDERING_RESOURCES_GPURESOURCE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Resource/Resource.h"

#include "GPUDeferredContext.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// GPUResource definitions
enum GPUResourceType {
    GPURESOURCE_BUFFER_VERTEX   = DEVICERESOURCE_BUFFER_VERTEX,
    GPURESOURCE_BUFFER_INDEX    = DEVICERESOURCE_BUFFER_INDEX,
    GPURESOURCE_BUFFER_CONSTANT = DEVICERESOURCE_BUFFER_CONSTANT,
    GPURESOURCE_BUFFER_RAW      = DEVICERESOURCE_BUFFER_RAW,
    GPURESOURCE_BUFFER_STRUCT   = DEVICERESOURCE_BUFFER_STRUCT,
    GPURESOURCE_TEXTURE_1D      = DEVICERESOURCE_TEXTURE_1D,
    GPURESOURCE_TEXTURE_2D      = DEVICERESOURCE_TEXTURE_2D,
    GPURESOURCE_TEXTURE_3D      = DEVICERESOURCE_TEXTURE_3D,
    GPURESOURCE_TEXTURE_CUBE    = DEVICERESOURCE_TEXTURE_CUBE
};

enum GPUResourceUsage {
    GPURESOURCE_USAGE_DEFAULT = DEVICERESOURCE_USAGE_DEFAULT,
    GPURESOURCE_USAGE_CONST   = DEVICERESOURCE_USAGE_CONST,
    GPURESOURCE_USAGE_DYNAMIC = DEVICERESOURCE_USAGE_DYNAMIC,
    GPURESOURCE_USAGE_STAGING = DEVICERESOURCE_USAGE_STAGING
};

enum GPUResourceBind {
    GPURESOURCE_BIND_NONE             = DEVICERESOURCE_BIND_NONE,
    GPURESOURCE_BIND_VERTEX_BUFFER    = DEVICERESOURCE_BIND_VERTEX_BUFFER,
    GPURESOURCE_BIND_INDEX_BUFFER     = DEVICERESOURCE_BIND_INDEX_BUFFER,
    GPURESOURCE_BIND_CONSTANT_BUFFER  = DEVICERESOURCE_BIND_CONSTANT_BUFFER,
    GPURESOURCE_BIND_SHADER_INPUT     = DEVICERESOURCE_BIND_SHADER_INPUT,
    GPURESOURCE_BIND_STREAM_OUTPUT    = DEVICERESOURCE_BIND_STREAM_OUTPUT,
    GPURESOURCE_BIND_RENDER_TARGET    = DEVICERESOURCE_BIND_RENDER_TARGET,
    GPURESOURCE_BIND_DEPTH_STENCIL    = DEVICERESOURCE_BIND_DEPTH_STENCIL,
    GPURESOURCE_BIND_UNORDERED_ACCESS = DEVICERESOURCE_BIND_UNORDERED_ACCESS
};

enum GPUResourcePriority {
    GPURESOURCE_PRIORITY_MINIMUM = DEVICERESOURCE_PRIORITY_MINIMUM,
    GPURESOURCE_PRIORITY_LOW     = DEVICERESOURCE_PRIORITY_LOW,
    GPURESOURCE_PRIORITY_NORMAL  = DEVICERESOURCE_PRIORITY_NORMAL,
    GPURESOURCE_PRIORITY_HIGH    = DEVICERESOURCE_PRIORITY_HIGH,
    GPURESOURCE_PRIORITY_MAXIMUM = DEVICERESOURCE_PRIORITY_MAXIMUM
};

enum GPUResourceLock {
    GPURESOURCE_LOCK_NONE               = DEVICERESOURCE_LOCK_NONE,
    GPURESOURCE_LOCK_READ               = DEVICERESOURCE_LOCK_READ,
    GPURESOURCE_LOCK_WRITE              = DEVICERESOURCE_LOCK_WRITE,
    GPURESOURCE_LOCK_READ_WRITE         = DEVICERESOURCE_LOCK_READ_WRITE,
    GPURESOURCE_LOCK_WRITE_DISCARD      = DEVICERESOURCE_LOCK_WRITE_DISCARD,
    GPURESOURCE_LOCK_WRITE_NO_OVERWRITE = DEVICERESOURCE_LOCK_WRITE_NO_OVERWRITE,
};

enum GPUResourceLockFlags {
    GPURESOURCE_LOCKFLAG_NONE      = DEVICERESOURCE_LOCKFLAG_NONE,
    GPURESOURCE_LOCKFLAG_DONT_WAIT = DEVICERESOURCE_LOCKFLAG_DONT_WAIT
};

// Prototypes
class RenderingManager;

/////////////////////////////////////////////////////////////////////////////////
// The GPUResource class
class GPUResource : public Resource
{
protected:
    friend class RenderingManager;
    GPUResource();
    virtual ~GPUResource();

public:
    // Binding
    inline Bool IsBound() const;

    // Getters
    virtual ResourceType GetResourceType() const = 0;

    inline GPUResourceType GetType() const;
    inline Bool IsBuffer() const;
    inline Bool IsTexture() const;

    inline GPUResourceUsage GetUsage() const;

    inline Bool HasBind( GPUResourceBind iBind ) const;
    inline UInt GetBinds() const;

    inline Bool CanCPURead() const;
    inline Bool CanCPUWrite() const;
    inline Bool CanGPUWrite() const; // GPU-Read is allways allowed
    inline Bool CanUpdate() const;
    inline Bool CanLock() const;

    inline Bool IsLocked() const;

    // Methods
    inline Void SetUsage( GPUResourceUsage iUsage );

    inline GPUResourcePriority GetEvictionPriority() const;
    inline Void SetEvictionPriority( GPUResourcePriority iPriority );

protected:
    // Resource management
    virtual Bool _OnLoad( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) = 0;
    virtual Bool _OnSave( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) const = 0;

    // Device data
    DeviceResource * m_pDeviceResource;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GPUResource.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RENDERING_RESOURCES_GPURESOURCE_H


