/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/Gameplay/Blocks/Block.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Block base interface
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Structures implementation

/////////////////////////////////////////////////////////////////////////////////
// Block implementation
inline const BlockID & Block::GetID() const {
    return m_hID;
}
inline const GChar * Block::GetName() const {
    return m_strName;
}

inline Bool Block::CanPassThrough() const {
    return m_bPassThrough;
}
inline Bool Block::IsBreakable() const {
    return m_bBreakable;
}
inline Scalar Block::GetHardness() const {
    return m_fHardness;
}
inline Bool Block::HasGravity() const {
    return m_bHasGravity;
}
inline Scalar Block::GetMass() const {
    return m_fMass;
}
inline Bool Block::IsSolid() const {
    return ( !m_bIsLiquid && !m_bIsGas );
}
inline Bool Block::IsLiquid() const {
    return m_bIsLiquid;
}
inline Bool Block::IsGas() const {
    return m_bIsGas;
}
inline Bool Block::IsFluid() const {
    return ( m_bIsLiquid || m_bIsGas );
}
inline Scalar Block::GetViscosity() const {
    return m_fViscosity;
}
    
inline Bool Block::IsMicroBlock() const {
    return m_bIsMicroBlock;
}
inline UInt Block::GetMinMicroBlockDepth() const {
    return m_iMinMicroBlockDepth;
}
inline const Point3 & Block::GetMicroBlockSize() const {
    return m_vMicroBlockSize;
}

inline GPU2DBitmap * Block::GetIconSmall() const {
    return m_pIconSmall;
}
inline GPU2DBitmap * Block::GetIconLarge() const {
    return m_pIconLarge;
}
inline const Material * Block::GetMaterial( BlockFace iFace ) const {
    return ( m_arrMaterials + iFace );
}
inline GPUTextureCube * Block::GetTexture( BlockFace iFace ) const {
    return m_arrTextures[iFace];
}

/////////////////////////////////////////////////////////////////////////////////
// BlockInstance implementation
inline const Block * BlockInstance::GetBlock() const {
    return m_pBlock;
}

inline BlockOrientation BlockInstance::GetOrientation() const {
    return m_iOrientation;
}
inline BlockOrientation BlockInstance::CycleOrientation() {
    m_iOrientation = (BlockOrientation)( ( ((Int)m_iOrientation) + 1 ) % 6 );
}

