#include "RF231.h"
#include "RF231RegDef.h"
#include <tinyhal.h>

#define DEBUG_ON


// Function Definitions
//extern "C"
//{
//void Radio_Handler(GPIO_PIN Pin, BOOL PinState, void* Param);
//}

//////////////////////// Global Variables //////////////////////////////////////////////////////////////

RadioEventHandler gDefaultHandler;

///////////////////////////////////////////////////////////////////////////////////////////////////////

#if 0
// Initializes the three pins, SELN, SLPTR and RSTN to the states specified
// Assumes that the these pins are not used by other modules. This should generally be handled by the gpio module
// Returns a void data type
template<class T>
void RF231Radio<T>::GpioPinInitialize()
{

	CPU_GPIO_EnableOutputPin(kseln,TRUE);
	CPU_GPIO_EnableOutputPin(kslpTr,FALSE);
	CPU_GPIO_EnableOutputPin(krstn,TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN)0, TRUE);

}

// Calls the mf spi initialize function and returns true if the intialization was successful
template<class T>
BOOL RF231Radio<T>::SpiInitialize()
{
	// Calling mf spi initialize function
	//if(TRUE != CPU_SPI_Initialize())
	//	return FALSE;

	config.DeviceCS               = 10; //TODO - fix me..
	config.CS_Active              = false;
	config.CS_Setup_uSecs         = 0;
	config.CS_Hold_uSecs          = 0;
	config.MSK_IDLE               = false;
	config.MSK_SampleEdge         = false;
	config.Clock_RateKHz          = 2;
	config.SPI_mod                = 0;
	config.MD_16bits = FALSE;



	CPU_SPI_Enable();

	return TRUE;
}


// Calls the gpio initialize and spi initialize modules and asserts if the spi initialization failed
// Once SPI initialization is complete, radio initialization including setting radio state, channel, tx power etc carried out here
// The radio is sleeping at the end of initialization
template<class T>
DeviceStatus RF231Radio<T>::Initialize(RadioEventHandler *event_handler, UINT8 mac_id)
{
	// Set active mac index
	active_mac_index = Radio<T>::GetMacIdIndex();
	if(Radio<T>::Initialize(event_handler, mac_id) != DS_Success)
		return DS_Fail;


	GLOBAL_LOCK(irq);

	for(UINT8 i = 0; i < 30; i++)
		data[i] = 0;

	GpioPinInitialize();
	//configure_exti();
	if(TRUE != SpiInitialize())
	{
		ASSERT_RADIO("SPI Initialization failed");
	}

	// The radio intialization steps in the following lines are semantically copied from the corresponding tinyos implementation
	// Specified in the datasheet a sleep of 510us
	// The performance of this function is good but at times its found to generate different times. Its possible that there were other
	// events happening on the pin that was used to measure this or there is a possible bug !!!
	HAL_Time_Sleep_MicroSeconds(510);


	// Clear rstn pin
	RstnClear();

	// Clear the slptr pin
	SlptrClear();

	// Sleep for 6us
	HAL_Time_Sleep_MicroSeconds(6);

	RstnSet();

	// The RF230_TRX_CTRL_0 register controls the drive current of the digital output pads and the CLKM clock rate
	// Setting value to 0
	WriteRegister(RF230_TRX_CTRL_0, RF230_TRX_CTRL_0_VALUE);


	// The RF230_TRX_STATE register controls the state transition
	WriteRegister(RF230_TRX_STATE, RF230_TRX_OFF);

	UINT32 reg = 0;

	while(true)
	{
		reg = ReadRegister(RF230_TRX_STATE);
		if(reg == RF230_TRX_OFF)
			break;
	}



	HAL_Time_Sleep_MicroSeconds(510);

	// Register controls the interrupts that are currently enabled
#ifndef RADIO_DEBUG
	WriteRegister(RF230_IRQ_MASK, RF230_IRQ_TRX_UR | RF230_IRQ_PLL_LOCK | RF230_IRQ_TRX_END | RF230_IRQ_RX_START);
#else
	WriteRegister(RF230_IRQ_MASK, RF230_IRQ_BAT_LOW | RF230_IRQ_TRX_UR | RF230_IRQ_PLL_LOCK | RF230_IRQ_TRX_END | RF230_IRQ_RX_START);
#endif

	// The RF230_CCA_THRES sets the ed level for CCA, currently setting threshold to 0xc7
	WriteRegister(RF230_CCA_THRES, RF230_CCA_THRES_VALUE);

	// Controls output power and ramping of the transistor
	//writeRegister(RF230_PHY_TX_PWR, RF230_TX_AUTO_CRC_ON | (RF230_DEF_RFPOWER & RF230_TX_PWR_MASK));
	// Nived.Sivadas - turning off auto crc check
	WriteRegister(RF230_PHY_TX_PWR, 0 | (0 & RF230_TX_PWR_MASK));

	tx_power = 0 & RF230_TX_PWR_MASK;
	channel = RF230_DEF_CHANNEL & RF230_CHANNEL_MASK;

	// Sets the channel number
	WriteRegister(RF230_PHY_CC_CCA, RF230_CCA_MODE_VALUE | channel);

	// Enable the gpio pin as the interrupt point
	CPU_GPIO_EnableInputPin(INTERRUPT_PIN,FALSE, Radio_Handler, GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED);


	SlptrSet();

	// set software state machine state to sleep
	state = STATE_SLEEP;

	// Initialize default radio handlers

	// Added here until the state issues are resolved
	TurnOn();

}


