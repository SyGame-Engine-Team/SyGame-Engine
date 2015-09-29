/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Joints/JointSlider.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Joints : Slider joint (often called Prismatic)
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// JointSlider implementation
inline JointConstraintType JointSlider::GetType() const {
    return JOINT_SLIDER;
}

inline JointLimitMotorSpring * JointSlider::GetLimitMotorSpring() {
    return &m_hLinearLMS;
}

inline Bool JointSlider::IsFree() const {
    return m_hLinearLMS.IsFree();
}
inline Bool JointSlider::IsLimited() const {
    return m_hLinearLMS.IsLimited();
}
inline Bool JointSlider::IsFixed() const {
    return m_hLinearLMS.IsFixed();
}
inline Scalar JointSlider::GetLowerLimit() const {
    return m_hLinearLMS.GetLowerLimit();
}
inline Scalar JointSlider::GetUpperLimit() const {
    return m_hLinearLMS.GetUpperLimit();
}

inline Void JointSlider::EnableLimits( Scalar fLowerLimit, Scalar fUpperLimit ) {
    m_hLinearLMS.EnableLimits( fLowerLimit, fUpperLimit );
}
inline Void JointSlider::DisableLimits() {
    m_hLinearLMS.DisableLimits();
}

inline Bool JointSlider::HasMotor() const {
    return m_hLinearLMS.HasMotor();
}
inline Bool JointSlider::IsServoMotor() const {
    return m_hLinearLMS.IsServoMotor();
}
inline Scalar JointSlider::GetMotorTargetVelocity() const {
    return m_hLinearLMS.GetMotorTargetVelocity();
}
inline Scalar JointSlider::GetServoMotorTargetPosition() const {
    return m_hLinearLMS.GetServoMotorTargetPosition();
}
inline Scalar JointSlider::GetMotorMaxImpulse() const {
    return m_hLinearLMS.GetMotorMaxImpulse();
}

inline Void JointSlider::EnableMotor( Scalar fTargetVelocity, Scalar fMaxImpulse ) {
    m_hLinearLMS.EnableMotor( fTargetVelocity, fMaxImpulse );
    m_pBodyA->SetSleeping( false ); // Wake up bodies when
    m_pBodyB->SetSleeping( false ); // enabling motor !
}
inline Void JointSlider::EnableServoMotor( Scalar fTargetVelocity, Scalar fTargetPosition, Scalar fMaxImpulse ) {
    m_hLinearLMS.EnableServoMotor( fTargetVelocity, fTargetPosition, fMaxImpulse );
    m_pBodyA->SetSleeping( false ); // Wake up bodies when
    m_pBodyB->SetSleeping( false ); // enabling motor !
}
inline Void JointSlider::DisableMotor() {
    m_hLinearLMS.DisableMotor();
}

inline Bool JointSlider::HasSpring() const {
    return m_hLinearLMS.HasSpring();
}
inline Scalar JointSlider::GetSpringStiffness() const {
    return m_hLinearLMS.GetSpringStiffness();
}
inline Scalar JointSlider::GetSpringEquilibriumPoint() const {
    return m_hLinearLMS.GetSpringEquilibriumPoint();
}

inline Void JointSlider::EnableSpring( Scalar fStiffness, Scalar fEquilibriumPoint ) {
    m_hLinearLMS.EnableSpring( fStiffness, fEquilibriumPoint );
}
inline Void JointSlider::DisableSpring() {
    m_hLinearLMS.DisableSpring();
}

inline Scalar JointSlider::GetTranslation( Vector3 * outAxis ) const {
    *outAxis = m_vSliderAxis;
    return m_fSliderPosition;
}

