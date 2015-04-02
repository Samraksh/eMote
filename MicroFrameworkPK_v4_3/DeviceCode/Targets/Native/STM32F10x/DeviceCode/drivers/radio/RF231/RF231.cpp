#include "RF231.h"
#include "RF231RegDef.h"
#include <tinyhal.h>

//#define DEBUG_RF231 1

//extern STM32F10x_GPIO_Driver g_STM32F10x_Gpio_Driver;

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
	// Adding 2 for crc and 4 bytes for timestamp
	 if(size+2+4 > IEEE802_15_4_FRAME_LENGTH){
#ifdef DEBUG_RF231
	                hal_printf("Radio Send Error: Big packet: %d\r\n", size+2);
#endif
	                SendAckFuncPtrType AckHandler = Radio<Message_15_4_t>::GetMacHandler(active_mac_index)->GetSendAckHandler();
	                // Should be bad size
	                (*AckHandler)(tx_msg_ptr, tx_length,NO_BadPacket);
	                return msg;
	 }

	        INIT_STATE_CHECK();
	        GLOBAL_LOCK(irq);
	        //__ASM volatile("cpsid i");
	        //pulse 1
	#ifdef DEBUG_RF231
	        CPU_GPIO_SetPinState((GPIO_PIN)24, TRUE);
	        CPU_GPIO_SetPinState((GPIO_PIN)24, FALSE);
	#endif

	        UINT32 channel = 0;
	        UINT32 reg = 0;
	        UINT32 read = 0;

	        tx_length = size;

	        if(cmd != CMD_NONE || state == STATE_BUSY_TX)
	        {
	                SendAckFuncPtrType AckHandler = Radio<Message_15_4_t>::GetMacHandler(active_mac_index)->GetSendAckHandler();
#ifdef DEBUG_RF231
	                hal_printf("I have no command: %d or I am busy: %d\r\n", cmd, state);
#endif
	                (*AckHandler)(tx_msg_ptr, tx_length,NO_Busy);
	                return msg;
	        }

	        // If the radio is sleeping when the send command is issued, it is still capable of sending
	        // data out and going back to sleep once this is done
	        if(state == STATE_SLEEP)
	        {
	                sleep_pending = TRUE;
	                //TurnOn();
	        }

	        // Send gives the CMD_TRANSMIT to the radio
	        //cmd = CMD_TRANSMIT;
	#if 0
	        if(state != STATE_PLL_ON)
	                return DS_Busy;
	#endif
	        //pulse 2
	#ifdef DEBUG_RF231
	        CPU_GPIO_SetPinState((GPIO_PIN)24, TRUE);
	        CPU_GPIO_SetPinState((GPIO_PIN)24, FALSE);
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
	        CPU_GPIO_SetPinState((GPIO_PIN)24, TRUE);
	        CPU_GPIO_SetPinState((GPIO_PIN)24, FALSE);
	#endif

	        SlptrSet();
	        for(UINT8 i =0; i < 10; i++);
	        SlptrClear();

	        // Load buffer before initiating the transmit command
	        SelnClear();

	        CPU_SPI_WriteByte(config, RF230_CMD_FRAME_WRITE);

	        // Write the size of packet that is sent out
	        // Including FCS which is automatically generated and is two bytes
	        //Plus 4 bytes for timestamp
	        CPU_SPI_ReadWriteByte(config, size+ 2 +4);

	        UINT8 lLength = size ;
	        UINT8 timeStampLength = 4;
	        UINT32 eventOffset;
	        UINT8 * timeStampPtr = (UINT8 *) &eventOffset;
	        //AnanthAtSamraksh: defaulting to the AdvancedTimer
	        UINT32 timestamp = HAL_Time_CurrentTicks() & (~(UINT32) 0);
	        eventOffset = timestamp - eventTime;

	#ifdef DEBUG_TIMESYNC
	        CPU_GPIO_SetPinState((GPIO_PIN) 30, TRUE);
	        CPU_GPIO_SetPinState((GPIO_PIN) 30, FALSE);
	#endif
	        //Transmit the packet
	        do
	        {
	                CPU_SPI_ReadWriteByte(config, *(ldata++));
	        }while(--lLength != 0);
	#ifdef DEBUG_TIMESYNC
	                CPU_GPIO_SetPinState((GPIO_PIN) 30, TRUE);
	                CPU_GPIO_SetPinState((GPIO_PIN) 30, FALSE);
	#endif
	        //Transmit the event timestamp
	        do
	        {
	                CPU_SPI_ReadWriteByte(config, *(timeStampPtr++));
	        }while(--timeStampLength != 0);
	#ifdef DEBUG_TIMESYNC
	                CPU_GPIO_SetPinState((GPIO_PIN) 30, TRUE);
	                CPU_GPIO_SetPinState((GPIO_PIN) 30, FALSE);
	#endif
	        CPU_SPI_ReadByte(config);

	        //pulse 4
	#ifdef DEBUG_RF231
	        CPU_GPIO_SetPinState((GPIO_PIN)24, TRUE);
	        CPU_GPIO_SetPinState((GPIO_PIN)24, FALSE);
	        //hal_printf("Radio timestamp: %lu \n", eventOffset);
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
	        CPU_GPIO_SetPinState((GPIO_PIN)24, TRUE);
	        CPU_GPIO_SetPinState((GPIO_PIN)24, FALSE);
	#endif
	        //__ASM volatile("cpsie i");
	        return temp;
}

