/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/IslandSolver/IslandSolver.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Spatio-Temporal Coherence analysis using islands.
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
#include "IslandSolver.h"

#include "../PhysicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// IslandSolverListener implementation
IslandSolverListener::IslandSolverListener( IslandSolver * pSolver )
{
    m_pSolver = pSolver;
}
IslandSolverListener::~IslandSolverListener()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// ContactCallback implementation
ContactCallback::ContactCallback()
{
    // nothing to do
}
ContactCallback::~ContactCallback()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// IslandSolver implementation
IslandSolver::IslandSolver():
    m_mapContacts(), m_mapJoints()
{
    // Islands
    m_iIslandCount = 0;

    m_pListIslands = NULL;

    // Nodes
    m_iBodyCount = 0;

    m_pListBodies = NULL;

    // Edges
    m_iContactCount = 0;
    m_iJointCount = 0;

    m_pListContacts = NULL;
    m_pListJoints = NULL;

    m_mapContacts.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("IslandSolver") );
    m_mapContacts.SetComparator( IslandEdgeID::Compare, NULL );
    m_mapContacts.Create();
    m_mapJoints.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("IslandSolver") );
    m_mapJoints.SetComparator( IslandEdgeID::Compare, NULL );
    m_mapJoints.Create();


    // Sleeping policy
    m_fSleepLinearThreshold = ISLANDSOLVER_SLEEP_LINEARTHRESHOLD;
    m_fSleepAngularThreshold = ISLANDSOLVER_SLEEP_ANGULARTHRESHOLD;
    m_iSleepTimeThreshold = ISLANDSOLVER_SLEEP_TIMETHRESHOLD;

    // Contact callback
    m_pContactCallback = NULL;

    // Edge streaming
    m_arrEdgeStream.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("IslandSolver") );
    m_arrEdgeStream.Create();

    // Deferred body removal
    m_arrRemovedBodies.UseMemoryContext( PhysicsFn->GetMemoryContext(), TEXT("IslandSolver") );
    m_arrRemovedBodies.Create();
}
IslandSolver::~IslandSolver()
{
    // Deferred body removal
    m_arrRemovedBodies.Destroy();

    // Edge streaming
    m_arrEdgeStream.Destroy();

    PhysicsFn->SelectMemory( TEXT("IslandSolver") );

    // Destroy islands
    Island * pIsland = m_pListIslands;
    while( pIsland != NULL ) {
        _RemoveIsland( pIsland );
        Delete( pIsland );
        pIsland = m_pListIslands;
    }

    // Destroy edges
    ContactEdge * pContactEdge = m_pListContacts;
    while( pContactEdge != NULL ) {
        _RemoveContact( pContactEdge );
        Delete( pContactEdge );
        pContactEdge = m_pListContacts;
    }
    m_mapContacts.Destroy();

    JointEdge * pJointEdge = m_pListJoints;
    while( pJointEdge != NULL ) {
        _RemoveJoint( pJointEdge );
        Delete( pJointEdge );
        pJointEdge = m_pListJoints;
    }
    m_mapJoints.Destroy();

    PhysicsFn->UnSelectMemory();
    PhysicsFn->SelectMemory( TEXT("Bodies") );

    // Destroy nodes
    BodyNode * pBody = m_pListBodies;
    while( pBody != NULL ) {
        _RemoveBody( pBody );
        Delete( pBody );
        pBody = m_pListBodies;
    }

    PhysicsFn->UnSelectMemory();
}

Void IslandSolver::AddBody( BodyNode * pBody )
{
    // Simply add to world list
    _AddBody( pBody );
}
Void IslandSolver::RemoveBody( BodyNode * pBody, Bool bDestroyShape )
{
    // Proxy has been deleted in this frame's broadphase.
    // In order to be consistent with broadphase events, we need
    // to defer body removals after next commit, as post-processing.
    _RemovedBody remBody;
    remBody.pBody = pBody;
    remBody.bDestroyShape = bDestroyShape;
    m_arrRemovedBodies.Push( remBody );
}

Void IslandSolver::AddJoint( JointEdge * pJointEdge )
{
    // Add to world list
    _AddJoint( pJointEdge );

    // Add to nodes lists
    IslandNode * pNodeA = pJointEdge->GetNodeA();
    IslandNode * pNodeB = pJointEdge->GetNodeB();

    pNodeA->AddJoint( pJointEdge );
    pNodeB->AddJoint( pJointEdge );

    // Add to island list
    _MergeIslands( pJointEdge );
}
Void IslandSolver::RemoveJoint( JointEdge * pJointEdge )
{
    // Remove from world list
    _RemoveJoint( pJointEdge );

    // Remove from nodes lists
    IslandNode * pNodeA = pJointEdge->GetNodeA();
    IslandNode * pNodeB = pJointEdge->GetNodeB();

    pNodeA->RemoveJoint( pJointEdge );
    pNodeB->RemoveJoint( pJointEdge );

    // Remove from island list
    Island * pIsland = pJointEdge->GetIsland();
    pIsland->RemoveJoint( pJointEdge );

    if ( pIsland->GetTotalEdgeCount() == 0 ) {
        // Destroy island
        _RemoveIsland( pIsland );

        PhysicsFn->SelectMemory( TEXT("IslandSolver") );
        Delete( pIsland );
        PhysicsFn->UnSelectMemory();
    } else {
        // Split island
        UInt iComponentCount = _ComputeIslandComponents( pIsland );
        if ( iComponentCount > 1 )
            _SplitIsland( pIsland, iComponentCount );
    }
}

