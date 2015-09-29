/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/RenderingManager.inl
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
// RenderingManagerCallbacks implementation

/////////////////////////////////////////////////////////////////////////////////
// RenderingManager implementation
inline const GChar * RenderingManager::GetName() {
    return TEXT("RenderingManager");
}
inline const GChar * RenderingManager::GetMemoryBudget() {
    return TEXT (
"Heap:Scratch(16777216);Heap:Buffers(4194304);Heap:Textures(33554432);Heap:Effects(4194304);\
Heap:Bitmaps(4194304);Heap:2DMeshes(4194304);"
    );
}

inline Void RenderingManager::GetScreenSize( UInt * outWidth, UInt * outHeight ) const {
    return m_pDeviceWindow->GetScreenSize( outWidth, outHeight );
}

inline UInt RenderingManager::GetAdapterCount() const {
    return m_pDeviceWindow->GetAdapterCount();
}
inline Void RenderingManager::GetAdapterDesc( GPUAdapterDesc * outAdapterDesc, UInt iAdapter ) const {
    m_pDeviceWindow->GetAdapterDesc( (DeviceAdapterDesc*)outAdapterDesc, iAdapter );
}

inline UInt RenderingManager::GetCurrentAdapter() const {
    return m_pDeviceWindow->GetCurrentAdapter();
}
inline const GPUAdapterDesc * RenderingManager::GetCurrentAdapterDesc() const {
    return (const GPUAdapterDesc *)( m_pDeviceWindow->GetCurrentAdapterDesc() );
}

inline UInt RenderingManager::GetOutputCount( UInt iAdapter ) const {
    return m_pDeviceWindow->GetOutputCount( iAdapter );
}
inline Void RenderingManager::GetOutputDesc( GPUOutputDesc * outOutputDesc, UInt iOutput, UInt iAdapter ) const {
    m_pDeviceWindow->GetOutputDesc( (DeviceOutputDesc*)outOutputDesc, iOutput, iAdapter );
}

inline UInt RenderingManager::GetCurrentOutput() const {
    return m_pDeviceWindow->GetCurrentOutput();
}
inline const GPUOutputDesc * RenderingManager::GetCurrentOutputDesc() const {
    return (const GPUOutputDesc *)( m_pDeviceWindow->GetCurrentOutputDesc() );
}

inline UInt RenderingManager::GetDisplayModeCount( UInt iOutput, UInt iAdapter ) const {
    return m_pDeviceWindow->GetDisplayModeCount( iOutput, iAdapter );
}
inline Void RenderingManager::GetDisplayModeDesc( GPUDisplayModeDesc * outDisplayModeDesc, UInt iDisplayMode, UInt iOutput, UInt iAdapter ) const {
    m_pDeviceWindow->GetDisplayModeDesc( (DeviceDisplayModeDesc*)outDisplayModeDesc, iDisplayMode, iOutput, iAdapter );
}

inline UInt RenderingManager::GetCurrentDisplayMode() const {
    return m_pDeviceWindow->GetCurrentDisplayMode();
}
inline const GPUDisplayModeDesc * RenderingManager::GetCurrentDisplayModeDesc() const {
    return (const GPUDisplayModeDesc *)( m_pDeviceWindow->GetCurrentDisplayModeDesc() );
}

inline UInt RenderingManager::MatchDisplayMode( GPUDisplayModeDesc * outMatchedDisplayMode, const GPUDisplayModeDesc * pDisplayMode, UInt iOutput, UInt iAdapter ) {
    return m_pDeviceWindow->MatchDisplayMode( (DeviceDisplayModeDesc*)outMatchedDisplayMode, (const DeviceDisplayModeDesc *)pDisplayMode, iOutput, iAdapter );
}

inline Void RenderingManager::SwitchAdapter( UInt iAdapter, UInt iOutput, UInt iDisplayMode ) {
    m_pDeviceWindow->SwitchAdapter( iAdapter, iOutput, iDisplayMode );
}
inline Void RenderingManager::SwitchOutput( UInt iOutput, UInt iDisplayMode ) {
    m_pDeviceWindow->SwitchOutput( iOutput, iDisplayMode );
}
inline Void RenderingManager::SwitchDisplayMode( UInt iDisplayMode ) {
    m_pDeviceWindow->SwitchDisplayMode( iDisplayMode );
}

