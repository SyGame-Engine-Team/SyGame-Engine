/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Algebra/Matrix/EigenDecomposition3.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Eigen decomposition for 3x3 matrices
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TEigenDecomposition3 implementation
template<typename Real>
const Real TEigenDecomposition3<Real>::ZERO = (Real)0;
template<typename Real>
const Real TEigenDecomposition3<Real>::ONE = (Real)1;
template<typename Real>
const Real TEigenDecomposition3<Real>::TWO = (Real)2;
template<typename Real>
const Real TEigenDecomposition3<Real>::HALF = (Real)0.5f;
template<typename Real>
const Double TEigenDecomposition3<Real>::INV3 = 1.0 / 3.0;
template<typename Real>
const Double TEigenDecomposition3<Real>::SQRT3 = 1.73205080756887729352; // sqrt(3)

template<typename Real>
TEigenDecomposition3<Real>::TEigenDecomposition3( const TMatrix3<Real> & matSymmetric,
                                                  UInt iMaxIterations, Real fZeroTolerance ):
    m_matSystem( matSymmetric ), m_vDiagonal(), m_vSubDiagonal()
{
    MaxIterations = iMaxIterations;
    ZeroTolerance = fZeroTolerance;

    m_bIsRotation = false;
}
template<typename Real>
TEigenDecomposition3<Real>::~TEigenDecomposition3()
{
    // nothing to do
}

template<typename Real>
inline TMatrix3<Real> & TEigenDecomposition3<Real>::GetSystem() {
    return m_matSystem;
}

template<typename Real>
Void TEigenDecomposition3<Real>::SolveSystem( Bool bSortIncreasing ) const
{
    _TriDiagonalize();

    Bool bConvergent = _QLAlgorithm();
    Assert( bConvergent );

    if ( bSortIncreasing )
        _SortIncreasing();
    else
        _SortDecreasing();

    _EnsureRotation();
}
template<typename Real>
Void TEigenDecomposition3<Real>::QuickSolveSystem() const
{
    // Apply scaling only if at least one entry has
    // magnitude > 1
    Real fMax = MathRealFn->Abs( m_matSystem.m00 );
    Real fAbs = MathRealFn->Abs( m_matSystem.m01 );
    if ( fAbs > fMax )
        fMax = fAbs;
    fAbs = MathRealFn->Abs( m_matSystem.m02 );
    if ( fAbs > fMax )
        fMax = fAbs;
    fAbs = MathRealFn->Abs( m_matSystem.m11 );
    if ( fAbs > fMax )
        fMax = fAbs;
    fAbs = MathRealFn->Abs( m_matSystem.m12 );
    if ( fAbs > fMax )
        fMax = fAbs;
    fAbs = MathRealFn->Abs( m_matSystem.m22 );
    if ( fAbs > fMax )
        fMax = fAbs;
    if ( fMax > ONE )
        m_matSystem *= ( MathRealFn->Invert(fMax) );

    // Compute eigen values
    _ComputeRoots();

    // Check ranks positivity
    TVector3<Real> vMaxEntry;
    TVector3<Real> vMaxRow[3];
    TMatrix3<Real> matM( m_matSystem );
    UInt i;
    for( i = 0; i < 3; ++i ) {
        matM.m00 = (m_matSystem.m00 - m_vDiagonal[i]);
        matM.m11 = (m_matSystem.m11 - m_vDiagonal[i]);
        matM.m22 = (m_matSystem.m22 - m_vDiagonal[i]);
        if ( !(_PositiveRank(vMaxEntry[i], vMaxRow[i], matM)) ) {
            // Scale back
            if ( fMax > ONE ) {
                m_vDiagonal.X *= fMax;
                m_vDiagonal.Y *= fMax;
                m_vDiagonal.Z *= fMax;
            }
            m_matSystem.MakeIdentity();
            return;
        }
    }

    // Compute eigen vectors
    static const s_arrNext[3] = { 1, 2, 0 };
    static const s_arrNext2[3] = { 2, 0, 1 };
    i = 0;
    Real fMaxEntry = vMaxEntry.X;
    if ( vMaxEntry.Y > fMaxEntry ) {
        fMaxEntry = vMaxEntry.Y;
        i = 1;
    }
    if ( vMaxEntry.Z > fMaxEntry )
        i = 2;
    vMaxRow[i].Normalize();
    _ComputeVectors( vMaxRow[i], s_arrNext[i], s_arrNext2[i], i );

    // Scale back
    if ( fMax > ONE ) {
        m_vDiagonal.X *= fMax;
        m_vDiagonal.Y *= fMax;
        m_vDiagonal.Z *= fMax;
    }
}

