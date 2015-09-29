/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Controllers/Motion/KeyFrameController.inl
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
// KeyFrameController implementation
inline ControllerType KeyFrameController::GetType() const {
    return CONTROLLER_MOTION_KEYFRAME;
}

inline Bool KeyFrameController::HasSharedKeys() const {
    return ( m_iSharedKeyCount > 0 );
}
inline UInt KeyFrameController::GetSharedKeyCount() const {
    return m_iSharedKeyCount;
}

inline Scalar KeyFrameController::GetSharedTime( UInt iKey ) const {
    Assert( iKey < m_iSharedKeyCount );
    return m_arrSharedTimes[iKey];
}
inline Void KeyFrameController::SetSharedTime( UInt iKey, Scalar fTime ) {
    Assert( iKey < m_iSharedKeyCount );
    m_arrSharedTimes[iKey] = fTime;
}

inline UInt KeyFrameController::GetPositionKeyCount() const {
    return m_iPositionKeyCount;
}

inline Scalar KeyFrameController::GetPositionTime( UInt iKey ) const {
    Assert( iKey < ((m_iSharedKeyCount > 0) ? m_iSharedKeyCount : m_iPositionKeyCount) );
    return m_arrPositionTimes[iKey];
}
inline Void KeyFrameController::SetPositionTime( UInt iKey, Scalar fTime ) {
    Assert( iKey < ((m_iSharedKeyCount > 0) ? m_iSharedKeyCount : m_iPositionKeyCount) );
    m_arrPositionTimes[iKey] = fTime;
}

inline const Vertex3 & KeyFrameController::GetPosition( UInt iKey ) const {
    Assert( iKey < m_iPositionKeyCount );
    return m_arrPositionKeys[iKey];
}
inline Void KeyFrameController::SetPosition( UInt iKey, const Vertex3 & vPosition ) {
    Assert( iKey < m_iPositionKeyCount );
    m_arrPositionKeys[iKey] = vPosition;
}

inline UInt KeyFrameController::GetOrientationKeyCount() const {
    return m_iOrientationKeyCount;
}

inline Scalar KeyFrameController::GetOrientationTime( UInt iKey ) const {
    Assert( iKey < ((m_iSharedKeyCount > 0) ? m_iSharedKeyCount : m_iOrientationKeyCount) );
    return m_arrOrientationTimes[iKey];
}
inline Void KeyFrameController::SetOrientationTime( UInt iKey, Scalar fTime ) {
    Assert( iKey < ((m_iSharedKeyCount > 0) ? m_iSharedKeyCount : m_iOrientationKeyCount) );
    m_arrOrientationTimes[iKey] = fTime;
}

inline const Quaternion & KeyFrameController::GetOrientation( UInt iKey ) const {
    Assert( iKey < m_iOrientationKeyCount );
    return m_arrOrientationKeys[iKey];
}
inline Void KeyFrameController::SetOrientation( UInt iKey, const Quaternion & qOrientation ) {
    Assert( iKey < m_iOrientationKeyCount );
    m_arrOrientationKeys[iKey] = qOrientation;
}


