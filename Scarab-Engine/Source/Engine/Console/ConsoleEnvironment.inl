/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/ConsoleEnvironment.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Delegate of Console manager, variables & execution state ...
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// ConsoleVariable implementation
inline const GChar * ConsoleVariable::GetName() const {
    return m_strName;
}

inline ConsoleVariableType ConsoleVariable::GetType() const {
    return m_iType;
}
inline Void * ConsoleVariable::GetValue() const {
    return m_pValue;
}

/////////////////////////////////////////////////////////////////////////////////
// ConsoleEnvironment implementation
inline UInt ConsoleEnvironment::GetVariableCount() const {
    return m_mapVariables.Count();
}
inline ConsoleVariable * ConsoleEnvironment::GetVariable( const GChar * strName ) const {
    VariableMap::Iterator itVar = m_mapVariables.Get( strName );
    if ( itVar.IsNull() )
        return NULL;
    return itVar.GetItem();
}

inline Void ConsoleEnvironment::EnumVariables() const {
    m_itEnumerate = m_mapVariables.Begin();
}
inline ConsoleVariable * ConsoleEnvironment::EnumNextVariable() const {
    if ( m_itEnumerate.IsNull() )
        return NULL;
    ConsoleVariable * pVariable = m_itEnumerate.GetItem();
    ++m_itEnumerate;
    return pVariable;
}

inline ResourceDB * ConsoleEnvironment::GetActiveRDB() const {
    return m_pActiveDB;
}
inline Void ConsoleEnvironment::SetActiveRDB( ResourceDB * pRDB ) {
    Assert( !m_bOpenedResource );
    m_pActiveDB = pRDB;
}

inline Bool ConsoleEnvironment::HasOpenedResource() const {
    return m_bOpenedResource;
}
inline RDB_ResourceHandle * ConsoleEnvironment::GetOpenedResource() {
    Assert( m_bOpenedResource );
    return &m_hOpenedResource;
}
inline const GChar * ConsoleEnvironment::GetOpenedResourceName() const {
    Assert( m_bOpenedResource );
    return m_strOpenedResourceName;
}

/////////////////////////////////////////////////////////////////////////////////

inline Int ConsoleEnvironment::_Name_Comparator( const GChar* const & rLeft, const GChar* const & rRight, Void * /*pUserData*/ ) {
    return StringFn->NCmp( rLeft, rRight, CONSOLEVAR_NAME_LENGTH - 1 );
}
