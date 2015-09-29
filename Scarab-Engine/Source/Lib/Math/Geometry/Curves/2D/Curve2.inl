/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Curves/2D/Curve2.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Curves 2D : Base class
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TCurve2 implementation
template<typename Real>
TCurve2<Real>::TCurve2( Real fMinT, Real fMaxT )
{
    Assert( fMinT < fMaxT );
    m_fMinT = fMinT;
    m_fMaxT = fMaxT;
}
template<typename Real>
TCurve2<Real>::~TCurve2()
{
    // nothing to do
}

template<typename Real>
inline Real TCurve2<Real>::GetMinT() const {
    return m_fMinT;
}
template<typename Real>
inline Real TCurve2<Real>::GetMaxT() const {
    return m_fMaxT;
}
template<typename Real>
inline Void TCurve2<Real>::SetIntervalT( Real fMinT, Real fMaxT ) {
    Assert( fMinT < fMaxT );
    m_fMinT = fMinT;
    m_fMaxT = fMaxT;
}

template<typename Real>
inline Real TCurve2<Real>::Length() const {
    return Length( m_fMinT, m_fMaxT );
}

template<typename Real>
inline TVertex2<Real> TCurve2<Real>::Position( Real fT ) const {
    return P( fT );
}
template<typename Real>
inline TVector2<Real> TCurve2<Real>::Velocity( Real fT ) const {
    return PdT( fT );
}
template<typename Real>
inline Real TCurve2<Real>::Speed( Real fT ) const {
    return PdT( fT ).Norm();
}

template<typename Real>
inline TVector2<Real> TCurve2<Real>::Tangent( Real fT ) const {
    TVector2<Real> vSpeed = PdT( fT );
    vSpeed.Normalize();
    return vSpeed;
}
template<typename Real>
inline TVector2<Real> TCurve2<Real>::Normal( Real fT ) const {
    TVector2<Real> vSpeed = PdT( fT );
    vSpeed.Normalize();
    return vSpeed.Perp();
}

template<typename Real>
Void TCurve2<Real>::Frame( Real fT, TVertex2<Real> & outPosition, TVector2<Real> & outTangent,
                                    TVector2<Real> & outNormal ) const
{
    outPosition = P( fT );
    outTangent = PdT( fT );
    outTangent.Normalize();
    outNormal = outTangent.Perp();
}

template<typename Real>
Real TCurve2<Real>::Curvature( Real fT ) const
{
    TVector2<Real> vSpeed = PdT( fT );
    Real fSpeedSqr = vSpeed.NormSqr();
    if ( fSpeedSqr < (Real)SCALAR_ERROR )
        return (Real)0;
    TVector2<Real> vAccel = PdT2( fT );
    Real fDotPerp = ( vSpeed * vAccel.Perp() );
    Real fSpeed = MathRealFn->Sqrt( fSpeedSqr );
    return ( fDotPerp / (fSpeedSqr * fSpeed) );
}

template<typename Real>
Void TCurve2<Real>::SubDivideByParameter( Real * outParameters, UInt iCount ) const
{
    Assert( iCount >= 2 );

    Real fInterval = ( (m_fMaxT - m_fMinT) / (Real)(iCount - 1) );
    Real fT = m_fMinT;
    for( UInt i = 0; i < iCount; ++i ) {
        outParameters[i] = fT;
        fT += fInterval;
    }
}
template<typename Real>
Void TCurve2<Real>::SubDivideByLength( Real * outParameters, UInt iCount ) const
{
    Assert( iCount >= 2 );

    Real fInterval = ( Length() / (Real)(iCount - 1) );
    Real fCurLen = (Real)0;
    for( UInt i = 0; i < iCount; ++i ) {
        outParameters[i] = Parameter( fCurLen );
        fCurLen += fInterval;
    }
}

