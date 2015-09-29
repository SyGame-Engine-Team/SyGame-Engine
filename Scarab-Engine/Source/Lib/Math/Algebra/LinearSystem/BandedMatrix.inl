/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Algebra/LinearSystem/BandedMatrix.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Banded matrix support, Cholesky factorization
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TBandedMatrix implementation
template<typename Real>
const Real TBandedMatrix<Real>::ZERO = (Real)0;
template<typename Real>
const Real TBandedMatrix<Real>::ONE = (Real)1;

template<typename Real>
TBandedMatrix<Real>::TBandedMatrix( UInt iSize, UInt iLowerBands, UInt iUpperBands )
{
    Assert( iSize > 0 );
    Assert( iLowerBands < iSize && iUpperBands < iSize );

    m_iSize = iSize;
    m_iLowerBands = iLowerBands;
    m_iUpperBands = iUpperBands;
    m_pDiagonal = NULL;
    m_arrLowerBands = NULL;
    m_arrUpperBands = NULL;

    _AllocateBands();
}
template<typename Real>
TBandedMatrix<Real>::TBandedMatrix( const TBandedMatrix<Real> & rhs )
{
    m_iSize = rhs.m_iSize;
    m_iLowerBands = rhs.m_iLowerBands;
    m_iUpperBands = rhs.m_iUpperBands;
    m_pDiagonal = NULL;
    m_arrLowerBands = NULL;
    m_arrUpperBands = NULL;

    _AllocateBands();

    *m_pDiagonal = *(rhs.m_pDiagonal);

    UInt i;
    for( i = 0; i < m_iLowerBands; ++i )
        m_arrLowerBands[i] = rhs.m_arrLowerBands[i];
    for( i = 0; i < m_iUpperBands; ++i )
        m_arrUpperBands[i] = rhs.m_arrUpperBands[i];
}
template<typename Real>
TBandedMatrix<Real>::~TBandedMatrix()
{
    _DestroyBands();
}

template<typename Real>
TBandedMatrix<Real> & TBandedMatrix<Real>::operator=( const TBandedMatrix<Real> & rhs )
{
    m_iSize = rhs.m_iSize;
    m_iLowerBands = rhs.m_iLowerBands;
    m_iUpperBands = rhs.m_iUpperBands;

    _AllocateBands();

    *m_pDiagonal = *(rhs.m_pDiagonal);

    UInt i;
    for( i = 0; i < m_iLowerBands; ++i )
        m_arrLowerBands[i] = rhs.m_arrLowerBands[i];
    for( i = 0; i < m_iUpperBands; ++i )
        m_arrUpperBands[i] = rhs.m_arrUpperBands[i];

    return (*this);
}

template<typename Real>
inline UInt TBandedMatrix<Real>::GetSize() const {
    return m_iSize;
}
template<typename Real>
inline UInt TBandedMatrix<Real>::GetLowerBandCount() const {
    return m_iLowerBands;
}
template<typename Real>
inline UInt TBandedMatrix<Real>::GetLowerBandSize(UInt iLowerBand) const {
    Assert( iLowerBand < m_iLowerBands );
    return m_iSize - 1 - iLowerBand;
}
template<typename Real>
inline UInt TBandedMatrix<Real>::GetUpperBandCount() const {
    return m_iUpperBands;
}
template<typename Real>
inline UInt TBandedMatrix<Real>::GetUpperBandSize(UInt iUpperBand) const {
    Assert( iUpperBand < m_iUpperBands );
    return m_iSize - 1 - iUpperBand;
}

template<typename Real>
inline TVectorN<Real> & TBandedMatrix<Real>::GetDiagonal() {
    return (*m_pDiagonal);
}
template<typename Real>
inline const TVectorN<Real> & TBandedMatrix<Real>::GetDiagonal() const {
    return (*m_pDiagonal);
}
template<typename Real>
inline TVectorN<Real> & TBandedMatrix<Real>::GetLowerBand( UInt iLowerBand ) {
    Assert( iLowerBand < m_iLowerBands );
    return m_arrLowerBands[iLowerBand];
}
template<typename Real>
inline const TVectorN<Real> & TBandedMatrix<Real>::GetLowerBand( UInt iLowerBand ) const {
    Assert( iLowerBand < m_iLowerBands );
    return m_arrLowerBands[iLowerBand];
}
template<typename Real>
inline TVectorN<Real> & TBandedMatrix<Real>::GetUpperBand( UInt iUpperBand ) {
    Assert( iUpperBand < m_iUpperBands );
    return m_arrUpperBands[iUpperBand];
}
template<typename Real>
inline const TVectorN<Real> & TBandedMatrix<Real>::GetUpperBand( UInt iUpperBand ) const {
    Assert( iUpperBand < m_iUpperBands );
    return m_arrUpperBands[iUpperBand];
}