template<typename Real>
inline Real TEigenDecomposition3<Real>::GetValue( UInt iValue ) const {
    Assert( iValue < 3 );
    return m_vDiagonal[iValue];
}
template<typename Real>
inline const TVector3<Real> & TEigenDecomposition3<Real>::GetValues() const {
    return m_vDiagonal;
}
template<typename Real>
inline Void TEigenDecomposition3<Real>::GetDiagonalMatrix( TMatrix3<Real> & outDiagonal ) const {
    outDiagonal.MakeDiagonal( m_vDiagonal );
}

template<typename Real>
inline Void TEigenDecomposition3<Real>::GetVector( TVector3<Real> & outVector, UInt iVector ) const {
    Assert( iVector < 3 );
    m_matSystem.GetColumn( outVector, iVector );
}
template<typename Real>
inline const TMatrix3<Real> & TEigenDecomposition3<Real>::GetVectors() const {
    return m_matSystem;
}
template<typename Real>
inline Void TEigenDecomposition3<Real>::GetRotationMatrix( TMatrix3<Real> & outRotation ) const {
    outRotation = m_matSystem;
}

/////////////////////////////////////////////////////////////////////////////////

template<typename Real>
Void TEigenDecomposition3<Real>::_TriDiagonalize()
{
    Real m00 = m_matSystem.m00;
    Real m01 = m_matSystem.m01;
    Real m02 = m_matSystem.m02;
    Real m11 = m_matSystem.m11;
    Real m12 = m_matSystem.m12;
    Real m22 = m_matSystem.m22;

    m_vDiagonal.X = m00;
    if ( MathRealFn->Abs(m02) >= ZeroTolerance ) {
        m_vSubDiagonal.X = MathRealFn->Sqrt( (m01 * m01) + (m02 * m02) );
        Real fInvLength = MathRealFn->Invert( m_vSubDiagonal.X );
        m01 *= fInvLength;
        m02 *= fInvLength;
        Real fTmp = ( (TWO * m01 * m12) + (m02 * (m22 - m11)) );
        m_vDiagonal.Y = ( m11 + (m02 * fTmp) );
        m_vDiagonal.Z = ( m22 - (m02 * fTmp) );
        m_vSubDiagonal.Y = ( m12 - (m01 * fTmp) );
        m_matSystem.m00 = ONE;  m_matSystem.m01 = ZERO; m_matSystem.m02 = ZERO;
        m_matSystem.m10 = ZERO; m_matSystem.m11 = m01;  m_matSystem.m12 = m02;
        m_matSystem.m20 = ZERO; m_matSystem.m21 = m02;  m_matSystem.m22 = -m01;
        m_bIsRotation = false;
    } else {
        m_vDiagonal.Y = m11;
        m_vDiagonal.Z = m22;
        m_vSubDiagonal.X = m01;
        m_vSubDiagonal.Y = m12;
        m_matSystem.MakeIdentity();
        m_bIsRotation = true;
    }
}

