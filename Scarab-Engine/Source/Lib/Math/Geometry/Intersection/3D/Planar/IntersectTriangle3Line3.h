/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Intersection/3D/Planar/IntersectTriangle3Line3.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Intersection 3D : Planar, Triangle3 and Line3
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_3D_PLANAR_INTERSECTTRIANGLE3LINE3_H
#define SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_3D_PLANAR_INTERSECTTRIANGLE3LINE3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Primitives/3D/Planar/Triangle3.h"
#include "../../../Primitives/3D/Linear/Line3.h"

#include "../../Intersector3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TIntersectTriangle3Line3 class
template<typename Real>
class TIntersectTriangle3Line3 : public TIntersector3<Real>
{
public:
    TIntersectTriangle3Line3( const TTriangle3<Real> & rTriangle, const TLine3<Real> & rLine,
                              Real fZeroTolerance = (Real)SCALAR_ERROR );
    virtual ~TIntersectTriangle3Line3();

    // Getters
    inline const TTriangle3<Real> & GetTriangle() const;
    inline const TLine3<Real> & GetLine() const;

    // Computed results access
    inline Real GetLineParameter() const;
    inline const TVertex3<Real> & GetTriangleParameters() const;

    // Static intersection
    virtual Bool Test();
    virtual Bool Compute();

    // No Dynamic Intersection ( infinite primitive(s) )

private:
    const TTriangle3<Real> * m_pTriangle;
    const TLine3<Real> * m_pLine;

    Real m_fLineParameter;
    TVertex3<Real> m_vTriangleParameters;
};

// Explicit instanciation
typedef TIntersectTriangle3Line3<Float> IntersectTriangle3Line3f;
typedef TIntersectTriangle3Line3<Double> IntersectTriangle3Line3d;
typedef TIntersectTriangle3Line3<Scalar> IntersectTriangle3Line3;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "IntersectTriangle3Line3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_3D_PLANAR_INTERSECTTRIANGLE3LINE3_H

