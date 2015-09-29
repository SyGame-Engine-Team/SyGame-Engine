/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Algebra/Matrix/EigenDecompositionN.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Eigen decomposition for general symmetric NxN matrices
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// TEigenDecompositionN implementation
template<typename Real>
const Real TEigenDecompositionN<Real>::ZERO = (Real)0;
template<typename Real>
const Real TEigenDecompositionN<Real>::ONE = (Real)1;
template<typename Real>
const Real TEigenDecompositionN<Real>::TWO = (Real)2;
template<typename Real>
const Real TEigenDecompositionN<Real>::HALF = (Real)0.5f;

template<typename Real>
TEigenDecompositionN<Real>::TEigenDecompositionN( const TMatrixN<Real> & matSymmetric,
                                                  UInt iMaxIterations, Real fZeroTolerance ):
    m_matSystem( matSymmetric ), m_vDiagonal(), m_vSubDiagonal()
{
    MaxIterations = iMaxIterations;
    ZeroTolerance = fZeroTolerance;

    m_bIsRotation = false;

    m_iSystemSize = m_matSystem.GetRowCount();
    Assert( (m_iSystemSize >= 2) && (m_iSystemSize == m_matSystem.GetColumnCount()) );

    m_vDiagonal.SetSize( m_iSystemSize );
    m_vSubDiagonal.SetSize( m_iSystemSize );
}
template<typename Real>
TEigenDecompositionN<Real>::~TEigenDecompositionN()
{
    // nothing to do
}

template<typename Real>
inline TMatrixN<Real> & TEigenDecompositionN<Real>::GetSystem() {
    return m_matSystem;
}

template<typename Real>
Void TEigenDecompositionN<Real>::SolveSystem( Bool bSortIncreasing ) const
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
inline Real TEigenDecompositionN<Real>::GetValue( UInt iValue ) const {
    Assert( iValue < m_iSystemSize );
    return m_vDiagonal[iValue];
}
template<typename Real>
inline const TVectorN<Real> & TEigenDecompositionN<Real>::GetValues() const {
    return m_vDiagonal;
}
template<typename Real>
inline Void TEigenDecompositionN<Real>::GetDiagonalMatrix( TMatrixN<Real> & outDiagonal ) const {
    outDiagonal.SetSize( m_iSystemSize, m_iSystemSize );
    outDiagonal.MakeNull();
    for( UInt i = 0; i < m_iSystemSize; ++i )
        outDiagonal(i,i) = m_vDiagonal[i];
}

template<typename Real>
inline Void TEigenDecompositionN<Real>::GetVector( TVectorN<Real> & outVector, UInt iVector ) const {
    Assert( iVector < m_iSystemSize );
    m_matSystem.GetColumn( outVector, iVector );
}
template<typename Real>
inline const TMatrixN<Real> & TEigenDecompositionN<Real>::GetVectors() const {
    return m_matSystem;
}
template<typename Real>
inline Void TEigenDecompositionN<Real>::GetRotationMatrix( TMatrixN<Real> & outRotation ) const {
    outRotation = m_matSystem;
}

/////////////////////////////////////////////////////////////////////////////////

