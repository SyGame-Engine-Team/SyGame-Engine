/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Algebra/LinearSystem/LinearSystemN.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : General NxM linear systems solver
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
#ifndef SCARAB_LIB_MATH_ALGEBRA_LINEARSYSTEM_LINEARSYSTEMN_H
#define SCARAB_LIB_MATH_ALGEBRA_LINEARSYSTEM_LINEARSYSTEMN_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Types/Matrix/MatrixN.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TLinearSystemN class
template<typename Real>
class TLinearSystemN
{
public:
	TLinearSystemN( Real fZeroTolerance = (Real)SCALAR_ERROR ); // unitialized
	TLinearSystemN( const TMatrixN<Real> & matA, const TVectorN<Real> & vB,
                    Real fZeroTolerance = (Real)SCALAR_ERROR );
	~TLinearSystemN();

    // Precision management
    Real ZeroTolerance;

    // Linear system access
    inline TMatrixN<Real> & GetA();
    inline TVectorN<Real> & GetB();

	// Solves the linear system AX = B
	Bool SolveSystem();          // Inverse method
	Bool SolveSymmetricSystem(); // Conjugate Gradient method

    // Output access
    inline const TMatrixN<Real> & GetInvA() const;
	inline const TVectorN<Real> & GetX() const;

private:
	TMatrixN<Real> m_matA;
	TVectorN<Real> m_vB;

	TMatrixN<Real> m_matInvA;
    TVectorN<Real> m_vX;
};

// Explicit instanciation
typedef TLinearSystemN<Float> LinearSystemNf;
typedef TLinearSystemN<Double> LinearSystemNd;
typedef TLinearSystemN<Scalar> LinearSystemN;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "LinearSystemN.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_ALGEBRA_LINEARSYSTEM_LINEARSYSTEMN_H

