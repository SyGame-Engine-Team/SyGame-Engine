/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Curves/3D/CurvePatch3.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Curves 3D : Single patch definition
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TCurvePatch3 implementation
template<typename Real>
TCurvePatch3<Real>::TCurvePatch3( Real fMinT, Real fMaxT ):
    TCurve3<Real>( fMinT, fMaxT )
{
    // nothing to do
}
template<typename Real>
TCurvePatch3<Real>::~TCurvePatch3()
{
    // nothing to do
}

template<typename Real>
Real TCurvePatch3<Real>::Length( Real fT0, Real fT1 ) const
{
    Assert( fT0 >= m_fMinT && fT0 <= m_fMaxT );
    Assert( fT1 >= m_fMinT && fT1 <= m_fMaxT );
    Assert( fT0 <= fT1 );

    TIntegrator1<Real> speedIntegral( _SpeedFunction, (Void*)this );
    return speedIntegral.RombergIntegral( fT0, fT1 );
}
template<typename Real>
Real TCurvePatch3<Real>::Parameter( Real fLength, UInt iMaxIterations, Real fTolerance ) const
{
    // Use Newton's root finding method (mixed with bisection)
    Real fTotalLength = TCurve3<Real>::Length();

    // Out of range
    if ( fLength <= (Real)0 )
        return m_fMinT;
    if ( fLength >= fTotalLength )
        return m_fMaxT;

    // Initial guess
    Real fT = ( fLength / fTotalLength );
    fT = ( (((Real)1) - fT) * m_fMinT + fT * m_fMaxT );

    // Interval bounding
    Real fLow = m_fMinT;
    Real fHigh = m_fMaxT;
    Real fDelta, fTry;
    for( UInt i = 0; i < iMaxIterations; ++i ) {
        fDelta = Length(m_fMinT, fT) - fLength;
        if ( MathRealFn->Abs(fDelta) < fTolerance )
            return fT; // close enough
        fTry = fT - ( fDelta / Speed(fT) );
        if ( fDelta > (Real)0 ) {
            fHigh = fT;
            if ( fTry <= fLow ) // out of bound
                fT = ( fLow + fHigh ) * (Real)0.5f;
            else
                fT = fTry;
        } else {
            fLow = fT;
            if ( fTry >= fHigh ) // out of bound
                fT = ( fLow + fHigh ) * (Real)0.5f;
            else
                fT = fTry;
        }
    }
    return fT;
}

/////////////////////////////////////////////////////////////////////////////////

template<typename Real>
inline Real TCurvePatch3<Real>::_SpeedFunction( Real fT, Void * pInstance ) {
    return ( (TCurvePatch3<Real>*)pInstance )->Speed( fT );
}

