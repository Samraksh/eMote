/*
 * 	Copyright The Samraksh Company
 *
 *	Name 		: 	netmf_radio.cpp
 *
 *  Author  	:   nived.sivads@samraksh.com, mukundan.sridharan@samrkash.com
 *
 *	Description :   Contains the interface functions between the radio pal (mac layers defined in Radio_decl.h) and
 *					the aggregation layer
 *
 */

// Called by MAC layer to register handler functions and start the radio
// Takes inputs of a pointer to eventhandler, radioid and numberRadios
// Returns the status of the initalization task whether successful or not

// History 		v0.1 - Added sleep functionality, channel change and power change functionality (nived.sivadas)
//              v0.2 - Added long range radio support (nived.sivadas)
//              v0.3 - Added asserts (MichaelAtSamraksh)
//				v0.4 - Added initial support for Si446x driver (Nathan Stohs)

#include <tinyhal.h>
#include "RF231\RF231.h"
#include "SI4468\si446x.h"

#define ASSERT_NOFAIL(x) {if(x==DS_Fail){ SOFT_BREAKPOINT(); }}

#define SI4468_SPI2_POWER_OFFSET 16
#define SI4468_SPI2_CHANNEL_OFFSET 16

const char * strUfoRadio = "[NATIVE] Error in function %s : Unidentified radio \r\n";
#define PRINTF_UNIDENTIFIED_RADIO()  hal_printf( strUfoRadio , __func__ );

INT8 currentRadioName;

// Calls the corresponding radio object initialize function based on the radio chosen
DeviceStatus CPU_Radio_Initialize(RadioEventHandler* eventHandlers, UINT8 radioID, UINT8 numberRadios, UINT8 mac_id )
{
	DeviceStatus status = DS_Fail;
	currentRadioName = -1;

	if(eventHandlers == NULL)
		return DS_Fail;

	switch(radioID)
	{
		case RF231RADIO:
			currentRadioName = RF231RADIO;
			status = grf231Radio.Initialize(eventHandlers, radioID, mac_id);
			break;
		case RF231RADIOLR:
			currentRadioName = RF231RADIOLR;
			status = grf231RadioLR.Initialize(eventHandlers, radioID, mac_id);
			break;
		case SI4468_SPI2:
			currentRadioName = SI4468_SPI2;
			status = si446x_hal_init(eventHandlers, radioID, mac_id);
			break;
		default:
			PRINTF_UNIDENTIFIED_RADIO();
			break;
	}

	//ASSERT_NOFAIL(status);
	{if(status==DS_Fail) SOFT_BREAKPOINT();}
	return status;
}


BOOL CPU_Radio_Reset(UINT8 radioID)
{
	DeviceStatus status = DS_Fail;

	switch(radioID)
	{
		case RF231RADIO:
			status = grf231Radio.Reset();
			break;
		case RF231RADIOLR:
			status = grf231RadioLR.Reset();
			break;
		case SI4468_SPI2:
			status = si446x_hal_reset(radioID);
			break;
		default:
			PRINTF_UNIDENTIFIED_RADIO();
			break;
	}

	ASSERT_NOFAIL(status);
	return status;
}


BOOL CPU_Radio_UnInitialize(UINT8 id)
{
	BOOL result = FALSE;

	switch(id)
	{
		case RF231RADIO:
			result = (grf231Radio.UnInitialize() == DS_Success);
			break;
		case RF231RADIOLR:
			result = (grf231RadioLR.UnInitialize() == DS_Success);
			break;
		case SI4468_SPI2:
			result = (si446x_hal_uninitialize(id) == DS_Success);
			break;
		default:
			PRINTF_UNIDENTIFIED_RADIO();
			break;

	}

	ASSERT_SP(result == TRUE);
	return result;
}

// This function is no longer supported, it is upto the user to name the radio
UINT8 CPU_Radio_GetRadioIDs(UINT8* radioIDs)
{
	//*radioIDs=grf231Radio.GetRadioID();
	ASSERT_SP(0);
	return 1;
}

UINT16 CPU_Radio_GetAddress(UINT8 radioID)
{
	UINT16 address = 0;

	switch(radioID)
	{
	case RF231RADIO:
		address = grf231Radio.GetAddress();
		break;
	case RF231RADIOLR:
		address = grf231RadioLR.GetAddress();
		break;
	case SI4468_SPI2:
		address = si446x_hal_get_address(radioID);
		break;
	default:
		PRINTF_UNIDENTIFIED_RADIO();
		break;
	}

	ASSERT_SP(address != 0);   // note: 0 is valid address and will pass in Release flavor.
	return address;
}


