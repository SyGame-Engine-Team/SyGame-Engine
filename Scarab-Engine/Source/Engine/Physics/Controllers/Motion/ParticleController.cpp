/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Controllers/Motion/ParticleController.cpp
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
// Includes
#include "ParticleController.h"

#include "../../PhysicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// ParticleController implementation
ParticleController::ParticleController( const GChar * strName, const Vertex3 & vPosition, const Quaternion & qOrientation,
                                                               const Vector3 & vLinearSpeed, const Vector3 & vAngularSpeed ):
    MotionController( strName ),
    m_vForce( Vector3::Null ), m_vTorque( Vector3::Null ),
    m_vOldPosition( vPosition ), m_qOldOrientation( qOrientation )
{
    m_vPosition = vPosition;
    m_qOrientation = qOrientation;
    m_vLinearSpeed = vLinearSpeed;
    m_vAngularSpeed = vAngularSpeed;

    UseIntegration = false;
    UpdateVelocity = true;
}
ParticleController::~ParticleController()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

Bool ParticleController::_UpdateControllerState( Scalar fTimeStep )
{
    if ( UseIntegration ) {
        // Use dynamic motion
        if ( UpdateVelocity ) {
            m_vLinearSpeed += ( m_vForce * (m_pTarget->GetInvMass() * fTimeStep) );
            m_vAngularSpeed += ( m_pTarget->GetInvInertiaTensor() * (m_vTorque * fTimeStep) );

            m_vLinearSpeed *= Clamp<Scalar>( 1.0f - (fTimeStep * m_pTarget->GetLinearDamping()), 0.0f, 1.0f );
            m_vAngularSpeed *= Clamp<Scalar>( 1.0f - (fTimeStep * m_pTarget->GetAngularDamping()), 0.0f, 1.0f );
        }

        m_vPosition += ( m_vLinearSpeed * fTimeStep );

        Vector3 vAxis = m_vAngularSpeed;
        Scalar fAngle = vAxis.Normalize();
        Quaternion qDelta( vAxis, fAngle * fTimeStep );
        m_qOrientation = ( qDelta * m_qOrientation );
        m_qOrientation.Normalize();
    } else {
        // Use kinematic motion
        if ( UpdateVelocity ) {
            Scalar fInvTimeStep = MathFn->Invert( fTimeStep );

            m_vLinearSpeed = ( m_vOldPosition - m_vPosition ) * fInvTimeStep;

            Quaternion qRotation;
            m_qOldOrientation.Invert( qRotation );
            qRotation = m_qOrientation * qRotation;
            Scalar fAngle;
            qRotation.GetAxisAngle( m_vAngularSpeed, fAngle );
            m_vAngularSpeed *= ( fAngle * fInvTimeStep );
        }

        m_vOldPosition = m_vPosition;
        m_qOldOrientation = m_qOrientation;
    }

    // Done
    return true;
}

