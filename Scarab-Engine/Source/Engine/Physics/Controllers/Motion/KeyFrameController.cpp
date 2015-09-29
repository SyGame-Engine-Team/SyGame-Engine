/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Controllers/Motion/KeyFrameController.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Motion Controller : Key-Frame interpolation
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
#include "KeyFrameController.h"

#include "../../PhysicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// KeyFrameController implementation
KeyFrameController::KeyFrameController( const GChar * strName, UInt iPositionKeyCount, UInt iOrientationKeyCount,
                                                               UInt iSharedKeyCount ):
    MotionController( strName )
{
    PhysicsFn->SelectMemory( TEXT("Controllers") );

    // Shared time-line
    m_iSharedKeyCount = iSharedKeyCount;
    m_iSharedLastKey = 0;
    m_arrSharedTimes = NULL;
    if ( m_iSharedKeyCount > 0 )
        m_arrSharedTimes = New Scalar[m_iSharedKeyCount];

    // Position time-line
    m_iPositionKeyCount = iPositionKeyCount;
    m_iPositionLastKey = 0;
    m_arrPositionTimes = NULL;
    m_arrPositionKeys = NULL;
    if ( m_iPositionKeyCount > 0 ) {
        if ( m_arrSharedTimes != NULL )
            m_arrPositionTimes = m_arrSharedTimes;
        else
            m_arrPositionTimes = New Scalar[m_iPositionKeyCount];
        m_arrPositionKeys = New Vertex3[m_iPositionKeyCount];
    }

    // Orientation time-line
    m_iOrientationKeyCount = iOrientationKeyCount;
    m_iOrientationLastKey = 0;
    m_arrOrientationTimes = NULL;
    m_arrOrientationKeys = NULL;
    if ( m_iOrientationKeyCount > 0 ) {
        if ( m_arrSharedTimes != NULL )
            m_arrOrientationTimes = m_arrSharedTimes;
        else
            m_arrOrientationTimes = New Scalar[m_iOrientationKeyCount];
        m_arrOrientationKeys = New Quaternion[m_iOrientationKeyCount];
    }

    PhysicsFn->UnSelectMemory();
}
KeyFrameController::~KeyFrameController()
{
    PhysicsFn->SelectMemory( TEXT("Controllers") );

    if ( m_arrSharedTimes != NULL ) {
        DeleteA( m_arrSharedTimes );
    } else {
        if ( m_arrPositionTimes != NULL )
            DeleteA( m_arrPositionTimes );
        if ( m_arrOrientationTimes != NULL )
            DeleteA( m_arrOrientationTimes );
    }
    if ( m_arrPositionKeys != NULL )
        DeleteA( m_arrPositionKeys );
    if ( m_arrOrientationKeys != NULL )
        DeleteA( m_arrOrientationKeys );

    PhysicsFn->UnSelectMemory();
}

/////////////////////////////////////////////////////////////////////////////////

