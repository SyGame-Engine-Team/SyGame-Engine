/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Controllers/Motion/ManualController.cpp
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
// Includes
#include "ManualController.h"

/////////////////////////////////////////////////////////////////////////////////
// ManualController implementation
ManualController::ManualController( const GChar * strName, const Vertex3 & vPosition, const Quaternion & qOrientation,
                                                           const Vector3 & vLinearSpeed, const Vector3 & vAngularSpeed ):
    MotionController( strName ),
    m_vManualLinearSpeed( vLinearSpeed ), m_vManualAngularSpeed( vAngularSpeed ),
    m_vOldPosition( vPosition ), m_qOldOrientation( qOrientation ),
    m_vBoundMin(), m_vBoundMax()
{
    m_vPosition = vPosition;
    m_qOrientation = qOrientation;
    m_vLinearSpeed = Vector3::Null;
    m_vAngularSpeed = Vector3::Null;

    m_bManualUpdate = true;
    m_bUseManualSpeed = true;

    m_bBounded = false;
    m_vBoundMin = Vertex3::Null;
    m_vBoundMax = Vertex3::Null;
}
ManualController::~ManualController()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////

Bool ManualController::_UpdateControllerState( Scalar fTimeStep )
{
    if ( !m_bManualUpdate )
        return false;

    if ( m_bUseManualSpeed ) {
        m_vLinearSpeed = m_vManualLinearSpeed;
        m_vAngularSpeed = m_vManualAngularSpeed;

        m_vOldPosition = m_vPosition;
        m_qOldOrientation = m_qOrientation;

        m_vPosition += ( m_vLinearSpeed * fTimeStep );

        Vector3 vAxis = m_vAngularSpeed;
        Scalar fAngle = vAxis.Normalize();
        Quaternion qDelta( vAxis, fAngle * fTimeStep );
        m_qOrientation = ( qDelta * m_qOrientation );
        m_qOrientation.Normalize();

        if ( m_bBounded ) {
            m_vPosition.X = Clamp<Scalar>( m_vPosition.X, m_vBoundMin.X, m_vBoundMax.X );
            m_vPosition.Y = Clamp<Scalar>( m_vPosition.Y, m_vBoundMin.Y, m_vBoundMax.Y );
            m_vPosition.Z = Clamp<Scalar>( m_vPosition.Z, m_vBoundMin.Z, m_vBoundMax.Z );
        }
    } else {
        Scalar fInvTimeStep = MathFn->Invert( fTimeStep );

        m_vLinearSpeed = ( m_vPosition - m_vOldPosition ) * fInvTimeStep;

        Quaternion qRotation;
        m_qOldOrientation.Invert( qRotation );
        qRotation = m_qOrientation * qRotation;
        Scalar fAngle;
        qRotation.GetAxisAngle( m_vAngularSpeed, fAngle );
        m_vAngularSpeed *= ( fAngle * fInvTimeStep );

        m_vOldPosition = m_vPosition;
        m_qOldOrientation = m_qOrientation;

        m_bManualUpdate = false;
    }

    return true;
}