template<class T>
void RF231Radio<T>::WriteRegister(UINT8 reg, UINT8 value)
{

	SelnClear();

	CPU_SPI_WriteByte(config, RF230_CMD_REGISTER_WRITE | reg);
	CPU_SPI_ReadWriteByte(config, value);
	CPU_SPI_ReadByte(config);

	SelnSet();

}
#endif
#if 0
template<class T>
UINT8 RF231Radio<T>::ReadRegister(UINT8 reg)
{
	UINT8 read_reg;

	SelnClear();

	CPU_SPI_WriteByte(config, RF230_CMD_REGISTER_READ | reg);
	CPU_SPI_ReadWriteByte(config, 0);
	read_reg = CPU_SPI_ReadByte(config);


	SelnSet();

	return read_reg;

}

template<class T>
void RF231Radio<T>::HandleInterrupt()
{
	UINT8 irq_cause;
	UINT32 temp;
	INIT_STATE_CHECK();
	UINT32 time;

	irq_cause = ReadRegister(RF230_IRQ_STATUS);

	if(irq_cause & TRX_IRQ_PLL_LOCK)
	{
		//if(gRadioObject.GetCommand() == CMD_TRANSMIT)
		//{
			state = STATE_PLL_ON;
		//}
	}
	if(irq_cause & TRX_IRQ_RX_START)
	{
		// We got an Recieve frame start when the driver is not doing any thing particular
		// let us process this interrupt then
		if(cmd == CMD_NONE)
		{
			// We better be in one of these states, other wise the radio and the software state machine are not in synch and
			// we are in big trouble.
			// All bets are off and we may have to reset at this point
			ASSERT_RADIO(state == STATE_RX_ON || state == STATE_PLL_ON_2_RX_ON);

			// Indicates start of reception of a packet
			if(irq_cause == TRX_IRQ_RX_START)
			{
				temp = ReadRegister(RF230_PHY_RSSI) & RF230_RSSI_MASK;

				// Keeps track of average rssi
				rssi_busy += temp - (rssi_busy >> 2);

				// Add the rssi to the message
				//rx_msg.SetRssi(temp);
			}
			// Not sure why we are clearing at this point, possible scenario is if two interrupts happened at the same
			// time, not sure why that would still warrant a clear of the rssi, maybe the value is not believable ??
			else
			{
				//rx_msg.ClearRssi();
			}
			// Time stamp the packet that just came in
			if(irq_cause == RF230_IRQ_RX_START)
			{
				// We have a 64 bit local clock, do we need 64 bit timestamping, perhaps not
				// Lets stick to 32, the iris implementation uses the timer to measure when the input was
				// captured giving more accurate timestamping, we are going to rely on this less reliable technique
				time = (HAL_Time_CurrentTime() >> 32);

				// Time stamp the packet
		//		rx_msg.SetTimeStamp(time);

			}
			// Again, not sure why we are clearing the timestamp here
			else
			{
				//rx_msg.ClearTimeStamp();
			}

			// Initiate cmd receive
			cmd = CMD_RECEIVE;

		}
	}

	// The contents of the frame buffer went out
	if(irq_cause & TRX_IRQ_TRX_END)
	{
#if 0
		if(cmd == CMD_TRANSMIT)
		{
			CPU_GPIO_SetPinState((GPIO_PIN)0, TRUE);
			CPU_GPIO_SetPinState((GPIO_PIN)0, FALSE);

			// Call radio send done event handler when the send is complete
			rf230EventHandlers.RadioSendDoneEvent();

			cmd = CMD_LISTEN;

			// Move radio to RX_ON and keep it in a state of listening
			ChangeState();

		}
		else if(cmd == CMD_RECEIVE)
		{
			ASSERT_RADIO(STATE_RX_ON == state || STATE_PLL_ON_2_RX_ON == state);

			if(state == STATE_PLL_ON_2_RX_ON)
			{
				ASSERT_RADIO((ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK) == RF230_PLL_ON);

				cmd = CMD_LISTEN;

				ChangeState();

			}
			else
			{
				rssi_clear += (ReadRegister(RF230_PHY_RSSI) & RF230_RSSI_MASK) - (rssi_clear >> 2);
			}

			DownloadMessage();
		}
#endif
		if(cmd == CMD_TRANSMIT)
		{
					CPU_GPIO_SetPinState((GPIO_PIN)0, TRUE);
					CPU_GPIO_SetPinState((GPIO_PIN)0, FALSE);

					// Call radio send done event handler when the send is complete
					(Radio<T>::GetMacHandler(active_mac_index)->GetSendAckHandler())(NULL, 0);

					state = STATE_PLL_ON;
					//cmd = CMD_LISTEN;

					// Move radio to RX_ON and keep it in a state of listening
					//ChangeState();

		}
		else if(cmd == CMD_RECEIVE)
		{
			DownloadMessage();
			(Radio<T>::GetMacHandler(active_mac_index)->GetRecieveHandler())(NULL, 0);
		}


	}

}

