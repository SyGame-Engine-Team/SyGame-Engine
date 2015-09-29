/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Discrete/Point2.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Discrete 2D position
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
#ifndef SCARAB_LIB_MATH_TYPES_DISCRETE_POINT2_H
#define SCARAB_LIB_MATH_TYPES_DISCRETE_POINT2_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Offset2.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The Point2 class
class Point2
{
public:
    // Constant values
    static const Point2 Null; // Null Point

	// Constructors
	Point2(); // uninitialized
	Point2(Int x, Int y);
	Point2(const Int vArr[2]);
	Point2(const Point2 & rhs);
    ~Point2();

	// Assignment operator
	inline Point2 & operator=(const Point2 & rhs);

	// Casting operations
	inline operator Int*() const;
	inline operator const Int*() const;

	// Index operations
	inline Int & operator[](Int i);
    inline Int operator[](Int i) const;
	inline Int & operator[](UInt i);
	inline Int operator[](UInt i) const;

	// Unary operations
	inline Point2 operator+() const;
	inline Point2 operator-() const;

	// Boolean operations
	inline Bool operator==(const Point2 & rhs) const;
	inline Bool operator!=(const Point2 & rhs) const;

    // Scalar operations
	inline Point2 operator+(Int rhs) const;
	inline Point2 operator-(Int rhs) const;
	inline Point2 operator*(Int rhs) const;
	inline Point2 operator/(Int rhs) const;

	inline Point2 & operator+=(Int rhs);
	inline Point2 & operator-=(Int rhs);
	inline Point2 & operator*=(Int rhs);
	inline Point2 & operator/=(Int rhs);

	// Point operations
	inline Offset2 operator-(const Point2 & rhs) const;

    // Offset operations
	inline Point2 operator+(const Offset2 & rhs) const;
	inline Point2 operator-(const Offset2 & rhs) const;

	inline Point2 & operator+=(const Offset2 & rhs);
	inline Point2 & operator-=(const Offset2 & rhs);

	// Methods
	inline Scalar Dist() const; // Distance to Origin
	inline Int DistSqr() const; // Squared Distance to Origin
    inline Scalar InvDist() const;
    inline Scalar InvDistSqr() const;

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
#include "Point2.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_TYPES_DISCRETE_POINT2_H
