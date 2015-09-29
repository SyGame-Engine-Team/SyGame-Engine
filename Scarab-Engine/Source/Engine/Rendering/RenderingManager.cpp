/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/RenderingManager.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : The Core-Class of the Rendering system.
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
#include "RenderingManager.h"

/////////////////////////////////////////////////////////////////////////////////
// RenderingManagerCallbacks implementation
RenderingManagerCallbacks::RenderingManagerCallbacks():
    DeviceRendererCallbacks()
{
    // nothing to do
}
RenderingManagerCallbacks::~RenderingManagerCallbacks()
{
    // nothing to do
}

Void RenderingManagerCallbacks::OnDestroyDevice()
{
    Assert( m_pUserData != NULL )
    RenderingManager * pThis = (RenderingManager*)m_pUserData;
    pThis->_OnDestroyDevice();
}
Void RenderingManagerCallbacks::OnRestoreDevice()
{
    Assert( m_pUserData != NULL )
    RenderingManager * pThis = (RenderingManager*)m_pUserData;
    pThis->_OnRestoreDevice();
}

Void RenderingManagerCallbacks::OnDestroySwapChain()
{
    Assert( m_pUserData != NULL )
    RenderingManager * pThis = (RenderingManager*)m_pUserData;
    pThis->_OnDestroySwapChain();
}
Void RenderingManagerCallbacks::OnRestoreSwapChain()
{
    Assert( m_pUserData != NULL )
    RenderingManager * pThis = (RenderingManager*)m_pUserData;
    pThis->_OnRestoreSwapChain();
}

/////////////////////////////////////////////////////////////////////////////////
// RenderingManager implementation
RenderingManager::RenderingManager():
    Manager<RenderingManager>(), m_hRendererCallbacks(),
    m_lstDeferredContexts(), m_lstInputLayouts(), m_lstRenderStates(), m_lstAsynchronouses(), m_lstBuffers(), m_lstTextures(), m_lstResourceViews(), m_lstShaders(), m_lst2DContexts(),
    m_hScreenViewport()
{
    // Renderer callbacks
    m_hRendererCallbacks.SetUserData( this );

    // Device-dependant delegates
    m_pDeviceWindow = NULL;
    m_pDeviceRenderer = NULL;

    // Screen resources
    m_hScreenViewport.fTopLeftX = 0.0f;
    m_hScreenViewport.fTopLeftY = 0.0f;
    m_hScreenViewport.fWidth = 0.0f;
    m_hScreenViewport.fHeight = 0.0f;
    m_hScreenViewport.fMinDepth = 0.0f;
    m_hScreenViewport.fMaxDepth = 0.0f;

    m_pScreenDepthStencil = NULL;
    m_pScreenDepthStencilView = NULL;
    m_pScreenRenderTargetView = NULL;

    m_pScreen2DContext = NULL;

    // Screen camera
    m_pScreenDefaultCamera = NULL;
    m_pScreenDefault2DCamera = NULL;
    m_pScreenCamera = NULL;

    // Batched rendering
    m_iFreeBatchesCount = RENDERING_BATCH_COUNT;
    m_iNextFreeBatchID = 0;
    MemZero( m_arrBatches, RENDERING_BATCH_COUNT * sizeof(_RenderingBatch) );
    for( UInt i = 0; i < RENDERING_BATCH_COUNT; ++i )
        m_arrBatches[i].iNextFreeID = (i + 1);
    m_arrBatches[RENDERING_BATCH_COUNT - 1].iNextFreeID = INVALID_OFFSET;

    //MemZero( m_arrFonts, RENDERING_FONT_COUNT * sizeof(_FontData) );
    //m_iFontCount = 0;
    //m_iFontSelected = INVALID_OFFSET;

    //m_pBatch2DVF = NULL;
    //m_pBatch2DEffect = NULL;
    //for( UInt i = 0; i < RENDERING_BATCH2D_COUNT; ++i )
    //    m_arrBatch2DEffectInstance[i] = NULL;
    //MemZero( m_arrBatch2D, RENDERING_BATCH2D_COUNT * sizeof(_RenderingBatch) );

    //m_pBatchTextVF = NULL;
    //m_pBatchTextEffect = NULL;
    //for( UInt i = 0; i < RENDERING_BATCHTEXT_COUNT; ++i )
    //    m_arrBatchTextEffectInstance[i] = NULL;
    //MemZero( m_arrBatchText, RENDERING_BATCHTEXT_COUNT * sizeof(_RenderingBatch) );

    //m_pBatchDebugVF = NULL;
    //m_pBatchDebugEffect = NULL;
    //for( UInt i = 0; i < RENDERING_BATCHDEBUG_COUNT; ++i )
    //    m_arrBatchDebugEffectInstance[i] = NULL;
    //MemZero( m_arrBatchDebug, RENDERING_BATCHDEBUG_COUNT * sizeof(_RenderingBatch) );
}
RenderingManager::~RenderingManager()
{
    // nothing to do
}

Void RenderingManager::Initialize( DeviceWindow * pDeviceWindow )
{
    Assert( m_pDeviceWindow == NULL && pDeviceWindow != NULL );

    // Store our window
    m_pDeviceWindow = pDeviceWindow;

    // Resource database
    m_lstDeferredContexts.UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
    m_lstDeferredContexts.SetComparator( _Compare_GPUDeferredContext );
    m_lstDeferredContexts.Create();

    m_lstInputLayouts.UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
    m_lstInputLayouts.SetComparator( _Compare_GPUInputLayout );
    m_lstInputLayouts.Create();

    m_lstRenderStates.UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
    m_lstRenderStates.SetComparator( _Compare_GPURenderState );
    m_lstRenderStates.Create();

    m_lstAsynchronouses.UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
    m_lstAsynchronouses.SetComparator( _Compare_GPUAsynchronous );
    m_lstAsynchronouses.Create();

    m_lstBuffers.UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
    m_lstBuffers.SetComparator( _Compare_GPUBuffer );
    m_lstBuffers.Create();

    m_lstTextures.UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
    m_lstTextures.SetComparator( _Compare_GPUTexture );
    m_lstTextures.Create();

    m_lstResourceViews.UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
    m_lstResourceViews.SetComparator( _Compare_GPUResourceView );
    m_lstResourceViews.Create();

    m_lstShaders.UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
    m_lstShaders.SetComparator( _Compare_GPUShader );
    m_lstShaders.Create();

    m_lst2DContexts.UseMemoryContext( GetMemoryContext(), TEXT("Scratch") );
    m_lst2DContexts.SetComparator( _Compare_GPU2DContext );
    m_lst2DContexts.Create();

    // Device-dependant delegate
    SelectMemory( TEXT("Scratch") );
    m_pDeviceRenderer = New DeviceRenderer( m_pDeviceWindow, &m_hRendererCallbacks );
    UnSelectMemory();

    // Screen resources
    const DeviceSwapChainDesc * pSwapChainDesc = m_pDeviceRenderer->GetSwapChainDesc();
    
    m_hScreenViewport.fTopLeftX = 0.0f;
    m_hScreenViewport.fTopLeftY = 0.0f;
    m_hScreenViewport.fWidth = (Float)( pSwapChainDesc->iWidth );
    m_hScreenViewport.fHeight = (Float)( pSwapChainDesc->iHeight );
    m_hScreenViewport.fMinDepth = 0.0f;
    m_hScreenViewport.fMaxDepth = 1.0f;

    m_pScreenDepthStencil = CreateTexture2D( PIXEL_FMT_D24UN_S8UI, pSwapChainDesc->iWidth, pSwapChainDesc->iHeight, 1, 1, pSwapChainDesc->iSampleCount, NULL, false );
    m_pScreenDepthStencil->SetUsage( GPURESOURCE_USAGE_DEFAULT );
    m_pScreenDepthStencil->Bind( GPURESOURCE_BIND_DEPTH_STENCIL );

    SelectMemory( TEXT("Scratch") );
    m_pScreenDepthStencilView = New GPUDepthStencilView();
    m_pScreenRenderTargetView = New GPURenderTargetView();
    m_pScreen2DContext = New GPU2DContext();
    UnSelectMemory();

    m_pScreenDepthStencilView->Bind( m_pScreenDepthStencil, 0, 0, 1, false, false );
    m_pScreenRenderTargetView->BindToBackBuffer( 0 );
    m_pScreen2DContext->BindToBackBuffer( 0 );
    
    RSEnableViewports( 1, &m_hScreenViewport, NULL );
    OMEnableRenderTarget( m_pScreenRenderTargetView, m_pScreenDepthStencilView, NULL );

    // Camera
#if ( defined(RENDERER_USE_DIRECT3D11) || defined(RENDERER_USE_SOFTWARE) )
    Camera::SetDefaultDepthMapping( PROJECT_DEPTH_ZERO_ONE );
#elif ( defined(RENDERER_USE_OPENGL) )
    Camera::SetDefaultDepthMapping( PROJECT_DEPTH_MINUSONE_ONE );
#endif

    SelectMemory( TEXT("Scratch") );
    m_pScreenDefaultCamera = New Camera( true );
    m_pScreenDefault2DCamera = New Camera( false );
    UnSelectMemory();

    m_pScreenDefaultCamera->SetView( Vertex3::Null, Vector3::eJ, Vector3::eK );
    m_pScreenDefaultCamera->SetFrustrum( SCALAR_PI_2, m_hScreenViewport.fWidth / m_hScreenViewport.fHeight, 1.0f, 1000.0f );

    m_pScreenDefault2DCamera->SetView( Vertex3::Null, Vector3::eK, -Vector3::eJ );
    m_pScreenDefault2DCamera->SetFrustrum( -0.5f, 0.5f, -0.5f, 0.5f, 0.0f, 1.0f );

    m_pScreenCamera = m_pScreenDefaultCamera;
}
Void RenderingManager::Cleanup()
{
    Assert( m_pDeviceWindow != NULL );

    // Disable everything
    Clear( NULL );

    // Camera
    m_pScreenCamera = NULL;
    
    SelectMemory( TEXT("Scratch") );
    Delete( m_pScreenDefault2DCamera );
    Delete( m_pScreenDefaultCamera );
    UnSelectMemory();

    m_pScreenDefault2DCamera = NULL;
    m_pScreenDefaultCamera = NULL;

    // Screen resources
    RSDisableViewports( NULL );
    OMDisableAllRenderTargets( NULL );

    m_pScreen2DContext->UnBind();
    m_pScreenRenderTargetView->UnBind();
    m_pScreenDepthStencilView->UnBind();
    m_pScreenDepthStencil->UnBind();

    SelectMemory( TEXT("Scratch") );
    Delete( m_pScreen2DContext );
    Delete( m_pScreenRenderTargetView );
    Delete( m_pScreenDepthStencilView );
    UnSelectMemory();
    DestroyResource( m_pScreenDepthStencil );

    m_pScreen2DContext = NULL;
    m_pScreenRenderTargetView = NULL;
    m_pScreenDepthStencilView = NULL;
    m_pScreenDepthStencil = NULL;

    // Device-dependant delegates
    SelectMemory( TEXT("Scratch") );
    Delete( m_pDeviceRenderer );
    UnSelectMemory();

    m_pDeviceRenderer = NULL;
    m_pDeviceWindow = NULL;

    // Resource database
    Assert( m_lstDeferredContexts.IsEmpty() );
    m_lstDeferredContexts.Destroy();

    Assert( m_lstInputLayouts.IsEmpty() );
    m_lstInputLayouts.Destroy();

    Assert( m_lstRenderStates.IsEmpty() );
    m_lstRenderStates.Destroy();

    Assert( m_lstAsynchronouses.IsEmpty() );
    m_lstAsynchronouses.Destroy();

    Assert( m_lstBuffers.IsEmpty() );
    m_lstBuffers.Destroy();

    Assert( m_lstTextures.IsEmpty() );
    m_lstTextures.Destroy();

    Assert( m_lstResourceViews.IsEmpty() );
    m_lstResourceViews.Destroy();

    Assert( m_lstShaders.IsEmpty() );
    m_lstShaders.Destroy();

    Assert( m_lst2DContexts.IsEmpty() );
    m_lst2DContexts.Destroy();
}