template<class T>
DeviceStatus RF231Radio<T>::DownloadMessage()
{
	INIT_STATE_CHECK();
	UINT8 length;
	UINT8 counter = 0;
	SelnClear();

	CPU_SPI_WriteReadByte(config, RF230_CMD_FRAME_READ);

	length = CPU_SPI_WriteReadByte(config, 0);

	if(length >= 3)
	{
		UINT8 read;

		CPU_SPI_WriteReadByte(config, 0);

		//rx_msg.SetLength(length);

		// Returns a pointer to the data buffer
		//data = rx_msg;

		read = length;

		do
		{
			rx_msg[counter++] = CPU_SPI_WriteReadByte(config, 0);
		}
		while(--read != 0);

	}
	SelnSet();

	state = STATE_RX_ON;

	cmd = CMD_NONE;



}
#endif
template<class T>
DeviceStatus RF231Radio<T>::Send()
{
	return DS_Fail;
}

template<class T>
INT32 RF231Radio<T>::GetSNR()
{
	return 0;
}

template<class T>
INT32 RF231Radio<T>::GetRSSI()
{
	return 0;
}

template<class T>
UINT8 RF231Radio<T>::GetRadioID()
{
	return 0;
}


template<class T>
DeviceStatus RF231Radio<T>::SetTimeStamp()
{
	return DS_Fail;
}

template<class T>
DeviceStatus RF231Radio<T>::SetTimeStamp(UINT64 timeStamp)
{
	return DS_Fail;
}


#if 0
template<class T>
DeviceStatus RF231Radio<T>::UnInitialize()
{
	return DS_Success;
}
#endif

template<class T>
T* RF231Radio<T>::Preload(T* msg, UINT16 size)
{
	return NULL;
}

