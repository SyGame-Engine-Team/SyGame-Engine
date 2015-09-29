/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Controllers/Shape/BoneController.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Shape Controller : Vertex skinning using bones
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// BoneController implementation
//inline ControllerType BoneController::GetType() const {
//    return CONTROLLER_SHAPE_BONE;
//}
//
//inline UInt BoneController::GetVertexCount() const {
//    return m_iVertexCount;
//}
//inline UInt BoneController::GetBoneCount() const {
//    return m_iBoneCount;
//}
//
//inline RigidBody * BoneController::GetBone( UInt iBone ) const {
//    Assert( iBone < m_iBoneCount );
//    return m_arrBones[iBone];
//}
//inline Void BoneController::SetBone( UInt iBone, RigidBody * pBone ) {
//    Assert( iBone < m_iBoneCount );
//    m_arrBones[iBone] = pBone;
//}
//
//inline const Vector3 & BoneController::GetBoneOffset( UInt iVertex, UInt iBone ) const {
//    Assert( iVertex < m_iVertexCount );
//    Assert( iBone < m_iBoneCount );
//    return m_arrBoneOffsets[(iVertex * m_iBoneCount) + iBone];
//}
//inline Void BoneController::SetBoneOffset( UInt iVertex, UInt iBone, const Vector3 & vOffset ) {
//    Assert( iVertex < m_iVertexCount );
//    Assert( iBone < m_iBoneCount );
//    m_arrBoneOffsets[(iVertex * m_iBoneCount) + iBone] = vOffset;
//}
//
//inline Scalar BoneController::GetBoneWeight( UInt iVertex, UInt iBone ) const {
//    Assert( iVertex < m_iVertexCount );
//    Assert( iBone < m_iBoneCount );
//    return m_arrBoneWeights[(iVertex * m_iBoneCount) + iBone];
//}
//inline Void BoneController::SetBoneWeight( UInt iVertex, UInt iBone, Scalar fWeight ) {
//    Assert( iVertex < m_iVertexCount );
//    Assert( iBone < m_iBoneCount );
//    m_arrBoneWeights[(iVertex * m_iBoneCount) + iBone] = fWeight;
//}
//
