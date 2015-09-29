/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/Resources/D3D11Texture.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU Resources : Textures.
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
#ifndef SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_RESOURCES_D3D11TEXTURE_H
#define SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_RESOURCES_D3D11TEXTURE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "D3D11Resource.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define D3D11TEXTURE_MAX_DIMENSIONS 3

#define D3D11TEXTURE_MIPLEVELS_AUTO 0
#define D3D11TEXTURE_MIPLEVELS_NONE 1

// Prototypes
class D3D11RenderTargetView;
class D3D11DepthStencilView;
class D3D11ShaderView;
class D3D11UnorderedAccessView;

/////////////////////////////////////////////////////////////////////////////////
// The D3D11Texture class
class D3D11Texture : public D3D11Resource
{
protected:
    D3D11Texture( D3D11Renderer * pRenderer );
public:
    virtual ~D3D11Texture();

    // Deferred construction
    inline Bool IsCreated() const;
    inline Void UpdateDataSource( const Void * pData );

    virtual Void Destroy();

    Void OnDestroyDevice();
    Void OnRestoreDevice();

    // Getters
    virtual D3D11ResourceType GetType() const = 0;

    inline UInt GetStride() const;
    virtual PixelFormat GetFormat() const = 0;
    virtual UInt GetBound( UInt iDimension, UInt iMipLevel = 0 ) const = 0;

    virtual UInt GetMipLevelCount() const = 0;
    inline virtual Bool IsArray() const;
    virtual UInt GetArrayCount() const = 0;
    inline UInt GetSubResourceCount() const;

    // Methods
    Float GetMinLOD( D3D11DeferredContext * pContext = NULL ) const;
    Void SetMinLOD( Float fMinLOD = FLOAT_INFINITE, D3D11DeferredContext * pContext = NULL ) const;

protected:
    friend class D3D11Renderer;
    D3D11Renderer * m_pRenderer;

    // Helpers
    inline static UInt _GetStride( DXGI_FORMAT iFormat );
    inline static UInt _GetBound( UInt iMaxBound, UInt iMipLevel );
    inline static UInt _GetSubResourceIndex( const D3D11SubResourceIndex * pIndex, UInt iMipLevelCount, UInt iArrayCount );
    inline static UInt _GetSubResourceIndex( const D3D11SubResourceIndex * pIndex, D3D11TextureCubeFace iFace, UInt iMipLevelCount, UInt iArrayCount );

    // Auto-Regen system
    virtual Void _NakedCreate() = 0;
    virtual Void _NakedDestroy() = 0;

    typedef struct _creation_parameters {
        const Void * pData;
    } _CreationParameters;

    Bool m_bTemporaryDestroyed;
    _CreationParameters m_hCreationParameters;
};

/////////////////////////////////////////////////////////////////////////////////
// The D3D11Texture1D class
class D3D11Texture1D : public D3D11Texture
{
public:
    D3D11Texture1D( D3D11Renderer * pRenderer );
    virtual ~D3D11Texture1D();

    // Deferred construction
    Void Create( UInt iResourceBinds, PixelFormat iFormat, UInt iWidth, UInt iMipLevelCount = D3D11TEXTURE_MIPLEVELS_NONE, UInt iArrayCount = 1, const Void * pData = NULL );

    // Getters
    inline virtual D3D11ResourceType GetType() const;

    inline virtual PixelFormat GetFormat() const;
    inline virtual UInt GetBound( UInt iDimension, UInt iMipLevel = 0 ) const;

    inline virtual UInt GetMipLevelCount() const;
    inline virtual UInt GetArrayCount() const;

    // Methods
    Void * Lock( const D3D11SubResourceIndex * pIndex, D3D11ResourceLock iLockType, UInt iResourceLockFlags, D3D11DeferredContext * pContext = NULL );
    Void UnLock( const D3D11SubResourceIndex * pIndex, D3D11DeferredContext * pContext = NULL );

    Void Update( const D3D11SubResourceIndex * pIndex, UInt iX, UInt iWidth, const Void * pSrcData, D3D11DeferredContext * pContext = NULL ) const;

