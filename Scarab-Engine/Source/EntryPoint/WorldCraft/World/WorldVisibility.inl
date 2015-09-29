/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/World/WorldVisibility.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World integration with outdoor culling system
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// BlockSpaceNode implementation

/////////////////////////////////////////////////////////////////////////////////
// BlockWorldArea implementation

/////////////////////////////////////////////////////////////////////////////////
// BlockWorldRegion implementation
inline const RegionUID & BlockWorldRegion::GetRegionUID() const {
    return m_hRegionUID;
}

/////////////////////////////////////////////////////////////////////////////////

inline Vertex2 BlockWorldRegion::_GetOrigin( const RegionCoords & vRegionCoords ) {
    return Vertex2( REGION_UNIT * (Scalar)(vRegionCoords.X),
                    REGION_UNIT * (Scalar)(vRegionCoords.Y) );
}

