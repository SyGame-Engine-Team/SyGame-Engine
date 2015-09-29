/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Intersection/3D/Planar/IntersectTriangle3Segment3.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Intersection 3D : Planar, Triangle3 and Segment3
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_3D_PLANAR_INTERSECTTRIANGLE3SEGMENT3_H
#define SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_3D_PLANAR_INTERSECTTRIANGLE3SEGMENT3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Primitives/3D/Planar/Triangle3.h"
#include "../../../Primitives/3D/Linear/Segment3.h"

#include "../../Intersector3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TIntersectTriangle3Segment3 class
template<typename Real>
class TIntersectTriangle3Segment3 : public TIntersector3<Real>
{
public:
    TIntersectTriangle3Segment3( const TTriangle3<Real> & rTriangle, const TSegment3<Real> & rSegment,
                                 Real fZeroTolerance = (Real)SCALAR_ERROR );
    virtual ~TIntersectTriangle3Segment3();

    // Getters
    inline const TTriangle3<Real> & GetTriangle() const;
    inline const TSegment3<Real> & GetSegment() const;

    // Computed results access
    inline Real GetSegmentParameterAB() const;
    inline Real GetSegmentParameterCDE() const;
    inline const TVertex3<Real> & GetTriangleParameters() const;

    inline UInt GetContactCount() const;
    inline const TVertex3<Real> & GetContact( UInt iContact ) const;

    // Static intersection
    virtual Bool Test();
    virtual Bool Compute();

    // Dynamic Intersection : NOT IMPLEMENTED
    //virtual Bool Test( const TVector3<Real> & vSpeedTriangle, const TVector3<Real> & vSpeedSegment, Real fMaxTime );
    //virtual Bool Compute( const TVector3<Real> & vSpeedTriangle, const TVector3<Real> & vSpeedSegment, Real fMaxTime );

private:
    const TTriangle3<Real> * m_pTriangle;
    const TSegment3<Real> * m_pSegment;

    Real m_fSegmentParameterAB;
    Real m_fSegmentParameterCDE;
    TVertex3<Real> m_vTriangleParameters;

    UInt m_iContactCount;
    TVertex3<Real> m_arrContacts[2];
};

// Explicit instanciation
typedef TIntersectTriangle3Segment3<Float> IntersectTriangle3Segment3f;
typedef TIntersectTriangle3Segment3<Double> IntersectTriangle3Segment3d;
typedef TIntersectTriangle3Segment3<Scalar> IntersectTriangle3Segment3;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "IntersectTriangle3Segment3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_3D_PLANAR_INTERSECTTRIANGLE3SEGMENT3_H