inline Bool RenderingManager::IsFullScreen( UInt * outOutput ) const {
    return m_pDeviceWindow->IsFullScreen( outOutput );
}
inline Void RenderingManager::SetFullScreen( Bool bFullScreen, UInt iOutput, UInt iDisplayMode ) {
    m_pDeviceWindow->SetFullScreen( bFullScreen, iOutput, iDisplayMode );
}
inline Void RenderingManager::SetFullScreenWindowed( Bool bFullScreenWindowed ) {
    m_pDeviceWindow->SetFullScreenWindowed( bFullScreenWindowed );
}

inline Void RenderingManager::MoveWindow( UInt iX, UInt iY ) const {
    m_pDeviceWindow->MoveWindow( iX, iY );
}

inline Void RenderingManager::GetGammaCaps( GPUGammaCaps * outGammaCaps ) const {
    m_pDeviceWindow->GetGammaCaps( (DeviceGammaCaps*)outGammaCaps );
}
inline Void RenderingManager::GetGammaControl( GPUGammaControl * outGammaControl ) const {
    m_pDeviceWindow->GetGammaControl( (DeviceGammaControl*)outGammaControl );
}
inline Void RenderingManager::SetGammaControl( const GPUGammaControl * pGammaControl ) const {
    m_pDeviceWindow->SetGammaControl( (const DeviceGammaControl *)pGammaControl );
}

inline Void RenderingManager::WaitForVSync() const {
    m_pDeviceWindow->WaitForVSync();
}

inline Void RenderingManager::ClipCursor( Bool bClip, Bool bClipToClientRect ) const {
    m_pDeviceWindow->ClipCursor( bClip, bClipToClientRect );
}
inline Void RenderingManager::ShowCursor( Bool bShow ) const {
    m_pDeviceWindow->ShowCursor( bShow );
}
inline Void RenderingManager::WarpCursor( UInt iX, UInt iY ) const {
    m_pDeviceWindow->WarpCursor( iX, iY );
}
inline Void RenderingManager::SetCursor( const DWord * pCursorRGBA, UInt iWidth, UInt iHeight, DWord dwKeyColor, UInt iHotspotX, UInt iHotspotY ) {
    m_pDeviceWindow->SetCursor( pCursorRGBA, iWidth, iHeight, dwKeyColor, iHotspotX, iHotspotY );
}

inline Bool RenderingManager::GetClipboardData( GChar * outText, UInt iMaxLength ) const {
    return m_pDeviceWindow->GetClipboardData( outText, iMaxLength );
}
inline Bool RenderingManager::SetClipboardData( const GChar * strText, UInt iLength ) const {
    return m_pDeviceWindow->SetClipboardData( strText, iLength );
}

inline Void RenderingManager::GetDeviceFeatures( GPUDeviceFeatures * outFeatures ) const {
    m_pDeviceRenderer->GetDeviceFeatures( (DeviceDeviceFeatures*)outFeatures );
}
inline Void RenderingManager::GetFormatSupport( GPUPixelFormatSupport * outFormatSupport, PixelFormat iFormat ) const {
    m_pDeviceRenderer->GetFormatSupport( (DevicePixelFormatSupport*)outFormatSupport, iFormat );
}
inline Void RenderingManager::GetCounterSupport( GPUCounterSupport * outCounterSupport ) const {
    m_pDeviceRenderer->GetCounterSupport( (DeviceCounterSupport*)outCounterSupport );
}

inline const GPUSwapChainDesc * RenderingManager::GetSwapChainDesc() const {
    return (const GPUSwapChainDesc *)( m_pDeviceRenderer->GetSwapChainDesc() );
}
inline Void RenderingManager::GetFrameStats( GPUFrameStats * outFrameStats ) const {
    m_pDeviceRenderer->GetFrameStats( (DeviceFrameStats*)outFrameStats );
}

inline Void RenderingManager::SetMultisampling( UInt iSampleCount ) {
    m_pDeviceRenderer->SetMultisampling( iSampleCount );
}

inline Bool RenderingManager::IsIdle() const {
    return m_pDeviceRenderer->IsIdle();
}
inline Bool RenderingManager::Present( UInt iVSyncInterval, UInt iPresentFlags ) {
    return m_pDeviceRenderer->Present( iVSyncInterval, iPresentFlags );
}

