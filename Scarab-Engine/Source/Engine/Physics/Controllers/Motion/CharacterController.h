/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Controllers/Motion/CharacterController.h
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
// Header prelude
#ifndef SCARAB_ENGINE_PHYSICS_CONTROLLERS_MOTION_CHARACTERCONTROLLER_H
#define SCARAB_ENGINE_PHYSICS_CONTROLLERS_MOTION_CHARACTERCONTROLLER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "ManualController.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The CharacterController class
class CharacterController : public ManualController
{
public:
    CharacterController( const GChar * strName, const Vertex3 & vPosition, const Quaternion & qOrientation,
                                                const Vector3 & vLinearSpeed, const Vector3 & vAngularSpeed );
    virtual ~CharacterController();

    // Type
    inline virtual ControllerType GetType() const;

    // Forces control
    inline Void EnableForces( Bool bEnable );

    // Movement methods
    inline Bool IsMovingForward() const;
    inline Bool IsMovingBackward() const;
    inline Bool IsMovingLeft() const;
    inline Bool IsMovingRight() const;
    inline Bool IsMovingUp() const;
    inline Bool IsMovingDown() const;
    inline Bool IsMoving() const;

    inline Void SetMovingForward( Bool bMoving );
    inline Void SetMovingBackward( Bool bMoving );
    inline Void SetMovingLeft( Bool bMoving );
    inline Void SetMovingRight( Bool bMoving );
    inline Void SetMovingUp( Bool bMoving );
    inline Void SetMovingDown( Bool bMoving );
    inline Void SetStanding();

    inline const Vector3 & GetMovementDirection() const;
    inline Scalar GetMovementSpeed() const;
    inline Void SetMovementSpeed( Scalar fSpeed );

    inline Void Teleport( const Vertex3 & vPosition, Bool bKeepInertia = false );
    
    //Void Jump( Scalar fHeight );
    //Void InterruptJump();

protected:
    // Update support
    virtual Bool _UpdateControllerState( Scalar fTimeStep );

    // Forces control
    Bool m_bApplyForces;

    // Movement control
    Bool m_bMovingForward, m_bMovingBackward;
    Bool m_bMovingLeft, m_bMovingRight;
    Bool m_bMovingUp, m_bMovingDown;

    Vector3 m_vMovementDirection;
    Scalar m_fMovementSpeed;

    // Jump control
    //Bool m_bJumping;
    //Vector3 m_vJumpImpulse;
    //Scalar m_fJumpHeight;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "CharacterController.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_CONTROLLERS_MOTION_CHARACTERCONTROLLER_H

