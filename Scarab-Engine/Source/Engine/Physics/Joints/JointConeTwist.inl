/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Joints/JointConeTwist.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Joints : Cone-Twist joint (often called ... Shoulder ?!)
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// JointConeTwist implementation
inline JointConstraintType JointConeTwist::GetType() const {
    return JOINT_CONETWIST;
}

inline JointLimitMotorSpring * JointConeTwist::GetLimitMotorSpring( JointAxis iAxis ) {
    if ( iAxis == JOINTAXIS_SWING )
        return &m_hSwingLMS;
    Assert( iAxis == JOINTAXIS_TWIST );
    return &m_hTwistLMS;
}

inline Bool JointConeTwist::IsFree( JointAxis iAxis ) const {
    if ( iAxis == JOINTAXIS_SWING )
        return m_hSwingLMS.IsFree();
    Assert( iAxis == JOINTAXIS_TWIST );
    return m_hTwistLMS.IsFree();
}
inline Bool JointConeTwist::IsLimited( JointAxis iAxis ) const {
    if ( iAxis == JOINTAXIS_SWING )
        return m_hSwingLMS.IsLimited();
    Assert( iAxis == JOINTAXIS_TWIST );
    return m_hTwistLMS.IsLimited();
}
inline Bool JointConeTwist::IsFixed( JointAxis iAxis ) const {
    if ( iAxis == JOINTAXIS_SWING )
        return m_hSwingLMS.IsFixed();
    Assert( iAxis == JOINTAXIS_TWIST );
    return m_hTwistLMS.IsFixed();
}
inline Scalar JointConeTwist::GetLowerLimit( JointAxis iAxis ) const {
    if ( iAxis == JOINTAXIS_SWING )
        return m_hSwingLMS.GetLowerLimit();
    Assert( iAxis == JOINTAXIS_TWIST );
    return m_hTwistLMS.GetLowerLimit();
}
inline Scalar JointConeTwist::GetUpperLimit( JointAxis iAxis ) const {
    if ( iAxis == JOINTAXIS_SWING )
        return m_hSwingLMS.GetUpperLimit();
    Assert( iAxis == JOINTAXIS_TWIST );
    return m_hTwistLMS.GetUpperLimit();
}

inline Void JointConeTwist::SetSwingLimits( Scalar fSwingSpan1, Scalar fSwingSpan2 ) {
    Assert( fSwingSpan1 >= JOINT_CONETWIST_MINIMAL_SPAN && fSwingSpan2 >= JOINT_CONETWIST_MINIMAL_SPAN );
    m_fSwingSpan1 = fSwingSpan1;
    m_fSwingSpan2 = fSwingSpan2;
}
inline Void JointConeTwist::EnableTwistLimits( Scalar fLowerLimit, Scalar fUpperLimit ) {
    m_hTwistLMS.EnableLimits( fLowerLimit, fUpperLimit );
}
inline Void JointConeTwist::DisableTwistLimits() {
    m_hTwistLMS.DisableLimits();
}

inline Bool JointConeTwist::HasMotor( JointAxis iAxis ) const {
    if ( iAxis == JOINTAXIS_SWING )
        return m_hSwingLMS.HasMotor();
    Assert( iAxis == JOINTAXIS_TWIST );
    return m_hTwistLMS.HasMotor();
}
inline Bool JointConeTwist::IsServoMotor( JointAxis iAxis ) const {
    if ( iAxis == JOINTAXIS_SWING )
        return m_hSwingLMS.IsServoMotor();
    Assert( iAxis == JOINTAXIS_TWIST );
    return m_hTwistLMS.IsServoMotor();
}
inline Scalar JointConeTwist::GetMotorTargetVelocity( JointAxis iAxis ) const {
    if ( iAxis == JOINTAXIS_SWING )
        return m_hSwingLMS.GetMotorTargetVelocity();
    Assert( iAxis == JOINTAXIS_TWIST );
    return m_hTwistLMS.GetMotorTargetVelocity();
}
inline Scalar JointConeTwist::GetServoMotorTargetPosition( JointAxis iAxis ) const {
    if ( iAxis == JOINTAXIS_SWING )
        return m_hSwingLMS.GetServoMotorTargetPosition();
    Assert( iAxis == JOINTAXIS_TWIST );
    return m_hTwistLMS.GetServoMotorTargetPosition();
}
inline Scalar JointConeTwist::GetMotorMaxImpulse( JointAxis iAxis ) const {
    if ( iAxis == JOINTAXIS_SWING )
        return m_hSwingLMS.GetMotorMaxImpulse();
    Assert( iAxis == JOINTAXIS_TWIST );
    return m_hTwistLMS.GetMotorMaxImpulse();
}

