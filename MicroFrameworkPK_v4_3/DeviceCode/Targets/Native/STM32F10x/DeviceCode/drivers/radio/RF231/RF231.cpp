#include "RF231.h"
#include "RF231RegDef.h"
#include <tinyhal.h>

#define DEBUG_RF231 1

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

void* RF231Radio::Send_TimeStamped(void* msg, UINT16 size, UINT32 eventTime)
{
	if(size+2 > IEEE802_15_4_FRAME_LENGTH){
		hal_printf("Radio Send Error: Big packet: %d ", size+2);
		return msg;
	}
	ASSERT(size+1 < IEEE802_15_4_FRAME_LENGTH);

	INIT_STATE_CHECK();
	GLOBAL_LOCK(irq);
	//__ASM volatile("cpsid i");
	//pulse 1
#ifdef DEBUG_RF231
	CPU_GPIO_SetPinState((GPIO_PIN)0, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN)0, FALSE);
#endif

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
	//pulse 2
#ifdef DEBUG_RF231
	CPU_GPIO_SetPinState((GPIO_PIN)0, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN)0, FALSE);
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

	UINT32 timestamp = HAL_Time_CurrentTicks() & (~(UINT32) 0);

	UINT32 timeOffset = timestamp - eventTime;

	//pulse 3
#ifdef DEBUG_RF231
	CPU_GPIO_SetPinState((GPIO_PIN)0, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN)0, FALSE);
#endif

	SlptrSet();
	for(UINT8 i =0; i < 10; i++);
	SlptrClear();

	// Load buffer before initiating the transmit command
	SelnClear();

	CPU_SPI_WriteByte(config, RF230_CMD_FRAME_WRITE);

	// Including FCS which is automatically generated and is two bytes
	CPU_SPI_ReadWriteByte(config, size);

	UINT8 lLength = size + sizeof(eventTime);

	do
	{
		CPU_SPI_ReadWriteByte(config, *(ldata++));
	}while(--lLength != 0);

	lLength = sizeof(eventTime);

	do
	{
		CPU_SPI_ReadWriteByte(config, *(ldata++));
	}while(--lLength != 0);

	CPU_SPI_ReadByte(config);

	//pulse 4
#ifdef DEBUG_RF231
	CPU_GPIO_SetPinState((GPIO_PIN)0, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN)0, FALSE);
#endif

	SelnSet();

	//WriteRegister(RF230_TRX_STATE, RF230_TX_START);

	reg = ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK;

	state = STATE_BUSY_TX;

	// exchange bags
	Message_15_4_t* temp = tx_msg_ptr;
	tx_msg_ptr = (Message_15_4_t*) msg;
	cmd = CMD_TRANSMIT;

	//pulse 5
#ifdef DEBUG_RF231
	CPU_GPIO_SetPinState((GPIO_PIN)0, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN)0, FALSE);
#endif
	//__ASM volatile("cpsie i");
	return temp;

}

