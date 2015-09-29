/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/System/System.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : System dependant APIs.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Third-Party Includes
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <winsock2.h>
#include <ws2tcpip.h>
#include "Iphlpapi.h" // needed to get MAC

#undef DebugAssert
#undef CreateDirectory
#undef GetCurrentDirectory
#undef SetCurrentDirectory
#undef CreateFile
#undef MoveFile
#undef CopyFile
#undef GetFileAttributes
#undef SetFileAttributes
#undef CreateHardLink
#undef CreateSymbolicLink

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "System.h"

/////////////////////////////////////////////////////////////////////////////////
// System implementation
System::System()
{
    // Memory allocation ///////////////////////////////////////////
    m_hDefaultHeap = GetProcessHeap();
    m_hMainHeap = HeapCreate( HEAP_NO_SERIALIZE | HEAP_GENERATE_EXCEPTIONS, 0, 0 );
    DebugAssert( m_hMainHeap != NULL );

    // Time ////////////////////////////////////////////////////////
    LARGE_INTEGER iTmp;
    BOOL bRes = QueryPerformanceFrequency( &iTmp );
    DebugAssert( bRes != FALSE );

    m_fPerformanceFrequency = (Double)( iTmp.QuadPart );
    m_fInvPerformanceFrequency = ( 1.0 / m_fPerformanceFrequency );

    m_iTicksOverhead = 0;
    TickValue iStart, iDelta, iAverage = 0;
    for( UInt i = 0; i < 1000; ++i ) {
        iStart = TicksAbsolute();
        // NO OPERATION //
        iDelta = TicksRelative( iStart );
        iAverage += iDelta;
    }
    m_iTicksOverhead = ( iAverage / 1000 );

    m_fNanoSecondsOverhead = 0.0;
    TimeMeasure fStart, fDelta, fAverage = 0.0;
    for( UInt i = 0; i < 1000; ++i ) {
        fStart = TimeAbsolute( TIMEUNIT_NANOSECONDS );
        // NO OPERATION //
        fDelta = TimeRelative( fStart, TIMEUNIT_NANOSECONDS );
        fAverage += fDelta;
    }
    m_fNanoSecondsOverhead = ( fAverage / 1000.0 );

    // File IO /////////////////////////////////////////////////////
    m_hDirectoryListing = NULL;

    // Networking //////////////////////////////////////////////////
    m_pCurAddrInfo = NULL;
}
System::~System()
{
    // Memory allocation ///////////////////////////////////////////
    BOOL bRes = HeapDestroy( m_hMainHeap );
    DebugAssert( bRes != FALSE );
}

// Debug helpers ///////////////////////////////////////////////
Void System::DebugMessage( const GChar * strMessage ) const
{
    OutputDebugString( strMessage );
}

// Memory allocation ///////////////////////////////////////////
Void * System::MemAlloc( UInt iSize ) const
{
    return HeapAlloc( m_hMainHeap, 0, iSize );
}
Void System::MemFree( Void * pMemory ) const
{
    HeapFree( m_hMainHeap, 0, pMemory );
}

// Time ////////////////////////////////////////////////////////
TickValue System::TicksAbsolute() const
{
    LARGE_INTEGER iTmp;
    BOOL bRes = QueryPerformanceCounter( &iTmp );
    DebugAssert( bRes != FALSE );

    return (TickValue)( iTmp.QuadPart );
}
TickValue System::TicksRelative( TickValue iOrigin ) const
{
    LARGE_INTEGER iTmp;
    BOOL bRes = QueryPerformanceCounter( &iTmp );
    DebugAssert( bRes != FALSE );

    return ( (TickValue)(iTmp.QuadPart) - iOrigin - m_iTicksOverhead );
}

TimeMeasure System::TimeAbsolute( TimeUnit iResolution ) const
{
    LARGE_INTEGER iTmp;
    BOOL bRes = QueryPerformanceCounter( &iTmp );
    DebugAssert( bRes != FALSE );

    Double fTmp = (Double)( iTmp.QuadPart );

    switch( iResolution ) {
        case TIMEUNIT_NANOSECONDS:  return ( fTmp * m_fInvPerformanceFrequency * 1.0e9 );
        case TIMEUNIT_MICROSECONDS: return ( fTmp * m_fInvPerformanceFrequency * 1.0e6 );
        case TIMEUNIT_MILLISECONDS: return ( fTmp * m_fInvPerformanceFrequency * 1.0e3 );
        case TIMEUNIT_SECONDS:      return ( fTmp * m_fInvPerformanceFrequency );
        default: DebugAssert( false ); return 0;
    }
}
TimeMeasure System::TimeRelative( TimeMeasure fStart, TimeUnit iResolution, TimeMeasure * outAbsolute ) const
{
    LARGE_INTEGER iTmp;
    BOOL bRes = QueryPerformanceCounter( &iTmp );
    DebugAssert( bRes != FALSE );

    Double fTmp = (Double)( iTmp.QuadPart );

    switch( iResolution ) {
        case TIMEUNIT_NANOSECONDS:
            fTmp *= ( m_fInvPerformanceFrequency * 1.0e9 );
            if ( outAbsolute != NULL )
                *outAbsolute = fTmp;
            return ( fTmp - fStart - m_fNanoSecondsOverhead );
        case TIMEUNIT_MICROSECONDS:
            fTmp *= ( m_fInvPerformanceFrequency * 1.0e6 );
            if ( outAbsolute != NULL )
                *outAbsolute = fTmp;
            return ( fTmp - fStart - (m_fNanoSecondsOverhead * 1.0e-3) );
        case TIMEUNIT_MILLISECONDS:
            fTmp *= ( m_fInvPerformanceFrequency * 1.0e3 );
            if ( outAbsolute != NULL )
                *outAbsolute = fTmp;
            return ( fTmp - fStart - (m_fNanoSecondsOverhead * 1.0e-6) );
        case TIMEUNIT_SECONDS:
            fTmp *= m_fInvPerformanceFrequency;
            if ( outAbsolute != NULL )
                *outAbsolute = fTmp;
            return ( fTmp - fStart - (m_fNanoSecondsOverhead * 1.0e-9) );
        default: DebugAssert( false ); return 0;
    }
}

