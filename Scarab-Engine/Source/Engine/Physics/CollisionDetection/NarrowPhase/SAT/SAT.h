/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/NarrowPhase/SAT/SAT.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Narrow-Phase : Convex, Separating Axis Theorem
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : SAT isn't the fastest algorithm and isn't very reliable either,
//              V-Clip will allways perform better ... and both need MeshGraphs.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_SAT_SAT_H
#define SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_SAT_SAT_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../../Lib/Math/Geometry/Primitives/3D/Planar/Plane.h"

#include "../../../Shape/TransformedShape.h"

#include "../NarrowPhase.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define SAT_MAX_VERTICES 256

/////////////////////////////////////////////////////////////////////////////////
// The SAT class
class SAT : public NarrowPhaseConvex
{
public:
    SAT( TransformedShapeConvex * pConvexA, TransformedShapeConvex * pConvexB );
    virtual ~SAT();

    // Narrow Collision algorithm
    virtual Void Initialize();
    virtual Bool DetectCollision( Bool bGenerateContactPoints );

private:
    // Internal routines
    static Void _ProjectAxis( Scalar * outMin, Scalar * outMax, const TransformedShapeConvex * pConvex, const Vector3 & vAxis );
    Bool _TestSeparatingAxis( Scalar * outDistance, const Vector3 & vCandidate );
    Bool _FindSeparatingAxis( Vector3 * outSeparatingAxis, Scalar * outDistance, Bool bTestOnly = false );
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "SAT.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_SAT_SAT_H