BOOL RF231Radio::Reset()
{
	INIT_STATE_CHECK()

	GLOBAL_LOCK(irq);

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

	UINT32 reg = 0;
	reg = ReadRegister(RF230_TRX_CTRL_0);

			// The RF230_TRX_STATE register controls the state transition
	WriteRegister(RF230_TRX_STATE, RF230_TRX_OFF);

	// Nived.Sivadas - Hanging in the initialize function caused by the radio being in an unstable state
	// This fix will return false from initialize and enable the user of the function to exit gracefully
	// Fix for the hanging in the initialize function
	#if 0
			while(true)
			{
				//reg = ReadRegister(RF230_TRX_STATE);
				reg = (ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK);
				if(reg == RF230_TRX_OFF)
					break;
			}
	#endif
	DID_STATE_CHANGE(RF230_TRX_OFF);

	HAL_Time_Sleep_MicroSeconds(510);

			// Register controls the interrupts that are currently enabled
	#ifndef RADIO_DEBUG
			//WriteRegister(RF230_IRQ_MASK, RF230_IRQ_TRX_UR | RF230_IRQ_PLL_LOCK | RF230_IRQ_TRX_END | RF230_IRQ_RX_START);
		WriteRegister(RF230_IRQ_MASK, RF230_IRQ_TRX_UR | RF230_IRQ_TRX_END | RF230_IRQ_RX_START);
	#else
		//WriteRegister(RF230_IRQ_MASK, RF230_IRQ_BAT_LOW | RF230_IRQ_TRX_UR | RF230_IRQ_PLL_LOCK | RF230_IRQ_TRX_END | RF230_IRQ_RX_START);
		WriteRegister(RF230_IRQ_MASK, RF230_IRQ_BAT_LOW | RF230_IRQ_TRX_UR | RF230_IRQ_TRX_END | RF230_IRQ_RX_START);
	#endif

			// The RF230_CCA_THRES sets the ed level for CCA, currently setting threshold to 0xc7
	WriteRegister(RF230_CCA_THRES, RF230_CCA_THRES_VALUE);

			// Controls output power and ramping of the transistor
	WriteRegister(RF230_PHY_TX_PWR, RF230_TX_AUTO_CRC_ON | (0 & RF230_TX_PWR_MASK));
			// Nived.Sivadas - turning off auto crc check
			//WriteRegister(RF230_PHY_TX_PWR, 0 | (0 & RF230_TX_PWR_MASK));
	tx_power = 0 & RF230_TX_PWR_MASK;
	channel = RF230_DEF_CHANNEL & RF230_CHANNEL_MASK;

			// Sets the channel number
	WriteRegister(RF230_PHY_CC_CCA, RF230_CCA_MODE_VALUE | channel);
	#ifdef DEBUG_RF231
			CPU_GPIO_SetPinState((GPIO_PIN)0, TRUE);
			CPU_GPIO_SetPinState((GPIO_PIN)0, FALSE);
	#endif
			// Enable the gpio pin as the interrupt point
	CPU_GPIO_EnableInputPin(INTERRUPT_PIN,FALSE, Radio_Handler, GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED);


	SlptrSet();
	#ifdef DEBUG_RF231
			CPU_GPIO_SetPinState((GPIO_PIN)0, TRUE);
			CPU_GPIO_SetPinState((GPIO_PIN)0, FALSE);
	#endif
			// set software state machine state to sleep
	state = STATE_SLEEP;

			// Initialize default radio handlers

			// Added here until the state issues are resolved
	TurnOn();

	cmd = CMD_NONE;
	#ifdef DEBUG_RF231
			CPU_GPIO_SetPinState((GPIO_PIN)0, TRUE);
			CPU_GPIO_SetPinState((GPIO_PIN)0, FALSE);
	#endif
	return TRUE;
}

void* RF231Radio::Send(void* msg, UINT16 size)
{
	if(size+2 > IEEE802_15_4_FRAME_LENGTH){
		hal_printf("Radio Send Error: Big packet: %d ", size+2);
		return msg;
	}
	ASSERT(size+1 < IEEE802_15_4_FRAME_LENGTH);

	INIT_STATE_CHECK();

	GLOBAL_LOCK(irq);
	//__ASM volatile("cpsid i");
	//pulse 1
#ifdef DEBUG_RF231
	CPU_GPIO_SetPinState((GPIO_PIN)0, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN)0, FALSE);
#endif

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
	//pulse 2
#ifdef DEBUG_RF231
	CPU_GPIO_SetPinState((GPIO_PIN)0, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN)0, FALSE);
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
	//pulse 3
#ifdef DEBUG_RF231
	CPU_GPIO_SetPinState((GPIO_PIN)0, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN)0, FALSE);
#endif

	//SlptrSet();
	//for(UINT8 i =0; i < 10; i++);
	//SlptrClear();

	//pulse 4
#ifdef DEBUG_RF231
	//CPU_GPIO_SetPinState((GPIO_PIN)0, TRUE);
	//CPU_GPIO_SetPinState((GPIO_PIN)0, FALSE);
#endif
	// Load buffer before initiating the transmit command
	SelnClear();

	CPU_SPI_WriteByte(config, RF230_CMD_FRAME_WRITE);

	// Including FCS which is automatically generated and is two bytes
	CPU_SPI_ReadWriteByte(config, size + 2);

	UINT8 lLength = size;

	do
	{
		CPU_SPI_ReadWriteByte(config, *(ldata++));
	}while(--lLength != 0);

	CPU_SPI_ReadByte(config);

	//pulse 5
#ifdef DEBUG_RF231
	CPU_GPIO_SetPinState((GPIO_PIN)0, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN)0, FALSE);
