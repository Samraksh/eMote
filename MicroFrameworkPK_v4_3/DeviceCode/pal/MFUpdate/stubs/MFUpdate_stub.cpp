////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <MFUpdate_decl.h>

void  MFUpdate_Initialize(void)
{
}

BOOL MFUpdate_GetProperty( UpdateID_t updateHandle, LPCSTR szPropName, UINT8* pPropValue, INT32* pPropValueSize )
{
    return FALSE;
}

BOOL MFUpdate_SetProperty( UpdateID_t updateHandle, LPCSTR szPropName, UINT8* pPropValue, INT32 pPropValueSize )
{
    return FALSE;
}

UpdateID_t MFUpdate_InitUpdate( LPCSTR szProvider, MFUpdateHeader& update )
{
    return MFUpdate::badHandle;
}

BOOL MFUpdate_AuthCommand( UpdateID_t updateHandle, UINT32 cmd, UINT8* pArgs, INT32 argsLen, UINT8* pResponse, INT32& responseLen )
{
    return FALSE;
}

BOOL MFUpdate_Authenticate( UpdateID_t updateHandle, UINT8* pAuthData, INT32 authLen )
{
    return FALSE;
}


BOOL MFUpdate_Open( UpdateID_t updateHandle )
{
    return FALSE;
}

BOOL MFUpdate_Create( UpdateID_t updateHandle )
{
    return FALSE;
}

BOOL MFUpdate_GetMissingPackets( UpdateID_t updateHandle, UINT32* pPacketBits, INT32* pCount )
{
    return FALSE;
}

BOOL MFUpdate_AddPacket( UpdateID_t updateHandle, INT32 packetIndex, UINT8* packetData, INT32 packetLen, UINT8* pValidationData, INT32 validationLen )
{
    return FALSE;
}

BOOL MFUpdate_Validate( UpdateID_t updateHandle, UINT8* pValidationData, INT32 validationLen )
{
    return FALSE;
}

BOOL MFUpdate_Install( UpdateID_t updateHandle, UINT8* pValidationData, INT32 validationLen )
{
    return FALSE;
}

BOOL MFUpdate_Delete( UpdateID_t updateHandle )
{
    return FALSE;
}

