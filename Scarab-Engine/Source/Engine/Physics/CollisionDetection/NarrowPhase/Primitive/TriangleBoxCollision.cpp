/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/NarrowPhase/Primitive/TriangleBoxCollision.cpp
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
// Includes
#include "TriangleBoxCollision.h"

/////////////////////////////////////////////////////////////////////////////////
// TriangleBoxCollision implementation
TriangleBoxCollision::TriangleBoxCollision( TransformedShapeConvex * pShapeA, TransformedShapeConvex * pShapeB ):
    VoronoiClip( pShapeA, pShapeB )
{
    // nothing to do
}
TriangleBoxCollision::~TriangleBoxCollision()
{
    // nothing to do
}

//TriangleBoxCollision::TriangleBoxCollision( TransformedShapeConvex * pShapeA, TransformedShapeConvex * pShapeB ):
//    NarrowPhase( pShapeA, pShapeB )
//{
//    if ( pShapeA->GetType() == SHAPE_PRIMITIVE_TRIANGLE ) {
//        Assert( pShapeB->GetType() == SHAPE_PRIMITIVE_BOX );
//        m_pTriangle = pShapeA;
//        m_pBox = pShapeB;
//        m_bReverseNormal = false;
//    } else {
//        Assert( pShapeA->GetType() == SHAPE_PRIMITIVE_BOX );
//        Assert( pShapeB->GetType() == SHAPE_PRIMITIVE_TRIANGLE );
//        m_pTriangle = pShapeB;
//        m_pBox = pShapeA;
//        m_bReverseNormal = true;
//    }
//}
//TriangleBoxCollision::~TriangleBoxCollision()
//{
//    // nothing to do
//}
//
//Void TriangleBoxCollision::Initialize()
//{
//    // nothing to do
//}
//Bool TriangleBoxCollision::DetectCollision( Bool bGenerateContactPoints )
//{
//    /////////////////////////////////////////////////
//    Assert( false );
//    return false;
//}
