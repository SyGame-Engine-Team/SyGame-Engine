/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/Resources/D3D11Buffer.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU Resources : Buffers.
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
#ifndef SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_RESOURCES_D3D11BUFFER_H
#define SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_RESOURCES_D3D11BUFFER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "D3D11Resource.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Prototypes
class D3D11Renderer;

/////////////////////////////////////////////////////////////////////////////////
// The D3D11Buffer class
class D3D11Buffer : public D3D11Resource
{
protected:
    D3D11Buffer( D3D11Renderer * pRenderer );
public:
    virtual ~D3D11Buffer();

    // Deferred construction
    inline virtual Bool IsCreated() const;
    inline Void UpdateDataSource( const Void * pData );

    Void Destroy();
    
    Void OnDestroyDevice();
    Void OnRestoreDevice();

    // Getters
    virtual D3D11ResourceType GetType() const = 0;

    inline UInt GetByteSize() const;
    inline UInt GetStride() const;

    // Methods
    Void * Lock( D3D11ResourceLock iLockType, UInt iResourceLockFlags, UInt * outByteSize, D3D11DeferredContext * pContext = NULL );
    Void UnLock( D3D11DeferredContext * pContext = NULL );

    Void Update( UInt iOffset, UInt iSize, const Void * pSrcData, D3D11DeferredContext * pContext = NULL ) const;

    Void Copy( D3D11Buffer * pDstBuffer, D3D11DeferredContext * pContext = NULL ) const;
    Void Copy( D3D11Buffer * pDstBuffer, UInt iDstOffset, UInt iSrcOffset, UInt iSize, D3D11DeferredContext * pContext = NULL ) const;

protected:
    friend class D3D11Renderer;
    D3D11Renderer * m_pRenderer;

    // Buffer data
    __D3D11_BUFFER_DESC m_hBufferDesc;
    ID3D11Buffer * m_pBuffer;

    // Auto-Regen system
    Void _NakedCreate();
    Void _NakedDestroy();

    typedef struct _creation_parameters {
        const Void * pData;
    } _CreationParameters;

    Bool m_bTemporaryDestroyed;
    _CreationParameters m_hCreationParameters;
};

/////////////////////////////////////////////////////////////////////////////////
// The D3D11VertexBuffer class
class D3D11VertexBuffer : public D3D11Buffer
{
public:
    D3D11VertexBuffer( D3D11Renderer * pRenderer );
    virtual ~D3D11VertexBuffer();

    // Deferred construction
    Void Create( UInt iByteSize, const Void * pData = NULL );

    // Getters
    inline virtual D3D11ResourceType GetType() const;

    // Methods
    Bool IsIndirect() const;
    Void SetIndirect( Bool bDrawIndirect );
};

/////////////////////////////////////////////////////////////////////////////////
// The D3D11IndexBuffer class
class D3D11IndexBuffer : public D3D11Buffer
{
public:
    D3D11IndexBuffer( D3D11Renderer * pRenderer );
    virtual ~D3D11IndexBuffer();

    // Deferred construction
    Void Create( UInt iByteSize, const Void * pData = NULL );

    // Getters
    inline virtual D3D11ResourceType GetType() const;

    // Methods
    Bool IsIndirect() const;
    Void SetIndirect( Bool bDrawIndirect );
};

/////////////////////////////////////////////////////////////////////////////////
// The D3D11ConstantBuffer class
class D3D11ConstantBuffer : public D3D11Buffer
{
public:
    D3D11ConstantBuffer( D3D11Renderer * pRenderer );
    virtual ~D3D11ConstantBuffer();

    // Deferred construction
    Void Create( UInt iByteSize, const Void * pData = NULL );

    // Getters
    inline virtual D3D11ResourceType GetType() const;
};

/////////////////////////////////////////////////////////////////////////////////
// The D3D11RawBuffer class
class D3D11RawBuffer : public D3D11Buffer
{
public:
    D3D11RawBuffer( D3D11Renderer * pRenderer );
    virtual ~D3D11RawBuffer();

    // Deferred construction
    Void Create( UInt iResourceBinds, UInt iByteSize, const Void * pData = NULL );

    // Getters
    inline virtual D3D11ResourceType GetType() const;
};

/////////////////////////////////////////////////////////////////////////////////
// The D3D11StructuredBuffer class
class D3D11StructuredBuffer : public D3D11Buffer
{
public:
    D3D11StructuredBuffer( D3D11Renderer * pRenderer );
    virtual ~D3D11StructuredBuffer();

    // Deferred construction
    Void Create( UInt iResourceBinds, UInt iStructureSize, UInt iStructureCount, const Void * pData = NULL );

    // Getters
    inline virtual D3D11ResourceType GetType() const;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "D3D11Buffer.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_THIRDPARTY_DIRECTX11_DIRECT3D_RESOURCES_D3D11BUFFER_H