Void IslandSolver::OnEnterProximity( IslandNode * pNodeA, IslandNode * pNodeB )
{
    Assert( pNodeA->GetType() == ISLAND_NODE_BODY );
    Assert( pNodeB->GetType() == ISLAND_NODE_BODY );
    BodyNode * pBodyA = (BodyNode*)pNodeA;
    BodyNode * pBodyB = (BodyNode*)pNodeB;

    // Check for contact existence
    IslandEdgeID idEdge( pBodyA, pBodyB );
    if ( m_mapContacts.Contains(idEdge) ) {
        Assert( false );
        return;
    }

    // Collision filtering
    if ( !(BodyNode::ShouldCollide(pBodyA, pBodyB)) )
        return;

    // Create contact edge
    PhysicsFn->SelectMemory( TEXT("IslandSolver") );
    ContactEdge * pContactEdge = New ContactEdge( pBodyA, pBodyB );
    PhysicsFn->UnSelectMemory();

    // Add to world list
    _AddContact( pContactEdge );

    // Add to nodes lists
    pNodeA->AddContact( pContactEdge );
    pNodeB->AddContact( pContactEdge );

    // Add to island list
    _MergeIslands( pContactEdge );

    // Raise callback
    if ( m_pContactCallback != NULL )
        m_pContactCallback->ProximityEnter( pBodyA, pBodyB );
}
Void IslandSolver::OnLeaveProximity( IslandNode * pNodeA, IslandNode * pNodeB )
{
    Assert( pNodeA->GetType() == ISLAND_NODE_BODY );
    Assert( pNodeB->GetType() == ISLAND_NODE_BODY );
    BodyNode * pBodyA = (BodyNode*)pNodeA;
    BodyNode * pBodyB = (BodyNode*)pNodeB;

    // Retrieve contact
    ContactEdge * pContactEdge = GetContact( pBodyA, pBodyB );
    if ( pContactEdge == NULL )
        return;

    // Raise callback
    if ( m_pContactCallback != NULL )
        m_pContactCallback->ProximityLeave( pBodyA, pBodyB );

    // Remove from world list
    _RemoveContact( pContactEdge );

    // Remove from nodes lists
    pNodeA->RemoveContact( pContactEdge );
    pNodeB->RemoveContact( pContactEdge );

    // Remove from island list
    Island * pIsland = pContactEdge->GetIsland();
    pIsland->RemoveContact( pContactEdge );

    if ( pIsland->GetTotalEdgeCount() == 0 ) {
        // Destroy island
        _RemoveIsland( pIsland );

        PhysicsFn->SelectMemory( TEXT("IslandSolver") );
        Delete( pIsland );
        PhysicsFn->UnSelectMemory();
    } else {
        // Split island
        UInt iComponentCount = _ComputeIslandComponents( pIsland );
        if ( iComponentCount > 1 )
            _SplitIsland( pIsland, iComponentCount );
    }

    // Destroy contact edge
    PhysicsFn->SelectMemory( TEXT("IslandSolver") );
    Delete( pContactEdge );
    PhysicsFn->UnSelectMemory();
}
Void IslandSolver::OnPersistentProximity( IslandNode * pNodeA, IslandNode * pNodeB )
{
    Assert( pNodeA->GetType() == ISLAND_NODE_BODY );
    Assert( pNodeB->GetType() == ISLAND_NODE_BODY );
    BodyNode * pBodyA = (BodyNode*)pNodeA;
    BodyNode * pBodyB = (BodyNode*)pNodeB;

    // Retrieve contact
    ContactEdge * pContactEdge = GetContact( pBodyA, pBodyB );
    if ( pContactEdge == NULL )
        return;

    // Raise callback
    if ( m_pContactCallback != NULL )
        m_pContactCallback->ProximityPersist( pBodyA, pBodyB );
}

Void IslandSolver::DestroyProxies( BroadPhase * pBroadPhase )
{
    // Deferred proxy removals
    UInt iCount = m_arrRemovedBodies.Count();
    for( UInt i = 0; i < iCount; ++i )
        pBroadPhase->DestroyProxy( m_arrRemovedBodies[i].pBody );
}
Void IslandSolver::ProcessBroadPhase()
{
    // Deferred body removals
    Shape * pLocalShape;
    UInt iCount = m_arrRemovedBodies.Count();
    for( UInt i = 0; i < iCount; ++i ) {
        pLocalShape = m_arrRemovedBodies[i].pBody->GetLocalShape();

        // Remove from island solver
        _DeferredRemoveBody( m_arrRemovedBodies[i].pBody );

        // Destroy body
        PhysicsFn->SelectMemory( TEXT("Bodies") );
        Delete( m_arrRemovedBodies[i].pBody );
        PhysicsFn->UnSelectMemory();

        // Destroy shape
        if ( m_arrRemovedBodies[i].bDestroyShape )
            PhysicsFn->DestroyShape( pLocalShape );
    }
    m_arrRemovedBodies.Clear();
}

Void IslandSolver::ProcessNarrowPhase( Island * pIsland )
{
    // Walk through all contacts
    BodyNode *pBodyA, *pBodyB;
    Bool bCollision, bIsSensor;

    NarrowPhase * pNarrowPhase;
    ContactManifold * pContactManifold;
    const ContactManifold * pCollisionManifold;

    ContactEdge * pContactEdge = pIsland->GetContactList();
    while( pContactEdge != NULL ) {
        // Pick bodies
        pBodyA = pContactEdge->GetBodyA();
        pBodyB = pContactEdge->GetBodyB();
        bIsSensor = ( pBodyA->IsSensor() || pBodyB->IsSensor() );

        // Check sleepers
        if ( BodyNode::CheckSleepers(pBodyA, pBodyB) ) {
            pContactEdge = (ContactEdge*)( pContactEdge->GetNextInIsland() );
            continue;
        }

        // Pick narrow phase instance
        pNarrowPhase = pContactEdge->GetNarrowPhase();

        // Pick contact manifold
        pContactManifold = pContactEdge->GetManifold();

        // Perform narrow phase
        bCollision = pNarrowPhase->DetectCollision( !bIsSensor );
        if ( pNarrowPhase->IsConvex() )
            pCollisionManifold = ((const NarrowPhaseConvex *)pNarrowPhase)->GetManifold();
        else
            pCollisionManifold = ((const NarrowPhaseConcave *)pNarrowPhase)->GetManifold();

        if ( bIsSensor ) {
            // Empty contact manifold
            pContactManifold->Clear();

            if ( bCollision ) {
                // Raise callback
                if ( m_pContactCallback != NULL )
                    m_pContactCallback->SensorTrigger( pBodyA, pBodyB );
            } else {
                // Raise callback
                if ( m_pContactCallback != NULL )
                    m_pContactCallback->SensorClear( pBodyA, pBodyB );
            }
        } else {
            if ( pCollisionManifold->IsEmpty() ) {
                if ( pContactManifold->IsEmpty() ) {
                    // Raise callback
                    if ( m_pContactCallback != NULL )
                        m_pContactCallback->ContactSeparated( pBodyA, pBodyB );
                } else {
                    // Raise callback
                    if ( m_pContactCallback != NULL )
                        m_pContactCallback->ContactLeave( pBodyA, pBodyB );

                    // Empty contact manifold
                    pContactManifold->Clear();
                }
            } else {
                if ( bCollision ) {
                    if ( pContactManifold->IsEmpty() ) {
                        // Raise callback
                        if ( m_pContactCallback != NULL )
                            m_pContactCallback->ContactEnter( pBodyA, pBodyB );
                    } else {
                        // Raise callback
                        if ( m_pContactCallback != NULL )
                            m_pContactCallback->ContactPersist( pBodyA, pBodyB );
                    }
                } else {
                    // Raise callback
                    if ( m_pContactCallback != NULL )
                        m_pContactCallback->ContactSpeculate( pBodyA, pBodyB );
                }

                // Update contact manifold
                pContactManifold->UpdateAfterCollision( pCollisionManifold );
            }
        }

        // Next contact
        pContactEdge = (ContactEdge*)( pContactEdge->GetNextInIsland() );
    }
}

