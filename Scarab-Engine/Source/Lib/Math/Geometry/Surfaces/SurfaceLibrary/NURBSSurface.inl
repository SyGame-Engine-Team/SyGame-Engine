/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Surfaces/SurfaceLibrary/NURBSSurface.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Parametric Surfaces : NURBS surfaces
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TNURBSSurface implementation
template<typename Real>
TNURBSSurface<Real>::TNURBSSurface( const TVertex3<Real> * arrControlPoints, const Real * arrWeights,
                                    UInt iControlPointCountU, UInt iControlPointCountV,
                                    UInt iDegreeU, UInt iDegreeV, Bool bLoopU, Bool bLoopV,
                                    Bool bPeriodicU, Bool bPeriodicV ):
    TParametricSurface<Real>( (Real)0, (Real)1, (Real)0, (Real)1, false ),
    m_BasisU(), m_BasisV()
{
    Assert( iControlPointCountU >= 2 );
    Assert( iControlPointCountV >= 2 );
    Assert( iDegreeU >= 1 && iDegreeU < iControlPointCountU );
    Assert( iDegreeV >= 1 && iDegreeV < iControlPointCountV );

    m_iControlPointCountU = iControlPointCountU;
    m_iControlPointCountV = iControlPointCountV;
    m_iReplicateCountU = ( bLoopU ? ( bPeriodicU ? iDegreeU : 1 ) : 0 );
    m_iReplicateCountV = ( bLoopV ? ( bPeriodicV ? iDegreeV : 1 ) : 0 );

    m_bLoopU = bLoopU;
    m_bLoopV = bLoopV;

    m_BasisU.InitUniform( m_iControlPointCountU + m_iReplicateCountU, iDegreeU, bPeriodicU );
    m_BasisV.InitUniform( m_iControlPointCountV + m_iReplicateCountV, iDegreeV, bPeriodicV );
    _CreateControlPoints( arrControlPoints, arrWeights );
}
template<typename Real>
TNURBSSurface<Real>::TNURBSSurface( const TVertex3<Real> * arrControlPoints, const Real * arrWeights,
                                    UInt iControlPointCountU, UInt iControlPointCountV,
                                    UInt iDegreeU, UInt iDegreeV, Bool bLoopU, Bool bLoopV,
                                    Bool bPeriodicU, const Real * arrKnotsV ):
    TParametricSurface<Real>( (Real)0, (Real)1, (Real)0, (Real)1, false ),
    m_BasisU(), m_BasisV()
{
    Assert( iControlPointCountU >= 2 );
    Assert( iControlPointCountV >= 2 );
    Assert( iDegreeU >= 1 && iDegreeU < iControlPointCountU );
    Assert( iDegreeV >= 1 && iDegreeV < iControlPointCountV );

    m_iControlPointCountU = iControlPointCountU;
    m_iControlPointCountV = iControlPointCountV;
    m_iReplicateCountU = ( bLoopU ? ( bPeriodicU ? iDegreeU : 1 ) : 0 );
    m_iReplicateCountV = ( bLoopV ? 1 : 0 );

    m_bLoopU = bLoopU;
    m_bLoopV = bLoopV;

    m_BasisU.InitUniform( m_iControlPointCountU + m_iReplicateCountU, iDegreeU, bPeriodicU );
    m_BasisV.InitNonUniform( m_iControlPointCountV + m_iReplicateCountV, iDegreeV, arrKnotsV );
    _CreateControlPoints( arrControlPoints, arrWeights );
}
template<typename Real>
TNURBSSurface<Real>::TNURBSSurface( const TVertex3<Real> * arrControlPoints, const Real * arrWeights,
                                    UInt iControlPointCountU, UInt iControlPointCountV,
                                    UInt iDegreeU, UInt iDegreeV, Bool bLoopU, Bool bLoopV,
                                    const Real * arrKnotsU, Bool bPeriodicV ):
    TParametricSurface<Real>( (Real)0, (Real)1, (Real)0, (Real)1, false ),
    m_BasisU(), m_BasisV()
{
    Assert( iControlPointCountU >= 2 );
    Assert( iControlPointCountV >= 2 );
    Assert( iDegreeU >= 1 && iDegreeU < iControlPointCountU );
    Assert( iDegreeV >= 1 && iDegreeV < iControlPointCountV );

    m_iControlPointCountU = iControlPointCountU;
    m_iControlPointCountV = iControlPointCountV;
    m_iReplicateCountU = ( bLoopU ? 1 : 0 );
    m_iReplicateCountV = ( bLoopV ? ( bPeriodicV ? iDegreeV : 1 ) : 0 );

    m_bLoopU = bLoopU;
    m_bLoopV = bLoopV;

    m_BasisU.InitNonUniform( m_iControlPointCountU + m_iReplicateCountU, iDegreeU, arrKnotsU );
    m_BasisV.InitUniform( m_iControlPointCountV + m_iReplicateCountV, iDegreeV, bPeriodicV );
    _CreateControlPoints( arrControlPoints, arrWeights );
}
template<typename Real>
TNURBSSurface<Real>::TNURBSSurface( const TVertex3<Real> * arrControlPoints, const Real * arrWeights,
                                    UInt iControlPointCountU, UInt iControlPointCountV,
                                    UInt iDegreeU, UInt iDegreeV, Bool bLoopU, Bool bLoopV,
                                    const Real * arrKnotsU, const Real * arrKnotsV ):
    TParametricSurface<Real>( (Real)0, (Real)1, (Real)0, (Real)1, false ),
    m_BasisU(), m_BasisV()
{
    Assert( iControlPointCountU >= 2 );
    Assert( iControlPointCountV >= 2 );
    Assert( iDegreeU >= 1 && iDegreeU < iControlPointCountU );
    Assert( iDegreeV >= 1 && iDegreeV < iControlPointCountV );

    m_iControlPointCountU = iControlPointCountU;
    m_iControlPointCountV = iControlPointCountV;
    m_iReplicateCountU = ( bLoopU ? 1 : 0 );
    m_iReplicateCountV = ( bLoopV ? 1 : 0 );

    m_bLoopU = bLoopU;
    m_bLoopV = bLoopV;

    m_BasisU.InitNonUniform( m_iControlPointCountU + m_iReplicateCountU, iDegreeU, arrKnotsU );
    m_BasisV.InitNonUniform( m_iControlPointCountV + m_iReplicateCountV, iDegreeV, arrKnotsV );
    _CreateControlPoints( arrControlPoints, arrWeights );
}
template<typename Real>
TNURBSSurface<Real>::~TNURBSSurface()
{
    DeleteA( m_arrControlPoints );
    DeleteA( m_arrWeights );
}

