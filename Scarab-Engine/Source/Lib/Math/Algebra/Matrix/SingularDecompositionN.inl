/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Algebra/Matrix/SingularDecompositionN.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Singular decomposition for general NxM matrices
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TSingularDecompositionN implementation
template<typename Real>
const Real TSingularDecompositionN<Real>::ZERO = (Real)0;
template<typename Real>
const Real TSingularDecompositionN<Real>::ONE = (Real)1;
template<typename Real>
const Real TSingularDecompositionN<Real>::TWO = (Real)2;
template<typename Real>
const Real TSingularDecompositionN<Real>::HALF = (Real)0.5f;

template<typename Real>
TSingularDecompositionN<Real>::TSingularDecompositionN( const TMatrixN<Real> & matNonSymmetric,
                                                        UInt iMaxIterations, Real fZeroTolerance ):
    m_matSystem( matNonSymmetric ), m_matLeft(), m_matDiagonal(), m_matTransposedR()
{
    MaxIterations = iMaxIterations;
    ZeroTolerance = fZeroTolerance;

    m_iRowCount = m_matSystem.GetRowCount();
    m_iColumnCount = m_matSystem.GetColumnCount();
    Assert( m_iRowCount >= m_iColumnCount );
}
template<typename Real>
TSingularDecompositionN<Real>::~TSingularDecompositionN()
{
    // nothing to do
}

template<typename Real>
inline TMatrixN<Real> & TSingularDecompositionN<Real>::GetSystem() {
    return m_matSystem;
}

template<typename Real>
Void TSingularDecompositionN<Real>::SolveSystem()
{
    TMatrixN<Real> matTrMxM;
    m_matSystem.Transpose( matTrMxM );
    m_matTransposedR = ( matTrMxM * m_matSystem );

    TEigenDecompositionN<Real> eigenSolver( m_matTransposedR, MaxIterations, ZeroTolerance );
    eigenSolver.SolveSystem( false );
    const TMatrixN<Real> & matEigenVectors = eigenSolver.GetVectors();

    m_matLeft.SetSize( m_iRowCount, m_iRowCount );
    m_matDiagonal.SetSize( m_iRowCount, m_iColumnCount );
    m_matDiagonal = ( m_matSystem * matEigenVectors );
    _QRAlgorithm();

    matEigenVectors.Transpose( m_matTransposedR );
}

template<typename Real>
inline const TMatrixN<Real> & TSingularDecompositionN<Real>::GetL() const {
    return m_matLeft;
}
template<typename Real>
inline const TMatrixN<Real> & TSingularDecompositionN<Real>::GetD() const {
    return m_matDiagonal;
}
template<typename Real>
inline const TMatrixN<Real> & TSingularDecompositionN<Real>::GetTrR() const {
    return m_matTransposedR;
}

