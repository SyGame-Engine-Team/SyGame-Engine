/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Algebra/Matrix/EigenDecompositionN.h
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
// Header prelude
#ifndef SCARAB_LIB_MATH_ALGEBRA_MATRIX_EIGENDECOMPOSITIONN_H
#define SCARAB_LIB_MATH_ALGEBRA_MATRIX_EIGENDECOMPOSITIONN_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Types/Matrix/MatrixN.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TEigenDecompositionN class
template<typename Real>
class TEigenDecompositionN
{
public:
    TEigenDecompositionN( const TMatrixN<Real> & matSymmetric,
                          UInt iMaxIterations = 32, Real fZeroTolerance = (Real)SCALAR_ERROR );
    ~TEigenDecompositionN();

    // Precision management
    UInt MaxIterations;
    Real ZeroTolerance;

    // Eigen system access
    inline TMatrixN<Real> & GetSystem();

    // Solves the eigen system
    Void SolveSystem( Bool bSortIncreasing = true ) const;

    // Eigen values access
    inline Real GetValue( UInt iValue ) const;
    inline const TVectorN<Real> & GetValues() const;
    inline Void GetDiagonalMatrix( TMatrixN<Real> & outDiagonal ) const;

    // Eigen vectors access
    inline Void GetVector( TVectorN<Real> & outVector, UInt iVector ) const;
    inline const TMatrixN<Real> & GetVectors() const;
    inline Void GetRotationMatrix( TMatrixN<Real> & outRotation ) const;

private:
    static const Real ZERO;
    static const Real ONE;
    static const Real TWO;
    static const Real HALF;

    // Householder reduction
    Void _TriDiagonalize();

    // QL algorithm for tri-diagonal
    // matrices with implicit shifting
    Bool _QLAlgorithm();

    // Output sorting
    Void _SortIncreasing();
    Void _SortDecreasing();

    // Reflection / Rotation tracking
    Void _EnsureRotation();
    Bool m_bIsRotation;

    // Eigen system data
    UInt m_iSystemSize;
    TMatrixN<Real> m_matSystem;
    TVectorN<Real> m_vDiagonal;
    TVectorN<Real> m_vSubDiagonal;
};

// Explicit instanciation
typedef TEigenDecompositionN<Float> EigenDecompositionNf;
typedef TEigenDecompositionN<Double> EigenDecompositionNd;
typedef TEigenDecompositionN<Scalar> EigenDecompositionN;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "EigenDecompositionN.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_ALGEBRA_MATRIX_EIGENDECOMPOSITIONN_H

