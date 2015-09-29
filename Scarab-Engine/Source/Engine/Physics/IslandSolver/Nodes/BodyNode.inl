/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/IslandSolver/Nodes/BodyNode.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Island Node : Bodies
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// BodyNode implementation
inline IslandNodeType BodyNode::GetType() const {
    return ISLAND_NODE_BODY;
}

inline Bool BodyNode::IsRigid() const {
    return ( GetBodyType() == BODY_TYPE_RIGID );
}
inline Bool BodyNode::IsSoft() const {
    return ( GetBodyType() == BODY_TYPE_SOFT );
}
inline Bool BodyNode::IsKinematic() const {
    return ( GetBodyType() == BODY_TYPE_KINEMATIC );
}

inline Bool BodyNode::IsStatic() const {
    return m_bStatic;
}

inline BodyCollisionGroup BodyNode::GetCollisionGroup() const {
    return m_iCollisionGroup;
}
inline Void BodyNode::SetCollisionGroup( BodyCollisionGroup iGroup ) {
    m_iCollisionGroup = iGroup;
}

inline BodyCollisionMask BodyNode::GetCollisionMask() const {
    return m_iCollisionMask;
}
inline Void BodyNode::SetCollisionMask( BodyCollisionMask iMask ) {
    m_iCollisionMask = iMask;
}

inline Bool BodyNode::IsSensor() const {
    return m_bSensor;
}
inline Void BodyNode::SetSensor( Bool bSensor ) {
    m_bSensor = bSensor;
}

inline Bool BodyNode::IsSleeping() const {
    return m_bSleeping;
}
inline Void BodyNode::SetSleeping( Bool bSleeping ) {
    m_bSleeping = bSleeping;
    if ( m_bSleeping ) {
        m_vLinearSpeed = Vector3::Null;
        m_vAngularSpeed = Vector3::Null;
    }
}

inline UInt BodyNode::GetSleepTime() const {
    return m_iSleepTime;
}
inline Void BodyNode::IncrementSleepTime() {
    ++m_iSleepTime;
}
inline Void BodyNode::ResetSleepTime() {
    m_iSleepTime = 0;
}

inline Shape * BodyNode::GetLocalShape() const {
    return m_pLocalShape;
}
inline TransformedShape * BodyNode::GetWorldShape() const {
    return m_pWorldShape;
}

inline Scalar BodyNode::GetDensity() const {
    return m_fDensity;
}

inline const Vertex3 & BodyNode::GetCenterOfMass() const {
    return m_vCenterOfMass;
}

inline Scalar BodyNode::GetMass() const {
    if ( IsInsulator() || IsFixed() )
        return SCALAR_INFINITE;
    return m_fMass;
}
inline Scalar BodyNode::GetInvMass() const {
    if ( IsInsulator() || IsFixed() )
        return 0.0f;
    return m_fInvMass;
}

inline const Matrix3 & BodyNode::GetInertiaTensor() const {
    if ( IsInsulator() || IsFixed() )
        return sm_matInfinity;
    return m_matInertiaTensor;
}
inline const Matrix3 & BodyNode::GetInvInertiaTensor() const {
    if ( IsInsulator() || IsFixed() )
        return Matrix3::Null;
    return m_matInvInertiaTensor;
}

inline Scalar BodyNode::GetRestitution() const {
    return m_fRestitution;
}
inline Void BodyNode::SetRestitution( Scalar fRestitution ) {
    m_fRestitution = fRestitution;
}

inline Scalar BodyNode::GetFriction() const {
    return m_fFriction;
}
inline Void BodyNode::SetFriction( Scalar fFriction ) {
    m_fFriction = fFriction;
}

inline Scalar BodyNode::GetRollingFriction() const {
    return m_fRollingFriction;
}
inline Void BodyNode::SetRollingFriction( Scalar fRollingFriction ) {
    m_fRollingFriction = fRollingFriction;
}

