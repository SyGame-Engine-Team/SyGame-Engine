/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Matrix/MatrixN.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Generic NxM-dimensional matrix
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TMatrixN implementation
template<typename Real>
const Real TMatrixN<Real>::ZERO = (Real)0;
template<typename Real>
const Real TMatrixN<Real>::ONE = (Real)1;

template<typename Real>
TMatrixN<Real>::TMatrixN()
{
    m_iRows = 0;
    m_iColumns = 0;
    m_iSize = 0;
    m_arrMatrix = NULL;
    m_arrRows = NULL;
}
template<typename Real>
TMatrixN<Real>::TMatrixN( UInt iRows, UInt iColumns )
{
    Assert( iRows > 0 );
    Assert( iColumns > 0 );
    _AllocateMatrix( iRows, iColumns );
}
template<typename Real>
TMatrixN<Real>::TMatrixN( UInt iRows, UInt iColumns, const Real * arrMat, Bool bRows )
{
    Assert( iRows > 0 );
    Assert( iColumns > 0 );
    _AllocateMatrix( iRows, iColumns );
    _SetMatrix( arrMat, bRows );
}
template<typename Real>
TMatrixN<Real>::TMatrixN( UInt iRows, UInt iColumns, const Real ** arrMat, Bool bRows )
{
    Assert( iRows > 0 );
    Assert( iColumns > 0 );
    _AllocateMatrix( iRows, iColumns );
    _SetMatrix( arrMat, bRows );
}
template<typename Real>
TMatrixN<Real>::TMatrixN( UInt iRows, UInt iColumns, const TVectorN<Real> * arrMat, Bool bRows )
{
    Assert( iRows > 0 );
    Assert( iColumns > 0 );
    _AllocateMatrix( iRows, iColumns );
    _SetMatrix( arrMat, bRows );
}
template<typename Real>
TMatrixN<Real>::TMatrixN(const TMatrixN<Real> & rhs)
{
    if ( rhs.m_arrMatrix == NULL ) {
        m_iRows = 0;
        m_iColumns = 0;
        m_iSize = 0;
        m_arrMatrix = NULL;
        m_arrRows = NULL;
    } else {
        _AllocateMatrix( rhs.m_iRows, rhs.m_iColumns );
        _SetMatrix( rhs.m_arrMatrix, true );
    }
}
template<typename Real>
TMatrixN<Real>::~TMatrixN()
{
    _DestroyMatrix();
}

template<typename Real>
inline Void TMatrixN<Real>::SetSize( UInt iRows, UInt iColumns ) {
    _AllocateMatrix( iRows, iColumns );
}

template<typename Real>
inline TMatrixN<Real> & TMatrixN<Real>::operator=(const TMatrixN<Real> & rhs) {
    if ( rhs.m_arrMatrix == NULL ) {
        _DestroyMatrix();
        m_iRows = 0;
        m_iColumns = 0;
        m_iSize = 0;
        m_arrMatrix = NULL;
        m_arrRows = NULL;
    } else {
        if ( (m_iRows != rhs.m_iRows) || (m_iColumns != rhs.m_iColumns) )
            _AllocateMatrix( rhs.m_iRows, rhs.m_iColumns );
        _SetMatrix( rhs.m_arrMatrix, true );
    }
    return (*this);
}

template<typename Real>
inline TMatrixN<Real>::operator Real*() const {
    Assert(m_arrMatrix != NULL);
    return m_arrMatrix;
}
template<typename Real>
inline TMatrixN<Real>::operator const Real*() const {
    Assert(m_arrMatrix != NULL);
    return m_arrMatrix;
}

template<typename Real>
inline Real & TMatrixN<Real>::operator[](Int i) {
    Assert(m_arrMatrix != NULL);
    Assert( i >= 0 && i < m_iSize );
    return m_arrMatrix[i];
}
template<typename Real>
inline Real TMatrixN<Real>::operator[](Int i) const {
    Assert(m_arrMatrix != NULL);
    Assert( i >= 0 && i < m_iSize );
    return m_arrMatrix[i];
}
template<typename Real>
inline Real & TMatrixN<Real>::operator[](UInt i) {
    Assert(m_arrMatrix != NULL);
    Assert(i < m_iSize);
    return m_arrMatrix[i];
}
template<typename Real>
inline Real TMatrixN<Real>::operator[](UInt i) const {
    Assert(m_arrMatrix != NULL);
    Assert(i < m_iSize);
    return m_arrMatrix[i];
}

