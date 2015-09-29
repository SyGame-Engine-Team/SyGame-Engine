/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Curves/3D/CurveLibrary/NaturalSplineChain3.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Curves 3D : Natural Spline curves
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_CURVES_3D_CURVELIBRARY_NATURALSPLINECHAIN3_H
#define SCARAB_LIB_MATH_GEOMETRY_CURVES_3D_CURVELIBRARY_NATURALSPLINECHAIN3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../../Memory/MemoryManager.h"

#include "../../../../Algebra/LinearSystem/LinearSystemN.h"
#include "../../../../Analysis/Integration/Integrator1.h"

#include "../CurveChain3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
enum NaturalSplineType
{
    NATURAL_SPLINE_FREE = 0,
    NATURAL_SPLINE_CLAMPED,
    NATURAL_SPLINE_CLOSED
};

/////////////////////////////////////////////////////////////////////////////////
// The TNaturalSplineChain3 class
template<typename Real>
class TNaturalSplineChain3 : public TCurveChain3<Real>
{
public:
    TNaturalSplineChain3( const Real * arrParameters, UInt iPatchCount,
                          const TVertex3<Real> * arrControlPoints, NaturalSplineType iType );
    virtual ~TNaturalSplineChain3();

    // Control points access
    inline UInt GetControlPointCount() const;
    inline const TVertex3<Real> & GetControlPoint( UInt iControlPoint ) const;

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
    Void _CreateFreeSpline();
    Void _CreateClampedSpline();
    Void _CreateClosedSpline();

    // Control points
    UInt m_iControlPointCount;
    TVertex3<Real> * m_arrControlPoints;

    // Derivatives
    TVector3<Real> * m_arrPdT;
    TVector3<Real> * m_arrPdT2;
    TVector3<Real> * m_arrPdT3;
};

// Explicit instanciation
typedef TNaturalSplineChain3<Float> NaturalSplineChain3f;
typedef TNaturalSplineChain3<Double> NaturalSplineChain3d;
typedef TNaturalSplineChain3<Scalar> NaturalSplineChain3;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "NaturalSplineChain3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_CURVES_3D_CURVELIBRARY_NATURALSPLINECHAIN3_H


