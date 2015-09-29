/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Algebra/Matrix/SingularDecomposition3.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Singular decomposition for 3x3 matrices
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TSingularDecomposition3 implementation
template<typename Real>
const Real TSingularDecomposition3<Real>::ZERO = (Real)0;
template<typename Real>
const Real TSingularDecomposition3<Real>::ONE = (Real)1;
template<typename Real>
const Real TSingularDecomposition3<Real>::TWO = (Real)2;
template<typename Real>
const Real TSingularDecomposition3<Real>::HALF = (Real)0.5f;

template<typename Real>
TSingularDecomposition3<Real>::TSingularDecomposition3( const TMatrix3<Real> & matNonSymmetric,
                                                        UInt iMaxIterations, Real fZeroTolerance ):
    m_matSystem( matNonSymmetric ), m_matLeft(), m_matDiagonal(), m_matTransposedR()
{
    MaxIterations = iMaxIterations;
    ZeroTolerance = fZeroTolerance;
}
template<typename Real>
TSingularDecomposition3<Real>::~TSingularDecomposition3()
{
    // nothing to do
}

template<typename Real>
inline TMatrix3<Real> & TSingularDecomposition3<Real>::GetSystem() {
    return m_matSystem;
}

template<typename Real>
Void TSingularDecomposition3<Real>::SolveSystem()
{
    TMatrix3<Real> matTrMxM;
    m_matSystem.Transpose( matTrMxM );
    m_matTransposedR = ( matTrMxM * m_matSystem );

    TEigenDecomposition3<Real> eigenSolver( m_matTransposedR, MaxIterations, ZeroTolerance );
    eigenSolver.SolveSystem( false );
    const TMatrix3<Real> & matEigenVectors = eigenSolver.GetVectors();

    m_matDiagonal = ( m_matSystem * matEigenVectors );
    _QRAlgorithm();

    matEigenVectors.Transpose( m_matTransposedR );
}

template<typename Real>
inline const TMatrix3<Real> & TSingularDecomposition3<Real>::GetL() const {
    return m_matLeft;
}
template<typename Real>
inline const TMatrix3<Real> & TSingularDecomposition3<Real>::GetD() const {
    return m_matDiagonal;
}
template<typename Real>
inline const TMatrix3<Real> & TSingularDecomposition3<Real>::GetTrR() const {
    return m_matTransposedR;
}

template<typename Real>
inline Void TSingularDecomposition3<Real>::GetPolarDecomposition( TMatrix3<Real> & outQ, TMatrix3<Real> & outS ) const {
    outQ = ( m_matLeft * m_matTransposedR );
    TMatrix3<Real> matR;
    m_matTransposedR.Transpose( matR );
    outS = ( matR * m_matDiagonal * m_matTransposedR );

    // Numerical correction to ensure S is symmetric
    // S = (S + Tr(S)) / 2
    outS.m01 = HALF * ( outS.m01 + outS.m10 );
    outS.m10 = outS.m01;
    outS.m02 = HALF * ( outS.m02 + outS.m20 );
    outS.m20 = outS.m02;
    outS.m12 = HALF * ( outS.m12 + outS.m21 );
    outS.m21 = outS.m12;
}

/////////////////////////////////////////////////////////////////////////////////

template<typename Real>
Void TSingularDecomposition3<Real>::_HouseHolder_PreMultiply( TMatrix3<Real> & matA, const TVector3<Real> & vFactor ) const
{
    // Compute A = ( Id - 2 * (V * Tr(V)) / (Tr(V) * V) ) * A

    // W = (-2 / V.Norm^2) * (V * A)
    UInt iRow, iCol;
    TVector3<Real> vW;
    vW.X = (vFactor.X * matA.m00) + (vFactor.Y * matA.m10) + (vFactor.Z * matA.m20);
    vW.Y = (vFactor.X * matA.m01) + (vFactor.Y * matA.m11) + (vFactor.Z * matA.m21);
    vW.Z = (vFactor.X * matA.m02) + (vFactor.Y * matA.m12) + (vFactor.Z * matA.m22);
    vW *= ( -TWO / vFactor.NormSqr() );

    // A += Matrix(V x W)
    matA.m00 += (vFactor.X * vW.X); matA.m01 += (vFactor.X * vW.Y); matA.m02 += (vFactor.X * vW.Z);
    matA.m10 += (vFactor.Y * vW.X); matA.m11 += (vFactor.Y * vW.Y); matA.m12 += (vFactor.Y * vW.Z);
    matA.m20 += (vFactor.Z * vW.X); matA.m21 += (vFactor.Z * vW.Y); matA.m22 += (vFactor.Z * vW.Z);
}

template<typename Real>
Void TSingularDecomposition3<Real>::_QRAlgorithm()
{
    // m_matDiagonal contains input matrix MxN and
    // gets diagonalized during HH reduction giving
    // the R matrix.
    // m_matLeft receives the Q matrix.

    // Temp array
    TVector3<Real> arrHHVectors[3];

    // Build R = D
    arrHHVectors[0].X = m_matDiagonal.m00;
    arrHHVectors[0].Y = m_matDiagonal.m10;
    arrHHVectors[0].Z = m_matDiagonal.m20;
    Real fLength = arrHHVectors[0].Norm();
    Real fBeta = ( arrHHVectors[0].X + (MathRealFn->Sign(arrHHVectors[0].X) * fLength) );
    if ( !(MathRealFn->EqualsZero(fBeta)) ) {
        Real fInvBeta = MathRealFn->Invert(fBeta);
        arrHHVectors[0].Y *= fInvBeta;
        arrHHVectors[0].Z *= fInvBeta;
    }
    arrHHVectors[0].X = ONE;
    _HouseHolder_PreMultiply( m_matDiagonal, arrHHVectors[0] );

    arrHHVectors[1].X = ZERO;
    arrHHVectors[1].Y = m_matDiagonal.m11;
    arrHHVectors[1].Z = m_matDiagonal.m21;
    fLength = arrHHVectors[1].Norm();
    fBeta = ( arrHHVectors[1].Y + (MathRealFn->Sign(arrHHVectors[1].Y) * fLength) );
    if ( !(MathRealFn->EqualsZero(fBeta)) )
        arrHHVectors[1].Z /= fBeta;
    arrHHVectors[1].Y = ONE;
    _HouseHolder_PreMultiply( m_matDiagonal, arrHHVectors[1] );

    arrHHVectors[2].X = ZERO;
    arrHHVectors[2].Y = ZERO;
    arrHHVectors[2].Z = ONE;
    _HouseHolder_PreMultiply( m_matDiagonal, arrHHVectors[2] );

    // Build Q = L
    m_matLeft.MakeIdentity();
    _HouseHolder_PreMultiply( m_matLeft, arrHHVectors[2] );
    _HouseHolder_PreMultiply( m_matLeft, arrHHVectors[1] );
    _HouseHolder_PreMultiply( m_matLeft, arrHHVectors[0] );
}
