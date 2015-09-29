/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Algebra/Matrix/SingularDecompositionN.h
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
// Header prelude
#ifndef SCARAB_LIB_MATH_ALGEBRA_MATRIX_SINGULARDECOMPOSITIONN_H
#define SCARAB_LIB_MATH_ALGEBRA_MATRIX_SINGULARDECOMPOSITIONN_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "EigenDecompositionN.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TSingularDecompositionN class
template<typename Real>
class TSingularDecompositionN
{
public:
    TSingularDecompositionN( const TMatrixN<Real> & matNonSymmetric,
                             UInt iMaxIterations = 32, Real fZeroTolerance = (Real)SCALAR_ERROR );
    ~TSingularDecompositionN();

    // Precision management
    UInt MaxIterations;
    Real ZeroTolerance;

    // Singular system access
    inline TMatrixN<Real> & GetSystem();

    // Solves the singular system
    Void SolveSystem();

    // Output access
    inline const TMatrixN<Real> & GetL() const;
    inline const TMatrixN<Real> & GetD() const;
    inline const TMatrixN<Real> & GetTrR() const;

    inline Void GetPolarDecomposition( TMatrixN<Real> & outQ, TMatrixN<Real> & outS ) const;

private:
    static const Real ZERO;
    static const Real ONE;
    static const Real TWO;
    static const Real HALF;

    // HouseHolder operations
    Void _HouseHolder_PreMultiply( TMatrixN<Real> & matA, const TVectorN<Real> & vFactor ) const;
    //Void _HouseHolder_PostMultiply( TMatrixN<Real> & matA, const TVectorN<Real> & vFactor ) const;
    //Void _HouseHolder_Vector( TVectorN<Real> & outHHVector, const TVectorN<Real> & vVector ) const;

    // QR algorithm
    Void _QRAlgorithm();

    UInt m_iRowCount, m_iColumnCount;
    TMatrixN<Real> m_matSystem;
    TMatrixN<Real> m_matLeft;
    TMatrixN<Real> m_matDiagonal;
    TMatrixN<Real> m_matTransposedR;
};

// Explicit instanciation
typedef TSingularDecompositionN<Float> SingularDecompositionNf;
typedef TSingularDecompositionN<Double> SingularDecompositionNd;
typedef TSingularDecompositionN<Scalar> SingularDecompositionN;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "SingularDecompositionN.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_ALGEBRA_MATRIX_SINGULARDECOMPOSITIONN_H