TimeMeasure System::ConvertTicks( TickValue iTickValue, TimeUnit iResolution ) const
{
    switch( iResolution ) {
        case TIMEUNIT_NANOSECONDS:  return ( ((Double)iTickValue) * m_fInvPerformanceFrequency * 1.0e9 );
        case TIMEUNIT_MICROSECONDS: return ( ((Double)iTickValue) * m_fInvPerformanceFrequency * 1.0e6 );
        case TIMEUNIT_MILLISECONDS: return ( ((Double)iTickValue) * m_fInvPerformanceFrequency * 1.0e3 );
        case TIMEUNIT_SECONDS:      return ( ((Double)iTickValue) * m_fInvPerformanceFrequency );
        default: DebugAssert( false ); return 0;
    }
}
TickValue System::ConvertTimeMeasure( TimeMeasure fTimeValue, TimeUnit iResolution ) const
{
    switch( iResolution ) {
        case TIMEUNIT_NANOSECONDS:  return (TickValue)( fTimeValue * m_fPerformanceFrequency * 1.0e-9 );
        case TIMEUNIT_MICROSECONDS: return (TickValue)( fTimeValue * m_fPerformanceFrequency * 1.0e-6 );
        case TIMEUNIT_MILLISECONDS: return (TickValue)( fTimeValue * m_fPerformanceFrequency * 1.0e-3 );
        case TIMEUNIT_SECONDS:      return (TickValue)( fTimeValue * m_fPerformanceFrequency );
        default: DebugAssert( false ); return 0;
    }
}

Bool System::GetTimeZone( TimeZone * outTimeZone ) const
{
    TIME_ZONE_INFORMATION hTimeZone;
    DWord dwDayLightSaving = GetTimeZoneInformation( &hTimeZone );

    _ConvertTimeZone( outTimeZone, &hTimeZone );

    return ( dwDayLightSaving == TIME_ZONE_ID_DAYLIGHT );
}

Void System::GetUTCTime( TimeDate * outUTCTime ) const
{
    SYSTEMTIME hSystemTimeUTC;
    GetSystemTime( &hSystemTimeUTC );

    _ConvertTimeDate( outUTCTime, &hSystemTimeUTC );
}
Void System::GetLocalTime( TimeDate * outLocalTime, const TimeZone * pTimeZone ) const
{
    if ( pTimeZone == NULL ) {
        SYSTEMTIME hSystemTimeLocal;
        ::GetLocalTime( &hSystemTimeLocal );

        _ConvertTimeDate( outLocalTime, &hSystemTimeLocal );
    } else {
        TIME_ZONE_INFORMATION hTimeZone;
        _RevertTimeZone( &hTimeZone, pTimeZone );

        SYSTEMTIME hSystemTimeUTC;
        GetSystemTime( &hSystemTimeUTC );

        SYSTEMTIME hSystemTimeLocal;
        BOOL bRes = SystemTimeToTzSpecificLocalTime( &hTimeZone, &hSystemTimeUTC, &hSystemTimeLocal );
        DebugAssert( bRes != FALSE );

        _ConvertTimeDate( outLocalTime, &hSystemTimeLocal );
    }
}

Void System::UTCToLocalTime( TimeDate * outLocalTime, const TimeDate & hUTCTime, const TimeZone * pTimeZone ) const
{
    SYSTEMTIME hSystemTimeUTC;
    _RevertTimeDate( &hSystemTimeUTC, &hUTCTime );

    TIME_ZONE_INFORMATION hTimeZone;
    if ( pTimeZone == NULL )
        GetTimeZoneInformation( &hTimeZone );
    else    
        _RevertTimeZone( &hTimeZone, pTimeZone );

    SYSTEMTIME hSystemTimeLocal;
    BOOL bRes = SystemTimeToTzSpecificLocalTime( &hTimeZone, &hSystemTimeUTC, &hSystemTimeLocal );
    DebugAssert( bRes != FALSE );

    _ConvertTimeDate( outLocalTime, &hSystemTimeLocal );
}
Void System::LocalToUTCTime( TimeDate * outUTCTime, const TimeDate & hLocalTime, const TimeZone * pTimeZone ) const
{
    SYSTEMTIME hSystemTimeLocal;
    _RevertTimeDate( &hSystemTimeLocal, &hLocalTime );

    TIME_ZONE_INFORMATION hTimeZone;
    if ( pTimeZone == NULL )
        GetTimeZoneInformation( &hTimeZone );
    else    
        _RevertTimeZone( &hTimeZone, pTimeZone );

    SYSTEMTIME hSystemTimeUTC;
    BOOL bRes = TzSpecificLocalTimeToSystemTime( &hTimeZone, &hSystemTimeLocal, &hSystemTimeUTC );
    DebugAssert( bRes != FALSE );

    _ConvertTimeDate( outUTCTime, &hSystemTimeUTC );
}

TimeValue System::GetUTCTime( TimeUnit iResolution ) const
{
    FILETIME hFlatTimeUTC;
    GetSystemTimeAsFileTime( &hFlatTimeUTC );
    //GetSystemTimePreciseAsFileTime( &hFlatTimeUTC ); // Windows 8 will provide this

    return _ConvertTimeValue( &hFlatTimeUTC, iResolution );
}
TimeValue System::GetLocalTime( TimeUnit iResolution, const TimeZone * pTimeZone ) const
{
    if ( pTimeZone == NULL ) {
        SYSTEMTIME hSystemTimeLocal;
        ::GetLocalTime( &hSystemTimeLocal );

        FILETIME hFlatTimeLocal;
        BOOL bRes = SystemTimeToFileTime( &hSystemTimeLocal, &hFlatTimeLocal );
        DebugAssert( bRes != FALSE );

        return _ConvertTimeValue( &hFlatTimeLocal, iResolution );
    } else {
        TIME_ZONE_INFORMATION hTimeZone;
        _RevertTimeZone( &hTimeZone, pTimeZone );

        SYSTEMTIME hSystemTimeUTC;
        GetSystemTime( &hSystemTimeUTC );

        SYSTEMTIME hSystemTimeLocal;
        BOOL bRes = SystemTimeToTzSpecificLocalTime( &hTimeZone, &hSystemTimeUTC, &hSystemTimeLocal );
        DebugAssert( bRes != FALSE );

        FILETIME hFlatTimeLocal;
        bRes = SystemTimeToFileTime( &hSystemTimeLocal, &hFlatTimeLocal );
        DebugAssert( bRes != FALSE );

        return _ConvertTimeValue( &hFlatTimeLocal, iResolution );
    }
}

