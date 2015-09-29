/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Primitives/2D/Linear/Ray2.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives 2D : Linear, Ray2
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_2D_LINEAR_RAY2_H
#define SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_2D_LINEAR_RAY2_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../../Error/ErrorManager.h"

#include "../../../../Types/Vector/Vector2.h"
#include "../../../../Types/Vertex/Vertex2.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TRay2 class
template<typename Real>
class TRay2
{
public:
    // Ray = Pos + T * Dir, with T in [0;+inf[
	TRay2(); // uninitialized
	TRay2( const TVertex2<Real> & vPos, const TVector2<Real> & vDir );
	TRay2( const TVertex2<Real> & vA, const TVertex2<Real> & vB );
	TRay2( const TRay2<Real> & rhs );
	~TRay2();

	// Operators
	inline TRay2<Real> & operator=( const TRay2<Real> & rhs );

    // Parameter methods
	inline TVertex2<Real> GetPoint( Real fParameter ) const;
	inline Real GetParameter( const TVertex2<Real> & vPoint ) const;

    // Helper methods
    TVertex2<Real> ClosestPoint( const TVertex2<Real> & vPoint, Real * outParameter = NULL ) const;
    inline Real Distance( const TVertex2<Real> & vPoint ) const;

    inline Int ClassifyPoint( const TVertex2<Real> & vPoint, Real fThreshold = (Real)SCALAR_ERROR ) const;

    // Pos/Dir representation
	TVertex2<Real> Position;
	TVector2<Real> Direction; // Keep normalized manually
};

// Explicit instanciation
typedef TRay2<Float> Ray2f;
typedef TRay2<Double> Ray2d;
typedef TRay2<Scalar> Ray2;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Ray2.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_2D_LINEAR_RAY2_H

