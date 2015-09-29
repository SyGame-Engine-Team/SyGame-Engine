/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Controllers/Motion/CharacterController.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Motion Controller : Character movement
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
#include "CharacterController.h"

#include "../../Bodies/KinematicBody.h"
#include "../../../Rendering/RenderingManager.h"

/////////////////////////////////////////////////////////////////////////////////
// CharacterController implementation
CharacterController::CharacterController( const GChar * strName, const Vertex3 & vPosition, const Quaternion & qOrientation,
                                                                 const Vector3 & vLinearSpeed, const Vector3 & vAngularSpeed ):
    ManualController( strName, vPosition, qOrientation, vLinearSpeed, vAngularSpeed ),
    m_vMovementDirection()
{
    m_bApplyForces = false;

    m_bMovingForward = false;
    m_bMovingBackward = false;
    m_bMovingLeft = false;
    m_bMovingRight = false;
    m_bMovingUp = false;
    m_bMovingDown = false;

    m_vMovementDirection = Vector3::Null;
    m_fMovementSpeed = 1.0f;
}
CharacterController::~CharacterController()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

Bool CharacterController::_UpdateControllerState( Scalar fTimeStep )
{
    // Compute movement frame from camera
    const Camera * pRenderCamera = RenderingFn->GetCamera();

    Vector3 vAim = pRenderCamera->GetViewDir();
    vAim.Z = 0.0f;
    vAim.Normalize();
    Vector3 vSide = pRenderCamera->GetViewSide();
    Vector3 vUp = ( vAim ^ vSide );
    vUp.Normalize();

    // Compute moving speed
    m_vMovementDirection = Vector3::Null;
    Bool bMoving = false;

    if ( m_bMovingForward ) {
        m_vMovementDirection += vAim;
        bMoving = true;
    } else if ( m_bMovingBackward ) {
        m_vMovementDirection += -vAim;
        bMoving = true;
    }
    if ( m_bMovingLeft ) {
        m_vMovementDirection += vSide;
        bMoving = true;
    } else if ( m_bMovingRight ) {
        m_vMovementDirection += -vSide;
        bMoving = true;
    }
    if ( m_bMovingUp ) {
        m_vMovementDirection += vUp;
        bMoving = true;
    } else if ( m_bMovingDown ) {
        m_vMovementDirection += -vUp;
        bMoving = true;
    }
    if ( bMoving )
        m_vMovementDirection.Normalize();

    // Update moving speed
    Vector3 vLinearSpeed = ( m_vMovementDirection * m_fMovementSpeed );
    Vector3 vAngularSpeed = Vector3::Null;
    if ( m_bApplyForces ) {
        if ( !(m_pTarget->IsInsulator()) && !(m_pTarget->IsFixed()) && !(m_pTarget->IsSleeping()) ) {
            vLinearSpeed += ( m_pTarget->GetForce() * (m_pTarget->GetInvMass() * fTimeStep) );
            vAngularSpeed += ( m_pTarget->GetInvInertiaTensor() * (m_pTarget->GetTorque() * fTimeStep) );
        }
    }
    SetManualSpeed( vLinearSpeed, vAngularSpeed );

    // Process as manual controller
    return ManualController::_UpdateControllerState( fTimeStep );
}

