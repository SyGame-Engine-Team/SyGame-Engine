/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldCraft/Gameplay/Blocks/BlockManager.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : WorldCraft, Blocks : BlockManager
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// BlockManager implementation
inline const GChar * BlockManager::GetName() {
    return TEXT("BlockManager");
}
inline const GChar * BlockManager::GetMemoryBudget() {
    return TEXT (
"Heap:Scratch(4194304);Heap:Blocks(4194304);Heap:MultiBlockPatterns(4194304);Heap:IOInterfaces(4194304);"
    );
}

inline Block * BlockManager::GetBlock( const BlockID & hBlockID ) const {
    if ( m_mapBlockDB.Contains(hBlockID) )
        return m_mapBlockDB[hBlockID];
    return NULL;
}

inline Void BlockManager::RegisterBlock( Block * pBlock ) {
    if ( m_mapBlockDB.Contains(pBlock->GetID()) )
        return;
    Bool bInserted = m_mapBlockDB.Insert( pBlock->GetID(), pBlock );
    Assert( bInserted );
}
inline Block * BlockManager::UnRegisterBlock( const BlockID & hBlockID ) {
    if ( m_mapBlockDB.Contains(hBlockID) ) {
        Block * pBlock = NULL;
        Bool bRemoved = m_mapBlockDB.Remove( hBlockID, &pBlock );
        Assert( bRemoved && pBlock != NULL );
        return pBlock;
    }
    return NULL;
}

/////////////////////////////////////////////////////////////////////////////////

inline Int BlockManager::_Compare_BlockID( const BlockID & rLeft, const BlockID & rRight, Void * /*pUserData*/ ) {
    if ( rLeft.dwID < rRight.dwID )
        return +1;
    if ( rLeft.dwID > rRight.dwID )
        return -1;
    if ( rLeft.dwSpecifier < rRight.dwSpecifier )
        return +1;
    if ( rLeft.dwSpecifier > rRight.dwSpecifier )
        return -1;
    return 0;
}

