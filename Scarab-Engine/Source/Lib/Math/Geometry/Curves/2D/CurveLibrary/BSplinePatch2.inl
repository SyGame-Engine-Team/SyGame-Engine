/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Curves/2D/CurveLibrary/BSplinePatch2.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Curves 2D : BSpline curves
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TBSplinePatch2 implementation
template<typename Real>
TBSplinePatch2<Real>::TBSplinePatch2( const TVertex2<Real> * arrControlPoints, UInt iControlPointCount,
                                      UInt iDegree, Bool bLoop, Bool bPeriodic ):
    TCurvePatch2<Real>( (Real)0, (Real)1 ), m_Basis()
{
    Assert( iControlPointCount >= 2 );
    Assert( iDegree >= 1 && iDegree < iControlPointCount );

    m_iControlPointCount = iControlPointCount;
    m_iReplicateCount = ( bLoop ? (bPeriodic ? iDegree : 1) : 0 );

    m_bLoop = bLoop;

    m_Basis.InitUniform( m_iControlPointCount + m_iReplicateCount, iDegree, bPeriodic );
    _CreateControlPoints( arrControlPoints );
}
template<typename Real>
TBSplinePatch2<Real>::TBSplinePatch2( const TVertex2<Real> * arrControlPoints, UInt iControlPointCount,
                                      UInt iDegree, Bool bLoop, const Real * arrKnots ):
    TCurvePatch2<Real>( (Real)0, (Real)1 ), m_Basis()
{
    Assert( iControlPointCount >= 2 );
    Assert( iDegree >= 1 && iDegree < iControlPointCount );

    m_iControlPointCount = iControlPointCount;
    m_iReplicateCount = ( bLoop ? 1 : 0 );

    m_bLoop = bLoop;

    m_Basis.InitNonUniform( m_iControlPointCount + m_iReplicateCount, iDegree, arrKnots );
    _CreateControlPoints( arrControlPoints );
}
template<typename Real>
TBSplinePatch2<Real>::~TBSplinePatch2()
{
    DeleteA( m_arrControlPoints );
}

template<typename Real>
inline UInt TBSplinePatch2<Real>::GetDegree() const {
    return m_Basis.GetDegree();
}

template<typename Real>
inline Bool TBSplinePatch2<Real>::IsUniform() const {
    return m_Basis.IsUniform();
}
template<typename Real>
inline Bool TBSplinePatch2<Real>::IsPeriodic() const {
    return m_Basis.IsPeriodic();
}
template<typename Real>
inline Bool TBSplinePatch2<Real>::IsLoop() const {
    return m_bLoop;
}

template<typename Real>
inline UInt TBSplinePatch2<Real>::GetControlPointCount() const {
    return m_iControlPointCount;
}
template<typename Real>
inline const TVertex2<Real> & TBSplinePatch2<Real>::GetControlPoint( UInt iControlPoint ) const {
    Assert( iControlPoint < m_iControlPointCount );
    return m_arrControlPoints[iControlPoint];
}
template<typename Real>
inline Void TBSplinePatch2<Real>::SetControlPoint( UInt iControlPoint, const TVertex2<Real> & vPoint ) {
    Assert( iControlPoint < m_iControlPointCount );
    m_arrControlPoints[iControlPoint] = vPoint;
    if ( iControlPoint < m_iReplicateCount )
        m_arrControlPoints[m_iControlPointCount + iControlPoint] = vPoint;
}

template<typename Real>
inline UInt TBSplinePatch2<Real>::GetKnotCount() const {
    return m_Basis.GetKnotCount();
}
template<typename Real>
inline Real TBSplinePatch2<Real>::GetKnot( UInt iKnot ) const {
    return m_Basis.GetKnot( iKnot );
}
template<typename Real>
inline Void TBSplinePatch2<Real>::SetKnot( UInt iKnot, Real fKnotValue ) {
    m_Basis.SetKnot( iKnot, fKnotValue );
}

