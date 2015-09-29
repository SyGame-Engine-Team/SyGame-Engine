/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Primitives/3D/Linear/Ray3.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives 3D : Linear, Ray3
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_3D_LINEAR_RAY3_H
#define SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_3D_LINEAR_RAY3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../../Error/ErrorManager.h"

#include "../../../../Types/Vector/Vector3.h"
#include "../../../../Types/Vertex/Vertex3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TRay3 class
template<typename Real>
class TRay3
{
public:
    // Ray = Pos + T * Dir, with T in [0;+inf[
	TRay3(); // uninitialized
	TRay3( const TVertex3<Real> & vPos, const TVector3<Real> & vDir );
	TRay3( const TVertex3<Real> & vA, const TVertex3<Real> & vB );
	TRay3( const TRay3<Real> & rhs );
	~TRay3();

	// Operators
	inline TRay3<Real> & operator=( const TRay3<Real> & rhs );

    // Parameter methods
	inline TVertex3<Real> GetPoint( Real fParameter ) const;
	inline Real GetParameter( const TVertex3<Real> & vPoint ) const;

    // Helper methods
    TVertex3<Real> ClosestPoint( const TVertex3<Real> & vPoint, Real * outParameter = NULL ) const;
    Real Distance( const TVertex3<Real> & vPoint ) const;

    // Pos/Dir representation
	TVertex3<Real> Position;
	TVector3<Real> Direction; // Keep normalized manually
};

// Explicit instanciation
typedef TRay3<Float> Ray3f;
typedef TRay3<Double> Ray3d;
typedef TRay3<Scalar> Ray3;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Ray3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_3D_LINEAR_RAY3_H

