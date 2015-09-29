/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Controllers/Motion/IKJointController.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Motion Controller : Inverse Kinematics
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// IKBone implementation
inline KinematicBody * IKBone::GetBody() const {
    return m_pBody;
}

inline UInt IKBone::GetChildJointCount() const {
    return m_iJointCount;
}
inline IKJointController * IKBone::GetChildJoint( UInt iChild ) const {
    Assert( iChild < m_iJointCount );
    return m_arrJoints[iChild];
}

inline Void IKBone::PushChildJoint( IKJointController * pJoint ) {
    Assert( m_iJointCount < m_iJointCapacity );
    m_arrJoints[m_iJointCount++] = pJoint;
}

/////////////////////////////////////////////////////////////////////////////////
// IKGoal implementation
inline IKBone * IKGoal::GetEffector() const {
    return m_pEffector;
}
inline BodyNode * IKGoal::GetGoal() const {
    return m_pGoal;
}

/////////////////////////////////////////////////////////////////////////////////
// IKJointController implementation
inline ControllerType IKJointController::GetType() const {
    return CONTROLLER_MOTION_IKJOINT;
}

inline IKBone * IKJointController::GetParent() const {
    return m_pParent;
}
inline IKBone * IKJointController::GetChild() const {
    return m_pChild;
}

inline UInt IKJointController::GetGoalCount() const {
    return m_iGoalCount;
}
inline IKGoal * IKJointController::GetGoal( UInt iGoal ) const {
    Assert( iGoal < m_iGoalCount );
    return m_arrGoals[iGoal];
}

inline const Vector3 & IKJointController::GetJointTranslation() const {
    return m_vJointTranslation;
}
inline const Vector3 & IKJointController::GetJointRotation() const {
    return m_vJointRotation;
}

