/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Controllers/Motion/BlendController.cpp
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
// Includes
#include "BlendController.h"

/////////////////////////////////////////////////////////////////////////////////
// BlendController implementation
BlendController::BlendController( const GChar * strName, MotionController * pSource, MotionController * pDestination,
                                                         Bool bGeometricBlend ):
    MotionController( strName )
{
    m_pSource = pSource;
    m_pDestination = pDestination;

    m_bGeometricBlend = bGeometricBlend;

    StartTime = 1.0f;
    StopTime = 0.0f;
}
BlendController::~BlendController()
{
    // nothing to do
}

Void BlendController::SetTarget( KinematicBody * pTarget )
{
    KinematicController::SetTarget( pTarget );

    m_pSource->SetTarget( pTarget );
    m_pDestination->SetTarget( pTarget );
}

/////////////////////////////////////////////////////////////////////////////////

Bool BlendController::_UpdateControllerState( Scalar fTimeStep )
{
    // Before blend case
    Scalar fSrcControlTime = m_pSource->GetControlTime();
    if ( fSrcControlTime < StartTime ) {
        m_pSource->Update( fTimeStep );

        m_vPosition = m_pSource->GetPosition();
        m_qOrientation = m_pSource->GetOrientation();

        m_vLinearSpeed = m_pSource->GetLinearSpeed();
        m_vAngularSpeed = m_pSource->GetAngularSpeed();

        m_fLocalTime = 0.0f; // Keep local time at 0 before blending
        m_fControlTime = 0.0f;

        return true;
    }

    // After blend case
    Scalar fDstControlTime = m_pDestination->GetControlTime();
    if ( fDstControlTime > StopTime ) {
        m_pDestination->Update( fTimeStep );

        m_vPosition = m_pDestination->GetPosition();
        m_qOrientation = m_pDestination->GetOrientation();

        m_vLinearSpeed = m_pDestination->GetLinearSpeed();
        m_vAngularSpeed = m_pDestination->GetAngularSpeed();

        m_fLocalTime = 0.0f; // Keep local time at 0 after blending
        m_fControlTime = 0.0f;

        return true;
    }

    // Blending interval case
    m_pSource->Update( fTimeStep );
    m_pDestination->Update( fTimeStep );

    Scalar fWeight = ( m_fControlTime - MinTime ) / ( MaxTime - MinTime );
    Scalar fInvWeight = ( 1.0f - fWeight );

        // Position => Arithmetic blend
    const Vertex3 & vSrcPosition = m_pSource->GetPosition();
    Vector3 vDstPosition = m_pDestination->GetPosition().ToVector();

    m_vPosition = ( vSrcPosition * fInvWeight );
    m_vPosition += ( vDstPosition * fWeight );

        // Orientation => Geometric OR Arithmetic blend
    const Quaternion & qSrcOrientation = m_pSource->GetOrientation();
    Quaternion qDstOrientation( m_pDestination->GetOrientation() );
    if ( qSrcOrientation.Dot(qDstOrientation) < 0.0f )
        qDstOrientation = -qDstOrientation;

    if ( m_bGeometricBlend )
        m_qOrientation.SLerp( qSrcOrientation, qDstOrientation, fWeight );
    else {
        m_qOrientation = ( (qSrcOrientation * fInvWeight) + (qDstOrientation * fWeight) );
        m_qOrientation.Normalize();
    }

        // Linear Speed => Arithmetic blend
    const Vector3 & vSrcLinearSpeed = m_pSource->GetLinearSpeed();
    const Vector3 & vDstLinearSpeed = m_pDestination->GetLinearSpeed();

    m_vLinearSpeed = ( (vSrcLinearSpeed * fInvWeight) + (vDstLinearSpeed * fWeight) );

        // Angular Speed => Arithmetic blend
    const Vector3 & vSrcAngularSpeed = m_pSource->GetAngularSpeed();
    const Vector3 & vDstAngularSpeed = m_pDestination->GetAngularSpeed();

    m_vAngularSpeed = ( (vSrcAngularSpeed * fInvWeight) + (vDstAngularSpeed * fWeight) );

    return true;
}