Void IslandSolver::ComputeStackLayers( Island * pIsland )
{
    IslandNode * pNodeA;
    IslandNode * pNodeB;
    UInt iStackLayerA;
    UInt iStackLayerB;

    UInt iStackLayersCount = 0;
    UInt iVisitedEdges = 0;

    // Compute base layer
    IslandEdge * pEdge = pIsland->GetContactList();
    while( pEdge != NULL ) {
        pNodeA = pEdge->GetNodeA();
        pNodeB = pEdge->GetNodeB();

        // Check for insulators
        if ( pNodeA->IsInsulator() ) {
            Assert( !(pNodeB->IsInsulator()) );
            pNodeA->_SetStackLayer( 0 );
            pEdge->_SetStackLayer( 0 );
            pNodeB->_SetStackLayer( 1 );
            ++iVisitedEdges;
        } else if ( pNodeB->IsInsulator() ) {
            Assert( !(pNodeA->IsInsulator()) );
            pNodeB->_SetStackLayer( 0 );
            pEdge->_SetStackLayer( 0 );
            pNodeA->_SetStackLayer( 1 );
            ++iVisitedEdges;
        } else {
            pNodeA->_SetStackLayer( INVALID_OFFSET );
            pNodeB->_SetStackLayer( INVALID_OFFSET );
            pEdge->_SetStackLayer( INVALID_OFFSET );
        }

        // Next edge
        pEdge = pEdge->GetNextInIsland();
    }

    // Update layer count
    if ( iVisitedEdges > 0 )
        ++iStackLayersCount;

    // Compute stacked layers
    while( iVisitedEdges > 0 ) {
        iVisitedEdges = 0;

        pEdge = pIsland->GetContactList();
        while( pEdge != NULL ) {
            // Already visited
            if ( pEdge->GetStackLayer() != INVALID_OFFSET ) {
                pEdge = pEdge->GetNextInIsland();
                continue;
            }

            // Visit adjacent nodes
            pNodeA = pEdge->GetNodeA();
            pNodeB = pEdge->GetNodeB();
            iStackLayerA = pNodeA->GetStackLayer();
            iStackLayerB = pNodeB->GetStackLayer();

            if ( iStackLayerA != INVALID_OFFSET && iStackLayerB != INVALID_OFFSET ) {
                if ( iStackLayerA < iStackLayerB ) {
                    pEdge->_SetStackLayer( iStackLayerA );
                    pNodeB->_SetStackLayer( iStackLayerA + 1 );
                } else if ( iStackLayerB < iStackLayerA ) {
                    pEdge->_SetStackLayer( iStackLayerB );
                    pNodeA->_SetStackLayer( iStackLayerB + 1 );
                } else
                    pEdge->_SetStackLayer( iStackLayerA );
                ++iVisitedEdges;
            } else if ( iStackLayerA != INVALID_OFFSET ) {
                pEdge->_SetStackLayer( iStackLayerA );
                pNodeB->_SetStackLayer( iStackLayerA + 1 );
                ++iVisitedEdges;
            } else if ( iStackLayerB != INVALID_OFFSET ) {
                pEdge->_SetStackLayer( iStackLayerB );
                pNodeA->_SetStackLayer( iStackLayerB + 1 );
                ++iVisitedEdges;
            }
            // else cannot visit yet

            // Next edge
            pEdge = pEdge->GetNextInIsland();
        }

        // Update layer count
        if ( iVisitedEdges > 0 )
            ++iStackLayersCount;
    }

    // Save stack layer count
    pIsland->_SetStackLayerCount( iStackLayersCount );
}

