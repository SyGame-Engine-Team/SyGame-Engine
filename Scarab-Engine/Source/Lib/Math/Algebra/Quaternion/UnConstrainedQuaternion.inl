/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Algebra/Quaternion/UnConstrainedQuaternion.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Euler closest-orientation solver for unconstrained quaternions
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TUnConstrainedQuaternion implementation
template<typename Real>
const Real TUnConstrainedQuaternion<Real>::ZERO = (Real)0;
template<typename Real>
const Real TUnConstrainedQuaternion<Real>::ONE = (Real)1;
template<typename Real>
const Real TUnConstrainedQuaternion<Real>::HALF = (Real)0.5f;

template<typename Real>
TUnConstrainedQuaternion<Real>::TUnConstrainedQuaternion( const TQuaternion<Real> & qOrientation, Real fZeroTolerance ):
    m_qOrientation( qOrientation )
{
    ZeroTolerance = fZeroTolerance;
}
template<typename Real>
TUnConstrainedQuaternion<Real>::~TUnConstrainedQuaternion()
{
    // nothing to do
}

template<typename Real>
inline Void TUnConstrainedQuaternion<Real>::ClosestX( TQuaternion<Real> & outClosest ) const {
    _SolveClosest( outClosest, AXIS_X );
}
template<typename Real>
inline Void TUnConstrainedQuaternion<Real>::ClosestY( TQuaternion<Real> & outClosest ) const {
    _SolveClosest( outClosest, AXIS_Y );
}
template<typename Real>
inline Void TUnConstrainedQuaternion<Real>::ClosestZ( TQuaternion<Real> & outClosest ) const {
    _SolveClosest( outClosest, AXIS_Z );
}

template<typename Real>
inline Void TUnConstrainedQuaternion<Real>::ClosestXY( TQuaternion<Real> & outClosest ) const {
    _SolveClosestXY( outClosest, m_qOrientation );
}
template<typename Real>
inline Void TUnConstrainedQuaternion<Real>::ClosestXZ( TQuaternion<Real> & outClosest ) const {
    TQuaternion<Real> qTmp( m_qOrientation.X, -(m_qOrientation.Z), m_qOrientation.Y, m_qOrientation.W );
    _SolveClosestXY( outClosest, qTmp );
    Real fSwap = outClosest.Y;
    outClosest.Y = outClosest.Z;
    outClosest.Z = -fSwap;
}
template<typename Real>
inline Void TUnConstrainedQuaternion<Real>::ClosestYX( TQuaternion<Real> & outClosest ) const {
    TQuaternion<Real> qTmp( m_qOrientation.X, m_qOrientation.Y, -(m_qOrientation.Z), m_qOrientation.W );
    _SolveClosestXY( outClosest, qTmp );
    outClosest.Z = -(outClosest.Z);
}
template<typename Real>
inline Void TUnConstrainedQuaternion<Real>::ClosestYZ( TQuaternion<Real> & outClosest ) const {
    TQuaternion<Real> qTmp( m_qOrientation.Y, m_qOrientation.Z, m_qOrientation.X, m_qOrientation.W );
    _SolveClosestXY( outClosest, qTmp );
    Real fSwap = outClosest.Z;
    outClosest.Z = outClosest.Y;
    outClosest.Y = outClosest.X;
    outClosest.X = fSwap;
}
template<typename Real>
inline Void TUnConstrainedQuaternion<Real>::ClosestZX( TQuaternion<Real> & outClosest ) const {
    TQuaternion<Real> qTmp( m_qOrientation.X, m_qOrientation.Z, m_qOrientation.Y, m_qOrientation.W );
    _SolveClosestXY( outClosest, qTmp );
    Real fSwap = outClosest.Y;
    outClosest.Y = outClosest.Z;
    outClosest.Z = fSwap;
}
template<typename Real>
inline Void TUnConstrainedQuaternion<Real>::ClosestZY( TQuaternion<Real> & outClosest ) const {
    TQuaternion<Real> qTmp( m_qOrientation.Y, m_qOrientation.Z, -(m_qOrientation.X), m_qOrientation.W );
    _SolveClosestXY( outClosest, qTmp );
    Real fSwap = outClosest.Z;
    outClosest.Z = outClosest.Y;
    outClosest.Y = outClosest.X;
    outClosest.X = -fSwap;
}

