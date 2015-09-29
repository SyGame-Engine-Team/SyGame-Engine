/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/System/File.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : File IO standard API
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Third-Party Includes
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#undef DebugAssert

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "File.h"

/////////////////////////////////////////////////////////////////////////////////
// HFile implementation
HFile::HFile()
{
    m_strPathName[0] = NULLBYTE;
    m_iMode = 0;
    m_iFlags = 0;
    m_iAttributes = 0;
    m_hFile = NULL;
}
HFile::HFile( const GChar * strPathName, FileMode iMode, FileFlags iFlags, Bool bCreateElseOpen )
{
    DWord dwCreateDisposition = 0;
    DWord dwAttribs = 0;
    if ( bCreateElseOpen ) {
        dwCreateDisposition = CREATE_NEW;
        dwAttribs = FILE_ATTRIBUTE_NORMAL;
    } else {
        dwCreateDisposition = OPEN_EXISTING;
        dwAttribs = GetFileAttributes( strPathName );
        if ( dwAttribs == INVALID_FILE_ATTRIBUTES )
            dwAttribs = FILE_ATTRIBUTE_NORMAL;
        if ( (iMode & FILE_WRITE) && (dwAttribs & FILE_ATTRIBUTE_READONLY) )
            DebugAssert( false );
    }

    DWord dwAccess = 0;
    if ( iMode & FILE_READ )
        dwAccess |= FILE_GENERIC_READ;
    if ( iMode & FILE_WRITE )
        dwAccess |= FILE_GENERIC_WRITE;
    DebugAssert( dwAccess != 0 );

    DWord dwShareRead = 0;
    if ( dwAccess == FILE_GENERIC_READ ) // Read-only
        dwShareRead = FILE_SHARE_READ;

    DWord dwFlags = 0;
    if ( iFlags & FILE_FLAG_RANDOM )
        dwFlags |= FILE_FLAG_RANDOM_ACCESS;
    if ( iFlags & FILE_FLAG_SEQUENTIAL )
        dwFlags |= FILE_FLAG_SEQUENTIAL_SCAN;
    if ( iFlags & FILE_FLAG_NO_WRITE_CACHE )
        dwFlags |= FILE_FLAG_WRITE_THROUGH;
    if ( iFlags & FILE_FLAG_NO_CACHE )
        dwFlags |= FILE_FLAG_NO_BUFFERING;

    for( UInt iRetry = 0; iRetry < FILE_ACCESS_RETRIES; ++iRetry ) {
        m_hFile = CreateFile( strPathName, dwAccess, dwShareRead, NULL,
                              dwCreateDisposition, (dwAttribs | dwFlags), NULL );
        if ( m_hFile != INVALID_HANDLE_VALUE )
            break;
    }

    if ( m_hFile == INVALID_HANDLE_VALUE ) {
        m_strPathName[0] = NULLBYTE;
        m_iMode = 0;
        m_iFlags = 0;
        m_iAttributes = 0;
        m_hFile = NULL;
    } else {
        StringFn->NCopy( m_strPathName, strPathName, PATHNAME_LENGTH - 1 );
        m_iMode = iMode;
        m_iFlags = iFlags;
        m_iAttributes = _ConvertAttributes( dwAttribs );
    }
}
HFile::~HFile()
{
    // nothing to do
    // Must call Close explicitly
}

//Bool HFile::ReOpen( FileMode iMode, FileFlags iFlags )
//{
//    DebugAssert( m_hFile != NULL );
//
//    DWord dwAttribs = _RevertAttributes( m_iAttributes );
//    if ( (iMode & FILE_WRITE) && (dwAttribs & FILE_ATTRIBUTE_READONLY) )
//        DebugAssert( false );
//
//    DWord dwAccess = 0;
//    if ( iMode & FILE_READ )
//        dwAccess |= FILE_GENERIC_READ;
//    if ( iMode & FILE_WRITE )
//        dwAccess |= FILE_GENERIC_WRITE;
//    DebugAssert( dwAccess != 0 );
//
//    DWord dwShareRead = 0;
//    if ( dwAccess == FILE_GENERIC_READ ) // Read-only
//        dwShareRead = FILE_SHARE_READ;
//
//    DWord dwFlags = 0;
//    if ( iFlags & FILE_FLAG_RANDOM )
//        dwFlags |= FILE_FLAG_RANDOM_ACCESS;
//    if ( iFlags & FILE_FLAG_SEQUENTIAL )
//        dwFlags |= FILE_FLAG_SEQUENTIAL_SCAN;
//    if ( iFlags & FILE_FLAG_NO_WRITE_CACHE )
//        dwFlags |= FILE_FLAG_WRITE_THROUGH;
//    if ( iFlags & FILE_FLAG_NO_CACHE )
//        dwFlags |= FILE_FLAG_NO_BUFFERING;
//
//    HANDLE hNewFile = INVALID_HANDLE_VALUE;
//    for( UInt iRetry = 0; iRetry < FILE_ACCESS_RETRIES; ++iRetry ) {
//        hNewFile = ReOpenFile( m_hFile, dwAccess, dwShareRead, (dwAttribs | dwFlags) );
//        if ( hNewFile != INVALID_HANDLE_VALUE )
//            break;
//    }
//
//    if ( hNewFile == INVALID_HANDLE_VALUE )
//        return false;
//    m_iMode = iMode;
//    m_iFlags = iFlags;
//    m_hFile = hNewFile;
//    return true;
//}
Void HFile::Close()
{
    if ( m_hFile == NULL )
        return;

    BOOL bRes = CloseHandle( m_hFile );
    DebugAssert( bRes != FALSE );

    m_strPathName[0] = NULLBYTE;
    m_iMode = 0;
    m_iFlags = 0;
    m_iAttributes = 0;
    m_hFile = NULL;
}

