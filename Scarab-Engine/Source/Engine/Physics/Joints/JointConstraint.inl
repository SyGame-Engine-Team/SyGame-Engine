/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Joints/JointConstraint.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Joint constraints definitions
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// JointLimitMotorSpring implementation
inline Bool JointLimitMotorSpring::IsLinear() const {
    return !m_bAngularElseLinear;
}
inline Bool JointLimitMotorSpring::IsAngular() const {
    return m_bAngularElseLinear;
}

inline Bool JointLimitMotorSpring::IsFree() const {
    return ( m_fLowerLimit > m_fUpperLimit );
}
inline Bool JointLimitMotorSpring::IsLimited() const {
    return ( m_fLowerLimit <= m_fUpperLimit );
}
inline Bool JointLimitMotorSpring::IsFixed() const {
    return ( m_fLowerLimit == m_fUpperLimit );
}
inline Scalar JointLimitMotorSpring::GetLowerLimit() const {
    Assert( ( m_fLowerLimit <= m_fUpperLimit ) );
    return m_fLowerLimit;
}
inline Scalar JointLimitMotorSpring::GetUpperLimit() const {
    Assert( ( m_fLowerLimit <= m_fUpperLimit ) );
    return m_fUpperLimit;
}

inline Void JointLimitMotorSpring::EnableLimits( Scalar fLowerLimit, Scalar fUpperLimit ) {
    if ( m_bAngularElseLinear ) {
        fLowerLimit = MathFn->NormalizeAngle( fLowerLimit );
        fUpperLimit = MathFn->NormalizeAngle( fUpperLimit );
    }
    Assert( fLowerLimit <= fUpperLimit );
    m_fLowerLimit = fLowerLimit;
    m_fUpperLimit = fUpperLimit;
}
inline Void JointLimitMotorSpring::DisableLimits() {
    m_fLowerLimit = 1.0f;
    m_fUpperLimit = -1.0f;
}

inline Bool JointLimitMotorSpring::HasMotor() const {
    return m_bMotorEnabled;
}
inline Bool JointLimitMotorSpring::IsServoMotor() const {
    Assert( m_bMotorEnabled );
    return m_bIsServoMotor;
}
inline Scalar JointLimitMotorSpring::GetMotorTargetVelocity() const {
    Assert( m_bMotorEnabled );
    return m_fMotorTargetVelocity;
}
inline Scalar JointLimitMotorSpring::GetServoMotorTargetPosition() const {
    Assert( m_bMotorEnabled && m_bIsServoMotor );
    return m_fServoMotorTargetPosition;
}
inline Scalar JointLimitMotorSpring::GetMotorMaxImpulse() const {
    Assert( m_bMotorEnabled );
    return m_fMotorMaxImpulse;
}

inline Void JointLimitMotorSpring::EnableMotor( Scalar fTargetVelocity, Scalar fMaxImpulse ) {
    Assert( fTargetVelocity != 0.0f );
    m_bMotorEnabled = true;
    m_bIsServoMotor = false;
    m_fMotorTargetVelocity = fTargetVelocity;
    m_fServoMotorTargetPosition = 0.0f;
    m_fMotorMaxImpulse = fMaxImpulse;
}
inline Void JointLimitMotorSpring::EnableServoMotor( Scalar fTargetVelocity, Scalar fTargetPosition, Scalar fMaxImpulse ) {
    Assert( fTargetVelocity > 0.0f );
    m_bMotorEnabled = true;
    m_bIsServoMotor = true;
    m_fMotorTargetVelocity = fTargetVelocity;
    m_fServoMotorTargetPosition = fTargetPosition;
    m_fMotorMaxImpulse = fMaxImpulse;
}
inline Void JointLimitMotorSpring::DisableMotor() {
    m_bMotorEnabled = false;
    m_bIsServoMotor = false;
    m_fMotorTargetVelocity = 0.0f;
    m_fServoMotorTargetPosition = 0.0f;
    m_fMotorMaxImpulse = 0.0f;
}