inline const Camera * RenderingManager::GetDefaultCamera() const {
    return m_pScreenDefaultCamera;
}
inline Camera * RenderingManager::GetDefault2DCamera() const {
    return m_pScreenDefault2DCamera;
}
inline Camera * RenderingManager::GetCamera() const {
    return m_pScreenCamera;
}

inline Void RenderingManager::EnableDefaultCamera() {
    m_pScreenCamera = m_pScreenDefaultCamera;
}
inline Void RenderingManager::EnableDefault2DCamera() {
    m_pScreenCamera = m_pScreenDefault2DCamera;
}
inline Void RenderingManager::EnableCamera( Camera * pCamera ) {
    Assert( pCamera != NULL );
    m_pScreenCamera = pCamera;
}

inline const GPUViewport * RenderingManager::GetScreenViewport() const {
    return &m_hScreenViewport;
}
inline Void RenderingManager::SetScreenViewport( const GPUViewport * pViewport ) {
    m_hScreenViewport.fTopLeftX = pViewport->fTopLeftX;
    m_hScreenViewport.fTopLeftY = pViewport->fTopLeftY;
    m_hScreenViewport.fWidth = pViewport->fWidth;
    m_hScreenViewport.fHeight = pViewport->fHeight;
    m_hScreenViewport.fMinDepth = pViewport->fMinDepth;
    m_hScreenViewport.fMaxDepth = pViewport->fMaxDepth;
}

inline GPUDepthStencilView * RenderingManager::GetScreenDepthStencil() {
    return m_pScreenDepthStencilView;
}
inline GPURenderTargetView * RenderingManager::GetScreenRenderTarget() {
    return m_pScreenRenderTargetView;
}

inline Void RenderingManager::EnableScreenResources( GPUDeferredContext * pContext ) {
    RSEnableViewports( 1, &m_hScreenViewport, pContext );
    OMEnableRenderTarget( m_pScreenRenderTargetView, m_pScreenDepthStencilView, pContext );
}

inline GPU2DContext * RenderingManager::GetScreen2DContext() {
    return m_pScreen2DContext;
}

inline UInt RenderingManager::GetBatchCount() const {
    return ( RENDERING_BATCH_COUNT - m_iFreeBatchesCount );
}
inline UInt RenderingManager::GetFreeBatchesCount() const {
    return m_iFreeBatchesCount;
}

inline RenderingBatchID RenderingManager::BatchCreate( const EffectInstance * pEffectInstance, GPUShaderPrimitiveTopology iPrimitiveTopology, GPUDeferredContext * pContext ) {
    Assert( m_iFreeBatchesCount > 0 );
    Assert( m_iNextFreeBatchID != INVALID_OFFSET );
    return _Batch_Create( pEffectInstance, iPrimitiveTopology, pContext );
}
inline Void RenderingManager::BatchDestroy( UInt idBatch ) {
    Assert( idBatch < RENDERING_BATCH_COUNT );
    Assert( m_arrBatches[idBatch].iNextFreeID == idBatch );
    Assert( m_arrBatches[idBatch].pEffectInstance != NULL );
    Assert( m_arrBatches[idBatch].arrVertices == NULL );
    _Batch_Destroy( idBatch );
}

inline Void RenderingManager::BatchSetWorldTransform( UInt idBatch, const Transform3 & trWorldTransform ) {
    Assert( idBatch < RENDERING_BATCH_COUNT );
    Assert( m_arrBatches[idBatch].iNextFreeID == idBatch );
    Assert( m_arrBatches[idBatch].pEffectInstance != NULL );
    Assert( m_arrBatches[idBatch].arrVertices == NULL );
    m_arrBatches[idBatch].trWorldTransform = trWorldTransform;
}

inline Void RenderingManager::BatchStart( UInt idBatch ) {
    Assert( idBatch < RENDERING_BATCH_COUNT );
    Assert( m_arrBatches[idBatch].iNextFreeID == idBatch );
    Assert( m_arrBatches[idBatch].pEffectInstance != NULL );
    Assert( m_arrBatches[idBatch].arrVertices == NULL );
    _Batch_Start( idBatch );
}
inline Void RenderingManager::BatchFinish( UInt idBatch ) {
    Assert( idBatch < RENDERING_BATCH_COUNT );
    Assert( m_arrBatches[idBatch].iNextFreeID == idBatch );
    Assert( m_arrBatches[idBatch].pEffectInstance != NULL );
    Assert( m_arrBatches[idBatch].arrVertices != NULL );
    _Batch_Finish( idBatch );
}
inline Void RenderingManager::BatchAbort( UInt idBatch ) {
    Assert( idBatch < RENDERING_BATCH_COUNT );
    Assert( m_arrBatches[idBatch].iNextFreeID == idBatch );
    Assert( m_arrBatches[idBatch].pEffectInstance != NULL );
    Assert( m_arrBatches[idBatch].arrVertices != NULL );
    _Batch_Abort( idBatch );
}

