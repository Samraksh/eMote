/*
 *
 * 	File Name   :		RF231.h
 *
 *  Author Name :   	Nived.Sivadas@samraksh.com
 *
 *  Description :    	Contains the core class implementation of the rf231 radio derived from the radio base class
 *
 *
 *
 *
 */

#ifndef _RF231_H_
#define _RF231_H_

/* === INCLUDES ============================================================ */
#include <Tinyhal_types.h>
#include <tinyhal.h>
#include "radio_error.h"
#include "RF231RegDef.h"
#include <..\pal\COM\Radio\MAC\Base\message.h>
#include "..\radio.h"
#include <..\pal\COM\Radio\MAC\CSMAMAC\csmaMAC.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////

#define RF231RADIO 1

//Interrupts
void radio_irq_init(void (*irq_handler)());

//For C linkage
extern "C" {
void radio_irq_handler();
void Radio_Handler(GPIO_PIN Pin, BOOL PinState, void* Param);
void (*irq_handler)();
}

template<class T>
class RF231Radio : public Radio<T>
{
	// Constant node id defined by macro NODE_ID
	static const UINT32 kNodeId 	=	 NODE_ID;

	// Pin Definitions for the state control pins declared by macros
	static const GPIO_PIN kslpTr 	= 	 SLP_TR_PIN;
	static const GPIO_PIN krstn 	= 	 RSTN_PIN;
	static const GPIO_PIN kseln		= 	 SELN_PIN;

	static const GPIO_PIN kinterrupt	= INTERRUPT_PIN;


	// Stores the configuration of the spi
	SPI_CONFIGURATION config;

	// Variables to control the software state machine
	RadioCommandType cmd;
	RadioStateType state;

	// Control transmission power and channel of transmission
	UINT8 tx_power;
	UINT8 channel;

	// Time when the last interrupt was captured
	// TODO : Explain why this is needed
	UINT16 captured_time;

	// Record rssi values
	UINT8 rssi_clear;
	UINT8 rssi_busy;

	// Contains the length of the message that will be transmitted
	UINT8 length;
	// Pointer to the data
	UINT8 data[30];

	// Receive length and data, could have used the same as above, but trying to avoid unnecessary confusion
	UINT8 rx_length;
	UINT8* rx_data;

	UINT16 active_mac_index;

	// Pointer to the outgoing message
	T* tx_msg_ptr;

	// Pointer to the incoming message
	T* rx_msg_ptr;

	UINT8 tx_length;

	// Recieve Message Buffer
	UINT8 rx_msg[IEEE802_15_4_FRAME_LENGTH];

	// Send message buffer
	UINT8 tx_msg[IEEE802_15_4_FRAME_LENGTH];

	// Initialize the rstn, seln and slptr pins
	void GpioPinInitialize();

	// Initialize the spi module
    BOOL SpiInitialize();

    // All the below functions can be made inline, but this may increase the executable size, so i am deciding against this now
    // Am assuming the speed is not significant enough to warrant the change to inline
    // This remains to be profiled
    void SelnClear()
    {
    	CPU_GPIO_SetPinState(kseln, FALSE);
    }

    void SelnSet()
    {
    	CPU_GPIO_SetPinState(kseln, TRUE);
    }

    void RstnClear()
    {
    	CPU_GPIO_SetPinState(krstn, FALSE);
    }

    void RstnSet()
    {
    	CPU_GPIO_SetPinState(krstn, TRUE);
    }

    void SlptrClear()
    {
    	CPU_GPIO_SetPinState(kslpTr, FALSE);
    }

    void SlptrSet()
    {
    	CPU_GPIO_SetPinState(kslpTr, TRUE);
    }



public:
    UINT8* GetRxMsgBuffer()
    {
    	return rx_msg;
    }

    DeviceStatus ChangeState();
    // Indicates whether the message has been loaded into the frame buffer or not
    BOOL message_status;

    // This function has been moved from the cpp file because the linker is unable to call the function
    // otherwise, something to do with instantiation of the template
    // Calls the gpio initialize and spi initialize modules and asserts if the spi initialization failed
    // Once SPI initialization is complete, radio initialization including setting radio state, channel, tx power etc carried out here
    // The radio is sleeping at the end of initialization
    DeviceStatus Initialize(RadioEventHandler *event_handler, UINT8 mac_id);

    DeviceStatus UnInitialize()
    {
    	return DS_Fail;
    }

    void* Send(void* msg, UINT16 size);

    T* Preload(T* msg, UINT16 size);

    DeviceStatus Send();

    DeviceStatus ClearChannelAssesment();
    DeviceStatus ClearChannelAssesment(UINT32 numberMicroSecond);
    DeviceStatus SetTimeStamp();
    DeviceStatus SetTimeStamp(UINT64 timeStamp);
    INT32 GetSNR();
    INT32 GetRSSI();

    UINT8 GetRadioID();

	void WriteRegister(UINT8 reg, UINT8 value);



	UINT8 ReadRegister(UINT8 reg);
	// returns the current channel, this is initially set to 11 by default
	UINT8 GetChannel()
	{
		return channel;
	}

