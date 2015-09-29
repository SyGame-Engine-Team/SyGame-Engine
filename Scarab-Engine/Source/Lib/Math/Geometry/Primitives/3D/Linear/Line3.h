/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Primitives/3D/Linear/Line3.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives 3D : Linear, Line3
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_3D_LINEAR_LINE3_H
#define SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_3D_LINEAR_LINE3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../../Error/ErrorManager.h"

#include "../../../../Types/Vector/Vector3.h"
#include "../../../../Types/Vertex/Vertex3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TLine3 class
template<typename Real>
class TLine3
{
public:
    // Line = Pos + T * Dir, with T in ]-inf;+inf[
	TLine3(); // uninitialized
	TLine3( const TVertex3<Real> & vPos, const TVector3<Real> & vDir );
	TLine3( const TVertex3<Real> & vA, const TVertex3<Real> & vB );
	TLine3( const TLine3<Real> & rhs );
	~TLine3();

	// Operators
	inline TLine3<Real> & operator=( const TLine3<Real> & rhs );

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
typedef TLine3<Float> Line3f;
typedef TLine3<Double> Line3d;
typedef TLine3<Scalar> Line3;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Line3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_3D_LINEAR_LINE3_H

