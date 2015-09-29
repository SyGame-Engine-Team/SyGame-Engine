/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/Entities/CharEntity.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : WorldCraft, Entities : CharEntity
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// CharEntity implementation
inline EntityType CharEntity::GetEntityType() const {
    return ENTITYTYPE_CHARACTER;
}

inline BaseCharacter * CharEntity::GetCharacter() const {
    return m_pCharacter;
}

inline CharacterController * CharEntity::GetController() const {
    return m_pController;
}

inline Bool CharEntity::IsMovingForward() const {
    Assert( !m_bPathTrackingMode );
    return m_pController->IsMovingForward();
}
inline Bool CharEntity::IsMovingBackward() const {
    Assert( !m_bPathTrackingMode );
    return m_pController->IsMovingBackward();
}
inline Bool CharEntity::IsMovingLeft() const {
    Assert( !m_bPathTrackingMode );
    return m_pController->IsMovingLeft();
}
inline Bool CharEntity::IsMovingRight() const {
    Assert( !m_bPathTrackingMode );
    return m_pController->IsMovingRight();
}
inline Bool CharEntity::IsMovingUp() const {
    Assert( !m_bPathTrackingMode );
    return m_pController->IsMovingUp();
}
inline Bool CharEntity::IsMovingDown() const {
    Assert( !m_bPathTrackingMode );
    return m_pController->IsMovingDown();
}
inline Bool CharEntity::IsMoving() const {
    Assert( !m_bPathTrackingMode );
    return m_pController->IsMoving();
}

inline Void CharEntity::SetMovingForward( Bool bMoving ) {
    if ( bMoving && m_bPathTrackingMode )
        EndPathTracking();
    m_pController->SetMovingForward( bMoving );
}
inline Void CharEntity::SetMovingBackward( Bool bMoving ) {
    if ( bMoving && m_bPathTrackingMode )
        EndPathTracking();
    m_pController->SetMovingBackward( bMoving );
}
inline Void CharEntity::SetMovingLeft( Bool bMoving ) {
    if ( bMoving && m_bPathTrackingMode )
        EndPathTracking();
    m_pController->SetMovingLeft( bMoving );
}
inline Void CharEntity::SetMovingRight( Bool bMoving ) {
    if ( bMoving && m_bPathTrackingMode )
        EndPathTracking();
    m_pController->SetMovingRight( bMoving );
}
inline Void CharEntity::SetMovingUp( Bool bMoving ) {
    if ( bMoving && m_bPathTrackingMode )
        EndPathTracking();
    m_pController->SetMovingUp( bMoving );
}
inline Void CharEntity::SetMovingDown( Bool bMoving ) {
    if ( bMoving && m_bPathTrackingMode )
        EndPathTracking();
    m_pController->SetMovingDown( bMoving );
}
inline Void CharEntity::SetStanding() {
    if ( m_bPathTrackingMode )
        EndPathTracking();
    m_pController->SetStanding();
}

inline const Vector3 & CharEntity::GetMovementDirection() const {
    Assert( !m_bPathTrackingMode );
    return m_pController->GetMovementDirection();
}

inline Void CharEntity::Teleport( const Vertex3 & vPosition, Bool bKeepInertia ) {
    if ( m_bPathTrackingMode )
        EndPathTracking();
    m_pController->Teleport( vPosition, bKeepInertia );
}

inline PathController * CharEntity::GetPathController() const {
    return m_pPathController;
}

inline BSplinePatch3 * CharEntity::GetLinearPath() const {
    return m_pLinearPath;
}
inline BSplinePatch2 * CharEntity::GetLinearTravel() const {
    return m_pLinearTravel;
}
inline BSplinePatch3 * CharEntity::GetAngularPath() const {
    return m_pAngularPath;
}
inline BSplinePatch2 * CharEntity::GetAngularTravel() const {
    return m_pAngularTravel;
}

inline Bool CharEntity::IsPathTracking() const {
    return m_bPathTrackingMode;
}

/////////////////////////////////////////////////////////////////////////////////
// SelectionMarkerEntity implementation
inline EntityType SelectionMarkerEntity::GetEntityType() const {
    return ENTITYTYPE_EFFECT;
}

inline BaseCharacter * SelectionMarkerEntity::GetCharacter() const {
    return m_pCharacter;
}
inline ManualController * SelectionMarkerEntity::GetController() const {
    return m_pController;
}
