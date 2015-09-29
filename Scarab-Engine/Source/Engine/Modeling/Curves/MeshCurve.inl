/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/Curves/MeshCurve.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Mesh Curve
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// MeshCurve implementation
inline const Curve3 * MeshCurve::GetGeometry( Bool * outIsClosed ) const {
    if ( outIsClosed != NULL )
        *outIsClosed = m_bClosed;
    return m_pCurve;
}

inline UInt MeshCurve::GetSampleCount() const {
    return m_iSampleCount;
}
inline Scalar MeshCurve::GetSample( UInt iSample ) const {
    Assert( iSample < m_iSampleCount );
    return m_arrSamples[iSample];
}

inline Curve3 * MeshCurve::UpdateGeometryBegin( Bool * outIsClosed ) {
    *outIsClosed = m_bClosed;
    return m_pCurve;
}
inline Void MeshCurve::UpdateGeometryEnd( Bool bClosed, GPUDeferredContext * pContext ) {
    m_bClosed = bClosed;
    m_pCurve->SubDivideByParameter( m_arrSamples, m_iSampleCount );
    _Update( pContext );
}


