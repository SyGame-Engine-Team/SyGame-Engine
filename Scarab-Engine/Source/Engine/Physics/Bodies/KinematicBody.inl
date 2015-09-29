/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Bodies/KinematicBody.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Body : Kinematic bodies
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// KinematicBody implementation
inline BodyType KinematicBody::GetBodyType() const {
    return BODY_TYPE_KINEMATIC;
}

inline Bool KinematicBody::IsInsulator() const {
    return m_bStatic;
}

inline Void KinematicBody::ApplyLinearImpulse( const Vector3 & vImpulse ) {
    if ( IsInsulator() || IsFixed() )
        return;
    m_bSleeping = false;
    if ( m_pMotionController == NULL )
        m_vLinearSpeed += ( vImpulse * m_fInvMass );
    else
        m_pMotionController->ApplyLinearImpulse( vImpulse * m_fInvMass );
}
inline Void KinematicBody::ApplyAngularImpulse( const Vector3 & vImpulse ) {
    if ( IsInsulator() || IsFixed() )
        return;
    m_bSleeping = false;
    if ( m_pMotionController == NULL )
        m_vAngularSpeed += ( m_matInvInertiaTensor * vImpulse );
    else
        m_pMotionController->ApplyAngularImpulse( m_matInvInertiaTensor * vImpulse );
}
inline Void KinematicBody::ApplyImpulse( const Vector3 & vImpulse, const Vertex3 & vWorldPoint ) {
    if ( IsInsulator() || IsFixed() )
        return;
    m_bSleeping = false;
    if ( m_pMotionController == NULL ) {
        m_vLinearSpeed += ( vImpulse * m_fInvMass );
        m_vAngularSpeed += ( m_matInvInertiaTensor * ((vWorldPoint - m_vPosition) ^ vImpulse) );
    } else {
        m_pMotionController->ApplyLinearImpulse( vImpulse * m_fInvMass );
        m_pMotionController->ApplyAngularImpulse( m_matInvInertiaTensor * ((vWorldPoint - m_vPosition) ^ vImpulse) );
    }
}
inline Void KinematicBody::ApplyImpulse( const Vector3 & vImpulse, const Vector3 & vLocalPoint ) {
    if ( IsInsulator() || IsFixed() )
        return;
    m_bSleeping = false;
    if ( m_pMotionController == NULL ) {
        m_vLinearSpeed += ( vImpulse * m_fInvMass );
        m_vAngularSpeed += ( m_matInvInertiaTensor * (vLocalPoint ^ vImpulse) );
    } else {
        m_pMotionController->ApplyLinearImpulse( vImpulse * m_fInvMass );
        m_pMotionController->ApplyAngularImpulse( m_matInvInertiaTensor * (vLocalPoint ^ vImpulse) );
    }
}

inline Void KinematicBody::ApplyAccumulatedImpulses() {
    if ( IsInsulator() || IsFixed() )
        return;
    m_bSleeping = false;
    if ( m_pMotionController == NULL ) {
        m_vLinearSpeed += m_vLinearImpulseBuffer;
        m_vAngularSpeed += m_vAngularImpulseBuffer;
    } else {
        m_pMotionController->ApplyLinearImpulse( m_vLinearImpulseBuffer );
        m_pMotionController->ApplyAngularImpulse( m_vAngularImpulseBuffer );
    }
}

