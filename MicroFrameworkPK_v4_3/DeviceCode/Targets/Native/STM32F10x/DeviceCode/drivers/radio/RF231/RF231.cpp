#include "RF231.h"
#include <tinyhal.h>
#include <stm32f10x.h> // TODO. FIX ME. Only needed for interrupt pin check and NOPs. Not platform independant.
#include <Samraksh/MAC/OMAC/DiscoveryHandler.h>

RF231Radio grf231Radio;
RF231Radio grf231RadioLR;

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

//#define NATHAN_DEBUG_JUNK
#ifdef NATHAN_DEBUG_JUNK
static uint64_t send_ts_times[16];
static uint64_t send_times[16];
static volatile uint64_t rx_times[16];
static volatile uint64_t send_dones[16];
static volatile uint64_t rx_start_times[16];
static uint64_t sleep_times[16];
static volatile uint64_t irq_times[16];
static volatile uint64_t irq_causes[16];
static volatile uint32_t size_log[32];
static volatile uint64_t trx_ur_times[16];
static volatile uint64_t download_error_times[16];
static volatile uint64_t int_pend_times[16];

static void add_size(uint32_t size) {
	static int index=0;
	size_log[index] = size;
	index = (++index) % 32;
}

static void add_irq_time(uint32_t cause) {
	static int index=0;
	irq_times[index] = HAL_Time_CurrentTicks();
	irq_causes[index] = cause;
	index = (++index) % 16;
}

static void add_sleep_time() {
	static int index=0;
	sleep_times[index] = HAL_Time_CurrentTicks();
	index = (++index) % 16;
}

static void add_rx_start_time() {
	static int index=0;
	rx_start_times[index] = HAL_Time_CurrentTicks();
	index = (++index) % 16;
}

static void add_send_ts_time() {
	static int index=0;
	send_ts_times[index] = HAL_Time_CurrentTicks();
	index = (++index) % 16;
}

static void add_send_time() {
	static int index=0;
	send_times[index] = HAL_Time_CurrentTicks();
	index = (++index) % 16;
}

static void add_rx_time() {
	static int index=0;
	rx_times[index] = HAL_Time_CurrentTicks();
	index = (++index) % 16;
}

static void add_send_done() {
	static int index=0;
	send_dones[index] = HAL_Time_CurrentTicks();
	index = (++index) % 16;
}

static void add_trx_ur() {
	static int index=0;
	trx_ur_times[index] = HAL_Time_CurrentTicks();
	index = (++index) % 16;
}

static void add_download_error() {
	static int index=0;
	download_error_times[index] = HAL_Time_CurrentTicks();
	index = (++index) % 16;
}
static void add_int_pend() {
	static int index=0;
	int_pend_times[index] = HAL_Time_CurrentTicks();
	index = (++index) % 16;
}
#else
static void add_size(uint32_t size) { }
static void add_irq_time(uint32_t cause) { }
static void add_sleep_time() { }
static void add_rx_start_time() { }
static void add_send_ts_time() { }
static void add_send_time() { }
static void add_rx_time() { }
static void add_send_done() { }
static void add_trx_ur() { }
static void add_download_error() { }
static void add_int_pend() { }
#endif

