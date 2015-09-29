/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/System/Networking.h
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
// Known Bugs :
//  - Win32 => IPv6 not fully supported unless Vista/Seven or later
//  - Win32 => UNICODE support : Add Vista/Seven improvements when I switch on
//  - Linux => <UNIMPLEMENTED>
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_THIRDPARTY_SYSTEM_NETWORKING_H
#define SCARAB_THIRDPARTY_SYSTEM_NETWORKING_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Platform.h"

/////////////////////////////////////////////////////////////////////////////////
// Forward declarations for Win32
typedef __w64 unsigned int SOCKET;

struct addrinfo;
typedef struct addrinfo ADDRINFOA, *PADDRINFOA;
struct addrinfoW;
typedef struct addrinfoW ADDRINFOW, *PADDRINFOW;

#if ( defined(UNICODE) || defined(_UNICODE) )
    typedef ADDRINFOW ADDRINFOT, *PADDRINFOT;
#else
    typedef ADDRINFOA ADDRINFOT, *PADDRINFOT;
#endif

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
    // Error handling
enum NetworkError
{
    NETWORK_OK = 0,

    NETWORK_ADDRESS_INVALID,     // Wrong Addr family / Bad Addr format / Unavailable address
    NETWORK_ADDRESS_IN_USE,      // Address is already in use on the same network

    NETWORK_UNREACHABLE_NET,     // Network is unreachable
    NETWORK_UNREACHABLE_HOST,    // Host is unreachable

    NETWORK_HOST_NOT_FOUND,      // Host could not be resolved at this time ...
    NETWORK_NO_BROADCAST,        // Broadcasting is disabled, can't use broadcast address
    NETWORK_NO_DATA,             // No data to output ...

    NETWORK_BUSY,                // HSocket is busy, try again later
    NETWORK_RETRY,               // Non-blocking socket can't service right now, try again later
    NETWORK_INTERRUPTED,         // A blocking routine has been interrupted
    NETWORK_SHUTDOWN,            // HSocket was shutdown for send and/or receive operations

    NETWORK_CONNECT_ALREADY,     // Already connected
    NETWORK_CONNECT_IN_PROGRESS, // Connection negociation is in progress
    NETWORK_CONNECT_REFUSED,     // Connection was refused by server
    NETWORK_CONNECT_TIMEOUT,     // Connection dropped : network failure / other end failed to respond or went down without notice
    NETWORK_CONNECT_BROKEN,      // Connection broken : Keep-Alive detected a failure / TTL expired
    NETWORK_CONNECT_RESET,       // Connection reset : Remote side executed hard/abortive close (UDP:Port Unreachable)
    NETWORK_CONNECT_ABORTED,     // Connection aborted : Timeout / Other failure

    NETWORK_ERROR // "Fatal" Error
};

    // Network addresses
enum NetAddressType
{
    NETADDRESS_UNKNOWN = 0,
    NETADDRESS_IPv4,
    NETADDRESS_IPv6
};

typedef struct _netaddress {
    NetAddressType iType;
    Byte arrPadding[28];
} NetAddress;

typedef union _addr_ipv4 {
    Byte Bytes[4];
    Word Words[2];
    DWord Value;
} AddrIPv4;
typedef struct _netaddress_ipv4 {
    NetAddressType iType;
    AddrIPv4 Addr;
    Word wPort;
} NetAddressIPv4;

typedef union _addr_ipv6 {
    Byte Bytes[16];
    Word Words[8];
} AddrIPv6;
typedef struct _netaddress_ipv6 {
    NetAddressType iType;
    DWord dwFlowInfo;
    DWord dwScopeID;
    AddrIPv6 Addr;
    Word wPort;
} NetAddressIPv6;

    // HSocket definitions
#define SOCKET_MAX_CONNECTIONS SOMAXCONN

enum SocketType
{
    SOCKET_RAW = 0,
    SOCKET_DATAGRAM,
    SOCKET_STREAM,
    //SOCKET_RDM,
    //SOCKET_SEQPACKET
};

enum SocketShutdownType
{
    SOCKET_SHUTDOWN_RECEIVE = 0,
    SOCKET_SHUTDOWN_SEND    = 1,
    SOCKET_SHUTDOWN_BOTH    = 2
};

enum SocketProtocol
{
    SOCKET_UNSPECIFIED = 0,

    SOCKET_ICMPv4      = 0x00000001,
    SOCKET_IGMPv4,
    SOCKET_UDPIPv4,
    SOCKET_TCPIPv4,

    SOCKET_ICMPv6      = 0x80000001,
    SOCKET_IGMPv6,
    SOCKET_UDPIPv6,
    SOCKET_TCPIPv6
};

enum SocketOption
{
    // HSocket level options
    SOCKETOPTION_SERVICE_CONFIG = 0, // (Char*, RW) Provider-dependant structure
    SOCKETOPTION_PROTOCOL_INFOS,     // (struct, R) Gives protocol information, struct = SocketOptionProtocolInfos
    SOCKETOPTION_TYPE,               // (ULong,  R) Gives socket's type
    SOCKETOPTION_LISTENING,          // (Bool,   R) Tells if socket is listening and accepts connections
    SOCKETOPTION_ADDR_USE_EXCLUSIVE, // (Bool,  RW) Use address exclusive access
    SOCKETOPTION_ADDR_USE_SHARE,     // (Bool,  RW) Use address shared access (address re-using)
    SOCKETOPTION_BROADCAST,          // (Bool,  RW) Toggles broadcasting messages
    SOCKETOPTION_KEEP_ALIVE,         // (Bool,  RW) Toggles sending of keep-alive probes on a connection    
    SOCKETOPTION_INLINE_OOB,         // (Bool,  RW) Toggles out-of-band data inlining with regular data
    SOCKETOPTION_BUFFER_SEND,        // (ULong, RW) Specifies buffer size for send operations
    SOCKETOPTION_BUFFER_RECEIVE,     // (ULong, RW) Specifies buffer size for receive operations
    SOCKETOPTION_LINGER,             // (struct,RW) Linger state, struct = SocketOptionLinger

