
#include <tinyhal.h>
#include <Samraksh\Message.h>
#include <Samraksh\Radio.h>
#include <string.h>

#include "si446x.h"

// Hardware stuff
#include <stm32f10x.h>

// If MAC layer calls an operation before previous finished, refuse the new request
// Alternatively, can attempt to service ourself.
#define SI4468_FORCE_MAC_SERVICE

// Automatically moves the radio to RX after a transmit instead of sleeping.
// Comment out below line to disable and use default ('0')
#define SI446x_TX_DONE_STATE (SI_STATE_RX<<4)
// Default value for above, radio returns to previous state.
#ifndef SI446x_TX_DONE_STATE
#define SI446x_TX_DONE_STATE 0
#endif

// if, FOR TESTING ONLY, you want to run the continuations inside the interrupts...
//#define SI4468_FORCE_CONTINUATIONS_IN_INTERRUPT_CONTEXT

enum { SI_DUMMY=0, };

// For now, memorize all WWF serial numbers
// Yes these are strings and yes I'm a terrible person.
// These are hex CPU serial numbers
enum { serial_max = 2, serial_per = 25 };
//const char wwf_serial_numbers[serial_max][serial_per] = { "3300d9054642353044501643", "3300e0054642353045241643" };
const char dotnow_serial_numbers[serial_max][serial_per] = { "392dd9054355353848400843", "3400d805414d303635341043" };
// end serial number list.

// SETS SI446X PRINTF DEBUG VERBOSITY
const unsigned si4468x_debug_level = DEBUG02; // CHANGE ME.

// Pin list used in setup.
static SI446X_pin_setup_t SI446X_pin_setup;

// CORE LOCKING STUFF, MOVE ME LATER
static volatile uint32_t spi_lock;
static volatile uint32_t radio_lock;

