/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/NarrowPhase/NarrowPhase.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Narrow-Phase layer for collision detection
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : Convention : Contact normals MUST allways point from A to B.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_NARROWPHASE_H
#define SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_NARROWPHASE_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../Lib/Math/Geometry/Intersection/3D/Linear/IntersectSegment3.h"
#include "../../../../Lib/Math/Geometry/Intersection/3D/Planar/IntersectPlaneLine3.h"

#include "../../Shape/TransformedShape.h"

#include "ContactManifold.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
    // Default tolerance to zero for narrow phase
#define NARROWPHASE_ZEROTOLERANCE SCALAR_ERROR

    // Maximum number of facet vertices supported
#define NARROWPHASE_CONTACTPOLYGONS_MAX_VERTICES 256

    // Normal perturbation parameters
#define NARROWPHASE_NORMALPERTURBATION_OFFSET      0.01f // Length of perturbation vector
#define NARROWPHASE_NORMALPERTURBATION_SAMPLECOUNT 8     // Number of directions to sample when
                                                         // using normal perturbation to generate
                                                         // contact points

/////////////////////////////////////////////////////////////////////////////////
// The NarrowPhase class
class NarrowPhase
{
public:
    NarrowPhase();
    virtual ~NarrowPhase();

    // Precision management
    Scalar ZeroTolerance;

    // Getters
    virtual Bool IsConvex() const = 0;
    virtual Bool IsConcave() const = 0;

    // Narrow Collision algorithm
    virtual Void Initialize() = 0;
    virtual Bool DetectCollision( Bool bGenerateContactPoints ) = 0;
};

/////////////////////////////////////////////////////////////////////////////////
// The NarrowPhaseConvex class
class NarrowPhaseConvex : public NarrowPhase
{
public:
    NarrowPhaseConvex( TransformedShapeConvex * pShapeA, TransformedShapeConvex * pShapeB );
    virtual ~NarrowPhaseConvex();

    // Getters
    inline virtual Bool IsConvex() const;
    inline virtual Bool IsConcave() const;

    inline TransformedShapeConvex * GetShapeA() const;
    inline TransformedShapeConvex * GetShapeB() const;

    // Collision manifold access
    inline const SinglePlaneManifold * GetManifold() const;

private:
    // Contact point generation
    Void _GenerateContactPoints_ClipPolygons( Vertex3 * outClippedPolygon, ContactPointID * outClippedCPIDs, UInt * outClippedPolygonSize, const Vector3 & vSeparatingAxis,
                                              const Vertex3 * arrPolyA, const ContactPointID * arrCPIDsA, UInt iPolygonASize,
                                              const Vertex3 * arrPolyB, const ContactPointID * arrCPIDsB, UInt iPolygonBSize );

    Void _GenerateContactPoints_NormalPerturbation( Vertex3 * outPolyA, ContactPointID * outCPIDsA, Vertex3 * outPolyB, ContactPointID * outCPIDsB, const Vector3 & vSeparatingAxis,
                                                    Scalar fPerturbationOffset = NARROWPHASE_NORMALPERTURBATION_OFFSET, UInt iSampleCount = NARROWPHASE_NORMALPERTURBATION_SAMPLECOUNT );

    Void _GenerateContactPoints_MostParallelFacet( Vertex3 * outPolyA, ContactPointID * outCPIDsA, UInt * outPolyASize,
                                                   Vertex3 * outPolyB, ContactPointID * outCPIDsB, UInt * outPolyBSize, Vector3 * outSeparatingAxis,
                                                   const MeshFeature * pClosestFeatureA, const MeshFeature * pClosestFeatureB );

protected:
    // Contact point generation
    Void _GenerateContactPoints( const Vector3 & vSeparatingAxis, Scalar fMinDistance = -1.0f, Scalar fMaxDistance = +1.0f );
    Void _GenerateContactPoints( const MeshFeature * pClosestFeatureA, const MeshFeature * pClosestFeatureB, Scalar fMinDistance = -1.0f, Scalar fMaxDistance = +1.0f );

    // Convex collision data
    TransformedShapeConvex * m_pShapeA;
    TransformedShapeConvex * m_pShapeB;

    SinglePlaneManifold * m_pCollisionManifold;
};

/////////////////////////////////////////////////////////////////////////////////
// The NarrowPhaseConcave class
class NarrowPhaseConcave : public NarrowPhase
{
public:
    NarrowPhaseConcave( TransformedShape * pShapeA, TransformedShape * pShapeB );
    virtual ~NarrowPhaseConcave();

    // Collision manifold access
    inline virtual Bool IsConvex() const;
    inline virtual Bool IsConcave() const;

    inline TransformedShape * GetShapeA() const;
    inline TransformedShape * GetShapeB() const;

    inline const MultiPlaneManifold * GetManifold() const;

protected:
    // Contact point generation

    // Concave collision data
    TransformedShape * m_pShapeA;
    TransformedShape * m_pShapeB;

    MultiPlaneManifold * m_pCollisionManifold;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "NarrowPhase.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_NARROWPHASE_H

