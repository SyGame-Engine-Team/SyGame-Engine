/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Types/Vector/VectorN.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Generic N-dimensional vector
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : Beware, this is dynamic allocation, you should use fixed-size
//              pools or stacks if you want more than a few ...
//              TODO : SSE loop-packed specializations
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_LIB_MATH_TYPES_VECTOR_VECTORN_H
#define SCARAB_LIB_MATH_TYPES_VECTOR_VECTORN_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Memory/MemoryManager.h"

#include "../../Functions/MathFunction.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TVectorN class
template<typename Real>
class TVectorN
{
public:
	// Constructors
	TVectorN(); // unallocated
	TVectorN( UInt iSize ); // uninitialized
	TVectorN( UInt iSize, const Real * vArr );
	TVectorN(const TVectorN<Real> & rhs);
	~TVectorN();

    // Deferred construction
    inline Void SetSize( UInt iSize );

	// Assignment operator
	inline TVectorN<Real> & operator=(const TVectorN<Real> & rhs);

	// Casting operations
	inline operator Real*() const;
    inline operator const Real*() const;

	// Index operations
	inline Real & operator[](Int i);
	inline Real operator[](Int i) const;
    inline Real & operator[](UInt i);
	inline Real operator[](UInt i) const;

	// Unary operations
	inline TVectorN<Real> operator+() const;
	inline TVectorN<Real> operator-() const;

	// Boolean operations
	inline Bool operator==(const TVectorN<Real> & rhs) const;
	inline Bool operator!=(const TVectorN<Real> & rhs) const;

	// Real operations
	inline TVectorN<Real> operator+(Real rhs) const;
	inline TVectorN<Real> operator-(Real rhs) const;
	inline TVectorN<Real> operator*(Real rhs) const;
	inline TVectorN<Real> operator/(Real rhs) const;

	inline TVectorN<Real> & operator+=(Real rhs);
	inline TVectorN<Real> & operator-=(Real rhs);
	inline TVectorN<Real> & operator*=(Real rhs);
	inline TVectorN<Real> & operator/=(Real rhs);

	// Vector operations
	inline TVectorN<Real> operator+(const TVectorN<Real> & rhs) const;
	inline TVectorN<Real> operator-(const TVectorN<Real> & rhs) const;

	inline TVectorN<Real> & operator+=(const TVectorN<Real> & rhs);
	inline TVectorN<Real> & operator-=(const TVectorN<Real> & rhs);

	inline Real operator*(const TVectorN<Real> & rhs) const; // DOT Product

	// Methods
    inline UInt GetSize() const;

    inline Real NormSqr() const;
	inline Real Norm() const;
	inline Real InvNormSqr() const;
    inline Real InvNorm() const;
	inline Real Normalize();

private:
    static const Real ZERO;

	UInt m_iSize;
	Real * m_arrVector;
};

// Explicit instanciation
typedef TVectorN<Float> VectorNf;
typedef TVectorN<Double> VectorNd;
typedef TVectorN<Scalar> VectorN;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "VectorN.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_TYPES_VECTOR_VECTORN_H