Void IslandSolver::ProcessSleepPhase()
{
    // Walk through all islands
    BodyNode * pBody;
    Vector3 vLinearSpeed;
    Vector3 vAngularSpeed;
    Scalar fLinearDot;
    Scalar fAngularDot;
    UInt iMinSleepTime;

    Island * pIsland = m_pListIslands;
    while( pIsland != NULL ) {
        // Compute min sleep time in the island
        iMinSleepTime = INVALID_OFFSET;

        // Walk through bodies
        pBody = pIsland->GetBodyList();
        while( pBody != NULL ) {
            // Skip insulators
            if ( pBody->IsInsulator() ) {
                // Next body
                pBody = (BodyNode*)( pBody->GetNextInIsland() );
                continue;
            }

            // Velocity test
            vLinearSpeed = pBody->GetLinearSpeed();
            vAngularSpeed = pBody->GetAngularSpeed();
            fLinearDot = ( vLinearSpeed * vLinearSpeed );
            fAngularDot = ( vAngularSpeed * vAngularSpeed );
            if ( fLinearDot > m_fSleepLinearThreshold || fAngularDot > m_fSleepAngularThreshold ) {
                pBody->ResetSleepTime();
                iMinSleepTime = 0;
            } else {
                pBody->IncrementSleepTime();
                if ( pBody->GetSleepTime() < iMinSleepTime )
                    iMinSleepTime = pBody->GetSleepTime();
            }

            // Next body
            pBody = (BodyNode*)( pBody->GetNextInIsland() );
        }
        if ( iMinSleepTime == INVALID_OFFSET )
            iMinSleepTime = 0;

        // Time test
        if ( iMinSleepTime >= m_iSleepTimeThreshold ) {
            // Walk through bodies
            pBody = pIsland->GetBodyList();
            while( pBody != NULL ) {
                // Skip insulators
                if ( pBody->IsInsulator() ) {
                    // Next body
                    pBody = (BodyNode*)( pBody->GetNextInIsland() );
                    continue;
                }

                // Put to sleep
                pBody->SetSleeping( true );

                // Next body
                pBody = (BodyNode*)( pBody->GetNextInIsland() );
            }
        }

        // Next island
        pIsland = pIsland->GetNextInWorld();
    }
}

/////////////////////////////////////////////////////////////////////////////////

Void IslandSolver::_AddIsland( Island * pIsland )
{
    pIsland->_SetPrevInWorld( NULL );
    pIsland->_SetNextInWorld( m_pListIslands );

    if ( m_pListIslands != NULL )
        m_pListIslands->_SetPrevInWorld( pIsland );
    m_pListIslands = pIsland;

    ++m_iIslandCount;
}
Void IslandSolver::_RemoveIsland( Island * pIsland )
{
    Island * pPrevIsland = pIsland->GetPrevInWorld();
    Island * pNextIsland = pIsland->GetNextInWorld();

    if ( pPrevIsland != NULL )
        pPrevIsland->_SetNextInWorld( pNextIsland );
    else {
        Assert( m_pListIslands == pIsland );
        m_pListIslands = pNextIsland;
    }
    if ( pNextIsland != NULL )
        pNextIsland->_SetPrevInWorld( pPrevIsland );

    pIsland->_SetPrevInWorld( NULL );
    pIsland->_SetNextInWorld( NULL );

    --m_iIslandCount;
}

Void IslandSolver::_AddBody( BodyNode * pBody )
{
    pBody->_SetPrevInWorld( NULL );
    pBody->_SetNextInWorld( m_pListBodies );

    if ( m_pListBodies != NULL )
        m_pListBodies->_SetPrevInWorld( pBody );
    m_pListBodies = pBody;

    ++m_iBodyCount;
}
Void IslandSolver::_RemoveBody( BodyNode * pBody )
{
    IslandNode * pPrevNode = pBody->GetPrevInWorld();
    IslandNode * pNextNode = pBody->GetNextInWorld();

    if ( pPrevNode != NULL )
        pPrevNode->_SetNextInWorld( pNextNode );
    else {
        Assert( m_pListBodies == pBody );
        m_pListBodies = (BodyNode*)pNextNode;
    }
    if ( pNextNode != NULL )
        pNextNode->_SetPrevInWorld( pPrevNode );

    pBody->_SetPrevInWorld( NULL );
    pBody->_SetNextInWorld( NULL );

    --m_iBodyCount;
}

Void IslandSolver::_AddContact( ContactEdge * pContactEdge )
{
    const IslandEdgeID & idEdge = pContactEdge->GetID();
    if ( m_mapContacts.Contains(idEdge) ) {
        Assert( false );
        return;
    }

    Bool bInserted = m_mapContacts.Insert( idEdge, pContactEdge );
    Assert( bInserted );

    pContactEdge->_SetPrevInWorld( NULL );
    pContactEdge->_SetNextInWorld( m_pListContacts );

    if ( m_pListContacts != NULL )
        m_pListContacts->_SetPrevInWorld( pContactEdge );
    m_pListContacts = pContactEdge;

    ++m_iContactCount;
}
Void IslandSolver::_AddJoint( JointEdge * pJointEdge )
{
    const IslandEdgeID & idEdge = pJointEdge->GetID();
    if ( m_mapJoints.Contains(idEdge) ) {
        Assert( false );
        return;
    }

    Bool bInserted = m_mapJoints.Insert( idEdge, pJointEdge );
    Assert( bInserted );

    pJointEdge->_SetPrevInWorld( NULL );
    pJointEdge->_SetNextInWorld( m_pListJoints );

    if ( m_pListJoints != NULL )
        m_pListJoints->_SetPrevInWorld( pJointEdge );
    m_pListJoints = pJointEdge;

    ++m_iJointCount;
}

Void IslandSolver::_RemoveContact( ContactEdge * pContactEdge )
{
    const IslandEdgeID & idEdge = pContactEdge->GetID();

    Bool bRemoved = m_mapContacts.Remove( idEdge );
    Assert( bRemoved );

    IslandEdge * pPrevEdge = pContactEdge->GetPrevInWorld();
    IslandEdge * pNextEdge = pContactEdge->GetNextInWorld();

    if ( pPrevEdge != NULL )
        pPrevEdge->_SetNextInWorld( pNextEdge );
    else {
        Assert( m_pListContacts == pContactEdge );
        m_pListContacts = (ContactEdge*)pNextEdge;
    }
    if ( pNextEdge != NULL )
        pNextEdge->_SetPrevInWorld( pPrevEdge );

    pContactEdge->_SetPrevInWorld( NULL );
    pContactEdge->_SetNextInWorld( NULL );

    --m_iContactCount;
}
Void IslandSolver::_RemoveJoint( JointEdge * pJointEdge )
{
    const IslandEdgeID & idEdge = pJointEdge->GetID();

    Bool bRemoved = m_mapJoints.Remove( idEdge );
    Assert( bRemoved );

    IslandEdge * pPrevEdge = pJointEdge->GetPrevInWorld();
    IslandEdge * pNextEdge = pJointEdge->GetNextInWorld();

    if ( pPrevEdge != NULL )
        pPrevEdge->_SetNextInWorld( pNextEdge );
    else {
        Assert( m_pListJoints == pJointEdge );
        m_pListJoints = (JointEdge*)pNextEdge;
    }
    if ( pNextEdge != NULL )
        pNextEdge->_SetPrevInWorld( pPrevEdge );

    pJointEdge->_SetPrevInWorld( NULL );
    pJointEdge->_SetNextInWorld( NULL );

    --m_iJointCount;
}

