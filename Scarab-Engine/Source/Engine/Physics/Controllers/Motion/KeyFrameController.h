/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Controllers/Motion/KeyFrameController.h
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
// Header prelude
#ifndef SCARAB_ENGINE_PHYSICS_CONTROLLERS_MOTION_KEYFRAMECONTROLLER_H
#define SCARAB_ENGINE_PHYSICS_CONTROLLERS_MOTION_KEYFRAMECONTROLLER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../KinematicController.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The KeyFrameController class
class KeyFrameController : public MotionController
{
public:
    KeyFrameController( const GChar * strName, UInt iPositionKeyCount, UInt iOrientationKeyCount,
                                               UInt iSharedKeyCount = 0 );
    virtual ~KeyFrameController();

    // Type
    inline virtual ControllerType GetType() const;

    // Shared time-line
    inline Bool HasSharedKeys() const;
    inline UInt GetSharedKeyCount() const;

    inline Scalar GetSharedTime( UInt iKey ) const;
    inline Void SetSharedTime( UInt iKey, Scalar fTime );

    // Position time-line
    inline UInt GetPositionKeyCount() const;

    inline Scalar GetPositionTime( UInt iKey ) const;
    inline Void SetPositionTime( UInt iKey, Scalar fTime );

    inline const Vertex3 & GetPosition( UInt iKey ) const;
    inline Void SetPosition( UInt iKey, const Vertex3 & vPosition );

    // Orientation time-line
    inline UInt GetOrientationKeyCount() const;

    inline Scalar GetOrientationTime( UInt iKey ) const;
    inline Void SetOrientationTime( UInt iKey, Scalar fTime );

    inline const Quaternion & GetOrientation( UInt iKey ) const;
    inline Void SetOrientation( UInt iKey, const Quaternion & qOrientation );

protected:
    // Update support
    virtual Bool _UpdateControllerState( Scalar fTimeStep );

    // Helpers
    static Void _LookupKeyFrames( Scalar fControlTime, Scalar * arrTimeLine, UInt iKeyCount,
                                  UInt * pLastKey, Scalar * outNormalTime, UInt * outPrevKey, UInt * outNextKey );

    Vertex3 _Lerp_Position( Scalar fNormalTime, UInt iPrevKey, UInt iNextKey ) const;
    Quaternion _Lerp_Orientation( Scalar fNormalTime, UInt iPrevKey, UInt iNextKey ) const;

    // Shared time-line
    UInt m_iSharedKeyCount;
    UInt m_iSharedLastKey;
    Scalar * m_arrSharedTimes;

    // Position time-line
    UInt m_iPositionKeyCount;
    UInt m_iPositionLastKey;
    Scalar * m_arrPositionTimes;
    Vertex3 * m_arrPositionKeys;

    // Orientation time-line
    UInt m_iOrientationKeyCount;
    UInt m_iOrientationLastKey;
    Scalar * m_arrOrientationTimes;
    Quaternion * m_arrOrientationKeys;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "KeyFrameController.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENGINE_PHYSICS_CONTROLLERS_MOTION_KEYFRAMECONTROLLER_H