#endif

	SelnSet();

	WriteRegister(RF230_TRX_STATE, RF230_TX_START);

	reg = ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK;

	state = STATE_BUSY_TX;

	// exchange bags
	Message_15_4_t* temp = tx_msg_ptr;
	tx_msg_ptr = (Message_15_4_t*) msg;
	cmd = CMD_TRANSMIT;

	//pulse 6
#ifdef DEBUG_RF231
	CPU_GPIO_SetPinState((GPIO_PIN)0, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN)0, FALSE);
#endif
	//__ASM volatile("cpsie i");
	return temp;
}


DeviceStatus RF231Radio::Initialize(RadioEventHandler *event_handler, UINT8* radioID, UINT8 mac_id)
{
	INIT_STATE_CHECK()
#ifdef DEBUG_RF231
	CPU_GPIO_SetPinState((GPIO_PIN)0, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN)0, FALSE);
#endif
	// Set MAC datastructures
	active_mac_index = Radio<Message_15_4_t>::GetMacIdIndex();
	if(Radio<Message_15_4_t>::Initialize(event_handler, mac_id) != DS_Success)
			return DS_Fail;

	//If the radio hardware is not alrady initialised, initialize it
	if(!IsInitialized()){
		SetRadioID(RadioID::GetUniqueRadioId());//Get your unique ID

		//Get cpu serial and hash it to use as node id
		UINT8 cpuserial[12];
		GetCPUSerial(cpuserial,12);
		MF_NODE_ID=0;
		UINT16 * temp = (UINT16 *) cpuserial;
		for (int i=0; i< 6; i++){
			MF_NODE_ID=MF_NODE_ID ^ temp[i]; //XOR 72-bit number to generate 16-bit hash
		}
		SetAddress(MF_NODE_ID);
		SetInitialized(TRUE);

		// Initially point to the driver buffer
		tx_msg_ptr = (Message_15_4_t *) tx_msg;

		// Initially point to driver buffer
		rx_msg_ptr = (Message_15_4_t *) rx_msg;

		GLOBAL_LOCK(irq);

		//for(UINT8 i = 0; i < 30; i++)
			//data[i] = 0;
#ifdef DEBUG_RF231
		CPU_GPIO_SetPinState((GPIO_PIN)0, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN)0, FALSE);
#endif
		GpioPinInitialize();
		//configure_exti();
#ifdef DEBUG_RF231
		CPU_GPIO_SetPinState((GPIO_PIN)0, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN)0, FALSE);
#endif
		if(TRUE != SpiInitialize())
		{
			ASSERT_RADIO("SPI Initialization failed");
		}

		// The radio intialization steps in the following lines are semantically copied from the corresponding tinyos implementation
		// Specified in the datasheet a sleep of 510us
		// The performance of this function is good but at times its found to generate different times. Its possible that there were other
		// events happening on the pin that was used to measure this or there is a possible bug !!!
		HAL_Time_Sleep_MicroSeconds(510);
#ifdef DEBUG_RF231
		CPU_GPIO_SetPinState((GPIO_PIN)0, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN)0, FALSE);
#endif
		// Clear rstn pin
		RstnClear();
#ifdef DEBUG_RF231
		CPU_GPIO_SetPinState((GPIO_PIN)0, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN)0, FALSE);
#endif
		// Clear the slptr pin
		SlptrClear();
#ifdef DEBUG_RF231
		CPU_GPIO_SetPinState((GPIO_PIN)0, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN)0, FALSE);
#endif
		// Sleep for 6us
		HAL_Time_Sleep_MicroSeconds(6);

#ifdef DEBUG_RF231
		CPU_GPIO_SetPinState((GPIO_PIN)0, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN)0, FALSE);
#endif
		RstnSet();

#ifdef DEBUG_RF231
		CPU_GPIO_SetPinState((GPIO_PIN)0, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN)0, FALSE);
#endif
		// The RF230_TRX_CTRL_0 register controls the drive current of the digital output pads and the CLKM clock rate
		// Setting value to 0
		WriteRegister(RF230_TRX_CTRL_0, RF230_TRX_CTRL_0_VALUE);

		UINT32 reg = 0;
		reg = ReadRegister(RF230_TRX_CTRL_0);

		// The RF230_TRX_STATE register controls the state transition
		WriteRegister(RF230_TRX_STATE, RF230_TRX_OFF);

