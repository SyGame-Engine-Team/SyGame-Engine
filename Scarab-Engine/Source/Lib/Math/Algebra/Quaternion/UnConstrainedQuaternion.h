/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Algebra/Quaternion/UnConstrainedQuaternion.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Euler closest-orientation solver for unconstrained quaternions
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
#ifndef SCARAB_LIB_MATH_ALGEBRA_QUATERNION_UNCONSTRAINEDQUATERNION_H
#define SCARAB_LIB_MATH_ALGEBRA_QUATERNION_UNCONSTRAINEDQUATERNION_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Types/Vector/Vector2.h"
#include "../../Types/Quaternion/Quaternion.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TUnConstrainedQuaternion class
template<typename Real>
class TUnConstrainedQuaternion
{
public:
    TUnConstrainedQuaternion( const TQuaternion<Real> & qOrientation, Real fZeroTolerance = (Real)SCALAR_ERROR );
    ~TUnConstrainedQuaternion();

    // Precision management
    Real ZeroTolerance;

    // We want to solve the closest orientation along any set of given axes
    // for the given orientation, without any constraint ...
    inline Void ClosestX( TQuaternion<Real> & outClosest ) const;
    inline Void ClosestY( TQuaternion<Real> & outClosest ) const;
    inline Void ClosestZ( TQuaternion<Real> & outClosest ) const;

    inline Void ClosestXY( TQuaternion<Real> & outClosest ) const;
    inline Void ClosestXZ( TQuaternion<Real> & outClosest ) const;
    inline Void ClosestYX( TQuaternion<Real> & outClosest ) const;
    inline Void ClosestYZ( TQuaternion<Real> & outClosest ) const;
    inline Void ClosestZX( TQuaternion<Real> & outClosest ) const;
    inline Void ClosestZY( TQuaternion<Real> & outClosest ) const;

    // Factorizations ( complex results contain (cos, sin) couples )
    inline Void FactorizeXYZ( TVector2<Real> & outComplexX, TVector2<Real> & outComplexY, TVector2<Real> & outComplexZ ) const;
    inline Void FactorizeXZY( TVector2<Real> & outComplexX, TVector2<Real> & outComplexZ, TVector2<Real> & outComplexY ) const;
    inline Void FactorizeYXZ( TVector2<Real> & outComplexY, TVector2<Real> & outComplexX, TVector2<Real> & outComplexZ ) const;
    inline Void FactorizeYZX( TVector2<Real> & outComplexY, TVector2<Real> & outComplexZ, TVector2<Real> & outComplexX ) const;
    inline Void FactorizeZXY( TVector2<Real> & outComplexZ, TVector2<Real> & outComplexX, TVector2<Real> & outComplexY ) const;
    inline Void FactorizeZYX( TVector2<Real> & outComplexZ, TVector2<Real> & outComplexY, TVector2<Real> & outComplexX ) const;

private:
    // Closest unconstrained quaternion of the form:
    //   (cx + sx*i) when iAxis = AXIS_X,
    //   (cy + sy*j) when iAxis = AXIS_Y,
    //   (cz + sz*k) when iAxis = AXIS_Z
    Void _SolveClosest( TQuaternion<Real> & outClosest, Axis iAxis ) const;

    // Closest unconstrained quaternion of the form:
    //   (cx + sx*i) + (cy + sy*j)
    Void _SolveClosestXY( TQuaternion<Real> & outClosest, const TQuaternion<Real> & qOrientation ) const;

    // Factorize to (cx + sx*i) * (cy + sy*j) * (cz + sz*k)
    Void _SolveFactorsXYZ( TVector2<Real> & outComplexX, TVector2<Real> & outComplexY, TVector2<Real> & outComplexZ,
                           const TQuaternion<Real> & qOrientation ) const;

    static const Real ZERO;
    static const Real ONE;
    static const Real HALF;

    TQuaternion<Real> m_qOrientation;
};

// Explicit instanciation
typedef TUnConstrainedQuaternion<Float> UnConstrainedQuaternionf;
typedef TUnConstrainedQuaternion<Double> UnConstrainedQuaterniond;
typedef TUnConstrainedQuaternion<Scalar> UnConstrainedQuaternion;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "UnConstrainedQuaternion.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_ALGEBRA_QUATERNION_UNCONSTRAINEDQUATERNION_H

