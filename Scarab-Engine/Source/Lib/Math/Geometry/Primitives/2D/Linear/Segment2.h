/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Primitives/2D/Linear/Segment2.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives 2D : Linear, Segment2
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_2D_LINEAR_SEGMENT2_H
#define SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_2D_LINEAR_SEGMENT2_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../../Error/ErrorManager.h"

#include "../../../../Types/Vector/Vector2.h"
#include "../../../../Types/Vertex/Vertex2.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TSegment2 class
template<typename Real>
class TSegment2
{
public:
    // Segment = (1-T) * A + T * B, with T in [0;1]
    //        or Center + T * Dir, with T in [-Ext;+Ext], Ext >= 0
	TSegment2(); // uninitialized
	TSegment2( const TVertex2<Real> & vA, const TVertex2<Real> & vB );
    TSegment2( const TVertex2<Real> & vCenter, const TVector2<Real> & vDir, Real fExtent );
	TSegment2( const TSegment2<Real> & rhs );
	~TSegment2();

	// Operators
	inline TSegment2<Real> & operator=( const TSegment2<Real> & rhs );

    // Parameter methods
	inline TVertex2<Real> GetPointAB( Real fParameter ) const;
	inline TVertex2<Real> GetPointCDE( Real fParameter ) const;
	inline Real GetParameterAB( const TVertex2<Real> & vPoint ) const;
	inline Real GetParameterCDE( const TVertex2<Real> & vPoint ) const;

    // Update methods
    inline Void UpdateEndPoints();
    inline Void UpdateCenterDirExtent();

    // Helper methods
    TVertex2<Real> ClosestPoint( const TVertex2<Real> & vPoint, Real * outParameter = NULL ) const;
    inline Real Distance( const TVertex2<Real> & vPoint ) const;

    inline Int ClassifyPoint( const TVertex2<Real> & vPoint, Real fThreshold = (Real)SCALAR_ERROR ) const;

    // End-Points representation
    TVertex2<Real> EndA;
    TVertex2<Real> EndB;

    // Center/Direction/Extent representation
	TVertex2<Real> Center;
	TVector2<Real> Direction; // Keep normalized manually
	Real Extent;
};

// Explicit instanciation
typedef TSegment2<Float> Segment2f;
typedef TSegment2<Double> Segment2d;
typedef TSegment2<Scalar> Segment2;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Segment2.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_2D_LINEAR_SEGMENT2_H

