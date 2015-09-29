/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Vector/Vector4.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Homogeneous 4D vector
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
#ifndef SCARAB_LIB_MATH_TYPES_VECTOR_VECTOR4_H
#define SCARAB_LIB_MATH_TYPES_VECTOR_VECTOR4_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Functions/MathFunction.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
    // prototypes
template<typename Real>
class TVector2;
template<typename Real>
class TVector3;

/////////////////////////////////////////////////////////////////////////////////
// The TVector4 class
template<typename Real>
class TVector4
{
public:
    // Constant values
    static const TVector4<Real> Null; // Null vector
    static const TVector4<Real> eI;   // Canonical I
    static const TVector4<Real> eJ;   // Canonical J
    static const TVector4<Real> eK;   // Canonical K

	// Constructors
	TVector4(); // uninitialized
	TVector4(Real x, Real y, Real z, Real w = (Real)0);
	TVector4(const Real vArr[4]);
	TVector4(const TVector2<Real> & rhs);
	TVector4(const TVector3<Real> & rhs);
	TVector4(const TVector4<Real> & rhs);
	~TVector4();

	// Assignment operator
	inline TVector4<Real> & operator=(const TVector4<Real> & rhs);

	// Casting operations
	inline operator Real*() const;
    inline operator const Real*() const;

	// Index operations
    inline Real & operator[](Int i);
	inline Real operator[](Int i) const;
	inline Real & operator[](UInt i);
	inline Real operator[](UInt i) const;

	// Unary operations
	inline TVector4<Real> operator+() const;
	inline TVector4<Real> operator-() const;

	// Boolean operations
	inline Bool operator==(const TVector4<Real> & rhs) const;
	inline Bool operator!=(const TVector4<Real> & rhs) const;

	// Real operations
	inline TVector4<Real> operator+(Real rhs) const;
	inline TVector4<Real> operator-(Real rhs) const;
	inline TVector4<Real> operator*(Real rhs) const;
	inline TVector4<Real> operator/(Real rhs) const;

	inline TVector4<Real> & operator+=(Real rhs);
	inline TVector4<Real> & operator-=(Real rhs);
	inline TVector4<Real> & operator*=(Real rhs);
	inline TVector4<Real> & operator/=(Real rhs);

	// Vector operations
	inline TVector4<Real> operator+(const TVector4<Real> & rhs) const;
	inline TVector4<Real> operator-(const TVector4<Real> & rhs) const;

	inline TVector4<Real> & operator+=(const TVector4<Real> & rhs);
	inline TVector4<Real> & operator-=(const TVector4<Real> & rhs);

	inline Real operator*(const TVector4<Real> & rhs) const; // DOT Product
	inline TVector4<Real> operator^(const TVector4<Real> & rhs) const; // CROSS Product

	// Methods
    inline Real NormSqr() const;
	inline Real Norm() const;
	inline Real InvNormSqr() const;
    inline Real InvNorm() const;
	inline Real Normalize();

	inline TVector4<Real> ProjectToNormal(const TVector4<Real> & vNormal) const; // vNormal must be normalized
	inline TVector4<Real> ProjectToPlane(const TVector4<Real> & vNormal) const;  // vNormal must be normalized

    static Void OrthoNormalize( TVector4<Real> & vI, TVector4<Real> & vJ, TVector4<Real> & vK );
    static Void MakeComplementBasis( TVector4<Real> & vI, TVector4<Real> & vJ, const TVector4<Real> & vK ); // vK must be normalized

	// Data
	Real X,Y,Z,W;
};

// Explicit instanciation
typedef TVector4<Float> Vector4f;
typedef TVector4<Double> Vector4d;
typedef TVector4<Scalar> Vector4;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Vector4.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_TYPES_VECTOR_VECTOR4_H
