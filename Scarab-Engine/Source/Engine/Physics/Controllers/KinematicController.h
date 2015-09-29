/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Controllers/KinematicController.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : The Base-Class for all kinematic controllers
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
#ifndef SCARAB_ENGINE_PHYSICS_CONTROLLERS_KINEMATICCONTROLLER_H
#define SCARAB_ENGINE_PHYSICS_CONTROLLERS_KINEMATICCONTROLLER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Error/ErrorManager.h"

#include "../../../Lib/Math/Types/Vector/Vector3.h"
#include "../../../Lib/Math/Types/Vertex/Vertex3.h"
#include "../../../Lib/Math/Types/Quaternion/Quaternion.h"

#include "../Shape/Shape.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
    // Controller names
#define CONTROLLER_NAME_LENGTH 64

    // Controller types
enum ControllerType
{
    // Motion controllers
    CONTROLLER_MOTION_MANUAL = 0,
    CONTROLLER_MOTION_CHARACTER,
    CONTROLLER_MOTION_KEYFRAME,
    CONTROLLER_MOTION_PATH,
    CONTROLLER_MOTION_BLEND,
    CONTROLLER_MOTION_PARTICLE,
    CONTROLLER_MOTION_IKJOINT,

    // Shape controllers
    CONTROLLER_SHAPE_MORPH,
    CONTROLLER_SHAPE_BONE,

    CONTROLLER_COUNT
};

    // Controller behaviour
enum ControllerRepeat
{
    CONTROLLER_REPEAT_CLAMP = 0, // No repeat, clamp to time interval
    CONTROLLER_REPEAT_WRAP,      // Wrap around at end
    CONTROLLER_REPEAT_MIRROR     // Walk back & forth
};

    // Prototypes
class BodyNode;
class KinematicBody;

/////////////////////////////////////////////////////////////////////////////////
// The KinematicController class
class KinematicController
{
public:
    KinematicController( const GChar * strName );
    virtual ~KinematicController();

    // Type & Name
    virtual ControllerType GetType() const = 0;
    inline Bool IsMotionController() const;
    inline Bool IsShapeController() const;

    inline const GChar * GetName() const;

    // Local time access
    inline Scalar GetLocalTime() const;
    inline Void SetLocalTime( Scalar fLocalTime );

    // Control time access
    inline Scalar GetControlTime() const;

    // Target access
    inline KinematicBody * GetTarget() const;

    virtual Void SetTarget( KinematicBody * pTarget );

    // Update support
    Bool Update( Scalar fTimeStep );

    // Controller data
    Bool Enabled;     // Default = false
    Scalar MinTime;   // Default = 0.0f
    Scalar MaxTime;   // Default = 1.0f
    Scalar Phase;     // Default = 0.0f
    Scalar Frequency; // Default = 1.0f
    ControllerRepeat Repeat; // Default = CONTROLLER_REPEAT_CLAMP

protected:
    // Control time helper
    Scalar _ComputeControlTime() const;

    // Update support
    virtual Bool _UpdateControllerState( Scalar fTimeStep ) = 0;

    // Name
    GChar m_strName[CONTROLLER_NAME_LENGTH];

    // Local time
    Scalar m_fLocalTime;
    Scalar m_fControlTime;

    // Target
    KinematicBody * m_pTarget;
};

/////////////////////////////////////////////////////////////////////////////////
// The MotionController class
class MotionController : public KinematicController
{
public:
    MotionController( const GChar * strName );
    virtual ~MotionController();

    // Position control access
    inline const Vertex3 & GetPosition() const;
    inline const Quaternion & GetOrientation() const;

    // Velocity control access
    inline const Vector3 & GetLinearSpeed() const;
    inline const Vector3 & GetAngularSpeed() const;

    // Impulse access
    inline virtual Void ApplyLinearImpulse( const Vector3 & vImpulse );
    inline virtual Void ApplyAngularImpulse( const Vector3 & vImpulse );

    // Correction access
    inline virtual Void ApplyLinearCorrection( const Vector3 & vCorrection );
    inline virtual Void ApplyAngularCorrection( const Quaternion & qCorrection );

protected:
    // Motion state
    Vertex3 m_vPosition;
    Quaternion m_qOrientation;

    Vector3 m_vLinearSpeed;
    Vector3 m_vAngularSpeed;
};

/////////////////////////////////////////////////////////////////////////////////
// The ShapeController class
class ShapeController : public KinematicController
{
public:
    ShapeController( const GChar * strName );
    virtual ~ShapeController();

protected:
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "KinematicController.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_CONTROLLERS_KINEMATICCONTROLLER_H

