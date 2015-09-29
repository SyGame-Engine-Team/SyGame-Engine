/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Joints/JointGeneric6DOF.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Joints : Generic 6-DOF joint with limits & motors
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// JointGeneric6DOF implementation
inline JointConstraintType JointGeneric6DOF::GetType() const {
    return JOINT_GENERIC6DOF;
}

inline EulerAngles JointGeneric6DOF::GetEulerAnglesOrdering() const {
    return m_iEulerAnglesOrdering;
}

inline JointLimitMotorSpring * JointGeneric6DOF::GetLimitMotorSpring( JointAxis iAxis ) {
    if ( iAxis < JOINTAXIS_ANGULAR_0 )
        return ( m_hLinearLMS + iAxis );
    return ( m_hAngularLMS + (iAxis-JOINTAXIS_ANGULAR_0) );
}

inline Bool JointGeneric6DOF::IsFree( JointAxis iAxis ) const {
    if ( iAxis < JOINTAXIS_ANGULAR_0 )
        return m_hLinearLMS[iAxis].IsFree();
    return m_hAngularLMS[iAxis-JOINTAXIS_ANGULAR_0].IsFree();
}
inline Bool JointGeneric6DOF::IsLimited( JointAxis iAxis ) const {
    if ( iAxis < JOINTAXIS_ANGULAR_0 )
        return m_hLinearLMS[iAxis].IsLimited();
    return m_hAngularLMS[iAxis-JOINTAXIS_ANGULAR_0].IsLimited();
}
inline Bool JointGeneric6DOF::IsFixed( JointAxis iAxis ) const {
    if ( iAxis < JOINTAXIS_ANGULAR_0 )
        return m_hLinearLMS[iAxis].IsFixed();
    return m_hAngularLMS[iAxis-JOINTAXIS_ANGULAR_0].IsFixed();
}
inline Scalar JointGeneric6DOF::GetLowerLimit( JointAxis iAxis ) const {
    if ( iAxis < JOINTAXIS_ANGULAR_0 )
        return m_hLinearLMS[iAxis].GetLowerLimit();
    return m_hAngularLMS[iAxis-JOINTAXIS_ANGULAR_0].GetLowerLimit();
}
inline Scalar JointGeneric6DOF::GetUpperLimit( JointAxis iAxis ) const {
    if ( iAxis < JOINTAXIS_ANGULAR_0 )
        return m_hLinearLMS[iAxis].GetUpperLimit();
    return m_hAngularLMS[iAxis-JOINTAXIS_ANGULAR_0].GetUpperLimit();
}

inline Void JointGeneric6DOF::EnableLimits( JointAxis iAxis, Scalar fLowerLimit, Scalar fUpperLimit ) {
    if ( iAxis < JOINTAXIS_ANGULAR_0 )
        m_hLinearLMS[iAxis].EnableLimits( fLowerLimit, fUpperLimit );
    else
        m_hAngularLMS[iAxis-JOINTAXIS_ANGULAR_0].EnableLimits( fLowerLimit, fUpperLimit );
}
inline Void JointGeneric6DOF::DisableLimits( JointAxis iAxis ) {
    if ( iAxis < JOINTAXIS_ANGULAR_0 )
        m_hLinearLMS[iAxis].DisableLimits();
    else
        m_hAngularLMS[iAxis-JOINTAXIS_ANGULAR_0].DisableLimits();
}

inline Bool JointGeneric6DOF::HasMotor( JointAxis iAxis ) const {
    if ( iAxis < JOINTAXIS_ANGULAR_0 )
        return m_hLinearLMS[iAxis].HasMotor();
    return m_hAngularLMS[iAxis-JOINTAXIS_ANGULAR_0].HasMotor();
}
inline Bool JointGeneric6DOF::IsServoMotor( JointAxis iAxis ) const {
    if ( iAxis < JOINTAXIS_ANGULAR_0 )
        return m_hLinearLMS[iAxis].IsServoMotor();
    return m_hAngularLMS[iAxis-JOINTAXIS_ANGULAR_0].IsServoMotor();
}
inline Scalar JointGeneric6DOF::GetMotorTargetVelocity( JointAxis iAxis ) const {
    if ( iAxis < JOINTAXIS_ANGULAR_0 )
        return m_hLinearLMS[iAxis].GetMotorTargetVelocity();
    return m_hAngularLMS[iAxis-JOINTAXIS_ANGULAR_0].GetMotorTargetVelocity();
}
inline Scalar JointGeneric6DOF::GetServoMotorTargetPosition( JointAxis iAxis ) const {
    if ( iAxis < JOINTAXIS_ANGULAR_0 )
        return m_hLinearLMS[iAxis].GetServoMotorTargetPosition();
    return m_hAngularLMS[iAxis-JOINTAXIS_ANGULAR_0].GetServoMotorTargetPosition();
}
inline Scalar JointGeneric6DOF::GetMotorMaxImpulse( JointAxis iAxis ) const {
    if ( iAxis < JOINTAXIS_ANGULAR_0 )
        return m_hLinearLMS[iAxis].GetMotorMaxImpulse();
    return m_hAngularLMS[iAxis-JOINTAXIS_ANGULAR_0].GetMotorMaxImpulse();
}

