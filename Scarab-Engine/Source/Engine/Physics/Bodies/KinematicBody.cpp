/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Bodies/KinematicBody.cpp
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
// Includes
#include "KinematicBody.h"

/////////////////////////////////////////////////////////////////////////////////
// KinematicBody implementation
KinematicBody::KinematicBody( Bool bStatic, Shape * pLocalShape, Scalar fDensity,
                              const Vertex3 & vPosition, const Quaternion & qOrientation,
                              const Vector3 & vLinearSpeed, const Vector3 & vAngularSpeed ):
    BodyNode( bStatic, pLocalShape, fDensity, vPosition, qOrientation, vLinearSpeed, vAngularSpeed )
{
    // Kinematic controllers
    m_bControllerShapeUpdate = false;
    m_pShapeController = NULL;

    m_bControllerMotionUpdate = false;
    m_pMotionController = NULL;
}
KinematicBody::~KinematicBody()
{
    // nothing to do
}

//Bool KinematicBody::IntegrateShape( Scalar fTimeStep )
//{
//    m_bControllerShapeUpdate = m_pShapeController->Update( fTimeStep );
//
//    return m_bControllerShapeUpdate;
//}
Void KinematicBody::IntegrateVelocity( Scalar fTimeStep )
{
    if ( IsFixed() )
        return;
    if ( m_pMotionController == NULL )
        return;

    m_bControllerMotionUpdate = m_pMotionController->Update( fTimeStep );
    if ( !m_bControllerMotionUpdate )
        return;
    m_bSleeping = false;

    m_vLinearSpeed = m_pMotionController->GetLinearSpeed();
    m_vAngularSpeed = m_pMotionController->GetAngularSpeed();

    // Apply damping
    m_vLinearSpeed *= Clamp<Scalar>( 1.0f - (fTimeStep * m_fLinearDamping), 0.0f, 1.0f );
    m_vAngularSpeed *= Clamp<Scalar>( 1.0f - (fTimeStep * m_fAngularDamping), 0.0f, 1.0f );
}
Void KinematicBody::IntegratePosition( Scalar /*fTimeStep*/ )
{
    if ( IsFixed() )
        return;
    if ( m_pMotionController == NULL )
        return;

    if ( !m_bControllerMotionUpdate )
        return;
    m_bSleeping = false;

    m_vPosition = m_pMotionController->GetPosition();
    m_qOrientation = m_pMotionController->GetOrientation();

    // Update transform
    Matrix3 matOrient;
    m_qOrientation.GetMatrix( matOrient );
    m_vTransform.SetRotate( matOrient );
    m_vTransform.SetUniformScale( 1.0f );
    m_vTransform.SetTranslate( m_vPosition - m_vCenterOfMass );

    // Update world shape
    m_pWorldShape->OnTransformUpdate( m_vTransform );

    // Done updating
    m_bControllerMotionUpdate = false;
}

