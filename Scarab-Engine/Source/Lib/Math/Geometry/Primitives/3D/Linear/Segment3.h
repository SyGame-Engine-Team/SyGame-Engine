/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/Primitives/3D/Linear/Segment3.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Primitives 3D : Linear, Segment3
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_3D_LINEAR_SEGMENT3_H
#define SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_3D_LINEAR_SEGMENT3_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../../Error/ErrorManager.h"

#include "../../../../Types/Vector/Vector3.h"
#include "../../../../Types/Vertex/Vertex3.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TSegment3 class
template<typename Real>
class TSegment3
{
public:
    // Segment = (1-T) * A + T * B, with T in [0;1]
    //        or Center + T * Dir, with T in [-Ext;+Ext], Ext >= 0
	TSegment3(); // uninitialized
	TSegment3( const TVertex3<Real> & vA, const TVertex3<Real> & vB );
    TSegment3( const TVertex3<Real> & vCenter, const TVector3<Real> & vDir, Real fExtent );
	TSegment3( const TSegment3<Real> & rhs );
	~TSegment3();

	// Operators
	inline TSegment3<Real> & operator=( const TSegment3<Real> & rhs );

    // Parameter methods
	inline TVertex3<Real> GetPointAB( Real fParameter ) const;
	inline TVertex3<Real> GetPointCDE( Real fParameter ) const;
	inline Real GetParameterAB( const TVertex3<Real> & vPoint ) const;
	inline Real GetParameterCDE( const TVertex3<Real> & vPoint ) const;

    // Update methods
    inline Void UpdateEndPoints();
    inline Void UpdateCenterDirExtent();

    // Helper methods
    TVertex3<Real> ClosestPoint( const TVertex3<Real> & vPoint, Real * outParameter = NULL ) const;
    Real Distance( const TVertex3<Real> & vPoint ) const;

    // End-Points representation
    TVertex3<Real> EndA;
    TVertex3<Real> EndB;

    // Center/Direction/Extent representation
	TVertex3<Real> Center;
	TVector3<Real> Direction; // Keep normalized manually
	Real Extent;
};

// Explicit instanciation
typedef TSegment3<Float> Segment3f;
typedef TSegment3<Double> Segment3d;
typedef TSegment3<Scalar> Segment3;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Segment3.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_PRIMITIVES_3D_LINEAR_SEGMENT3_H