template<typename Real>
Bool TEigenDecomposition3<Real>::_QLAlgorithm()
{
    Real fSum, fDiff, fDiscr;
    Real fEigen0, fEigen1;
    Real fCos, fSin, fTmp;
    Int iRow;
    for( UInt i = 0; i < MaxIterations; ++i ) {
        // Direct 2x2 lower block reduction
        fSum = ( MathRealFn->Abs(m_vDiagonal.X) + MathRealFn->Abs(m_vDiagonal.Y) );
        if ( MathRealFn->Abs(m_vSubDiagonal.X) + fSum == fSum ) {
            // we have M = ( d0  0  0
            //               0  d1 s1
            //               0  s1 d2 )

            // Test if M is diagonal
            fSum = ( MathRealFn->Abs(m_vDiagonal.Y) + MathRealFn->Abs(m_vDiagonal.Z) );
            if ( MathRealFn->Abs(m_vSubDiagonal.Y) + fSum == fSum )
                return true;

            // Eigen values
            fSum = m_vDiagonal.Y + m_vDiagonal.Z;
            fDiff = m_vDiagonal.Y - m_vDiagonal.Z;
            fDiscr = MathRealFn->Sqrt( (fDiff * fDiff) + (m_vSubDiagonal.Y * m_vSubDiagonal.Y * (Real)4) );
            fEigen0 = HALF * (fSum - fDiscr);
            fEigen1 = HALF * (fSum + fDiscr); // pre-sorted

            // Givens rotation
            if ( fDiff >= ZERO ) {
                fCos = m_vSubDiagonal.Y;
                fSin = m_vDiagonal.Y - fEigen0;
            } else {
                fCos = m_vDiagonal.Z - fEigen0;
                fSin = m_vSubDiagonal.Y;
            }
            fTmp = MathRealFn->InvSqrt( (fCos * fCos) + (fSin * fSin) );
            fCos *= fTmp;
            fSin *= fTmp;

            // Post-multiply
            fTmp = m_matSystem.m02;
            m_matSystem.m02 = ( (fSin * m_matSystem.m01) + (fCos * fTmp) );
            m_matSystem.m01 = ( (fCos * m_matSystem.m01) - (fSin * fTmp) );
            fTmp = m_matSystem.m12;
            m_matSystem.m12 = ( (fSin * m_matSystem.m11) + (fCos * fTmp) );
            m_matSystem.m11 = ( (fCos * m_matSystem.m11) - (fSin * fTmp) );
            fTmp = m_matSystem.m22;
            m_matSystem.m22 = ( (fSin * m_matSystem.m21) + (fCos * fTmp) );
            m_matSystem.m21 = ( (fCos * m_matSystem.m21) - (fSin * fTmp) );

            // Update tri-diagonal matrix
            m_vDiagonal.Y = fEigen0;
            m_vDiagonal.Z = fEigen1;
            m_vSubDiagonal.X = ZERO;
            m_vSubDiagonal.Y = ZERO;
            return true;
        }

        // Direct 2x2 upper block reduction
        fSum = MathRealFn->Abs(m_vDiagonal.Y) + MathRealFn->Abs(m_vDiagonal.Z);
        if ( MathRealFn->Abs(m_vSubDiagonal.Y) + fSum == fSum ) {
            // we have M = ( d0 s0  0
            //               s0 d1  0
            //               0   0 d2 )

            // Test if M is diagonal
            fSum = ( MathRealFn->Abs(m_vDiagonal.X) + MathRealFn->Abs(m_vDiagonal.Y) );
            if ( MathRealFn->Abs(m_vSubDiagonal.X) + fSum == fSum )
                return true;

            // Eigen values
            fSum = m_vDiagonal.X + m_vDiagonal.Y;
            fDiff = m_vDiagonal.X - m_vDiagonal.Y;
            fDiscr = MathRealFn->Sqrt( (fDiff * fDiff) + (m_vSubDiagonal.X * m_vSubDiagonal.X * (Real)4) );
            fEigen0 = HALF * (fSum - fDiscr);
            fEigen1 = HALF * (fSum + fDiscr); // pre-sorted

            // Givens rotation
            if ( fDiff >= ZERO ) {
                fCos = m_vSubDiagonal.X;
                fSin = m_vDiagonal.X - fEigen0;
            } else {
                fCos = m_vDiagonal.Y - fEigen0;
                fSin = m_vSubDiagonal.X;
            }
            fTmp = MathRealFn->InvSqrt( (fCos * fCos) + (fSin * fSin) );
            fCos *= fTmp;
            fSin *= fTmp;

            // Post-multiply
            fTmp = m_matSystem.m01;
            m_matSystem.m01 = ( (fSin * m_matSystem.m00) + (fCos * fTmp) );
            m_matSystem.m00 = ( (fCos * m_matSystem.m00) - (fSin * fTmp) );
            fTmp = m_matSystem.m11;
            m_matSystem.m11 = ( (fSin * m_matSystem.m10) + (fCos * fTmp) );
            m_matSystem.m10 = ( (fCos * m_matSystem.m10) - (fSin * fTmp) );
            fTmp = m_matSystem.m21;
            m_matSystem.m21 = ( (fSin * m_matSystem.m20) + (fCos * fTmp) );
            m_matSystem.m20 = ( (fCos * m_matSystem.m20) - (fSin * fTmp) );

            // update tri-diagonal matrix
            m_vDiagonal.X = fEigen0;
            m_vDiagonal.Y = fEigen1;
            m_vSubDiagonal.X = ZERO;
            m_vSubDiagonal.Y = ZERO;
            return true;
        }

        // we have M = ( d0 s0  0
        //               s0 d1 s1
        //               0  s1 d2 )

        // FIRST PASS
            // Parameters
        Real fRatio = ( m_vDiagonal.Y - m_vDiagonal.X ) / ( TWO * m_vSubDiagonal.X );
        Real fRoot = MathRealFn->Sqrt( (fRatio * fRatio) + ONE );
        Real fB = m_vSubDiagonal.Y;
        Real fA = m_vDiagonal.Z - m_vDiagonal.X;
        if ( fRatio >= ZERO )
            fA += m_vSubDiagonal.X / (fRatio + fRoot);
        else
            fA += m_vSubDiagonal.X / (fRatio - fRoot);

            // Givens rotation
        if ( MathRealFn->Abs(fB) >= MathRealFn->Abs(fA) ) {
            fRatio = fA / fB;
            fSin = MathRealFn->InvSqrt( (fRatio * fRatio) + ONE );
            fCos = fRatio * fSin;
        } else {
            fRatio = fB / fA;
            fCos = MathRealFn->InvSqrt( (fRatio * fRatio) + ONE );
            fSin = fRatio * fCos;
        }

            // Post-multiply
        fTmp = m_matSystem.m02;
        m_matSystem.m02 = ( (fSin * m_matSystem.m01) + (fCos * fTmp) );
        m_matSystem.m01 = ( (fCos * m_matSystem.m01) - (fSin * fTmp) );
        fTmp = m_matSystem.m12;
        m_matSystem.m12 = ( (fSin * m_matSystem.m11) + (fCos * fTmp) );
        m_matSystem.m11 = ( (fCos * m_matSystem.m11) - (fSin * fTmp) );
        fTmp = m_matSystem.m22;
        m_matSystem.m22 = ( (fSin * m_matSystem.m21) + (fCos * fTmp) );
        m_matSystem.m21 = ( (fCos * m_matSystem.m21) - (fSin * fTmp) );

        // SECOND PASS
            // Parameters
        Real fTmp0 = (m_vDiagonal.Y - m_vDiagonal.Z) * fSin + (TWO * m_vSubDiagonal.Y * fCos);
        Real fTmp1 = fCos * m_vSubDiagonal.X;
        fB = fSin * m_vSubDiagonal.X;
        fA = (fCos * fTmp0) - m_vSubDiagonal.Y;
        fTmp0 *= fSin;

            // Givens rotation & update SubDiag
        if ( MathRealFn->Abs(fB) >= MathRealFn->Abs(fA) ) {
            fRatio = fA / fB;
            fRoot = MathRealFn->Sqrt( (fRatio * fRatio) + ONE );
            fSin = MathRealFn->Invert(fRoot);
            fCos = fRatio * fSin;
            m_vSubDiagonal.Y = fB * fRoot;
        } else {
            fRatio = fB / fA;
            fRoot = MathRealFn->Sqrt( (fRatio * fRatio) + ONE );
            fCos = MathRealFn->Invert(fRoot);
            fSin = fRatio * fCos;
            m_vSubDiagonal.Y = fA * fRoot;
        }

            // Post-multiply
        fTmp = m_matSystem.m01;
        m_matSystem.m01 = ( (fSin * m_matSystem.m00) + (fCos * fTmp) );
        m_matSystem.m00 = ( (fCos * m_matSystem.m00) - (fSin * fTmp) );
        fTmp = m_matSystem.m11;
        m_matSystem.m11 = ( (fSin * m_matSystem.m10) + (fCos * fTmp) );
        m_matSystem.m10 = ( (fCos * m_matSystem.m10) - (fSin * fTmp) );
        fTmp = m_matSystem.m21;
        m_matSystem.m21 = ( (fSin * m_matSystem.m20) + (fCos * fTmp) );
        m_matSystem.m20 = ( (fCos * m_matSystem.m20) - (fSin * fTmp) );

        // UPDATE
        Real fTmp2 = m_vDiagonal.Y - fTmp0;
        m_vDiagonal.Z += fTmp0;
        fTmp0 = ( ((m_vDiagonal.X - fTmp2) * fSin) + (TWO * fTmp1 * fCos) );
        m_vSubDiagonal.X = (fCos * fTmp0) - fTmp1;
        fTmp0 *= fSin;
        m_vDiagonal.Y = fTmp2 + fTmp0;
        m_vDiagonal.X -= fTmp0;
    }

    return false;
}

