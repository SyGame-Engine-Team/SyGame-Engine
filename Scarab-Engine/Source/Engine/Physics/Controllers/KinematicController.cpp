/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Controllers/KinematicController.cpp
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
// Includes
#include "KinematicController.h"

#include "../Bodies/KinematicBody.h"

/////////////////////////////////////////////////////////////////////////////////
// KinematicController implementation
KinematicController::KinematicController( const GChar * strName )
{
    Assert( strName != NULL );
    StringFn->NCopy( m_strName, strName, CONTROLLER_NAME_LENGTH - 1 );

    m_fLocalTime = 0.0f;
    m_fControlTime = 0.0f;

    m_pTarget = NULL;

    Enabled = false;
    MinTime = 0.0f;
    MaxTime = 1.0f;
    Phase = 0.0f;
    Frequency = 1.0f;
    Repeat = CONTROLLER_REPEAT_CLAMP;
}
KinematicController::~KinematicController()
{
    // nothing to do
}

Void KinematicController::SetTarget( KinematicBody * pTarget )
{
    // Default : nothing to do

    m_pTarget = pTarget;
}

Bool KinematicController::Update( Scalar fTimeStep )
{
    if ( Enabled ) {
        Assert( m_pTarget != NULL );
        Assert( MinTime <= MaxTime );
        m_fLocalTime += fTimeStep;
        m_fControlTime = _ComputeControlTime();
        return _UpdateControllerState( fTimeStep );
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////////

Scalar KinematicController::_ComputeControlTime() const
{
    Scalar fControlTime = (m_fLocalTime * Frequency) + Phase;

    // Clamp case
    if ( Repeat == CONTROLLER_REPEAT_CLAMP ) {
        if ( fControlTime < MinTime )
            return MinTime;
        if ( fControlTime > MaxTime )
            return MaxTime;
        return fControlTime;
    }

    // Get time interval
    Scalar fTimeInterval = ( MaxTime - MinTime );
    if ( fTimeInterval < SCALAR_ERROR )
        return MinTime; // Empty/Singleton interval

    // Compute cycles
    Scalar fCycles = ( fControlTime - MinTime ) / fTimeInterval;
    Int iCycles = MathFn->Floor( fCycles );
    Scalar fRemainder = ( fCycles - (Scalar)iCycles );

    // Wrap case
    if ( Repeat == CONTROLLER_REPEAT_WRAP )
        return MinTime + (fRemainder * fTimeInterval);

    // Mirror case
    Assert( Repeat == CONTROLLER_REPEAT_MIRROR );
    if ( (iCycles & 1) != 0 )
        return MaxTime - (fRemainder * fTimeInterval);
    else
        return MinTime + (fRemainder * fTimeInterval);
}

/////////////////////////////////////////////////////////////////////////////////
// MotionController implementation
MotionController::MotionController( const GChar * strName ):
    KinematicController( strName ),
    m_vPosition(), m_qOrientation(), m_vLinearSpeed(), m_vAngularSpeed()
{
    m_vPosition = Vertex3::Null;
    m_qOrientation = Quaternion::Identity;

    m_vLinearSpeed = Vector3::Null;
    m_vAngularSpeed = Vector3::Null;
}
MotionController::~MotionController()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// ShapeController implementation
ShapeController::ShapeController( const GChar * strName ):
    KinematicController( strName )
{
    // nothing to do
}
ShapeController::~ShapeController()
{
    // nothing to do
}