inline Void KinematicBody::ApplyLinearCorrection( const Vector3 & vCorrection, Scalar fTimeStep ) {
    if ( IsInsulator() || IsFixed() )
        return;
    m_bSleeping = false;
    if ( m_pMotionController == NULL )
        m_vPosition += ( vCorrection * (m_fInvMass * fTimeStep) );
    else
        m_pMotionController->ApplyLinearCorrection( vCorrection * (m_fInvMass * fTimeStep) );
}
inline Void KinematicBody::ApplyAngularCorrection( const Vector3 & vCorrection, Scalar fTimeStep ) {
    if ( IsInsulator() || IsFixed() )
        return;
    m_bSleeping = false;
    Vector3 vAngularCorrection = ( m_matInvInertiaTensor * vCorrection );
    Quaternion qDelta( vAngularCorrection.X, vAngularCorrection.Y, vAngularCorrection.Z, 0.0f );
    if ( m_pMotionController == NULL )
        m_qOrientation += ( (qDelta * m_qOrientation) * (0.5f * fTimeStep) );
    else
        m_pMotionController->ApplyAngularCorrection( (qDelta * m_qOrientation) * (0.5f * fTimeStep) );
}
inline Void KinematicBody::ApplyCorrection( const Vector3 & vCorrection, const Vertex3 & vWorldPoint, Scalar fTimeStep ) {
    if ( IsInsulator() || IsFixed() )
        return;
    m_bSleeping = false;
    Vector3 vAngularCorrection = ( m_matInvInertiaTensor * ((vWorldPoint - m_vPosition) ^ vCorrection) );
    Quaternion qDelta( vAngularCorrection.X, vAngularCorrection.Y, vAngularCorrection.Z, 0.0f );
    if ( m_pMotionController == NULL ) {
        m_vPosition += ( vCorrection * (m_fInvMass * fTimeStep) );
        m_qOrientation += ( (qDelta * m_qOrientation) * (0.5f * fTimeStep) );
    } else {
        m_pMotionController->ApplyLinearCorrection( vCorrection * (m_fInvMass * fTimeStep) );
        m_pMotionController->ApplyAngularCorrection( (qDelta * m_qOrientation) * (0.5f * fTimeStep) );
    }
}
inline Void KinematicBody::ApplyCorrection( const Vector3 & vCorrection, const Vector3 & vLocalPoint, Scalar fTimeStep ) {
    if ( IsInsulator() || IsFixed() )
        return;
    m_bSleeping = false;
    Vector3 vAngularCorrection = ( m_matInvInertiaTensor * (vLocalPoint ^ vCorrection) );
    Quaternion qDelta( vAngularCorrection.X, vAngularCorrection.Y, vAngularCorrection.Z, 0.0f );
    if ( m_pMotionController == NULL ) {
        m_vPosition += ( vCorrection * (m_fInvMass * fTimeStep) );
        m_qOrientation += ( (qDelta * m_qOrientation) * (0.5f * fTimeStep) );
    } else {
        m_pMotionController->ApplyLinearCorrection( vCorrection * (m_fInvMass * fTimeStep) );
        m_pMotionController->ApplyAngularCorrection( (qDelta * m_qOrientation) * (0.5f * fTimeStep) );
    }
}

inline Void KinematicBody::ApplyAccumulatedCorrections( Scalar fTimeStep ) {
    if ( IsInsulator() || IsFixed() )
        return;
    m_bSleeping = false;
    Quaternion qDelta( m_vAngularCorrectionBuffer.X, m_vAngularCorrectionBuffer.Y, m_vAngularCorrectionBuffer.Z, 0.0f );
    if ( m_pMotionController == NULL ) {
        m_vPosition += ( m_vLinearCorrectionBuffer * fTimeStep );
        m_qOrientation += ( (qDelta * m_qOrientation) * (0.5f * fTimeStep) );
    } else {
        m_pMotionController->ApplyLinearCorrection( m_vLinearCorrectionBuffer * fTimeStep );
        m_pMotionController->ApplyAngularCorrection( (qDelta * m_qOrientation) * (0.5f * fTimeStep) );
    }
}

inline ShapeController * KinematicBody::GetShapeController() const {
    return m_pShapeController;
}
inline MotionController * KinematicBody::GetMotionController() const {
    return m_pMotionController;
}

inline Void KinematicBody::AttachController( KinematicController * pController ) {
    Assert( pController != NULL );
    if ( pController->IsShapeController() ) {
        if ( m_pShapeController != NULL )
            m_pShapeController->SetTarget( NULL );
        m_pShapeController = (ShapeController*)pController;
        pController->SetTarget( this );
    } else {
        Assert( pController->IsMotionController() );
        if ( m_pMotionController != NULL )
            m_pMotionController->SetTarget( NULL );
        m_pMotionController = (MotionController*)pController;
        m_pMotionController->SetTarget( this );
    }
}
inline ShapeController * KinematicBody::DetachShapeController() {
    if ( m_pShapeController != NULL )
        m_pShapeController->SetTarget( NULL );
    ShapeController * pController = m_pShapeController;
    m_pShapeController = NULL;
    return pController;
}
inline MotionController * KinematicBody::DetachMotionController() {
    if ( m_pMotionController != NULL )
        m_pMotionController->SetTarget( NULL );
    MotionController * pController = m_pMotionController;
    m_pMotionController = NULL;
    return pController;
}