template<typename Real>
inline UInt TNURBSSurface<Real>::GetDegreeU() const {
    return m_BasisU.GetDegree();
}
template<typename Real>
inline UInt TNURBSSurface<Real>::GetDegreeV() const {
    return m_BasisV.GetDegree();
}

template<typename Real>
inline Bool TNURBSSurface<Real>::IsUniformU() const {
    return m_BasisU.IsUniform();
}
template<typename Real>
inline Bool TNURBSSurface<Real>::IsUniformV() const {
    return m_BasisV.IsUniform();
}
template<typename Real>
inline Bool TNURBSSurface<Real>::IsPeriodicU() const {
    return m_BasisU.IsPeriodic();
}
template<typename Real>
inline Bool TNURBSSurface<Real>::IsPeriodicV() const {
    return m_BasisV.IsPeriodic();
}
template<typename Real>
inline Bool TNURBSSurface<Real>::IsLoopU() const {
    return m_bLoopU;
}
template<typename Real>
inline Bool TNURBSSurface<Real>::IsLoopV() const {
    return m_bLoopV;
}

template<typename Real>
inline UInt TNURBSSurface<Real>::GetControlPointCountU() const {
    return m_iControlPointCountU;
}
template<typename Real>
inline UInt TNURBSSurface<Real>::GetControlPointCountV() const {
    return m_iControlPointCountV;
}
template<typename Real>
inline const TVertex3<Real> & TNURBSSurface<Real>::GetControlPoint( UInt iU, UInt iV ) const {
    Assert( iU < m_iControlPointCountU );
    Assert( iV < m_iControlPointCountV );
    return m_arrControlPoints[ ( iV * m_iControlPointsPitch ) + iU ];
}
template<typename Real>
Void TNURBSSurface<Real>::SetControlPoint( UInt iU, UInt iV, const TVertex3<Real> & vPoint )
{
    Assert( iU < m_iControlPointCountU );
    Assert( iV < m_iControlPointCountV );
    UInt iIndex = ( iV * m_iControlPointsPitch + iU );
    m_arrControlPoints[iIndex] = vPoint;

    Bool bReplicateU = (iU < m_iReplicateCountU);
    Bool bReplicateV = (iV < m_iReplicateCountV);
    if ( bReplicateU )
        m_arrControlPoints[iIndex + m_iControlPointCountU] = vPoint;
    if ( bReplicateV ) {
        iIndex = ( (m_iControlPointCountV + iV) * m_iControlPointsPitch + iU );
        m_arrControlPoints[iIndex] = vPoint;
    }
    if ( bReplicateU && bReplicateV )
        m_arrControlPoints[iIndex + m_iControlPointCountU] = vPoint;
}

