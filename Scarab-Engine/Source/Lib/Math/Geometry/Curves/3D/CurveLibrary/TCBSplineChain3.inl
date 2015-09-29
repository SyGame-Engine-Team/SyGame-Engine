/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Curves/3D/CurveLibrary/TCBSplineChain3.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Curves 3D : Tension-Continuity-Bias Spline curves
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TTCBSplineChain3 implementation
template<typename Real>
TTCBSplineChain3<Real>::TTCBSplineChain3( const Real * arrParameters, UInt iPatchCount,
                                          const TVertex3<Real> * arrControlPoints,
                                          const Real * arrTensions, const Real * arrContinuities, const Real * arrBiases ):
    TCurveChain3<Real>( arrParameters, iPatchCount )
{
    Assert( m_iPatchCount >= 3 );

    UInt i;

    m_iControlPointCount = m_iPatchCount + 1;

    m_arrControlPoints = New TVertex3<Real>[m_iControlPointCount];
    for( i = 0; i < m_iControlPointCount; ++i )
        m_arrControlPoints[i] = arrControlPoints[i];

    m_arrTensions = New Real[m_iControlPointCount];
    for( i = 0; i < m_iControlPointCount; ++i )
        m_arrTensions[i] = arrTensions[i];

    m_arrContinuities = New Real[m_iControlPointCount];
    for( i = 0; i < m_iControlPointCount; ++i )
        m_arrContinuities[i] = arrContinuities[i];

    m_arrBiases = New Real[m_iControlPointCount];
    for( i = 0; i < m_iControlPointCount; ++i )
        m_arrBiases[i] = arrBiases[i];

    m_arrPdT = New TVector3<Real>[m_iPatchCount];
    m_arrPdT2 = New TVector3<Real>[m_iPatchCount];
    m_arrPdT3 = New TVector3<Real>[m_iPatchCount];

    // Build Derivatives
    _ComputeDerivatives(0, 0, 1, 2);
    for( i = 1; i < m_iPatchCount - 1; ++i )
        _ComputeDerivatives( i-1, i, i+1, i+2 );
    _ComputeDerivatives( m_iPatchCount-2, m_iPatchCount-1, m_iPatchCount, m_iPatchCount );
}
template<typename Real>
TTCBSplineChain3<Real>::~TTCBSplineChain3()
{
    DeleteA( m_arrControlPoints );
    DeleteA( m_arrTensions );
    DeleteA( m_arrContinuities );
    DeleteA( m_arrBiases );
    DeleteA( m_arrPdT );
    DeleteA( m_arrPdT2 );
    DeleteA( m_arrPdT3 );
}

template<typename Real>
inline UInt TTCBSplineChain3<Real>::GetControlPointCount() const {
    return m_iControlPointCount;
}
template<typename Real>
inline const TVertex3<Real> & TTCBSplineChain3<Real>::GetControlPoint( UInt iControlPoint ) const {
    Assert( iControlPoint < m_iControlPointCount );
    return m_arrControlPoints[iControlPoint];
}

template<typename Real>
inline Real TTCBSplineChain3<Real>::GetTension( UInt iControlPoint ) const {
    Assert( iControlPoint < m_iControlPointCount );
    return m_arrTensions[iControlPoint];
}
template<typename Real>
inline Real TTCBSplineChain3<Real>::GetContinuity( UInt iControlPoint ) const {
    Assert( iControlPoint < m_iControlPointCount );
    return m_arrContinuities[iControlPoint];
}
template<typename Real>
inline Real TTCBSplineChain3<Real>::GetBias( UInt iControlPoint ) const {
    Assert( iControlPoint < m_iControlPointCount );
    return m_arrBiases[iControlPoint];
}

