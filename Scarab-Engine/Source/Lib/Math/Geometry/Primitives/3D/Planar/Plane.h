/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Primitives/3D/Planar/Plane.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives 3D : Planar, Plane
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_3D_PLANAR_PLANE_H
#define SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_3D_PLANAR_PLANE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../Types/Vector/Vector3.h"
#include "../../../../Types/Vertex/Vertex3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TPlane class
template<typename Real>
class TPlane
{
public:
    // Plane = (Normal . P) = Const, with P in the plane, Const is distance to (0,0,0)
    //      or (Normal . (P - Pos)) = 0, with P in the plane
	TPlane(); // uninitialized
	TPlane( const TVector3<Real> & vNormal, Real fConstant );
	TPlane( const TVector3<Real> & vNormal, const TVertex3<Real> & vPosition );
	TPlane( const TPlane<Real> & rhs );
	~TPlane();

	// Operators
	inline TPlane<Real> & operator=( const TPlane<Real> & rhs );

    // Update methods
    inline Void UpdatePosition();
    inline Void UpdateConstant();

    // Helper methods
    inline TVertex3<Real> ClosestPointNP( const TVertex3<Real> & vPoint ) const;
    inline TVertex3<Real> ClosestPointNC( const TVertex3<Real> & vPoint ) const;
    inline Real DistanceNP( const TVertex3<Real> & vPoint ) const;
    inline Real DistanceNC( const TVertex3<Real> & vPoint ) const;

    inline Int ClassifyPointNP( const TVertex3<Real> & vPoint, Real fZeroTolerance = (Real)SCALAR_ERROR ) const;
    inline Int ClassifyPointNC( const TVertex3<Real> & vPoint, Real fZeroTolerance = (Real)SCALAR_ERROR ) const;

    // Normal-Origin representation
    TVector3<Real> Normal;
	TVertex3<Real> Position;

    // Normal-Constant representation
    //TVector3<Real> Normal;
    Real Constant;
};

// Explicit instanciation
typedef TPlane<Float> Planef;
typedef TPlane<Double> Planed;
typedef TPlane<Scalar> Plane;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Plane.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_3D_PLANAR_PLANE_H

