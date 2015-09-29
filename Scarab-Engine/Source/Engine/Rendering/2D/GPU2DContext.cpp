/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/2D/GPU2DContext.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU Resource : Core-Class for 2D rendering.
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
#include "GPU2DContext.h"

#include "../RenderingManager.h"

/////////////////////////////////////////////////////////////////////////////////
// GPU2DContext implementation
GPU2DContext::GPU2DContext():
    m_lstBitmaps(), m_lstBrushes(), m_lstMeshes(), m_lstLayers(),
    m_hDevice2DContext( RenderingFn->m_pDeviceRenderer )
{
    m_pRenderTarget = NULL;

    // Resource database
    m_lstBitmaps.UseMemoryContext( RenderingFn->GetMemoryContext(), TEXT("Scratch") );
    m_lstBitmaps.SetComparator( _Compare_GPU2DBitmap );
    m_lstBitmaps.Create();

    m_lstBrushes.UseMemoryContext( RenderingFn->GetMemoryContext(), TEXT("Scratch") );
    m_lstBrushes.SetComparator( _Compare_GPU2DBrush );
    m_lstBrushes.Create();

    m_lstMeshes.UseMemoryContext( RenderingFn->GetMemoryContext(), TEXT("Scratch") );
    m_lstMeshes.SetComparator( _Compare_GPU2DMesh );
    m_lstMeshes.Create();

    m_lstLayers.UseMemoryContext( RenderingFn->GetMemoryContext(), TEXT("Scratch") );
    m_lstLayers.SetComparator( _Compare_GPU2DLayer );
    m_lstLayers.Create();
}
GPU2DContext::~GPU2DContext()
{
    if ( IsBound() )
        UnBind();

    // Resource database
    Assert( m_lstBitmaps.IsEmpty() );
    m_lstBitmaps.Destroy();

    Assert( m_lstBrushes.IsEmpty() );
    m_lstBrushes.Destroy();

    Assert( m_lstMeshes.IsEmpty() );
    m_lstMeshes.Destroy();

    Assert( m_lstLayers.IsEmpty() );
    m_lstLayers.Destroy();
}

Void GPU2DContext::BindToBackBuffer( UInt iBackBuffer )
{
    Assert( !(IsBound()) );

    m_hDevice2DContext.Create( iBackBuffer );
    m_pRenderTarget = NULL;

    RenderingFn->_Register2DContext( this );
}
Void GPU2DContext::BindToTexture( GPUTexture2D * pRenderTarget )
{
    Assert( !(IsBound()) );

    m_hDevice2DContext.Create( &(pRenderTarget->m_hDeviceTexture2D) );
    m_pRenderTarget = pRenderTarget;

    RenderingFn->_Register2DContext( this );
}
Void GPU2DContext::UnBind()
{
    Assert( IsBound() );

    RenderingFn->_UnRegister2DContext( this );

    m_hDevice2DContext.Destroy();
}

GPU2DBitmap * GPU2DContext::CreateBitmap( PixelFormat iFormat, UInt iWidth, UInt iHeight, const Byte * arrBitmap, Bool bCreateCPUData )
{
    RenderingFn->SelectMemory( TEXT("Bitmaps") );
    GPU2DBitmap * pBitmap = New GPU2DBitmap( this, iFormat, iWidth, iHeight, arrBitmap, bCreateCPUData );
    RenderingFn->UnSelectMemory();

    return pBitmap;
}
Void GPU2DContext::DestroyBitmap( GPU2DBitmap * pBitmap ) const
{
    RenderingFn->SelectMemory( TEXT("Bitmaps") );
    Delete( pBitmap );
    RenderingFn->UnSelectMemory();
}


GPU2DMesh * GPU2DContext::CreateMesh()
{
    RenderingFn->SelectMemory( TEXT("2DMeshes") );
    GPU2DMesh * pMesh = New GPU2DMesh( this );
    RenderingFn->UnSelectMemory();

    return pMesh;
}
Void GPU2DContext::DestroyMesh( GPU2DMesh * pMesh ) const
{
    RenderingFn->SelectMemory( TEXT("2DMeshes") );
    Delete( pMesh );
    RenderingFn->UnSelectMemory();
}

/////////////////////////////////////////////////////////////////////////////////

Void GPU2DContext::_OnDestroyDevice()
{
    // Unbind all layers
    _LayerList::Iterator itLayer = m_lstLayers.Begin();
    while( !(itLayer.IsNull()) ) {
        itLayer.Get()->m_hDevice2DLayer.OnDestroyDevice();
        ++itLayer;
    }

    // Unbind all meshes
    _MeshList::Iterator itMesh = m_lstMeshes.Begin();
    while( !(itMesh.IsNull()) ) {
        itMesh.Get()->m_hDevice2DMesh.OnDestroyDevice();
        ++itMesh;
    }

    // Unbind all brushes
    _BrushList::Iterator itBrush = m_lstBrushes.Begin();
    while( !(itBrush.IsNull()) ) {
        itBrush.Get()->m_pDevice2DBrush->OnDestroyDevice();
        ++itBrush;
    }

    // Unbind all bitmaps
    _BitmapList::Iterator itBitmap = m_lstBitmaps.Begin();
    while( !(itBitmap.IsNull()) ) {
        itBitmap.Get()->m_hDevice2DBitmap.OnDestroyDevice();
        ++itBitmap;
    }

    // Unbind 2D Context
    m_hDevice2DContext.OnDestroyDevice();
}
Void GPU2DContext::_OnRestoreDevice()
{
    // Rebind 2D Context
    m_hDevice2DContext.OnRestoreDevice();

    // Rebind all bitmaps
    _BitmapList::Iterator itBitmap = m_lstBitmaps.Begin();
    while( !(itBitmap.IsNull()) ) {
        itBitmap.Get()->m_hDevice2DBitmap.OnRestoreDevice();
        ++itBitmap;
    }

    // Rebind all brushes
    _BrushList::Iterator itBrush = m_lstBrushes.Begin();
    while( !(itBrush.IsNull()) ) {
        itBrush.Get()->m_pDevice2DBrush->OnRestoreDevice();
        ++itBrush;
    }

    // Rebind all meshes
    _MeshList::Iterator itMesh = m_lstMeshes.Begin();
    while( !(itMesh.IsNull()) ) {
        itMesh.Get()->m_hDevice2DMesh.OnRestoreDevice();
        ++itMesh;
    }

    // Rebind all layers
    _LayerList::Iterator itLayer = m_lstLayers.Begin();
    while( !(itLayer.IsNull()) ) {
        itLayer.Get()->m_hDevice2DLayer.OnRestoreDevice();
        ++itLayer;
    }
}