template<typename Real>
Void TEigenDecompositionN<Real>::_TriDiagonalize()
{
    Real fScale, fInvScale, fSum;
    Real fVal0, fVal1, fVal2, fVal3, fInvVal0;

    Int i, iCol;
    Int iMaxRow = (signed)(m_iSystemSize - 2);
    Int iRow = (signed)(m_iSystemSize - 1);
    while( true ) {
        if ( iMaxRow == 0 ) {
            m_vSubDiagonal[iRow] = m_matSystem(iRow,iMaxRow);
            m_vDiagonal[iRow] = ZERO;
            break;
        }

        fScale = ZERO;
        for( iCol = 0; iCol <= iMaxRow; ++iCol )
            fScale += MathRealFn->Abs( m_matSystem(iRow,iCol) );
        if ( MathRealFn->Abs(fScale) < ZeroTolerance ) {
            m_vSubDiagonal[iRow] = m_matSystem(iRow,iMaxRow);
            m_vDiagonal[iRow] = ZERO;
            --iMaxRow;
            --iRow;
            continue;
        }

        fVal0 = ZERO;
        fInvScale = MathRealFn->Invert(fScale);
        for( iCol = 0; iCol <= iMaxRow; ++iCol ) {
            Real & rVal = m_matSystem(iRow,iCol);
            rVal *= fInvScale;
            fVal0 += ( rVal * rVal );
        }
        fVal1 = m_matSystem(iRow,iMaxRow);
        fVal2 = MathRealFn->Sqrt( fVal0 );
        if ( fVal1 > ZERO )
            fVal2 = -fVal2;

        m_vSubDiagonal[iRow] = ( fScale * fVal2 );
        fVal0 -= ( fVal1 * fVal2 );
        m_matSystem(iRow,iMaxRow) = ( fVal1 - fVal2 );
        fVal1 = ZERO;
        fInvVal0 = MathRealFn->Invert(fVal0);
        for( iCol = 0; iCol <= iMaxRow; ++iCol ) {
            m_matSystem(iCol,iRow) = ( m_matSystem(iRow,iCol) * fInvVal0 );
            fVal2 = ZERO;
            for( i = 0; i <= iCol; ++i )
                fVal2 += ( m_matSystem(iCol,i) * m_matSystem(iRow,i) );
            for( i = iCol + 1; i <= iMaxRow; ++i )
                fVal2 += ( m_matSystem(i,iCol) * m_matSystem(iRow,i) );
            m_vSubDiagonal[iCol] = ( fVal2 * fInvVal0 );
            fVal1 += ( m_vSubDiagonal[iCol] * m_matSystem(iRow,iCol) );
        }
        fVal3 = HALF * (fVal1 * fInvVal0);
        for( iCol = 0; iCol <= iMaxRow; ++iCol ) {
            fVal1 = m_matSystem(iRow,iCol);
            fVal2 = ( m_vSubDiagonal[iCol] - (fVal3 * fVal1) );
            m_vSubDiagonal[iCol] = fVal2;
            for( i = 0; i <= iCol; ++i )
                m_matSystem(iCol,i) -= ( (fVal1 * m_vSubDiagonal[i]) + (fVal2 * m_matSystem(iRow,i)) );
        }

        m_vDiagonal[iRow] = fVal0;
        --iMaxRow;
        --iRow;
    }

    m_vDiagonal[0] = ZERO;
    m_vSubDiagonal[0] = ZERO;
    iMaxRow = -1;
    for( iRow = 0; iRow < m_iSystemSize; ++iRow ) {
        if ( MathRealFn->Abs(m_vDiagonal[iRow]) >= ZeroTolerance ) {
            for( iCol = 0; iCol <= iMaxRow; ++iCol ) {
                fSum = ZERO;
                for( i = 0; i <= iMaxRow; ++i )
                    fSum += ( m_matSystem(iRow,i) * m_matSystem(i,iCol) );
                for( i = 0; i <= iMaxRow; ++i )
                    m_matSystem(i,iCol) -= ( fSum * m_matSystem(i,iRow) );
            }
        }
        m_vDiagonal[iRow] = m_matSystem(iRow,iRow);
        m_matSystem(iRow,iRow) = ONE;
        for( iCol = 0; iCol <= iMaxRow; ++iCol ) {
            m_matSystem(iCol,iRow) = ZERO;
            m_matSystem(iRow,iCol) = ZERO;
        }

        ++iMaxRow;
    }

    // Shift subdiagonal to the left
    for( iRow = 1; iRow < m_iSystemSize; ++iRow )
        m_vSubDiagonal[iRow - 1] = m_vSubDiagonal[iRow];
    m_vSubDiagonal[m_iSystemSize-1] = ZERO;

    // Reflection / Rotation initial state
    m_bIsRotation = ( (m_iSystemSize & 1) == 0 );
}