#if 0
template<class T>
T* RF231Radio<T>::Send(T* msg, UINT16 size)
{

	INIT_STATE_CHECK();
	//__ASM volatile("cpsid i");
	UINT32 channel = 0;
	UINT32 reg = 0;
	UINT32 read = 0;

	if(state == STATE_BUSY_TX)
		return NULL;

	// Send gives the CMD_TRANSMIT to the radio
	cmd = CMD_TRANSMIT;
#if 0
	if(state != STATE_PLL_ON)
		return DS_Busy;
#endif
	// Push radio to pll on state
	if((ReadRegister(RF230_TRX_STATE) == RF230_RX_ON) || (ReadRegister(RF230_TRX_STATE) == RF230_TRX_OFF))
	{
		WriteRegister(RF230_TRX_STATE, RF230_PLL_ON);

		// Wait for radio to go into pll on and return efail if the radio fails to transition
		DID_STATE_CHANGE_NULL(RF230_PLL_ON);

		//WriteRegister(RF230_TRX_STATE, RF230_RX_ON);

		//DID_STATE_CHANGE(RF230_RX_ON);
	}

	UINT8* ldata = this->data;

	//radio_frame_write(data, 119);
	reg = ReadRegister(RF230_TRX_STATE);

	SlptrSet();
	for(UINT8 i =0; i < 10; i++);
	SlptrClear();

	// Load buffer before initiating the transmit command
	SelnClear();


	CPU_SPI_WriteByte(config, RF230_CMD_FRAME_WRITE);

	// Including FCS which is automatically generated and is two bytes
	CPU_SPI_ReadWriteByte(config, length);


	UINT8 lLength = length;



	do
	{
		CPU_SPI_ReadWriteByte(config, *(ldata++));
	}while(--lLength != 0);

	CPU_SPI_ReadByte(config);

	SelnSet();

	//WriteRegister(RF230_TRX_STATE, RF230_TX_START);

	reg = ReadRegister(RF230_TRX_STATE);

	state = STATE_BUSY_TX;

	//__ASM volatile("cpsie i");
	return NULL;

}
#endif

#if 0
template<class T>
DeviceStatus RF231Radio<T>::TurnOn()
{
	INIT_STATE_CHECK();
	GLOBAL_LOCK(irq);
#if 0
	if(cmd != CMD_NONE || (state != STATE_SLEEP))
		return EBUSY;
	else if(state = STATE_RX_ON)
		return EALREADY;

	cmd = CMD_TURNON;

	return ChangeState();
#endif
	SlptrClear();

	DID_STATE_CHANGE(RF230_TRX_OFF);

	WriteRegister(RF230_TRX_STATE, RF230_PLL_ON);

	DID_STATE_CHANGE(RF230_PLL_ON);

	WriteRegister(RF230_TRX_STATE, RF230_RX_ON);

	// OPTIMIZATION_POSSIBLE
	// Wait till radio is in rx on state
	DID_STATE_CHANGE(RF230_RX_ON);

	// Change the state to RX_ON
	state = STATE_RX_ON;
}
#endif


#if 0
extern "C"
{

// Call radio_irq_handler from here
void Radio_Handler(GPIO_PIN Pin, BOOL PinState, void* Param)
{
	grf231Radio.HandleInterrupt();
	//radio_irq_handler();
}
}


