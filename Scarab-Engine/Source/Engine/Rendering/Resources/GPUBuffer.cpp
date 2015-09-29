/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Resources/GPUBuffer.cpp
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
// Includes
#include "GPUBuffer.h"

#include "../RenderingManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GPUBuffer implementation
GPUBuffer::GPUBuffer( UInt iElementSize, UInt iElementCount, const Void * arrElements, Bool bCreateCPUData ):
    GPUResource()
{
    m_iElementSize = iElementSize;
    m_iElementCount = iElementCount;
    m_iSize = ( m_iElementSize * m_iElementCount );

    m_arrBuffer = NULL;
    if ( bCreateCPUData ) {
        m_arrBuffer = New Byte[m_iSize];

        if ( arrElements != NULL )
            MemCopy( m_arrBuffer, arrElements, m_iSize );
    }

    m_pDeviceBuffer = NULL;
}
GPUBuffer::~GPUBuffer()
{
    if ( m_arrBuffer != NULL )
        DeleteA( m_arrBuffer );
}

Void GPUBuffer::UnBind()
{
    Assert( IsBound() );

    RenderingFn->_UnRegisterBuffer( this );

    m_pDeviceBuffer->Destroy();
}

Void GPUBuffer::DestroyCPUData()
{
    Assert( HasCPUData() );

    m_pDeviceBuffer->UpdateDataSource( NULL );

    RenderingFn->SelectMemory( TEXT("Buffers") );
    DeleteA( m_arrBuffer );
    m_arrBuffer = NULL;
    RenderingFn->UnSelectMemory();
}
Void GPUBuffer::RestoreCPUData()
{
    Assert( !(HasCPUData()) );

    RenderingFn->SelectMemory( TEXT("Buffers") );
    m_arrBuffer = New Byte[m_iSize];
    RenderingFn->UnSelectMemory();

    m_pDeviceBuffer->UpdateDataSource( m_arrBuffer );
}

Void * GPUBuffer::Lock( GPUResourceLock iLockType, UInt iGPUResourceLockFlags, UInt * outByteSize, GPUDeferredContext * pContext )
{
    Assert( IsBound() );
    Assert( CanLock() );
    Assert( !(IsLocked()) );

    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    return m_pDeviceBuffer->Lock( (DeviceResourceLock)iLockType, iGPUResourceLockFlags, outByteSize, pDeviceContext );
}
Void GPUBuffer::UnLock( GPUDeferredContext * pContext )
{
    Assert( IsBound() );
    Assert( CanLock() );
    Assert( IsLocked() );

    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceBuffer->UnLock( pDeviceContext );
}

Void GPUBuffer::Update( UInt iIndex, UInt iCount, GPUDeferredContext * pContext )
{
    Assert( HasCPUData() && IsBound() );
    Assert( CanUpdate() );
    Assert( !(IsLocked()) );

    Assert( iIndex < m_iElementCount );
    if ( iCount == INVALID_OFFSET )
        iCount = ( m_iElementCount - iIndex );
    Assert( (iIndex + iCount) <= m_iElementCount );

    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceBuffer->Update( (iIndex * m_iElementSize), (iCount * m_iElementSize), m_arrBuffer, pDeviceContext );
}
Void GPUBuffer::UpdateBack( UInt iIndex, UInt iCount, GPUDeferredContext * pContext )
{
    Assert( HasCPUData() && IsBound() );
    Assert( CanLock() && CanCPURead() );
    Assert( !(IsLocked()) );

    Assert( iIndex < m_iElementCount );
    if ( iCount == INVALID_OFFSET )
        iCount = ( m_iElementCount - iIndex );
    Assert( (iIndex + iCount) <= m_iElementCount );

    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    Byte * pData = NULL;
    UInt iByteSize = 0;

    pData = (Byte*)( m_pDeviceBuffer->Lock( DEVICERESOURCE_LOCK_READ, DEVICERESOURCE_LOCKFLAG_NONE, &iByteSize, pDeviceContext ) );

    Assert( iByteSize == m_iSize );
    MemCopy( m_arrBuffer + (iIndex * m_iElementSize), pData + (iIndex * m_iElementSize), (iCount * m_iElementSize) );

    m_pDeviceBuffer->UnLock( pDeviceContext );
}

