/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Intersection/3D/Planar/IntersectPlaneRay3.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Intersection 3D : Planar, Plane and Ray3
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_3D_PLANAR_INTERSECTPLANERAY3_H
#define SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_3D_PLANAR_INTERSECTPLANERAY3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Primitives/3D/Planar/Plane.h"
#include "../../../Primitives/3D/Linear/Ray3.h"

#include "../../Intersector3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The IntersectPlaneRay3 class
template<typename Real>
class TIntersectPlaneRay3 : public TIntersector3<Real>
{
public:
    TIntersectPlaneRay3( const TPlane<Real> & rPlane, const TRay3<Real> & rRay,
                         Real fZeroTolerance = (Real)SCALAR_ERROR );
    virtual ~TIntersectPlaneRay3();

    // Getters
    inline const TPlane<Real> & GetPlane() const;
    inline const TRay3<Real> & GetRay() const;

    // Computed results access
    inline Real GetRayParameter() const;

    // Static intersection
    virtual Bool Test();
    virtual Bool Compute();

    // No Dynamic Intersection ( infinite primitive(s) )

private:
    const TPlane<Real> * m_pPlane;
    const TRay3<Real> * m_pRay;

    Real m_fRayParameter;
};

// Explicit instanciation
typedef TIntersectPlaneRay3<Float> IntersectPlaneRay3f;
typedef TIntersectPlaneRay3<Double> IntersectPlaneRay3d;
typedef TIntersectPlaneRay3<Scalar> IntersectPlaneRay3;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "IntersectPlaneRay3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_3D_PLANAR_INTERSECTPLANERAY3_H

