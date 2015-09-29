/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Algebra/Quaternion/ConstrainedQuaternion.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Euler closest-orientation solver for constrained quaternions
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TConstrainedQuaternion implementation
template<typename Real>
const Real TConstrainedQuaternion<Real>::ZERO = (Real)0;
template<typename Real>
const Real TConstrainedQuaternion<Real>::ONE = (Real)1;
template<typename Real>
const Real TConstrainedQuaternion<Real>::HALF = (Real)0.5f;

template<typename Real>
Void TConstrainedQuaternion<Real>::AngleConstraint::SetValues( Real fMin, Real fMax )
{
    fMinAngle = fMin;
    fMaxAngle = fMax;
    MathRealFn->SinCos( &fSinMin, &fCosMin, fMinAngle );
    MathRealFn->SinCos( &fSinMax, &fCosMax, fMaxAngle );
    fDeltaSin = fSinMax - fSinMin;
    fDeltaCos = fCosMax - fCosMin;
    MathRealFn->SinCos( &fSinAvg, &fCosAvg, (fMinAngle + fMaxAngle) * HALF );
}
template<typename Real>
Bool TConstrainedQuaternion<Real>::AngleConstraint::IsValid( Real fX, Real fY ) const
{
    // test (x,y)
    Real fDX = fX - fCosMin;
    Real fDY = fY - fSinMin;
    if ( (fDX * fDeltaSin) >= (fDY * fDeltaCos) )
        return true;

    // test (-x,-y)
    fDX = fX + fCosMin;
    fDY = fY + fSinMin;
    if ( (fDX * fDeltaSin) <= (fDY * fDeltaCos) )
        return true;

    return false;
}

template<typename Real>
TConstrainedQuaternion<Real>::TConstrainedQuaternion( const TQuaternion<Real> & qOrientation, Real fZeroTolerance ):
    m_qOrientation( qOrientation )
{
    ZeroTolerance = fZeroTolerance;
}
template<typename Real>
TConstrainedQuaternion<Real>::~TConstrainedQuaternion()
{
    // nothing to do
}

template<typename Real>
inline Void TConstrainedQuaternion<Real>::ClosestX( TQuaternion<Real> & outClosest, const AngleConstraint & constraintX ) const {
    _SolveClosest( outClosest, m_qOrientation, AXIS_X, constraintX );
}
template<typename Real>
inline Void TConstrainedQuaternion<Real>::ClosestY( TQuaternion<Real> & outClosest, const AngleConstraint & constraintY ) const {
    _SolveClosest( outClosest, m_qOrientation, AXIS_Y, constraintY );
}
template<typename Real>
inline Void TConstrainedQuaternion<Real>::ClosestZ( TQuaternion<Real> & outClosest, const AngleConstraint & constraintZ ) const {
    _SolveClosest( outClosest, m_qOrientation, AXIS_Z, constraintZ );
}

template<typename Real>
inline Void TConstrainedQuaternion<Real>::ClosestXY( TQuaternion<Real> & outClosest,
                                                     const AngleConstraint & constraintX, const AngleConstraint & constraintY ) const {
    _SolveClosestXY( outClosest, m_qOrientation, constraintX, constraintY );
}
template<typename Real>
inline Void TConstrainedQuaternion<Real>::ClosestXZ( TQuaternion<Real> & outClosest,
                                                     const AngleConstraint & constraintX, const AngleConstraint & constraintZ ) const {
    TQuaternion<Real> qTmp( m_qOrientation.X, -(m_qOrientation.Y), m_qOrientation.Z, m_qOrientation.W );
    _SolveClosestZX( outClosest, qTmp, constraintZ, constraintX );
    outClosest.Y = -(outClosest.Y);
}
template<typename Real>
inline Void TConstrainedQuaternion<Real>::ClosestYX( TQuaternion<Real> & outClosest,
                                                     const AngleConstraint & constraintY, const AngleConstraint & constraintX ) const {
    TQuaternion<Real> qTmp( m_qOrientation.X, m_qOrientation.Y, -(m_qOrientation.Z), m_qOrientation.W );
    _SolveClosestXY( outClosest, qTmp, constraintX, constraintY );
    outClosest.Z = -(outClosest.Z);
}
template<typename Real>
inline Void TConstrainedQuaternion<Real>::ClosestYZ( TQuaternion<Real> & outClosest,
                                                     const AngleConstraint & constraintY, const AngleConstraint & constraintZ ) const {
    TQuaternion<Real> qTmp( -(m_qOrientation.X), m_qOrientation.Y, m_qOrientation.Z, m_qOrientation.W );
    _SolveClosestZY( outClosest, qTmp, constraintZ, constraintY );
    outClosest.X = -(outClosest.X);
}
template<typename Real>
inline Void TConstrainedQuaternion<Real>::ClosestZX( TQuaternion<Real> & outClosest,
                                                     const AngleConstraint & constraintZ, const AngleConstraint & constraintX ) const {
    _SolveClosestZX( outClosest, m_qOrientation, constraintZ, constraintX );
}
template<typename Real>
inline Void TConstrainedQuaternion<Real>::ClosestZY( TQuaternion<Real> & outClosest,
                                                     const AngleConstraint & constraintZ, const AngleConstraint & constraintY ) const {
    _SolveClosestZY( outClosest, m_qOrientation, constraintZ, constraintY );
}