	// May have to toggle slp_tr, check this during testing of this interface
	void setChannel(UINT8 channel)
	{
		WriteRegister(RF230_PHY_CC_CCA, RF230_CCA_MODE_VALUE | channel);
	}

	UINT8 GetTxPower()
	{
		return tx_power;
	}


	RadioCommandType GetCommand()
	{
		return cmd;
	}

	RadioStateType GetState()
	{
		return state;
	}

	void SetCommand(RadioCommandType cmd)
	{
		this->cmd = cmd;
	}

	void SetState(RadioStateType state)
	{
		this->state = state;
	}



	DeviceStatus PreLoadFrame();

	DeviceStatus TurnOn();

	DeviceStatus TurnOff();

	DeviceStatus StandBy();

	DeviceStatus DownloadMessage();

	void HandleInterrupt();
};

template<class T>
void* RF231Radio<T>::Send(void* msg, UINT16 size)
{

	INIT_STATE_CHECK();

	GLOBAL_LOCK(irq);

	//__ASM volatile("cpsid i");
	UINT32 channel = 0;
	UINT32 reg = 0;
	UINT32 read = 0;

	tx_length = size;

	if(cmd != CMD_NONE || state == STATE_BUSY_TX)
		return msg;


	// Send gives the CMD_TRANSMIT to the radio
	//cmd = CMD_TRANSMIT;
#if 0
	if(state != STATE_PLL_ON)
		return DS_Busy;
#endif
	// Push radio to pll on state
	if(((ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK)== RF230_RX_ON) || ((ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK) == RF230_TRX_OFF))
	{
		WriteRegister(RF230_TRX_STATE, RF230_PLL_ON);

		// Wait for radio to go into pll on and return efail if the radio fails to transition
		DID_STATE_CHANGE_ASSERT(RF230_PLL_ON);

		//WriteRegister(RF230_TRX_STATE, RF230_RX_ON);

		//DID_STATE_CHANGE(RF230_RX_ON);
	}

	UINT8* ldata =(UINT8*) msg;

	//radio_frame_write(data, 119);
	reg = ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK;

	SlptrSet();
	for(UINT8 i =0; i < 10; i++);
	SlptrClear();

	// Load buffer before initiating the transmit command
	SelnClear();


	CPU_SPI_WriteByte(config, RF230_CMD_FRAME_WRITE);

	// Including FCS which is automatically generated and is two bytes
	CPU_SPI_ReadWriteByte(config, size);


	UINT8 lLength = size;



	do
	{
		CPU_SPI_ReadWriteByte(config, *(ldata++));
	}while(--lLength != 0);

	CPU_SPI_ReadByte(config);

	SelnSet();

	//WriteRegister(RF230_TRX_STATE, RF230_TX_START);

	reg = ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK;

	state = STATE_BUSY_TX;

	// exchange bags
	T* temp = tx_msg_ptr;
	tx_msg_ptr = (T*) msg;
	cmd = CMD_TRANSMIT;
	//__ASM volatile("cpsie i");
	return temp;

}




template<class T>
DeviceStatus RF231Radio<T>::Initialize(RadioEventHandler *event_handler, UINT8 mac_id)
{
	// Set active mac index
	active_mac_index = Radio<T>::GetMacIdIndex();
	if(Radio<T>::Initialize(event_handler, mac_id) != DS_Success)
		return DS_Fail;

	// Initially point to the driver buffer
	tx_msg_ptr = (T *) tx_msg;

	// Initially point to driver buffer
	rx_msg_ptr = (T *) rx_msg;

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

	cmd = CMD_NONE;

}

template<class T>
void RF231Radio<T>::WriteRegister(UINT8 reg, UINT8 value)
{
	GLOBAL_LOCK(irq);

	SelnClear();

	CPU_SPI_WriteByte(config, RF230_CMD_REGISTER_WRITE | reg);
	CPU_SPI_ReadWriteByte(config, value);
	CPU_SPI_ReadByte(config);

	SelnSet();

}

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
	CPU_GPIO_EnableOutputPin((GPIO_PIN)9, TRUE);

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

template<class T>
UINT8 RF231Radio<T>::ReadRegister(UINT8 reg)
{
	GLOBAL_LOCK(irq);
	UINT8 read_reg;

	SelnClear();

	CPU_SPI_WriteByte(config, RF230_CMD_REGISTER_READ | reg);
	CPU_SPI_ReadWriteByte(config, 0);
	read_reg = CPU_SPI_ReadByte(config);


	SelnSet();

	return read_reg;

}

//	Responsible for clear channel assessment
//  Takes numberMicroSecond as parameter allowing the user to specify the time to watch the channels
//	Returns DS_Success if the channel is free, DS_Busy is not and DS_Fail is the assessment failed
//  While this function takes an argument for time, it should be noted that the rf231 radio does not
//  have the ability to increase its measurement time to more than 8 symbols in the RX_ON state. Hence in the
//  RX_ON state the result is available at the end of 140 us. However, it may happen that the radio is in BUSY_RX state
//  in which case we have to wait for a while before the cca can be done, this function can be used in those cases.