template<typename Real>
Void TEigenDecomposition3<Real>::_SortIncreasing()
{
    if ( m_vDiagonal.Y < m_vDiagonal.X ) {
        // swap d0, d1 and v0, v1
        Swap<Real>( &(m_vDiagonal.X), &(m_vDiagonal.Y) );
        Swap<Real>( &(m_matSystem.m00), &(m_matSystem.m01) );
        Swap<Real>( &(m_matSystem.m10), &(m_matSystem.m11) );
        Swap<Real>( &(m_matSystem.m20), &(m_matSystem.m21) );
        m_bIsRotation = !m_bIsRotation;
    }
    if ( m_vDiagonal.Z < m_vDiagonal.Y ) {
        // swap d1, d2 and v1, v2
        Swap<Real>( &(m_vDiagonal.Y), &(m_vDiagonal.Z) );
        Swap<Real>( &(m_matSystem.m01), &(m_matSystem.m02) );
        Swap<Real>( &(m_matSystem.m11), &(m_matSystem.m12) );
        Swap<Real>( &(m_matSystem.m21), &(m_matSystem.m22) );
        m_bIsRotation = !m_bIsRotation;
    }
    if ( m_vDiagonal.Y < m_vDiagonal.X ) {
        // swap d0, d1 and v0, v1
        Swap<Real>( &(m_vDiagonal.X), &(m_vDiagonal.Y) );
        Swap<Real>( &(m_matSystem.m00), &(m_matSystem.m01) );
        Swap<Real>( &(m_matSystem.m10), &(m_matSystem.m11) );
        Swap<Real>( &(m_matSystem.m20), &(m_matSystem.m21) );
        m_bIsRotation = !m_bIsRotation;
    }
}
template<typename Real>
Void TEigenDecomposition3<Real>::_SortDecreasing()
{
    if ( m_vDiagonal.Y > m_vDiagonal.X ) {
        // swap d0, d1 and v0, v1
        Swap<Real>( &(m_vDiagonal.X), &(m_vDiagonal.Y) );
        Swap<Real>( &(m_matSystem.m00), &(m_matSystem.m01) );
        Swap<Real>( &(m_matSystem.m10), &(m_matSystem.m11) );
        Swap<Real>( &(m_matSystem.m20), &(m_matSystem.m21) );
        m_bIsRotation = !m_bIsRotation;
    }
    if ( m_vDiagonal.Z > m_vDiagonal.Y ) {
        // swap d1, d2 and v1, v2
        Swap<Real>( &(m_vDiagonal.Y), &(m_vDiagonal.Z) );
        Swap<Real>( &(m_matSystem.m01), &(m_matSystem.m02) );
        Swap<Real>( &(m_matSystem.m11), &(m_matSystem.m12) );
        Swap<Real>( &(m_matSystem.m21), &(m_matSystem.m22) );
        m_bIsRotation = !m_bIsRotation;
    }
    if ( m_vDiagonal.Y > m_vDiagonal.X ) {
        // swap d0, d1 and v0, v1
        Swap<Real>( &(m_vDiagonal.X), &(m_vDiagonal.Y) );
        Swap<Real>( &(m_matSystem.m00), &(m_matSystem.m01) );
        Swap<Real>( &(m_matSystem.m10), &(m_matSystem.m11) );
        Swap<Real>( &(m_matSystem.m20), &(m_matSystem.m21) );
        m_bIsRotation = !m_bIsRotation;
    }
}

