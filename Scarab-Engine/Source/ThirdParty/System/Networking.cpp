/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/System/Networking.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Networking socket API
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

#include <winsock2.h>
#include <ws2tcpip.h>

#undef DebugAssert

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Networking.h"

/////////////////////////////////////////////////////////////////////////////////
// HSocket implementation
HSocket::HSocket( SocketProtocol iProtocol )
{
    m_hSocket = INVALID_SOCKET;
    switch( iProtocol ) {
        case SOCKET_ICMPv4:  m_hSocket = socket( AF_INET, SOCK_RAW, IPPROTO_ICMP ); break;
        case SOCKET_IGMPv4:  m_hSocket = socket( AF_INET, SOCK_RAW, IPPROTO_IGMP ); break;
        case SOCKET_UDPIPv4: m_hSocket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP ); break;
        case SOCKET_TCPIPv4: m_hSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP ); break;
        case SOCKET_ICMPv6:  m_hSocket = socket( AF_INET6, SOCK_RAW, IPPROTO_ICMPV6 ); break;
        case SOCKET_IGMPv6:  m_hSocket = socket( AF_INET6, SOCK_RAW, IPPROTO_IGMP ); break;
        case SOCKET_UDPIPv6: m_hSocket = socket( AF_INET6, SOCK_DGRAM, IPPROTO_UDP ); break;
        case SOCKET_TCPIPv6: m_hSocket = socket( AF_INET6, SOCK_STREAM, IPPROTO_TCP ); break;
        default: DebugAssert( false ); break;
    }
}
HSocket::~HSocket()
{
    // nothing to do
    // Must call Close explicitly ...
}

Void HSocket::Shutdown( SocketShutdownType iType ) const
{
    Int iRes = shutdown( m_hSocket, (Int)iType );
    DebugAssert( iRes == 0 );
}
Void HSocket::Close()
{
    Int iRes = closesocket( m_hSocket );
    DebugAssert( iRes == 0 );

    m_hSocket = INVALID_SOCKET;
}

NetworkError HSocket::GetOption( SocketOption iOption, Void * outValue ) const
{
    Int iLevel, iOptionName, iOptionLength;
    _BuildSocketOption( &iLevel, &iOptionName, &iOptionLength, iOption );

    Int iResult = getsockopt( m_hSocket, iLevel, iOptionName, (Char*)outValue, &iOptionLength );
    if ( iResult == 0 )
        return NETWORK_OK;

    if ( iResult == SOCKET_ERROR ) {
        iResult = WSAGetLastError();
        switch( iResult ) {
            case WSAEINPROGRESS: return NETWORK_BUSY;
            case WSANOTINITIALISED:
            case WSAENOTSOCK:
            case WSAEINVAL:
            case WSAEFAULT:
            case WSAENOPROTOOPT:
            case WSAENETDOWN:
            default: break;
        }
    }
    return NETWORK_ERROR;
}
NetworkError HSocket::SetOption( SocketOption iOption, const Void * pValue ) const
{
    Int iLevel, iOptionName, iOptionLength;
    _BuildSocketOption( &iLevel, &iOptionName, &iOptionLength, iOption );

    Int iResult = setsockopt( m_hSocket, iLevel, iOptionName, (const Char*)pValue, iOptionLength );
    if ( iResult == 0 )
        return NETWORK_OK;

    if ( iResult == SOCKET_ERROR ) {
        iResult = WSAGetLastError();
        switch( iResult ) {
            case WSAEINPROGRESS: return NETWORK_BUSY;
            case WSAENETRESET:   return NETWORK_CONNECT_TIMEOUT;
            case WSAENOTCONN:    return NETWORK_CONNECT_RESET;
            case WSANOTINITIALISED:
            case WSAENOTSOCK:
            case WSAEINVAL:
            case WSAEFAULT:
            case WSAENOPROTOOPT:
            case WSAENETDOWN:
            default: break;
        }
    }
    return NETWORK_ERROR;
}

