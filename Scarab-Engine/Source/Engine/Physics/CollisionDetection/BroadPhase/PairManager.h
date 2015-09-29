/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/BroadPhase/PairManager.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Generic pair-manager for incremental event-based notification.
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
#ifndef SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_BROADPHASE_PAIRMANAGER_H
#define SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_BROADPHASE_PAIRMANAGER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../../Lib/Datastruct/Array/Array.h"
#include "../../../../Lib/Datastruct/Tree/RedBlackTree.h"

#include "../../IslandSolver/IslandNode.h"

#include "PairListener.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// NodePair Batch Flag
typedef DWord NodePairBatchFlag;
#define NODEPAIR_BATCHED    0x00000001 // Pair is being batched
#define NODEPAIR_REMOVED    0x00000002 // Pair was last removed during batch
#define NODEPAIR_PERSISTENT 0x80000000 // Pair existed before batch

// BatchedNodePair
typedef struct _batched_nodepair
{
    inline static Int _Compare( const struct _batched_nodepair & leftPair, const struct _batched_nodepair & rightPair, Void * pUserData );

    IslandNode * pNodeA; // Unicity :
    IslandNode * pNodeB; // pNodeA < pNodeB
    NodePairBatchFlag iFlags;
} BatchedNodePair;

/////////////////////////////////////////////////////////////////////////////////
// The PairManager class
class PairManager
{
public:
    PairManager( PairListener * pPairListener );
    ~PairManager();

    // Pair batching
	Void CreatePair( IslandNode * pNodeA, IslandNode * pNodeB );
	Void DestroyPair( IslandNode * pNodeA, IslandNode * pNodeB );

	Void Commit();

    // Synchronous listener
    inline Void SwitchListener( PairListener * pPairListener );

    // Asynchronous queries
    Bool IsPair( IslandNode * pNodeA, IslandNode * pNodeB ) const;

private:
    // Listener
	PairListener * m_pListener;

    // Pairs set
    typedef RedBlackTree<BatchedNodePair> PairSet;
    PairSet m_setPairs;

	// Pairs batch
    Array<BatchedNodePair*> m_arrBatch;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "PairManager.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_COLLISIONDETECTION_BROADPHASE_PAIRMANAGER_H