/////////////////////////////////////////////////////////////////////////////////

template<typename Real>
Void TConstrainedQuaternion<Real>::_SolveClosest( TQuaternion<Real> & outClosest, const TQuaternion<Real> & qOrientation,
                                                  Axis iAxis, const AngleConstraint & constraintAxis  ) const
{
    outClosest.X = ZERO;
    outClosest.Y = ZERO;
    outClosest.Z = ZERO;
    Real fP0 = qOrientation.W;
    Real fP1 = qOrientation[iAxis];
    Real fInvLength = ( fP0*fP0 + fP1*fP1 );
    if ( fInvLength > ZeroTolerance ) {
        fInvLength = MathRealFn->InvSqrt(fInvLength);
        fP0 *= fInvLength;
        fP1 *= fInvLength;
        if ( constraintAxis.IsValid(fP0, fP1) ) {
            outClosest.W = fP0;
            outClosest[iAxis] = fP1;
        } else {
            Real fCMin = constraintAxis.fCosMin;
            Real fSMin = constraintAxis.fSinMin;
            Real fDotMin = ( fP0*fCMin + fP1*fSMin );
            if ( fDotMin < ZERO ) {
                fCMin = -fCMin;
                fSMin = -fSMin;
                fDotMin = -fDotMin;
            }
            Real fCMax = constraintAxis.fCosMax;
            Real fSMax = constraintAxis.fSinMax;
            Real fDotMax = ( fP0*fCMax + fP1*fSMax );
            if ( fDotMax < ZERO ) {
                fCMax = -fCMax;
                fSMax = -fSMax;
                fDotMax = -fDotMax;
            }
            if ( fDotMax >= fDotMin ) {
                outClosest.W = fCMax;
                outClosest[iAxis] = fSMax;
            } else {
                outClosest.W = fCMin;
                outClosest[iAxis] = fSMin;
            }
        }
    } else {
        outClosest.W = constraintAxis.fCosAvg;
        outClosest[iAxis] = constraintAxis.fSinAvg;
    }
}