inline Void JointConeTwist::EnableMotor( JointAxis iAxis, Scalar fTargetVelocity, Scalar fMaxImpulse ) {
    if ( iAxis == JOINTAXIS_SWING )
        m_hSwingLMS.EnableMotor( fTargetVelocity, fMaxImpulse );
    else {
        Assert( iAxis == JOINTAXIS_TWIST );
        m_hTwistLMS.EnableMotor( fTargetVelocity, fMaxImpulse );
    }
    m_pBodyA->SetSleeping( false ); // Wake up bodies when
    m_pBodyB->SetSleeping( false ); // enabling motor !
}
inline Void JointConeTwist::EnableServoMotor( JointAxis iAxis, Scalar fTargetVelocity, Scalar fTargetPosition, Scalar fMaxImpulse ) {
    if ( iAxis == JOINTAXIS_SWING )
        m_hSwingLMS.EnableServoMotor( fTargetVelocity, fTargetPosition, fMaxImpulse );
    else {
        Assert( iAxis == JOINTAXIS_TWIST );
        m_hTwistLMS.EnableServoMotor( fTargetVelocity, fTargetPosition, fMaxImpulse );
    }
    m_pBodyA->SetSleeping( false ); // Wake up bodies when
    m_pBodyB->SetSleeping( false ); // enabling motor !
}
inline Void JointConeTwist::DisableMotor( JointAxis iAxis ) {
    if ( iAxis == JOINTAXIS_SWING )
        m_hSwingLMS.DisableMotor();
    else {
        Assert( iAxis == JOINTAXIS_TWIST );
        m_hTwistLMS.DisableMotor();
    }
}

inline Bool JointConeTwist::HasSpring( JointAxis iAxis ) const {
    if ( iAxis == JOINTAXIS_SWING )
        return m_hSwingLMS.HasSpring();
    Assert( iAxis == JOINTAXIS_TWIST );
    return m_hTwistLMS.HasSpring();
}
inline Scalar JointConeTwist::GetSpringStiffness( JointAxis iAxis ) const {
    if ( iAxis == JOINTAXIS_SWING )
        return m_hSwingLMS.GetSpringStiffness();
    Assert( iAxis == JOINTAXIS_TWIST );
    return m_hTwistLMS.GetSpringStiffness();
}
inline Scalar JointConeTwist::GetSpringEquilibriumPoint( JointAxis iAxis ) const {
    if ( iAxis == JOINTAXIS_SWING )
        return m_hSwingLMS.GetSpringEquilibriumPoint();
    Assert( iAxis == JOINTAXIS_TWIST );
    return m_hTwistLMS.GetSpringEquilibriumPoint();
}

inline Void JointConeTwist::EnableSpring( JointAxis iAxis, Scalar fStiffness, Scalar fEquilibriumPoint ) {
    if ( iAxis == JOINTAXIS_SWING )
        m_hSwingLMS.EnableSpring( fStiffness, fEquilibriumPoint );
    else {
        Assert( iAxis == JOINTAXIS_TWIST );
        m_hTwistLMS.EnableSpring( fStiffness, fEquilibriumPoint );
    }
}
inline Void JointConeTwist::DisableSpring( JointAxis iAxis ) {
    if ( iAxis == JOINTAXIS_SWING )
        m_hSwingLMS.DisableSpring();
    else {
        Assert( iAxis == JOINTAXIS_TWIST );
        m_hTwistLMS.DisableSpring();
    }
}

inline Scalar JointConeTwist::GetSwingRotation( Vector3 * outSwingAxis ) const {
    *outSwingAxis = m_vSwingAxis;
    return m_fSwingAngle;
}
inline Scalar JointConeTwist::GetTwistRotation( Vector3 * outTwistAxis ) const {
    *outTwistAxis = m_vTwistAxis;
    return m_fTwistAngle;
}

