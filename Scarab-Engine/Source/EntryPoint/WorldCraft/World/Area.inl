/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/World/Area.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World hierarchy : Area (16*16*256 BlockSpaces)
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Area implementation
inline const AreaCoords & Area::GetAreaCoords() const {
    return m_vAreaCoords;
}

inline Region * Area::GetParentRegion() const {
    return m_pParentRegion;
}

inline BlockSpace * Area::GetBlockSpace( const BlockSpaceCoords & vBlockSpaceCoords ) const {
    Assert( vBlockSpaceCoords.X >= 0 && vBlockSpaceCoords.X < AREA_SIZE );
    Assert( vBlockSpaceCoords.Y >= 0 && vBlockSpaceCoords.Y < AREA_SIZE );
    Assert( vBlockSpaceCoords.Z >= 0 && vBlockSpaceCoords.Z < AREA_HEIGHT );
    return m_arrBlockSpaces[ ( vBlockSpaceCoords.Z * AREA_SLICE ) + ( vBlockSpaceCoords.Y * AREA_SIZE ) + vBlockSpaceCoords.X ];
}

