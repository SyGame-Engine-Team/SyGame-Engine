/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/NarrowPhase/Concave/ConcaveConcaveCollision.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Narrow-Phase : Concave, Concave-Concave
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
#ifndef SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_CONCAVE_CONCAVECONCAVECOLLISION_H
#define SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_CONCAVE_CONCAVECONCAVECOLLISION_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Shape/Primitive/ShapeTriangle.h"
#include "../../../Shape/TransformedShape.h"

#include "../NarrowPhase.h"

#include "../Primitive/TriangleTriangleCollision.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The ConcaveConcaveCollision class
class ConcaveConcaveCollision : public NarrowPhaseConcave
{
public:
    ConcaveConcaveCollision( TransformedShapeConcave * pShapeA, TransformedShapeConcave * pShapeB );
    virtual ~ConcaveConcaveCollision();

    // Narrow Collision algorithm
    virtual Void Initialize();
    virtual Bool DetectCollision( Bool bGenerateContactPoints );

private:
    TransformedShapeConcave * m_pConcaveA;
    TransformedShapeConcave * m_pConcaveB;

    // Query data
    class CollisionQuery : public TransformedShapeConcave::HTransformedTriangleQuery { public:
        CollisionQuery();
        virtual ~CollisionQuery();

        virtual Void ProcessTransformed( const ShapeConcave::TriangleLeaf & triLeaf );

        ConcaveConcaveCollision * pThis;
        Bool bCollisionFound;
        Bool bGenerateContactPoints;

        Bool bQueryB;
        const ShapeConcave::TriangleLeaf * pTriLeafA;
    };
    CollisionQuery m_hCollisionQuery;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ConcaveConcaveCollision.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_NARROWPHASE_CONCAVE_CONCAVECONCAVECOLLISION_H

