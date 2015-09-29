/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Resource/Database/RDBFileStructure.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Resource Database, file structure descriptors
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// File pointer
inline Int _RDB_FilePtr_Compare( const RDB_FilePtr & rLeft, const RDB_FilePtr & rRight, Void * /*pUserData*/ ) {
    if ( rLeft < rRight )
        return +1;
    if ( rLeft > rRight )
        return -1;
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////
// Chunk layer
inline Bool RDB_ChunkPtr::operator==( const RDB_ChunkPtr & rhs ) const {
    return ( (idBlock == rhs.idBlock) && (idChunk == rhs.idChunk) && (iType == rhs.iType) );
}
inline Bool RDB_ChunkPtr::operator!=( const RDB_ChunkPtr & rhs ) const {
    return ( (idBlock != rhs.idBlock) || (idChunk != rhs.idChunk) || (iType != rhs.iType) );
}

/////////////////////////////////////////////////////////////////////////////////
// B+Tree layer
inline RDB_BTreeKeyData & RDB_BTreeKeyData::operator=( const RDB_BTreeKeyData & rhs ) {
    idParent = rhs.idParent;
    StringFn->NCopyA( strName, rhs.strName, RDB_BTREE_NAME_LENGTH - 1 );
    return (*this);
}

inline RDB_BTreeKey & RDB_BTreeKey::operator=( const RDB_BTreeKey & rhs ) {
    keyData = rhs.keyData;
    pLeftChild.idBlock = rhs.pLeftChild.idBlock;
    pLeftChild.idChunk = rhs.pLeftChild.idChunk;
    pLeftChild.iType = rhs.pLeftChild.iType;
    return (*this);
}

inline RDB_BTreeItem & RDB_BTreeItem::operator=( const RDB_BTreeItem & rhs ) {
    keyData = rhs.keyData;
    iResourceType = rhs.iResourceType;
    MemCopy( &(Content), &(rhs.Content), sizeof(Content) );
    return (*this);
}
