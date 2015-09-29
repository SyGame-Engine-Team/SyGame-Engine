/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/2D/Resources/GPU2DLayer.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU 2D-Resource : Layers
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// GPU2DLayer implementation
inline Bool GPU2DLayer::IsBound() const {
    return m_hDevice2DLayer.IsCreated();
}

inline Float GPU2DLayer::GetInitialDIPWidth() const {
    Assert( IsBound() );
    return m_hDevice2DLayer.GetInitialDIPWidth();
}
inline Float GPU2DLayer::GetInitialDIPHeight() const {
    Assert( IsBound() );
    return m_hDevice2DLayer.GetInitialDIPHeight();
}

inline Void GPU2DLayer::GetCurrentDIPSize( Float * outWidth, Float * outHeight ) const {
    Assert( IsBound() );
    m_hDevice2DLayer.GetCurrentDIPSize( outWidth, outHeight );
}
inline Void GPU2DLayer::UpdateInitialDIPSize() {
    Assert( IsBound() );
    m_hDevice2DLayer.UpdateInitialDIPSize();
}

