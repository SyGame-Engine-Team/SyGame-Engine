/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Curves/3D/CurveLibrary/NURBSPatch3.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Curves 3D : (Non-)Uniform Rational BSpline curves
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TNURBSPatch3 implementation
template<typename Real>
TNURBSPatch3<Real>::TNURBSPatch3( const TVertex3<Real> * arrControlPoints, const Real * arrWeights, UInt iControlPointCount,
                                  UInt iDegree, Bool bLoop, Bool bPeriodic ):
    TCurvePatch3<Real>( (Real)0, (Real)1 ), m_Basis()
{
    Assert( iControlPointCount >= 2 );
    Assert( iDegree >= 1 && iDegree < iControlPointCount );

    m_iControlPointCount = iControlPointCount;
    m_iReplicateCount = ( bLoop ? (bPeriodic ? iDegree : 1) : 0 );

    m_bLoop = bLoop;

    m_Basis.InitUniform( m_iControlPointCount + m_iReplicateCount, iDegree, bPeriodic );
    _CreateControlPoints( arrControlPoints, arrWeights );
}
template<typename Real>
TNURBSPatch3<Real>::TNURBSPatch3( const TVertex3<Real> * arrControlPoints, const Real * arrWeights, UInt iControlPointCount,
                                  UInt iDegree, Bool bLoop, const Real * arrKnots ):
    TCurvePatch3<Real>( (Real)0, (Real)1 ), m_Basis()
{
    Assert( iControlPointCount >= 2 );
    Assert( iDegree >= 1 && iDegree < iControlPointCount );

    m_iControlPointCount = iControlPointCount;
    m_iReplicateCount = ( bLoop ? 1 : 0 );

    m_bLoop = bLoop;

    m_Basis.InitNonUniform( m_iControlPointCount + m_iReplicateCount, iDegree, arrKnots );
    _CreateControlPoints( arrControlPoints, arrWeights );
}
template<typename Real>
TNURBSPatch3<Real>::~TNURBSPatch3()
{
    DeleteA( m_arrControlPoints );
    DeleteA( m_arrWeights );
}

template<typename Real>
inline UInt TNURBSPatch3<Real>::GetDegree() const {
    return m_Basis.GetDegree();
}

template<typename Real>
inline Bool TNURBSPatch3<Real>::IsUniform() const {
    return m_Basis.IsUniform();
}
template<typename Real>
inline Bool TNURBSPatch3<Real>::IsPeriodic() const {
    return m_Basis.IsPeriodic();
}
template<typename Real>
inline Bool TNURBSPatch3<Real>::IsLoop() const {
    return m_bLoop;
}

template<typename Real>
inline UInt TNURBSPatch3<Real>::GetControlPointCount() const {
    return m_iControlPointCount;
}
template<typename Real>
inline const TVertex3<Real> & TNURBSPatch3<Real>::GetControlPoint( UInt iControlPoint ) const {
    Assert( iControlPoint < m_iControlPointCount );
    return m_arrControlPoints[iControlPoint];
}
template<typename Real>
inline Void TNURBSPatch3<Real>::SetControlPoint( UInt iControlPoint, const TVertex3<Real> & vPoint ) {
    Assert( iControlPoint < m_iControlPointCount );
    m_arrControlPoints[iControlPoint] = vPoint;
    if ( iControlPoint < m_iReplicateCount )
        m_arrControlPoints[m_iControlPointCount + iControlPoint] = vPoint;
}

template<typename Real>
inline Real TNURBSPatch3<Real>::GetWeight( UInt iControlPoint ) const {
    Assert( iControlPoint < m_iControlPointCount );
    return m_arrWeights[iControlPoint];
}
template<typename Real>
inline Void TNURBSPatch3<Real>::SetWeight( UInt iControlPoint, Real fWeightValue ) {
    Assert( iControlPoint < m_iControlPointCount );
    m_arrWeights[iControlPoint] = fWeightValue;
    if ( iControlPoint < m_iReplicateCount )
        m_arrWeights[m_iControlPointCount + iControlPoint] = fWeightValue;
}

template<typename Real>
inline UInt TNURBSPatch3<Real>::GetKnotCount() const {
    return m_Basis.GetKnotCount();
}
template<typename Real>
inline Real TNURBSPatch3<Real>::GetKnot( UInt iKnot ) const {
    return m_Basis.GetKnot( iKnot );
}
template<typename Real>
inline Void TNURBSPatch3<Real>::SetKnot( UInt iKnot, Real fKnotValue ) {
    m_Basis.SetKnot( iKnot, fKnotValue );
}

