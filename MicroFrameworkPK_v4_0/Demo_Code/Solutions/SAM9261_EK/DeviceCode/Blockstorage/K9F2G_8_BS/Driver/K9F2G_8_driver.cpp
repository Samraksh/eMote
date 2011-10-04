////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "..\K9F2G_8.h"

//--//

extern K9F2G_8_BS_Driver g_K9F2G_8_BS_Driver;

static BYTE  s_sectorBuff[2048];

//--//

void K9F2G_8_BS_Driver::WaitReady()
{
    // Loop uintl Ready/Busy Pin returns to High state
    while (!CPU_GPIO_GetPinState(AT91_NAND_RB));
}


BOOL K9F2G_8_BS_Driver::ChipInitialize( void* context )
{
    AT91_SMC &smctrl = AT91::SMCTRL();

    AT91_MATRIX &matrix = AT91::MATRIX();


    GLOBAL_LOCK(irq);

    matrix.MATRIX_EBICSA |= AT91_MATRIX::MATRIX_CS3A_SM;

    // Configures the EBI for NandFlash access at 48MHz
    smctrl.SMC_SETUP3 = 0x00010001;
    smctrl.SMC_PULSE3 = 0x04030302;
    smctrl.SMC_CYCLE3 = 0x00070004;
    smctrl.SMC_CTRL3  = (AT91_SMC::SMC_READMODE
                       | AT91_SMC::SMC_WRITEMODE
                       | AT91_SMC::SMC_NWAITM_NWAIT_DISABLE
                       | ((0x1 << 16) & AT91_SMC::SMC_TDF))
                       | AT91_SMC::SMC_DBW_WIDTH_EIGTH_BITS;

    CPU_GPIO_DisablePin(AT91_NAND_RE, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_1);
    CPU_GPIO_DisablePin(AT91_NAND_WE, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_1);	
    CPU_GPIO_EnableInputPin(AT91_NAND_RB, TRUE, NULL, GPIO_INT_NONE, RESISTOR_PULLUP);
    CPU_GPIO_EnableOutputPin(AT91_NAND_CE, TRUE);

    EnableCE();
    WriteCommand(COMMAND_RESET);
    WaitReady();
    DisableCE();

    
    FLASH_WORD ManufacturerCode = 0;
    FLASH_WORD DeviceCode       = 0;

    ReadProductID(context, ManufacturerCode, DeviceCode );
   
    if (0xEC != ManufacturerCode)
    {
        debug_printf( "Flash_ChipInitialize: ManufacturerCode failure!\r\n" );
        return FALSE;
    }

    if (0xDA != DeviceCode)
    {
        debug_printf( "Flash_ChipInitialize: DeviceCode failure!\r\n" );
        return FALSE;		
    }
/* Test
    {
        UINT8 Buf[2048];

        EraseBlock(0);
        ReadSector(0, 1, Buf, NULL);
        for(UINT32 i = 0; i < 2048; i++)
        {
            if(Buf[i] != 0xFF)
            {
                debug_printf("Erase operation failed!\r\n");
                return FALSE;
            }
        }
        for(UINT32 i = 0; i < 2048; i++)
        {
            Buf[i] = i & 0xFF;
        }
        WriteSector(0, 1, Buf, NULL);
        for(UINT32 i = 0; i < 2048; i++)
        {
            Buf[i] = 0;
        }
        ReadSector(0, 1, Buf, NULL);
        for(UINT32 i = 0; i < 2048; i++)
        {
            if(Buf[i] != (i &0xFF))
            {
                debug_printf("Write operation failed!\r\n");
                return FALSE;
            }
        }
    }
*/
    return TRUE;

}

BOOL K9F2G_8_BS_Driver::ChipUnInitialize( void* context )
{
    return TRUE;
}

