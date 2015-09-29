/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/Resources/D3D11Resource.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Platform-dependant abstraction for GPU resources.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// D3D11Resource implementation
inline D3D11ResourceUsage D3D11Resource::GetUsage() const {
    return m_iUsage;
}

inline Bool D3D11Resource::HasBind( D3D11ResourceBind iBind ) const {
    return ( (m_iBinds & iBind) != 0 );
}
inline UInt D3D11Resource::GetBinds() const {
    return m_iBinds;
}

inline Bool D3D11Resource::CanCPURead() const {
    return ( m_iUsage == D3D11RESOURCE_USAGE_STAGING );
}
inline Bool D3D11Resource::CanCPUWrite() const {
    return ( m_iUsage != D3D11RESOURCE_USAGE_CONST );
}
inline Bool D3D11Resource::CanGPUWrite() const {
    return ( m_iUsage == D3D11RESOURCE_USAGE_DEFAULT || m_iUsage == D3D11RESOURCE_USAGE_STAGING );
}
inline Bool D3D11Resource::CanUpdate() const {
    return ( m_iUsage == D3D11RESOURCE_USAGE_DEFAULT || m_iUsage == D3D11RESOURCE_USAGE_STAGING );
}
inline Bool D3D11Resource::CanLock() const {
    return ( m_iUsage == D3D11RESOURCE_USAGE_DYNAMIC || m_iUsage == D3D11RESOURCE_USAGE_STAGING );
}

inline Bool D3D11Resource::IsLocked() const {
    return m_bLocked;
}


