/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/IslandSolver/Edges/ContactEdge.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Island edges : Contact
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
#include "ContactEdge.h"

#include "../../PhysicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// ContactEdge implementation
ContactEdge::ContactEdge( BodyNode * pNodeA, BodyNode * pNodeB ):
    IslandEdge( pNodeA, pNodeB )
{
    PhysicsFn->SelectMemory( TEXT("Narrowphase") );

    m_pNarrowPhase = _SelectNarrowAlgorithm( pNodeA->GetWorldShape(), pNodeB->GetWorldShape() );
    m_pNarrowPhase->Initialize();

    if ( pNodeA->GetWorldShape()->IsConcave() || pNodeB->GetWorldShape()->IsConcave() )
        m_pContactManifold = New MultiPlaneManifold();
    else
        m_pContactManifold = New SinglePlaneManifold();

    PhysicsFn->UnSelectMemory();
}
ContactEdge::~ContactEdge()
{
    PhysicsFn->SelectMemory( TEXT("Narrowphase") );
    Delete( m_pContactManifold );
    Delete( m_pNarrowPhase );
    PhysicsFn->UnSelectMemory();
}

/////////////////////////////////////////////////////////////////////////////////

NarrowPhase * ContactEdge::_SelectNarrowAlgorithm( TransformedShape * pShapeA, TransformedShape * pShapeB )
{
    ShapeType iTypeA = pShapeA->GetType();
    ShapeType iTypeB = pShapeB->GetType();

    if ( iTypeA == SHAPE_PRIMITIVE_TRIANGLE ) {
        if ( iTypeB == SHAPE_PRIMITIVE_TRIANGLE )
            return New TriangleTriangleCollision( (TransformedShapeConvex*)pShapeA, (TransformedShapeConvex*)pShapeB );
        if ( iTypeB == SHAPE_PRIMITIVE_SPHERE )
            return New TriangleSphereCollision( (TransformedShapeConvex*)pShapeA, (TransformedShapeConvex*)pShapeB );
        if ( iTypeB == SHAPE_PRIMITIVE_BOX )
            return New TriangleBoxCollision( (TransformedShapeConvex*)pShapeA, (TransformedShapeConvex*)pShapeB );

        if ( iTypeB == SHAPE_CONVEX_MESH )
            return New ConvexTriangleCollision( (TransformedShapeConvex*)pShapeA, (TransformedShapeConvex*)pShapeB );

        //if ( iTypeB == SHAPE_CONCAVE_MESH || iTypeB == SHAPE_CONCAVE_HEIGHTFIELD )
        //    return New ConcaveTriangleCollision( (TransformedShapeConvex*)pShapeA, (TransformedShapeConcave*)pShapeB );

        Assert( false );
        return NULL;
    }
    if ( iTypeA == SHAPE_PRIMITIVE_SPHERE ) {
        if ( iTypeB == SHAPE_PRIMITIVE_TRIANGLE )
            return New TriangleSphereCollision( (TransformedShapeConvex*)pShapeA, (TransformedShapeConvex*)pShapeB );
        if ( iTypeB == SHAPE_PRIMITIVE_SPHERE )
            return New SphereSphereCollision( (TransformedShapeConvex*)pShapeA, (TransformedShapeConvex*)pShapeB );
        if ( iTypeB == SHAPE_PRIMITIVE_BOX )
            return New SphereBoxCollision( (TransformedShapeConvex*)pShapeA, (TransformedShapeConvex*)pShapeB );

        if ( iTypeB == SHAPE_CONVEX_MESH )
            return New ConvexSphereCollision( (TransformedShapeConvex*)pShapeA, (TransformedShapeConvex*)pShapeB );

        if ( iTypeB == SHAPE_CONCAVE_MESH || iTypeB == SHAPE_CONCAVE_HEIGHTFIELD )
            return New ConcaveSphereCollision( (TransformedShapeConvex*)pShapeA, (TransformedShapeConcave*)pShapeB );

        Assert( false );
        return NULL;
    }

    if ( iTypeA == SHAPE_PRIMITIVE_BOX ) {
        if ( iTypeB == SHAPE_PRIMITIVE_TRIANGLE )
            return New TriangleBoxCollision( (TransformedShapeConvex*)pShapeA, (TransformedShapeConvex*)pShapeB );
        if ( iTypeB == SHAPE_PRIMITIVE_SPHERE )
            return New SphereBoxCollision( (TransformedShapeConvex*)pShapeA, (TransformedShapeConvex*)pShapeB );
        if ( iTypeB == SHAPE_PRIMITIVE_BOX )
            return New BoxBoxCollision( (TransformedShapeConvex*)pShapeA, (TransformedShapeConvex*)pShapeB );

        if ( iTypeB == SHAPE_CONVEX_MESH )
            return New ConvexBoxCollision( (TransformedShapeConvex*)pShapeA, (TransformedShapeConvex*)pShapeB );

        if ( iTypeB == SHAPE_CONCAVE_MESH || iTypeB == SHAPE_CONCAVE_HEIGHTFIELD )
            return New ConcaveBoxCollision( (TransformedShapeConvex*)pShapeA, (TransformedShapeConcave*)pShapeB );

        Assert( false );
        return NULL;
    }

    if ( iTypeA == SHAPE_CONVEX_MESH ) {
        if ( iTypeB == SHAPE_PRIMITIVE_TRIANGLE )
            return New ConvexTriangleCollision( (TransformedShapeConvex*)pShapeA, (TransformedShapeConvex*)pShapeB );
        if ( iTypeB == SHAPE_PRIMITIVE_SPHERE )
            return New ConvexSphereCollision( (TransformedShapeConvex*)pShapeA, (TransformedShapeConvex*)pShapeB );
        if ( iTypeB == SHAPE_PRIMITIVE_BOX )
            return New ConvexBoxCollision( (TransformedShapeConvex*)pShapeA, (TransformedShapeConvex*)pShapeB );

        if ( iTypeB == SHAPE_CONVEX_MESH )
            return New ConvexConvexCollision( (TransformedShapeConvex*)pShapeA, (TransformedShapeConvex*)pShapeB );

        if ( iTypeB == SHAPE_CONCAVE_MESH || iTypeB == SHAPE_CONCAVE_HEIGHTFIELD )
            return New ConcaveConvexCollision( (TransformedShapeConvex*)pShapeA, (TransformedShapeConcave*)pShapeB );

        Assert( false );
        return NULL;
    }

    if ( iTypeA == SHAPE_CONCAVE_MESH || iTypeA == SHAPE_CONCAVE_HEIGHTFIELD ) {
        //if ( iTypeB == SHAPE_PRIMITIVE_TRIANGLE )
        //    return New ConcaveTriangleCollision( (TransformedShapeConcave*)pShapeA, (TransformedShapeConvex*)pShapeB );
        if ( iTypeB == SHAPE_PRIMITIVE_SPHERE )
            return New ConcaveSphereCollision( (TransformedShapeConcave*)pShapeA, (TransformedShapeConvex*)pShapeB );
        if ( iTypeB == SHAPE_PRIMITIVE_BOX )
            return New ConcaveBoxCollision( (TransformedShapeConcave*)pShapeA, (TransformedShapeConvex*)pShapeB );

        if ( iTypeB == SHAPE_CONVEX_MESH )
            return New ConcaveConvexCollision( (TransformedShapeConcave*)pShapeA, (TransformedShapeConvex*)pShapeB );

        if ( iTypeB == SHAPE_CONCAVE_MESH || iTypeB == SHAPE_CONCAVE_HEIGHTFIELD )
            return New ConcaveConcaveCollision( (TransformedShapeConcave*)pShapeA, (TransformedShapeConcave*)pShapeB );

        Assert( false );
        return NULL;
    }

    Assert( false );
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////////
//// ContactRigidRigidEdge implementation
//ContactRigidRigidEdge::ContactRigidRigidEdge( RigidBody * pRigidBodyA, RigidBody * pRigidBodyB ):
//    ContactEdge( pRigidBodyA, pRigidBodyB )
//{
//    // nothing to do
//}
//ContactRigidRigidEdge::~ContactRigidRigidEdge()
//{
//    // nothing to do
//}
//
///////////////////////////////////////////////////////////////////////////////////
//// ContactRigidSoftEdge implementation
//ContactRigidSoftEdge::ContactRigidSoftEdge( RigidBody * pRigidBodyA, SoftBody * pSoftBodyB ):
//    ContactEdge( pRigidBodyA, pSoftBodyB )
//{
//    // nothing to do
//}
//ContactRigidSoftEdge::ContactRigidSoftEdge( SoftBody * pSoftBodyA, RigidBody * pRigidBodyB ):
//    ContactEdge( pSoftBodyA, pRigidBodyB )
//{
//    // nothing to do
//}
//ContactRigidSoftEdge::~ContactRigidSoftEdge()
//{
//    // nothing to do
//}
//
///////////////////////////////////////////////////////////////////////////////////
//// ContactSoftSoftEdge implementation
//ContactSoftSoftEdge::ContactSoftSoftEdge( SoftBody * pSoftBodyA, SoftBody * pSoftBodyB ):
//    ContactEdge( pSoftBodyA, pSoftBodyB )
//{
//    // nothing to do
//}
//ContactSoftSoftEdge::~ContactSoftSoftEdge()
//{
//    // nothing to do
//}