DeviceStatus RF231Radio::Reset()
{
	INIT_STATE_CHECK()

	//GLOBAL_LOCK(irq);

	/*if(TRUE != GpioPinInitialize())
	{
		return DS_Fail;
	}
			//configure_exti();
	if(TRUE != SpiInitialize())
	{
		ASSERT_RADIO("SPI Initialization failed");
	}*/


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
			CPU_GPIO_SetPinState((GPIO_PIN)24, TRUE);
			CPU_GPIO_SetPinState((GPIO_PIN)24, FALSE);
	#endif

	// Enable the gpio pin as the interrupt point
	if(this->GetRadioName() == RF231RADIO)
		CPU_GPIO_EnableInputPin(INTERRUPT_PIN, FALSE, Radio_Handler, GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED);
	else if(this->GetRadioName() == RF231RADIOLR)
		CPU_GPIO_EnableInputPin(INTERRUPT_PIN_LR, FALSE, Radio_Handler_LR, GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED);


	SlptrSet();
	#ifdef DEBUG_RF231
			CPU_GPIO_SetPinState((GPIO_PIN)24, TRUE);
			CPU_GPIO_SetPinState((GPIO_PIN)24, FALSE);
	#endif
			// set software state machine state to sleep
	state = STATE_SLEEP;

	// Introducing radio sleep
	//TurnOn();

	cmd = CMD_NONE;
	#ifdef DEBUG_RF231
			CPU_GPIO_SetPinState((GPIO_PIN)24, TRUE);
			CPU_GPIO_SetPinState((GPIO_PIN)24, FALSE);
	#endif
	return DS_Success;
}


UINT32 RF231Radio::GetChannel()
{
	return this->channel;
}

UINT32 RF231Radio::GetTxPower()
{
	return this->tx_power;
}

// Change the power level of the radio
DeviceStatus RF231Radio::ChangeTxPower(int power)
{
	// Cannot change power level if radio is in the middle of something
	// There is no reason for this in the manual, but adding this check for sanity sake
	if(state != STATE_SLEEP && state != STATE_RX_ON)
		return DS_Fail;

	if(state == STATE_SLEEP)
	{
		if(TurnOnPLL() != DS_Success)
		{
			return DS_Fail;
		}
		sleep_pending = TRUE;
	}

	this->tx_power = power  & RF230_TX_PWR_MASK;

	WriteRegister(RF230_PHY_TX_PWR, RF230_TX_AUTO_CRC_ON | (power & RF230_TX_PWR_MASK));


	if(sleep_pending == TRUE)
	{
		// If we are unable to go back to sleep for some reason, return a failure and then set
		// sleep pending to true in a hope it can be put to sleep later on
		if(Sleep(0) != DS_Success)
		{
			sleep_pending = TRUE;
			return DS_Fail;
		}
		sleep_pending = FALSE;
	}

	return DS_Success;
}

// Change the channel of the radio
DeviceStatus RF231Radio::ChangeChannel(int channel)
{
	// Cannot change channel if radio is in the middle of something
	// There is no reason for this in the manual, but adding this check for sanity sake
	if(state != STATE_SLEEP && state != STATE_RX_ON)
		return DS_Fail;

	// If the radio is sleeping and you attempt to change channel
	// Turn it on, change the channel and push it back to sleep
	if(state == STATE_SLEEP)
	{
		if(TurnOnPLL() != DS_Success)
		{
			return DS_Fail;
		}
		sleep_pending = TRUE;
	}

	// The value passed as channel until this point is an enum and needs to be offset by 11 to set the
	// actual radio channel value
	this->channel = (channel + RF230_CHANNEL_OFFSET) & RF230_CHANNEL_MASK;

	WriteRegister(RF230_PHY_CC_CCA, RF230_CCA_MODE_VALUE | this->channel);

	if(sleep_pending == TRUE)
	{
		// If we are unable to go back to sleep for some reason, return a failure and then set
		// sleep pending to true in a hope it can be put to sleep later on
		if(Sleep(0) != DS_Success)
		{
			sleep_pending = TRUE;
			return DS_Fail;
		}
		sleep_pending = FALSE;
	}

	return DS_Success;
}