Void GPUBuffer::CopyCPUData( GPUBuffer * pDstBuffer, Bool bUpdateGPUData, GPUDeferredContext * pContext ) const
{
    Assert( HasCPUData() && pDstBuffer->HasCPUData() );
    Assert( m_iElementSize == pDstBuffer->m_iElementSize );

    Assert( m_iSize <= pDstBuffer->m_iSize );

    MemCopy( pDstBuffer->m_arrBuffer, m_arrBuffer, m_iSize );

    if ( bUpdateGPUData )
        pDstBuffer->Update( 0, m_iSize, pContext );
}
Void GPUBuffer::CopyCPUData( GPUBuffer * pDstBuffer, UInt iDstIndex, UInt iSrcIndex, UInt iCount, Bool bUpdateGPUData, GPUDeferredContext * pContext ) const
{
    Assert( HasCPUData() && pDstBuffer->HasCPUData() );
    Assert( m_iElementSize == pDstBuffer->m_iElementSize );

    Assert( iSrcIndex < m_iElementCount );
    if ( iCount == INVALID_OFFSET )
        iCount = ( m_iElementCount - iSrcIndex );
    Assert( (iSrcIndex + iCount) <= m_iElementCount );
    Assert( (iDstIndex + iCount) <= pDstBuffer->m_iElementCount );

    MemCopy( pDstBuffer->m_arrBuffer + (iDstIndex * pDstBuffer->m_iElementSize), m_arrBuffer + (iSrcIndex * m_iElementSize), (iCount * m_iElementSize) );

    if ( bUpdateGPUData )
        pDstBuffer->Update( iDstIndex, iCount, pContext );
}
Void GPUBuffer::CopyGPUData( GPUBuffer * pDstBuffer, Bool bUpdateCPUData, GPUDeferredContext * pContext ) const
{
    Assert( IsBound() && pDstBuffer->IsBound() );
    Assert( !(IsLocked()) && !(pDstBuffer->IsLocked()) );
    Assert( pDstBuffer->CanGPUWrite() );

    Assert( pDstBuffer != this );
    Assert( GetType() == pDstBuffer->GetType() );
    Assert( m_iElementSize == pDstBuffer->m_iElementSize );

    Assert( m_iSize <= pDstBuffer->m_iSize );

    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceBuffer->Copy( pDstBuffer->m_pDeviceBuffer, pDeviceContext );

    if ( bUpdateCPUData )
        pDstBuffer->UpdateBack( 0, m_iSize, pContext );
}
Void GPUBuffer::CopyGPUData( GPUBuffer * pDstBuffer, UInt iDstIndex, UInt iSrcIndex, UInt iCount, Bool bUpdateCPUData, GPUDeferredContext * pContext ) const
{
    Assert( IsBound() && pDstBuffer->IsBound() );
    Assert( !(IsLocked()) && !(pDstBuffer->IsLocked()) );
    Assert( pDstBuffer->CanGPUWrite() );

    Assert( GetType() == pDstBuffer->GetType() );
    Assert( m_iElementSize == pDstBuffer->m_iElementSize );

    Assert( iSrcIndex < m_iElementCount );
    if ( iCount == INVALID_OFFSET )
        iCount = ( m_iElementCount - iSrcIndex );
    Assert( (iSrcIndex + iCount) <= m_iElementCount );
    Assert( (iDstIndex + iCount) <= pDstBuffer->m_iElementCount );

    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceBuffer->Copy( pDstBuffer->m_pDeviceBuffer, (iDstIndex * pDstBuffer->m_iElementSize), (iSrcIndex * m_iElementSize), (iCount * m_iElementSize), pDeviceContext );

    if ( bUpdateCPUData )
        pDstBuffer->UpdateBack( iDstIndex, iCount, pContext );
}

/////////////////////////////////////////////////////////////////////////////////
// GPUVertexBuffer implementation
GPUVertexBuffer::GPUVertexBuffer( UInt iVertexSize, UInt iVertexCount, const Void * arrVertices, Bool bCreateCPUData ):
    GPUBuffer( iVertexSize, iVertexCount, arrVertices, bCreateCPUData ), m_hDeviceVertexBuffer( RenderingFn->m_pDeviceRenderer )
{
    m_pDeviceBuffer = &m_hDeviceVertexBuffer;
    m_pDeviceResource = m_pDeviceBuffer;
}
GPUVertexBuffer::~GPUVertexBuffer()
{
    if ( IsBound() )
        UnBind();
}

Void GPUVertexBuffer::Bind()
{
    Assert( !(IsBound()) );

    m_hDeviceVertexBuffer.Create( m_iSize, m_arrBuffer );

    RenderingFn->_RegisterBuffer( this );
}

/////////////////////////////////////////////////////////////////////////////////

Bool GPUVertexBuffer::_OnLoad( ResourceDB * pRDB, RDB_ResourceHandle * hResource )
{
    Assert( false );
    return false;
}
Bool GPUVertexBuffer::_OnSave( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) const
{
    Assert( false );
    return false;
}

/////////////////////////////////////////////////////////////////////////////////
// GPUIndexBuffer implementation
GPUIndexBuffer::GPUIndexBuffer( UInt iIndexCount, const Void * arrIndices, Bool bUseShorts, Bool bCreateCPUData ):
    GPUBuffer( (bUseShorts) ? sizeof(Word) : sizeof(DWord), iIndexCount, arrIndices, bCreateCPUData ), m_hDeviceIndexBuffer( RenderingFn->m_pDeviceRenderer )
{
    m_pDeviceBuffer = &m_hDeviceIndexBuffer;
    m_pDeviceResource = m_pDeviceBuffer;
}
GPUIndexBuffer::~GPUIndexBuffer()
{
    if ( IsBound() )
        UnBind();
}