static inline BOOL isInterrupt()
{
    return ((SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0);
}

void* RF231Radio::Send_Ack(void *msg, UINT16 size, NetOpStatus status) {
	SendAckFuncPtrType AckHandler = Radio<Message_15_4_t>::GetMacHandler(active_mac_index)->GetSendAckHandler();
	(*AckHandler)(msg, size, status);
	return msg;
}


void RF231Radio::Wakeup() {
	INIT_STATE_CHECK();
	GLOBAL_LOCK(irq);
	if (state == STATE_SLEEP) {
		SlptrClear();
		HAL_Time_Sleep_MicroSeconds(380); // Wait for the radio to come out of sleep
		DID_STATE_CHANGE_ASSERT(RF230_TRX_OFF);
		ENABLE_LRR(TRUE);
		state = STATE_TRX_OFF;
	}
}

// For when interrupts are disabled. Checks radio IRQ pin.
BOOL RF231Radio::Interrupt_Pending() {
#ifndef NATHAN_DEBUG_JUNK
	return (EXTI_GetITStatus(kinterrupt % 16) == SET ) ? TRUE : FALSE;
#else
	if (EXTI_GetITStatus(kinterrupt % 16) == SET) {
		add_int_pend();
		return TRUE;
	}
	else {
		return FALSE;
	}
#endif
}

// Not for use with going to BUSY_TX, etc.
// On success, caller must change 'state'
BOOL RF231Radio::Careful_State_Change(uint32_t target) { return Careful_State_Change( (radio_hal_trx_status_t) target ); }
BOOL RF231Radio::Careful_State_Change(radio_hal_trx_status_t target) {

	uint32_t poll_counter=0;
	const uint32_t timeout = 0xFFFF;

	Wakeup();

	GLOBAL_LOCK(irq);

	// current status
	radio_hal_trx_status_t trx_status = (radio_hal_trx_status_t) (ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK);
	radio_hal_trx_status_t orig_status = trx_status;

	if (target == trx_status) { return TRUE; } // already there!

	// Make sure we're not busy and can move.
	if ( trx_status == BUSY_RX || trx_status == BUSY_TX || Interrupt_Pending() )
	{
		return FALSE;
	}

	WriteRegister(RF230_TRX_STATE, target); // do the move

	do{
		trx_status = (radio_hal_trx_status_t) (ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK);
		if( poll_counter == timeout || (trx_status != orig_status \
				&& trx_status != target \
				&& trx_status != RF230_STATE_TRANSITION_IN_PROGRESS) )
			{
				switch(trx_status) {
					case RF230_BUSY_RX: state = STATE_BUSY_RX; break;
					case RF230_BUSY_TX: state = STATE_BUSY_TX; break;
					default: state = STATE_BUSY_RX; ASSERT(0); // Unknown, put in RX state for lack of anything better.
				}
				return FALSE;
			}
		poll_counter++;
	} while(trx_status != target);

	// Check one last time for interrupt.
	// Not clear how this could happen, but assume it would be an RX. --NPS
	if ( Interrupt_Pending() ) { state = STATE_BUSY_RX; return FALSE; }
	
	// Reset cmd here just to be clean.
	if ( trx_status == PLL_ON || trx_status == TRX_OFF )
	{
		cmd = CMD_NONE;
	}

	// We made it!
	return TRUE;
}

void* RF231Radio::Send_TimeStamped(void* msg, UINT16 size, UINT32 eventTime)
{
	UINT32 eventOffset;
	UINT32 timestamp;

	UINT8 lLength;
	UINT8 *timeStampPtr;
	UINT8 *ldata;

	Message_15_4_t* temp;

	const int timestamp_size = 4; // we decrement in a loop later.
	const int crc_size = 2;

	// Adding 2 for crc and 4 bytes for timestamp
	if(size + crc_size + timestamp_size > IEEE802_15_4_FRAME_LENGTH){
		hal_printf("returning send_ack 1\n");
		return Send_Ack(tx_msg_ptr, tx_length, NO_BadPacket);
	}

	if ( !IsInitialized() ) {
		hal_printf("returning send_ack 2\n");
		return Send_Ack(tx_msg_ptr, tx_length, NO_Fail);
	}

	ASSERT ( !isInterrupt() );

	GLOBAL_LOCK(irq);

	add_send_ts_time(); // Debugging. Will remove.

	// Go to TRX_OFF
	if(state == STATE_SLEEP){
		if ( Careful_State_Change(RF230_TRX_OFF) ) {
			state = STATE_TRX_OFF;
		}
		else {
			hal_printf("RF231Radio::Send_TimeStamped returning send_ack 3\n");
			return Send_Ack(tx_msg_ptr, tx_length, NO_Busy);
		}
	}

	// Go to PLL_ON
	if(state == STATE_TRX_OFF || state == STATE_RX_ON){
		if ( Careful_State_Change(RF230_PLL_ON) ) {
			state = STATE_PLL_ON;
		}
		else {
			hal_printf("RF231Radio::Send_TimeStamped returning send_ack 4\n");
			return Send_Ack(tx_msg_ptr, tx_length, NO_Busy);
		}
	}
	else if(state == STATE_BUSY_TX){
		hal_printf("RF231Radio::Send_TimeStamped returning send_ack 5\n");
		return Send_Ack(tx_msg_ptr, tx_length, NO_Busy);
	}
	else {
		hal_printf("*^*^*^*^*^*RF231Radio::Send_TimeStamped state is: %d*^*^*^*^*^*\n", state);
	}

	CPU_GPIO_SetPinState( RF231_TX_TIMESTAMP, TRUE );

	tx_length = size;
	ldata =(UINT8*) msg;

	// Make sure SLP_TR is low before we start.
	SlptrSet();
	HAL_Time_Sleep_MicroSeconds(16);
	SlptrClear();

	SelnClear();

	CPU_SPI_WriteByte(config, RF230_CMD_FRAME_WRITE);

	// Write the size of packet that is sent out
	// Including FCS which is automatically generated and is two bytes
	//Plus 4 bytes for timestamp
	CPU_SPI_ReadWriteByte(config, size + crc_size + timestamp_size);

	lLength = size;
	timeStampPtr = (UINT8 *) &eventOffset;

	add_size(size);

	for(int ii=0; ii<lLength; ii++) {
		CPU_SPI_ReadWriteByte(config, *(ldata++));
	}

	//Transmit the event timestamp
	timestamp = HAL_Time_CurrentTicks() & 0xFFFFFFFF; // Lower bits only
	eventOffset = timestamp - eventTime;

	for(int ii=0; ii<timestamp_size; ii++) {
		CPU_SPI_ReadWriteByte(config, *(timeStampPtr++));
	}

	CPU_SPI_ReadByte(config);

	SelnSet();
	state = STATE_BUSY_TX;

	// exchange bags
	temp = tx_msg_ptr;
	tx_msg_ptr = (Message_15_4_t*) msg;
	cmd = CMD_TRANSMIT;

	CPU_GPIO_SetPinState( RF231_TX_TIMESTAMP, FALSE );

	return temp;
}

DeviceStatus RF231Radio::Reset()
{
	INIT_STATE_CHECK()

	//GLOBAL_LOCK(irq);

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
	DID_STATE_CHANGE_ASSERT(RF230_TRX_OFF);

	HAL_Time_Sleep_MicroSeconds(510);

	WriteRegister(RF230_IRQ_MASK, RF230_IRQ_TRX_UR | RF230_IRQ_TRX_END | RF230_IRQ_RX_START);

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

	// Enable the gpio pin as the interrupt point
	if(this->GetRadioName() == RF231RADIO)
		CPU_GPIO_EnableInputPin(INTERRUPT_PIN, FALSE, Radio_Handler, GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED);
	else if(this->GetRadioName() == RF231RADIOLR)
		CPU_GPIO_EnableInputPin(INTERRUPT_PIN_LR, FALSE, Radio_Handler_LR, GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED);


	SlptrSet();

	// set software state machine state to sleep
	state = STATE_SLEEP;
#	ifdef DEBUG_RF231
	hal_printf("RF231: RESET\r\n");
#	endif

	cmd = CMD_NONE;
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
// Always ends in TRX_OFF or SLEEP states.
DeviceStatus RF231Radio::ChangeTxPower(int power) {

	if ( !Careful_State_Change(TRX_OFF) || !IsInitialized() ) {
		return DS_Fail; // We were busy.
	}

	ASSERT ( !isInterrupt() );

	GLOBAL_LOCK(irq);

	this->tx_power = power  & RF230_TX_PWR_MASK;
	WriteRegister(RF230_PHY_TX_PWR, RF230_TX_AUTO_CRC_ON | (power & RF230_TX_PWR_MASK));

	if (sleep_pending) { Sleep(0); } // I disagree. --NPS

	return DS_Success;
}

// Change the power level of the radio
// Always ends in TRX_OFF or SLEEP states.
DeviceStatus RF231Radio::ChangeChannel(int channel) {

	if ( !Careful_State_Change(TRX_OFF) || !IsInitialized() ) {
		return DS_Fail; // We were busy.
	}

	ASSERT ( !isInterrupt() );

	GLOBAL_LOCK(irq);

	// The value passed as channel until this point is an enum and needs to be offset by 11 to set the
	// actual radio channel value
	this->channel = (channel + RF230_CHANNEL_OFFSET) & RF230_CHANNEL_MASK;
	WriteRegister(RF230_PHY_CC_CCA, RF230_CCA_MODE_VALUE | this->channel);

	if (sleep_pending) { Sleep(0); } // I disagree. --NPS

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
		//hal_printf("state is busy TX. sleep pending to true\n");
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
		hal_printf("radio is busy doing something. sleep pending to true. state is %d; regState is %lu\n", state, regState);
		sleep_pending = TRUE;
	}

	return DS_Success;
}


/*// Nathan's re-write of sleep function
DeviceStatus RF231Radio::Sleep(int level)
{
	// Initiailize state change check variables
	// Primarily used if DID_STATE_CHANGE_ASSERT is used
	INIT_STATE_CHECK();
	UINT32 regState;

	GLOBAL_LOCK(irq);
	sleep_pending = TRUE;
	//irq.Probe();

	////hal_printf("RF231Radio::Sleep: before state:%d\n", state);
	// If we are already in sleep state do nothing
	// Unsure if during sleep we can read registers (No, you can't --NPS).
	if(state == STATE_SLEEP) { return DS_Success; }

	// Queue the sleep request if we think we are busy.
	//if(state == STATE_BUSY_TX || state == STATE_BUSY_RX) { return DS_Success; } // This is now done in Careful_State_Change()

	// Go to TRX_OFF
	if ( Careful_State_Change(RF230_TRX_OFF) ) {
		state = STATE_TRX_OFF;
	}
	else { // If we are busy that's OK, the sleep request is still queued.
		return DS_Success;
	}

	// Read current state of radio to be sure. --Update, I'm actually sure now, but we'll leave it. --NPS
	regState = (ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK);

	// Observe that I am trying, perhaps stupidly, to be clever with fall-through here... --NPS
	switch(regState) {
		case RF230_RX_ON:
		case RF230_PLL_ON:
		case RF230_P_ON:
			WriteRegister(RF230_TRX_STATE, RF230_TRX_OFF);
			DID_STATE_CHANGE_ASSERT(RF230_TRX_OFF);
		case RF230_TRX_OFF:
			ENABLE_LRR(FALSE);
			SlptrSet();
			state = STATE_SLEEP;
			sleep_pending = TRUE;
#			ifdef DEBUG_RF231
			hal_printf("RF231: SLEEP.\r\n");
#			endif
			break;
		default:
			sleep_pending = TRUE; // Only sleep from known states, otherwise just try later. Redundant.
	}

	//NATHAN_SET_DEBUG_GPIO(0);

	//CPU_GPIO_SetPinState( RF231_RADIO_STATEPIN2, FALSE );
	add_sleep_time();

	////hal_printf("RF231Radio::Sleep: after state:%d\n", state);
	return DS_Success;
}*/

typedef struct  {
	UINT32 MSGID;
	char* msgContent;
}Payload_t_ping;

// See RF231 datasheet section 8.3.4
// Return 0 to 28, 3dB steps, from -91 dBm to 10 dBm
INT32 RF231Radio::GetRSSI() {

	if (state != STATE_RX_ON) {
		return 0x7FFFFFFF;
	}

	return ReadRegister(RF230_PHY_RSSI) & 0x1F;
}

void* RF231Radio::Send(void* msg, UINT16 size)
{
	Message_15_4_t* msgBeingSent = (Message_15_4_t*)msg;
	Payload_t_ping* pingPayload = (Payload_t_ping*)msgBeingSent->GetPayload();
	hal_printf(">>>>RF231Radio::Send pingPayload msgId: %d\n", pingPayload->MSGID);
	hal_printf(">>>>RF231Radio::Send pingPayload msgContent: %s\n", pingPayload->msgContent);

	UINT32 eventOffset;
	UINT32 timestamp;

	UINT8 lLength;
	UINT8 *timeStampPtr;
	UINT8 *ldata;

	Message_15_4_t* temp;

	const int crc_size = 2;

	// Adding 2 for crc
	if(size + crc_size> IEEE802_15_4_FRAME_LENGTH){
		hal_printf("returning send_ack 1\n");
		return Send_Ack(tx_msg_ptr, tx_length, NO_BadPacket);
	}

	if ( !IsInitialized() ) {
		hal_printf("returning send_ack 2\n");
		return Send_Ack(tx_msg_ptr, tx_length, NO_Fail);
	}

	ASSERT ( !isInterrupt() );

	GLOBAL_LOCK(irq);

	add_send_time(); // Debugging. Will remove.

	// Go to TRX_OFF
	if(state == STATE_SLEEP){
		if ( Careful_State_Change(RF230_TRX_OFF) ) {
			state = STATE_TRX_OFF;
		}
		else {
			hal_printf("returning send_ack 3\n");
			return Send_Ack(tx_msg_ptr, tx_length, NO_Busy);
		}
	}

	// Go to PLL_ON
	if(state == STATE_TRX_OFF || state == STATE_RX_ON){
		if ( Careful_State_Change(RF230_PLL_ON) ) {
			state = STATE_PLL_ON;
		}
		else {
			hal_printf("returning send_ack 4\n");
			return Send_Ack(tx_msg_ptr, tx_length, NO_Busy);
		}
	}
	else if(state == STATE_BUSY_TX){
		hal_printf("returning send_ack 5\n");
		return Send_Ack(tx_msg_ptr, tx_length, NO_Busy);
	}
	else {
		hal_printf("*^*^*^*^*^*state is: %d*^*^*^*^*^*\n", state);
	}

	CPU_GPIO_SetPinState( RF231_TX, TRUE );

	tx_length = size;
	ldata =(UINT8*) msg;

	// Make sure SLP_TR is low before we start.
	SlptrSet();
	HAL_Time_Sleep_MicroSeconds(16);
	SlptrClear();

	SelnClear();

	CPU_SPI_WriteByte(config, RF230_CMD_FRAME_WRITE);

	// Write the size of packet that is sent out
	// Including FCS which is automatically generated and is two bytes
	//Plus 4 bytes for timestamp
	CPU_SPI_ReadWriteByte(config, size + crc_size);

	lLength = size;

	add_size(size);

	for(int ii=0; ii<lLength; ii++) {
		CPU_SPI_ReadWriteByte(config, *(ldata++));
	}

	CPU_SPI_ReadByte(config);

	SelnSet();
	state = STATE_BUSY_TX;

	/*Payload_t_ping* pingPayloadTxMsgPtr = (Payload_t_ping*)tx_msg_ptr->GetPayload();
	hal_printf(">>>>RF231Radio::Send (before exchange) pingPayload msgId: %d\n", pingPayloadTxMsgPtr->MSGID);
	if(pingPayloadTxMsgPtr->msgContent != NULL){
		hal_printf(">>>>RF231Radio::Send (before exchange) pingPayload msgContent: %s\n", pingPayloadTxMsgPtr->msgContent);
	}
	else{
		hal_printf(">>>>RF231Radio::Send (before exchange) pingPayload msgContent is NULL\n");
	}*/

	// exchange bags
	temp = tx_msg_ptr;
	tx_msg_ptr = (Message_15_4_t*) msg;
	cmd = CMD_TRANSMIT;

	/*pingPayloadTxMsgPtr = (Payload_t_ping*)tx_msg_ptr->GetPayload();
	hal_printf(">>>>RF231Radio::Send (after exchange) pingPayload msgId: %d\n", pingPayloadTxMsgPtr->MSGID);
	hal_printf(">>>>RF231Radio::Send (after exchange) pingPayload msgContent: %s\n", pingPayloadTxMsgPtr->msgContent);*/

	CPU_GPIO_SetPinState( RF231_TX, FALSE );

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

	CPU_GPIO_SetPinState(AMP_PIN_LR, TurnOn);
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
	INIT_STATE_CHECK();

	CPU_GPIO_EnableOutputPin(RF231_RADIO_STATEPIN2, TRUE);
	CPU_GPIO_SetPinState( RF231_RADIO_STATEPIN2, TRUE );
	CPU_GPIO_SetPinState( RF231_RADIO_STATEPIN2, FALSE );

	CPU_GPIO_EnableOutputPin(RF231_TX_INTERRUPT, FALSE);
	CPU_GPIO_EnableOutputPin(RF231_RX, FALSE);
	CPU_GPIO_EnableOutputPin(RF231_TX_TIMESTAMP, FALSE);
	CPU_GPIO_EnableOutputPin(RF231_TX, FALSE);
	CPU_GPIO_EnableOutputPin(RF231_FRAME_BUFF_ACTIVE, FALSE);

	// Set MAC datastructures
	active_mac_index = Radio<Message_15_4_t>::GetMacIdIndex();
	if(Radio<Message_15_4_t>::Initialize(event_handler, mac_id) != DS_Success){
		return DS_Fail;
	}

	//If the radio hardware is not already initialized, initialize it
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
			CPU_GPIO_EnableOutputPin(AMP_PIN_LR, FALSE);
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
		hal_printf("RF231: Radio ID: %d\r\n", tempNum);
#endif
		SetAddress(tempNum);
		SetInitialized(TRUE);

		// Initially point to the driver buffer
		tx_msg_ptr = (Message_15_4_t *) tx_msg;

		// Initially point to driver buffer
		rx_msg_ptr = (Message_15_4_t *) rx_msg;

		GLOBAL_LOCK(irq);
		//for(UINT8 i = 0; i < 30; i++)
			//data[i] = 0;
		if(TRUE != GpioPinInitialize())
		{
			return DS_Fail;
		}
		//configure_exti();

		if(TRUE != SpiInitialize())
		{
			ASSERT_RADIO(0);
		}

		// The radio initialization steps in the following lines are semantically copied from the corresponding tinyos implementation
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
		DID_STATE_CHANGE_ASSERT(RF230_TRX_OFF);

		HAL_Time_Sleep_MicroSeconds(510);

		if(this->GetRadioName() == RF231RADIOLR)
		{
			/* AnanthAtSamraksh - adding below line to fix receive on LR radio extension board - 2/11/2014 */
			// Enable external PA and control from RF231
			rf231_enable_pa_rxtx();
		}


		// Register controls the interrupts that are currently enabled

		WriteRegister(RF230_IRQ_MASK, RF230_IRQ_TRX_UR | RF230_IRQ_TRX_END | RF230_IRQ_RX_START);

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
		// Enable the gpio pin as the interrupt point
		if(this->GetRadioName() == RF231RADIO){
			CPU_GPIO_EnableInputPin(INTERRUPT_PIN, FALSE, Radio_Handler, GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED);
		}
		else if(this->GetRadioName() == RF231RADIOLR){
			CPU_GPIO_EnableInputPin(INTERRUPT_PIN_LR, FALSE, Radio_Handler_LR, GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED);
			//CPU_GPIO_EnableOutputPin(AMP_PIN_LR, FALSE);
			GPIO_ConfigurePin(GPIOB, GPIO_Pin_12, GPIO_Mode_Out_PP, GPIO_Speed_2MHz);
		}

		SlptrSet();
		sleep_pending = TRUE;
		// set software state machine state to sleep
		state = STATE_SLEEP;
		//NATHAN_SET_DEBUG_GPIO(0);
#		ifdef DEBUG_RF231
		hal_printf("RF231: INIT. Default sleep\r\n");
#		endif
		//CPU_GPIO_SetPinState(   RF231_RADIO_STATEPIN2, FALSE );
		// Initialize default radio handlers

		// Added here until the state issues are resolved
		//TurnOn();

		cmd = CMD_NONE;

	}

	return DS_Success;
}


