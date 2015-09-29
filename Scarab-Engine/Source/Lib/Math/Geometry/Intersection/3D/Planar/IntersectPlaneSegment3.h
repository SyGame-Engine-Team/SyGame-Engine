/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Intersection/3D/Planar/IntersectPlaneSegment3.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Intersection 3D : Planar, Plane and Segment3
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_3D_PLANAR_INTERSECTPLANESEGMENT3_H
#define SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_3D_PLANAR_INTERSECTPLANESEGMENT3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Primitives/3D/Planar/Plane.h"
#include "../../../Primitives/3D/Linear/Segment3.h"

#include "../../Intersector3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The IntersectPlaneSegment3 class
template<typename Real>
class TIntersectPlaneSegment3 : public TIntersector3<Real>
{
public:
    TIntersectPlaneSegment3( const TPlane<Real> & rPlane, const TSegment3<Real> & rSegment,
                             Real fZeroTolerance = (Real)SCALAR_ERROR );
    virtual ~TIntersectPlaneSegment3();

    // Getters
    inline const TPlane<Real> & GetPlane() const;
    inline const TSegment3<Real> & GetSegment() const;

    // Computed results access
    inline Real GetSegmentParameterAB() const;
    inline Real GetSegmentParameterCDE() const;

    // Static intersection
    virtual Bool Test();
    virtual Bool Compute();

    // No Dynamic Intersection ( infinite primitive(s) )

private:
    const TPlane<Real> * m_pPlane;
    const TSegment3<Real> * m_pSegment;

    Real m_fSegmentParameterAB;
    Real m_fSegmentParameterCDE;
};

// Explicit instanciation
typedef TIntersectPlaneSegment3<Float> IntersectPlaneSegment3f;
typedef TIntersectPlaneSegment3<Double> IntersectPlaneSegment3d;
typedef TIntersectPlaneSegment3<Scalar> IntersectPlaneSegment3;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "IntersectPlaneSegment3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_3D_PLANAR_INTERSECTPLANESEGMENT3_H

