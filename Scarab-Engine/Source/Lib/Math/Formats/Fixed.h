/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Formats/Fixed.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Fixed provides fixed point arithmetic support
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
#ifndef SCARAB_LIB_MATH_FORMATS_FIXED_H
#define SCARAB_LIB_MATH_FORMATS_FIXED_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Scalar.h"

/////////////////////////////////////////////////////////////////////////////////
// Conditional Compilation Settings
	// Fixed Point position, default is 16.16
//#define MATH_FIXED_8_24
#define MATH_FIXED_16_16
//#define MATH_FIXED_24_8

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#if defined (MATH_FIXED_8_24)
	#define FIXED_FRACT_BITS		24	// # of bits right of the point
	#define FIXED_EPSILON			(Fixed(0.0000007f)) // untested
#elif defined (MATH_FIXED_16_16)
	#define FIXED_FRACT_BITS		16	// # of bits right of the point
	#define FIXED_EPSILON			(Fixed(0.00007f))
#elif defined (MATH_FIXED_24_8)
	#define FIXED_FRACT_BITS		8	// # of bits right of the point
	#define FIXED_EPSILON			(Fixed(0.007f)) // untested
#endif

#define FIXED_MIN      ( -(1 << (32 - FIXED_FRACT_BITS - 1)) )
#define FIXED_MAX      ( (1 << (32 - FIXED_FRACT_BITS - 1)) - 1 )
#define FIXED_INFINITE FIXED_MAX

/////////////////////////////////////////////////////////////////////////////////
// The Fixed class
class Fixed
{
public:
    static const Fixed Zero;
    static const Fixed One;
    static const Fixed Infinity;

	// constructors & destructor
	inline Fixed(); // uninitialized
	inline Fixed(const Fixed & rhs);
    inline Fixed(UChar iValue);
	inline Fixed(SChar iValue);
	inline Fixed(UShort iValue);
	inline Fixed(SShort iValue);
	inline Fixed(ULong iValue);
	inline Fixed(SLong iValue);
	inline Fixed(UInt iValue);
	inline Fixed(SInt iValue);
	inline Fixed(Float fValue);
	inline Fixed(Double fValue);
    inline ~Fixed() {}

	// operators : casts
	inline operator UChar() const;
	inline operator SChar() const;
    inline operator UShort() const;
	inline operator SShort() const;
	inline operator ULong() const;
	inline operator SLong() const;
	inline operator UInt() const;
	inline operator SInt() const;
	inline operator Float() const;
	inline operator Double() const;

	// operators : affectations
	inline Fixed & operator=(const Fixed & rhs);

	// operators : shifts
	inline Fixed operator>>(const Int rhs) const;
	inline Fixed operator<<(const Int rhs) const;
	inline Fixed & operator>>=(const Int rhs);
	inline Fixed & operator<<=(const Int rhs);

	// operators : arithmetic, unary
	inline Fixed operator+() const;
	inline Fixed operator-() const;

	// operators : arithmetic, binary
	inline Fixed operator+(const Fixed & rhs) const;
	inline Fixed operator-(const Fixed & rhs) const;
	inline Fixed operator*(const Fixed & rhs) const;
	inline Fixed operator/(const Fixed & rhs) const;

	// operators : arithmetic & affect, binary
	inline Fixed & operator+=(const Fixed & rhs);
	inline Fixed & operator-=(const Fixed & rhs);
	inline Fixed & operator*=(const Fixed & rhs);
	inline Fixed & operator/=(const Fixed & rhs);

	// operators : booleans
	inline Bool operator==(const Fixed & rhs) const;
	inline Bool operator!=(const Fixed & rhs) const;
	inline Bool operator<=(const Fixed & rhs) const;
	inline Bool operator>=(const Fixed & rhs) const;
	inline Bool operator<(const Fixed & rhs) const;
	inline Bool operator>(const Fixed & rhs) const;

    // Functions
    inline static Fixed Abs(const Fixed & fpValue);
    static Fixed Sqrt(const Fixed & fpValue);
    static Fixed ArcTan2(const Fixed & fpNum, const Fixed & fpDenom);

private:
	// dummyarg needed to avoid conflict with public Fixed(Int)
	static const Int RAW; // dumb value
	inline Fixed(Int dummyarg, Int fpValue);

	static const Int FP; // fixed point bit position
	static const Int FP_POW; // (1 << FP) = 2^FP
	static const Double FP_INVPOW; // 1 / FP_POW
	static const Int FP_DBL; // (FP << 1) = FP * 2
	static const Int FP_HALF; // (FP >> 1) = FP / 2

	Int m_fpValue;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Fixed.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_FORMATS_FIXED_H
