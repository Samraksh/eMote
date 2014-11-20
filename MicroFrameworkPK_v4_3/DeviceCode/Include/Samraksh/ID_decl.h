#ifndef _DRIVERS_ID_DECL_H_
#define _DRIVERS_ID_DECL_H_ 1

#include <tinyhal.h>
#include <tinyhal_releaseinfo.h>

/**
 * @file:   ID_decl.h
 * @author: Michael Andrew McGrath <Michael.McGrath@Samraksh.com>
 * @date:   Nov 20, 2014
 * @copyright: (C) 2014 The Samraksh Company
 * @brief: device-specific HAL functions for obtaining unique ID/serial number.
 * @detail: pal\configuration\reduc\ConfigHelper.cpp:GetHalSystemInfo() calls ID_GetSerialNumbers().
 */

unsigned int ID_GetSerialNumbers(struct OEM_SERIAL_NUMBERS& OemConfig );

unsigned short ID_InitializeMF_NODE_ID();

/**
 * MF_NODE_ID was originally declared as const in platform_selector.h.  Then a developer made MF_NODE_ID a regular read/write variable, but still left MF_NODE_ID the header file.  So MF_NODE_ID showed up in many translation units, and ended up making multiple definitions in the binary.  RF231 used the CPU unique ID and wrote MF_NODE_ID, but reading the CPU ID needed moved to an API so it could be used by both the RF231 driver and MFDeploy's hardware serial number response.
 * For now, MF_NODE_ID is left in scope as a global variable to avoid reworking too much code, but it is moved to the ID driver to stop defining it in the header file, and to prevent the radio initializing the MAC.
 */
extern UINT16 MF_NODE_ID;

#endif /* _DRIVERS_ID_DECL_H_ */