Void IslandSolver::_UpdateIslandComponent( IslandEdge * pStartEdge, Byte iBlankColor, Byte iComponentColor )
{
    IslandEdge * pEdge;
    IslandEdge * pAdjacentEdge;
    IslandNode * arrNodes[2];
    IslandNode * pNode;

    // Init edge stream
    Assert( pStartEdge->GetColor() == iBlankColor );
    m_arrEdgeStream.Clear();
    m_arrEdgeStream.Push( pStartEdge );

    // Breadth-first traversal
    while( m_arrEdgeStream.Count() > 0 ) {
        // Pick next edge
        m_arrEdgeStream.Shift( pEdge );

        // Update color
        pEdge->SetColor( iComponentColor );

        // Check adjacent nodes
        arrNodes[0] = pEdge->GetNodeA();
        arrNodes[1] = pEdge->GetNodeB();
        for( UInt i = 0; i < 2; ++i ) {
            pNode = arrNodes[i];

            // Reject insulators
            if ( pNode->IsInsulator() )
                continue;

            // Walk through adjacent contacts
            pAdjacentEdge = pNode->GetContactList();
            while( pAdjacentEdge != NULL ) {
                if ( pAdjacentEdge->GetColor() == iBlankColor )
                    m_arrEdgeStream.Push( pAdjacentEdge );
                pAdjacentEdge = pAdjacentEdge->GetNextInNode( pNode );
            }
            // Walk through adjacent joints
            pAdjacentEdge = pNode->GetJointList();
            while( pAdjacentEdge != NULL ) {
                if ( pAdjacentEdge->GetColor() == iBlankColor )
                    m_arrEdgeStream.Push( pAdjacentEdge );
                pAdjacentEdge = pAdjacentEdge->GetNextInNode( pNode );
            }
        }
    }
}
UInt IslandSolver::_ComputeIslandComponents( Island * pIsland )
{
    IslandEdge * pEdge;
    UInt iComponentCount = 0;

    // Pick blank color
    Byte iBlankColor = pIsland->GetColor();
    Byte iComponentColor = ( iBlankColor + 1 ); // Intended overflow

// Debug
    pEdge = pIsland->GetContactList();
    while( pEdge != NULL ) {
        Assert( pEdge->GetColor() == iBlankColor );
        pEdge = pEdge->GetNextInIsland();
    }
// Debug

    // Walk through joints
    pEdge = pIsland->GetJointList();
    while( pEdge != NULL ) {
        // New joint encountered
        if ( pEdge->GetColor() == iBlankColor ) {
            // Update component
            _UpdateIslandComponent( pEdge, iBlankColor, iComponentColor );
            ++iComponentColor; // Intended overflow
            ++iComponentCount;
        }

        // Next joint
        pEdge = pEdge->GetNextInIsland();
    }

    // Walk through contacts last
    pEdge = pIsland->GetContactList();
    while( pEdge != NULL ) {
        // New contact encountered
        if ( pEdge->GetColor() == iBlankColor ) {
            // Update component
            _UpdateIslandComponent( pEdge, iBlankColor, iComponentColor );
            ++iComponentColor; // Intended overflow
            ++iComponentCount;
        }

        // Next contact
        pEdge = pEdge->GetNextInIsland();
    }

    // Update island color when it will persist (no split)
    if ( iComponentCount == 1 )
        pIsland->SetColor( iBlankColor + 1 ); // Intended overflow

    // Done
    return iComponentCount;
}

Void IslandSolver::_TransferIslandContent( Island * pDestIsland, Island * pSrcIsland )
{
    IslandEdge * pNextEdge;

    // Transfer contacts
    ContactEdge * pContactEdge = pSrcIsland->GetContactList();
    while( pContactEdge != NULL ) {
        pNextEdge = pContactEdge->GetNextInIsland();

        pContactEdge->SetColor( pDestIsland->GetColor() );
        pSrcIsland->RemoveContact( pContactEdge );
        pDestIsland->AddContact( pContactEdge );

        pContactEdge = (ContactEdge*)( pNextEdge );
    }

    // Transfer joints
    JointEdge * pJointEdge = pSrcIsland->GetJointList();
    while( pJointEdge != NULL ) {
        pNextEdge = pJointEdge->GetNextInIsland();

        pJointEdge->SetColor( pDestIsland->GetColor() );
        pSrcIsland->RemoveJoint( pJointEdge );
        pDestIsland->AddJoint( pJointEdge );

        pJointEdge = (JointEdge*)( pNextEdge );
    }
}

