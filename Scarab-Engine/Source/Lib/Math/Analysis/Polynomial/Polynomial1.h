/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Analysis/Polynomial/Polynomial1.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Polynomial arithmetic : Polynomial 1D
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
#ifndef SCARAB_LIB_MATH_ANALYSIS_POLYNOMIAL_POLYNOMIAL1_H
#define SCARAB_LIB_MATH_ANALYSIS_POLYNOMIAL_POLYNOMIAL1_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Memory/MemoryManager.h"

#include "../../Functions/MathFunction.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TPolynomial1 class
template <typename Real>
class TPolynomial1
{
public:
    // Construction and destruction.
    TPolynomial1();
    TPolynomial1( UInt iDegree );
    TPolynomial1( const TPolynomial1<Real> & rhs );
    ~TPolynomial1();

	// Assignment operator
    TPolynomial1<Real> & operator=( const TPolynomial1<Real> & rhs );

	// Casting operations
    inline operator const Real*() const;
    inline operator Real*();

	// Index operations
    inline Real operator[]( Int i ) const;
    inline Real & operator[]( Int i );
    inline Real operator[]( UInt i ) const;
    inline Real & operator[]( UInt i );

	// Unary operations
    TPolynomial1<Real> operator-() const;

    // Boolean operations
	Bool operator==( const TPolynomial1<Real> & rhs ) const;
	Bool operator!=( const TPolynomial1<Real> & rhs ) const;

	// Real operations
    TPolynomial1<Real> operator+( Real rhs ) const;
    TPolynomial1<Real> operator-( Real rhs ) const;
    TPolynomial1<Real> operator*( Real rhs ) const;
    TPolynomial1<Real> operator/( Real rhs ) const;

    inline TPolynomial1<Real> & operator+=( Real rhs );
    inline TPolynomial1<Real> & operator-=( Real rhs );
    inline TPolynomial1<Real> & operator*=( Real rhs );
    inline TPolynomial1<Real> & operator/=( Real rhs );

	// Polynomial operations
    TPolynomial1<Real> operator+( const TPolynomial1<Real> & rhs ) const;
    TPolynomial1<Real> operator-( const TPolynomial1<Real> & rhs ) const;
    TPolynomial1<Real> operator*( const TPolynomial1<Real> & rhs ) const;

    inline TPolynomial1<Real> & operator+=( const TPolynomial1<Real> & rhs );
    inline TPolynomial1<Real> & operator-=( const TPolynomial1<Real> & rhs );
    inline TPolynomial1<Real> & operator*=( const TPolynomial1<Real> & rhs );

    // Degree access
    inline UInt GetDegree() const;
    Void SetDegree( UInt iDegree );

    // Methods
    Real Evaluate( Real fT ) const;

    TPolynomial1<Real> GetDerivative() const;
    TPolynomial1<Real> GetInvert() const;

    Void Divide( TPolynomial1<Real> * outQuotient, TPolynomial1<Real> * outRemainder,
                 const TPolynomial1<Real> & rDivisor, Real fTolerance = (Real)SCALAR_ERROR ) const;

    Void Compression( Real fTolerance = (Real)SCALAR_ERROR );

protected:
    UInt m_iDegree;
    Real * m_arrCoefficients;
};

// Explicit instanciation
typedef TPolynomial1<Float> Polynomial1f;
typedef TPolynomial1<Double> Polynomial1d;
typedef TPolynomial1<Scalar> Polynomial1;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Polynomial1.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_ANALYSIS_POLYNOMIAL_POLYNOMIAL1_H


