/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Resources/GPUResourceView.cpp
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
// Includes
#include "GPUResourceView.h"

#include "../RenderingManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GPUResourceView implementation
GPUResourceView::GPUResourceView()
{
    m_pResource = NULL;

    m_pDeviceResourceView = NULL;
}
GPUResourceView::~GPUResourceView()
{
    // nothing to do
}

Void GPUResourceView::UnBind()
{
    Assert( IsBound() );

    RenderingFn->_UnRegisterResourceView( this );

    m_pDeviceResourceView->Destroy();
    m_pResource = NULL;
}

/////////////////////////////////////////////////////////////////////////////////
// GPURenderTargetView implementation
GPURenderTargetView::GPURenderTargetView():
    GPUResourceView(), m_hDeviceRenderTargetView( RenderingFn->m_pDeviceRenderer )
{
    m_pDeviceResourceView = &m_hDeviceRenderTargetView;
}
GPURenderTargetView::~GPURenderTargetView()
{
    if ( IsBound() )
        UnBind();
}

Void GPURenderTargetView::BindToBackBuffer( UInt iBackBuffer )
{
    Assert( !(IsBound()) );

    m_hDeviceRenderTargetView.AttachToBackBuffer( iBackBuffer );
    m_pResource = NULL;

    RenderingFn->_RegisterResourceView( this );
}
Void GPURenderTargetView::Bind( GPUBuffer * pBuffer, UInt iIndex, UInt iCount )
{
    Assert( !(IsBound()) );
    Assert( pBuffer->IsBound() );

    m_hDeviceRenderTargetView.Create( pBuffer->m_pDeviceBuffer, iIndex, iCount );
    m_pResource = pBuffer;

    RenderingFn->_RegisterResourceView( this );
}
Void GPURenderTargetView::Bind( GPUTexture * pTexture, UInt iMipSlice, UInt iArraySlice, UInt iArraySliceCount )
{
    Assert( !(IsBound()) );
    Assert( pTexture->IsBound() );

    m_hDeviceRenderTargetView.Create( pTexture->m_pDeviceTexture, iMipSlice, iArraySlice, iArraySliceCount );
    m_pResource = pTexture;

    RenderingFn->_RegisterResourceView( this );
}

/////////////////////////////////////////////////////////////////////////////////
// GPUDepthStencilView implementation
GPUDepthStencilView::GPUDepthStencilView():
    GPUResourceView(), m_hDeviceDepthStencilView( RenderingFn->m_pDeviceRenderer )
{
    m_pDeviceResourceView = &m_hDeviceDepthStencilView;
}
GPUDepthStencilView::~GPUDepthStencilView()
{
    if ( IsBound() )
        UnBind();
}

Void GPUDepthStencilView::BindToBackBuffer( UInt iBackBuffer, Bool bReadOnlyDepth, Bool bReadOnlyStencil )
{
    Assert( !(IsBound()) );

    m_hDeviceDepthStencilView.AttachToBackBuffer( iBackBuffer, bReadOnlyDepth, bReadOnlyStencil );
    m_pResource = NULL;

    RenderingFn->_RegisterResourceView( this );
}
Void GPUDepthStencilView::Bind( GPUTexture * pTexture, UInt iMipSlice, UInt iArraySlice, UInt iArraySliceCount, Bool bReadOnlyDepth, Bool bReadOnlyStencil )
{
    Assert( !(IsBound()) );
    Assert( pTexture->IsBound() );

    m_hDeviceDepthStencilView.Create( pTexture->m_pDeviceTexture, iMipSlice, iArraySlice, iArraySliceCount, bReadOnlyDepth, bReadOnlyStencil );
    m_pResource = pTexture;

    RenderingFn->_RegisterResourceView( this );
}

/////////////////////////////////////////////////////////////////////////////////
// GPUShaderView implementation
GPUShaderView::GPUShaderView():
    GPUResourceView(), m_hDeviceShaderView( RenderingFn->m_pDeviceRenderer )
{
    m_pDeviceResourceView = &m_hDeviceShaderView;
}
GPUShaderView::~GPUShaderView()
{
    if ( IsBound() )
        UnBind();
}

Void GPUShaderView::BindToBackBuffer( UInt iBackBuffer )
{
    Assert( !(IsBound()) );

    m_hDeviceShaderView.AttachToBackBuffer( iBackBuffer );
    m_pResource = NULL;

    RenderingFn->_RegisterResourceView( this );
}
Void GPUShaderView::Bind( GPUBuffer * pBuffer, UInt iIndex, UInt iCount )
{
    Assert( !(IsBound()) );
    Assert( pBuffer->IsBound() );

    m_hDeviceShaderView.Create( pBuffer->m_pDeviceBuffer, iIndex, iCount );
    m_pResource = pBuffer;

    RenderingFn->_RegisterResourceView( this );
}
Void GPUShaderView::Bind( GPUTexture * pTexture, UInt iMostDetailedMip, UInt iMipLevelCount, UInt iArraySlice, UInt iArraySliceCount )
{
    Assert( !(IsBound()) );
    Assert( pTexture->IsBound() );

    m_hDeviceShaderView.Create( pTexture->m_pDeviceTexture, iMostDetailedMip, iMipLevelCount, iArraySlice, iArraySliceCount );
    m_pResource = pTexture;

    RenderingFn->_RegisterResourceView( this );
}

/////////////////////////////////////////////////////////////////////////////////
// GPUUnorderedAccessView implementation
GPUUnorderedAccessView::GPUUnorderedAccessView():
    GPUResourceView(), m_hDeviceUnorderedAccessView( RenderingFn->m_pDeviceRenderer )
{
    m_pDeviceResourceView = &m_hDeviceUnorderedAccessView;
}
GPUUnorderedAccessView::~GPUUnorderedAccessView()
{
    if ( IsBound() )
        UnBind();
}

Void GPUUnorderedAccessView::BindToBackBuffer( UInt iBackBuffer )
{
    Assert( !(IsBound()) );

    m_hDeviceUnorderedAccessView.AttachToBackBuffer( iBackBuffer );
    m_pResource = NULL;

    RenderingFn->_RegisterResourceView( this );
}
Void GPUUnorderedAccessView::Bind( GPUBuffer * pBuffer, UInt iIndex, UInt iCount, Bool bAppendConsume, Bool bUseCounter )
{
    Assert( !(IsBound()) );
    Assert( pBuffer->IsBound() );

    m_hDeviceUnorderedAccessView.Create( pBuffer->m_pDeviceBuffer, iIndex, iCount, bAppendConsume, bUseCounter );
    m_pResource = pBuffer;

    RenderingFn->_RegisterResourceView( this );
}
Void GPUUnorderedAccessView::Bind( GPUTexture * pTexture, UInt iMipSlice, UInt iArraySlice, UInt iArraySliceCount )
{
    Assert( !(IsBound()) );
    Assert( pTexture->IsBound() );

    m_hDeviceUnorderedAccessView.Create( pTexture->m_pDeviceTexture, iMipSlice, iArraySlice, iArraySliceCount );
    m_pResource = pTexture;

    RenderingFn->_RegisterResourceView( this );
}

