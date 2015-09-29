/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Algebra/LinearSystem/LinearSystem2.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : 2D linear systems solver
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_LIB_MATH_ALGEBRA_LINEARSYSTEM_LINEARSYSTEM2_H
#define SCARAB_LIB_MATH_ALGEBRA_LINEARSYSTEM_LINEARSYSTEM2_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Types/Matrix/Matrix2.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TLinearSystem2 class
template<typename Real>
class TLinearSystem2
{
public:
	TLinearSystem2( Real fZeroTolerance = (Real)SCALAR_ERROR ); // unitialized
	TLinearSystem2( const TMatrix2<Real> & matA, const TVector2<Real> & vB,
                    Real fZeroTolerance = (Real)SCALAR_ERROR );
	~TLinearSystem2();

    // Precision management
    Real ZeroTolerance;

    // Linear system access
    inline TMatrix2<Real> & GetA();
    inline TVector2<Real> & GetB();

	// Solves the linear system AX = B
	Bool SolveSystem();

    // Output access
    inline const TMatrix2<Real> & GetInvA() const;
	inline const TVector2<Real> & GetX() const;

private:
	TMatrix2<Real> m_matA;
	TVector2<Real> m_vB;

    TMatrix2<Real> m_matInvA;
    TVector2<Real> m_vX;
};

// Explicit instanciation
typedef TLinearSystem2<Float> LinearSystem2f;
typedef TLinearSystem2<Double> LinearSystem2d;
typedef TLinearSystem2<Scalar> LinearSystem2;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "LinearSystem2.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_ALGEBRA_LINEARSYSTEM_LINEARSYSTEM2_H

