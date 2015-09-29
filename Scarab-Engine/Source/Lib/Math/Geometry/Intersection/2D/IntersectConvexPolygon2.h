/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Intersection/2D/IntersectConvexPolygon2.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Intersection 2D : ConvexPolygon2 and ConvexPolygon2
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : No colinear edge allowed !
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_2D_INTERSECTCONVEXPOLYGON2_H
#define SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_2D_INTERSECTCONVEXPOLYGON2_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Primitives/2D/ConvexPolygon2.h"

#include "../Intersector2.h"

#include "Linear/IntersectSegment2.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Specifiers
#define INTERSECTCONVEXPOLYGON2_SPEC_ISINCLUSION(_spec) ( ((_spec) & 0x0000000f) != 0 )

#define INTERSECTCONVEXPOLYGON2_SPEC_INCLUSION_A      0x00000001 // A in B
#define INTERSECTCONVEXPOLYGON2_SPEC_INCLUSION_B      0x00000002 // B in A
#define INTERSECTCONVEXPOLYGON2_SPEC_EQUALITY         0x00000003 // A == B
#define INTERSECTCONVEXPOLYGON2_SPEC_INCLUSION_STRICT 0x00000008 // strict else touching

#define INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_A 0x00000001 // this contact belongs to a vertex from A
#define INTERSECTCONVEXPOLYGON2_SPEC_VERTEX_B 0x00000002 // this contact belongs to a vertex from B
#define INTERSECTCONVEXPOLYGON2_SPEC_EDGE_A   0x00000004 // this contact belongs to an egde from A
#define INTERSECTCONVEXPOLYGON2_SPEC_EDGE_B   0x00000008 // this contact belongs to an egde from B

/////////////////////////////////////////////////////////////////////////////////
// The TIntersectConvexPolygon2 class
template<typename Real>
class TIntersectConvexPolygon2 : public TIntersector2<Real>
{
public:
    TIntersectConvexPolygon2( const TConvexPolygon2<Real> & rPolygonA, const TConvexPolygon2<Real> & rPolygonB,
                              Real fZeroTolerance = (Real)SCALAR_ERROR );
    virtual ~TIntersectConvexPolygon2();

    // Getters
    inline const TConvexPolygon2<Real> & GetPolygonA() const;
    inline const TConvexPolygon2<Real> & GetPolygonB() const;

    // Computed results access
    inline virtual UInt GetContactCount() const;
    inline virtual const TVertex2<Real> & GetContact( UInt iContact ) const;
    inline virtual const TVertex2<Real> * GetContactArray() const;

    inline Intersect2Specifier GetContactSpecifier( UInt iContact ) const;
    inline UInt GetContactIndexA( UInt iContact ) const;
    inline UInt GetContactIndexB( UInt iContact ) const;
    inline Real GetContactParameterA( UInt iContact ) const;
    inline Real GetContactParameterB( UInt iContact ) const;

    // Static intersection
    virtual Bool Test();
    virtual Bool Compute();

    // Dynamic Intersection : NOT IMPLEMENTED

private:
    // Helpers
    enum _SickleState {
        _SICKLE_UNKNOWN = 0, // Lookup phase
        _SICKLE_INNER_A = 1, // Sickle phase, A is the inner polygon
        _SICKLE_INNER_B = 2  // Sickle phase, B is the inner polygon
    };

    inline Void _Exclude_A();
    inline Void _Exclude_B();
    inline Void _Exclude_Both();

    inline Void _AddContact( Intersect2Specifier iSpecifier, UInt iIndexA, UInt iIndexB,
                             const TVertex2<Real> & vPosition, Real fParameterA, Real fParameterB );

    static Void _ConvertSpecifier( Intersect2Specifier * outSpecifier0, Intersect2Specifier * outSpecifier1,
                                   Intersect2Specifier iSegmentSpecifier );

    // Intersection data
    const TConvexPolygon2<Real> * m_pPolygonA;
    const TConvexPolygon2<Real> * m_pPolygonB;

    typedef struct _contact_data {
        Intersect2Specifier iSpecifier;
        UInt iIndexA, iIndexB;
        Real fParameterA, fParameterB;
    } _ContactData;

    UInt m_iContactCount;
    TVertex2<Real> m_arrContacts[CONVEXPOLYGON2_MAX_VERTICES * 2];
    _ContactData m_arrContactsData[CONVEXPOLYGON2_MAX_VERTICES * 2];
};

// Explicit instanciation
typedef TIntersectConvexPolygon2<Float> IntersectConvexPolygon2f;
typedef TIntersectConvexPolygon2<Double> IntersectConvexPolygon2d;
typedef TIntersectConvexPolygon2<Scalar> IntersectConvexPolygon2;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "IntersectConvexPolygon2.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_2D_INTERSECTCONVEXPOLYGON2_H

