/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/TextureCoord/TextureCoord1.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : 1D texture coordinates
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
#ifndef SCARAB_LIB_MATH_TYPES_TEXTURECOORD_TEXTURECOORD1_H
#define SCARAB_LIB_MATH_TYPES_TEXTURECOORD_TEXTURECOORD1_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Vector/Vector2.h"
#include "../Vector/Vector3.h"
#include "../Vector/Vector4.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#ifndef _TEXTURECOORD_DIMENSIONS_DECLARED
#define _TEXTURECOORD_DIMENSIONS_DECLARED
    enum TextureCoordDimension
    {
	    TEXCOORD_1D = 1,
	    TEXCOORD_2D,
	    TEXCOORD_3D,
	    TEXCOORD_4D,
    };
#endif

    // prototypes
template<typename Real>
class TTextureCoord2;
template<typename Real>
class TTextureCoord3;
template<typename Real>
class TTextureCoord4;

/////////////////////////////////////////////////////////////////////////////////
// The TTextureCoord1 class
template<typename Real>
class TTextureCoord1
{
public:
    // Constant values
    static const TTextureCoord1<Real> Null; // Null vector

	// Constructors
	TTextureCoord1(); // uninitialized
	TTextureCoord1(Real t);
    TTextureCoord1(const TVector2<Real> & rhs);
	TTextureCoord1(const TVector3<Real> & rhs);
	TTextureCoord1(const TVector4<Real> & rhs);
	TTextureCoord1(const TTextureCoord1<Real> & rhs);
	TTextureCoord1(const TTextureCoord2<Real> & rhs);
	TTextureCoord1(const TTextureCoord3<Real> & rhs);
	TTextureCoord1(const TTextureCoord4<Real> & rhs);
	~TTextureCoord1();

    inline TextureCoordDimension Dim() const { return TEXCOORD_1D; }

	// Assignment operator
	inline TTextureCoord1<Real> & operator=(const TTextureCoord1<Real> & rhs);

	// Casting operations
	inline operator Real*() const;
    inline operator const Real*() const;

	// Index operations
    inline Real & operator[](Int i);
	inline Real operator[](Int i) const;
	inline Real & operator[](UInt i);
	inline Real operator[](UInt i) const;

	// Unary operations
	inline TTextureCoord1<Real> operator+() const;
	inline TTextureCoord1<Real> operator-() const;

	// Boolean operations
	inline Bool operator==(const TTextureCoord1<Real> & rhs) const;
	inline Bool operator!=(const TTextureCoord1<Real> & rhs) const;

	// Real operations
	inline TTextureCoord1<Real> operator+(Real rhs) const;
	inline TTextureCoord1<Real> operator-(Real rhs) const;
	inline TTextureCoord1<Real> operator*(Real rhs) const;
	inline TTextureCoord1<Real> operator/(Real rhs) const;

	inline TTextureCoord1<Real> & operator+=(Real rhs);
	inline TTextureCoord1<Real> & operator-=(Real rhs);
	inline TTextureCoord1<Real> & operator*=(Real rhs);
	inline TTextureCoord1<Real> & operator/=(Real rhs);

	// Vector operations
	inline TTextureCoord1<Real> operator+(const TTextureCoord1<Real> & rhs) const;
	inline TTextureCoord1<Real> operator-(const TTextureCoord1<Real> & rhs) const;

	inline TTextureCoord1<Real> & operator+=(const TTextureCoord1<Real> & rhs);
	inline TTextureCoord1<Real> & operator-=(const TTextureCoord1<Real> & rhs);

	inline Real operator*(const TTextureCoord1<Real> & rhs) const; // DOT Product

	// Data
	Real T;
};

// Explicit instanciation
typedef TTextureCoord1<Float> TextureCoord1f;
typedef TTextureCoord1<Double> TextureCoord1d;
typedef TTextureCoord1<Scalar> TextureCoord1;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "TextureCoord1.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_TYPES_TEXTURECOORD_TEXTURECOORD1_H
