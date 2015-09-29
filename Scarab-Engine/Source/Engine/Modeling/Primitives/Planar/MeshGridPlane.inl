/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/Primitives/Planar/MeshGridPlane.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives : Planar, Grid-Plane
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// MeshGridPlane implementation
inline const Plane & MeshGridPlane::GetGeometry() const {
    return m_vPlane;
}
inline Scalar MeshGridPlane::GetWidth() const {
    return m_fWidth;
}
inline Scalar MeshGridPlane::GetHeight() const {
    return m_fHeight;
}

inline UInt MeshGridPlane::GetWidthSamples() const {
    return m_iWidthSamples;
}
inline UInt MeshGridPlane::GetHeightSamples() const {
    return m_iHeightSamples;
}

inline Void MeshGridPlane::UpdateGeometry( const Plane & vPlane, Scalar fWidth, Scalar fHeight, GPUDeferredContext * pContext ) {
    m_vPlane = vPlane;
    m_fWidth = fWidth;
    m_fHeight = fHeight;
    _Update( pContext );
}