// This function lets the user decouple the loading of the message into the buffer and the actual sending of the data
// The loading happens in TRX_OFF state. Interestingly the radio is impervious to receiving packets during preloading
// Not sure if there is a way around this however.
DeviceStatus RF231Radio<T>::PreLoadFrame()
{

#ifdef RADIO_PRINT_DEBUG
	hal_printf("Check Point 1");
#endif
	if((state != STATE_TRX_OFF) && (state != STATE_SLEEP))
	{
		return EBUSY;
	}

#ifdef RADIO_PRINT_DEBUG
	hal_printf("Check Point 2");
#endif


	if(state == STATE_SLEEP)
	{
#ifdef RADIO_PRINT_DEBUG
	hal_printf("Check Point 3");
#endif

		// Move the radio to TRX_OFF state because spi interface and frame buffer are available here
		WriteRegister(RF230_TRX_STATE, RF230_TRX_OFF);
		state = STATE_SLEEP_2_TRX_OFF;

		// Busy wait till we go into TRX_OFF, can make this asynchronous and do useful work in the mean time
		// but sticking to this for initial revision
		// OPTIMIZATION_POSSIBLE
		while(ReadRegister(RF230_TRX_STATE) != RF230_TRX_OFF);

		// Move into state trx off
		state = STATE_TRX_OFF;

	}

	if(state != STATE_PLL_ON)
	{
		SlptrClear();

		WriteRegister(RF230_TRX_STATE, RF230_PLL_ON);

		while((ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK) != RF230_PLL_ON);

		//SlptrSet();

	}

#ifdef RADIO_PRINT_DEBUG
	hal_printf("Check Point 4");
#endif



//	data[0] = RF230_CMD_FRAME_WRITE;

//	CPU_SPI_nWrite8_nRead8(config, data, length, NULL, NULL, 0 );

	SlptrSet();

	for(UINT8 i = 0; i < 100; i++);
	//WriteRegister(RF230_TRX_STATE, RF230_TX_START);

	SlptrClear();
	//while(ReadRegister(RF230_TRX_STATE) == RF230_TX_START);
#if 0
	// Tell radio you are about to start writing to the frame buffer
	CPU_SPI_WriteByte(config, RF230_CMD_FRAME_WRITE);

	// Writing length first, always write length of the data first as
	// delay here can corrupt the whole packet
	// Not very critical in preloading scenario as radio has not started transmission yet.
	// This line needs to be tested thoroughly, not sure if this will work
	CPU_SPI_nWrite8_nRead8(config, &length, 1, NULL, 0, 0);

	UINT8 dataCount = length;
#ifdef RADIO_PRINT_DEBUG
	hal_printf("Check Point 5");
#endif


	// Write the rest of the data. Writing a byte at a point at this time, this can be increased to increase speed
	// OPTIMIZATION_POSSIBLE
	// Preloading is complete at this point
	do
	{
		CPU_SPI_nWrite8_nRead8(config, &data[dataCount], 1, NULL, 0, 0);

	}while(--dataCount != 0);
#ifdef RADIO_PRINT_DEBUG
	hal_printf("Check Point 6");
#endif
#endif
	return ESUCCESS;

}

DeviceStatus RF231Radio<T>::LoadByteMessage(UINT8* data, UINT8 length)
{
	UINT8 dataCounter = 0;

	UINT8 header[] = {117,65,136,97,34,0,255,255,102,0,63,239,0,102,0,10};

	for(UINT8 i = 0; i < 16; i++)
		this->data[dataCounter++] = header[i];

	for(UINT8 i = 0; i < length; i++)
		this->data[dataCounter++] = data[i];

	this->data[dataCounter++] = 0;
	this->data[dataCounter++] = 0;

	this->length = 16 + length + 2;
//	this->length = length;
//	this->data = data;
}

DeviceStatus RF231Radio<T>::LoadMessage(MessageType* msg, BOOL force)
{
	// Can not load another image unless user requests forcing, this will overwrite the last message, use with caution
	if((MESSAGE_SENT != message_status) && (FALSE == force))
		return DS_Fail;

//	length = msg->GetLength();
//	data = msg->GetMessageData();
}

// Change the state of the radio depending on the command from the driver layer
DeviceStatus RF231Radio<T>::ChangeState()
{
	INIT_STATE_CHECK();

	// Move from TRX_OFF to RX_ON when the command is CMD_TURNON or if the command is listen
	if((CMD_TURNON == cmd && STATE_TRX_OFF == state) || (CMD_LISTEN == cmd))
	{
		WriteRegister(RF230_TRX_STATE, RF230_RX_ON);

		// OPTIMIZATION_POSSIBLE
		// Wait till radio is in rx on state
		DID_STATE_CHANGE(RF230_RX_ON);

		// Change the state to RX_ON
		state = STATE_RX_ON;
	}
	if(CMD_TURNOFF == cmd)
	{
		// If the radio is in TRX_OFF just pull Slptr high to go into sleep
		if(STATE_TRX_OFF == state)
		{
			SlptrSet();
		}
		// if radio is any of the below states move to TRX_OFF before trying to put radio to sleep
		// Acc to state diagram, SLEEP can only be reached from TRX_OFF
		else if(STATE_PLL_ON == state || STATE_RX_ON == state || STATE_P_ON == state)
		{

			WriteRegister(RF230_TRX_STATE, RF230_TRX_OFF);

			// OPTIMIZATION_POSSIBLE
			DID_STATE_CHANGE(RF230_TRX_OFF);

			// Put radio to sleep
			SlptrSet();
		}
		else
		{
			return EFAIL;
		}

		state = STATE_SLEEP;
	}
	if(CMD_STANDBY == cmd)
	{
		// If the radio is sleeping, clear slptr to wake it up
		if(STATE_SLEEP == state)
			SlptrClear();
		// If radio is either in STATE_RX_ON or STATE_PLL_ON initiate TRX_OFF command
		else if(STATE_RX_ON == state || STATE_PLL_ON == state)
		{
			WriteRegister(RF230_TRX_STATE, RF230_TRX_OFF);

			DID_STATE_CHANGE(RF230_TRX_OFF);

		}
		// If the radio is in any other state, its probably doing some useful work and its state can not be changed
		else
		{
			return EFAIL;
		}
	}

	return ESUCCESS;

}