template<typename Real>
TVertex3<Real> TNURBSPatch3<Real>::P( Real fT ) const
{
    UInt i, iMin, iMax;
    m_Basis.Evaluate( fT, 0, iMin, iMax );

    Real fW = (Real)0, fInvW, fTmp;
    TVertex3<Real> vRes = TVertex3<Real>::Null;
    for( i = iMin; i <= iMax; ++i ) {
        fTmp = ( m_arrWeights[i] * m_Basis.N(i) );
        vRes += ( m_arrControlPoints[i].ToVector() * fTmp );
        fW += fTmp;
    }
    fInvW = MathRealFn->Invert( fW );
    vRes *= fInvW;

    return vRes;
}
template<typename Real>
TVector3<Real> TNURBSPatch3<Real>::PdT( Real fT ) const
{
    UInt i, iMin, iMax;
    m_Basis.Evaluate( fT, 0, iMin, iMax );
    m_Basis.Evaluate( fT, 1, iMin, iMax );

    Real fW = (Real)0, fInvW, fTmp;
    TVector3<Real> vP = TVector3<Real>::Null;
    for( i = iMin; i <= iMax; ++i ) {
        fTmp = ( m_arrWeights[i] * m_Basis.N(i) );
        vP += ( m_arrControlPoints[i].ToVector() * fTmp );
        fW += fTmp;
    }
    fInvW = MathRealFn->Invert( fW );
    vP *= fInvW;

    Real fWdT = (Real)0;
    TVector3<Real> vRes = TVector3<Real>::Null;
    for( i = iMin; i <= iMax; ++i ) {
        fTmp = ( m_arrWeights[i] * m_Basis.NdT(i) );
        vRes += ( m_arrControlPoints[i].ToVector() * fTmp );
        fWdT += fTmp;
    }
    vRes = ( vRes - (vP * fWdT) ) * fInvW;

    return vRes;
}
template<typename Real>
TVector3<Real> TNURBSPatch3<Real>::PdT2( Real fT ) const
{
    UInt i, iMin, iMax;
    m_Basis.Evaluate( fT, 0, iMin, iMax );
    m_Basis.Evaluate( fT, 1, iMin, iMax );
    m_Basis.Evaluate( fT, 2, iMin, iMax );

    Real fW = (Real)0, fInvW, fTmp;
    TVector3<Real> vP = TVector3<Real>::Null;
    for( i = iMin; i <= iMax; ++i ) {
        fTmp = ( m_arrWeights[i] * m_Basis.N(i) );
        vP += ( m_arrControlPoints[i].ToVector() * fTmp );
        fW += fTmp;
    }
    fInvW = MathRealFn->Invert( fW );
    vP *= fInvW;

    Real fWdT = (Real)0;
    TVector3<Real> vPdT = TVector3<Real>::Null;
    for( i = iMin; i <= iMax; ++i ) {
        fTmp = ( m_arrWeights[i] * m_Basis.NdT(i) );
        vPdT += ( m_arrControlPoints[i].ToVector() * fTmp );
        fWdT += fTmp;
    }
    vPdT = ( vPdT - (vP * fWdT) ) * fInvW;

    Real fWdT2 = (Real)0;
    TVector3<Real> vRes = TVector3<Real>::Null;
    for( i = iMin; i <= iMax; ++i ) {
        fTmp = ( m_arrWeights[i] * m_Basis.NdT2(i) );
        vRes += ( m_arrControlPoints[i].ToVector() * fTmp );
        fWdT2 += fTmp;
    }
    vRes = ( vRes - (vPdT * (fWdT * (Real)2)) - (vP * fWdT2) ) * fInvW;

    return vRes;
}
template<typename Real>
TVector3<Real> TNURBSPatch3<Real>::PdT3( Real fT ) const
{
    UInt i, iMin, iMax;
    m_Basis.Evaluate( fT, 0, iMin, iMax );
    m_Basis.Evaluate( fT, 1, iMin, iMax );
    m_Basis.Evaluate( fT, 2, iMin, iMax );
    m_Basis.Evaluate( fT, 3, iMin, iMax );

    Real fW = (Real)0, fInvW, fTmp;
    TVector3<Real> vP = TVector3<Real>::Null;
    for( i = iMin; i <= iMax; ++i ) {
        fTmp = ( m_arrWeights[i] * m_Basis.N(i) );
        vP += ( m_arrControlPoints[i].ToVector() * fTmp );
        fW += fTmp;
    }
    fInvW = MathRealFn->Invert( fW );
    vP *= fInvW;

    Real fWdT = (Real)0;
    TVector3<Real> vPdT = TVector3<Real>::Null;
    for( i = iMin; i <= iMax; ++i ) {
        fTmp = ( m_arrWeights[i] * m_Basis.NdT(i) );
        vPdT += ( m_arrControlPoints[i].ToVector() * fTmp );
        fWdT += fTmp;
    }
    vPdT = ( vPdT - (vP * fWdT) ) * fInvW;

    Real fWdT2 = (Real)0;
    TVector3<Real> vPdT2 = TVector3<Real>::Null;
    for( i = iMin; i <= iMax; ++i ) {
        fTmp = ( m_arrWeights[i] * m_Basis.NdT2(i) );
        vPdT2 += ( m_arrControlPoints[i].ToVector() * fTmp );
        fWdT2 += fTmp;
    }
    vPdT2 = ( vPdT2 - (vPdT * (fWdT * (Real)2)) - (vP * fWdT2) ) * fInvW;

    Real fWdT3 = (Real)0;
    TVector3<Real> vRes = TVector3<Real>::Null;
    for( i = iMin; i <= iMax; ++i ) {
        fTmp = ( m_arrWeights[i] * m_Basis.NdT3(i) );
        vRes += ( m_arrControlPoints[i].ToVector() * fTmp );
        fWdT3 += fTmp;
    }
    vRes = ( vRes - (vPdT2 * (fWdT * (Real)3)) - (vPdT * (fWdT2 * (Real)3)) - (vP * fWdT3) ) * fInvW;

    return vRes;
}