BOOL K9F2G_8_BS_Driver::ReadProductID( void* context, FLASH_WORD& ManufacturerCode, FLASH_WORD& DeviceCode )
{
    EnableCE();

    WriteCommand(COMMAND_READID);
    WriteAddress(0);

    ManufacturerCode  = (FLASH_WORD)ReadData8();
    DeviceCode = (FLASH_WORD)ReadData8();
   
    DisableCE();

    return TRUE;
}


const BlockDeviceInfo* K9F2G_8_BS_Driver::GetDeviceInfo( void* context)
{
    BLOCK_CONFIG* config = (BLOCK_CONFIG* )context;
    
    return config->BlockDeviceInformation;    
}

BOOL K9F2G_8_BS_Driver::Read( void* context, ByteAddress byteAddr, UINT32 NumBytes, BYTE * pSectorBuff )
{
    NATIVE_PROFILE_HAL_DRIVERS_FLASH();
    UINT32 RegionIndex, RangeIndex;
    UINT32 BytesToRead;

    BLOCK_CONFIG* pConfig = (BLOCK_CONFIG*)context;

    if (pSectorBuff == NULL) return FALSE;
    
    if (pConfig->BlockDeviceInformation->FindRegionFromAddress( byteAddr, RegionIndex, RangeIndex))
    {
        const BlockRegionInfo* pRegion = &pConfig->BlockDeviceInformation->Regions[RegionIndex];
        
        UINT32 BytesPerSector = pConfig->BlockDeviceInformation->BytesPerSector;

        ByteAddress StartSector = pConfig->BlockDeviceInformation->PhysicalToSectorAddress( pRegion, byteAddr );

        INT32 offset = ((byteAddr - pRegion->Start) % BytesPerSector);

        CHIP_WORD *pBuf = (CHIP_WORD *)pSectorBuff;

        BytesToRead = (NumBytes + offset > BytesPerSector ? BytesPerSector - offset : NumBytes);

        EnableCE();

        while(NumBytes > 0)
        {
            WriteCommand(COMMAND_READ_1);

            WriteAddress((offset >> 0) & 0xFF);
            WriteAddress((offset >> 8) & 0xFF);
            WriteAddress((StartSector >>  0) & 0xFF);
            WriteAddress((StartSector >>  8) & 0xFF);
            WriteAddress((StartSector >> 16) & 0xFF);

            WriteCommand(COMMAND_READ_2);
            WaitReady();
            // Read loop
            for (UINT32 j = 0; j < BytesToRead; j++)
            {
                *pBuf++ = ReadData8();
            }

            offset = 0;

            NumBytes -= BytesToRead;

            BytesToRead = __min(NumBytes, BytesPerSector);

            StartSector++;

        }

        DisableCE();

        return TRUE;
    }
    else
    {
        return FALSE;
    }

}


BOOL K9F2G_8_BS_Driver::ReadPage(SectorAddress StartSector, BYTE *outBuf, BYTE* inBuf, UINT32 size, UINT32 offset, UINT32 sectorSize, BOOL fIncrementDataPtr)
{
    // EnableCE and DisableCE is done by caller.
    BYTE* wrBur = outBuf;
    BOOL fModifyRequired = FALSE;

    WriteCommand(COMMAND_READ_1);
    
    WriteAddress(0);
    WriteAddress(0);
    WriteAddress((StartSector >>  0) & 0xFF);
    WriteAddress((StartSector >>  8) & 0xFF);
    WriteAddress((StartSector >> 16) & 0xFF);

    WriteCommand(COMMAND_READ_2);
    WaitReady();
    // Read loop
    for (UINT32 j = 0; j < sectorSize; j++)
    {
        *wrBur++ = ReadData8();
    }

    // replace the data be written
    outBuf = (BYTE*) ((UINT32)outBuf + offset);
    for(UINT32 i = 0; i<size; i++)
    {
        if(!fModifyRequired && (0 != (~*outBuf & *inBuf)))
        {
            fModifyRequired = TRUE;
        }
        
        *outBuf ++ = *inBuf;

        if(fIncrementDataPtr) inBuf++;
    }
    
    return fModifyRequired;
}