Void HSocket::SetMode( Bool bNonBlocking ) const
{
    ULong ulParam = bNonBlocking ? 1 : 0;
    Int iRes = ioctlsocket( m_hSocket, FIONBIO, &ulParam );
    DebugAssert( iRes == 0 );
}
Void HSocket::GetPendingData( ULong * outPendingBytes ) const
{
    Int iRes = ioctlsocket( m_hSocket, FIONREAD, outPendingBytes );
    DebugAssert( iRes == 0 );
}

NetworkError HSocket::Connect( const NetAddress * pRemoteAddress ) const
{
    Int iResult;
    switch( pRemoteAddress->iType ) {
        case NETADDRESS_IPv4: {
                const NetAddressIPv4 * pAddr = (const NetAddressIPv4 *)pRemoteAddress;

                SOCKADDR_IN sockIPv4;
                MemZero( &sockIPv4, sizeof(SOCKADDR_IN) );
                sockIPv4.sin_family = AF_INET;
                sockIPv4.sin_addr.s_addr = pAddr->Addr.Value;
                sockIPv4.sin_port = pAddr->wPort;

                iResult = connect( m_hSocket, (const SOCKADDR *)( &sockIPv4 ), sizeof(SOCKADDR_IN) );
            } break;
        case NETADDRESS_IPv6: {
                const NetAddressIPv6 * pAddr = (const NetAddressIPv6 *)pRemoteAddress;

                SOCKADDR_IN6 sockIPv6;
                MemZero( &sockIPv6, sizeof(SOCKADDR_IN6) );
                sockIPv6.sin6_family = AF_INET6;
                sockIPv6.sin6_flowinfo = pAddr->dwFlowInfo;
                sockIPv6.sin6_scope_id = pAddr->dwScopeID;
                MemCopy( sockIPv6.sin6_addr.s6_addr, pAddr->Addr.Bytes, 16 );
                sockIPv6.sin6_port = pAddr->wPort;

                iResult = connect( m_hSocket, (const SOCKADDR*)( &sockIPv6 ), sizeof(SOCKADDR_IN6) );
            } break;
        default: DebugAssert( false ); break;
    }
    if ( iResult == 0 )
        return NETWORK_OK;

    if ( iResult == SOCKET_ERROR ) {
        iResult = WSAGetLastError();
        switch(iResult) {
            case WSAEADDRNOTAVAIL: return NETWORK_ADDRESS_INVALID;
            case WSAEADDRINUSE:    return NETWORK_ADDRESS_IN_USE;
            case WSAEACCES:        return NETWORK_NO_BROADCAST;
            case WSAENETUNREACH:   return NETWORK_UNREACHABLE_NET;
            case WSAEHOSTUNREACH:  return NETWORK_UNREACHABLE_HOST;
            case WSAEISCONN:       return NETWORK_CONNECT_ALREADY;
            case WSAEALREADY:      return NETWORK_CONNECT_IN_PROGRESS;
            case WSAECONNREFUSED:  return NETWORK_CONNECT_REFUSED;
            case WSAETIMEDOUT:     return NETWORK_CONNECT_TIMEOUT;
            case WSAEINPROGRESS:   return NETWORK_BUSY;
            case WSAEWOULDBLOCK:   return NETWORK_RETRY;
            case WSAEINTR:         return NETWORK_INTERRUPTED;
            case WSANOTINITIALISED:
            case WSAEAFNOSUPPORT:
            case WSAENOTSOCK:
            case WSAEINVAL:
            case WSAEFAULT:
            case WSAENETDOWN:
            case WSAENOBUFS:
            default: break;
        }
    }
    return NETWORK_ERROR;
}

