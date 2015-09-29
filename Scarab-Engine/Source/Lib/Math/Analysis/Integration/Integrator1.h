/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Analysis/Integration/Integrator1.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Numerical Integration for single variable functions
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
#ifndef SCARAB_LIB_MATH_ANALYSIS_INTEGRATION_INTEGRATOR1_H
#define SCARAB_LIB_MATH_ANALYSIS_INTEGRATION_INTEGRATOR1_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Error/ErrorManager.h"

#include "../../Functions/MathFunction.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define INTEGRATOR1_ROMBERG_MAX_ORDER 32

/////////////////////////////////////////////////////////////////////////////////
// The TIntegrator1 class
template<typename Real>
class TIntegrator1
{
public:
    // Functions of single variable
    typedef Real (*Function)( Real, Void* );

    // Construction
    TIntegrator1( Function pfFunction, Void * pUserData = NULL );
    ~TIntegrator1();

    // Precision management
    UInt TrapezoidSampleCount; // Default = 16
    UInt RombergOrder;         // Default = 8

    // User data access
    inline Void * GetUserData() const;
    inline Void SetUserData( Void * pUserData );

    // Function access
    inline Function GetFunction() const;
    inline Void SetFunction( Function pfFunction );

    // Single variable integration
    Real GaussianQuadrature( Real fT0, Real fT1 );
    Real TrapezoidRule( Real fT0, Real fT1 );
    Real RombergIntegral( Real fT0, Real fT1 );

private:
    Function m_pfFunction;
    Void * m_pUserData;
};

// Explicit instanciation
typedef TIntegrator1<Float> Integrator1f;
typedef TIntegrator1<Double> Integrator1d;
typedef TIntegrator1<Scalar> Integrator1;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Integrator1.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_ANALYSIS_INTEGRATION_INTEGRATOR1_H
