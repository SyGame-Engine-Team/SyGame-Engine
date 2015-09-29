/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/Outdoor/WorldOutdoor.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Node : Outdoor
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// WorldOutdoor implementation
inline WorldNodeType WorldOutdoor::GetType() const {
    return WORLDNODE_OUTDOOR;
}

inline const Vertex2 & WorldOutdoor::GetOrigin() const {
    return m_vOrigin;
}
inline const Vector2 & WorldOutdoor::GetAreaSize() const {
    return m_vAreaSize;
}

inline UInt WorldOutdoor::GetViewDistance() const {
    return m_iViewDistance;
}
inline UInt WorldOutdoor::GetVisibleAreasPitch() const {
    return m_iVisibleAreasPitch;
}

inline Void WorldOutdoor::GetEyeArea( Point2 * outAreaCoords ) const {
    *outAreaCoords = m_vEyeArea;
}

/////////////////////////////////////////////////////////////////////////////////

inline UInt WorldOutdoor::_GetVisibleAreaIndex( UInt iVisibleX, UInt iVisibleY ) const {
    Assert( iVisibleX < m_iVisibleAreasPitch && iVisibleY < m_iVisibleAreasPitch );
    return ( (iVisibleY * m_iVisibleAreasPitch) + iVisibleX );
}
inline WorldOutdoorArea * WorldOutdoor::_GetVisibleArea( UInt iVisibleX, UInt iVisibleY ) const {
    Assert( iVisibleX < m_iVisibleAreasPitch && iVisibleY < m_iVisibleAreasPitch );
    return m_arrVisibleAreas[(iVisibleY * m_iVisibleAreasPitch) + iVisibleX];
}
inline Void WorldOutdoor::_UpdateArea( UInt iVisibleX, UInt iVisibleY, const Point2 & vAreaCoords ) {
    Assert( iVisibleX < m_iVisibleAreasPitch && iVisibleY < m_iVisibleAreasPitch );
	WorldOutdoorArea * pArea = _GetVisibleArea( iVisibleX, iVisibleY );
    pArea->OnAreaUpdate( vAreaCoords );
}

inline Void WorldOutdoor::_GetAreaCoords( Point2 * outAreaCoords, const Vertex2 & vPosition ) const {
    Vector2 vLocalPos = ( vPosition - m_vOrigin );
    outAreaCoords->X = MathFn->Floor( vLocalPos.X / m_vAreaSize.X ); // Top-Left convention,
    outAreaCoords->Y = MathFn->Floor( vLocalPos.Y / m_vAreaSize.Y ); // Round toward -inf
}
inline Void WorldOutdoor::_GetVisibleAreaCoords( Point2 * outAreaCoords, UInt iVisibleX, UInt iVisibleY, const Point2 & vEyeArea ) const {
    Assert( iVisibleX < m_iVisibleAreasPitch );
    Assert( iVisibleY < m_iVisibleAreasPitch );
    *outAreaCoords = ( vEyeArea - m_iViewDistance ) + Offset2( iVisibleX, iVisibleY );
}
