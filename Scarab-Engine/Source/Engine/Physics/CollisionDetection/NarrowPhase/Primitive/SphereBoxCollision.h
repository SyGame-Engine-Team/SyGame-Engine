/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/NarrowPhase/Primitive/SphereBoxCollision.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Narrow-Phase : Primitive, Sphere-Box
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
#ifndef SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_PRIMITIVE_SPHEREBOXCOLLISION_H
#define SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_PRIMITIVE_SPHEREBOXCOLLISION_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Shape/Primitive/ShapeSphere.h"
#include "../../../Shape/Primitive/ShapeBox.h"
#include "../../../Shape/TransformedShape.h"

#include "../NarrowPhase.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The SphereBoxCollision class
class SphereBoxCollision : public NarrowPhaseConvex
{
public:
    SphereBoxCollision( TransformedShapeConvex * pShapeA, TransformedShapeConvex * pShapeB );
    virtual ~SphereBoxCollision();

    // Narrow Collision algorithm
    virtual Void Initialize();
    virtual Bool DetectCollision( Bool bGenerateContactPoints );

private:
    TransformedShapeConvex * m_pSphere;
    TransformedShapeConvex * m_pBox;

    Bool m_bReverseNormal;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "SphereBoxCollision.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_PRIMITIVE_SPHEREBOXCOLLISION_H