NetworkError HSocket::Bind( const NetAddress * pLocalAddress ) const
{
    Int iResult;
    switch( pLocalAddress->iType ) {
        case NETADDRESS_IPv4: {
                const NetAddressIPv4 * pAddr = (const NetAddressIPv4 *)pLocalAddress;

                SOCKADDR_IN sockIPv4;
                MemZero( &sockIPv4, sizeof(SOCKADDR_IN) );
                sockIPv4.sin_family = AF_INET;
                sockIPv4.sin_addr.s_addr = pAddr->Addr.Value;
                sockIPv4.sin_port = pAddr->wPort;

                iResult = bind( m_hSocket, (const SOCKADDR *)( &sockIPv4 ), sizeof(SOCKADDR_IN) );
            } break;
        case NETADDRESS_IPv6: {
                const NetAddressIPv6 * pAddr = (const NetAddressIPv6 *)pLocalAddress;

                SOCKADDR_IN6 sockIPv6;
                MemZero( &sockIPv6, sizeof(SOCKADDR_IN6) );
                sockIPv6.sin6_family = AF_INET6;
                sockIPv6.sin6_flowinfo = pAddr->dwFlowInfo;
                sockIPv6.sin6_scope_id = pAddr->dwScopeID;
                MemCopy( sockIPv6.sin6_addr.s6_addr, pAddr->Addr.Bytes, 16 );
                sockIPv6.sin6_port = pAddr->wPort;

                iResult = bind( m_hSocket, (const SOCKADDR*)( &sockIPv6 ), sizeof(SOCKADDR_IN6) );
            } break;
        default: DebugAssert( false ); break;
    }
    if ( iResult == 0 )
        return NETWORK_OK;

    if ( iResult == SOCKET_ERROR ) {
        iResult = WSAGetLastError();
        switch(iResult) {
            case WSAEADDRNOTAVAIL: return NETWORK_ADDRESS_INVALID;
            case WSAEADDRINUSE:    return NETWORK_ADDRESS_IN_USE;
            case WSAEACCES:        return NETWORK_NO_BROADCAST;
            case WSAEINPROGRESS:   return NETWORK_BUSY;
            case WSANOTINITIALISED:
            case WSAENOTSOCK:
            case WSAEINVAL:
            case WSAEFAULT:
            case WSAENETDOWN:
            case WSAENOBUFS:
            default: break;
        }
    }
    return NETWORK_ERROR;
}
NetworkError HSocket::Listen( UInt iMaxPending ) const
{
    if ( iMaxPending == 0 || iMaxPending >= SOMAXCONN )
        return NETWORK_ERROR;

    Int iResult = listen( m_hSocket, (Int)iMaxPending );
    if ( iResult == 0 )
        return NETWORK_OK;

    if ( iResult == SOCKET_ERROR ) {
        iResult = WSAGetLastError();
        switch(iResult) {
            case WSAEISCONN:     return NETWORK_CONNECT_ALREADY;
            case WSAEADDRINUSE:  return NETWORK_ADDRESS_IN_USE;
            case WSAEINPROGRESS: return NETWORK_BUSY;
            case WSANOTINITIALISED:
            case WSAENOTSOCK:
            case WSAEOPNOTSUPP:
            case WSAEINVAL:
            case WSAENETDOWN:
            case WSAENOBUFS:
            case WSAEMFILE:
            default: break;
        }
    }
    return NETWORK_ERROR;
}
NetworkError HSocket::Accept( HSocket * outClientSocket, NetAddress * outClientAddress ) const
{
    if ( outClientAddress != NULL ) {
        SOCKADDR sockAddr;
        MemZero( &sockAddr, sizeof(SOCKADDR) );
        Int iAddrLength = sizeof(SOCKADDR);

        outClientSocket->m_hSocket = accept( m_hSocket, &sockAddr, &iAddrLength );

        switch( sockAddr.sa_family ) {
            case AF_INET: {
                    NetAddressIPv4 * outAddr = (NetAddressIPv4*)outClientAddress;
                    outAddr->iType = NETADDRESS_IPv4;

                    SOCKADDR_IN * pSockIPv4 = (SOCKADDR_IN*)( &sockAddr );
                    outAddr->Addr.Value = pSockIPv4->sin_addr.s_addr;
                    outAddr->wPort = pSockIPv4->sin_port;
                } break;
            case AF_INET6: {
                    NetAddressIPv6 * outAddr = (NetAddressIPv6*)outClientAddress;
                    outAddr->iType = NETADDRESS_IPv6;

                    SOCKADDR_IN6 * pSockIPv6 = (SOCKADDR_IN6*)( &sockAddr );
                    outAddr->dwFlowInfo = pSockIPv6->sin6_flowinfo;
                    outAddr->dwScopeID = pSockIPv6->sin6_scope_id;
                    MemCopy( outAddr->Addr.Bytes, pSockIPv6->sin6_addr.s6_addr, 16 );
                    outAddr->wPort = pSockIPv6->sin6_port;
                } break;
            default: DebugAssert( false ); break;
        }
    } else
        outClientSocket->m_hSocket = accept( m_hSocket, NULL, NULL );

    if ( outClientSocket->m_hSocket == INVALID_SOCKET ) {
        Int iError = WSAGetLastError();
        switch(iError) {
            case WSAECONNRESET:  return NETWORK_CONNECT_RESET;
            case WSAEINPROGRESS: return NETWORK_BUSY;
            case WSAEWOULDBLOCK: return NETWORK_RETRY;
            case WSAEINTR:       return NETWORK_INTERRUPTED;
            case WSANOTINITIALISED:
            case WSAENOTSOCK:
            case WSAEOPNOTSUPP:
            case WSAEINVAL:
            case WSAEFAULT:
            case WSAENETDOWN:
            case WSAENOBUFS:
            case WSAEMFILE:
            default: break;
        }
        return NETWORK_ERROR;
    }

    return NETWORK_OK;
}