template<typename Real>
inline Real & TMatrixN<Real>::operator()(Int iRow, Int iColumn) {
    Assert(m_arrMatrix != NULL);
    Assert(iRow >= 0 && iColumn >= 0);
    Assert(iRow < m_iRows && iColumn < m_iColumns);
    return (m_arrRows[iRow])[iColumn];
}
template<typename Real>
inline Real TMatrixN<Real>::operator()(Int iRow, Int iColumn) const {
    Assert(m_arrMatrix != NULL);
    Assert(iRow >= 0 && iColumn >= 0);
    Assert(iRow < m_iRows && iColumn < m_iColumns);
    return (m_arrRows[iRow])[iColumn];
}
template<typename Real>
inline Real & TMatrixN<Real>::operator()(UInt iRow, UInt iColumn) {
    Assert(m_arrMatrix != NULL);
    Assert(iRow < m_iRows && iColumn < m_iColumns);
    return (m_arrRows[iRow])[iColumn];
}
template<typename Real>
inline Real TMatrixN<Real>::operator()(UInt iRow, UInt iColumn) const {
    Assert(m_arrMatrix != NULL);
    Assert(iRow < m_iRows && iColumn < m_iColumns);
    return (m_arrRows[iRow])[iColumn];
}

template<typename Real>
inline TMatrixN<Real> TMatrixN<Real>::operator+() const {
    Assert(m_arrMatrix != NULL);
    return TMatrixN<Real>( m_iRows, m_iColumns, m_arrMatrix, true );
}
template<typename Real>
inline TMatrixN<Real> TMatrixN<Real>::operator-() const {
    Assert(m_arrMatrix != NULL);
    TMatrixN<Real> matRes(m_iRows, m_iColumns);
    for( UInt i = 0; i < m_iSize; ++i )
        matRes.m_arrMatrix[i] = -(m_arrMatrix[i]);
    return matRes;
}

template<typename Real>
Bool TMatrixN<Real>::operator==(const TMatrixN<Real> & rhs) const
{
    if ( m_arrMatrix == NULL ) {
        if ( rhs.m_arrMatrix == NULL )
            return true;
        return false;
    }
    if ( rhs.m_arrMatrix == NULL )
        return false;
    if ( m_iRows != rhs.m_iRows || m_iColumns != rhs.m_iColumns )
        return false;
    for( UInt i = 0; i < m_iSize; ++i ) {
        if ( !(MathRealFn->Equals(m_arrMatrix[i], rhs.m_arrMatrix[i])) )
            return false;
    }
    return true;
}
template<typename Real>
Bool TMatrixN<Real>::operator!=(const TMatrixN<Real> & rhs) const
{
    if ( m_arrMatrix == NULL ) {
        if ( rhs.m_arrMatrix == NULL )
            return false;
        return true;
    }
    if ( rhs.m_arrMatrix == NULL )
        return true;
    if ( m_iRows != rhs.m_iRows || m_iColumns != rhs.m_iColumns )
        return true;
    for( UInt i = 0; i < m_iSize; ++i ) {
        if ( !(MathRealFn->Equals(m_arrMatrix[i], rhs.m_arrMatrix[i])) )
            return true;
    }
    return false;
}

template<typename Real>
inline TMatrixN<Real> TMatrixN<Real>::operator*(Real rhs) const {
    Assert(m_arrMatrix != NULL);
    TMatrixN<Real> matRes(m_iRows, m_iColumns);
    for( UInt i = 0; i < m_iSize; ++i )
        matRes.m_arrMatrix[i] = m_arrMatrix[i] * rhs;
    return matRes;
}
template<typename Real>
inline  TMatrixN<Real> TMatrixN<Real>::operator/(Real rhs) const {
    Assert(m_arrMatrix != NULL);
    TMatrixN<Real> matRes(m_iRows, m_iColumns);
    for( UInt i = 0; i < m_iSize; ++i )
        matRes.m_arrMatrix[i] = m_arrMatrix[i] / rhs;
    return matRes;
}