BOOL K9F2G_8_BS_Driver::Write( void* context, ByteAddress byteAddr, UINT32 NumBytes, BYTE * pSectorBuff, BOOL ReadModifyWrite )
{
    NATIVE_PROFILE_PAL_FLASH();
    return WriteX( context, byteAddr, NumBytes, pSectorBuff, ReadModifyWrite, TRUE );
}

BOOL K9F2G_8_BS_Driver::Memset( void* context, ByteAddress byteAddr, UINT8 Data, UINT32 NumBytes )
{
    NATIVE_PROFILE_PAL_FLASH();

    CHIP_WORD chipData;

    memset(&chipData, Data, sizeof(CHIP_WORD));

    return WriteX( context, byteAddr, NumBytes, &chipData, TRUE, FALSE );
}

BOOL K9F2G_8_BS_Driver::WriteX( void* context, ByteAddress byteAddr, UINT32 NumBytes, BYTE * pSectorBuff, BOOL ReadModifyWrite, BOOL fIncrementDataPtr )
{
    NATIVE_PROFILE_PAL_FLASH();

    UINT32 RegionIndex, RangeIndex;
    UINT32 BytesToWrite;
    UINT32 Status;

    const BlockRegionInfo *pRegion;

    CHIP_WORD * pData;

    BLOCK_CONFIG* pConfig = (BLOCK_CONFIG*)context;

    if (pSectorBuff == NULL) return FALSE;
    
    if (!pConfig->BlockDeviceInformation->FindRegionFromAddress( byteAddr, RegionIndex, RangeIndex))
    {
        return FALSE;
    }

    UINT32 BytesPerSector = pConfig->BlockDeviceInformation->BytesPerSector;

    pRegion = &pConfig->BlockDeviceInformation->Regions[RegionIndex];

    pData = (CHIP_WORD *)pSectorBuff;

    SectorAddress StartSector = pConfig->BlockDeviceInformation->PhysicalToSectorAddress( pRegion, byteAddr );

    INT32 offset = (byteAddr - pRegion->Start) % BytesPerSector;

    BytesToWrite = (NumBytes + offset > BytesPerSector ? BytesPerSector - offset : NumBytes);

    CHIP_WORD* pWrite;
    
    EnableCE();

    while(NumBytes > 0)
    {
        BOOL fIncr = fIncrementDataPtr; 
        BOOL fRMW = ReadModifyWrite;

        // less than a page, read a page in 
        if (ReadModifyWrite)
        {
            pWrite = &s_sectorBuff[0];
            
            if(ReadPage( StartSector, pWrite, pData, BytesToWrite, offset, BytesPerSector, fIncrementDataPtr ))
            {
                EraseBlock( context, StartSector * BytesPerSector );

                offset = 0;
                fIncr = TRUE;
            }
            else
            {
                fRMW = FALSE;
                pWrite = pData;
            }
        }
        else
        {
            pWrite = pData;
        }

        WriteCommand(COMMAND_WRITE_1);

        WriteAddress((offset >> 0) & 0xFF);
        WriteAddress((offset >> 8) & 0xFF);
        WriteAddress((StartSector >>  0) & 0xFF);
        WriteAddress((StartSector >>  8) & 0xFF);
        WriteAddress((StartSector >> 16) & 0xFF);

        // Write loop
        for (UINT32 j = 0; j < (fRMW ? BytesPerSector : BytesToWrite); j++)
        {
            WriteData8(*pWrite);

            if(fIncr) pWrite++;
        }

        WriteCommand(COMMAND_WRITE_2);

        WaitReady();
        WriteCommand(COMMAND_STATUS);
        Status = ReadData8();

        if (((Status & STATUS_READY) != STATUS_READY) || ((Status & STATUS_ERROR) != 0))
        {
            DisableCE();
            return FALSE;	
        }

        if(fIncrementDataPtr) 
        {
            pData += BytesToWrite;
        }
        
        NumBytes -= BytesToWrite;

        BytesToWrite = __min(NumBytes, BytesPerSector);

        StartSector++;
    }

    DisableCE();

    return TRUE;

}