NetworkError HSocket::Send( const Byte * streamData, UInt iLength, UInt * outBytesSent ) const
{
    Int iResult = send( m_hSocket, (const Char *)streamData, (Int)iLength, 0 );
    if ( iResult == SOCKET_ERROR ) {
        iResult = WSAGetLastError();
        switch(iResult) {
            case WSAEACCES:       return NETWORK_NO_BROADCAST;
            case WSAEHOSTUNREACH: return NETWORK_UNREACHABLE_HOST;
            case WSAETIMEDOUT:    return NETWORK_CONNECT_TIMEOUT;
            case WSAENETRESET:    return NETWORK_CONNECT_BROKEN;
            case WSAECONNRESET:   return NETWORK_CONNECT_RESET;
            case WSAECONNABORTED: return NETWORK_CONNECT_ABORTED;
            case WSAEINPROGRESS:  return NETWORK_BUSY;
            case WSAEWOULDBLOCK:  return NETWORK_RETRY;
            case WSAEINTR:        return NETWORK_INTERRUPTED;
            case WSAESHUTDOWN:    return NETWORK_SHUTDOWN;
            case WSANOTINITIALISED:
            case WSAENOTSOCK:
            case WSAENOTCONN:
            case WSAEOPNOTSUPP:
            case WSAEINVAL:
            case WSAEFAULT:
            case WSAEMSGSIZE:
            case WSAENETDOWN:
            case WSAENOBUFS:
            default: break;
        }
        return NETWORK_ERROR;
    }

    if ( outBytesSent != NULL )
        *outBytesSent = (UInt)iResult;
    return NETWORK_OK;
}
NetworkError HSocket::Receive( Byte * streamData, UInt iLength, UInt * outBytesReceived ) const
{
    Int iResult = recv( m_hSocket, (Char*)streamData, (Int)iLength, 0 );
    if ( iResult == SOCKET_ERROR ) {
        iResult = WSAGetLastError();
        switch(iResult) {
            case WSAETIMEDOUT:    return NETWORK_CONNECT_TIMEOUT;
            case WSAENETRESET:    return NETWORK_CONNECT_BROKEN;
            case WSAECONNRESET:   return NETWORK_CONNECT_RESET;
            case WSAECONNABORTED: return NETWORK_CONNECT_ABORTED;
            case WSAEINPROGRESS:  return NETWORK_BUSY;
            case WSAEWOULDBLOCK:  return NETWORK_RETRY;
            case WSAEINTR:        return NETWORK_INTERRUPTED;
            case WSAESHUTDOWN:    return NETWORK_SHUTDOWN;
            case WSANOTINITIALISED:
            case WSAENOTSOCK:
            case WSAENOTCONN:
            case WSAEOPNOTSUPP:
            case WSAEINVAL:
            case WSAEFAULT:
            case WSAEMSGSIZE:
            case WSAENETDOWN:
            default: break;
        }
        return NETWORK_ERROR;
    }

    if ( outBytesReceived != NULL )
        *outBytesReceived = (UInt)iResult;
    return NETWORK_OK;
}