UInt64 HFile::GetSize() const
{
    DebugAssert( m_hFile != NULL );

    LARGE_INTEGER largeInt;
    BOOL bRes = GetFileSizeEx( m_hFile, &largeInt );
    DebugAssert( bRes != FALSE );

    return (UInt64)( largeInt.QuadPart );
}
Bool HFile::SetSize( UInt64 iByteSize ) const
{
    DebugAssert( m_hFile != NULL );

    UInt64 iSavedPtr;
    Bool bOk = Tell( &iSavedPtr );
    DebugAssert( bOk );

    bOk = Seek( iByteSize );
    DebugAssert( bOk );

    BOOL bRes = SetEndOfFile( m_hFile );

    bOk = Seek( iSavedPtr );
    DebugAssert( bOk );

    return ( bRes != FALSE );
}

Bool HFile::Tell( UInt64 * outAbsolute ) const
{
    DebugAssert( m_hFile != NULL );

    LARGE_INTEGER largeNullOffset, largeCurOffset;
    largeNullOffset.QuadPart = 0i64;

    BOOL bRes = SetFilePointerEx( m_hFile, largeNullOffset, &largeCurOffset, FILE_CURRENT );
    if ( bRes == FALSE )
        return false;

    *outAbsolute = (UInt64)( largeCurOffset.QuadPart );
    return true;
}
Bool HFile::Seek( FileSeek iBase, Int64 iRelative ) const
{
    DebugAssert( m_hFile != NULL );

    DWord dwBase = 0;
    switch( iBase ) {
        case FILE_SEEK_BEGIN:   dwBase = FILE_BEGIN;   break;
        case FILE_SEEK_CURRENT: dwBase = FILE_CURRENT; break;
        case FILE_SEEK_END:     dwBase = FILE_END;     break;
        default: DebugAssert( false ); break;
    }

    LARGE_INTEGER largeOffset;
    largeOffset.QuadPart = iRelative;

    BOOL bRes = SetFilePointerEx( m_hFile, largeOffset, NULL, dwBase );
    return ( bRes != FALSE );
}

Bool HFile::ReadBytes( Byte * pOutBuffer, UInt iByteSize ) const
{
    DebugAssert( m_hFile != NULL );

    DWord dwBytesRead = 0;
    BOOL bRes = ReadFile( m_hFile, pOutBuffer, (DWord)iByteSize, &dwBytesRead, NULL );
    if ( bRes == FALSE )
        return false;
    if ( dwBytesRead != (DWord)iByteSize )
        return false;
    return true;
}
Bool HFile::WriteBytes( const Byte * pInBuffer, UInt iByteSize ) const
{
    DebugAssert( m_hFile != NULL );

    DWord dwBytesWritten = 0;
    BOOL bRes = WriteFile( m_hFile, pInBuffer, (DWord)iByteSize, &dwBytesWritten, NULL );
    if ( bRes == FALSE )
        return false;
    if ( dwBytesWritten != (DWord)iByteSize )
        return false;
    return true;
}

Bool HFile::ReadChar( GChar * outCh ) const
{
    DebugAssert( m_hFile != NULL );

    Byte byteValue;

    DWord dwBytesRead = 0;
    BOOL bRes = ReadFile( m_hFile, &byteValue, 1, &dwBytesRead, NULL );
    if ( bRes == FALSE )
        return false;
    if ( dwBytesRead != 1 )
        return false;

    *outCh = (GChar)byteValue;
    return true;
}
Bool HFile::WriteChar( GChar ch ) const
{
    DebugAssert( m_hFile != NULL );

    // Temporary assumption, system doesn't default to UTF-16,
    // Use UTF-8 / Legacy ASCII instead ...
    DebugAssert( ch <= 0xff );

    Byte byteValue = (Byte)ch;

    DWord dwBytesWritten = 0;
    BOOL bRes = WriteFile( m_hFile, &byteValue, 1, &dwBytesWritten, NULL );
    if ( bRes == FALSE )
        return false;
    if ( dwBytesWritten != 1 )
        return false;

    return true;
}