GPUVertexBuffer * RenderingManager::CreateVertexBuffer( UInt iVertexSize, UInt iVertexCount, const Void * arrVertices, Bool bCreateCPUData ) const
{
    SelectMemory( TEXT("Buffers") );
    GPUVertexBuffer * pVB = New GPUVertexBuffer( iVertexSize, iVertexCount, arrVertices, bCreateCPUData );
    UnSelectMemory();

    return pVB;
}
GPUIndexBuffer * RenderingManager::CreateIndexBuffer( UInt iIndexCount, const Void * arrIndices, Bool bUseShorts, Bool bCreateCPUData ) const
{
    SelectMemory( TEXT("Buffers") );
    GPUIndexBuffer * pIB = New GPUIndexBuffer( iIndexCount, arrIndices, bUseShorts, bCreateCPUData );
    UnSelectMemory();

    return pIB;
}
GPUConstantBuffer * RenderingManager::CreateConstantBuffer( UInt iConstantCount, const GPUConstant * arrConstants, Bool bCreateCPUData ) const
{
    SelectMemory( TEXT("Buffers") );
    GPUConstantBuffer * pCB = New GPUConstantBuffer( iConstantCount, arrConstants, bCreateCPUData );
    UnSelectMemory();

    return pCB;
}
GPURawBuffer * RenderingManager::CreateRawBuffer( UInt iDWordCount, const DWord * arrDWords, Bool bCreateCPUData ) const
{
    SelectMemory( TEXT("Buffers") );
    GPURawBuffer * pRB = New GPURawBuffer( iDWordCount, arrDWords, bCreateCPUData );
    UnSelectMemory();

    return pRB;
}
GPUStructuredBuffer * RenderingManager::CreateStructuredBuffer( UInt iStructSize, UInt iStructCount, const Void * arrStructs, Bool bCreateCPUData ) const
{
    SelectMemory( TEXT("Buffers") );
    GPUStructuredBuffer * pSB = New GPUStructuredBuffer( iStructSize, iStructCount, arrStructs, bCreateCPUData );
    UnSelectMemory();

    return pSB;
}

GPUTexture1D * RenderingManager::CreateTexture1D( PixelFormat iFormat, UInt iWidth, UInt iMipLevelCount, UInt iArrayCount, const Byte * arrTexels, Bool bCreateCPUData ) const
{
    SelectMemory( TEXT("Textures") );
    GPUTexture1D * pT1D = New GPUTexture1D( iFormat, iWidth, iMipLevelCount, iArrayCount, arrTexels, bCreateCPUData );
    UnSelectMemory();

    return pT1D;
}
GPUTexture2D * RenderingManager::CreateTexture2D( PixelFormat iFormat, UInt iWidth, UInt iHeight, UInt iMipLevelCount, UInt iArrayCount, UInt iSampleCount, const Byte * arrTexels, Bool bCreateCPUData ) const
{
    SelectMemory( TEXT("Textures") );
    GPUTexture2D * pT2D = New GPUTexture2D( iFormat, iWidth, iHeight, iMipLevelCount, iArrayCount, iSampleCount, arrTexels, bCreateCPUData );
    UnSelectMemory();

    return pT2D;
}
GPUTexture3D * RenderingManager::CreateTexture3D( PixelFormat iFormat, UInt iWidth, UInt iHeight, UInt iDepth, UInt iMipLevelCount, const Byte * arrTexels, Bool bCreateCPUData ) const
{
    SelectMemory( TEXT("Textures") );
    GPUTexture3D * pT3D = New GPUTexture3D( iFormat, iWidth, iHeight, iDepth, iMipLevelCount, arrTexels, bCreateCPUData );
    UnSelectMemory();

    return pT3D;
}
GPUTextureCube * RenderingManager::CreateTextureCube( PixelFormat iFormat, UInt iEdgeLength, UInt iMipLevelCount, UInt iCubeCount, const Byte * arrTexels, Bool bCreateCPUData ) const
{
    SelectMemory( TEXT("Textures") );
    GPUTextureCube * pTC = New GPUTextureCube( iFormat, iEdgeLength, iMipLevelCount, iCubeCount, arrTexels, bCreateCPUData );
    UnSelectMemory();

    return pTC;
}

Void RenderingManager::DestroyResource( GPUResource * pResource ) const
{
    if ( pResource->IsBuffer() )
        SelectMemory( TEXT("Buffers") );
    else
        SelectMemory( TEXT("Textures") );
    Delete( pResource );
    UnSelectMemory();
}

Void RenderingManager::IAEnableIL( GPUInputLayout * pIL, GPUDeferredContext * pContext )
{
    Assert( pIL->IsBound() );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->IASetInputLayout( &(pIL->m_hDeviceInputLayout), pDeviceContext );
}
Void RenderingManager::IADisableIL( GPUDeferredContext * pContext )
{
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->IASetInputLayout( NULL, pDeviceContext );
}

Void RenderingManager::IAEnablePT( GPUShaderPrimitiveTopology iPrimitiveTopology, GPUDeferredContext * pContext )
{
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->IASetPrimitiveTopology( (DeviceShaderPrimitiveTopology)iPrimitiveTopology, pDeviceContext );
}
Void RenderingManager::IADisablePT( GPUDeferredContext * pContext )
{
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->IASetPrimitiveTopology( DEVICESHADER_PRIMITIVETOPOLOGY_UNDEFINED, pDeviceContext );
}

