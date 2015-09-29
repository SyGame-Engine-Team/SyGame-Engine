/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/System/Threading.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Multiple processes & threads API
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
#include "Threading.h"

/////////////////////////////////////////////////////////////////////////////////
// HThreadingObject implementation
HThreadingObject::HThreadingObject()
{
    m_hThreadingObject = NULL;
}
HThreadingObject::~HThreadingObject()
{
    // nothing to do
    // Must call Close explicitly ...
}

Void HThreadingObject::Close()
{
    BOOL bRes = CloseHandle( m_hThreadingObject );
    DebugAssert( bRes != FALSE );

    m_hThreadingObject = NULL;
}

//HThreadingObject HThreadingObject::Duplicate( const HProcess & hTargetProcess, const HProcess & hSourceProcess, ThreadingAccess iAccessFlags ) const
//{
//    DWord dwOptions = 0;
//    DWord dwWinAccess = 0;
//
//    if ( iAccessFlags == ACCESS_DUPLICATE )
//        dwOptions |= DUPLICATE_SAME_ACCESS;
//    else if ( iAccessFlags != ACCESS_NONE ) {
//        dwWinAccess = ( iAccessFlags & 0x0fffffff ); // non-common part
//        if ( dwWinAccess & ACCESS_PROCESS_ALL )
//            dwWinAccess = _BuildThreadingAccess_Process( iAccessFlags );
//        else if ( dwWinAccess & ACCESS_THREAD_ALL )
//            dwWinAccess = _BuildThreadingAccess_Thread( iAccessFlags );
//        else if ( dwWinAccess & ACCESS_THREADEVENT_ALL )
//            dwWinAccess = _BuildThreadingAccess_ThreadEvent( iAccessFlags );
//        else if ( dwWinAccess & ACCESS_SEMAPHORE_ALL )
//            dwWinAccess = _BuildThreadingAccess_Semaphore( iAccessFlags );
//        else if ( dwWinAccess & ACCESS_MUTEX_ALL )
//            dwWinAccess = _BuildThreadingAccess_Mutex( iAccessFlags );
//        else
//            dwWinAccess = _BuildThreadingAccess_Common( iAccessFlags );
//    }
//
//    HANDLE hDuplicated;
//    BOOL bRes = DuplicateHandle( hSourceProcess.m_hThreadingObject, m_hThreadingObject, hTargetProcess.m_hThreadingObject,
//                                 &hDuplicated, dwWinAccess, FALSE, dwOptions );
//    DebugAssert( bRes != FALSE );
//
//    return HThreadingObject( hDuplicated );
//}

/////////////////////////////////////////////////////////////////////////////////

DWord HThreadingObject::_BuildThreadingAccess_Common( ThreadingAccess iAccessFlags ) const
{
    DWord dwWinAccess = 0;

    if ( iAccessFlags & ACCESS_COMMON_DELETE )
        dwWinAccess |= DELETE;
    if ( iAccessFlags & ACCESS_COMMON_SYNCHRONIZE )
        dwWinAccess |= SYNCHRONIZE;

    return dwWinAccess;
}

/////////////////////////////////////////////////////////////////////////////////
// HProcess implementation
HProcess::HProcess( ProcessCreationFlags iFlags, GChar * strCmdLine, Void * pEnvironment,
                    HThread * outMainThread, ProcessID * outProcessID, ThreadID * outMainThreadID ):
    HThreadingObject()
{
    STARTUPINFO startupInfos;
    MemZero( &startupInfos, sizeof(STARTUPINFO) );
    startupInfos.cb = sizeof(STARTUPINFO);
    if ( iFlags & PROCESS_FULLSCREEN )
        startupInfos.dwFlags |= STARTF_RUNFULLSCREEN;

    DWord dwFlags = 0;
    if ( iFlags & PROCESS_SUSPENDED ) // main thread is suspended upon creation
        dwFlags |= CREATE_SUSPENDED;
    if ( iFlags & PROCESS_NO_WINDOW ) // no console window
        dwFlags |= CREATE_NO_WINDOW;
    else {
        if ( iFlags & PROCESS_DETACHED ) // new console instead of parent's
            dwFlags |= DETACHED_PROCESS;
        else if ( iFlags & PROCESS_NEW_CONSOLE ) // don't inherit parent's console, create manually later (or not)
            dwFlags |= CREATE_NEW_CONSOLE;
    }

    PROCESS_INFORMATION processInfos;
    MemZero( &processInfos, sizeof(PROCESS_INFORMATION) );

    BOOL bRes = CreateProcess( NULL, strCmdLine, NULL, NULL, FALSE, dwFlags, pEnvironment, NULL, &startupInfos, &processInfos );
    DebugAssert( bRes );

    m_hThreadingObject = processInfos.hProcess;
    if ( outMainThread != NULL )
        outMainThread->m_hThreadingObject = processInfos.hThread;
    if ( outProcessID != NULL )
        *outProcessID = processInfos.dwProcessId;
    if ( outMainThreadID != NULL )
        *outMainThreadID = processInfos.dwThreadId;
}
HProcess::HProcess( ProcessID iProcessID, ThreadingAccess iAccessFlags ):
    HThreadingObject()
{
    m_hThreadingObject = OpenProcess( _BuildThreadingAccess(iAccessFlags), FALSE, iProcessID );
    DebugAssert( m_hThreadingObject != NULL );
}
HProcess::HProcess( HANDLE hProcess )
{
    m_hThreadingObject = hProcess;
}
HProcess::~HProcess()
{
    // nothing to do
}

