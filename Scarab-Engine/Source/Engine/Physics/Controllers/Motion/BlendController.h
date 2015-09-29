/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Controllers/Motion/BlendController.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Motion Controller : Animation blending
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
#ifndef SCARAB_ENGINE_PHYSICS_CONTROLLERS_MOTION_BLENDCONTROLLER_H
#define SCARAB_ENGINE_PHYSICS_CONTROLLERS_MOTION_BLENDCONTROLLER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../KinematicController.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The BlendController class
class BlendController : public MotionController
{
public:
    BlendController( const GChar * strName, MotionController * pSource, MotionController * pDestination,
                                            Bool bGeometricBlend = true );
    virtual ~BlendController();

    // Type
    inline virtual ControllerType GetType() const;

    // Target access
    virtual Void SetTarget( KinematicBody * pTarget );

    // Blending access
    inline MotionController * GetSource() const;
    inline MotionController * GetDestination() const;

    inline Bool IsGeometricBlend() const;

    Scalar StartTime; // Default = 1.0f, source's control time when to start blending
    Scalar StopTime;  // Default = 0.0f, destination's control time when to stop blending

protected:
    // Update support
    virtual Bool _UpdateControllerState( Scalar fTimeStep );

    MotionController * m_pSource;
    MotionController * m_pDestination;

    Bool m_bGeometricBlend;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "BlendController.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_CONTROLLERS_MOTION_BLENDCONTROLLER_H

