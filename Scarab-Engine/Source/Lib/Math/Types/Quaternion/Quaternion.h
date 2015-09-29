/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Quaternion/Quaternion.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Homogeneous 4D quaternion
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : A quaternion is a complex 4D object such that :
//                            q = x*i + y*j + z*k + w
//              with i,j,k traditionnal complex roots of -1, (x,y,z,w) does not
//              have to be unit length.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_LIB_MATH_TYPES_QUATERNION_QUATERNION_H
#define SCARAB_LIB_MATH_TYPES_QUATERNION_QUATERNION_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Vector/Vector3.h"
#include "../Vector/Vector4.h"
#include "../Matrix/Matrix3.h"
#include "../Matrix/Matrix4.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TQuaternion class
template<typename Real>
class TQuaternion
{
public:
    // Constant values
    static const TQuaternion<Real> Null; // Null quaternion
    static const TQuaternion<Real> Identity; // Identity quaternion

	// Constructors
	TQuaternion(); // uninitialized
	TQuaternion(Real x, Real y, Real z, Real w);
	TQuaternion(const Real vArr[4]);
    TQuaternion(const TVector4<Real> & vQuaternion);
	TQuaternion(const TQuaternion<Real> & rhs);
    TQuaternion(const TVector3<Real> & vAxis, Real fAngle);
	TQuaternion(const TMatrix3<Real> & matRotation);
	~TQuaternion();

	// Assignment operator
	inline TQuaternion<Real> & operator=(const TQuaternion<Real> & rhs);

	// Casting operations
	inline operator Real*() const;
	inline operator const Real*() const;

	// Index operations
	inline Real & operator[](Int i);
    inline Real operator[](Int i) const;
	inline Real & operator[](UInt i);
	inline Real operator[](UInt i) const;

	// Unary operations
	inline TQuaternion<Real> operator+() const;
	inline TQuaternion<Real> operator-() const;
	inline TQuaternion<Real> operator~() const; // Conjugate

	// Boolean operations
	inline Bool operator==(const TQuaternion<Real> & rhs) const;
	inline Bool operator!=(const TQuaternion<Real> & rhs) const;

	// Real operations
	inline TQuaternion<Real> operator*(Real rhs) const;
	inline TQuaternion<Real> operator/(Real rhs) const;

	inline TQuaternion<Real> & operator*=(Real rhs);
	inline TQuaternion<Real> & operator/=(Real rhs);

	// Vector operations
	inline TVector3<Real> operator*(const TVector3<Real> & rhs) const; // Transform vector
	inline TVector4<Real> operator*(const TVector4<Real> & rhs) const; // Transform vector

	// TQuaternion operations
	inline TQuaternion<Real> operator+(const TQuaternion<Real> & rhs) const;
	inline TQuaternion<Real> operator-(const TQuaternion<Real> & rhs) const;
	TQuaternion<Real> operator*(const TQuaternion<Real> & rhs) const; // Composition

	inline TQuaternion<Real> & operator+=(const TQuaternion<Real> & rhs);
	inline TQuaternion<Real> & operator-=(const TQuaternion<Real> & rhs);
    TQuaternion<Real> & operator*=(const TQuaternion<Real> & rhs); // Composition

	// Representations
    inline Void GetAxisAngle(TVector3<Real> & outAxis, Real & outAngle) const;
    inline Void GetMatrix(TMatrix3<Real> & outRotation) const;

    // Methods
    inline Real NormSqr() const;
	inline Real Norm() const;
    inline Real InvNormSqr() const;
    inline Real InvNorm() const;
    inline Real Normalize();

        // Those select between qTarget and -qTarget returning the nearest/farthest one,
        // thus giving the side to use to align this with qTarget with wanted behaviour.
    inline Void Nearest( TQuaternion<Real> & qOut, const TQuaternion<Real> & qTarget ) const;
    inline Void Farthest( TQuaternion<Real> & qOut, const TQuaternion<Real> & qTarget ) const;

    inline Real Dot(const TQuaternion<Real> & rhs) const;

	Void Invert(TQuaternion<Real> & qOut) const;
	inline Void InvertN(TQuaternion<Real> & qOut) const;

	Void Exp(TQuaternion<Real> & qOut) const;
	Void Ln(TQuaternion<Real> & qOut) const;

    // From euler angles
    Void MakeRotate(Real fYaw, Real fPitch, Real fRoll, EulerAngles eulerAnglesOrder);

    // Creates the quaternion which rotates V1 into V2, has
    // rotation axis perpendicular to V1 and V2 and angle
    // of that between V1 and V2 (that's our canonical transform).
    // vV1 & vV2 MUST be normalized.
    Void MakeRotate(const TVector3<Real> & vV1, const TVector3<Real> & vV2);

    // Split this quaternion into q = qTwist * qSwing or q = qSwing * qTwist.
    // Let V1 be an input vector, and V2 = q * V1, qSwing represents a rotation
    // about the axis perpendicular to V1 and V2, and qTwist a rotation about V1.
    inline Void SplitTwistSwing(const TVector3<Real> & vV1, TQuaternion<Real> & outTwist, TQuaternion<Real> & outSwing);
    inline Void SplitSwingTwist(const TVector3<Real> & vV1, TQuaternion<Real> & outSwing, TQuaternion<Real> & outTwist);

    // Spherical interpolation
    Void SLerp(const TQuaternion<Real> & qSource, const TQuaternion<Real> & qTarget, Real fT);
    Void SLerpSpins(const TQuaternion<Real> & qSource, const TQuaternion<Real> & qTarget, Real fT, Int iSpins);
    Void SQuad(const TQuaternion<Real> & qQ0, const TQuaternion<Real> & qA0,
               const TQuaternion<Real> & qA1, const TQuaternion<Real> & qQ1, Real fT) const;
    Void SQuadIntermediate(const TQuaternion<Real> & qQ0, const TQuaternion<Real> & qQ1, const TQuaternion<Real> & qQ2);

	// Data
	Real X,Y,Z,W;

private:
    // Helpers
    Void _Quaternion_To_AxisAngle(TVector3<Real> & outAxis, Real & outAngle) const;
    Void _Quaternion_To_Matrix(TMatrix3<Real> & outRot) const;
    Void _AxisAngle_To_Quaternion(const TVector3<Real> & vAxis, Real fAngle);
    Void _Matrix_To_Quaternion(const TMatrix3<Real> & matRot);

    static const Real ZERO;
    static const Real ONE;
    static const Real TWO;
};

// Explicit instanciation
typedef TQuaternion<Float> Quaternionf;
typedef TQuaternion<Double> Quaterniond;
typedef TQuaternion<Scalar> Quaternion;

// Specializations
template<>
TQuaternion<Float> TQuaternion<Float>::operator*(const TQuaternion<Float> & rhs) const;
template<>
TQuaternion<Double> TQuaternion<Double>::operator*(const TQuaternion<Double> & rhs) const;

template<>
TQuaternion<Float> & TQuaternion<Float>::operator*=(const TQuaternion<Float> & rhs);
template<>
TQuaternion<Double> & TQuaternion<Double>::operator*=(const TQuaternion<Double> & rhs);

template<>
Void TQuaternion<Float>::_Quaternion_To_Matrix(TMatrix3<Float> & outRot) const;
template<>
Void TQuaternion<Double>::_Quaternion_To_Matrix(TMatrix3<Double> & outRot) const;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Quaternion.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_TYPES_QUATERNION_QUATERNION_H


