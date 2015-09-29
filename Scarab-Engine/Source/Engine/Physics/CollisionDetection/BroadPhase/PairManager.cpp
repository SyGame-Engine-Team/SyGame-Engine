/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/CollisionDetection/BroadPhase/PairManager.cpp
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
// Includes
#include "PairManager.h"

#include "../../PhysicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// PairManager implementation
PairManager::PairManager( PairListener * pPairListener )
{
    // Callbacks
    Assert( pPairListener != NULL );
    m_pListener = pPairListener;

    // Pairs set
    m_setPairs.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("Broadphase") );
    m_setPairs.SetComparator( BatchedNodePair::_Compare );
    m_setPairs.Create();

    // Pairs batch
    m_arrBatch.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("Broadphase") );
    m_arrBatch.Create();
}
PairManager::~PairManager()
{
    // Pairs batch
    m_arrBatch.Destroy();

    // Pairs set
    m_setPairs.Destroy();
}

Void PairManager::CreatePair( IslandNode * pNodeA, IslandNode * pNodeB )
{
    // Ensure unicity
    if ( pNodeA > pNodeB )
        Swap<IslandNode*>( &pNodeA, &pNodeB );

    // Find the pair
    BatchedNodePair tmpPair;
    tmpPair.pNodeA = pNodeA;
    tmpPair.pNodeB = pNodeB;
    tmpPair.iFlags = 0;

    PairSet::Iterator itPair;
    Bool bFound = m_setPairs.Search( tmpPair, &itPair );

    // Create the pair if needed
    if ( !bFound ) {
        Bool bInserted = m_setPairs.Insert( tmpPair, &itPair );
        Assert( bInserted );
    }

    Assert( !(itPair.IsNull()) );
    BatchedNodePair * pPair = itPair.GetPtr();

    // Add to batch if not already in
    if ( (pPair->iFlags & NODEPAIR_BATCHED) == 0 ) {
        pPair->iFlags |= NODEPAIR_BATCHED;
        m_arrBatch.Push( pPair );
    }

    // Mark as created
    pPair->iFlags &= ~NODEPAIR_REMOVED;
}
Void PairManager::DestroyPair( IslandNode * pNodeA, IslandNode * pNodeB )
{
    // Ensure unicity
    if ( pNodeA > pNodeB )
        Swap<IslandNode*>( &pNodeA, &pNodeB );

    // Find the pair
    BatchedNodePair tmpPair;
    tmpPair.pNodeA = pNodeA;
    tmpPair.pNodeB = pNodeB;
    tmpPair.iFlags = 0;

    PairSet::Iterator itPair;
    Bool bFound = m_setPairs.Search( tmpPair, &itPair );

    // Pair doesn't exist, nothing to do
	if ( !bFound )
		return;

    Assert( !(itPair.IsNull()) );
    BatchedNodePair * pPair = itPair.GetPtr();

    // Add to batch if not already in
	if ( (pPair->iFlags & NODEPAIR_BATCHED) == 0 ) {
        pPair->iFlags |= NODEPAIR_BATCHED;
        m_arrBatch.Push( pPair );
	}

    // Mark as removed
    pPair->iFlags |= NODEPAIR_REMOVED;
}

Void PairManager::Commit()
{
    // Nothing to commit
    if ( m_arrBatch.IsEmpty() )
        return;

    // Store back in the same array
    UInt iPairCount = m_arrBatch.Count();
    UInt i, iRemoveCount = 0;
    BatchedNodePair * pPair;
    Bool bRemoved;

    // Pass 1 : Creations, collect removals
    for( i = 0; i < iPairCount; ++i ) {
        pPair = m_arrBatch[i];

        // Remove from batch
        Assert( pPair->iFlags & NODEPAIR_BATCHED );
        pPair->iFlags &= ~NODEPAIR_BATCHED;

        // Check pair state
        if ( pPair->iFlags & NODEPAIR_REMOVED ) {
            // Stack for removal
            m_arrBatch[iRemoveCount++] = pPair;
        } else {
            // Pair was added to persistent set
            if ( (pPair->iFlags & NODEPAIR_PERSISTENT) == 0 ) {
                m_pListener->OnPairCreated( pPair->pNodeA, pPair->pNodeB );
                pPair->iFlags |= NODEPAIR_PERSISTENT;
            } else
                m_pListener->OnPairPersisted( pPair->pNodeA, pPair->pNodeB );
        }
    }

    // Pass 2 : Removals
    for( i = 0; i < iRemoveCount; ++i ) {
        pPair = m_arrBatch[i];

        // Pair was removed from persistent set
        if ( pPair->iFlags & NODEPAIR_PERSISTENT )
            m_pListener->OnPairDestroyed( pPair->pNodeA, pPair->pNodeB );

        bRemoved = m_setPairs.Remove( *pPair );
        Assert( bRemoved );
    }

    // Clear batch
    m_arrBatch.Clear();
}

Bool PairManager::IsPair( IslandNode * pNodeA, IslandNode * pNodeB ) const
{
    // Ensure unicity
    if ( pNodeA > pNodeB )
        Swap<IslandNode*>( &pNodeA, &pNodeB );

    // Find the pair
    BatchedNodePair tmpPair;
    tmpPair.pNodeA = pNodeA;
    tmpPair.pNodeB = pNodeB;
    tmpPair.iFlags = 0;

    return m_setPairs.Search( tmpPair );
}