template<typename Real>
Void TConstrainedQuaternion<Real>::_SolveClosestXY( TQuaternion<Real> & outClosest, const TQuaternion<Real> & qOrientation,
                                                    const AngleConstraint & constraintX, const AngleConstraint & constraintY ) const
{
    Real fCos0, fSin0, fCos1, fSin1, fInvLength;
    TQuaternion<Real> qTmp;

    Real fDet = ( (qOrientation.W * qOrientation.Z) - (qOrientation.X * qOrientation.Y) );
    if ( MathRealFn->Abs(fDet) < (HALF - ZeroTolerance) ) {
        Real fDiscr = MathRealFn->Abs( ONE - fDet * fDet * (Real)4 );
        fDiscr = MathRealFn->Sqrt(fDiscr);
        Real fA = ( qOrientation.W * qOrientation.X + qOrientation.Y * qOrientation.Z );
        Real fB = ( qOrientation.W * qOrientation.W - qOrientation.X * qOrientation.X )
                + ( qOrientation.Y * qOrientation.Y - qOrientation.Z * qOrientation.Z );
        if ( fB >= ZERO ) {
            fCos0 = HALF * (fDiscr + fB);
            fSin0 = fA;
        } else {
            fCos0 = fA;
            fSin0 = HALF * (fDiscr - fB);
        }
        fInvLength = MathRealFn->InvSqrt( (fCos0 * fCos0) + (fSin0 * fSin0) );
        fCos0 *= fInvLength;
        fSin0 *= fInvLength;
        fCos1 = qOrientation.W * fCos0 + qOrientation.X * fSin0;
        fSin1 = qOrientation.Y * fCos0 + qOrientation.Z * fSin0;
        fInvLength = MathRealFn->InvSqrt( (fCos1 * fCos1) + (fSin1 * fSin1) );
        fCos1 *= fInvLength;
        fSin1 *= fInvLength;

        if ( constraintX.IsValid(fCos0, fSin0) && constraintY.IsValid(fCos1, fSin1) ) {
            outClosest.X = fSin0 * fCos1;
            outClosest.Y = fCos0 * fSin1;
            outClosest.Z = fSin0 * fSin1;
            outClosest.W = fCos0 * fCos1;
        } else {
            TQuaternion<Real> qR(constraintX.fSinMin, ZERO, ZERO, constraintX.fCosMin);
            TQuaternion<Real> qInvR(-(constraintX.fSinMin), ZERO, ZERO, constraintX.fCosMin);
            TQuaternion<Real> qProd = (qInvR * qOrientation);
            _SolveClosest( qTmp, qProd, AXIS_Y, constraintY );
            Real fDot = qProd.Dot(qTmp);
            outClosest = (qR * qTmp);

            qR.X = constraintX.fSinMax;
            qR.W = constraintX.fCosMax;
            qInvR.X = -(constraintX.fSinMax);
            qInvR.W = constraintX.fCosMax;
            qProd = (qInvR * qOrientation);
            _SolveClosest( qTmp, qProd, AXIS_Y, constraintY );
            Real fTmpDot = qProd.Dot(qTmp);
            if (fTmpDot > fDot) {
                outClosest = (qR * qTmp);
                fDot = fTmpDot;
            }

            qR.X = ZERO;
            qInvR.X = ZERO;

            qR.Y = constraintY.fSinMin;
            qR.W = constraintY.fCosMin;
            qInvR.Y = -(constraintY.fSinMin);
            qInvR.W = constraintY.fCosMin;
            qProd = (qOrientation * qInvR);
            _SolveClosest( qTmp, qProd, AXIS_X, constraintX );
            fTmpDot = qProd.Dot(qTmp);
            if (fTmpDot > fDot) {
                outClosest = (qTmp * qR);
                fDot = fTmpDot;
            }

            qR.Y = constraintY.fSinMax;
            qR.W = constraintY.fCosMax;
            qInvR.Y = -(constraintY.fSinMax);
            qInvR.W = constraintY.fCosMax;
            qProd = (qOrientation * qInvR);
            _SolveClosest( qTmp, qProd, AXIS_X, constraintX );
            fTmpDot = qProd.Dot(qTmp);
            if (fTmpDot > fDot) {
                outClosest = (qTmp * qR);
                fDot = fTmpDot;
            }
        }
    } else {
        Real fMin, fMax, fAngle;
        AngleConstraint constraintTmp;
        if ( fDet > ZERO ) {
            fMin = constraintX.fMinAngle - constraintY.fMaxAngle;
            fMax = constraintX.fMaxAngle - constraintY.fMinAngle;
            constraintTmp.SetValues( fMin, fMax );
            _SolveClosest( qTmp, qOrientation, AXIS_X, constraintTmp );

            fAngle = MathRealFn->ArcTan2( qTmp.X, qTmp.W );
            if ( fAngle < fMin || fAngle > fMax )
                fAngle -= (Real)( (qTmp.X >= ZERO) ? SCALAR_PI : (-SCALAR_PI) );

            if ( fAngle <= (constraintX.fMaxAngle - constraintY.fMaxAngle) ) {
                fCos1 = constraintY.fCosMax;
                fSin1 = constraintY.fSinMax;
                fAngle = constraintY.fMaxAngle + fAngle;
                MathRealFn->SinCos( &fSin0, &fCos0, fAngle );
            } else {
                fCos0 = constraintX.fCosMax;
                fSin0 = constraintX.fSinMax;
                fAngle = constraintX.fMaxAngle - fAngle;
                MathRealFn->SinCos( &fSin1, &fCos1, fAngle );
            }
        } else {
            fMin = constraintX.fMinAngle + constraintY.fMinAngle;
            fMax = constraintX.fMaxAngle + constraintY.fMaxAngle;
            constraintTmp.SetValues( fMin, fMax );
            _SolveClosest( qTmp, qOrientation, AXIS_X, constraintTmp );

            fAngle = MathRealFn->ArcTan2( qTmp.X, qTmp.W );
            if (fAngle < fMin || fAngle > fMax)
                fAngle -= (Real)( (qTmp.X >= ZERO) ? SCALAR_PI : (-SCALAR_PI) );

            if ( fAngle >= (constraintX.fMinAngle + constraintY.fMaxAngle) ) {
                fCos1 = constraintY.fCosMax;
                fSin1 = constraintY.fSinMax;
                fAngle = fAngle - constraintY.fMaxAngle;
                MathRealFn->SinCos( &fSin0, &fCos0, fAngle );
            } else {
                fCos0 = constraintX.fCosMax;
                fSin0 = constraintX.fSinMax;
                fAngle = fAngle - constraintX.fMaxAngle;
                MathRealFn->SinCos( &fSin1, &fCos1, fAngle );
            }
        }
        outClosest.X = fSin0 * fCos1;
        outClosest.Y = fCos0 * fSin1;
        outClosest.Z = fSin0 * fSin1;
        outClosest.W = fCos0 * fCos1;
        if ( qOrientation.Dot(outClosest) < ZERO )
            outClosest = -outClosest;
    }
}

