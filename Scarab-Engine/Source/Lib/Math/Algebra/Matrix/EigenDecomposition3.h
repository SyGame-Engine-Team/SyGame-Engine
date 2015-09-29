/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Algebra/Matrix/EigenDecomposition3.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Eigen decomposition for 3x3 matrices
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
#ifndef SCARAB_LIB_MATH_ALGEBRA_MATRIX_EIGENDECOMPOSITION3_H
#define SCARAB_LIB_MATH_ALGEBRA_MATRIX_EIGENDECOMPOSITION3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Types/Matrix/Matrix3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TEigenDecomposition3 class
template<typename Real>
class TEigenDecomposition3
{
public:
    TEigenDecomposition3( const TMatrix3<Real> & matSymmetric,
                          UInt iMaxIterations = 32, Real fZeroTolerance = (Real)SCALAR_ERROR );
    ~TEigenDecomposition3();

    // Precision management
    UInt MaxIterations;
    Real ZeroTolerance;

    // Eigen system access
    inline TMatrix3<Real> & GetSystem();

    // Solves the eigen system
    Void SolveSystem( Bool bSortIncreasing = true ) const;
    Void QuickSolveSystem() const; // Allways sorted increasingly

    // Eigen values access
    inline Real GetValue( UInt iValue ) const;
    inline const TVector3<Real> & GetValues() const;
    inline Void GetDiagonalMatrix( TMatrix3<Real> & outDiagonal ) const;

    // Eigen vectors access
    inline Void GetVector( TVector3<Real> & outVector, UInt iVector ) const;
    inline const TMatrix3<Real> & GetVectors() const;
    inline Void GetRotationMatrix( TMatrix3<Real> & outRotation ) const;

private:
    static const Real ZERO;
    static const Real ONE;
    static const Real TWO;
    static const Real HALF;
    static const Double INV3;
    static const Double SQRT3;

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
    TMatrix3<Real> m_matSystem;
    TVector3<Real> m_vDiagonal;
    TVector2<Real> m_vSubDiagonal;

    // Non-Iterative algorithm, specific to 3x3 matrices
    Bool _PositiveRank( Real & outMaxEntry, TVector3<Real> & outMaxRow, const TMatrix3<Real> & matM ) const;
    Void _ComputeRoots();
    Void _ComputeVectors( const TVector3<Real> & vU2, UInt i0, UInt i1, UInt i2 );
};

// Explicit instanciation
typedef TEigenDecomposition3<Float> EigenDecomposition3f;
typedef TEigenDecomposition3<Double> EigenDecomposition3d;
typedef TEigenDecomposition3<Scalar> EigenDecomposition3;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "EigenDecomposition3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_ALGEBRA_MATRIX_EIGENDECOMPOSITION3_H

