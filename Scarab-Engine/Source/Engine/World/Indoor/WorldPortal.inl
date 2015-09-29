/////////////////////////////////////////////////////////////////////////////////
// File : Engine/World/Indoor/WorldPortal.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : The Portal class, traditional convex room separator
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// WorldPortal implementation
inline UInt WorldPortal::GetPolygonSize() const {
    return m_iPolygonSize;
}
inline const Vertex3 & WorldPortal::GetPolygon( UInt iVertex ) const {
    Assert( iVertex < m_iPolygonSize );
    return m_arrWorldPolygon[iVertex];
}