BOOL CPU_Radio_SetAddress(UINT8 radioID, UINT16 address)
{
	BOOL status = FALSE;

	switch(radioID)
	{
		case RF231RADIO:
			status = grf231Radio.SetAddress(address);
			break;
		case RF231RADIOLR:
			status = grf231RadioLR.SetAddress(address);
			break;
		case SI4468_SPI2:
			status = si446x_hal_set_address(radioID, address);
			break;
		default:
			PRINTF_UNIDENTIFIED_RADIO();
			break;
	}

	ASSERT_SP(status == TRUE);
	return status;
}

INT8 CPU_Radio_GetRadioName()
{
	INT8 radioType = -1;
	switch(currentRadioName)
	{
		case RF231RADIO:
			radioType = grf231Radio.GetRadioName();
			break;
		case RF231RADIOLR:
			radioType = grf231RadioLR.GetRadioName();
			break;
		case SI4468_SPI2:
			radioType = si446x_hal_get_RadioType();
			break;
		default:
			PRINTF_UNIDENTIFIED_RADIO();
			break;
	}

	return radioType;
}

DeviceStatus CPU_Radio_SetRadioName(INT8 radioName)
{
	DeviceStatus status = DS_Fail;
	switch(radioName)
	{
		currentRadioName = radioName;
		case RF231RADIO:
			grf231Radio.SetRadioName(radioName);
			status = DS_Success;
			break;
		case RF231RADIOLR:
			grf231RadioLR.SetRadioName(radioName);
			status = DS_Success;
			break;
		case SI4468_SPI2:
			si446x_hal_set_RadioType(radioName);
			status = DS_Success;
			break;
		default:
			currentRadioName = -1;
			PRINTF_UNIDENTIFIED_RADIO();
			ASSERT_SP(0);
			break;
	}

	return status;
}

DeviceStatus CPU_Radio_ChangeTxPower(UINT8 radioID, int power)
{
	DeviceStatus status = DS_Fail;

	switch(radioID)
	{
		case RF231RADIO:
			status = grf231Radio.ChangeTxPower(power);
			break;
		case RF231RADIOLR:
			status = grf231RadioLR.ChangeTxPower(power);
			break;
		case SI4468_SPI2:
			status = si446x_hal_tx_power(radioID, power);
			break;
		default:
			PRINTF_UNIDENTIFIED_RADIO();
			break;
	}

	ASSERT_NOFAIL(status);
	return status;
}

UINT32 CPU_Radio_GetTxPower(UINT8 radioID)
{
	UINT32 txPower = 0;

	switch(radioID)
	{
		case RF231RADIO:
			txPower = grf231Radio.GetTxPower();
			break;
		case RF231RADIOLR:
			txPower = grf231RadioLR.GetTxPower();
			break;
		case SI4468_SPI2:
			txPower = si446x_hal_get_power(radioID);
			break; 
		default:
			PRINTF_UNIDENTIFIED_RADIO();
			break;
	}
	return txPower;
}

DeviceStatus CPU_Radio_ChangeChannel(UINT8 radioID, int channel)
{
	DeviceStatus status = DS_Fail;

	switch(radioID)
	{
		case RF231RADIO:
			status = grf231Radio.ChangeChannel(channel);
			break;
		case RF231RADIOLR:
			status = grf231RadioLR.ChangeChannel(channel);
			break;
		case SI4468_SPI2:
			status = si446x_hal_set_channel(radioID, channel);
			break;
		default:
			PRINTF_UNIDENTIFIED_RADIO();
			break;
	}

	ASSERT_NOFAIL(status);
	return status;
}

UINT32 CPU_Radio_GetChannel(UINT8 radioID)
{
	UINT32 channel = 0;

	switch(radioID)
	{
		case RF231RADIO:
			channel = grf231Radio.GetChannel();
			break;
		case RF231RADIOLR:
			channel = grf231RadioLR.GetChannel();
			break;
		case SI4468_SPI2:
			channel = si446x_hal_get_chan(radioID);
			break;
		default:
			PRINTF_UNIDENTIFIED_RADIO();
			break;
	}

	return channel;
}

// Function is not currently supported
void* CPU_Radio_Preload(UINT8 radioID, void * msg, UINT16 size)
{
	ASSERT_SP(0);
	return NULL;
}

void* CPU_Radio_SendRetry(UINT8 radioID)
{
	void* ptr_temp = NULL;

	switch(radioID)
	{
		case RF231RADIO:
			ptr_temp = (void *) grf231Radio.SendRetry();
			break;
		case RF231RADIOLR:
			ptr_temp = (void *) grf231RadioLR.SendRetry();
			break;
		default:
			PRINTF_UNIDENTIFIED_RADIO();
			break;
	}

	//ASSERT_SP(ptr_temp != NULL);
	return ptr_temp;
}

