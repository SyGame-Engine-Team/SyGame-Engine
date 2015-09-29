/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/ConsoleEnvironment.cpp
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
// Includes
#include "ConsoleEnvironment.h"

#include "ConsoleManager.h"

/////////////////////////////////////////////////////////////////////////////////
// ConsoleVariable implementation
const ConsoleVariable ConsoleVariable::Null = ConsoleVariable( TEXT("null") );

ConsoleVariable::ConsoleVariable( const GChar * strName )
{
    StringFn->NCopy( m_strName, strName, CONSOLEVAR_NAME_LENGTH - 1 );

    m_iType = CONSOLEVAR_TYPE_UNDEFINED;
    m_pValue = NULL;
}
ConsoleVariable::ConsoleVariable( const GChar * strName, ConsoleVariableType iType, Void * pValue )
{
    StringFn->NCopy( m_strName, strName, CONSOLEVAR_NAME_LENGTH - 1 );

    m_iType = iType;
    m_pValue = pValue;
}
ConsoleVariable::~ConsoleVariable()
{
    if ( m_pValue != NULL )
        Delete( m_pValue );
}

const GChar * ConsoleVariable::GetTypeName( ConsoleVariableType iTypeValue )
{
    switch( iTypeValue ) {
        case CONSOLEVAR_TYPE_UNDEFINED: return TEXT("<undefined>");
            // Rendering types
        case CONSOLEVAR_TYPE_MESH:    return TEXT("mesh");
        case CONSOLEVAR_TYPE_TEXTURE: return TEXT("texture");

        default: Assert(false); return NULL;
    }
}
ConsoleVariableType ConsoleVariable::GetTypeValue( const GChar * strTypeName )
{
    // Simple search
    ConsoleVariableType iType;
    for( UInt i = 1; i < CONSOLEVAR_TYPE_COUNT; ++i ) {
        iType = (ConsoleVariableType)i;
        if ( StringFn->Cmp(GetTypeName(iType), strTypeName) == 0 )
            return iType;
    }

    // Not found
    return CONSOLEVAR_TYPE_UNDEFINED;
}

Void ConsoleVariable::Affect( ConsoleVariableType iType, Void * pValue )
{
    if ( m_pValue != NULL ) {
        ConsoleFn->SelectMemory( TEXT("Environment") );
        Delete( m_pValue );
        ConsoleFn->UnSelectMemory();
    }

    m_iType = iType;
    m_pValue = pValue;
}
Void ConsoleVariable::Affect( ConsoleVariable * pValue )
{
    if ( m_pValue != NULL ) {
        ConsoleFn->SelectMemory( TEXT("Environment") );
        Delete( m_pValue );
        ConsoleFn->UnSelectMemory();
    }

    m_iType = pValue->m_iType;
    m_pValue = pValue->m_pValue;

    // Forbid multiple referencing
    pValue->m_pValue = NULL;
}

/////////////////////////////////////////////////////////////////////////////////
// ConsoleEnvironment implementation
ConsoleEnvironment::ConsoleEnvironment():
    m_mapVariables(), m_itEnumerate()
{
    // Resource state data
    m_pActiveDB = NULL;

    m_bOpenedResource = false;
    MemZero( &m_hOpenedResource, sizeof(RDB_ResourceHandle) );
    m_strOpenedResourceName[0] = NULLBYTE;
}
ConsoleEnvironment::~ConsoleEnvironment()
{
    // nothing to do
}

Void ConsoleEnvironment::Initialize()
{
    // Environment variables
    m_mapVariables.UseMemoryContext( ConsoleFn->GetMemoryContext(), TEXT("Environment") );
    m_mapVariables.SetComparator( _Name_Comparator );
    m_mapVariables.Create();
}
Void ConsoleEnvironment::Cleanup()
{
    // Environment variables
    ConsoleFn->SelectMemory( TEXT("Environment") );
    VariableMap::Iterator itVar;
    for( itVar = m_mapVariables.Begin(); !(itVar.IsNull()); ++itVar )
        Delete( itVar.GetItem() );
    ConsoleFn->UnSelectMemory();

    m_mapVariables.Destroy();
}

ConsoleVariable * ConsoleEnvironment::CreateVariable( const GChar * strName )
{
    // Variable already exists
    if ( m_mapVariables.Contains(strName) )
        return NULL;

    ConsoleFn->SelectMemory( TEXT("Environment") );
    ConsoleVariable * pVar = New ConsoleVariable( strName );
    ConsoleFn->UnSelectMemory();

    VariableMap::Iterator itVar;
    Bool bInserted = m_mapVariables.Insert( pVar->GetName(), pVar, &itVar );
    Assert( bInserted && !(itVar.IsNull()) );

    return itVar.GetItem();
}
ConsoleVariable * ConsoleEnvironment::CreateVariable( const GChar * strName, ConsoleVariableType iType, Void * pValue )
{
    // Variable already exists
    if ( m_mapVariables.Contains(strName) )
        return NULL;

    ConsoleFn->SelectMemory( TEXT("Environment") );
    ConsoleVariable * pVar = New ConsoleVariable( strName, iType, pValue );
    ConsoleFn->UnSelectMemory();

    VariableMap::Iterator itVar;
    Bool bInserted = m_mapVariables.Insert( pVar->GetName(), pVar, &itVar );
    Assert( bInserted && !(itVar.IsNull()) );

    return itVar.GetItem();
}
Bool ConsoleEnvironment::DestroyVariable( const GChar * strName )
{
    ConsoleVariable * pRemoved = NULL;
    Bool bRemoved = m_mapVariables.Remove( strName, &pRemoved );
    if ( !bRemoved )
        return false;

    Assert( pRemoved != NULL );

    ConsoleFn->SelectMemory( TEXT("Environment") );
    Delete( pRemoved );
    ConsoleFn->UnSelectMemory();

    return true;
}

Bool ConsoleEnvironment::AffectVariable( const GChar * strName, ConsoleVariableType iType, Void * pValue )
{
    ConsoleVariable * pVar = GetVariable( strName );
    if ( pVar == NULL )
        return false;

    pVar->Affect( iType, pValue );
    return true;
}
Bool ConsoleEnvironment::AffectVariable( const GChar * strDstName, const GChar * strSrcName )
{
    ConsoleVariable * pDstVar = GetVariable( strDstName );
    if ( pDstVar == NULL )
        return false;

    ConsoleVariable * pSrcVar = GetVariable( strSrcName );
    if ( pSrcVar == NULL )
        return false;

    pDstVar->Affect( pSrcVar );
    return true;
}

Bool ConsoleEnvironment::OpenResource( const GChar * strResPath )
{
    Assert( m_pActiveDB != NULL );
    Assert( !m_bOpenedResource );

    Bool bOk = m_pActiveDB->ResourceOpen( &m_hOpenedResource, strResPath, true );
    if ( !bOk )
        return false;

    RDB_ResourceInfos resInfos;
    m_pActiveDB->GetResourceInfo( &resInfos, &m_hOpenedResource );
    StringFn->NCopy( m_strOpenedResourceName, resInfos.strName, RDB_NAME_LENGTH - 1 );

    m_bOpenedResource = true;

    return true;
}
Void ConsoleEnvironment::CloseResource()
{
    Assert( m_pActiveDB != NULL );
    Assert( m_bOpenedResource );

    m_pActiveDB->ResourceClose( &m_hOpenedResource );

    m_strOpenedResourceName[0] = NULLBYTE;

    m_bOpenedResource = false;
}

