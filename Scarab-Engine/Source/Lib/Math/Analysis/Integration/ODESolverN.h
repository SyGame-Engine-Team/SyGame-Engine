/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Analysis/Integration/ODESolverN.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Numerical solver for ordinary differential equations
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : Solves ODE systems : d[X(t)]/dt = F( X(t), t )
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_LIB_MATH_ANALYSIS_INTEGRATION_ODESOLVERN_H
#define SCARAB_LIB_MATH_ANALYSIS_INTEGRATION_ODESOLVERN_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Types/Matrix/MatrixN.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TODESolverN interface
template<typename Real>
class TODESolverN
{
public:
    typedef Void (*Function)( TVectorN<Real> &, const TVectorN<Real> &, Real, Void* );

    // Construction
    TODESolverN( Function pfFunction, UInt iDimension, Void * pUserData = NULL );
    virtual ~TODESolverN();

    // Precision management
    inline Real GetStepValue() const;
    virtual Void SetStepValue( Real fStepValue ) = 0;

    // User data access
    inline Void * GetUserData() const;
    inline Void SetUserData( Void * pUserData );

    // Function access
    inline UInt GetDimension() const;
    inline Function GetFunction() const;
    inline Void SetFunction( Function pfFunction );

    // Solver
    virtual Void Update( TVectorN<Real> & outX, Real & outT, const TVectorN<Real> & vX, Real fT ) = 0;

protected:
    UInt m_iDimension;
    Real m_fStepValue; // Default = (Real)SCALAR_ERROR
    Function m_pfFunction;
    Void * m_pUserData;
};

/////////////////////////////////////////////////////////////////////////////////
// The TODESolverEulerN class
template<typename Real>
class TODESolverEulerN : public TODESolverN<Real>
{
public:
    // Construction
    TODESolverEulerN( Function pfFunction, UInt iDimension, Void * pUserData = NULL );
    virtual ~TODESolverEulerN();

    // Precision management
    virtual Void SetStepValue( Real fStepValue );

    // Solver
    virtual Void Update( TVectorN<Real> & outX, Real & outT, const TVectorN<Real> & vX, Real fT );
};

// Explicit instanciation
typedef TODESolverEulerN<Float> ODESolverEulerNf;
typedef TODESolverEulerN<Double> ODESolverEulerNd;
typedef TODESolverEulerN<Scalar> ODESolverEulerN;

/////////////////////////////////////////////////////////////////////////////////
// The TODESolverImplicitEulerN class
template<typename Real>
class TODESolverImplicitEulerN : public TODESolverN<Real>
{
public:
    typedef Void (*DerivativeFunction)( TMatrixN<Real> &, const TVectorN<Real> &, Real, Void* );

    // Construction
    TODESolverImplicitEulerN( Function pfFunction, DerivativeFunction pfDerivativeFunction,
                              UInt iDimension, Void * pUserData = NULL );
    virtual ~TODESolverImplicitEulerN();

    // Precision management
    virtual Void SetStepValue( Real fStepValue );

    // Derivative Function access
    inline DerivativeFunction GetDerivativeFunction() const;
    inline Void SetDerivativeFunction( DerivativeFunction pfDerivativeFunction );

    // Solver
    virtual Void Update( TVectorN<Real> & outX, Real & outT, const TVectorN<Real> & vX, Real fT );

protected:
    DerivativeFunction m_pfDerivativeFunction;

    TMatrixN<Real> m_matIdentity;
    TMatrixN<Real> m_matDF;
};

// Explicit instanciation
typedef TODESolverImplicitEulerN<Float> ODESolverImplicitEulerNf;
typedef TODESolverImplicitEulerN<Double> ODESolverImplicitEulerNd;
typedef TODESolverImplicitEulerN<Scalar> ODESolverImplicitEulerN;

/////////////////////////////////////////////////////////////////////////////////
// The TODESolverHeunN class
template<typename Real>
class TODESolverHeunN : public TODESolverN<Real>
{
public:
    // Construction
    TODESolverHeunN( Function pfFunction, UInt iDimension, Void * pUserData = NULL );
    virtual ~TODESolverHeunN();

    // Precision management
    virtual Void SetStepValue( Real fStepValue );

    // Solver
    virtual Void Update( TVectorN<Real> & outX, Real & outT, const TVectorN<Real> & vX, Real fT );

protected:
    Real m_fHalfStep;

    TVectorN<Real> m_vHalfX;
};

// Explicit instanciation
typedef TODESolverHeunN<Float> ODESolverHeunNf;
typedef TODESolverHeunN<Double> ODESolverHeunNd;
typedef TODESolverHeunN<Scalar> ODESolverHeunN;

/////////////////////////////////////////////////////////////////////////////////
// The TODESolverRungeKuttaN class
template<typename Real>
class TODESolverRungeKuttaN : public TODESolverN<Real>
{
public:
    // Construction
    TODESolverRungeKuttaN( Function pfFunction, UInt iDimension, Void * pUserData = NULL );
    virtual ~TODESolverRungeKuttaN();

    // Precision management
    virtual Void SetStepValue( Real fStepValue );

    // Solver
    virtual Void Update( TVectorN<Real> & outX, Real & outT, const TVectorN<Real> & vX, Real fT );

protected:
    Real m_fHalfStep;
    Real m_fSixthStep;

    TVectorN<Real> m_vFX1;
    TVectorN<Real> m_vFX2;
    TVectorN<Real> m_vFX3;
    TVectorN<Real> m_vFX4;
};

// Explicit instanciation
typedef TODESolverRungeKuttaN<Float> ODESolverRungeKuttaNf;
typedef TODESolverRungeKuttaN<Double> ODESolverRungeKuttaNd;
typedef TODESolverRungeKuttaN<Scalar> ODESolverRungeKuttaN;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ODESolverN.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_ANALYSIS_INTEGRATION_ODESOLVERN_H
