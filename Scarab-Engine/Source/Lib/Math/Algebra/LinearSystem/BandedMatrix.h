/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Algebra/LinearSystem/BandedMatrix.h
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
// Header prelude
#ifndef SCARAB_LIB_MATH_ALGEBRA_LINEARSYSTEM_BANDEDMATRIX_H
#define SCARAB_LIB_MATH_ALGEBRA_LINEARSYSTEM_BANDEDMATRIX_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Types/Vector/VectorN.h"
#include "../../Types/Matrix/MatrixN.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TBandedMatrix class
template<typename Real>
class TBandedMatrix
{
public:
    // A BandedMatrix is a N*N matrix filled with '0' except for the
    // diagonal plus several lower and upper bands.
    TBandedMatrix( UInt iSize, UInt iLowerBands, UInt iUpperBands );
    TBandedMatrix( const TBandedMatrix<Real> & rhs );
    ~TBandedMatrix();

    // affectation
    TBandedMatrix<Real> & operator=( const TBandedMatrix<Real> & rhs );

    // Getters
    inline UInt GetSize() const;
    inline UInt GetLowerBandCount() const;
    inline UInt GetLowerBandSize(UInt iLowerBand) const;
    inline UInt GetUpperBandCount() const;
    inline UInt GetUpperBandSize(UInt iUpperBand) const;

    // Matrix access
    inline TVectorN<Real> & GetDiagonal();
    inline const TVectorN<Real> & GetDiagonal() const;
    inline TVectorN<Real> & GetLowerBand( UInt iLowerBand );
    inline const TVectorN<Real> & GetLowerBand( UInt iLowerBand ) const;
    inline TVectorN<Real> & GetUpperBand( UInt iUpperBand );
    inline const TVectorN<Real> & GetUpperBand( UInt iUpperBand ) const;

    inline Real & operator()( UInt iRow, UInt iColumn );
    inline Real operator()( UInt iRow, UInt iColumn ) const;

    Void MakeNull();
    Void MakeIdentity();

    // Cholesky factorization is an LU decomposition from a banded
    // matrix A to A = L * Transpose(L), where L is a lower triangular
    // matrix (and Transpose(L) is upper triangular).
    // If successfull, A contains L in its lower triangular part and
    // Transpose(L) in its upper triangular part.
    Bool CholeskyFactorization( Real fZeroTolerance = (Real)SCALAR_ERROR );

    // Solves the linear system A*X = B, with A an N*N banded matrix and
    // B and X two N*1 vectors.
    // If successfull, A contains its Cholesky factorization
    Bool SolveSystem( TVectorN<Real> & outX, const TVectorN<Real> & vB );

    // Solves the linear system A*X = B, with A an N*N banded matrix,
    // B a N*M matrix and X a N*M matrix too.
    // If successfull, A contains its Cholesky factorization
    Bool SolveSystem( TMatrixN<Real> & outX, const TMatrixN<Real> & matB );

private:
    // Helpers
    Void _AllocateBands();
    Void _DestroyBands();

    // Solvers
        // Reduct A*X = L*U*X = B, with U = Transpose(L)
        // To U*X = Invert(L)*B
    Bool _Solve_Lower( TVectorN<Real> & vData ) const;
        // Reduct U*X = Invert(L)*B
        // To X = Invert(U)*Invert(L)*B
    Bool _Solve_Upper( TVectorN<Real> & vData ) const;

        // Reduct A*X = L*U*X = B, with U = Transpose(L)
        // To U*X = Invert(L)*B
    Bool _Solve_Lower( TMatrixN<Real> & matData ) const;
        // Reduct U*X = Invert(L)*B
        // To X = Invert(U)*Invert(L)*B
    Bool _Solve_Upper( TMatrixN<Real> & matData ) const;

    static const Real ZERO;
    static const Real ONE;

    // Data
    UInt m_iSize; // = N
    UInt m_iLowerBands, m_iUpperBands;
    TVectorN<Real> * m_pDiagonal; // N
    TVectorN<Real> * m_arrLowerBands;
    TVectorN<Real> * m_arrUpperBands;
};

// Explicit instanciation
typedef TBandedMatrix<Float> BandedMatrixf;
typedef TBandedMatrix<Double> BandedMatrixd;
typedef TBandedMatrix<Scalar> BandedMatrix;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "BandedMatrix.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_ALGEBRA_LINEARSYSTEM_BANDEDMATRIX_H

