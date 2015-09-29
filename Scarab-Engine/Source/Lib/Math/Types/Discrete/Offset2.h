/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Discrete/Offset2.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Discrete 2D displacement
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
#ifndef SCARAB_LIB_MATH_TYPES_DISCRETE_OFFSET2_H
#define SCARAB_LIB_MATH_TYPES_DISCRETE_OFFSET2_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Functions/MathFunction.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The Offset2 class
class Offset2
{
public:
    // Constant values
    static const Offset2 Null; // Null Point
    static const Offset2 eI;   // Canonical I
    static const Offset2 eJ;   // Canonical J

	// Constructors
	Offset2(); // uninitialized
	Offset2(Int x, Int y);
	Offset2(const Int vArr[2]);
	Offset2(const Offset2 & rhs);
    ~Offset2();

	// Assignment operator
	inline Offset2 & operator=(const Offset2 & rhs);

	// Casting operations
	inline operator Int*() const;
	inline operator const Int*() const;

	// Index operations
	inline Int & operator[](Int i);
    inline Int operator[](Int i) const;
	inline Int & operator[](UInt i);
	inline Int operator[](UInt i) const;

	// Unary operations
	inline Offset2 operator+() const;
	inline Offset2 operator-() const;

	// Boolean operations
	inline Bool operator==(const Offset2 & rhs) const;
	inline Bool operator!=(const Offset2 & rhs) const;

    // Scalar operations
	inline Offset2 operator+(Int rhs) const;
	inline Offset2 operator-(Int rhs) const;
	inline Offset2 operator*(Int rhs) const;
	inline Offset2 operator/(Int rhs) const;

	inline Offset2 & operator+=(Int rhs);
	inline Offset2 & operator-=(Int rhs);
	inline Offset2 & operator*=(Int rhs);
	inline Offset2 & operator/=(Int rhs);

	// Offset operations
	inline Offset2 operator+(const Offset2 & rhs) const;
	inline Offset2 operator-(const Offset2 & rhs) const;

	inline Offset2 & operator+=(const Offset2 & rhs);
	inline Offset2 & operator-=(const Offset2 & rhs);

	inline Int operator*(const Offset2 & rhs) const; // DOT Product

	// Methods
	inline Scalar Norm() const; // Distance to Origin
	inline Int NormSqr() const; // Squared Distance to Origin
    inline Scalar InvNorm() const;
    inline Scalar InvNormSqr() const;
    inline Scalar Normalize();

    inline Offset2 Perp() const; // Counter-clockwise, 2D "Cross-product"

	inline Offset2 ProjectToNormal(const Offset2 & vNormal) const; // vNormal must be normalized
	inline Offset2 ProjectToPlane(const Offset2 & vNormal) const;  // vNormal must be normalized

	// Data
	union {
		struct {
			Int X,Y;
		};
		Int Values[2];
	};
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Offset2.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_TYPES_DISCRETE_OFFSET2_H