template<typename Real>
inline Void TSingularDecompositionN<Real>::GetPolarDecomposition( TMatrixN<Real> & outQ, TMatrixN<Real> & outS ) const {
    outQ = ( m_matLeft * m_matTransposedR );
    TMatrixN<Real> matR;
    m_matTransposedR.Transpose( matR );
    outS = ( matR * m_matDiagonal * m_matTransposedR );

    // Numerical correction to ensure S is symmetric
    // S = (S + Tr(S)) / 2
    UInt iRow, iCol;
    UInt iRowCount = outS.GetRowCount();
    UInt iColumnCount = outS.GetColumnCount();
    for( iRow = 0; iRow < iRowCount; ++iRow ) {
        for( iCol = iRow + 1; iCol < iColumnCount; ++iCol ) {
            outS(iRow,iCol) = HALF * ( outS(iRow,iCol) + outS(iCol,iRow) );
            outS(iCol,iRow) = outS(iRow,iCol);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////

template<typename Real>
Void TSingularDecompositionN<Real>::_HouseHolder_PreMultiply( TMatrixN<Real> & matA, const TVectorN<Real> & vFactor ) const
{
    // Compute A = ( Id - 2 * (V * Tr(V)) / (Tr(V) * V) ) * A
    UInt iRowCount = matA.GetRowCount();
    UInt iColumnCount = matA.GetColumnCount();
    Assert( iRowCount == vFactor.GetSize() );

    // W = (-2 / V.Norm^2) * (V * A)
    UInt iRow, iCol;
    TVectorN<Real> vW( iColumnCount );
    for( iCol = 0; iCol < iColumnCount; ++iCol ) {
        vW[iCol] = ZERO;
        for( iRow = 0; iRow < iRowCount; ++iRow )
            vW[iCol] += ( vFactor[iRow] * matA(iRow,iCol) );
    }
    vW *= ( -TWO / vFactor.NormSqr() );

    // A += Matrix(V x W)
    for( iRow = 0; iRow < iRowCount; ++iRow ) {
        for( iCol = 0; iCol < iColumnCount; ++iCol )
            matA(iRow,iCol) += ( vFactor[iRow] * vW[iCol] );
    }
}
//template<typename Real>
//Void TSingularDecompositionN<Real>::_HouseHolder_PostMultiply( TMatrixN<Real> & matA, const TVectorN<Real> & vFactor ) const
//{
//    // A = A * ( Id - 2 * (V * Tr(V)) / (Tr(V) * V) )
//    UInt iRowCount = matA.GetRowCount();
//    UInt iColumnCount = matA.GetColumnCount();
//
//    // W = (-2 / V.Norm^2) * (A * V)
//    TVectorN<Real> vW = ( matA * vFactor );
//    vW *= ( -TWO / vFactor.NormSqr() );
//
//    // A += Matrix(W x V)
//    for( iRow = 0; iRow < iRowCount; ++iRow ) {
//        for( iCol = 0; iCol < iColumnCount; ++iCol )
//            matA(iRow,iCol) += ( vW[iRow] * vFactor[iCol] );
//    }
//}
//template<typename Real>
//Void TSingularDecompositionN<Real>::_HouseHolder_Vector( TVectorN<Real> & outHHVector, const TVectorN<Real> & vVector ) const
//{
//    UInt iSize = vVector.GetSize();
//    Real fLength = vVector.Norm();
//    Real fBeta = vVector[0] + ( MathRealFn->Sign(vVector[0]) * fLength );
//    Real fInvBeta;
//
//    outHHVector = vVector;
//    if ( !(MathRealFn->EqualsZero(fBeta)) ) {
//        fInvBeta = MathRealFn->Invert(fBeta);
//        for( UInt i = 1; i < iSize; ++i )
//            outHHVector[i] *= fInvBeta;
//    }
//    outHHVector[0] = ONE;
//}

template<typename Real>
Void TSingularDecompositionN<Real>::_QRAlgorithm()
{
    // m_matDiagonal contains input matrix MxN and
    // gets diagonalized during HH reduction giving
    // the R matrix.
    // m_matLeft receives the Q matrix.

    // Allocate temp array
    TVectorN<Real> * arrHHVectors = New TVectorN<Real>[m_iColumnCount];

    // Build R = D
    Real fLength, fBeta, fInvBeta;
    UInt i, iRow, iCol;
    for( iCol = 0; iCol < m_iColumnCount; ++iCol ) {
        TVectorN<Real> & vColumn = arrHHVectors[iCol];
        vColumn.SetSize( m_iRowCount );

        // Partial HH vector
        for( iRow = 0; iRow < iCol; ++iRow )
            vColumn[iRow] = ZERO;
        fLength = ZERO;
        for( iRow = iCol; iRow < m_iRowCount; ++iRow ) {
            vColumn[iRow] = m_matDiagonal(iRow,iCol);
            fLength += ( vColumn[iRow] * vColumn[iRow] );
        }
        fLength = MathRealFn->Sqrt(fLength);
        fBeta = ( vColumn[iCol] + (MathRealFn->Sign(vColumn[iCol]) * fLength) );
        if ( !(MathRealFn->EqualsZero(fBeta)) ) {
            fInvBeta = MathRealFn->Invert(fBeta);
            for( i = iCol + 1; i < m_iRowCount; ++i )
                vColumn[i] *= fInvBeta;
        }
        vColumn[iCol] = ONE;

        _HouseHolder_PreMultiply( m_matDiagonal, vColumn );
    }

    // Build Q = L
    m_matLeft.MakeIdentity();
    iCol = m_iColumnCount - 1;
    for( i = 0; i < m_iColumnCount; ++i )
        _HouseHolder_PreMultiply( m_matLeft, arrHHVectors[iCol--] );

    // Destroy temp array
    DeleteA( arrHHVectors );
}
