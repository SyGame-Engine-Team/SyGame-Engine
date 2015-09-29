/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/NarrowPhase/Primitive/TriangleSphereCollision.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Narrow-Phase : Primitive, Triangle-Sphere
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
#ifndef SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_PRIMITIVE_TRIANGLESPHERECOLLISION_H
#define SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_PRIMITIVE_TRIANGLESPHERECOLLISION_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../../Lib/Math/Geometry/Primitives/3D/Linear/Segment3.h"

#include "../../../Shape/Primitive/ShapeTriangle.h"
#include "../../../Shape/Primitive/ShapeSphere.h"
#include "../../../Shape/TransformedShape.h"

#include "../NarrowPhase.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TriangleSphereCollision class
class TriangleSphereCollision : public NarrowPhaseConvex
{
public:
    TriangleSphereCollision( TransformedShapeConvex * pShapeA, TransformedShapeConvex * pShapeB );
    virtual ~TriangleSphereCollision();

    // Narrow Collision algorithm
    virtual Void Initialize();
    virtual Bool DetectCollision( Bool bGenerateContactPoints );

private:
    TransformedShapeConvex * m_pTriangle;
    TransformedShapeConvex * m_pSphere;

    Bool m_bReverseNormal;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "TriangleSphereCollision.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_PRIMITIVE_TRIANGLESPHERECOLLISION_H

