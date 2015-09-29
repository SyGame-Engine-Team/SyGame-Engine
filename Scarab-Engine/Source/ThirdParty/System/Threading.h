/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/System/Threading.h
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
// Known Bugs : Access ThreadEvent, Mutex & Semaphore states
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_THIRDPARTY_SYSTEM_THREADING_H
#define SCARAB_THIRDPARTY_SYSTEM_THREADING_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Platform.h"

/////////////////////////////////////////////////////////////////////////////////
// Forward declarations for Win32
typedef void * HANDLE;

typedef unsigned long (__stdcall * PTHREAD_START_ROUTINE)( void * );
typedef PTHREAD_START_ROUTINE LPTHREAD_START_ROUTINE;

struct _RTL_CRITICAL_SECTION_DEBUG;

/////////////////////////////////////////////////////////////////////////////////
// Faked structs for Win32
typedef struct __RTL_CRITICAL_SECTION {
    _RTL_CRITICAL_SECTION_DEBUG * DebugInfo;
    long LockCount;
    long RecursionCount;
    HANDLE OwningThread;
    HANDLE LockSemaphore;
    __w64 unsigned long SpinCount;
} __RTL_CRITICAL_SECTION;

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
typedef DWord ProcessID;
typedef DWord ThreadID;

typedef LPTHREAD_START_ROUTINE HThreadRoutine;

typedef DWord ProcessCreationFlags;
#define PROCESS_DEFAULT     0x00000000
#define PROCESS_FULLSCREEN  0x80000000
#define PROCESS_SUSPENDED   0x40000000
#define PROCESS_NO_WINDOW   0x00000001
#define PROCESS_DETACHED    0x00000002
#define PROCESS_NEW_CONSOLE 0x00000003

typedef DWord ThreadingAccess;
#define ACCESS_NONE                   0x00000000
#define ACCESS_DUPLICATE              0xffffffff

#define ACCESS_COMMON_DELETE          0x80000000
#define ACCESS_COMMON_SYNCHRONIZE     0x40000000
#define ACCESS_COMMON_ALL             0xf0000000

#define ACCESS_PROCESS_QUERY          0x00000001
#define ACCESS_PROCESS_MODIFY         0x00000002
#define ACCESS_PROCESS_SUSPEND_RESUME 0x00000004
#define ACCESS_PROCESS_TERMINATE      0x00000008
#define ACCESS_PROCESS_CREATE_PROCESS 0x00000010
#define ACCESS_PROCESS_CREATE_THREAD  0x00000020
#define ACCESS_PROCESS_MEMORY_READ    0x00000040
#define ACCESS_PROCESS_MEMORY_WRITE   0x00000080
#define ACCESS_PROCESS_DUP_HANDLES    0x00000100
#define ACCESS_PROCESS_ALL           (0x00000fff | ACCESS_COMMON_ALL)

#define ACCESS_THREAD_QUERY           0x00001000
#define ACCESS_THREAD_MODIFY          0x00002000
#define ACCESS_THREAD_SUSPEND_RESUME  0x00004000
#define ACCESS_THREAD_TERMINATE       0x00008000
#define ACCESS_THREAD_ALL            (0x0000f000 | ACCESS_COMMON_ALL)

#define ACCESS_THREADEVENT_MODIFY     0x00010000
#define ACCESS_THREADEVENT_ALL       (0x000f0000 | ACCESS_COMMON_ALL)

#define ACCESS_SEMAPHORE_MODIFY       0x00100000
#define ACCESS_SEMAPHORE_ALL         (0x00f00000 | ACCESS_COMMON_ALL)

#define ACCESS_MUTEX_MODIFY           0x01000000
#define ACCESS_MUTEX_ALL             (0x0f000000 | ACCESS_COMMON_ALL)

enum ProcessPriority
{
    PROCESS_PRIORITY_IDLETIME = 0,
    PROCESS_PRIORITY_LOW,
    PROCESS_PRIORITY_DEFAULT,
    PROCESS_PRIORITY_HIGH,
    PROCESS_PRIORITY_VERYHIGH,
    PROCESS_PRIORITY_REALTIME
};
enum ThreadPriority
{
    THREAD_PRIORITY_IDLETIME = 0,
    THREAD_PRIORITY_VERYLOW,
    THREAD_PRIORITY_LOW,
    THREAD_PRIORITY_DEFAULT,
    THREAD_PRIORITY_HIGH,
    THREAD_PRIORITY_VERYHIGH,
    THREAD_PRIORITY_REALTIME
};

    // Prototypes
class HProcess;
class HThread;

/////////////////////////////////////////////////////////////////////////////////
// The HThreadingObject class
class HThreadingObject
{
public:
    virtual ~HThreadingObject();

