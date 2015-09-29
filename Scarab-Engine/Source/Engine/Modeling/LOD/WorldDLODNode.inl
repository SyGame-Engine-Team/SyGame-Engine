/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Modeling/LOD/WorldDLODNode.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Discrete Level Of Detail by model switching
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// WorldDLODNode implementation
inline const Vertex3 & WorldDLODNode::GetModelCenter() const {
    return m_vModelCenter;
}
inline const Vertex3 & WorldDLODNode::GetWorldCenter() const {
    return m_vWorldCenter;
}
inline Void WorldDLODNode::SetModelCenter( const Vertex3 & vCenter ) {
    m_vModelCenter = vCenter;
}

inline UInt WorldDLODNode::GetLODCount() const {
    return m_iLODCount;
}
inline Scalar WorldDLODNode::GetModelMinDistance( UInt iLOD ) const {
    Assert( iLOD < m_iLODCount );
    return m_arrModelMinDist[iLOD];
}
inline Scalar WorldDLODNode::GetModelMaxDistance( UInt iLOD ) const {
    Assert( iLOD < m_iLODCount );
    return m_arrModelMaxDist[iLOD];
}
inline Scalar WorldDLODNode::GetWorldMinDistance( UInt iLOD ) const {
    Assert( iLOD < m_iLODCount );
    return m_arrWorldMinDist[iLOD];
}
inline Scalar WorldDLODNode::GetWorldMaxDistance( UInt iLOD ) const {
    Assert( iLOD < m_iLODCount );
    return m_arrWorldMaxDist[iLOD];
}
inline Void WorldDLODNode::SetModelDistance( UInt iLOD, Scalar fMin, Scalar fMax ) {
    Assert( iLOD < m_iLODCount );
    m_arrModelMinDist[iLOD] = fMin;
    m_arrModelMaxDist[iLOD] = fMax;
    m_arrWorldMinDist[iLOD] = fMin;
    m_arrWorldMaxDist[iLOD] = fMax;
}
