/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/2D/Resources/GPU2DRenderState.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU 2D-Resource : Render States
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// GPU2DRenderState implementation
inline Bool GPU2DRenderState::IsBound() const {
    return m_hDevice2DRenderState.IsCreated();
}

inline Void GPU2DRenderState::Bind( const GPU2DRenderStateDesc * pDesc ) {
    Assert( !(IsBound()) );
    m_hDevice2DRenderState.Create( (const Device2DRenderStateDesc *)pDesc );
}
inline Void GPU2DRenderState::UnBind() {
    Assert( IsBound() );
    m_hDevice2DRenderState.Destroy();
}

inline Void GPU2DRenderState::GetDesc( GPU2DRenderStateDesc * outDesc ) const {
    Assert( IsBound() );
    m_hDevice2DRenderState.GetDesc( (Device2DRenderStateDesc*)outDesc );
}
inline Void GPU2DRenderState::SetDesc( const GPU2DRenderStateDesc * pDesc ) {
    Assert( IsBound() );
    m_hDevice2DRenderState.SetDesc( (const Device2DRenderStateDesc *)pDesc );
}

inline Void GPU2DRenderState::GetTextRenderState( GPU2DTextRenderState * outTextRenderState ) const {
    Assert( IsBound() );
    Assert( !(outTextRenderState->IsBound()) );
    m_hDevice2DRenderState.GetTextRenderState( &(outTextRenderState->m_hDevice2DTextRenderState) );
}
inline Void GPU2DRenderState::SetTextRenderState( GPU2DTextRenderState * pTextRenderState ) {
    Assert( IsBound() );
    Assert( pTextRenderState == NULL || pTextRenderState->IsBound() );
    m_hDevice2DRenderState.SetTextRenderState( (pTextRenderState != NULL) ? &(pTextRenderState->m_hDevice2DTextRenderState) : NULL );
}

/////////////////////////////////////////////////////////////////////////////////
// GPU2DTextRenderState implementation
inline Bool GPU2DTextRenderState::IsBound() const {
    return m_hDevice2DTextRenderState.IsCreated();
}

inline Void GPU2DTextRenderState::Bind( Void * pMonitor ) {
    Assert( !(IsBound()) );
    m_hDevice2DTextRenderState.Create( pMonitor );
}
inline Void GPU2DTextRenderState::Bind( const GPU2DTextRenderStateDesc * pDesc ) {
    Assert( !(IsBound()) );
    m_hDevice2DTextRenderState.Create( (const Device2DTextRenderStateDesc *)pDesc );
}
inline Void GPU2DTextRenderState::UnBind() {
    Assert( IsBound() );
    m_hDevice2DTextRenderState.Destroy();
}

inline Float GPU2DTextRenderState::GetGamma() const {
    Assert( IsBound() );
    return m_hDevice2DTextRenderState.GetGamma();
}
inline Float GPU2DTextRenderState::GetEnhancedContrast() const {
    Assert( IsBound() );
    return m_hDevice2DTextRenderState.GetEnhancedContrast();
}
inline Float GPU2DTextRenderState::GetClearTypeLevel() const {
    Assert( IsBound() );
    return m_hDevice2DTextRenderState.GetClearTypeLevel();
}
inline GPU2DTextPixelGeometry GPU2DTextRenderState::GetPixelGeometry() const {
    Assert( IsBound() );
    return (GPU2DTextPixelGeometry)( m_hDevice2DTextRenderState.GetPixelGeometry() );
}
inline GPU2DTextRenderingMode GPU2DTextRenderState::GetRenderingMode() const {
    Assert( IsBound() );
    return (GPU2DTextRenderingMode)( m_hDevice2DTextRenderState.GetRenderingMode() );
}

inline Void GPU2DTextRenderState::GetDesc( GPU2DTextRenderStateDesc * outDesc ) const {
    Assert( IsBound() );
    m_hDevice2DTextRenderState.GetDesc( (Device2DTextRenderStateDesc*)outDesc );
}