template<typename Real>
inline TMatrixN<Real> & TMatrixN<Real>::operator*=(Real rhs) {
    Assert(m_arrMatrix != NULL);
    for( UInt i = 0; i < m_iSize; ++i )
        m_arrMatrix[i] *= rhs;
    return (*this);
}
template<typename Real>
inline TMatrixN<Real> & TMatrixN<Real>::operator/=(Real rhs) {
    Assert(m_arrMatrix != NULL);
    for( UInt i = 0; i < m_iSize; ++i )
        m_arrMatrix[i] /= rhs;
    return (*this);
}

template<typename Real>
TVectorN<Real> TMatrixN<Real>::operator*(const TVectorN<Real> & rhs) const
{
    Assert(m_arrMatrix != NULL);
    Assert( m_iColumns == rhs.GetSize() );
    UInt i, j;
    Real * pRow;
    TVectorN<Real> vRes(m_iRows);
    for( i = 0; i < m_iRows; ++i ) {
        vRes[i] = ZERO;
        pRow = m_arrRows[i];
        for( j = 0; j < m_iColumns; ++j )
            vRes[i] += ( pRow[j] * rhs[j] );
    }
    return vRes;
}

template<typename Real>
TMatrixN<Real> TMatrixN<Real>::operator+(const TMatrixN<Real> & rhs) const
{
    Assert( (m_arrMatrix != NULL) && (rhs.m_arrMatrix != NULL) );
    Assert( (m_iRows == rhs.m_iRows) && (m_iColumns == rhs.m_iColumns) );
    TMatrixN<Real> matRes(m_iRows, m_iColumns);
    for( UInt i = 0; i < m_iSize; ++i )
        matRes.m_arrMatrix[i] = m_arrMatrix[i] + rhs.m_arrMatrix[i];
    return matRes;
}
template<typename Real>
TMatrixN<Real> TMatrixN<Real>::operator-(const TMatrixN<Real> & rhs) const
{
    Assert( (m_arrMatrix != NULL) && (rhs.m_arrMatrix != NULL) );
    Assert( (m_iRows == rhs.m_iRows) && (m_iColumns == rhs.m_iColumns) );
    TMatrixN<Real> matRes(m_iRows, m_iColumns);
    for( UInt i = 0; i < m_iSize; ++i )
        matRes.m_arrMatrix[i] = m_arrMatrix[i] - rhs.m_arrMatrix[i];
    return matRes;
}
template<typename Real>
TMatrixN<Real> TMatrixN<Real>::operator*(const TMatrixN<Real> & rhs) const
{
    Assert( (m_arrMatrix != NULL) && (rhs.m_arrMatrix != NULL) );
    Assert( m_iColumns == rhs.m_iRows );
    UInt i, j, k;
    Real *pRow, *pResRow;
    TMatrixN<Real> matRes(m_iRows, rhs.m_iColumns);
    for( i = 0; i < matRes.m_iRows; ++i ) {
        pRow = m_arrRows[i];
        pResRow = matRes.m_arrRows[i];
        for( j = 0; j < matRes.m_iColumns; ++j ) {
            pResRow[j] = ZERO;
            for( k = 0; k < m_iColumns; ++k )
                 pResRow[j] += pRow[k] * (rhs.m_arrRows[k])[j];
        }
    }
    return matRes;
}