BOOL K9F2G_8_BS_Driver::GetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata)
{
    if (pSectorMetadata == NULL) return FALSE;

    BLOCK_CONFIG* pConfig = (BLOCK_CONFIG*)context;
    UINT32 offset         = pConfig->BlockDeviceInformation->BytesPerSector;
    UINT32 BytesToRead    = sizeof(SectorMetadata);
    UINT8* pData          = (UINT8*)pSectorMetadata;

    SectorStart = SectorStart / pConfig->BlockDeviceInformation->BytesPerSector;
    
    EnableCE();

    WriteCommand(COMMAND_READ_1);

    WriteAddress((offset >> 0) & 0xFF);
    WriteAddress((offset >> 8) & 0xFF);
    WriteAddress((SectorStart >>  0) & 0xFF);
    WriteAddress((SectorStart >>  8) & 0xFF);
    WriteAddress((SectorStart >> 16) & 0xFF);

    WriteCommand(COMMAND_READ_2);
    WaitReady();

    // Read loop
    while(BytesToRead--)
    {
        *pData++ = ReadData8();
    }

    DisableCE();

    return TRUE;
}

BOOL K9F2G_8_BS_Driver::SetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata)
{
    BLOCK_CONFIG* pConfig = (BLOCK_CONFIG*)context;
    UINT32 offset         = pConfig->BlockDeviceInformation->BytesPerSector;
    UINT32 BytesToWrite   = sizeof(SectorMetadata);
    UINT8* pData          = (UINT8*)pSectorMetadata;
    UINT8  Status;

    SectorStart = SectorStart / pConfig->BlockDeviceInformation->BytesPerSector;
    
    EnableCE();

    WriteCommand(COMMAND_WRITE_1);

    WriteAddress((offset >> 0) & 0xFF);
    WriteAddress((offset >> 8) & 0xFF);
    WriteAddress((SectorStart >>  0) & 0xFF);
    WriteAddress((SectorStart >>  8) & 0xFF);
    WriteAddress((SectorStart >> 16) & 0xFF);

    // Write loop
    while(BytesToWrite--)
    {
        WriteData8(*pData++);
    }

    WriteCommand(COMMAND_WRITE_2);

    WaitReady();
    WriteCommand(COMMAND_STATUS);
    Status = ReadData8();

    if (((Status & STATUS_READY) != STATUS_READY) || ((Status & STATUS_ERROR) != 0))
    {
        DisableCE();
        return FALSE;	
    }

    DisableCE();

    return TRUE;
}


BOOL K9F2G_8_BS_Driver::IsBlockErased( void* context, ByteAddress BlockStartAddress, UINT32 BlockLength  )
{

    NATIVE_PROFILE_HAL_DRIVERS_FLASH();

    UINT32 RegionIndex, RangeIndex;

    BLOCK_CONFIG* pConfig = (BLOCK_CONFIG*)context;

    if (!pConfig->BlockDeviceInformation->FindRegionFromAddress( BlockStartAddress, RegionIndex, RangeIndex))
    {
        return FALSE;
    }

    const BlockRegionInfo* pRegion = &pConfig->BlockDeviceInformation->Regions[RegionIndex];

    // we can do this because we know the block length is 2^n
    ByteAddress StartSector = pConfig->BlockDeviceInformation->PhysicalToSectorAddress( pRegion, BlockStartAddress );
    UINT32 BytesPerSector   = pConfig->BlockDeviceInformation->BytesPerSector;
    UINT32 SectorsPerBlock  = pRegion->BytesPerBlock / BytesPerSector;
    BOOL fErased = TRUE;

    EnableCE();

    for(UINT32 i=0; i<SectorsPerBlock && fErased; i++)
    {
        WriteCommand(COMMAND_READ_1);

        WriteAddress(0);
        WriteAddress(0);
        WriteAddress((StartSector >>  0) & 0xFF);
        WriteAddress((StartSector >>  8) & 0xFF);
        WriteAddress((StartSector >> 16) & 0xFF);

        WriteCommand(COMMAND_READ_2);
        WaitReady();
        // Read loop
        for (UINT32 j = 0; j < BytesPerSector; j++)
        {
            if( ReadData8() != 0xFF)
            {
                fErased = FALSE;
                break;
            }
        }

        StartSector++;

    }

    DisableCE();

    return fErased;

}

