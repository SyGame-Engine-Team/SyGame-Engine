/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Controllers/KinematicController.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : The Base-Class for all kinematic controllers
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// KinematicController implementation
inline Bool KinematicController::IsMotionController() const {
    return ( GetType() < CONTROLLER_SHAPE_MORPH );
}
inline Bool KinematicController::IsShapeController() const {
    return ( GetType() >= CONTROLLER_SHAPE_MORPH );
}

inline const GChar * KinematicController::GetName() const {
    return m_strName;
}

inline Scalar KinematicController::GetLocalTime() const {
    return m_fLocalTime;
}
inline Void KinematicController::SetLocalTime( Scalar fLocalTime ) {
    m_fLocalTime = fLocalTime;
}

inline Scalar KinematicController::GetControlTime() const {
    return m_fControlTime;
}

inline KinematicBody * KinematicController::GetTarget() const {
    return m_pTarget;
}

/////////////////////////////////////////////////////////////////////////////////
// MotionController implementation
inline const Vertex3 & MotionController::GetPosition() const {
    return m_vPosition;
}
inline const Quaternion & MotionController::GetOrientation() const {
    return m_qOrientation;
}

inline const Vector3 & MotionController::GetLinearSpeed() const {
    return m_vLinearSpeed;
}
inline const Vector3 & MotionController::GetAngularSpeed() const {
    return m_vAngularSpeed;
}

inline Void MotionController::ApplyLinearImpulse( const Vector3 & vImpulse ) {
    m_vLinearSpeed += vImpulse;
}
inline Void MotionController::ApplyAngularImpulse( const Vector3 & vImpulse ) {
    m_vAngularSpeed += vImpulse;
}

inline Void MotionController::ApplyLinearCorrection( const Vector3 & vCorrection ) {
    m_vPosition += vCorrection;
}
inline Void MotionController::ApplyAngularCorrection( const Quaternion & qCorrection ) {
    m_qOrientation += qCorrection;
}

/////////////////////////////////////////////////////////////////////////////////
// ShapeController implementation


