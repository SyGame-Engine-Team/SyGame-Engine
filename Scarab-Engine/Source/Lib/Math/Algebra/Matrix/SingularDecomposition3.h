/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Algebra/Matrix/SingularDecomposition3.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Singular decomposition for 3x3 matrices
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
#ifndef SCARAB_LIB_MATH_ALGEBRA_MATRIX_SINGULARDECOMPOSITION3_H
#define SCARAB_LIB_MATH_ALGEBRA_MATRIX_SINGULARDECOMPOSITION3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "EigenDecomposition3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TSingularDecomposition3 class
template<typename Real>
class TSingularDecomposition3
{
public:
    TSingularDecomposition3( const TMatrix3<Real> & matNonSymmetric,
                             UInt iMaxIterations = 32, Real fZeroTolerance = (Real)SCALAR_ERROR );
    ~TSingularDecomposition3();

    // Precision management
    UInt MaxIterations;
    Real ZeroTolerance;

    // Singular system access
    inline TMatrix3<Real> & GetSystem();

    // Solves the singular system
    Void SolveSystem();

    // Output access
    inline const TMatrix3<Real> & GetL() const;
    inline const TMatrix3<Real> & GetD() const;
    inline const TMatrix3<Real> & GetTrR() const;

    inline Void GetPolarDecomposition( TMatrix3<Real> & outQ, TMatrix3<Real> & outS ) const;

private:
    static const Real ZERO;
    static const Real ONE;
    static const Real TWO;
    static const Real HALF;

    // HouseHolder operations
    Void _HouseHolder_PreMultiply( TMatrix3<Real> & matA, const TVector3<Real> & vFactor ) const;
    //Void _HouseHolder_PostMultiply( TMatrix3<Real> & matA, const TVector3<Real> & vFactor ) const;
    //Void _HouseHolder_Vector( TVector3<Real> & outHHVector, const TVector3<Real> & vVector ) const;

    // QR algorithm
    Void _QRAlgorithm();

    TMatrix3<Real> m_matSystem;
    TMatrix3<Real> m_matLeft;
    TMatrix3<Real> m_matDiagonal;
    TMatrix3<Real> m_matTransposedR;
};

// Explicit instanciation
typedef TSingularDecomposition3<Float> SingularDecomposition3f;
typedef TSingularDecomposition3<Double> SingularDecomposition3d;
typedef TSingularDecomposition3<Scalar> SingularDecomposition3;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "SingularDecomposition3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_ALGEBRA_MATRIX_SINGULARDECOMPOSITION3_H

