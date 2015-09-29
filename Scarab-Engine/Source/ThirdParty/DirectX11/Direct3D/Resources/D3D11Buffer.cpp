/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/Resources/D3D11Buffer.cpp
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
// Third-Party Includes
#pragma warning(disable:4005)

#define WIN32_LEAN_AND_MEAN
#include <d3d11.h>

#undef DebugAssert

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "D3D11Buffer.h"

#include "../D3D11Renderer.h"

/////////////////////////////////////////////////////////////////////////////////
// D3D11Buffer implementation
D3D11Buffer::D3D11Buffer( D3D11Renderer * pRenderer ):
    D3D11Resource()
{
    m_pRenderer = pRenderer;

    m_pBuffer = NULL;

    m_bTemporaryDestroyed = false;
    m_hCreationParameters.pData = NULL;
}
D3D11Buffer::~D3D11Buffer()
{
    // nothing to do
}

Void D3D11Buffer::Destroy()
{
    DebugAssert( IsCreated() );
    DebugAssert( !m_bLocked );

    if ( m_bTemporaryDestroyed )
        m_bTemporaryDestroyed = false;
    else
        _NakedDestroy();

    m_iBinds = D3D11RESOURCE_BIND_NONE;

    m_hCreationParameters.pData = NULL;
}

Void D3D11Buffer::OnDestroyDevice()
{
    DebugAssert( !m_bTemporaryDestroyed );

    if ( m_pBuffer != NULL ) {
        _NakedDestroy();
        m_bTemporaryDestroyed = true;
    }
}
Void D3D11Buffer::OnRestoreDevice()
{
    DebugAssert( m_pBuffer == NULL );

    if ( m_bTemporaryDestroyed ) {
        _NakedCreate();
        m_bTemporaryDestroyed = false;
    }
}

Void * D3D11Buffer::Lock( D3D11ResourceLock iLockType, UInt iResourceLockFlags, UInt * outByteSize, D3D11DeferredContext * pContext )
{
    DebugAssert( IsCreated() );
    DebugAssert( CanLock() );
    DebugAssert( !m_bLocked );

    if ( iLockType == D3D11RESOURCE_LOCK_WRITE_DISCARD ) {
        DebugAssert( m_iUsage == D3D11RESOURCE_USAGE_DYNAMIC );
    } else if ( iLockType == D3D11RESOURCE_LOCK_WRITE_NO_OVERWRITE ) {
        DebugAssert( m_iUsage == D3D11RESOURCE_USAGE_DYNAMIC );
        DebugAssert( (m_iBinds & D3D11RESOURCE_BIND_CONSTANT_BUFFER) == 0 );
        DebugAssert( (m_iBinds & D3D11RESOURCE_BIND_SHADER_INPUT) == 0 );
    } else {
        if ( (iLockType & D3D11RESOURCE_LOCK_READ) != 0 ) {
            DebugAssert( CanCPURead() );
        } else if ( (iLockType & D3D11RESOURCE_LOCK_WRITE) != 0 ) {
            DebugAssert( CanCPUWrite() );
        }
    }

    D3D11_MAPPED_SUBRESOURCE hMappedSubResource;
    hMappedSubResource.pData = NULL;
    hMappedSubResource.RowPitch = 0;
    hMappedSubResource.DepthPitch = 0;

    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    HRESULT hRes = pDeviceContext->Map( m_pBuffer, 0, D3D11ResourceLockToD3D11[iLockType], _D3D11ConvertFlags32(D3D11ResourceLockFlagsToD3D11, iResourceLockFlags), &hMappedSubResource );
    DebugAssert( hRes == S_OK && hMappedSubResource.pData != NULL );
    
    *outByteSize = hMappedSubResource.RowPitch;

    m_bLocked = true;

    return hMappedSubResource.pData;
}
Void D3D11Buffer::UnLock( D3D11DeferredContext * pContext )
{
    DebugAssert( IsCreated() );
    DebugAssert( CanLock() );
    DebugAssert( m_bLocked );

    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->Unmap( m_pBuffer, 0 );

    m_bLocked = false;
}

