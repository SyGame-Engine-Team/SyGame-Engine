/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Resources/GPUResourceView.h
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
#ifndef SCARAB_ENGINE_RENDERING_RESOURCES_GPURESOURCEVIEW_H
#define SCARAB_ENGINE_RENDERING_RESOURCES_GPURESOURCEVIEW_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "GPUBuffer.h"
#include "GPUTexture.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// GPUResourceView definitions
enum GPUResourceViewType {
    GPURESOURCEVIEW_RENDER_TARGET    = DEVICERESOURCEVIEW_RENDER_TARGET,
    GPURESOURCEVIEW_DEPTH_STENCIL    = DEVICERESOURCEVIEW_DEPTH_STENCIL,
    GPURESOURCEVIEW_SHADER           = DEVICERESOURCEVIEW_SHADER,
    GPURESOURCEVIEW_UNORDERED_ACCESS = DEVICERESOURCEVIEW_UNORDERED_ACCESS
};

// GPURenderTargetView definitions
enum GPURenderTargetViewDimension {
    GPURENDERTARGETVIEW_DIM_BUFFER           = DEVICERENDERTARGETVIEW_DIM_BUFFER,
    GPURENDERTARGETVIEW_DIM_TEXTURE1D        = DEVICERENDERTARGETVIEW_DIM_TEXTURE1D,
    GPURENDERTARGETVIEW_DIM_TEXTURE1DARRAY   = DEVICERENDERTARGETVIEW_DIM_TEXTURE1DARRAY,
    GPURENDERTARGETVIEW_DIM_TEXTURE2D        = DEVICERENDERTARGETVIEW_DIM_TEXTURE2D,
    GPURENDERTARGETVIEW_DIM_TEXTURE2DARRAY   = DEVICERENDERTARGETVIEW_DIM_TEXTURE2DARRAY,
    GPURENDERTARGETVIEW_DIM_TEXTURE2DMS      = DEVICERENDERTARGETVIEW_DIM_TEXTURE2DMS,
    GPURENDERTARGETVIEW_DIM_TEXTURE2DMSARRAY = DEVICERENDERTARGETVIEW_DIM_TEXTURE2DMSARRAY,
    GPURENDERTARGETVIEW_DIM_TEXTURE3D        = DEVICERENDERTARGETVIEW_DIM_TEXTURE3D
};

typedef struct _gpu_rendertarget_view_desc {
    PixelFormat iFormat;

    GPURenderTargetViewDimension iViewDimension;
    union {
        struct { UInt iOffset; UInt iSize; } hBuffer;
        struct { UInt iMipSlice; } hTexture1D;
        struct { UInt iMipSlice; UInt iArraySlice; UInt iArraySliceCount; } hTexture1DArray;
        struct { UInt iMipSlice; } hTexture2D;
        struct { UInt iMipSlice; UInt iArraySlice; UInt iArraySliceCount; } hTexture2DArray;
        struct { UInt _reserved; } hTexture2DMS;
        struct { UInt iArraySlice; UInt iArraySliceCount; } hTexture2DMSArray;
        struct { UInt iMipSlice; UInt iDepthSlice; UInt iDepthSliceCount; } hTexture3D;
    };
} GPURenderTargetViewDesc;

// GPUDepthStencilView definitions
enum GPUDepthStencilViewDimension {
    GPUDEPTHSTENCILVIEW_DIM_TEXTURE1D        = DEVICEDEPTHSTENCILVIEW_DIM_TEXTURE1D,
    GPUDEPTHSTENCILVIEW_DIM_TEXTURE1DARRAY   = DEVICEDEPTHSTENCILVIEW_DIM_TEXTURE1DARRAY,
    GPUDEPTHSTENCILVIEW_DIM_TEXTURE2D        = DEVICEDEPTHSTENCILVIEW_DIM_TEXTURE2D,
    GPUDEPTHSTENCILVIEW_DIM_TEXTURE2DARRAY   = DEVICEDEPTHSTENCILVIEW_DIM_TEXTURE2DARRAY,
    GPUDEPTHSTENCILVIEW_DIM_TEXTURE2DMS      = DEVICEDEPTHSTENCILVIEW_DIM_TEXTURE2DMS,
    GPUDEPTHSTENCILVIEW_DIM_TEXTURE2DMSARRAY = DEVICEDEPTHSTENCILVIEW_DIM_TEXTURE2DMSARRAY
};

