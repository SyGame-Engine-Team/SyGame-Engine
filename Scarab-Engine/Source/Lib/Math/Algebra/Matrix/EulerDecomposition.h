/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Algebra/Matrix/EulerDecomposition.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Euler UDU and UDV decompositions for 3x3 matrices
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
#ifndef SCARAB_LIB_MATH_ALGEBRA_MATRIX_EULERDECOMPOSITION_H
#define SCARAB_LIB_MATH_ALGEBRA_MATRIX_EULERDECOMPOSITION_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Types/Matrix/Matrix3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
enum EulerResult
{
    EULER_UNIQUE = 0,
    EULER_CONST_SUM,  // not unique, (Roll + Yaw) is constant
    EULER_CONST_DIFF  // not unique, (Roll - Yaw) is constant
};

/////////////////////////////////////////////////////////////////////////////////
// The TEulerDecomposition class
template<typename Real>
class TEulerDecomposition
{
public:
    TEulerDecomposition( const TMatrix3<Real> & matRotation );
    ~TEulerDecomposition();

    // Extract euler angles from a given axis pattern ...
        // UDV Decompositions
    EulerResult ExtractXYZ( Real & outYaw, Real & outPitch, Real & outRoll ) const;
    EulerResult ExtractXZY( Real & outYaw, Real & outPitch, Real & outRoll ) const;
    EulerResult ExtractYXZ( Real & outYaw, Real & outPitch, Real & outRoll ) const;
    EulerResult ExtractYZX( Real & outYaw, Real & outPitch, Real & outRoll ) const;
    EulerResult ExtractZXY( Real & outYaw, Real & outPitch, Real & outRoll ) const;
    EulerResult ExtractZYX( Real & outYaw, Real & outPitch, Real & outRoll ) const;
        // UDU Decompositions
    // Need proper re-ordering ... !
    //EulerResult ExtractXYX( Real & outYaw, Real & outPitch, Real & outRoll ) const;
    //EulerResult ExtractXZX( Real & outYaw, Real & outPitch, Real & outRoll ) const;
    //EulerResult ExtractYXY( Real & outYaw, Real & outPitch, Real & outRoll ) const;
    //EulerResult ExtractYZY( Real & outYaw, Real & outPitch, Real & outRoll ) const;
    //EulerResult ExtractZXZ( Real & outYaw, Real & outPitch, Real & outRoll ) const;
    //EulerResult ExtractZYZ( Real & outYaw, Real & outPitch, Real & outRoll ) const;

private:
    static const Real ZERO;
    static const Real ONE;
    static const Real PI_2;
    static const Real PI;

    TMatrix3<Real> m_matRotation;
};

// Explicit instanciation
typedef TEulerDecomposition<Float> EulerDecompositionf;
typedef TEulerDecomposition<Double> EulerDecompositiond;
typedef TEulerDecomposition<Scalar> EulerDecomposition;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "EulerDecomposition.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_ALGEBRA_MATRIX_EULERDECOMPOSITION_H