Void HProcess::Terminate( DWord dwExitCode ) const
{
    BOOL bRes = TerminateProcess( m_hThreadingObject, (UINT)dwExitCode );
    DebugAssert( bRes != FALSE );
}

ProcessID HProcess::GetID() const
{
    DWord dwProcessID = GetProcessId( m_hThreadingObject );
    DebugAssert( dwProcessID != 0 );
    return dwProcessID;
}
DWord HProcess::GetExitCode() const
{
    DWord dwExitCode = 0;
    BOOL bRes = GetExitCodeProcess( m_hThreadingObject, &dwExitCode );
    DebugAssert( bRes != FALSE );
    return dwExitCode;
}

ProcessPriority HProcess::GetPriority() const
{
    DWord dwWinPriority = GetPriorityClass( m_hThreadingObject );
    DebugAssert( dwWinPriority != 0 );

    switch( dwWinPriority ) {
        case IDLE_PRIORITY_CLASS:         return PROCESS_PRIORITY_IDLETIME;
        case BELOW_NORMAL_PRIORITY_CLASS: return PROCESS_PRIORITY_LOW;
        case NORMAL_PRIORITY_CLASS:       return PROCESS_PRIORITY_DEFAULT;
        case ABOVE_NORMAL_PRIORITY_CLASS: return PROCESS_PRIORITY_HIGH;
        case HIGH_PRIORITY_CLASS:         return PROCESS_PRIORITY_VERYHIGH;
        case REALTIME_PRIORITY_CLASS:     return PROCESS_PRIORITY_REALTIME;
        default: DebugAssert( false ); return PROCESS_PRIORITY_DEFAULT;
    }
}
Void HProcess::SetPriority( ProcessPriority iPriority ) const
{
    DWord dwWinPriority;
    switch( iPriority ) {
        case PROCESS_PRIORITY_IDLETIME: dwWinPriority = IDLE_PRIORITY_CLASS;          break;
        case PROCESS_PRIORITY_LOW:      dwWinPriority = BELOW_NORMAL_PRIORITY_CLASS;  break;
        case PROCESS_PRIORITY_DEFAULT:  dwWinPriority = NORMAL_PRIORITY_CLASS;        break;
        case PROCESS_PRIORITY_HIGH:     dwWinPriority = ABOVE_NORMAL_PRIORITY_CLASS;  break;
        case PROCESS_PRIORITY_VERYHIGH: dwWinPriority = HIGH_PRIORITY_CLASS;          break;
        case PROCESS_PRIORITY_REALTIME: dwWinPriority = REALTIME_PRIORITY_CLASS;      break;
        default: DebugAssert( false ); dwWinPriority = NORMAL_PRIORITY_CLASS; break;
    }

    BOOL bRes = SetPriorityClass( m_hThreadingObject, dwWinPriority );
    DebugAssert( bRes != FALSE );
}

/////////////////////////////////////////////////////////////////////////////////