Bool HFile::ReadString( GChar * outString, UInt iMaxLength ) const
{
    DebugAssert( m_hFile != NULL );

    Bool bOk;
    for( UInt i = 0; i < iMaxLength; ++i ) {
        bOk = ReadChar( outString );
        if ( !bOk )
            return false;
        if ( *outString == NULLBYTE )
            return true;
        ++outString;
    }
    *outString = NULLBYTE;
    return true;
}
Bool HFile::WriteString( const GChar * strString ) const
{
    DebugAssert( m_hFile != NULL );

    Bool bOk;
    while( *strString != NULLBYTE ) {
        bOk = WriteChar( *strString );
        if ( !bOk )
            return false;
        ++strString;
    }
    return true;
}

//Bool HFile::LockBytes( FileLock iLock, UInt64 iByteOffset, UInt64 iByteSize, Bool bWait ) const
//{
//    if ( m_hFile == NULL )
//        return false;
//
//    DWord dwFlags = 0;
//    if ( iLock == FILE_LOCK_EXCLUSIVE )
//        dwFlags |= LOCKFILE_EXCLUSIVE_LOCK;
//    if ( !bWait )
//        dwFlags |= LOCKFILE_FAIL_IMMEDIATELY;
//
//    OVERLAPPED fileRegion;
//    MemZero( &fileRegion, sizeof(OVERLAPPED) );
//    fileRegion.OffsetHigh = HighWord(iByteOffset);
//    fileRegion.Offset = LowWord(iByteOffset);
//    fileRegion.hEvent = NULL;
//
//    BOOL bLocked = LockFileEx( m_hFile, dwFlags, 0, LowWord(iByteSize), HighWord(iByteSize), &fileRegion );
//    return (bLocked == FALSE) ? false : true;
//}
//Bool HFile::UnLockBytes( UInt64 iByteOffset, UInt64 iByteSize ) const
//{
//    if ( m_hFile == NULL )
//        return false;
//
//    OVERLAPPED fileRegion;
//    MemZero( &fileRegion, sizeof(OVERLAPPED) );
//    fileRegion.OffsetHigh = HighWord(iByteOffset);
//    fileRegion.Offset = LowWord(iByteOffset);
//    fileRegion.hEvent = NULL;
//
//    BOOL bUnLocked = UnlockFileEx( m_hFile, 0, LowWord(iByteSize), HighWord(iByteSize), &fileRegion );
//    return (bUnLocked == FALSE) ? false : true;
//}

/////////////////////////////////////////////////////////////////////////////////

FileAttributes HFile::_ConvertAttributes( DWord dwAttribs )
{
    if ( dwAttribs == FILE_ATTRIBUTE_NORMAL )
        return 0;

    FileAttributes iAttributes = 0;

    if ( dwAttribs & FILE_ATTRIBUTE_READONLY )
        iAttributes |= FILE_ATTRIB_READONLY;
    if ( dwAttribs & FILE_ATTRIBUTE_TEMPORARY )
        iAttributes |= FILE_ATTRIB_TEMPORARY;
    if ( dwAttribs & FILE_ATTRIBUTE_ARCHIVE )
        iAttributes |= FILE_ATTRIB_ARCHIVE;
    if ( dwAttribs & FILE_ATTRIBUTE_HIDDEN )
        iAttributes |= FILE_ATTRIB_HIDDEN;

    return iAttributes;
}
DWord HFile::_RevertAttributes( FileAttributes iAttributes )
{
    if ( iAttributes == 0 )
        return FILE_ATTRIBUTE_NORMAL;

    DWord dwAttribs = 0;

    if ( iAttributes & FILE_ATTRIB_READONLY )
        dwAttribs |= FILE_ATTRIBUTE_READONLY;
    if ( iAttributes & FILE_ATTRIB_TEMPORARY )
        dwAttribs |= FILE_ATTRIBUTE_TEMPORARY;
    if ( iAttributes & FILE_ATTRIB_ARCHIVE )
        dwAttribs |= FILE_ATTRIBUTE_ARCHIVE;
    if ( iAttributes & FILE_ATTRIB_HIDDEN )
        dwAttribs |= FILE_ATTRIBUTE_HIDDEN;

    return dwAttribs;
}