// There is one level of sleeping
DeviceStatus RF231Radio::Sleep(int level)
{
	// State variable change in this function, possible race condition
	// Lock
	GLOBAL_LOCK(irq);

	// Initiailize state change check variables
	// Primarily used if DID_STATE_CHANGE_ASSERT is used
	INIT_STATE_CHECK();

	// If we are already in sleep state do nothing
	// Unsure if during sleep we can read registers
	if(state == STATE_SLEEP)
	{
		return DS_Success;
	}
	else if(state == STATE_BUSY_TX)
	{
		sleep_pending = TRUE;
		return DS_Success;
	}

	// Turn of things before going to sleep
	if(RF231RADIOLR == this->GetRadioName())
	{
		this->Amp(FALSE);
		this->PARXTX(FALSE);
		this->AntDiversity(FALSE);
	}

	// Read current state of radio
	UINT32 regState = (ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK);

	// If radio is in trx off state then acc to the state diagram just pull slptr high
	if(regState == RF230_TRX_OFF)
	{
		// Setting Slptr moves the radio to sleep state
		SlptrSet();
	}
	// If radio is in RX_ON or PLL_ON move the radio to TRX_OFF before pulling slptr high
	else if(regState == RF230_RX_ON || regState == RF230_PLL_ON)
	{
		// First move the radio to TRX_OFF
		WriteRegister(RF230_TRX_STATE, RF230_TRX_OFF);

		// Check if state change was successful
		DID_STATE_CHANGE(RF230_TRX_OFF);

		// Setting Slptr moves the radio to sleep state
		SlptrSet();

		state = STATE_SLEEP;
	}
	// The radio is busy doing something, we are not in a position to handle this request
	else
	{
		sleep_pending = TRUE;
	}

	return DS_Success;

}

void* RF231Radio::Send(void* msg, UINT16 size)
{
	// Adding two bytes for crc
	if(size+2 > IEEE802_15_4_FRAME_LENGTH){
#ifdef DEBUG_RF231
		hal_printf("Radio Send Error: Big packet: %d \r\n", size+2);
#endif
		
		SendAckFuncPtrType AckHandler = Radio<Message_15_4_t>::GetMacHandler(active_mac_index)->GetSendAckHandler();
		// Should be bad size
		(*AckHandler)(tx_msg_ptr, tx_length,NO_BadPacket);

		
		return msg;
	}
	
	INIT_STATE_CHECK();

	GLOBAL_LOCK(irq);
	//__ASM volatile("cpsid i");
	//pulse 1
#ifdef DEBUG_RF231
	CPU_GPIO_SetPinState((GPIO_PIN)24, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN)24, FALSE);
#endif

	UINT32 channel = 0;
	UINT32 reg = 0;
	UINT32 read = 0;

	tx_length = size;

	
	if(cmd != CMD_NONE || state == STATE_BUSY_TX)
	{
		SendAckFuncPtrType AckHandler = Radio<Message_15_4_t>::GetMacHandler(active_mac_index)->GetSendAckHandler();
		// Should be bad size
		(*AckHandler)(tx_msg_ptr, tx_length,NO_Busy);

		return msg;
	}


	// If the radio is sleeping when the send command is issued, it is still capable of sending
	// data out and going back to sleep once this is done
	if(state == STATE_SLEEP)
	{
		sleep_pending = TRUE;
	}
	// Send gives the CMD_TRANSMIT to the radio
	//cmd = CMD_TRANSMIT;
#if 0
	if(state != STATE_PLL_ON)
		return DS_Busy;
#endif
	//pulse 2
#ifdef DEBUG_RF231
	CPU_GPIO_SetPinState((GPIO_PIN)24, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN)24, FALSE);
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
	CPU_GPIO_SetPinState((GPIO_PIN)24, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN)24, FALSE);
#endif

	//SlptrSet();
	//for(UINT8 i =0; i < 10; i++);
	//SlptrClear();

	//pulse 4
#ifdef DEBUG_RF231
	//CPU_GPIO_SetPinState((GPIO_PIN)24, TRUE);
	//CPU_GPIO_SetPinState((GPIO_PIN)24, FALSE);
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
	CPU_GPIO_SetPinState((GPIO_PIN)24, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN)24, FALSE);
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
	CPU_GPIO_SetPinState((GPIO_PIN)24, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN)24, FALSE);
#endif
	//__ASM volatile("cpsie i");
	return temp;
}

DeviceStatus RF231Radio::AntDiversity(BOOL enable)
{
	// only works on the long range radio board
	if(this->GetRadioName() != RF231RADIOLR)
	{
		return DS_Fail;
	}

	UINT8 data = ReadRegister(RF231_REG_ANT_DIV);

	if(enable)
	{
		data |= 0x0C;
	}
	else
	{
		data &= ~(3 << 2);
	}


	WriteRegister(RF231_REG_ANT_DIV, data);

	return DS_Success;

}