// This is a synchronous send. Assumes that the data to be sent has been loaded into the driver buffer.
// The function then copies this data to the frame buffer of the radio and initiates send
// The completion of send is indicated by a send done interrupt
// After send is complete the radio is automatically put into rx_on
DeviceStatus RF231Radio<T>::Send()
{

	INIT_STATE_CHECK();
	//__ASM volatile("cpsid i");
	UINT32 channel = 0;
	UINT32 reg = 0;
	UINT32 read = 0;

	if(state == STATE_BUSY_TX)
		return EBUSY;

	// Send gives the CMD_TRANSMIT to the radio
	cmd = CMD_TRANSMIT;
#if 0
	if(state != STATE_PLL_ON)
		return EBUSY;
#endif
	// Push radio to pll on state
	if((ReadRegister(RF230_TRX_STATE) == RF230_RX_ON) || (ReadRegister(RF230_TRX_STATE) == RF230_TRX_OFF))
	{
		WriteRegister(RF230_TRX_STATE, RF230_PLL_ON);

		// Wait for radio to go into pll on and return efail if the radio fails to transition
		DID_STATE_CHANGE(RF230_PLL_ON);

		//WriteRegister(RF230_TRX_STATE, RF230_RX_ON);

		//DID_STATE_CHANGE(RF230_RX_ON);
	}

	UINT8* ldata = this->data;

	//radio_frame_write(data, 119);
	reg = ReadRegister(RF230_TRX_STATE);

	SlptrSet();
	for(UINT8 i =0; i < 10; i++);
	SlptrClear();

	// Load buffer before initiating the transmit command
	SelnClear();


	CPU_SPI_WriteByte(config, RF230_CMD_FRAME_WRITE);

	// Including FCS which is automatically generated and is two bytes
	CPU_SPI_ReadWriteByte(config, length);


	UINT8 lLength = length;



	do
	{
		CPU_SPI_ReadWriteByte(config, *(ldata++));
	}while(--lLength != 0);

	CPU_SPI_ReadByte(config);

	SelnSet();

	//WriteRegister(RF230_TRX_STATE, RF230_TX_START);

	reg = ReadRegister(RF230_TRX_STATE);

	state = STATE_BUSY_TX;

	//__ASM volatile("cpsie i");

}

DeviceStatus RF231Radio<T>::TurnOff()
{
	if(cmd != CMD_NONE)
		return EBUSY;
	if(state == STATE_SLEEP)
		return EALREADY;

	cmd = CMD_TURNOFF;

	return ChangeState();
}

DeviceStatus RF231Radio<T>::TurnOn()
{
	INIT_STATE_CHECK();
	GLOBAL_LOCK(irq);
#if 0
	if(cmd != CMD_NONE || (state != STATE_SLEEP))
		return EBUSY;
	else if(state = STATE_RX_ON)
		return EALREADY;

	cmd = CMD_TURNON;

	return ChangeState();
#endif
	SlptrClear();

	DID_STATE_CHANGE(RF230_TRX_OFF);

	WriteRegister(RF230_TRX_STATE, RF230_PLL_ON);

	DID_STATE_CHANGE(RF230_PLL_ON);

	WriteRegister(RF230_TRX_STATE, RF230_RX_ON);

	// OPTIMIZATION_POSSIBLE
	// Wait till radio is in rx on state
	DID_STATE_CHANGE(RF230_RX_ON);

	// Change the state to RX_ON
	state = STATE_RX_ON;
}

DeviceStatus RF231Radio<T>::StandBy()
{
	if(cmd != CMD_NONE)
		return EBUSY;
	else if(state == STATE_TRX_OFF)
		return EALREADY;

	cmd = CMD_STANDBY;

	return ChangeState();

}
#endif


