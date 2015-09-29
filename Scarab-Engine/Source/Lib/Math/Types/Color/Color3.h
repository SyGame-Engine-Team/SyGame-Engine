/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Color/Color3.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : RGB 3-channels color
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
#ifndef SCARAB_LIB_MATH_TYPES_COLOR_COLOR3_H
#define SCARAB_LIB_MATH_TYPES_COLOR_COLOR3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Functions/MathFunction.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

    // prototypes
template<typename Real>
class TColor4;

/////////////////////////////////////////////////////////////////////////////////
// The TColor3 class
template<typename Real>
class TColor3
{
public:
    // Constant values
    static const TColor3<Real> Black;
    static const TColor3<Real> White;
    static const TColor3<Real> LightGray;
    static const TColor3<Real> Gray;
    static const TColor3<Real> DarkGray;
    static const TColor3<Real> Red;
    static const TColor3<Real> Green;
    static const TColor3<Real> Blue;
    static const TColor3<Real> Cyan;
    static const TColor3<Real> Magenta;
    static const TColor3<Real> Yellow;

    // Constructors
	TColor3(); // uninitialized
	TColor3(Real r, Real g, Real b);
    TColor3(const Real arrComponents[3]);
	TColor3(const TColor3<Real> & rhs);
	TColor3(const TColor4<Real> & rhs);
    ~TColor3();

    // Assignment operator
    inline TColor3<Real> & operator=(const TColor3<Real> & rhs);

    // Casting operations
	inline operator Real*() const;
	inline operator const Real*() const;

	// Index operations
    inline Real & operator[](Int i);
	inline Real operator[](Int i) const;
	inline Real & operator[](UInt i);
	inline Real operator[](UInt i) const;

    // Unary operations
	inline TColor3<Real> operator+() const;
	inline TColor3<Real> operator-() const;

    // Boolean operations
	inline Bool operator==(const TColor3<Real> & rhs) const;
	inline Bool operator!=(const TColor3<Real> & rhs) const;

    // Real operations
	inline TColor3<Real> operator+(Real rhs) const;
	inline TColor3<Real> operator-(Real rhs) const;
	inline TColor3<Real> operator*(Real rhs) const;
	inline TColor3<Real> operator/(Real rhs) const;

    inline TColor3<Real> & operator+=(Real rhs);
	inline TColor3<Real> & operator-=(Real rhs);
	inline TColor3<Real> & operator*=(Real rhs);
	inline TColor3<Real> & operator/=(Real rhs);

    // Color operations
	inline TColor3<Real> operator+(const TColor3<Real> & rhs) const;
	inline TColor3<Real> operator-(const TColor3<Real> & rhs) const;
	inline TColor3<Real> operator*(const TColor3<Real> & rhs) const;
	inline TColor3<Real> operator/(const TColor3<Real> & rhs) const;

	inline TColor3<Real> & operator+=(const TColor3<Real> & rhs);
	inline TColor3<Real> & operator-=(const TColor3<Real> & rhs);
	inline TColor3<Real> & operator*=(const TColor3<Real> & rhs);
	inline TColor3<Real> & operator/=(const TColor3<Real> & rhs);

	// Methods
    inline Void Clamp();

    inline Void FromYCbCr( Real fY, Real fCb, Real fCr );
	inline Void ToYCbCr( Real & outY, Real & outCb, Real & outCr ) const;

	inline Void ToMonochrome( TColor3<Real> & outMonochrome ) const;
	inline Void ToGrayscale( TColor3<Real> & outGrayscale ) const;

    // Data
	Real R,G,B;

private:
    static const Real ZERO;
    static const Real ONE;
};

// Explicit instanciation
typedef TColor3<Float> Color3f;
typedef TColor3<Double> Color3d;
typedef TColor3<Scalar> Color3;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Color3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_TYPES_COLOR_COLOR3_H


