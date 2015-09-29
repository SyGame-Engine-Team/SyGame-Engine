/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Intersection/Intersector2.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Base class for intersection of primitives in 2D
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_INTERSECTOR2_H
#define SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_INTERSECTOR2_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Error/ErrorManager.h"

#include "../../Types/Vector/Vector2.h"
#include "../../Types/Vertex/Vertex2.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
enum Intersect2Type
{
    INTERSECT2D_EMPTY = 0,

    INTERSECT2D_POINT,
    INTERSECT2D_LINE,
    INTERSECT2D_RAY,
    INTERSECT2D_SEGMENT,
    INTERSECT2D_POLYGON,

    INTERSECT2D_OTHER
};

typedef DWord Intersect2Specifier;
#define INTERSECT2D_SPECIFIER_NONE 0

/////////////////////////////////////////////////////////////////////////////////
// The TIntersector2 interface
template<typename Real>
class TIntersector2
{
public:
    TIntersector2( Real fZeroTolerance = (Real)SCALAR_ERROR );
    virtual ~TIntersector2();

    // Precision management
    Real ZeroTolerance;

    // Computed results access
    inline Intersect2Type GetType() const;
    inline Intersect2Specifier GetSpecifier() const;

    inline Real GetTOI() const;

    virtual UInt GetContactCount() const = 0;
    virtual const TVertex2<Real> & GetContact( UInt iContact ) const = 0;
    virtual const TVertex2<Real> * GetContactArray() const = 0;

    // Static intersection
    virtual Bool Test();
    virtual Bool Compute();

    // Dynamic intersection
    virtual Bool Test( const TVector2<Real> & vSpeedA, const TVector2<Real> & vSpeedB, Real fMaxTime );
    virtual Bool Compute( const TVector2<Real> & vSpeedA, const TVector2<Real> & vSpeedB, Real fMaxTime );

protected:
    Intersect2Type m_iType;
    Intersect2Specifier m_iSpecifier;

    Real m_fTimeOfImpact;
};

// Explicit instanciation
typedef TIntersector2<Float> Intersector2f;
typedef TIntersector2<Double> Intersector2d;
typedef TIntersector2<Scalar> Intersector2;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Intersector2.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_INTERSECTION_INTERSECTOR2_H

