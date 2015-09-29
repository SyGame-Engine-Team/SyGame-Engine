/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Algebra/Matrix/EigenDecomposition2.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Eigen decomposition for 2x2 matrices
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TEigenDecomposition2 implementation
template<typename Real>
const Real TEigenDecomposition2<Real>::ZERO = (Real)0;
template<typename Real>
const Real TEigenDecomposition2<Real>::ONE = (Real)1;
template<typename Real>
const Real TEigenDecomposition2<Real>::TWO = (Real)2;
template<typename Real>
const Real TEigenDecomposition2<Real>::HALF = (Real)0.5f;

template<typename Real>
TEigenDecomposition2<Real>::TEigenDecomposition2( const TMatrix2<Real> & matSymmetric,
                                                  UInt iMaxIterations, Real fZeroTolerance ):
    m_matSystem( matSymmetric ), m_vDiagonal()
{
    MaxIterations = iMaxIterations;
    ZeroTolerance = fZeroTolerance;

    m_bIsRotation = false;
}
template<typename Real>
TEigenDecomposition2<Real>::~TEigenDecomposition2()
{
    // nothing to do
}

template<typename Real>
inline TMatrix2<Real> & TEigenDecomposition2<Real>::GetSystem() {
    return m_matSystem;
}

template<typename Real>
Void TEigenDecomposition2<Real>::SolveSystem( Bool bSortIncreasing ) const
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
Void TEigenDecomposition2<Real>::QuickSolveSystem() const
{
    // Apply scaling only if at least one entry has
    // magnitude > 1
    Real fMax = MathRealFn->Abs( m_matSystem.m00 );
    Real fAbs = MathRealFn->Abs( m_matSystem.m01 );
    if ( fAbs > fMax )
        fMax = fAbs;
    fAbs = MathRealFn->Abs( m_matSystem.m11 );
    if ( fAbs > fMax )
        fMax = fAbs;
    if ( fMax > ONE )
        m_matSystem *= ( MathRealFn->Invert(fMax) );

    // Compute eigen values
    _ComputeRoots();

    // Check ranks positivity
    TVector2<Real> vMaxEntry;
    TVector2<Real> vMaxRow[2];
    TMatrix2<Real> matM( m_matSystem );
    UInt i;
    for( i = 0; i < 2; ++i ) {
        matM.m00 = (m_matSystem.m00 - m_vDiagonal[i]);
        matM.m11 = (m_matSystem.m11 - m_vDiagonal[i]);
        if ( !(_PositiveRank(vMaxEntry[i], vMaxRow[i], matM)) ) {
            // Scale back
            if ( fMax > ONE ) {
                m_vDiagonal.X *= fMax;
                m_vDiagonal.Y *= fMax;
            }
            m_matSystem.MakeIdentity();
            return;
        }
    }

    // Compute eigen vectors
    i = 0;
    Real fMaxEntry = vMaxEntry.X;
    if ( vMaxEntry.Y > fMaxEntry ) {
        fMaxEntry = vMaxEntry.Y;
        i = 1;
    }
    vMaxRow[i].Normalize();
    _ComputeVectors( vMaxRow[i], (1 - i), i );

    // Scale back
    if ( fMax > ONE ) {
        m_vDiagonal.X *= fMax;
        m_vDiagonal.Y *= fMax;
    }
}

template<typename Real>
inline Real TEigenDecomposition2<Real>::GetValue( UInt iValue ) const {
    Assert( iValue < 2 );
    return m_vDiagonal[iValue];
}
template<typename Real>
inline const TVector2<Real> & TEigenDecomposition2<Real>::GetValues() const {
    return m_vDiagonal;
}
template<typename Real>
inline Void TEigenDecomposition2<Real>::GetDiagonalMatrix( TMatrix2<Real> & outDiagonal ) const {
    outDiagonal.MakeDiagonal( m_vDiagonal );
}

template<typename Real>
inline Void TEigenDecomposition2<Real>::GetVector( TVector2<Real> & outVector, UInt iVector ) const {
    Assert( iVector < 2 );
    m_matSystem.GetColumn( outVector, iVector );
}
template<typename Real>
inline const TMatrix2<Real> & TEigenDecomposition2<Real>::GetVectors() const {
    return m_matSystem;
}
template<typename Real>
inline Void TEigenDecomposition2<Real>::GetRotationMatrix( TMatrix2<Real> & outRotation ) const {
    outRotation = m_matSystem;
}

/////////////////////////////////////////////////////////////////////////////////

template<typename Real>
Void TEigenDecomposition2<Real>::_TriDiagonalize()
{
    // A 2x2 matrix is allways tridiagonal
    m_vDiagonal.X = m_matSystem.m00;
    m_vDiagonal.Y = m_matSystem.m11;
    m_fSubDiagonal = m_matSystem.m01;
    m_matSystem.MakeIdentity();
    m_bIsRotation = true;
}