inline Void RenderingManager::BatchFlush( UInt idBatch ) {
    Assert( idBatch < RENDERING_BATCH_COUNT );
    Assert( m_arrBatches[idBatch].iNextFreeID == idBatch );
    Assert( m_arrBatches[idBatch].pEffectInstance != NULL );
    Assert( m_arrBatches[idBatch].arrVertices != NULL );
    _Batch_Flush( idBatch );
}
inline Void RenderingManager::BatchReset( UInt idBatch ) {
    Assert( idBatch < RENDERING_BATCH_COUNT );
    Assert( m_arrBatches[idBatch].iNextFreeID == idBatch );
    Assert( m_arrBatches[idBatch].pEffectInstance != NULL );
    Assert( m_arrBatches[idBatch].arrVertices != NULL );
    _Batch_Reset( idBatch );
}

inline Void RenderingManager::BatchPrimitive( UInt idBatch, const Byte * arrVertices ) {
    Assert( idBatch < RENDERING_BATCH_COUNT );
    Assert( m_arrBatches[idBatch].iNextFreeID == idBatch );
    Assert( m_arrBatches[idBatch].pEffectInstance != NULL );
    Assert( m_arrBatches[idBatch].arrVertices != NULL );
    _Batch_Primitive( idBatch, arrVertices );
}

///////////////////////////////////////////////////////////////////////////////////

inline Int RenderingManager::_Compare_GPUDeferredContext( GPUDeferredContext * const & rLeft, GPUDeferredContext * const & rRight, Void * /*pUserData*/ ) {
    if ( rLeft < rRight )
        return +1;
    if ( rLeft > rRight )
        return -1;
    return 0;
}
inline Int RenderingManager::_Compare_GPUInputLayout( GPUInputLayout * const & rLeft, GPUInputLayout * const & rRight, Void * /*pUserData*/ ) {
    if ( rLeft < rRight )
        return +1;
    if ( rLeft > rRight )
        return -1;
    return 0;
}
inline Int RenderingManager::_Compare_GPURenderState( GPURenderState * const & rLeft, GPURenderState * const & rRight, Void * /*pUserData*/ ) {
    if ( rLeft < rRight )
        return +1;
    if ( rLeft > rRight )
        return -1;
    return 0;
}
inline Int RenderingManager::_Compare_GPUAsynchronous( GPUAsynchronous * const & rLeft, GPUAsynchronous * const & rRight, Void * /*pUserData*/ ) {
    if ( rLeft < rRight )
        return +1;
    if ( rLeft > rRight )
        return -1;
    return 0;
}
inline Int RenderingManager::_Compare_GPUBuffer( GPUBuffer * const & rLeft, GPUBuffer * const & rRight, Void * /*pUserData*/ ) {
    if ( rLeft < rRight )
        return +1;
    if ( rLeft > rRight )
        return -1;
    return 0;
}
inline Int RenderingManager::_Compare_GPUTexture( GPUTexture * const & rLeft, GPUTexture * const & rRight, Void * /*pUserData*/ ) {
    if ( rLeft < rRight )
        return +1;
    if ( rLeft > rRight )
        return -1;
    return 0;
}
inline Int RenderingManager::_Compare_GPUResourceView( GPUResourceView * const & rLeft, GPUResourceView * const & rRight, Void * /*pUserData*/ ) {
    if ( rLeft < rRight )
        return +1;
    if ( rLeft > rRight )
        return -1;
    return 0;
}
inline Int RenderingManager::_Compare_GPUShader( GPUShader * const & rLeft, GPUShader * const & rRight, Void * /*pUserData*/ ) {
    if ( rLeft < rRight )
        return +1;
    if ( rLeft > rRight )
        return -1;
    return 0;
}
inline Int RenderingManager::_Compare_GPU2DContext( GPU2DContext * const & rLeft, GPU2DContext * const & rRight, Void * /*pUserData*/ ) {
    if ( rLeft < rRight )
        return +1;
    if ( rLeft > rRight )
        return -1;
    return 0;
}

