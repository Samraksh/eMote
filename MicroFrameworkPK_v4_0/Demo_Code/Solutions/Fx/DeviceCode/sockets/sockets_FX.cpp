////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "tinyhal.h"

//--//


BOOL Network_Initialize()
{ 
    return TRUE; 
}

BOOL Network_Uninitialize()
{ 
    return TRUE; 
}

BOOL SOCKETS_Initialize( int ComPortNum )
{
    return TRUE;
}

BOOL SOCKETS_Uninitialize( int ComPortNum )
{
    return TRUE;
}

int SOCKETS_Write( int ComPortNum, const char* Data, size_t size )
{ 
    return 0; 
}

int SOCKETS_Read( int ComPortNum, char* Data, size_t size )
{ 
    return 0; 
}

BOOL SOCKETS_Flush( int ComPortNum )
{ 
    return FALSE; 
}

void SOCKETS_CloseConnections()
{
}

BOOL SOCKETS_ProcessSocketActivity(SOCK_SOCKET signalSocket)
{
    return TRUE;
}

UINT32 SOCK_CONFIGURATION_GetAdapterCount()
{
    return 0;
}

HRESULT SOCK_CONFIGURATION_LoadAdapterConfiguration( UINT32 interfaceIndex, SOCK_NetworkConfiguration* config )
{ 
    return CLR_E_FAIL; 
}

HRESULT SOCK_CONFIGURATION_UpdateAdapterConfiguration( UINT32 interfaceIndex, UINT32 updateFlags, SOCK_NetworkConfiguration* config )
{ 
    return CLR_E_FAIL; 
}

HRESULT SOCK_CONFIGURATION_LoadConfiguration( UINT32 interfaceIndex, SOCK_NetworkConfiguration* config )
{
    return CLR_E_FAIL; 
}

HRESULT SOCK_CONFIGURATION_LoadWirelessConfiguration( UINT32 interfaceIndex, SOCK_WirelessConfiguration* wirelessConfig )
{
    return CLR_E_FAIL; 
}

HRESULT SOCK_CONFIGURATION_UpdateWirelessConfiguration( UINT32 interfaceIndex, SOCK_WirelessConfiguration* wirelessConfig )
{
    return CLR_E_FAIL; 
}

HRESULT SOCK_CONFIGURATION_SaveAllWirelessConfigurations( )
{
    return CLR_E_FAIL; 
}

HRESULT HAL_SOCK_CONFIGURATION_LoadWirelessConfiguration( UINT32 interfaceIndex, SOCK_WirelessConfiguration* wirelessConfig )
{
    return CLR_E_FAIL; 
}

int SOCK_socket( int family, int type, int protocol ) 
{ 
    return SOCK_SOCKET_ERROR; 
}

int SOCK_bind( int socket, const struct SOCK_sockaddr* address, int addressLen  ) 
{ 
    return SOCK_SOCKET_ERROR; 
}

int SOCK_connect(int socket, const struct SOCK_sockaddr* address, int addressLen) 
{ 
    return SOCK_SOCKET_ERROR; 
}

int SOCK_send(int socket, const char* buf, int len, int flags) 
{ 
    return SOCK_SOCKET_ERROR; 
}

int SOCK_recv(int socket, char* buf, int len, int timeout) 
{ 
    return SOCK_SOCKET_ERROR; 
}

int SOCK_close(int socket) 
{ 
    return SOCK_SOCKET_ERROR; 
}

int SOCK_listen( int socket, int backlog ) 
{ 
    return SOCK_SOCKET_ERROR; 
}

int SOCK_accept( int socket, struct SOCK_sockaddr* address, int* addressLen ) 
{ 
    return SOCK_SOCKET_ERROR; 
}

int SOCK_shutdown( int socket, int how ) 
{ 
    return SOCK_SOCKET_ERROR; 
}

int SOCK_getaddrinfo(  const char* nodename, char* servname, const struct SOCK_addrinfo* hints, struct SOCK_addrinfo** res ) 
{ 
    return SOCK_SOCKET_ERROR; 
}

void SOCK_freeaddrinfo( struct SOCK_addrinfo* ai )
{
}

int SOCK_ioctl( int socket, int cmd, int* data ) 
{ 
    return SOCK_SOCKET_ERROR; 
}

int SOCK_getlasterror() 
{ 
    return SOCK_SOCKET_ERROR; 
}

int SOCK_select( int socket, SOCK_fd_set* readfds, SOCK_fd_set* writefds, SOCK_fd_set* except, const struct SOCK_timeval* timeout ) 
{ 
    return SOCK_SOCKET_ERROR; 
}

int SOCK_setsockopt( int socket, int level, int optname, const char* optval, int  optlen ) 
{ 
    return SOCK_SOCKET_ERROR; 
}

int SOCK_getsockopt( int socket, int level, int optname,       char* optval, int* optlen ) 
{ 
    return SOCK_SOCKET_ERROR; 
}

int SOCK_getpeername( int socket, struct SOCK_sockaddr* name, int* namelen ) 
{ 
    return SOCK_SOCKET_ERROR; 
}

int SOCK_getsockname( int socket, struct SOCK_sockaddr* name, int* namelen ) 
{ 
    return SOCK_SOCKET_ERROR; 
}

int SOCK_recvfrom( int s, char* buf, int len, int flags, struct SOCK_sockaddr* from, int* fromlen ) 
{ 
    return SOCK_SOCKET_ERROR; 
}

int SOCK_sendto( int s, const char* buf, int len, int flags, const struct SOCK_sockaddr* to, int tolen ) 
{ 
    return SOCK_SOCKET_ERROR; 
}