DeviceStatus RF231Radio::PARXTX(BOOL enable)
{
	// only works on the long range radio board
	if(this->GetRadioName() != RF231RADIOLR)
	{
		return DS_Fail;
	}

	UINT8 data = ReadRegister(RF231_REG_TX_CTRL_1);

	if(enable)
	{
		data |= 0x80;
	}
	else
	{
		data &= ~(1 << 7);
	}

	WriteRegister(RF231_REG_TX_CTRL_1, data);

	return DS_Success;

}

void RF231Radio::Amp(BOOL TurnOn)
{
	if(this->GetRadioName() != RF231RADIOLR)
	{
		return;
	}

	CPU_GPIO_SetPinState((GPIO_PIN) AMP_LR, TurnOn);
}


/* AnanthAtSamraksh - adding below change to fix receive on LR radio extension board - 2/11/2014 */

uint8_t  rf231_read_reg(uint8_t reg) {
	uint8_t ret;
	RF231_SEL(0);
	while (SPI_I2S_GetFlagStatus(RF231_SPI, SPI_I2S_FLAG_TXE) == RESET) { ; }
	SPI_I2S_SendData(RF231_SPI, 0x80 | reg);
	while ( SPI_I2S_GetFlagStatus(RF231_SPI, SPI_I2S_FLAG_RXNE) == RESET) {;}
	ret = SPI_I2S_ReceiveData(RF231_SPI);
	while (SPI_I2S_GetFlagStatus(RF231_SPI, SPI_I2S_FLAG_TXE) == RESET) { ; }
	SPI_I2S_SendData(RF231_SPI, 0x00);
	while ( SPI_I2S_GetFlagStatus(RF231_SPI, SPI_I2S_FLAG_RXNE) == RESET) {;}
	ret = SPI_I2S_ReceiveData(RF231_SPI);
	RF231_SEL(1);
	return ret;
}

uint8_t rf231_write_reg(uint8_t reg, uint8_t data) {
	uint8_t ret;
	volatile int i;
	RF231_SEL(0);
	while (SPI_I2S_GetFlagStatus(RF231_SPI, SPI_I2S_FLAG_TXE) == RESET) { ; }
	SPI_I2S_SendData(RF231_SPI, 0xC0 | reg);
	while ( SPI_I2S_GetFlagStatus(RF231_SPI, SPI_I2S_FLAG_RXNE) == RESET) {;}
	ret = SPI_I2S_ReceiveData(RF231_SPI);
	while (SPI_I2S_GetFlagStatus(RF231_SPI, SPI_I2S_FLAG_TXE) == RESET) { ; }
	SPI_I2S_SendData(RF231_SPI, data);
	while ( SPI_I2S_GetFlagStatus(RF231_SPI, SPI_I2S_FLAG_RXNE) == RESET) {;}
	ret = SPI_I2S_ReceiveData(RF231_SPI);
	RF231_SEL(1);
	HAL_Time_Sleep_MicroSeconds(3500);
	return ret;
}

void rf231_enable_pa_rxtx(void) {
	volatile uint8_t data = rf231_read_reg(RF231_REG_TX_CTRL_1);
	data |= 0x80;
	rf231_write_reg(RF231_REG_TX_CTRL_1, data);
}

/* AnanthAtSamraksh */


DeviceStatus RF231Radio::Initialize(RadioEventHandler *event_handler, UINT8 radio, UINT8 mac_id)
{
	INIT_STATE_CHECK()
#ifdef DEBUG_RF231
	CPU_GPIO_SetPinState((GPIO_PIN)24, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN)24, FALSE);
