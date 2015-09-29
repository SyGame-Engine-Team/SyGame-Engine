/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/WorldCuller.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Graph : Culling operations
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : none
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// VisibleGroup implementation
inline UInt VisibleGroup::GetCount() const {
    return m_arrLeaves.Count();
}
inline WorldLeaf * VisibleGroup::GetLeaf( UInt iIndex ) const {
    Assert( iIndex < m_arrLeaves.Count() );
    return m_arrLeaves[iIndex];
}

inline Void VisibleGroup::Insert( WorldLeaf * pLeaf ) {
    m_arrLeaves.Push( pLeaf );
}
inline Void VisibleGroup::Clear() {
    m_arrLeaves.Clear();
}

/////////////////////////////////////////////////////////////////////////////////
// WorldCuller implementation
inline const Camera * WorldCuller::GetCamera() const {
    return m_pCamera;
}

inline const Plane & WorldCuller::GetFrustrumPlane( UInt iFrustrumPlane ) const {
    Assert( iFrustrumPlane < FRUSTRUM_COUNT );
    return m_arrFrustrumPlanes[iFrustrumPlane];
}
inline const Vertex3 & WorldCuller::GetFrustrumVertex( UInt iFrustrumVertex ) const {
    Assert( iFrustrumVertex < 8 );
    return m_arrFrustrumVertices[iFrustrumVertex];
}
inline const Vertex3 & WorldCuller::GetFrustrumVertex( Bool bFar, Bool bTop, Bool bLeft ) const {
    UInt iVertex = 0;
    if ( bFar ) iVertex += 4;
    if ( bTop ) iVertex += 2;
    if ( bLeft ) ++iVertex;
    return m_arrFrustrumVertices[iVertex];
}

inline UInt WorldCuller::GetUserPlaneCount() const {
    return m_iUserPlaneCount;
}
inline const Plane & WorldCuller::GetUserPlane( UInt iUserPlane ) const {
    Assert( iUserPlane < m_iUserPlaneCount );
    return m_arrUserPlanes[iUserPlane];
}

inline Void WorldCuller::PushUserPlane( const Plane & newPlane ) {
    Assert( m_iUserPlaneCount < WORLDCULLER_MAX_USERPLANES );
    m_arrUserPlanes[m_iUserPlaneCount++] = newPlane;
}
inline Void WorldCuller::PopUserPlane() {
    Assert( m_iUserPlaneCount > 0 );
    --m_iUserPlaneCount;
}

inline DWord WorldCuller::GetPlaneState() const {
    return m_dwPlaneState;
}
inline Void WorldCuller::SetPlaneState( DWord dwPlaneState ) {
    m_dwPlaneState = dwPlaneState;
}

inline const VisibleGroup & WorldCuller::GetVisibleGroup() const {
    return m_curVisibleGroup;
}


