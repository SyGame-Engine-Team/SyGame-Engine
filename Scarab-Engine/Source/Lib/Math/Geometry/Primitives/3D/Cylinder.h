/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Primitives/3D/Cylinder.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives 3D : Cylinder
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_3D_CYLINDER_H
#define SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_3D_CYLINDER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../Error/ErrorManager.h"

#include "../../../Types/Vector/Vector3.h"
#include "../../../Types/Vertex/Vertex3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TCylinder class
template<typename Real>
class TCylinder
{
public:
	TCylinder(); // uninitialized
	TCylinder( const TVertex3<Real> & vCenter, const TVector3<Real> & vAxis, Real fHeight, Real fRadius );
	TCylinder( const TCylinder<Real> & rhs );
	~TCylinder();

	// Operators
	inline TCylinder<Real> & operator=( const TCylinder<Real> & rhs );

    // Helper methods
    Bool TestInclusion( const TVertex3<Real> & vPoint ) const;

    TVertex3<Real> ClosestPoint( const TVertex3<Real> & vPoint ) const;
    Real Distance( const TVertex3<Real> & vPoint ) const;

    // Center/Axis/Height/Radius representation
	TVertex3<Real> Center;
	TVector3<Real> Axis;
	Real Height;
	Real Radius;
};

// Explicit instanciation
typedef TCylinder<Float> Cylinderf;
typedef TCylinder<Double> Cylinderd;
typedef TCylinder<Scalar> Cylinder;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Cylinder.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_3D_CYLINDER_H