template<typename Real>
inline Real TNURBSSurface<Real>::GetWeight( UInt iU, UInt iV ) const {
    Assert( iU < m_iControlPointCountU );
    Assert( iV < m_iControlPointCountV );
    return m_arrWeights[ ( iV * m_iControlPointsPitch ) + iU ];
}
template<typename Real>
Void TNURBSSurface<Real>::SetWeight( UInt iU, UInt iV, Real fWeightValue )
{
    Assert( iU < m_iControlPointCountU );
    Assert( iV < m_iControlPointCountV );
    UInt iIndex = ( iV * m_iControlPointsPitch + iU );
    m_arrWeights[iIndex] = fWeightValue;

    Bool bReplicateU = (iU < m_iReplicateCountU);
    Bool bReplicateV = (iV < m_iReplicateCountV);
    if ( bReplicateU )
        m_arrWeights[iIndex + m_iControlPointCountU] = fWeightValue;
    if ( bReplicateV ) {
        iIndex = ( (m_iControlPointCountV + iV) * m_iControlPointsPitch + iU );
        m_arrWeights[iIndex] = fWeightValue;
    }
    if ( bReplicateU && bReplicateV )
        m_arrWeights[iIndex + m_iControlPointCountU] = fWeightValue;
}

template<typename Real>
inline UInt TNURBSSurface<Real>::GetKnotCountU() const {
    return m_BasisU.GetKnotCount();
}
template<typename Real>
inline UInt TNURBSSurface<Real>::GetKnotCountV() const {
    return m_BasisV.GetKnotCount();
}
template<typename Real>
inline Real TNURBSSurface<Real>::GetKnotU( UInt iKnot ) const {
    m_BasisU.GetKnot( iKnot );
}
template<typename Real>
inline Real TNURBSSurface<Real>::GetKnotV( UInt iKnot ) const {
    m_BasisV.GetKnot( iKnot );
}
template<typename Real>
inline Void TNURBSSurface<Real>::SetKnotU( UInt iKnot, Real fKnotValue ) {
    m_BasisU.SetKnot( iKnot, fKnotValue );
}
template<typename Real>
inline Void TNURBSSurface<Real>::SetKnotV( UInt iKnot, Real fKnotValue ) {
    m_BasisV.SetKnot( iKnot, fKnotValue );
}

