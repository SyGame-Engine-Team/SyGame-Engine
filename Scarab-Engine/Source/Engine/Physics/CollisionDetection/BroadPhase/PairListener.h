/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/BroadPhase/PairListener.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Pair events entry point for NarrowPhase.
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
#ifndef SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_BROADPHASE_PAIRLISTENER_H
#define SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_BROADPHASE_PAIRLISTENER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../IslandSolver/IslandNode.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// NodePair
typedef struct _nodepair
{
    inline static Int _Compare( const struct _nodepair & leftPair, const struct _nodepair & rightPair, Void * pUserData );

    IslandNode * pNodeA; // Unicity :
    IslandNode * pNodeB; // pNodeA < pNodeB
} NodePair;

/////////////////////////////////////////////////////////////////////////////////
// The PairListener class
class PairListener
{
public:
    PairListener();
    virtual ~PairListener();

	virtual Void OnPairCreated( IslandNode * pNodeA, IslandNode * pNodeB ) = 0;
	virtual Void OnPairDestroyed( IslandNode * pNodeA, IslandNode * pNodeB ) = 0;
	virtual Void OnPairPersisted( IslandNode * pNodeA, IslandNode * pNodeB ) = 0;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "PairListener.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_BROADPHASE_PAIRLISTENER_H

