/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/NarrowPhase/Concave/ConcaveBoxCollision.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Narrow-Phase : Concave, Concave-Box
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
#ifndef SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_CONCAVE_CONCAVEBOXCOLLISION_H
#define SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_CONCAVE_CONCAVEBOXCOLLISION_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Shape/Primitive/ShapeTriangle.h"
#include "../../../Shape/TransformedShape.h"

#include "../NarrowPhase.h"

#include "../Primitive/TriangleBoxCollision.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The ConcaveBoxCollision class
class ConcaveBoxCollision : public NarrowPhaseConcave
{
public:
    ConcaveBoxCollision( TransformedShapeConcave * pShapeA, TransformedShapeConvex * pShapeB );
    ConcaveBoxCollision( TransformedShapeConvex * pShapeA, TransformedShapeConcave * pShapeB );
    virtual ~ConcaveBoxCollision();

    // Narrow Collision algorithm
    virtual Void Initialize();
    virtual Bool DetectCollision( Bool bGenerateContactPoints );

private:
    TransformedShapeConcave * m_pConcave;
    TransformedShapeConvex * m_pBox;

    Bool m_bReverseNormal;

    // Query data
    class CollisionQuery : public TransformedShapeConcave::HTransformedTriangleQuery { public:
        CollisionQuery();
        virtual ~CollisionQuery();

        virtual Void ProcessTransformed( const ShapeConcave::TriangleLeaf & triLeaf );

        ConcaveBoxCollision * pThis;
        Bool bCollisionFound;
        Bool bGenerateContactPoints;
    };
    CollisionQuery m_hCollisionQuery;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ConcaveBoxCollision.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_CONCAVE_CONCAVEBOXCOLLISION_H