template<typename Real>
TVertex3<Real> TTCBSplineChain3<Real>::P( Real fT ) const
{
    UInt iPatch;
    Real fDT;
    _GetPatchInfo( fT, iPatch, fDT );
    fDT /= ( m_arrParameters[iPatch + 1] - m_arrParameters[iPatch] );

    TVertex3<Real> vRes( TVertex3<Real>::Null );
    vRes += m_arrPdT3[iPatch];
    vRes *= fDT;
    vRes += m_arrPdT2[iPatch];
    vRes *= fDT;
    vRes += m_arrPdT[iPatch];
    vRes *= fDT;
    vRes += m_arrControlPoints[iPatch].ToVector();
    return vRes;
}
template<typename Real>
TVector3<Real> TTCBSplineChain3<Real>::PdT( Real fT ) const
{
    UInt iPatch;
    Real fDT;
    _GetPatchInfo( fT, iPatch, fDT );
    fDT /= ( m_arrParameters[iPatch + 1] - m_arrParameters[iPatch] );

    TVector3<Real> vRes;
    vRes  = ( m_arrPdT3[iPatch] * (Real)3 );
    vRes *= fDT;
    vRes += ( m_arrPdT2[iPatch] * (Real)2 );
    vRes *= fDT;
    vRes += m_arrPdT[iPatch];
    return vRes;
}
template<typename Real>
TVector3<Real> TTCBSplineChain3<Real>::PdT2( Real fT ) const
{
    UInt iPatch;
    Real fDT;
    _GetPatchInfo( fT, iPatch, fDT );
    fDT /= ( m_arrParameters[iPatch + 1] - m_arrParameters[iPatch] );

    TVector3<Real> vRes;
    vRes  = ( m_arrPdT3[iPatch] * (Real)6 );
    vRes *= fDT;
    vRes += ( m_arrPdT2[iPatch] * (Real)2 );
    return vRes;
}
template<typename Real>
TVector3<Real> TTCBSplineChain3<Real>::PdT3( Real fT ) const
{
    UInt iPatch;
    Real fDT;
    _GetPatchInfo( fT, iPatch, fDT );
    //fDT /= ( m_arrParameters[iPatch + 1] - m_arrParameters[iPatch] );
    return ( m_arrPdT3[iPatch] * (Real)6 );
}

/////////////////////////////////////////////////////////////////////////////////

template<typename Real>
Real TTCBSplineChain3<Real>::_GetPatchSpeed( UInt iPatch, Real fT ) const
{
    TVector3<Real> vSpeed = ( m_arrPdT3[iPatch] * (Real)3 );
    vSpeed *= fT;
    vSpeed += ( m_arrPdT2[iPatch] * (Real)2 );
    vSpeed *= fT;
    vSpeed += m_arrPdT[iPatch];
    return vSpeed.Norm();
}
template<typename Real>
Real TTCBSplineChain3<Real>::_GetPatchLength( UInt iPatch, Real fT0, Real fT1 ) const
{
    _PatchUserData tmpPUD;
    tmpPUD.pThis = this;
    tmpPUD.iPatch = iPatch;
    TIntegrator1<Real> speedIntegral( _SpeedFunction, &tmpPUD );
    return speedIntegral.RombergIntegral( fT0, fT1 );
}

template<typename Real>
Void TTCBSplineChain3<Real>::_ComputeDerivatives( UInt iPrev, UInt iA, UInt iB, UInt iNext )
{
    TVector<Real> vAB = ( m_arrControlPoints[iB] - m_arrControlPoints[iA] );
    Real fDT = ( m_arrParameters[iB] - m_arrParameters[iA] );

    // Outer tangent at A
    Real f1mT = ( ((Real)1) - m_arrTensions[iA] );
    Real f1mC = ( ((Real)1) - m_arrContinuities[iA] );
    Real f1pC = ( ((Real)1) + m_arrContinuities[iA] );
    Real f1mB = ( ((Real)1) - m_arrBiases[iA] );
    Real f1pB = ( ((Real)1) + m_arrBiases[iA] );
    Real fAdj = ( fDT * (Real)2 ) / ( m_arrParameters[iB] - m_arrParameters[iPrev] );
    Real fOut0 = fAdj * f1mT * f1pC * f1pB * (Real)0.5f;
    Real fOut1 = fAdj * f1mT * f1mC * f1mB * (Real)0.5f;

    TVector3<Real> vPA = ( m_arrControlPoints[iA] - m_arrControlPoints[iPrev] );
    TVector3<Real> vOuterTangent = ( vPA * fOut0 + vAB * fOut1 );

    // Inner tangent at B
    f1mT = ( ((Real)1) - m_arrTensions[iB] );
    f1mC = ( ((Real)1) - m_arrContinuities[iB] );
    f1pC = ( ((Real)1) + m_arrContinuities[iB] );
    f1mB = ( ((Real)1) - m_arrBiases[iB] );
    f1pB = ( ((Real)1) + m_arrBiases[iB] );
    fAdj = ( fDT * (Real)2 ) / ( m_arrParameters[iNext] - m_arrParameters[iA] );
    Real fIn0 = fAdj * f1mT * f1mC * f1pB * (Real)0.5f;
    Real fIn1 = fAdj * f1mT * f1pC * f1mB * (Real)0.5f;

    TVector3<Real> vBN = ( m_arrControlPoints[iNext] - m_arrControlPoints[iB] );
    TVector3<Real> vInnerTangent = ( vAB * fIn0  + vBN * fIn1 );

    // Finalize
    m_arrPdT[iA] = vOuterTangent;
    m_arrPdT2[iA] = ( (vAB * (Real)3) - (vOuterTangent * (Real)2) - vInnerTangent );
    m_arrPdT3[iA] = ( vOuterTangent + vInnerTangent - (vAB * (Real)2) );
}