TimeValue System::UTCToLocalTime( TimeValue iUTCTime, TimeUnit iResolution, const TimeZone * pTimeZone ) const
{
    FILETIME hFlatTimeUTC;
    _RevertTimeValue( &hFlatTimeUTC, iUTCTime, iResolution );

    if ( pTimeZone == NULL ) {
        FILETIME hFlatTimeLocal;
        BOOL bRes = FileTimeToLocalFileTime( &hFlatTimeUTC, &hFlatTimeLocal );
        DebugAssert( bRes != FALSE );

        return _ConvertTimeValue( &hFlatTimeLocal, iResolution );
    } else {
        TIME_ZONE_INFORMATION hTimeZone;
        _RevertTimeZone( &hTimeZone, pTimeZone );

        SYSTEMTIME hSystemTimeUTC;
        BOOL bRes = FileTimeToSystemTime( &hFlatTimeUTC, &hSystemTimeUTC );
        DebugAssert( bRes != FALSE );

        SYSTEMTIME hSystemTimeLocal;
        bRes = SystemTimeToTzSpecificLocalTime( &hTimeZone, &hSystemTimeUTC, &hSystemTimeLocal );
        DebugAssert( bRes != FALSE );

        FILETIME hFlatTimeLocal;
        bRes = SystemTimeToFileTime( &hSystemTimeLocal, &hFlatTimeLocal );
        DebugAssert( bRes != FALSE );

        return _ConvertTimeValue( &hFlatTimeLocal, iResolution );
    }
}
TimeValue System::LocalToUTCTime( TimeValue iLocalTime, TimeUnit iResolution, const TimeZone * pTimeZone ) const
{
    FILETIME hFlatTimeLocal;
    _RevertTimeValue( &hFlatTimeLocal, iLocalTime, iResolution );

    if ( pTimeZone == NULL ) {
        FILETIME hFlatTimeUTC;
        BOOL bRes = LocalFileTimeToFileTime( &hFlatTimeLocal, &hFlatTimeUTC );
        DebugAssert( bRes != FALSE );

        return _ConvertTimeValue( &hFlatTimeUTC, iResolution );
    } else {
        TIME_ZONE_INFORMATION hTimeZone;
        _RevertTimeZone( &hTimeZone, pTimeZone );

        SYSTEMTIME hSystemTimeLocal;
        BOOL bRes = FileTimeToSystemTime( &hFlatTimeLocal, &hSystemTimeLocal );
        DebugAssert( bRes != FALSE );

        SYSTEMTIME hSystemTimeUTC;
        bRes = TzSpecificLocalTimeToSystemTime( &hTimeZone, &hSystemTimeLocal, &hSystemTimeUTC );
        DebugAssert( bRes != FALSE );

        FILETIME hFlatTimeUTC;
        bRes = SystemTimeToFileTime( &hSystemTimeUTC, &hFlatTimeUTC );
        DebugAssert( bRes != FALSE );

        return _ConvertTimeValue( &hFlatTimeUTC, iResolution );
    }
}

TimeValue System::ConvertTimeDate( const TimeDate & hTimeDate, TimeUnit iResolution ) const
{
    SYSTEMTIME hSystemTime;
    _RevertTimeDate( &hSystemTime, &hTimeDate );

    FILETIME hFlatTime;
    BOOL bRes = SystemTimeToFileTime( &hSystemTime, &hFlatTime );
    DebugAssert( bRes != FALSE );

    return _ConvertTimeValue( &hFlatTime, iResolution );
}
Void System::ConvertTimeValue( TimeDate * outTimeDate, TimeValue iTimeValue, TimeUnit iResolution ) const
{
    FILETIME hFlatTime;
    _RevertTimeValue( &hFlatTime, iTimeValue, iResolution );

    SYSTEMTIME hSystemTime;
    BOOL bRes = FileTimeToSystemTime( &hFlatTime, &hSystemTime );
    DebugAssert( bRes != FALSE );

    _ConvertTimeDate( outTimeDate, &hSystemTime );
}

const GChar * System::MonthToString( TimeMonth iMonth ) const
{
    switch( iMonth ) {
        case MONTH_JANUARY:   return TEXT("January");
        case MONTH_FEBRUARY:  return TEXT("February");
        case MONTH_MARCH:     return TEXT("March");
        case MONTH_APRIL:     return TEXT("April");
        case MONTH_MAY:       return TEXT("May");
        case MONTH_JUNE:      return TEXT("June");
        case MONTH_JULY:      return TEXT("July");
        case MONTH_AUGUST:    return TEXT("August");
        case MONTH_SEPTEMBER: return TEXT("September");
        case MONTH_OCTOBER:   return TEXT("October");
        case MONTH_NOVEMBER:  return TEXT("November");
        case MONTH_DECEMBER:  return TEXT("December");
        default: DebugAssert( false ); return NULL;
    }
}
const GChar * System::DayToString( TimeDay iDay ) const
{
    switch( iDay ) {
        case DAY_MONDAY:    return TEXT("Monday");
        case DAY_TUESDAY:   return TEXT("Tuesday");
        case DAY_WEDNESDAY: return TEXT("Wednesday");
        case DAY_THURSDAY:  return TEXT("Thursday");
        case DAY_FRIDAY:    return TEXT("Friday");
        case DAY_SATURDAY:  return TEXT("Saturday");
        case DAY_SUNDAY:    return TEXT("Sunday");
        default: DebugAssert( false ); return NULL;
    }
}

GChar * System::FormatTime( GChar * strOutput, const TimeDate & hTimeDate ) const
{
    return StringFn->Format( strOutput, TEXT("%u2d:%u2d:%u2d"), hTimeDate.iHours, hTimeDate.iMinutes, hTimeDate.iSeconds );
}
GChar * System::FormatDate( GChar * strOutput, const TimeDate & hTimeDate ) const
{
    return StringFn->Format( strOutput, TEXT("%s %s, %ud %ud"), DayToString(hTimeDate.iDayOfWeek), MonthToString(hTimeDate.iMonth),
                                                                hTimeDate.iDay, hTimeDate.iYear );
}
GChar * System::FormatTimeDate( GChar * strOutput, const TimeDate & hTimeDate ) const
{
    return StringFn->Format( strOutput, TEXT("%s %s, %ud %ud %u2d:%u2d:%u2d"), DayToString(hTimeDate.iDayOfWeek), MonthToString(hTimeDate.iMonth),
                                                                               hTimeDate.iDay, hTimeDate.iYear,
                                                                               hTimeDate.iHours, hTimeDate.iMinutes, hTimeDate.iSeconds );
}

