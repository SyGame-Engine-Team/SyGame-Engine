/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/TextureCoord/TextureCoord3.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : 3D texture coordinates
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
#ifndef SCARAB_LIB_MATH_TYPES_TEXTURECOORD_TEXTURECOORD3_H
#define SCARAB_LIB_MATH_TYPES_TEXTURECOORD_TEXTURECOORD3_H

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
class TTextureCoord4;

/////////////////////////////////////////////////////////////////////////////////
// The TTextureCoord3 class
template<typename Real>
class TTextureCoord3
{
public:
    // Constant values
    static const TTextureCoord3<Real> Null; // Null vector

	// Constructors
	TTextureCoord3(); // uninitialized
	TTextureCoord3(Real t);
	TTextureCoord3(Real t, Real u);
	TTextureCoord3(Real t, Real u, Real v);
	TTextureCoord3(const Real vArr[3]);
    TTextureCoord3(const TVector2<Real> & rhs);
	TTextureCoord3(const TVector3<Real> & rhs);
	TTextureCoord3(const TVector4<Real> & rhs);
	TTextureCoord3(const TTextureCoord1<Real> & rhs);
	TTextureCoord3(const TTextureCoord2<Real> & rhs);
	TTextureCoord3(const TTextureCoord3<Real> & rhs);
	TTextureCoord3(const TTextureCoord4<Real> & rhs);
	~TTextureCoord3();

    inline TextureCoordDimension Dim() const { return TEXCOORD_3D; }

	// Assignment operator
	inline TTextureCoord3<Real> & operator=(const TTextureCoord3<Real> & rhs);

	// Casting operations
	inline operator Real*() const;
    inline operator const Real*() const;

	// Index operations
    inline Real & operator[](Int i);
	inline Real operator[](Int i) const;
	inline Real & operator[](UInt i);
	inline Real operator[](UInt i) const;

	// Unary operations
	inline TTextureCoord3<Real> operator+() const;
	inline TTextureCoord3<Real> operator-() const;

	// Boolean operations
	inline Bool operator==(const TTextureCoord3<Real> & rhs) const;
	inline Bool operator!=(const TTextureCoord3<Real> & rhs) const;

	// Real operations
	inline TTextureCoord3<Real> operator+(Real rhs) const;
	inline TTextureCoord3<Real> operator-(Real rhs) const;
	inline TTextureCoord3<Real> operator*(Real rhs) const;
	inline TTextureCoord3<Real> operator/(Real rhs) const;

	inline TTextureCoord3<Real> & operator+=(Real rhs);
	inline TTextureCoord3<Real> & operator-=(Real rhs);
	inline TTextureCoord3<Real> & operator*=(Real rhs);
	inline TTextureCoord3<Real> & operator/=(Real rhs);

	// Vector operations
	inline TTextureCoord3<Real> operator+(const TTextureCoord3<Real> & rhs) const;
	inline TTextureCoord3<Real> operator-(const TTextureCoord3<Real> & rhs) const;

	inline TTextureCoord3<Real> & operator+=(const TTextureCoord3<Real> & rhs);
	inline TTextureCoord3<Real> & operator-=(const TTextureCoord3<Real> & rhs);

	inline Real operator*(const TTextureCoord3<Real> & rhs) const; // DOT Product
	inline TTextureCoord3<Real> operator^(const TTextureCoord3<Real> & rhs) const; // CROSS Product

	// Methods
    inline Real DistSqr() const;
	inline Real Dist() const;
	inline Real InvDistSqr() const;
    inline Real InvDist() const;

	// Data
	Real T,U,V;
};

// Explicit instanciation
typedef TTextureCoord3<Float> TextureCoord3f;
typedef TTextureCoord3<Double> TextureCoord3d;
typedef TTextureCoord3<Scalar> TextureCoord3;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "TextureCoord3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_TYPES_TEXTURECOORD_TEXTURECOORD3_H