DeviceStatus RF231Radio::UnInitialize()
{
    DeviceStatus ret = DS_Success;
    if(IsInitialized())
    {
        RstnClear();
        SetInitialized(FALSE);
        ASSERT((active_mac_index & 0xFF00) == 0);
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
            CPU_GPIO_DisablePin(AMP_PIN_LR, RESISTOR_DISABLED, GPIO_Mode_IN_FLOATING, GPIO_ALT_PRIMARY);
        }
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
	config.Clock_RateKHz          = 16; // THIS IS IGNORED.
	if(this->GetRadioName() == RF231RADIO)
	{
		config.SPI_mod                = RF231_SPI_BUS;
	}
	else if(this->GetRadioName() == RF231RADIOLR)
	{
		config.SPI_mod 				  = RF231_LR_SPI_BUS;
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

	//CPU_GPIO_SetPinState( RF231_TURN_ON_RX, TRUE );

	////hal_printf("RF231Radio::TurnOnRx: before state:%d\n", state);
	sleep_pending = FALSE;

	// The radio is not sleeping or is already on
	if(state == STATE_RX_ON || state == STATE_BUSY_RX)
	{
		return DS_Success;
	}

	if (state == STATE_BUSY_TX) {
		return DS_Fail; // We are busy
		/* As per my comment, we are BUSY. This is not ok. --NPS
		WriteRegister(RF230_TRX_STATE, RF230_PLL_ON);
		DID_STATE_CHANGE_ASSERT(RF230_PLL_ON);
		state = STATE_PLL_ON;
		*/
	}

	// Wakey wakey
	if (state == STATE_SLEEP) {
		SlptrClear();
		// Wait for the radio to come out of sleep
		HAL_Time_Sleep_MicroSeconds(380);
		DID_STATE_CHANGE_ASSERT(RF230_TRX_OFF);
		ENABLE_LRR(TRUE);
		state = STATE_TRX_OFF;
	}

	if (state == STATE_TRX_OFF || state == STATE_PLL_ON) {
		WriteRegister(RF230_TRX_STATE, RF230_RX_ON);
	}
	else {
#		ifdef DEBUG_RF231
		hal_printf("RF231: ERROR. Radio in funny state. Line %d\r\n", __LINE__);
#		endif
		ASSERT_RADIO(0);
	}

	DID_STATE_CHANGE_ASSERT(RF230_RX_ON);
	state = STATE_RX_ON;

#	ifdef DEBUG_RF231
	hal_printf("RF231: RX_ON\r\n");
#	endif

	//CPU_GPIO_SetPinState( RF231_TURN_ON_RX, FALSE );
	return DS_Success;
}	//RF231Radio::TurnOnRx()


// This function moves the radio from sleep to RX_ON
//template<class T>
/*
DeviceStatus RF231Radio::TurnOnPLL()
{
	INIT_STATE_CHECK();
	GLOBAL_LOCK(irq);

	////hal_printf("RF231Radio::TurnOnPLL: before state:%d\n", state);
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

	if (state == STATE_RX_ON) {
		WriteRegister(RF230_TRX_STATE, RF230_TRX_OFF);
		DID_STATE_CHANGE_ASSERT(RF230_TRX_OFF);
		state = STATE_TRX_OFF;
	}

	DID_STATE_CHANGE_ASSERT(RF230_TRX_OFF);

	// Push radio to pll on state
	if(((ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK)== RF230_RX_ON) || ((ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK) == RF230_TRX_OFF))
	{
		WriteRegister(RF230_TRX_STATE, RF230_PLL_ON);
	    // Wait for radio to go into pll on and return efail if the radio fails to transition
	    DID_STATE_CHANGE_ASSERT(RF230_PLL_ON);
	}

	state = STATE_PLL_ON;
	cmd = CMD_NONE;

	CPU_GPIO_SetPinState( RF231_RADIO_STATEPIN2, TRUE );
	return DS_Success;
}	//RF231Radio::TurnOnPLL()
*/


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
}	//RF231Radio::ClearChannelAssesment


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
	UINT32 irq_cause;
	INT16 temp;
	const UINT8 UNSUPPORTED_INTERRUPTS = TRX_IRQ_4 | TRX_IRQ_5 | TRX_IRQ_BAT_LOW | TRX_IRQ_TRX_UR;
	INIT_STATE_CHECK();

	// I don't want to do a big lock here but the rest of the driver is so ugly... --NPS
	GLOBAL_LOCK(irq);

	irq_cause = ReadRegister(RF230_IRQ_STATUS); // This clears the IRQ as well

	// DEBUG NATHAN
	add_irq_time(irq_cause);
	// DEBUG NATHAN

	// Only consider CMD_TRANSMIT.
	// If this came in on CMD_RECEIVE, we had overlapping RX events
	if( irq_cause & UNSUPPORTED_INTERRUPTS ) {
		add_trx_ur();
		if (cmd == CMD_TRANSMIT) { ASSERT_RADIO(0); }
		//cmd = CMD_NONE; // Thinking about this... --NPS
		// else it was an RX overrun and we live with it.
	}

	// See datasheet section 9.7.5. We handle both of these manually.
	if(irq_cause & TRX_IRQ_PLL_LOCK) {  }
	if(irq_cause & TRX_IRQ_PLL_UNLOCK) {  }

	if(irq_cause & TRX_IRQ_RX_START)
	{
		add_rx_start_time();
		state = STATE_BUSY_RX; // Seems like we should change state, so I made one up...
		//NATHAN_SET_DEBUG_GPIO(0);
#		ifdef DEBUG_RF231
		hal_printf("RF231: TRX_IRQ_RX_START\r\n");
#		endif
		// We got an Recieve frame start when the driver is not doing any thing particular
		// let us process this interrupt then

		temp = ReadRegister(RF230_PHY_RSSI) & RF230_RSSI_MASK;

		// Keeps track of average rssi, why ?? may be useful someday :)
		// How is this an average??? --NPS
		rssi_busy += temp - (rssi_busy >> 2);

		// Add the rssi to the message
		IEEE802_15_4_Metadata_t* metadata = rx_msg_ptr->GetMetaData();
		metadata->SetRssi(temp);

		// Do the time stamp here instead of after done, I think.
		// Note there is potential to use a capture time here, for better accuracy.
		// Currently, this will depend on interrupt latency.
		receive_timestamp = HAL_Time_CurrentTicks();

		// We have a 64 bit local clock, do we need 64 bit timestamping, perhaps not
		// Lets stick to 32, the iris implementation uses the timer to measure when the input was
		// captured giving more accurate timestamping, we are going to rely on this less reliable technique
		//AnanthAtSamraksh: defaulting to the AdvancedTimer

		// This is not being used anywhere right now, so commenting out.
		//time = (HAL_Time_CurrentTicks() >> 32); // Throwing away the bottom 32-bits? Doesn't that really hurt timing??? --NPS

		// Initiate cmd receive
		cmd = CMD_RECEIVE;
		//HAL_Time_Sleep_MicroSeconds(64); // wait 64us to prevent spurious TRX_UR interrupts. // TODO... HELP --NPS
	}

	// The contents of the frame buffer went out (OR we finished a RX --NPS)
	if(irq_cause & TRX_IRQ_TRX_END)
	{
		if(cmd == CMD_TRANSMIT)
		{
			CPU_GPIO_SetPinState( RF231_TX_INTERRUPT, TRUE );
#			ifdef DEBUG_RF231
			hal_printf("RF231: TRX_IRQ_TRX_END : Transmit Done\r\n");
#			endif

			add_send_done();

			state = STATE_PLL_ON;
			// Call radio send done event handler when the send is complete
			SendAckFuncPtrType AckHandler = Radio<Message_15_4_t>::GetMacHandler(active_mac_index)->GetSendAckHandler();
			(*AckHandler)(tx_msg_ptr, tx_length,NO_Success);

			cmd = CMD_NONE;

			if(sleep_pending)
			{
				Sleep(0);
				CPU_GPIO_SetPinState( RF231_TX_INTERRUPT, FALSE );
				return;
			}
			else //
			{
				WriteRegister(RF230_TRX_STATE, RF230_RX_ON);
				DID_STATE_CHANGE_ASSERT(RF230_RX_ON);
				state = STATE_RX_ON;
			}
			CPU_GPIO_SetPinState( RF231_TX_INTERRUPT, FALSE );
		}
		else if(cmd == CMD_RECEIVE)
		{
			Message_15_4_t* rx_msg_ptr_temp;

			CPU_GPIO_SetPinState( RF231_RX, TRUE );
#			ifdef DEBUG_RF231
			hal_printf("RF231: TRX_IRQ_TRX_END : Receive Done\n");
#			endif

			if(state == STATE_PLL_ON || state == STATE_BUSY_TX){
				hal_printf("About to assert\n");
				ASSERT(0);
			}
			state = STATE_RX_ON; // Right out of BUSY_RX

			// Go to PLL_ON at least until the frame buffer is empty
			WriteRegister(RF230_TRX_STATE, RF230_PLL_ON);
			DID_STATE_CHANGE_ASSERT(RF230_PLL_ON);
			state = STATE_PLL_ON;

			if(DS_Success==DownloadMessage()){
				//rx_msg_ptr->SetActiveMessageSize(rx_length);
				if(rx_length>  IEEE802_15_4_FRAME_LENGTH){
#					ifdef DEBUG_RF231
					hal_printf("Radio Receive Error: Packet too big: %d\r\n",rx_length);
#					endif
					hal_printf("Radio Receive Error: Packet too big: %d\r\n",rx_length);
					CPU_GPIO_SetPinState( RF231_RX, FALSE );
					CPU_GPIO_SetPinState( RF231_RX, TRUE );
					CPU_GPIO_SetPinState( RF231_RX, FALSE );
					return;
				}

				// Please note this is kind of a hack.
				// Manually check our interrupts here (since we are locked).
				// If we see a new interrupt, just assume it means we had an RX overrun.
				// In which case we just drop the packet (or packets?) --NPS

				// Un-sure if this is how to drop a packet. --NPS

				if ( !Interrupt_Pending() ) {
					int type = rx_msg_ptr->GetHeader()->type;
					(rx_msg_ptr->GetHeader())->SetLength(rx_length);
					rx_msg_ptr_temp = rx_msg_ptr;
					rx_msg_ptr = (Message_15_4_t *) (Radio<Message_15_4_t>::GetMacHandler(active_mac_index)->GetReceiveHandler())(rx_msg_ptr, rx_length);
				}
			}
			else {
				// download error
				add_download_error();
			}

			cmd = CMD_NONE;

			// Check if mac issued a sleep while i was receiving something
			if(sleep_pending)
			{
				hal_printf("**********About to sleep in RX. State is %d**********\n", state);
				Sleep(0);
				hal_printf("msg contents are: \n");
				DiscoveryMsg_t* discoPayload = (DiscoveryMsg_t*)rx_msg_ptr_temp->GetPayload();
				hal_printf("src: %d\n", rx_msg_ptr_temp->GetHeader()->src);
				hal_printf("dest: %d\n", rx_msg_ptr_temp->GetHeader()->dest);
				hal_printf("disco nodeID: %u\n", discoPayload->nodeID);
				hal_printf("disco nextSeed: %u\n", discoPayload->nextSeed);
				hal_printf("disco seedUpdateIntervalinSlots: %lu\n", discoPayload->seedUpdateIntervalinSlots);
				hal_printf("*****************************************************\n");
				CPU_GPIO_SetPinState( RF231_RX, FALSE );
				CPU_GPIO_SetPinState( RF231_RX, TRUE );
				CPU_GPIO_SetPinState( RF231_RX, FALSE );
				/*if(state == STATE_PLL_ON || state == STATE_BUSY_TX){
					hal_printf("About to assert\n");
					ASSERT(0);
				}*/
				return;
			}
			else { // Now safe to go back to RX_ON
				WriteRegister(RF230_TRX_STATE, RF230_RX_ON);
				DID_STATE_CHANGE_ASSERT(RF230_RX_ON);
				state = STATE_RX_ON;
			}
			CPU_GPIO_SetPinState( RF231_RX, FALSE );
		}
		else { ASSERT(0); } // Unknown CMD
	}

	if(sleep_pending)
	{
		Sleep(0);
	}
}	//RF231Radio::HandleInterrupt()


