/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/BoundingShapes/3D/BVAABox.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Bounding Volumes : Axis-Aligned Box
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_BOUNDINGSHAPES_3D_BVAABOX_H
#define SCARAB_LIB_MATH_GEOMETRY_BOUNDINGSHAPES_3D_BVAABOX_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "BoundingVolume.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TBVAABox class
template<typename Real>
class TBVAABox : public TBoundingVolume<Real>
{
public:
	TBVAABox();
	TBVAABox( const TVertex3<Real> & vCenter, Real fRadius );
	TBVAABox( const TVertex3<Real> & vCenter, const TVector3<Real> & vExtents );
	TBVAABox( const TVertex3<Real> & vMin, const TVertex3<Real> & vMax );
	TBVAABox( const TVertex3<Real> * arrVertices, UInt iVertexCount );
    TBVAABox( const TBVAABox<Real> & rhs );
    virtual ~TBVAABox();

    // Operators
    inline TBVAABox<Real> & operator=( const TBVAABox<Real> & rhs );

    inline Bool operator==( const TBVAABox<Real> & rhs ) const;
    inline Bool operator!=( const TBVAABox<Real> & rhs ) const;

    // Getters
    inline virtual BoundingVolumeShape GetShape() const;

    inline virtual Bool IsEmpty() const;
    inline virtual Real GetVolume() const;
    inline virtual Real GetSizeEstimation() const;

    inline virtual const TVertex3<Real> & GetCenter() const;

    inline const TVector3<Real> & GetExtents() const;
    inline TVector3<Real> GetLengths() const;

    inline TVertex3<Real> GetMin() const;
    inline TVertex3<Real> GetMax() const;

    // Construction
    virtual Void Compute( const Byte * pVertices, UInt iVertexCount, UInt iStride );

    virtual Void Reset( const TVertex3<Real> & vCenter );
    virtual Void Grow( const TVertex3<Real> & vVertex );
    virtual Void Grow( const TBoundingVolume<Real> & newBV );

    inline virtual Void AddMargin( Real fMargin );

    inline Void Expand( const TVector3<Real> & vExtent );
    Void ExpandMinMax( const TVector3<Real> & vExtent );

    // Transformation
    virtual Void Transform( TBoundingVolume<Real> & outBV, const TTransform3<Real> & bvTransform ) const;

    // Point distance
    virtual Real Distance( const TVertex3<Real> & vPoint ) const;

    // Point containment
    virtual Bool Contains( const TVertex3<Real> & vPoint ) const;
    virtual Bool Contains( const TBoundingVolume<Real> & vBV ) const;

    // Planar relative position
    virtual Int WhichSide( const TPlane<Real> & vPlane ) const;

    // Ray casting
    virtual Bool RayCast( const TLine3<Real> & vRay, Real * outMinDistance = NULL, Real * outMaxDistance = NULL ) const;

    // Static intersection
    virtual Bool Intersect( const TBoundingVolume<Real> & vBV ) const;

    // Dynamic collision
    virtual Bool Collide( const TVector3<Real> & vThisSpeed, const TBoundingVolume<Real> & vBV, const TVector3<Real> & vBVSpeed,
                          Real * outTimeOfImpact = NULL ) const;

private:
	TVertex3<Real> m_vCenter;
    TVector3<Real> m_vExtents;
};

// Explicit instanciation
typedef TBVAABox<Float> BVAABoxf;
typedef TBVAABox<Double> BVAABoxd;
typedef TBVAABox<Scalar> BVAABox;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "BVAABox.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_BOUNDINGSHAPES_3D_BVAABOX_H
