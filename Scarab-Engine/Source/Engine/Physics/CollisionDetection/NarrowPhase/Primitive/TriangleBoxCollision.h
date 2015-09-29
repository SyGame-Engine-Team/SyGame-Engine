/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/NarrowPhase/Primitive/TriangleBoxCollision.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Narrow-Phase : Primitive, Triangle-Box
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
#ifndef SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_PRIMITIVE_TRIANGLEBOXCOLLISION_H
#define SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_PRIMITIVE_TRIANGLEBOXCOLLISION_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../VClip/VoronoiClip.h"

//#include "../../../Shape/Primitive/ShapeTriangle.h"
//#include "../../../Shape/Primitive/ShapeBox.h"
//#include "../../../Shape/TransformedShape.h"
//
//#include "../NarrowPhase.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The TriangleBoxCollision class
class TriangleBoxCollision : public VoronoiClip
{
public:
    TriangleBoxCollision( TransformedShapeConvex * pShapeA, TransformedShapeConvex * pShapeB );
    virtual ~TriangleBoxCollision();
};

//class TriangleBoxCollision : public NarrowPhase
//{
//public:
//    TriangleBoxCollision( TransformedShapeConvex * pShapeA, TransformedShapeConvex * pShapeB );
//    virtual ~TriangleBoxCollision();
//
//    // Narrow Collision algorithm
//    virtual Void Initialize();
//    virtual Bool DetectCollision( Bool bGenerateContactPoints );
//
//private:
//    TransformedShapeConvex * m_pTriangle;
//    TransformedShapeConvex * m_pBox;
//
//    Bool m_bReverseNormal;
//};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "TriangleBoxCollision.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_PRIMITIVE_TRIANGLEBOXCOLLISION_H

