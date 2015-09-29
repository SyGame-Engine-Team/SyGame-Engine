/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Controllers/Motion/ParticleController.h
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
// Header prelude
#ifndef SCARAB_ENGINE_PHYSICS_CONTROLLERS_MOTION_PARTICLECONTROLLER_H
#define SCARAB_ENGINE_PHYSICS_CONTROLLERS_MOTION_PARTICLECONTROLLER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../KinematicController.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The ParticleController class
class ParticleController : public MotionController
{
public:
    ParticleController( const GChar * strName, const Vertex3 & vPosition, const Quaternion & qOrientation,
                                               const Vector3 & vLinearSpeed, const Vector3 & vAngularSpeed );
    virtual ~ParticleController();

    // Type
    inline virtual ControllerType GetType() const;

    // Target access
    //virtual Void SetTarget( KinematicBody * pTarget );

    // Dynamic control
    inline const Vector3 & GetForce() const;
    inline const Vector3 & GetTorque() const;

    inline Void ResetForce();
    inline Void ResetTorque();

    inline Void AddForce( const Vector3 & vForce );
    inline Void AddTorque( const Vector3 & vTorque );

    inline Void SetForce( const Vector3 & vForce );
    inline Void SetTorque( const Vector3 & vTorque );

    inline Void ApplyLinearImpulse( const Vector3 & vLinearImpulse );
    inline Void ApplyAngularImpulse( const Vector3 & vAngularImpulse );

    inline Void ApplyLinearCorrection( const Vector3 & vLinearCorrection );
    inline Void ApplyAngularCorrection( const Quaternion & qAngularCorrection );

    // Kinematic control
    inline Void SetLinearSpeed( const Vector3 & vLinearSpeed );
    inline Void SetAngularSpeed( const Vector3 & vAngularSpeed );

    inline Void SetPosition( const Vertex3 & vPosition );
    inline Void SetOrientation( const Quaternion & qOrientation );

    // Controller data
    Bool UseIntegration; // Default = false
    Bool UpdateVelocity; // Default = true

protected:
    // Update support
    virtual Bool _UpdateControllerState( Scalar fTimeStep );

    // Dynamic control
    Vector3 m_vForce;
    Vector3 m_vTorque;

    // Kinematic control
    Vertex3 m_vOldPosition;
    Quaternion m_qOldOrientation;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ParticleController.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_CONTROLLERS_MOTION_PARTICLECONTROLLER_H

