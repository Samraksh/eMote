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
#include "RF231RegDef.h"
#include <Samraksh\Message.h>
#include <Samraksh\Radio.h>
#include <spi\netmf_spi.h>


/* AnanthAtSamraksh - adding below change to fix receive on LR radio extension board - 2/11/2014 */

//for PA_BRD
#define RF231_REG_TX_CTRL_1		0x04

#define RF231_SPI SPI2
#define RF231_SEL(x) { if(x) GPIO_WriteBit(GPIOF, GPIO_Pin_9, Bit_SET); else GPIO_WriteBit(GPIOF, GPIO_Pin_9, Bit_RESET); }
#define RF231_RST(x) { if(x) GPIO_WriteBit(GPIOF, GPIO_Pin_10, Bit_SET); else GPIO_WriteBit(GPIOF, GPIO_Pin_10, Bit_RESET); }
#define RF231_SLP(x) { if(x) GPIO_WriteBit(GPIOB, GPIO_Pin_11, Bit_SET); else GPIO_WriteBit(GPIOB, GPIO_Pin_11, Bit_RESET); }
#define RF231_AMP(x) { if(x) GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_SET); else GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_RESET); }

/* AnanthAtSamraksh */


//////////////////////////////////////////////////////////////////////////////////////////////////////////

enum RadioID : UINT8
{
	RF231RADIO,
	RF231RADIOLR,
};

//Interrupts
void radio_irq_init(void (*irq_handler)());


//For C linkage
extern "C" {
BOOL GetCPUSerial(UINT8 * ptr, UINT16 num_of_bytes);
void radio_irq_handler();
void Radio_Handler(GPIO_PIN Pin, BOOL PinState, void* Param);
void Radio_Handler_LR(GPIO_PIN Pin,BOOL PinState, void* Param);
void (*irq_handler)();
}

#define RF230_CHANNEL_OFFSET 11

enum Channels
{
	Channel_11,
	channel_12,
	channel_13,
	channel_14,
	channel_15,
	channel_16,
	channel_17,
	channel_18,
	channel_19,
	channel_20,
	channel_21,
	channel_22,
	channel_23,
	channel_24,
	channel_25,
	channel_26,
};

class RF231Radio : public Radio<Message_15_4_t>
{
	// Constant node id defined by macro NODE_ID
	static const UINT32 kNodeId 	=	 NODE_ID;

	// Pin Definitions for the state control pins declared by macros
	GPIO_PIN kslpTr;
	GPIO_PIN krstn;
	GPIO_PIN kseln;

	GPIO_PIN kinterrupt;

	INT32 radioName;

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
	//UINT8 data[30];

	// Receive length and data, could have used the same as above, but trying to avoid unnecessary confusion
	UINT8 rx_length;
	UINT8* rx_data;
	INT64 receive_timestamp;

	UINT16 active_mac_index;

	// Pointer to the outgoing message
	Message_15_4_t* tx_msg_ptr;

	// Pointer to the incoming message
	Message_15_4_t* rx_msg_ptr;

	UINT8 tx_length;

	// Recieve Message Buffer
	UINT8 rx_msg[sizeof(Message_15_4_t)];

	// Send message buffer
	UINT8 tx_msg[sizeof(Message_15_4_t)];

	// Initialize the rstn, seln and slptr pins
	BOOL GpioPinInitialize();

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

    BOOL sleep_pending;

    DeviceStatus ChangeState();
    // Indicates whether the message has been loaded into the frame buffer or not
    BOOL message_status;

    UINT32 GetChannel();

    UINT32 GetTxPower();

    void SetRadioName(INT32 radio)
    {
    	this->radioName = radio;
    }

    INT32 GetRadioName()
    {
    	return this->radioName;
    }

    // Functions added to support the long range radio
    DeviceStatus AntDiversity(BOOL enable);

    DeviceStatus PARXTX(BOOL enable);

    void Amp(BOOL TurnOn);

    // This function has been moved from the cpp file because the linker is unable to call the function
    // otherwise, something to do with instantiation of the template
    // Calls the gpio initialize and spi initialize modules and asserts if the spi initialization failed
    // Once SPI initialization is complete, radio initialization including setting radio state, channel, tx power etc carried out here
    // The radio is sleeping at the end of initialization
    DeviceStatus Initialize(RadioEventHandler *event_handler, UINT8 radio, UINT8 mac_id);

    DeviceStatus UnInitialize()
    {
    	return DS_Fail;
    }

    void* Send(void* msg, UINT16 size);

    void* Send_TimeStamped(void* msg, UINT16 size, UINT32 eventTime);

    Message_15_4_t* Preload(Message_15_4_t* msg, UINT16 size);


    DeviceStatus ClearChannelAssesment();
    DeviceStatus ClearChannelAssesment(UINT32 numberMicroSecond);
    DeviceStatus SetTimeStamp();
    DeviceStatus SetTimeStamp(UINT64 timeStamp);
    INT32 GetSNR();
    INT32 GetRSSI();

    DeviceStatus Sleep(int level);

    UINT8 GetRadioID();

	void WriteRegister(UINT8 reg, UINT8 value);

	BOOL Reset();

	UINT8 ReadRegister(UINT8 reg);


	// May have to toggle slp_tr, check this during testing of this interface
	void setChannel(UINT8 channel)
	{
		WriteRegister(RF230_PHY_CC_CCA, RF230_CCA_MODE_VALUE | channel);
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

	DeviceStatus ChangeTxPower(int power);

	DeviceStatus ChangeChannel(int channel);

	DeviceStatus PreLoadFrame();

	DeviceStatus TurnOn();

	DeviceStatus TurnOff();

	DeviceStatus StandBy();

	DeviceStatus DownloadMessage();

	void HandleInterrupt();
};

RF231Radio grf231Radio;

RF231Radio grf231RadioLR;


#endif /* RADIO_H */