    Void Copy( D3D11Texture1D * pDstTexture, D3D11DeferredContext * pContext = NULL ) const;
    Void Copy( D3D11Texture1D * pDstTexture, const D3D11SubResourceIndex * pDstIndex, UInt iDstX, const D3D11SubResourceIndex * pSrcIndex, UInt iSrcX, UInt iSrcWidth, D3D11DeferredContext * pContext = NULL ) const;

protected:
    friend class D3D11Renderer;

    __D3D11_TEXTURE1D_DESC m_hTextureDesc;
    ID3D11Texture1D * m_pTexture;

    // Auto-Regen system
    virtual Void _NakedCreate();
    virtual Void _NakedDestroy();
};

/////////////////////////////////////////////////////////////////////////////////
// The D3D11Texture2D class
class D3D11Texture2D : public D3D11Texture
{
public:
    typedef Void (*_BackBufferResizeCallback)( PixelFormat, UInt, UInt, UInt, Void* );
    D3D11Texture2D( D3D11Renderer * pRenderer, _BackBufferResizeCallback pfCallback, Void * pUserData );
    virtual ~D3D11Texture2D();

    // Deferred construction
    inline Bool IsBoundToBackBuffer( UInt * outBackBuffer = NULL ) const;

    Void AttachToBackBuffer( UInt iBackBuffer );
    Void Create( UInt iResourceBinds, PixelFormat iFormat, UInt iWidth, UInt iHeight, UInt iMipLevelCount = D3D11TEXTURE_MIPLEVELS_NONE, UInt iArrayCount = 1, const Void * pData = NULL );
    virtual Void Destroy();

    // Getters
    inline virtual D3D11ResourceType GetType() const;

    inline virtual PixelFormat GetFormat() const;
    inline virtual UInt GetBound( UInt iDimension, UInt iMipLevel = 0 ) const;

    inline virtual UInt GetMipLevelCount() const;
    inline virtual UInt GetArrayCount() const;

    inline Bool IsMultiSampled() const;
    inline UInt GetSampleCount() const;

    // Methods
    Void SetSampleCount( UInt iSampleCount );

    Void * Lock( const D3D11SubResourceIndex * pIndex, D3D11ResourceLock iLockType, UInt iResourceLockFlags, UInt * outPitch, D3D11DeferredContext * pContext = NULL );
    Void UnLock( const D3D11SubResourceIndex * pIndex, D3D11DeferredContext * pContext = NULL );

    Void Update( const D3D11SubResourceIndex * pIndex, const D3D11Rectangle * pRect, const Void * pSrcData, D3D11DeferredContext * pContext = NULL ) const;

    Void Copy( D3D11Texture2D * pDstTexture, D3D11DeferredContext * pContext = NULL ) const;
    Void Copy( D3D11Texture2D * pDstTexture, const D3D11SubResourceIndex * pDstIndex, const D3D11Point2 * pDstPoint, const D3D11SubResourceIndex * pSrcIndex, const D3D11Rectangle * pSrcRect,
               D3D11DeferredContext * pContext = NULL ) const;

protected:
    friend class D3D11Renderer;
    friend class D2D1RenderingContext;

    __D3D11_TEXTURE2D_DESC m_hTextureDesc;
    ID3D11Texture2D * m_pTexture;

    // Auto-Regen system
    virtual Void _NakedCreate();
    virtual Void _NakedDestroy();

    UInt m_iBoundToBackBuffer;
    _BackBufferResizeCallback m_pfBackBufferResizeCallback;
    Void * m_pBackBufferResizeUserData;
};

/////////////////////////////////////////////////////////////////////////////////
// The D3D11Texture3D class
class D3D11Texture3D : public D3D11Texture
{
public:
    D3D11Texture3D( D3D11Renderer * pRenderer );
    virtual ~D3D11Texture3D();

    // Deferred construction
    Void Create( UInt iResourceBinds, PixelFormat iFormat, UInt iWidth, UInt iHeight, UInt iDepth, UInt iMipLevelCount = D3D11TEXTURE_MIPLEVELS_NONE, const Void * pData = NULL );