Void D3D11Buffer::Update( UInt iOffset, UInt iSize, const Void * pSrcData, D3D11DeferredContext * pContext ) const
{
    DebugAssert( IsCreated() );
    DebugAssert( CanUpdate() );
    DebugAssert( !m_bLocked );

    DebugAssert( iOffset + iSize <= m_hBufferDesc.ByteWidth );

    D3D11_BOX hDstBox;
    hDstBox.left = iOffset;
    hDstBox.right = iOffset + iSize;
    hDstBox.top = 0;
    hDstBox.bottom = 1;
    hDstBox.front = 0;
    hDstBox.back = 1;

    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->UpdateSubresource( m_pBuffer, 0, (GetType() != D3D11RESOURCE_BUFFER_CONSTANT) ? &hDstBox : NULL, pSrcData, m_hBufferDesc.ByteWidth, 0 );
}

Void D3D11Buffer::Copy( D3D11Buffer * pDstBuffer, D3D11DeferredContext * pContext ) const
{
    DebugAssert( IsCreated() && pDstBuffer->IsCreated() );
    DebugAssert( !m_bLocked && !(pDstBuffer->m_bLocked) );
    DebugAssert( pDstBuffer->CanGPUWrite() );

    DebugAssert( pDstBuffer != this );
    DebugAssert( GetType() == pDstBuffer->GetType() );
    DebugAssert( m_hBufferDesc.StructureByteStride == pDstBuffer->m_hBufferDesc.StructureByteStride );

    DebugAssert( m_hBufferDesc.ByteWidth <= pDstBuffer->m_hBufferDesc.ByteWidth );

    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->CopyResource( pDstBuffer->m_pBuffer, m_pBuffer );
}
Void D3D11Buffer::Copy( D3D11Buffer * pDstBuffer, UInt iDstOffset, UInt iSrcOffset, UInt iSize, D3D11DeferredContext * pContext ) const
{
    DebugAssert( IsCreated() && pDstBuffer->IsCreated() );
    DebugAssert( !m_bLocked && !(pDstBuffer->m_bLocked) );
    DebugAssert( pDstBuffer->CanGPUWrite() );

    DebugAssert( GetType() == pDstBuffer->GetType() );
    DebugAssert( m_hBufferDesc.StructureByteStride == pDstBuffer->m_hBufferDesc.StructureByteStride );

    DebugAssert( iSrcOffset + iSize <= m_hBufferDesc.ByteWidth );
    DebugAssert( iDstOffset + iSize <= pDstBuffer->m_hBufferDesc.ByteWidth );

    D3D11_BOX hSrcBox;
    hSrcBox.left = iSrcOffset;
    hSrcBox.right = iSrcOffset + iSize;
    hSrcBox.top = 0;
    hSrcBox.bottom = 1;
    hSrcBox.front = 0;
    hSrcBox.back = 1;

    ID3D11DeviceContext * pDeviceContext = m_pRenderer->m_pImmediateContext;
    if ( pContext != NULL && pContext->IsCreated() )
        pDeviceContext = pContext->m_pDeferredContext;

    pDeviceContext->CopySubresourceRegion( pDstBuffer->m_pBuffer, 0, iDstOffset, 0, 0, m_pBuffer, 0, &hSrcBox );
}

/////////////////////////////////////////////////////////////////////////////////

