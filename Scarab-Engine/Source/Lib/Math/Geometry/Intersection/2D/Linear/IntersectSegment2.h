/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Intersection/2D/Linear/IntersectSegment2.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Intersection 2D : Segment2 and Segment2
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_2D_LINEAR_INTERSECTSEGMENT2_H
#define SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_2D_LINEAR_INTERSECTSEGMENT2_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Primitives/2D/Linear/Segment2.h"

#include "../../Intersector2.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Specifiers
    // Junction cases
#define INTERSECTSEGMENT2_SPEC_ISJUNCTION(_spec)  ( ((_spec) & 0x0000000f) != 0 )
#define INTERSECTSEGMENT2_SPEC_GETJUNCTION(_spec) ( (_spec) & 0x0000000f )

#define INTERSECTSEGMENT2_SPEC_TJUNCTION_A  0x00000001
#define INTERSECTSEGMENT2_SPEC_TJUNCTION_B  0x00000002
#define INTERSECTSEGMENT2_SPEC_TJUNCTION_C  0x00000004
#define INTERSECTSEGMENT2_SPEC_TJUNCTION_D  0x00000008

#define INTERSECTSEGMENT2_SPEC_LJUNCTION_AC ( INTERSECTSEGMENT2_SPEC_TJUNCTION_A | INTERSECTSEGMENT2_SPEC_TJUNCTION_C )
#define INTERSECTSEGMENT2_SPEC_LJUNCTION_AD ( INTERSECTSEGMENT2_SPEC_TJUNCTION_A | INTERSECTSEGMENT2_SPEC_TJUNCTION_D )
#define INTERSECTSEGMENT2_SPEC_LJUNCTION_BC ( INTERSECTSEGMENT2_SPEC_TJUNCTION_B | INTERSECTSEGMENT2_SPEC_TJUNCTION_C )
#define INTERSECTSEGMENT2_SPEC_LJUNCTION_BD ( INTERSECTSEGMENT2_SPEC_TJUNCTION_B | INTERSECTSEGMENT2_SPEC_TJUNCTION_D )

    // Parallel / Colinear cases
#define INTERSECTSEGMENT2_SPEC_ISPARALLEL(_spec) ( ((_spec) & 0x000000f0) == INTERSECTSEGMENT2_SPEC_PARALLEL )
#define INTERSECTSEGMENT2_SPEC_ISCOLINEAR(_spec) ( ((_spec) & 0x000000f0) == INTERSECTSEGMENT2_SPEC_COLINEAR )

#define INTERSECTSEGMENT2_SPEC_PARALLEL        0x00000010
#define INTERSECTSEGMENT2_SPEC_COLINEAR        0x00000020

    // Colinear sub-cases
#define INTERSECTSEGMENT2_SPEC_ISEQUALITY(_spec)  ( ((_spec) & 0xf0000000) != 0 )
#define INTERSECTSEGMENT2_SPEC_ISDISJOINT(_spec)  ( ((_spec) & 0x0f000000) != 0 )
#define INTERSECTSEGMENT2_SPEC_ISINCLUSION(_spec) ( ((_spec) & 0x00f00000) != 0 )
#define INTERSECTSEGMENT2_SPEC_ISOVERLAP(_spec)   ( ((_spec) & 0x000f0000) != 0 )

#define INTERSECTSEGMENT2_SPEC_EQUALITY        0x10000000
#define INTERSECTSEGMENT2_SPEC_EQUALITY_INV    0x20000000

#define INTERSECTSEGMENT2_SPEC_DISJOINT_ABCD   0x01000000
#define INTERSECTSEGMENT2_SPEC_DISJOINT_CDAB   0x02000000

#define INTERSECTSEGMENT2_SPEC_INCLUSION_AB    0x00100000
#define INTERSECTSEGMENT2_SPEC_INCLUSION_CD    0x00200000

#define INTERSECTSEGMENT2_SPEC_OVERLAP_ACBD    0x00010000
#define INTERSECTSEGMENT2_SPEC_OVERLAP_ADBC    0x00020000
#define INTERSECTSEGMENT2_SPEC_OVERLAP_CADB    0x00040000
#define INTERSECTSEGMENT2_SPEC_OVERLAP_DACB    0x00080000

/////////////////////////////////////////////////////////////////////////////////
// The TIntersectSegment2 class
template<typename Real>
class TIntersectSegment2 : public TIntersector2<Real>
{
public:
    TIntersectSegment2( const TSegment2<Real> & rSegmentA, const TSegment2<Real> & rSegmentB,
                        Real fZeroTolerance = (Real)SCALAR_ERROR );
    virtual ~TIntersectSegment2();

    // Getters
    inline const TSegment2<Real> & GetSegmentA() const;
    inline const TSegment2<Real> & GetSegmentB() const;

    // Computed results access
    inline virtual UInt GetContactCount() const;
    inline virtual const TVertex2<Real> & GetContact( UInt iContact ) const;
    inline virtual const TVertex2<Real> * GetContactArray() const;

    inline Real GetSegmentAParameter( UInt iContact ) const;
    inline Real GetSegmentBParameter( UInt iContact ) const;

    // Static intersection
    virtual Bool Test();
    virtual Bool Compute();

    // Dynamic Intersection : NOT IMPLEMENTED

private:
    // Helpers
    Int _InclusionTest( const TVertex2<Real> & vA, const TVertex2<Real> & vB,
                        const TVertex2<Real> & vIsIncluded ) const;
    Bool _ParallelCompute();

    // Intersection data
    const TSegment2<Real> * m_pSegmentA;
    const TSegment2<Real> * m_pSegmentB;

    UInt m_iContactCount;
    TVertex2<Real> m_arrContacts[2];

    Real m_fSegmentAParameter[2];
    Real m_fSegmentBParameter[2];
};

// Explicit instanciation
typedef TIntersectSegment2<Float> IntersectSegment2f;
typedef TIntersectSegment2<Double> IntersectSegment2d;
typedef TIntersectSegment2<Scalar> IntersectSegment2;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "IntersectSegment2.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_2D_LINEAR_INTERSECTSEGMENT2_H