template<typename Real>
TVertex3<Real> TNURBSSurface<Real>::P( Real fU, Real fV ) const
{
    UInt iU, iMinU, iMaxU;
    m_BasisU.Evaluate( fU, 0, iMinU, iMaxU );

    UInt iV, iMinV, iMaxV;
    m_BasisV.Evaluate( fV, 0, iMinV, iMaxV );

    Real fTmp;
    UInt iIndex;

    TVertex3<Real> vRes = TVertex3<Real>::Null;
    Real fW = (Real)0, fInvW;
    for( iV = iMinV; iV <= iMaxV; ++iV ) {
        for( iU = iMinU; iU <= iMaxU; ++iU ) {
            iIndex = (iV * m_iControlPointsPitch) + iU;
            fTmp = m_BasisU.N(iU) * m_BasisV.N(iV) * m_arrWeights[iIndex];
            vRes += ( m_arrControlPoints[iIndex].ToVector() * fTmp );
            fW += fTmp;
        }
    }
    fInvW = MathRealFn->Invert( fW );
    vRes *= fInvW;

    return vRes;
}
template<typename Real>
TVector3<Real> TNURBSSurface<Real>::PdU( Real fU, Real fV ) const
{
    UInt iU, iMinU, iMaxU;
    m_BasisU.Evaluate( fU, 0, iMinU, iMaxU );
    m_BasisU.Evaluate( fU, 1, iMinU, iMaxU );

    UInt iV, iMinV, iMaxV;
    m_BasisV.Evaluate( fV, 0, iMinV, iMaxV );

    Real fTmp;
    UInt iIndex;

    TVector3<Real> vP = TVector3<Real>::Null;
    Real fW = (Real)0, fInvW;
    for( iV = iMinV; iV <= iMaxV; ++iV ) {
        for( iU = iMinU; iU <= iMaxU; ++iU ) {
            iIndex = (iV * m_iControlPointsPitch) + iU;
            fTmp = m_BasisU.N(iU) * m_BasisV.N(iV) * m_arrWeights[iIndex];
            vP += ( m_arrControlPoints[iIndex].ToVector() * fTmp );
            fW += fTmp;
        }
    }
    fInvW = MathRealFn->Invert( fW );
    vP *= fInvW;

    TVector3<Real> vRes = TVector3<Real>::Null;
    Real fWdU = (Real)0;
    for( iV = iMinV; iV <= iMaxV; ++iV ) {
        for( iU = iMinU; iU <= iMaxU; ++iU ) {
            iIndex = (iV * m_iControlPointsPitch) + iU;
            fTmp = m_BasisU.NdT(iU) * m_BasisV.N(iV) * m_arrWeights[iIndex];
            vRes += ( m_arrControlPoints[iIndex].ToVector() * fTmp );
            fWdU += fTmp;
        }
    }
    vRes = ( vRes - (vP * fWdU) ) * fInvW;

    return vRes;
}
template<typename Real>
TVector3<Real> TNURBSSurface<Real>::PdV( Real fU, Real fV ) const
{
    UInt iU, iMinU, iMaxU;
    m_BasisU.Evaluate( fU, 0, iMinU, iMaxU );

    UInt iV, iMinV, iMaxV;
    m_BasisV.Evaluate( fV, 0, iMinV, iMaxV );
    m_BasisV.Evaluate( fV, 1, iMinV, iMaxV );

    Real fTmp;
    UInt iIndex;

    TVector3<Real> vP = TVector3<Real>::Null;
    Real fW = (Real)0, fInvW;
    for( iV = iMinV; iV <= iMaxV; ++iV ) {
        for( iU = iMinU; iU <= iMaxU; ++iU ) {
            iIndex = (iV * m_iControlPointsPitch) + iU;
            fTmp = m_BasisU.N(iU) * m_BasisV.N(iV) * m_arrWeights[iIndex];
            vP += ( m_arrControlPoints[iIndex].ToVector() * fTmp );
            fW += fTmp;
        }
    }
    fInvW = MathRealFn->Invert( fW );
    vP *= fInvW;

    TVector3<Real> vRes = TVector3<Real>::Null;
    Real fWdV = (Real)0;
    for( iV = iMinV; iV <= iMaxV; ++iV ) {
        for( iU = iMinU; iU <= iMaxU; ++iU ) {
            iIndex = (iV * m_iControlPointsPitch) + iU;
            fTmp = m_BasisU.N(iU) * m_BasisV.NdT(iV) * m_arrWeights[iIndex];
            vRes += ( m_arrControlPoints[iIndex].ToVector() * fTmp );
            fWdV += fTmp;
        }
    }
    vRes = ( vRes - (vP * fWdV) ) * fInvW;

    return vRes;
}
template<typename Real>
TVector3<Real> TNURBSSurface<Real>::PdU2( Real fU, Real fV ) const
{
    UInt iU, iMinU, iMaxU;
    m_BasisU.Evaluate( fU, 0, iMinU, iMaxU );
    m_BasisU.Evaluate( fU, 1, iMinU, iMaxU );
    m_BasisU.Evaluate( fU, 2, iMinU, iMaxU );

    UInt iV, iMinV, iMaxV;
    m_BasisV.Evaluate( fV, 0, iMinV, iMaxV );

    Real fTmp;
    UInt iIndex;

    TVector3<Real> vP = TVector3<Real>::Null;
    Real fW = (Real)0, fInvW;
    for( iV = iMinV; iV <= iMaxV; ++iV ) {
        for( iU = iMinU; iU <= iMaxU; ++iU ) {
            iIndex = (iV * m_iControlPointsPitch) + iU;
            fTmp = m_BasisU.N(iU) * m_BasisV.N(iV) * m_arrWeights[iIndex];
            vP += ( m_arrControlPoints[iIndex].ToVector() * fTmp );
            fW += fTmp;
        }
    }
    fInvW = MathRealFn->Invert( fW );
    vP *= fInvW;

    TVector3<Real> vPdU = TVector3<Real>::Null;
    Real fWdU = (Real)0;
    for( iV = iMinV; iV <= iMaxV; ++iV ) {
        for( iU = iMinU; iU <= iMaxU; ++iU ) {
            iIndex = (iV * m_iControlPointsPitch) + iU;
            fTmp = m_BasisU.NdT(iU) * m_BasisV.N(iV) * m_arrWeights[iIndex];
            vPdU += ( m_arrControlPoints[iIndex].ToVector() * fTmp );
            fWdU += fTmp;
        }
    }
    vPdU = ( vPdU - (vP * fWdU) ) * fInvW;

    TVector3<Real> vRes = TVector3<Real>::Null;
    Real fWdU2 = (Real)0;
    for( iV = iMinV; iV <= iMaxV; ++iV ) {
        for( iU = iMinU; iU <= iMaxU; ++iU ) {
            iIndex = (iV * m_iControlPointsPitch) + iU;
            fTmp = m_BasisU.NdT2(iU) * m_BasisV.N(iV) * m_arrWeights[iIndex];
            vRes += ( m_arrControlPoints[iIndex].ToVector() * fTmp );
            fWdU2 += fTmp;
        }
    }
    vRes = ( vRes - (vPdU * (fWdU * (Real)2)) - (vP * fWdU2) ) * fInvW;

    return vRes;
}
template<typename Real>
TVector3<Real> TNURBSSurface<Real>::PdV2( Real fU, Real fV ) const
{
    UInt iU, iMinU, iMaxU;
    m_BasisU.Evaluate( fU, 0, iMinU, iMaxU );

    UInt iV, iMinV, iMaxV;
    m_BasisV.Evaluate( fV, 0, iMinV, iMaxV );
    m_BasisV.Evaluate( fV, 1, iMinV, iMaxV );
    m_BasisV.Evaluate( fV, 2, iMinV, iMaxV );

    Real fTmp;
    UInt iIndex;

    TVector3<Real> vP = TVector3<Real>::Null;
    Real fW = (Real)0, fInvW;
    for( iV = iMinV; iV <= iMaxV; ++iV ) {
        for( iU = iMinU; iU <= iMaxU; ++iU ) {
            iIndex = (iV * m_iControlPointsPitch) + iU;
            fTmp = m_BasisU.N(iU) * m_BasisV.N(iV) * m_arrWeights[iIndex];
            vP += ( m_arrControlPoints[iIndex].ToVector() * fTmp );
            fW += fTmp;
        }
    }
    fInvW = MathRealFn->Invert( fW );
    vP *= fInvW;

    TVector3<Real> vPdV = TVector3<Real>::Null;
    Real fWdV = (Real)0;
    for( iV = iMinV; iV <= iMaxV; ++iV ) {
        for( iU = iMinU; iU <= iMaxU; ++iU ) {
            iIndex = (iV * m_iControlPointsPitch) + iU;
            fTmp = m_BasisU.N(iU) * m_BasisV.NdT(iV) * m_arrWeights[iIndex];
            vPdV += ( m_arrControlPoints[iIndex].ToVector() * fTmp );
            fWdV += fTmp;
        }
    }
    vPdV = ( vPdV - (vP * fWdV) ) * fInvW;

    TVector3<Real> vRes = TVector3<Real>::Null;
    Real fWdV2 = (Real)0;
    for( iV = iMinV; iV <= iMaxV; ++iV ) {
        for( iU = iMinU; iU <= iMaxU; ++iU ) {
            iIndex = (iV * m_iControlPointsPitch) + iU;
            fTmp = m_BasisU.N(iU) * m_BasisV.NdT2(iV) * m_arrWeights[iIndex];
            vRes += ( m_arrControlPoints[iIndex].ToVector() * fTmp );
            fWdV2 += fTmp;
        }
    }
    vRes = ( vRes - (vPdV * (fWdV * (Real)2)) - (vP * fWdV2) ) * fInvW;

    return vRes;
}
template<typename Real>
TVector3<Real> TNURBSSurface<Real>::PdUdV( Real fU, Real fV ) const
{
    UInt iU, iMinU, iMaxU;
    m_BasisU.Evaluate( fU, 0, iMinU, iMaxU );
    m_BasisU.Evaluate( fU, 1, iMinU, iMaxU );

    UInt iV, iMinV, iMaxV;
    m_BasisV.Evaluate( fV, 0, iMinV, iMaxV );
    m_BasisV.Evaluate( fV, 1, iMinV, iMaxV );

    Real fTmp;
    UInt iIndex;

    TVector3<Real> vP = TVector3<Real>::Null;
    Real fW = (Real)0, fInvW;
    for( iV = iMinV; iV <= iMaxV; ++iV ) {
        for( iU = iMinU; iU <= iMaxU; ++iU ) {
            iIndex = (iV * m_iControlPointsPitch) + iU;
            fTmp = m_BasisU.N(iU) * m_BasisV.N(iV) * m_arrWeights[iIndex];
            vP += ( m_arrControlPoints[iIndex].ToVector() * fTmp );
            fW += fTmp;
        }
    }
    fInvW = MathRealFn->Invert( fW );
    vP *= fInvW;

    TVector3<Real> vPdU = TVector3<Real>::Null;
    Real fWdU = (Real)0;
    for( iV = iMinV; iV <= iMaxV; ++iV ) {
        for( iU = iMinU; iU <= iMaxU; ++iU ) {
            iIndex = (iV * m_iControlPointsPitch) + iU;
            fTmp = m_BasisU.NdT(iU) * m_BasisV.N(iV) * m_arrWeights[iIndex];
            vPdU += ( m_arrControlPoints[iIndex].ToVector() * fTmp );
            fWdU += fTmp;
        }
    }
    vPdU = ( vPdU - (vP * fWdU) ) * fInvW;

    TVector3<Real> vPdV = TVector3<Real>::Null;
    Real fWdV = (Real)0;
    for( iV = iMinV; iV <= iMaxV; ++iV ) {
        for( iU = iMinU; iU <= iMaxU; ++iU ) {
            iIndex = (iV * m_iControlPointsPitch) + iU;
            fTmp = m_BasisU.N(iU) * m_BasisV.NdT(iV) * m_arrWeights[iIndex];
            vPdV += ( m_arrControlPoints[iIndex].ToVector() * fTmp );
            fWdV += fTmp;
        }
    }
    vPdV = ( vPdV - (vP * fWdV) ) * fInvW;

    TVector3<Real> vRes = TVector3<Real>::Null;
    Real fWdUdV = (Real)0;
    for( iV = iMinV; iV <= iMaxV; ++iV ) {
        for( iU = iMinU; iU <= iMaxU; ++iU ) {
            iIndex = (iV * m_iControlPointsPitch) + iU;
            fTmp = m_BasisU.NdT(iU) * m_BasisV.NdT(iV) * m_arrWeights[iIndex];
            vRes += ( m_arrControlPoints[iIndex].ToVector() * fTmp );
            fWdUdV += fTmp;
        }
    }
    vRes = ( vRes - (vPdV * fWdU) - (vPdU * fWdV) - (vP * fWdUdV) ) * fInvW;

    return vRes;
}

