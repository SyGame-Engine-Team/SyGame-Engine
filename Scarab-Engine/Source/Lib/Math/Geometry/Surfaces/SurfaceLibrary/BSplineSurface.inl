/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Surfaces/SurfaceLibrary/BSplineSurface.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Parametric Surfaces : BSpline surfaces
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TBSplineSurface implementation
template<typename Real>
TBSplineSurface<Real>::TBSplineSurface( const TVertex3<Real> * arrControlPoints, UInt iControlPointCountU, UInt iControlPointCountV,
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
    _CreateControlPoints( arrControlPoints );
}
template<typename Real>
TBSplineSurface<Real>::TBSplineSurface( const TVertex3<Real> * arrControlPoints, UInt iControlPointCountU, UInt iControlPointCountV,
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
    _CreateControlPoints( arrControlPoints );
}
template<typename Real>
TBSplineSurface<Real>::TBSplineSurface( const TVertex3<Real> * arrControlPoints, UInt iControlPointCountU, UInt iControlPointCountV,
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
    _CreateControlPoints( arrControlPoints );
}
template<typename Real>
TBSplineSurface<Real>::TBSplineSurface( const TVertex3<Real> * arrControlPoints, UInt iControlPointCountU, UInt iControlPointCountV,
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
    _CreateControlPoints( arrControlPoints );
}
template<typename Real>
TBSplineSurface<Real>::~TBSplineSurface()
{
    DeleteA( m_arrControlPoints );
}

template<typename Real>
inline UInt TBSplineSurface<Real>::GetDegreeU() const {
    return m_BasisU.GetDegree();
}
template<typename Real>
inline UInt TBSplineSurface<Real>::GetDegreeV() const {
    return m_BasisV.GetDegree();
}

template<typename Real>
inline Bool TBSplineSurface<Real>::IsUniformU() const {
    return m_BasisU.IsUniform();
}
template<typename Real>
inline Bool TBSplineSurface<Real>::IsUniformV() const {
    return m_BasisV.IsUniform();
}
template<typename Real>
inline Bool TBSplineSurface<Real>::IsPeriodicU() const {
    return m_BasisU.IsPeriodic();
}
template<typename Real>
inline Bool TBSplineSurface<Real>::IsPeriodicV() const {
    return m_BasisV.IsPeriodic();
}
template<typename Real>
inline Bool TBSplineSurface<Real>::IsLoopU() const {
    return m_bLoopU;
}
template<typename Real>
inline Bool TBSplineSurface<Real>::IsLoopV() const {
    return m_bLoopV;
}

template<typename Real>
inline UInt TBSplineSurface<Real>::GetControlPointCountU() const {
    return m_iControlPointCountU;
}
template<typename Real>
inline UInt TBSplineSurface<Real>::GetControlPointCountV() const {
    return m_iControlPointCountV;
}
template<typename Real>
inline const TVertex3<Real> & TBSplineSurface<Real>::GetControlPoint( UInt iU, UInt iV ) const {
    Assert( iU < m_iControlPointCountU );
    Assert( iV < m_iControlPointCountV );
    return m_arrControlPoints[ ( iV * m_iControlPointsPitch ) + iU ];
}
template<typename Real>
Void TBSplineSurface<Real>::SetControlPoint( UInt iU, UInt iV, const TVertex3<Real> & vPoint )
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
inline UInt TBSplineSurface<Real>::GetKnotCountU() const {
    return m_BasisU.GetKnotCount();
}
template<typename Real>
inline UInt TBSplineSurface<Real>::GetKnotCountV() const {
    return m_BasisV.GetKnotCount();
}
template<typename Real>
inline Real TBSplineSurface<Real>::GetKnotU( UInt iKnot ) const {
    return m_BasisU.GetKnot( iKnot );
}
template<typename Real>
inline Real TBSplineSurface<Real>::GetKnotV( UInt iKnot ) const {
    return m_BasisV.GetKnot( iKnot );
}
template<typename Real>
inline Void TBSplineSurface<Real>::SetKnotU( UInt iKnot, Real fKnotValue ) {
    m_BasisU.SetKnot( iKnot, fKnotValue );
}
template<typename Real>
inline Void TBSplineSurface<Real>::SetKnotV( UInt iKnot, Real fKnotValue ) {
    m_BasisV.SetKnot( iKnot, fKnotValue );
}