// File IO /////////////////////////////////////////////////////
Bool System::CreateDirectory( const GChar * strPathName ) const
{
#if ( defined(UNICODE) || defined (_UNICODE) )
    BOOL bRes = CreateDirectoryW( strPathName, NULL );
#else
    BOOL bRes = CreateDirectoryA( strPathName, NULL );
#endif
    return ( bRes != FALSE );
}
Bool System::DestroyDirectory( const GChar * strPathName )
{
    // Destroy all files / sub-directories (recurse)
    Bool bIsSubDir;
    GChar strName[FILENAME_LENGTH + 1];
    Bool bContinue = ListDirectoryFirst( strPathName, &bIsSubDir, strName, FILENAME_LENGTH );
    while( bContinue ) {
        GChar strSubPath[PATHNAME_LENGTH + 1];
        GChar * strPtr = StringFn->NCopy( strSubPath, strPathName, PATHNAME_LENGTH );
        strPtr = StringFn->NCopy( strPtr, TEXT("/"), PATHNAME_LENGTH - (strPtr - strSubPath) );
        strPtr = StringFn->NCopy( strPtr, strName, PATHNAME_LENGTH - (strPtr - strSubPath) );

        if ( bIsSubDir ) { // Recurse
            Bool bRes = DestroyDirectory( strSubPath );
            DebugAssert( bRes );
        } else {
            Bool bRes = DestroyFile( strSubPath );
            DebugAssert( bRes );
        }

        bContinue = ListDirectoryNext( &bIsSubDir, strName, FILENAME_LENGTH );
    }

    // Done
    BOOL bRes = RemoveDirectory( strPathName );
    return ( bRes != FALSE );
}

