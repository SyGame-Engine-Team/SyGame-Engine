/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/TextureCoord/TextureCoord2.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : 2D texture coordinates
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
#ifndef SCARAB_LIB_MATH_TYPES_TEXTURECOORD_TEXTURECOORD2_H
#define SCARAB_LIB_MATH_TYPES_TEXTURECOORD_TEXTURECOORD2_H

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
class TTextureCoord1;
template<typename Real>
class TTextureCoord3;
template<typename Real>
class TTextureCoord4;

/////////////////////////////////////////////////////////////////////////////////
// The TTextureCoord2 class
template<typename Real>
class TTextureCoord2
{
public:
    // Constant values
    static const TTextureCoord2<Real> Null; // Null vector

	// Constructors
	TTextureCoord2(); // uninitialized
	TTextureCoord2(Real t);
	TTextureCoord2(Real t, Real u);
	TTextureCoord2(const Real vArr[2]);
    TTextureCoord2(const TVector2<Real> & rhs);
	TTextureCoord2(const TVector3<Real> & rhs);
	TTextureCoord2(const TVector4<Real> & rhs);
	TTextureCoord2(const TTextureCoord1<Real> & rhs);
	TTextureCoord2(const TTextureCoord2<Real> & rhs);
	TTextureCoord2(const TTextureCoord3<Real> & rhs);
	TTextureCoord2(const TTextureCoord4<Real> & rhs);
	~TTextureCoord2();

    inline TextureCoordDimension Dim() const { return TEXCOORD_2D; }

	// Assignment operator
	inline TTextureCoord2<Real> & operator=(const TTextureCoord2<Real> & rhs);

	// Casting operations
	inline operator Real*() const;
    inline operator const Real*() const;

	// Index operations
    inline Real & operator[](Int i);
	inline Real operator[](Int i) const;
	inline Real & operator[](UInt i);
	inline Real operator[](UInt i) const;

	// Unary operations
	inline TTextureCoord2<Real> operator+() const;
	inline TTextureCoord2<Real> operator-() const;

	// Boolean operations
	inline Bool operator==(const TTextureCoord2<Real> & rhs) const;
	inline Bool operator!=(const TTextureCoord2<Real> & rhs) const;

	// Real operations
	inline TTextureCoord2<Real> operator+(Real rhs) const;
	inline TTextureCoord2<Real> operator-(Real rhs) const;
	inline TTextureCoord2<Real> operator*(Real rhs) const;
	inline TTextureCoord2<Real> operator/(Real rhs) const;

	inline TTextureCoord2<Real> & operator+=(Real rhs);
	inline TTextureCoord2<Real> & operator-=(Real rhs);
	inline TTextureCoord2<Real> & operator*=(Real rhs);
	inline TTextureCoord2<Real> & operator/=(Real rhs);

	// Vector operations
	inline TTextureCoord2<Real> operator+(const TTextureCoord2<Real> & rhs) const;
	inline TTextureCoord2<Real> operator-(const TTextureCoord2<Real> & rhs) const;

	inline TTextureCoord2<Real> & operator+=(const TTextureCoord2<Real> & rhs);
	inline TTextureCoord2<Real> & operator-=(const TTextureCoord2<Real> & rhs);

	inline Real operator*(const TTextureCoord2<Real> & rhs) const; // DOT Product

	// Methods
    inline Real DistSqr() const;
	inline Real Dist() const;
	inline Real InvDistSqr() const;
    inline Real InvDist() const;

	// Data
	Real T,U;
};

// Explicit instanciation
typedef TTextureCoord2<Float> TextureCoord2f;
typedef TTextureCoord2<Double> TextureCoord2d;
typedef TTextureCoord2<Scalar> TextureCoord2;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "TextureCoord2.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_TYPES_TEXTURECOORD_TEXTURECOORD2_H