void* CPU_Radio_SendStrobe(UINT8 radioID, UINT16 size)
{
	void* ptr_temp = NULL;

	switch(radioID)
	{
		case RF231RADIO:
			ptr_temp = (void *) grf231Radio.SendStrobe(size);
			break;
		case RF231RADIOLR:
			ptr_temp = (void *) grf231RadioLR.SendStrobe(size);
			break;
		default:
			PRINTF_UNIDENTIFIED_RADIO();
			break;
	}

	//ASSERT_SP(ptr_temp != NULL);
	return ptr_temp;
}

void* CPU_Radio_Send(UINT8 radioID, void* msg, UINT16 size)
{
	void* ptr_temp = NULL;

	switch(radioID)
	{
		case RF231RADIO:
			ptr_temp = (void *) grf231Radio.Send(msg, size);
			break;
		case RF231RADIOLR:
			ptr_temp = (void *) grf231RadioLR.Send(msg, size);
			break;
		case SI4468_SPI2:
			ptr_temp = si446x_hal_send(radioID, msg, size);
			break;
		default:
			PRINTF_UNIDENTIFIED_RADIO();
			break;
	}

	//ASSERT_SP(ptr_temp != NULL);
	return ptr_temp;
}

// This function is used to send a time stamped packet, time stamping is done just before
// physical send in hardware
void* CPU_Radio_Send_TimeStamped(UINT8 radioID, void* msg, UINT16 size, UINT32 eventTime)
{
	void* ptr_temp = NULL;

	switch(radioID)
	{
		case RF231RADIO:
			ptr_temp = (void *) grf231Radio.Send_TimeStamped(msg, size, eventTime);
			break;
		case RF231RADIOLR:
			ptr_temp = (void *) grf231RadioLR.Send_TimeStamped(msg, size, eventTime);
			break;
		case SI4468_SPI2:
			ptr_temp = si446x_hal_send_ts(radioID, msg, size, eventTime);
			break;
		default:
			PRINTF_UNIDENTIFIED_RADIO();
			break;
	}

	//ASSERT_SP(ptr_temp != NULL);
	return ptr_temp;
}

NetOpStatus CPU_Radio_PreloadMessage(UINT8* msgBuffer, UINT16 size)
{
	NetOpStatus status = NetworkOperations_Success;
	Message_15_4_t* retMsg = grf231Radio.Preload((Message_15_4_t*)msgBuffer, size);
	if(retMsg == NULL){
		status = NetworkOperations_Busy;
	}
	return status;
}

/*This is not needed anymore. These were added on a trial-and-error basis.
 * Just to check if it is possible to enable CSMA for 2nd attempt when initial
 * attempt at sending a packet fails.
DeviceStatus CPU_Radio_EnableCSMA(UINT8 radioID)
{
	DeviceStatus status = DS_Fail;

	switch(radioID)
	{
		case RF231RADIO:
			status = grf231Radio.EnableCSMA();
			break;
		case RF231RADIOLR:
			status = grf231RadioLR.EnableCSMA();
			break;
		case SI4468_SPI2:
			status = si446x_hal_rx(radioID);
			break;
		default:
			PRINTF_UNIDENTIFIED_RADIO();
			break;

	}

	//ASSERT_NOFAIL(status);
	return status;
}

DeviceStatus CPU_Radio_DisableCSMA(UINT8 radioID)
{
	DeviceStatus status = DS_Fail;

	switch(radioID)
	{
		case RF231RADIO:
			status = grf231Radio.DisableCSMA();
			break;
		case RF231RADIOLR:
			status = grf231RadioLR.DisableCSMA();
			break;
		case SI4468_SPI2:
			status = si446x_hal_sleep(radioID);
			break;
		default:
			PRINTF_UNIDENTIFIED_RADIO();
			break;

	}

	//ASSERT_NOFAIL(status);
	return status;
}*/


// This function calls the corresponding radio turn on function based on the input radio id
DeviceStatus CPU_Radio_TurnOnRx(UINT8 radioID)
{
	DeviceStatus status = DS_Fail;

	switch(radioID)
	{
		case RF231RADIO:
			status = grf231Radio.TurnOnRx();
			break;
		case RF231RADIOLR:
			status = grf231RadioLR.TurnOnRx();
			break;
		case SI4468_SPI2:
			status = si446x_hal_rx(radioID);
			break;
		default:
			PRINTF_UNIDENTIFIED_RADIO();
			break;
	}

	//ASSERT_NOFAIL(status);
	return status;
}

// This function calls the corresponding radio turn off function based on the input radio id
DeviceStatus CPU_Radio_TurnOffRx(UINT8 radioID)
{
	DeviceStatus status = DS_Fail;

	switch(radioID)
	{
		case RF231RADIO:
			status = grf231Radio.TurnOffRx();
			break;
		case RF231RADIOLR:
			status = grf231RadioLR.TurnOffRx();
			break;
		case SI4468_SPI2:
			status = si446x_hal_sleep(radioID);
			break;
		default:
			PRINTF_UNIDENTIFIED_RADIO();
			break;
	}

	//ASSERT_NOFAIL(status);
	return status;
}


