/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Curves/3D/Curve3.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Curves 3D : Base class
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TCurve3 implementation
template<typename Real>
TCurve3<Real>::TCurve3( Real fMinT, Real fMaxT )
{
    Assert( fMinT < fMaxT );
    m_fMinT = fMinT;
    m_fMaxT = fMaxT;
}
template<typename Real>
TCurve3<Real>::~TCurve3()
{
    // nothing to do
}

template<typename Real>
inline Real TCurve3<Real>::GetMinT() const {
    return m_fMinT;
}
template<typename Real>
inline Real TCurve3<Real>::GetMaxT() const {
    return m_fMaxT;
}
template<typename Real>
inline Void TCurve3<Real>::SetIntervalT( Real fMinT, Real fMaxT ) {
    Assert( fMinT < fMaxT );
    m_fMinT = fMinT;
    m_fMaxT = fMaxT;
}

template<typename Real>
inline Real TCurve3<Real>::Length() const {
    return Length( m_fMinT, m_fMaxT );
}

template<typename Real>
inline TVertex3<Real> TCurve3<Real>::Position( Real fT ) const {
    return P( fT );
}
template<typename Real>
inline TVector3<Real> TCurve3<Real>::Velocity( Real fT ) const {
    return PdT( fT );
}
template<typename Real>
inline Real TCurve3<Real>::Speed( Real fT ) const {
    return PdT( fT ).Norm();
}

template<typename Real>
inline TVector3<Real> TCurve3<Real>::Tangent( Real fT ) const {
    TVector3<Real> vSpeed = PdT( fT );
    vSpeed.Normalize();
    return vSpeed;
}
template<typename Real>
TVector3<Real> TCurve3<Real>::Normal( Real fT ) const
{
    TVector3<Real> vSpeed = PdT( fT );
    TVector3<Real> vAccel = PdT2( fT );
    Real fDotVV = (vSpeed * vSpeed);
    Real fDotVA = (vSpeed * vAccel);
    TVector3<Real> vNormal = ( (vAccel * fDotVV) - (vSpeed * fDotVA) );
    vNormal.Normalize();
    return vNormal;
}
template<typename Real>
TVector3<Real> TCurve3<Real>::BiNormal( Real fT ) const
{
    TVector3<Real> vSpeed = PdT( fT );
    TVector3<Real> vAccel = PdT2( fT );
    Real fDotVV = (vSpeed * vSpeed);
    Real fDotVA = (vSpeed * vAccel);
    TVector3<Real> vNormal = ( (vAccel * fDotVV) - (vSpeed * fDotVA) );
    TVector3<Real> vBiNormal = ( vSpeed ^ vNormal );
    vBiNormal.Normalize();
    return vBiNormal;
}
template<typename Real>
Void TCurve3<Real>::Frame( Real fT, TVertex3<Real> & outPosition, TVector3<Real> & outTangent,
                                    TVector3<Real> & outNormal, TVector3<Real> & outBiNormal ) const
{
    outPosition = P( fT );
    outTangent = PdT( fT );
    outBiNormal = PdT2( fT );
    Real fDotVV = (outTangent * outTangent);
    Real fDotVA = (outTangent * outBiNormal);
    outNormal = ( (outBiNormal * fDotVV) - (outTangent * fDotVA) );
    outTangent.Normalize();
    outNormal.Normalize();
    outBiNormal = ( outTangent ^ outNormal );
}

template<typename Real>
Real TCurve3<Real>::Curvature( Real fT ) const
{
    TVector3<Real> vSpeed = PdT( fT );
    Real fSpeedSqr = vSpeed.NormSqr();
    if ( fSpeedSqr < (Real)SCALAR_ERROR )
        return (Real)0;
    TVector3<Real> vAccel = PdT2( fT );
    TVector3<Real> vCross = ( vSpeed ^ vAccel );
    Real fSpeed = MathRealFn->Sqrt( fSpeedSqr );
    return ( vCross.Norm() / (fSpeedSqr * fSpeed) );
}
template<typename Real>
Real TCurve3<Real>::Torsion( Real fT ) const
{
    TVector3<Real> vSpeed = PdT( fT );
    TVector3<Real> vAccel = PdT2( fT );
    TVector3<Real> vCross = ( vSpeed ^ vAccel );
    Real fCrossSqr = vCross.NormSqr();
    if ( fCrossSqr < (Real)SCALAR_ERROR )
        return (Real)0;
    return ( (vCross * PdT3(fT)) / fCrossSqr );
}

template<typename Real>
Void TCurve3<Real>::SubDivideByParameter( Real * outParameters, UInt iCount ) const
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
Void TCurve3<Real>::SubDivideByLength( Real * outParameters, UInt iCount ) const
{
    Assert( iCount >= 2 );

    Real fInterval = ( Length() / (Real)(iCount - 1) );
    Real fCurLen = (Real)0;
    for( UInt i = 0; i < iCount; ++i ) {
        outParameters[i] = Parameter( fCurLen );
        fCurLen += fInterval;
    }
}


