/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/NarrowPhase/Convex/ConvexConvexCollision.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Narrow-Phase : Convex, Convex-Convex
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
#ifndef SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_CONVEX_CONVEXCONVEXCOLLISION_H
#define SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_CONVEX_CONVEXCONVEXCOLLISION_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../VClip/VoronoiClip.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The ConvexConvexCollision class
class ConvexConvexCollision : public VoronoiClip
{
public:
    ConvexConvexCollision( TransformedShapeConvex * pShapeA, TransformedShapeConvex * pShapeB );
    virtual ~ConvexConvexCollision();
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ConvexConvexCollision.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_CONVEX_CONVEXCONVEXCOLLISION_H