inline Scalar BodyNode::GetLinearDamping() const {
    return m_fLinearDamping;
}
inline Void BodyNode::SetLinearDamping( Scalar fDamping ) {
    m_fLinearDamping = fDamping;
}

inline Scalar BodyNode::GetAngularDamping() const {
    return m_fAngularDamping;
}
inline Void BodyNode::SetAngularDamping( Scalar fDamping ) {
    m_fAngularDamping = fDamping;
}

inline const Vector3 & BodyNode::GetForce() const {
    return m_vForceBuffer;
}
inline const Vector3 & BodyNode::GetTorque() const {
    return m_vTorqueBuffer;
}

inline Void BodyNode::ResetForce() {
    m_vForceBuffer = Vector3::Null;
}
inline Void BodyNode::ResetTorque() {
    m_vTorqueBuffer = Vector3::Null;
}

inline Void BodyNode::ApplyForce( const Vector3 & vForce ) {
    m_bSleeping = false;
    m_vForceBuffer += vForce;
}
inline Void BodyNode::ApplyTorque( const Vector3 & vTorque ) {
    m_bSleeping = false;
    m_vTorqueBuffer += vTorque;
}
inline Void BodyNode::ApplyForce( const Vector3 & vForce, const Vertex3 & vWorldPoint ) {
    m_bSleeping = false;
    m_vForceBuffer += vForce;
    m_vTorqueBuffer += ( (vWorldPoint - m_vPosition) ^ vForce );
}
inline Void BodyNode::ApplyForce( const Vector3 & vForce, const Vector3 & vLocalPoint ) {
    m_bSleeping = false;
    m_vForceBuffer += vForce;
    m_vTorqueBuffer += ( vLocalPoint ^ vForce );
}

inline const Vector3 & BodyNode::GetLinearImpulse() const {
    return m_vLinearImpulseBuffer;
}
inline const Vector3 & BodyNode::GetAngularImpulse() const {
    return m_vAngularImpulseBuffer;
}

inline Void BodyNode::ResetLinearImpulse() {
    m_vLinearImpulseBuffer = Vector3::Null;
}
inline Void BodyNode::ResetAngularImpulse() {
    m_vAngularImpulseBuffer = Vector3::Null;
}

inline Void BodyNode::ApplyLinearImpulse( const Vector3 & vImpulse ) {
    if ( IsInsulator() || IsFixed() )
        return;
    m_bSleeping = false;
    m_vLinearSpeed += ( vImpulse * m_fInvMass );
}
inline Void BodyNode::ApplyAngularImpulse( const Vector3 & vImpulse ) {
    if ( IsInsulator() || IsFixed() )
        return;
    m_bSleeping = false;
    m_vAngularSpeed += ( m_matInvInertiaTensor * vImpulse );
}
inline Void BodyNode::ApplyImpulse( const Vector3 & vImpulse, const Vertex3 & vWorldPoint ) {
    if ( IsInsulator() || IsFixed() )
        return;
    m_bSleeping = false;
    m_vLinearSpeed += ( vImpulse * m_fInvMass );
    m_vAngularSpeed += ( m_matInvInertiaTensor * ((vWorldPoint - m_vPosition) ^ vImpulse) );
}
inline Void BodyNode::ApplyImpulse( const Vector3 & vImpulse, const Vector3 & vLocalPoint ) {
    if ( IsInsulator() || IsFixed() )
        return;
    m_bSleeping = false;
    m_vLinearSpeed += ( vImpulse * m_fInvMass );
    m_vAngularSpeed += ( m_matInvInertiaTensor * (vLocalPoint ^ vImpulse) );
}