#ifdef DEBUG_RF231
		CPU_GPIO_SetPinState((GPIO_PIN)0, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN)0, FALSE);
#endif
		// Nived.Sivadas - Hanging in the initialize function caused by the radio being in an unstable state
		// This fix will return false from initialize and enable the user of the function to exit gracefully
		// Fix for the hanging in the initialize function
#if 0
		while(true)
		{
			//reg = ReadRegister(RF230_TRX_STATE);
			reg = (ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK);
			if(reg == RF230_TRX_OFF)
				break;
		}
#endif
		DID_STATE_CHANGE(RF230_TRX_OFF);

#ifdef DEBUG_RF231
		CPU_GPIO_SetPinState((GPIO_PIN)0, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN)0, FALSE);
#endif
		HAL_Time_Sleep_MicroSeconds(510);

		// Register controls the interrupts that are currently enabled
	#ifndef RADIO_DEBUG
		//WriteRegister(RF230_IRQ_MASK, RF230_IRQ_TRX_UR | RF230_IRQ_PLL_LOCK | RF230_IRQ_TRX_END | RF230_IRQ_RX_START);
		WriteRegister(RF230_IRQ_MASK, RF230_IRQ_TRX_UR | RF230_IRQ_TRX_END | RF230_IRQ_RX_START);
	#else
		//WriteRegister(RF230_IRQ_MASK, RF230_IRQ_BAT_LOW | RF230_IRQ_TRX_UR | RF230_IRQ_PLL_LOCK | RF230_IRQ_TRX_END | RF230_IRQ_RX_START);
		WriteRegister(RF230_IRQ_MASK, RF230_IRQ_BAT_LOW | RF230_IRQ_TRX_UR | RF230_IRQ_TRX_END | RF230_IRQ_RX_START);
	#endif

		// The RF230_CCA_THRES sets the ed level for CCA, currently setting threshold to 0xc7
		WriteRegister(RF230_CCA_THRES, RF230_CCA_THRES_VALUE);

		// Controls output power and ramping of the transistor
		WriteRegister(RF230_PHY_TX_PWR, RF230_TX_AUTO_CRC_ON | (0 & RF230_TX_PWR_MASK));
		// Nived.Sivadas - turning off auto crc check
		//WriteRegister(RF230_PHY_TX_PWR, 0 | (0 & RF230_TX_PWR_MASK));
#ifdef DEBUG_RF231
		CPU_GPIO_SetPinState((GPIO_PIN)0, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN)0, FALSE);
#endif
		tx_power = 0 & RF230_TX_PWR_MASK;
		channel = RF230_DEF_CHANNEL & RF230_CHANNEL_MASK;

		// Sets the channel number
		WriteRegister(RF230_PHY_CC_CCA, RF230_CCA_MODE_VALUE | channel);
#ifdef DEBUG_RF231
		CPU_GPIO_SetPinState((GPIO_PIN)0, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN)0, FALSE);
#endif
		// Enable the gpio pin as the interrupt point
		CPU_GPIO_EnableInputPin(INTERRUPT_PIN,FALSE, Radio_Handler, GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED);


		SlptrSet();
#ifdef DEBUG_RF231
		CPU_GPIO_SetPinState((GPIO_PIN)0, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN)0, FALSE);
#endif
		// set software state machine state to sleep
		state = STATE_SLEEP;

		// Initialize default radio handlers

		// Added here until the state issues are resolved
		TurnOn();

		cmd = CMD_NONE;
#ifdef DEBUG_RF231
		CPU_GPIO_SetPinState((GPIO_PIN)0, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN)0, FALSE);
#endif
	}

	UINT8 tempId = Radio<Message_15_4_t>::GetRadioID();
	*radioID = tempId;
	return DS_Success;
}

//template<class T>
void RF231Radio::WriteRegister(UINT8 reg, UINT8 value)
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
//template<class T>
void RF231Radio::GpioPinInitialize()
{
	CPU_GPIO_EnableOutputPin(kseln,TRUE);
	CPU_GPIO_EnableOutputPin(kslpTr,FALSE);
	CPU_GPIO_EnableOutputPin(krstn,TRUE);

#ifdef DEBUG_RF231
	CPU_GPIO_EnableOutputPin((GPIO_PIN)0, TRUE);
#endif
}

