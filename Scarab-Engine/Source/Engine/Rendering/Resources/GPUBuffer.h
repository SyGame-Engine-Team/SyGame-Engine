/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Resources/GPUBuffer.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU Resource : Buffers
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
#ifndef SCARAB_ENGINE_RENDERING_RESOURCES_GPUBUFFER_H
#define SCARAB_ENGINE_RENDERING_RESOURCES_GPUBUFFER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "GPUResource.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// GPUConstantBuffer definitions
#define GPUCONSTANT_REGISTER_SIZE            DEVICECONSTANTBUFFER_REGISTER_SIZE
#define GPUCONSTANT_REGISTER_COMPONENT_SIZE  DEVICECONSTANTBUFFER_REGISTER_COMPONENT_SIZE
#define GPUCONSTANT_REGISTER_COMPONENT_COUNT DEVICECONSTANTBUFFER_REGISTER_COMPONENT_COUNT

typedef union _gpu_constant {
    Byte Bytes[GPUCONSTANT_REGISTER_SIZE];
    Word Words[GPUCONSTANT_REGISTER_SIZE >> 1];
    DWord DWords[GPUCONSTANT_REGISTER_SIZE >> 2];
    Float Floats[GPUCONSTANT_REGISTER_SIZE >> 2];
    union {
        Byte Bytes[GPUCONSTANT_REGISTER_COMPONENT_SIZE];
        Word Words[GPUCONSTANT_REGISTER_COMPONENT_SIZE >> 1];
        DWord iValue;
        Float fValue;
    } Components[GPUCONSTANT_REGISTER_COMPONENT_COUNT];
} GPUConstant;

// Prototypes
class RenderingManager;

/////////////////////////////////////////////////////////////////////////////////
// The GPUBuffer class
class GPUBuffer : public GPUResource
{
protected:
    friend class RenderingManager;
    GPUBuffer( UInt iElementSize, UInt iElementCount, const Void * arrElements = NULL, Bool bCreateCPUData = true );
    virtual ~GPUBuffer();

public:
    // Binding
    inline Bool IsBound() const;

    Void UnBind();

    inline Bool HasCPUData() const;

    Void DestroyCPUData(); // Content will no longer be safe from device loss.
    Void RestoreCPUData(); // Content is NOT restored, user must either use UpdateBack when possible
                           // or manually refill buffer data then commit to GPU.

    // Getters
    inline virtual ResourceType GetResourceType() const;

    inline UInt GetElementSize() const;
    inline UInt GetElementCount() const;
    inline UInt GetSize() const;

    inline const Byte * GetData( UInt iIndex = 0 ) const;
    inline Byte * GetData( UInt iIndex = 0 );
    inline Void SetData( const Void * arrElements, UInt iIndex = 0, UInt iCount = INVALID_OFFSET );

    template<typename T> inline const T * GetElement( UInt iIndex = 0 ) const;
    template<typename T> inline T * GetElement( UInt iIndex = 0 );
    template<typename T> inline Void SetElement( const T * pElement, UInt iIndex );
    template<typename T> inline Void SetElements( const T * arrElements, UInt iIndex = 0, UInt iCount = INVALID_OFFSET );

    // Methods
    Void * Lock( GPUResourceLock iLockType, UInt iGPUResourceLockFlags, UInt * outByteSize, GPUDeferredContext * pContext = NULL );
    Void UnLock( GPUDeferredContext * pContext = NULL );

    Void Update( UInt iIndex = 0, UInt iCount = INVALID_OFFSET, GPUDeferredContext * pContext = NULL );     // CPU => GPU
    Void UpdateBack( UInt iIndex = 0, UInt iCount = INVALID_OFFSET, GPUDeferredContext * pContext = NULL ); // GPU => CPU (requires staging resource with CPU-Read enabled)

    Void CopyCPUData( GPUBuffer * pDstBuffer, Bool bUpdateGPUData = false, GPUDeferredContext * pContext = NULL ) const;
    Void CopyCPUData( GPUBuffer * pDstBuffer, UInt iDstIndex, UInt iSrcIndex = 0, UInt iCount = INVALID_OFFSET, Bool bUpdateGPUData = false, GPUDeferredContext * pContext = NULL ) const;
    Void CopyGPUData( GPUBuffer * pDstBuffer, Bool bUpdateCPUData = false, GPUDeferredContext * pContext = NULL ) const;
    Void CopyGPUData( GPUBuffer * pDstBuffer, UInt iDstIndex, UInt iSrcIndex = 0, UInt iCount = INVALID_OFFSET, Bool bUpdateCPUData = false, GPUDeferredContext * pContext = NULL ) const;

protected:
    // Access to Views
    friend class GPURenderTargetView;
    friend class GPUShaderView;
    friend class GPUUnorderedAccessView;