NetworkError HSocket::SendTo( const NetAddress * pRemoteAddress, const Byte * packetData, UInt iLength, UInt * outBytesSent ) const
{
    Int iResult;
    switch( pRemoteAddress->iType ) {
        case NETADDRESS_IPv4: {
                const NetAddressIPv4 * pAddr = (const NetAddressIPv4 *)pRemoteAddress;

                SOCKADDR_IN sockIPv4;
                MemZero( &sockIPv4, sizeof(SOCKADDR_IN) );
                sockIPv4.sin_family = AF_INET;
                sockIPv4.sin_addr.s_addr = pAddr->Addr.Value;
                sockIPv4.sin_port = pAddr->wPort;

                iResult = sendto( m_hSocket, (const Char *)packetData, (Int)iLength, 0, (const SOCKADDR *)( &sockIPv4 ), sizeof(SOCKADDR_IN) );
            } break;
        case NETADDRESS_IPv6: {
                const NetAddressIPv6 * pAddr = (const NetAddressIPv6 *)pRemoteAddress;

                SOCKADDR_IN6 sockIPv6;
                MemZero( &sockIPv6, sizeof(SOCKADDR_IN6) );
                sockIPv6.sin6_family = AF_INET6;
                sockIPv6.sin6_flowinfo = pAddr->dwFlowInfo;
                sockIPv6.sin6_scope_id = pAddr->dwScopeID;
                MemCopy( sockIPv6.sin6_addr.s6_addr, pAddr->Addr.Bytes, 16 );
                sockIPv6.sin6_port = pAddr->wPort;

                iResult = sendto( m_hSocket, (const Char *)packetData, (Int)iLength, 0, (const SOCKADDR *)( &sockIPv6 ), sizeof(SOCKADDR_IN6) );
            } break;
        default: DebugAssert( false ); break;
    }

    if ( iResult == SOCKET_ERROR ) {
        iResult = WSAGetLastError();
        switch(iResult) {
            case WSAEADDRNOTAVAIL: return NETWORK_ADDRESS_INVALID;
            case WSAEACCES:        return NETWORK_NO_BROADCAST;
            case WSAENETUNREACH:   return NETWORK_UNREACHABLE_NET;
            case WSAEHOSTUNREACH:  return NETWORK_UNREACHABLE_HOST;
            case WSAETIMEDOUT:     return NETWORK_CONNECT_TIMEOUT;
            case WSAENETRESET:     return NETWORK_CONNECT_BROKEN;
            case WSAECONNRESET:    return NETWORK_CONNECT_RESET;
            case WSAECONNABORTED:  return NETWORK_CONNECT_ABORTED;
            case WSAEINPROGRESS:   return NETWORK_BUSY;
            case WSAEWOULDBLOCK:   return NETWORK_RETRY;
            case WSAEINTR:         return NETWORK_INTERRUPTED;
            case WSAESHUTDOWN:     return NETWORK_SHUTDOWN;
            case WSANOTINITIALISED:
            case WSAEAFNOSUPPORT:
            case WSAEDESTADDRREQ:
            case WSAENOTSOCK:
            case WSAENOTCONN:
            case WSAEOPNOTSUPP:
            case WSAEINVAL:
            case WSAEFAULT:
            case WSAEMSGSIZE:
            case WSAENETDOWN:
            case WSAENOBUFS:
            default: break;
        }
        return NETWORK_ERROR;
    }

    if ( outBytesSent != NULL )
        *outBytesSent = (UInt)iResult;
    return NETWORK_OK;
}
NetworkError HSocket::ReceiveFrom( NetAddress * outRemoteAddress, Byte * packetData, UInt iLength, UInt * outBytesReceived ) const
{
    SOCKADDR sockAddr;
    MemZero( &sockAddr, sizeof(SOCKADDR) );
    Int iAddrLength = sizeof(SOCKADDR);

    Int iResult = recvfrom( m_hSocket, (Char*)packetData, (Int)iLength, 0, &sockAddr, &iAddrLength );

    switch( sockAddr.sa_family ) {
        case AF_INET: {
                NetAddressIPv4 * outAddr = (NetAddressIPv4*)outRemoteAddress;
                outAddr->iType = NETADDRESS_IPv4;

                SOCKADDR_IN * pSockIPv4 = (SOCKADDR_IN*)( &sockAddr );
                outAddr->Addr.Value = pSockIPv4->sin_addr.s_addr;
                outAddr->wPort = pSockIPv4->sin_port;
            } break;
        case AF_INET6: {
                NetAddressIPv6 * outAddr = (NetAddressIPv6*)outRemoteAddress;
                outAddr->iType = NETADDRESS_IPv6;

                SOCKADDR_IN6 * pSockIPv6 = (SOCKADDR_IN6*)( &sockAddr );
                outAddr->dwFlowInfo = pSockIPv6->sin6_flowinfo;
                outAddr->dwScopeID = pSockIPv6->sin6_scope_id;
                MemCopy( outAddr->Addr.Bytes, pSockIPv6->sin6_addr.s6_addr, 16 );
                outAddr->wPort = pSockIPv6->sin6_port;
            } break;
        default: DebugAssert( false ); break;
    }

    if ( iResult == SOCKET_ERROR ) {
        iResult = WSAGetLastError();
        switch(iResult) {
            case WSAETIMEDOUT:   return NETWORK_CONNECT_TIMEOUT;
            case WSAENETRESET:   return NETWORK_CONNECT_BROKEN;
            case WSAECONNRESET:  return NETWORK_CONNECT_RESET;
            case WSAEINPROGRESS: return NETWORK_BUSY;
            case WSAEWOULDBLOCK: return NETWORK_RETRY;
            case WSAEINTR:       return NETWORK_INTERRUPTED;
            case WSAESHUTDOWN:   return NETWORK_SHUTDOWN;
            case WSANOTINITIALISED:
            case WSAENOTSOCK:
            case WSAEISCONN:
            case WSAEOPNOTSUPP:
            case WSAEINVAL:
            case WSAEFAULT:
            case WSAEMSGSIZE:
            case WSAENETDOWN:
            default: break;
        }
        return NETWORK_ERROR;
    }

    if ( outBytesReceived != NULL )
        *outBytesReceived = (UInt)iResult;
    return NETWORK_OK;
}

