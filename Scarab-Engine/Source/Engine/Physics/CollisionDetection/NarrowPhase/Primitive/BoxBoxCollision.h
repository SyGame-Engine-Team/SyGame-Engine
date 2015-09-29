/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/NarrowPhase/Primitive/BoxBoxCollision.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Narrow-Phase : Primitive, Box-Box
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
#ifndef SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_PRIMITIVE_BOXBOXCOLLISION_H
#define SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_PRIMITIVE_BOXBOXCOLLISION_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../VClip/VoronoiClip.h"
//#include "../GJK/GJK.h"

//#include "../../../Shape/Primitive/ShapeBox.h"
//#include "../../../Shape/TransformedShape.h"
//
//#include "../NarrowPhase.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The BoxBoxCollision class
class BoxBoxCollision : public VoronoiClip
{
public:
    BoxBoxCollision( TransformedShapeConvex * pBoxA, TransformedShapeConvex * pBoxB );
    virtual ~BoxBoxCollision();
};

//class BoxBoxCollision : public NarrowPhase
//{
//public:
//    BoxBoxCollision( TransformedShapeConvex * pBoxA, TransformedShapeConvex * pBoxB );
//    virtual ~BoxBoxCollision();
//
//    // Narrow Collision algorithm
//    virtual Void Initialize();
//    virtual Bool DetectCollision( Bool bGenerateContactPoints );
//
//private:
//    //// Helpers
//    //Void _EdgesClosestPoints( Scalar * outParamA, Scalar * outParamB,
//    //                          const Vertex3 & vA, const Vector3 & vAxisA,
//    //                          const Vertex3 & vB, const Vector3 & vAxisB );
//    //UInt _ClipFacePolygons( Vertex2 outClippedPolygon[8],
//    //                        Vertex2 vQuadIncident[4], const Vertex2 & vRectReferenceSize );
//    //Void _CullContactPolygon( UInt outCulledPolygon[NARROWPHASE_MAX_CONTACTPOINTS],
//    //                          Vertex2 vClippedPolygon[8], UInt iClippedPolygonSize, UInt iMustKeep );
//};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "BoxBoxCollision.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_PRIMITIVE_BOXBOXCOLLISION_H

