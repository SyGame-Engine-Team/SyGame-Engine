/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Bodies/RigidBody.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Body : Rigid bodies
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
#include "RigidBody.h"

/////////////////////////////////////////////////////////////////////////////////
// RigidBody implementation
RigidBody::RigidBody( Bool bStatic, Shape * pLocalShape, Scalar fDensity,
                      const Vertex3 & vPosition, const Quaternion & qOrientation,
                      const Vector3 & vLinearSpeed, const Vector3 & vAngularSpeed ):
    BodyNode( bStatic, pLocalShape, fDensity, vPosition, qOrientation, vLinearSpeed, vAngularSpeed )
{
    // Ensure null velocity for statics
    if ( m_bStatic ) {
        m_vLinearSpeed = Vector3::Null;
        m_vAngularSpeed = Vector3::Null;
    }
}
RigidBody::~RigidBody()
{
    // nothing to do
}

Void RigidBody::IntegrateVelocity( Scalar fTimeStep )
{
    if ( IsInsulator() || IsFixed() || m_bSleeping )
        return;

    // Integration
    m_vLinearSpeed += ( m_vForceBuffer * (m_fInvMass * fTimeStep) );

    m_vAngularSpeed += ( m_matInvInertiaTensor * (m_vTorqueBuffer * fTimeStep) );

    // Apply damping
    m_vLinearSpeed *= Clamp<Scalar>( 1.0f - (fTimeStep * m_fLinearDamping), 0.0f, 1.0f );
    m_vAngularSpeed *= Clamp<Scalar>( 1.0f - (fTimeStep * m_fAngularDamping), 0.0f, 1.0f );
}
Void RigidBody::IntegratePosition( Scalar fTimeStep )
{
    if ( IsInsulator() || IsFixed() || m_bSleeping )
        return;

    // Integration
    m_vPosition += ( m_vLinearSpeed * fTimeStep );

    Vector3 vAxis = m_vAngularSpeed;
    Scalar fAngle = vAxis.Normalize();
    Quaternion qDelta( vAxis, fAngle * fTimeStep );
    m_qOrientation = ( qDelta * m_qOrientation );
    m_qOrientation.Normalize();

    // Update transform
    Matrix3 matOrient;
    m_qOrientation.GetMatrix( matOrient );
    m_vTransform.SetRotate( matOrient );
    m_vTransform.SetUniformScale( 1.0f );
    m_vTransform.SetTranslate( m_vPosition - m_vCenterOfMass );

    // Update world shape
    m_pWorldShape->OnTransformUpdate( m_vTransform );
}