template<typename Real>
Void TNURBSSurface<Real>::Evaluate( Real fU, Real fV, TVertex3<Real> * outP, TVector3<Real> * outPdU, TVector3<Real> * outPdV,
                                                      TVector3<Real> * outPdU2, TVector3<Real> * outPdV2, TVector3<Real> * outPdUdV ) const
{
    UInt iU, iMinU, iMaxU;
    if ( outPdU2 != NULL ) {
        m_BasisU.Evaluate( fU, 0, iMinU, iMaxU );
        m_BasisU.Evaluate( fU, 1, iMinU, iMaxU );
        m_BasisU.Evaluate( fU, 2, iMinU, iMaxU );
    } else if ( outPdU != NULL || outPdUdV != NULL ) {
        m_BasisU.Evaluate( fU, 0, iMinU, iMaxU );
        m_BasisU.Evaluate( fU, 1, iMinU, iMaxU );
    } else
        m_BasisU.Evaluate( fU, 0, iMinU, iMaxU );

    UInt iV, iMinV, iMaxV;
    if ( outPdV2 != NULL ) {
        m_BasisV.Evaluate( fV, 0, iMinV, iMaxV );
        m_BasisV.Evaluate( fV, 1, iMinV, iMaxV );
        m_BasisV.Evaluate( fV, 2, iMinV, iMaxV );
    } else if ( outPdV != NULL || outPdUdV != NULL ) {
        m_BasisV.Evaluate( fV, 0, iMinV, iMaxV );
        m_BasisV.Evaluate( fV, 1, iMinV, iMaxV );
    } else
        m_BasisV.Evaluate( fV, 0, iMinV, iMaxV );

    UInt iIndex;
    Real fTmp;

    // P
    TVertex3<Real> vP = TVector3<Real>::Null;
    Real fW = (Real)0, fInvW;
    for( iV = iMinV; iV <= iMaxV; ++iV ) {
        for( iU = iMinU; iU <= iMaxU; ++iU ) {
            iIndex = (iV * m_iControlPointsPitch) + iU;
            fTmp = m_BasisU.N(iU) * m_BasisV.N(iV) * m_arrWeights[iIndex];
            vP += ( m_arrControlPoints[iIndex].ToVector() * fTmp );
            fW += fTmp;
        }
    }
    fInvW = MathRealFn->Invert( fW );
    vP *= fInvW;
    if ( outP != NULL )
        *outP = vP;
    if ( outPdU == NULL && outPdV == NULL && outPdU2 == NULL && outPdV2 == NULL && outPdUdV == NULL )
        return;

    // PdU
    TVector3<Real> vPdU = TVector3<Real>::Null;
    Real fWdU = (Real)0;
    if ( outPdU != NULL || outPdU2 != NULL || outPdUdV != NULL ) {
        for( iV = iMinV; iV <= iMaxV; ++iV ) {
            for( iU = iMinU; iU <= iMaxU; ++iU ) {
                iIndex = (iV * m_iControlPointsPitch) + iU;
                fTmp = m_BasisU.NdT(iU) * m_BasisV.N(iV) * m_arrWeights[iIndex];
                vPdU += ( m_arrControlPoints[iIndex].ToVector() * fTmp );
                fWdU += fTmp;
            }
        }
        vPdU = ( vPdU - (vP * fWdU) ) * fInvW;
        if ( outPdU != NULL )
            *outPdU = vPdU;
    }

    // PdV
    TVector3<Real> vPdV = TVector3<Real>::Null;
    Real fWdV = (Real)0;
    if ( outPdV != NULL || outPdV2 != NULL || outPdUdV != NULL ) {
        for( iV = iMinV; iV <= iMaxV; ++iV ) {
            for( iU = iMinU; iU <= iMaxU; ++iU ) {
                iIndex = (iV * m_iControlPointsPitch) + iU;
                fTmp = m_BasisU.N(iU) * m_BasisV.NdT(iV) * m_arrWeights[iIndex];
                vPdV += ( m_arrControlPoints[iIndex].ToVector() * fTmp );
                fWdV += fTmp;
            }
        }
        vPdV = ( vPdV - (vP * fWdV) ) * fInvW;
        if ( outPdV != NULL )
            *outPdV = vPdV;
    }

    // PdU2
    if ( outPdU2 != NULL ) {
        TVector3<Real> vPdU2 = TVector3<Real>::Null;
        Real fWdU2 = (Real)0;
        for( iV = iMinV; iV <= iMaxV; ++iV ) {
            for( iU = iMinU; iU <= iMaxU; ++iU ) {
                iIndex = (iV * m_iControlPointsPitch) + iU;
                fTmp = m_BasisU.NdT2(iU) * m_BasisV.N(iV) * m_arrWeights[iIndex];
                vPdU2 += ( m_arrControlPoints[iIndex].ToVector() * fTmp );
                fWdU2 += fTmp;
            }
        }
        *outPdU2 = ( vPdU2 - (vPdU * (fWdU * (Real)2)) - (vP * fWdU2) ) * fInvW;
    }

    // PdV2
    if ( outPdV2 != NULL ) {
        TVector3<Real> vPdV2 = TVector3<Real>::Null;
        Real fWdV2 = (Real)0;
        for( iV = iMinV; iV <= iMaxV; ++iV ) {
            for( iU = iMinU; iU <= iMaxU; ++iU ) {
                iIndex = (iV * m_iControlPointsPitch) + iU;
                fTmp = m_BasisU.N(iU) * m_BasisV.NdT2(iV) * m_arrWeights[iIndex];
                vPdV2 += ( m_arrControlPoints[iIndex].ToVector() * fTmp );
                fWdV2 += fTmp;
            }
        }
        *outPdV2 = ( vPdV2 - (vPdV * (fWdV * (Real)2)) - (vP * fWdV2) ) * fInvW;
    }

    // PdUdV
    if ( outPdUdV != NULL ) {
        TVector3<Real> vPdUdV = TVector3<Real>::Null;
        Real fWdUdV = (Real)0;
        for( iV = iMinV; iV <= iMaxV; ++iV ) {
            for( iU = iMinU; iU <= iMaxU; ++iU ) {
                iIndex = (iV * m_iControlPointsPitch) + iU;
                fTmp = m_BasisU.NdT(iU) * m_BasisV.NdT(iV) * m_arrWeights[iIndex];
                vPdUdV += ( m_arrControlPoints[iIndex].ToVector() * fTmp );
                fWdUdV += fTmp;
            }
        }
        *outPdUdV = ( vPdUdV - (vPdV * fWdU) - (vPdU * fWdV) - (vP * fWdUdV) ) * fInvW;
    }
}

/////////////////////////////////////////////////////////////////////////////////

template<typename Real>
Void TNURBSSurface<Real>::_CreateControlPoints( const TVertex3<Real> * arrControlPoints, const Real * arrWeights )
{
    UInt iNewCountU = m_iControlPointCountU + m_iReplicateCountU;
    UInt iNewCountV = m_iControlPointCountV + m_iReplicateCountV;
    m_iControlPointsPitch = iNewCountU;

    UInt iU, iV, iIndex, iOldU, iOldV, iOldIndex;
    m_arrControlPoints = New TVertex3<Real>[ iNewCountU * iNewCountV ];
    m_arrWeights = New Real[ iNewCountU * iNewCountV ];
    for( iV = 0; iV < iNewCountV; ++iV ) {
        iOldV = ( iV % m_iControlPointCountV );
        for ( iU = 0; iU < iNewCountU; ++iU ) {
            iOldU = iU % m_iControlPointCountU;
            iOldIndex = (iOldV * m_iControlPointCountU) + iOldU;
            iIndex = (iV * iNewCountU) + iU;
            m_arrControlPoints[iIndex] = arrControlPoints[iOldIndex];
            m_arrWeights[iIndex] = arrWeights[iOldIndex];
        }
    }
}
