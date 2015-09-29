/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Discrete/Point3.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Discrete 3D position
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
#ifndef SCARAB_LIB_MATH_TYPES_DISCRETE_POINT3_H
#define SCARAB_LIB_MATH_TYPES_DISCRETE_POINT3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Offset3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The Point3 class
class Point3
{
public:
    // Constant values
    static const Point3 Null; // Null Point

	// Constructors
	Point3(); // uninitialized
	Point3(Int x, Int y, Int z);
	Point3(const Int vArr[3]);
	Point3(const Point3 & rhs);
    ~Point3();

	// Assignment operator
	inline Point3 & operator=(const Point3 & rhs);

	// Casting operations
	inline operator Int*() const;
	inline operator const Int*() const;

	// Index operations
	inline Int & operator[](Int i);
    inline Int operator[](Int i) const;
	inline Int & operator[](UInt i);
	inline Int operator[](UInt i) const;

	// Unary operations
	inline Point3 operator+() const;
	inline Point3 operator-() const;

	// Boolean operations
	inline Bool operator==(const Point3 & rhs) const;
	inline Bool operator!=(const Point3 & rhs) const;

    // Scalar operations
	inline Point3 operator+(Int rhs) const;
	inline Point3 operator-(Int rhs) const;
	inline Point3 operator*(Int rhs) const;
	inline Point3 operator/(Int rhs) const;

	inline Point3 & operator+=(Int rhs);
	inline Point3 & operator-=(Int rhs);
	inline Point3 & operator*=(Int rhs);
	inline Point3 & operator/=(Int rhs);

	// Point operations
	inline Offset3 operator-(const Point3 & rhs) const;

    // Offset operations
	inline Point3 operator+(const Offset3 & rhs) const;
	inline Point3 operator-(const Offset3 & rhs) const;

	inline Point3 & operator+=(const Offset3 & rhs);
	inline Point3 & operator-=(const Offset3 & rhs);

	// Methods
	inline Scalar Dist() const; // Distance to Origin
	inline Int DistSqr() const; // Squared Distance to Origin
    inline Scalar InvDist() const;
    inline Scalar InvDistSqr() const;

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
#include "Point3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_TYPES_DISCRETE_POINT3_H