/////////////////////////////////////////////////////////////////////////////////

SocketProtocol HSocket::_BuildSocketProtocol( DWord dwSockType, DWord dwFamily, DWord dwProtocol )
{
    switch( dwSockType ) {
        case SOCK_RAW:
            switch( dwFamily ) {
                case AF_INET:
                    switch( dwProtocol ) {
                        case IPPROTO_ICMP: return SOCKET_ICMPv4;
                        case IPPROTO_IGMP: return SOCKET_IGMPv4;
                        default: break;
                    }
                case AF_INET6:
                    switch( dwProtocol ) {
                        case IPPROTO_ICMPV6: return SOCKET_ICMPv6;
                        case IPPROTO_IGMP:   return SOCKET_IGMPv6;
                        default: break;
                    }
                default: break;
            }
        case SOCK_DGRAM:
            switch( dwFamily ) {
                case AF_INET:
                    switch( dwProtocol ) {
                        case IPPROTO_UDP: return SOCKET_UDPIPv4;
                        default: break;
                    }
                case AF_INET6:
                    switch( dwProtocol ) {
                        case IPPROTO_UDP: return SOCKET_UDPIPv6;
                        default: break;
                    }
                default: break;
            }
        case SOCK_STREAM:
            switch( dwFamily ) {
                case AF_INET:
                    switch( dwProtocol ) {
                        case IPPROTO_TCP: return SOCKET_TCPIPv4;
                        default: break;
                    }
                case AF_INET6:
                    switch( dwProtocol ) {
                        case IPPROTO_TCP: return SOCKET_TCPIPv6;
                        default: break;
                    }
                default: break;
            }
        default: break;
    }

    return SOCKET_UNSPECIFIED;
}
Void HSocket::_BuildSocketOption( Int * outLevel, Int * outOptionName, Int * outOptionLength, SocketOption iOption )
{
    switch( iOption ) {
        case SOCKETOPTION_SERVICE_CONFIG:     *outLevel = SOL_SOCKET; *outOptionName = PVD_CONFIG;          *outOptionLength = sizeof(Char*); break;
        case SOCKETOPTION_PROTOCOL_INFOS:     *outLevel = SOL_SOCKET; *outOptionName = SO_PROTOCOL_INFOA;   *outOptionLength = sizeof(WSAPROTOCOL_INFOA); break;
        case SOCKETOPTION_TYPE:               *outLevel = SOL_SOCKET; *outOptionName = SO_TYPE;             *outOptionLength = sizeof(ULong); break;
        case SOCKETOPTION_LISTENING:          *outLevel = SOL_SOCKET; *outOptionName = SO_ACCEPTCONN;       *outOptionLength = sizeof(DWord); break;
        case SOCKETOPTION_ADDR_USE_EXCLUSIVE: *outLevel = SOL_SOCKET; *outOptionName = SO_EXCLUSIVEADDRUSE; *outOptionLength = sizeof(DWord); break;
        case SOCKETOPTION_ADDR_USE_SHARE:     *outLevel = SOL_SOCKET; *outOptionName = SO_REUSEADDR;        *outOptionLength = sizeof(DWord); break;
        case SOCKETOPTION_BROADCAST:          *outLevel = SOL_SOCKET; *outOptionName = SO_BROADCAST;        *outOptionLength = sizeof(DWord); break;
        case SOCKETOPTION_KEEP_ALIVE:         *outLevel = SOL_SOCKET; *outOptionName = SO_KEEPALIVE;        *outOptionLength = sizeof(DWord); break;
        case SOCKETOPTION_INLINE_OOB:         *outLevel = SOL_SOCKET; *outOptionName = SO_OOBINLINE;        *outOptionLength = sizeof(DWord); break;
        case SOCKETOPTION_BUFFER_SEND:        *outLevel = SOL_SOCKET; *outOptionName = SO_SNDBUF;           *outOptionLength = sizeof(ULong); break;
        case SOCKETOPTION_BUFFER_RECEIVE:     *outLevel = SOL_SOCKET; *outOptionName = SO_RCVBUF;           *outOptionLength = sizeof(ULong); break;
        case SOCKETOPTION_LINGER:             *outLevel = SOL_SOCKET; *outOptionName = SO_LINGER;           *outOptionLength = sizeof(linger); break;

        case SOCKETOPTION_IPv4_DONT_FRAGMENT:       *outLevel = IPPROTO_IP; *outOptionName = IP_DONTFRAGMENT;           *outOptionLength = sizeof(DWord); break;
        case SOCKETOPTION_IPv4_HEADER_PROVIDED:     *outLevel = IPPROTO_IP; *outOptionName = IP_HDRINCL;                *outOptionLength = sizeof(DWord); break;
        case SOCKETOPTION_IPv4_BROADCAST_RECEIVE:   *outLevel = IPPROTO_IP; *outOptionName = IP_RECEIVE_BROADCAST;      *outOptionLength = sizeof(DWord); break;
        case SOCKETOPTION_IPv4_UNICAST_INTERFACE:   *outLevel = IPPROTO_IP; *outOptionName = IP_UNICAST_IF;             *outOptionLength = sizeof(DWord); break;
        case SOCKETOPTION_IPv4_UNICAST_TTL:         *outLevel = IPPROTO_IP; *outOptionName = IP_TTL;                    *outOptionLength = sizeof(DWord); break;
        case SOCKETOPTION_IPv4_MULTICAST_INTERFACE: *outLevel = IPPROTO_IP; *outOptionName = IP_MULTICAST_IF;           *outOptionLength = sizeof(DWord); break;
        case SOCKETOPTION_IPv4_MULTICAST_JOIN:      *outLevel = IPPROTO_IP; *outOptionName = IP_ADD_MEMBERSHIP;         *outOptionLength = sizeof(ip_mreq); break;
        case SOCKETOPTION_IPv4_MULTICAST_LEAVE:     *outLevel = IPPROTO_IP; *outOptionName = IP_DROP_MEMBERSHIP;        *outOptionLength = sizeof(ip_mreq); break;
        case SOCKETOPTION_IPv4_MULTICAST_SRC_ALLOW: *outLevel = IPPROTO_IP; *outOptionName = IP_UNBLOCK_SOURCE;         *outOptionLength = sizeof(ip_mreq_source); break;
        case SOCKETOPTION_IPv4_MULTICAST_SRC_DENY:  *outLevel = IPPROTO_IP; *outOptionName = IP_BLOCK_SOURCE;           *outOptionLength = sizeof(ip_mreq_source); break;
        case SOCKETOPTION_IPv4_MULTICAST_SRC_JOIN:  *outLevel = IPPROTO_IP; *outOptionName = IP_ADD_SOURCE_MEMBERSHIP;  *outOptionLength = sizeof(ip_mreq_source); break;
        case SOCKETOPTION_IPv4_MULTICAST_SRC_LEAVE: *outLevel = IPPROTO_IP; *outOptionName = IP_DROP_SOURCE_MEMBERSHIP; *outOptionLength = sizeof(ip_mreq_source); break;
        case SOCKETOPTION_IPv4_MULTICAST_LOOPBACK:  *outLevel = IPPROTO_IP; *outOptionName = IP_MULTICAST_LOOP;         *outOptionLength = sizeof(DWord); break;
        case SOCKETOPTION_IPv4_MULTICAST_TTL:       *outLevel = IPPROTO_IP; *outOptionName = IP_MULTICAST_TTL;          *outOptionLength = sizeof(DWord); break;

        //case SOCKETOPTION_IPv6_: *outLevel = IPPROTO_IPV6; *outOption = ; *outSize = sizeof(); break;

        case SOCKETOPTION_UDP_CHECKSUM_USE:  *outLevel = IPPROTO_UDP; *outOptionName = UDP_CHECKSUM_COVERAGE; *outOptionLength = sizeof(DWord); break;
        case SOCKETOPTION_UDP_CHECKSUM_ZERO: *outLevel = IPPROTO_UDP; *outOptionName = UDP_NOCHECKSUM;        *outOptionLength = sizeof(DWord); break;

        case SOCKETOPTION_TCP_NAGLE: *outLevel = IPPROTO_TCP; *outOptionName = TCP_NODELAY; *outOptionLength = sizeof(DWord); break;

        default: DebugAssert( false ); break;
    }
}