enum GPUDepthStencilViewFlags {
    GPUDEPTHSTENCILVIEW_FLAG_NONE             = DEVICEDEPTHSTENCILVIEW_FLAG_NONE,
    GPUDEPTHSTENCILVIEW_FLAG_READONLY_DEPTH   = DEVICEDEPTHSTENCILVIEW_FLAG_READONLY_DEPTH,
    GPUDEPTHSTENCILVIEW_FLAG_READONLY_STENCIL = DEVICEDEPTHSTENCILVIEW_FLAG_READONLY_STENCIL
};

typedef struct _gpu_depthstencil_view_desc {
    PixelFormat iFormat;
    UInt iFlags; // GPUDepthStencilViewFlags

    GPUDepthStencilViewDimension iViewDimension;
    union {
        struct { UInt iMipSlice; } hTexture1D;
        struct { UInt iMipSlice; UInt iArraySlice; UInt iArraySliceCount; } hTexture1DArray;
        struct { UInt iMipSlice; } hTexture2D;
        struct { UInt iMipSlice; UInt iArraySlice; UInt iArraySliceCount; } hTexture2DArray;
        struct { UInt _reserved; } hTexture2DMS;
        struct { UInt iArraySlice; UInt iArraySliceCount; } hTexture2DMSArray;
    };
} GPUDepthStencilViewDesc;

// GPUShaderView definitions
enum GPUShaderViewDimension {
    GPUSHADERVIEW_DIM_BUFFER           = DEVICESHADERVIEW_DIM_BUFFER,
    GPUSHADERVIEW_DIM_BUFFEREX         = DEVICESHADERVIEW_DIM_BUFFEREX,
    GPUSHADERVIEW_DIM_TEXTURE1D        = DEVICESHADERVIEW_DIM_TEXTURE1D,
    GPUSHADERVIEW_DIM_TEXTURE1DARRAY   = DEVICESHADERVIEW_DIM_TEXTURE1DARRAY,
    GPUSHADERVIEW_DIM_TEXTURE2D        = DEVICESHADERVIEW_DIM_TEXTURE2D,
    GPUSHADERVIEW_DIM_TEXTURE2DARRAY   = DEVICESHADERVIEW_DIM_TEXTURE2DARRAY,
    GPUSHADERVIEW_DIM_TEXTURE2DMS      = DEVICESHADERVIEW_DIM_TEXTURE2DMS,
    GPUSHADERVIEW_DIM_TEXTURE2DMSARRAY = DEVICESHADERVIEW_DIM_TEXTURE2DMSARRAY,
    GPUSHADERVIEW_DIM_TEXTURE3D        = DEVICESHADERVIEW_DIM_TEXTURE3D,
    GPUSHADERVIEW_DIM_TEXTURECUBE      = DEVICESHADERVIEW_DIM_TEXTURECUBE,
    GPUSHADERVIEW_DIM_TEXTURECUBEARRAY = DEVICESHADERVIEW_DIM_TEXTURECUBEARRAY
};

enum GPUShaderViewBufferExFlags {
    GPUSHADERVIEW_BUFFEREXFLAG_NONE = DEVICESHADERVIEW_BUFFEREXFLAG_NONE,
    GPUSHADERVIEW_BUFFEREXFLAG_RAW  = DEVICESHADERVIEW_BUFFEREXFLAG_RAW
};

