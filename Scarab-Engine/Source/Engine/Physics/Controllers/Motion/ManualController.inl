/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Controllers/Motion/ManualController.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Motion Controller : Manual movement
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// ManualController implementation
inline ControllerType ManualController::GetType() const {
    return CONTROLLER_MOTION_MANUAL;
}

inline Void ManualController::ApplyLinearImpulse( const Vector3 & vImpulse ) {
    m_bManualUpdate = true;
    if ( !m_bUseManualSpeed ) {
        m_vManualLinearSpeed = m_vLinearSpeed;
        m_vManualAngularSpeed = m_vAngularSpeed;
        m_bUseManualSpeed = true;
    }
    m_vManualLinearSpeed += vImpulse;
}
inline Void ManualController::ApplyAngularImpulse( const Vector3 & vImpulse ) {
    m_bManualUpdate = true;
    if ( !m_bUseManualSpeed ) {
        m_vManualLinearSpeed = m_vLinearSpeed;
        m_vManualAngularSpeed = m_vAngularSpeed;
        m_bUseManualSpeed = true;
    }
    m_vManualAngularSpeed += vImpulse;
}

inline Void ManualController::ApplyLinearCorrection( const Vector3 & vCorrection ) {
    m_bManualUpdate = true;
    if ( !m_bUseManualSpeed ) {
        m_vManualLinearSpeed = m_vLinearSpeed;
        m_vManualAngularSpeed = m_vAngularSpeed;
        m_bUseManualSpeed = true;
    }
    m_vPosition += vCorrection;
}
inline Void ManualController::ApplyAngularCorrection( const Quaternion & qCorrection ) {
    m_bManualUpdate = true;
    if ( !m_bUseManualSpeed ) {
        m_vManualLinearSpeed = m_vLinearSpeed;
        m_vManualAngularSpeed = m_vAngularSpeed;
        m_bUseManualSpeed = true;
    }
    m_qOrientation += qCorrection;
}

inline Void ManualController::Translate( Scalar fTimeStep ) {
    Assert( m_bUseManualSpeed );
    m_bManualUpdate = true;
    m_vPosition += ( m_vManualLinearSpeed * fTimeStep );
    if ( m_bBounded ) {
        m_vPosition.X = Clamp<Scalar>( m_vPosition.X, m_vBoundMin.X, m_vBoundMax.X );
        m_vPosition.Y = Clamp<Scalar>( m_vPosition.Y, m_vBoundMin.Y, m_vBoundMax.Y );
        m_vPosition.Z = Clamp<Scalar>( m_vPosition.Z, m_vBoundMin.Z, m_vBoundMax.Z );
    }
}
inline Void ManualController::Translate( const Vector3 & vTranslation ) {
    m_bManualUpdate = true;
    m_bUseManualSpeed = false;
    m_vPosition += vTranslation;
    if ( m_bBounded ) {
        m_vPosition.X = Clamp<Scalar>( m_vPosition.X, m_vBoundMin.X, m_vBoundMax.X );
        m_vPosition.Y = Clamp<Scalar>( m_vPosition.Y, m_vBoundMin.Y, m_vBoundMax.Y );
        m_vPosition.Z = Clamp<Scalar>( m_vPosition.Z, m_vBoundMin.Z, m_vBoundMax.Z );
    }
}
inline Void ManualController::SetPosition( const Vertex3 & vPosition ) {
    m_bManualUpdate = true;
    m_bUseManualSpeed = false;
    m_vPosition = vPosition;
    if ( m_bBounded ) {
        m_vPosition.X = Clamp<Scalar>( m_vPosition.X, m_vBoundMin.X, m_vBoundMax.X );
        m_vPosition.Y = Clamp<Scalar>( m_vPosition.Y, m_vBoundMin.Y, m_vBoundMax.Y );
        m_vPosition.Z = Clamp<Scalar>( m_vPosition.Z, m_vBoundMin.Z, m_vBoundMax.Z );
    }
}

inline Void ManualController::Rotate( Scalar fTimeStep ) {
    Assert( m_bUseManualSpeed );
    m_bManualUpdate = true;
    Quaternion qDelta( m_vManualAngularSpeed.X, m_vManualAngularSpeed.Y, m_vManualAngularSpeed.Z, 0.0f );
    m_qOrientation += (qDelta * m_qOrientation) * (0.5f * fTimeStep);
}
inline Void ManualController::Rotate( const Quaternion & qOrientation ) {
    m_bManualUpdate = true;
    m_bUseManualSpeed = false;
    m_qOrientation = ( qOrientation * m_qOrientation );
}
inline Void ManualController::SetOrientation( const Quaternion & qOrientation ) {
    m_bManualUpdate = true;
    m_bUseManualSpeed = false;
    m_qOrientation = qOrientation;
}

inline Void ManualController::SetManualSpeed( const Vector3 & vLinearSpeed, const Vector3 & vAngularSpeed  ) {
    m_bManualUpdate = true;
    m_bUseManualSpeed = true;
    m_vManualLinearSpeed = vLinearSpeed;
    m_vManualAngularSpeed = vAngularSpeed;
}

inline Void ManualController::SetBounded( Bool bBounded, const Vertex3 & vMin, const Vertex3 & vMax ) {
    m_bBounded = bBounded;
    if ( m_bBounded ) {
        Assert( vMin.X < vMax.X && vMin.Y < vMax.Y && vMin.Z < vMax.Z );
        m_vBoundMin = vMin;
        m_vBoundMax = vMax;
    } else {
        m_vBoundMin = Vertex3::Null;
        m_vBoundMax = Vertex3::Null;
    }
}

