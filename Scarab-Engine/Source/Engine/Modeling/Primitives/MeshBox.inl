/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/Primitives/MeshBox.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives : Box
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// MeshBox implementation
inline const Box & MeshBox::GetGeometry() const {
    return m_vBox;
}

inline Bool MeshBox::IsInterior() const {
    return m_bInterior;
}

inline Void MeshBox::UpdateGeometry( const Box & vBox, GPUDeferredContext * pContext ) {
    m_vBox = vBox;
    _Update( pContext );
}

