/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Primitives/2D/Linear/Line2.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives 2D : Linear, Line2
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_2D_LINEAR_LINE2_H
#define SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_2D_LINEAR_LINE2_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../../Error/ErrorManager.h"

#include "../../../../Types/Vector/Vector2.h"
#include "../../../../Types/Vertex/Vertex2.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TLine2 class
template<typename Real>
class TLine2
{
public:
    // Line = Pos + T * Dir, with T in ]-inf;+inf[
	TLine2(); // uninitialized
	TLine2( const TVertex2<Real> & vPos, const TVector2<Real> & vDir );
	TLine2( const TVertex2<Real> & vA, const TVertex2<Real> & vB );
	TLine2( const TLine2<Real> & rhs );
	~TLine2();

	// Operators
	inline TLine2<Real> & operator=( const TLine2<Real> & rhs );

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
typedef TLine2<Float> Line2f;
typedef TLine2<Double> Line2d;
typedef TLine2<Scalar> Line2;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Line2.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_2D_LINEAR_LINE2_H

