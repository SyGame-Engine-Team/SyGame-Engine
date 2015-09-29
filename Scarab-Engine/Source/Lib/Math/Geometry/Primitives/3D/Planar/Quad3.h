/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Primitives/3D/Planar/Quad3.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives 3D : Planar, Quad3
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs :
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_3D_PLANAR_QUAD3_H
#define SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_3D_PLANAR_QUAD3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../Types/Vector/Vector3.h"
#include "../../../../Types/Vertex/Vertex3.h"

#include "Plane.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TQuad3 class
template<typename Real>
class TQuad3
{
public:
    TQuad3(); // uninitialized
    TQuad3( const TVertex3<Real> & vA, const TVertex3<Real> & vB,
            const TVertex3<Real> & vC, const TVertex3<Real> & vD );
    TQuad3( const TVertex3<Real> vArr[4] );
    TQuad3( const TQuad3<Real> & rhs );
    ~TQuad3();

    // Operators
	inline TQuad3<Real> & operator=( const TQuad3<Real> & rhs );

    // Helper methods
    inline Bool IsCoplanar( Real fZeroTolerance = (Real)SCALAR_ERROR ) const;

    inline TPlane<Real> GetPlane( Bool bCounterClockWise = true, Bool bRightHanded = true ) const;

    UInt ClosestFeature( const TVertex3<Real> & vPoint, Bool * outIsEdgeElseVertex ) const;
    TVertex3<Real> ClosestPoint( const TVertex3<Real> & vPoint ) const;
    Real Distance( const TVertex3<Real> & vPoint ) const;

    // Vertex representation
    TVertex3<Real> A, B, C, D; // Edges AB, BC, CD, DA
};

// Explicit instanciation
typedef TQuad3<Float> Quad3f;
typedef TQuad3<Double> Quad3d;
typedef TQuad3<Scalar> Quad3;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Quad3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_3D_PLANAR_QUAD3_H