// Calls the mf spi initialize function and returns true if the intialization was successful
//template<class T>
BOOL RF231Radio::SpiInitialize()
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


//template<class T>
DeviceStatus RF231Radio::TurnOn()
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

	//Mukundan: Oct 11,2012: Commented the below two lines, to stop initialization being interrupted
	//WriteRegister(RF230_TRX_STATE, RF230_PLL_ON);
	//DID_STATE_CHANGE(RF230_PLL_ON);

	WriteRegister(RF230_TRX_STATE, RF230_RX_ON);

	// OPTIMIZATION_POSSIBLE
	// Wait till radio is in rx on state
	DID_STATE_CHANGE(RF230_RX_ON);

	// Change the state to RX_ON
	state = STATE_RX_ON;
}

//template<class T>
UINT8 RF231Radio::ReadRegister(UINT8 reg)
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

//template<class T>
DeviceStatus RF231Radio::ClearChannelAssesment(UINT32 numberMicroSecond)
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

//template<class T>
DeviceStatus RF231Radio::ClearChannelAssesment()
{
	UINT8 trx_status;

	GLOBAL_LOCK(irq);

	WriteRegister(RF230_PHY_CC_CCA, RF230_CCA_REQUEST | RF230_CCA_MODE_VALUE | channel);

	// Busy wait for the minimum duration of 140 us
	HAL_Time_Sleep_MicroSeconds(150);

	trx_status = ReadRegister(RF230_TRX_STATE);

	return ((trx_status & RF230_CCA_DONE) ? ((trx_status & RF230_CCA_STATUS) ? DS_Success : DS_Busy) : DS_Fail );
}


//template<class T>
void RF231Radio::HandleInterrupt()
{
	UINT8 irq_cause;
	INT16 temp;
	INIT_STATE_CHECK();
	UINT32 time;
#ifdef DEBUG_RF231
	CPU_GPIO_SetPinState((GPIO_PIN)0, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN)0, FALSE);
#endif

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
#ifdef DEBUG_RF231
			CPU_GPIO_SetPinState((GPIO_PIN)0, TRUE);
			CPU_GPIO_SetPinState((GPIO_PIN)0, FALSE);
#endif
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
#ifdef DEBUG_RF231
					//CPU_GPIO_SetPinState((GPIO_PIN)0, TRUE);
					//CPU_GPIO_SetPinState((GPIO_PIN)0, FALSE);
					CPU_GPIO_SetPinState((GPIO_PIN) 4, FALSE);
#endif

					// Call radio send done event handler when the send is complete
					SendAckFuncPtrType AckHandler = Radio<Message_15_4_t>::GetMacHandler(active_mac_index)->GetSendAckHandler();
					(*AckHandler)(tx_msg_ptr, tx_length,NO_Success);

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
			DownloadMessage();
			//rx_msg_ptr->SetActiveMessageSize(rx_length);
			if(rx_length>  IEEE802_15_4_FRAME_LENGTH){
				hal_printf("Radio Receive Error: Packet too big: %d ",rx_length);
				return;
			}

			(rx_msg_ptr->GetHeader())->SetLength(rx_length);
			rx_msg_ptr = (Message_15_4_t *) (Radio<Message_15_4_t>::GetMacHandler(active_mac_index)->GetRecieveHandler())(rx_msg_ptr, rx_length);
		}
	}
#ifdef DEBUG_RF231
	CPU_GPIO_SetPinState((GPIO_PIN)0, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN)0, FALSE);
#endif
}

//template<class T>
DeviceStatus RF231Radio::DownloadMessage()
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

		read = length - 2;
		temp_rx_msg_ptr[counter++] =length;
		do
		{
			temp_rx_msg_ptr[counter++] = CPU_SPI_WriteReadByte(config, 0);
		}
		while(--read != 0);

	}
	SelnSet();

	state = STATE_RX_ON;

	cmd = CMD_NONE;

	rx_length = length - 2;
}


extern RF231Radio grf231Radio;

extern "C"
{

	// Call radio_irq_handler from here
	void Radio_Handler(GPIO_PIN Pin, BOOL PinState, void* Param)
	{
		grf231Radio.HandleInterrupt();
	}
}

