/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/PhysicsManager.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Core-Class for the Physics system (Forward Dynamics)
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : none
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// PhysicsManager implementation
inline const GChar * PhysicsManager::GetName() {
    return TEXT("PhysicsManager");
}
inline const GChar * PhysicsManager::GetMemoryBudget() {
    return TEXT (
"Heap:Scratch(4194304);\
Heap:Shapes(4194304);Heap:Bodies(4194304);Heap:SoftBodies(4194304);\
Heap:Contacts(4194304);Heap:Joints(4194304);Heap:Controllers(4194304);\
Heap:Broadphase(4194304);Heap:Narrowphase(4194304);\
Heap:IslandSolver(4194304);Heap:ConstraintSolver(4194304);"
    );
}

inline Scalar PhysicsManager::GetShockPropagation() const {
    return m_fShockPropagationFactor;
}
inline Void PhysicsManager::SetShockPropagation( Scalar fFactor ) {
    m_fShockPropagationFactor = Clamp<Scalar>( fFactor, 0.0f, 1.0f );
}

inline const Vector3 & PhysicsManager::GetGravity() const {
    return m_vGravity;
}

inline UInt PhysicsManager::GetIslandCount() const {
    return m_pIslandSolver->GetIslandCount();
}
inline const Island * PhysicsManager::GetIslandList() const {
    return m_pIslandSolver->GetIslandList();
}

inline UInt PhysicsManager::GetBodyCount() const {
    return m_pIslandSolver->GetBodyCount();
}
inline const BodyNode * PhysicsManager::GetBodyList() const {
    return m_pIslandSolver->GetBodyList();
}

inline UInt PhysicsManager::GetContactCount() const {
    return m_pIslandSolver->GetContactCount();
}
inline const ContactEdge * PhysicsManager::GetContactList() const {
    return m_pIslandSolver->GetContactList();
}

inline ContactEdge * PhysicsManager::GetContact( const BodyNode * pBodyA, const BodyNode * pBodyB ) const {
    return m_pIslandSolver->GetContact( pBodyA, pBodyB );
}

inline ContactCallback * PhysicsManager::GetContactCallback() const {
    return m_pIslandSolver->GetContactCallback();
}
inline Void PhysicsManager::SetContactCallback( ContactCallback * pContactCallback ) {
    m_pIslandSolver->SetContactCallback( pContactCallback );
}

inline UInt PhysicsManager::GetJointCount() const {
    return m_pIslandSolver->GetJointCount();
}
inline const JointEdge * PhysicsManager::GetJointList() const {
    return m_pIslandSolver->GetJointList();
}

inline JointEdge * PhysicsManager::GetJoint( const BodyNode * pBodyA, const BodyNode * pBodyB ) const {
    return m_pIslandSolver->GetJoint( pBodyA, pBodyB );
}