template<typename Real>
Bool TEigenDecomposition2<Real>::_QLAlgorithm()
{
    Real fSum, fDiff, fDiscr;
    Real fEigen0, fEigen1;
    Real fCos, fSin, fTmp;
    Int iRow;
    for( UInt i = 0; i < MaxIterations; ++i ) {
        // we have M = ( d0 s0
        //               s0 d1 )

        // Test if M is diagonal
        fSum = ( MathRealFn->Abs(m_vDiagonal.X) + MathRealFn->Abs(m_vDiagonal.Y) );
        if ( MathRealFn->Abs(m_fSubDiagonal) + fSum == fSum )
            return true;

        // Eigen values
        fSum = m_vDiagonal.X + m_vDiagonal.Y;
        fDiff = m_vDiagonal.X - m_vDiagonal.Y;
        fDiscr = MathRealFn->Sqrt( (fDiff * fDiff) + (m_fSubDiagonal * m_fSubDiagonal * (Real)4) );
        fEigen0 = HALF * (fSum - fDiscr);
        fEigen1 = HALF * (fSum + fDiscr); // pre-sorted

        // Givens rotation
        if ( fDiff >= ZERO ) {
            fCos = m_fSubDiagonal;
            fSin = m_vDiagonal.X - fEigen0;
        } else {
            fCos = m_vDiagonal.Y - fEigen0;
            fSin = m_fSubDiagonal;
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

        // update tri-diagonal matrix
        m_vDiagonal.X = fEigen0;
        m_vDiagonal.Y = fEigen1;
        m_fSubDiagonal = ZERO;
        return true;
    }

    return false;
}

template<typename Real>
Void TEigenDecomposition2<Real>::_SortIncreasing()
{
    if ( m_vDiagonal.Y < m_vDiagonal.X ) {
        // swap d0, d1 and v0, v1
        Swap<Real>( &(m_vDiagonal.X), &(m_vDiagonal.Y) );
        Swap<Real>( &(m_matSystem.m00), &(m_matSystem.m01) );
        Swap<Real>( &(m_matSystem.m10), &(m_matSystem.m11) );
        m_bIsRotation = !m_bIsRotation;
    }
}
template<typename Real>
Void TEigenDecomposition2<Real>::_SortDecreasing()
{
    if ( m_vDiagonal.Y > m_vDiagonal.X ) {
        // swap d0, d1 and v0, v1
        Swap<Real>( &(m_vDiagonal.X), &(m_vDiagonal.Y) );
        Swap<Real>( &(m_matSystem.m00), &(m_matSystem.m01) );
        Swap<Real>( &(m_matSystem.m10), &(m_matSystem.m11) );
        m_bIsRotation = !m_bIsRotation;
    }
}

template<typename Real>
Void TEigenDecomposition2<Real>::_EnsureRotation()
{
    if ( !m_bIsRotation ) {
        // Change sign on the first column
        m_matSystem.m00 = -(m_matSystem.m00);
        m_matSystem.m10 = -(m_matSystem.m10);

        m_bIsRotation = true;
    }
}


template<typename Real>
Bool TEigenDecomposition2<Real>::_PositiveRank( Real & outMaxEntry, TVector2<Real> & outMaxRow, const TMatrix2<Real> & matM ) const
{
    outMaxEntry = (Real)(-1);
    UInt iMaxRow = 0;
    Real fAbs = MathRealFn->Abs( matM.m00 );
    if ( fAbs > outMaxEntry )
        outMaxEntry = fAbs;
    fAbs = MathRealFn->Abs( matM.m01 );
    if ( fAbs > outMaxEntry )
        outMaxEntry = fAbs;
    fAbs = MathRealFn->Abs( matM.m11 );
    if ( fAbs > outMaxEntry ) {
        outMaxEntry = fAbs;
        iMaxRow = 1;
    }
    matM.GetRow(outMaxRow, iMaxRow);
    return ( outMaxEntry >= ZeroTolerance );
}
template<typename Real>
Void TEigenDecomposition2<Real>::_ComputeRoots()
{
    // Switch to double precision
    Double m00 = (Double)( m_matSystem.m00 );
    Double m01 = (Double)( m_matSystem.m01 );
    Double m11 = (Double)( m_matSystem.m11 );

    // Characteristic equation is :
    //     x^2 - c1 * x + c0 = 0
    // c0 = Determinant of A
    // c1 = Trace of A
    Double c0 = ( (m00 * m11) - (m01 * m01) );
    Double c1 = ( m00 + m11 );

    // Solve polynomial
    Double fDiscr = MathDFn->Abs( (c1 * c1) - (c0 * (Real)4) );
    fDiscr = MathDFn->Sqrt( fDiscr );
    Double fRoot0 = HALF * (c1 - fDiscr);
    Double fRoot1 = HALF * (c1 + fDiscr);

    // Sort roots in crescent order
    Double fMinRoot, fMaxRoot;
    if ( fRoot1 >= fRoot0 ) {
        fMinRoot = fRoot0;
        fMaxRoot = fRoot1;
    } else {
        fMinRoot = fRoot1;
        fMaxRoot = fRoot0;
    }

    // Switch back to real
    m_vDiagonal.X = (Real)fMinRoot;
    m_vDiagonal.Y = (Real)fMaxRoot;
}
template<typename Real>
Void TEigenDecomposition2<Real>::_ComputeVectors( const TVector2<Real> & vU1, UInt i0, UInt i1 )
{
    TVector2<Real> outVectors[2];
    TVector2<Real> vU0;
    TVector2<Real>::MakeComplementBasis( vU0, vU1 );

    // Compute vector for i1
    TVector2<Real> vTmp = ( m_matSystem * vU0 );
    Real p00 = m_vDiagonal[i1] - (vU0 * vTmp);
    Real p01 = (vU1 * vTmp);
    Real p11 = m_vDiagonal[i1] - ( vU1 * (m_matSystem * vU1) );

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
            outVectors[i1] = ( (vU0 * p01) + (vU1 * p00) );
        } else { // ( i == 1 )
            fInvLength = MathRealFn->InvSqrt( (p11 * p11) + (p01 * p01) );
            p11 *= fInvLength;
            p01 *= fInvLength;
            outVectors[i1] = ( (vU0 * p11) + (vU1 * p01) );
        }
    } else {
        if ( i == 0 )
            outVectors[i1] = vU1;
        else // ( i == 1 )
            outVectors[i1] = vU0;
    }

    // Compute vector for i0
    outVectors[i0] = ( outVectors[i1].Perp() );

    // Save
    m_matSystem.MakeBasis( outVectors[i0], outVectors[i1] );
}

