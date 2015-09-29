/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Primitives/2D/ConvexPolygon2.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives 2D : Convex Polygon
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_2D_CONVEXPOLYGON2_H
#define SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_2D_CONVEXPOLYGON2_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Linear/Segment2.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define CONVEXPOLYGON2_MAX_VERTICES 16 // enough for now ...

/////////////////////////////////////////////////////////////////////////////////
// The TConvexPolygon2 class
template<typename Real>
class TConvexPolygon2
{
public:
	TConvexPolygon2(); // uninitialized
	TConvexPolygon2( TVertex2<Real> * arrVertices, UInt iVertexCount );
	TConvexPolygon2( const TConvexPolygon2<Real> & rhs );
	~TConvexPolygon2();

	// Operators
	TConvexPolygon2<Real> & operator=( const TConvexPolygon2<Real> & rhs );

    // Vertices access
    inline UInt GetVertexCount() const;
    inline const TVertex2<Real> & GetVertex( UInt iVertex ) const;

    inline Void SetVertex( UInt iVertex, const TVertex2<Real> & vValue );

    inline Void PushVertex( const TVertex2<Real> & vValue );
    inline Void PopVertex( TVertex2<Real> * outValue = NULL );

    Void AddVertex( UInt iVertex, const TVertex2<Real> & vValue );
    Void RemoveVertex( UInt iVertex, TVertex2<Real> * outValue = NULL );

    inline Void Clear();

    // Edge access
    inline UInt GetEdgeCount() const;
    inline Void GetEdge( TSegment2<Real> * outEdge, UInt iEdge ) const;

    // Helper methods
    Bool TestInclusion( const TVertex2<Real> & vPoint, Real fThreshold = (Real)SCALAR_ERROR ) const;

    UInt ClosestFeature( const TVertex2<Real> & vPoint, Bool * outIsEdgeElseVertex ) const;
    TVertex2<Real> ClosestPoint( const TVertex2<Real> & vPoint ) const;

    Real Distance( const TVertex2<Real> & vPoint ) const;

private:
    UInt m_iVertexCount;
    TVertex2<Real> m_arrVertices[CONVEXPOLYGON2_MAX_VERTICES];
};

// Explicit instanciation
typedef TConvexPolygon2<Float> ConvexPolygon2f;
typedef TConvexPolygon2<Double> ConvexPolygon2d;
typedef TConvexPolygon2<Scalar> ConvexPolygon2;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ConvexPolygon2.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_2D_CONVEXPOLYGON2_H

