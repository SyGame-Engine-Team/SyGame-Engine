/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Resources/GPURenderState.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU Resource : Render States
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// GPURenderState implementation
inline Bool GPURenderState::IsBound() const {
    return m_pDeviceRenderState->IsCreated();
}

inline GPURenderStateType GPURenderState::GetType() const {
    return (GPURenderStateType)( m_pDeviceRenderState->GetType() );
}

/////////////////////////////////////////////////////////////////////////////////
// GPUSamplerState implementation
inline Void GPUSamplerState::GetDesc( GPUSamplerStateDesc * outDesc ) const {
    Assert( IsBound() );
    m_hDeviceSamplerState.GetDesc( (DeviceSamplerStateDesc*)outDesc );
}

/////////////////////////////////////////////////////////////////////////////////
// GPURasterizerState implementation
inline Void GPURasterizerState::GetDesc( GPURasterizerStateDesc * outDesc ) const {
    Assert( IsBound() );
    m_hDeviceRasterizerState.GetDesc( (DeviceRasterizerStateDesc*)outDesc );
}

/////////////////////////////////////////////////////////////////////////////////
// GPUDepthStencilState implementation
inline Void GPUDepthStencilState::GetDesc( GPUDepthStencilStateDesc * outDesc ) const {
    Assert( IsBound() );
    m_hDeviceDepthStencilState.GetDesc( (DeviceDepthStencilStateDesc*)outDesc );
}

/////////////////////////////////////////////////////////////////////////////////
// GPUBlendState implementation
inline Void GPUBlendState::GetDesc( GPUBlendStateDesc * outDesc ) const {
    Assert( IsBound() );
    m_hDeviceBlendState.GetDesc( (DeviceBlendStateDesc*)outDesc );
}

