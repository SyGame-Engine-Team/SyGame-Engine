/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/Primitives/MeshTorus.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives : Torus
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// MeshTorus implementation
inline const Torus & MeshTorus::GetGeometry() const {
    return m_vTorus;
}

inline UInt MeshTorus::GetSliceSamples() const {
    return m_iSliceSamples;
}
inline UInt MeshTorus::GetRadialSamples() const {
    return m_iRadialSamples;
}

inline Bool MeshTorus::IsInterior() const {
    return m_bInterior;
}

inline Void MeshTorus::UpdateGeometry( const Torus & vTorus, GPUDeferredContext * pContext ) {
    m_vTorus = vTorus;
    _Update( pContext );
}