template<typename Real>
TVertex3<Real> TBSplineSurface<Real>::P( Real fU, Real fV ) const
{
    UInt iU, iMinU, iMaxU;
    m_BasisU.Evaluate( fU, 0, iMinU, iMaxU );

    UInt iV, iMinV, iMaxV;
    m_BasisV.Evaluate( fV, 0, iMinV, iMaxV );

    Real fTmp;
    UInt iIndex;
    TVertex3<Real> vRes = TVertex3<Real>::Null;
    for( iV = iMinV; iV <= iMaxV; ++iV ) {
        for( iU = iMinU; iU <= iMaxU; ++iU ) {
            iIndex = (iV * m_iControlPointsPitch) + iU;
            fTmp = m_BasisU.N(iU) * m_BasisV.N(iV);
            vRes += ( m_arrControlPoints[iIndex].ToVector() * fTmp );
        }
    }

    return vRes;
}
template<typename Real>
TVector3<Real> TBSplineSurface<Real>::PdU( Real fU, Real fV ) const
{
    UInt iU, iMinU, iMaxU;
    m_BasisU.Evaluate( fU, 0, iMinU, iMaxU );
    m_BasisU.Evaluate( fU, 1, iMinU, iMaxU );

    UInt iV, iMinV, iMaxV;
    m_BasisV.Evaluate( fV, 0, iMinV, iMaxV );

    Real fTmp;
    UInt iIndex;
    TVector3<Real> vRes = TVector3<Real>::Null;
    for( iV = iMinV; iV <= iMaxV; ++iV ) {
        for( iU = iMinU; iU <= iMaxU; ++iU ) {
            iIndex = (iV * m_iControlPointsPitch) + iU;
            fTmp = m_BasisU.NdT(iU) * m_BasisV.N(iV);
            vRes += ( m_arrControlPoints[iIndex].ToVector() * fTmp );
        }
    }

    return vRes;
}
template<typename Real>
TVector3<Real> TBSplineSurface<Real>::PdV( Real fU, Real fV ) const
{
    UInt iU, iMinU, iMaxU;
    m_BasisU.Evaluate( fU, 0, iMinU, iMaxU );

    UInt iV, iMinV, iMaxV;
    m_BasisV.Evaluate( fV, 0, iMinV, iMaxV );
    m_BasisV.Evaluate( fV, 1, iMinV, iMaxV );

    Real fTmp;
    UInt iIndex;
    TVector3<Real> vRes = TVector3<Real>::Null;
    for( iV = iMinV; iV <= iMaxV; ++iV ) {
        for( iU = iMinU; iU <= iMaxU; ++iU ) {
            iIndex = (iV * m_iControlPointsPitch) + iU;
            fTmp = m_BasisU.N(iU) * m_BasisV.NdT(iV);
            vRes += ( m_arrControlPoints[iIndex].ToVector() * fTmp );
        }
    }

    return vRes;
}
template<typename Real>
TVector3<Real> TBSplineSurface<Real>::PdU2( Real fU, Real fV ) const
{
    UInt iU, iMinU, iMaxU;
    m_BasisU.Evaluate( fU, 0, iMinU, iMaxU );
    m_BasisU.Evaluate( fU, 1, iMinU, iMaxU );
    m_BasisU.Evaluate( fU, 2, iMinU, iMaxU );

    UInt iV, iMinV, iMaxV;
    m_BasisV.Evaluate( fV, 0, iMinV, iMaxV );

    Real fTmp;
    UInt iIndex;
    TVector3<Real> vRes = TVector3<Real>::Null;
    for( iV = iMinV; iV <= iMaxV; ++iV ) {
        for( iU = iMinU; iU <= iMaxU; ++iU ) {
            iIndex = (iV * m_iControlPointsPitch) + iU;
            fTmp = m_BasisU.NdT2(iU) * m_BasisV.N(iV);
            vRes += ( m_arrControlPoints[iIndex].ToVector() * fTmp );
        }
    }

    return vRes;
}
template<typename Real>
TVector3<Real> TBSplineSurface<Real>::PdV2( Real fU, Real fV ) const
{
    UInt iU, iMinU, iMaxU;
    m_BasisU.Evaluate( fU, 0, iMinU, iMaxU );

    UInt iV, iMinV, iMaxV;
    m_BasisV.Evaluate( fV, 0, iMinV, iMaxV );
    m_BasisV.Evaluate( fV, 1, iMinV, iMaxV );
    m_BasisV.Evaluate( fV, 2, iMinV, iMaxV );

    Real fTmp;
    UInt iIndex;
    TVector3<Real> vRes = TVector3<Real>::Null;
    for( iV = iMinV; iV <= iMaxV; ++iV ) {
        for( iU = iMinU; iU <= iMaxU; ++iU ) {
            iIndex = (iV * m_iControlPointsPitch) + iU;
            fTmp = m_BasisU.N(iU) * m_BasisV.NdT2(iV);
            vRes += ( m_arrControlPoints[iIndex].ToVector() * fTmp );
        }
    }

    return vRes;
}
template<typename Real>
TVector3<Real> TBSplineSurface<Real>::PdUdV( Real fU, Real fV ) const
{
    UInt iU, iMinU, iMaxU;
    m_BasisU.Evaluate( fU, 0, iMinU, iMaxU );
    m_BasisU.Evaluate( fU, 1, iMinU, iMaxU );

    UInt iV, iMinV, iMaxV;
    m_BasisV.Evaluate( fV, 0, iMinV, iMaxV );
    m_BasisV.Evaluate( fV, 1, iMinV, iMaxV );

    Real fTmp;
    UInt iIndex;
    TVector3<Real> vRes = TVector3<Real>::Null;
    for( iV = iMinV; iV <= iMaxV; ++iV ) {
        for( iU = iMinU; iU <= iMaxU; ++iU ) {
            iIndex = (iV * m_iControlPointsPitch) + iU;
            fTmp = m_BasisU.NdT(iU) * m_BasisV.NdT(iV);
            vRes += ( m_arrControlPoints[iIndex].ToVector() * fTmp );
        }
    }

    return vRes;
}

