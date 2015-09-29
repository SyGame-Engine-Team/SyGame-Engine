/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Vector/Vector3.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : 3D vector
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_LIB_MATH_TYPES_VECTOR_VECTOR3_H
#define SCARAB_LIB_MATH_TYPES_VECTOR_VECTOR3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Functions/MathFunction.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
    // prototypes
template<typename Real>
class TVector2;
template<typename Real>
class TVector4;

/////////////////////////////////////////////////////////////////////////////////
// The TVector3 class
template<typename Real>
class TVector3
{
public:
    // Constant values
    static const TVector3<Real> Null; // Null vector
    static const TVector3<Real> eI;   // Canonical I
    static const TVector3<Real> eJ;   // Canonical J
    static const TVector3<Real> eK;   // Canonical K

	// Constructors
	TVector3(); // uninitialized
	TVector3(Real x, Real y, Real z);
	TVector3(const Real vArr[3]);
	TVector3(const TVector2<Real> & rhs);
	TVector3(const TVector3<Real> & rhs);
	TVector3(const TVector4<Real> & rhs);
	~TVector3();

	// Assignment operator
	inline TVector3<Real> & operator=(const TVector3<Real> & rhs);

	// Casting operations
	inline operator Real*() const;
    inline operator const Real*() const;

	// Index operations
    inline Real & operator[](Int i);
	inline Real operator[](Int i) const;
	inline Real & operator[](UInt i);
	inline Real operator[](UInt i) const;

	// Unary operations
	inline TVector3<Real> operator+() const;
	inline TVector3<Real> operator-() const;

	// Boolean operations
	inline Bool operator==(const TVector3<Real> & rhs) const;
	inline Bool operator!=(const TVector3<Real> & rhs) const;

	// Real operations
	inline TVector3<Real> operator+(Real rhs) const;
	inline TVector3<Real> operator-(Real rhs) const;
	inline TVector3<Real> operator*(Real rhs) const;
	inline TVector3<Real> operator/(Real rhs) const;

	inline TVector3<Real> & operator+=(Real rhs);
	inline TVector3<Real> & operator-=(Real rhs);
	inline TVector3<Real> & operator*=(Real rhs);
	inline TVector3<Real> & operator/=(Real rhs);

	// Vector operations
	inline TVector3<Real> operator+(const TVector3<Real> & rhs) const;
	inline TVector3<Real> operator-(const TVector3<Real> & rhs) const;

	inline TVector3<Real> & operator+=(const TVector3<Real> & rhs);
	inline TVector3<Real> & operator-=(const TVector3<Real> & rhs);

	inline Real operator*(const TVector3<Real> & rhs) const; // DOT Product
	inline TVector3<Real> operator^(const TVector3<Real> & rhs) const; // CROSS Product

	// Methods
    inline Real NormSqr() const;
	inline Real Norm() const;
	inline Real InvNormSqr() const;
    inline Real InvNorm() const;
	inline Real Normalize();

	inline TVector3<Real> ProjectToNormal(const TVector3<Real> & vNormal) const; // vNormal must be normalized
	inline TVector3<Real> ProjectToPlane(const TVector3<Real> & vNormal) const;  // vNormal must be normalized

    static Void OrthoNormalize( TVector3<Real> & vI, TVector3<Real> & vJ, TVector3<Real> & vK );
    static Void MakeComplementBasis( TVector3<Real> & vI, TVector3<Real> & vJ, const TVector3<Real> & vK ); // vK must be normalized

	// Data
	Real X,Y,Z;
};

// Explicit instanciation
typedef TVector3<Float> Vector3f;
typedef TVector3<Double> Vector3d;
typedef TVector3<Scalar> Vector3;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Vector3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_TYPES_VECTOR_VECTOR3_H
