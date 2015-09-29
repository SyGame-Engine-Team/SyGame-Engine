/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Algebra/LinearSystem/LinearSystem3.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : 3D linear systems solver
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
#ifndef SCARAB_LIB_MATH_ALGEBRA_LINEARSYSTEM_LINEARSYSTEM3_H
#define SCARAB_LIB_MATH_ALGEBRA_LINEARSYSTEM_LINEARSYSTEM3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Types/Matrix/Matrix3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TLinearSystem3 class
template<typename Real>
class TLinearSystem3
{
public:
	TLinearSystem3( Real fZeroTolerance = (Real)SCALAR_ERROR ); // unitialized
	TLinearSystem3( const TMatrix3<Real> & matA, const TVector3<Real> & vB,
                    Real fZeroTolerance = (Real)SCALAR_ERROR );
	~TLinearSystem3();

    // Precision management
    Real ZeroTolerance;

    // Linear system access
    inline TMatrix3<Real> & GetA();
    inline TVector3<Real> & GetB();

	// Solves the linear system AX = B
	Bool SolveSystem();

    // Output access
    inline const TMatrix3<Real> & GetInvA() const;
	inline const TVector3<Real> & GetX() const;

private:
	TMatrix3<Real> m_matA;
	TVector3<Real> m_vB;

	TMatrix3<Real> m_matInvA;
    TVector3<Real> m_vX;
};

// Explicit instanciation
typedef TLinearSystem3<Float> LinearSystem3f;
typedef TLinearSystem3<Double> LinearSystem3d;
typedef TLinearSystem3<Scalar> LinearSystem3;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "LinearSystem3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_ALGEBRA_LINEARSYSTEM_LINEARSYSTEM3_H

