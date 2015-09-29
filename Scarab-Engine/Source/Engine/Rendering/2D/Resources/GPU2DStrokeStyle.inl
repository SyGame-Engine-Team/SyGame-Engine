/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/2D/Resources/GPU2DStrokeStyle.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU 2D-Resource : StrokeStyles
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// GPU2DStrokeStyle implementation
inline Bool GPU2DStrokeStyle::IsBound() const {
    return m_hDevice2DStrokeStyle.IsCreated();
}

inline Void GPU2DStrokeStyle::Bind( const GPU2DStrokeStyleDesc * pDesc, const Float * arrDashes, UInt iDashCount ) {
    Assert( !(IsBound()) );
    m_hDevice2DStrokeStyle.Create( (const Device2DStrokeStyleDesc *)pDesc, arrDashes, iDashCount );
}
inline Void GPU2DStrokeStyle::UnBind() {
    Assert( IsBound() );
    m_hDevice2DStrokeStyle.Destroy();
}

inline GPU2DStrokeCapStyle GPU2DStrokeStyle::GetStartCap() const {
    Assert( IsBound() );
    return (GPU2DStrokeCapStyle)( m_hDevice2DStrokeStyle.GetStartCap() );
}
inline GPU2DStrokeCapStyle GPU2DStrokeStyle::GetEndCap() const {
    Assert( IsBound() );
    return (GPU2DStrokeCapStyle)( m_hDevice2DStrokeStyle.GetEndCap() );
}
inline GPU2DStrokeCapStyle GPU2DStrokeStyle::GetDashCap() const {
    Assert( IsBound() );
    return (GPU2DStrokeCapStyle)( m_hDevice2DStrokeStyle.GetDashCap() );
}

inline GPU2DStrokeLineJoin GPU2DStrokeStyle::GetLineJoin() const {
    Assert( IsBound() );
    return (GPU2DStrokeLineJoin)( m_hDevice2DStrokeStyle.GetLineJoin() );
}
inline Float GPU2DStrokeStyle::GetMiterLimit() const {
    Assert( IsBound() );
    return m_hDevice2DStrokeStyle.GetMiterLimit();
}

inline GPU2DStrokeDashStyle GPU2DStrokeStyle::GetDashStyle() const {
    Assert( IsBound() );
    return (GPU2DStrokeDashStyle)( m_hDevice2DStrokeStyle.GetDashStyle() );
}
inline Float GPU2DStrokeStyle::GetDashOffset() const {
    Assert( IsBound() );
    return m_hDevice2DStrokeStyle.GetDashOffset();
}

inline Void GPU2DStrokeStyle::GetDesc( GPU2DStrokeStyleDesc * outDesc ) const {
    Assert( IsBound() );
    m_hDevice2DStrokeStyle.GetDesc( (Device2DStrokeStyleDesc*)outDesc );
}

inline UInt GPU2DStrokeStyle::GetDashCount() const {
    Assert( IsBound() );
    return m_hDevice2DStrokeStyle.GetDashCount();
}
inline Void GPU2DStrokeStyle::GetDashes( Float * outDashes, UInt iMaxDashes ) const {
    Assert( IsBound() );
    m_hDevice2DStrokeStyle.GetDashes( outDashes, iMaxDashes );
}

