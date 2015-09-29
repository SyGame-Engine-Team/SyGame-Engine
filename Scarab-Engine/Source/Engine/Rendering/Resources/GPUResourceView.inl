/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Resources/GPUResourceView.inl
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
// GPUResourceView implementation
inline Bool GPUResourceView::IsBound() const {
    return m_pDeviceResourceView->IsCreated();
}
inline Bool GPUResourceView::IsBoundToBackBuffer( UInt * outBackBuffer ) const {
    Assert( IsBound() );
    return m_pDeviceResourceView->IsBoundToBackBuffer( outBackBuffer );
}

inline GPUResourceViewType GPUResourceView::GetType() const {
    return (GPUResourceViewType)( m_pDeviceResourceView->GetType() );
}

inline GPUResource * GPUResourceView::GetResource() const {
    return m_pResource;
}

/////////////////////////////////////////////////////////////////////////////////
// GPURenderTargetView implementation
inline Void GPURenderTargetView::GetDesc( GPURenderTargetViewDesc * outDesc ) const {
    Assert( IsBound() );
    m_hDeviceRenderTargetView.GetDesc( (DeviceRenderTargetViewDesc*)outDesc );
}

/////////////////////////////////////////////////////////////////////////////////
// GPUDepthStencilView implementation
inline Void GPUDepthStencilView::GetDesc( GPUDepthStencilViewDesc * outDesc ) const {
    Assert( IsBound() );
    m_hDeviceDepthStencilView.GetDesc( (DeviceDepthStencilViewDesc*)outDesc );
}

/////////////////////////////////////////////////////////////////////////////////
// GPUShaderView implementation
inline Void GPUShaderView::GetDesc( GPUShaderViewDesc * outDesc ) const {
    Assert( IsBound() );
    m_hDeviceShaderView.GetDesc( (DeviceShaderViewDesc*)outDesc );
}

/////////////////////////////////////////////////////////////////////////////////
// GPUUnorderedAccessView implementation
inline Void GPUUnorderedAccessView::GetDesc( GPUUnorderedAccessViewDesc * outDesc ) const {
    Assert( IsBound() );
    m_hDeviceUnorderedAccessView.GetDesc( (DeviceUnorderedAccessViewDesc*)outDesc );
}

