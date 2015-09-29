/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Algebra/Matrix/EigenDecomposition2.h
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
// Header prelude
#ifndef SCARAB_LIB_MATH_ALGEBRA_MATRIX_EIGENDECOMPOSITION2_H
#define SCARAB_LIB_MATH_ALGEBRA_MATRIX_EIGENDECOMPOSITION2_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Types/Matrix/Matrix2.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TEigenDecomposition2 class
template<typename Real>
class TEigenDecomposition2
{
public:
    TEigenDecomposition2( const TMatrix2<Real> & matSymmetric,
                          UInt iMaxIterations = 32, Real fZeroTolerance = (Real)SCALAR_ERROR );
    ~TEigenDecomposition2();

    // Precision management
    UInt MaxIterations;
    Real ZeroTolerance;

    // Eigen system access
    inline TMatrix2<Real> & GetSystem();

    // Solves the eigen system
    Void SolveSystem( Bool bSortIncreasing = true ) const;
    Void QuickSolveSystem() const; // Allways sorted increasingly

    // Eigen values access
    inline Real GetValue( UInt iValue ) const;
    inline const TVector2<Real> & GetValues() const;
    inline Void GetDiagonalMatrix( TMatrix2<Real> & outDiagonal ) const;

    // Eigen vectors access
    inline Void GetVector( TVector2<Real> & outVector, UInt iVector ) const;
    inline const TMatrix2<Real> & GetVectors() const;
    inline Void GetRotationMatrix( TMatrix2<Real> & outRotation ) const;

private:
    static const Real ZERO;
    static const Real ONE;
    static const Real TWO;
    static const Real HALF;

    // Householder reduction
    Void _TriDiagonalize();

    // QL algorithm
    Bool _QLAlgorithm();

    // Output sorting
    Void _SortIncreasing();
    Void _SortDecreasing();

    // Reflection / Rotation tracking
    Void _EnsureRotation();
    Bool m_bIsRotation;

    // Eigen system data
    TMatrix2<Real> m_matSystem;
    TVector2<Real> m_vDiagonal;
    Real m_fSubDiagonal;

    // Non-Iterative algorithm, specific to 2x2 matrices
    Bool _PositiveRank( Real & outMaxEntry, TVector2<Real> & outMaxRow, const TMatrix2<Real> & matM ) const;
    Void _ComputeRoots();
    Void _ComputeVectors( const TVector2<Real> & vU1, UInt i0, UInt i1 );
};

// Explicit instanciation
typedef TEigenDecomposition2<Float> EigenDecomposition2f;
typedef TEigenDecomposition2<Double> EigenDecomposition2d;
typedef TEigenDecomposition2<Scalar> EigenDecomposition2;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "EigenDecomposition2.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_ALGEBRA_MATRIX_EIGENDECOMPOSITION2_H