template<typename Real>
inline Void TUnConstrainedQuaternion<Real>::FactorizeXYZ( TVector2<Real> & outComplexX,
                                                          TVector2<Real> & outComplexY, TVector2<Real> & outComplexZ ) const {
    _SolveFactorsXYZ( outComplexX, outComplexY, outComplexZ, m_qOrientation );
}
template<typename Real>
inline Void TUnConstrainedQuaternion<Real>::FactorizeXZY( TVector2<Real> & outComplexX,
                                                          TVector2<Real> & outComplexZ, TVector2<Real> & outComplexY ) const {
    TQuaternion<Real> qTmp( m_qOrientation.X, m_qOrientation.Z, -(m_qOrientation.Y), m_qOrientation.W );
    _SolveFactorsXYZ( outComplexX, outComplexZ, outComplexY, qTmp );
    outComplexY.Y = -outComplexY.Y; // siny
}
template<typename Real>
inline Void TUnConstrainedQuaternion<Real>::FactorizeYXZ( TVector2<Real> & outComplexY,
                                                          TVector2<Real> & outComplexX, TVector2<Real> & outComplexZ ) const {
    TQuaternion<Real> qTmp( -(m_qOrientation.Y), m_qOrientation.X, m_qOrientation.Z, m_qOrientation.W );
    _SolveFactorsXYZ( outComplexY, outComplexX, outComplexZ, qTmp );
    outComplexY.Y = -outComplexY.Y; // siny
}
template<typename Real>
inline Void TUnConstrainedQuaternion<Real>::FactorizeYZX( TVector2<Real> & outComplexY,
                                                          TVector2<Real> & outComplexZ, TVector2<Real> & outComplexX ) const {
    TQuaternion<Real> qTmp( -(m_qOrientation.Y), m_qOrientation.Z, -(m_qOrientation.X), m_qOrientation.W );
    _SolveFactorsXYZ( outComplexY, outComplexZ, outComplexX, qTmp );
    outComplexY.Y = -outComplexY.Y; // siny
    outComplexX.Y = -outComplexX.Y; // sinx
}
template<typename Real>
inline Void TUnConstrainedQuaternion<Real>::FactorizeZXY( TVector2<Real> & outComplexZ,
                                                          TVector2<Real> & outComplexX, TVector2<Real> & outComplexY ) const {
    TQuaternion<Real> qTmp( -(m_qOrientation.Z), m_qOrientation.X, -(m_qOrientation.Y), m_qOrientation.W );
    _SolveFactorsXYZ( outComplexZ, outComplexX, outComplexY, qTmp );
    outComplexZ.Y = -outComplexZ.Y; // sinz
    outComplexY.Y = -outComplexY.Y; // siny
}
template<typename Real>
inline Void TUnConstrainedQuaternion<Real>::FactorizeZYX( TVector2<Real> & outComplexZ,
                                                          TVector2<Real> & outComplexY, TVector2<Real> & outComplexX ) const {
    TQuaternion<Real> qTmp( m_qOrientation.Z, -(m_qOrientation.Y), m_qOrientation.X, m_qOrientation.W );
    _SolveFactorsXYZ( outComplexZ, outComplexY, outComplexX, qTmp );
    outComplexY.Y = -outComplexY.Y; // siny
}

/////////////////////////////////////////////////////////////////////////////////

template<typename Real>
Void TUnConstrainedQuaternion<Real>::_SolveClosest( TQuaternion<Real> & outClosest, Axis iAxis ) const
{
    outClosest.X = ZERO;
    outClosest.Y = ZERO;
    outClosest.Z = ZERO;
    Real fP0 = m_qOrientation.W;
    Real fP1 = m_qOrientation[iAxis];
    Real fInvLength = ( fP0*fP0 + fP1*fP1 );
    if ( fInvLength > ZeroTolerance ) {
        fInvLength = MathRealFn->InvSqrt(fInvLength);
        outClosest.W = fP0 * fInvLength;
        outClosest[iAxis] = fP1 * fInvLength;
    } else {
        outClosest.W = ONE;
        outClosest[iAxis] = ZERO;
    }
}

