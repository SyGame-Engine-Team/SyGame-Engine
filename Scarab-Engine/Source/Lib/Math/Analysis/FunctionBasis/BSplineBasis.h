/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Analysis/FuctionBasis/BSplineBasis.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Function basis, BSpline N(i,k) basis
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
#ifndef SCARAB_LIB_MATH_ANALYSIS_FUNCTIONBASIS_BSPLINEBASIS_H
#define SCARAB_LIB_MATH_ANALYSIS_FUNCTIONBASIS_BSPLINEBASIS_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Memory/MemoryManager.h"

#include "../../Functions/MathFunction.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TBSplineBasis class
template<typename Real>
class TBSplineBasis
{
public:
    TBSplineBasis();
    ~TBSplineBasis();

    // Uniform, Open/Periodic
    Void InitUniform( UInt iControlPointCount, UInt iDegree, Bool bPeriodic );

    // Non-Uniform, Open
    Void InitNonUniform( UInt iControlPointCount, UInt iDegree, const Real * arrKnotVector );

    // Getters
    inline UInt GetControlPointCount() const;
    inline UInt GetDegree() const;

    inline Bool IsUniform() const;
    inline Bool IsPeriodic() const;

    // Knot vector access (non-uniform only)
    inline UInt GetKnotCount() const;
    inline Real GetKnot( UInt iKnot ) const;
    inline Void SetKnot( UInt iKnot, Real fKnotValue ) const;

    // N(i,k) basis vectors access
    inline Real N( UInt i ) const;
    inline Real NdT( UInt i ) const;
    inline Real NdT2( UInt i ) const;
    inline Real NdT3( UInt i ) const;

    // Evaluation
    Void Evaluate( Real fT, UInt iOrder, UInt & outMinIndex, UInt & outMaxIndex ) const;

private:
    // Helpers
    Void _Initialize( UInt iControlPointCount, UInt iDegree, Bool bPeriodic );
    UInt _GetKnot( Real & fT ) const;

    // Polynomial data
    UInt m_iControlPointCount; // n+1
    UInt m_iDegree;            // d
    UInt m_iBasisPitch;        // (n+d+1)
    UInt m_iBasisDim;          // (d+1)

    Bool m_bUniform, m_bPeriodic;

    // Knot vector
    UInt m_iKnotCount;
    Real * m_vKnots; // (n+d+2)

    // N(i,k) values & derivatives
    Real * m_arrN;     // (n+d+1) * (d+1)
    Real * m_arrNdT;   // (n+d+1) * (d+1)
    Real * m_arrNdT2;  // (n+d+1) * (d+1)
    Real * m_arrNdT3;  // (n+d+1) * (d+1)
};

// Explicit instanciation
typedef TBSplineBasis<Float> BSplineBasisf;
typedef TBSplineBasis<Double> BSplineBasisd;
typedef TBSplineBasis<Scalar> BSplineBasis;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "BSplineBasis.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_ANALYSIS_FUNCTIONBASIS_BSPLINEBASIS_H

