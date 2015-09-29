/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/IslandSolver/IslandSolver.h
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
// Header prelude
#ifndef SCARAB_ENGINE_PHYSICS_ISLANDSOLVER_ISLANDSOLVER_H
#define SCARAB_ENGINE_PHYSICS_ISLANDSOLVER_ISLANDSOLVER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Datastruct/Array/Array.h"
#include "../../../Lib/Datastruct/Table/TreeMap.h"

#include "../CollisionDetection/BroadPhase/BroadPhase.h"

#include "Nodes/BodyNode.h"

#include "Edges/ContactEdge.h"
#include "Edges/JointEdge.h"

#include "Island.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Sleeping policy
#define ISLANDSOLVER_SLEEP_LINEARTHRESHOLD  0.01f // Check against squared linear speed
#define ISLANDSOLVER_SLEEP_ANGULARTHRESHOLD 0.01f // Check against squared angular speed
#define ISLANDSOLVER_SLEEP_TIMETHRESHOLD    8  // in frames, should be > integration order,
                                               // also prefer values > contact resting threshold
                                               // from the constraint solver ...

// Prototypes
class IslandSolver;

/////////////////////////////////////////////////////////////////////////////////
// The IslandSolverListener class
class IslandSolverListener : public PairListener
{
public:
    IslandSolverListener( IslandSolver * pSolver );
    virtual ~IslandSolverListener();

	inline virtual Void OnPairCreated( IslandNode * pNodeA, IslandNode * pNodeB );
	inline virtual Void OnPairDestroyed( IslandNode * pNodeA, IslandNode * pNodeB );
	inline virtual Void OnPairPersisted( IslandNode * pNodeA, IslandNode * pNodeB );

private:
    IslandSolver * m_pSolver;
};

/////////////////////////////////////////////////////////////////////////////////
// The ContactCallback class
class ContactCallback
{
public:
    ContactCallback();
    virtual ~ContactCallback();

    virtual Void ProximityEnter( BodyNode * pBodyA, BodyNode * pBodyB ) = 0;
    virtual Void ProximityLeave( BodyNode * pBodyA, BodyNode * pBodyB ) = 0;
    virtual Void ProximityPersist( BodyNode * pBodyA, BodyNode * pBodyB ) = 0;

    virtual Void SensorTrigger( BodyNode * pBodyA, BodyNode * pBodyB ) = 0;
    virtual Void SensorClear( BodyNode * pBodyA, BodyNode * pBodyB ) = 0;

    virtual Void ContactSeparated( BodyNode * pBodyA, BodyNode * pBodyB ) = 0;
    virtual Void ContactSpeculate( BodyNode * pBodyA, BodyNode * pBodyB ) = 0;
    virtual Void ContactEnter( BodyNode * pBodyA, BodyNode * pBodyB ) = 0;
    virtual Void ContactLeave( BodyNode * pBodyA, BodyNode * pBodyB ) = 0;
    virtual Void ContactPersist( BodyNode * pBodyA, BodyNode * pBodyB ) = 0;
};

/////////////////////////////////////////////////////////////////////////////////
// The IslandSolver class
class IslandSolver
{
public:
    IslandSolver();
    ~IslandSolver();

    // Islands access
    inline UInt GetIslandCount() const;

    inline Island * GetIslandList() const;

    // Nodes access
    inline UInt GetBodyCount() const;

    inline BodyNode * GetBodyList() const;

    Void AddBody( BodyNode * pBody );
    Void RemoveBody( BodyNode * pBody, Bool bDestroyShape );

    // Edges access
    inline UInt GetContactCount() const;
    inline UInt GetJointCount() const;

    inline ContactEdge * GetContactList() const;
    inline JointEdge * GetJointList() const;

    inline ContactEdge * GetContact( const IslandNode * pNodeA, const IslandNode * pNodeB ) const;
    inline JointEdge * GetJoint( const IslandNode * pNodeA, const IslandNode * pNodeB ) const;

    Void AddJoint( JointEdge * pJointEdge );
    Void RemoveJoint( JointEdge * pJointEdge );

    // Contact callback
    inline ContactCallback * GetContactCallback() const;
    inline Void SetContactCallback( ContactCallback * pContactCallback );

    // Broad phase output
    Void OnEnterProximity( IslandNode * pNodeA, IslandNode * pNodeB );
    Void OnLeaveProximity( IslandNode * pNodeA, IslandNode * pNodeB );
    Void OnPersistentProximity( IslandNode * pNodeA, IslandNode * pNodeB );

    // Broad phase post-processing
    Void DestroyProxies( BroadPhase * pBroadPhase );
    Void ProcessBroadPhase();

    // Narrow + Contact phase
    Void ProcessNarrowPhase( Island * pIsland );

    // Stack solver
    Void ComputeStackLayers( Island * pIsland );

    // Sleep phase
    Void ProcessSleepPhase();

private:
    // List operations
    Void _AddIsland( Island * pIsland );
    Void _RemoveIsland( Island * pIsland );

    Void _AddBody( BodyNode * pBody );
    Void _RemoveBody( BodyNode * pBody );

    Void _AddContact( ContactEdge * pContactEdge );
    Void _AddJoint( JointEdge * pJointEdge );

    Void _RemoveContact( ContactEdge * pContactEdge );
    Void _RemoveJoint( JointEdge * pJointEdge );

    // Connected components algorithm
    Void _UpdateIslandComponent( IslandEdge * pStartEdge, Byte iBlankColor, Byte iComponentColor );
    UInt _ComputeIslandComponents( Island * pIsland );

    Array<IslandEdge*> m_arrEdgeStream;

    // Island Merge & Split routines
    Void _TransferIslandContent( Island * pDestIsland, Island * pSrcIsland );

    Void _MergeIslands( IslandEdge * pMergingEdge );
    Void _SplitIsland( Island * pIsland, UInt iComponentCount );

    // Deferred body removal
    typedef struct _removed_body {
        BodyNode * pBody;
        Bool bDestroyShape;
    } _RemovedBody;

    Void _DeferredRemoveBody( BodyNode * pBody );

    Array<_RemovedBody> m_arrRemovedBodies;

// Debug
    Void _DebugPrint_ContactPoints( const ContactManifold * pManifold ) const;
    Void _DebugPrint_Body( const BodyNode * pBody ) const;
    Void _DebugPrint_Contact( const ContactEdge * pContactEdge ) const;
    Void _DebugPrint_Island( const Island * pIsland ) const;
    Void _DebugPrint_Solver() const;
// Debug

    // Islands
    UInt m_iIslandCount;

    Island * m_pListIslands;

    // Nodes
    UInt m_iBodyCount;

    BodyNode * m_pListBodies;

    // Edges
    UInt m_iContactCount;
    UInt m_iJointCount;

    ContactEdge * m_pListContacts;
    JointEdge * m_pListJoints;

    typedef TreeMap<IslandEdgeID, ContactEdge*> ContactMap;
    typedef TreeMap<IslandEdgeID, JointEdge*> JointMap;
    ContactMap m_mapContacts;
    JointMap m_mapJoints;

    // Sleeping policy
    Scalar m_fSleepLinearThreshold;
    Scalar m_fSleepAngularThreshold;
    UInt m_iSleepTimeThreshold;

    // Contact callback
    ContactCallback * m_pContactCallback;
};


/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "IslandSolver.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_ISLANDSOLVER_ISLANDSOLVER_H