#endif



	// Set MAC datastructures
	active_mac_index = Radio<Message_15_4_t>::GetMacIdIndex();
	if(Radio<Message_15_4_t>::Initialize(event_handler, mac_id) != DS_Success)
			return DS_Fail;

	//If the radio hardware is not alrady initialised, initialize it
	if(!IsInitialized())
	{

		// Give the radio its name , rf231 or rf231 long range
		this->SetRadioName(radio);

		// Set the corresponding gpio pins
		if(this->GetRadioName() == RF231RADIO)
		{
			kslpTr 		= 	 SLP_TR_PIN;
			krstn 		= 	 RSTN_PIN;
			kseln		= 	 SELN_PIN;
			kinterrupt	= 	 INTERRUPT_PIN;
		}
		else if(this->GetRadioName() == RF231RADIOLR)
		{
			kslpTr		=    SLP_TR_PIN_LR;
			krstn 		= 	 RSTN_PIN_LR;
			kseln		= 	 SELN_PIN_LR;
			kinterrupt	= 	 INTERRUPT_PIN_LR;

			// Enable the amp pin
			CPU_GPIO_EnableOutputPin((GPIO_PIN) AMP_LR, FALSE);
		}

		//Get cpu serial and hash it to use as node id
		UINT8 cpuserial[12];
		GetCPUSerial(cpuserial,12);
		UINT16 tempNum=0;
		UINT16 * temp = (UINT16 *) cpuserial;
		for (int i=0; i< 6; i++){
			tempNum=tempNum ^ temp[i]; //XOR 72-bit number to generate 16-bit hash
		}
#ifdef DEBUG_RF231
		hal_printf("gen tempNum: %d\r\n", tempNum);
#endif
		SetAddress(tempNum);
		SetInitialized(TRUE);

		// Initially point to the driver buffer
		tx_msg_ptr = (Message_15_4_t *) tx_msg;

		// Initially point to driver buffer
		rx_msg_ptr = (Message_15_4_t *) rx_msg;

		// Set the state of sleep pending to false
		sleep_pending = FALSE;

		GLOBAL_LOCK(irq);
		//for(UINT8 i = 0; i < 30; i++)
			//data[i] = 0;
#ifdef DEBUG_RF231
		CPU_GPIO_SetPinState((GPIO_PIN)24, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN)24, FALSE);
#endif
		if(TRUE != GpioPinInitialize())
		{
			return DS_Fail;
		}
		//configure_exti();
#ifdef DEBUG_RF231
		CPU_GPIO_SetPinState((GPIO_PIN)24, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN)24, FALSE);
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
		CPU_GPIO_SetPinState((GPIO_PIN)24, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN)24, FALSE);
#endif
		// Clear rstn pin
		RstnClear();
#ifdef DEBUG_RF231
		CPU_GPIO_SetPinState((GPIO_PIN)24, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN)24, FALSE);
#endif
		// Clear the slptr pin
		SlptrClear();
#ifdef DEBUG_RF231
		CPU_GPIO_SetPinState((GPIO_PIN)24, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN)24, FALSE);
#endif
		// Sleep for 6us
		HAL_Time_Sleep_MicroSeconds(6);

#ifdef DEBUG_RF231
		CPU_GPIO_SetPinState((GPIO_PIN)24, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN)24, FALSE);
#endif
		RstnSet();

#ifdef DEBUG_RF231
		CPU_GPIO_SetPinState((GPIO_PIN)24, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN)24, FALSE);
#endif
		// The RF230_TRX_CTRL_0 register controls the drive current of the digital output pads and the CLKM clock rate
		// Setting value to 0
		WriteRegister(RF230_TRX_CTRL_0, RF230_TRX_CTRL_0_VALUE);

		UINT32 reg = 0;
		reg = ReadRegister(RF230_TRX_CTRL_0);

		// The RF230_TRX_STATE register controls the state transition
		WriteRegister(RF230_TRX_STATE, RF230_TRX_OFF);

#ifdef DEBUG_RF231
		CPU_GPIO_SetPinState((GPIO_PIN)24, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN)24, FALSE);
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
		CPU_GPIO_SetPinState((GPIO_PIN)24, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN)24, FALSE);
#endif
		HAL_Time_Sleep_MicroSeconds(510);

		if(this->GetRadioName() == RF231RADIOLR)
		{
			/* AnanthAtSamraksh - adding below line to fix receive on LR radio extension board - 2/11/2014 */
			// Enable external PA and control from RF231
			rf231_enable_pa_rxtx();
		}


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
		CPU_GPIO_SetPinState((GPIO_PIN)24, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN)24, FALSE);
#endif
		tx_power = 0 & RF230_TX_PWR_MASK;
		channel = RF230_DEF_CHANNEL & RF230_CHANNEL_MASK;

		// Sets the channel number
		WriteRegister(RF230_PHY_CC_CCA, RF230_CCA_MODE_VALUE | channel);
#ifdef DEBUG_RF231
		CPU_GPIO_SetPinState((GPIO_PIN)24, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN)24, FALSE);
#endif
		// Enable the gpio pin as the interrupt point
		if(this->GetRadioName() == RF231RADIO){
			CPU_GPIO_EnableInputPin(INTERRUPT_PIN, FALSE, Radio_Handler, GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED);
		}
		else if(this->GetRadioName() == RF231RADIOLR){
			CPU_GPIO_EnableInputPin(INTERRUPT_PIN_LR, FALSE, Radio_Handler_LR, GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED);
			GPIO_ConfigurePin(GPIOB, (GPIO_PIN)12, GPIO_Mode_Out_PP, GPIO_Speed_2MHz);
		}

		SlptrSet();
#ifdef DEBUG_RF231
		CPU_GPIO_SetPinState((GPIO_PIN)24, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN)24, FALSE);