    // Resource management
    virtual Bool _OnLoad( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) = 0;
    virtual Bool _OnSave( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) const = 0;

    // Buffer data
    UInt m_iElementSize;
    UInt m_iElementCount;
    UInt m_iSize;
    Byte * m_arrBuffer;

    // Device data
    DeviceBuffer * m_pDeviceBuffer;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPUVertexBuffer class
class GPUVertexBuffer : public GPUBuffer
{
protected:
    friend class RenderingManager;
    GPUVertexBuffer( UInt iVertexSize, UInt iVertexCount, const Void * arrVertices = NULL, Bool bCreateCPUData = true );
    virtual ~GPUVertexBuffer();

public:
    // Binding
    Void Bind();

    // Methods
    inline Bool IsIndirect() const;
    inline Void SetIndirect( Bool bDrawIndirect );

protected:
    // Resource management
    virtual Bool _OnLoad( ResourceDB * pRDB, RDB_ResourceHandle * hResource );
    virtual Bool _OnSave( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) const;

    // Device data
    DeviceVertexBuffer m_hDeviceVertexBuffer;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPUIndexBuffer class
class GPUIndexBuffer : public GPUBuffer
{
protected:
    friend class RenderingManager;
    GPUIndexBuffer( UInt iIndexCount, const Void * arrIndices = NULL, Bool bUseShorts = false, Bool bCreateCPUData = true );
    virtual ~GPUIndexBuffer();

public:
    // Binding
    Void Bind();

    // Methods
    inline Bool UseShorts() const;
    inline Bool UseLongs() const;

    inline Bool IsIndirect() const;
    inline Void SetIndirect( Bool bDrawIndirect );

protected:
    // Resource management
    virtual Bool _OnLoad( ResourceDB * pRDB, RDB_ResourceHandle * hResource );
    virtual Bool _OnSave( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) const;

    // Device data
    DeviceIndexBuffer m_hDeviceIndexBuffer;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPUConstantBuffer class
class GPUConstantBuffer : public GPUBuffer
{
protected:
    friend class RenderingManager;
    GPUConstantBuffer( UInt iConstantCount, const GPUConstant * arrConstants = NULL, Bool bCreateCPUData = true );
    virtual ~GPUConstantBuffer();

public:
    // Binding
    Void Bind();

protected:
    // Resource management
    virtual Bool _OnLoad( ResourceDB * pRDB, RDB_ResourceHandle * hResource );
    virtual Bool _OnSave( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) const;

    // Device data
    DeviceConstantBuffer m_hDeviceConstantBuffer;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPURawBuffer class
class GPURawBuffer : public GPUBuffer
{
protected:
    friend class RenderingManager;
    GPURawBuffer( UInt iDWordCount, const DWord * arrData = NULL, Bool bCreateCPUData = true );
    virtual ~GPURawBuffer();

public:
    // Binding
    Void Bind( UInt iGPUResourceBinds );

protected:
    // Resource management
    virtual Bool _OnLoad( ResourceDB * pRDB, RDB_ResourceHandle * hResource );
    virtual Bool _OnSave( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) const;

    // Device data
    DeviceRawBuffer m_hDeviceRawBuffer;
};

/////////////////////////////////////////////////////////////////////////////////
// The GPUStructuredBuffer class
class GPUStructuredBuffer : public GPUBuffer
{
protected:
    friend class RenderingManager;
    GPUStructuredBuffer( UInt iStructSize, UInt iStructCount, const Void * arrStructs = NULL, Bool bCreateCPUData = true );
    virtual ~GPUStructuredBuffer();

public:
    // Binding
    Void Bind( UInt iGPUResourceBinds );

protected:
    // Resource management
    virtual Bool _OnLoad( ResourceDB * pRDB, RDB_ResourceHandle * hResource );
    virtual Bool _OnSave( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) const;

    // Device data
    DeviceStructuredBuffer m_hDeviceStructuredBuffer;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "GPUBuffer.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_RENDERING_RESOURCES_GPUBUFFERs_H


