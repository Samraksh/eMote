#include "austere.h"

// TODO: Move power init stuff here

static HAL_CONTINUATION *pwr_cb = NULL;

// Interrupt. TODO: CHANGE ME
static void rad_power_monitor(GPIO_PIN Pin, BOOL PinState, void* Param) {
	if (pwr_cb == NULL) return;
	pwr_cb->Enqueue();
	// if (CPU_GPIO_GetPinState(39) == TRUE){
		// hal_printf("*** rad pwr good ***\r\n");
	// } else {
		// hal_printf("*** rad pwr bad ***\r\n");
	// }
}

//PC12, PC9, PB5, PC7
void print_power_supply_status(void) {
	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12)) {
		hal_printf("Radio Cap Fully Charged OK\r\n");
	}  else {
		hal_printf("Radio Cap Fully Charged FAIL\r\n");
	}
	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9)) {
		hal_printf("3.3v supply OK\r\n");
	}  else {
		hal_printf("3.3v supply FAIL\r\n");
	}
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)) {
		hal_printf("1.8v supply OK\r\n");
	}  else {
		hal_printf("1.8v supply FAIL\r\n");
	}
	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7)) {
		hal_printf("2.5v radio rail OK\r\n");
	}  else {
		hal_printf("2.5v radio rail FAIL\r\n");
	}
}

static void power_supply_reset() {
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_11 | GPIO_Pin_13; // leave out PC8 due to schematic issues
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  // Configure Inputs
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  // Configure Inputs
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // PB5 is open-drain from LTC3103
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  // Configure Inputs
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  // Radio shutdown pin
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

// AUSTERE. ONLY GPIOC and FOR IPU/IPD (i.e. not for 3.3v ctrl which uses OD logic)
static void power_supply_enable(uint16_t pin) {
	ASSERT(IS_AUSTERE_PWR_PIN(pin));
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

// AUSTERE. ONLY GPIOC and FOR IPU/IPD (i.e. not for 3.3v ctrl which uses OD logic)
static void power_supply_disable(uint16_t pin) {
	ASSERT(IS_AUSTERE_PWR_PIN(pin));
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

// 0 = Cap not rady, 1 = Cap Ready
static int get_radio_charge_status(void) {
	return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12);
}

static int get_radio_power_status(void) {
	return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7);
}

void austere_radio_shutdown(int go) {
	if (go) // turn off the radio
		GPIO_WriteBit(GPIOB, GPIO_Pin_11, Bit_SET);
	else
		GPIO_WriteBit(GPIOB, GPIO_Pin_11, Bit_RESET);
}

// PC7. This is called from TinyCLR.cpp at boot after driver init but before CLR starts.
void austere_init(void) {
	CPU_GPIO_EnableInputPin( (GPIO_PIN) 39, FALSE, rad_power_monitor, GPIO_INT_EDGE_BOTH, RESISTOR_PULLUP );
}

void austere_power_init(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	power_supply_reset();
	power_supply_enable(AUSTERE_1V8); // 1.8v rail (with the RTC clock)

	// Spin long enough for the 1.8v domain to power up. This delay is a mostly blind guess.
	for(volatile int i=0; i<106666; i++) ; // spin, maybe about 10ms ???

	// Default the 2.5v rail and its tank cap to OFF at start. Will use on demand.
	power_supply_disable(AUSTERE_BIG_CAP);
	power_supply_disable(AUSTERE_2V5);
	austere_radio_shutdown(AUSTERE_RADIO_OFF);	// Disable radio. This will be over-ridden by driver
}

int platform_power_event_sub(HAL_CONTINUATION *cb) {
	pwr_cb = cb;
	return 0;
}

// Turn the radio on/off.
// Only one radio, so second parameter is ignored.
int platform_radio_pwr_ctrl(int on, int radio) {
	if (on) {
		power_supply_enable(AUSTERE_BIG_CAP);
		power_supply_enable(AUSTERE_2V5);
	} else {
		power_supply_disable(AUSTERE_2V5);
		power_supply_disable(AUSTERE_BIG_CAP);
	}
	return 0;
}