inline Void BodyNode::AccumulateLinearImpulse( const Vector3 & vImpulse ) {
    m_vLinearImpulseBuffer += ( vImpulse * m_fInvMass );
}
inline Void BodyNode::AccumulateAngularImpulse( const Vector3 & vImpulse ) {
    m_vAngularImpulseBuffer += ( m_matInvInertiaTensor * vImpulse );
}
inline Void BodyNode::AccumulateImpulse( const Vector3 & vImpulse, const Vertex3 & vWorldPoint ) {
    m_vLinearImpulseBuffer += ( vImpulse * m_fInvMass );
    m_vAngularImpulseBuffer += ( m_matInvInertiaTensor * ((vWorldPoint - m_vPosition) ^ vImpulse) );
}
inline Void BodyNode::AccumulateImpulse( const Vector3 & vImpulse, const Vector3 & vLocalPoint ) {
    m_vLinearImpulseBuffer += ( vImpulse * m_fInvMass );
    m_vAngularImpulseBuffer += ( m_matInvInertiaTensor * (vLocalPoint ^ vImpulse) );
}
inline Void BodyNode::AccumulateImpulse( const Vector3 & vLinearAxis, const Vector3 & vAngularAxis, Scalar fImpulse ) {
    m_vLinearImpulseBuffer += ( vLinearAxis * fImpulse );
    m_vAngularImpulseBuffer += ( vAngularAxis * fImpulse );
}
inline Void BodyNode::ApplyAccumulatedImpulses() {
    if ( IsInsulator() || IsFixed() )
        return;
    m_bSleeping = false;
    m_vLinearSpeed += m_vLinearImpulseBuffer;
    m_vAngularSpeed += m_vAngularImpulseBuffer;
}

inline const Vector3 & BodyNode::GetLinearCorrection() const {
    return m_vLinearCorrectionBuffer;
}
inline const Vector3 & BodyNode::GetAngularCorrection() const {
    return m_vAngularCorrectionBuffer;
}

inline Void BodyNode::ResetLinearCorrection() {
    m_vLinearCorrectionBuffer = Vector3::Null;
}
inline Void BodyNode::ResetAngularCorrection() {
    m_vAngularCorrectionBuffer = Vector3::Null;
}

inline Void BodyNode::ApplyLinearCorrection( const Vector3 & vCorrection, Scalar fTimeStep ) {
    if ( IsInsulator() || IsFixed() )
        return;
    m_bSleeping = false;

    m_vPosition += ( vCorrection * (m_fInvMass * fTimeStep) );
}
inline Void BodyNode::ApplyAngularCorrection( const Vector3 & vCorrection, Scalar fTimeStep ) {
    if ( IsInsulator() || IsFixed() )
        return;
    m_bSleeping = false;

    Vector3 vAngularCorrection = ( m_matInvInertiaTensor * vCorrection );
    Scalar fAngle = vAngularCorrection.Normalize();
    Quaternion qRotation( vAngularCorrection, fAngle * fTimeStep );
    m_qOrientation = ( qRotation * m_qOrientation );
    m_qOrientation.Normalize();
}
inline Void BodyNode::ApplyCorrection( const Vector3 & vCorrection, const Vertex3 & vWorldPoint, Scalar fTimeStep ) {
    if ( IsInsulator() || IsFixed() )
        return;
    m_bSleeping = false;

    m_vPosition += ( vCorrection * (m_fInvMass * fTimeStep) );

    Vector3 vAngularCorrection = ( m_matInvInertiaTensor * ((vWorldPoint - m_vPosition) ^ vCorrection) );
    Scalar fAngle = vAngularCorrection.Normalize();
    Quaternion qRotation( vAngularCorrection, fAngle * fTimeStep );
    m_qOrientation = ( qRotation * m_qOrientation );
    m_qOrientation.Normalize();
}
inline Void BodyNode::ApplyCorrection( const Vector3 & vCorrection, const Vector3 & vLocalPoint, Scalar fTimeStep ) {
    if ( IsInsulator() || IsFixed() )
        return;
    m_bSleeping = false;

    m_vPosition += ( vCorrection * (m_fInvMass * fTimeStep) );

    Vector3 vAngularCorrection = ( m_matInvInertiaTensor * (vLocalPoint ^ vCorrection) );
    Scalar fAngle = vAngularCorrection.Normalize();
    Quaternion qRotation( vAngularCorrection, fAngle * fTimeStep );
    m_qOrientation = ( qRotation * m_qOrientation );
    m_qOrientation.Normalize();
}

