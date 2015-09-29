/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Controllers/Motion/CharacterController.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Motion Controller : Character movement
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// CharacterController implementation
inline ControllerType CharacterController::GetType() const {
    return CONTROLLER_MOTION_CHARACTER;
}

inline Void CharacterController::EnableForces( Bool bEnable ) {
    m_bApplyForces = bEnable;
}

inline Bool CharacterController::IsMovingForward() const {
    return m_bMovingForward;
}
inline Bool CharacterController::IsMovingBackward() const {
    return m_bMovingBackward;
}
inline Bool CharacterController::IsMovingLeft() const {
    return m_bMovingLeft;
}
inline Bool CharacterController::IsMovingRight() const {
    return m_bMovingRight;
}
inline Bool CharacterController::IsMovingUp() const {
    return m_bMovingUp;
}
inline Bool CharacterController::IsMovingDown() const {
    return m_bMovingDown;
}
inline Bool CharacterController::IsMoving() const {
    return ( m_bMovingForward || m_bMovingBackward || m_bMovingLeft || m_bMovingRight || m_bMovingUp || m_bMovingDown );
}

inline Void CharacterController::SetMovingForward( Bool bMoving ) {
    if ( bMoving )
        m_bMovingBackward = false;
    m_bMovingForward = bMoving;
}
inline Void CharacterController::SetMovingBackward( Bool bMoving ) {
    if ( bMoving )
        m_bMovingForward = false;
    m_bMovingBackward = bMoving;
}
inline Void CharacterController::SetMovingLeft( Bool bMoving ) {
    if ( bMoving )
        m_bMovingRight = false;
    m_bMovingLeft = bMoving;
}
inline Void CharacterController::SetMovingRight( Bool bMoving ) {
    if ( bMoving )
        m_bMovingLeft = false;
    m_bMovingRight = bMoving;
}
inline Void CharacterController::SetMovingUp( Bool bMoving ) {
    if ( bMoving )
        m_bMovingDown = false;
    m_bMovingUp = bMoving;
}
inline Void CharacterController::SetMovingDown( Bool bMoving ) {
    if ( bMoving )
        m_bMovingUp = false;
    m_bMovingDown = bMoving;
}
inline Void CharacterController::SetStanding() {
    m_bMovingForward = false;
    m_bMovingBackward = false;
    m_bMovingLeft = false;
    m_bMovingRight = false;
    m_bMovingUp = false;
    m_bMovingDown = false;
}

inline const Vector3 & CharacterController::GetMovementDirection() const {
    return m_vMovementDirection;
}
inline Scalar CharacterController::GetMovementSpeed() const {
    return m_fMovementSpeed;
}
inline Void CharacterController::SetMovementSpeed( Scalar fSpeed ) {
    Assert( fSpeed >= 0.0f );
    m_fMovementSpeed = fSpeed;
}

inline Void CharacterController::Teleport( const Vertex3 & vPosition, Bool bKeepInertia ) {
    if ( !bKeepInertia )
        SetStanding();
    SetPosition( vPosition );
}