    // IP version 4 options
    SOCKETOPTION_IPv4_DONT_FRAGMENT,       // (Bool,  RW) Don't fragment datagram packets (UDP)
    SOCKETOPTION_IPv4_HEADER_PROVIDED,     // (Bool,  RW) IP header is provided by application
    SOCKETOPTION_IPv4_BROADCAST_RECEIVE,   // (Bool,  RW) Toggles broadcast reception
    SOCKETOPTION_IPv4_UNICAST_INTERFACE,   // (DWord, RW) Interface index ( 0.x.x.x except 0.0.0.0 )
    SOCKETOPTION_IPv4_UNICAST_TTL,         // (DWord, RW) Unicast Packets time to live, in jumps
    SOCKETOPTION_IPv4_MULTICAST_INTERFACE, // (DWord, RW) Interface index ( 0.x.x.x except 0.0.0.0 )
    SOCKETOPTION_IPv4_MULTICAST_JOIN,      // (struct, W) Join group, struct = SocketOptionMulticast
    SOCKETOPTION_IPv4_MULTICAST_LEAVE,     // (struct, W) Leave group, struct = SocketOptionMulticast
    SOCKETOPTION_IPv4_MULTICAST_SRC_ALLOW, // (struct, W) Add multicast sender, struct = SocketOptionMulticastSource
    SOCKETOPTION_IPv4_MULTICAST_SRC_DENY,  // (struct, W) Remove multicast sender, struct = SocketOptionMulticastSource
    SOCKETOPTION_IPv4_MULTICAST_SRC_JOIN,  // (struct, W) Join group + Add sender, struct = SocketOptionMulticastSource
    SOCKETOPTION_IPv4_MULTICAST_SRC_LEAVE, // (struct, W) Remove sender + Leave group, struct = SocketOptionMulticastSource
    SOCKETOPTION_IPv4_MULTICAST_LOOPBACK,  // (Bool,  RW) Toggles multicast reception on the loopback interface (security)
    SOCKETOPTION_IPv4_MULTICAST_TTL,       // (DWord, RW) Multicast Packets time to live, in jumps

    // IP version 6 options
    //SOCKETOPTION_IPv6_,

    // UDP options
    SOCKETOPTION_UDP_CHECKSUM_USE,  // (Bool, RW) Send a checksum with datagrams
    SOCKETOPTION_UDP_CHECKSUM_ZERO, // (Bool, RW) Don't compute checksums, send a 0 value (required for providers)

    // TCP options
    SOCKETOPTION_TCP_NAGLE, // (Bool, RW) Toggles Nagle algorithm for buffered-coalesced sending (usefull with many small packets & timing)

    SOCKETOPTION_COUNT
};

typedef struct _socket_option_protocol_infos
{
    /////////////
} SocketOptionProtocolInfos;

typedef struct _socket_option_linger
{
    Word bOnOff;
    Word iSeconds;
} SocketOptionLinger;

typedef struct _socket_option_ipv4_multicast
{
    AddrIPv4 ipMulticast;
    AddrIPv4 ipInterface;
} SocketOptionIPv4Multicast;
typedef struct _socket_option_ipv4_multicast_source
{
    AddrIPv4 ipMulticast;
    AddrIPv4 ipSource;
    AddrIPv4 ipInterface;
} SocketOptionIPv4MulticastSource;

/////////////////////////////////////////////////////////////////////////////////
// The HSocket class
class HSocket
{
public:
    ~HSocket();

    Void Shutdown( SocketShutdownType iType ) const;
    Void Close();

    NetworkError GetOption( SocketOption iOption, Void * outValue ) const;
    NetworkError SetOption( SocketOption iOption, const Void * pValue ) const;

    Void SetMode( Bool bNonBlocking ) const;
    Void GetPendingData( ULong * outPendingBytes ) const;

    NetworkError Connect( const NetAddress * pRemoteAddress ) const;

    NetworkError Bind( const NetAddress * pLocalAddress ) const;
    NetworkError Listen( UInt iMaxPending ) const;
    NetworkError Accept( HSocket * outClientSocket, NetAddress * outClientAddress = NULL ) const;

    NetworkError Send( const Byte * streamData, UInt iLength, UInt * outBytesSent = NULL ) const;
    NetworkError Receive( Byte * streamData, UInt iLength, UInt * outBytesReceived = NULL ) const;

    NetworkError SendTo( const NetAddress * pRemoteAddress, const Byte * packetData, UInt iLength, UInt * outBytesSent = NULL ) const;
    NetworkError ReceiveFrom( NetAddress * outRemoteAddress, Byte * packetData, UInt iLength, UInt * outBytesReceived = NULL ) const;

private:
    // Delegate of system
    friend class System;
    HSocket( SocketProtocol iProtocol );

    // Helpers
    static SocketProtocol _BuildSocketProtocol( DWord dwSockType, DWord dwFamily, DWord dwProtocol );
    static Void _BuildSocketOption( Int * outLevel, Int * outOptionName, Int * outOptionLength, SocketOption iOption );

    // Handle
    SOCKET m_hSocket;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Networking.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_THIRDPARTY_SYSTEM_NETWORKING_H