DWord HProcess::_BuildThreadingAccess( ThreadingAccess iAccessFlags ) const
{
    if ( iAccessFlags == ACCESS_PROCESS_ALL )
        return PROCESS_ALL_ACCESS;

    DWord dwWinAccess = _BuildThreadingAccess_Common( iAccessFlags );

    if ( iAccessFlags & ACCESS_PROCESS_QUERY )
        dwWinAccess |= PROCESS_QUERY_INFORMATION;
    if ( iAccessFlags & ACCESS_PROCESS_MODIFY )
        dwWinAccess |= PROCESS_SET_INFORMATION;
    if ( iAccessFlags & ACCESS_PROCESS_SUSPEND_RESUME )
        dwWinAccess |= PROCESS_SUSPEND_RESUME;
    if ( iAccessFlags & ACCESS_PROCESS_TERMINATE )
        dwWinAccess |= PROCESS_TERMINATE;
    if ( iAccessFlags & ACCESS_PROCESS_CREATE_PROCESS )
        dwWinAccess |= PROCESS_CREATE_PROCESS;
    if ( iAccessFlags & ACCESS_PROCESS_CREATE_THREAD )
        dwWinAccess |= PROCESS_CREATE_THREAD;
    if ( iAccessFlags & ACCESS_PROCESS_MEMORY_READ )
        dwWinAccess |= ( PROCESS_VM_OPERATION | PROCESS_VM_READ );
    if ( iAccessFlags & ACCESS_PROCESS_MEMORY_WRITE )
        dwWinAccess |= ( PROCESS_VM_OPERATION | PROCESS_VM_WRITE );
    if ( iAccessFlags & ACCESS_PROCESS_DUP_HANDLES )
        dwWinAccess |= PROCESS_DUP_HANDLE;

    return dwWinAccess;
}

/////////////////////////////////////////////////////////////////////////////////
// HThread implementation
HThread::HThread( HThreadRoutine pfEntryPoint, Void * pParameters, Bool bRun, ThreadID * outID ):
    HThreadingObject()
{
    m_hThreadingObject = CreateThread( NULL, 0, pfEntryPoint, pParameters,
                                       bRun ? 0 : CREATE_SUSPENDED, outID );
    DebugAssert( m_hThreadingObject != NULL );
}
HThread::HThread( const HProcess & hHostProcess, HThreadRoutine pfEntryPoint, Void * pParameters, Bool bRun, ThreadID * outID ):
    HThreadingObject()
{
    m_hThreadingObject = CreateRemoteThread( hHostProcess.m_hThreadingObject, NULL, 0, pfEntryPoint, pParameters,
                                                                              bRun ? 0 : CREATE_SUSPENDED, outID );
    DebugAssert( m_hThreadingObject != NULL );
}
HThread::HThread( ThreadID iThreadID, ThreadingAccess iAccessFlags ):
    HThreadingObject()
{
    m_hThreadingObject = OpenThread( _BuildThreadingAccess(iAccessFlags), FALSE, iThreadID );
    DebugAssert( m_hThreadingObject != NULL );
}
HThread::HThread( HANDLE hThread )
{
    m_hThreadingObject = hThread;
}
HThread::~HThread()
{
    // nothing to do
}

Void HThread::Terminate( DWord dwExitCode ) const
{
    BOOL bRes = TerminateThread( m_hThreadingObject, (UINT)dwExitCode );
    DebugAssert( bRes != FALSE );
}

Void HThread::Suspend( UInt * outPrevSuspendCount ) const
{
    UInt iCount = SuspendThread( m_hThreadingObject );
    DebugAssert( iCount != INVALID_OFFSET );

    if ( outPrevSuspendCount != NULL )
        *outPrevSuspendCount = iCount;
}
Void HThread::Resume( UInt * outPrevSuspendCount ) const
{
    UInt iCount = ResumeThread( m_hThreadingObject );
    DebugAssert( iCount != INVALID_OFFSET );

    if ( outPrevSuspendCount != NULL  )
        *outPrevSuspendCount = iCount;
}

DWord HThread::GetExitCode() const
{
    DWord dwExitCode = 0;
    BOOL bRes = GetExitCodeThread( m_hThreadingObject, &dwExitCode );
    DebugAssert( bRes != FALSE );
    return dwExitCode;
}

