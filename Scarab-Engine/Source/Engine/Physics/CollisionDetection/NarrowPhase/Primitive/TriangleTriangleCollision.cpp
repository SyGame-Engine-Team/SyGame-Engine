/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/NarrowPhase/Primitive/TriangleTriangleCollision.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Narrow-Phase : Primitive, Triangle-Triangle
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "TriangleTriangleCollision.h"

/////////////////////////////////////////////////////////////////////////////////
// TriangleTriangleCollision implementation
TriangleTriangleCollision::TriangleTriangleCollision( TransformedShapeConvex * pTriangleA, TransformedShapeConvex * pTriangleB ):
    VoronoiClip( pTriangleA, pTriangleB )
{
    // nothing to do
}
TriangleTriangleCollision::~TriangleTriangleCollision()
{
    // nothing to do
}

//TriangleTriangleCollision::TriangleTriangleCollision( TransformedShapeConvex * pTriangleA, TransformedShapeConvex * pTriangleB ):
//    NarrowPhase( pTriangleA, pTriangleB )
//{
//    Assert( pTriangleA->GetType() == SHAPE_PRIMITIVE_TRIANGLE );
//    Assert( pTriangleB->GetType() == SHAPE_PRIMITIVE_TRIANGLE );
//}
//TriangleTriangleCollision::~TriangleTriangleCollision()
//{
//    // nothing to do
//}
//
//Void TriangleTriangleCollision::Initialize()
//{
//    // nothing to do
//}
//Bool TriangleTriangleCollision::DetectCollision( Bool bGenerateContactPoints )
//{
//    /////////////////////////////////////////////////
//    Assert( false );
//    return false;
//}

