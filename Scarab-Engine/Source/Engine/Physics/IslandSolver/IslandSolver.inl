/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/IslandSolver/IslandSolver.inl
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
// IslandSolverListener implementation
inline Void IslandSolverListener::OnPairCreated( IslandNode * pNodeA, IslandNode * pNodeB ) {
    m_pSolver->OnEnterProximity( pNodeA, pNodeB );
}
inline Void IslandSolverListener::OnPairDestroyed( IslandNode * pNodeA, IslandNode * pNodeB ) {
    m_pSolver->OnLeaveProximity( pNodeA, pNodeB );
}
inline Void IslandSolverListener::OnPairPersisted( IslandNode * pNodeA, IslandNode * pNodeB ) {
    m_pSolver->OnPersistentProximity( pNodeA, pNodeB );
}

/////////////////////////////////////////////////////////////////////////////////
// IslandSolver implementation
inline UInt IslandSolver::GetIslandCount() const {
    return m_iIslandCount;
}

inline Island * IslandSolver::GetIslandList() const {
    return m_pListIslands;
}

inline UInt IslandSolver::GetBodyCount() const {
    return m_iBodyCount;
}

inline BodyNode * IslandSolver::GetBodyList() const {
    return m_pListBodies;
}

inline UInt IslandSolver::GetContactCount() const {
    return m_iContactCount;
}
inline UInt IslandSolver::GetJointCount() const {
    return m_iJointCount;
}

inline ContactEdge * IslandSolver::GetContactList() const {
    return m_pListContacts;
}
inline JointEdge * IslandSolver::GetJointList() const {
    return m_pListJoints;
}

inline ContactEdge * IslandSolver::GetContact( const IslandNode * pNodeA, const IslandNode * pNodeB ) const {
    IslandEdgeID idEdge( pNodeA, pNodeB );
    ContactMap::Iterator itEdge = m_mapContacts.Get( idEdge );
    if ( itEdge.IsNull() )
        return NULL;
    return itEdge.GetItem();
}
inline JointEdge * IslandSolver::GetJoint( const IslandNode * pNodeA, const IslandNode * pNodeB ) const {
    IslandEdgeID idEdge( pNodeA, pNodeB );
    JointMap::Iterator itEdge = m_mapJoints.Get( idEdge );
    if ( itEdge.IsNull() )
        return NULL;
    return itEdge.GetItem();
}

inline ContactCallback * IslandSolver::GetContactCallback() const {
    return m_pContactCallback;
}
inline Void IslandSolver::SetContactCallback( ContactCallback * pContactCallback ) {
    m_pContactCallback = pContactCallback;
}