template<typename Real>
Void TNURBSPatch3<Real>::Evaluate( Real fT, TVertex3<Real> * outP, TVector3<Real> * outPdT,
                                            TVector3<Real> * outPdT2, TVector3<Real> * outPdT3 ) const
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

    Real fW = (Real)0, fInvW, fTmp;
    TVertex3<Real> vP = TVertex3<Real>::Null;
    for( i = iMin; i <= iMax; ++i ) {
        fTmp = ( m_arrWeights[i] * m_Basis.N(i) );
        vP += ( m_arrControlPoints[i].ToVector() * fTmp );
        fW += fTmp;
    }
    fInvW = MathRealFn->Invert( fW );
    vP *= fInvW;
    if ( outP != NULL )
        *outP = vP;
    if ( outPdT == NULL && outPdT2 == NULL && outPdT3 == NULL )
        return;

    Real fWdT = (Real)0;
    TVector3<Real> vPdT = TVector3<Real>::Null;
    for( i = iMin; i <= iMax; ++i ) {
        fTmp = ( m_arrWeights[i] * m_Basis.NdT(i) );
        vPdT += ( m_arrControlPoints[i].ToVector() * fTmp );
        fWdT += fTmp;
    }
    vPdT = ( vPdT - (vP * fWdT) ) * fInvW;
    if ( outPdT != NULL )
        *outPdT = vPdT;
    if ( outPdT2 == NULL && outPdT3 == NULL )
        return;

    Real fWdT2 = (Real)0;
    TVector3<Real> vPdT2 = TVector3<Real>::Null;
    for( i = iMin; i <= iMax; ++i ) {
        fTmp = ( m_arrWeights[i] * m_Basis.NdT2(i) );
        vPdT2 += ( m_arrControlPoints[i].ToVector() * fTmp );
        fWdT2 += fTmp;
    }
    vPdT2 = ( vPdT2 - (vPdT * (fWdT * (Real)2)) - (vP * fWdT2) ) * fInvW;
    if ( outPdT2 != NULL )
        *outPdT2 = vPdT2;
    if ( outPdT3 == NULL )
        return;

    Real fWdT3 = (Real)0;
    TVector3<Real> vPdT3 = TVector3<Real>::Null;
    for( i = iMin; i <= iMax; ++i ) {
        fTmp = ( m_arrWeights[i] * m_Basis.NdT3(i) );
        vPdT3 += ( m_arrControlPoints[i].ToVector() * fTmp );
        fWdT3 += fTmp;
    }
    *outPdT3 = ( vPdT3 - (vPdT2 * (fWdT * (Real)3)) - (vPdT * (fWdT2 * (Real)3)) - (vP * fWdT3) ) * fInvW;
}

/////////////////////////////////////////////////////////////////////////////////

template<typename Real>
Void TNURBSPatch3<Real>::_CreateControlPoints( const TVertex3<Real> * arrControlPoints, const Real * arrWeights )
{
    UInt i;
    m_arrControlPoints = New TVertex3<Real>[m_iControlPointCount + m_iReplicateCount];
    m_arrWeights = New Real[m_iControlPointCount + m_iReplicateCount];
    for( i = 0; i < m_iControlPointCount; ++i ) {
        m_arrControlPoints[i] = arrControlPoints[i];
        m_arrWeights[i] = arrWeights[i];
    }
    for ( i = 0; i < m_iReplicateCount; ++i ) {
        m_arrControlPoints[m_iControlPointCount + i] = arrControlPoints[i];
        m_arrWeights[m_iControlPointCount + i] = arrWeights[i];
    }
}