ThreadPriority HThread::GetPriority() const
{
    Int iWinPriority = GetThreadPriority( m_hThreadingObject );
    DebugAssert( iWinPriority != THREAD_PRIORITY_ERROR_RETURN );

    switch( iWinPriority ) {
        case THREAD_PRIORITY_IDLE:          return THREAD_PRIORITY_IDLETIME;
        case THREAD_PRIORITY_LOWEST:        return THREAD_PRIORITY_VERYLOW;
        case THREAD_PRIORITY_BELOW_NORMAL:  return THREAD_PRIORITY_LOW;
        case THREAD_PRIORITY_NORMAL:        return THREAD_PRIORITY_DEFAULT;
        case THREAD_PRIORITY_ABOVE_NORMAL:  return THREAD_PRIORITY_HIGH;
        case THREAD_PRIORITY_HIGHEST:       return THREAD_PRIORITY_VERYHIGH;
        case THREAD_PRIORITY_TIME_CRITICAL: return THREAD_PRIORITY_REALTIME;
        default: DebugAssert( false ); return THREAD_PRIORITY_DEFAULT;
    }
}
Void HThread::SetPriority( ThreadPriority iPriority ) const
{
    Int iWinPriority;
    switch( iPriority ) {
        case THREAD_PRIORITY_IDLETIME: iWinPriority = THREAD_PRIORITY_IDLE;          break;
        case THREAD_PRIORITY_VERYLOW:  iWinPriority = THREAD_PRIORITY_LOWEST;        break;
        case THREAD_PRIORITY_LOW:      iWinPriority = THREAD_PRIORITY_BELOW_NORMAL;  break;
        case THREAD_PRIORITY_DEFAULT:  iWinPriority = THREAD_PRIORITY_NORMAL;        break;
        case THREAD_PRIORITY_HIGH:     iWinPriority = THREAD_PRIORITY_ABOVE_NORMAL;  break;
        case THREAD_PRIORITY_VERYHIGH: iWinPriority = THREAD_PRIORITY_HIGHEST;       break;
        case THREAD_PRIORITY_REALTIME: iWinPriority = THREAD_PRIORITY_TIME_CRITICAL; break;
        default: DebugAssert( false ); iWinPriority = THREAD_PRIORITY_NORMAL; break;
    }

    BOOL bRes = SetThreadPriority( m_hThreadingObject, iWinPriority );
    DebugAssert( bRes != FALSE );
}

/////////////////////////////////////////////////////////////////////////////////

DWord HThread::_BuildThreadingAccess( ThreadingAccess iAccessFlags ) const
{
    if ( iAccessFlags == ACCESS_THREAD_ALL )
        return THREAD_ALL_ACCESS;

    DWord dwWinAccess = _BuildThreadingAccess_Common( iAccessFlags );

    if ( iAccessFlags & ACCESS_THREAD_QUERY )
        dwWinAccess |= THREAD_QUERY_INFORMATION;
    if ( iAccessFlags & ACCESS_THREAD_MODIFY )
        dwWinAccess |= THREAD_SET_INFORMATION;
    if ( iAccessFlags & ACCESS_THREAD_SUSPEND_RESUME )
        dwWinAccess |= THREAD_SUSPEND_RESUME;
    if ( iAccessFlags & ACCESS_THREAD_TERMINATE )
        dwWinAccess |= THREAD_TERMINATE;

    return dwWinAccess;
}

/////////////////////////////////////////////////////////////////////////////////
// HThreadEvent implementation
HThreadEvent::HThreadEvent( Bool bSignaled, Bool bManualReset ):
    HThreadingObject()
{
    m_hThreadingObject = CreateEvent( NULL, bManualReset ? TRUE : FALSE, bSignaled ? TRUE : FALSE, NULL );
    DebugAssert( m_hThreadingObject != NULL );
}
HThreadEvent::HThreadEvent( const GChar * strName, Bool bSignaled, Bool bManualReset ):
    HThreadingObject()
{
    m_hThreadingObject = CreateEvent( NULL, bManualReset ? TRUE : FALSE, bSignaled ? TRUE : FALSE, strName );
    DebugAssert( m_hThreadingObject != NULL );
}
HThreadEvent::HThreadEvent( const GChar * strName, ThreadingAccess iAccessFlags ):
    HThreadingObject()
{
    m_hThreadingObject = OpenEvent( _BuildThreadingAccess(iAccessFlags), FALSE, strName );
    DebugAssert( m_hThreadingObject != NULL );
}
HThreadEvent::~HThreadEvent()
{
    // nothing to do
}

Void HThreadEvent::Set() const
{
    BOOL bRes = SetEvent( m_hThreadingObject );
    DebugAssert( bRes != FALSE );
}
Void HThreadEvent::Reset() const
{
    BOOL bRes = ResetEvent( m_hThreadingObject );
    DebugAssert( bRes != FALSE );
}

/////////////////////////////////////////////////////////////////////////////////

DWord HThreadEvent::_BuildThreadingAccess( ThreadingAccess iAccessFlags ) const
{
    if ( iAccessFlags == ACCESS_THREADEVENT_ALL )
        return EVENT_ALL_ACCESS;

    DWord dwWinAccess = _BuildThreadingAccess_Common( iAccessFlags );

    if ( iAccessFlags & ACCESS_THREADEVENT_MODIFY )
        dwWinAccess |= EVENT_MODIFY_STATE;

    return dwWinAccess;
}

