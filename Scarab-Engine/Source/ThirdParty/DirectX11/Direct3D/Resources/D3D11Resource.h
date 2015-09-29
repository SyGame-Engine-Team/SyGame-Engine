/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/Resources/D3D11Resource.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Platform-dependant abstraction for GPU resources.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs :
// A resource is represented this way :
//      i=0           i=1           i=2           i=3
//  /--------/    /--------/    /--------/    /--------/  j=0
//      /----/        /----/        /----/        /----/  j=1
//        /--/          /--/          /--/          /--/  j=2
//         /-/           /-/           /-/           /-/  j=3
// Where (i,j) are "Array slices" and "Mip slices" respectively
// A region is a set of subresources delimited by a rectangle of
// array and mip slices.
// The sub-resource flat array is "i-Major" : idx = ( i * MipLevels + j )
// Buffer resources don't use subresources and will only be made of a single
// array index and a single mip index. The layout is heavily used by textures
// on the other hand.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_RESOURCES_D3D11RESOURCE_H
#define SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_RESOURCES_D3D11RESOURCE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "D3D11DeferredContext.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Resource types
enum D3D11ResourceType {
    D3D11RESOURCE_BUFFER_VERTEX = 0,
    D3D11RESOURCE_BUFFER_INDEX,
    D3D11RESOURCE_BUFFER_CONSTANT,
    D3D11RESOURCE_BUFFER_RAW,
    D3D11RESOURCE_BUFFER_STRUCT,
    D3D11RESOURCE_TEXTURE_1D,
    D3D11RESOURCE_TEXTURE_2D,
    D3D11RESOURCE_TEXTURE_3D,
    D3D11RESOURCE_TEXTURE_CUBE
};

// Prototypes
class D3D11RenderTargetView;
class D3D11DepthStencilView;
class D3D11ShaderView;
class D3D11UnorderedAccessView;

/////////////////////////////////////////////////////////////////////////////////
// The D3D11Resource class
class D3D11Resource
{
protected:
    D3D11Resource();
public:
    virtual ~D3D11Resource();

    // Deferred construction
    virtual Bool IsCreated() const = 0;

    // Getters
    virtual D3D11ResourceType GetType() const = 0;

    inline D3D11ResourceUsage GetUsage() const;
    
    inline Bool HasBind( D3D11ResourceBind iBind ) const;
    inline UInt GetBinds() const;

    inline Bool CanCPURead() const;
    inline Bool CanCPUWrite() const;
    inline Bool CanGPUWrite() const; // GPU can allways read
    inline Bool CanUpdate() const;
    inline Bool CanLock() const;

    inline Bool IsLocked() const;

    // Methods
    Void SetUsage( D3D11ResourceUsage iUsage );

    D3D11ResourcePriority GetEvictionPriority() const;
    Void SetEvictionPriority( D3D11ResourcePriority iPriority );

protected:
    friend class D3D11Renderer;

    // Access to views
    friend class D3D11RenderTargetView;
    friend class D3D11DepthStencilView;
    friend class D3D11ShaderView;
    friend class D3D11UnorderedAccessView;

    // Helpers
    UInt _GetCPUAccessFlags() const;

    // Resource data
    D3D11ResourceUsage m_iUsage;
    UInt m_iBinds;

    Bool m_bLocked; // TODO : Make this a map, allow multiple subresources locks

    ID3D11Resource * m_pResource;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "D3D11Resource.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_RESOURCES_D3D11RESOURCE_H