static inline BOOL isInterrupt()
{
    return ((SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0);
}

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
	if (___LOAD(Lock_Variable) != 0) {
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
	*Lock_Variable = 0;
	return;
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

static void si446x_debug_print(int priority, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    if(priority >= si4468x_debug_level) {
		hal_vprintf(fmt, args);
	}

    va_end(args);
}

static void si446x_spi2_handle_interrupt(GPIO_PIN Pin, BOOL PinState, void* Param);

static si446x_tx_callback_t tx_callback;
static si446x_rx_callback_t rx_callback;

static HAL_CONTINUATION tx_callback_continuation;
static HAL_CONTINUATION rx_callback_continuation;
static HAL_CONTINUATION int_defer_continuation;

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
	si446x_debug_print(DEBUG01,"SI446X: int_cont_do()\r\n");
	si446x_spi2_handle_interrupt( SI446X_pin_setup.nirq_mf_pin, false, NULL );
}

// I agree its questionable that I'm being too complicated with continuation stuff... --NPS.
// TX CALLBACK
static void tx_cont_do(void *arg) {
	if (tx_callback != NULL)
		tx_callback();

	si446x_debug_print(DEBUG02,"SI446X: tx_cont_do()\r\n");

	SendAckFuncPtrType AckHandler = radio_si446x_spi2.GetMacHandler(active_mac_index)->GetSendAckHandler();
	(*AckHandler)(tx_msg_ptr, si446x_packet_size, NetworkOperations_Success, SI_DUMMY);

	si446x_radio_unlock();
}

static void rx_cont_do(void *arg) {
	uint8_t rx_pkt[si446x_packet_size];
	int size;
	radio_lock_id_t owner;

	si446x_debug_print(DEBUG01,"SI446X: rx_cont_do()\r\n");

	// TODO: NEED TO LOCK SPI BUS HERE, BUT CAREFULLY. (a day later I forgot why...)

	if ( owner = si446x_spi_lock(radio_lock_rx) ) {
		si446x_debug_print(ERR99,"SI446X: rx_cont_do() Somehow radio SPI was busy when servicing RX... tell Nathan. Trying again.\r\n");
		rx_callback_continuation.Enqueue();
		return;
	}

	size = si446x_get_packet_info(0,0,0);
	si446x_read_rx_fifo(size, rx_pkt);

	if (rx_callback != NULL)
		rx_callback(rx_timestamp, size, rx_pkt);

	si446x_get_modem_status( 0xFF ); // Refresh RSSI

	si446x_radio_unlock();
	si446x_spi_unlock();

	if (rx_msg_ptr == NULL) return; // Nothing left to do.

	// Metadata struct
	IEEE802_15_4_Metadata_t* metadata = rx_msg_ptr->GetMetaData();

	// Copy packet to MAC packet
	memcpy( (uint8_t *)rx_msg_ptr, rx_pkt, size );

	// Set Metadata
	metadata->SetRssi( si446x_get_latched_rssi() );
	metadata->SetLqi(0);  // No meaning on this radio
	metadata->SetReceiveTimeStamp((INT64)rx_timestamp);

	// I guess this swaps rx_msg_ptr as well???
	//(rx_msg_ptr->GetHeader())->SetLength(size);
	(rx_msg_ptr->GetHeader())->length = size; // the "new" way. blarg.

	rx_msg_ptr = (Message_15_4_t *) (radio_si446x_spi2.GetMacHandler(active_mac_index)->GetReceiveHandler())(rx_msg_ptr, size);
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

static void initSPI2() {
	GPIO_InitTypeDef GPIO_InitStructure;
	unsigned int baud;
	unsigned SpiBusClock;
	SPI_InitTypeDef SPI_InitStruct;

	const SI446X_pin_setup_t *config = &SI446X_pin_setup;

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

	SI446X_pin_setup_t *config = &SI446X_pin_setup;

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin =  config->sdn_pin;
	GPIO_Init(config->sdn_port, &GPIO_InitStructure);

	// GPIO 0 / 1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = config->gpio0_pin | config->gpio1_pin;
	GPIO_Init(config->gpio_port, &GPIO_InitStructure);

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
	GPIO_WriteBit(SI446X_pin_setup.cs_port, SI446X_pin_setup.cs_pin, Bit_RESET); // chip select
	__NOP();
}

void radio_spi_sel_no_assert() {
	GPIO_WriteBit(SI446X_pin_setup.cs_port, SI446X_pin_setup.cs_pin, Bit_SET); // chip select
}

uint8_t radio_spi_go(uint8_t data) {
	while( SPI_I2S_GetFlagStatus(SI446X_pin_setup.spi_base, SPI_I2S_FLAG_TXE) == RESET ) ; // spin
	SPI_I2S_SendData(SI446X_pin_setup.spi_base, data);
	while( SPI_I2S_GetFlagStatus(SI446X_pin_setup.spi_base, SPI_I2S_FLAG_RXNE) == RESET ) ; // spin
	return SPI_I2S_ReceiveData(SI446X_pin_setup.spi_base);
}

void radio_shutdown(int go) {
	if (go) // turn off the radio
		GPIO_WriteBit(SI446X_pin_setup.sdn_port, SI446X_pin_setup.sdn_pin, Bit_SET);
	else
		GPIO_WriteBit(SI446X_pin_setup.sdn_port, SI446X_pin_setup.sdn_pin, Bit_RESET);
}

static int convert_rssi(uint8_t x) {
	return x/2 - 0x40 - 70;
}

// Quick and dirty. Clean me up later. --NPS
static int am_i_wwf(void) {
	uint8_t cpuserial[serial_size];
	GetCPUSerial(cpuserial, serial_size);
	char my_serial[serial_per];

	// Build CPU serial string. Quick and dirty. Please help me =(
	for(int i=0,j=0; i<12; i++, j+=2) {
		hal_snprintf(&my_serial[j], 3, "%.2x", cpuserial[i]);
	}

	si446x_debug_print(DEBUG02, "SI446X: Found Serial Number 0x%s\r\n", my_serial);

	// check against all other serials.
	// This is a brutal ugly O(n) search.
	for(int i=0; i<serial_max; i++) {
		if ( strcmp( dotnow_serial_numbers[i], my_serial ) == 0 )
			return 0;
	}
	return 1;
}

static void choose_hardware_config(int isWWF, SI446X_pin_setup_t *config) {
	if (isWWF) {
		config->spi_base 		= SPI2;
		config->spi_port 		= GPIOB;
		config->nirq_port		= GPIOB;
		config->nirq_pin		= GPIO_Pin_10;
		config->nirq_mf_pin		= (GPIO_PIN) 26;
		config->gpio_port		= GPIOA;
		config->gpio0_pin		= GPIO_Pin_6;
		config->gpio1_pin		= GPIO_Pin_0;
		config->cs_port			= GPIOB;
		config->cs_pin			= GPIO_Pin_12;
		config->sclk_pin		= GPIO_Pin_13;
		config->miso_pin		= GPIO_Pin_14;
		config->mosi_pin		= GPIO_Pin_15;
		config->sdn_port		= GPIOB;
		config->sdn_pin			= GPIO_Pin_11;
		config->spi_rcc			= RCC_APB1Periph_SPI2;
		si446x_debug_print(DEBUG02, "SI446X: Using WWF Hardware Config\r\n");
	}
	else { // I am a .NOW
		config->spi_base 		= SPI2;
		config->spi_port 		= GPIOB;
		config->nirq_port		= GPIOA;
		config->nirq_pin		= GPIO_Pin_1;
		config->nirq_mf_pin		= (GPIO_PIN) 1;
		config->gpio_port		= GPIOA;
		config->gpio0_pin		= GPIO_Pin_3;
		config->gpio1_pin		= GPIO_Pin_8;
		config->cs_port			= GPIOA;
		config->cs_pin			= GPIO_Pin_4;
		config->sclk_pin		= GPIO_Pin_13;
		config->miso_pin		= GPIO_Pin_14;
		config->mosi_pin		= GPIO_Pin_15;
		config->sdn_port		= GPIOA;
		config->sdn_pin			= GPIO_Pin_2;
		config->spi_rcc			= RCC_APB1Periph_SPI2;
		si446x_debug_print(DEBUG02, "SI446X: Using .NOW Hardware Config\r\n");
	}
}

// FIXME: Hard-coded to si4468 and SPI2 for the moment.
DeviceStatus si446x_hal_init(RadioEventHandler *event_handler, UINT8 radio, UINT8 mac_id) {

	DeviceStatus ret = DS_Success;
	int reset_errors;
	uint8_t temp;
	radio_lock_id_t owner;

	// Set up debugging output
	si446x_set_debug_print(si446x_debug_print, si4468x_debug_level);
	si446x_debug_print(DEBUG02, "SI446X: si446x_hal_init()\r\n");

	if ( owner = si446x_spi_lock(radio_lock_init) ) {
		si446x_debug_print(DEBUG02, "SI446X: si446x_hal_init() FAIL, SPI busy???\r\n");
		return DS_Fail;
	}

	choose_hardware_config(am_i_wwf(), &SI446X_pin_setup);

	// Default settings
	si446x_channel = si446x_default_channel;
	tx_power = si446x_default_power;
	tx_msg_ptr = &tx_msg;
	rx_msg_ptr = &rx_msg;
	initSPI2();
	init_si446x_pins();

	si446x_reset();
	reset_errors  = si446x_part_info();
	reset_errors += si446x_func_info();
	si446x_get_int_status(0x0, 0x0, 0x0); // Saves status and clears all interrupts
	si446x_request_device_state();
	si446x_debug_print(DEBUG01, "SI446X: Radio Interrupts Cleared\r\n");

	temp = si446x_get_property(0x00, 0x01, 0x03);
	if (temp != RF_GLOBAL_CONFIG_1_1) {
		si446x_debug_print(ERR99, "SI446X: si446x_hal_init GLOBAL_CONFIG Setting Looks Wrong... Overriding...\r\n");
		si446x_set_property( 0x00, 0x01, 0x03, RF_GLOBAL_CONFIG_1_1 );
	}

	temp = si446x_get_property(0x12, 0x01, 0x08);
	if (temp != PKT_LEN) {
		si446x_debug_print(ERR99, "SI446X: si446x_hal_init PKT_LEN Setting Looks Wrong...Overriding...\r\n");
		si446x_set_property( 0x12, 0x01, 0x08, PKT_LEN );
	}

	temp = si446x_get_property(0x12, 0x01, 0x12);
	if (temp != PKT_FIELD_2_LENGTH) {
		si446x_debug_print(ERR99, "SI446X: si446x_hal_init PKT_FIELD_2_LENGTH Setting Looks Wrong...Overriding...\r\n");
		si446x_set_property( 0x12, 0x01, 0x12, PKT_FIELD_2_LENGTH );
	}

	si446x_fifo_info(0x3); // Reset both FIFOs. bit1 RX, bit0 TX
	si446x_debug_print(DEBUG01, "SI446X: Radio RX/TX FIFOs Cleared\r\n");

	isInit = 1;

	if ( reset_errors ) {
		ret = DS_Fail;
		si446x_debug_print(ERR100, "SI446X: si446x_hal_init(): reset failed.\r\n");
		goto si446x_hal_init_CLEANUP;
	}

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
	CPU_GPIO_EnableInputPin( SI446X_pin_setup.nirq_mf_pin, FALSE, si446x_spi2_handle_interrupt, GPIO_INT_EDGE_LOW, RESISTOR_DISABLED);
	tx_callback_continuation.InitializeCallback(tx_cont_do, NULL);
	rx_callback_continuation.InitializeCallback(rx_cont_do, NULL);
	int_defer_continuation.InitializeCallback(int_cont_do, NULL);

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

	CPU_GPIO_DisablePin(SI446X_pin_setup.nirq_mf_pin, RESISTOR_DISABLED, 0, GPIO_ALT_PRIMARY); // Only PIN matters
	radio_si446x_spi2.SetInitialized(FALSE);
	ASSERT((active_mac_index & 0xFF00) == 0); // ??? copied from RF231 driver

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

// eventTime is ignored unless doTS (USE_TIMESTAMP) is set.
DeviceStatus si446x_packet_send(uint8_t chan, uint8_t *pkt, uint8_t len, UINT32 eventTime, int doTS) {
	uint8_t tx_buf[si446x_packet_size+1]; // Add one for packet size field
	radio_lock_id_t owner;

	DeviceStatus ret = DS_Success;

	si446x_debug_print(DEBUG02, "SI446X: si446x_packet_send() size:%d doTs:%d\r\n", len, doTS);
	si446x_debug_print(DEBUG01, "\tcontents: %s\r\n", (char *)pkt);

	// First check if we have a previous send done and waiting to be processed
	if (tx_callback_continuation.IsLinked()) {
#		ifdef SI4468_FORCE_MAC_SERVICE
		si446x_debug_print(ERR100, "SI446X: si446x_packet_send() ERROR! Previously completed radio op not serviced. New request denied.\r\n");
		return DS_Bug;
#		endif
		// Lock and check again before we remove it.
		GLOBAL_LOCK(irq);
		if (tx_callback_continuation.IsLinked()) {
			tx_callback_continuation.Abort();
			irq.Release();
			si446x_debug_print(DEBUG02, "SI446X: si446x_packet_send() Warning! TX attempt before previous TX serviced.\r\n");
			si446x_debug_print(DEBUG02, "SI446X: si446x_packet_send() ...Servicing previous TX before continuing\r\n");
			tx_cont_do(NULL);
		} else {
			irq.Release();
		}
	}

	if (rx_callback_continuation.IsLinked()) {
#		ifdef SI4468_FORCE_MAC_SERVICE
		si446x_debug_print(ERR100, "SI446X: si446x_packet_send() ERROR! Previously completed radio op not serviced. New request denied.\r\n");
		return DS_Bug;
#		endif
		// Lock and check again before we remove it.
		GLOBAL_LOCK(irq);
		if (rx_callback_continuation.IsLinked()) {
			rx_callback_continuation.Abort();
			irq.Release();
			si446x_debug_print(DEBUG02, "SI446X: si446x_packet_send() Warning! TX attempt before previous RX serviced.\r\n");
			si446x_debug_print(DEBUG02, "SI446X: si446x_packet_send() ...Servicing previous before continuing...\r\n");
			rx_cont_do(NULL);
		} else {
			irq.Release();
		}
	}

	if ( len > si446x_packet_size || (doTS && (len > si446x_payload_ts)) ) {
		si446x_debug_print(ERR99, "SI446X: si446x_packet_send() Fail. Packet Too Large.\r\n");
		return DS_Timeout; // Cheating here. Not really a timeout but up a layer will return NO_BadPacket. TODO: Make proper.
	}

	if ( !isInit ) {
		si446x_debug_print(ERR99, "SI446X: si446x_packet_send() Fail. Not initialized.\r\n");
		return DS_Fail;
	}

	if ( owner = si446x_spi_lock(radio_lock_tx) ) 	{
		si446x_debug_print(DEBUG02, "SI446X: si446x_packet_send() Fail. SPI locked, op in progress.\r\n");
		return DS_Busy;
	}

	// Start state change before we have radio lock.
	// Done early to kick radio out of RX before we get the lock
	si446x_change_state(SI_STATE_TX_TUNE);

	// Lock radio until TX is done.
	if ( owner = si446x_radio_lock(radio_lock_tx) ) 	{
		si446x_debug_print(DEBUG02, "SI446X: si446x_packet_send() Fail. Radio Op %d in progress.\r\n", owner);
		ret = DS_Busy;
		goto si446x_packet_send_CLEANUP;
	}

	tx_buf[0] = len;
	if (doTS) { tx_buf[0] += 4; } // Add timestamp to packet size if used.
	memcpy(&tx_buf[1], pkt, len);

	si446x_write_tx_fifo(len+1, tx_buf); // add one for packet size


	if (doTS) { // Timestamp Case
		GLOBAL_LOCK(irq);

		while( si446x_request_device_state() != SI_STATE_TX_TUNE ) ; // spin. TODO: Add timeout.

		UINT32 eventOffset = (HAL_Time_CurrentTicks() & 0xFFFFFFFF) - eventTime;

		si446x_write_tx_fifo(4, (uint8_t*)&eventOffset); // generate and write timestamp late as possible.
		si446x_start_tx(chan, SI446x_TX_DONE_STATE, tx_buf[0]+1);
		irq.Release();
	} else { // Normal Case
		si446x_start_tx(chan, 0, tx_buf[0]+1);
	}

si446x_packet_send_CLEANUP:

	si446x_spi_unlock();
	// RADIO STAYS LOCKED UNTIL TX DONE

	return ret;
}

void *si446x_hal_send(UINT8 radioID, void *msg, UINT16 size) {
	si446x_debug_print(DEBUG02, "SI446X: si446x_hal_send()\r\n");

	DeviceStatus ret;

	// Do the send
	ret = si446x_packet_send(si446x_channel, (uint8_t *) msg, size, 0, NO_TIMESTAMP);
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

	return temp;
}

void *si446x_hal_send_ts(UINT8 radioID, void *msg, UINT16 size, UINT32 eventTime) {
	si446x_debug_print(DEBUG02, "SI446X: si446x_hal_send_ts()\r\n");

	DeviceStatus ret;

	// Do the send
	ret = si446x_packet_send(si446x_channel, (uint8_t *) msg, size, eventTime, YES_TIMESTAMP);
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

	return temp;
}

// Does NOT set the radio busy unless a packet comes in.
DeviceStatus si446x_hal_rx(UINT8 radioID) {
	radio_lock_id_t owner;
	si446x_debug_print(DEBUG02, "SI446X: si446x_hal_rx()\r\n");

	if (!isInit) {
		si446x_debug_print(DEBUG01, "SI446X: si446x_hal_rx() FAIL. Not Init.\r\n");
		return DS_Fail;
	}

	// First check if we have a previous TX/RX done and waiting to be processed
	if (tx_callback_continuation.IsLinked()) {
#		ifdef SI4468_FORCE_MAC_SERVICE
		si446x_debug_print(ERR100, "SI446X: si446x_hal_rx() ERROR! Previously completed radio op not serviced. New request denied.\r\n");
		return DS_Bug;
#		endif
		// Lock and check again before we remove it.
		GLOBAL_LOCK(irq);
		if (tx_callback_continuation.IsLinked()) {
			tx_callback_continuation.Abort();
			irq.Release();
			si446x_debug_print(DEBUG02, "SI446X: si446x_hal_rx() Warning! RX attempt before previous TX serviced.\r\n");
			si446x_debug_print(DEBUG02, "SI446X: si446x_hal_rx() ...Servicing previous before continuing...\r\n");
			tx_cont_do(NULL);
		} else {
			irq.Release();
		}
	}

	if (rx_callback_continuation.IsLinked()) {
#		ifdef SI4468_FORCE_MAC_SERVICE
		si446x_debug_print(ERR100, "SI446X: si446x_hal_rx() ERROR! Previously completed radio op not serviced. New request denied.\r\n");
		return DS_Bug;
#		endif
		// Lock and check again before we remove it.
		GLOBAL_LOCK(irq);
		if (rx_callback_continuation.IsLinked()) {
			rx_callback_continuation.Abort();
			irq.Release();
			si446x_debug_print(DEBUG02, "SI446X: si446x_hal_rx() Warning! RX attempt before previous RX serviced.\r\n");
			si446x_debug_print(DEBUG02, "SI446X: si446x_hal_rx() ...Servicing previous before continuing...\r\n");
			rx_cont_do(NULL);
		} else {
			irq.Release();
		}
	}


	if ( owner = si446x_spi_lock(radio_lock_rx) ) {
		si446x_debug_print(DEBUG01, "SI446X: si446x_hal_rx() FAIL. SPI locked.\r\n");
		return DS_Fail;
	}

	si_state_t state = si446x_request_device_state();

	// We were already in RX, no need to do anything.
	if (state == SI_STATE_RX) {
		si446x_spi_unlock();
		return DS_Success;
	}

	// Supposedly this property is squirrely and likes to mutate
	// This should be temp1==0 temp2==1 for 1-byte length field
	// Give it a couple of days, if not an issue can remove it.
	{
		uint8_t temp[2];
		si446x_get_property_multi(0x12, 0x02, 0x0D, temp);
		if (temp[0] != 0 || temp[1] != 1) {
			si446x_debug_print(ERR100, "SI446X: si446x_hal_rx() WARNING, PKT_FIELD_1_LENGTH mutation! Tell Nathan\r\n");
		}
	}

	si446x_start_rx_fast_channel(si446x_channel);
	si446x_spi_unlock();
	return DS_Success;
}


DeviceStatus si446x_hal_sleep(UINT8 radioID) {
	radio_lock_id_t owner;
	si446x_debug_print(DEBUG01, "SI446X: si446x_hal_sleep()\r\n");

	if (!isInit) {
		si446x_debug_print(DEBUG02, "SI446X: si446x_hal_sleep() FAIL. No Init.\r\n");
		return DS_Fail;
	}

	if ( owner = si446x_spi_lock(radio_lock_sleep) ) {
		si446x_debug_print(DEBUG02, "SI446X: si446x_hal_sleep() FAIL. SPI locked.\r\n");
		return DS_Fail;
	}

	if ( owner = si446x_radio_lock(radio_lock_sleep) ) {
		si446x_debug_print(DEBUG02, "SI446X: si446x_hal_sleep() FAIL. Radio Busy.\r\n");
		si446x_spi_unlock();
		return DS_Fail;
	}

	si446x_change_state(SI_STATE_SLEEP);

	si446x_radio_unlock();
	si446x_spi_unlock();

	return DS_Success;
}


DeviceStatus si446x_hal_tx_power(UINT8 radioID, int pwr) {
	radio_lock_id_t owner;
	si446x_debug_print(DEBUG02, "SI446X: si446x_hal_tx_power()\r\n");

	if (pwr < 0) return DS_Fail; // Not worth of a printf

	if ( owner = si446x_spi_lock(radio_lock_tx_power) ) {
		si446x_debug_print(DEBUG02, "SI446X: si446x_hal_tx_power() FAIL. SPI locked.\r\n");
		return DS_Fail;
	}

	if (!isInit) {
		si446x_debug_print(DEBUG02, "SI446X: si446x_hal_tx_power() FAIL. No Init.\r\n");
		si446x_spi_unlock();
		return DS_Fail;
	}

	if ( owner = si446x_radio_lock(radio_lock_tx_power) ) {
		si446x_debug_print(DEBUG02, "SI446X: si446x_hal_tx_power() FAIL. Radio Busy.\r\n");
		si446x_spi_unlock();
		return DS_Fail;
	}

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
		si446x_debug_print(DEBUG01, "SI446X: si446x_hal_set_channel() FAIL. SPI locked.\r\n");
		return DS_Fail;
	}

	if (!isInit) {
		si446x_debug_print(DEBUG01, "SI446X: si446x_hal_set_channel() FAIL. No Init.\r\n");
		si446x_spi_unlock();
		return DS_Fail;
	}

	if ( owner = si446x_radio_lock(radio_lock_set_channel) ) {
		si446x_debug_print(DEBUG01, "SI446X: si446x_hal_set_channel() FAIL. Radio Busy.\r\n");
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

DeviceStatus si446x_hal_cca_ms(UINT8 radioID, UINT32 ms) {
	radio_lock_id_t owner;
	si446x_debug_print(DEBUG02, "SI446X: si446x_hal_cca_ms() ms:%d\r\n",ms);

	DeviceStatus ret;

	if (!isInit) {
		si446x_debug_print(DEBUG02, "SI446X: si446x_hal_cca_ms() FAIL. No Init.\r\n");
		return DS_Fail;
	}

	if ( owner = si446x_spi_lock(radio_lock_cca_ms) ) {
		si446x_debug_print(DEBUG02, "SI446X: si446x_hal_cca_ms() FAIL. SPI locked.\r\n");
		return DS_Fail;
	}

	if ( owner = si446x_radio_lock(radio_lock_cca_ms) ) {
		si446x_debug_print(DEBUG02, "SI446X: si446x_hal_cca_ms() FAIL. Radio Busy.\r\n");
		si446x_spi_unlock();
		return DS_Fail;
	}

	if ( rx_callback_continuation.IsLinked() || tx_callback_continuation.IsLinked() ) {
		si446x_debug_print(DEBUG02, "SI446X: si446x_hal_cca_ms() FAIL. Continuation Pending.\r\n");
		si446x_radio_unlock();
		si446x_spi_unlock();
		return DS_Fail;
	}

	si_state_t state = si446x_request_device_state();

	// Check for bad state while we're at it.
	if (state == SI_STATE_TX || state == SI_STATE_TX_TUNE || state == SI_STATE_ERROR) {
		si446x_debug_print(ERR100, "SI446X: si446x_hal_cca_ms(): Warning, impossible state.\r\n");
		si446x_radio_unlock();
		si446x_spi_unlock();
		return DS_Fail;
	}

	// If radio is already in RX, stay in RX.
	// If a packet comes in, shouldn't lose it.
	if ( state == SI_STATE_RX_TUNE || state == SI_STATE_RX ) {
		bool debug_print = false;
		while (state == SI_STATE_RX_TUNE) { state = si446x_request_device_state(); }
		HAL_Time_Sleep_MicroSeconds(ms);
		si446x_get_modem_status( 0xFF );
		if (si446x_get_current_rssi() >= si446x_rssi_cca_thresh)
			ret = DS_Busy;
		else
			ret = DS_Success;

		// Its possible we got a packet during CCA, if so, directly move the lock to RX.
		GLOBAL_LOCK(irq);
		if (!rx_callback_continuation.IsLinked())
			si446x_radio_unlock();
		else {
			radio_lock = (uint32_t)radio_lock_rx;
			debug_print = true;
		}
		si446x_spi_unlock();
		irq.Release();
		if (debug_print) si446x_debug_print(DEBUG02, "SI446X: si446x_hal_cca_ms(): CCA passed lock to RX\r\n");
		return ret;
	}

	// Otherwise we were asleep and need to stay asleep
	uint8_t int_enable;
	int_enable = si446x_get_property(0x01, 1, 0); 	// save interrupt states
	si446x_set_property(0x01, 1, 0, 0); 			// Writes 0, disables all interrupts

	si446x_get_int_status(0xFF, 0xFF, 0xFF); 		// Check interrupts, does NOT clear.

	// If there are any interrupts pending at this point, back-off
	// TX continuation check may be not necessary.
	if (si446x_get_ph_pend() || si446x_get_modem_pend() || rx_callback_continuation.IsLinked() || tx_callback_continuation.IsLinked()) {
		si446x_debug_print(DEBUG02, "SI446X: si446x_hal_cca_ms(): Radio not idle, aborting CCA\r\n");
		si446x_radio_unlock();
		si446x_spi_unlock();
		si446x_set_property(0x01, 1, 0, int_enable); 	// Re-enables interrupts
		return DS_Fail;
	}

	si446x_start_rx_fast_channel(si446x_channel);
	while (state != SI_STATE_RX) { state = si446x_request_device_state(); }
	HAL_Time_Sleep_MicroSeconds(ms);
	si446x_get_modem_status( 0xFF );
	if (si446x_get_current_rssi() >= si446x_rssi_cca_thresh)
		ret = DS_Busy;
	else
		ret = DS_Success;

	si446x_change_state(SI_STATE_SPI_ACTIVE); 		// disables RX
	si446x_fifo_info(0x3); 							// Clear FIFO in case we caught something.
	si446x_set_property(0x01, 1, 0, int_enable); 	// Re-enables interrupts
	si446x_change_state(SI_STATE_SLEEP); 			// All done, sleep.

	si446x_radio_unlock();
	si446x_spi_unlock();

	return ret;
}

UINT32 si446x_hal_get_chan(UINT8 radioID) {
	si446x_debug_print(DEBUG02, "SI446X: si446x_hal_get_chan()\r\n");
	return si446x_channel;
}


UINT32 si446x_hal_get_power(UINT8 radioID) {
	si446x_debug_print(DEBUG02, "SI446X: si446x_hal_get_power()\r\n");
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
	si446x_debug_print(DEBUG02, "SI446X: si446x_pkt_tx_int()\r\n");
	tx_cont_do(NULL);
	//tx_callback_continuation.Enqueue();
}

// INTERRUPT CONTEXT. LOCKED, radio_busy until we pull from continuation
static void si446x_pkt_rx_int() {
	radio_lock_id_t owner;
	if (owner = si446x_radio_lock(radio_lock_rx)) {
		if (owner == radio_lock_cca_ms) {
			// This is an exception, getting a RX here is sort-of OK. We will assume CCA will keep the lock for us.
			si446x_debug_print(DEBUG02,"SI446X: si446x_pkt_rx_int(): Packet arrived during CCA. CCA will keep lock. This is OK.\r\n");
		}
		else {
			si446x_debug_print(ERR99,\
				"SI446X: si446x_pkt_rx_int(): Packet arrived but unexpected lock by %d. Unstable operation likely.\r\n", owner);
			SOFT_BREAKPOINT();
		}
	}
	si446x_debug_print(DEBUG01, "SI446X: si446x_pkt_rx_int()\r\n");
	rx_cont_do(NULL);
	//rx_callback_continuation.Enqueue();
}

// INTERRUPT CONTEXT, LOCKED
static void si446x_pkt_bad_crc_int() {
	radio_lock_id_t owner;
	if ( (owner = si446x_spi_lock(radio_lock_crc)) == 0) { // Only branch if we DO get the lock
		si446x_fifo_info(0x3); // clear the FIFOs if we can
		si446x_debug_print(DEBUG02, "SI446X: si446x_pkt_bad_crc_int() FIFOs cleared\r\n");
		si446x_spi_unlock();
	}
	si446x_debug_print(DEBUG02, "SI446X: si446x_pkt_bad_crc_int()\r\n");
}

// INTERRUPT CONTEXT
static void si446x_spi2_handle_interrupt(GPIO_PIN Pin, BOOL PinState, void* Param)
{
	unsigned modem_pend, ph_pend;
	radio_lock_id_t owner;
	UINT64 int_ts;

	GLOBAL_LOCK(irq); // Locked due to time critical.
	int_ts = HAL_Time_CurrentTicks(); // Log RX time.
	irq.Release(); // Unlock after timestamp.

	if (Pin != SI446X_pin_setup.nirq_mf_pin) { return; }

	si446x_debug_print(DEBUG01, "SI446X: INT\r\n");

	if ( owner = si446x_spi_lock(radio_lock_interrupt) ) {
		// Damn, we got an interrupt in the middle of another transaction. Have to defer it.
		// Hope this doesn't happen much because will screw up timestamp.
		// TODO: Spend some effort to mitigate this if/when it happens.
		si446x_debug_print(ERR99, "SI446X: si446x_spi2_handle_interrupt() SPI busy during interrupt.\r\n");
		int_defer_continuation.Enqueue();
		return;
	}

	si446x_get_int_status(0x0, 0x0, 0x0); // Saves status and clears all interrupts
	ph_pend			= si446x_get_ph_pend();
	modem_pend 		= si446x_get_modem_pend();

	si446x_spi_unlock();

	// Only save timestamp if it was an RX event.
	if (modem_pend & MODEM_MASK_SYNC_DETECT)		{ rx_timestamp = int_ts; }

	if (ph_pend & PH_STATUS_MASK_PACKET_RX) 	{ si446x_pkt_rx_int(); }
	if (ph_pend & PH_STATUS_MASK_PACKET_SENT) 	{ si446x_pkt_tx_int(); }
	if (ph_pend & PH_STATUS_MASK_CRC_ERROR) 	{ si446x_pkt_bad_crc_int(); }

// NOT FOR PRODUCTION CODE
#ifdef SI4468_FORCE_CONTINUATIONS_IN_INTERRUPT_CONTEXT
	if (tx_callback_continuation.IsLinked()) {
		GLOBAL_LOCK(irq);
		if (tx_callback_continuation.IsLinked()) {
			tx_callback_continuation.Abort();
			irq.Release();
			tx_cont_do(NULL);
		} else {
			irq.Release();
		}
	}

	if (rx_callback_continuation.IsLinked()) {
		GLOBAL_LOCK(irq);
		if (rx_callback_continuation.IsLinked()) {
			rx_callback_continuation.Abort();
			irq.Release();
			rx_cont_do(NULL);
		} else {
			irq.Release();
		}
	}
#endif
}