Void IslandSolver::_MergeIslands( IslandEdge * pMergingEdge )
{
    // Pick nodes
    IslandNode * pNodeA = pMergingEdge->GetNodeA();
    IslandNode * pNodeB = pMergingEdge->GetNodeB();

    // Pick neightbouring islands
    Island * pIslandA = pNodeA->GetIsland();
    Island * pIslandB = pNodeB->GetIsland();

    // Compute merged island
    Island * pMergedIsland = NULL;
    if ( pIslandA == NULL && pIslandB == NULL ) {
        // Both insulator/singleton nodes case
        PhysicsFn->SelectMemory( TEXT("IslandSolver") );
        pMergedIsland = New Island();
        PhysicsFn->UnSelectMemory();

        _AddIsland( pMergedIsland );
    } else if ( pIslandA == NULL ) {
        // A is insulator/singleton node case
        pMergedIsland = pIslandB;
    } else if ( pIslandB == NULL ) {
        // B is insulator/singleton node case
        pMergedIsland = pIslandA;
    } else if ( pIslandA == pIslandB ) {
        // Same island case
        pMergedIsland = pIslandA;
    } else {
        // Distinct islands case
        pMergedIsland = pIslandA;
        _TransferIslandContent( pMergedIsland, pIslandB );
        _RemoveIsland( pIslandB );

        PhysicsFn->SelectMemory( TEXT("IslandSolver") );
        Delete( pIslandB );
        PhysicsFn->UnSelectMemory();
    }

    // Add merging edge
    pMergingEdge->SetColor( pMergedIsland->GetColor() );
    switch( pMergingEdge->GetType() ) {
        case ISLAND_EDGE_CONTACT: pMergedIsland->AddContact( (ContactEdge*)pMergingEdge ); break;
        case ISLAND_EDGE_JOINT:   pMergedIsland->AddJoint( (JointEdge*)pMergingEdge );     break;
        default: Assert(false); break;
    }
}
Void IslandSolver::_SplitIsland( Island * pIsland, UInt iComponentCount )
{
    Assert( iComponentCount > 1 );

    // Create temp array
    PhysicsFn->SelectMemory( TEXT("Scratch") );
    Island ** arrSplittedIslands = New Island*[iComponentCount];
    PhysicsFn->UnSelectMemory();

    // Pick component colors
    Byte iBlankColor = pIsland->GetColor();
    Byte iFirstComponentColor = ( iBlankColor + 1 ); // Intended overflow
    Byte iComponentColor = iFirstComponentColor;
    Int iIndex;

    // Create splitted islands
    for( UInt i = 0; i < iComponentCount; ++i ) {
        // Create splitted island
        PhysicsFn->SelectMemory( TEXT("IslandSolver") );
        arrSplittedIslands[i] = New Island();
        PhysicsFn->UnSelectMemory();

        arrSplittedIslands[i]->SetColor( iComponentColor );
        ++iComponentColor; // Intended overflow

        // Add island to world list
        _AddIsland( arrSplittedIslands[i] );
    }

    // Walk through contacts
    IslandEdge * pNextEdge;
    IslandEdge * pEdge = pIsland->GetContactList();
    while( pEdge != NULL ) {
        pNextEdge = pEdge->GetNextInIsland();

        iComponentColor = pEdge->GetColor();
        iIndex = ( (Int)iComponentColor - (Int)iFirstComponentColor );
        if ( iIndex < 0 ) iIndex += 0xff;

        pIsland->RemoveContact( (ContactEdge*)pEdge );
        arrSplittedIslands[iIndex]->AddContact( (ContactEdge*)pEdge );

        pEdge = pNextEdge;
    }

    // Walk through joints
    pEdge = pIsland->GetJointList();
    while( pEdge != NULL ) {
        pNextEdge = pEdge->GetNextInIsland();

        iComponentColor = pEdge->GetColor();
        iIndex = ( (Int)iComponentColor - (Int)iFirstComponentColor );
        if ( iIndex < 0 ) iIndex += 0xff;

        pIsland->RemoveJoint( (JointEdge*)pEdge );
        arrSplittedIslands[iIndex]->AddJoint( (JointEdge*)pEdge );

        pEdge = pNextEdge;
    }

    // Destroy old island
    _RemoveIsland( pIsland );

    PhysicsFn->SelectMemory( TEXT("IslandSolver") );
    Delete( pIsland );
    PhysicsFn->UnSelectMemory();

    // Destroy temp array
    PhysicsFn->SelectMemory( TEXT("Scratch") );
    DeleteA( arrSplittedIslands );
    PhysicsFn->UnSelectMemory();
}

Void IslandSolver::_DeferredRemoveBody( BodyNode * pBody )
{
    Assert( pBody->GetContactCount() == 0 );

    // Pass 1 : Remove node, singleton case
    _RemoveBody( pBody );
    if ( pBody->GetTotalEdgeCount() == 0 )
        return;

    // Pass 2 : Adjacent edges, remove linkage
    Island * pSurroundingIsland = pBody->GetIsland();
    Island * pAdjacentIsland;
    IslandNode * pAdjacentNode;

    IslandEdge * pEdge = pBody->GetJointList();
    while( pEdge != NULL ) {
        // Remove from adjacent node
        pAdjacentNode = pEdge->GetNodeA();
        if ( pAdjacentNode == (IslandNode*)pBody )
            pAdjacentNode = pEdge->GetNodeB();

        pAdjacentNode->RemoveJoint( (JointEdge*)pEdge );

        // Remove from adjacent island
        if ( pSurroundingIsland != NULL ) {
            Assert( pSurroundingIsland == pEdge->GetIsland() );
            pSurroundingIsland->RemoveJoint( (JointEdge*)pEdge );
        } else {
            // Deleting an edge adjacent to an insulator
            // never produces a splitting ...
            pAdjacentIsland = pEdge->GetIsland();
            pAdjacentIsland->RemoveJoint( (JointEdge*)pEdge );

            if ( pAdjacentIsland->GetTotalEdgeCount() == 0 ) {
                // Destroy island
                _RemoveIsland( pAdjacentIsland );

                PhysicsFn->SelectMemory( TEXT("IslandSolver") );
                Delete( pAdjacentIsland );
                PhysicsFn->UnSelectMemory();
            }
        }

        // Next edge
        pEdge = pEdge->GetNextInNode( pBody );
    }

    // Pass 3 : Remove adjacent edges
    pEdge = pBody->GetJointList();
    while( pEdge != NULL ) {
        pBody->RemoveJoint( (JointEdge*)pEdge );
        _RemoveJoint( (JointEdge*)pEdge );

        PhysicsFn->SelectMemory( TEXT("IslandSolver") );
        Delete( pEdge );
        PhysicsFn->UnSelectMemory();

        // Next edge
        pEdge = pBody->GetJointList();
    }

    // Pass 4 : Update surrounding island
    if ( pSurroundingIsland != NULL ) {
        if ( pSurroundingIsland->GetTotalEdgeCount() == 0 ) {
            // Destroy island
            _RemoveIsland( pSurroundingIsland );

            PhysicsFn->SelectMemory( TEXT("IslandSolver") );
            Delete( pSurroundingIsland );
            PhysicsFn->UnSelectMemory();
        } else {
            // Split island
            UInt iComponentCount = _ComputeIslandComponents( pSurroundingIsland );
            if ( iComponentCount > 1 )
                _SplitIsland( pSurroundingIsland, iComponentCount );
        }
    }
}

