/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Intersection/Intersector1.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Intersection of intervals in 1D
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_INTERSECTOR1_H
#define SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_INTERSECTOR1_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Error/ErrorManager.h"

#include "../../Types/Vector/Vector2.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TIntersector1 class
template<typename Real>
class TIntersector1
{
public:
    // Interval 1D intersection between U[u0;u1] and V[v0;v1] with
    // u0 <= u1 and v0 <= v1. Infinite bounds && degenerate
    // intervals are allowed.
    TIntersector1( Real fU0, Real fU1, Real fV0, Real fV1,
                   Real fZeroTolerance = (Real)SCALAR_ERROR );
    TIntersector1( const Real vU[2], const Real vV[2],
                   Real fZeroTolerance = (Real)SCALAR_ERROR );
    TIntersector1( const TVector2<Real> & vU, const TVector2<Real> & vV,
                   Real fZeroTolerance = (Real)SCALAR_ERROR );
    ~TIntersector1();

    // Precision management
    Real ZeroTolerance;

    // Getters
    inline const TVector2<Real> & GetU() const;
    inline const TVector2<Real> & GetV() const;

    // Computed results access
    inline UInt GetContactCount() const;
    inline Real GetContact( UInt iContact ) const;

    inline Real GetFirstTOI() const;
    inline Real GetLastTOI() const;

    // Static intersection
    Bool Test();
    Bool Compute();

    // Dynamic intersection
    Bool Test( Real fSpeedU, Real fSpeedV, Real fMaxTime );
    Bool Compute( Real fSpeedU, Real fSpeedV, Real fMaxTime );

private:
    TVector2<Real> m_intU, m_intV;

    Real m_fFirstTOI, m_fLastTOI;

    UInt m_iContactCount;
    Real m_arrContacts[2];
};

// Explicit instanciation
typedef TIntersector1<Float> Intersector1f;
typedef TIntersector1<Double> Intersector1d;
typedef TIntersector1<Scalar> Intersector1;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Intersector1.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_INTERSECTOR1_H

