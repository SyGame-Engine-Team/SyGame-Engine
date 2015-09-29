/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/Surfaces/MeshSurface.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Mesh Surface
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// MeshSurface implementation
inline const ParametricSurface * MeshSurface::GetGeometry() const {
    return m_pSurface;
}

inline UInt MeshSurface::GetSampleCountU() const {
    return m_iSampleCountU;
}
inline UInt MeshSurface::GetSampleCountV() const {
    return m_iSampleCountV;
}
inline Scalar MeshSurface::GetSampleU( UInt iSampleU ) const {
    Assert( m_arrSamplesU != NULL );
    Assert( iSampleU < m_iSampleCountU );
    return m_arrSamplesU[iSampleU];
}
inline Scalar MeshSurface::GetSampleV( UInt iSampleV ) const {
    Assert( m_arrSamplesV != NULL );
    Assert( iSampleV < m_iSampleCountV );
    return m_arrSamplesV[iSampleV];
}

inline ParametricSurface * MeshSurface::UpdateGeometryBegin() {
    return m_pSurface;
}
inline Void MeshSurface::UpdateGeometryEnd( GPUDeferredContext * pContext ) {
    m_pSurface->SubDivideByParameter( m_arrSamplesU, m_iSampleCountU, m_arrSamplesV, m_iSampleCountV );
    _Update( pContext );
}