template<typename Real>
TMatrixN<Real> & TMatrixN<Real>::operator+=(const TMatrixN<Real> & rhs)
{
    Assert( (m_arrMatrix != NULL) && (rhs.m_arrMatrix != NULL) );
    Assert( (m_iRows == rhs.m_iRows) && (m_iColumns == rhs.m_iColumns) );
    for( UInt i = 0; i < m_iSize; ++i )
        m_arrMatrix[i] += rhs.m_arrMatrix[i];
    return (*this);
}
template<typename Real>
TMatrixN<Real> & TMatrixN<Real>::operator-=(const TMatrixN<Real> & rhs)
{
    Assert( (m_arrMatrix != NULL) && (rhs.m_arrMatrix != NULL) );
    Assert( (m_iRows == rhs.m_iRows) && (m_iColumns == rhs.m_iColumns) );
    for( UInt i = 0; i < m_iSize; ++i )
        m_arrMatrix[i] -= rhs.m_arrMatrix[i];
    return (*this);
}

template<typename Real>
inline UInt TMatrixN<Real>::GetRowCount() const {
    return m_iRows;
}
template<typename Real>
inline UInt TMatrixN<Real>::GetColumnCount() const {
    return m_iColumns;
}
template<typename Real>
inline UInt TMatrixN<Real>::GetSize() const {
    return m_iSize;
}

template<typename Real>
inline Void TMatrixN<Real>::GetRow(TVectorN<Real> & outRow, UInt iRow) const {
    Assert(m_arrMatrix != NULL);
    Assert(iRow < m_iRows);
    outRow = TVectorN<Real>( m_iColumns, m_arrRows[iRow] );
}
template<typename Real>
inline Void TMatrixN<Real>::SetRow(UInt iRow, const Real * vRow) {
    Assert(m_arrMatrix != NULL);
    Assert(iRow < m_iRows);
    Real * pRow = m_arrRows[iRow];
    for( UInt i = 0; i < m_iColumns; ++i )
        pRow[i] = vRow[i];
}
template<typename Real>
inline Void TMatrixN<Real>::SetRow(UInt iRow, const TVectorN<Real> & vRow) {
    Assert(m_arrMatrix != NULL);
    Assert(iRow < m_iRows);
    Real * pRow = m_arrRows[iRow];
    for( UInt i = 0; i < m_iColumns; ++i )
        pRow[i] = vRow[i];
}

template<typename Real>
inline Void TMatrixN<Real>::GetColumn(TVectorN<Real> & outColumn, UInt iColumn) const {
    Assert(m_arrMatrix != NULL);
    Assert(iColumn < m_iColumns);
    outColumn = TVectorN<Real>( m_iRows );
    for( UInt i = 0; i < m_iRows; ++i )
        outColumn[i] = (m_arrRows[i])[iColumn];
}
template<typename Real>
inline Void TMatrixN<Real>::SetColumn(UInt iColumn, const Real * vCol) {
    Assert(m_arrMatrix != NULL);
    Assert(iColumn < m_iColumns);
    for( UInt i = 0; i < m_iRows; ++i )
        (m_arrRows[i])[iColumn] = vCol[iColumn];
}
template<typename Real>
inline Void TMatrixN<Real>::SetColumn(UInt iColumn, const TVectorN<Real> & vCol) {
    Assert(m_arrMatrix != NULL);
    Assert(iColumn < m_iColumns);
    for( UInt i = 0; i < m_iRows; ++i )
        (m_arrRows[i])[iColumn] = vCol[iColumn];
}

template<typename Real>
inline Void TMatrixN<Real>::SwapRows( UInt iRow0, UInt iRow1 ) {
    Assert(m_arrMatrix != NULL);
    Assert(iRow0 < m_iRows && iRow1 < m_iRows);
    Real * pSwap = m_arrRows[iRow0];
    m_arrRows[iRow0] = m_arrRows[iRow1];
    m_arrRows[iRow1] = pSwap;
}