template<typename Real>
TVertex2<Real> TBSplinePatch2<Real>::P( Real fT ) const
{
    UInt i, iMin, iMax;
    m_Basis.Evaluate( fT, 0, iMin, iMax );

    TVertex2<Real> vRes = TVertex2<Real>::Null;
    for( i = iMin; i <= iMax; ++i )
        vRes += ( m_arrControlPoints[i].ToVector() * m_Basis.N(i) );

    return vRes;
}
template<typename Real>
TVector2<Real> TBSplinePatch2<Real>::PdT( Real fT ) const
{
    UInt i, iMin, iMax;
    m_Basis.Evaluate( fT, 0, iMin, iMax );
    m_Basis.Evaluate( fT, 1, iMin, iMax );

    TVector2<Real> vRes = TVector2<Real>::Null;
    for( i = iMin; i <= iMax; ++i )
        vRes += ( m_arrControlPoints[i].ToVector() * m_Basis.NdT(i) );

    return vRes;
}
template<typename Real>
TVector2<Real> TBSplinePatch2<Real>::PdT2( Real fT ) const
{
    UInt i, iMin, iMax;
    m_Basis.Evaluate( fT, 0, iMin, iMax );
    m_Basis.Evaluate( fT, 1, iMin, iMax );
    m_Basis.Evaluate( fT, 2, iMin, iMax );

    TVector2<Real> vRes = TVector2<Real>::Null;
    for( i = iMin; i <= iMax; ++i )
        vRes += ( m_arrControlPoints[i].ToVector() * m_Basis.NdT2(i) );

    return vRes;
}
template<typename Real>
TVector2<Real> TBSplinePatch2<Real>::PdT3( Real fT ) const
{
    UInt i, iMin, iMax;
    m_Basis.Evaluate( fT, 0, iMin, iMax );
    m_Basis.Evaluate( fT, 1, iMin, iMax );
    m_Basis.Evaluate( fT, 2, iMin, iMax );
    m_Basis.Evaluate( fT, 3, iMin, iMax );

    TVector2<Real> vRes = TVector2<Real>::Null;
    for( i = iMin; i <= iMax; ++i )
        vRes += ( m_arrControlPoints[i].ToVector() * m_Basis.NdT3(i) );

    return vRes;
}

template<typename Real>
Void TBSplinePatch2<Real>::Evaluate( Real fT, TVertex2<Real> * outP, TVector2<Real> * outPdT,
                                              TVector2<Real> * outPdT2, TVector2<Real> * outPdT3 ) const
{
    UInt i, iMin, iMax;
    if ( outPdT3 != NULL ) {
        m_Basis.Evaluate( fT, 0, iMin, iMax );
        m_Basis.Evaluate( fT, 1, iMin, iMax );
        m_Basis.Evaluate( fT, 2, iMin, iMax );
        m_Basis.Evaluate( fT, 3, iMin, iMax );
    } else if ( outPdT2 != NULL ) {
        m_Basis.Evaluate( fT, 0, iMin, iMax );
        m_Basis.Evaluate( fT, 1, iMin, iMax );
        m_Basis.Evaluate( fT, 2, iMin, iMax );
    } else if ( outPdT != NULL ) {
        m_Basis.Evaluate( fT, 0, iMin, iMax );
        m_Basis.Evaluate( fT, 1, iMin, iMax );
    } else
        m_Basis.Evaluate( fT, 0, iMin, iMax );

    if ( outP != NULL ) {
        *outP = TVertex2<Real>::Null;
        for( i = iMin; i <= iMax; ++i )
            *outP += ( m_arrControlPoints[i].ToVector() * m_Basis.N(i) );
    }
    if ( outPdT != NULL ) {
        *outPdT = TVector2<Real>::Null;
        for( i = iMin; i <= iMax; ++i )
            *outPdT += ( m_arrControlPoints[i].ToVector() * m_Basis.NdT(i) );
    }
    if ( outPdT2 != NULL ) {
        *outPdT2 = TVector2<Real>::Null;
        for( i = iMin; i <= iMax; ++i )
            *outPdT2 += ( m_arrControlPoints[i].ToVector() * m_Basis.NdT2(i) );
    }
    if ( outPdT3 != NULL ) {
        *outPdT3 = TVector2<Real>::Null;
        for( i = iMin; i <= iMax; ++i )
            *outPdT3 += ( m_arrControlPoints[i].ToVector() * m_Basis.NdT3(i) );
    }
}

/////////////////////////////////////////////////////////////////////////////////

template<typename Real>
Void TBSplinePatch2<Real>::_CreateControlPoints( const TVertex2<Real> * arrControlPoints )
{
    UInt i;
    m_arrControlPoints = New TVertex2<Real>[m_iControlPointCount + m_iReplicateCount];
    for( i = 0; i < m_iControlPointCount; ++i )
        m_arrControlPoints[i] = arrControlPoints[i];
    for ( i = 0; i < m_iReplicateCount; ++i )
        m_arrControlPoints[m_iControlPointCount + i] = arrControlPoints[i];
}

