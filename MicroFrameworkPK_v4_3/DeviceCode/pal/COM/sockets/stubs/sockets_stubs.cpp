////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "tinyhal.h"

//--//

#if defined(__RENESAS__)
volatile int errno;
#else
int errno;
#endif

BOOL Network_Initialize()
{ 
    NATIVE_PROFILE_PAL_COM();
    return TRUE; 
}

BOOL Network_Uninitialize()
{ 
    NATIVE_PROFILE_PAL_COM();
    return TRUE; 
}

BOOL SOCKETS_Initialize( int ComPortNum )
{
	hal_printf("Inside SOCKETS_Initialize in sockets_stubs.cpp\n");
    NATIVE_PROFILE_PAL_COM();
    return TRUE;
}

BOOL SOCKETS_Uninitialize( int ComPortNum )
{
	hal_printf("Inside SOCKETS_Uninitialize in sockets_stubs.cpp\n");
    NATIVE_PROFILE_PAL_COM();
    return TRUE;
}

int SOCKETS_Write( int ComPortNum, const char* Data, size_t size )
{ 
	hal_printf("Inside SOCKETS_Write in sockets_stubs.cpp\n");
    NATIVE_PROFILE_PAL_COM();
    return 0; 
}

int SOCKETS_Read( int ComPortNum, char* Data, size_t size )
{ 
	hal_printf("Inside SOCK_bind in sockets_stubs.cpp\n");
    NATIVE_PROFILE_PAL_COM();
    return 0; 
}

BOOL SOCKETS_Flush( int ComPortNum )
{ 
	hal_printf("Inside SOCKETS_Flush in sockets_stubs.cpp\n");
    NATIVE_PROFILE_PAL_COM();
    return FALSE; 
}

void SOCKETS_CloseConnections()
{
	hal_printf("Inside SOCKETS_CloseConnections in sockets_stubs.cpp\n");
    NATIVE_PROFILE_PAL_COM();
}

BOOL SOCKETS_UpgradeToSsl( INT32 ComPortNum, const UINT8* pCACert, UINT32 caCertLen, const UINT8* pDeviceCert, UINT32 deviceCertLen, LPCSTR szTargetHost )
{
	hal_printf("Inside SOCKETS_UpgradeToSsl in sockets_stubs.cpp\n");
    NATIVE_PROFILE_PAL_COM();
    return FALSE;
}

BOOL SOCKETS_IsUsingSsl( INT32 ComPortNum )
{
	hal_printf("Inside SOCKETS_IsUsingSsl in sockets_stubs.cpp\n");
    NATIVE_PROFILE_PAL_COM();
    return FALSE;
}


BOOL SOCKETS_ProcessSocketActivity(SOCK_SOCKET signalSocket)
{
	hal_printf("Inside SOCKETS_ProcessSocketActivity in sockets_stubs.cpp\n");
    NATIVE_PROFILE_PAL_COM();
    return TRUE;
}

UINT32 SOCK_CONFIGURATION_GetAdapterCount()
{
    NATIVE_PROFILE_PAL_COM();
    return 0;
}

HRESULT SOCK_CONFIGURATION_LoadAdapterConfiguration( UINT32 interfaceIndex, SOCK_NetworkConfiguration* config )
{ 
    NATIVE_PROFILE_PAL_COM();
    return CLR_E_FAIL; 
}

HRESULT SOCK_CONFIGURATION_UpdateAdapterConfiguration( UINT32 interfaceIndex, UINT32 updateFlags, SOCK_NetworkConfiguration* config )
{ 
    NATIVE_PROFILE_PAL_COM();
    return CLR_E_FAIL; 
}

HRESULT SOCK_CONFIGURATION_LoadConfiguration( UINT32 interfaceIndex, SOCK_NetworkConfiguration* config )
{
    NATIVE_PROFILE_PAL_COM();
    return CLR_E_FAIL; 
}

HRESULT SOCK_CONFIGURATION_LoadWirelessConfiguration( UINT32 interfaceIndex, SOCK_WirelessConfiguration* wirelessConfig )
{
    NATIVE_PROFILE_PAL_COM();
    return CLR_E_FAIL; 
}