// This function calls the corresponding radio turn on function based on the input radio id
DeviceStatus CPU_Radio_TurnOnPLL(UINT8 radioID)
{
	return DS_Success;
	/* Should not be user exposed. The driver handles this. --NPS
	DeviceStatus status = DS_Fail;

	switch(radioID)
	{
		case RF231RADIO:
			status = grf231Radio.TurnOnPLL();
			break;
		case RF231RADIOLR:
			status = grf231RadioLR.TurnOnPLL();
			break;
		case SI4468_SPI2:
			status = si446x_hal_cca(radioID);
			break;
		default:
			PRINTF_UNIDENTIFIED_RADIO();
			break;

	}

	ASSERT_NOFAIL(status);
	return status;
	*/
}


DeviceStatus CPU_Radio_Sleep(UINT8 radioID, UINT8 level)
{
	DeviceStatus status = DS_Fail;

	switch(radioID)
	{
		case RF231RADIO:
			status = grf231Radio.Sleep(level);  //TODO: translate level from device-agnostic to device-specific level if needed... or change the device driver to support the CPU_Radio Level enum
			break;
		case RF231RADIOLR:
			status = grf231RadioLR.Sleep(level);
			break;
		case SI4468_SPI2:
			status = si446x_hal_sleep(radioID);
			break;
		default:
			PRINTF_UNIDENTIFIED_RADIO();
			break;
	}

	ASSERT_NOFAIL(status);
	return status;

}

DeviceStatus CPU_Radio_ClearChannelAssesment (UINT8 radioID)
{
	DeviceStatus status  = DS_Fail;

	switch(radioID)
	{
		case RF231RADIO:
			status = grf231Radio.ClearChannelAssesment();
			break;
		case RF231RADIOLR:
			status = grf231RadioLR.ClearChannelAssesment();
			break;
		case SI4468_SPI2:
			//status = si446x_hal_cca(radioID);
			status = si446x_hal_cca_ms(radioID, 200);
			break;
		default:
			PRINTF_UNIDENTIFIED_RADIO();
			break;
	}

	//DS_Fail is a valid state returned from ClearChannelAssesment
	//ASSERT_NOFAIL(status);
	return status;
}


DeviceStatus CPU_Radio_ClearChannelAssesment(UINT8 radioID, UINT32 numberMicroSecond)
{
	DeviceStatus status  = DS_Fail;

	switch(radioID)
	{
		case RF231RADIO:
			status = grf231Radio.ClearChannelAssesment(numberMicroSecond);
			break;
		case RF231RADIOLR:
			status = grf231RadioLR.ClearChannelAssesment(numberMicroSecond);
			break;
		case SI4468_SPI2:
			status = si446x_hal_cca_ms(radioID, numberMicroSecond);
			break;
		default:
			PRINTF_UNIDENTIFIED_RADIO();
			break;
	}

	//DS_Fail is a valid state returned from ClearChannelAssesment
	//ASSERT_NOFAIL(status);
	return status;

}

BOOL CPU_Radio_SetTimeStamp(UINT8 radioID)
{
	return FALSE;
}

BOOL CPU_Radio_SetTimeStamp(UINT64 timeStamp)
{
	return FALSE;
}

UINT32 CPU_Radio_GetSNR(UINT8 radioID)
{
	return 0;
}

// Gets the RSSI. Does not use the ED module of the RF231 currently.
// Must be called when in RX mode. Caller must do this.
// Return MAX_INT for total fail.
// Returns 0x7FFFFFFF if not in RX mode.
UINT32 CPU_Radio_GetRSSI(UINT8 radioID)
{
	UINT32 val;

	switch(radioID)
	{
		case RF231RADIO:
			val = grf231Radio.GetRSSI();
			break;
		case RF231RADIOLR:
			val = grf231RadioLR.GetRSSI();
			break;
		case SI4468_SPI2:
			val = si446x_hal_get_rssi(radioID);
			break;
		default:
			val=0xFFFFFFFF; // error condition.
			PRINTF_UNIDENTIFIED_RADIO();
			break;
	}
	return val;
}


//Aggregate apis
BOOL CPU_RadioLayer_SwitchType(UINT8 radioID)
{
	return FALSE;
}

#if 0
Radio* CPU_RadioLayer_GetRadio(UINT8 radioID)
{
}
#endif

// At this point we are only supporting the RF231 radio, if this situation changes in future we can have
// a more sophisticated way of determining the number of radios supported, for now this is good
UINT8 CPU_RadioLayer_NumberRadiosSupported()
{
	return 2;
}