template<typename Real>
Bool TEigenDecompositionN<Real>::_QLAlgorithm()
{
    UInt iStep, iRow, iCol, iCurRow;
    Int iCurCol;
    Real fRatio, fRoot, fA, fB;
    Real fCos, fSin;
    Real fTmp, fTmpRoot;

    for( iRow = 0; iRow < m_iSystemSize; ++iRow ) {
        for( iStep = 0; iStep < MaxIterations; ++iStep ) {
            // Check end of block reduction
            for( iCol = iRow; iCol < (m_iSystemSize - 1); ++iCol ) {
                fTmp = ( MathRealFn->Abs(m_vDiagonal[iCol]) + MathRealFn->Abs(m_vDiagonal[iCol+1]) );
                if ( ( MathRealFn->Abs(m_vSubDiagonal[iCol]) + fTmp ) == fTmp )
                    break;
            }
            if ( iCol == iRow )
                break;

            // Parameters
            fRatio = (m_vDiagonal[iRow+1] - m_vDiagonal[iRow]) / (TWO * m_vSubDiagonal[iRow]);
            fRoot = MathRealFn->Sqrt( (fRatio * fRatio) + ONE );
            fA = m_vDiagonal[iCol] - m_vDiagonal[iRow];
            if ( fRatio >= ZERO )
                fA += m_vSubDiagonal[iRow] / (fRatio + fRoot);
            else
                fA += m_vSubDiagonal[iRow] / (fRatio - fRoot);

            fCos = ONE;
            fSin = ONE;
            fRoot = ZERO;
            for( iCurCol = iCol - 1; iCurCol >= iRow; --iCurCol ) {
                // Givens rotation & update SubDiag
                fTmp = fCos * m_vSubDiagonal[iCurCol];
                fB = fSin * m_vSubDiagonal[iCurCol];
                if ( MathRealFn->Abs(fB) >= MathRealFn->Abs(fA) ) {
                    fRatio = fA / fB;
                    fTmpRoot = MathRealFn->Sqrt( (fRatio * fRatio) + ONE );
                    fSin = MathRealFn->Invert(fTmpRoot);
                    fCos = fRatio * fSin;
                    m_vSubDiagonal[iCurCol+1] = fB * fTmpRoot;
                } else {
                    fRatio = fB / fA;
                    fTmpRoot = MathRealFn->Sqrt( (fRatio * fRatio) + ONE );
                    fCos = MathRealFn->Invert(fTmpRoot);
                    fSin = fRatio * fCos;
                    m_vSubDiagonal[iCurCol+1] = fA * fTmpRoot;
                }

                // Update
                fRatio = m_vDiagonal[iCurCol+1] - fRoot;
                fRoot = ( fSin * (m_vDiagonal[iCurCol] - fRatio) + TWO * (fCos * fTmp) );
                fA = ( (fCos * fRoot) - fTmp );
                fRoot *= fSin;
                m_vDiagonal[iCurCol+1] = fRatio + fRoot;

                // Post-multiply
                for( iCurRow = 0; iCurRow < m_iSystemSize; ++iCurRow ) {
                    fTmp = m_matSystem(iCurRow,iCurCol+1);
                    m_matSystem(iCurRow,iCurCol+1) = ( (fSin * m_matSystem(iCurRow,iCurCol)) + (fCos * fTmp) );
                    m_matSystem(iCurRow,iCurCol)   = ( (fCos * m_matSystem(iCurRow,iCurCol)) - (fSin * fTmp) );
                }
            }
            m_vDiagonal[iRow] -= fRoot;
            m_vSubDiagonal[iRow] = fA;
            m_vSubDiagonal[iCol] = ZERO;
        }
        // Couldn't Solve within max iterations, QL appears
        // to diverge, avoid an infinite loop.
        if (iStep == m_iMaxIterations)
            return false;
    }

    return true;
}

template<typename Real>
Void TEigenDecompositionN<Real>::_SortIncreasing()
{
    UInt iCol, iCurCol, iRow;
    Real fMin, fTmp;
    for( iCol = 0; iCol < m_iSystemSize - 1; ++iCol ) {
        iCurCol = iCol;
        fMin = m_vDiagonal[iCurCol];
        for( iRow = iCol + 1; iRow < m_iSystemSize; ++iRow ) {
            if ( m_vDiagonal[iRow] < fMin ) {
                iCurCol = iRow;
                fMin = m_vDiagonal[iCurCol];
            }
        }
        if ( iCurCol == iCol )
            continue;
        m_vDiagonal[iCurCol] = m_vDiagonal[iCol];
        m_vDiagonal[iCol] = fMin;
        for( iRow = 0; iRow < m_iSystemSize; ++iRow ) {
            fTmp = m_matSystem(iRow,iCol);
            m_matSystem(iRow,iCol) = m_matSystem(iRow,iCurCol);
            m_matSystem(iRow,iCurCol) = fTmp;
            m_bIsRotation = !m_bIsRotation;
        }
    }
}
template<typename Real>
Void TEigenDecompositionN<Real>::_SortDecreasing()
{
    UInt iCol, iCurCol, iRow;
    Real fMax, fTmp;
    for( iCol = 0; iCol < m_iSystemSize - 1; ++iCol ) {
        iCurCol = iCol;
        fMax = m_vDiagonal[iCurCol];
        for( iRow = iCol + 1; iRow < m_iSystemSize; ++iRow ) {
            if ( m_vDiagonal[iRow] > fMax ) {
                iCurCol = iRow;
                fMax = m_vDiagonal[iCurCol];
            }
        }
        if ( iCurCol == iCol )
            continue;
        m_vDiagonal[iCurCol] = m_vDiagonal[iCol];
        m_vDiagonal[iCol] = fMax;
        for( iRow = 0; iRow < m_iSystemSize; ++iRow ) {
            fTmp = m_matSystem(iRow,iCol);
            m_matSystem(iRow,iCol) = m_matSystem(iRow,iCurCol);
            m_matSystem(iRow,iCurCol) = fTmp;
            m_bIsRotation = !m_bIsRotation;
        }
    }
}

template<typename Real>
Void TEigenDecompositionN<Real>::_EnsureRotation()
{
    if ( !m_bIsRotation ) {
        // Change sign on the first column
        for( UInt i = 0; i < m_iSystemSize; ++i )
            m_matSystem(i,0) = -(m_matSystem(i,0));

        m_bIsRotation = true;
    }
}