template<typename Real>
inline Void TMatrixN<Real>::MakeNull() {
    Assert(m_arrMatrix != NULL);
    for( UInt i = 0; i < m_iSize; ++i )
        m_arrMatrix[i] = ZERO;
}
template<typename Real>
inline Void TMatrixN<Real>::MakeIdentity() {
    Assert(m_arrMatrix != NULL);
    UInt i, j;
    Real * pRow;
    for( i = 0; i < m_iRows; ++i ) {
        pRow = m_arrRows[i];
        for( j = 0; j < m_iColumns; ++j ) {
            if (j == i)
                pRow[j] = ONE;
            else
                pRow[j] = ZERO;
        }
    }
}
template<typename Real>
inline Void TMatrixN<Real>::MakeMatrix( const Real * arrMat, Bool bRows ) {
    Assert(m_arrMatrix != NULL);
    _SetMatrix( arrMat, bRows );
}
template<typename Real>
inline Void TMatrixN<Real>::MakeMatrix( const Real ** arrMat, Bool bRows ) {
    Assert(m_arrMatrix != NULL);
    _SetMatrix( arrMat, bRows );
}
template<typename Real>
inline Void TMatrixN<Real>::MakeMatrix( const TVectorN<Real> * arrMat, Bool bRows ) {
    Assert(m_arrMatrix != NULL);
    _SetMatrix( arrMat, bRows );
}

template<typename Real>
inline Void TMatrixN<Real>::Transpose(TMatrixN<Real> & outTransposedMatrix) const {
    Assert(m_arrMatrix != NULL);
    outTransposedMatrix = TMatrixN<Real>( m_iColumns, m_iRows );
    UInt i, j;
    Real * pRow;
    for( i = 0; i < m_iRows; ++i ) {
        pRow = m_arrRows[i];
        for( j = 0; j < m_iColumns; ++j )
            (outTransposedMatrix.m_arrRows[j])[i] = pRow[j];
    }
}

template<typename Real>
Bool TMatrixN<Real>::Invert(TMatrixN<Real> & outInvMatrix, Real fZeroTolerance) const
{
    Assert(m_arrMatrix != NULL);
    if ( m_iRows != m_iColumns )
        return false;

    UInt i, iSystemSize = m_iRows;
    outInvMatrix = (*this);

    UInt * arrRows = New UInt[iSystemSize];
    UInt * arrColumns = New UInt[iSystemSize];
    Bool * arrIsPivoted = New Bool[iSystemSize];
    for( i = 0; i < iSystemSize; ++i )
        arrIsPivoted[i] = false;

    // Full-Pivot elimination
    UInt iRow, iCol, i0, i1, i2;
    Real fMax, fAbs, fInv, fSaved;
    Real *pRow, *pPivotRow;
    for( i0 = 0; i0 < iSystemSize; ++i0 ) {
        // Search max abs value in non-pivoted rows
        fMax = ZERO;
        for( i1 = 0; i1 < iSystemSize; ++i1 ) {
            if ( arrIsPivoted[i1] )
                continue;
            pRow = outInvMatrix.m_arrRows[i1];
            for( i2 = 0; i2 < iSystemSize; ++i2 ) {
                if ( arrIsPivoted[i2] )
                    continue;
                fAbs = MathRealFn->Abs(pRow[i2]);
                if (fAbs > fMax) {
                    fMax = fAbs;
                    iRow = i1;
                    iCol = i2;
                }
            }
        }

        // Non-Invertible case
        if ( MathRealFn->Abs(fMax) < fZeroTolerance ) {
            DeleteA( arrRows );
            DeleteA( arrColumns );
            DeleteA( arrIsPivoted );
            return false;
        }

        arrIsPivoted[iCol] = true;

        // Swap rows
        if (iRow != iCol)
            outInvMatrix.SwapRows(iRow, iCol);

        // Keep track of row permutations
        arrRows[i0] = iRow;
        arrColumns[i0] = iCol;

        // Scale row
        pPivotRow = outInvMatrix.m_arrRows[iCol];
        fInv = MathRealFn->Invert(pPivotRow[iCol]);
        for( i2 = 0; i2 < iSystemSize; ++i2 )
            pPivotRow[i2] *= fInv;
        pPivotRow[iCol] = ONE;

        // Reduct other rows
        for( i1 = 0; i1 < iSystemSize; ++i1 ) {
            if ( i1 == iCol )
                continue;
            pRow = outInvMatrix.m_arrRows[i1];
            fSaved = pRow[iCol];
            for( i2 = 0; i2 < iSystemSize; ++i2 )
                pRow[i2] -= (pPivotRow[i2] * fSaved);
            pRow[iCol] = ZERO;
        }
    }

    // Reorder rows to get the inverted matrix
    for ( Int j = (Int)(iSystemSize-1); j >= 0; --j ) {
        if ( arrRows[j] == arrColumns[j] )
            continue;
        for ( i2 = 0; i2 < iSystemSize; ++i2 ) {
            pRow = outInvMatrix.m_arrRows[i2];
            fSaved = pRow[arrRows[j]];
            pRow[arrRows[j]] = pRow[arrColumns[j]];
            pRow[arrColumns[j]] = fSaved;
        }
    }

    DeleteA( arrRows );
    DeleteA( arrColumns );
    DeleteA( arrIsPivoted );
    return true;
}

