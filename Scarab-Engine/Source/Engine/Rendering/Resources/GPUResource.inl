/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Resources/GPUResource.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU Resource : Common interface
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// GPUResource implementation
inline Bool GPUResource::IsBound() const {
    return m_pDeviceResource->IsCreated();
}

inline GPUResourceType GPUResource::GetType() const {
    return (GPUResourceType)( m_pDeviceResource->GetType() );
}
inline Bool GPUResource::IsBuffer() const {
    return ( GetType() < GPURESOURCE_TEXTURE_1D );
}
inline Bool GPUResource::IsTexture() const {
    return ( GetType() >= GPURESOURCE_TEXTURE_1D );
}

inline GPUResourceUsage GPUResource::GetUsage() const {
    return (GPUResourceUsage)( m_pDeviceResource->GetUsage() );
}

inline Bool GPUResource::HasBind( GPUResourceBind iBind ) const {
    return m_pDeviceResource->HasBind( (DeviceResourceBind)iBind );
}
inline UInt GPUResource::GetBinds() const {
    return m_pDeviceResource->GetBinds();
}

inline Bool GPUResource::CanCPURead() const {
    return m_pDeviceResource->CanCPURead();
}
inline Bool GPUResource::CanCPUWrite() const {
    return m_pDeviceResource->CanCPUWrite();
}
inline Bool GPUResource::CanGPUWrite() const {
    return m_pDeviceResource->CanGPUWrite();
}
inline Bool GPUResource::CanUpdate() const {
    return m_pDeviceResource->CanUpdate();
}
inline Bool GPUResource::CanLock() const {
    return m_pDeviceResource->CanLock();
}

inline Bool GPUResource::IsLocked() const {
    return m_pDeviceResource->IsLocked();
}

inline Void GPUResource::SetUsage( GPUResourceUsage iUsage ) {
    Assert( !(IsBound()) );
    return m_pDeviceResource->SetUsage( (DeviceResourceUsage)iUsage );
}

inline GPUResourcePriority GPUResource::GetEvictionPriority() const {
    Assert( IsBound() );
    return (GPUResourcePriority)( m_pDeviceResource->GetEvictionPriority() );
}
inline Void GPUResource::SetEvictionPriority( GPUResourcePriority iPriority ) {
    Assert( IsBound() );
    m_pDeviceResource->SetEvictionPriority( (DeviceResourcePriority)iPriority );
}
