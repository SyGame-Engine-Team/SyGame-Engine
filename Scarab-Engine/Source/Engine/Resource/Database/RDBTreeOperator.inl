/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Resource/Database/RDBTreeOperator.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Resource Database, B*Tree operations
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// RDBTreeOperator implementation
inline Bool RDBTreeOperator::InsertDirectory( const RDB_BTreeItem & rItem ) {
    Assert( rItem.iResourceType == RESOURCE_TYPE_DIRECTORY );
    m_iCurRoot = RDB_CHUNKTABLE_DIRECTORIES_ROOT;
    return _BTree_Insert( m_pStorage->GetTableChunk(m_iCurRoot), rItem, NULL );
}
inline Bool RDBTreeOperator::InsertResource( const RDB_BTreeItem & rItem ) {
    Assert( rItem.iResourceType != RESOURCE_TYPE_DIRECTORY );
    m_iCurRoot = RDB_CHUNKTABLE_RESOURCES_ROOT;
    return _BTree_Insert( m_pStorage->GetTableChunk(m_iCurRoot), rItem, NULL );
}

inline Bool RDBTreeOperator::RemoveDirectory( const RDB_BTreeKeyData & rKey ) {
    m_iCurRoot = RDB_CHUNKTABLE_DIRECTORIES_ROOT;
    return _BTree_Remove( m_pStorage->GetTableChunk(m_iCurRoot), rKey, NULL );
}
inline Bool RDBTreeOperator::RemoveResource( const RDB_BTreeKeyData & rKey ) {
    m_iCurRoot = RDB_CHUNKTABLE_RESOURCES_ROOT;
    return _BTree_Remove( m_pStorage->GetTableChunk(m_iCurRoot), rKey, NULL );
}
