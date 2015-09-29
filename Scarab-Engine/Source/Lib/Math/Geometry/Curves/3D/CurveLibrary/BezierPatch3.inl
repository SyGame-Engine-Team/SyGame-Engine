/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Curves/3D/CurveLibrary/BezierPatch3.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Curves 3D : Bezier curves
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TBezierPatch3 implementation
template<typename Real>
TBezierPatch3<Real>::TBezierPatch3( const TVertex3<Real> * arrControlPoints, UInt iCount ):
    TCurvePatch3<Real>( (Real)0, (Real)1 )
{
    Assert( iCount >= 3 );

    m_iDegree = iCount - 1;
    m_iControlPointCount = iCount;

    // Binomial values
    m_arrBinomial = New Real[m_iControlPointCount * m_iControlPointCount];
    m_arrBinomial[0] = (Real)1;
    m_arrBinomial[m_iControlPointCount] = (Real)1;
    m_arrBinomial[m_iControlPointCount + 1] = (Real)1;
    UInt i, j, iOffset, iPrevOffset;
    for( i = 2; i < m_iControlPointCount; ++i ) {
        iOffset = ( i * m_iControlPointCount );
        iPrevOffset = ( (i-1) * m_iControlPointCount );
        m_arrBinomial[iOffset] = (Real)1;
        for( j = 1; j < i; ++j ) {
            m_arrBinomial[iOffset + j] = ( m_arrBinomial[iPrevOffset + j-1] +
                                           m_arrBinomial[iPrevOffset + j] );
        }
        m_arrBinomial[iOffset + i] = (Real)1;
    }

    // Control points
    m_arrControlPoints = New TVertex3<Real>[m_iControlPointCount];
    for( i = 0; i < m_iControlPointCount; ++i )
        m_arrControlPoints[i] = arrControlPoints[i];

    // Derivatives
    m_arrPdT = New TVector3<Real>[m_iControlPointCount - 1];
    for( i = 0; i < m_iControlPointCount - 1; ++i )
        m_arrPdT[i] = m_arrControlPoints[i + 1] - m_arrControlPoints[i];

    m_arrPdT2 = New TVector3<Real>[m_iControlPointCount - 2];
    for( i = 0; i < m_iControlPointCount - 2; ++i )
        m_arrPdT2[i] = m_arrPdT[i + 1] - m_arrPdT[i];

    m_arrPdT3 = NULL;
    if ( m_iDegree >= 3 ) {
        m_arrPdT3 = New TVector3<Real>[m_iControlPointCount - 3];
        for( i = 0; i < m_iControlPointCount - 3; ++i )
            m_arrPdT3[i] = m_arrPdT2[i + 1] - m_arrPdT2[i];
    }
}
template<typename Real>
TBezierPatch3<Real>::~TBezierPatch3()
{
    DeleteA( m_arrBinomial );
    DeleteA( m_arrControlPoints );
    DeleteA( m_arrPdT );
    DeleteA( m_arrPdT2 );
    if ( m_arrPdT3 != NULL )
        DeleteA( m_arrPdT3 );
}

template<typename Real>
inline UInt TBezierPatch3<Real>::GetDegree() const {
    return m_iDegree;
}

template<typename Real>
inline UInt TBezierPatch3<Real>::GetControlPointCount() const {
    return m_iControlPointCount;
}
template<typename Real>
inline const TVertex3<Real> & TBezierPatch3<Real>::GetControlPoint( UInt iControlPoint ) const {
    Assert( iControlPoint < m_iControlPointCount );
    return m_arrControlPoints[iControlPoint];
}
template<typename Real>
Void TBezierPatch3<Real>::SetControlPoint( UInt iControlPoint, const TVertex3<Real> & vPoint )
{
    Assert( iControlPoint < m_iControlPointCount );
    m_arrControlPoints[iControlPoint] = vPoint;

    // m_arrControlPoints[iControlPoint] has changed
    if ( iControlPoint < m_iControlPointCount - 1 )
        m_arrPdT[iControlPoint] = m_arrControlPoints[iControlPoint + 1] - m_arrControlPoints[iControlPoint];
    if ( iControlPoint > 0 )
        m_arrPdT[iControlPoint - 1] = m_arrControlPoints[iControlPoint] - m_arrControlPoints[iControlPoint - 1];

    // m_arrPdT[iControlPoint], m_arrPdT[iControlPoint-1] have changed
    if ( iControlPoint < m_iControlPointCount - 2 )
        m_arrPdT2[iControlPoint] = m_arrPdT[iControlPoint + 1] - m_arrPdT[iControlPoint];
    if ( iControlPoint > 0 && iControlPoint < m_iControlPointCount - 1 )
        m_arrPdT2[iControlPoint - 1] = m_arrPdT[iControlPoint] - m_arrPdT[iControlPoint - 1];
    if ( iControlPoint > 1 )
        m_arrPdT2[iControlPoint - 2] = m_arrPdT[iControlPoint - 1] - m_arrPdT[iControlPoint - 2];

    // m_arrPdT2[iControlPoint], m_arrPdT2[iControlPoint-1], m_arrPdT2[iControlPoint-2] have changed
    if ( m_iDegree >= 3 ) {
        if ( iControlPoint < m_iControlPointCount - 3 )
            m_arrPdT3[iControlPoint] = m_arrPdT2[iControlPoint + 1] - m_arrPdT2[iControlPoint];
        if ( iControlPoint > 0 && iControlPoint < m_iControlPointCount - 2 )
            m_arrPdT3[iControlPoint - 1] = m_arrPdT2[iControlPoint] - m_arrPdT2[iControlPoint - 1];
        if ( iControlPoint > 1 && iControlPoint < m_iControlPointCount - 1 )
            m_arrPdT3[iControlPoint - 2] = m_arrPdT2[iControlPoint - 1] - m_arrPdT2[iControlPoint - 2];
        if ( iControlPoint > 2 )
            m_arrPdT3[iControlPoint - 3] = m_arrPdT2[iControlPoint - 2] - m_arrPdT2[iControlPoint - 3];
    }
}