/////////////////////////////////////////////////////////////////////////////////
// HMutex implementation
HMutex::HMutex( Bool bAcquire ):
    HThreadingObject()
{
    m_hThreadingObject = CreateMutex( NULL, bAcquire ? TRUE : FALSE, NULL );
    DebugAssert( m_hThreadingObject != NULL );
}
HMutex::HMutex( const GChar * strName, Bool bAcquire ):
    HThreadingObject()
{
    m_hThreadingObject = CreateMutex( NULL, bAcquire ? TRUE : FALSE, strName );
    DebugAssert( m_hThreadingObject != NULL );
}
HMutex::HMutex( const GChar * strName, ThreadingAccess iAccessFlags ):
    HThreadingObject()
{
    m_hThreadingObject = OpenMutex( _BuildThreadingAccess(iAccessFlags), FALSE, strName );
    DebugAssert( m_hThreadingObject != NULL );
}
HMutex::~HMutex()
{
    // nothing to do
}

Void HMutex::Release() const
{
    BOOL bRes = ReleaseMutex( m_hThreadingObject );
    DebugAssert( bRes != FALSE );
}

/////////////////////////////////////////////////////////////////////////////////

DWord HMutex::_BuildThreadingAccess( ThreadingAccess iAccessFlags ) const
{
    if ( iAccessFlags == ACCESS_MUTEX_ALL )
        return MUTEX_ALL_ACCESS;

    DWord dwWinAccess = _BuildThreadingAccess_Common( iAccessFlags );

    if ( iAccessFlags & ACCESS_MUTEX_MODIFY )
        dwWinAccess |= MUTEX_MODIFY_STATE;

    return dwWinAccess;
}

/////////////////////////////////////////////////////////////////////////////////
// HSemaphore implementation
HSemaphore::HSemaphore( UInt iInitCount, UInt iMaxCount ):
    HThreadingObject()
{
    m_hThreadingObject = CreateSemaphore( NULL, (LONG)iInitCount, (LONG)iMaxCount, NULL );
    DebugAssert( m_hThreadingObject != NULL );
}
HSemaphore::HSemaphore( const GChar * strName, UInt iInitCount, UInt iMaxCount ):
    HThreadingObject()
{
    m_hThreadingObject = CreateSemaphore( NULL, (LONG)iInitCount, (LONG)iMaxCount, strName );
    DebugAssert( m_hThreadingObject != NULL );
}
HSemaphore::HSemaphore( const GChar * strName, ThreadingAccess iAccessFlags ):
    HThreadingObject()
{
    m_hThreadingObject = OpenSemaphore( _BuildThreadingAccess(iAccessFlags), FALSE, strName );
    DebugAssert( m_hThreadingObject != NULL );
}
HSemaphore::~HSemaphore()
{
    // nothing to do
}

Void HSemaphore::Release( UInt iCount, UInt * outPrevCount ) const
{
    BOOL bRes = ReleaseSemaphore( m_hThreadingObject, (LONG)iCount, (LPLONG)outPrevCount );
    DebugAssert( bRes != FALSE );
}

/////////////////////////////////////////////////////////////////////////////////

DWord HSemaphore::_BuildThreadingAccess( ThreadingAccess iAccessFlags ) const
{
    if ( iAccessFlags == ACCESS_SEMAPHORE_ALL )
        return SEMAPHORE_ALL_ACCESS;

    DWord dwWinAccess = _BuildThreadingAccess_Common( iAccessFlags );

    if ( iAccessFlags & ACCESS_SEMAPHORE_MODIFY )
        dwWinAccess |= SEMAPHORE_MODIFY_STATE;

    return dwWinAccess;
}

/////////////////////////////////////////////////////////////////////////////////
// HCriticalSection implementation
HCriticalSection::HCriticalSection( UInt iSpinCount )
{
    if ( iSpinCount != 0 ) {
        BOOL bRes = InitializeCriticalSectionAndSpinCount( (RTL_CRITICAL_SECTION*)&m_hCriticalSection, (DWord)iSpinCount );
        DebugAssert( bRes != FALSE );
    } else
        InitializeCriticalSection( (RTL_CRITICAL_SECTION*)&m_hCriticalSection );
}
HCriticalSection::~HCriticalSection()
{
    // nothing to do
    // Must call Destroy explicitly ...
}

Void HCriticalSection::Destroy()
{
    DeleteCriticalSection( (RTL_CRITICAL_SECTION*)&m_hCriticalSection );
}

Bool HCriticalSection::Enter( Bool bWait )
{
    if ( bWait ) {
        EnterCriticalSection( (RTL_CRITICAL_SECTION*)&m_hCriticalSection );
        return true;
    }
    return ( TryEnterCriticalSection( (RTL_CRITICAL_SECTION*)&m_hCriticalSection ) != FALSE );
}
Void HCriticalSection::Leave()
{
    LeaveCriticalSection( (RTL_CRITICAL_SECTION*)&m_hCriticalSection );
}