template<typename Real>
inline Real & TBandedMatrix<Real>::operator()( UInt iRow, UInt iColumn ) {
    Assert( (iRow < m_iSize) && (iColumn < m_iSize) );

    Int iBand = (iColumn - iRow);
    if (iBand == 0)
        return (*m_pDiagonal)[iRow];
    if (iBand > 0) {
        --iBand;
        if ( ((UInt)iBand) < m_iUpperBands )
            return (m_arrUpperBands[iBand])[iRow];
    } else { // (iBand < 0)
        iBand = (-iBand) - 1;
        if ( ((UInt)iBand) < m_iLowerBands )
            return (m_arrLowerBands[iBand])[iColumn];
    }

    static Real s_Dummy = ZERO;
    return s_Dummy;
}
template<typename Real>
inline Real TBandedMatrix<Real>::operator()( UInt iRow, UInt iColumn ) const {
    Assert( (iRow < m_iSize) && (iColumn < m_iSize) );

    Int iBand = (iColumn - iRow);
    if (iBand == 0)
        return (*m_pDiagonal)[iRow];
    if (iBand > 0) {
        --iBand;
        if ( ((UInt)iBand) < m_iUpperBands )
            return (m_arrUpperBands[iBand])[iRow];
    } else { // (iBand < 0)
        iBand = (-iBand) - 1;
        if ( ((UInt)iBand) < m_iLowerBands )
            return (m_arrLowerBands[iBand])[iColumn];
    }

    return ZERO;
}

template<typename Real>
Void TBandedMatrix<Real>::MakeNull()
{
    UInt i, j, iSize;
    for( i = 0; i < m_iSize; ++i )
        (*m_pDiagonal)[i] = ZERO;
    for( i = 0; i < m_iLowerBands; ++i ) {
        iSize = m_iSize - 1 - i;
        for( j = 0; j < iSize; ++j )
            (m_arrLowerBands[i])[j] = ZERO;
    }
    for( i = 0; i < m_iUpperBands; ++i ) {
        iSize = m_iSize - 1 - i;
        for( j = 0; j < iSize; ++j )
            (m_arrUpperBands[i])[j] = ZERO;
    }
}
template<typename Real>
Void TBandedMatrix<Real>::MakeIdentity()
{
    UInt i, j, iSize;
    for( i = 0; i < m_iSize; ++i )
        (*m_pDiagonal)[i] = ONE;
    for( i = 0; i < m_iLowerBands; ++i ) {
        iSize = m_iSize - 1 - i;
        for( j = 0; j < iSize; ++j )
            (m_arrLowerBands[i])[j] = ZERO;
    }
    for( i = 0; i < m_iUpperBands; ++i ) {
        iSize = m_iSize - 1 - i;
        for( j = 0; j < iSize; ++j )
            (m_arrUpperBands[i])[j] = ZERO;
    }
}

template<typename Real>
Bool TBandedMatrix<Real>::CholeskyFactorization( Real fZeroTolerance )
{
    // Symetric banded matrices only
    if ( m_iLowerBands != m_iUpperBands )
        return false;

    // Cholesky factorization
    UInt iSizeM1 = m_iSize - 1;
    UInt i, j, jMin, k, kMax;
    Real fDiag, fInvSqrtDiag;
    for( i = 0; i < m_iSize; ++i ) {
        jMin = 0;
        if ( i > m_iLowerBands )
            jMin = (i - m_iLowerBands);
        for( j = jMin; j < i; ++j ) {
            kMax = j + m_iLowerBands;
            if ( kMax > iSizeM1 )
                kMax = iSizeM1;
            for( k = i; k <= kMax; ++k )
                (*this)(k,i) -= ( (*this)(i,j) * (*this)(k,j) );
        }

        kMax = i + m_iLowerBands;
        if ( kMax > iSizeM1 )
            kMax = iSizeM1;
        for( k = 0; k < i; ++k )
            (*this)(k,i) = (*this)(i,k);
        fDiag = (*this)(i,i);
        if ( fDiag < fZeroTolerance )
            return false;
        fInvSqrtDiag = MathRealFn->InvSqrt(fDiag);
        for( k = i; k <= kMax; ++k )
            (*this)(k,i) *= fInvSqrtDiag;
    }

    // Success
    return true;
}

template<typename Real>
Bool TBandedMatrix<Real>::SolveSystem( TVectorN<Real> & outX, const TVectorN<Real> & vB )
{
    Bool bSolvable = CholeskyFactorization();
    if (!bSolvable)
        return false;

    outX = vB;

    bSolvable = _Solve_Lower( outX );
    if (!bSolvable)
        return false;

    bSolvable = _Solve_Upper( outX );
    if (!bSolvable)
        return false;

    return true;
}

template<typename Real>
Bool TBandedMatrix<Real>::SolveSystem( TMatrixN<Real> & outX, const TMatrixN<Real> & matB )
{
    Bool bSolvable = CholeskyFactorization();
    if (!bSolvable)
        return false;

    outX = matB;

    bSolvable = _Solve_Lower( outX );
    if (!bSolvable)
        return false;

    bSolvable = _Solve_Upper( outX );
    if (!bSolvable)
        return false;

    return true;
}