Void RenderingManager::IAEnableVB( UInt iSlot, GPUVertexBuffer * pVB, UInt iOffset, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_VERTEXBUFFER_SLOTS );
    Assert( pVB->IsBound() );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceVertexBuffer * pInstance = &(pVB->m_hDeviceVertexBuffer);
    UInt iStride = pVB->GetElementSize();

    m_pDeviceRenderer->IASetVertexBuffers( iSlot, 1, &pInstance, &iStride, &iOffset, pDeviceContext );
}
Void RenderingManager::IAEnableVBs( UInt iSlot, UInt iVBCount, GPUVertexBuffer ** arrVBs, const UInt * arrOffsets, GPUDeferredContext * pContext )
{
    Assert( iSlot + iVBCount <= DEVICERENDERER_MAX_VERTEXBUFFER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceVertexBuffer * arrInstances[DEVICERENDERER_MAX_VERTEXBUFFER_SLOTS];
    UInt arrStrides[DEVICERENDERER_MAX_VERTEXBUFFER_SLOTS];
    for( UInt i = 0; i < iVBCount; ++i ) {
        Assert( arrVBs[i]->IsBound() );
        arrInstances[i] = &(arrVBs[i]->m_hDeviceVertexBuffer);
        arrStrides[i] = arrVBs[i]->GetElementSize();
    }

    m_pDeviceRenderer->IASetVertexBuffers( iSlot, iVBCount, arrInstances, arrStrides, arrOffsets, pDeviceContext );
}
Void RenderingManager::IADisableVB( UInt iSlot, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_VERTEXBUFFER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->IASetVertexBuffers( iSlot, 1, NULL, NULL, NULL, pDeviceContext );
}
Void RenderingManager::IADisableVBs( UInt iSlot, UInt iVBCount, GPUDeferredContext * pContext )
{
    Assert( iSlot + iVBCount <= DEVICERENDERER_MAX_VERTEXBUFFER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->IASetVertexBuffers( iSlot, iVBCount, NULL, NULL, NULL, pDeviceContext );
}
Void RenderingManager::IADisableAllVBs( GPUDeferredContext * pContext )
{
    IADisableVBs( 0, DEVICERENDERER_MAX_VERTEXBUFFER_SLOTS, pContext );
}

Void RenderingManager::IAEnableIB( GPUIndexBuffer * pIB, UInt iOffset, GPUDeferredContext * pContext )
{
    Assert( pIB->IsBound() );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->IASetIndexBuffer( &(pIB->m_hDeviceIndexBuffer), pIB->UseShorts(), iOffset, pDeviceContext );
}
Void RenderingManager::IADisableIB( GPUDeferredContext * pContext )
{
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->IASetIndexBuffer( NULL, false, 0, pDeviceContext );
}

Void RenderingManager::VSEnable( GPUVertexShader * pVS, GPUDeferredContext * pContext )
{
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->VSSetShader( &(pVS->m_hDeviceVertexShader), pDeviceContext );
}
Void RenderingManager::VSDisable( GPUDeferredContext * pContext )
{
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->VSSetShader( NULL, pDeviceContext );
}

Void RenderingManager::VSEnableCB( UInt iSlot, GPUConstantBuffer * pCB, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS );
    Assert( pCB->IsBound() );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceConstantBuffer * pInstance = &(pCB->m_hDeviceConstantBuffer);

    m_pDeviceRenderer->VSSetConstantBuffers( iSlot, 1, &pInstance, pDeviceContext );
}
Void RenderingManager::VSEnableCBs( UInt iSlot, UInt iCBCount, GPUConstantBuffer ** arrCBs, GPUDeferredContext * pContext )
{
    Assert( iSlot + iCBCount <= DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceConstantBuffer * arrInstances[DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS];
    for( UInt i = 0; i < iCBCount; ++i ) {
        Assert( arrCBs[i]->IsBound() );
        arrInstances[i] = &(arrCBs[i]->m_hDeviceConstantBuffer);
    }

    m_pDeviceRenderer->VSSetConstantBuffers( iSlot, iCBCount, arrInstances, pDeviceContext );
}
Void RenderingManager::VSDisableCB( UInt iSlot, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->VSSetConstantBuffers( iSlot, 1, NULL, pDeviceContext );
}
Void RenderingManager::VSDisableCBs( UInt iSlot, UInt iCBCount, GPUDeferredContext * pContext )
{
    Assert( iSlot + iCBCount <= DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->VSSetConstantBuffers( iSlot, iCBCount, NULL, pDeviceContext );
}
Void RenderingManager::VSDisableAllCBs( GPUDeferredContext * pContext )
{
    VSDisableCBs( 0, DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS, pContext );
}

Void RenderingManager::VSEnableSampler( UInt iSlot, GPUSamplerState * pSampler, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS );
    Assert( pSampler->IsBound() );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceSamplerState * pInstance = &(pSampler->m_hDeviceSamplerState);

    m_pDeviceRenderer->VSSetSamplers( iSlot, 1, &pInstance, pDeviceContext );
}
Void RenderingManager::VSEnableSamplers( UInt iSlot, UInt iSamplerCount, GPUSamplerState ** arrSamplers, GPUDeferredContext * pContext )
{
    Assert( iSlot + iSamplerCount <= DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceSamplerState * arrInstances[DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS];
    for( UInt i = 0; i < iSamplerCount; ++i ) {
        Assert( arrSamplers[i]->IsBound() );
        arrInstances[i] = &(arrSamplers[i]->m_hDeviceSamplerState);
    }

    m_pDeviceRenderer->VSSetSamplers( iSlot, iSamplerCount, arrInstances, pDeviceContext );
}
Void RenderingManager::VSDisableSampler( UInt iSlot, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->VSSetSamplers( iSlot, 1, NULL, pDeviceContext );
}
Void RenderingManager::VSDisableSamplers( UInt iSlot, UInt iSamplerCount, GPUDeferredContext * pContext )
{
    Assert( iSlot + iSamplerCount <= DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->VSSetSamplers( iSlot, iSamplerCount, NULL, pDeviceContext );
}
Void RenderingManager::VSDisableAllSamplers( GPUDeferredContext * pContext )
{
    VSDisableSamplers( 0, DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS, pContext );
}

Void RenderingManager::VSEnableResource( UInt iSlot, GPUShaderView * pResource, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceShaderView * pInstance = &(pResource->m_hDeviceShaderView);

    m_pDeviceRenderer->VSSetResources( iSlot, 1, &pInstance, pDeviceContext );
}
Void RenderingManager::VSEnableResources( UInt iSlot, UInt iResourceCount, GPUShaderView ** arrResources, GPUDeferredContext * pContext )
{
    Assert( iSlot + iResourceCount <= DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceShaderView * arrInstances[DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS];
    for( UInt i = 0; i < iResourceCount; ++i )
        arrInstances[i] = &(arrResources[i]->m_hDeviceShaderView);

    m_pDeviceRenderer->VSSetResources( iSlot, iResourceCount, arrInstances, pDeviceContext );
}
Void RenderingManager::VSDisableResource( UInt iSlot, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->VSSetResources( iSlot, 1, NULL, pDeviceContext );
}
Void RenderingManager::VSDisableResources( UInt iSlot, UInt iResourceCount, GPUDeferredContext * pContext )
{
    Assert( iSlot + iResourceCount <= DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->VSSetResources( iSlot, iResourceCount, NULL, pDeviceContext );
}
Void RenderingManager::VSDisableAllResources( GPUDeferredContext * pContext )
{
    VSDisableResources( 0, DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS, pContext );
}

Void RenderingManager::GSEnable( GPUGeometryShader * pGS, GPUDeferredContext * pContext )
{
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->GSSetShader( &(pGS->m_hDeviceGeometryShader), pDeviceContext );
}
Void RenderingManager::GSDisable( GPUDeferredContext * pContext )
{
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->GSSetShader( NULL, pDeviceContext );
}

Void RenderingManager::GSEnableCB( UInt iSlot, GPUConstantBuffer * pCB, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS );
    Assert( pCB->IsBound() );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceConstantBuffer * pInstance = &(pCB->m_hDeviceConstantBuffer);

    m_pDeviceRenderer->GSSetConstantBuffers( iSlot, 1, &pInstance, pDeviceContext );
}
Void RenderingManager::GSEnableCBs( UInt iSlot, UInt iCBCount, GPUConstantBuffer ** arrCBs, GPUDeferredContext * pContext )
{
    Assert( iSlot + iCBCount <= DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceConstantBuffer * arrInstances[DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS];
    for( UInt i = 0; i < iCBCount; ++i ) {
        Assert( arrCBs[i]->IsBound() );
        arrInstances[i] = &(arrCBs[i]->m_hDeviceConstantBuffer);
    }

    m_pDeviceRenderer->GSSetConstantBuffers( iSlot, iCBCount, arrInstances, pDeviceContext );
}
Void RenderingManager::GSDisableCB( UInt iSlot, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->GSSetConstantBuffers( iSlot, 1, NULL, pDeviceContext );
}
Void RenderingManager::GSDisableCBs( UInt iSlot, UInt iCBCount, GPUDeferredContext * pContext )
{
    Assert( iSlot + iCBCount <= DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->GSSetConstantBuffers( iSlot, iCBCount, NULL, pDeviceContext );
}
Void RenderingManager::GSDisableAllCBs( GPUDeferredContext * pContext )
{
    GSDisableCBs( 0, DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS, pContext );
}

Void RenderingManager::GSEnableSampler( UInt iSlot, GPUSamplerState * pSampler, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS );
    Assert( pSampler->IsBound() );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceSamplerState * pInstance = &(pSampler->m_hDeviceSamplerState);

    m_pDeviceRenderer->GSSetSamplers( iSlot, 1, &pInstance, pDeviceContext );
}
Void RenderingManager::GSEnableSamplers( UInt iSlot, UInt iSamplerCount, GPUSamplerState ** arrSamplers, GPUDeferredContext * pContext )
{
    Assert( iSlot + iSamplerCount <= DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceSamplerState * arrInstances[DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS];
    for( UInt i = 0; i < iSamplerCount; ++i ) {
        Assert( arrSamplers[i]->IsBound() );
        arrInstances[i] = &(arrSamplers[i]->m_hDeviceSamplerState);
    }

    m_pDeviceRenderer->GSSetSamplers( iSlot, iSamplerCount, arrInstances, pDeviceContext );
}
Void RenderingManager::GSDisableSampler( UInt iSlot, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->GSSetSamplers( iSlot, 1, NULL, pDeviceContext );
}
Void RenderingManager::GSDisableSamplers( UInt iSlot, UInt iSamplerCount, GPUDeferredContext * pContext )
{
    Assert( iSlot + iSamplerCount <= DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->GSSetSamplers( iSlot, iSamplerCount, NULL, pDeviceContext );
}
Void RenderingManager::GSDisableAllSamplers( GPUDeferredContext * pContext )
{
    GSDisableSamplers( 0, DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS, pContext );
}

Void RenderingManager::GSEnableResource( UInt iSlot, GPUShaderView * pResource, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceShaderView * pInstance = &(pResource->m_hDeviceShaderView);

    m_pDeviceRenderer->GSSetResources( iSlot, 1, &pInstance, pDeviceContext );
}
Void RenderingManager::GSEnableResources( UInt iSlot, UInt iResourceCount, GPUShaderView ** arrResources, GPUDeferredContext * pContext )
{
    Assert( iSlot + iResourceCount <= DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceShaderView * arrInstances[DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS];
    for( UInt i = 0; i < iResourceCount; ++i )
        arrInstances[i] = &(arrResources[i]->m_hDeviceShaderView);

    m_pDeviceRenderer->GSSetResources( iSlot, iResourceCount, arrInstances, pDeviceContext );
}
Void RenderingManager::GSDisableResource( UInt iSlot, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->GSSetResources( iSlot, 1, NULL, pDeviceContext );
}
Void RenderingManager::GSDisableResources( UInt iSlot, UInt iResourceCount, GPUDeferredContext * pContext )
{
    Assert( iSlot + iResourceCount <= DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->GSSetResources( iSlot, iResourceCount, NULL, pDeviceContext );
}
Void RenderingManager::GSDisableAllResources( GPUDeferredContext * pContext )
{
    GSDisableResources( 0, DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS, pContext );
}

Void RenderingManager::PSEnable( GPUPixelShader * pPS, GPUDeferredContext * pContext )
{
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->PSSetShader( &(pPS->m_hDevicePixelShader), pDeviceContext );
}
Void RenderingManager::PSDisable( GPUDeferredContext * pContext )
{
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->PSSetShader( NULL, pDeviceContext );
}

Void RenderingManager::PSEnableCB( UInt iSlot, GPUConstantBuffer * pCB, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS );
    Assert( pCB->IsBound() );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceConstantBuffer * pInstance = &(pCB->m_hDeviceConstantBuffer);

    m_pDeviceRenderer->PSSetConstantBuffers( iSlot, 1, &pInstance, pDeviceContext );
}
Void RenderingManager::PSEnableCBs( UInt iSlot, UInt iCBCount, GPUConstantBuffer ** arrCBs, GPUDeferredContext * pContext )
{
    Assert( iSlot + iCBCount <= DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceConstantBuffer * arrInstances[DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS];
    for( UInt i = 0; i < iCBCount; ++i ) {
        Assert( arrCBs[i]->IsBound() );
        arrInstances[i] = &(arrCBs[i]->m_hDeviceConstantBuffer);
    }

    m_pDeviceRenderer->PSSetConstantBuffers( iSlot, iCBCount, arrInstances, pDeviceContext );
}
Void RenderingManager::PSDisableCB( UInt iSlot, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->PSSetConstantBuffers( iSlot, 1, NULL, pDeviceContext );
}
Void RenderingManager::PSDisableCBs( UInt iSlot, UInt iCBCount, GPUDeferredContext * pContext )
{
    Assert( iSlot + iCBCount <= DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->PSSetConstantBuffers( iSlot, iCBCount, NULL, pDeviceContext );
}
Void RenderingManager::PSDisableAllCBs( GPUDeferredContext * pContext )
{
    PSDisableCBs( 0, DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS, pContext );
}

Void RenderingManager::PSEnableSampler( UInt iSlot, GPUSamplerState * pSampler, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS );
    Assert( pSampler->IsBound() );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceSamplerState * pInstance = &(pSampler->m_hDeviceSamplerState);

    m_pDeviceRenderer->PSSetSamplers( iSlot, 1, &pInstance, pDeviceContext );
}
Void RenderingManager::PSEnableSamplers( UInt iSlot, UInt iSamplerCount, GPUSamplerState ** arrSamplers, GPUDeferredContext * pContext )
{
    Assert( iSlot + iSamplerCount <= DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceSamplerState * arrInstances[DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS];
    for( UInt i = 0; i < iSamplerCount; ++i ) {
        Assert( arrSamplers[i]->IsBound() );
        arrInstances[i] = &(arrSamplers[i]->m_hDeviceSamplerState);
    }

    m_pDeviceRenderer->PSSetSamplers( iSlot, iSamplerCount, arrInstances, pDeviceContext );
}
Void RenderingManager::PSDisableSampler( UInt iSlot, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->PSSetSamplers( iSlot, 1, NULL, pDeviceContext );
}
Void RenderingManager::PSDisableSamplers( UInt iSlot, UInt iSamplerCount, GPUDeferredContext * pContext )
{
    Assert( iSlot + iSamplerCount <= DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->PSSetSamplers( iSlot, iSamplerCount, NULL, pDeviceContext );
}
Void RenderingManager::PSDisableAllSamplers( GPUDeferredContext * pContext )
{
    PSDisableSamplers( 0, DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS, pContext );
}

Void RenderingManager::PSEnableResource( UInt iSlot, GPUShaderView * pResource, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceShaderView * pInstance = &(pResource->m_hDeviceShaderView);

    m_pDeviceRenderer->PSSetResources( iSlot, 1, &pInstance, pDeviceContext );
}
Void RenderingManager::PSEnableResources( UInt iSlot, UInt iResourceCount, GPUShaderView ** arrResources, GPUDeferredContext * pContext )
{
    Assert( iSlot + iResourceCount <= DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceShaderView * arrInstances[DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS];
    for( UInt i = 0; i < iResourceCount; ++i )
        arrInstances[i] = &(arrResources[i]->m_hDeviceShaderView);

    m_pDeviceRenderer->PSSetResources( iSlot, iResourceCount, arrInstances, pDeviceContext );
}
Void RenderingManager::PSDisableResource( UInt iSlot, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->PSSetResources( iSlot, 1, NULL, pDeviceContext );
}
Void RenderingManager::PSDisableResources( UInt iSlot, UInt iResourceCount, GPUDeferredContext * pContext )
{
    Assert( iSlot + iResourceCount <= DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->PSSetResources( iSlot, iResourceCount, NULL, pDeviceContext );
}
Void RenderingManager::PSDisableAllResources( GPUDeferredContext * pContext )
{
    PSDisableResources( 0, DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS, pContext );
}

Void RenderingManager::HSEnable( GPUHullShader * pHS, GPUDeferredContext * pContext )
{
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->HSSetShader( &(pHS->m_hDeviceHullShader), pDeviceContext );
}
Void RenderingManager::HSDisable( GPUDeferredContext * pContext )
{
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->HSSetShader( NULL, pDeviceContext );
}

Void RenderingManager::HSEnableCB( UInt iSlot, GPUConstantBuffer * pCB, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS );
    Assert( pCB->IsBound() );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceConstantBuffer * pInstance = &(pCB->m_hDeviceConstantBuffer);

    m_pDeviceRenderer->HSSetConstantBuffers( iSlot, 1, &pInstance, pDeviceContext );
}
Void RenderingManager::HSEnableCBs( UInt iSlot, UInt iCBCount, GPUConstantBuffer ** arrCBs, GPUDeferredContext * pContext )
{
    Assert( iSlot + iCBCount <= DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceConstantBuffer * arrInstances[DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS];
    for( UInt i = 0; i < iCBCount; ++i ) {
        Assert( arrCBs[i]->IsBound() );
        arrInstances[i] = &(arrCBs[i]->m_hDeviceConstantBuffer);
    }

    m_pDeviceRenderer->HSSetConstantBuffers( iSlot, iCBCount, arrInstances, pDeviceContext );
}
Void RenderingManager::HSDisableCB( UInt iSlot, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->HSSetConstantBuffers( iSlot, 1, NULL, pDeviceContext );
}
Void RenderingManager::HSDisableCBs( UInt iSlot, UInt iCBCount, GPUDeferredContext * pContext )
{
    Assert( iSlot + iCBCount <= DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->HSSetConstantBuffers( iSlot, iCBCount, NULL, pDeviceContext );
}
Void RenderingManager::HSDisableAllCBs( GPUDeferredContext * pContext )
{
    HSDisableCBs( 0, DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS, pContext );
}

Void RenderingManager::HSEnableSampler( UInt iSlot, GPUSamplerState * pSampler, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS );
    Assert( pSampler->IsBound() );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceSamplerState * pInstance = &(pSampler->m_hDeviceSamplerState);

    m_pDeviceRenderer->HSSetSamplers( iSlot, 1, &pInstance, pDeviceContext );
}
Void RenderingManager::HSEnableSamplers( UInt iSlot, UInt iSamplerCount, GPUSamplerState ** arrSamplers, GPUDeferredContext * pContext )
{
    Assert( iSlot + iSamplerCount <= DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceSamplerState * arrInstances[DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS];
    for( UInt i = 0; i < iSamplerCount; ++i ) {
        Assert( arrSamplers[i]->IsBound() );
        arrInstances[i] = &(arrSamplers[i]->m_hDeviceSamplerState);
    }

    m_pDeviceRenderer->HSSetSamplers( iSlot, iSamplerCount, arrInstances, pDeviceContext );
}
Void RenderingManager::HSDisableSampler( UInt iSlot, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->HSSetSamplers( iSlot, 1, NULL, pDeviceContext );
}
Void RenderingManager::HSDisableSamplers( UInt iSlot, UInt iSamplerCount, GPUDeferredContext * pContext )
{
    Assert( iSlot + iSamplerCount <= DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->HSSetSamplers( iSlot, iSamplerCount, NULL, pDeviceContext );
}
Void RenderingManager::HSDisableAllSamplers( GPUDeferredContext * pContext )
{
    HSDisableSamplers( 0, DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS, pContext );
}

Void RenderingManager::HSEnableResource( UInt iSlot, GPUShaderView * pResource, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceShaderView * pInstance = &(pResource->m_hDeviceShaderView);

    m_pDeviceRenderer->HSSetResources( iSlot, 1, &pInstance, pDeviceContext );
}
Void RenderingManager::HSEnableResources( UInt iSlot, UInt iResourceCount, GPUShaderView ** arrResources, GPUDeferredContext * pContext )
{
    Assert( iSlot + iResourceCount <= DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceShaderView * arrInstances[DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS];
    for( UInt i = 0; i < iResourceCount; ++i )
        arrInstances[i] = &(arrResources[i]->m_hDeviceShaderView);

    m_pDeviceRenderer->HSSetResources( iSlot, iResourceCount, arrInstances, pDeviceContext );
}
Void RenderingManager::HSDisableResource( UInt iSlot, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->HSSetResources( iSlot, 1, NULL, pDeviceContext );
}
Void RenderingManager::HSDisableResources( UInt iSlot, UInt iResourceCount, GPUDeferredContext * pContext )
{
    Assert( iSlot + iResourceCount <= DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->HSSetResources( iSlot, iResourceCount, NULL, pDeviceContext );
}
Void RenderingManager::HSDisableAllResources( GPUDeferredContext * pContext )
{
    HSDisableResources( 0, DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS, pContext );
}

Void RenderingManager::DSEnable( GPUDomainShader * pDS, GPUDeferredContext * pContext )
{
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->DSSetShader( &(pDS->m_hDeviceDomainShader), pDeviceContext );
}
Void RenderingManager::DSDisable( GPUDeferredContext * pContext )
{
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->DSSetShader( NULL, pDeviceContext );
}

Void RenderingManager::DSEnableCB( UInt iSlot, GPUConstantBuffer * pCB, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS );
    Assert( pCB->IsBound() );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceConstantBuffer * pInstance = &(pCB->m_hDeviceConstantBuffer);

    m_pDeviceRenderer->DSSetConstantBuffers( iSlot, 1, &pInstance, pDeviceContext );
}
Void RenderingManager::DSEnableCBs( UInt iSlot, UInt iCBCount, GPUConstantBuffer ** arrCBs, GPUDeferredContext * pContext )
{
    Assert( iSlot + iCBCount <= DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceConstantBuffer * arrInstances[DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS];
    for( UInt i = 0; i < iCBCount; ++i ) {
        Assert( arrCBs[i]->IsBound() );
        arrInstances[i] = &(arrCBs[i]->m_hDeviceConstantBuffer);
    }

    m_pDeviceRenderer->DSSetConstantBuffers( iSlot, iCBCount, arrInstances, pDeviceContext );
}
Void RenderingManager::DSDisableCB( UInt iSlot, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->DSSetConstantBuffers( iSlot, 1, NULL, pDeviceContext );
}
Void RenderingManager::DSDisableCBs( UInt iSlot, UInt iCBCount, GPUDeferredContext * pContext )
{
    Assert( iSlot + iCBCount <= DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->DSSetConstantBuffers( iSlot, iCBCount, NULL, pDeviceContext );
}
Void RenderingManager::DSDisableAllCBs( GPUDeferredContext * pContext )
{
    DSDisableCBs( 0, DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS, pContext );
}

Void RenderingManager::DSEnableSampler( UInt iSlot, GPUSamplerState * pSampler, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS );
    Assert( pSampler->IsBound() );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceSamplerState * pInstance = &(pSampler->m_hDeviceSamplerState);

    m_pDeviceRenderer->DSSetSamplers( iSlot, 1, &pInstance, pDeviceContext );
}
Void RenderingManager::DSEnableSamplers( UInt iSlot, UInt iSamplerCount, GPUSamplerState ** arrSamplers, GPUDeferredContext * pContext )
{
    Assert( iSlot + iSamplerCount <= DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceSamplerState * arrInstances[DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS];
    for( UInt i = 0; i < iSamplerCount; ++i ) {
        Assert( arrSamplers[i]->IsBound() );
        arrInstances[i] = &(arrSamplers[i]->m_hDeviceSamplerState);
    }

    m_pDeviceRenderer->DSSetSamplers( iSlot, iSamplerCount, arrInstances, pDeviceContext );
}
Void RenderingManager::DSDisableSampler( UInt iSlot, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->DSSetSamplers( iSlot, 1, NULL, pDeviceContext );
}
Void RenderingManager::DSDisableSamplers( UInt iSlot, UInt iSamplerCount, GPUDeferredContext * pContext )
{
    Assert( iSlot + iSamplerCount <= DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->DSSetSamplers( iSlot, iSamplerCount, NULL, pDeviceContext );
}
Void RenderingManager::DSDisableAllSamplers( GPUDeferredContext * pContext )
{
    DSDisableSamplers( 0, DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS, pContext );
}

Void RenderingManager::DSEnableResource( UInt iSlot, GPUShaderView * pResource, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceShaderView * pInstance = &(pResource->m_hDeviceShaderView);

    m_pDeviceRenderer->DSSetResources( iSlot, 1, &pInstance, pDeviceContext );
}
Void RenderingManager::DSEnableResources( UInt iSlot, UInt iResourceCount, GPUShaderView ** arrResources, GPUDeferredContext * pContext )
{
    Assert( iSlot + iResourceCount <= DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceShaderView * arrInstances[DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS];
    for( UInt i = 0; i < iResourceCount; ++i )
        arrInstances[i] = &(arrResources[i]->m_hDeviceShaderView);

    m_pDeviceRenderer->DSSetResources( iSlot, iResourceCount, arrInstances, pDeviceContext );
}
Void RenderingManager::DSDisableResource( UInt iSlot, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->DSSetResources( iSlot, 1, NULL, pDeviceContext );
}
Void RenderingManager::DSDisableResources( UInt iSlot, UInt iResourceCount, GPUDeferredContext * pContext )
{
    Assert( iSlot + iResourceCount <= DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->DSSetResources( iSlot, iResourceCount, NULL, pDeviceContext );
}
Void RenderingManager::DSDisableAllResources( GPUDeferredContext * pContext )
{
    DSDisableResources( 0, DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS, pContext );
}

Void RenderingManager::CSEnable( GPUComputeShader * pCS, GPUDeferredContext * pContext )
{
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->CSSetShader( &(pCS->m_hDeviceComputeShader), pDeviceContext );
}
Void RenderingManager::CSDisable( GPUDeferredContext * pContext )
{
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->CSSetShader( NULL, pDeviceContext );
}

Void RenderingManager::CSEnableCB( UInt iSlot, GPUConstantBuffer * pCB, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS );
    Assert( pCB->IsBound() );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceConstantBuffer * pInstance = &(pCB->m_hDeviceConstantBuffer);

    m_pDeviceRenderer->CSSetConstantBuffers( iSlot, 1, &pInstance, pDeviceContext );
}
Void RenderingManager::CSEnableCBs( UInt iSlot, UInt iCBCount, GPUConstantBuffer ** arrCBs, GPUDeferredContext * pContext )
{
    Assert( iSlot + iCBCount <= DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceConstantBuffer * arrInstances[DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS];
    for( UInt i = 0; i < iCBCount; ++i ) {
        Assert( arrCBs[i]->IsBound() );
        arrInstances[i] = &(arrCBs[i]->m_hDeviceConstantBuffer);
    }

    m_pDeviceRenderer->CSSetConstantBuffers( iSlot, iCBCount, arrInstances, pDeviceContext );
}
Void RenderingManager::CSDisableCB( UInt iSlot, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->CSSetConstantBuffers( iSlot, 1, NULL, pDeviceContext );
}
Void RenderingManager::CSDisableCBs( UInt iSlot, UInt iCBCount, GPUDeferredContext * pContext )
{
    Assert( iSlot + iCBCount <= DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->CSSetConstantBuffers( iSlot, iCBCount, NULL, pDeviceContext );
}
Void RenderingManager::CSDisableAllCBs( GPUDeferredContext * pContext )
{
    CSDisableCBs( 0, DEVICERENDERER_MAX_CONSTANTBUFFER_SLOTS, pContext );
}

Void RenderingManager::CSEnableSampler( UInt iSlot, GPUSamplerState * pSampler, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS );
    Assert( pSampler->IsBound() );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceSamplerState * pInstance = &(pSampler->m_hDeviceSamplerState);

    m_pDeviceRenderer->CSSetSamplers( iSlot, 1, &pInstance, pDeviceContext );
}
Void RenderingManager::CSEnableSamplers( UInt iSlot, UInt iSamplerCount, GPUSamplerState ** arrSamplers, GPUDeferredContext * pContext )
{
    Assert( iSlot + iSamplerCount <= DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceSamplerState * arrInstances[DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS];
    for( UInt i = 0; i < iSamplerCount; ++i ) {
        Assert( arrSamplers[i]->IsBound() );
        arrInstances[i] = &(arrSamplers[i]->m_hDeviceSamplerState);
    }

    m_pDeviceRenderer->CSSetSamplers( iSlot, iSamplerCount, arrInstances, pDeviceContext );
}
Void RenderingManager::CSDisableSampler( UInt iSlot, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->CSSetSamplers( iSlot, 1, NULL, pDeviceContext );
}
Void RenderingManager::CSDisableSamplers( UInt iSlot, UInt iSamplerCount, GPUDeferredContext * pContext )
{
    Assert( iSlot + iSamplerCount <= DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->CSSetSamplers( iSlot, iSamplerCount, NULL, pDeviceContext );
}
Void RenderingManager::CSDisableAllSamplers( GPUDeferredContext * pContext )
{
    CSDisableSamplers( 0, DEVICERENDERER_MAX_SHADERSAMPLER_SLOTS, pContext );
}

Void RenderingManager::CSEnableResource( UInt iSlot, GPUShaderView * pResource, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceShaderView * pInstance = &(pResource->m_hDeviceShaderView);

    m_pDeviceRenderer->CSSetResources( iSlot, 1, &pInstance, pDeviceContext );
}
Void RenderingManager::CSEnableResources( UInt iSlot, UInt iResourceCount, GPUShaderView ** arrResources, GPUDeferredContext * pContext )
{
    Assert( iSlot + iResourceCount <= DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceShaderView * arrInstances[DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS];
    for( UInt i = 0; i < iResourceCount; ++i )
        arrInstances[i] = &(arrResources[i]->m_hDeviceShaderView);

    m_pDeviceRenderer->CSSetResources( iSlot, iResourceCount, arrInstances, pDeviceContext );
}
Void RenderingManager::CSDisableResource( UInt iSlot, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->CSSetResources( iSlot, 1, NULL, pDeviceContext );
}
Void RenderingManager::CSDisableResources( UInt iSlot, UInt iResourceCount, GPUDeferredContext * pContext )
{
    Assert( iSlot + iResourceCount <= DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->CSSetResources( iSlot, iResourceCount, NULL, pDeviceContext );
}
Void RenderingManager::CSDisableAllResources( GPUDeferredContext * pContext )
{
    CSDisableResources( 0, DEVICERENDERER_MAX_SHADERRESOURCE_SLOTS, pContext );
}

Void RenderingManager::CSEnableUAV( UInt iSlot, GPUUnorderedAccessView * pUAV, UInt iAppendConsumeInitCount, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_SHADERUAV_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceUnorderedAccessView * pInstance = &(pUAV->m_hDeviceUnorderedAccessView);
    UInt * pAppendConsumeInitCount = NULL;
    if ( iAppendConsumeInitCount != INVALID_OFFSET )
        pAppendConsumeInitCount = &iAppendConsumeInitCount;

    m_pDeviceRenderer->CSSetUAVs( iSlot, 1, &pInstance, pAppendConsumeInitCount, pDeviceContext );
}
Void RenderingManager::CSEnableUAVs( UInt iSlot, UInt iUAVCount, GPUUnorderedAccessView ** arrUAVs, const UInt * arrAppendConsumeInitCounts, GPUDeferredContext * pContext )
{
    Assert( iSlot + iUAVCount <= DEVICERENDERER_MAX_SHADERUAV_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceUnorderedAccessView * arrInstances[DEVICERENDERER_MAX_SHADERUAV_SLOTS];
    for( UInt i = 0; i < iUAVCount; ++i )
        arrInstances[i] = &(arrUAVs[i]->m_hDeviceUnorderedAccessView);

    m_pDeviceRenderer->CSSetUAVs( iSlot, iUAVCount, arrInstances, arrAppendConsumeInitCounts, pDeviceContext );
}
Void RenderingManager::CSDisableUAV( UInt iSlot, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_SHADERUAV_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->CSSetUAVs( iSlot, 1, NULL, NULL, pDeviceContext );
}
Void RenderingManager::CSDisableUAVs( UInt iSlot, UInt iUAVCount, GPUDeferredContext * pContext )
{
    Assert( iSlot + iUAVCount <= DEVICERENDERER_MAX_SHADERUAV_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->CSSetUAVs( iSlot, iUAVCount, NULL, NULL, pDeviceContext );
}
Void RenderingManager::CSDisableAllUAVs( GPUDeferredContext * pContext )
{
    CSDisableUAVs( 0, DEVICERENDERER_MAX_SHADERUAV_SLOTS, pContext );
}

Void RenderingManager::SOEnable( UInt iBufferCount, GPUBuffer ** arrBuffers, const UInt * arrOffsets, GPUDeferredContext * pContext )
{
    Assert( iBufferCount <= DEVICERENDERER_MAX_STREAMOUTPUT_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceBuffer * arrInstances[DEVICERENDERER_MAX_STREAMOUTPUT_SLOTS];
    for( UInt i = 0; i < iBufferCount; ++i ) {
        Assert( arrBuffers[i]->IsBound() );
        arrInstances[i] = arrBuffers[i]->m_pDeviceBuffer;
    }

    m_pDeviceRenderer->SOSetBuffers( iBufferCount, arrInstances, arrOffsets, pDeviceContext );
}
Void RenderingManager::SODisable( GPUDeferredContext * pContext )
{
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->SOSetBuffers( 0, NULL, NULL, pDeviceContext );
}

Void RenderingManager::RSEnableState( GPURasterizerState * pRasterizerState, GPUDeferredContext * pContext )
{
    Assert( pRasterizerState->IsBound() );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceRasterizerState * pInstance = &(pRasterizerState->m_hDeviceRasterizerState);

    m_pDeviceRenderer->RSSetState( pInstance, pDeviceContext );
}
Void RenderingManager::RSDisableState( GPUDeferredContext * pContext )
{
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->RSSetState( NULL, pDeviceContext );
}

Void RenderingManager::RSEnableViewports( UInt iViewportCount, const GPUViewport * arrViewports, GPUDeferredContext * pContext )
{
    Assert( iViewportCount <= DEVICERENDERER_MAX_VIEWPORT_SCISSOR_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->RSSetViewports( iViewportCount, (const DeviceViewport *)arrViewports, pDeviceContext );
}
Void RenderingManager::RSDisableViewports( GPUDeferredContext * pContext )
{
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->RSSetViewports( 0, NULL, pDeviceContext );
}

Void RenderingManager::RSEnableScissors( UInt iScissorCount, const GPURectangle * arrScissors, GPUDeferredContext * pContext )
{
    Assert( iScissorCount <= DEVICERENDERER_MAX_VIEWPORT_SCISSOR_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->RSSetScissors( iScissorCount, (const DeviceRectangle *)arrScissors, pDeviceContext );
}
Void RenderingManager::RSDisableScissors( GPUDeferredContext * pContext )
{
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->RSSetScissors( 0, NULL, pDeviceContext );
}

Void RenderingManager::OMEnableDepthStencilState( GPUDepthStencilState * pDepthStencilState, DWord iStencilRef, GPUDeferredContext * pContext )
{
    Assert( pDepthStencilState->IsBound() );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceDepthStencilState * pInstance = &(pDepthStencilState->m_hDeviceDepthStencilState);

    m_pDeviceRenderer->OMSetDepthStencilState( pInstance, iStencilRef, pDeviceContext );
}
Void RenderingManager::OMDisableDepthStencilState( GPUDeferredContext * pContext )
{
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->OMSetDepthStencilState( NULL, 0, pDeviceContext );
}

Void RenderingManager::OMEnableBlendState( GPUBlendState * pBlendState, const Float arrBlendFactors[4], DWord iSampleMask, GPUDeferredContext * pContext )
{
    Assert( pBlendState->IsBound() );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceBlendState * pInstance = &(pBlendState->m_hDeviceBlendState);

    m_pDeviceRenderer->OMSetBlendState( pInstance, arrBlendFactors, iSampleMask, pDeviceContext );
}
Void RenderingManager::OMDisableBlendState( GPUDeferredContext * pContext )
{
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->OMSetBlendState( NULL, NULL, 0, pDeviceContext );
}

Void RenderingManager::OMEnableRenderTarget( GPURenderTargetView * pRTV, GPUDepthStencilView * pDSV, GPUDeferredContext * pContext )
{
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceRenderTargetView * pRTInstance = &(pRTV->m_hDeviceRenderTargetView);
    DeviceDepthStencilView * pDSInstance = &(pDSV->m_hDeviceDepthStencilView);

    m_pDeviceRenderer->OMSetRenderTargets( 1, &pRTInstance, pDSInstance, pDeviceContext );
}
Void RenderingManager::OMEnableRenderTargets( UInt iRTVCount, GPURenderTargetView ** arrRTVs, GPUDepthStencilView * pDSV, GPUDeferredContext * pContext )
{
    Assert( iRTVCount <= DEVICERENDERER_MAX_RENDERTARGET_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceRenderTargetView * arrRTInstances[DEVICERENDERER_MAX_RENDERTARGET_SLOTS];
    for( UInt i = 0; i < iRTVCount; ++i )
        arrRTInstances[i] = &(arrRTVs[i]->m_hDeviceRenderTargetView);
    DeviceDepthStencilView * pDSInstance = &(pDSV->m_hDeviceDepthStencilView);

    m_pDeviceRenderer->OMSetRenderTargets( iRTVCount, arrRTInstances, pDSInstance, pDeviceContext );
}
Void RenderingManager::OMDisableAllRenderTargets( GPUDeferredContext * pContext )
{
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->OMSetRenderTargets( 0, NULL, NULL, pDeviceContext );
}

Void RenderingManager::OMEnableUAV( UInt iSlot, GPUUnorderedAccessView * pUAV, UInt iAppendConsumeInitCount, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_SHADERUAV_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceUnorderedAccessView * pInstance = &(pUAV->m_hDeviceUnorderedAccessView);
    UInt * pAppendConsumeInitCount = NULL;
    if ( iAppendConsumeInitCount != INVALID_OFFSET )
        pAppendConsumeInitCount = &iAppendConsumeInitCount;

    m_pDeviceRenderer->OMSetUAVs( iSlot, 1, &pInstance, pAppendConsumeInitCount, pDeviceContext );
}
Void RenderingManager::OMEnableUAVs( UInt iSlot, UInt iUAVCount, GPUUnorderedAccessView ** arrUAVs, const UInt * arrAppendConsumeInitCounts, GPUDeferredContext * pContext )
{
    Assert( iSlot + iUAVCount <= DEVICERENDERER_MAX_SHADERUAV_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    DeviceUnorderedAccessView * arrInstances[DEVICERENDERER_MAX_SHADERUAV_SLOTS];
    for( UInt i = 0; i < iUAVCount; ++i )
        arrInstances[i] = &(arrUAVs[i]->m_hDeviceUnorderedAccessView);

    m_pDeviceRenderer->OMSetUAVs( iSlot, iUAVCount, arrInstances, arrAppendConsumeInitCounts, pDeviceContext );
}
Void RenderingManager::OMDisableUAV( UInt iSlot, GPUDeferredContext * pContext )
{
    Assert( iSlot < DEVICERENDERER_MAX_SHADERUAV_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->OMSetUAVs( iSlot, 1, NULL, NULL, pDeviceContext );
}
Void RenderingManager::OMDisableUAVs( UInt iSlot, UInt iUAVCount, GPUDeferredContext * pContext )
{
    Assert( iSlot + iUAVCount <= DEVICERENDERER_MAX_SHADERUAV_SLOTS );
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->OMSetUAVs( iSlot, iUAVCount, NULL, NULL, pDeviceContext );
}
Void RenderingManager::OMDisableAllUAVs( GPUDeferredContext * pContext )
{
    OMDisableUAVs( 0, DEVICERENDERER_MAX_SHADERUAV_SLOTS, pContext );
}

Void RenderingManager::Clear( GPUDeferredContext * pContext )
{
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->Clear( pDeviceContext );
}
Void RenderingManager::Flush( GPUDeferredContext * pContext )
{
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->Flush( pDeviceContext );
}
Void RenderingManager::DestroyObjects( GPUDeferredContext * pContext )
{
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->DestroyObjects( pDeviceContext );
}

Void RenderingManager::ClearDepthStencil( GPUDepthStencilView * pDepthStencil, Bool bClearDepth, Bool bClearStencil, Float fDepth, Byte iStencil, GPUDeferredContext * pContext )
{
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->ClearDepthStencil( &(pDepthStencil->m_hDeviceDepthStencilView), bClearDepth, bClearStencil, fDepth, iStencil, pDeviceContext );
}
Void RenderingManager::ClearRenderTarget( GPURenderTargetView * pRenderTarget, const Float arrRGBA[4], GPUDeferredContext * pContext )
{
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->ClearRenderTarget( &(pRenderTarget->m_hDeviceRenderTargetView), arrRGBA, pDeviceContext );
}

Void RenderingManager::ClearUAVUInt( GPUUnorderedAccessView * pUAV, const UInt arrValues[4], GPUDeferredContext * pContext )
{
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->ClearUAVUInt( &(pUAV->m_hDeviceUnorderedAccessView), arrValues, pDeviceContext );
}
Void RenderingManager::ClearUAVFloat( GPUUnorderedAccessView * pUAV, const Float arrValues[4], GPUDeferredContext * pContext )
{
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->ClearUAVFloat( &(pUAV->m_hDeviceUnorderedAccessView), arrValues, pDeviceContext );
}

Bool RenderingManager::GetPickRay( Vertex3 * outRayPos, Vector3 * outRayDir, UInt iX, UInt iY ) const
{
    Assert( m_pScreenCamera != NULL );

    Scalar fX = (Scalar)iX;
    Scalar fY = (Scalar)iY;

    // Clip to Viewport
    if ( (fX < m_hScreenViewport.fTopLeftX) || (fX >= m_hScreenViewport.fTopLeftX + m_hScreenViewport.fWidth) ||
         (fY < m_hScreenViewport.fTopLeftY) || (fY >= m_hScreenViewport.fTopLeftY + m_hScreenViewport.fHeight) )
        return false;

    // Normalized Display Coords
    Scalar fT = ( fX - m_hScreenViewport.fTopLeftX ) / m_hScreenViewport.fWidth;
    Scalar fU = ( fY - m_hScreenViewport.fTopLeftY ) / m_hScreenViewport.fHeight;

    // Clip Coords
    Scalar fClipT = (1.0f - fT) * m_pScreenCamera->GetFrustrum(FRUSTRUM_LEFT) + fT * m_pScreenCamera->GetFrustrum(FRUSTRUM_RIGHT);
    Scalar fClipU = (1.0f - fU) * m_pScreenCamera->GetFrustrum(FRUSTRUM_TOP) + fU * m_pScreenCamera->GetFrustrum(FRUSTRUM_BOTTOM);
    Scalar fClipZ = m_pScreenCamera->GetFrustrum(FRUSTRUM_NEAR);

    // Compute Ray
    *outRayPos = m_pScreenCamera->GetViewEye();
    *outRayDir  = ( m_pScreenCamera->GetViewSide() * fClipT );
    *outRayDir += ( m_pScreenCamera->GetViewUp()   * fClipU );
    *outRayDir += ( m_pScreenCamera->GetViewDir()  * fClipZ );
    outRayDir->Normalize();
    return true;
}

Bool RenderingManager::GetPredication( GPUPredicate * outPredicate, Bool * outValue, GPUDeferredContext * pContext )
{
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    return m_pDeviceRenderer->GetPredication( outPredicate->m_pDevicePredicate, outValue, pDeviceContext );
}
Void RenderingManager::SetPredication( GPUPredicate * pPredicate, Bool bPredicateReference, GPUDeferredContext * pContext )
{
    Assert( pPredicate->IsBound() );

    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->SetPredication( pPredicate->m_pDevicePredicate, bPredicateReference, pDeviceContext );
}

Void RenderingManager::AsyncBegin( GPUAsynchronous * pAsynchronous, GPUDeferredContext * pContext )
{
    Assert( pAsynchronous->IsBound() );

    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->AsyncBegin( pAsynchronous->m_pDeviceAsynchronous, pDeviceContext );
}
Void RenderingManager::AsyncEnd( GPUAsynchronous * pAsynchronous, GPUDeferredContext * pContext )
{
    Assert( pAsynchronous->IsBound() );

    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->AsyncEnd( pAsynchronous->m_pDeviceAsynchronous, pDeviceContext );
}

Void RenderingManager::Draw( const Mesh * pMesh, const Transform3 * pWorldTransform, const EffectInstance * pEffectInstance, UInt iSlot, GPUDeferredContext * pContext )
{
    Assert( m_pScreenCamera != NULL );

    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    // Setup input
    IAEnablePT( pMesh->GetTopology(), pContext );

    IAEnableVB( iSlot, pMesh->GetVB(), pMesh->GetVertexOffset(), pContext );
    if ( pMesh->HasIndexBuffer() )
        IAEnableIB( pMesh->GetIB(), pMesh->GetIndexOffset(), pContext );

    // Multipass rendering
    UInt iPassCount = pEffectInstance->GetPassCount();
    for( UInt iPass = 0; iPass < iPassCount; ++iPass ) {
        Assert( pMesh->GetIL() == pEffectInstance->GetTechnique()->GetInputLayout(iPass) );

        pEffectInstance->UpdateConstants( iPass, pWorldTransform, m_pScreenCamera, pContext );
        pEffectInstance->Enable( iPass, pContext );

        if ( pMesh->HasIndexBuffer() )
            m_pDeviceRenderer->DrawIndexed( pMesh->GetIndexCount(), pMesh->GetIndexOffset(), pMesh->GetVertexOffset(), pDeviceContext );
        else
            m_pDeviceRenderer->Draw( pMesh->GetVertexCount(), pMesh->GetVertexOffset(), pDeviceContext );

        pEffectInstance->Disable( iPass, pContext );
    }

    if ( pMesh->HasIndexBuffer() )
        IADisableIB( pContext );
    IADisableVB( iSlot, pContext );

    IADisablePT( pContext );
}
//Void RenderingManager::DrawInstanced( const Mesh * pMesh, const Transform3 * pWorldTransform, const EffectInstance * pEffectInstance,
//                                      UInt iElementsPerInstance, UInt iInstanceCount, UInt iBaseInstance, GPUDeferredContext * pContext )
//{
//}
//
//Void RenderingManager::DrawInstancedIndirect( GPUVertexBuffer * outGeneratedPrimitives, UInt iBaseVertex, GPUDeferredContext * pContext )
//{
//}
//Void RenderingManager::DrawInstancedIndirect( GPUIndexBuffer * outGeneratedPrimitives, UInt iBaseIndex, GPUDeferredContext * pContext )
//{
//}
//
//Void RenderingManager::DrawStreamOutput( const Transform3 * pWorldTransform, const EffectInstance * pEffectInstance, GPUDeferredContext * pContext )
//{
//}

Void RenderingManager::ExecuteCommandList( GPUCommandList * pCommandList, Bool bRestoreContextState )
{
    m_pDeviceRenderer->ExecuteCommandList( pCommandList, bRestoreContextState );
}

Void RenderingManager::Dispatch( UInt iThreadGroupX, UInt iThreadGroupY, UInt iThreadGroupZ, GPUDeferredContext * pContext )
{
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->Dispatch( iThreadGroupX, iThreadGroupY, iThreadGroupZ, pDeviceContext );
}
Void RenderingManager::DispatchIndirect( GPUBuffer * pGeneratedDispatch, UInt iByteOffset, GPUDeferredContext * pContext )
{
    DeviceDeferredContext * pDeviceContext = ( pContext != NULL && pContext->IsBound() ) ? &(pContext->m_hDeviceDeferredContext) : NULL;

    m_pDeviceRenderer->DispatchIndirect( pGeneratedDispatch->m_pDeviceBuffer, iByteOffset, pDeviceContext );
}

Void RenderingManager::BatchSort( RenderingBatchID idBatch, RenderingBatchSortMode iSortMode )
{
    Assert( idBatch < RENDERING_BATCH_COUNT );
    Assert( m_arrBatches[idBatch].iNextFreeID == idBatch );
    Assert( m_arrBatches[idBatch].pEffectInstance != NULL );
    Assert( m_arrBatches[idBatch].arrVertices != NULL );

    _Batch_SortCallback pfSortCallback = NULL;
    switch( iSortMode ) {
        case RENDERING_BATCH_SORT_FRONT_TO_BACK: pfSortCallback = _Batch_Sort_FrontToBack; break;
        case RENDERING_BATCH_SORT_BACK_TO_FRONT: pfSortCallback = _Batch_Sort_BackToFront; break;
        default: Assert( false ); break;
    }

    _Batch_QuickSort( idBatch, m_arrBatches[idBatch].arrVertices, m_arrBatches[idBatch].iPrimitiveCount, pfSortCallback );
}

/////////////////////////////////////////////////////////////////////////////////

Void RenderingManager::_OnDestroyDevice()
{
    // Disable everything in all contexts
    _DeferredContextList::Iterator itDC = m_lstDeferredContexts.Begin();
    while( !(itDC.IsNull()) ) {
        itDC.Get()->Reset();
        ++itDC;
    }
    Clear( NULL );

    // Unbind all 2D contexts
    _2DContextList::Iterator it2DC = m_lst2DContexts.Begin();
    while( !(it2DC.IsNull()) ) {
        it2DC.Get()->_OnDestroyDevice(); // Recurse here, they have their own resources
        ++it2DC;
    }

    // Unbind all shaders
    _ShaderList::Iterator itShr = m_lstShaders.Begin();
    while( !(itShr.IsNull()) ) {
        itShr.Get()->m_pDeviceShader->OnDestroyDevice();
        ++itShr;
    }

    // Unbind all resource views
    _ResourceViewList::Iterator itRV = m_lstResourceViews.Begin();
    while( !(itRV.IsNull()) ) {
        itRV.Get()->m_pDeviceResourceView->OnDestroyDevice();
        ++itRV;
    }
    
    // Unbind all resources
    _BufferList::Iterator itBuf = m_lstBuffers.Begin();
    while( !(itBuf.IsNull()) ) {
        itBuf.Get()->m_pDeviceBuffer->OnDestroyDevice();
        ++itBuf;
    }

    _TextureList::Iterator itTex = m_lstTextures.Begin();
    while( !(itTex.IsNull()) ) {
        itTex.Get()->m_pDeviceTexture->OnDestroyDevice();
        ++itTex;
    }

    // Unbind all asynchronous
    _AsynchronousList::Iterator itAsync = m_lstAsynchronouses.Begin();
    while( !(itAsync.IsNull()) ) {
        itAsync.Get()->m_pDeviceAsynchronous->OnDestroyDevice();
        ++itAsync;
    }

    // Unbind all render states
    _RenderStateList::Iterator itRS = m_lstRenderStates.Begin();
    while( !(itRS.IsNull()) ) {
        itRS.Get()->m_pDeviceRenderState->OnDestroyDevice();
        ++itRS;
    }

    // Unbind all input layouts
    _InputLayoutList::Iterator itIL = m_lstInputLayouts.Begin();
    while( !(itIL.IsNull()) ) {
        itIL.Get()->m_hDeviceInputLayout.OnDestroyDevice();
        ++itIL;
    }

    // Unbind all deferred contexts
    itDC = m_lstDeferredContexts.Begin();
    while( !(itDC.IsNull()) ) {
        itDC.Get()->m_hDeviceDeferredContext.OnDestroyDevice();
        ++itDC;
    }
}
Void RenderingManager::_OnRestoreDevice()
{
    // Rebind all deferred contexts
    _DeferredContextList::Iterator itDC = m_lstDeferredContexts.Begin();
    while( !(itDC.IsNull()) ) {
        itDC.Get()->m_hDeviceDeferredContext.OnRestoreDevice();
        ++itDC;
    }
    // Can't recover cleared contexts & dropped command lists !

    // Rebind all input layouts
    _InputLayoutList::Iterator itIL = m_lstInputLayouts.Begin();
    while( !(itIL.IsNull()) ) {
        itIL.Get()->m_hDeviceInputLayout.OnRestoreDevice();
        ++itIL;
    }

    // Rebind all render states
    _RenderStateList::Iterator itRS = m_lstRenderStates.Begin();
    while( !(itRS.IsNull()) ) {
        itRS.Get()->m_pDeviceRenderState->OnRestoreDevice();
        ++itRS;
    }

    // Rebind all asynchronous
    _AsynchronousList::Iterator itAsync = m_lstAsynchronouses.Begin();
    while( !(itAsync.IsNull()) ) {
        itAsync.Get()->m_pDeviceAsynchronous->OnRestoreDevice();
        ++itAsync;
    }

    // Rebind all resources
    _BufferList::Iterator itBuf = m_lstBuffers.Begin();
    while( !(itBuf.IsNull()) ) {
        itBuf.Get()->m_pDeviceBuffer->OnRestoreDevice();
        ++itBuf;
    }

    _TextureList::Iterator itTex = m_lstTextures.Begin();
    while( !(itTex.IsNull()) ) {
        itTex.Get()->m_pDeviceTexture->OnRestoreDevice();
        ++itTex;
    }

    // Rebind all resource views
    _ResourceViewList::Iterator itRV = m_lstResourceViews.Begin();
    while( !(itRV.IsNull()) ) {
        itRV.Get()->m_pDeviceResourceView->OnRestoreDevice();
        ++itRV;
    }

    // Rebind all shaders
    _ShaderList::Iterator itShr = m_lstShaders.Begin();
    while( !(itShr.IsNull()) ) {
        itShr.Get()->m_pDeviceShader->OnRestoreDevice();
        ++itShr;
    }

    // Rebind all 2D contexts
    _2DContextList::Iterator it2DC = m_lst2DContexts.Begin();
    while( !(it2DC.IsNull()) ) {
        it2DC.Get()->_OnRestoreDevice(); // Recurse here, they have their own resources
        ++it2DC;
    }
}

Void RenderingManager::_OnDestroySwapChain()
{
    // Disable screen setup
    RSDisableViewports( NULL );
    OMDisableAllRenderTargets( NULL );

    // Unbind screen resources
    m_pScreenDepthStencilView->UnBind();

    m_pScreenDepthStencil->UnBind();
    DestroyResource( m_pScreenDepthStencil );
    m_pScreenDepthStencil = NULL;

    // Unbind all 2D contexts
    _2DContextList::Iterator it2DC = m_lst2DContexts.Begin();
    while( !(it2DC.IsNull()) ) {
        if ( it2DC.Get()->IsBoundToBackBuffer() )
            it2DC.Get()->_OnDestroyDevice(); // Recurse here, they have their own resources
        ++it2DC;
    }

    // Unbind backbuffer views
    _ResourceViewList::Iterator itRV = m_lstResourceViews.Begin();
    while( !(itRV.IsNull()) ) {
        if ( itRV.Get()->IsBoundToBackBuffer() )
            itRV.Get()->m_pDeviceResourceView->OnDestroyDevice();
        ++itRV;
    }

    // Unbind backbuffers
    _TextureList::Iterator itTex = m_lstTextures.Begin();
    while( !(itTex.IsNull()) ) {
        if ( itTex.Get()->GetType() == GPURESOURCE_TEXTURE_2D ) {
            if ( ((GPUTexture2D*)(itTex.Get()))->IsBoundToBackBuffer() )
                itTex.Get()->m_pDeviceTexture->OnDestroyDevice();
        }
        ++itTex;
    }
}
Void RenderingManager::_OnRestoreSwapChain()
{
    // Rebind backbuffers
    _TextureList::Iterator itTex = m_lstTextures.Begin();
    while( !(itTex.IsNull()) ) {
        if ( itTex.Get()->GetType() == GPURESOURCE_TEXTURE_2D ) {
            if ( ((GPUTexture2D*)(itTex.Get()))->IsBoundToBackBuffer() )
                itTex.Get()->m_pDeviceTexture->OnRestoreDevice();
        }
        ++itTex;
    }

    // Rebind backbuffer views
    _ResourceViewList::Iterator itRV = m_lstResourceViews.Begin();
    while( !(itRV.IsNull()) ) {
        if ( itRV.Get()->IsBoundToBackBuffer() )
            itRV.Get()->m_pDeviceResourceView->OnRestoreDevice();
        ++itRV;
    }

    // Rebind all 2D contexts
    _2DContextList::Iterator it2DC = m_lst2DContexts.Begin();
    while( !(it2DC.IsNull()) ) {
        if ( it2DC.Get()->IsBoundToBackBuffer() )
            it2DC.Get()->_OnRestoreDevice(); // Recurse here, they have their own resources
        ++it2DC;
    }

    // Rebind screen resources
    const DeviceSwapChainDesc * pSwapChainDesc = m_pDeviceRenderer->GetSwapChainDesc();
    
    m_hScreenViewport.fTopLeftX = 0.0f;
    m_hScreenViewport.fTopLeftY = 0.0f;
    m_hScreenViewport.fWidth = (Float)( pSwapChainDesc->iWidth );
    m_hScreenViewport.fHeight = (Float)( pSwapChainDesc->iHeight );
    m_hScreenViewport.fMinDepth = 0.0f;
    m_hScreenViewport.fMaxDepth = 1.0f;

    m_pScreenDepthStencil = CreateTexture2D( PIXEL_FMT_D24UN_S8UI, pSwapChainDesc->iWidth, pSwapChainDesc->iHeight, 1, 1, pSwapChainDesc->iSampleCount, NULL, false );
    m_pScreenDepthStencil->SetUsage( GPURESOURCE_USAGE_DEFAULT );
    m_pScreenDepthStencil->Bind( GPURESOURCE_BIND_DEPTH_STENCIL );

    m_pScreenDepthStencilView->Bind( m_pScreenDepthStencil, 0, 0, 1, false, false );

    // Enable screen setup
    RSEnableViewports( 1, &m_hScreenViewport, NULL );
    OMEnableRenderTarget( m_pScreenRenderTargetView, m_pScreenDepthStencilView, NULL );
}

Float RenderingManager::_Batch_PrimitiveDepth( RenderingBatchID idBatch, const Byte * pPrimitive )
{
    const GPUInputLayout * pIL = m_arrBatches[idBatch].pEffectInstance->GetTechnique()->GetInputLayout(0);

    UInt iVertexSize = pIL->GetSize();
    UInt iVertexCount = m_arrBatches[idBatch].iPrimitiveSize;
    UInt iOffset, iSize;
    pIL->GetFieldRange( &iOffset, &iSize, GPUINPUTFIELD_SEMANTIC_POSITION, 0 );

    const Vertex3 * pPosition;
    Float fSumZ = 0.0f;

    for( UInt i = 0; i < iVertexCount; ++i ) {
        pPosition = (const Vertex3 *)( pPrimitive + iOffset );
        fSumZ += pPosition->Z;
        pPrimitive += iVertexSize;
    }
    return ( fSumZ / (Float)iVertexCount );
}
Void RenderingManager::_Batch_QuickSort( RenderingBatchID idBatch, Byte * arrVertices, UInt iPrimitiveCount, _Batch_SortCallback pfSortCallback )
{
    static Bool s_bSendLeft = true;
    if ( iPrimitiveCount <= 1 )
        return;

    const GPUInputLayout * pIL = m_arrBatches[idBatch].pEffectInstance->GetTechnique()->GetInputLayout(0);

    UInt iVertexSize = pIL->GetSize();
    UInt iPrimitiveBytes = ( m_arrBatches[idBatch].iPrimitiveSize * iVertexSize );
    Float fLeft, fRight;

    Byte * pPivot = arrVertices;
    Byte * pBack = arrVertices + ( (iPrimitiveCount - 1) * iPrimitiveBytes );
    Int iCmp;
    while( pPivot < pBack ) {
        fLeft = _Batch_PrimitiveDepth( idBatch, pPivot );
        fRight = _Batch_PrimitiveDepth( idBatch, pPivot + iPrimitiveBytes );
        iCmp = pfSortCallback( fLeft, fRight );
        if ( iCmp < 0 ) {
            for( UInt i = 0; i < iPrimitiveBytes; ++i )
                Swap<Byte>( pPivot + i, pPivot + iPrimitiveBytes + i );
            pPivot += iPrimitiveBytes;
        } else if ( iCmp > 0 ) {
            if ( pPivot + iPrimitiveBytes != pBack ) {
                for( UInt i = 0; i < iPrimitiveBytes; ++i )
                    Swap<Byte>( pPivot + iPrimitiveBytes + i, pBack + i );
            }
		    pBack -= iPrimitiveBytes;
        } else {
            // balance distribution of equal cases to temper some annoying
            // issues like arrays filled with the same value ...
            if ( s_bSendLeft ) {
                for( UInt i = 0; i < iPrimitiveBytes; ++i )
                    Swap<Byte>( pPivot + i, pPivot + iPrimitiveBytes + i );
			    pPivot += iPrimitiveBytes;
		    } else {
			    if ( pPivot + iPrimitiveBytes != pBack ) {
                    for( UInt i = 0; i < iPrimitiveBytes; ++i )
                        Swap<Byte>( pPivot + iPrimitiveBytes + i, pBack + i );
                }
			    pBack -= iPrimitiveBytes;
		    }
		    s_bSendLeft = !s_bSendLeft;
        }
    }

    UInt iLeft = ( pPivot - arrVertices ) / iPrimitiveBytes;
    UInt iRight = ( iPrimitiveCount - iLeft - 1 );
    if ( iLeft > 1 )
	    _Batch_QuickSort( idBatch, arrVertices, iLeft, pfSortCallback );
    if ( iRight > 1 )
	    _Batch_QuickSort( idBatch, pPivot + iPrimitiveBytes, iRight, pfSortCallback );
}

RenderingBatchID RenderingManager::_Batch_Create( const EffectInstance * pEffectInstance, GPUShaderPrimitiveTopology iPrimitiveTopology, GPUDeferredContext * pContext )
{
    Assert( m_iFreeBatchesCount > 0 );
    Assert( m_iNextFreeBatchID != INVALID_OFFSET );

    // Allocate Batch
    RenderingBatchID idBatch = m_iNextFreeBatchID;
    m_iNextFreeBatchID = m_arrBatches[idBatch].iNextFreeID;
    m_arrBatches[idBatch].iNextFreeID = idBatch;
    --m_iFreeBatchesCount;

    // Initialize Batch
    m_arrBatches[idBatch].pEffectInstance = pEffectInstance;
    m_arrBatches[idBatch].iPrimitiveTopology = iPrimitiveTopology;
    switch( m_arrBatches[idBatch].iPrimitiveTopology ) {
        case GPUSHADER_PRIMITIVETOPOLOGY_POINTLIST:    m_arrBatches[idBatch].iPrimitiveSize = 1; break;
        case GPUSHADER_PRIMITIVETOPOLOGY_LINELIST:     m_arrBatches[idBatch].iPrimitiveSize = 2; break;
        case GPUSHADER_PRIMITIVETOPOLOGY_TRIANGLELIST: m_arrBatches[idBatch].iPrimitiveSize = 3; break;
        default: Assert(false); break;
    }
    m_arrBatches[idBatch].pContext = pContext;

    const GPUInputLayout * pIL = pEffectInstance->GetTechnique()->GetInputLayout(0);

    m_arrBatches[idBatch].pVB = CreateVertexBuffer( pIL->GetSize(), RENDERING_BATCH_MAX_VERTICES, NULL, false );
    m_arrBatches[idBatch].pVB->SetUsage( GPURESOURCE_USAGE_DYNAMIC );
    m_arrBatches[idBatch].pVB->Bind();
    m_arrBatches[idBatch].trWorldTransform = Transform3::Identity;

    m_arrBatches[idBatch].iVertexCount = 0;
    m_arrBatches[idBatch].iPrimitiveCount = 0;
    m_arrBatches[idBatch].arrVertices = NULL;

    return idBatch;
}
Void RenderingManager::_Batch_Destroy( RenderingBatchID idBatch )
{
    Assert( m_iFreeBatchesCount < RENDERING_BATCH_COUNT );
    Assert( m_arrBatches[idBatch].iNextFreeID == idBatch );
    Assert( m_arrBatches[idBatch].pEffectInstance != NULL );
    Assert( m_arrBatches[idBatch].arrVertices == NULL );

    // Cleanup Batch
    m_arrBatches[idBatch].iVertexCount = 0;
    m_arrBatches[idBatch].iPrimitiveCount = 0;
    m_arrBatches[idBatch].arrVertices = NULL;

    m_arrBatches[idBatch].pVB->UnBind();
    DestroyResource( m_arrBatches[idBatch].pVB );
    m_arrBatches[idBatch].pVB = NULL;
    m_arrBatches[idBatch].trWorldTransform = Transform3::Identity;

    m_arrBatches[idBatch].pEffectInstance = NULL;
    m_arrBatches[idBatch].iPrimitiveTopology = GPUSHADER_PRIMITIVETOPOLOGY_UNDEFINED;
    m_arrBatches[idBatch].iPrimitiveSize = 0;

    // Free Batch
    m_arrBatches[idBatch].iNextFreeID = m_iNextFreeBatchID;
    m_iNextFreeBatchID = idBatch;
    ++m_iFreeBatchesCount;
}

Void RenderingManager::_Batch_Start( RenderingBatchID idBatch )
{
    Assert( m_arrBatches[idBatch].iNextFreeID == idBatch );
    Assert( m_arrBatches[idBatch].pEffectInstance != NULL );
    Assert( m_arrBatches[idBatch].arrVertices == NULL );

    m_arrBatches[idBatch].iVertexCount = 0;
    m_arrBatches[idBatch].iPrimitiveCount = 0;

    UInt iByteSize = 0;
    m_arrBatches[idBatch].arrVertices = (Byte*)( m_arrBatches[idBatch].pVB->Lock( GPURESOURCE_LOCK_WRITE_DISCARD, 0, &iByteSize, m_arrBatches[idBatch].pContext ) );
    Assert( iByteSize == m_arrBatches[idBatch].pVB->GetSize() );
}
Void RenderingManager::_Batch_Finish( RenderingBatchID idBatch )
{
    Assert( m_arrBatches[idBatch].iNextFreeID == idBatch );
    Assert( m_arrBatches[idBatch].pEffectInstance != NULL );
    Assert( m_arrBatches[idBatch].arrVertices != NULL );

    m_arrBatches[idBatch].pVB->UnLock( m_arrBatches[idBatch].pContext );
    m_arrBatches[idBatch].arrVertices = NULL;

    _Batch_DrawPrimitives( idBatch );

    m_arrBatches[idBatch].iVertexCount = 0;
    m_arrBatches[idBatch].iPrimitiveCount = 0;
}
Void RenderingManager::_Batch_Abort( RenderingBatchID idBatch )
{
    Assert( m_arrBatches[idBatch].iNextFreeID == idBatch );
    Assert( m_arrBatches[idBatch].pEffectInstance != NULL );
    Assert( m_arrBatches[idBatch].arrVertices != NULL );

    m_arrBatches[idBatch].pVB->UnLock( m_arrBatches[idBatch].pContext );
    m_arrBatches[idBatch].arrVertices = NULL;

    m_arrBatches[idBatch].iVertexCount = 0;
    m_arrBatches[idBatch].iPrimitiveCount = 0;
}

Void RenderingManager::_Batch_Flush( RenderingBatchID idBatch )
{
    Assert( m_arrBatches[idBatch].iNextFreeID == idBatch );
    Assert( m_arrBatches[idBatch].pEffectInstance != NULL );
    Assert( m_arrBatches[idBatch].arrVertices != NULL );

    m_arrBatches[idBatch].pVB->UnLock( m_arrBatches[idBatch].pContext );
    m_arrBatches[idBatch].arrVertices = NULL;

    _Batch_DrawPrimitives( idBatch );

    m_arrBatches[idBatch].iVertexCount = 0;
    m_arrBatches[idBatch].iPrimitiveCount = 0;

    UInt iByteSize = 0;
    m_arrBatches[idBatch].arrVertices = (Byte*)( m_arrBatches[idBatch].pVB->Lock( GPURESOURCE_LOCK_WRITE_DISCARD, 0, &iByteSize, m_arrBatches[idBatch].pContext ) );
    Assert( iByteSize == m_arrBatches[idBatch].pVB->GetSize() );
}
Void RenderingManager::_Batch_Reset( RenderingBatchID idBatch )
{
    Assert( m_arrBatches[idBatch].iNextFreeID == idBatch );
    Assert( m_arrBatches[idBatch].pEffectInstance != NULL );
    Assert( m_arrBatches[idBatch].arrVertices != NULL );

    m_arrBatches[idBatch].iVertexCount = 0;
    m_arrBatches[idBatch].iPrimitiveCount = 0;
}

Void RenderingManager::_Batch_Primitive( RenderingBatchID idBatch, const Byte * arrVertices )
{
    Assert( m_arrBatches[idBatch].iNextFreeID == idBatch );
    Assert( m_arrBatches[idBatch].pEffectInstance != NULL );
    Assert( m_arrBatches[idBatch].arrVertices != NULL );

    const GPUInputLayout * pIL = m_arrBatches[idBatch].pEffectInstance->GetTechnique()->GetInputLayout(0);

    UInt iVertexSize = pIL->GetSize();
    UInt iVertexCount = m_arrBatches[idBatch].iPrimitiveSize;

    if ( (m_arrBatches[idBatch].iVertexCount + iVertexCount) > RENDERING_BATCH_MAX_VERTICES )
        _Batch_Flush( idBatch );

    Byte * pNextVertex = m_arrBatches[idBatch].arrVertices + ( m_arrBatches[idBatch].iVertexCount * iVertexSize );
    MemCopy( pNextVertex, arrVertices, iVertexCount * iVertexSize );
    m_arrBatches[idBatch].iVertexCount += iVertexCount;
    ++(m_arrBatches[idBatch].iPrimitiveCount);
}

Void RenderingManager::_Batch_DrawPrimitives( RenderingBatchID idBatch )
{
    Assert( m_arrBatches[idBatch].iNextFreeID == idBatch );
    Assert( m_arrBatches[idBatch].pEffectInstance != NULL );

    DeviceDeferredContext * pDeviceContext = ( m_arrBatches[idBatch].pContext != NULL && m_arrBatches[idBatch].pContext->IsBound() ) ? &(m_arrBatches[idBatch].pContext->m_hDeviceDeferredContext) : NULL;

    // Pick vertex & primitive counts, effect instance and world transform
    UInt iVertexCount = m_arrBatches[idBatch].iVertexCount;
    const EffectInstance * pEffectInstance = m_arrBatches[idBatch].pEffectInstance;
    const Transform3 & trWorldTransform = m_arrBatches[idBatch].trWorldTransform;

    // Multipass rendering
    IAEnablePT( m_arrBatches[idBatch].iPrimitiveTopology, m_arrBatches[idBatch].pContext );
    IAEnableVB( 0, m_arrBatches[idBatch].pVB, 0, m_arrBatches[idBatch].pContext );

    UInt iPassCount = pEffectInstance->GetPassCount();
    for( UInt iPass = 0; iPass < iPassCount; ++iPass ) {
        pEffectInstance->UpdateConstants( iPass, &trWorldTransform, m_pScreenCamera, m_arrBatches[idBatch].pContext );
        pEffectInstance->Enable( iPass, m_arrBatches[idBatch].pContext );

        m_pDeviceRenderer->Draw( iVertexCount, 0, pDeviceContext );

        pEffectInstance->Disable( iPass, m_arrBatches[idBatch].pContext );
    }

    IADisableVB( 0, m_arrBatches[idBatch].pContext );
    IADisablePT( m_arrBatches[idBatch].pContext );
}



