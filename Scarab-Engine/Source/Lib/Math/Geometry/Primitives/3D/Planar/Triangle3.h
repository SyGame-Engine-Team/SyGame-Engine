/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Primitives/3D/Planar/Triangle3.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives 3D : Planar, Triangle3
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_3D_PLANAR_TRIANGLE3_H
#define SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_3D_PLANAR_TRIANGLE3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../Types/Vector/Vector3.h"
#include "../../../../Types/Vertex/Vertex3.h"

#include "Plane.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TTriangle3 class
template<typename Real>
class TTriangle3
{
public:
    TTriangle3(); // uninitialized
    TTriangle3( const TVertex3<Real> & vA, const TVertex3<Real> & vB, const TVertex3<Real> & vC );
    TTriangle3( const TVertex3<Real> vArr[3] );
    TTriangle3( const TTriangle3<Real> & rhs );
    ~TTriangle3();

    // Operators
	inline TTriangle3<Real> & operator=( const TTriangle3<Real> & rhs );

    // Helper methods
    inline Bool IsDegenerated( Real fZeroTolerance = (Real)SCALAR_ERROR ) const;

    inline TPlane<Real> GetPlane( Bool bCounterClockWise = true, Bool bRightHanded = true ) const;
    Bool Contains( const TVertex3<Real> & vPoint, const TVector3<Real> & vNormal ) const;

    UInt ClosestFeature( const TVertex3<Real> & vPoint, Bool * outIsEdgeElseVertex ) const;
    TVertex3<Real> ClosestPoint( const TVertex3<Real> & vPoint ) const;
    Real Distance( const TVertex3<Real> & vPoint ) const;

    // Vertex representation
    TVertex3<Real> A, B, C; // Edges AB, BC, CA
};

// Explicit instanciation
typedef TTriangle3<Float> Triangle3f;
typedef TTriangle3<Double> Triangle3d;
typedef TTriangle3<Scalar> Triangle3;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Triangle3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_3D_PLANAR_TRIANGLE3_H

