////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRIVERS_BLUETOOTH_DECL_H_
#define _DRIVERS_BLUETOOTH_DECL_H_ 1

#include <Samraksh\Mac_decl.h>

DeviceStatus CPU_Bluetooth_Initialize(MACEventHandler* eventHandler );
BOOL CPU_Bluetooth_Reset();
BOOL CPU_Bluetooth_UnInitialize();
DeviceStatus Bluetooth_Send_Data(UINT16 dest, uint8_t *buffer, uint16_t buffer_size);
DeviceStatus Bluetooth_Receive_Data(UINT16 packetType, uint8_t *buffer, uint16_t buffer_size);
DeviceStatus GetNeighbors(UINT16* buffer);
void CPU_Bluetooth_Connected(int number, int connectionType);
void CPU_Bluetooth_Disconnected(int number, int connectionType);

#endif // _DRIVERS_BLUETOOTH_DECL_H_

