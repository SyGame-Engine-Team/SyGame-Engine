/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Algebra/Matrix/SingularDecomposition2.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Singular decomposition for 2x2 matrices
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TSingularDecomposition2 implementation
template<typename Real>
const Real TSingularDecomposition2<Real>::ZERO = (Real)0;
template<typename Real>
const Real TSingularDecomposition2<Real>::ONE = (Real)1;
template<typename Real>
const Real TSingularDecomposition2<Real>::TWO = (Real)2;
template<typename Real>
const Real TSingularDecomposition2<Real>::HALF = (Real)0.5f;

template<typename Real>
TSingularDecomposition2<Real>::TSingularDecomposition2( const TMatrix2<Real> & matNonSymmetric,
                                                        UInt iMaxIterations, Real fZeroTolerance ):
    m_matSystem( matNonSymmetric ), m_matLeft(), m_matDiagonal(), m_matTransposedR()
{
    MaxIterations = iMaxIterations;
    ZeroTolerance = fZeroTolerance;
}
template<typename Real>
TSingularDecomposition2<Real>::~TSingularDecomposition2()
{
    // nothing to do
}

template<typename Real>
inline TMatrix2<Real> & TSingularDecomposition2<Real>::GetSystem() {
    return m_matSystem;
}

template<typename Real>
Void TSingularDecomposition2<Real>::SolveSystem()
{
    TMatrix2<Real> matTrMxM;
    m_matSystem.Transpose( matTrMxM );
    m_matTransposedR = ( matTrMxM * m_matSystem );

    TEigenDecomposition2<Real> eigenSolver( m_matTransposedR, MaxIterations, ZeroTolerance );
    eigenSolver.SolveSystem( false );
    const TMatrix2<Real> & matEigenVectors = eigenSolver.GetVectors();

    m_matDiagonal = ( m_matSystem * matEigenVectors );
    _QRAlgorithm();

    matEigenVectors.Transpose( m_matTransposedR );
}

template<typename Real>
inline const TMatrix2<Real> & TSingularDecomposition2<Real>::GetL() const {
    return m_matLeft;
}
template<typename Real>
inline const TMatrix2<Real> & TSingularDecomposition2<Real>::GetD() const {
    return m_matDiagonal;
}
template<typename Real>
inline const TMatrix2<Real> & TSingularDecomposition2<Real>::GetTrR() const {
    return m_matTransposedR;
}

template<typename Real>
inline Void TSingularDecomposition2<Real>::GetPolarDecomposition( TMatrix2<Real> & outQ, TMatrix2<Real> & outS ) const {
    outQ = ( m_matLeft * m_matTransposedR );
    TMatrix2<Real> matR;
    m_matTransposedR.Transpose( matR );
    outS = ( matR * m_matDiagonal * m_matTransposedR );

    // Numerical correction to ensure S is symmetric
    // S = (S + Tr(S)) / 2
    outS.m01 = HALF * ( outS.m01 + outS.m10 );
    outS.m10 = outS.m01;
}

/////////////////////////////////////////////////////////////////////////////////

template<typename Real>
Void TSingularDecomposition2<Real>::_HouseHolder_PreMultiply( TMatrix2<Real> & matA, const TVector2<Real> & vFactor ) const
{
    // Compute A = ( Id - 2 * (V * Tr(V)) / (Tr(V) * V) ) * A

    // W = (-2 / V.Norm^2) * (V * A)
    UInt iRow, iCol;
    TVector2<Real> vW;
    vW.X = (vFactor.X * matA.m00) + (vFactor.Y * matA.m10);
    vW.Y = (vFactor.X * matA.m01) + (vFactor.Y * matA.m11);
    vW *= ( -TWO / vFactor.NormSqr() );

    // A += Matrix(V x W)
    matA.m00 += (vFactor.X * vW.X); matA.m01 += (vFactor.X * vW.Y);
    matA.m10 += (vFactor.Y * vW.X); matA.m11 += (vFactor.Y * vW.Y);
}

template<typename Real>
Void TSingularDecomposition2<Real>::_QRAlgorithm()
{
    // m_matDiagonal contains input matrix MxN and
    // gets diagonalized during HH reduction giving
    // the R matrix.
    // m_matLeft receives the Q matrix.

    // Temp array
    TVector2<Real> arrHHVectors[2];

    // Build R = D
    arrHHVectors[0].X = m_matDiagonal.m00;
    arrHHVectors[0].Y = m_matDiagonal.m10;
    Real fLength = arrHHVectors[0].Norm();
    Real fBeta = ( arrHHVectors[0].X + (MathRealFn->Sign(arrHHVectors[0].X) * fLength) );
    if ( !(MathRealFn->EqualsZero(fBeta)) )
        arrHHVectors[0].Y /= fBeta;
    arrHHVectors[0].X = ONE;
    _HouseHolder_PreMultiply( m_matDiagonal, arrHHVectors[0] );

    arrHHVectors[1].X = ZERO;
    arrHHVectors[1].Y = ONE;
    _HouseHolder_PreMultiply( m_matDiagonal, arrHHVectors[1] );

    // Build Q = L
    m_matLeft.MakeIdentity();
    _HouseHolder_PreMultiply( m_matLeft, arrHHVectors[1] );
    _HouseHolder_PreMultiply( m_matLeft, arrHHVectors[0] );
}
