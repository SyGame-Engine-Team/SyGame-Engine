/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Color/Color4.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : RGBA 4-channels color
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
#ifndef SCARAB_LIB_MATH_TYPES_COLOR_COLOR4_H
#define SCARAB_LIB_MATH_TYPES_COLOR_COLOR4_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Functions/MathFunction.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

    // prototypes
template<typename Real>
class TColor3;

/////////////////////////////////////////////////////////////////////////////////
// The TColor4 class
template<typename Real>
class TColor4
{
public:
    // Constant values
    static const TColor4<Real> Black;
    static const TColor4<Real> White;
    static const TColor4<Real> LightGray;
    static const TColor4<Real> Gray;
    static const TColor4<Real> DarkGray;
    static const TColor4<Real> Red;
    static const TColor4<Real> Green;
    static const TColor4<Real> Blue;
    static const TColor4<Real> Cyan;
    static const TColor4<Real> Magenta;
    static const TColor4<Real> Yellow;

    // Constructors
	TColor4(); // uninitialized
	TColor4(Real r, Real g, Real b, Real a = (Real)1);
    TColor4(const Real arrComponents[4]);
	TColor4(const TColor3<Real> & rhs);
	TColor4(const TColor4<Real> & rhs);
    ~TColor4();

    // Assignment operator
    inline TColor4<Real> & operator=(const TColor4<Real> & rhs);

    // Casting operations
	inline operator Real*() const;
	inline operator const Real*() const;

	// Index operations
	inline Real & operator[](Int i);
	inline Real operator[](Int i) const;
    inline Real & operator[](UInt i);
	inline Real operator[](UInt i) const;

    // Unary operations
	inline TColor4<Real> operator+() const;
	inline TColor4<Real> operator-() const;

    // Boolean operations
	inline Bool operator==(const TColor4<Real> & rhs) const;
	inline Bool operator!=(const TColor4<Real> & rhs) const;

    // Real operations
	inline TColor4<Real> operator+(Real rhs) const;
	inline TColor4<Real> operator-(Real rhs) const;
	inline TColor4<Real> operator*(Real rhs) const;
	inline TColor4<Real> operator/(Real rhs) const;

    inline TColor4<Real> & operator+=(Real rhs);
	inline TColor4<Real> & operator-=(Real rhs);
	inline TColor4<Real> & operator*=(Real rhs);
	inline TColor4<Real> & operator/=(Real rhs);

    // Color operations
	inline TColor4<Real> operator+(const TColor4<Real> & rhs) const;
	inline TColor4<Real> operator-(const TColor4<Real> & rhs) const;
	inline TColor4<Real> operator*(const TColor4<Real> & rhs) const;
	inline TColor4<Real> operator/(const TColor4<Real> & rhs) const;

	inline TColor4<Real> & operator+=(const TColor4<Real> & rhs);
	inline TColor4<Real> & operator-=(const TColor4<Real> & rhs);
	inline TColor4<Real> & operator*=(const TColor4<Real> & rhs);
	inline TColor4<Real> & operator/=(const TColor4<Real> & rhs);

	// Methods
    inline Void Clamp();

    inline Void FromYCbCr( Real fY, Real fCb, Real fCr );
	inline Void ToYCbCr( Real & outY, Real & outCb, Real & outCr ) const;

	inline Void ToMonochrome( TColor4<Real> & outMonochrome ) const;
	inline Void ToGrayscale( TColor4<Real> & outGrayscale ) const;

    // Data
	Real R,G,B,A;

private:
    static const Real ZERO;
    static const Real ONE;
};

// Explicit instanciation
typedef TColor4<Float> Color4f;
typedef TColor4<Double> Color4d;
typedef TColor4<Scalar> Color4;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Color4.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_TYPES_COLOR_COLOR4_H