inline Void BodyNode::AccumulateLinearCorrection( const Vector3 & vCorrection ) {
    m_vLinearCorrectionBuffer += ( vCorrection * m_fInvMass );
}
inline Void BodyNode::AccumulateAngularCorrection( const Vector3 & vCorrection ) {
    m_vAngularCorrectionBuffer += ( m_matInvInertiaTensor * vCorrection );
}
inline Void BodyNode::AccumulateCorrection( const Vector3 & vCorrection, const Vertex3 & vWorldPoint ) {
    m_vLinearCorrectionBuffer += ( vCorrection * m_fInvMass );
    m_vAngularCorrectionBuffer += ( m_matInvInertiaTensor * ((vWorldPoint - m_vPosition) ^ vCorrection) );
}
inline Void BodyNode::AccumulateCorrection( const Vector3 & vCorrection, const Vector3 & vLocalPoint ) {
    m_vLinearCorrectionBuffer += ( vCorrection * m_fInvMass );
    m_vAngularCorrectionBuffer += ( m_matInvInertiaTensor * (vLocalPoint ^ vCorrection) );
}
inline Void BodyNode::AccumulateCorrection( const Vector3 & vLinearAxis, const Vector3 & vAngularAxis, Scalar fCorrection ) {
    m_vLinearCorrectionBuffer += ( vLinearAxis * fCorrection );
    m_vAngularCorrectionBuffer += ( vAngularAxis * fCorrection );
}
inline Void BodyNode::ApplyAccumulatedCorrections( Scalar fTimeStep ) {
    if ( IsInsulator() || IsFixed() )
        return;
    m_bSleeping = false;

    m_vPosition += ( m_vLinearCorrectionBuffer * fTimeStep );

    Vector3 vAxis = m_vAngularCorrectionBuffer;
    Scalar fAngle = vAxis.Normalize();
    Quaternion qRotation( vAxis, fAngle * fTimeStep );
    m_qOrientation = ( qRotation * m_qOrientation );
    m_qOrientation.Normalize();
}

inline const Vertex3 & BodyNode::GetPosition() const {
    return m_vPosition;
}
inline const Quaternion & BodyNode::GetOrientation() const {
    return m_qOrientation;
}
inline const Transform3 & BodyNode::GetTransform() const {
    return m_vTransform;
}

inline const Vector3 & BodyNode::GetLinearSpeed() const {
    if ( IsFixed() )
        return Vector3::Null;
    return m_vLinearSpeed;
}
inline const Vector3 & BodyNode::GetAngularSpeed() const {
    if ( IsFixed() )
        return Vector3::Null;
    return m_vAngularSpeed;
}

inline Vector3 BodyNode::GetLinearSpeedAt( const Vertex3 & vWorldPoint ) const {
    if ( IsFixed() )
        return Vector3::Null;
    return m_vLinearSpeed + ( m_vAngularSpeed ^ (vWorldPoint - m_vPosition) );
}
inline Vector3 BodyNode::GetLinearSpeedAt( const Vector3 & vLocalPoint ) const {
    if ( IsFixed() )
        return Vector3::Null;
    return m_vLinearSpeed + ( m_vAngularSpeed ^ vLocalPoint );
}

inline Vector3 BodyNode::GetAngularSpeedAt( const Vertex3 & vWorldPoint ) const {
    if ( IsFixed() )
        return Vector3::Null;
    return ( m_vAngularSpeed ^ (vWorldPoint - m_vPosition) );
}
inline Vector3 BodyNode::GetAngularSpeedAt( const Vector3 & vLocalPoint ) const {
    if ( IsFixed() )
        return Vector3::Null;
    return ( m_vAngularSpeed ^ vLocalPoint );
}