// Re-writing this crap
DeviceStatus RF231Radio::DownloadMessage()
{
	DeviceStatus retStatus = DS_Success;
	UINT8 phy_rssi = ReadRegister(RF230_PHY_RSSI) & 0x80;
	UINT8 phy_status;
	UINT8 len;
	UINT8 lqi;
	UINT32 cnt = 0;
	UINT8* temp_rx_msg_ptr;

	// Auto-CRC is enabled. This checks the status bit.
	if ( !phy_rssi ) { retStatus = DS_Fail; }

	GLOBAL_LOCK(irq);

	// DEBUG NATHAN
	add_rx_time();
	// DEBUG NATHAN

	temp_rx_msg_ptr = (UINT8 *) rx_msg_ptr;
	memset(temp_rx_msg_ptr, 0,  IEEE802_15_4_FRAME_LENGTH);

	//RF231_240NS_DELAY();
	SelnClear();
	//RF231_240NS_DELAY();

	// phy_status could contain meta data depending on settings.
	// At the moment it does not.
	phy_status = CPU_SPI_WriteReadByte(config, RF230_CMD_FRAME_READ);
	//RF231_240NS_DELAY();

	// next byte is legnth to read including CRC
	len = length = CPU_SPI_WriteReadByte(config, 0);
	//RF231_240NS_DELAY();

	// We don't want to read the two CRC bytes into the packet
	rx_length = len - 2;

	while ( ((len--) -2) > 0) {
		temp_rx_msg_ptr[cnt++] = CPU_SPI_WriteReadByte(config, 0);
		//RF231_240NS_DELAY();
	}

	// Two dummy reads for the CRC bytes
	CPU_SPI_WriteReadByte(config, 0); //RF231_240NS_DELAY();
	CPU_SPI_WriteReadByte(config, 0); //RF231_240NS_DELAY();

	// last, the LQI
	lqi = CPU_SPI_WriteReadByte(config, 0); //RF231_240NS_DELAY();

	SelnSet();

	IEEE802_15_4_Metadata_t* metadata = rx_msg_ptr->GetMetaData();
	metadata->SetLqi(lqi);
	metadata->SetReceiveTimeStamp(receive_timestamp);

	return retStatus;
}

void Radio_Handler_LR(GPIO_PIN Pin,BOOL PinState, void* Param)
{
	grf231RadioLR.HandleInterrupt();
}

void Radio_Handler(GPIO_PIN Pin, BOOL PinState, void* Param)
{
	grf231Radio.HandleInterrupt();
}

