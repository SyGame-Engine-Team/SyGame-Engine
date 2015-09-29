/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Resource/ResourceManager.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Core-Class for the resource management system.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// ResourceManager implementation
inline const GChar * ResourceManager::GetName() {
    return TEXT("ResourceManager");
}
inline const GChar * ResourceManager::GetMemoryBudget() {
    return TEXT (
"Heap:Scratch(4194304);Heap:Internal(4194304);Heap:CacheMemory(4194304);\
Heap:XML(4194304);"
    );
}

inline ResourceDB * ResourceManager::GetRDB( const GChar * strName ) const {
    ResourceDBMap::Iterator itRDB = m_mapResourceDBs.Get( strName );
    if ( itRDB.IsNull() )
        return NULL;
    return itRDB.GetItem();
}

inline Void ResourceManager::EnumRDBs() const {
    m_itEnumerate = m_mapResourceDBs.Begin();
}
inline ResourceDB * ResourceManager::EnumNextRDB() const {
    if ( m_itEnumerate.IsNull() )
        return NULL;
    ResourceDB * pRDB = m_itEnumerate.GetItem();
    ++m_itEnumerate;
    return pRDB;
}

/////////////////////////////////////////////////////////////////////////////////

inline Int ResourceManager::_Name_Comparator( const GChar* const & rLeft, const GChar* const & rRight, Void * /*pUserData*/ ) {
    return StringFn->NCmp( rLeft, rRight, RDB_NAME_LENGTH - 1 );
}
