/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Discrete/Offset3.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Discrete 3D displacement
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
#ifndef SCARAB_LIB_MATH_TYPES_DISCRETE_OFFSET3_H
#define SCARAB_LIB_MATH_TYPES_DISCRETE_OFFSET3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Functions/MathFunction.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The Offset3 class
class Offset3
{
public:
    // Constant values
    static const Offset3 Null; // Null Point
    static const Offset3 eI;   // Canonical I
    static const Offset3 eJ;   // Canonical J
    static const Offset3 eK;   // Canonical K

	// Constructors
	Offset3(); // uninitialized
	Offset3(Int x, Int y, Int z);
	Offset3(const Int vArr[3]);
	Offset3(const Offset3 & rhs);
    ~Offset3();

	// Assignment operator
	inline Offset3 & operator=(const Offset3 & rhs);

	// Casting operations
	inline operator Int*() const;
	inline operator const Int*() const;

	// Index operations
	inline Int & operator[](Int i);
    inline Int operator[](Int i) const;
	inline Int & operator[](UInt i);
	inline Int operator[](UInt i) const;

	// Unary operations
	inline Offset3 operator+() const;
	inline Offset3 operator-() const;

	// Boolean operations
	inline Bool operator==(const Offset3 & rhs) const;
	inline Bool operator!=(const Offset3 & rhs) const;

    // Scalar operations
	inline Offset3 operator+(Int rhs) const;
	inline Offset3 operator-(Int rhs) const;
	inline Offset3 operator*(Int rhs) const;
	inline Offset3 operator/(Int rhs) const;

	inline Offset3 & operator+=(Int rhs);
	inline Offset3 & operator-=(Int rhs);
	inline Offset3 & operator*=(Int rhs);
	inline Offset3 & operator/=(Int rhs);

	// Offset operations
	inline Offset3 operator+(const Offset3 & rhs) const;
	inline Offset3 operator-(const Offset3 & rhs) const;

	inline Offset3 & operator+=(const Offset3 & rhs);
	inline Offset3 & operator-=(const Offset3 & rhs);

	inline Int operator*(const Offset3 & rhs) const; // DOT Product
	inline Offset3 operator^(const Offset3 & rhs) const; // CROSS Product

	// Methods
	inline Scalar Norm() const; // Distance to Origin
	inline Int NormSqr() const; // Squared Distance to Origin
    inline Scalar InvNorm() const;
    inline Scalar InvNormSqr() const;
    inline Scalar Normalize();

	inline Offset3 ProjectToNormal(const Offset3 & vNormal) const; // vNormal must be normalized
	inline Offset3 ProjectToPlane(const Offset3 & vNormal) const;  // vNormal must be normalized

    static Void OrthoNormalize( Offset3 & vI, Offset3 & vJ, Offset3 & vK );
    static Void MakeComplementBasis( Offset3 & vI, Offset3 & vJ, const Offset3 & vK ); // vK must be normalized

	// Data
	union {
		struct {
			Int X,Y,Z;
		};
		Int Values[3];
	};
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Offset3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_TYPES_DISCRETE_OFFSET3_H