template<typename Real>
Void TUnConstrainedQuaternion<Real>::_SolveClosestXY( TQuaternion<Real> & outClosest, const TQuaternion<Real> & qOrientation ) const
{
    Real fDet = ( (qOrientation.W * qOrientation.Z) - (qOrientation.X * qOrientation.Y) );
    Real fAbsDet = MathRealFn->Abs(fDet);
    if ( fAbsDet < (HALF - ZeroTolerance) ) {
        Real fDiscr = MathRealFn->Abs( ONE - (fDet * fDet * (Real)4) );
        fDiscr = MathRealFn->Sqrt(fDiscr);
        Real fA = ( (qOrientation.W * qOrientation.X) + (qOrientation.Y * qOrientation.Z) );
        Real fB = ( (qOrientation.W * qOrientation.W) - (qOrientation.X * qOrientation.X) +
                    (qOrientation.Y * qOrientation.Y) - (qOrientation.Z * qOrientation.Z) );
        Real fCos0, fSin0;
        if ( fB >= ZERO ) {
            fCos0 = HALF * (fDiscr + fB);
            fSin0 = fA;
        } else {
            fCos0 = fA;
            fSin0 = HALF * (fDiscr - fB);
        }
        Real fInvLength = MathRealFn->InvSqrt( (fCos0 * fCos0) + (fSin0 * fSin0) );
        fCos0 *= fInvLength;
        fSin0 *= fInvLength;
        Real fCos1 = ( (qOrientation.W * fCos0) + (qOrientation.X * fSin0) );
        Real fSin1 = ( (qOrientation.Y * fCos0) + (qOrientation.Z * fSin0) );
        fInvLength = MathRealFn->InvSqrt( (fCos1 * fCos1) + (fSin1 * fSin1) );
        fCos1 *= fInvLength;
        fSin1 *= fInvLength;
        outClosest.X = fSin0 * fCos1;
        outClosest.Y = fCos0 * fSin1;
        outClosest.Z = fSin0 * fSin1;
        outClosest.W = fCos0 * fCos1;
    } else {
        Real fInvLength = MathRealFn->InvSqrt(fAbsDet);
        outClosest.X = qOrientation.X * fInvLength;
        outClosest.Y = ZERO;
        outClosest.Z = ZERO;
        outClosest.W = qOrientation.W * fInvLength;
    }
}

template<typename Real>
Void TUnConstrainedQuaternion<Real>::_SolveFactorsXYZ( TVector2<Real> & outComplexX,
                                                       TVector2<Real> & outComplexY, TVector2<Real> & outComplexZ,
                                                       const TQuaternion<Real> & qOrientation ) const
{
    Real fA = ( (qOrientation.W * qOrientation.X) - (qOrientation.Y * qOrientation.Z) );
    Real fB = HALF * ( (qOrientation.W * qOrientation.W) - (qOrientation.X * qOrientation.X) -
                       (qOrientation.Y * qOrientation.Y) + (qOrientation.Z * qOrientation.Z) );
    Real fInvLength = MathRealFn->Sqrt( (fA * fA) + (fB * fB) );
    if ( fInvLength > ZeroTolerance ) {
        fInvLength = MathRealFn->Invert(fInvLength);
        Real fSigma0 = fA * fInvLength;
        Real fGamma0 = fB * fInvLength;
        if ( fGamma0 >= ZERO ) {
            outComplexX.X = MathRealFn->Sqrt( HALF * (ONE + fGamma0) ); // cosx
            outComplexX.Y = HALF * fSigma0 / outComplexX.X; // sinx
        } else {
            outComplexX.Y = MathRealFn->Sqrt( HALF * (ONE - fGamma0) ); // sinx
            outComplexX.X = HALF * fSigma0 / outComplexX.Y; // cosx
        }

        Real fTmp0 = ( (qOrientation.W * outComplexX.X) + (qOrientation.X * outComplexX.Y) );
        Real fTmp1 = ( (qOrientation.Z * outComplexX.X) - (qOrientation.Y * outComplexX.Y) );
        fInvLength = MathRealFn->InvSqrt( (fTmp0 * fTmp0) + (fTmp1 * fTmp1) );
        outComplexZ.X = fTmp0 * fInvLength; // cosz
        outComplexZ.Y = fTmp1 * fInvLength; // sinz

        if ( MathRealFn->Abs(outComplexZ.X) >= MathRealFn->Abs(outComplexZ.Y) ) {
            fInvLength = MathRealFn->Invert(outComplexZ.X);
            outComplexY.X = fTmp0 * fInvLength; // cosy
            outComplexY.Y = ( (qOrientation.Y * outComplexX.X) + (qOrientation.Z * outComplexX.Y) ) * fInvLength; // siny
        } else {
            fInvLength = MathRealFn->Invert(outComplexZ.Y);
            outComplexY.X = fTmp1 * fInvLength; // cosy
            outComplexY.Y = ( (qOrientation.X * outComplexX.X) - (qOrientation.W * outComplexX.Y) ) * fInvLength; // siny
        }
    } else {
        const Real fInvSqrt2 = (Real)SCALAR_SQRT2_2;
        const Real fSqrt2 = (Real)SCALAR_SQRT2;
        outComplexX.X = ONE; // cosx
        outComplexX.Y = ZERO; // sinx
        outComplexY.X = fInvSqrt2; // cosy
        outComplexZ.X = qOrientation.W * fSqrt2; // cosz
        if ( ((qOrientation.W * qOrientation.Y) + (qOrientation.X * qOrientation.Z)) > ZERO ) {
            outComplexY.Y = fInvSqrt2; // siny
            outComplexZ.Y = qOrientation.X * fSqrt2; // sinz
        } else {
            outComplexY.Y = -fInvSqrt2; // siny
            outComplexZ.Y = -qOrientation.X * fSqrt2; // sinz
        }
    }
}