template<typename Real>
Void TConstrainedQuaternion<Real>::_SolveClosestZX( TQuaternion<Real> & outClosest, const TQuaternion<Real> & qOrientation,
                                                    const AngleConstraint & constraintZ, const AngleConstraint & constraintX ) const
{
    Real fCos0, fSin0, fCos2, fSin2, fInvLength;
    TQuaternion<Real> qTmp;

    Real fDet = ( qOrientation.W * qOrientation.Y - qOrientation.X * qOrientation.Z );
    if ( MathRealFn->Abs(fDet) < (HALF - ZeroTolerance) ) {
        Real fDiscr = MathRealFn->Abs( ONE - fDet * fDet * (Real)4 );
        fDiscr = MathRealFn->Sqrt(fDiscr);
        Real fA = ( qOrientation.W * qOrientation.Z + qOrientation.X * qOrientation.Y );
        Real fB = ( qOrientation.W * qOrientation.W + qOrientation.X * qOrientation.X )
                - ( qOrientation.Y * qOrientation.Y + qOrientation.Z * qOrientation.Z );
        if ( fB >= ZERO ) {
            fCos2 = HALF * (fDiscr + fB);
            fSin2 = fA;
        } else {
            fCos2 = fA;
            fSin2 = HALF * (fDiscr - fB);
        }
        fInvLength = MathRealFn->InvSqrt( (fCos2 * fCos2) + (fSin2 * fSin2) );
        fCos2 *= fInvLength;
        fSin2 *= fInvLength;
        fCos0 = qOrientation.W * fCos2 + qOrientation.Z * fSin2;
        fSin0 = qOrientation.X * fCos2 + qOrientation.Y * fSin2;
        fInvLength = MathRealFn->InvSqrt( (fCos0 * fCos0) + (fSin0 * fSin0) );
        fCos0 *= fInvLength;
        fSin0 *= fInvLength;

        if ( constraintZ.IsValid(fCos2, fSin2) && constraintX.IsValid(fCos0, fSin0) ) {
            outClosest.X = fCos2 * fSin0;
            outClosest.Y = fSin2 * fSin0;
            outClosest.Z = fSin2 * fCos0;
            outClosest.W = fCos2 * fCos0;
        } else {
            TQuaternion<Real> qR(ZERO, ZERO, constraintZ.fSinMin, constraintZ.fCosMin);
            TQuaternion<Real> qInvR(ZERO, ZERO, -(constraintZ.fSinMin), constraintZ.fCosMin);
            TQuaternion<Real> qProd = (qInvR * qOrientation);
            _SolveClosest( qTmp, qProd, AXIS_X, constraintX );
            Real fDot = qProd.Dot(qTmp);
            outClosest = (qR * qTmp);

            qR.Z = constraintZ.fSinMax;
            qR.W = constraintZ.fCosMax;
            qInvR.Z = -(constraintZ.fSinMax);
            qInvR.W = constraintZ.fCosMax;
            qProd = (qInvR * qOrientation);
            _SolveClosest( qTmp, qProd, AXIS_X, constraintX );
            Real fTmpDot = qProd.Dot(qTmp);
            if (fTmpDot > fDot) {
                outClosest = (qR * qTmp);
                fDot = fTmpDot;
            }

            qR.Z = ZERO;
            qInvR.Z = ZERO;

            qR.X = constraintX.fSinMin;
            qR.W = constraintX.fCosMin;
            qInvR.X = -(constraintX.fSinMin);
            qInvR.W = constraintX.fCosMin;
            qProd = (qOrientation * qInvR);
            _SolveClosest( qTmp, qProd, AXIS_Z, constraintZ );
            fTmpDot = qProd.Dot(qTmp);
            if (fTmpDot > fDot) {
                outClosest = (qTmp * qR);
                fDot = fTmpDot;
            }

            qR.X = constraintX.fSinMax;
            qR.W = constraintX.fCosMax;
            qInvR.X = -(constraintX.fSinMax);
            qInvR.W = constraintX.fCosMax;
            qProd = (qOrientation * qInvR);
            _SolveClosest( qTmp, qProd, AXIS_Z, constraintZ );
            fTmpDot = qProd.Dot(qTmp);
            if (fTmpDot > fDot) {
                outClosest = (qTmp * qR);
                fDot = fTmpDot;
            }
        }
    } else {
        Real fMin, fMax, fAngle;
        AngleConstraint constraintTmp;
        if ( fDet > ZERO ) {
            fMin = constraintX.fMinAngle - constraintZ.fMaxAngle;
            fMax = constraintX.fMaxAngle - constraintZ.fMinAngle;
            constraintTmp.SetValues( fMin, fMax );
            _SolveClosest( qTmp, qOrientation, AXIS_X, constraintTmp );

            fAngle = MathRealFn->ArcTan2( qTmp.X, qTmp.W );
            if (fAngle < fMin || fAngle > fMax)
                fAngle -= (Real)( (qTmp.X >= ZERO) ? SCALAR_PI : (-SCALAR_PI) );

            if ( fAngle <= (constraintX.fMaxAngle - constraintZ.fMaxAngle) ) {
                fCos2 = constraintZ.fCosMax;
                fSin2 = constraintZ.fSinMax;
                fAngle = constraintZ.fMaxAngle + fAngle;
                MathRealFn->SinCos( &fSin0, &fCos0, fAngle );
            } else {
                fCos0 = constraintX.fCosMax;
                fSin0 = constraintX.fSinMax;
                fAngle = constraintX.fMaxAngle - fAngle;
                MathRealFn->SinCos( &fSin2, &fCos2, fAngle );
            }
        } else {
            fMin = constraintX.fMinAngle + constraintZ.fMinAngle;
            fMax = constraintX.fMaxAngle + constraintZ.fMaxAngle;
            constraintTmp.SetValues( fMin, fMax );
            _SolveClosest( qTmp, qOrientation, AXIS_X, constraintTmp );

            fAngle = MathRealFn->ArcTan2( qTmp.X, qTmp.W );
            if (fAngle < fMin || fAngle > fMax)
                fAngle -= (Real)( (qTmp.X >= ZERO) ? SCALAR_PI : (-SCALAR_PI) );

            if ( fAngle >= (constraintX.fMinAngle + constraintZ.fMaxAngle) ) {
                fCos2 = constraintZ.fCosMax;
                fSin2 = constraintZ.fSinMax;
                fAngle = fAngle - constraintZ.fMaxAngle;
                MathRealFn->SinCos( &fSin0, &fCos0, fAngle );
            } else {
                fCos0 = constraintX.fCosMax;
                fSin0 = constraintX.fSinMax;
                fAngle = fAngle - constraintX.fMaxAngle;
                MathRealFn->SinCos( &fSin2, &fCos2, fAngle );
            }
        }
        outClosest.X = fCos2 * fSin0;
        outClosest.Y = fSin2 * fSin0;
        outClosest.Z = fSin2 * fCos0;
        outClosest.W = fCos2 * fCos0;
        if ( qOrientation.Dot(outClosest) < ZERO )
            outClosest = -outClosest;
    }
}

