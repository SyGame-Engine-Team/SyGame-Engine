/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Intersection/3D/Linear/IntersectSegment3.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Intersection 3D : Linear, Segment3 and Segment3
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : TODO : Add specified result (parallel, colinear, adjacency)
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_3D_LINEAR_INTERSECTSEGMENT3_H
#define SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_3D_LINEAR_INTERSECTSEGMENT3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Primitives/3D/Linear/Segment3.h"

#include "../../Intersector3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Specifiers
//#define INTERSECTSEGMENT3_SPEC_

/////////////////////////////////////////////////////////////////////////////////
// The TIntersectSegment3 class
template<typename Real>
class TIntersectSegment3 : public TIntersector3<Real>
{
public:
    TIntersectSegment3( const TSegment3<Real> & rSegmentA, const TSegment3<Real> & rSegmentB,
                        Real fZeroTolerance = (Real)SCALAR_ERROR );
    virtual ~TIntersectSegment3();

    // Getters
    inline const TSegment3<Real> & GetSegmentA() const;
    inline const TSegment3<Real> & GetSegmentB() const;

    // Computed results access
    inline Real GetDistance() const;

    inline Real GetParameterA() const;
    inline Real GetParameterB() const;

    inline const TVertex3<Real> & GetClosestPointA() const;
    inline const TVertex3<Real> & GetClosestPointB() const;

    // Static intersection
    virtual Bool Test();
    virtual Bool Compute();

    // Dynamic Intersection : NOT IMPLEMENTED

private:
    // Intersection data
    const TSegment3<Real> * m_pSegmentA;
    const TSegment3<Real> * m_pSegmentB;

    Real m_fDistance;

    Real m_fParameterA;
    Real m_fParameterB;

    TVertex3<Real> m_vClosestPointA;
    TVertex3<Real> m_vClosestPointB;
};

// Explicit instanciation
typedef TIntersectSegment3<Float> IntersectSegment3f;
typedef TIntersectSegment3<Double> IntersectSegment3d;
typedef TIntersectSegment3<Scalar> IntersectSegment3;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "IntersectSegment3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_3D_LINEAR_INTERSECTSEGMENT3_H

