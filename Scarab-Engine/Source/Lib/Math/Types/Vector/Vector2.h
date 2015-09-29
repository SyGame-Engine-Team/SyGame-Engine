/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Vector/Vector2.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : 2D vector
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
#ifndef SCARAB_LIB_MATH_TYPES_VECTOR_VECTOR2_H
#define SCARAB_LIB_MATH_TYPES_VECTOR_VECTOR2_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Functions/MathFunction.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
    // prototypes
template<typename Real>
class TVector3;
template<typename Real>
class TVector4;

/////////////////////////////////////////////////////////////////////////////////
// The TVector2 class
template<typename Real>
class TVector2
{
public:
    // Constant values
    static const TVector2<Real> Null; // Null vector
    static const TVector2<Real> eI;   // Canonical I
    static const TVector2<Real> eJ;   // Canonical J

	// Constructors
	TVector2(); // uninitialized
	TVector2(Real x, Real y);
	TVector2(const Real vArr[2]);
	TVector2(const TVector2<Real> & rhs);
	TVector2(const TVector3<Real> & rhs);
	TVector2(const TVector4<Real> & rhs);
	~TVector2();

	// Assignment operator
	inline TVector2<Real> & operator=(const TVector2<Real> & rhs);

	// Casting operations
	inline operator Real*() const;
    inline operator const Real*() const;

	// Index operations
    inline Real & operator[](Int i);
	inline Real operator[](Int i) const;
	inline Real & operator[](UInt i);
	inline Real operator[](UInt i) const;

	// Unary operations
	inline TVector2<Real> operator+() const;
	inline TVector2<Real> operator-() const;

	// Boolean operations
	inline Bool operator==(const TVector2<Real> & rhs) const;
	inline Bool operator!=(const TVector2<Real> & rhs) const;

	// Real operations
	inline TVector2<Real> operator+(Real rhs) const;
	inline TVector2<Real> operator-(Real rhs) const;
	inline TVector2<Real> operator*(Real rhs) const;
	inline TVector2<Real> operator/(Real rhs) const;

	inline TVector2<Real> & operator+=(Real rhs);
	inline TVector2<Real> & operator-=(Real rhs);
	inline TVector2<Real> & operator*=(Real rhs);
	inline TVector2<Real> & operator/=(Real rhs);

	// Vector operations
	inline TVector2<Real> operator+(const TVector2<Real> & rhs) const;
	inline TVector2<Real> operator-(const TVector2<Real> & rhs) const;

	inline TVector2<Real> & operator+=(const TVector2<Real> & rhs);
	inline TVector2<Real> & operator-=(const TVector2<Real> & rhs);

	inline Real operator*(const TVector2<Real> & rhs) const; // DOT Product
	inline Real operator^(const TVector2<Real> & rhs) const; // CROSS Product

	// Methods
    inline Real NormSqr() const;
	inline Real Norm() const;
	inline Real InvNormSqr() const;
    inline Real InvNorm() const;
	inline Real Normalize();

  	inline TVector2<Real> Perp() const;

	inline TVector2<Real> ProjectToNormal(const TVector2<Real> & vNormal) const; // vNormal must be normalized
	inline TVector2<Real> ProjectToPlane(const TVector2<Real> & vNormal) const;  // vNormal must be normalized

    static Void OrthoNormalize( TVector2<Real> & vI, TVector2<Real> & vJ );
    inline static Void MakeComplementBasis( TVector2<Real> & vI, const TVector2<Real> & vJ ); // vJ must be normalized

	// Data
	Real X,Y;
};

// Explicit instanciation
typedef TVector2<Float> Vector2f;
typedef TVector2<Double> Vector2d;
typedef TVector2<Scalar> Vector2;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Vector2.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_TYPES_VECTOR_VECTOR2_H