// Debug
Void IslandSolver::_DebugPrint_ContactPoints( const ContactManifold * pManifold ) const
{
    // Contact manifold
    UInt iContactPointCount = pManifold->GetContactPointCount();

    DebugPrint( TEXT("Updated contact manifold : (Count = %d)\n"), iContactPointCount );
    for( UInt i = 0; i < iContactPointCount; ++i ) {
        const ContactPoint * pCP = pManifold->GetContactPoint(i);

        const MeshFeature * pFeatA = (const MeshFeature *)( pCP->idCPID.dwFeatureA );
        const MeshFeature * pFeatB = (const MeshFeature *)( pCP->idCPID.dwFeatureB );

        GChar strTypeA[2] = TEXT("X");
        UInt iKeyA = 0;
        switch( pFeatA->GetType() ) {
            case MESH_FEATURE_VERTEX: strTypeA[0] = TEXT('V');
                                      iKeyA  = ( ((const MeshVertex *)pFeatA)->Key.V ); break;
            case MESH_FEATURE_EDGE:   strTypeA[0] = TEXT('E');
                                      iKeyA  = ( ((const MeshEdge *)pFeatA)->Key.A ) << 16;
                                      iKeyA |= ( ((const MeshEdge *)pFeatA)->Key.B ); break;
            case MESH_FEATURE_FACET:
                strTypeA[0] = TEXT('F');
                switch( ((const MeshFacet *)pFeatA)->GetFacetType() ) {
                    case MESH_FACET_TRIANGLE: iKeyA  = ( ((const MeshFacetTriangle *)pFeatA)->Key.A ) << 16;
                                              iKeyA |= ( ((const MeshFacetTriangle *)pFeatA)->Key.B ) << 8;
                                              iKeyA |= ( ((const MeshFacetTriangle *)pFeatA)->Key.C ); break;
                    case MESH_FACET_QUAD:     iKeyA  = ( ((const MeshFacetQuad *)pFeatA)->Key.A ) << 24;
                                              iKeyA |= ( ((const MeshFacetQuad *)pFeatA)->Key.B ) << 16;
                                              iKeyA |= ( ((const MeshFacetQuad *)pFeatA)->Key.C ) << 8;
                                              iKeyA |= ( ((const MeshFacetQuad *)pFeatA)->Key.D ); break;
                    case MESH_FACET_POLYGON:  iKeyA = 0;
                                              for( Int i = ((const MeshFacetPolygon *)pFeatA)->Key.SIZE - 1; i >= 0; --i )
                                                  iKeyA |= ( ((const MeshFacetPolygon *)pFeatA)->Key.V[i] ) << (4*i);
                                              break;
                    default: Assert( false ); break;
                } break;
            default: Assert( false ); break;
        }
        GChar strTypeB[2] = TEXT("X");
        UInt iKeyB = 0;
        switch( pFeatB->GetType() ) {
            case MESH_FEATURE_VERTEX: strTypeB[0] = TEXT('V');
                                      iKeyB  = ( ((const MeshVertex *)pFeatB)->Key.V ); break;
            case MESH_FEATURE_EDGE:   strTypeB[0] = TEXT('E');
                                      iKeyB  = ( ((const MeshEdge *)pFeatB)->Key.A ) << 16;
                                      iKeyB |= ( ((const MeshEdge *)pFeatB)->Key.B ); break;
            case MESH_FEATURE_FACET:
                strTypeB[0] = TEXT('F');
                switch( ((const MeshFacet *)pFeatB)->GetFacetType() ) {
                    case MESH_FACET_TRIANGLE: iKeyB  = ( ((const MeshFacetTriangle *)pFeatB)->Key.A ) << 16;
                                              iKeyB |= ( ((const MeshFacetTriangle *)pFeatB)->Key.B ) << 8;
                                              iKeyB |= ( ((const MeshFacetTriangle *)pFeatB)->Key.C ); break;
                    case MESH_FACET_QUAD:     iKeyB  = ( ((const MeshFacetQuad *)pFeatB)->Key.A ) << 24;
                                              iKeyB |= ( ((const MeshFacetQuad *)pFeatB)->Key.B ) << 16;
                                              iKeyB |= ( ((const MeshFacetQuad *)pFeatB)->Key.C ) << 8;
                                              iKeyB |= ( ((const MeshFacetQuad *)pFeatB)->Key.D ); break;
                    case MESH_FACET_POLYGON:  iKeyB = 0;
                                              for( Int i = ((const MeshFacetPolygon *)pFeatB)->Key.SIZE - 1; i >= 0; --i )
                                                  iKeyB |= ( ((const MeshFacetPolygon *)pFeatB)->Key.V[i] ) << (4*i);
                                              break;
                    default: Assert( false ); break;
                } break;
            default: Assert( false ); break;
        }

        DebugPrint( TEXT("\t -> (ID=%x/%x, LifeTime=%d) : Features(%s(%8x),%s(%8x)), Normal(%1f,%1f,%1f),\n"),
                    pCP->idCPID.dwFeatureA, pCP->idCPID.dwFeatureB, pCP->iLifeTime, strTypeA, iKeyA, strTypeB, iKeyB,
                    pCP->vWorldNormalBA.X, pCP->vWorldNormalBA.Y, pCP->vWorldNormalBA.Z );
        DebugPrint( TEXT("\t                                             Distance(%1f), Impulse(%1f,%1f,%1f)\n"),
                    pCP->fDistance, pCP->fNormalImpulse, pCP->fFrictionImpulse0, pCP->fFrictionImpulse1 );
    }
    DebugPrint( TEXT("\n") );
}
Void IslandSolver::_DebugPrint_Body( const BodyNode * pBody ) const
{
    DebugPrint( TEXT("BodyNode state (%8x) =\n"), (DWord)pBody );
    DebugPrint( TEXT("\t -> Parent Island              : %8x\n"), (DWord)( pBody->GetIsland() ) );
    DebugPrint( TEXT("\t -> Reference-Count in Island  : %d\n"), (DWord)( pBody->_Debug_GetIslandRefCount() ) );
    DebugPrint( TEXT("\t -> Prev / Next Node in World  : %8x / %8x\n"), (DWord)( pBody->GetPrevInWorld() ),
                                                                        (DWord)( pBody->GetNextInWorld() ) );
    DebugPrint( TEXT("\t -> Prev / Next Node in Island : %8x / %8x\n"), (DWord)( pBody->GetPrevInIsland() ),
                                                                        (DWord)( pBody->GetNextInIsland() ) );
    DebugPrint( TEXT("\t -> Stack-Layer                : %d\n"), pBody->GetStackLayer() );

    DebugPrint( TEXT("\t -> Contact List (%d) = "), pBody->GetContactCount() );
    const ContactEdge * pContactEdge = pBody->GetContactList();
    while( pContactEdge != NULL ) {
        DebugPrint( TEXT("%8x,"), (DWord)pContactEdge );
        pContactEdge = (const ContactEdge *)( pContactEdge->GetNextInNode(pBody) );
    }
    DebugPrint( TEXT("\n\n") );
}
Void IslandSolver::_DebugPrint_Contact( const ContactEdge * pContactEdge ) const
{
    DebugPrint( TEXT("Contact-Edge state (%8x) =\n"), (DWord)pContactEdge );
    DebugPrint( TEXT("\t -> Parent Island                 : %8x\n"), (DWord)( pContactEdge->GetIsland() ) );
    DebugPrint( TEXT("\t -> Node A                        : %8x\n"), (DWord)( pContactEdge->GetNodeA() ) );
    DebugPrint( TEXT("\t -> Node B                        : %8x\n"), (DWord)( pContactEdge->GetNodeB() ) );
    DebugPrint( TEXT("\t -> Prev / Next Contact in World  : %8x / %8x\n"), (DWord)( pContactEdge->GetPrevInWorld() ),
                                                                           (DWord)( pContactEdge->GetNextInWorld() ) );
    DebugPrint( TEXT("\t -> Prev / Next Contact in Island : %8x / %8x\n"), (DWord)( pContactEdge->GetPrevInIsland() ),
                                                                           (DWord)( pContactEdge->GetNextInIsland() ) );
    DebugPrint( TEXT("\t -> Prev / Next Contact in NodeA  : %8x / %8x\n"), (DWord)( pContactEdge->GetPrevInNode(pContactEdge->GetNodeA()) ),
                                                                           (DWord)( pContactEdge->GetNextInNode(pContactEdge->GetNodeA()) ) );
    DebugPrint( TEXT("\t -> Prev / Next Contact in NodeB  : %8x / %8x\n"), (DWord)( pContactEdge->GetPrevInNode(pContactEdge->GetNodeB()) ),
                                                                           (DWord)( pContactEdge->GetNextInNode(pContactEdge->GetNodeB()) ) );
    DebugPrint( TEXT("\t -> Stack-Layer                   : %d\n"), pContactEdge->GetStackLayer() );
    DebugPrint( TEXT("\t -> Color                         : %d\n"), (DWord)( pContactEdge->GetColor() ) );
    DebugPrint( TEXT("\n") );
}
Void IslandSolver::_DebugPrint_Island( const Island * pIsland ) const
{
    if ( pIsland == NULL ) {
        DebugPrint( TEXT("Island state (NULL) = <Singleton body>\n\n") );
        return;
    }

    DebugPrint( TEXT("Island state (%8x) =\n"), (DWord)pIsland );
    DebugPrint( TEXT("\t -> Prev / Next Island : %8x / %8x\n"), (DWord)( pIsland->GetPrevInWorld() ),
                                                                (DWord)( pIsland->GetNextInWorld() ) );
    DebugPrint( TEXT("\t -> Stack-Layer Count  : %d\n"), pIsland->GetStackLayerCount() );
    DebugPrint( TEXT("\t -> Color              : %d\n"), (DWord)( pIsland->GetColor() ) );

    DebugPrint( TEXT("\t -> BodyNode List (%d) = "), pIsland->GetBodyCount() );
    const BodyNode * pBody = pIsland->GetBodyList();
    while( pBody != NULL ) {
        DebugPrint( TEXT("%8x,"), (DWord)pBody );
        pBody = (const BodyNode *)( pBody->GetNextInIsland() );
    }
    DebugPrint( TEXT("\n") );

    DebugPrint( TEXT("\t -> Contact List (%d) = "), pIsland->GetContactCount() );
    const ContactEdge * pContactEdge = pIsland->GetContactList();
    while( pContactEdge != NULL ) {
        DebugPrint( TEXT("%8x,"), (DWord)pContactEdge );
        pContactEdge = (const ContactEdge *)( pContactEdge->GetNextInIsland() );
    }
    DebugPrint( TEXT("\n\n") );
}
Void IslandSolver::_DebugPrint_Solver() const
{
    DebugPrint( TEXT("Island solver state =\n") );

    DebugPrint( TEXT("\t -> BodyNode List (%d)    = "), m_iBodyCount );
    const BodyNode * pBody = m_pListBodies;
    while( pBody != NULL ) {
        DebugPrint( TEXT("%8x,"), (DWord)pBody );
        pBody = (const BodyNode *)( pBody->GetNextInWorld() );
    }
    DebugPrint( TEXT("\n") );

    DebugPrint( TEXT("\t -> Contact List (%d) = "), m_iContactCount );
    const ContactEdge * pContactEdge = m_pListContacts;
    while( pContactEdge != NULL ) {
        DebugPrint( TEXT("%8x,"), (DWord)pContactEdge );
        pContactEdge = (const ContactEdge *)( pContactEdge->GetNextInWorld() );
    }
    DebugPrint( TEXT("\n") );

    DebugPrint( TEXT("\t -> Island List (%d)  = "), m_iIslandCount );
    const Island * pIsland = m_pListIslands;
    while( pIsland != NULL ) {
        DebugPrint( TEXT("%8x,"), (DWord)pIsland );
        pIsland = (const Island *)( pIsland->GetNextInWorld() );
    }
    DebugPrint( TEXT("\n\n") );
}
// Debug