template<typename Real>
inline Real TMatrixN<Real>::QuadraticForm(const TVectorN<Real> & v0, const TVectorN<Real> & v1) const {
    Assert(m_arrMatrix != NULL);
    return ( v0 * ((*this) * v1) );
}

/////////////////////////////////////////////////////////////////////////////////

template<typename Real>
Void TMatrixN<Real>::_AllocateMatrix( UInt iRows, UInt iColumns )
{
    _DestroyMatrix();

    m_iRows = iRows;
    m_iColumns = iColumns;
    m_iSize = m_iRows * m_iColumns;
    m_arrMatrix = New Real[m_iSize];

    m_arrRows = New Real*[m_iRows];
    for( UInt i = 0; i < m_iRows; ++i )
        m_arrRows[i] = ( m_arrMatrix + (i * m_iColumns) );
}
template<typename Real>
Void TMatrixN<Real>::_DestroyMatrix()
{
    if ( m_arrRows != NULL ) {
        DeleteA( m_arrRows );
        m_arrRows = NULL;
    }
    if ( m_arrMatrix != NULL ) {
        DeleteA( m_arrMatrix );
        m_arrMatrix = NULL;
    }
}

template<typename Real>
Void TMatrixN<Real>::_SetMatrix( const Real * arrMat, Bool bRows )
{
    UInt i, j;
    Real * pRow;
    const Real * pMatOffset;
    if ( bRows ) {
        for( i = 0; i < m_iRows; ++i ) {
            pRow = m_arrRows[i];
            pMatOffset = ( arrMat + (i * m_iColumns) );
            for( j = 0; j < m_iColumns; ++j )
                *pRow++ = *pMatOffset++;
        }
    } else {
        for( i = 0; i < m_iRows; ++i ) {
            pRow = m_arrRows[i];
            pMatOffset = ( arrMat + i );
            for( j = 0; j < m_iColumns; ++j ) {
                *pRow++ = *pMatOffset;
                pMatOffset += m_iRows;
            }
        }
    }
}
template<typename Real>
Void TMatrixN<Real>::_SetMatrix( const Real ** arrMat, Bool bRows )
{
    UInt i, j;
    Real * pRow;
    if ( bRows ) {
        const Real * pMatOffset;
        for( i = 0; i < m_iRows; ++i ) {
            pRow = m_arrRows[i];
            pMatOffset = arrMat[i];
            for( j = 0; j < m_iColumns; ++j )
                *pRow++ = *pMatOffset++;
        }
    } else {
        for( i = 0; i < m_iRows; ++i ) {
            pRow = m_arrRows[i];
            for( j = 0; j < m_iColumns; ++j )
                *pRow++ = arrMat[j][i];
        }
    }
}
template<typename Real>
Void TMatrixN<Real>::_SetMatrix( const TVectorN<Real> * arrMat, Bool bRows )
{
    UInt i, j;
    Real * pRow;
    if ( bRows ) {
        const Real * pMatOffset;
        for( i = 0; i < m_iRows; ++i ) {
            pRow = m_arrRows[i];
            pMatOffset = (const Real *)( arrMat[i] );
            for( j = 0; j < m_iColumns; ++j )
                *pRow++ = *pMatOffset++;
        }
    } else {
        for( i = 0; i < m_iRows; ++i ) {
            pRow = m_arrRows[i];
            for( j = 0; j < m_iColumns; ++j )
                *pRow++ = (arrMat[j])[i];
        }
    }
}

