/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/Outdoor/WorldOutdoorArea.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World Node : Outdoor, Area
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : none
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// WorldOutdoorArea implementation
inline WorldNodeType WorldOutdoorArea::GetType() const {
    return WORLDNODE_OUTDOOR_AREA;
}

inline const Vertex2 & WorldOutdoorArea::GetOrigin() const {
    return m_vOrigin;
}
inline const Vector2 & WorldOutdoorArea::GetSize() const {
    return m_vSize;
}

inline WorldPathMicroNode * WorldOutdoorArea::GetAttachedWaypoint() {
    return &m_hAttachedWaypoint;
}

inline UInt WorldOutdoorArea::GetGridWidth() const {
    return m_iGridWidth;
}
inline UInt WorldOutdoorArea::GetGridHeight() const {
    return m_iGridHeight;
}
inline const Vector2 & WorldOutdoorArea::GetGridCellSize() const {
    return m_vGridCellSize;
}

inline Scalar WorldOutdoorArea::GetGridCost( UInt iGridX, UInt iGridY ) const {
    Assert( iGridX < m_iGridWidth );
    Assert( iGridY < m_iGridHeight );
    return m_arrCostGrid[ (iGridY * m_iGridWidth) + iGridX ];
}
inline Scalar WorldOutdoorArea::GetGridCost( const Vertex2 & vPosition ) const {
    UInt iGridX, iGridY;
    Bool bClipped = GetGridPosition( &iGridX, &iGridY, vPosition );
    Assert( bClipped );
    return m_arrCostGrid[ (iGridY * m_iGridWidth) + iGridX ];
}
inline WorldPathNanoNode * WorldOutdoorArea::GetGridWaypoint( UInt iGridX, UInt iGridY ) const {
    Assert( iGridX < m_iGridWidth );
    Assert( iGridY < m_iGridHeight );
    return ( m_arrWaypointGrid + ( (iGridY * m_iGridWidth) + iGridX ) );
}
inline WorldPathNanoNode * WorldOutdoorArea::GetGridWaypoint( const Vertex2 & vPosition ) const {
    UInt iGridX, iGridY;
    Bool bClipped = GetGridPosition( &iGridX, &iGridY, vPosition );
    Assert( bClipped );
    return ( m_arrWaypointGrid + ( (iGridY * m_iGridWidth) + iGridX ) );
}

