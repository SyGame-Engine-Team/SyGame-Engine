/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Intersection/3D/Planar/IntersectTriangle3Ray3.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Intersection 3D : Planar, Triangle3 and Ray3
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_3D_PLANAR_INTERSECTTRIANGLE3RAY3_H
#define SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_3D_PLANAR_INTERSECTTRIANGLE3RAY3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Primitives/3D/Planar/Triangle3.h"
#include "../../../Primitives/3D/Linear/Ray3.h"

#include "../../Intersector3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TIntersectTriangle3Ray3 class
template<typename Real>
class TIntersectTriangle3Ray3 : public TIntersector3<Real>
{
public:
    TIntersectTriangle3Ray3( const TTriangle3<Real> & rTriangle, const TRay3<Real> & rRay,
                             Real fZeroTolerance = (Real)SCALAR_ERROR );
    virtual ~TIntersectTriangle3Ray3();

    // Getters
    inline const TTriangle3<Real> & GetTriangle() const;
    inline const TRay3<Real> & GetRay() const;

    // Computed results access
    inline Real GetRayParameter() const;
    inline const TVertex3<Real> & GetTriangleParameters() const;

    // Static intersection
    virtual Bool Test();
    virtual Bool Compute();

    // No Dynamic Intersection ( infinite primitive(s) )

private:
    const TTriangle3<Real> * m_pTriangle;
    const TRay3<Real> * m_pRay;

    Real m_fRayParameter;
    TVertex3<Real> m_vTriangleParameters;
};

// Explicit instanciation
typedef TIntersectTriangle3Ray3<Float> IntersectTriangle3Ray3f;
typedef TIntersectTriangle3Ray3<Double> IntersectTriangle3Ray3d;
typedef TIntersectTriangle3Ray3<Scalar> IntersectTriangle3Ray3;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "IntersectTriangle3Ray3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_3D_PLANAR_INTERSECTTRIANGLE3RAY3_H