    Void Close();

protected:
    // Delegate of system
    friend class System;
    HThreadingObject();

    // Helpers
    DWord _BuildThreadingAccess_Common( ThreadingAccess iAccessFlags ) const;

    // Handle
    HANDLE m_hThreadingObject;
};

/////////////////////////////////////////////////////////////////////////////////
// The HProcess class
class HProcess : public HThreadingObject
{
public:
    virtual ~HProcess();

    Void Terminate( DWord dwExitCode ) const;

    ProcessID GetID() const;
    DWord GetExitCode() const;

    ProcessPriority GetPriority() const;
    Void SetPriority( ProcessPriority iPriority ) const;

private:
    // Delegate of system
    friend class System;
    HProcess( ProcessCreationFlags iFlags, GChar * strCmdLine, Void * pEnvironment,
              HThread * outMainThread, ProcessID * outProcessID, ThreadID * outMainThreadID );
    HProcess( ProcessID iProcessID, ThreadingAccess iAccessFlags );
    HProcess( HANDLE hProcess );

    // Helpers
    friend class HThread;
    DWord _BuildThreadingAccess( ThreadingAccess iAccessFlags ) const;
};

/////////////////////////////////////////////////////////////////////////////////
// The HThread class
class HThread : public HThreadingObject
{
public:
    virtual ~HThread();

    Void Terminate( DWord dwExitCode ) const;

    Void Suspend( UInt * outPrevSuspendCount = NULL ) const;
    Void Resume( UInt * outPrevSuspendCount = NULL ) const;

    //ThreadID GetID() const;         // only for Vista/Seven
    //ProcessID GetProcessID() const; // only for Vista/Seven
    DWord GetExitCode() const;

    ThreadPriority GetPriority() const;
    Void SetPriority( ThreadPriority iPriority ) const;

private:
    // Delegate of system
    friend class System;
    HThread( HThreadRoutine pfEntryPoint, Void * pParameters, Bool bRun, ThreadID * outID );
    HThread( const HProcess & hHostProcess, HThreadRoutine pfEntryPoint, Void * pParameters, Bool bRun, ThreadID * outID );
    HThread( ThreadID iThreadID, ThreadingAccess iAccessFlags );
    HThread( HANDLE hThread );

    // Helpers
    friend class HProcess;
    DWord _BuildThreadingAccess( ThreadingAccess iAccessFlags ) const;
};

/////////////////////////////////////////////////////////////////////////////////
// The HThreadEvent class
class HThreadEvent : public HThreadingObject
{
public:
    ~HThreadEvent();

    Void Set() const;
    Void Reset() const;

private:
    // Delegate of system
    friend class System;
    HThreadEvent( Bool bSignaled, Bool bManualReset );
    HThreadEvent( const GChar * strName, Bool bSignaled, Bool bManualReset );
    HThreadEvent( const GChar * strName, ThreadingAccess iAccessFlags );

    // Helpers
    DWord _BuildThreadingAccess( ThreadingAccess iAccessFlags ) const;
};

/////////////////////////////////////////////////////////////////////////////////
// The HMutex class
class HMutex : public HThreadingObject
{
public:
    ~HMutex();

    Void Release() const;

private:
    // Delegate of system
    friend class System;
    HMutex( Bool bAcquire );
    HMutex( const GChar * strName, Bool bAcquire );
    HMutex( const GChar * strName, ThreadingAccess iAccessFlags );

    // Helpers
    DWord _BuildThreadingAccess( ThreadingAccess iAccessFlags ) const;
};

/////////////////////////////////////////////////////////////////////////////////
// The HSemaphore class
class HSemaphore : public HThreadingObject
{
public:
    ~HSemaphore();

    Void Release( UInt iCount, UInt * outPrevCount = NULL ) const;

private:
    // Delegate of system
    friend class System;
    HSemaphore( UInt iInitCount, UInt iMaxCount );
    HSemaphore( const GChar * strName, UInt iInitCount, UInt iMaxCount );
    HSemaphore( const GChar * strName, ThreadingAccess iAccessFlags );

    // Helpers
    DWord _BuildThreadingAccess( ThreadingAccess iAccessFlags ) const;
};

/////////////////////////////////////////////////////////////////////////////////
// The HCriticalSection class
class HCriticalSection
{
public:
    ~HCriticalSection();

    Void Destroy();

    Bool Enter( Bool bWait = true );
    Void Leave();

private:
    // Delegate of system
    friend class System;
    HCriticalSection( UInt iSpinCount );

    // Handle
    __RTL_CRITICAL_SECTION m_hCriticalSection;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Threading.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_THIRDPARTY_SYSTEM_THREADING_H