template<typename Real>
Void TConstrainedQuaternion<Real>::_SolveClosestZY( TQuaternion<Real> & outClosest, const TQuaternion<Real> & qOrientation,
                                                    const AngleConstraint & constraintZ, const AngleConstraint & constraintY ) const
{
    Real fCos1, fSin1, fCos2, fSin2, fInvLength;
    TQuaternion<Real> qTmp;

    Real fDet = ( qOrientation.W * qOrientation.X + qOrientation.Y * qOrientation.Z );
    if ( MathRealFn->Abs(fDet) < (HALF - ZeroTolerance) ) {
        Real fDiscr = MathRealFn->Abs( ONE - fDet * fDet * (Real)4 );
        fDiscr = MathRealFn->Sqrt(fDiscr);
        Real fA = ( qOrientation.W * qOrientation.Z - qOrientation.X * qOrientation.Y );
        Real fB = ( qOrientation.W * qOrientation.W - qOrientation.X * qOrientation.X )
                + ( qOrientation.Y * qOrientation.Y - qOrientation.Z * qOrientation.Z );
        if ( fB >= ZERO ) {
            fCos2 = HALF * (fDiscr + fB);
            fSin2 = fA;
        } else {
            fCos2 = fA;
            fSin2 = HALF * (fDiscr - fB);
        }
        fInvLength = MathRealFn->InvSqrt( (fCos2 * fCos2) + (fSin2 * fSin2) );
        fCos2 *= fInvLength;
        fSin2 *= fInvLength;
        fCos1 = qOrientation.W * fCos2 + qOrientation.Z * fSin2;
        fSin1 = qOrientation.Y * fCos2 - qOrientation.X * fSin2;
        fInvLength = MathRealFn->InvSqrt( (fCos1 * fCos1) + (fSin1 * fSin1) );
        fCos1 *= fInvLength;
        fSin1 *= fInvLength;

        if ( constraintZ.IsValid(fCos2, fSin2) && constraintY.IsValid(fCos1, fSin1) ) {
            outClosest.X = -fSin2 * fSin1;
            outClosest.Y = fCos2 * fSin1;
            outClosest.Z = fSin2 * fCos1;
            outClosest.W = fCos2 * fCos1;
        } else {
            TQuaternion<Real> qR(ZERO, ZERO, constraintZ.fSinMin, constraintZ.fCosMin);
            TQuaternion<Real> qInvR(ZERO, ZERO, -(constraintZ.fSinMin), constraintZ.fCosMin);
            TQuaternion<Real> qProd = (qInvR * qOrientation);
            _SolveClosest( qTmp, qProd, AXIS_Y, constraintY );
            Real fDot = qProd.Dot(qTmp);
            outClosest = (qR * qTmp);

            qR.Z = constraintZ.fSinMax;
            qR.W = constraintZ.fCosMax;
            qInvR.Z = -(constraintZ.fSinMax);
            qInvR.W = constraintZ.fCosMax;
            qProd = (qInvR * qOrientation);
            _SolveClosest( qTmp, qProd, AXIS_Y, constraintY );
            Real fTmpDot = qProd.Dot(qTmp);
            if (fTmpDot > fDot) {
                outClosest = (qR * qTmp);
                fDot = fTmpDot;
            }

            qR.Z = ZERO;
            qInvR.Z = ZERO;

            qR.Y = constraintY.fSinMin;
            qR.W = constraintY.fCosMin;
            qInvR.Y = -(constraintY.fSinMin);
            qInvR.W = constraintY.fCosMin;
            qProd = (qOrientation * qInvR);
            _SolveClosest( qTmp, qProd, AXIS_Z, constraintZ );
            fTmpDot = qProd.Dot(qTmp);
            if (fTmpDot > fDot) {
                outClosest = (qTmp * qR);
                fDot = fTmpDot;
            }

            qR.Y = constraintY.fSinMax;
            qR.W = constraintY.fCosMax;
            qInvR.Y = -(constraintY.fSinMax);
            qInvR.W = constraintY.fCosMax;
            qProd = (qOrientation * qInvR);
            _SolveClosest( qTmp, qProd, AXIS_Z, constraintZ );
            fTmpDot = qProd.Dot(qTmp);
            if (fTmpDot > fDot) {
                outClosest = (qTmp * qR);
                fDot = fTmpDot;
            }
        }
    } else {
        Real fMin, fMax, fAngle;
        AngleConstraint constraintTmp;
        if ( fDet < ZERO ) {
            fMin = constraintY.fMinAngle - constraintZ.fMaxAngle;
            fMax = constraintY.fMaxAngle - constraintZ.fMinAngle;
            constraintTmp.SetValues( fMin, fMax );
            _SolveClosest( qTmp, qOrientation, AXIS_Y, constraintTmp );

            fAngle = MathRealFn->ArcTan2( qTmp.Y, qTmp.W );
            if (fAngle < fMin || fAngle > fMax)
                fAngle -= (Real)( (qTmp.Y >= ZERO) ? SCALAR_PI : (-SCALAR_PI) );

            if ( fAngle <= (constraintY.fMaxAngle - constraintZ.fMaxAngle) ) {
                fCos2 = constraintZ.fCosMax;
                fSin2 = constraintZ.fSinMax;
                fAngle = constraintZ.fMaxAngle + fAngle;
                MathRealFn->SinCos( &fSin1, &fCos1, fAngle );
            } else {
                fCos1 = constraintY.fCosMax;
                fSin1 = constraintY.fSinMax;
                fAngle = constraintY.fMaxAngle - fAngle;
                MathRealFn->SinCos( &fSin2, &fCos2, fAngle );
            }
        } else {
            fMin = constraintY.fMinAngle + constraintZ.fMinAngle;
            fMax = constraintY.fMaxAngle + constraintZ.fMaxAngle;
            constraintTmp.SetValues( fMin, fMax );
            _SolveClosest( qTmp, qOrientation, AXIS_Y, constraintTmp );

            fAngle = MathRealFn->ArcTan2( qTmp.Y, qTmp.W );
            if (fAngle < fMin || fAngle > fMax)
                fAngle -= (Real)( (qTmp.Y >= ZERO) ? SCALAR_PI : (-SCALAR_PI) );

            if ( fAngle >= (constraintY.fMinAngle + constraintZ.fMaxAngle) ) {
                fCos2 = constraintZ.fCosMax;
                fSin2 = constraintZ.fSinMax;
                fAngle = fAngle - constraintZ.fMaxAngle;
                MathRealFn->SinCos( &fSin1, &fCos1, fAngle );
            } else {
                fCos1 = constraintY.fCosMax;
                fSin1 = constraintY.fSinMax;
                fAngle = fAngle - constraintY.fMaxAngle;
                MathRealFn->SinCos( &fSin2, &fCos2, fAngle );
            }
        }
        outClosest.X = -fSin2 * fSin1;
        outClosest.Y = fCos2 * fSin1;
        outClosest.Z = fSin2 * fCos1;
        outClosest.W = fCos2 * fCos1;
        if ( qOrientation.Dot(outClosest) < ZERO )
            outClosest = -outClosest;
    }
}

