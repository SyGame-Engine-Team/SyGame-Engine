/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Primitives/3D/Sphere.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives 3D : Sphere
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_3D_SPHERE_H
#define SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_3D_SPHERE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../Error/ErrorManager.h"

#include "../../../Types/Vector/Vector3.h"
#include "../../../Types/Vertex/Vertex3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TSphere class
template<typename Real>
class TSphere
{
public:
	TSphere(); // uninitialized
	TSphere( const TVertex3<Real> & vCenter, Real fRadius );
	TSphere( const TSphere<Real> & rhs );
	~TSphere();

	// Operators
	inline TSphere<Real> & operator=( const TSphere<Real> & rhs );

    // Helper methods
    inline Bool TestInclusion( const TVertex3<Real> & vPoint ) const;

    inline TVertex3<Real> ClosestPoint( const TVertex3<Real> & vPoint ) const;
    inline Real Distance( const TVertex3<Real> & vPoint ) const;

    // Center/Radius representation
	TVertex3<Real> Center;
	Real Radius;
};

// Explicit instanciation
typedef TSphere<Float> Spheref;
typedef TSphere<Double> Sphered;
typedef TSphere<Scalar> Sphere;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Sphere.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_3D_SPHERE_H
