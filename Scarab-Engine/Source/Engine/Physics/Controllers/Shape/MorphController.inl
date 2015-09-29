/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Physics/Controllers/Shape/MorphController.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Shape Controller : Vertex morphing
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// MorphController implementation
//inline ControllerType MorphController::GetType() const {
//    return CONTROLLER_SHAPE_MORPH;
//}
//
//inline UInt MorphController::GetMorphKeyCount() const {
//    return m_iMorphKeyCount;
//}
//
//inline Scalar MorphController::GetMorphTime( UInt iKey ) const {
//    Assert( iKey < m_iMorphKeyCount );
//    return m_arrMorphTimes[iKey];
//}
//inline Void MorphController::SetMorphTime( UInt iKey, Scalar fTime ) {
//    Assert( iKey < m_iMorphKeyCount );
//    m_arrMorphTimes[iKey] = fTime;
//}
//
//inline Scalar MorphController::GetMorphWeight( UInt iKey, UInt iTarget ) const {
//    Assert( iKey < m_iMorphKeyCount );
//    Assert( iTarget < m_iTargetCount );
//    return m_arrMorphKeys[(iKey * m_iTargetCount) + iTarget];
//}
//inline Void MorphController::SetMorphWeight( UInt iKey, UInt iTarget, Scalar fWeight ) {
//    Assert( iKey < m_iMorphKeyCount );
//    Assert( iTarget < m_iTargetCount );
//    m_arrMorphKeys[(iKey * m_iTargetCount) + iTarget] = fWeight;
//}
//
//inline UInt MorphController::GetVertexCount() const {
//    return m_iVertexCount;
//}
//inline UInt MorphController::GetTargetCount() const {
//    return m_iTargetCount;
//}
//
//inline const Vertex3 & MorphController::GetBaseVertex( UInt iVertex ) const {
//    Assert( iVertex < m_iVertexCount );
//    return m_arrBaseVertices[iVertex];
//}
//inline Void MorphController::SetBaseVertex( UInt iVertex, const Vertex3 & vBaseVertex ) {
//    Assert( iVertex < m_iVertexCount );
//    m_arrBaseVertices[iVertex] = vBaseVertex;
//}
//
//inline const Vector3 & MorphController::GetTargetVector( UInt iTarget, UInt iVertex ) const {
//    Assert( iTarget < m_iTargetCount );
//    Assert( iVertex < m_iVertexCount );
//    return m_arrTargetVectors[(iTarget * m_iVertexCount) + iVertex];
//}
//inline Void MorphController::SetTargetVector( UInt iTarget, UInt iVertex, const Vector3 & vTargetVector ) {
//    Assert( iTarget < m_iTargetCount );
//    Assert( iVertex < m_iVertexCount );
//    m_arrTargetVectors[(iTarget * m_iVertexCount) + iVertex] = vTargetVector;
//}