inline Void JointGeneric6DOF::EnableMotor( JointAxis iAxis, Scalar fTargetVelocity, Scalar fMaxImpulse ) {
    if ( iAxis < JOINTAXIS_ANGULAR_0 )
        m_hLinearLMS[iAxis].EnableMotor( fTargetVelocity, fMaxImpulse );
    else
        m_hAngularLMS[iAxis-JOINTAXIS_ANGULAR_0].EnableMotor( fTargetVelocity, fMaxImpulse );
    m_pBodyA->SetSleeping( false ); // Wake up bodies when
    m_pBodyB->SetSleeping( false ); // enabling motor !
}
inline Void JointGeneric6DOF::EnableServoMotor( JointAxis iAxis, Scalar fTargetVelocity, Scalar fTargetPosition, Scalar fMaxImpulse ) {
    if ( iAxis < JOINTAXIS_ANGULAR_0 )
        m_hLinearLMS[iAxis].EnableServoMotor( fTargetVelocity, fTargetPosition, fMaxImpulse );
    else
        m_hAngularLMS[iAxis-JOINTAXIS_ANGULAR_0].EnableServoMotor( fTargetVelocity, fTargetPosition, fMaxImpulse );
    m_pBodyA->SetSleeping( false ); // Wake up bodies when
    m_pBodyB->SetSleeping( false ); // enabling motor !
}
inline Void JointGeneric6DOF::DisableMotor( JointAxis iAxis ) {
    if ( iAxis < JOINTAXIS_ANGULAR_0 )
        m_hLinearLMS[iAxis].DisableMotor();
    else
        m_hAngularLMS[iAxis-JOINTAXIS_ANGULAR_0].DisableMotor();
}

inline Bool JointGeneric6DOF::HasSpring( JointAxis iAxis ) const {
    if ( iAxis < JOINTAXIS_ANGULAR_0 )
        return m_hLinearLMS[iAxis].HasSpring();
    return m_hAngularLMS[iAxis-JOINTAXIS_ANGULAR_0].HasSpring();
}
inline Scalar JointGeneric6DOF::GetSpringStiffness( JointAxis iAxis ) const {
    if ( iAxis < JOINTAXIS_ANGULAR_0 )
        return m_hLinearLMS[iAxis].GetSpringStiffness();
    return m_hAngularLMS[iAxis-JOINTAXIS_ANGULAR_0].GetSpringStiffness();
}
inline Scalar JointGeneric6DOF::GetSpringEquilibriumPoint( JointAxis iAxis ) const {
    if ( iAxis < JOINTAXIS_ANGULAR_0 )
        return m_hLinearLMS[iAxis].GetSpringEquilibriumPoint();
    return m_hAngularLMS[iAxis-JOINTAXIS_ANGULAR_0].GetSpringEquilibriumPoint();
}

inline Void JointGeneric6DOF::EnableSpring( JointAxis iAxis, Scalar fStiffness, Scalar fEquilibriumPoint ) {
    if ( iAxis < JOINTAXIS_ANGULAR_0 )
        m_hLinearLMS[iAxis].EnableSpring( fStiffness, fEquilibriumPoint );
    else
        m_hAngularLMS[iAxis-JOINTAXIS_ANGULAR_0].EnableSpring( fStiffness, fEquilibriumPoint );
}
inline Void JointGeneric6DOF::DisableSpring( JointAxis iAxis ) {
    if ( iAxis < JOINTAXIS_ANGULAR_0 )
        m_hLinearLMS[iAxis].DisableSpring();
    else
        m_hAngularLMS[iAxis-JOINTAXIS_ANGULAR_0].DisableSpring();
}

inline Scalar JointGeneric6DOF::GetTranslation( JointAxis iAxis ) const {
    Assert( iAxis < JOINTAXIS_ANGULAR_0 );
    return m_vTranslation[iAxis];
}
inline Scalar JointGeneric6DOF::GetRotation( Vector3 * outAxis, JointAxis iAxis ) const {
    Assert( iAxis >= JOINTAXIS_ANGULAR_0 );
    *outAxis = m_vRotationAxis[iAxis-JOINTAXIS_ANGULAR_0];
    return m_vRotationEulerAngles[iAxis-JOINTAXIS_ANGULAR_0];
}