Void GPUIndexBuffer::Bind()
{
    Assert( !(IsBound()) );

    m_hDeviceIndexBuffer.Create( m_iSize, m_arrBuffer );

    RenderingFn->_RegisterBuffer( this );
}

/////////////////////////////////////////////////////////////////////////////////

Bool GPUIndexBuffer::_OnLoad( ResourceDB * pRDB, RDB_ResourceHandle * hResource )
{
    Assert( false );
    return false;
}
Bool GPUIndexBuffer::_OnSave( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) const
{
    Assert( false );
    return false;
}

/////////////////////////////////////////////////////////////////////////////////
// GPUConstantBuffer implementation
GPUConstantBuffer::GPUConstantBuffer( UInt iConstantCount, const GPUConstant * arrConstants, Bool bCreateCPUData ):
    GPUBuffer( sizeof(GPUConstant), iConstantCount, arrConstants, bCreateCPUData ), m_hDeviceConstantBuffer( RenderingFn->m_pDeviceRenderer )
{
    m_pDeviceBuffer = &m_hDeviceConstantBuffer;
    m_pDeviceResource = m_pDeviceBuffer;
}
GPUConstantBuffer::~GPUConstantBuffer()
{
    if ( IsBound() )
        UnBind();
}

Void GPUConstantBuffer::Bind()
{
    Assert( !(IsBound()) );

    m_hDeviceConstantBuffer.Create( m_iSize, m_arrBuffer );

    RenderingFn->_RegisterBuffer( this );
}

/////////////////////////////////////////////////////////////////////////////////

Bool GPUConstantBuffer::_OnLoad( ResourceDB * pRDB, RDB_ResourceHandle * hResource )
{
    Assert( false );
    return false;
}
Bool GPUConstantBuffer::_OnSave( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) const
{
    Assert( false );
    return false;
}

/////////////////////////////////////////////////////////////////////////////////
// GPURawBuffer implementation
GPURawBuffer::GPURawBuffer( UInt iDWordCount, const DWord * arrData, Bool bCreateCPUData ):
    GPUBuffer( sizeof(DWord), iDWordCount, arrData, bCreateCPUData ), m_hDeviceRawBuffer( RenderingFn->m_pDeviceRenderer )
{
    m_pDeviceBuffer = &m_hDeviceRawBuffer;
    m_pDeviceResource = m_pDeviceBuffer;
}
GPURawBuffer::~GPURawBuffer()
{
    if ( IsBound() )
        UnBind();
}

Void GPURawBuffer::Bind( UInt iGPUResourceBinds )
{
    Assert( !(IsBound()) );

    m_hDeviceRawBuffer.Create( iGPUResourceBinds, m_iSize, m_arrBuffer );

    RenderingFn->_RegisterBuffer( this );
}

/////////////////////////////////////////////////////////////////////////////////

Bool GPURawBuffer::_OnLoad( ResourceDB * pRDB, RDB_ResourceHandle * hResource )
{
    Assert( false );
    return false;
}
Bool GPURawBuffer::_OnSave( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) const
{
    Assert( false );
    return false;
}

/////////////////////////////////////////////////////////////////////////////////
// GPUStructuredBuffer implementation
GPUStructuredBuffer::GPUStructuredBuffer( UInt iStructSize, UInt iStructCount, const Void * arrStructs, Bool bCreateCPUData ):
    GPUBuffer( iStructSize, iStructCount, arrStructs, bCreateCPUData ), m_hDeviceStructuredBuffer( RenderingFn->m_pDeviceRenderer )
{
    m_pDeviceBuffer = &m_hDeviceStructuredBuffer;
    m_pDeviceResource = m_pDeviceBuffer;
}
GPUStructuredBuffer::~GPUStructuredBuffer()
{
    if ( IsBound() )
        UnBind();
}

Void GPUStructuredBuffer::Bind( UInt iGPUResourceBinds )
{
    Assert( !(IsBound()) );

    m_hDeviceStructuredBuffer.Create( iGPUResourceBinds, m_iElementSize, m_iElementCount, m_arrBuffer );

    RenderingFn->_RegisterBuffer( this );
}

/////////////////////////////////////////////////////////////////////////////////

Bool GPUStructuredBuffer::_OnLoad( ResourceDB * pRDB, RDB_ResourceHandle * hResource )
{
    Assert( false );
    return false;
}
Bool GPUStructuredBuffer::_OnSave( ResourceDB * pRDB, RDB_ResourceHandle * hResource ) const
{
    Assert( false );
    return false;
}

