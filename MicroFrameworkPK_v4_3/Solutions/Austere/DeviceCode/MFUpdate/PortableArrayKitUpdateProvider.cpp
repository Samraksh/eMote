/*
 * PortableArrayKitUpdateProvider.cpp
 *
 *  Created on: Apr 9, 2013
 *      Author: mcgrathm
 *      Summary: Is an update provider to MFUpdate interface on eMote platform.
 *      Update providers take care of installing the update.  This update provider is based on parts of MicroBooter.
 */

#include "PortableArrayKitUpdateProvider.h"
#include <TinyCLR_Types.h>

/**
 * MFUpdate's IUpdateProvider functions.
 */
IUpdateProvider g_PortableArrayKitUpdateProvider =
{
    PortableArrayKitUpdateProvider::InitializeUpdate,
    PortableArrayKitUpdateProvider::GetProperty,
    PortableArrayKitUpdateProvider::SetProperty,
    PortableArrayKitUpdateProvider::InstallUpdate
};

extern void ClrReboot(void);

/**
 * Initializes the given update by adding corresponding update components interface pointers
 * (IUpdateProvider, IUpdateStorageProvider, IUpdateBackupProvider, etc.)
 */
BOOL PortableArrayKitUpdateProvider::InitializeUpdate( MFUpdate* pUpdate )
{
    if(pUpdate == NULL) return FALSE;

    switch(pUpdate->Header.UpdateType)
    {
        case MFUPDATE_UPDATETYPE_FIRMWARE:
        case MFUPDATE_UPDATETYPE_ASSEMBLY:
            break;

        default:
            return FALSE;
    }

    return TRUE;
}

/**
 * Gets the property value for the given property name.  Properties are extensions of the MFUpdate base.
 */
BOOL PortableArrayKitUpdateProvider::GetProperty( MFUpdate* pUpdate, LPCSTR szPropName , UINT8* pPropValue, INT32* pPropValueSize )
{
    return FALSE;
}

/**
 * Sets the property value for the given property name.  Properties are extensions of the MFUpdate base.
 */
BOOL PortableArrayKitUpdateProvider::SetProperty( MFUpdate* pUpdate, LPCSTR szPropName , UINT8* pPropValue, INT32 pPropValueSize )
{
    return FALSE;
}

/**
 * Verifies and installs the update.
 */
BOOL PortableArrayKitUpdateProvider::InstallUpdate( MFUpdate* pUpdate, UINT8* pValidation, INT32 validationLen )
{
    if(pUpdate->Providers->Storage == NULL)
    {
        SOFT_BREAKPOINT();
        return FALSE;
    }

    if(!pUpdate->IsValidated())
    {
        SOFT_BREAKPOINT();
        return FALSE;
    }

    switch(pUpdate->Header.UpdateType)
    {
        /*//From MicroBooterUpdate
        case MFUPDATE_UPDATETYPE_FIRMWARE:
            {
                HAL_UPDATE_CONFIG cfg;

                if(sizeof(cfg.UpdateSignature) < validationLen) return FALSE;

                cfg.Header.Enable = TRUE;
                cfg.UpdateID = pUpdate->Header.UpdateID;

                if(validationLen == sizeof(UINT32))
                {
                    cfg.UpdateSignType = HAL_UPDATE_CONFIG_SIGN_TYPE__CRC;
                }
                else
                {
                    cfg.UpdateSignType = HAL_UPDATE_CONFIG_SIGN_TYPE__SIGNATURE;
                }

                memcpy( cfg.UpdateSignature, pValidation, validationLen );

                if(HAL_CONFIG_BLOCK::UpdateBlockWithName(cfg.GetDriverName(), &cfg, sizeof(cfg), FALSE))
                {
                    CPU_Reset();
                }
            }
            break;
        */
        case MFUPDATE_UPDATETYPE_ASSEMBLY:
            {
                BlockStorageStream stream;

                if(NULL == BlockStorageList::GetFirstDevice())
                {
                    BlockStorageList::Initialize();

                    BlockStorage_AddDevices();

                    BlockStorageList::InitializeDevices();
                }

                if(stream.Initialize(BlockUsage::DEPLOYMENT))
                {
                    if(pUpdate->Header.UpdateSubType == MFUPDATE_UPDATESUBTYPE_ASSEMBLY_REPLACE_DEPLOY)
                    {
                        do
                        {
                            stream.Erase(stream.Length);
                        }
                        while(stream.NextStream());

                        stream.Initialize(BlockUsage::DEPLOYMENT);
                    }

                    do
                    {
                        UINT8 buf[512];
                        INT32 offset = 0;
                        INT32 len = sizeof(buf);
                        const BlockDeviceInfo* deviceInfo = stream.Device->GetDeviceInfo();
                        BOOL isXIP = deviceInfo->Attribute.SupportsXIP;

                        const CLR_RECORD_ASSEMBLY* header;
                        INT32  headerInBytes = sizeof(CLR_RECORD_ASSEMBLY);
                        BYTE * headerBuffer  = NULL;

                        if(!isXIP)
                        {
                            headerBuffer = (BYTE*)private_malloc( headerInBytes );  if(!headerBuffer) return FALSE;
                            memset( headerBuffer, 0,  headerInBytes );
                        }

                        while(TRUE)
                        {
                            if(!stream.Read( &headerBuffer, headerInBytes )) break;

                            header = (const CLR_RECORD_ASSEMBLY*)headerBuffer;

                            // check header first before read
                            if(!header->GoodHeader())
                            {
                                //deployment's header is bad so erase/write over it.
                                stream.Seek(-headerInBytes);

                                if(stream.IsErased(pUpdate->Header.UpdateSize))
                                {
                                    while(offset < pUpdate->Header.UpdateSize)
                                    {
                                        if((pUpdate->Header.UpdateSize - offset) < len)
                                        {
                                            len = pUpdate->Header.UpdateSize - offset;
                                        }

                                        offset += pUpdate->Providers->Storage->Read(pUpdate->StorageHandle, offset, buf, len);

                                        stream.Write(buf, len);
                                    }

                                    ClrReboot();
                                    return TRUE;
                                }
                                break;
                            }

                            UINT32 AssemblySizeInByte = ROUNDTOMULTIPLE(header->TotalSize(), CLR_UINT32);

                            stream.Seek( AssemblySizeInByte );
                        }
                        if(!isXIP) private_free( headerBuffer );

                    }
                    while(stream.NextStream());
                }

            }
            break;
    }
    ASSERT_SP(0);
    return FALSE;
}



