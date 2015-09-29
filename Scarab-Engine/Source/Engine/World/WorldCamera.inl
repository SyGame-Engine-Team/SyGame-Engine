/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/WorldCamera.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Graph : Camera management
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// WorldCamera implementation
inline Camera * WorldCamera::GetCamera() const {
    return m_pCamera;
}
inline WorldBaseNode * WorldCamera::GetTarget() const {
    return m_pTarget;
}

/////////////////////////////////////////////////////////////////////////////////
// WorldCameraSubjective implementation
inline WorldCameraType WorldCameraSubjective::GetCameraType() const {
    return WORLDCAMERA_SUBJECTIVE;
}

/////////////////////////////////////////////////////////////////////////////////
// WorldCamera3rdPerson implementation
inline WorldCameraType WorldCamera3rdPerson::GetCameraType() const {
    return WORLDCAMERA_3RDPERSON;
}

inline ManualController * WorldCamera3rdPerson::GetController() const {
    return m_pCameraController;
}

inline Scalar WorldCamera3rdPerson::GetMinRadius() const {
    return m_fMinRadius;
}
inline Void WorldCamera3rdPerson::SetMinRadius( Scalar fMinRadius ) {
    if ( fMinRadius < WORLDCAMERA_MIN_RADIUS )
        fMinRadius = WORLDCAMERA_MIN_RADIUS;
    m_fMinRadius = fMinRadius;
    if ( m_fMaxRadius < m_fMinRadius )
        m_fMaxRadius = m_fMinRadius;
}
inline Scalar WorldCamera3rdPerson::GetMaxRadius() const {
    return m_fMaxRadius;
}
inline Void WorldCamera3rdPerson::SetMaxRadius( Scalar fMaxRadius ) {
    if ( fMaxRadius > WORLDCAMERA_MAX_RADIUS )
        fMaxRadius = WORLDCAMERA_MAX_RADIUS;
    m_fMaxRadius = fMaxRadius;
    if ( m_fMinRadius > m_fMaxRadius )
        m_fMinRadius = m_fMaxRadius;
}

inline Void WorldCamera3rdPerson::Zoom( Scalar fDistance ) {
    m_fRadius = Clamp<Scalar>( m_fRadius + fDistance, m_fMinRadius, m_fMaxRadius );
}
inline Void WorldCamera3rdPerson::RotateH( Scalar fAngle ) {
    m_fAngleH += fAngle;
    if ( m_fAngleH < 0.0f )
        m_fAngleH += SCALAR_2PI;
    else if ( m_fAngleH >= SCALAR_2PI )
        m_fAngleH -= SCALAR_2PI;
}
