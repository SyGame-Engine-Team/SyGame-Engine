/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Algebra/Matrix/SingularDecomposition2.h
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
// Header prelude
#ifndef SCARAB_LIB_MATH_ALGEBRA_MATRIX_SINGULARDECOMPOSITION2_H
#define SCARAB_LIB_MATH_ALGEBRA_MATRIX_SINGULARDECOMPOSITION2_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "EigenDecomposition2.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TSingularDecomposition2 class
template<typename Real>
class TSingularDecomposition2
{
public:
    TSingularDecomposition2( const TMatrix2<Real> & matNonSymmetric,
                             UInt iMaxIterations = 32, Real fZeroTolerance = (Real)SCALAR_ERROR );
    ~TSingularDecomposition2();

    // Precision management
    UInt MaxIterations;
    Real ZeroTolerance;

    // Singular system access
    inline TMatrix2<Real> & GetSystem();

    // Solves the singular system
    Void SolveSystem();

    // Output access
    inline const TMatrix2<Real> & GetL() const;
    inline const TMatrix2<Real> & GetD() const;
    inline const TMatrix2<Real> & GetTrR() const;

    inline Void GetPolarDecomposition( TMatrix2<Real> & outQ, TMatrix2<Real> & outS ) const;

private:
    static const Real ZERO;
    static const Real ONE;
    static const Real TWO;
    static const Real HALF;

    // HouseHolder operations
    Void _HouseHolder_PreMultiply( TMatrix2<Real> & matA, const TVector2<Real> & vFactor ) const;
    //Void _HouseHolder_PostMultiply( TMatrix2<Real> & matA, const TVector2<Real> & vFactor ) const;
    //Void _HouseHolder_Vector( TVector2<Real> & outHHVector, const TVector2<Real> & vVector ) const;

    // QR algorithm
    Void _QRAlgorithm();

    TMatrix2<Real> m_matSystem;
    TMatrix2<Real> m_matLeft;
    TMatrix2<Real> m_matDiagonal;
    TMatrix2<Real> m_matTransposedR;
};

// Explicit instanciation
typedef TSingularDecomposition2<Float> SingularDecomposition2f;
typedef TSingularDecomposition2<Double> SingularDecomposition2d;
typedef TSingularDecomposition2<Scalar> SingularDecomposition2;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "SingularDecomposition2.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_ALGEBRA_MATRIX_SINGULARDECOMPOSITION2_H

