/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Formats/Rational.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Rational provides exact rational arithmetic support
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
#ifndef SCARAB_LIB_MATH_FORMATS_RATIONAL_H
#define SCARAB_LIB_MATH_FORMATS_RATIONAL_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Integer.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The Rational class
template<UInt INT_BYTES>
class Rational
{
public:
    // constructors & destructor
	Rational(); // 0/1
	Rational( const Rational & rhs );
    Rational( const Integer<INT_BYTES> & intNumerator );
    Rational( const Integer<INT_BYTES> & intNumerator, const Integer<INT_BYTES> & intDenominator );
	Rational( Float fValue );
	Rational( Double fValue );
	~Rational();

	// operators : casts
	operator Float() const;
	operator Double() const;

	// operators : affectations
	inline Rational<INT_BYTES> & operator=( const Rational<INT_BYTES> & rhs );
	inline Rational<INT_BYTES> & operator=( const Integer<INT_BYTES> & rhs );

	// operators : arithmetic, unary
	inline Rational<INT_BYTES> operator+() const;
	inline Rational<INT_BYTES> operator-() const;

	// operators : arithmetic, binary
	Rational<INT_BYTES> operator+(const Rational<INT_BYTES> & rhs) const;
	Rational<INT_BYTES> operator+(const Integer<INT_BYTES> & rhs) const;

	Rational<INT_BYTES> operator-(const Rational<INT_BYTES> & rhs) const;
	Rational<INT_BYTES> operator-(const Integer<INT_BYTES> & rhs) const;

	Rational<INT_BYTES> operator*(const Rational<INT_BYTES> & rhs) const;
	Rational<INT_BYTES> operator*(const Integer<INT_BYTES> & rhs) const;

	Rational<INT_BYTES> operator/(const Rational<INT_BYTES> & rhs) const;
	Rational<INT_BYTES> operator/(const Integer<INT_BYTES> & rhs) const;

	// operators : arithmetic & affect, binary
	Rational<INT_BYTES> & operator+=(const Rational<INT_BYTES> & rhs);
	Rational<INT_BYTES> & operator+=(const Integer<INT_BYTES> & rhs);

    Rational<INT_BYTES> & operator-=(const Rational<INT_BYTES> & rhs);
	Rational<INT_BYTES> & operator-=(const Integer<INT_BYTES> & rhs);

    Rational<INT_BYTES> & operator*=(const Rational<INT_BYTES> & rhs);
	Rational<INT_BYTES> & operator*=(const Integer<INT_BYTES> & rhs);

    Rational<INT_BYTES> & operator/=(const Rational<INT_BYTES> & rhs);
	Rational<INT_BYTES> & operator/=(const Integer<INT_BYTES> & rhs);

	// operators : booleans
	inline Bool operator==(const Rational<INT_BYTES> & rhs) const;
	inline Bool operator==(const Integer<INT_BYTES> & rhs) const;

    inline Bool operator!=(const Rational<INT_BYTES> & rhs) const;
	inline Bool operator!=(const Integer<INT_BYTES> & rhs) const;

    Bool operator<=(const Rational<INT_BYTES> & rhs) const;
	Bool operator<=(const Integer<INT_BYTES> & rhs) const;

    Bool operator>=(const Rational<INT_BYTES> & rhs) const;
	Bool operator>=(const Integer<INT_BYTES> & rhs) const;

    Bool operator<(const Rational<INT_BYTES> & rhs) const;
	Bool operator<(const Integer<INT_BYTES> & rhs) const;

    Bool operator>(const Rational<INT_BYTES> & rhs) const;
	Bool operator>(const Integer<INT_BYTES> & rhs) const;

    // Fraction access
    inline Integer<INT_BYTES> & Numerator();
    inline Integer<INT_BYTES> & Denominator();
    inline const Integer<INT_BYTES> & Numerator() const;
    inline const Integer<INT_BYTES> & Denominator() const;

    // Absolute value helper
    inline Rational<INT_BYTES> Abs() const;

private:
    Void _FractionReduction();

    Integer<INT_BYTES> m_iNumerator;
    Integer<INT_BYTES> m_iDenominator;
};

typedef Rational<16> Rational128;
typedef Rational<32> Rational256;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Rational.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_FORMATS_RATIONAL_H
