/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Algebra/Quaternion/ConstrainedQuaternion.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Euler closest-orientation solver for constrained quaternions
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
#ifndef SCARAB_LIB_MATH_ALGEBRA_QUATERNION_CONSTRAINEDQUATERNION_H
#define SCARAB_LIB_MATH_ALGEBRA_QUATERNION_CONSTRAINEDQUATERNION_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Types/Quaternion/Quaternion.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TConstrainedQuaternion class
template<typename Real>
class TConstrainedQuaternion
{
public:
    // Constraint descriptor
    typedef struct _angle_constraint
    {
        Real fMinAngle; // in [-PI/2,PI/2]
        Real fMaxAngle; // in [fMinAngle/2,PI/2]
        Real fCosMin;   // = cos(fMinAngle)
        Real fSinMin;   // = sin(fMinAngle)
        Real fCosMax;   // = cos(fMaxAngle)
        Real fSinMax;   // = sin(fMaxAngle)
        Real fDeltaCos; // = cos(m_fMaxAngle) - cos(m_fMinAngle)
        Real fDeltaSin; // = sin(m_fMaxAngle) - sin(m_fMinAngle)
        Real fCosAvg;   // = cos((fMinAngle + fMaxAngle)/2)
        Real fSinAvg;   // = sin((fMinAngle + fMaxAngle)/2)

        Void SetValues( Real fMin, Real fMax );
        Bool IsValid( Real fX, Real fY ) const; // (X,Y) must be unit-length
    } AngleConstraint;

public:
    TConstrainedQuaternion( const TQuaternion<Real> & qOrientation, Real fZeroTolerance = (Real)SCALAR_ERROR );
    ~TConstrainedQuaternion();

    // Precision management
    Real ZeroTolerance;

    // We want to solve the closest orientation along any set of given axes
    // for the given orientation, with any possible constraint ...
    inline Void ClosestX( TQuaternion<Real> & outClosest, const AngleConstraint & constraintX ) const;
    inline Void ClosestY( TQuaternion<Real> & outClosest, const AngleConstraint & constraintY ) const;
    inline Void ClosestZ( TQuaternion<Real> & outClosest, const AngleConstraint & constraintZ ) const;

    inline Void ClosestXY( TQuaternion<Real> & outClosest,
                           const AngleConstraint & constraintX, const AngleConstraint & constraintY ) const;
    inline Void ClosestXZ( TQuaternion<Real> & outClosest,
                           const AngleConstraint & constraintX, const AngleConstraint & constraintZ ) const;
    inline Void ClosestYX( TQuaternion<Real> & outClosest,
                           const AngleConstraint & constraintY, const AngleConstraint & constraintX ) const;
    inline Void ClosestYZ( TQuaternion<Real> & outClosest,
                           const AngleConstraint & constraintY, const AngleConstraint & constraintZ ) const;
    inline Void ClosestZX( TQuaternion<Real> & outClosest,
                           const AngleConstraint & constraintZ, const AngleConstraint & constraintX ) const;
    inline Void ClosestZY( TQuaternion<Real> & outClosest,
                           const AngleConstraint & constraintZ, const AngleConstraint & constraintY ) const;

private:
    // Closest constrained quaternion of the form:
    //   (cx + sx*i) when iAxis = AXIS_X,
    //   (cy + sy*j) when iAxis = AXIS_Y,
    //   (cz + sz*k) when iAxis = AXIS_Z
    Void _SolveClosest( TQuaternion<Real> & outClosest, const TQuaternion<Real> & qOrientation,
                        Axis iAxis, const AngleConstraint & constraintAxis  ) const;

    // Closest constrained quaternion of the form:
    //   (cT + sT*i) + (cU + sU*j)
    Void _SolveClosestXY( TQuaternion<Real> & outClosest, const TQuaternion<Real> & qOrientation,
                          const AngleConstraint & constraintX, const AngleConstraint & constraintY ) const;
    Void _SolveClosestZX( TQuaternion<Real> & outClosest, const TQuaternion<Real> & qOrientation,
                          const AngleConstraint & constraintZ, const AngleConstraint & constraintX ) const;
    Void _SolveClosestZY( TQuaternion<Real> & outClosest, const TQuaternion<Real> & qOrientation,
                          const AngleConstraint & constraintZ, const AngleConstraint & constraintY ) const;

    static const Real ZERO;
    static const Real ONE;
    static const Real HALF;

    TQuaternion<Real> m_qOrientation;
};

// Explicit instanciation
typedef TConstrainedQuaternion<Float> ConstrainedQuaternionf;
typedef TConstrainedQuaternion<Double> ConstrainedQuaterniond;
typedef TConstrainedQuaternion<Scalar> ConstrainedQuaternion;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ConstrainedQuaternion.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_ALGEBRA_QUATERNION_CONSTRAINEDQUATERNION_H

