/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Controllers/Motion/ParticleController.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Motion Controller : Particle Systems
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// ParticleController implementation
inline ControllerType ParticleController::GetType() const {
    return CONTROLLER_MOTION_PARTICLE;
}

inline const Vector3 & ParticleController::GetForce() const {
    return m_vForce;
}
inline const Vector3 & ParticleController::GetTorque() const {
    return m_vTorque;
}

inline Void ParticleController::ResetForce() {
    m_vForce = Vector3::Null;
}
inline Void ParticleController::ResetTorque() {
    m_vTorque = Vector3::Null;
}

inline Void ParticleController::AddForce( const Vector3 & vForce ) {
    m_vForce += vForce;
}
inline Void ParticleController::AddTorque( const Vector3 & vTorque ) {
    m_vTorque += vTorque;
}

inline Void ParticleController::SetForce( const Vector3 & vForce ) {
    m_vForce = vForce;
}
inline Void ParticleController::SetTorque( const Vector3 & vTorque ) {
    m_vTorque = vTorque;
}

inline Void ParticleController::ApplyLinearImpulse( const Vector3 & vLinearImpulse ) {
    m_vLinearSpeed += vLinearImpulse;
}
inline Void ParticleController::ApplyAngularImpulse( const Vector3 & vAngularImpulse ) {
    m_vAngularSpeed += vAngularImpulse;
}

inline Void ParticleController::ApplyLinearCorrection( const Vector3 & vLinearCorrection ) {
    m_vPosition += vLinearCorrection;
}
inline Void ParticleController::ApplyAngularCorrection( const Quaternion & qAngularCorrection ) {
    m_qOrientation = ( qAngularCorrection * m_qOrientation );
}

inline Void ParticleController::SetLinearSpeed( const Vector3 & vLinearSpeed ) {
    m_vLinearSpeed = vLinearSpeed;
}
inline Void ParticleController::SetAngularSpeed( const Vector3 & vAngularSpeed ) {
    m_vAngularSpeed = vAngularSpeed;
}

inline Void ParticleController::SetPosition( const Vertex3 & vPosition ) {
    m_vPosition = vPosition;
}
inline Void ParticleController::SetOrientation( const Quaternion & qOrientation ) {
    m_qOrientation = qOrientation;
}