#endif
		// set software state machine state to sleep
		state = STATE_SLEEP;

		// Initialize default radio handlers

		// Added here until the state issues are resolved
		//TurnOn();

		cmd = CMD_NONE;
#ifdef DEBUG_RF231
		CPU_GPIO_SetPinState((GPIO_PIN)24, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN)24, FALSE);
#endif

	}

	return DS_Success;
}

DeviceStatus RF231Radio::UnInitialize()
{
    DeviceStatus ret = DS_Success;
    if(IsInitialized())
    {
        RstnClear();
        ASSERT((active_mac_index & 0xFF00) == 0)
        if(Radio<Message_15_4_t>::UnInitialize((UINT8)active_mac_index) != DS_Success) {
                ret = DS_Fail;
        }
        SpiUnInitialize();
        GpioPinUnInitialize();
        if(this->GetRadioName() == RF231RADIO){
            CPU_GPIO_DisablePin(INTERRUPT_PIN, RESISTOR_DISABLED,  GPIO_Mode_IN_FLOATING, GPIO_ALT_PRIMARY);
        }
        else if(this->GetRadioName() == RF231RADIOLR){
            CPU_GPIO_DisablePin(INTERRUPT_PIN_LR, RESISTOR_DISABLED, GPIO_Mode_IN_FLOATING, GPIO_ALT_PRIMARY);
            CPU_GPIO_DisablePin((GPIO_PIN)12, RESISTOR_DISABLED, GPIO_Mode_IN_FLOATING, GPIO_ALT_PRIMARY);
        }
        SetInitialized(FALSE);
    }
    return ret;
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
BOOL RF231Radio::GpioPinInitialize()
{

	if(CPU_GPIO_PinIsBusy(kseln))
		return FALSE;

	if(CPU_GPIO_PinIsBusy(kslpTr))
		return FALSE;

	if(CPU_GPIO_PinIsBusy(krstn))
		return FALSE;


	if(!CPU_GPIO_ReservePin(kseln, TRUE))
		return FALSE;

	if(!CPU_GPIO_ReservePin(kslpTr, TRUE))
		return FALSE;

	if(!CPU_GPIO_ReservePin(krstn, TRUE))
		return FALSE;


	CPU_GPIO_EnableOutputPin(kseln,TRUE);
	CPU_GPIO_EnableOutputPin(kslpTr,FALSE);
	CPU_GPIO_EnableOutputPin(krstn,TRUE);

	return TRUE;

}

//TODO: combine GpioPinUnInitialize and GpioPinInitialize
BOOL RF231Radio::GpioPinUnInitialize()
{

    if(!CPU_GPIO_ReservePin(kseln, FALSE))
        return FALSE;

    if(!CPU_GPIO_ReservePin(kslpTr, FALSE))
        return FALSE;

    if(!CPU_GPIO_ReservePin(krstn, FALSE))
        return FALSE;

    CPU_GPIO_DisablePin(kseln, RESISTOR_DISABLED, GPIO_Mode_IN_FLOATING, GPIO_ALT_PRIMARY);
    CPU_GPIO_DisablePin(kslpTr, RESISTOR_DISABLED, GPIO_Mode_IN_FLOATING, GPIO_ALT_PRIMARY);
    CPU_GPIO_DisablePin(krstn, RESISTOR_DISABLED, GPIO_Mode_IN_FLOATING, GPIO_ALT_PRIMARY);

    return TRUE;

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
	if(this->GetRadioName() == RF231RADIO)
	{
		config.SPI_mod                = SPIBUS1;
	}
	else if(this->GetRadioName() == RF231RADIOLR)
	{
		config.SPI_mod 				  = SPIBUS2;
	}

	config.MD_16bits = FALSE;


	// Enable the SPI depending on the radio who is the user
	CPU_SPI_Enable(config);

	return TRUE;
}


BOOL RF231Radio::SpiUnInitialize()
{
    BOOL ret = FALSE;
    if(IsInitialized())
    {
        CPU_SPI_Uninitialize(config);
        ret = TRUE;
    }
    return ret;
}


// This function moves the radio from sleep to RX_ON
//template<class T>
DeviceStatus RF231Radio::TurnOnRx()
{
	INIT_STATE_CHECK();
	GLOBAL_LOCK(irq);

	// The radio is not sleeping or is already on
	if(state == STATE_RX_ON)
	{
		return DS_Success;
	}

	if(this->GetRadioName() == RF231RADIOLR)
	{
		// Enable antenna diversity mode
		this->AntDiversity(TRUE);

		// Enable external pa control
		this->PARXTX(TRUE);

		// take the amp  out of bypass mode
		this->Amp(TRUE);
	}

	SlptrClear();

	// Even though an interrupt can never happen here, just for the sake of being in synch with the radio
	state = STATE_SLEEP_2_TRX_OFF;

	// Sleep for 200us and wait for the radio to come oout of sleep
	HAL_Time_Sleep_MicroSeconds(200);

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

	return DS_Success;

}

// This function moves the radio from sleep to RX_ON
//template<class T>
DeviceStatus RF231Radio::TurnOnPLL()
{
	INIT_STATE_CHECK();
	GLOBAL_LOCK(irq);

	// The radio is not sleeping or is already on
	if(state == STATE_PLL_ON)
	{
		return DS_Success;
	}

	if(this->GetRadioName() == RF231RADIOLR)
	{
		// Enable antenna diversity mode
		this->AntDiversity(TRUE);

		// Enable external pa control
		this->PARXTX(TRUE);

		// take the amp  out of bypass mode
		this->Amp(TRUE);
	}

	SlptrClear();

	// Sleep for 200us and wait for the radio to come oout of sleep
	HAL_Time_Sleep_MicroSeconds(200);

	DID_STATE_CHANGE(RF230_TRX_OFF);

	// Push radio to pll on state
	if(((ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK)== RF230_RX_ON) || ((ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK) == RF230_TRX_OFF))
	{
		WriteRegister(RF230_TRX_STATE, RF230_PLL_ON);
	    // Wait for radio to go into pll on and return efail if the radio fails to transition
	    DID_STATE_CHANGE_ASSERT(RF230_PLL_ON);
	}

	state = STATE_PLL_ON;
	cmd = CMD_NONE;

	return DS_Success;

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

	// The radio takes a minimum of 140 us to do cca, any numbers less than this are not acceptable
	if(numberMicroSecond < 140)
		return DS_Fail;

	// If cca is initiated during sleep, come out of sleep do cca and go back to sleep
	if(state == STATE_SLEEP)
	{
		if(TurnOnRx() != DS_Success)
			return DS_Fail;

		sleep_pending = TRUE;
		state = STATE_RX_ON;
	}

	GLOBAL_LOCK(irq);

	if(state != STATE_RX_ON)
	{
		UINT8 reg = ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK;
		return DS_Fail;
	}

	//UINT8 reg = ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK;

	// Make a cca request
	WriteRegister(RF230_PHY_CC_CCA, RF230_CCA_REQUEST | RF230_CCA_MODE_VALUE | channel);

	// Busy wait for the duration of numberMicrosecond
	HAL_Time_Sleep_MicroSeconds(numberMicroSecond);

	// Read the register to check the result of the assessment
	trx_status = ReadRegister(RF230_TRX_STATUS);


	// If the CCA was initiated during sleep, go back to sleep once the work is done
	if(sleep_pending)
	{
		// If sleep is success then go back to sleep and turn sleep_pending to FALSE
		if(Sleep(0) == DS_Success)
		{
			state = STATE_SLEEP;
			sleep_pending = FALSE;
		}
		else
		{
			return DS_Fail;
		}
	}

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
	CPU_GPIO_SetPinState((GPIO_PIN)24, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN)24, FALSE);
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
#ifdef DEBUG_RF231
			hal_printf("+++\r\n");
#endif
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
				//AnanthAtSamraksh: defaulting to the AdvancedTimer
				time = (HAL_Time_CurrentTicks() >> 32);

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
			CPU_GPIO_SetPinState((GPIO_PIN)24, TRUE);
			CPU_GPIO_SetPinState((GPIO_PIN)24, FALSE);
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
					//CPU_GPIO_SetPinState((GPIO_PIN)24, TRUE);
					//CPU_GPIO_SetPinState((GPIO_PIN)24, FALSE);
					CPU_GPIO_SetPinState((GPIO_PIN) 4, FALSE);
#endif

					// Call radio send done event handler when the send is complete
					SendAckFuncPtrType AckHandler = Radio<Message_15_4_t>::GetMacHandler(active_mac_index)->GetSendAckHandler();
					(*AckHandler)(tx_msg_ptr, tx_length,NO_Success);

					if(sleep_pending)
					{
						state = STATE_PLL_ON;

						cmd = CMD_NONE;

						//Sleep(0);
						UINT32 regState = (ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK);

						if(regState == RF230_PLL_ON)
						{
							WriteRegister(RF230_TRX_STATE, RF230_TRX_OFF);

							DID_STATE_CHANGE_ASSERT(RF230_TRX_OFF);

							SlptrSet();

							state = STATE_SLEEP;

						}

						sleep_pending = FALSE;

					}
					else
					{
						state = STATE_PLL_ON;

						cmd = CMD_NONE;
						//SlptrClear();
						// Check if radio is in pll state, we shouldn't be here if we are not in this state
						// Hence die if you are not in RF230_PLL_ON
						//DID_STATE_CHANGE_ASSERT(RF230_PLL_ON);
						// Change the state of the radio to RX_ON

						WriteRegister(RF230_TRX_STATE, RF230_RX_ON);

						// OPTIMIZATION_POSSIBLE
						// Wait till radio is in rx on state or die here, other wise radio and software go out of sync
						DID_STATE_CHANGE_ASSERT(RF230_RX_ON);

						state = STATE_RX_ON;
					}
				}

		}
		else if(cmd == CMD_RECEIVE)
		{
			//AnanthAtSamraksh: defaulting to the AdvancedTimer
			receive_timestamp = HAL_Time_CurrentTicks();
			if(DS_Success==DownloadMessage()){
				//rx_msg_ptr->SetActiveMessageSize(rx_length);

				if(rx_length>  IEEE802_15_4_FRAME_LENGTH){
#ifdef DEBUG_RF231
					hal_printf("Radio Receive Error: Packet too big: %d\r\n",rx_length);
#endif
					return;
				}

				(rx_msg_ptr->GetHeader())->SetLength(rx_length);
				rx_msg_ptr = (Message_15_4_t *) (Radio<Message_15_4_t>::GetMacHandler(active_mac_index)->GetRecieveHandler())(rx_msg_ptr, rx_length);
			}
			else {
#ifdef DEBUG_RF231
				hal_printf("Radio Receive Error: Problem downloading message\r\n");
#endif
			}
			// Check if mac issued a sleep while i was receiving something
			if(sleep_pending)
			{

				// We should be on STATE_RX_ON
				UINT8 reg = ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK;
				//if(state = STATE_RX_ON)
				if(reg == RF230_RX_ON)
				{
					// First move the radio to TRX_OFF
					WriteRegister(RF230_TRX_STATE, RF230_TRX_OFF);

					// Check if state change was successful
					DID_STATE_CHANGE_ASSERT(RF230_TRX_OFF);

					// Setting Slptr moves the radio to sleep state
					SlptrSet();

					state = STATE_SLEEP;
				}

				sleep_pending = FALSE;
			}
		}
	}