inline Void RenderingManager::_RegisterDeferredContext( GPUDeferredContext * pDeferredContext ) {
    Bool bInserted = m_lstDeferredContexts.Insert( pDeferredContext );
    Assert( bInserted );
}
inline Void RenderingManager::_RegisterInputLayout( GPUInputLayout * pInputLayout ) {
    Bool bInserted = m_lstInputLayouts.Insert( pInputLayout );
    Assert( bInserted );
}
inline Void RenderingManager::_RegisterRenderState( GPURenderState * pRenderState ) {
    Bool bInserted = m_lstRenderStates.Insert( pRenderState );
    Assert( bInserted );
}
inline Void RenderingManager::_RegisterAsynchronous( GPUAsynchronous * pAsynchronous ) {
    Bool bInserted = m_lstAsynchronouses.Insert( pAsynchronous );
    Assert( bInserted );
}
inline Void RenderingManager::_RegisterBuffer( GPUBuffer * pBuffer ) {
    Bool bInserted = m_lstBuffers.Insert( pBuffer );
    Assert( bInserted );
}
inline Void RenderingManager::_RegisterTexture( GPUTexture * pTexture ) {
    Bool bInserted = m_lstTextures.Insert( pTexture );
    Assert( bInserted );
}
inline Void RenderingManager::_RegisterResourceView( GPUResourceView * pResourceView ) {
    Bool bInserted = m_lstResourceViews.Insert( pResourceView );
    Assert( bInserted );
}
inline Void RenderingManager::_RegisterShader( GPUShader * pShader ) {
    Bool bInserted = m_lstShaders.Insert( pShader );
    Assert( bInserted );
}
inline Void RenderingManager::_Register2DContext( GPU2DContext * p2DContext ) {
    Bool bInserted = m_lst2DContexts.Insert( p2DContext );
    Assert( bInserted );
}

inline Void RenderingManager::_UnRegisterDeferredContext( GPUDeferredContext * pDeferredContext ) {
    Bool bRemoved = m_lstDeferredContexts.Remove( pDeferredContext );
    Assert( bRemoved );
}
inline Void RenderingManager::_UnRegisterInputLayout( GPUInputLayout * pInputLayout ) {
    Bool bRemoved = m_lstInputLayouts.Remove( pInputLayout );
    Assert( bRemoved );
}
inline Void RenderingManager::_UnRegisterRenderState( GPURenderState * pRenderState ) {
    Bool bRemoved = m_lstRenderStates.Remove( pRenderState );
    Assert( bRemoved );
}
inline Void RenderingManager::_UnRegisterAsynchronous( GPUAsynchronous * pAsynchronous ) {
    Bool bRemoved = m_lstAsynchronouses.Remove( pAsynchronous );
    Assert( bRemoved );
}
inline Void RenderingManager::_UnRegisterBuffer( GPUBuffer * pBuffer ) {
    Bool bRemoved = m_lstBuffers.Remove( pBuffer );
    Assert( bRemoved );
}
inline Void RenderingManager::_UnRegisterTexture( GPUTexture * pTexture ) {
    Bool bRemoved = m_lstTextures.Remove( pTexture );
    Assert( bRemoved );
}
inline Void RenderingManager::_UnRegisterResourceView( GPUResourceView * pResourceView ) {
    Bool bRemoved = m_lstResourceViews.Remove( pResourceView );
    Assert( bRemoved );
}
inline Void RenderingManager::_UnRegisterShader( GPUShader * pShader ) {
    Bool bRemoved = m_lstShaders.Remove( pShader );
    Assert( bRemoved );
}
inline Void RenderingManager::_UnRegister2DContext( GPU2DContext * p2DContext ) {
    Bool bRemoved = m_lst2DContexts.Remove( p2DContext );
    Assert( bRemoved );
}

inline Int RenderingManager::_Batch_Sort_FrontToBack( Float fLeft, Float fRight ) {
    if ( fLeft < fRight )
        return +1;
    if ( fRight < fLeft )
        return -1;
    return 0;
}
inline Int RenderingManager::_Batch_Sort_BackToFront( Float fLeft, Float fRight ) {
    if ( fLeft > fRight )
        return +1;
    if ( fRight > fLeft )
        return -1;
    return 0;
}