BOOL K9F2G_8_BS_Driver::EraseBlock( void* context, ByteAddress byteAddress )
{
    NATIVE_PROFILE_HAL_DRIVERS_FLASH();

    UINT32 RegionIndex, RangeIndex;
    UINT8  Status;

    BLOCK_CONFIG* pConfig = (BLOCK_CONFIG*)context;

    if (!pConfig->BlockDeviceInformation->FindRegionFromAddress( byteAddress, RegionIndex, RangeIndex))
    {
        return FALSE;
    }

    const BlockRegionInfo* pRegion = &pConfig->BlockDeviceInformation->Regions[RegionIndex];

    SectorAddress RdSector = pConfig->BlockDeviceInformation->PhysicalToSectorAddress( pRegion, byteAddress );

    EnableCE();

    WriteCommand(COMMAND_ERASE_1);

    WriteAddress((RdSector >>  0) & 0xFF);
    WriteAddress((RdSector >>  8) & 0xFF);
    WriteAddress((RdSector >> 16) & 0xFF);

    WriteCommand(COMMAND_ERASE_2);

    WaitReady();

    WriteCommand(COMMAND_STATUS);

    Status = ReadData8();

    DisableCE();

    if (((Status & STATUS_READY) != STATUS_READY) || ((Status & STATUS_ERROR) != 0))
        return FALSE;

    return TRUE;

}

void K9F2G_8_BS_Driver::SetPowerState( void* context, UINT32 State )
{
    // our flash driver is always Power ON
    return ;
}


UINT32 K9F2G_8_BS_Driver::MaxSectorWrite_uSec( void* context )
{
    NATIVE_PROFILE_PAL_FLASH();

    BLOCK_CONFIG* config = (BLOCK_CONFIG* )context;
    
    return config->BlockDeviceInformation->MaxSectorWrite_uSec;
}

UINT32 K9F2G_8_BS_Driver::MaxBlockErase_uSec( void* context )
{
    NATIVE_PROFILE_PAL_FLASH();
    
    BLOCK_CONFIG* config = (BLOCK_CONFIG* )context;

    return config->BlockDeviceInformation->MaxBlockErase_uSec;

}

struct IBlockStorageDevice g_K9F2G08_BS_DeviceTable = 
{                          
    &K9F2G_8_BS_Driver::ChipInitialize,
    &K9F2G_8_BS_Driver::ChipUnInitialize,
    &K9F2G_8_BS_Driver::GetDeviceInfo,
    &K9F2G_8_BS_Driver::Read,
    &K9F2G_8_BS_Driver::Write,
    &K9F2G_8_BS_Driver::Memset,
    &K9F2G_8_BS_Driver::GetSectorMetadata,
    &K9F2G_8_BS_Driver::SetSectorMetadata,
    &K9F2G_8_BS_Driver::IsBlockErased,
    &K9F2G_8_BS_Driver::EraseBlock,
    &K9F2G_8_BS_Driver::SetPowerState,
    &K9F2G_8_BS_Driver::MaxSectorWrite_uSec,
    &K9F2G_8_BS_Driver::MaxBlockErase_uSec,    
};

