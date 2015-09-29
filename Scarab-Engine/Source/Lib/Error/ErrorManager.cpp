/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Error/ErrorManager.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : A general purpose manager for error tracking
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
#include "ErrorManager.h"

// Standard shared codes
ErrorDefine( ERROR_CONTEXT_SHARED, ERROR_OK,            TEXT("Success") );
ErrorDefine( ERROR_CONTEXT_SHARED, ERROR_UNKNOWN,       TEXT("Unknown error") );
ErrorDefine( ERROR_CONTEXT_SHARED, ERROR_BAD_ARGUMENT,  TEXT("Invalid argument") );
ErrorDefine( ERROR_CONTEXT_SHARED, ERROR_BAD_RESULT,    TEXT("Invalid result") );
ErrorDefine( ERROR_CONTEXT_SHARED, ERROR_OUT_OF_MEMORY, TEXT("Out of memory (OOM)") );

/////////////////////////////////////////////////////////////////////////////////
// ErrorManager implementation
GChar ErrorManager::sm_strLogBuffer[4096];

ErrorManager::ErrorManager()
{
    // Init contexts
    for( UInt i = 0; i < ERROR_MAX_CONTEXTS; ++i )
        m_arrContexts[i].idContext = INVALID_OFFSET;
    m_iNextFreeContext = 0;

    // Init shared context
    m_errorSharedContext.idContext = ERROR_CONTEXT_SHARED;
    m_errorSharedContext.iNextFreeCode = 0;
    StringFn->Copy( m_errorSharedContext.strName, TEXT("Shared / Global") );
}
ErrorManager::~ErrorManager()
{
    // nothing to do
}

ErrorContextID ErrorManager::CreateContext( const GChar * strName )
{
    if ( m_iNextFreeContext == INVALID_OFFSET )
        return INVALID_OFFSET;

    ErrorContext * pNewContext = &(m_arrContexts[m_iNextFreeContext]);
    pNewContext->idContext = m_iNextFreeContext;

    pNewContext->iNextFreeCode = 0;
    StringFn->NCopy( pNewContext->strName, strName, ERROR_CONTEXT_MAX_NAMELENGTH - 1 );

    ++m_iNextFreeContext;
    if ( m_iNextFreeContext >= ERROR_MAX_CONTEXTS )
        m_iNextFreeContext = INVALID_OFFSET;

    return pNewContext->idContext;
}

ErrorCode ErrorManager::CreateErrorCode( ErrorContextID idContext, const GChar * strErrorMsg )
{
    ErrorContext * pContext = _GetContext( idContext );
    if ( pContext == NULL )
        return ERROR_UNKNOWN;
    if ( pContext->iNextFreeCode >= ERROR_CONTEXT_MAX_ERRORCODES )
        return ERROR_UNKNOWN;

    ErrorCode iErrorCode = pContext->iNextFreeCode;
    ++(pContext->iNextFreeCode);

    StringFn->NCopy( pContext->arrMessages[iErrorCode], strErrorMsg, ERROR_CONTEXT_MAX_MESSAGELENGTH - 1 );

    return ( (pContext->idContext << 16) | iErrorCode );
}

const GChar * ErrorManager::GetContextName( ErrorCode iErrorCode )
{
    ErrorContextID idContext = ( iErrorCode >> 16 );
    ErrorContext * pContext = _GetContext( idContext );
    if ( pContext == NULL )
        return NULL;

    return pContext->strName;
}
const GChar * ErrorManager::GetErrorMessage( ErrorCode iErrorCode )
{
    ErrorContextID idContext = ( iErrorCode >> 16 );
    ErrorContext * pContext = _GetContext( idContext );
    if ( pContext == NULL )
        return NULL;

    UInt iIndex = ( iErrorCode & 0x0000ffff );
    if ( iIndex >= pContext->iNextFreeCode )
        return NULL;

    return pContext->arrMessages[iIndex];
}

Void ErrorManager::_DebugPrint( const GChar * strDebugMessage, ... )
{
    VArgPtr pArgList;
    VArg_Init( pArgList, strDebugMessage );

    StringFn->FormatVA( sm_strLogBuffer, strDebugMessage, pArgList );
    SystemFn->DebugMessage( sm_strLogBuffer );

    VArg_Destroy( pArgList );
}
Void ErrorManager::_Throw( ErrorCode iErrorCode, const GChar * strFileName, UInt iLineNumber )
{
    HFile logFile = SystemFn->OpenFile( ERROR_LOGFILE_THROWS, FILE_WRITE );
    if ( !logFile.IsValid() )
        return;
    logFile.Seek( FILE_SEEK_END, 0 );

    const GChar * strContextName = GetContextName( iErrorCode );
    const GChar * strErrMsg = GetErrorMessage( iErrorCode );

    StringFn->Format( sm_strLogBuffer, TEXT("Error thrown (Code = %u8x) :"), iErrorCode );
    logFile.WriteString( sm_strLogBuffer );

    StringFn->Format( sm_strLogBuffer, TEXT("\n => Filename : %s"), strFileName );
    logFile.WriteString( sm_strLogBuffer );

    StringFn->Format( sm_strLogBuffer, TEXT("\n => Line     : %ud"), iLineNumber );
    logFile.WriteString( sm_strLogBuffer );

    StringFn->Format( sm_strLogBuffer, TEXT("\n => Context  : %s"), strContextName );
    logFile.WriteString( sm_strLogBuffer );

    StringFn->Format( sm_strLogBuffer, TEXT("\n => Message  : %s"), strErrMsg );
    logFile.WriteString( sm_strLogBuffer );

    logFile.WriteString( TEXT("\n\n") );

    logFile.Close();
}
Void ErrorManager::_Assert(const GChar * strExpr, const GChar * strFileName, UInt iLineNumber)
{
    HFile logFile = SystemFn->OpenFile( ERROR_LOGFILE_ASSERTS, FILE_WRITE );
    if ( !logFile.IsValid() )
        return;
    logFile.Seek( FILE_SEEK_END, 0 );

    logFile.WriteString( TEXT("Assertion failed :") );

    StringFn->Format( sm_strLogBuffer, TEXT("\n => Filename   : %s"), strFileName );
    logFile.WriteString( sm_strLogBuffer );

    StringFn->Format( sm_strLogBuffer, TEXT("\n => Line       : %ud"), iLineNumber );
    logFile.WriteString( sm_strLogBuffer );

    StringFn->Format( sm_strLogBuffer, TEXT("\n => Expression : %s"), strExpr );
    logFile.WriteString( sm_strLogBuffer );

    logFile.WriteString( TEXT("\n\n") );

    logFile.Close();
}
Void ErrorManager::Log( const GChar * strLogFile, const GChar * strFormat, ... )
{
    VArgPtr pArguments;
    VArg_Init( pArguments, strFormat );

    HFile logFile = SystemFn->OpenFile( strLogFile, FILE_WRITE );
    if ( !logFile.IsValid() )
        return;
    logFile.Seek( FILE_SEEK_END, 0 );

    StringFn->FormatVA( sm_strLogBuffer, strFormat, pArguments );
    logFile.WriteString( sm_strLogBuffer );

    logFile.Close();

    VArg_Destroy( pArguments );
}
Void ErrorManager::Log( const HFile & hLogFile, const GChar * strFormat, ... )
{
    VArgPtr pArguments;
    VArg_Init( pArguments, strFormat );

    StringFn->FormatVA( sm_strLogBuffer, strFormat, pArguments );
    hLogFile.WriteString( sm_strLogBuffer );

    VArg_Destroy( pArguments );
}

