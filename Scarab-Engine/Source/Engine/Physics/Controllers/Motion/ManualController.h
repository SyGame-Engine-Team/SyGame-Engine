/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Controllers/Motion/ManualController.h
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
// Header prelude
#ifndef SCARAB_ENGINE_PHYSICS_CONTROLLERS_MOTION_MANUALCONTROLLER_H
#define SCARAB_ENGINE_PHYSICS_CONTROLLERS_MOTION_MANUALCONTROLLER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../KinematicController.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The ManualController class
class ManualController : public MotionController
{
public:
    ManualController( const GChar * strName, const Vertex3 & vPosition, const Quaternion & qOrientation,
                                             const Vector3 & vLinearSpeed, const Vector3 & vAngularSpeed );
    virtual ~ManualController();

    // Type
    inline virtual ControllerType GetType() const;

    // Impulse access
    inline virtual Void ApplyLinearImpulse( const Vector3 & vImpulse );
    inline virtual Void ApplyAngularImpulse( const Vector3 & vImpulse );

    // Correction access
    inline virtual Void ApplyLinearCorrection( const Vector3 & vCorrection );
    inline virtual Void ApplyAngularCorrection( const Quaternion & qCorrection );

    // Manual control
    inline Void Translate( Scalar fTimeStep );
    inline Void Translate( const Vector3 & vTranslation );
    inline Void SetPosition( const Vertex3 & vPosition );

    inline Void Rotate( Scalar fTimeStep );
    inline Void Rotate( const Quaternion & qOrientation );
    inline Void SetOrientation( const Quaternion & qOrientation );

    inline Void SetManualSpeed( const Vector3 & vLinearSpeed, const Vector3 & vAngularSpeed );

    // Movement bounding
    inline Void SetBounded( Bool bBounded, const Vertex3 & vMin = Vertex3::Null, const Vertex3 & vMax = Vertex3::Null );

protected:
    // Update support
    virtual Bool _UpdateControllerState( Scalar fTimeStep );

    Bool m_bManualUpdate;
    Bool m_bUseManualSpeed;
    Vector3 m_vManualLinearSpeed;
    Vector3 m_vManualAngularSpeed;
    Vertex3 m_vOldPosition;
    Quaternion m_qOldOrientation;

    Bool m_bBounded;
    Vertex3 m_vBoundMin, m_vBoundMax;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "ManualController.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_CONTROLLERS_MOTION_MANUALCONTROLLER_H

