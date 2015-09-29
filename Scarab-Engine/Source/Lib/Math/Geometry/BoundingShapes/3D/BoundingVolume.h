/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Math/Geometry/BoundingShapes/3D/BoundingVolume.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Bounding Volumes base class definition ...
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : TODO = Use primitives & intersection interface
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_LIB_MATH_GEOMETRY_BOUNDINGSHAPES_3D_BOUNDINGVOLUME_H
#define SCARAB_LIB_MATH_GEOMETRY_BOUNDINGSHAPES_3D_BOUNDINGVOLUME_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Types/Transform/Transform3.h"

#include "../../Primitives/3D/Linear/Line3.h"
#include "../../Primitives/3D/Planar/Plane.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
enum BoundingVolumeShape
{
    BVSHAPE_SPHERE = 0,
    BVSHAPE_AABOX,
    BVSHAPE_OBOX,
    BVSHAPE_CAPSULE,
    BVSHAPE_DOP,
    BVSHAPE_HULL,

    BVSHAPE_COUNT
};

/////////////////////////////////////////////////////////////////////////////////
// The TBoundingVolume class
template<typename Real>
class TBoundingVolume
{
public:
    TBoundingVolume();
    virtual ~TBoundingVolume();

    // Getters
    virtual BoundingVolumeShape GetShape() const = 0;

    virtual Bool IsEmpty() const = 0;
    virtual Real GetVolume() const = 0;
    virtual Real GetSizeEstimation() const = 0;

    virtual const TVertex3<Real> & GetCenter() const = 0;

    // Construction
    virtual Void Compute( const Byte * pVertices, UInt iVertexCount, UInt iStride ) = 0;

    virtual Void Reset( const TVertex3<Real> & vCenter ) = 0;
    virtual Void Grow( const TVertex3<Real> & vVertex ) = 0;
    virtual Void Grow( const TBoundingVolume<Real> & newBV ) = 0;

    virtual Void AddMargin( Real fMargin ) = 0;

    // Transformation
    virtual Void Transform( TBoundingVolume<Real> & outBV, const TTransform3<Real> & bvTransform ) const = 0;

    // Point distance
    virtual Real Distance( const TVertex3<Real> & vPoint ) const = 0;

    // Point containment
    virtual Bool Contains( const TVertex3<Real> & vPoint ) const = 0;
    virtual Bool Contains( const TBoundingVolume<Real> & vBV ) const = 0;

    // Planar relative position
    virtual Int WhichSide( const TPlane<Real> & vPlane ) const = 0;

    // Ray casting
    virtual Bool RayCast( const TLine3<Real> & vRay, Real * outMinDistance = NULL, Real * outMaxDistance = NULL ) const = 0;

    // Static intersection
    virtual Bool Intersect( const TBoundingVolume<Real> & vBV ) const = 0;

    // Dynamic collision
    virtual Bool Collide( const TVector3<Real> & vThisSpeed, const TBoundingVolume<Real> & vBV, const TVector3<Real> & vBVSpeed,
                          Real * outTimeOfImpact = NULL ) const = 0;

};

// Explicit instanciation
typedef TBoundingVolume<Float> BoundingVolumef;
typedef TBoundingVolume<Double> BoundingVolumed;
typedef TBoundingVolume<Scalar> BoundingVolume;

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "BoundingVolume.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_LIB_MATH_GEOMETRY_BOUNDINGSHAPES_3D_BOUNDINGVOLUME_H
