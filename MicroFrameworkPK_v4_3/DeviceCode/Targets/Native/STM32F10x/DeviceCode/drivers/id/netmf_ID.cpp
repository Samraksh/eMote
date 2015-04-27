
/**
 * @file: STM32F10x_ID.cpp
 * @author: Michael Andrew McGrath
 * @copyright: The Samraksh Company, (C) 2014
 * @date: April 2, 2014
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

#include <tinyhal.h>

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
