/*
 *
 * 		Name  : DS18S20.cpp
 *
 *      Author : nived.sivadas@samraksh.com
 *
 *      Description : Contains the driver to interface with the temperature sensor, internally used OneWire protocol
 * 					  for communication. MF 4.3 and above come with a one wire implementation but the temperature sensor
 * 					  does not seem to respond to this protocol which led to this implementation
 */


#include "DS18S20.h"

DS18S20 gDS18S20_Driver;

BOOL WriteBit(int portnum, UINT8 sendbit);


// This function takes the port num as input and is designed to elicit a presence response from the
// temperature sensor
DeviceStatus Reset(UINT16 portnum)
{
   UINT8 result;
   UINT32 pin = (UINT32)portnum;

   // Code from appnote 126.
   CPU_GPIO_EnableOutputPin( pin, false );  // impulse start OW_PORT = 0; // drive bus low.

   HAL_Time_Sleep_MicroSeconds(600);

   // OW_PORT = 1; // bus high.
   // Note that the 1Wire bus will need external pullup to supply the required current
   CPU_GPIO_EnableInputPin( pin, false, NULL, GPIO_INT_EDGE_HIGH, RESISTOR_PULLUP );

   HAL_Time_Sleep_MicroSeconds(65);

   result = CPU_GPIO_GetPinState(pin); 	//!OW_PORT; // get presence detect pulse.

   HAL_Time_Sleep_MicroSeconds(372);

   if(result == 0)
	   return DS_Success;
   else
	   return DS_Fail;
}

UINT8 WriteByte(int portnum, UINT8 sendbyte)
{
   UINT8 i;
   UINT8 result = 0;

   for (i = 0; i < 8; i++)
   {
       result |= (WriteBit(portnum,sendbyte & 1) << i);
       sendbyte >>= 1;
   }

   return result;
}

BOOL WriteBit(int portnum, UINT8 sendbit)
{
	   unsigned char result=0;
	   UINT32 pin = (UINT32)portnum;

	   //timing critical, so I'll disable interrupts here
	   GLOBAL_LOCK(irq); //EA = 0;

	   // start timeslot.
	   CPU_GPIO_EnableOutputPin( pin, false );

	   HAL_Time_Sleep_MicroSeconds(5);

	   if (sendbit == 1)
	   {
		   // send 1 bit out.
		   // sample result @ 15 us.
		   CPU_GPIO_EnableInputPin( pin, false, NULL, GPIO_INT_EDGE_HIGH, RESISTOR_PULLUP );
		   HAL_Time_Sleep_MicroSeconds(5);
		   result = CPU_GPIO_GetPinState(pin);
		   HAL_Time_Sleep_MicroSeconds(35);
	   }
	   else
	   {
	      // send 0 bit out.
	     ::CPU_GPIO_SetPinState(pin, 0);
	     HAL_Time_Sleep_MicroSeconds(50);
	   }
	   // timeslot done. Set output high.
	   CPU_GPIO_EnableOutputPin( pin, true );

	   //HAL_Time_Sleep_MicroSeconds(5);

	   //restore interrupts
	   irq.Release();

	   if (result != 0)
	   {
	     result = 1;
	   }
	   else
	   {
	     result = 0;
	   }

	   return result;

}

BOOL ReadROM(UINT16 portnum)
{
	UINT64 RomContents = 0;

	UINT16 i = 1;

	UINT8 bit_test = 0;

	WriteByte(portnum, COMMAND_READ_ROM);

	GLOBAL_LOCK(irq);

	for(;i < 8	; i++)
	{
		// read a bit and its compliment
		//bit_test = WriteBit(portnum,1) << 1;
		//bit_test |= WriteBit(portnum,1);
		CPU_GPIO_EnableOutputPin(portnum, false);
		HAL_Time_Sleep_MicroSeconds(5);

		CPU_GPIO_EnableInputPin( portnum, false, NULL, GPIO_INT_EDGE_HIGH, RESISTOR_PULLUP );
		HAL_Time_Sleep_MicroSeconds(5);
		bit_test = CPU_GPIO_GetPinState(portnum);
		HAL_Time_Sleep_MicroSeconds(65);

		CPU_GPIO_EnableOutputPin(portnum, true);

		RomContents |= (bit_test << (i));

	}

	hal_printf("The serial number is %ld", RomContents);

	return TRUE;
}


DeviceStatus DS18S20::Initialize(UINT16 portNum)
{
	UINT64 serialNum = 0;
	// Check if someone is there at the other end, if not return false
	if(Reset(portNum) != DS_Success)
		return DS_Fail;

	if(!ReadROM(portNum))
		return DS_Fail;


}