HRESULT SOCK_CONFIGURATION_UpdateWirelessConfiguration( UINT32 interfaceIndex, SOCK_WirelessConfiguration* wirelessConfig )
{
    NATIVE_PROFILE_PAL_COM();
    return CLR_E_FAIL; 
}

HRESULT SOCK_CONFIGURATION_SaveAllWirelessConfigurations( )
{
    NATIVE_PROFILE_PAL_COM();
    return CLR_E_FAIL; 
}

HRESULT HAL_SOCK_CONFIGURATION_LoadWirelessConfiguration( UINT32 interfaceIndex, SOCK_WirelessConfiguration* wirelessConfig )
{
    NATIVE_PROFILE_PAL_COM();
    return CLR_E_FAIL; 
}

int SOCK_socket( int family, int type, int protocol ) 
{ 
	hal_printf("Inside SOCK_socket in sockets_stubs.cpp\n");
	NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

int SOCK_bind( int socket, const struct SOCK_sockaddr* address, int addressLen  ) 
{ 
	hal_printf("Inside SOCK_bind in sockets_stubs.cpp\n");
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

int SOCK_connect(int socket, const struct SOCK_sockaddr* address, int addressLen) 
{ 
	hal_printf("Inside SOCK_connect in sockets_stubs.cpp\n");
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

int SOCK_send(int socket, const char* buf, int len, int flags) 
{ 
	hal_printf("Inside SOCK_send in sockets_stubs.cpp\n");
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

int SOCK_recv(int socket, char* buf, int len, int timeout) 
{ 
	hal_printf("Inside SOCK_recv in sockets_stubs.cpp\n");
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

int SOCK_close(int socket) 
{ 
	hal_printf("Inside SOCK_close in sockets_stubs.cpp\n");
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

int SOCK_listen( int socket, int backlog ) 
{ 
	hal_printf("Inside SOCK_listen in sockets_stubs.cpp\n");
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

int SOCK_accept( int socket, struct SOCK_sockaddr* address, int* addressLen ) 
{ 
	hal_printf("Inside SOCK_accept in sockets_stubs.cpp\n");
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

int SOCK_shutdown( int socket, int how ) 
{ 
	hal_printf("Inside SOCK_shutdown in sockets_stubs.cpp\n");
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

int SOCK_getaddrinfo(  const char* nodename, char* servname, const struct SOCK_addrinfo* hints, struct SOCK_addrinfo** res ) 
{ 
	hal_printf("Inside SOCK_getaddrinfo in sockets_stubs.cpp\n");
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

void SOCK_freeaddrinfo( struct SOCK_addrinfo* ai )
{
	hal_printf("Inside SOCK_freeaddrinfo in sockets_stubs.cpp\n");
    NATIVE_PROFILE_PAL_COM();
}
    

int SOCK_ioctl( int socket, int cmd, int* data ) 
{ 
	hal_printf("Inside SOCK_ioctl in sockets_stubs.cpp\n");
	NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

int SOCK_getlasterror() 
{ 
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

int SOCK_select( int socket, SOCK_fd_set* readfds, SOCK_fd_set* writefds, SOCK_fd_set* except, const struct SOCK_timeval* timeout ) 
{ 
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

int SOCK_setsockopt( int socket, int level, int optname, const char* optval, int  optlen ) 
{ 
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

int SOCK_getsockopt( int socket, int level, int optname,       char* optval, int* optlen ) 
{ 
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

int SOCK_getpeername( int socket, struct SOCK_sockaddr* name, int* namelen ) 
{ 
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

int SOCK_getsockname( int socket, struct SOCK_sockaddr* name, int* namelen ) 
{ 
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

int SOCK_recvfrom( int s, char* buf, int len, int flags, struct SOCK_sockaddr* from, int* fromlen ) 
{ 
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

int SOCK_sendto( int s, const char* buf, int len, int flags, const struct SOCK_sockaddr* to, int tolen ) 
{ 
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

