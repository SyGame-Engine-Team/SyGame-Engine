/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Algebra/Matrix/QDUDecomposition.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : QDU decomposition for 3x3 matrices
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : Beware ! Don't try to extract a singular decomposition from an
//              eigen decomposition by using QDU, non full-rank matrices are not
//              supported because it assumes an invertible matrix (full-rank matrix).
//              Use SingularDecomposition for the full-fledged QR algorithm ...
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_LIB_MATH_ALGEBRA_MATRIX_QDUDECOMPOSITION_H
#define SCARAB_LIB_MATH_ALGEBRA_MATRIX_QDUDECOMPOSITION_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Types/Matrix/Matrix3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TQDUDecomposition class
template<typename Real>
class TQDUDecomposition
{
public:
    TQDUDecomposition( const TMatrix3<Real> & matRotation );
    ~TQDUDecomposition();

    // Decompose M = QR = QDU
    // with Q an orthogonal rotation matrix, D a diagonal scaling matrix
    // and U an upper triangular shear matrix.
    // This uses Gram-Schmidt orthogonalization (QR algorithm).
    Void Compute( TMatrix3<Real> & outQ, TMatrix3<Real> & outD, TMatrix3<Real> & outU ) const;

private:
    static const Real ZERO;
    static const Real ONE;

    TMatrix3<Real> m_matRotation;
};

// Explicit instanciation
typedef TQDUDecomposition<Float> QDUDecompositionf;
typedef TQDUDecomposition<Double> QDUDecompositiond;
typedef TQDUDecomposition<Scalar> QDUDecomposition;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "QDUDecomposition.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_ALGEBRA_MATRIX_QDUDECOMPOSITION_H