Bool System::ListDirectoryFirst( const GChar * strPathName, Bool * outIsSubDirectory, GChar * outFileName, UInt iMaxLength )
{
    if ( m_hDirectoryListing != NULL )
        return false;

    GChar strSearch[PATHNAME_LENGTH + 3];
    GChar * strPtr = strSearch;
    strPtr = StringFn->NCopy( strPtr, strPathName, PATHNAME_LENGTH );
    strPtr = StringFn->NCopy( strPtr, TEXT("/*"), PATHNAME_LENGTH + 2 - (strPtr - strSearch) );

    WIN32_FIND_DATA fileInfos;
    m_hDirectoryListing = FindFirstFile( strSearch, &fileInfos );
    if ( m_hDirectoryListing == INVALID_HANDLE_VALUE ) {
        m_hDirectoryListing = NULL;
        return false;
    }

    *outIsSubDirectory = ( (fileInfos.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 );
    StringFn->NCopy( outFileName, fileInfos.cFileName, iMaxLength );
    return true;
}
Bool System::ListDirectoryNext( Bool * outIsSubDirectory, GChar * outFileName, UInt iMaxLength )
{
    if ( m_hDirectoryListing == NULL )
        return false;

    WIN32_FIND_DATA fileInfos;
    BOOL bFound = FindNextFile( m_hDirectoryListing, &fileInfos );
    if ( bFound == FALSE ) {
        FindClose( m_hDirectoryListing );
        m_hDirectoryListing = NULL;
        return false;
    }

    *outIsSubDirectory = ( (fileInfos.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 );
    StringFn->NCopy( outFileName, fileInfos.cFileName, iMaxLength );
    return true;
}

Bool System::GetCurrentDirectory( GChar * outPathName, UInt iMaxLength ) const
{
#if ( defined(UNICODE) || defined (_UNICODE) )
    DWord dwLength = GetCurrentDirectoryW( iMaxLength, outPathName );
#else
    DWord dwLength = GetCurrentDirectoryA( iMaxLength, outPathName );
#endif
    return ( dwLength != 0 );
}
Bool System::SetCurrentDirectory( const GChar * strPathName ) const
{
#if ( defined(UNICODE) || defined (_UNICODE) )
    BOOL bRes = SetCurrentDirectoryW( strPathName );
#else
    BOOL bRes = SetCurrentDirectoryA( strPathName );
#endif
    return ( bRes != FALSE );
}

Bool System::MoveFile( const GChar * strDestPathName, const GChar * strSrcPathName, Bool bOverwrite ) const
{
    BOOL bRes = MoveFileEx( strSrcPathName, strDestPathName, bOverwrite ? MOVEFILE_REPLACE_EXISTING : 0 );
    return ( bRes != FALSE );
}
Bool System::CopyFile( const GChar * strDestPathName, const GChar * strSrcPathName, Bool bOverwrite ) const
{
#if ( defined(UNICODE) || defined (_UNICODE) )
    BOOL bRes = CopyFileW( strSrcPathName, strDestPathName, bOverwrite ? FALSE : TRUE );
#else
    BOOL bRes = CopyFileA( strSrcPathName, strDestPathName, bOverwrite ? FALSE : TRUE );
#endif
    return ( bRes != FALSE );
}
Bool System::DestroyFile( const GChar * strPathName ) const
{
    BOOL bRes = DeleteFile( strPathName );
    return ( bRes != FALSE );
}

Bool System::SearchFile( GChar * outPathName, UInt iMaxLength, const GChar * strSearchPath, const GChar * strSearchName ) const
{
    DWord dwLength = SearchPath( strSearchPath, strSearchName, NULL, iMaxLength, outPathName, NULL );
    return ( dwLength != 0 );
}

FileAttributes System::GetFileAttributes( const GChar * strPathName ) const
{
#if ( defined(UNICODE) || defined (_UNICODE) )
    DWord dwAttribs = GetFileAttributesW( strPathName );
#else
    DWord dwAttribs = GetFileAttributesA( strPathName );
#endif
    return HFile::_ConvertAttributes( dwAttribs );
}
Void System::SetFileAttributes( const GChar * strPathName, FileAttributes iAttributes ) const
{
    DWord dwAttribs = HFile::_RevertAttributes( iAttributes );
#if ( defined(UNICODE) || defined (_UNICODE) )
    SetFileAttributesW( strPathName, dwAttribs );
#else
    SetFileAttributesA( strPathName, dwAttribs );
#endif
}

Bool System::CreateHardLink( const GChar * strPathName, const GChar * strTargetPathName ) const
{
#if ( defined(UNICODE) || defined (_UNICODE) )
    BOOL bRes = CreateHardLinkW( strPathName, strTargetPathName, NULL );
#else
    BOOL bRes = CreateHardLinkA( strPathName, strTargetPathName, NULL );
#endif
    return ( bRes != FALSE );
}
//Bool System::CreateSymbolicLink( const GChar * strPathName, const GChar * strTargetPathName, Bool bIsDirectory ) const
//{
//#if ( defined(UNICODE) || defined (_UNICODE) )
//    BOOL bRes = CreateSymbolicLinkW( strPathName, strTargetPathName, bIsDirectory ? SYMBOLIC_LINK_FLAG_DIRECTORY : 0 );
//#else
//    BOOL bRes = CreateSymbolicLinkA( strPathName, strTargetPathName, bIsDirectory ? SYMBOLIC_LINK_FLAG_DIRECTORY : 0 );
//#endif
//    return ( bRes != FALSE );
//}

// Processes & Threads /////////////////////////////////////////
const GChar * System::GetCommandLine() const
{
    return ::GetCommandLine();
}
const GChar * System::GetEnvironmentString() const
{
    return ::GetEnvironmentStrings();
}
Void System::GetEnvironmentVar( const GChar * strName, GChar * outValue, UInt iMaxSize ) const
{
    DWord dwResult = ::GetEnvironmentVariable( strName, outValue, (DWord)iMaxSize );
    DebugAssert( dwResult != 0 );
}
Void System::SetEnvironmentVar( const GChar * strName, const GChar * strValue ) const
{
    BOOL bResult = ::SetEnvironmentVariable( strName, strValue );
    DebugAssert( bResult != FALSE );
}

HProcess System::GetCurrentProcess() const
{
    return HProcess( ::GetCurrentProcess() );
}
ProcessID System::GetCurrentProcessID() const
{
    return ::GetCurrentProcessId();
}

HThread System::GetCurrentThread() const
{
    return HThread( ::GetCurrentThread() );
}
ThreadID System::GetCurrentThreadID() const
{
    return ::GetCurrentThreadId();
}

WaitResult System::WaitOne( HThreadingObject * pObject, UInt iMilliseconds ) const
{
    DWord dwRes = WaitForSingleObject( pObject->m_hThreadingObject, (DWord)iMilliseconds );
    if ( dwRes == WAIT_OBJECT_0 )
        return WAIT_OK;
    if ( dwRes == WAIT_ABANDONED )
        return WAIT_ORPHANED;
    if ( dwRes == WAIT_TIMEOUT )
        return WAIT_EXPIRED;
    return WAIT_ERROR;
}
WaitResult System::WaitAll( HThreadingObject ** arrhObjects, UInt iCount, UInt iMilliseconds ) const
{
    static HANDLE s_arrHandles[WAIT_MAX_OBJECTS];
    DebugAssert( iCount < WAIT_MAX_OBJECTS );

    for( UInt i = 0; i < iCount; ++i )
        s_arrHandles[i] = arrhObjects[i]->m_hThreadingObject;

    DWord dwRes = WaitForMultipleObjects( (DWord)iCount, s_arrHandles, TRUE, (DWord)iMilliseconds );
    if ( WAIT_OBJECT_0 <= dwRes && dwRes < WAIT_OBJECT_0 + iCount )
        return WAIT_OK;
    if ( WAIT_ABANDONED_0 <= dwRes && dwRes < WAIT_ABANDONED_0 + iCount )
        return WAIT_ORPHANED;
    if ( dwRes == WAIT_TIMEOUT )
        return WAIT_EXPIRED;
    return WAIT_ERROR;
}
WaitResult System::WaitAny( HThreadingObject ** arrhObjects, UInt iCount, UInt iMilliseconds, HThreadingObject ** outObject ) const
{
    static HANDLE s_arrHandles[WAIT_MAX_OBJECTS];
    DebugAssert( iCount < WAIT_MAX_OBJECTS );

    for( UInt i = 0; i < iCount; ++i )
        s_arrHandles[i] = arrhObjects[i]->m_hThreadingObject;

    DWord dwRes = WaitForMultipleObjects( (DWord)iCount, s_arrHandles, FALSE, (DWord)iMilliseconds );
    if ( WAIT_OBJECT_0 <= dwRes && dwRes < WAIT_OBJECT_0 + iCount ) {
        if ( outObject != NULL )
            *outObject = arrhObjects[dwRes - WAIT_OBJECT_0];
        return WAIT_OK;
    }
    if ( WAIT_ABANDONED_0 <= dwRes && dwRes < WAIT_ABANDONED_0 + iCount ) {
        if ( outObject != NULL )
            *outObject = arrhObjects[dwRes - WAIT_ABANDONED_0];
        return WAIT_ORPHANED;
    }
    if ( dwRes == WAIT_TIMEOUT )
        return WAIT_EXPIRED;
    return WAIT_ERROR;
}

WaitResult System::WaitInputIdle( const HProcess & hProcess, UInt iMilliseconds ) const
{
    DWord dwResult = WaitForInputIdle( hProcess.m_hThreadingObject, (DWord)iMilliseconds );
    if ( dwResult == 0 )
        return WAIT_OK;
    if ( dwResult == WAIT_TIMEOUT )
        return WAIT_EXPIRED;
    return WAIT_ERROR;
}
Bool System::WaitInput() const
{
    return ( WaitMessage() != 0 );
}

WaitResult System::Sleep( UInt iMilliseconds, Bool bAwakable ) const
{
    DWord dwResult = SleepEx( (DWord)iMilliseconds, bAwakable ? TRUE : FALSE );
    if ( dwResult == 0 )
        return WAIT_OK;
    if ( dwResult == WAIT_IO_COMPLETION )
        return WAIT_IO_INTERRUPT;
    return WAIT_ERROR;
}
WaitResult System::YieldTime() const
{
    DWord dwResult = SleepEx( 0, FALSE );
    if ( dwResult == 0 )
        return WAIT_OK;
    if ( dwResult == WAIT_IO_COMPLETION )
        return WAIT_IO_INTERRUPT;
    return WAIT_ERROR;
}

// Networking //////////////////////////////////////////////////
Void System::NetworkInitialize() const
{
    WSADATA wsaData;
    Int iRes = WSAStartup( MAKEWORD(2,2), &wsaData );
    DebugAssert( iRes == 0 );
}
Void System::NetworkCleanup() const
{
    Int iRes = WSACleanup();
    DebugAssert( iRes == 0 );
}

Void System::GetMACAddress( Byte outMACAddress[8] ) const
{
    ULong iLength = 16384; // Recommended size is 15k, lets align this

    IP_ADAPTER_ADDRESSES * pAddresses = (IP_ADAPTER_ADDRESSES*)( _ScratchAlloc(iLength) );
    ULong ulRes = GetAdaptersAddresses( AF_INET, 0, NULL, pAddresses, &iLength );
    if ( ulRes == ERROR_BUFFER_OVERFLOW ) {
        _ScratchFree( pAddresses );

        pAddresses = (IP_ADAPTER_ADDRESSES*)( _ScratchAlloc(iLength) );
        ulRes = GetAdaptersAddresses( AF_INET, 0, NULL, pAddresses, &iLength );
    }
    DebugAssert( ulRes == ERROR_SUCCESS );
    DebugAssert( pAddresses->PhysicalAddressLength <= 8 );

    for( UInt i = 0; i < pAddresses->PhysicalAddressLength; ++i )
        outMACAddress[i] = pAddresses->PhysicalAddress[i];

    _ScratchFree( pAddresses );
}
Void System::GetHostName( GChar outHostName[256] ) const
{
#if ( defined(UNICODE) || defined(_UNICODE) )
    AChar strBuffer[256];
    Int iResult = gethostname( strBuffer, 255 );
    DebugAssert( iResult == 0 );
    StringFn->AsciiToWideChar( outHostName, strBuffer, 255 );
#else
    Int iResult = gethostname( outHostName, 255 );
    DebugAssert( iResult == 0 );
#endif
}

Void System::StringToNetAddress( NetAddress * outAddress, const GChar * strDottedFormat, NetAddressType iType ) const
{
    switch( iType ) {
        case NETADDRESS_IPv4: {
                NetAddressIPv4 * pAddr = (NetAddressIPv4*)outAddress;

                const GChar * strPort = StringFn->Find( strDottedFormat, TEXT(':') );
                DebugAssert( strPort != NULL );
                
                GChar strAddr[16];
                StringFn->NCopy( strAddr, strDottedFormat, (strPort - strDottedFormat) );
                ++strPort;

#if ( defined(UNICODE) || defined(_UNICODE) )
                AChar strTemp[16];
                StringFn->WideCharToMultiByte( strTemp, strAddr, 15 );
                DWord dwAddr = inet_addr( strTemp );
#else
                DWord dwAddr = inet_addr( strAddr );
#endif
                DebugAssert( dwAddr != INADDR_NONE );

                Word wPort = (Word)( StringFn->ToUInt(strPort) );

                pAddr->iType = NETADDRESS_IPv4;
                pAddr->wPort = htons( wPort );
                pAddr->Addr.Value = dwAddr;
            } break;
        case NETADDRESS_IPv6: {
                DebugAssert( false );
            } break;
        default: DebugAssert( false ); break;
    }
}
Void System::NetAddressToString( GChar * outString, const NetAddress * pAddress ) const
{
    switch( pAddress->iType ) {
        case NETADDRESS_IPv4: {
                const NetAddressIPv4 * pAddr = (const NetAddressIPv4*)pAddress;

                in_addr ipAddr;
                ipAddr.s_addr = pAddr->Addr.Value;
                const AChar * strAddr = inet_ntoa( ipAddr );
                DebugAssert( strAddr != NULL );

#if ( defined(UNICODE) || defined(_UNICODE) )
                WChar strTemp[16];
                StringFn->AsciiToWideChar( strTemp, strAddr, 15 );
                outString = StringFn->Copy( outString, strTemp );
#else
                outString = StringFn->Copy( outString, strAddr );
#endif

                *outString++ = ':';
                StringFn->FromUInt( outString, (UInt64)( ntohs(pAddr->wPort) ) );
            } break;
        case NETADDRESS_IPv6: {
                DebugAssert( false );
            } break;
        default: DebugAssert( false ); break;
    }
}

NetworkError System::ResolveNetAddress( GChar * outServiceName, GChar * outHostName, const NetAddress * pAddress,
                                        AddressResolveFlags iFlags ) const
{
    Int iNameInfoFlags = 0;
    if ( iFlags & ADDRRESOLVE_FLAG_NUMERIC_SERVICE )
        iNameInfoFlags |= NI_NUMERICSERV;
    if ( iFlags & ADDRRESOLVE_FLAG_NUMERIC_HOST )
        iNameInfoFlags |= NI_NUMERICHOST;
    if ( iFlags & ADDRRESOLVE_FLAG_RELATIVE_NAME )
        iNameInfoFlags |= NI_NOFQDN;
    if ( iFlags & ADDRRESOLVE_FLAG_REQUIRED_NAME )
        iNameInfoFlags |= NI_NAMEREQD;
    if ( iFlags & ADDRRESOLVE_FLAG_DATAGRAM )
        iNameInfoFlags |= NI_DGRAM;

    Int iResult;
    switch( pAddress->iType ) {
        case NETADDRESS_IPv4: {
                const NetAddressIPv4 * pAddr = (const NetAddressIPv4 *)pAddress;

                SOCKADDR_IN sockIPv4;
                MemZero( &sockIPv4, sizeof(SOCKADDR_IN) );
                sockIPv4.sin_family = AF_INET;
                sockIPv4.sin_addr.s_addr = pAddr->Addr.Value;
                sockIPv4.sin_port = pAddr->wPort;

                iResult = GetNameInfo( (const SOCKADDR*)( &sockIPv4 ), sizeof(SOCKADDR_IN),
                                       outHostName, NI_MAXHOST, outServiceName, NI_MAXSERV, iNameInfoFlags );
            } break;
        case NETADDRESS_IPv6: {
                const NetAddressIPv6 * pAddr = (const NetAddressIPv6 *)pAddress;

                SOCKADDR_IN6 sockIPv6;
                MemZero( &sockIPv6, sizeof(SOCKADDR_IN6) );
                sockIPv6.sin6_family = AF_INET6;
                sockIPv6.sin6_flowinfo = pAddr->dwFlowInfo;
                sockIPv6.sin6_scope_id = pAddr->dwScopeID;
                MemCopy( sockIPv6.sin6_addr.s6_addr, pAddr->Addr.Bytes, 16 );
                sockIPv6.sin6_port = pAddr->wPort;

                iResult = GetNameInfo( (const SOCKADDR*)( &sockIPv6 ), sizeof(SOCKADDR_IN6),
                                       outHostName, NI_MAXHOST, outServiceName, NI_MAXSERV, iNameInfoFlags );
            } break;
        default: return NETWORK_ERROR;
    }
    if ( iResult == 0 )
        return NETWORK_OK;

    iResult = WSAGetLastError();
    switch(iResult) {
        case WSAHOST_NOT_FOUND: return NETWORK_HOST_NOT_FOUND;
        case WSATRY_AGAIN:      return NETWORK_RETRY;
        case WSANOTINITIALISED:
        case WSAEAFNOSUPPORT:
        case WSAEINVAL:
        case WSAEFAULT:
        case WSANO_RECOVERY:
        default: break;
    }
    return NETWORK_ERROR;
}

NetworkError System::ResolveHostName( const GChar * strServiceName, const GChar * strHostName,
                                      SocketProtocol protocolHint, AddressResolveFlags iFlags )
{
    ADDRINFOT hints;
    MemZero( &hints, sizeof(ADDRINFOT) );
    switch( protocolHint ) {
        case SOCKET_UNSPECIFIED: hints.ai_family = AF_UNSPEC; hints.ai_socktype = 0;           hints.ai_protocol = 0; break;
        case SOCKET_ICMPv4:      hints.ai_family = AF_INET;   hints.ai_socktype = SOCK_RAW;    hints.ai_protocol = IPPROTO_ICMP; break;
        case SOCKET_IGMPv4:      hints.ai_family = AF_INET;   hints.ai_socktype = SOCK_RAW;    hints.ai_protocol = IPPROTO_IGMP; break;
        case SOCKET_UDPIPv4:     hints.ai_family = AF_INET;   hints.ai_socktype = SOCK_DGRAM;  hints.ai_protocol = IPPROTO_UDP; break;
        case SOCKET_TCPIPv4:     hints.ai_family = AF_INET;   hints.ai_socktype = SOCK_STREAM; hints.ai_protocol = IPPROTO_TCP; break;
        case SOCKET_ICMPv6:      hints.ai_family = AF_INET6;  hints.ai_socktype = SOCK_RAW;    hints.ai_protocol = IPPROTO_ICMPV6; break;
        case SOCKET_IGMPv6:      hints.ai_family = AF_INET6;  hints.ai_socktype = SOCK_RAW;    hints.ai_protocol = IPPROTO_IGMP; break;
        case SOCKET_UDPIPv6:     hints.ai_family = AF_INET6;  hints.ai_socktype = SOCK_DGRAM;  hints.ai_protocol = IPPROTO_UDP; break;
        case SOCKET_TCPIPv6:     hints.ai_family = AF_INET6;  hints.ai_socktype = SOCK_STREAM; hints.ai_protocol = IPPROTO_TCP; break;
        default: return NETWORK_ERROR;
    }
    if ( iFlags & ADDRRESOLVE_FLAG_SERVER_USAGE )
        hints.ai_flags |= AI_PASSIVE;
    if ( iFlags & ADDRRESOLVE_FLAG_NUMERIC_HOST )
        hints.ai_flags |= AI_NUMERICHOST;
    if ( iFlags & ADDRRESOLVE_FLAG_CANONICAL_NAME )
        hints.ai_flags |= AI_CANONNAME;
    // Many more flags supported with Vista/Seven ...

    Int iResult = GetAddrInfo( strHostName, strServiceName, &hints, &m_pCurAddrInfo );
    if ( iResult == 0 )
        return NETWORK_OK;

    m_pCurAddrInfo = NULL;
    iResult = WSAGetLastError();
    switch(iResult) {
        case WSAHOST_NOT_FOUND: return NETWORK_HOST_NOT_FOUND;
        case WSANO_DATA:        return NETWORK_NO_DATA;
        case WSATRY_AGAIN:      return NETWORK_RETRY;
        case WSANOTINITIALISED:
        case WSAEAFNOSUPPORT:
        case WSAESOCKTNOSUPPORT:
        case WSAEINVAL:
        case WSATYPE_NOT_FOUND:
        case WSA_NOT_ENOUGH_MEMORY:
        case WSANO_RECOVERY:
        default: break;
    }
    return NETWORK_ERROR;
}
Bool System::EnumResolvedNetAddresses( NetAddress * outAddress, SocketProtocol * outProtocol )
{
    if ( m_pCurAddrInfo == NULL )
        return false;

    *outProtocol = HSocket::_BuildSocketProtocol( m_pCurAddrInfo->ai_socktype,
                                                  m_pCurAddrInfo->ai_family,
                                                  m_pCurAddrInfo->ai_protocol );

    if ( (*outProtocol) & 0x80000000 ) {
        NetAddressIPv6 * outIPv6 = (NetAddressIPv6*)outAddress;
        outIPv6->iType = NETADDRESS_IPv6;

        SOCKADDR_IN6 * pAddr = (SOCKADDR_IN6*)( m_pCurAddrInfo->ai_addr );
        outIPv6->dwFlowInfo = pAddr->sin6_flowinfo;
        outIPv6->dwScopeID = pAddr->sin6_scope_id;
        MemCopy( outIPv6->Addr.Bytes, pAddr->sin6_addr.s6_addr, 16 );
        outIPv6->wPort = pAddr->sin6_port;
    } else {
        NetAddressIPv4 * outIPv4 = (NetAddressIPv4*)outAddress;
        outIPv4->iType = NETADDRESS_IPv4;

        SOCKADDR_IN * pAddr = (SOCKADDR_IN*)( m_pCurAddrInfo->ai_addr );
        outIPv4->Addr.Value = pAddr->sin_addr.s_addr;
        outIPv4->wPort = pAddr->sin_port;
    }

    m_pCurAddrInfo = m_pCurAddrInfo->ai_next;
    return ( m_pCurAddrInfo != NULL );
}

Word System::NetByteOrderW( Word iValue ) const
{
    return htons(iValue);
}
DWord System::NetByteOrderDW( DWord iValue ) const
{
    return htonl(iValue);
}
QWord System::NetByteOrderQW( QWord iValue ) const
{
    DWord dwLowRef = (DWord)( iValue & 0xffffffff );
    DWord dwLow =  htonl( dwLowRef );
    if (dwLow == dwLowRef)
        return iValue;
    DWord dwHigh = htonl( (DWord)( (iValue >> 32) & 0xffffffff ) );
    return ( ( ((QWord)dwLow) << 32 ) | (QWord)dwHigh );
}
Word System::HostByteOrderW( Word iValue ) const
{
    return ntohs(iValue);
}
DWord System::HostByteOrderDW( DWord iValue ) const
{
    return ntohl(iValue);
}
QWord System::HostByteOrderQW( QWord iValue ) const
{
    DWord dwLowRef = (DWord)( iValue & 0xffffffff );
    DWord dwLow =  ntohl( dwLowRef );
    if (dwLow == dwLowRef)
        return iValue;
    DWord dwHigh = ntohl( (DWord)( (iValue >> 32) & 0xffffffff ) );
    return ( ( ((QWord)dwLow) << 32 ) | (QWord)dwHigh );
}

/////////////////////////////////////////////////////////////////////////////////

// Memory allocation ///////////////////////////////////////////
Void * System::_ScratchAlloc( UInt iSize ) const
{
    return HeapAlloc( m_hDefaultHeap, 0, iSize );
}
Void System::_ScratchFree( Void * pMemory ) const
{
    BOOL bOk = HeapFree( m_hDefaultHeap, 0, pMemory );
    DebugAssert( bOk );
}

// Time ////////////////////////////////////////////////////////
Void System::_ConvertTimeDate( TimeDate * outTimeDate, const SYSTEMTIME * pSystemTime ) const
{
    outTimeDate->iYear = pSystemTime->wYear;
    outTimeDate->iMonth = (TimeMonth)( pSystemTime->wMonth );
    outTimeDate->iDay = pSystemTime->wDay;
    outTimeDate->iDayOfWeek = (TimeDay)( pSystemTime->wDayOfWeek );
    outTimeDate->iHours = pSystemTime->wHour;
    outTimeDate->iMinutes = pSystemTime->wMinute;
    outTimeDate->iSeconds = pSystemTime->wSecond;
    outTimeDate->iMilliseconds = pSystemTime->wMilliseconds;
}
Void System::_RevertTimeDate( SYSTEMTIME * outSystemTime, const TimeDate * pTimeDate ) const
{
    outSystemTime->wYear = pTimeDate->iYear;
    outSystemTime->wMonth = (Word)( pTimeDate->iMonth );
    outSystemTime->wDay = pTimeDate->iDay;
    outSystemTime->wDayOfWeek = (Word)( pTimeDate->iDayOfWeek );
    outSystemTime->wHour = pTimeDate->iHours;
    outSystemTime->wMinute = pTimeDate->iMinutes;
    outSystemTime->wSecond = pTimeDate->iSeconds;
    outSystemTime->wMilliseconds = pTimeDate->iMilliseconds;
}

TimeValue System::_ConvertTimeValue( const FILETIME * pFlatTime, TimeUnit iResolution ) const
{
    ULARGE_INTEGER iTmp;
    iTmp.LowPart = pFlatTime->dwLowDateTime;
    iTmp.HighPart = pFlatTime->dwHighDateTime;

    switch( iResolution ) {
        case TIMEUNIT_NANOSECONDS:  return ( iTmp.QuadPart * 100 );    
        case TIMEUNIT_MICROSECONDS: return ( iTmp.QuadPart / 10 );
        case TIMEUNIT_MILLISECONDS: return ( iTmp.QuadPart / 10000 );
        case TIMEUNIT_SECONDS:      return ( iTmp.QuadPart / 10000000 );
        default: DebugAssert( false ); return 0;
    }
}
Void System::_RevertTimeValue( FILETIME * outFlatTime, TimeValue iTimeValue, TimeUnit iResolution ) const
{
    ULARGE_INTEGER iTmp;
    switch( iResolution ) {
        case TIMEUNIT_NANOSECONDS:  iTmp.QuadPart = ( iTimeValue / 100 );      break;
        case TIMEUNIT_MICROSECONDS: iTmp.QuadPart = ( iTimeValue * 10 );       break;
        case TIMEUNIT_MILLISECONDS: iTmp.QuadPart = ( iTimeValue * 10000 );    break;
        case TIMEUNIT_SECONDS:      iTmp.QuadPart = ( iTimeValue * 10000000 ); break;
        default: DebugAssert( false ); break;
    }

    outFlatTime->dwLowDateTime = iTmp.LowPart;
    outFlatTime->dwHighDateTime = iTmp.HighPart;
}

Void System::_ConvertTimeZone( TimeZone * outTimeZone, const TIME_ZONE_INFORMATION * pTimeZoneInfos ) const
{
    outTimeZone->iBaseBias = pTimeZoneInfos->Bias;

    StringFn->NCopy( outTimeZone->strStandardName, pTimeZoneInfos->StandardName, 31 );
    outTimeZone->iStandardBias = pTimeZoneInfos->StandardBias;
    _ConvertTimeDate( &(outTimeZone->hStandardTime), &(pTimeZoneInfos->StandardDate) );

    StringFn->NCopy( outTimeZone->strDaylightName, pTimeZoneInfos->DaylightName, 31 );
    outTimeZone->iDaylightBias = pTimeZoneInfos->DaylightBias;
    _ConvertTimeDate( &(outTimeZone->hDaylightTime), &(pTimeZoneInfos->DaylightDate) );
}
Void System::_RevertTimeZone( TIME_ZONE_INFORMATION * outTimeZoneInfos, const TimeZone * pTimeZone ) const
{
    outTimeZoneInfos->Bias = pTimeZone->iBaseBias;

    StringFn->NCopy( outTimeZoneInfos->StandardName, pTimeZone->strStandardName, 31 );
    outTimeZoneInfos->StandardBias = pTimeZone->iStandardBias;
    _RevertTimeDate( &(outTimeZoneInfos->StandardDate), &(pTimeZone->hStandardTime) );

    StringFn->NCopy( outTimeZoneInfos->DaylightName, pTimeZone->strDaylightName, 31 );
    outTimeZoneInfos->DaylightBias = pTimeZone->iDaylightBias;
    _RevertTimeDate( &(outTimeZoneInfos->DaylightDate), &(pTimeZone->hDaylightTime) );
}

