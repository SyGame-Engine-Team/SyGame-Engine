/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/TextureCoord/TextureCoord4.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : 4D texture coordinates
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
#ifndef SCARAB_LIB_MATH_TYPES_TEXTURECOORD_TEXTURECOORD4_H
#define SCARAB_LIB_MATH_TYPES_TEXTURECOORD_TEXTURECOORD4_H

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
class TTextureCoord2;
template<typename Real>
class TTextureCoord3;

/////////////////////////////////////////////////////////////////////////////////
// The TTextureCoord4 class
template<typename Real>
class TTextureCoord4
{
public:
    // Constant values
    static const TTextureCoord4<Real> Null; // Null vector

	// Constructors
	TTextureCoord4(); // uninitialized
	TTextureCoord4(Real t);
	TTextureCoord4(Real t, Real u);
	TTextureCoord4(Real t, Real u, Real v);
	TTextureCoord4(Real t, Real u, Real v, Real w);
	TTextureCoord4(const Real vArr[4]);
    TTextureCoord4(const TVector2<Real> & rhs);
	TTextureCoord4(const TVector3<Real> & rhs);
	TTextureCoord4(const TVector4<Real> & rhs);
	TTextureCoord4(const TTextureCoord1<Real> & rhs);
	TTextureCoord4(const TTextureCoord2<Real> & rhs);
	TTextureCoord4(const TTextureCoord3<Real> & rhs);
	TTextureCoord4(const TTextureCoord4<Real> & rhs);
	~TTextureCoord4();

    inline TextureCoordDimension Dim() const { return TEXCOORD_4D; }

	// Assignment operator
	inline TTextureCoord4<Real> & operator=(const TTextureCoord4<Real> & rhs);

	// Casting operations
	inline operator Real*() const;
    inline operator const Real*() const;

	// Index operations
	inline Real & operator[](Int i);
	inline Real operator[](Int i) const;
    inline Real & operator[](UInt i);
	inline Real operator[](UInt i) const;

	// Unary operations
	inline TTextureCoord4<Real> operator+() const;
	inline TTextureCoord4<Real> operator-() const;

	// Boolean operations
	inline Bool operator==(const TTextureCoord4<Real> & rhs) const;
	inline Bool operator!=(const TTextureCoord4<Real> & rhs) const;

	// Real operations
	inline TTextureCoord4<Real> operator+(Real rhs) const;
	inline TTextureCoord4<Real> operator-(Real rhs) const;
	inline TTextureCoord4<Real> operator*(Real rhs) const;
	inline TTextureCoord4<Real> operator/(Real rhs) const;

	inline TTextureCoord4<Real> & operator+=(Real rhs);
	inline TTextureCoord4<Real> & operator-=(Real rhs);
	inline TTextureCoord4<Real> & operator*=(Real rhs);
	inline TTextureCoord4<Real> & operator/=(Real rhs);

	// Vector operations
	inline TTextureCoord4<Real> operator+(const TTextureCoord4<Real> & rhs) const;
	inline TTextureCoord4<Real> operator-(const TTextureCoord4<Real> & rhs) const;

	inline TTextureCoord4<Real> & operator+=(const TTextureCoord4<Real> & rhs);
	inline TTextureCoord4<Real> & operator-=(const TTextureCoord4<Real> & rhs);

	inline Real operator*(const TTextureCoord4<Real> & rhs) const; // DOT Product
	inline TTextureCoord4<Real> operator^(const TTextureCoord4<Real> & rhs) const; // CROSS Product

	// Methods
    inline Real DistSqr() const;
	inline Real Dist() const;
	inline Real InvDistSqr() const;
    inline Real InvDist() const;

	// Data
	Real T,U,V,W;
};

// Explicit instanciation
typedef TTextureCoord4<Float> TextureCoord4f;
typedef TTextureCoord4<Double> TextureCoord4d;
typedef TTextureCoord4<Scalar> TextureCoord4;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "TextureCoord4.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_TYPES_TEXTURECOORD_TEXTURECOORD4_H