/////////////////////////////////////////////////////////////////////////////////

template<typename Real>
Void TBandedMatrix<Real>::_AllocateBands()
{
    _DestroyBands();

    UInt i, j, iSize;

    m_pDiagonal = New TVectorN<Real>( m_iSize );
    for( i = 0; i < m_iSize; ++i )
        (*m_pDiagonal)[i] = ZERO;

    if ( m_iLowerBands > 0 ) {
        m_arrLowerBands = New TVectorN<Real>[m_iLowerBands];
        for( i = 0; i < m_iLowerBands; ++i ) {
            iSize = m_iSize - 1 - i;
            m_arrLowerBands[i].SetSize( iSize );
            for( j = 0; j < iSize; ++j )
                (m_arrLowerBands[i])[j] = ZERO;
        }
    }

    if ( m_iUpperBands > 0 ) {
        m_arrUpperBands = New TVectorN<Real>[m_iUpperBands];
        for( i = 0; i < m_iUpperBands; ++i ) {
            iSize = m_iSize - 1 - i;
            m_arrUpperBands[i].SetSize( iSize );
            for( j = 0; j < iSize; ++j )
                (m_arrUpperBands[i])[j] = ZERO;
        }
    }
}
template<typename Real>
Void TBandedMatrix<Real>::_DestroyBands()
{
    if ( m_pDiagonal != NULL ) {
        Delete( m_pDiagonal );
        m_pDiagonal = NULL;
    }
    if ( m_arrLowerBands != NULL ) {
        DeleteA( m_arrLowerBands );
        m_arrLowerBands = NULL;
    }
    if ( m_arrUpperBands != NULL ) {
        DeleteA( m_arrUpperBands );
        m_arrUpperBands = NULL;
    }
}

template<typename Real>
Bool TBandedMatrix<Real>::_Solve_Lower( TVectorN<Real> & vData ) const
{
    UInt i, j;
    Real fLowerDiag;
    for( i = 0; i < m_iSize; ++i ) {
        fLowerDiag = (*this)(i,i);
        if ( MathRealFn->EqualsZero(fLowerDiag) )
            return false;
        for( j = 0; j < i; ++j )
            vData[i] -= ( (*this)(i,j) * vData[j] );
        vData[i] /= fLowerDiag;
    }
    return true;
}
template<typename Real>
Bool TBandedMatrix<Real>::_Solve_Upper( TVectorN<Real> & vData ) const
{
    UInt i, j;
    Real fUpperDiag;
    for( Int iTmp = m_iSize - 1; iTmp >= 0; --iTmp ) {
        i = (unsigned)iTmp;
        fUpperDiag = (*this)(i,i);
        if ( MathRealFn->EqualsZero(fUpperDiag) )
            return false;
        for( j = i + 1; j < m_iSize; ++j )
            vData[i] -= ( (*this)(i,j) * vData[j] );
        vData[i] /= fUpperDiag;
    }
    return true;
}

template<typename Real>
Bool TBandedMatrix<Real>::_Solve_Lower( TMatrixN<Real> & matData ) const
{
    UInt iColumns = matData.GetColumnCount();

    UInt i, j, iCol;
    Real fLowerDiag, fLower, fInvDiag;
    for( i = 0; i < m_iSize; ++i ) {
        fLowerDiag = (*this)(i,i);
        if ( MathRealFn->EqualsZero(fLowerDiag) )
            return false;
        for( j = 0; j < i; ++j ) {
            fLower = (*this)(i,j);
            for( iCol = 0; iCol < iColumns; ++iCol )
                matData(i,iCol) -= ( fLower * matData(j,iCol) );
        }
        fInvDiag = MathRealFn->Invert(fLowerDiag);
        for( iCol = 0; iCol < iColumns; ++iCol )
            matData(i,iCol) *= fInvDiag;
    }
    return true;
}
template<typename Real>
Bool TBandedMatrix<Real>::_Solve_Upper( TMatrixN<Real> & matData ) const
{
    UInt iColumns = matData.GetColumnCount();

    UInt i, j, iCol;
    Real fUpperDiag, fUpper, fInvDiag;
    for( Int iTmp = m_iSize - 1; iTmp >= 0; --iTmp ) {
        i = (unsigned)iTmp;
        fUpperDiag = (*this)(i,i);
        if ( MathRealFn->EqualsZero(fUpperDiag) )
            return false;
        for( j = i + 1; j < m_iSize; ++j ) {
            fUpper = (*this)(i,j);
            for( iCol = 0; iCol < iColumns; ++iCol )
                matData(i,iCol) -= ( fUpper * matData(j,iCol) );
        }
        fInvDiag = MathRealFn->Invert(fUpperDiag);
        for( iCol = 0; iCol < iColumns; ++iCol )
            matData(i,iCol) *= fInvDiag;
    }
    return true;
}