#ifdef DEBUG_RF231
	CPU_GPIO_SetPinState((GPIO_PIN)24, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN)24, FALSE);
#endif
}

//template<class T>
DeviceStatus RF231Radio::DownloadMessage()
{
	UINT16 crc;
	INT16 lqi;
	DeviceStatus retStatus = DS_Success;

	UINT32 phy_rssi = ReadRegister(RF230_PHY_RSSI);
	if(!(phy_rssi & (1 << 7))){
		// Auto crc check is failing return false
#ifdef DEBUG_RF231
		hal_printf("dm err phy_rssi: 0x%x\r\n",phy_rssi);
#endif
		retStatus = DS_Fail;
		SelnClear();
		HAL_Time_Sleep_MicroSeconds(50);
	} else {
		INIT_STATE_CHECK();
		UINT8 length;
		UINT8 counter = 0;
		SelnClear();
	

		UINT8* temp_rx_msg_ptr = (UINT8 *) rx_msg_ptr;
		//erase the packet clean before writing anything
		memset(temp_rx_msg_ptr, 0,  IEEE802_15_4_FRAME_LENGTH-2);
	
		CPU_SPI_WriteReadByte(config, RF230_CMD_FRAME_READ);
		length = CPU_SPI_WriteReadByte(config, 0);
		
		if(length-2 >  IEEE802_15_4_FRAME_LENGTH){
#ifdef DEBUG_RF231
			hal_printf("Radio Receive Error: Packet too big: %d\r\n",length);
#endif
			retStatus = DS_Fail;
		}
		else if(length >= 3)
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
		
			// Read the crc byte because we are trying to get to the lqi
			crc = CPU_SPI_WriteReadByte(config, 0);
			crc = CPU_SPI_WriteReadByte(config, 0);

			lqi = CPU_SPI_WriteReadByte(config, 0);
		
			IEEE802_15_4_Metadata_t* metadata = rx_msg_ptr->GetMetaData();
			metadata->SetLqi(lqi);
			metadata->SetReceiveTimeStamp(receive_timestamp);
		}
		rx_length = length - 2;
	}
	SelnSet();

	state = STATE_RX_ON;

	cmd = CMD_NONE;

	return retStatus;
}


extern RF231Radio grf231Radio;
extern RF231Radio grf231RadioLR;

extern "C"
{

	void  __attribute__((optimize("O0"))) Radio_Handler_LR(GPIO_PIN Pin,BOOL PinState, void* Param)
	{
		grf231RadioLR.HandleInterrupt();
	}

	// Call radio_irq_handler from here
	void  __attribute__((optimize("O0"))) Radio_Handler(GPIO_PIN Pin, BOOL PinState, void* Param)
	{
		grf231Radio.HandleInterrupt();
	}
}

