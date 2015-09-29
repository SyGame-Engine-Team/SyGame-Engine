/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/World/BlockSpace.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : World hierarchy : BlockSpace
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// BlockSpace implementation
inline const BlockSpaceCoords & BlockSpace::GetBlockSpaceCoords() const {
    return m_vBlockSpaceCoords;
}

inline Area * BlockSpace::GetParentArea() const {
    return m_pParentArea;
}

inline BlockInstance * BlockSpace::GetBlock() const {
    return m_pBlock;
}
inline Void BlockSpace::SetBlock( BlockInstance * pBlockInstance ) {
    m_pBlock = pBlockInstance;
}

inline Void BlockSpace::GetBlockOrigin( Vertex3 * outOrigin ) const {
    Point3 vPosition;
    GetBlockPosition( &vPosition );
    outOrigin->X = ( BLOCK_UNIT * (Scalar)vPosition.X );
    outOrigin->Y = ( BLOCK_UNIT * (Scalar)vPosition.Y );
    outOrigin->Z = ( BLOCK_UNIT * (Scalar)vPosition.Z );
}
inline Void BlockSpace::GetBlockCenter( Vertex3 * outCenter ) const {
    GetBlockOrigin( outCenter );
    outCenter->X += BLOCK_HALF_UNIT;
    outCenter->Y += BLOCK_HALF_UNIT;
    outCenter->Z += BLOCK_HALF_UNIT;
}

inline Bool BlockSpace::HasMicroBlockSpace() const {
    return m_bHasMicroBlockSpace;
}
inline UInt BlockSpace::GetMicroBlockSpaceDepth() const {
    return m_iMicroBlockSpaceDepth;
}
inline Scalar BlockSpace::GetMicroBlockUnit() const {
    return m_fMicroBlockUnit;
}

inline Void BlockSpace::GetMicroBlockOrigin( Vertex3 * outOrigin, const MicroBlockCoords & vMicroBlockCoords ) const {
    Assert( m_bHasMicroBlockSpace );
    Assert( vMicroBlockCoords.X >= 0 && vMicroBlockCoords.X < (Int)m_iMicroBlockSpaceDepth );
    Assert( vMicroBlockCoords.Y >= 0 && vMicroBlockCoords.Y < (Int)m_iMicroBlockSpaceDepth );
    Assert( vMicroBlockCoords.Z >= 0 && vMicroBlockCoords.Z < (Int)m_iMicroBlockSpaceDepth );
    GetBlockOrigin( outOrigin );
    outOrigin->X += ( m_fMicroBlockUnit * (Scalar)(vMicroBlockCoords.X) );
    outOrigin->Y += ( m_fMicroBlockUnit * (Scalar)(vMicroBlockCoords.Y) );
    outOrigin->Z += ( m_fMicroBlockUnit * (Scalar)(vMicroBlockCoords.Z) );
}
inline Void BlockSpace::GetMicroBlockCenter( Vertex3 * outCenter, const MicroBlockCoords & vMicroBlockCoords ) const {
    const Scalar fMicroBlockHalfUnit = ( m_fMicroBlockUnit * 0.5f );
    GetMicroBlockOrigin( outCenter, vMicroBlockCoords );
    outCenter->X += fMicroBlockHalfUnit;
    outCenter->Y += fMicroBlockHalfUnit;
    outCenter->Z += fMicroBlockHalfUnit;
}

inline BlockInstance * BlockSpace::GetMicroBlock( const MicroBlockCoords & vMicroBlockCoords ) const {
    Assert( m_bHasMicroBlockSpace );
    Assert( vMicroBlockCoords.X >= 0 && vMicroBlockCoords.X < (Int)m_iMicroBlockSpaceDepth );
    Assert( vMicroBlockCoords.Y >= 0 && vMicroBlockCoords.Y < (Int)m_iMicroBlockSpaceDepth );
    Assert( vMicroBlockCoords.Z >= 0 && vMicroBlockCoords.Z < (Int)m_iMicroBlockSpaceDepth );
    return m_arrMicroBlocks[ (vMicroBlockCoords.Z * m_iMicroBlockSpaceSlice) + (vMicroBlockCoords.Y * m_iMicroBlockSpaceDepth) + vMicroBlockCoords.X ];
}
inline Void BlockSpace::SetMicroBlock( const MicroBlockCoords & vMicroBlockCoords, BlockInstance * pBlockInstance ) {
    Assert( m_bHasMicroBlockSpace );
    Assert( vMicroBlockCoords.X >= 0 && vMicroBlockCoords.X < (Int)m_iMicroBlockSpaceDepth );
    Assert( vMicroBlockCoords.Y >= 0 && vMicroBlockCoords.Y < (Int)m_iMicroBlockSpaceDepth );
    Assert( vMicroBlockCoords.Z >= 0 && vMicroBlockCoords.Z < (Int)m_iMicroBlockSpaceDepth );
    m_arrMicroBlocks[ (vMicroBlockCoords.Z * m_iMicroBlockSpaceSlice) + (vMicroBlockCoords.Y * m_iMicroBlockSpaceDepth) + vMicroBlockCoords.X ] = pBlockInstance;
}