typedef struct _gpu_shader_view_desc {
    PixelFormat iFormat;

    GPUShaderViewDimension iViewDimension;
    union {
        struct { UInt iOffset; UInt iSize; } hBuffer;
        struct { UInt iOffset; UInt iSize; UInt iFlags; } hBufferEx; // GPUShaderViewBufferExFlags
        struct { UInt iMostDetailedMip; UInt iMipLevels; } hTexture1D;
        struct { UInt iMostDetailedMip; UInt iMipLevels; UInt iArraySlice; UInt iArraySliceCount; } hTexture1DArray;
        struct { UInt iMostDetailedMip; UInt iMipLevels; } hTexture2D;
        struct { UInt iMostDetailedMip; UInt iMipLevels; UInt iArraySlice; UInt iArraySliceCount; } hTexture2DArray;
        struct { UInt _reserved; } hTexture2DMS;
        struct { UInt iArraySlice; UInt iArraySliceCount; } hTexture2DMSArray;
        struct { UInt iMostDetailedMip; UInt iMipLevels; } hTexture3D;
        struct { UInt iMostDetailedMip; UInt iMipLevels; } hTextureCube;
        struct { UInt iMostDetailedMip; UInt iMipLevels; UInt iFirstCube; UInt iCubeCount; } hTextureCubeArray;
    };
} GPUShaderViewDesc;

// GPUUnorderedAccessView definitions
enum GPUUnorderedAccessViewDimension {
    GPUUNORDEREDACCESSVIEW_DIM_BUFFER         = DEVICEUNORDEREDACCESSVIEW_DIM_BUFFER,
    GPUUNORDEREDACCESSVIEW_DIM_TEXTURE1D      = DEVICEUNORDEREDACCESSVIEW_DIM_TEXTURE1D,
    GPUUNORDEREDACCESSVIEW_DIM_TEXTURE1DARRAY = DEVICEUNORDEREDACCESSVIEW_DIM_TEXTURE1DARRAY,
    GPUUNORDEREDACCESSVIEW_DIM_TEXTURE2D      = DEVICEUNORDEREDACCESSVIEW_DIM_TEXTURE2D,
    GPUUNORDEREDACCESSVIEW_DIM_TEXTURE2DARRAY = DEVICEUNORDEREDACCESSVIEW_DIM_TEXTURE2DARRAY,
    GPUUNORDEREDACCESSVIEW_DIM_TEXTURE3D      = DEVICEUNORDEREDACCESSVIEW_DIM_TEXTURE3D
};

enum GPUUnorderedAccessViewBufferFlags {
    GPUUNORDEREDACCESSVIEW_BUFFERFLAG_NONE    = DEVICEUNORDEREDACCESSVIEW_BUFFERFLAG_NONE,
    GPUUNORDEREDACCESSVIEW_BUFFERFLAG_RAW     = DEVICEUNORDEREDACCESSVIEW_BUFFERFLAG_RAW,
    GPUUNORDEREDACCESSVIEW_BUFFERFLAG_APPEND  = DEVICEUNORDEREDACCESSVIEW_BUFFERFLAG_APPEND,
    GPUUNORDEREDACCESSVIEW_BUFFERFLAG_COUNTER = DEVICEUNORDEREDACCESSVIEW_BUFFERFLAG_COUNTER
};

typedef struct _gpu_unorderedaccess_view_desc {
    PixelFormat iFormat;

    GPUUnorderedAccessViewDimension iViewDimension;
    union {
        struct { UInt iOffset; UInt iSize; UInt iFlags; } hBuffer; // GPUUnorderedAccessViewBufferFlags
        struct { UInt iMipSlice; } hTexture1D;
        struct { UInt iMipSlice; UInt iArraySlice; UInt iArraySliceCount; } hTexture1DArray;
        struct { UInt iMipSlice; } hTexture2D;
        struct { UInt iMipSlice; UInt iArraySlice; UInt iArraySliceCount; } hTexture2DArray;
        struct { UInt iMipSlice; UInt iDepthSlice; UInt iDepthSliceCount; } hTexture3D;
    };
} GPUUnorderedAccessViewDesc;