Bool KeyFrameController::_UpdateControllerState( Scalar /*fTimeStep*/ )
{
    UInt iPrevKey = 0;
    UInt iNextKey = 0;
    Scalar fNormalTime = 0.0f;

    Scalar fPrevDT, fNextDT;
    Vector3 vPrevSpeed, vNextSpeed;
    Quaternion qPrevDelta, qNextDelta;
    Scalar fAngle;

    if ( m_iSharedKeyCount > 0 ) {
        _LookupKeyFrames( m_fControlTime, m_arrSharedTimes, m_iSharedKeyCount,
                          &m_iSharedLastKey, &fNormalTime, &iPrevKey, &iNextKey );
        fPrevDT = ( m_fControlTime - m_arrSharedTimes[iPrevKey] );
        fNextDT = ( m_arrSharedTimes[iNextKey] - m_fControlTime );
        if ( m_iPositionKeyCount > 0 ) {
            m_vPosition = _Lerp_Position( fNormalTime, iPrevKey, iNextKey );

            vPrevSpeed = ( m_vPosition - m_arrPositionKeys[iPrevKey] ) / fPrevDT;
            vNextSpeed = ( m_arrPositionKeys[iNextKey] - m_vPosition ) / fNextDT;
            m_vLinearSpeed = ( vPrevSpeed + vNextSpeed ) * 0.5f;
        }
        if ( m_iOrientationKeyCount > 0 ) {
            m_qOrientation = _Lerp_Orientation( fNormalTime, iPrevKey, iNextKey );

            m_arrOrientationKeys[iPrevKey].Invert( qPrevDelta );
            qPrevDelta = ( m_qOrientation * qPrevDelta );
            qPrevDelta.GetAxisAngle( vPrevSpeed, fAngle );
            vPrevSpeed *= ( fAngle / fPrevDT );

            m_qOrientation.Invert( qNextDelta );
            qNextDelta = ( m_arrOrientationKeys[iNextKey] * qNextDelta );
            qNextDelta.GetAxisAngle( vNextSpeed, fAngle );
            vNextSpeed *= ( fAngle / fNextDT );

            m_vAngularSpeed = ( vPrevSpeed + vNextSpeed ) * 0.5f;
        }
    } else {
        if ( m_iPositionKeyCount > 0 ) {
            _LookupKeyFrames( m_fControlTime, m_arrPositionTimes, m_iPositionKeyCount,
                              &m_iPositionLastKey, &fNormalTime, &iPrevKey, &iNextKey );
            fPrevDT = ( m_fControlTime - m_arrPositionTimes[iPrevKey] );
            fNextDT = ( m_arrPositionTimes[iNextKey] - m_fControlTime );

            m_vPosition = _Lerp_Position( fNormalTime, iPrevKey, iNextKey );

            Vector3 vPrevSpeed = ( m_vPosition - m_arrPositionKeys[iPrevKey] ) / fPrevDT;
            Vector3 vNextSpeed = ( m_arrPositionKeys[iNextKey] - m_vPosition ) / fNextDT;
            m_vLinearSpeed = ( vPrevSpeed + vNextSpeed ) * 0.5f;
        }
        if ( m_iOrientationKeyCount > 0 ) {
            _LookupKeyFrames( m_fControlTime, m_arrOrientationTimes, m_iOrientationKeyCount,
                              &m_iOrientationLastKey, &fNormalTime, &iPrevKey, &iNextKey );
            fPrevDT = ( m_fControlTime - m_arrOrientationTimes[iPrevKey] );
            fNextDT = ( m_arrOrientationTimes[iNextKey] - m_fControlTime );

            m_qOrientation = _Lerp_Orientation( fNormalTime, iPrevKey, iNextKey );

            m_arrOrientationKeys[iPrevKey].Invert( qPrevDelta );
            qPrevDelta = ( m_qOrientation * qPrevDelta );
            qPrevDelta.GetAxisAngle( vPrevSpeed, fAngle );
            vPrevSpeed *= ( fAngle / fPrevDT );

            m_qOrientation.Invert( qNextDelta );
            qNextDelta = ( m_arrOrientationKeys[iNextKey] * qNextDelta );
            qNextDelta.GetAxisAngle( vNextSpeed, fAngle );
            vNextSpeed *= ( fAngle / fNextDT );

            m_vAngularSpeed = ( vPrevSpeed + vNextSpeed ) * 0.5f;
        }
    }

    return true;
}

Void KeyFrameController::_LookupKeyFrames( Scalar fControlTime, Scalar * arrTimeLine, UInt iKeyCount,
                                           UInt * pLastKey, Scalar * outNormalTime, UInt * outPrevKey, UInt * outNextKey )
{
    if ( fControlTime <= arrTimeLine[0] ) {
        *pLastKey = 0;
        *outPrevKey = 0;
        *outNextKey = 0;
        *outNormalTime = 0.0f;
        return;
    }
    if ( fControlTime >= arrTimeLine[iKeyCount - 1] ) {
        *pLastKey = iKeyCount - 1;
        *outPrevKey = *pLastKey;
        *outNextKey = *pLastKey;
        *outNormalTime = 0.0f;
        return;
    }

    UInt iNextKey;
    if ( fControlTime > arrTimeLine[*pLastKey] ) {
        iNextKey = (*pLastKey + 1);
        while( fControlTime >= arrTimeLine[iNextKey] )
            ++iNextKey;
        *pLastKey = iNextKey - 1;

        *outPrevKey = *pLastKey;
        *outNextKey = iNextKey;
        *outNormalTime = ( (fControlTime - arrTimeLine[*outPrevKey]) / (arrTimeLine[*outNextKey] - arrTimeLine[*outPrevKey]) );

        return;
    }
    if ( fControlTime < arrTimeLine[*pLastKey] ) {
        iNextKey = (*pLastKey - 1);
        while( fControlTime <= arrTimeLine[iNextKey] )
            --iNextKey;
        *pLastKey = iNextKey + 1;

        *outPrevKey = iNextKey;
        *outNextKey = *pLastKey;
        *outNormalTime = ( (fControlTime - arrTimeLine[*outPrevKey]) / (arrTimeLine[*outNextKey] - arrTimeLine[*outPrevKey]) );

        return;
    }

    *outPrevKey = *pLastKey;
    *outNextKey = *pLastKey;
    *outNormalTime = 0.0f;
}

Vertex3 KeyFrameController::_Lerp_Position( Scalar fNormalTime, UInt iPrevKey, UInt iNextKey ) const
{
    return ( (m_arrPositionKeys[iPrevKey] * (1.0f - fNormalTime)) +
             (m_arrPositionKeys[iNextKey].ToVector() * fNormalTime) );
}
Quaternion KeyFrameController::_Lerp_Orientation( Scalar fNormalTime, UInt iPrevKey, UInt iNextKey ) const
{
    Quaternion qLerp;
    qLerp.SLerp( m_arrOrientationKeys[iPrevKey], m_arrOrientationKeys[iNextKey], fNormalTime );
    return qLerp;
}