template<typename Real>
Void TBSplineSurface<Real>::Evaluate( Real fU, Real fV, TVertex3<Real> * outP, TVector3<Real> * outPdU, TVector3<Real> * outPdV,
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
    if ( outP != NULL ) {
        *outP = TVertex3<Real>::Null;
        for( iV = iMinV; iV <= iMaxV; ++iV ) {
            for( iU = iMinU; iU <= iMaxU; ++iU ) {
                iIndex = (iV * m_iControlPointsPitch) + iU;
                fTmp = m_BasisU.N(iU) * m_BasisV.N(iV);
                *outP += ( m_arrControlPoints[iIndex].ToVector() * fTmp );
            }
        }
    }
    if ( outPdU != NULL ) {
        *outPdU = TVector3<Real>::Null;
        for( iV = iMinV; iV <= iMaxV; ++iV ) {
            for( iU = iMinU; iU <= iMaxU; ++iU ) {
                iIndex = (iV * m_iControlPointsPitch) + iU;
                fTmp = m_BasisU.NdT(iU) * m_BasisV.N(iV);
                *outPdU += ( m_arrControlPoints[iIndex].ToVector() * fTmp );
            }
        }
    }
    if ( outPdV != NULL ) {
        *outPdV = TVector3<Real>::Null;
        for( iV = iMinV; iV <= iMaxV; ++iV ) {
            for( iU = iMinU; iU <= iMaxU; ++iU ) {
                iIndex = (iV * m_iControlPointsPitch) + iU;
                fTmp = m_BasisU.N(iU) * m_BasisV.NdT(iV);
                *outPdV += ( m_arrControlPoints[iIndex].ToVector() * fTmp );
            }
        }
    }
    if ( outPdU2 != NULL ) {
        *outPdU2 = TVector3<Real>::Null;
        for( iV = iMinV; iV <= iMaxV; ++iV ) {
            for( iU = iMinU; iU <= iMaxU; ++iU ) {
                iIndex = (iV * m_iControlPointsPitch) + iU;
                fTmp = m_BasisU.NdT2(iU) * m_BasisV.N(iV);
                *outPdU2 += ( m_arrControlPoints[iIndex].ToVector() * fTmp );
            }
        }
    }
    if ( outPdV2 != NULL ) {
        *outPdV2 = TVector3<Real>::Null;
        for( iV = iMinV; iV <= iMaxV; ++iV ) {
            for( iU = iMinU; iU <= iMaxU; ++iU ) {
                iIndex = (iV * m_iControlPointsPitch) + iU;
                fTmp = m_BasisU.N(iU) * m_BasisV.NdT2(iV);
                *outPdV2 += ( m_arrControlPoints[iIndex].ToVector() * fTmp );
            }
        }
    }
    if ( outPdUdV != NULL ) {
        *outPdUdV = TVector3<Real>::Null;
        for( iV = iMinV; iV <= iMaxV; ++iV ) {
            for( iU = iMinU; iU <= iMaxU; ++iU ) {
                iIndex = (iV * m_iControlPointsPitch) + iU;
                fTmp = m_BasisU.NdT(iU) * m_BasisV.NdT(iV);
                *outPdUdV += ( m_arrControlPoints[iIndex].ToVector() * fTmp );
            }
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////

template<typename Real>
Void TBSplineSurface<Real>::_CreateControlPoints( const TVertex3<Real> * arrControlPoints )
{
    UInt iNewCountU = m_iControlPointCountU + m_iReplicateCountU;
    UInt iNewCountV = m_iControlPointCountV + m_iReplicateCountV;
    m_iControlPointsPitch = iNewCountU;

    UInt iU, iV, iIndex, iOldU, iOldV, iOldIndex;
    m_arrControlPoints = New TVertex3<Real>[ iNewCountU * iNewCountV ];
    for( iV = 0; iV < iNewCountV; ++iV ) {
        iOldV = ( iV % m_iControlPointCountV );
        for ( iU = 0; iU < iNewCountU; ++iU ) {
            iOldU = iU % m_iControlPointCountU;
            iOldIndex = (iOldV * m_iControlPointCountU) + iOldU;
            iIndex = (iV * iNewCountU) + iU;
            m_arrControlPoints[iIndex] = arrControlPoints[iOldIndex];
        }
    }
}