template<typename Real>
Void TEigenDecomposition3<Real>::_EnsureRotation()
{
    if ( !m_bIsRotation ) {
        // Change sign on the first column
        m_matSystem.m00 = -(m_matSystem.m00);
        m_matSystem.m10 = -(m_matSystem.m10);
        m_matSystem.m20 = -(m_matSystem.m20);

        m_bIsRotation = true;
    }
}

template<typename Real>
Bool TEigenDecomposition3<Real>::_PositiveRank( Real & outMaxEntry, TVector3<Real> & outMaxRow, const TMatrix3<Real> & matM ) const
{
    outMaxEntry = (Real)(-1);
    UInt iMaxRow = 0;
    Real fAbs = MathRealFn->Abs( matM.m00 );
    if ( fAbs > outMaxEntry )
        outMaxEntry = fAbs;
    fAbs = MathRealFn->Abs( matM.m01 );
    if ( fAbs > outMaxEntry )
        outMaxEntry = fAbs;
    fAbs = MathRealFn->Abs( matM.m02 );
    if ( fAbs > outMaxEntry )
        outMaxEntry = fAbs;
    fAbs = MathRealFn->Abs( matM.m11 );
    if ( fAbs > outMaxEntry ) {
        outMaxEntry = fAbs;
        iMaxRow = 1;
    }
    fAbs = MathRealFn->Abs( matM.m12 );
    if ( fAbs > outMaxEntry ) {
        outMaxEntry = fAbs;
        iMaxRow = 1;
    }
    fAbs = MathRealFn->Abs( matM.m22 );
    if ( fAbs > outMaxEntry ) {
        outMaxEntry = fAbs;
        iMaxRow = 2;
    }
    matM.GetRow(outMaxRow, iMaxRow);
    return ( outMaxEntry >= ZeroTolerance );
}
template<typename Real>
Void TEigenDecomposition3<Real>::_ComputeRoots()
{
    // Switch to double precision
    Double m00 = (Double)( m_matSystem.m00 );
    Double m01 = (Double)( m_matSystem.m01 );
    Double m02 = (Double)( m_matSystem.m02 );
    Double m11 = (Double)( m_matSystem.m11 );
    Double m12 = (Double)( m_matSystem.m12 );
    Double m22 = (Double)( m_matSystem.m22 );

    // Characteristic equation is :
    //     x^3 - c2 * x^2 + c1 * x - c0 = 0
    // c0 = Determinant of A
    // c1 = Sum of the determinants of diagonal minors of A
    // c2 = Trace of A
    Double c0 = ( (m00 * m11 * m22) + TWO * (m01 * m02 * m12) -
                  (m00 * m12 * m12) - (m11 * m02 * m02) - (m22 * m01 * m01) );
    Double c1 = ( (m00 * m11 - m01 * m01) +
                  (m00 * m22 - m02 * m02) +
                  (m11 * m22 - m12 * m12) );
    Double c2 = ( m00 + m11 + m22 );

    // Parameters for roots classification and closed-form solving
    Double fC2Div3 = c2 * INV3;
    Double fADiv3 = ( c1 - (c2 * fC2Div3) ) * INV3;
    if ( fADiv3 > 0.0 )
        fADiv3 = 0.0;
    Double fHalfMB = HALF * ( c0 + fC2Div3 * (TWO * fC2Div3 * fC2Div3 - c1) );
    Double fQ = (fHalfMB * fHalfMB) + (fADiv3 * fADiv3 * fADiv3);
    if ( fQ > 0.0 )
        fQ = 0.0;

    // Solve polynomial
    Double fMagnitude = MathDFn->Sqrt(-fADiv3);
    Double fAngle = INV3 * MathDFn->ArcTan2( MathDFn->Sqrt(-fQ), fHalfMB );
    Double fSin, fCos;
    MathDFn->SinCos( &fSin, &fCos, fAngle );
    Double fRoot0 = fC2Div3 + ( fMagnitude * TWO * fCos );
    Double fRoot1 = fC2Div3 - ( fMagnitude * ( fCos + (SQRT3 * fSin) ) );
    Double fRoot2 = fC2Div3 - ( fMagnitude * ( fCos - (SQRT3 * fSin) ) );

    // Sort roots in crescent order
    Double fMinRoot, fIntRoot, fMaxRoot;
    if ( fRoot1 >= fRoot0 ) {
        fMinRoot = fRoot0;
        fIntRoot = fRoot1;
    } else {
        fMinRoot = fRoot1;
        fIntRoot = fRoot0;
    }
    if ( fRoot2 >= fIntRoot )
        fMaxRoot = fRoot2;
    else {
        fMaxRoot = fIntRoot;
        if ( fRoot2 >= fMinRoot )
            fIntRoot = fRoot2;
        else {
            fIntRoot = fMinRoot;
            fMinRoot = fRoot2;
        }
    }

    // Switch back to real
    m_vDiagonal.X = (Real)fMinRoot;
    m_vDiagonal.Y = (Real)fIntRoot;
    m_vDiagonal.Z = (Real)fMaxRoot;
}
template<typename Real>
Void TEigenDecomposition3<Real>::_ComputeVectors( const TVector3<Real> & vU2, UInt i0, UInt i1, UInt i2 )
{
    TVector3<Real> outVectors[3];
    TVector3<Real> vU0, vU1;
    TVector3<Real>::MakeComplementBasis( vU0, vU1, vU2 );

    // Compute vector for i2
    TVector3<Real> vTmp = ( m_matSystem * vU0 );
    Real p00 = m_vDiagonal[i2] - (vU0 * vTmp);
    Real p01 = (vU1 * vTmp);
    Real p11 = m_vDiagonal[i2] - ( vU1 * (m_matSystem * vU1) );

    UInt i = 0;
    Real fMax = MathRealFn->Abs(p00);
    Real fAbs = MathRealFn->Abs(p01);
    if ( fAbs > fMax )
        fMax = fAbs;
    fAbs = MathRealFn->Abs(p11);
    if ( fAbs > fMax ) {
        fMax = fAbs;
        i = 1;
    }

    Real fInvLength;
    if ( fMax >= ZeroTolerance ) {
        if ( i == 0 ) {
            fInvLength = MathRealFn->InvSqrt( (p00 * p00) + (p01 * p01) );
            p00 *= fInvLength;
            p01 *= fInvLength;
            outVectors[i2] = ( (vU0 * p01) + (vU1 * p00) );
        } else { // ( i == 1 )
            fInvLength = MathRealFn->InvSqrt( (p11 * p11) + (p01 * p01) );
            p11 *= fInvLength;
            p01 *= fInvLength;
            outVectors[i2] = ( (vU0 * p11) + (vU1 * p01) );
        }
    } else {
        if ( i == 0 )
            outVectors[i2] = vU1;
        else // ( i == 1 )
            outVectors[i2] = vU0;
    }

    // Compute vector for i0
    TVector3<Real> vS = ( vU2 ^ outVectors[i2] );
    vTmp = ( m_matSystem * vU2 );
    p00 = m_vDiagonal[i0] - (vU2 * vTmp);
    p01 = (vS * vTmp);
    p11 = m_vDiagonal[i0] - ( vS * (m_matSystem * vS) );

    i = 0;
    fMax = MathRealFn->Abs(p00);
    fAbs = MathRealFn->Abs(p01);
    if ( fAbs > fMax )
        fMax = fAbs;
    fAbs = MathRealFn->Abs(p11);
    if ( fAbs > fMax ) {
        fMax = fAbs;
        i = 1;
    }

    if ( fMax >= ZeroTolerance ) {
        if ( i == 0 ) {
            fInvLength = MathRealFn->InvSqrt( (p00 * p00) + (p01 * p01) );
            p00 *= fInvLength;
            p01 *= fInvLength;
            outVectors[i0] = ( (vU2 * p01) + (vS * p00) );
        } else { // ( i == 1 )
            fInvLength = MathRealFn->InvSqrt( (p11 * p11) + (p01 * p01) );
            p11 *= fInvLength;
            p01 *= fInvLength;
            outVectors[i0] = ( (vU2 * p11) + (vS * p01) );
        }
    } else {
        if ( i == 0 )
            outVectors[i0] = vS;
        else // ( i == 1 )
            outVectors[i0] = vU2;
    }

    // Compute vector for i1
    outVectors[i1] = ( outVectors[i2] ^ outVectors[i0] );

    // Save
    m_matSystem.MakeBasis( outVectors[i0], outVectors[i1], outVectors[i2] );
}


