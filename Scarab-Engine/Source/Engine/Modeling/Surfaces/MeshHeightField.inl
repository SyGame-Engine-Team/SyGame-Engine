/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/Surfaces/MeshHeightField.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Mesh Height-Field surface (landscapes)
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// MeshHeightField implementation
inline const Scalar * MeshHeightField::GetGeometry() const {
    return m_arrHeightField;
}

inline const Vertex2 & MeshHeightField::GetOrigin() const {
    return m_vOrigin;
}
inline const Vector2 & MeshHeightField::GetSize() const {
    return m_vSize;
}
inline Scalar MeshHeightField::GetMinHeight() const {
    return m_fMinHeight;
}
inline Scalar MeshHeightField::GetMaxHeight() const {
    return m_fMaxHeight;
}

inline UInt MeshHeightField::GetSampleCountU() const {
    return m_iSampleCountU;
}
inline UInt MeshHeightField::GetSampleCountV() const {
    return m_iSampleCountV;
}
inline const Vector2 & MeshHeightField::GetSampleSize() const {
    return m_vSampleSize;
}
inline Scalar MeshHeightField::GetHeight( UInt iSampleU, UInt iSampleV ) const {
    Assert( iSampleU < m_iSampleCountU );
    Assert( iSampleV < m_iSampleCountV );
    return m_arrHeightField[(iSampleV * m_iSampleCountU) + iSampleU];
}

inline Scalar * MeshHeightField::UpdateGeometryBegin( UInt * outSampleCountU, UInt * outSampleCountV ) {
    *outSampleCountU = m_iSampleCountU;
    *outSampleCountV = m_iSampleCountV;
    return m_arrHeightField;
}
inline Void MeshHeightField::UpdateGeometryEnd( const Vertex2 & vOrigin, const Vector2 & vSize, Scalar fMinHeight, Scalar fMaxHeight, GPUDeferredContext * pContext ) {
    m_vOrigin = vOrigin;
    m_vSize = vSize;
    m_fMinHeight = fMinHeight;
    m_fMaxHeight = fMaxHeight;
    m_vSampleSize.X = ( vSize.X / (Scalar)(m_iSampleCountU - 1) );
    m_vSampleSize.Y = ( vSize.Y / (Scalar)(m_iSampleCountV - 1) );
    m_vInvSampleSize.X = MathFn->Invert( m_vSampleSize.X );
    m_vInvSampleSize.Y = MathFn->Invert( m_vSampleSize.Y );
    m_fSampleDiagSlope = ( m_vSampleSize.Y * m_vInvSampleSize.X );
    _Update( pContext );
}