template<class T>
DeviceStatus RF231Radio<T>::ClearChannelAssesment(UINT32 numberMicroSecond)
{
	UINT8 trx_status;


	GLOBAL_LOCK(irq);

	if(state != STATE_RX_ON)
	{
		UINT8 reg = ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK;
		return DS_Fail;
	}

	//UINT8 reg = ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK;

	// The radio takes a minimum of 140 us to do cca, any numbers less than this are not acceptable
	if(numberMicroSecond < 140)
		return DS_Fail;

	// Make a cca request
	WriteRegister(RF230_PHY_CC_CCA, RF230_CCA_REQUEST | RF230_CCA_MODE_VALUE | channel);

	// Busy wait for the duration of numberMicrosecond
	HAL_Time_Sleep_MicroSeconds(numberMicroSecond);

	// Read the register to check the result of the assessment
	trx_status = ReadRegister(RF230_TRX_STATUS);

	// return the result of the assessment
	return ((trx_status & RF230_CCA_DONE) ? ((trx_status & RF230_CCA_STATUS) ? DS_Success : DS_Busy) : DS_Fail );

}

//	Responsible for clear channel assessment
//  Default version waits for 140 us
//	Returns DS_Success if the channel is free, DS_Busy is not and DS_Fail is the assessment failed
//

template<class T>
DeviceStatus RF231Radio<T>::ClearChannelAssesment()
{
	UINT8 trx_status;

	GLOBAL_LOCK(irq);

	WriteRegister(RF230_PHY_CC_CCA, RF230_CCA_REQUEST | RF230_CCA_MODE_VALUE | channel);

	// Busy wait for the minimum duration of 140 us
	HAL_Time_Sleep_MicroSeconds(150);

	trx_status = ReadRegister(RF230_TRX_STATE);

	return ((trx_status & RF230_CCA_DONE) ? ((trx_status & RF230_CCA_STATUS) ? DS_Success : DS_Busy) : DS_Fail );
}


template<class T>
void RF231Radio<T>::HandleInterrupt()
{
	UINT8 irq_cause;
	INT16 temp;
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
			//ASSERT_RADIO((state == STATE_RX_ON || state == STATE_PLL_ON_2_RX_ON));

			// Indicates start of reception of a packet
			if(irq_cause == TRX_IRQ_RX_START)
			{
				temp = ReadRegister(RF230_PHY_RSSI) & RF230_RSSI_MASK;

				// Keeps track of average rssi, why ?? may be useful someday :)
				rssi_busy += temp - (rssi_busy >> 2);

				// Add the rssi to the message
				IEEE802_15_4_Metadata_t* metadata = rx_msg_ptr->GetMetaData();
				metadata->SetRssi(temp);
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
				if(state == STATE_BUSY_TX)
				{
					//CPU_GPIO_SetPinState((GPIO_PIN)0, TRUE);
					//CPU_GPIO_SetPinState((GPIO_PIN)0, FALSE);
					CPU_GPIO_SetPinState((GPIO_PIN) 0, FALSE);

					// Call radio send done event handler when the send is complete
					(Radio<T>::GetMacHandler(active_mac_index)->GetSendAckHandler())(tx_msg_ptr, tx_length);

					state = STATE_PLL_ON;

					cmd = CMD_NONE;
					//SlptrClear();
					// Check if radio is in pll state, we shouldn't be here if we are not in this state
					// Hence die if you are not in RF230_PLL_ON
					//DID_STATE_CHANGE_ASSERT(RF230_PLL_ON);
					// Change the state of the radio to RX_ON

					WriteRegister(RF230_TRX_STATE, RF230_RX_ON);

					UINT8 reg = ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK;

					UINT8 i = 0;
					// OPTIMIZATION_POSSIBLE
					// Wait till radio is in rx on state or die here, other wise radio and software go out of sync
					DID_STATE_CHANGE_ASSERT(RF230_RX_ON);

					state = STATE_RX_ON;
				}

		}
		else if(cmd == CMD_RECEIVE)
		{
			CPU_GPIO_SetPinState((GPIO_PIN)9, TRUE);
			CPU_GPIO_SetPinState((GPIO_PIN)9, FALSE);

			DownloadMessage();
			rx_msg_ptr->SetActiveMessageSize(rx_length);
			rx_msg_ptr = (T *) (Radio<T>::GetMacHandler(active_mac_index)->GetRecieveHandler())(rx_msg_ptr, rx_length);
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

	UINT8* temp_rx_msg_ptr = (UINT8 *) rx_msg_ptr;

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
			temp_rx_msg_ptr[counter++] = CPU_SPI_WriteReadByte(config, 0);
		}
		while(--read != 0);

	}
	SelnSet();

	state = STATE_RX_ON;

	cmd = CMD_NONE;

	rx_length = length;



}


RF231Radio<IEEE802_15_4_Message_t> grf231Radio;

extern "C"
{

// Call radio_irq_handler from here
void Radio_Handler(GPIO_PIN Pin, BOOL PinState, void* Param)
{
	grf231Radio.HandleInterrupt();

}
}





#endif /* RADIO_H */