inline Bool JointLimitMotorSpring::HasSpring() const {
    return m_bSpringEnabled;
}
inline Scalar JointLimitMotorSpring::GetSpringStiffness() const {
    Assert( m_bSpringEnabled );
    return m_fSpringStiffness;
}
inline Scalar JointLimitMotorSpring::GetSpringEquilibriumPoint() const {
    Assert( m_bSpringEnabled );
    return m_fSpringEquilibriumPoint;
}

inline Void JointLimitMotorSpring::EnableSpring( Scalar fStiffness, Scalar fEquilibriumPoint ) {
    Assert( fStiffness != 0.0f );
    m_bSpringEnabled = true;
    m_fSpringStiffness = fStiffness;
    m_fSpringEquilibriumPoint = fEquilibriumPoint;
}
inline Void JointLimitMotorSpring::DisableSpring() {
    m_bSpringEnabled = false;
    m_fSpringStiffness = 0.0f;
    m_fSpringEquilibriumPoint = 0.0f;
}

inline Scalar JointLimitMotorSpring::GetTestPosition() const {
    return m_fTestPositionOrAngle;
}
inline Scalar JointLimitMotorSpring::GetTestAngle() const {
    return m_fTestPositionOrAngle;
}

inline JointLimitState JointLimitMotorSpring::GetLimitState() const {
    return m_iLimitState;
}
inline Scalar JointLimitMotorSpring::GetLimitError() const {
    return m_fLimitError;
}

inline Bool JointLimitMotorSpring::NeedCorrection() const {
    return ( m_iLimitState != JOINTLIMIT_FREE || m_bMotorEnabled || m_bSpringEnabled );
}

/////////////////////////////////////////////////////////////////////////////////
// JointConstraint implementation
inline BodyNode * JointConstraint::GetBodyA() const {
    return m_pBodyA;
}
inline BodyNode * JointConstraint::GetBodyB() const {
    return m_pBodyB;
}

inline const Transform3 & JointConstraint::GetFrameInA() const {
    return m_vFrameInA;
}
inline const Transform3 & JointConstraint::GetFrameInB() const {
    return m_vFrameInB;
}
inline Void JointConstraint::GetFrame( Transform3 * outJointFrame ) const {
    *outJointFrame = ( m_pBodyA->GetTransform() * m_vFrameInA );
    //Assert( *outJointFrame == (m_pBodyB->GetTransform() * m_vFrameInB) );
}

inline Bool JointConstraint::IsEnabled() const {
    return m_bEnabled;
}
inline Void JointConstraint::Enable() {
    m_bEnabled = true;
}
inline Void JointConstraint::Disable() {
    m_bEnabled = false;
}

inline Scalar JointConstraint::GetBreakingThreshold() const {
    return m_fBreakingThreshold;
}
inline Void JointConstraint::SetBreakingThreshold( Scalar fBreakingThreshold ) {
    m_fBreakingThreshold = fBreakingThreshold;
}

inline Bool JointConstraint::IsFeedbackEnabled() const {
    return m_bFeedbackEnabled;
}
inline Void JointConstraint::EnableFeedback() {
    m_bFeedbackEnabled = true;
}
inline Void JointConstraint::DisableFeedback() {
    m_bFeedbackEnabled = false;
}

inline JointFeedback * JointConstraint::GetFeedback() const {
    return m_pFeedback;
}
inline Void JointConstraint::SetFeedback( JointFeedback * pJointFeedback ) {
    m_pFeedback = pJointFeedback;
}

inline const Transform3 & JointConstraint::GetTransformA() const {
    return m_vJointTransformA;
}
inline const Transform3 & JointConstraint::GetTransformB() const {
    return m_vJointTransformB;
}

inline UInt JointConstraint::GetRowCount() const {
    return m_iRowCount;
}
inline JointConstraintRow * JointConstraint::GetRow( UInt iRow ) {
    Assert( iRow < m_iRowCount );
    return ( m_arrRows + iRow );
}

