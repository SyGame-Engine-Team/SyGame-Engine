/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Vertex/Vertex3.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : 3D vertex
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
#ifndef SCARAB_LIB_MATH_TYPES_VERTEX_VERTEX3_H
#define SCARAB_LIB_MATH_TYPES_VERTEX_VERTEX3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Vector/Vector3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
    // prototypes
template<typename Real>
class TVertex2;
template<typename Real>
class TVertex4;

/////////////////////////////////////////////////////////////////////////////////
// The TVertex3 class
template<typename Real>
class TVertex3
{
public:
    // Constant values
    static const TVertex3<Real> Null; // Null vertex

	// Constructors
	TVertex3(); // uninitialized
	TVertex3(Real x, Real y, Real z);
	TVertex3(const Real vArr[3]);
	TVertex3(const TVertex2<Real> & rhs);
	TVertex3(const TVertex3<Real> & rhs);
	TVertex3(const TVertex4<Real> & rhs);
	~TVertex3();

	// Assignment operator
	inline TVertex3<Real> & operator=(const TVertex3<Real> & rhs);

	// Casting operations
	inline operator Real*() const;
    inline operator const Real*() const;

    inline TVector3<Real> ToVector() const;

	// Index operations
	inline Real & operator[](Int i);
    inline Real operator[](Int i) const;
	inline Real & operator[](UInt i);
	inline Real operator[](UInt i) const;

	// Unary operations
	inline TVertex3<Real> operator+() const;
	inline TVertex3<Real> operator-() const;

	// Boolean operations
	inline Bool operator==(const TVertex3<Real> & rhs) const;
	inline Bool operator!=(const TVertex3<Real> & rhs) const;

	// Real operations
	inline TVertex3<Real> operator+(Real rhs) const;
	inline TVertex3<Real> operator-(Real rhs) const;
	inline TVertex3<Real> operator*(Real rhs) const;
	inline TVertex3<Real> operator/(Real rhs) const;

	inline TVertex3<Real> & operator+=(Real rhs);
	inline TVertex3<Real> & operator-=(Real rhs);
	inline TVertex3<Real> & operator*=(Real rhs);
	inline TVertex3<Real> & operator/=(Real rhs);

    // Vertex operations
	inline TVector3<Real> operator-(const TVertex3<Real> & rhs) const;

	// Vector operations
	inline TVertex3<Real> operator+(const TVector3<Real> & rhs) const;
	inline TVertex3<Real> operator-(const TVector3<Real> & rhs) const;

	inline TVertex3<Real> & operator+=(const TVector3<Real> & rhs);
	inline TVertex3<Real> & operator-=(const TVector3<Real> & rhs);

	// Methods
    inline Real DistSqr() const;
	inline Real Dist() const;
	inline Real InvDistSqr() const;
    inline Real InvDist() const;

    inline Void FromCylindric( Real fRadius, Real fTheta, Real fZ );
    inline Void ToCylindric( Real & outRadius, Real & outTheta, Real & outZ ) const;

    inline Void FromSpherical( Real fRadius, Real fTheta, Real fPhi );
    inline Void ToSpherical( Real & outRadius, Real & outTheta, Real & outPhi ) const;

	// Data
	Real X,Y,Z;
};

// Explicit instanciation
typedef TVertex3<Float> Vertex3f;
typedef TVertex3<Double> Vertex3d;
typedef TVertex3<Scalar> Vertex3;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Vertex3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_TYPES_VERTEX_VERTEX3_H