template<typename Real>
TVertex3<Real> TBezierPatch3<Real>::P( Real fT ) const
{
    Real fInvT = ( ((Real)1) - fT );
    Real fPowT = fT;
    Real fFactor;

    TVertex3<Real> vPos = ( m_arrControlPoints[0] * fInvT );

    TVector3<Real> vTmp;
    UInt iBinomOffset = (m_iDegree * m_iControlPointCount);
    for( UInt i = 1; i < m_iDegree; ++i ) {
        fFactor = m_arrBinomial[iBinomOffset + i] * fPowT;
        vTmp = ( m_arrControlPoints[i].ToVector() * fFactor );
        vPos = ( vPos + vTmp ) * fInvT;
        fPowT *= fT;
    }
    vTmp = ( m_arrControlPoints[m_iDegree].ToVector() * fPowT );
    vPos += vTmp;

    return vPos;
}
template<typename Real>
TVector3<Real> TBezierPatch3<Real>::PdT( Real fT ) const
{
    Real fInvT = ( ((Real)1) - fT );
    Real fPowT = fT;
    Real fFactor;

    TVector3<Real> vSpeed = ( m_arrPdT[0] * fInvT );

    UInt iDegM1 = (m_iDegree - 1);
    UInt iBinomOffset = (iDegM1 * m_iControlPointCount);
    for( UInt i = 1; i < iDegM1; ++i ) {
        fFactor = m_arrBinomial[iBinomOffset + i] * fPowT;
        vSpeed = ( vSpeed + (m_arrPdT[i] * fFactor) ) * fInvT;
        fPowT *= fT;
    }
    vSpeed += ( m_arrPdT[iDegM1] * fPowT );
    vSpeed *= (Real)m_iDegree;

    return vSpeed;
}
template<typename Real>
TVector3<Real> TBezierPatch3<Real>::PdT2( Real fT ) const
{
    Real fInvT = ( ((Real)1) - fT );
    Real fPowT = fT;
    Real fFactor;

    TVector3<Real> vAccel = ( m_arrPdT2[0] * fInvT );

    UInt iDegM2 = (m_iDegree - 2);
    UInt iBinomOffset = (iDegM2 * m_iControlPointCount);
    for( UInt i = 1; i < iDegM2; ++i ) {
        fFactor = m_arrBinomial[iBinomOffset + i] * fPowT;
        vAccel = ( vAccel + (m_arrPdT2[i] * fFactor) ) * fInvT;
        fPowT *= fT;
    }
    vAccel += ( m_arrPdT2[iDegM2] * fPowT );
    vAccel *= (Real)( m_iDegree * (m_iDegree - 1) );

    return vAccel;
}
template<typename Real>
TVector3<Real> TBezierPatch3<Real>::PdT3( Real fT ) const
{
    if ( m_iDegree < 3 )
        return TVector3<Real>::Null;

    Real fInvT = ( ((Real)1) - fT );
    Real fPowT = fT;
    Real fFactor;

    TVector3<Real> vJerk = ( m_arrPdT3[0] * fInvT );

    UInt iDegM3 = (m_iDegree - 3);
    UInt iBinomOffset = (iDegM3 * m_iControlPointCount);
    for( UInt i = 1; i < iDegM3; ++i ) {
        fFactor = m_arrBinomial[iBinomOffset + i] * fPowT;
        vJerk = ( vJerk + (m_arrPdT3[i] * fFactor) ) * fInvT;
        fPowT *= fT;
    }
    vJerk += ( m_arrPdT3[iDegM3] * fPowT );
    vJerk *= (Real)( m_iDegree * (m_iDegree - 1) * (m_iDegree - 2) );

    return vJerk;
}


