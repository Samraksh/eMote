
#include <tinyhal.h>
#include <Samraksh\Message.h>
#include <string.h>

// Uncomment to disable debug prints and save a little flash space
//#define SI446x_NO_DEBUG_PRINT

#include "si446x.h"
#include "si446x_pin_configs.h"

// Hardware stuff
#include <stm32f10x.h>

#ifndef RADIO_POWER_PAD_MS
#define RADIO_POWER_PAD_MS 0
#endif

// Automatically moves the radio to RX after a transmit instead of sleeping.
// Comment out below line to disable and use default ('0')
#define SI446x_TX_DONE_STATE (SI_STATE_RX<<4)
// Default value for above, radio returns to previous state.
#ifndef SI446x_TX_DONE_STATE
#define SI446x_TX_DONE_STATE (SI_STATE_SPI_ACTIVE<<4)
#endif

#define SI446x_RX_DONE_STATE SI_STATE_RX
// Do NOT to back to RX after sending an ACK.
#define SI446x_TX_ACK_DONE_STATE (SI_STATE_SPI_ACTIVE<<4)

//#define SI446x_INT_MODE_CHECK_DO
// comment out above to disable check
#ifdef SI446x_INT_MODE_CHECK_DO
#define SI446x_INT_MODE_CHECK() \
	if (((SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0)) { \
		si446x_debug_print(ERR99, "SI446X: %s() WARNING, CALLED IN INTERRUPT!\r\n", __func__); \
	}
#else
#define SI446x_INT_MODE_CHECK() {}
#endif

si_state_t defaultRxDoneState = SI_STATE_SLEEP;

//#define SI446X_DEBUG_UNFINISHED_PKT // remove me.

enum { SI_DUMMY=0, };

// SETS SI446X PRINTF DEBUG VERBOSITY
const unsigned si4468x_debug_level = DEBUG03; // CHANGE ME.

// Pin list used in setup.
static const SI446X_pin_setup_t *SI446X_pin_setup = NULL;

// CORE LOCKING STUFF, MOVE ME LATER
static volatile uint32_t spi_lock;
static volatile uint32_t radio_lock;

// static prototypes
static int convert_rssi(uint8_t x);

// CMSIS includes these functions in some form
// But they didn't quite work for me and I didn't want to mod library so... --NPS
static uint32_t ___LOAD(volatile uint32_t *addr) __attribute__ ((naked));
static uint32_t ___STORE(uint32_t value, volatile uint32_t *addr) __attribute__ ((naked));

static uint32_t ___LOAD(volatile uint32_t *addr)
{
  __ASM("ldrex r0, [r0]");
  __ASM("bx lr");
}

static uint32_t ___STORE(uint32_t value, volatile uint32_t *addr)
{
  __ASM("strex r0, r0, [r1]");
  __ASM("bx lr");
}

// Returns true if lock aquired
static bool get_lock_inner(volatile uint32_t *Lock_Variable, uint32_t id) {
	int status;
	__DMB();
	if (___LOAD(Lock_Variable) != radio_lock_none) {
		__ASM("clrex"); // TODO: AM I NEEDED?
		return false;
	}
	status = ___STORE(id, Lock_Variable);
	__DMB();

	return (status == 0);
}

// Should try more than once, can legit fail even if lock is free.
// Example, will never succeed if any interrupt hits in between.
// ldrex-strex only guarantees that lock is free when it says so, but NOT the inverse.
static uint32_t get_lock(volatile uint32_t *Lock_Variable, uint32_t id) {
	int attempts=si446x_lock_max_attempts;
	do {
		if ( get_lock_inner(Lock_Variable, id) )
			return 0;
	} while (--attempts);
	return *Lock_Variable; // return who we think the blocking owner is. NOT GUARANTEED TO BE RIGHT.
}

// TODO: Add ownership check --NPS
static void free_lock(volatile uint32_t *Lock_Variable) {
	__DMB();
	*Lock_Variable = radio_lock_none;
	__DMB();
	return;
}

// Guarantee a lock grab. Requires IRQ mask.
// Returns previous lock owner.
// ONLY FOR USE IN VERY SPECIFIC CASES
static radio_lock_id_t si446x_radio_lock_nofail(radio_lock_id_t id) {
	radio_lock_id_t ret;
	GLOBAL_LOCK(irq);
	__DMB();
	ret = (radio_lock_id_t) radio_lock;
	radio_lock = id;
	__DMB();
	return ret;
}

// Conditional lock set. If and only if lock owner is "cond", progress to "target".
// IRQ mask is used due to a) gives guarantee and b) cannot false-negative and c) chain-of-custody
// Returns: true if 'cond' was met.
// ONLY FOR USE IN VERY SPECIFIC CASES
static bool si446x_radio_lock_if_then_nofail(radio_lock_id_t cond, radio_lock_id_t target) {
	GLOBAL_LOCK(irq);
	__DMB();
	if (radio_lock == cond) {
		radio_lock = target;
		__DMB();
		return true;
	} else {
		return false;
	}
}

// Returns current owner if fail, 0 if success
static radio_lock_id_t si446x_spi_lock(radio_lock_id_t id) {
	return (radio_lock_id_t) get_lock(&spi_lock, (uint32_t)id);
}

static int si446x_spi_unlock() {
	free_lock(&spi_lock);
}

// Returns current owner if fail, 0 if success
static radio_lock_id_t si446x_radio_lock(radio_lock_id_t id) {
	return (radio_lock_id_t) get_lock(&radio_lock, (uint32_t)id);
}

static int si446x_radio_unlock(void) {
	free_lock(&radio_lock);
}
// END LOCKING STUFF

#ifndef SI446x_NO_DEBUG_PRINT
static void si446x_debug_print(int priority, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    if(priority >= si4468x_debug_level) {
		hal_vprintf(fmt, args);
	}

    va_end(args);
}
#else
// Compiler is smart enough to toss out debug strings even in debug mode
//static void si446x_debug_print(int priority, const char *fmt, ...) { return; }
#define si446x_debug_print(...) ((void)0)
#define si446x_set_debug_print(...) ((void)0)
#endif

static void si446x_spi2_handle_interrupt(GPIO_PIN Pin, BOOL PinState, void* Param);

static si446x_tx_callback_t tx_callback;
static si446x_rx_callback_t rx_callback;

static HAL_CONTINUATION tx_callback_continuation;
static HAL_CONTINUATION rx_callback_continuation;
static HAL_CONTINUATION int_defer_continuation;
static HAL_CONTINUATION state_callback_continuation;

static radio_state_t si446x_next_state = STATE_NONE;
#ifdef _DEBUG
static UINT64 state_tr_time;
#endif

static unsigned isInit = 0;
static int si446x_channel = 0;
static unsigned tx_power = 0;
static volatile UINT64 rx_timestamp;

// Radio PAL stuff
static Radio<Message_15_4_t> radio_si446x_spi2;
static UINT16 active_mac_index; // For some reason this is tied to HAL ... suspect should be in PAL object
static Message_15_4_t tx_msg; // SI_DUMMY message to handle stupid PAL. FIX ME.
static Message_15_4_t* tx_msg_ptr;
static Message_15_4_t rx_msg;
static Message_15_4_t* rx_msg_ptr;
// END RADIO PALL STUFF

// In case we had to defer interrupt handling for any reason...
static void int_cont_do(void *arg) {
	si446x_debug_print(DEBUG02,"SI446X: int_cont_do()\r\n");
	SI446x_INT_MODE_CHECK();
	si446x_spi2_handle_interrupt( SI446X_pin_setup->nirq_mf_pin, false, NULL );
}

// Gross
static void state_changed_handler_wrapper(UINT8 radio, radio_state_t st) {
	RadioEventHandler* ev_handler_ptr;
	RadioStateChangeFuncPtrType funcptr;

	ev_handler_ptr = radio_si446x_spi2.GetMacHandler(active_mac_index);
	if (ev_handler_ptr == NULL) goto OUT;
	funcptr = ev_handler_ptr->GetStateChangeHandler();
	if (funcptr == NULL) goto OUT;
	(*funcptr)(radio, st); // push the error up
OUT:
#ifdef NETMF_RADIO_DEBUG
	si446x_debug_print(DEBUG03,"New Radio State: %s\r\n", rs_tostring(st));
#endif
	return;
}

// Called when power is restored to the radio
static void state_cont_do(void *arg) {
	DeviceStatus ret;
	radio_state_t next;

	if (si446x_next_state == STATE_NONE) return; // nobody listening
	next = si446x_next_state; // shadow
	si446x_next_state = STATE_POWER_FAIL;	// If this hits again, its because power failed
	si446x_debug_print(DEBUG02,"%s()\r\n", __func__);
#ifdef _DEBUG
	if (next == STATE_SLEEP) {
		UINT64 tr_time = HAL_Time_CurrentTicks() - state_tr_time;
		state_tr_time = 0;
		si446x_debug_print(DEBUG03,"%s() Took %d ms\r\n", __func__, HAL_Time_TicksToMicroseconds(tr_time)/1000);
	}
#endif
	// Must handle this first.
	if (next == STATE_POWER_FAIL) {
		si446x_debug_print(ERR99,"%si446x: Power Blinked!!! Reset recommended.\r\n", __func__);
		state_changed_handler_wrapper(THIS_RADIO, STATE_POWER_FAIL);
		si446x_inform_state(SI_STATE_POWER_FAIL);
		si446x_next_state = STATE_NONE;
		return;
	}

	// Assume we are coming up from power off
	// If more state changes are added, this will no longer be true.
	HAL_Time_Sleep_MicroSeconds(RADIO_POWER_PAD_MS*1000);
	ret = si446x_reinit();

	if (ret != DS_Success) {
		si446x_debug_print(ERR99,"%s() Failed reinit()\r\n", __func__);
		state_changed_handler_wrapper(THIS_RADIO, STATE_ERROR);
		return;
	}
	if (next == STATE_SLEEP) { // Woke up from OFF, go to SLEEP
		ret = si446x_hal_sleep(0);
		if (ret == DS_Success) {
			state_changed_handler_wrapper(THIS_RADIO, STATE_SLEEP);
		} else {
			si446x_debug_print(ERR99,"%s() Failed sleep()\r\n", __func__);
			state_changed_handler_wrapper(THIS_RADIO, STATE_ERROR);
		}
		return;
	}
}

static void sendSoftwareAck(UINT16 dest){
	//CPU_GPIO_SetPinState(DATARX_SEND_SW_ACK, TRUE);
	si446x_debug_print(DEBUG01,"SI446X: sendSoftwareAck\r\n");
	static int i = 0;
	static softwareACKHeader softwareAckHeader;
	if(i == 0){
		softwareAckHeader.src = radio_si446x_spi2.GetAddress();
		softwareAckHeader.payloadType = MFM_OMAC_DATA_ACK;
		i++;
	}
	softwareAckHeader.dest = dest;
	si446x_packet_send(si446x_channel, (uint8_t *) &softwareAckHeader, sizeof(softwareACKHeader), 0, NO_TIMESTAMP, SI446x_TX_ACK_DONE_STATE);
	//CPU_GPIO_SetPinState(DATARX_SEND_SW_ACK, FALSE);
}

// I agree its questionable that I'm being too complicated with continuation stuff... --NPS.
// TX CALLBACK
static void tx_cont_do(void *arg) {
	if (tx_callback != NULL)
		tx_callback();

	SI446x_INT_MODE_CHECK();

	si446x_debug_print(DEBUG02,"SI446X: tx_cont_do()\r\n");

	SendAckFuncPtrType AckHandler = radio_si446x_spi2.GetMacHandler(active_mac_index)->GetSendAckHandler();
	(*AckHandler)(tx_msg_ptr, si446x_packet_size, NetworkOperations_Success, SI_DUMMY);

	// only unlock if TX was the source. Could overlap with RX, which overrides.
	si446x_radio_lock_if_then_nofail(radio_lock_tx, radio_lock_none);
	//CPU_GPIO_SetPinState( SI4468_HANDLE_INTERRUPT_TX, TRUE );
}

// Returns true if a continuation is linked and needs service.
static bool cont_busy(void) {
	return (tx_callback_continuation.IsLinked() || rx_callback_continuation.IsLinked() || int_defer_continuation.IsLinked());
}

static void rx_cont_do(void *arg) {
	uint8_t rx_pkt[si446x_packet_size];
	int size;
	radio_lock_id_t owner;
	uint8_t rssi;
	int16_t freq_error;

	si446x_debug_print(DEBUG02,"SI446X: rx_cont_do()\r\n");

	SI446x_INT_MODE_CHECK();

	if ( owner = si446x_spi_lock(radio_lock_rx) ) {
		si446x_debug_print(DEBUG02,"SI446X: rx_cont_do(): Radio busy, SPI: %s\r\n", print_lock(owner));
		rx_callback_continuation.Enqueue();
		return;
	}

	// For fun, double check to make sure RX holds the radio lock.
	if ( (owner = si446x_radio_lock(radio_lock_rx)) != radio_lock_rx ) {
		si446x_debug_print(ERR99,"SI446X: rx_cont_do(): Warning. RX without RX lock, owner was %s\r\n", print_lock(owner));
	}

	si446x_request_device_state(); // Don't need here, but want to refresh internal state.
	size = si446x_get_packet_info(0,0,0);
	/*if(size == sizeof(softwareACKHeader)){
		si446x_read_rx_fifo(size, rx_pkt);

		si446x_fifo_info(0x3); // Defensively reset FIFO
		si446x_radio_unlock();
		si446x_spi_unlock();

		memcpy( (uint8_t *)rx_msg_ptr, rx_pkt, size );
		rx_msg_ptr = (Message_15_4_t *) (radio_si446x_spi2.GetMacHandler(active_mac_index)->GetReceiveHandler())(rx_msg_ptr, size);

		return;
	}*/
	if (size > si446x_packet_size){
		si446x_debug_print(ERR99, "SI446X: incorrect size in rx_cont_do\r\n");

		si446x_get_modem_status( 0xFF ); // Refresh RSSI

		rssi = si446x_get_latched_rssi();
		freq_error = si446x_get_afc_info();


		si446x_fifo_info(0x3); // Defensively reset FIFO
		si446x_change_state(defaultRxDoneState); // All done, sleep.

		si446x_radio_unlock();
		si446x_spi_unlock();

		si446x_debug_print(ERR99,"SI446X: incorrect size in rx_cont_do:  Pkt RSSI: %d dBm Freq_Error: %d Hz\r\n", convert_rssi(rssi), freq_error);

		//CPU_GPIO_SetPinState( SI4468_MEASURE_RX_TIME, FALSE );
		return;
	}

	si446x_read_rx_fifo(size, rx_pkt);

	if (rx_callback != NULL)
		rx_callback(rx_timestamp, size, rx_pkt);

	si446x_get_modem_status( 0xFF ); // Refresh RSSI

	rssi = si446x_get_latched_rssi();
	freq_error = si446x_get_afc_info();

	si446x_fifo_info(0x3); // Defensively reset FIFO
	si446x_change_state(defaultRxDoneState); // All done, sleep.

	si446x_radio_unlock();
	si446x_spi_unlock();

	si446x_debug_print(DEBUG02,"SI446X: rx_cont_do(): Pkt RSSI: %d dBm Freq_Error: %d Hz\r\n", convert_rssi(rssi), freq_error);

	if (rx_msg_ptr == NULL) return; // Nothing left to do.

	// Metadata struct
	IEEE802_15_4_Metadata_t* metadata = rx_msg_ptr->GetMetaData();

	// Copy packet to MAC packet
	memcpy( (uint8_t *)rx_msg_ptr, rx_pkt, size );

	// Set Metadata
	metadata->SetRssi( convert_rssi(rssi) );
	metadata->SetLqi(0);  // No meaning on this radio
	metadata->SetReceiveTimeStamp((INT64)rx_timestamp);

	IEEE802_15_4_Header_t* header = rx_msg_ptr->GetHeader();
	int currentPayloadType = header->payloadType;

	//Send ack from radio itself.
//	if(__SI4468_SOFTWARE_ACK__){
//		if(currentPayloadType == MFM_OMAC_TIMESYNCREQ || currentPayloadType == MFM_DATA || currentPayloadType <= TYPE31){
//			if(currentPayloadType == MFM_DATA){
//				si446x_debug_print(DEBUG01, "rx_cont_do; sendSoftwareAck to %d; currentPayloadType: %d\n", header->src, currentPayloadType);
//			}
//			sendSoftwareAck(header->src);
//		}
//	}

	// I guess this swaps rx_msg_ptr as well???
	//(rx_msg_ptr->GetHeader())->SetLength(size);
	header->length = size; // the "new" way. blarg.

	rx_msg_ptr = (Message_15_4_t *) (radio_si446x_spi2.GetMacHandler(active_mac_index)->GetReceiveHandler())(rx_msg_ptr, header->length);

	//CPU_GPIO_SetPinState( SI4468_HANDLE_INTERRUPT_RX, TRUE );
	//CPU_GPIO_SetPinState( SI4468_MEASURE_RX_TIME, FALSE );
}

void si446x_hal_register_tx_callback(si446x_tx_callback_t callback) {
	tx_callback = callback;
}

void si446x_hal_unregister_tx_callback() {
	tx_callback = NULL;
}

void si446x_hal_register_rx_callback(si446x_rx_callback_t callback) {
	rx_callback = callback;
}

void si446x_hal_unregister_rx_callback() {
	rx_callback = NULL;
}
// END RX CALLBACK

// This somehow gets put in the radio function. Out of scope for now, but fix me later.
static void GetCPUSerial(uint8_t * ptr, unsigned num_of_bytes ){
	unsigned Device_Serial0;unsigned Device_Serial1; unsigned Device_Serial2;
	Device_Serial0 = *(unsigned*)(0x1FFFF7E8);
	Device_Serial1 = *(unsigned*)(0x1FFFF7EC);
	Device_Serial2 = *(unsigned*)(0x1FFFF7F0);
	if(num_of_bytes==12){
	    ptr[0] = (uint8_t)(Device_Serial0 & 0x000000FF);
	    ptr[1] = (uint8_t)((Device_Serial0 & 0x0000FF00) >> 8);
	    ptr[2] = (uint8_t)((Device_Serial0 & 0x00FF0000) >> 16);
	    ptr[3] = (uint8_t)((Device_Serial0 & 0xFF000000) >> 24);

	    ptr[4] = (uint8_t)(Device_Serial1 & 0x000000FF);
	    ptr[5] = (uint8_t)((Device_Serial1 & 0x0000FF00) >> 8);
	    ptr[6] = (uint8_t)((Device_Serial1 & 0x00FF0000) >> 16);
	    ptr[7] = (uint8_t)((Device_Serial1 & 0xFF000000) >> 24);

	    ptr[8] = (uint8_t)(Device_Serial2 & 0x000000FF);
	    ptr[9] = (uint8_t)((Device_Serial2 & 0x0000FF00) >> 8);
	    ptr[10] = (uint8_t)((Device_Serial2 & 0x00FF0000) >> 16);
	    ptr[11] = (uint8_t)((Device_Serial2 & 0xFF000000) >> 24);
	}
}

// Temporary hack.
static unsigned get_APB1_clock() {
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	return RCC_Clocks.PCLK1_Frequency;
}

static unsigned get_APB2_clock() {
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	return RCC_Clocks.PCLK2_Frequency;
}

static void init_radio_SPI() {
	GPIO_InitTypeDef GPIO_InitStructure;
	unsigned int baud;
	unsigned SpiBusClock;
	SPI_InitTypeDef SPI_InitStruct;

	const SI446X_pin_setup_t *config = SI446X_pin_setup;

	RCC_APB1PeriphClockCmd(config->spi_rcc,	ENABLE);
	SPI_I2S_DeInit(config->spi_base);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin =  config->sclk_pin | config->mosi_pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(config->spi_port, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = config->miso_pin;
	GPIO_Init(config->spi_port, &GPIO_InitStructure);

	SPI_StructInit(&SPI_InitStruct);
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CRCPolynomial = 7;

	// FIX ME, TEMP
	if (config->spi_base != SPI1)
		SpiBusClock = get_APB1_clock();
	else
		SpiBusClock = get_APB2_clock();

	switch(SPI_InitStruct.SPI_BaudRatePrescaler) {
		case SPI_BaudRatePrescaler_2:  baud = SpiBusClock / 2  / 1000; break;
		case SPI_BaudRatePrescaler_4:  baud = SpiBusClock / 4  / 1000; break;
		case SPI_BaudRatePrescaler_8:  baud = SpiBusClock / 8  / 1000; break;
		case SPI_BaudRatePrescaler_16: baud = SpiBusClock / 16 / 1000; break;
		default: baud = 0;
	}

	SPI_Init(config->spi_base, &SPI_InitStruct);
	SPI_Cmd(config->spi_base, ENABLE);

	if (config->spi_base == SPI2) {
		si446x_debug_print(DEBUG02,"SPI2 up CPOL: %d CPHA: %d Baud: %d kHz (%d kHz bus)\r\n",
			SPI_InitStruct.SPI_CPOL, SPI_InitStruct.SPI_CPHA, baud, SpiBusClock/1000);
	}
	else {
		si446x_debug_print(DEBUG02,"SPI??? up CPOL: %d CPHA: %d Baud: %d kHz (%d kHz bus)\r\n",
			SPI_InitStruct.SPI_CPOL, SPI_InitStruct.SPI_CPHA, baud, SpiBusClock/1000);
	}
}

static void init_si446x_pins() {
	GPIO_InitTypeDef GPIO_InitStructure;

	const SI446X_pin_setup_t *config = SI446X_pin_setup;

	if (config->sdn_port != NULL) {
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Pin =  config->sdn_pin;
		GPIO_Init(config->sdn_port, &GPIO_InitStructure);
	}

	// GPIO 0
	if (config->gpio0_port != NULL) {
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Pin = config->gpio0_pin;
		GPIO_Init(config->gpio0_port, &GPIO_InitStructure);
	}

	// GPIO 1
	if (config->gpio1_port != NULL) {
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Pin = config->gpio1_pin;
		GPIO_Init(config->gpio1_port, &GPIO_InitStructure);
	}

	// NIRQ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = config->nirq_pin;
	GPIO_Init(config->nirq_port, &GPIO_InitStructure);

	// PA4 SPI chip select
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = config->cs_pin;
	GPIO_WriteBit(config->cs_port, config->cs_pin, Bit_SET); // Set
	GPIO_Init(config->cs_port, &GPIO_InitStructure);
}

// TODO: Pass control struct with function pointers instead of direct linking
void radio_spi_sel_assert() {
	GPIO_WriteBit(SI446X_pin_setup->cs_port, SI446X_pin_setup->cs_pin, Bit_RESET); // chip select
	__NOP();
}

void radio_spi_sel_no_assert() {
	GPIO_WriteBit(SI446X_pin_setup->cs_port, SI446X_pin_setup->cs_pin, Bit_SET); // chip select
}

uint8_t radio_spi_go(uint8_t data) {
	while( SPI_I2S_GetFlagStatus(SI446X_pin_setup->spi_base, SPI_I2S_FLAG_TXE) == RESET ) ; // spin
	SPI_I2S_SendData(SI446X_pin_setup->spi_base, data);
	while( SPI_I2S_GetFlagStatus(SI446X_pin_setup->spi_base, SPI_I2S_FLAG_RXNE) == RESET ) ; // spin
	return SPI_I2S_ReceiveData(SI446X_pin_setup->spi_base);
}

void radio_shutdown(int go) {
	if (go) // turn off the radio
		GPIO_WriteBit(SI446X_pin_setup->sdn_port, SI446X_pin_setup->sdn_pin, Bit_SET);
	else
		GPIO_WriteBit(SI446X_pin_setup->sdn_port, SI446X_pin_setup->sdn_pin, Bit_RESET);
}

// Returns TRUE if IRQ is asserted
bool radio_get_assert_irq() {
	//return !(GPIO_ReadInputDataBit(SI446X_pin_setup.nirq_port, SI446X_pin_setup.nirq_pin));
	return false;
}

static int convert_rssi(uint8_t x) {
	return x/2 - 0x40 - 70;
}

static bool is_radio_asleep(void) {
	return (si446x_request_device_state_shadow() == SI_STATE_SLEEP);
}

// This is a WLN specific feature and a no-op otherwise
static void set_radio_power_pwm(int go) {
#if defined(PLATFORM_ARM_WLN)
	if (go)
		GPIO_WriteBit(GPIOB, GPIO_Pin_9, Bit_SET);
	else
		GPIO_WriteBit(GPIOB, GPIO_Pin_9, Bit_RESET);
#else
	return;
#endif
}

// Only support Austere and WLN. "WLN Half board" and DOT pin configs were prototype only.
// If you want to use an older (non-platform) pin config, hack it in here.
static int pin_config(void) {
#if defined(PLATFORM_ARM_AUSTERE)
	if (SI446X_pin_setup == NULL) {
		SI446X_pin_setup = &si446x_pin_config_austere;
		si446x_debug_print(DEBUG03, "SI446X: Using Austere V1 Hardware Config\r\n");
	}
	return RADIO_PLATFORM_AUSTERE;
#elif defined(PLATFORM_ARM_WLN)
	if (SI446X_pin_setup == NULL) {
		SI446X_pin_setup = &si446x_pin_config_wln;
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		set_radio_power_pwm(0);
		si446x_debug_print(DEBUG03, "SI446X: Using WWF Hardware Config\r\n");
	}
	return RADIO_PLATFORM_WLN;
#else
#error "SI446X: Unknown pin configuration"
	//si446x_debug_print(ERR99, "SI446X: Unknown hardware platform, pin config will probably fail\r\n");
	return RADIO_PLATFORM_UNKNOWN;
#endif
}

radio_state_t si446x_hal_get_state() {
	si_state_t state = si446x_request_device_state_shadow();
	//si446x_debug_print(DEBUG02,"%s\r\n", __func__);

	// If the radio is available, we can ask it the state directly.
	if (state != SI_STATE_OFF_NO_INIT && state != SI_STATE_OFF && state != SI_STATE_SLEEP && \
		state != SI_STATE_START && state != SI_STATE_BOOT && state != SI_STATE_POWER_FAIL) {
		// It is safe to poll the radio as long as the spi-bus is free
		if ( !si446x_spi_lock(radio_lock_state) ) {
			state = si446x_request_device_state();
			si446x_spi_unlock();
		}
	}

	switch(state) {
		case SI_STATE_OFF_NO_INIT: return STATE_OFF_NO_INIT;
		case SI_STATE_OFF: 		return STATE_OFF;
		case SI_STATE_SLEEP: 	return STATE_SLEEP;
		case SI_STATE_SPI_ACTIVE:
		case SI_STATE_READY:
		case SI_STATE_READY2: 	return STATE_IDLE;
		case SI_STATE_TX_TUNE:
		case SI_STATE_TX: 		return STATE_TX;
		case SI_STATE_RX_TUNE:
		case SI_STATE_RX: 		return STATE_RX;
		case SI_STATE_ERROR:
		case SI_STATE_UNKNOWN: 	return STATE_ERROR;
		case SI_STATE_BOOT: 	return STATE_BUSY;
		case SI_STATE_START:	return STATE_START;
		case SI_STATE_POWER_FAIL: return STATE_POWER_FAIL;
		default: 				return STATE_BUSY;
	}
}

static int radio_turn_on_internal(const int t_max) {
	int stat;
	int timeout = t_max; // 30 seconds

	stat = platform_radio_pwr_ctrl(TURN_RADIO_ON, THIS_RADIO);
	if (stat) return stat; // returns -1 if fail
	stat = platform_power_radio_status(THIS_RADIO);
	if (stat)
	si446x_debug_print(DEBUG02,"%s(): Charging...\r\n", __func__);
	while (stat == 0 && timeout != 0) {
		HAL_Time_Sleep_MicroSeconds(100000); // 100ms chunks
		timeout--;
		stat = platform_power_radio_status(THIS_RADIO);
		si446x_debug_print(DEBUG02,"+");
	}
	si446x_debug_print(DEBUG02,"\r\n");
	if (timeout != 0) HAL_Time_Sleep_MicroSeconds(RADIO_POWER_PAD_MS*1000); // Padding. Doesn't seem fully stable when indicated.
	return t_max-timeout;
}

// return 0 if charging started
static int radio_turn_on_internal_no_block(void) {
	int stat;
	stat = platform_radio_pwr_ctrl(TURN_RADIO_ON, THIS_RADIO);
	if (stat) return stat;
	si446x_debug_print(DEBUG02,"%s() Non-blocking Charge Started...\r\n", __func__);
	return stat;
}

DeviceStatus si446x_hal_set_state(radio_state_t next) {
	radio_lock_id_t owner;
	const int t_max = 300; // 30 seconds
	radio_state_t curr = si446x_hal_get_state();
	si446x_debug_print(DEBUG03,"%s() Curr=%s next=%s\r\n", __func__, rs_tostring(curr), rs_tostring(next));

	if (curr == next) return DS_Success;

	// BUSY is assumed to be a temporary state. Try again later.
	if (curr == STATE_BUSY) {
		si446x_debug_print(DEBUG03,"%s() Radio is BUSY, cannot change state\r\n", __func__);
		return DS_Busy;
	}

	// If power has failed, cannot do anything until after reset
	if (curr == STATE_POWER_FAIL) {
		si446x_debug_print(DEBUG03,"%s() Radio is POWER_FAIL, cannot change state\r\n", __func__);
		return DS_Fail;
	}

	// SLEEP --> RX
	if (curr == STATE_SLEEP && next == STATE_RX) {
		return si446x_hal_rx(0);
	}

	// RX --> SLEEP
	if (curr == STATE_RX && next == STATE_SLEEP) {
		return si446x_hal_sleep(0);
	}

	// IDLE --> SLEEP
	if (curr == STATE_IDLE && next == STATE_SLEEP) {
		return si446x_hal_sleep(0);
	}

	// OFF --> SLEEP
	if (curr == STATE_OFF && next == STATE_SLEEP) {
		int ret;
		DeviceStatus stat;
		if (!isInit) {
			si446x_debug_print(ERR99,"%s() Error, must init() first", __func__);
			return DS_Fail;
		}
		ret = radio_turn_on_internal_no_block();
		if (ret) {
			si446x_debug_print(ERR100,"%s() Power-up cmd error\r\n", __func__);
			return DS_Fail;
		}
#ifdef _DEBUG
		state_tr_time = HAL_Time_CurrentTicks();
#endif
		si446x_next_state = STATE_SLEEP; // Driver will target this state after power-up
		si446x_inform_state(SI_STATE_BOOT);
		// Check status now
		ret = platform_power_radio_status(THIS_RADIO);
		if (ret > 0) {
			state_callback_continuation.Enqueue(); // Ready now, interrupt may not fire, queue immediately.
		}
		return DS_Success; // Continuation will fire when power is ready
	}

	// OFF --> START
	// Special case. Must be coming from OFF. Will block until ready.
	// May or may not already be init()
	// We are confirmed OFF so no need to check for locks
	if ((curr == STATE_OFF || curr == STATE_OFF_NO_INIT) && next == STATE_START) {
		int time = radio_turn_on_internal(t_max);
		if (time == t_max || time < 0) {
			si446x_debug_print(ERR100,"%s(): STATE_START fatal timeout or error\r\n", __func__);
			return DS_Fail;
		} else { // Power indicated as good
			si446x_radio_unlock(); // In case was locked from previous sleep
			si446x_debug_print(DEBUG03,"%s() STATE_START took %d ms\r\n", __func__, (time)*100);
			if (isInit) return si446x_reinit();
			else {
				si446x_inform_state(SI_STATE_START);
				return DS_Success;
			}
		}
	}

	// ANY --> OFF (other than START, which is above)
	// Turn off the radio and its power supply. Checks locks so won't interrpt an op.
	if (next == STATE_OFF) {
		// Get both locks
		if ( owner = si446x_spi_lock(radio_lock_power) ) 	{
			si446x_debug_print(DEBUG02, "%s(): Fail. SPI locked by %s.\r\n", __func__, print_lock(owner));
			return DS_Busy;
		}

		if ( owner = si446x_radio_lock(radio_lock_power) ) 	{
			si446x_debug_print(DEBUG02, "%s(): Fail. Radio locked by %s\r\n", __func__, print_lock(owner));
			si446x_spi_unlock();
			return DS_Busy;
		}
		radio_shutdown(1);
		HAL_Time_Sleep_MicroSeconds(20);
		platform_radio_pwr_ctrl(TURN_RADIO_OFF, THIS_RADIO);
		si446x_inform_state(SI_STATE_OFF);
		//si446x_radio_unlock(); // Unlock it after we re-init
		si446x_spi_unlock();
		return DS_Success;
	}

	si446x_debug_print(ERR99,"%s(): error, state transition not (yet) defined!\r\n", __func__);
	return DS_Fail;
}

// Only called when BOTH
// a) Has been initialized once then powered off
// b) Power is back on again
DeviceStatus si446x_reinit(void) {
	int reset_errors;
	uint8_t temp;
	DeviceStatus ret = DS_Success;
	radio_lock_id_t owner;

	si446x_debug_print(DEBUG02, "%s()\r\n", __func__);

	if (!isInit) return DS_Bug;

	if (platform_power_radio_status(SI4468_SPI2) == 0) { // No Power =(
		si446x_debug_print(DEBUG03, "%s() Fatal error, radio power not ready.\r\n", __func__);
		return DS_Fail;
	}

	if ( owner = si446x_spi_lock(radio_lock_init) ) {
		si446x_debug_print(DEBUG03, "%s() FAIL, SPI busy: %s\r\n", print_lock(owner), __func__);
		return DS_Fail;
	}

	si446x_reset();
	reset_errors  = si446x_part_info();
	reset_errors += si446x_func_info();

	if ( reset_errors ) {
		ret = DS_Fail;
		si446x_debug_print(ERR100, "%s(): reset failed.\r\n", __func__);
		goto si446x_hal_reinit_CLEANUP;
	}

	si446x_get_int_status(0x0, 0x0, 0x0); // Saves status and clears all interrupts
	si446x_request_device_state();
	si446x_debug_print(DEBUG01, "%s() Radio Interrupts Cleared\r\n", __func__);

	temp = si446x_get_property(0x00, 0x01, 0x03);
	if (temp != RF_GLOBAL_CONFIG_1_1) {
		si446x_debug_print(DEBUG01, "%s() GLOBAL_CONFIG Setting Looks Wrong... Overriding...\r\n", __func__);
		si446x_set_property( 0x00, 0x01, 0x03, RF_GLOBAL_CONFIG_1_1 );
	}

	temp = si446x_get_property(0x12, 0x01, 0x08);
	if (temp != PKT_LEN) {
		si446x_debug_print(DEBUG01, "%s() PKT_LEN Setting Looks Wrong...Overriding...\r\n", __func__);
		si446x_set_property( 0x12, 0x01, 0x08, PKT_LEN );
	}

	temp = si446x_get_property(0x12, 0x01, 0x12);
	if (temp != PKT_FIELD_2_LENGTH) {
		si446x_debug_print(DEBUG01, "%s() PKT_FIELD_2_LENGTH Setting Looks Wrong...Overriding...\r\n", __func__);
		si446x_set_property( 0x12, 0x01, 0x12, PKT_FIELD_2_LENGTH );
	}

	// Re-set the previous power level. Init() does not do this.
	si446x_debug_print(DEBUG03, "%s() setting TX power to previous setting %d\r\n", __func__, tx_power);
	si446x_set_property( 0x22 , 1, 0x01, tx_power );

	si446x_fifo_info(0x3); // Reset both FIFOs. bit1 RX, bit0 TX
	si446x_debug_print(DEBUG01, "SI446X: Radio RX/TX FIFOs Cleared\r\n");

si446x_hal_reinit_CLEANUP:

	si446x_radio_unlock();
	si446x_spi_unlock();

	return ret;
}

DeviceStatus si446x_hal_init(RadioEventHandler *event_handler, UINT8 radio, UINT8 mac_id) {

	DeviceStatus ret = DS_Success;
	int reset_errors;
	uint8_t temp;
	radio_lock_id_t owner;

	if (isInit) return DS_Bug;

	// Set up debugging output
	si446x_set_debug_print(si446x_debug_print, si4468x_debug_level);
	si446x_debug_print(DEBUG02, "SI446X: si446x_hal_init()\r\n");

	if (platform_power_radio_status(radio) == 0) { // No Power =(
		si446x_debug_print(DEBUG03, "%s() Fatal error, radio power not ready.\r\n", __func__);
		return DS_Fail;
	}

	if ( owner = si446x_spi_lock(radio_lock_init) ) {
		si446x_debug_print(DEBUG02, "SI446X: si446x_hal_init() FAIL, SPI busy: %s\r\n", print_lock(owner));
		return DS_Fail;
	}

	pin_config();

	// Default settings
	si446x_channel = si446x_default_channel;
	tx_power = si446x_default_power;
	tx_msg_ptr = &tx_msg;
	rx_msg_ptr = &rx_msg;
	init_radio_SPI();
	init_si446x_pins();

	si446x_reset();
	reset_errors  = si446x_part_info();
	reset_errors += si446x_func_info();

	if ( reset_errors ) {
		ret = DS_Fail;
		si446x_debug_print(ERR100, "SI446X: si446x_hal_init(): reset failed.\r\n");
		goto si446x_hal_init_CLEANUP;
	}

	isInit = 1;

	si446x_get_int_status(0x0, 0x0, 0x0); // Saves status and clears all interrupts
	si446x_request_device_state();
	si446x_debug_print(DEBUG01, "SI446X: Radio Interrupts Cleared\r\n");

	temp = si446x_get_property(0x00, 0x01, 0x03);
	if (temp != RF_GLOBAL_CONFIG_1_1) {
		si446x_debug_print(DEBUG01, "SI446X: si446x_hal_init GLOBAL_CONFIG Setting Looks Wrong... Overriding...\r\n");
		si446x_set_property( 0x00, 0x01, 0x03, RF_GLOBAL_CONFIG_1_1 );
	}

	temp = si446x_get_property(0x12, 0x01, 0x08);
	if (temp != PKT_LEN) {
		si446x_debug_print(DEBUG01, "SI446X: si446x_hal_init PKT_LEN Setting Looks Wrong...Overriding...\r\n");
		si446x_set_property( 0x12, 0x01, 0x08, PKT_LEN );
	}

	temp = si446x_get_property(0x12, 0x01, 0x12);
	if (temp != PKT_FIELD_2_LENGTH) {
		si446x_debug_print(DEBUG01, "SI446X: si446x_hal_init PKT_FIELD_2_LENGTH Setting Looks Wrong...Overriding...\r\n");
		si446x_set_property( 0x12, 0x01, 0x12, PKT_FIELD_2_LENGTH );
	}

	si446x_fifo_info(0x3); // Reset both FIFOs. bit1 RX, bit0 TX
	si446x_debug_print(DEBUG01, "SI446X: Radio RX/TX FIFOs Cleared\r\n");

	// Set MAC datastructures
	active_mac_index = radio_si446x_spi2.GetMacIdIndex();
	if(radio_si446x_spi2.Initialize(event_handler, mac_id) != DS_Success) {
		ret = DS_Fail;
		si446x_debug_print(ERR100, "SI446X: si446x_hal_init(): MAC init failed.\r\n");
		goto si446x_hal_init_CLEANUP;
	}

	{
		//Get cpu serial and hash it to use as node id. THIS IS NOT A DRIVER FUNCTION. MOVE TO MAC LAYER.
		UINT8 cpuserial[serial_size];
		memset(cpuserial, 0, serial_size);
		GetCPUSerial(cpuserial, serial_size);
		UINT16 tempNum=0;
		UINT16 * temp = (UINT16 *) cpuserial;
		for (int i=0; i< 6; i++){
			tempNum=tempNum ^ temp[i]; //XOR 72-bit number to generate 16-bit hash
		}
		radio_si446x_spi2.SetAddress(tempNum);
		si446x_debug_print(DEBUG02, "SI446X: CPU Serial Hash: 0x%.4X\r\n", tempNum);
	}

	// Init Continuations and interrupts
	// Leave these last in case something above fails.
	CPU_GPIO_EnableInputPin( SI446X_pin_setup->nirq_mf_pin, FALSE, si446x_spi2_handle_interrupt, GPIO_INT_EDGE_LOW, RESISTOR_DISABLED);
	tx_callback_continuation.InitializeCallback(tx_cont_do, NULL);
	rx_callback_continuation.InitializeCallback(rx_cont_do, NULL);
	int_defer_continuation.InitializeCallback(int_cont_do, NULL);

	// Power system callbacks
	state_callback_continuation.InitializeCallback(state_cont_do, NULL);
	platform_power_event_sub(&state_callback_continuation);

si446x_hal_init_CLEANUP:

	si446x_radio_unlock();
	si446x_spi_unlock();

	return ret;
}

// Hard radio shut-off. Does not check busy.
// This will ALWAYS shut down the radio regardless of MAC level status.
// TODO: Fix above.
DeviceStatus si446x_hal_uninitialize(UINT8 radio) {
	DeviceStatus ret = DS_Success;
	radio_lock_id_t owner;
	si446x_debug_print(DEBUG02, "SI446X: si446x_hal_uninitialize()\r\n");

	if ( owner = si446x_spi_lock(radio_lock_uninit) ) 	{ return DS_Fail; }
	if ( !isInit )				{ si446x_spi_unlock(); return DS_Fail; }

	isInit = 0;
	radio_shutdown(1);

	CPU_GPIO_DisablePin(SI446X_pin_setup->nirq_mf_pin, RESISTOR_DISABLED, 0, GPIO_ALT_PRIMARY); // Only PIN matters
	radio_si446x_spi2.SetInitialized(FALSE);

	rx_callback = NULL;
	tx_callback = NULL;

	if(radio_si446x_spi2.UnInitialize((UINT8)active_mac_index) != DS_Success) {
		si446x_debug_print(ERR100, "SI446X: si446x_hal_uninitialize(): MAC UnInit failed.\r\n");
		ret = DS_Fail;
	}

	si446x_radio_unlock();
	si446x_spi_unlock();

	return ret;
}

// A full-stop reset of the chip. In the PAL API but you probably shouldn't be.
// Prefer to use UnInit() and Init() instead. --NPS
DeviceStatus si446x_hal_reset(UINT8 radio) {
	radio_lock_id_t owner;

	si446x_debug_print(ERR99, "SI446X: si446x_hal_reset(). PROBABLY A BAD IDEA. USE UNINIT() AND INIT() INSTEAD.\r\n");

	if ( owner = si446x_spi_lock(radio_lock_reset) ) 	{ return DS_Fail; }

	si446x_reset();
	si446x_get_int_status(0x0, 0x0, 0x0); // Clear all interrupts.
	si446x_fifo_info(0x3); // Reset both FIFOs. bit1 RX, bit0 TX

	return DS_Success;
}

// Probably should not be in the driver...
UINT16 si446x_hal_get_address(UINT8 radio) {
	si446x_debug_print(DEBUG01, "SI446X: si446x_hal_get_address()\r\n");
	return radio_si446x_spi2.GetAddress();
}

// Probably should not be in the driver...
BOOL si446x_hal_set_address(UINT8 radio, UINT16 address) {
	si446x_debug_print(DEBUG01, "SI446X: si446x_hal_set_address()\r\n");
	return radio_si446x_spi2.SetAddress(address);
}

void si446x_hal_set_default_state(si_state_t defaultState){
	defaultRxDoneState = defaultState;
}

// INTERNAL USE ONLY -- CALLER MUST HOLD SPI_LOCK AND RADIO_LOCK
// Caller passes in their lock id, verified in function.
// Returns 'true' if abort with possible packet recovery.
static bool si446x_leave_rx(radio_lock_id_t id) {
	uint8_t int_enable;
	si_state_t state;
	unsigned timeout=si446x_tx_timeout;

	if (id != spi_lock || id != radio_lock) {
		si446x_debug_print(ERR100, "SI446X: si446x_leave_rx() Calling with bad locks. Abort\r\n");
		return true;
	}

	state = si446x_request_device_state();
	if (state != SI_STATE_RX && state != SI_STATE_RX_TUNE) return false; // Nothing to do

	int_enable = si446x_get_property(0x01, 1, 0); 		// save interrupt mask
	si446x_set_property(0x01, 1, 0, 0); 				// Writes 0, mask all interrupts

	// Check one last time
	si446x_get_int_status(0xFF, 0xFF, 0xFF); 			// Refresh interrupts does NOT clear.
	if (si446x_get_ph_pend() || si446x_get_modem_pend() || cont_busy()) {
		si446x_set_property(0x01, 1, 0, int_enable); 	// Unmask interrupts
		si446x_debug_print(DEBUG02, "SI446X: si446x_leave_rx() Caught Event before TX attempt. Aborting.\r\n");
		return true;
	}

	// Past this point, any RX that comes in is kill

	si446x_change_state(SI_STATE_SPI_ACTIVE);
	while( si446x_request_device_state() != SI_STATE_SPI_ACTIVE && timeout-- ) ; // spin

	if (timeout == 0)
		si446x_debug_print(ERR100, "SI446X: si446x_leave_rx() State Change Timeout. Radio state unknown!\r\n");

	// The only thing that could maybe have happened is an RX, but its dead to us now.
	si446x_get_int_status(0x0, 0x0, 0x0);			// Clear interrupts
	si446x_fifo_info(0x3);							// Clears FIFO
	si446x_set_property(0x01, 1, 0, int_enable); 	// Unmasks interrupts

	// For debug purposes. The packet is gone.
	if (si446x_get_ph_pend() || si446x_get_modem_pend()) {
		si446x_debug_print(DEBUG02, "SI446X: si446x_leave_rx() Lost packet in RX-TX transition.\r\n");
	}

	return false;
}

// eventTime is ignored unless doTS (USE_TIMESTAMP) is set.
DeviceStatus si446x_packet_send(uint8_t chan, uint8_t *pkt, uint8_t len, UINT32 eventTime, int doTS, uint8_t after_state) {
	uint8_t tx_buf[si446x_packet_size+1]; // Add one for packet size field
	radio_lock_id_t owner;
	si_state_t state = SI_STATE_ERROR;

	si446x_debug_print(DEBUG02, "SI446X: si446x_packet_send() size:%d doTs:%d\r\n", len, doTS);
	//si446x_debug_print(DEBUG01, "\tcontents: %s\r\n", (char *)pkt);

	SI446x_INT_MODE_CHECK();

	if ( len > si446x_packet_size || (doTS && (len > si446x_payload_ts)) ) {
		si446x_debug_print(ERR99, "SI446X: si446x_packet_send() Fail. Packet Too Large.\r\n");
		return DS_Timeout; // Cheating here. Not really a timeout but up a layer will return NO_BadPacket. TODO: Make proper.
	}

	if ( !isInit ) {
		si446x_debug_print(ERR99, "SI446X: si446x_packet_send() Fail. Not initialized.\r\n");
		return DS_Fail;
	}

	if ( owner = si446x_spi_lock(radio_lock_tx) ) 	{
		si446x_debug_print(DEBUG02, "SI446X: si446x_packet_send() Fail. SPI locked by %s.\r\n", print_lock(owner));
		return DS_Busy;
	}

	// Lock radio until TX is done.
	if ( owner = si446x_radio_lock(radio_lock_tx) ) 	{
		si446x_debug_print(DEBUG02, "SI446X: si446x_packet_send() Fail. Radio locked by %s\r\n", print_lock(owner));
		si446x_spi_unlock();
		return DS_Busy;
	}

	// Last chance for packets to come in.
	if ( si446x_leave_rx(radio_lock_tx) ) {
		si446x_radio_unlock();
		si446x_spi_unlock();
		return DS_Busy;
	}

	state = si446x_request_device_state();
	if (state == SI_STATE_ERROR) {
		si446x_debug_print(DEBUG02, "SI446X: si446x_packet_send(): Bad State. Aborting. Reset radio?\r\n");
		si446x_radio_unlock();
		si446x_spi_unlock();
		return DS_Fail;
	}

	set_radio_power_pwm(1); // Make sure we have enough juice to do this

	tx_buf[0] = len;
	if (doTS) { tx_buf[0] += 4; } // Add timestamp to packet size if used.
	memcpy(&tx_buf[1], pkt, len);

	si446x_write_tx_fifo(len+1, tx_buf); // add one for packet size

	if (doTS) { // Timestamp Case
		GLOBAL_LOCK(irq);

		si446x_change_state(SI_STATE_TX_TUNE);
		while( si446x_request_device_state() != SI_STATE_TX_TUNE ) ; // spin. TODO: Add timeout.

		UINT32 eventOffset = (HAL_Time_CurrentTicks() & 0xFFFFFFFF) - eventTime;

		si446x_write_tx_fifo(4, (uint8_t*)&eventOffset); // generate and write timestamp late as possible.

		if(SI4468_Radio_TX_Instance != DISABLED_PIN ){
			//CPU_GPIO_SetPinState( SI4468_Radio_TX_Instance, !CPU_GPIO_GetPinState(SI4468_Radio_TX_Instance) );
			//CPU_GPIO_SetPinState( SI4468_Radio_TX_Instance, !CPU_GPIO_GetPinState(SI4468_Radio_TX_Instance) );
		}
		si446x_start_tx(chan, after_state, tx_buf[0]+1);
		irq.Release();
	} else { // Normal Case
		if(SI4468_Radio_TX_Instance_NOTS != DISABLED_PIN ){
			//CPU_GPIO_SetPinState( SI4468_Radio_TX_Instance_NOTS, !CPU_GPIO_GetPinState(SI4468_Radio_TX_Instance_NOTS) );
			//CPU_GPIO_SetPinState( SI4468_Radio_TX_Instance_NOTS, !CPU_GPIO_GetPinState(SI4468_Radio_TX_Instance_NOTS) );
		}
		si446x_start_tx(chan, after_state, tx_buf[0]+1);
	}

	si446x_spi_unlock();
	// RADIO STAYS LOCKED UNTIL TX DONE

	return DS_Success;
}

void *si446x_hal_send(UINT8 radioID, void *msg, UINT16 size) {
	//CPU_GPIO_SetPinState( SI4468_TX, TRUE );
	si446x_debug_print(DEBUG01, "SI446X: si446x_hal_send()\r\n");

	DeviceStatus ret;

	// Do the send
	ret = si446x_packet_send(si446x_channel, (uint8_t *) msg, size, 0, NO_TIMESTAMP, SI446x_TX_DONE_STATE);
	if (ret != DS_Success) {
		SendAckFuncPtrType AckHandler = radio_si446x_spi2.GetMacHandler(active_mac_index)->GetSendAckHandler();
		switch (ret) {
			case DS_Busy: 	(*AckHandler)(tx_msg_ptr, size, NetworkOperations_Busy, SI_DUMMY);			break;
			case DS_Timeout:(*AckHandler)(tx_msg_ptr, size, NetworkOperations_BadPacket, SI_DUMMY); 	break;
			default:		(*AckHandler)(tx_msg_ptr, size, NetworkOperations_Fail, SI_DUMMY); 		break;
		}
		return msg;
	}

	// exchange bags.
	Message_15_4_t* temp = tx_msg_ptr;
	tx_msg_ptr = (Message_15_4_t*) msg;

	//CPU_GPIO_SetPinState( SI4468_TX, FALSE );
	return msg;
}

void *si446x_hal_send_ts(UINT8 radioID, void *msg, UINT16 size, UINT32 eventTime) {

	//CPU_GPIO_SetPinState( SI4468_TX_TIMESTAMP, TRUE );

	si446x_debug_print(DEBUG01, "SI446X: si446x_hal_send_ts()\r\n");

	DeviceStatus ret;

	// Do the send
	ret = si446x_packet_send(si446x_channel, (uint8_t *) msg, size, eventTime, YES_TIMESTAMP, SI446x_TX_DONE_STATE);
	if (ret != DS_Success) {
		SendAckFuncPtrType AckHandler = radio_si446x_spi2.GetMacHandler(active_mac_index)->GetSendAckHandler();
		switch (ret) {
			case DS_Busy: 	(*AckHandler)(tx_msg_ptr, size, NetworkOperations_Busy, SI_DUMMY);			break;
			case DS_Timeout:(*AckHandler)(tx_msg_ptr, size, NetworkOperations_BadPacket, SI_DUMMY);	break;
			default:		(*AckHandler)(tx_msg_ptr, size, NetworkOperations_Fail, SI_DUMMY);			break;
		}
		return msg;
	}

	// exchange bags.
	Message_15_4_t* temp = tx_msg_ptr;
	tx_msg_ptr = (Message_15_4_t*) msg;

	//CPU_GPIO_SetPinState( SI4468_TX_TIMESTAMP, FALSE );
	return msg;
}

#ifdef SI446X_DEBUG_UNFINISHED_PKT
// ASSUMES YOU HOLD SPI LOCK
static bool rx_consistency_check(void) {
	GLOBAL_LOCK(irq); // lock while debugging only. Change me.

	// If the radio lock is held by RX
	// We must be in either RX mode or the continuation is pending
	// or an interrupt is pending
	if ( radio_lock == radio_lock_rx ) {
		si_state_t state = si446x_request_device_state();
		//bool isBusy = (cont_busy() || radio_get_assert_irq());
		si446x_get_int_status(0xFF, 0xFF, 0xFF);
		bool isBusy = cont_busy() || si446x_get_ph_pend() || si446x_get_modem_pend();

		if (state != SI_STATE_RX && !isBusy) {
			si446x_debug_print(ERR100, "SI446X: rx_consistency_check() Fail? Show Nathan.\r\n");
			return false;
		}

		// check for timeout of SYNC_DET
		UINT64 now = HAL_Time_CurrentTicks();
		if ( (now - rx_timestamp) > (si446x_rx_timeout_ms*8000) ) {
			si446x_debug_print(ERR100, "SI446X: rx_consistency_check() Timeout? Show Nathan.\r\n");
			return false;
		}
	}
	return true;
}
#endif // #ifdef SI446X_DEBUG_UNFINISHED_PKT

// Does NOT set the radio busy unless a packet comes in.
DeviceStatus si446x_hal_rx(UINT8 radioID) {
	//CPU_GPIO_SetPinState( SI4468_Radio_STATE, TRUE );
	radio_lock_id_t owner;
	si446x_debug_print(DEBUG02, "SI446X: si446x_hal_rx()\r\n");

	if (!isInit) {
		si446x_debug_print(DEBUG01, "SI446X: si446x_hal_rx() FAIL. Not Init.\r\n");
		return DS_Fail;
	}

	SI446x_INT_MODE_CHECK();

	if ( cont_busy() ) {
		si446x_debug_print(DEBUG02, "SI446X: si446x_hal_rx() Tasks outstanding, aborting.\r\n");
		return DS_Fail;
	}

	if ( owner = si446x_spi_lock(radio_lock_rx_setup) ) {
		si446x_debug_print(DEBUG01, "SI446X: si446x_hal_rx() FAIL. SPI locked by %s\r\n", print_lock(owner));
		return DS_Busy;
	}

#ifdef SI446X_DEBUG_UNFINISHED_PKT
	// DEBUGGING ONLY
	rx_consistency_check();
#endif

	// We have to hold radio lock to ensure we are free
	if ( owner = si446x_radio_lock(radio_lock_rx_setup) ) {
		si446x_debug_print(DEBUG02, "SI446X: si446x_hal_rx() FAIL. Radio locked by owner: %s\r\n", print_lock(owner));
		si446x_spi_unlock();
		return DS_Busy;
	}

	si446x_get_int_status(0xFF, 0xFF, 0xFF);
	if ( radio_get_assert_irq() || cont_busy() || si446x_get_ph_pend() || si446x_get_modem_pend() ) {
		si446x_debug_print(DEBUG01, "SI446X: si446x_hal_rx() radio ops pending, aborting.\r\n");
		si446x_radio_unlock();
		si446x_spi_unlock();
		return DS_Busy;
	}

	// Now that we are sure we aren't busy, can unlock the radio.
	si446x_radio_unlock();

	si446x_start_rx_fast_channel(si446x_channel);
	si446x_spi_unlock();
	si446x_debug_print(DEBUG01, "SI446X: si446x_hal_rx() END\r\n");


	return DS_Success;
}


DeviceStatus si446x_hal_sleep(UINT8 radioID) {
	//CPU_GPIO_SetPinState( SI4468_HANDLE_SLEEP, TRUE );
	radio_lock_id_t owner;
	si446x_debug_print(DEBUG01, "SI446X: si446x_hal_sleep()\r\n");

	SI446x_INT_MODE_CHECK();

	if (!isInit) {
		si446x_debug_print(DEBUG03, "SI446X: si446x_hal_sleep() FAIL. No Init.\r\n");
		return DS_Fail;
	}

	// We were already asleep
	if ( si446x_request_device_state_shadow() == SI_STATE_SLEEP ) {
		return DS_Success;
	}

	if ( owner = si446x_spi_lock(radio_lock_sleep) ) {
		si446x_debug_print(ERR99, "SI446X: si446x_hal_sleep() FAIL. SPI locked. Owner is %s\r\n", print_lock(owner));
		//CPU_GPIO_SetPinState( SI4468_HANDLE_SLEEP, FALSE );
		//CPU_GPIO_SetPinState( SI4468_HANDLE_SLEEP, TRUE );
		//CPU_GPIO_SetPinState( SI4468_HANDLE_SLEEP, FALSE );
		return DS_Fail;
	}

	if ( owner = si446x_radio_lock(radio_lock_sleep) ) {
		si446x_debug_print(ERR99, "SI446X: si446x_hal_sleep() FAIL. Radio Busy. Owner is %s\r\n", print_lock(owner));
		//Radio complains that tx is not yet done
		//si446x_radio_unlock();
		si446x_spi_unlock();
		//CPU_GPIO_SetPinState( SI4468_HANDLE_SLEEP, FALSE );
		//CPU_GPIO_SetPinState( SI4468_HANDLE_SLEEP, TRUE );
		//CPU_GPIO_SetPinState( SI4468_HANDLE_SLEEP, FALSE );
		return DS_Fail;
	}

	if ( si446x_leave_rx(radio_lock_sleep) ) {
		si446x_debug_print(DEBUG02, "SI446X: si446x_hal_sleep() FAIL. Radio Busy.\r\n");
		si446x_radio_unlock();
		si446x_spi_unlock();
		return DS_Fail;
	}

	si446x_change_state(SI_STATE_SLEEP);
	si446x_radio_unlock();
	si446x_spi_unlock();

	//CPU_GPIO_SetPinState( SI4468_HANDLE_SLEEP, FALSE );
	//CPU_GPIO_SetPinState( SI4468_Radio_STATE, FALSE );
	return DS_Success;
}


DeviceStatus si446x_hal_tx_power(UINT8 radioID, int pwr) {
	radio_lock_id_t owner;
	si446x_debug_print(DEBUG02, "SI446X: si446x_hal_tx_power()\r\n");

	if (pwr < 0) return DS_Fail; // Not worth of a printf

	if ( owner = si446x_spi_lock(radio_lock_tx_power) ) {
		si446x_debug_print(DEBUG02, "SI446X: si446x_hal_tx_power() FAIL. SPI locked: %s\r\n", print_lock(owner));
		return DS_Fail;
	}

	if (!isInit) {
		si446x_debug_print(DEBUG02, "SI446X: si446x_hal_tx_power() FAIL. No Init.\r\n");
		si446x_spi_unlock();
		return DS_Fail;
	}

	if ( owner = si446x_radio_lock(radio_lock_tx_power) ) {
		si446x_debug_print(DEBUG02, "SI446X: si446x_hal_tx_power() FAIL. Radio locked: %s\r\n", print_lock(owner));
		si446x_spi_unlock();
		return DS_Fail;
	}

	si446x_debug_print(DEBUG03, "SI446X: si446x_hal_tx_power() setting raw power %d\r\n", (uint8_t)pwr);
	si446x_set_property( 0x22 , 1, 0x01, (uint8_t) pwr );
	tx_power = pwr;

	si446x_radio_unlock();
	si446x_spi_unlock();

	return DS_Success;
}

DeviceStatus si446x_hal_set_channel(UINT8 radioID, int channel) {
	radio_lock_id_t owner;
	si446x_debug_print(DEBUG02, "SI446X: si446x_hal_set_channel()\r\n");
	if (channel < 0 ) return DS_Fail;

	if ( owner = si446x_spi_lock(radio_lock_set_channel) ) {
		si446x_debug_print(DEBUG01, "SI446X: si446x_hal_set_channel() FAIL. SPI locked: %s\r\n", print_lock(owner));
		return DS_Fail;
	}

	if (!isInit) {
		si446x_debug_print(DEBUG02, "SI446X: si446x_hal_set_channel() FAIL. No Init.\r\n");
		si446x_spi_unlock();
		return DS_Fail;
	}

	if ( owner = si446x_radio_lock(radio_lock_set_channel) ) {
		si446x_debug_print(DEBUG01, "SI446X: si446x_hal_set_channel() FAIL. Radio Busy: %s\r\n", print_lock(owner));
		si446x_spi_unlock();
		return DS_Fail;
	}

	si446x_channel = channel;

	// Technically may not need the locks.
	// But could confuse the user otherwise.
	// And still at least protects the vars.
	si446x_radio_unlock();
	si446x_spi_unlock();

	return DS_Success;
}


// Doesn't belong in radio driver... --NPS
//Properties of radio
static INT8 radioType;
void si446x_hal_set_RadioType(INT8 radio)
{
	radioType = radio;
}

// Doesn't belong in radio driver...
INT8 si446x_hal_get_RadioType()
{
	return radioType;
}

// New CCA semantics after talking to Ananth + Bora
// CCA request is only valid if radio is already in RX, otherwise discard.
// Radio remains in RX after CCA. Packets that come in during CCA are discarded.
// Packets that come in atter CCA while still in RX are kept.
// NPS 2016-07-06
DeviceStatus si446x_hal_cca_ms(UINT8 radioID, UINT32 ms) {
	radio_lock_id_t owner;
	uint8_t int_enable;
	DeviceStatus ret;
	UINT64 now, now2;
	int adjusted_ms;
	const unsigned ticks_per_ms = 8000;

	now = HAL_Time_CurrentTicks();

	si446x_debug_print(DEBUG01, "SI446X: si446x_hal_cca_ms() ms:%d\r\n",ms);

	SI446x_INT_MODE_CHECK();

	if (!isInit) {
		si446x_debug_print(ERR99, "SI446X: si446x_hal_cca_ms() FAIL. No Init.\r\n");
		return DS_Fail;
	}

	if ( owner = si446x_spi_lock(radio_lock_cca_ms) ) {
		si446x_debug_print(DEBUG03, "SI446X: si446x_hal_cca_ms() FAIL. SPI locked: %s\r\n", print_lock(owner));
		return DS_Fail;
	}

	if ( owner = si446x_radio_lock(radio_lock_cca_ms) ) {
		si446x_debug_print(DEBUG03, "SI446X: si446x_hal_cca_ms() FAIL. Radio locked: %s\r\n", print_lock(owner));
		si446x_spi_unlock();
		return DS_Fail;
	}

	if ( cont_busy() ) {
		si446x_debug_print(DEBUG03, "SI446X: si446x_hal_cca_ms() FAIL. Continuation Pending.\r\n");
		si446x_radio_unlock();
		si446x_spi_unlock();
		return DS_Fail;
	}

	si_state_t state = si446x_request_device_state();
	if (state != SI_STATE_RX && state != SI_STATE_RX_TUNE) {
		si446x_debug_print(DEBUG03, "SI446X: si446x_hal_cca_ms() Radio not in RX. Abort CCA. State: %d\r\n", state);
		si446x_radio_unlock();
		si446x_spi_unlock();
		return DS_Fail;
	}

	// Allow to move from RX_TUNE to RX if needed.
	// TODO: Timeout
	while ( state == SI_STATE_RX_TUNE ) { state = si446x_request_device_state(); }

	// We don't keep/want packets for the duration of the CCA. Turn off interupts and kill FIFO afterwards.
	int_enable = si446x_get_property(0x01, 1, 0); 	// save interrupt mask
	si446x_set_property(0x01, 1, 0, 0); 			// mask all radio interrupts
	si446x_get_int_status(0xFF, 0xFF, 0xFF); 		// check interrupts final time

	// last abort chance
	if ( si446x_get_ph_pend() || si446x_get_modem_pend() ) {
		si446x_set_property(0x01, 1, 0, int_enable); 	// Unmask interrupts
		si446x_debug_print(DEBUG03, "SI446X: si446x_hal_cca_ms() Radio Busy.\r\n");
		si446x_radio_unlock();
		si446x_spi_unlock();
		return DS_Fail;
	}

	// We do NOT retain packets encountered during CCA.
	// Not using GLOBAL_LOCK here so YMMV with how accurate this timing is.
	now2 = HAL_Time_CurrentTicks();
	adjusted_ms = ms - ((now2-now) / ticks_per_ms) - 1;
	if(adjusted_ms > 0)
		HAL_Time_Sleep_MicroSeconds(adjusted_ms); 	// Wait specified period
	si446x_get_modem_status( 0xFF ); 				// Fetch results

	if (si446x_get_current_rssi() >= si446x_rssi_cca_thresh)
		ret = DS_Busy;
	else
		ret = DS_Success;

	si446x_leave_rx(radio_lock_cca_ms);

	si446x_get_int_status(0x0, 0x0, 0x0);	 		// Clear interrupts
	si446x_fifo_info(0x3);							// Clear FIFO
	si446x_set_property(0x01, 1, 0, int_enable);	// Unmask interrupts
	si446x_start_rx_fast_channel(si446x_channel);	// re-arm RX

	si446x_radio_unlock();
	si446x_spi_unlock();
	si446x_debug_print(DEBUG03, "SI446X: CCA complete, saw RSSI: %d dBm (raw %d) thresh: %d\r\n",
		convert_rssi(si446x_get_current_rssi()), si446x_get_current_rssi(), si446x_rssi_cca_thresh);
	return ret;
}

UINT32 si446x_hal_get_chan(UINT8 radioID) {
	si446x_debug_print(DEBUG01, "SI446X: si446x_hal_get_chan()\r\n");
	return si446x_channel;
}


UINT32 si446x_hal_get_power(UINT8 radioID) {
	si446x_debug_print(DEBUG01, "SI446X: si446x_hal_get_power()\r\n");
	return tx_power;
}

// Also possible to do this with FRR.
// Automatically saved each packet, don't need to bother radio or lock.
UINT32 si446x_hal_get_rssi(UINT8 radioID) {
	UINT32 ret = si446x_get_latched_rssi();
	si446x_debug_print(DEBUG02, "SI446X: si446x_hal_get_rssi() : %d\r\n", ret);
	return ret;
}

// INTERRUPT CONTEXT, LOCKED
static void si446x_pkt_tx_int() {
	si446x_debug_print(DEBUG01, "SI446X: si446x_pkt_tx_int()\r\n");
	set_radio_power_pwm(0); // TX done, idle power supply
	tx_callback_continuation.Enqueue();
}

// INTERRUPT CONTEXT. LOCKED, radio_busy until we pull from continuation
static void si446x_pkt_rx_int() {
	// radio_lock owned by SYNC_DET at this point
	si446x_debug_print(DEBUG01, "SI446X: si446x_pkt_rx_int()\r\n");
	rx_callback_continuation.Enqueue();
}

// ASSUMES SPI_LOCK IS HELD BY CALLER (INTERRUPT HANDLER)
// Interface spec is to just to drop the packet on the floor
static void si446x_pkt_bad_crc_int(void) {
	radio_lock_id_t owner 		= (radio_lock_id_t) spi_lock;
	radio_lock_id_t radio_owner = (radio_lock_id_t) radio_lock;

	if (owner != radio_lock_interrupt) {
		si446x_debug_print(ERR99, "SI446X: si446x_pkt_bad_crc_int() Bad spi_lock!, owner: %s\r\n", print_lock(owner));
		return;
	}

	if (radio_owner != radio_lock_rx) {
		si446x_debug_print(ERR99, "SI446X: si446x_pkt_bad_crc_int() Bad radio_lock!, owner: %s\r\n", print_lock(radio_owner));
		return;
	}

	si446x_debug_print(DEBUG01, "SI446X: si446x_pkt_bad_crc_int() Bad CRC. FIFOs cleared\r\n");
	si446x_fifo_info(0x3); 		// clear the FIFO
	// We lost the packet, so give up the radio
	si446x_radio_unlock();
}

// INTERRUPT CONTEXT
static void si446x_spi2_handle_interrupt(GPIO_PIN Pin, BOOL PinState, void* Param)
{
	uint8_t modem_pend, ph_pend;
	radio_lock_id_t owner;
	UINT64 int_ts;

	GLOBAL_LOCK(irq); // Locked due to time critical.
	int_ts = HAL_Time_CurrentTicks(); // Log RX time.
	irq.Release(); // Unlock after timestamp.

	if (Pin != SI446X_pin_setup->nirq_mf_pin) { return; }

	si446x_debug_print(DEBUG02, "SI446X: INT\r\n");

	if ( owner = si446x_spi_lock(radio_lock_interrupt) ) {
		if (owner == radio_lock_init) return; // Ignore spurious error during init.

		// Damn, we got an interrupt in the middle of another transaction. Have to defer it.
		si446x_debug_print(ERR99, "SI446X: si446x_spi2_handle_interrupt() SPI locked: %s\r\n", print_lock(owner));
		int_defer_continuation.Enqueue();
		return;
	}

	si446x_get_int_status(0x0, 0x0, 0x0); // Saves status and clears all interrupts
	ph_pend			= si446x_get_ph_pend();
	modem_pend 		= si446x_get_modem_pend();

	// Only save timestamp if it was an RX event.
	// Unlock SPI after the potential radio_lock, so both don't glitch free.
	if (modem_pend & MODEM_MASK_SYNC_DETECT) {
		// Unconditional lock grab.
		owner = si446x_radio_lock_nofail(radio_lock_rx);
		if (owner != radio_lock_none && owner != radio_lock_tx)
			si446x_debug_print(ERR99, "SI446X: si446x_spi2_handle_interrupt() Odd radio_lock: %s\r\n", print_lock(owner));
		rx_timestamp = int_ts;
		(radio_si446x_spi2.GetMacHandler(active_mac_index)->GetRadioInterruptHandler())(StartOfReception, NULL);
	}

	if (ph_pend & PH_STATUS_MASK_PACKET_RX) 	{
		si446x_pkt_rx_int();
		if (!rx_callback_continuation.IsLinked()) {
			si446x_debug_print(ERR99, "SI446X: si446x_spi2_handle_interrupt() No RX cont linked???\r\n");
		}
	}

	if (ph_pend & PH_STATUS_MASK_PACKET_SENT) 	{ si446x_pkt_tx_int(); }
	if (ph_pend & PH_STATUS_MASK_CRC_ERROR) 	{
		si446x_pkt_bad_crc_int();
	}

	// If we finished a packet, go to sleep
	/*if ( (ph_pend & PH_STATUS_MASK_PACKET_RX) || (ph_pend & PH_STATUS_MASK_CRC_ERROR) ) {
		si446x_change_state(SI_STATE_SLEEP);
	}*/

	si446x_spi_unlock();
}
