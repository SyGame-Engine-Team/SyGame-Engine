/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Bodies/Systems/RagDoll.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Skeleton : Dynamic rigid bodies
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// RagDoll implementation
inline SystemBodyType RagDoll::GetType() const {
    return SYSTEMBODY_RAGDOLL;
}

inline UInt RagDoll::GetBoneCount() const {
    return m_iBoneCount;
}
inline RigidBody * RagDoll::GetBone( UInt iBone ) const {
    Assert( iBone < m_iBoneCount );
    return m_arrBones[iBone];
}
inline Void RagDoll::SetBone( UInt iBone, RigidBody * pBone ) {
    Assert( iBone < m_iBoneCount );
    Assert( !(pBone->IsStatic()) );
    m_arrBones[iBone] = pBone;
}

inline UInt RagDoll::GetJointCount() const {
    return m_iJointCount;
}
inline JointConstraint * RagDoll::GetJoint( UInt iJoint ) const {
    Assert( iJoint < m_iJointCount );
    return m_arrJoints[iJoint];
}
inline Void RagDoll::SetJoint( UInt iJoint, JointConstraint * pJoint ) {
    Assert( iJoint < m_iJointCount );
    m_arrJoints[iJoint] = pJoint;
}
