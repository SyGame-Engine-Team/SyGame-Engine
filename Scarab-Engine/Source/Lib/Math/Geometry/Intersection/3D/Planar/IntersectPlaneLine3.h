/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Intersection/3D/Planar/IntersectPlaneLine3.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Intersection 3D : Planar, Plane and Line3
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_3D_PLANAR_INTERSECTPLANELINE3_H
#define SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_3D_PLANAR_INTERSECTPLANELINE3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Primitives/3D/Planar/Plane.h"
#include "../../../Primitives/3D/Linear/Line3.h"

#include "../../Intersector3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TIntersectPlaneLine3 class
template<typename Real>
class TIntersectPlaneLine3 : public TIntersector3<Real>
{
public:
    TIntersectPlaneLine3( const TPlane<Real> & rPlane, const TLine3<Real> & rLine,
                          Real fZeroTolerance = (Real)SCALAR_ERROR );
    virtual ~TIntersectPlaneLine3();

    // Getters
    inline const TPlane<Real> & GetPlane() const;
    inline const TLine3<Real> & GetLine() const;

    // Computed results access
    inline Real GetLineParameter() const;

    // Static intersection
    virtual Bool Test();
    virtual Bool Compute();

    // No Dynamic Intersection ( infinite primitive(s) )

private:
    const TPlane<Real> * m_pPlane;
    const TLine3<Real> * m_pLine;

    Real m_fLineParameter;
};

// Explicit instanciation
typedef TIntersectPlaneLine3<Float> IntersectPlaneLine3f;
typedef TIntersectPlaneLine3<Double> IntersectPlaneLine3d;
typedef TIntersectPlaneLine3<Scalar> IntersectPlaneLine3;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "IntersectPlaneLine3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_3D_PLANAR_INTERSECTPLANELINE3_H

