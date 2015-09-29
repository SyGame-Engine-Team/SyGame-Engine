/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Joints/JointHinge.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Joints : Hinge joint (often called Revolute)
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// JointHinge implementation
inline JointConstraintType JointHinge::GetType() const {
    return JOINT_HINGE;
}

inline JointLimitMotorSpring * JointHinge::GetLimitMotorSpring() {
    return &m_hAngularLMS;
}

inline Bool JointHinge::IsFree() const {
    return m_hAngularLMS.IsFree();
}
inline Bool JointHinge::IsLimited() const {
    return m_hAngularLMS.IsLimited();
}
inline Bool JointHinge::IsFixed() const {
    return m_hAngularLMS.IsFixed();
}
inline Scalar JointHinge::GetLowerLimit() const {
    return m_hAngularLMS.GetLowerLimit();
}
inline Scalar JointHinge::GetUpperLimit() const {
    return m_hAngularLMS.GetUpperLimit();
}

inline Void JointHinge::EnableLimits( Scalar fLowerLimit, Scalar fUpperLimit ) {
    m_hAngularLMS.EnableLimits( fLowerLimit, fUpperLimit );
}
inline Void JointHinge::DisableLimits() {
    m_hAngularLMS.DisableLimits();
}

inline Bool JointHinge::HasMotor() const {
    return m_hAngularLMS.HasMotor();
}
inline Bool JointHinge::IsServoMotor() const {
    return m_hAngularLMS.IsServoMotor();
}
inline Scalar JointHinge::GetMotorTargetVelocity() const {
    return m_hAngularLMS.GetMotorTargetVelocity();
}
inline Scalar JointHinge::GetServoMotorTargetPosition() const {
    return m_hAngularLMS.GetServoMotorTargetPosition();
}
inline Scalar JointHinge::GetMotorMaxImpulse() const {
    return m_hAngularLMS.GetMotorMaxImpulse();
}

inline Void JointHinge::EnableMotor( Scalar fTargetVelocity, Scalar fMaxImpulse ) {
    m_hAngularLMS.EnableMotor( fTargetVelocity, fMaxImpulse );
    m_pBodyA->SetSleeping( false ); // Wake up bodies when
    m_pBodyB->SetSleeping( false ); // enabling motor !
}
inline Void JointHinge::EnableServoMotor( Scalar fTargetVelocity, Scalar fTargetPosition, Scalar fMaxImpulse ) {
    m_hAngularLMS.EnableServoMotor( fTargetVelocity, fTargetPosition, fMaxImpulse );
    m_pBodyA->SetSleeping( false ); // Wake up bodies when
    m_pBodyB->SetSleeping( false ); // enabling motor !
}
inline Void JointHinge::DisableMotor() {
    m_hAngularLMS.DisableMotor();
}

inline Bool JointHinge::HasSpring() const {
    return m_hAngularLMS.HasSpring();
}
inline Scalar JointHinge::GetSpringStiffness() const {
    return m_hAngularLMS.GetSpringStiffness();
}
inline Scalar JointHinge::GetSpringEquilibriumPoint() const {
    return m_hAngularLMS.GetSpringEquilibriumPoint();
}

inline Void JointHinge::EnableSpring( Scalar fStiffness, Scalar fEquilibriumPoint ) {
    m_hAngularLMS.EnableSpring( fStiffness, fEquilibriumPoint );
}
inline Void JointHinge::DisableSpring() {
    m_hAngularLMS.DisableSpring();
}

inline Scalar JointHinge::GetRotation( Vector3 * outAxis ) const {
    *outAxis = m_vHingeAxis;
    return m_fHingeAngle;
}



