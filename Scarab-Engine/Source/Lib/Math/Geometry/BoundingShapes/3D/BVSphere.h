/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/BoundingShapes/3D/BVSphere.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Bounding Volumes : Sphere
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
#ifndef SCARAB_LIB_MATH_GEOMETRY_BOUNDINGSHAPES_3D_BVSPHERE_H
#define SCARAB_LIB_MATH_GEOMETRY_BOUNDINGSHAPES_3D_BVSPHERE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Random/Random.h"

#include "BoundingVolume.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TBVSphere class
template<typename Real>
class TBVSphere : public TBoundingVolume<Real>
{
public:
    TBVSphere();
	TBVSphere( const TVertex3<Real> & vCenter, Real fRadius );
	TBVSphere( const TVertex3<Real> * arrVertices, UInt iVertexCount );
    TBVSphere( const TBVSphere<Real> & rhs );
    virtual ~TBVSphere();

    // Operators
    inline TBVSphere<Real> & operator=( const TBVSphere<Real> & rhs );

    inline Bool operator==( const TBVSphere<Real> & rhs ) const;
    inline Bool operator!=( const TBVSphere<Real> & rhs ) const;

    // Getters
    inline virtual BoundingVolumeShape GetShape() const;

    inline virtual Bool IsEmpty() const;
    inline virtual Real GetVolume() const;
    inline virtual Real GetSizeEstimation() const;

    inline virtual const TVertex3<Real> & GetCenter() const;

    inline Real GetRadius() const;

    // Construction
    virtual Void Compute( const Byte * pVertices, UInt iVertexCount, UInt iStride );

    inline virtual Void Reset( const TVertex3<Real> & vCenter );
    inline virtual Void Grow( const TVertex3<Real> & vVertex );
    inline virtual Void Grow( const TBoundingVolume<Real> & newBV );

    inline virtual Void AddMargin( Real fMargin );

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
    // Specific methods
    Void _BuildFromAABox( const Byte * pVertices, UInt iVertexCount, UInt iStride );
    Void _MergeWithPoint( const TVertex3<Real> & vVertex );
    Void _MergeWithSphere( const TBVSphere<Real> & bvSphere );

    Void _Refine( const Byte * pVertices, UInt iVertexCount, UInt iStride, UInt iIterations = 8 );

    TVertex3<Real> m_vCenter;
    Real m_fRadius;
};

// Explicit instanciation
typedef TBVSphere<Float> BVSpheref;
typedef TBVSphere<Double> BVSphered;
typedef TBVSphere<Scalar> BVSphere;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "BVSphere.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_BOUNDINGSHAPES_3D_BVSPHERE_H