    // Getters
    inline virtual D3D11ResourceType GetType() const;

    inline virtual PixelFormat GetFormat() const;
    inline virtual UInt GetBound( UInt iDimension, UInt iMipLevel = 0 ) const;

    inline virtual UInt GetMipLevelCount() const;
    inline virtual UInt GetArrayCount() const;

    // Methods
    Void * Lock( const D3D11SubResourceIndex * pIndex, D3D11ResourceLock iLockType, UInt iResourceLockFlags, UInt * outPitch, UInt * outSlice, D3D11DeferredContext * pContext = NULL );
    Void UnLock( const D3D11SubResourceIndex * pIndex, D3D11DeferredContext * pContext = NULL );

    Void Update( const D3D11SubResourceIndex * pIndex, const D3D11Box * pBox, const Void * pSrcData, D3D11DeferredContext * pContext = NULL ) const;

    Void Copy( D3D11Texture3D * pDstTexture, D3D11DeferredContext * pContext = NULL ) const;
    Void Copy( D3D11Texture3D * pDstTexture, const D3D11SubResourceIndex * pDstIndex, const D3D11Point3 * pDstPoint, const D3D11SubResourceIndex * pSrcIndex, const D3D11Box * pSrcBox,
               D3D11DeferredContext * pContext = NULL ) const;

protected:
    friend class D3D11Renderer;

    __D3D11_TEXTURE3D_DESC m_hTextureDesc;
    ID3D11Texture3D * m_pTexture;

    // Auto-Regen system
    virtual Void _NakedCreate();
    virtual Void _NakedDestroy();
};

/////////////////////////////////////////////////////////////////////////////////
// The D3D11TextureCube class
class D3D11TextureCube : public D3D11Texture
{
public:
    D3D11TextureCube( D3D11Renderer * pRenderer );
    virtual ~D3D11TextureCube();

    // Deferred construction
    Void Create( UInt iResourceBinds, PixelFormat iFormat, UInt iEdgeSize, UInt iMipLevelCount = D3D11TEXTURE_MIPLEVELS_NONE, UInt iArrayCount = 1, const Void * pData = NULL );

    // Getters
    inline virtual D3D11ResourceType GetType() const;

    inline virtual PixelFormat GetFormat() const;
    inline virtual UInt GetBound( UInt iDimension, UInt iMipLevel = 0 ) const;

    inline virtual UInt GetMipLevelCount() const;
    inline virtual UInt GetArrayCount() const;

    inline Bool IsCubeArray() const;
    inline UInt GetCubeCount() const;

    // Methods
    Void * Lock( const D3D11SubResourceIndex * pIndex, D3D11TextureCubeFace iFace, D3D11ResourceLock iLockType, UInt iResourceLockFlags, UInt * outPitch, D3D11DeferredContext * pContext = NULL );
    Void UnLock( const D3D11SubResourceIndex * pIndex, D3D11TextureCubeFace iFace, D3D11DeferredContext * pContext = NULL );

    Void Update( const D3D11SubResourceIndex * pSubResource, D3D11TextureCubeFace iFace, const D3D11Rectangle * pRect, const Void * pSrcData, D3D11DeferredContext * pContext = NULL ) const;

    Void Copy( D3D11TextureCube * pDstTexture, D3D11DeferredContext * pContext = NULL ) const;
    Void Copy( D3D11TextureCube * pDstTexture, const D3D11SubResourceIndex * pDstIndex, D3D11TextureCubeFace iDstFace, const D3D11Point2 * pDstPoint,
               const D3D11SubResourceIndex * pSrcIndex, D3D11TextureCubeFace iSrcFace, const D3D11Rectangle * pSrcRect, D3D11DeferredContext * pContext = NULL ) const;

protected:
    friend class D3D11Renderer;

    __D3D11_TEXTURE2D_DESC m_hTextureDesc;
    ID3D11Texture2D * m_pTexture;

    // Auto-Regen system
    virtual Void _NakedCreate();
    virtual Void _NakedDestroy();
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "D3D11Texture.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_RESOURCES_D3D11TEXTURE_H