Void D3D11Buffer::_NakedCreate()
{
    D3D11_SUBRESOURCE_DATA hInitialization;
    hInitialization.pSysMem = m_hCreationParameters.pData;
    hInitialization.SysMemPitch = 0;
    hInitialization.SysMemSlicePitch = 0;

    m_pBuffer = NULL;
    HRESULT hRes = m_pRenderer->m_pDevice->CreateBuffer( (const D3D11_BUFFER_DESC *)&m_hBufferDesc, (m_hCreationParameters.pData != NULL) ? &hInitialization : NULL, &m_pBuffer );
    DebugAssert( hRes == S_OK && m_pBuffer != NULL );

    m_pResource = NULL;
    hRes = m_pBuffer->QueryInterface( __uuidof(ID3D11Resource), (Void**)&m_pResource );
    DebugAssert( hRes == S_OK && m_pResource != NULL );
}
Void D3D11Buffer::_NakedDestroy()
{
    m_pResource->Release();
    m_pResource = NULL;

    m_pBuffer->Release();
    m_pBuffer = NULL;
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11VertexBuffer implementation
D3D11VertexBuffer::D3D11VertexBuffer( D3D11Renderer * pRenderer ):
    D3D11Buffer( pRenderer )
{
    m_hBufferDesc.StructureByteStride = 0;
    m_hBufferDesc.MiscFlags = 0;
}
D3D11VertexBuffer::~D3D11VertexBuffer()
{
    if ( IsCreated() )
        Destroy();
}

Void D3D11VertexBuffer::Create( UInt iByteSize, const Void * pData )
{
    DebugAssert( !(IsCreated()) );
    DebugAssert( m_iUsage != D3D11RESOURCE_USAGE_CONST || pData != NULL );

    m_iBinds = D3D11RESOURCE_BIND_VERTEX_BUFFER;

    m_hBufferDesc.Usage = D3D11ResourceUsageToD3D11[m_iUsage];
    m_hBufferDesc.CPUAccessFlags = _GetCPUAccessFlags();
    m_hBufferDesc.BindFlags = _D3D11ConvertFlags32( D3D11ResourceBindToD3D11, m_iBinds );
    m_hBufferDesc.ByteWidth = iByteSize;

    m_hCreationParameters.pData = pData;

    _NakedCreate();
}

Bool D3D11VertexBuffer::IsIndirect() const
{
    return ( m_hBufferDesc.MiscFlags == D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS );
}
Void D3D11VertexBuffer::SetIndirect( Bool bDrawIndirect )
{
    DebugAssert( !(IsCreated()) );
    m_hBufferDesc.MiscFlags = (bDrawIndirect) ? D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS : 0;
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11IndexBuffer implementation
D3D11IndexBuffer::D3D11IndexBuffer( D3D11Renderer * pRenderer ):
    D3D11Buffer( pRenderer )
{
    m_hBufferDesc.StructureByteStride = 0;
    m_hBufferDesc.MiscFlags = 0;
}
D3D11IndexBuffer::~D3D11IndexBuffer()
{
    if ( IsCreated() )
        Destroy();
}

Void D3D11IndexBuffer::Create( UInt iByteSize, const Void * pData )
{
    DebugAssert( !(IsCreated()) );
    DebugAssert( m_iUsage != D3D11RESOURCE_USAGE_CONST || pData != NULL );

    m_iBinds = D3D11RESOURCE_BIND_INDEX_BUFFER;

    m_hBufferDesc.Usage = D3D11ResourceUsageToD3D11[m_iUsage];
    m_hBufferDesc.CPUAccessFlags = _GetCPUAccessFlags();
    m_hBufferDesc.BindFlags = _D3D11ConvertFlags32( D3D11ResourceBindToD3D11, m_iBinds );
    m_hBufferDesc.ByteWidth = iByteSize;

    m_hCreationParameters.pData = pData;

    _NakedCreate();
}

Bool D3D11IndexBuffer::IsIndirect() const
{
    return ( m_hBufferDesc.MiscFlags == D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS );
}
Void D3D11IndexBuffer::SetIndirect( Bool bDrawIndirect )
{
    DebugAssert( !(IsCreated()) );
    m_hBufferDesc.MiscFlags = (bDrawIndirect) ? D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS : 0;
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11ConstantBuffer implementation
D3D11ConstantBuffer::D3D11ConstantBuffer( D3D11Renderer * pRenderer ):
    D3D11Buffer( pRenderer )
{
    m_hBufferDesc.StructureByteStride = 0;
    m_hBufferDesc.MiscFlags = 0;
}
D3D11ConstantBuffer::~D3D11ConstantBuffer()
{
    if ( IsCreated() )
        Destroy();
}

Void D3D11ConstantBuffer::Create( UInt iByteSize, const Void * pData )
{
    DebugAssert( !(IsCreated()) );
    DebugAssert( m_iUsage != D3D11RESOURCE_USAGE_CONST || pData != NULL );
    DebugAssert( (iByteSize & 0x0f) == 0 );

    m_iBinds = D3D11RESOURCE_BIND_CONSTANT_BUFFER;

    m_hBufferDesc.Usage = D3D11ResourceUsageToD3D11[m_iUsage];
    m_hBufferDesc.CPUAccessFlags = _GetCPUAccessFlags();
    m_hBufferDesc.BindFlags = _D3D11ConvertFlags32( D3D11ResourceBindToD3D11, m_iBinds );
    m_hBufferDesc.ByteWidth = iByteSize;

    m_hCreationParameters.pData = pData;

    _NakedCreate();
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11RawBuffer implementation
D3D11RawBuffer::D3D11RawBuffer( D3D11Renderer * pRenderer ):
    D3D11Buffer( pRenderer )
{
    m_hBufferDesc.StructureByteStride = 4;
    m_hBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
}
D3D11RawBuffer::~D3D11RawBuffer()
{
    if ( IsCreated() )
        Destroy();
}

Void D3D11RawBuffer::Create( UInt iResourceBinds, UInt iByteSize, const Void * pData )
{
    DebugAssert( !(IsCreated()) );
    DebugAssert( m_iUsage != D3D11RESOURCE_USAGE_CONST || pData != NULL );
    DebugAssert( (iByteSize & 0x03) == 0 );

    m_iBinds = iResourceBinds;

    m_hBufferDesc.Usage = D3D11ResourceUsageToD3D11[m_iUsage];
    m_hBufferDesc.CPUAccessFlags = _GetCPUAccessFlags();
    m_hBufferDesc.BindFlags = _D3D11ConvertFlags32( D3D11ResourceBindToD3D11, m_iBinds );
    m_hBufferDesc.ByteWidth = iByteSize;

    m_hCreationParameters.pData = pData;

    _NakedCreate();
}

/////////////////////////////////////////////////////////////////////////////////
// D3D11StructuredBuffer implementation
D3D11StructuredBuffer::D3D11StructuredBuffer( D3D11Renderer * pRenderer ):
    D3D11Buffer( pRenderer )
{
    m_hBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
}
D3D11StructuredBuffer::~D3D11StructuredBuffer()
{
    if ( IsCreated() )
        Destroy();
}

Void D3D11StructuredBuffer::Create( UInt iResourceBinds, UInt iStructureSize, UInt iStructureCount, const Void * pData )
{
    DebugAssert( !(IsCreated()) );
    DebugAssert( m_iUsage != D3D11RESOURCE_USAGE_CONST || pData != NULL );

    m_iBinds = iResourceBinds;

    m_hBufferDesc.Usage = D3D11ResourceUsageToD3D11[m_iUsage];
    m_hBufferDesc.CPUAccessFlags = _GetCPUAccessFlags();
    m_hBufferDesc.BindFlags = _D3D11ConvertFlags32( D3D11ResourceBindToD3D11, m_iBinds );
    m_hBufferDesc.ByteWidth = ( iStructureSize * iStructureCount );
    m_hBufferDesc.StructureByteStride = iStructureSize;

    m_hCreationParameters.pData = pData;

    _NakedCreate();
}