/////////////////////////////////////////////////////////////////////////////////
// The GPUResourceView class
class GPUResourceView
{
protected:
    GPUResourceView();
public:
    virtual ~GPUResourceView();

    // Binding
    inline Bool IsBound() const;
    inline Bool IsBoundToBackBuffer( UInt * outBackBuffer = NULL ) const;

    Void UnBind();

    // Getters
    inline GPUResourceViewType GetType() const;

    inline GPUResource * GetResource() const;

protected:
    friend class RenderingManager;

    GPUResource * m_pResource;

    // Device data
    DeviceResourceView * m_pDeviceResourceView;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPURenderTargetView class
class GPURenderTargetView : public GPUResourceView
{
public:
    GPURenderTargetView();
    virtual ~GPURenderTargetView();
    
    // Binding
    Void BindToBackBuffer( UInt iBackBuffer );
    Void Bind( GPUBuffer * pBuffer, UInt iIndex = 0, UInt iCount = INVALID_OFFSET );
    Void Bind( GPUTexture * pTexture, UInt iMipSlice = 0, UInt iArraySlice = 0, UInt iArraySliceCount = 1 );

    // Getters
    inline Void GetDesc( GPURenderTargetViewDesc * outDesc ) const;

protected:
    friend class RenderingManager;

    // Device data
    DeviceRenderTargetView m_hDeviceRenderTargetView;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPUDepthStencilView class
class GPUDepthStencilView : public GPUResourceView
{
public:
    GPUDepthStencilView();
    virtual ~GPUDepthStencilView();
    
    // Binding
    Void BindToBackBuffer( UInt iBackBuffer, Bool bReadOnlyDepth = false, Bool bReadOnlyStencil = false );
    Void Bind( GPUTexture * pTexture, UInt iMipSlice = 0, UInt iArraySlice = 0, UInt iArraySliceCount = 1, Bool bReadOnlyDepth = false, Bool bReadOnlyStencil = false );

    // Getters
    inline Void GetDesc( GPUDepthStencilViewDesc * outDesc ) const;

protected:
    friend class RenderingManager;

    // Device data
    DeviceDepthStencilView m_hDeviceDepthStencilView;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPUShaderView class
class GPUShaderView : public GPUResourceView
{
public:
    GPUShaderView();
    virtual ~GPUShaderView();
    
    // Binding (iArraySlice/iArraySliceCount = iFirstCube/iCubeCount for GPUTextureCube)
    Void BindToBackBuffer( UInt iBackBuffer );
    Void Bind( GPUBuffer * pBuffer, UInt iIndex = 0, UInt iCount = INVALID_OFFSET );
    Void Bind( GPUTexture * pTexture, UInt iMostDetailedMip = 0, UInt iMipLevelCount = INVALID_OFFSET, UInt iArraySlice = 0, UInt iArraySliceCount = 1 );

    // Getters
    inline Void GetDesc( GPUShaderViewDesc * outDesc ) const;

protected:
    friend class RenderingManager;

    // Device data
    DeviceShaderView m_hDeviceShaderView;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPUUnorderedAccessView class
class GPUUnorderedAccessView : public GPUResourceView
{
public:
    GPUUnorderedAccessView();
    virtual ~GPUUnorderedAccessView();
    
    // Binding
    Void BindToBackBuffer( UInt iBackBuffer );
    Void Bind( GPUBuffer * pBuffer, UInt iIndex = 0, UInt iCount = INVALID_OFFSET, Bool bAppendConsume = false, Bool bUseCounter = false );
    Void Bind( GPUTexture * pTexture, UInt iMipSlice = 0, UInt iArraySlice = 0, UInt iArraySliceCount = 1 );

    // Getters
    inline Void GetDesc( GPUUnorderedAccessViewDesc * outDesc ) const;

protected:
    friend class RenderingManager;

    // Device data
    DeviceUnorderedAccessView m_hDeviceUnorderedAccessView;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GPUResourceView.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RENDERING_RESOURCES_GPURESOURCEVIEW_H
