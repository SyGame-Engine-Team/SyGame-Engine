/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/System/System.inl
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
// System implementation
inline System * System::GetInstancePtr() {
    static System s_Instance;
    return &s_Instance;
}

// File IO /////////////////////////////////////////////////////
inline HFile System::CreateFile( const GChar * strPathName, FileMode iMode, FileFlags iFlags ) {
    return HFile( strPathName, iMode, iFlags, true );
}
inline HFile System::OpenFile( const GChar * strPathName, FileMode iMode, FileFlags iFlags ) {
    return HFile( strPathName, iMode, iFlags, false );
}

// Processes & Threads /////////////////////////////////////////
inline HProcess System::CreateProcess( ProcessCreationFlags iFlags, GChar * strCmdLine, Void * pEnvironment,
                                       HThread * outMainThread, ProcessID * outProcessID, ThreadID * outMainThreadID ) const {
    return HProcess( iFlags, strCmdLine, pEnvironment, outMainThread, outProcessID, outMainThreadID );
}
inline HProcess System::OpenProcess( ProcessID iProcessID, ThreadingAccess iAccessFlags ) const {
    return HProcess( iProcessID, iAccessFlags );
}

inline HThread System::CreateThread( HThreadRoutine pfEntryPoint, Void * pParameters, Bool bRun, ThreadID * outID ) const {
    return HThread( pfEntryPoint, pParameters, bRun, outID );
}
inline HThread System::CreateThread( const HProcess & hHostProcess, HThreadRoutine pfEntryPoint, Void * pParameters, Bool bRun,
                                    ThreadID * outID ) const {
    return HThread( hHostProcess, pfEntryPoint, pParameters, bRun, outID );
}
inline HThread System::OpenThread( ThreadID iThreadID, ThreadingAccess iAccessFlags ) const {
    return HThread( iThreadID, iAccessFlags );
}

inline HThreadEvent System::CreateThreadEvent( Bool bSignaled, Bool bManualReset ) const {
    return HThreadEvent( bSignaled, bManualReset );
}
inline HThreadEvent System::CreateThreadEvent( const GChar * strName, Bool bSignaled, Bool bManualReset ) const {
    return HThreadEvent( strName, bSignaled, bManualReset );
}
inline HThreadEvent System::OpenThreadEvent( const GChar * strName, ThreadingAccess iAccessFlags ) const {
    return HThreadEvent( strName, iAccessFlags );
}

inline HMutex System::CreateMutex( Bool bAcquire ) const {
    return HMutex( bAcquire );
}
inline HMutex System::CreateMutex( const GChar * strName, Bool bAcquire ) const {
    return HMutex( strName, bAcquire );
}
inline HMutex System::OpenMutex( const GChar * strName, ThreadingAccess iAccessFlags ) const {
    return HMutex( strName, iAccessFlags );
}

inline HSemaphore System::CreateSemaphore( UInt iInitCount, UInt iMaxCount ) const {
    return HSemaphore( iInitCount, iMaxCount );
}
inline HSemaphore System::CreateSemaphore( const GChar * strName, UInt iInitCount, UInt iMaxCount ) const {
    return HSemaphore( strName, iInitCount, iMaxCount );
}
inline HSemaphore System::OpenSemaphore( const GChar * strName, ThreadingAccess iAccessFlags ) const {
    return HSemaphore( strName, iAccessFlags );
}

inline HCriticalSection System::CreateCriticalSection( UInt iSpinCount ) const {
    return HCriticalSection( iSpinCount );
}

// Networking //////////////////////////////////////////////////
inline HSocket System::CreateSocket( SocketProtocol iProtocol ) const {
    return HSocket( iProtocol );
}

inline Void System::NetByteOrder( AddrIPv4 * pAddr ) const {
    pAddr->Value = NetByteOrderDW( pAddr->Value );
}
inline Void System::HostByteOrder( AddrIPv4 * pAddr ) const {
    pAddr->Value = HostByteOrderDW( pAddr->Value );
}

