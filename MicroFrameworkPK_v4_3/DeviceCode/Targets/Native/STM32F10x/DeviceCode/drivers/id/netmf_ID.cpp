/**
 * @file   netmf_ID.cpp
 * @author Michael Andrew McGrath <Michael.McGrath@Samraksh.com>
 * @date   April 2, 2014
 * @copyright (C) 2014 The Samraksh Company
 */
 /**
 @details: Design goal: The STM32F10x has a Unique ID, but such a hardware item currently
 is not exploited by any NetMF APIs.  The flash memory has a ConfigurationSector
 for storing unique items like crypto keys and serial numbers, but we currently
 aren't using it.  The ConfigSector is messed up - it is declared in
 OEM_Model_SKU_NetworkID.cpp next to tinyhal.cpp instead of per-cpu or per-solution.
 So for the time being, keep OEM_Model_SKU_NetworkID.cpp in the same location but
 initialize parts of g_ConfigurationSector at boot.  This way, we've abstracted the
 serial number into the configuration sector, where it could be controlled and
 accessed via MFDeploy's DeviceInfo and other places.
 */

#include <Samraksh\ID_decl.h>

UINT16 MF_NODE_ID;

unsigned int ID_GetSerialNumbers(struct OEM_SERIAL_NUMBERS& OemSerialNumbers)
{
    unsigned int s1,s2,s3;
    s1 = *(volatile unsigned int*)(0x1FFFF7E8);
    s2 = *(volatile unsigned int*)(0x1FFFF7EC);
    s3 = *(volatile unsigned int*)(0x1FFFF7F0);

    memcpy((void*)&(OemSerialNumbers.system_serial_number),(void*)&s1,sizeof(s1));
    memcpy((void*)&(OemSerialNumbers.system_serial_number[4]),(void*)&s2,sizeof(s2));
    memcpy((void*)&(OemSerialNumbers.system_serial_number[8]),(void*)&s3,sizeof(s3));

    memcpy((void*)&(OemSerialNumbers.module_serial_number),(void*)&OemSerialNumbers.system_serial_number,sizeof(OemSerialNumbers.system_serial_number));
    return 1;
}


BOOL GetCPUSerial(UINT8 * ptr, UINT16 num_of_bytes ){
    UINT32 Device_Serial0;UINT32 Device_Serial1; UINT32 Device_Serial2;
    Device_Serial0 = *(UINT32*)(0x1FFFF7E8);
    Device_Serial1 = *(UINT32*)(0x1FFFF7EC);
    Device_Serial2 = *(UINT32*)(0x1FFFF7F0);
    if(num_of_bytes==12){
        ptr[0] = (UINT8)(Device_Serial0 & 0x000000FF);
        ptr[1] = (UINT8)((Device_Serial0 & 0x0000FF00) >> 8);
        ptr[2] = (UINT8)((Device_Serial0 & 0x00FF0000) >> 16);
        ptr[3] = (UINT8)((Device_Serial0 & 0xFF000000) >> 24);

        ptr[4] = (UINT8)(Device_Serial1 & 0x000000FF);
        ptr[5] = (UINT8)((Device_Serial1 & 0x0000FF00) >> 8);
        ptr[6] = (UINT8)((Device_Serial1 & 0x00FF0000) >> 16);
        ptr[7] = (UINT8)((Device_Serial1 & 0xFF000000) >> 24);

        ptr[8] = (UINT8)(Device_Serial2 & 0x000000FF);
        ptr[9] = (UINT8)((Device_Serial2 & 0x0000FF00) >> 8);
        ptr[10] = (UINT8)((Device_Serial2 & 0x00FF0000) >> 16);
        ptr[11] = (UINT8)((Device_Serial2 & 0xFF000000) >> 24);
        return TRUE;
    }
    else return FALSE;

}

unsigned short ID_InitializeMF_NODE_ID() {
    //Get cpu serial and hash it to use as node id
    UINT8 cpuserial[12];
    GetCPUSerial(cpuserial,12);
    MF_NODE_ID=0;
    UINT16 * temp = (UINT16 *) cpuserial;
    for (int i=0; i< 6; i++){
        MF_NODE_ID=MF_NODE_ID ^ temp[i]; //XOR 72-bit number to generate 16-bit hash
    }
}
