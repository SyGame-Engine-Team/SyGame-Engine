/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Curves/3D/CurveLibrary/TCBSplineChain3.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Curves 3D : Tension-Continuity-Bias Spline curves
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_CURVES_3D_CURVELIBRARY_TCBSPLINECHAIN3_H
#define SCARAB_LIB_MATH_GEOMETRY_CURVES_3D_CURVELIBRARY_TCBSPLINECHAIN3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../../Memory/MemoryManager.h"

#include "../../../../Analysis/Integration/Integrator1.h"

#include "../CurveChain3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TTCBSplineChain3 class
template<typename Real>
class TTCBSplineChain3 : public TCurveChain3<Real>
{
public:
    TTCBSplineChain3( const Real * arrParameters, UInt iPatchCount,
                      const TVertex3<Real> * arrControlPoints,
                      const Real * arrTensions, const Real * arrContinuities, const Real * arrBiases );
    virtual ~TTCBSplineChain3();

    // Control points access
    inline UInt GetControlPointCount() const;
    inline const TVertex3<Real> & GetControlPoint( UInt iControlPoint ) const;

    inline Real GetTension( UInt iControlPoint ) const;
    inline Real GetContinuity( UInt iControlPoint ) const;
    inline Real GetBias( UInt iControlPoint ) const;

    // Position and derivatives
    virtual TVertex3<Real> P( Real fT ) const;
    virtual TVector3<Real> PdT( Real fT ) const;
    virtual TVector3<Real> PdT2( Real fT ) const;
    virtual TVector3<Real> PdT3( Real fT ) const;

private:
    // Patches management
    virtual Real _GetPatchSpeed( UInt iPatch, Real fT ) const;
    virtual Real _GetPatchLength( UInt iPatch, Real fT0, Real fT1 ) const;

    // Helpers
    Void _ComputeDerivatives( UInt iPrev, UInt iA, UInt iB, UInt iNext );

    // Control points
    UInt m_iControlPointCount;
    TVertex3<Real> * m_arrControlPoints;
    Real * m_arrTensions;
    Real * m_arrContinuities;
    Real * m_arrBiases;

    // Derivatives
    TVector3<Real> * m_arrPdT;
    TVector3<Real> * m_arrPdT2;
    TVector3<Real> * m_arrPdT3;
};

// Explicit instanciation
typedef TTCBSplineChain3<Float> TCBSplineChain3f;
typedef TTCBSplineChain3<Double> TCBSplineChain3d;
typedef TTCBSplineChain3<Scalar> TCBSplineChain3;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "TCBSplineChain3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_CURVES_3D_CURVELIBRARY_TCBSPLINECHAIN3_H


