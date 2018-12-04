
/*

STM32F1x Microframework power HAL driver

Nathan Stohs
nathan.stohs@samraksh.com

*/

#include "netmf_pwr.h"
#include "netmf_pwr_wakelock.h"
#include "../usart/sam_usart.h"
#include "../Timer/netmf_rtc/netmf_rtc.h"

#ifdef POWER_PROFILE_HACK
#define POWER_PROFILE_RTC_WARN
#define POWER_TABLE_WRAP
#define FILTER_WAKELOCK_DENY
#define FILTER_LOCKONOFF
#define FILTER_TOO_SHORT
#define FILTER_SNOOZE
#define POWER_EVENTS_SIZE 32
#define POWER_COUNT_INIT 0

typedef __attribute__ ((packed)) enum {
	GOING_TO_SLEEP,
	WAKEUP,
	TOO_SHORT,
	WAKELOCK_ON,
	WAKELOCK_OFF,
	WAKELOCK_DENY,
	TIMEWARP,
	SNOOZE,
} power_event_enum_t;

typedef struct {
	uint32_t time;
	uint16_t data16;
	//int8_t misc1;
	power_event_enum_t evt;
} power_event_t;

static power_event_t power_events[POWER_EVENTS_SIZE];
static int power_count = POWER_COUNT_INIT;

static const char* power_evt_to_string(power_event_enum_t x) {
	switch (x) {
		case GOING_TO_SLEEP:	return "SLEEP";
		case WAKEUP:			return "WAKEUP";
		case TOO_SHORT: 		return "TOO_SHORT";
		case WAKELOCK_ON: 		return "LOCK_ON";
		case WAKELOCK_OFF: 		return "LOCK_OFF";
		case WAKELOCK_DENY: 	return "WAKELOCK_DENY";
		case TIMEWARP: 			return "TIMEWARP";
		case SNOOZE:			return "SNOOZE";
		default:				return "UNKNOWN";
	}
}

static void print_power_event(power_event_t e) {
	hal_printf("%d %d %s 0\r\n", e.time, e.data16, power_evt_to_string(e.evt) /*, e.misc1*/);
}

// Only do this on wakeup for known state
static void print_all_power_events() {
	//TIM_Cmd(TIM1, DISABLE);
	//SOFT_BREAKPOINT();
	__enable_irq();
	for(int i=0; i<POWER_EVENTS_SIZE; i++) {
		print_power_event(power_events[i]);
	}
	while(1); // never return;
}

static void power_event_sleep(uint32_t now, uint16_t wakeup, enum stm_power_modes power_state) {
	if (power_count < 0) { power_count++; return; }
	if (power_count >= POWER_EVENTS_SIZE) { return; }
	power_events[power_count].time = now;
	power_events[power_count].data16 = wakeup;
	power_events[power_count].evt = GOING_TO_SLEEP;

	// int8_t misc1;
	// switch(power_state) {
		// case POWER_STATE_DEFAULT: 		misc1 = 0; break;
		// case POWER_STATE_LOW:			misc1 = 1; break;
		// case POWER_STATE_MID:			misc1 = 2; break;
		// case POWER_STATE_HIGH:			misc1 = 3; break;
		// case POWER_STATE_GOING_SLEEP:	misc1 = 4; break;
		// default:						misc1 = 5; break;
	// }
	// power_events[power_count].misc1 = misc1;
	power_count++;
#ifdef POWER_TABLE_WRAP
	if (power_count >= POWER_EVENTS_SIZE)
		power_count=0;
#endif
}

static void power_event_add(uint32_t now, power_event_enum_t evt, uint16_t data16, int8_t extra) {
	if (power_count < 0 || power_count >= POWER_EVENTS_SIZE) { return; }
	
	switch (evt) {
#ifdef FILTER_WAKELOCK_DENY
		case WAKELOCK_DENY:
#endif
#ifdef FILTER_LOCKONOFF
		case WAKELOCK_ON:
		case WAKELOCK_OFF:
#endif
#ifdef FILTER_TOO_SHORT
		case TOO_SHORT:
#endif
#ifdef FILTER_SNOOZE
		case SNOOZE:
#endif
		return;
		default: break;
	}

	power_events[power_count].time = now;
	power_events[power_count].data16 = data16;
	power_events[power_count].evt = evt;
	//power_events[power_count].misc1 = extra;
	power_count++;
#ifdef POWER_TABLE_WRAP
	if (power_count >= POWER_EVENTS_SIZE)
		power_count=0;
#endif
}

static void power_event_wakeup(uint32_t now) {
	if (power_count < 0) { power_count++; return; }
	if (power_count >= POWER_EVENTS_SIZE) { print_all_power_events(); return; }
	power_events[power_count].time = now;
	power_events[power_count].evt = WAKEUP;
	power_count++;
#ifdef POWER_TABLE_WRAP
	if (power_count >= POWER_EVENTS_SIZE)
		power_count=0;
#endif
}

#endif

#ifdef POWER_PROFILE_RTC_WARN
#define POWER_DEBUG_WAKEUP (8*32768) // Dump debug if we don't wakeup for X seconds.
#define POWER_DEBUG_WAKEUP_OFFSET (32768*60*15) // 15 minutes
static uint32_t startup_time;
static uint32_t debug_wakeup = 0xFFFFFFFF;
static HAL_CONTINUATION power_debug_continuation;
static void power_debug_dump(void *arg) {
	hal_printf("Power Debug Watchdog: %d seconds passed without sleep\r\n", POWER_DEBUG_WAKEUP/32768);
	SOFT_BREAKPOINT();
}
#endif

// Number of RTC ticks it takes to wakeup from each power level.
// So wakeup early by this amount plus some slop
enum wakeup_ticks{
	MIN_SLEEP_TICKS = 33,
	SLEEP_EXTRA_PAD = 2,
	SLEEP_PADDING_HIGH_POWER = 9,
	SLEEP_PADDING_MID_POWER = 9,
	SLEEP_PADDING_LOW_POWER = 6,
};

#ifdef PLATFORM_ARM_AUSTERE
#include <stm32f10x.h>
#endif

static int pwr_hsi_clock_measure;
static int pwr_hsi_clock_measure_orig;
static enum stm_power_modes stm_power_state = POWER_STATE_DEFAULT;

#ifdef PLATFORM_ARM_AUSTERE
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
static void power_supply_activate(uint16_t pin) {
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = pin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}
// 0 = Cap not rady, 1 = Cap Ready
static int get_radio_charge_status(void) {
	return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12);
}
static int get_radio_power_status(void) {
	return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7);
}
static void radio_shutdown(int go) {
	if (go) // turn off the radio
		GPIO_WriteBit(GPIOB, GPIO_Pin_11, Bit_SET);
	else
		GPIO_WriteBit(GPIOB, GPIO_Pin_11, Bit_RESET);
}
#endif // PLATFORM_ARM_AUSTERE


#ifdef EMOTE_WAKELOCKS
static uint32_t wakelock;
static UINT64 waketime;

void WakeLockInit(void) {
	wakelock = 0;
#if defined (EMOTE_WAKELOCK_STARTUP) && (EMOTE_WAKELOCK_STARTUP > 0)
	waketime = HAL_Time_CurrentTicks() + CPU_MicrosecondsToTicks((UINT32)1000000 * EMOTE_WAKELOCK_STARTUP);
#else
	waketime = 0;
#endif
}

void WakeLock(uint32_t lock) {
#ifdef POWER_PROFILE_HACK
	power_event_add(RTC_GetCounter(), WAKELOCK_ON, 0, -1);
#endif
	GLOBAL_LOCK(irq);
	wakelock |= lock;
}

void WakeUnlock(uint32_t lock) {
#ifdef POWER_PROFILE_HACK
	power_event_add(RTC_GetCounter(), WAKELOCK_OFF, 0, -1);
#endif
	GLOBAL_LOCK(irq);
	wakelock &= ~lock;
}

void WakeUntil(UINT64 until) {
	UINT64 now;

	GLOBAL_LOCK(irq);

	// Value of 0 will force kill the wakelock
	if (until == 0) {
		waketime = 0;
		return;
	}

	now = HAL_Time_CurrentTicks();

	// Make sure time is valid (in the future)
	// Make sure time is not before previous time.
	if (until < now || until < waketime) {
		return;
	}

	waketime = until;
}

bool GetWakeLocked(void) {
	bool doWFI;
	ASSERT_IRQ_MUST_BE_OFF();
	if (wakelock) { doWFI = TRUE; }
	else if (waketime > 0) {
		UINT64 now = HAL_Time_CurrentTicks();
		if (waketime >= now) {
			doWFI = TRUE; // Wakelocked
		} else {
			waketime = 0;
			doWFI = FALSE;
		}
	} else { doWFI = FALSE; }
	return doWFI;
}

#else // EMOTE_WAKELOCKS
void WakeLock(uint32_t lock) {}
void WakeUnlock(uint32_t lock) {}
void WakeUntil(UINT64 until) {}
bool GetWakeLocked(void) {return false;}
void WakeLockInit(void) {}
#endif // EMOTE_WAKELOCKS

#ifdef DEBUG_DOTNOW_ISR
extern volatile unsigned interrupt_count[64];
#endif

// Doesn't have to do anything but it does cause wakeup.
extern "C" {
void __irq RTCAlarm_IRQHandler(void) {
	//SOFT_BREAKPOINT();
#ifdef DEBUG_DOTNOW_ISR
	interrupt_count[RTCAlarm_IRQn]++;
#endif
	RTC_ClearITPendingBit(RTC_IT_ALR);
	EXTI_ClearITPendingBit(EXTI_Line17);
#ifdef POWER_PROFILE_RTC_WARN
	if ( RTC_GetCounter() >= debug_wakeup ) {
		power_debug_continuation.Enqueue();
	}
#endif
}
}

static void RTC_wakeup_init(void) {
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);
	EXTI_DeInit();

	EXTI_ClearITPendingBit(EXTI_Line17);
	EXTI_InitStruct.EXTI_Line = EXTI_Line17;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	//EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Event;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);

	// Setup RTC Alarm interrupt (for wakeup)
	NVIC_InitStruct.NVIC_IRQChannel = RTCAlarm_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}

void PowerInit() {
	GLOBAL_LOCK(irq);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	BKP_DeInit();

#if !defined(BUILD_RTM) // For non-RTM flavors (e.g. Release, Debug), do not artificially raise lowest power mode. But, in flavors Debug, Instrumented ...
	if(JTAG_Attached() > 0) // ... when JTAG is attached, artificially raise lowest power mode to support JTAG connection.
	{
	DBGMCU_Config(DBGMCU_SLEEP | DBGMCU_STANDBY | DBGMCU_STOP, ENABLE);
	}
	else {
#endif
	DBGMCU_Config(DBGMCU_SLEEP | DBGMCU_STANDBY | DBGMCU_STOP, DISABLE);
#if !defined(BUILD_RTM)
	}
#endif

#if defined(DOTNOW_HSI_CALIB) && !defined(PLATFORM_ARM_AUSTERE)
	Low_Power();
	CalibrateHSI();
#endif


#ifdef PLATFORM_ARM_AUSTERE
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	power_supply_reset();
	power_supply_activate(GPIO_Pin_6); // 1.8v rail (with the RTC clock)
	// Spin long enough for the 1.8v domain to power up. This delay is a mostly blind guess.
	for(volatile int i=0; i<106666; i++) ; // spin, maybe about 10ms ???
	power_supply_activate(GPIO_Pin_11);    // Big Cap

	Low_Power();
	CalibrateHSI(); // Lets calibrate the HSI while we wait for the cap to charge.

#ifdef AUSTERE_NO_CAP_TIMEOUT
	while( get_radio_charge_status() == 0 ); 	// wait for big cap to charge
#else
	volatile int i=0;
	while(get_radio_charge_status() == 0) {
		if (i++ == 10000000) // ~20 seconds
			break;
	}
#endif

	power_supply_activate(GPIO_Pin_13);			// Turn on 2.5v rail
	radio_shutdown(1);							// Disable radio

	volatile int ii=0;
	while( get_radio_power_status() == 0 ) {		// Wait for 2.5v to stab
		if (ii++ == 10000000) // ~20 seconds
		{ ASSERT(0); break; }
	}

	//Low_Power();
	//Mid_Power(); // Would prefer 8 MHz
	High_Power();
#else
	High_Power();
#endif

	hal_printf("POWER LEVEL: HIGH\r\n");
	RCC_LSICmd(DISABLE);
	RTC_wakeup_init();

	WakeLockInit();
#ifdef POWER_PROFILE_RTC_WARN
	power_debug_continuation.InitializeCallback(power_debug_dump, NULL);
	startup_time = RTC_GetCounter();
#endif
}

static void do_hsi_measure() {
	// Re-align
	RTC_ClearFlag(RTC_FLAG_ALR);
	RTC_SetAlarm(RTC_GetCounter()+1);
	while( RTC_GetFlagStatus(RTC_FLAG_ALR) == RESET );

	// Do the measurement
	TIM6->CNT=0;
	RTC_ClearFlag(RTC_FLAG_ALR);
	RTC_SetAlarm(RTC_GetCounter()+1);
	while( RTC_GetFlagStatus(RTC_FLAG_ALR) == RESET );
	pwr_hsi_clock_measure = (TIM6->CNT << 7);
	RTC_ClearFlag(RTC_FLAG_ALR);
}

static void align_to_rtc() {
	uint32_t now = RTC_GetCounter();

	RTC_SetAlarm(++now);

	// Make sure we weren't straddling the RTC clock edge
	if (now == RTC_GetCounter()) {
		RTC_ClearFlag(RTC_FLAG_ALR);
		return;
	}

	// Spin until aligned.
	while( RTC_GetFlagStatus(RTC_FLAG_ALR) == RESET );
	RTC_ClearFlag(RTC_FLAG_ALR);
}

// Returns measured HSI speed if the calibrate function was used.
UINT32 pwr_get_hsi(int x) {
#ifdef DOTNOW_HSI_CALIB
	uint32_t ret;
	switch(x) {
		case 0: ret = pwr_hsi_clock_measure_orig; break;
		case 1: ret = pwr_hsi_clock_measure; break;
		case 2:
		default: ret = PWR_HSI_SPEED;
	}
	return ret;
#else
	return 0;
#endif
}

static int trim_test(uint32_t *hsi_trim_error, uint32_t *hsi_trim_val, uint32_t trim) {
	if (hsi_trim_error[trim] == 0xFFFFFFFF && PWR_HSI_TRIM_VALID(trim)) {
		RCC_AdjustHSICalibrationValue(trim);
		do_hsi_measure();
		//hsi_trim_error[trim] = PWR_ABS( (int)PWR_HSI_SPEED - (int)pwr_hsi_clock_measure );
		hsi_trim_error[trim] = abs( PWR_HSI_SPEED - pwr_hsi_clock_measure );
		hsi_trim_val[trim] = pwr_hsi_clock_measure;
		if (hsi_trim_error[trim] < PWR_HSI_CLOSE_ENOUGH) {
			return 1;
		}
	}
	return 0;
}

// Currently this is only safe to do at startup.
// Not tested for mid program execution.
// No longer uses interrupts. Also Mike is a Farty-pants (and rumor has it a BIG JERK).
void CalibrateHSI() {
	uint32_t trim;
	int err  = 0;
	int skip = 0; 					// if we get close enough, skip rest of tests.
	uint32_t hsi_trim_error[32];	// Memoization table of absolute frequency error vs. TRIM
	uint32_t hsi_trim_val[32];		// Memoization table of absolute frequency vs TRIM

	if (stm_power_state != POWER_STATE_LOW) {
		return; // Must be done from low power mode.
	}

	// Reset tables
	memset(hsi_trim_error, 0xFFFFFFFF, sizeof(hsi_trim_error));
	memset(hsi_trim_val  , 0x00000000, sizeof(hsi_trim_val));

	// First make sure the RTC is running, we will use that to measure.
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	RTC_SetPrescaler(0x0FFF); // 1/8th second
	//RTC_SetPrescaler(0x0F41); // 1/8th second. Delete me. For a temporary gimped board.
#ifdef PLATFORM_ARM_AUSTERE
	RCC_LSEConfig(RCC_LSE_Bypass);
#else
	RCC_LSEConfig(RCC_LSE_ON);
#endif
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) {
		if (err++ == PWR_RTC_TIMEOUT) {
			ASSERT(0);
			return; // Crystal not starting. Give up.
		}
	}
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	RCC_RTCCLKCmd(ENABLE);
	RTC_WaitForLastTask();
	RTC_WaitForSynchro();
	RTC_WaitForLastTask();
	RCC_LSICmd(DISABLE);

	pwr_hsi_clock_measure = 0;
	pwr_hsi_clock_measure_orig = 0;

	// Setup TIM6 to do the actual timing
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	TIM_DeInit(TIM6);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
	TIM_TimeBaseInitStruct.TIM_Prescaler = 15;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);

	TIM_Cmd(TIM6, ENABLE);

	trim = BKP_ReadBackupRegister(BKP_DR1); // backup register 1 contains last good TRIM value
	if ( !PWR_HSI_TRIM_VALID(trim) ) {
		trim = PWR_HSI_DEFAULT_TRIM;
	}

	// Wait 1 tick so we are reasonably sure we are close to RTC clock edge. Keeps things clean.
	align_to_rtc();

	// Find trim with min error.
	// Note implicit assumptions about clock speed and TRIM relationship (not random).
	// STARTING GUESS MUST BE GOOD. DOES NOT WORK FROM ANY GIVEN BASE. THERE ARE LOCAL MINIMA.
	// SO WE MUST ALWAYS CHECK DEFAULT AS A STARTING "GOOD" GUESS.

	// Test Hardware defaults (trim==16)
	skip = trim_test(hsi_trim_error, hsi_trim_val, PWR_HSI_DEFAULT_TRIM);

	while( PWR_HSI_TRIM_VALID(trim) ) {
		unsigned min_trim = trim;

		if (!skip) { skip = trim_test(hsi_trim_error, hsi_trim_val, trim);   }
		if (!skip) { skip = trim_test(hsi_trim_error, hsi_trim_val, trim-1); }
		if (!skip) { skip = trim_test(hsi_trim_error, hsi_trim_val, trim+1); }

		// Find min error
		if (hsi_trim_error[min_trim] > hsi_trim_error[trim+1]) {
			min_trim = trim+1;
		}
		else if (hsi_trim_error[min_trim] > hsi_trim_error[trim-1]) {
			min_trim = trim-1;
		}

		// Always sanity check against default value due to local-minima problem
		// Must loop at least one more time as well.
		if (hsi_trim_error[min_trim] > hsi_trim_error[PWR_HSI_DEFAULT_TRIM]) {
			min_trim = PWR_HSI_DEFAULT_TRIM;
		}
		else if (min_trim == trim || skip || PWR_HSI_TRIM_MIN_OR_MAX(min_trim) ) {
			RCC_AdjustHSICalibrationValue(min_trim);
			pwr_hsi_clock_measure = hsi_trim_val[min_trim];
			BKP_WriteBackupRegister(BKP_DR1, (uint16_t)min_trim);
			break;
		}
		// go around and try again
		trim = min_trim;
	}

	pwr_hsi_clock_measure_orig = hsi_trim_val[PWR_HSI_DEFAULT_TRIM];

	// Clean up
	TIM_DeInit(TIM6);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, DISABLE);
	PWR_BackupAccessCmd(DISABLE);
	// Turn off RTC since not used currently.
	RCC_RTCCLKCmd(DISABLE);
	RCC_LSEConfig(RCC_LSE_OFF);
	// Leave PWR module ON.
	RCC_APB1PeriphClockCmd(/*RCC_APB1Periph_PWR |*/ RCC_APB1Periph_BKP, DISABLE);
}

// Block and pause until clock-sensitive peripherals are ready.
// Add to this list as needed.
static void pause_peripherals(void) {
	USART_pause();
}

// Just like Low power but doesn't re-init
static void Sleep_Power(void) {
	// Make sure actually changing
	if (stm_power_state == POWER_STATE_GOING_SLEEP) {
		return;
	}

	pause_peripherals();

	// Set HSI (instead of PLL) as SYSCLK source
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);

	// Spin waiting for HSI to be used.
	while ( RCC_GetSYSCLKSource() != 0x00 ) { ; }

	// Disable (now unused) PLL
	RCC_PLLCmd(DISABLE);

	// Set Bus speeds
	RCC_HCLKConfig(RCC_SYSCLK_Div1);  // 8 MHz
	RCC_PCLK1Config(RCC_HCLK_Div1);   // 8 MHz
	RCC_PCLK2Config(RCC_HCLK_Div1);   // 8 MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div2); // 4 MHz

	// Set flash speeds
	FLASH_SetLatency(FLASH_Latency_0);

	stm_power_state = POWER_STATE_GOING_SLEEP;
}

void Low_Power() {

	// Make sure actually changing
	if (stm_power_state == POWER_STATE_LOW) {
		return;
	}

	pause_peripherals();

	// Set HSI (instead of PLL) as SYSCLK source
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);

	// Spin waiting for HSI to be used.
	while ( RCC_GetSYSCLKSource() != 0x00 ) { ; }

	// Disable (now unused) PLL
	RCC_PLLCmd(DISABLE);

	// Set Bus speeds
	RCC_HCLKConfig(RCC_SYSCLK_Div1);  // 8 MHz
	RCC_PCLK1Config(RCC_HCLK_Div1);   // 8 MHz
	RCC_PCLK2Config(RCC_HCLK_Div1);   // 8 MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div2); // 4 MHz

	// Set timer prescaler for constant 8 MHz
	// Very not tested
	TIM_PrescalerConfig(TIM1, 0, TIM_PSCReloadMode_Immediate);

	// Set flash speeds
	FLASH_SetLatency(FLASH_Latency_0);

	stm_power_state = POWER_STATE_LOW;

	USART_reinit(); // Clock sensitive. Must redo.
}

void Mid_Power() {

	// Make sure actually changing
	if (stm_power_state == POWER_STATE_MID) {
		return;
	}

	pause_peripherals();

	// Set HSI (instead of PLL) as SYSCLK source
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	while ( RCC_GetSYSCLKSource() != 0x00 ) { ; }
	RCC_PLLCmd(DISABLE);

	// Setup PLL for 8/2*6 = 24 MHz
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_6);

	// Set timer prescaler for constant 8 MHz
	// Only TIM1 needed, TIM2 is slave
	// PCLK @ 24 MHz, TIM runs x1, so /3
	TIM_PrescalerConfig(TIM1, 2, TIM_PSCReloadMode_Immediate);

	// Set Bus Speeds
	RCC_HCLKConfig(RCC_SYSCLK_Div1);  // 24 MHz
	RCC_PCLK1Config(RCC_HCLK_Div1);   // 24 MHz
	RCC_PCLK2Config(RCC_HCLK_Div1);   // 24 MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div4); // 6 MHz

	// This is confusing, pay attention:
	// If the PCLK1/2 prescaler != 1
	// Then the associated TIM clock is x2
	// so PCLK @ 48/2 MHz really means TIM @ 48 MHz.

	// Set flash speeds
	FLASH_SetLatency(FLASH_Latency_0);

	// Enable PLL and spin waiting for PLL ready
	RCC_PLLCmd(ENABLE);
	while ( RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET ) { ; }

	// Set PLL as clock source and spin waiting for active
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while ( RCC_GetSYSCLKSource() != 0x08 ) { ; }

	stm_power_state = POWER_STATE_MID;

	USART_reinit(); // Clock sensitive. Must redo.
}

void High_Power() {

	// Make sure actually changing
	if (stm_power_state == POWER_STATE_HIGH) {
		return;
	}

	pause_peripherals();

	// Set HSI (instead of PLL) as SYSCLK source
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	while ( RCC_GetSYSCLKSource() != 0x00 ) { ; }
	RCC_PLLCmd(DISABLE);

	// Setup PLL for 8/2*16 = 64 MHz
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_16);

	// Set timer prescaler for constant 8 MHz
	// Only TIM1 needed, TIM2 is slave
	// PCLK @ 32 MHz, TIM runs x2, so /8
	TIM_PrescalerConfig(TIM1, 7, TIM_PSCReloadMode_Immediate);

	// Set Bus Speeds
	RCC_HCLKConfig(RCC_SYSCLK_Div1);  // 64 MHz
	RCC_PCLK1Config(RCC_HCLK_Div2);   // 32 MHz
	RCC_PCLK2Config(RCC_HCLK_Div2);   // 32 MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); // 10.67 MHz

	// This is confusing, pay attention:
	// If the PCLK1/2 prescaler != 1
	// Then the associated TIM clock is x2
	// so PCLK @ 48/2 MHz really means TIM @ 48 MHz.

	// Set flash speed
	FLASH_SetLatency(FLASH_Latency_2);

	// Enable PLL and spin waiting for PLL ready
	RCC_PLLCmd(ENABLE);
	while ( RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET ) { ; }

	// Set PLL as clock source and spin waiting for active
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while ( RCC_GetSYSCLKSource() != 0x08 ) { ; }

	stm_power_state = POWER_STATE_HIGH;

	USART_reinit(); // Clock sensitive. Must redo.
}

UINT8 Get_CurrentPower(){
	return stm_power_state;
}
static bool check_pending_isr(void) {
	const volatile unsigned icsr = *((volatile unsigned *)0xE000ED04);
	const unsigned ISRPENDING_MASK = 1<<22;
	return (icsr & ISRPENDING_MASK);
}

static bool usart_tx_busy(void) {
	bool ret = false;
	for (int i=0; i<TOTAL_USART_PORT; i++) {
		ret = ret | CPU_USART_TxBufferEmptyInterruptState(i);
	}
	return ret;
}

// Returns the first IRQn which is active.
// *probably* the one that woke us up, but impossible to tell.
static IRQn_Type get_first_irq(void) {
	for (int i=0; i<60; i++) {
		if (NVIC_GetPendingIRQ((IRQn_Type)i) == 1)
			return (IRQn_Type)i;
	}
	return (IRQn_Type)0xFFFF; // not valid
}

// Only to be called with interrupts disabled
static uint32_t align_to_rtc2(void) {
	uint32_t now = RTC_GetCounter();
	while (RTC_GetCounter() == now) ; // align by waiting until we tick
	return now+1;
}

void Snooze() {
#ifdef POWER_PROFILE_HACK
	power_event_add(RTC_GetCounter(), SNOOZE, 0, -1);
#endif
	__DSB();
	__WFI();
}

// Exit in the same power state as we entered.
// TODO: Need to clean this up...
void Sleep() {

	//USART_Flush(0); // Doesn't work when we're locked, derp
	// No deep sleep while TX bytes are in the queue.
	if ( usart_tx_busy() ) {
		__DSB();
		__WFI();
		return;
	}

#ifdef EMOTE_WAKELOCKS
	if ( GetWakeLocked() ) { // If wakelocked, use snooze mode.
#ifdef POWER_PROFILE_HACK
		power_event_add(RTC_GetCounter(), WAKELOCK_DENY, 0, -1);
#endif
		__DSB();
		__WFI();
		return; // Sleep completed, done here.
	}
#endif // EMOTE_WAKELOCKS

	NVIC_SystemLPConfig(NVIC_LP_SEVONPEND, ENABLE);
	ASSERT_IRQ_MUST_BE_OFF();
	//GLOBAL_LOCK(irq); // Should already be locked from caller

	if (check_pending_isr()) { // Must ensure something didn't slip in
		NVIC_SystemLPConfig(NVIC_LP_SEVONPEND, DISABLE);
		__SEV();
		__WFE();
		return;
	}

	uint32_t now; // 32-bits can store about 90 days worth of ticks
	uint32_t aft; // post sleep RTC time;
	uint32_t wakeup_time; // when the RTC is supposed to wake us up.

	// Abort if our wakeup time is too soon
	now = RTC_GetCounter();
	wakeup_time = g_STM32F10x_RTC.GetCompare();
	if(wakeup_time < now+MIN_SLEEP_TICKS) {
		// Abort!
		SOFT_BREAKPOINT();
		NVIC_SystemLPConfig(NVIC_LP_SEVONPEND, DISABLE);
		__SEV();
		__WFE();
#ifdef POWER_PROFILE_HACK
		if ( wakeup_time < now )
			power_event_add(now, TIMEWARP, now-wakeup_time, -1);
		else
			power_event_add(now, TOO_SHORT, wakeup_time-now, -1);
#endif
		return;
	}

#ifdef POWER_PROFILE_HACK
	power_event_sleep(now, wakeup_time-now, stm_power_state);
#endif

	switch(stm_power_state) {
		default:
		case POWER_STATE_LOW:
			RTC_SetAlarm(wakeup_time-SLEEP_EXTRA_PAD-SLEEP_PADDING_LOW_POWER);
			RTC_WaitForLastTask();
			now = align_to_rtc2();
			TIM_Cmd(TIM1, DISABLE);
			Sleep_Power();
			PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFE);
			RTC_WaitForSynchro();
			aft = RTC_GetCounter(); // align_to_rtc2() not needed because redundant with WaitForSyncrho() but ONLY FOR LOW-POWER CASE
			TIM_Cmd(TIM1, ENABLE);
			stm_power_state = POWER_STATE_LOW; // Low_Power is basically identical to Sleep_Power, so basically a no-op.
			break;
		case POWER_STATE_MID:
			RTC_SetAlarm(wakeup_time-SLEEP_EXTRA_PAD-SLEEP_PADDING_MID_POWER);
			RTC_WaitForLastTask();
			now = align_to_rtc2();
			TIM_Cmd(TIM1, DISABLE);
			Sleep_Power();
			PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFE);
			Mid_Power();
			RTC_WaitForSynchro();
			aft = align_to_rtc2();
			TIM_Cmd(TIM1, ENABLE);
			break;
		case POWER_STATE_HIGH:
			RTC_SetAlarm(wakeup_time-SLEEP_EXTRA_PAD-SLEEP_PADDING_HIGH_POWER);
			RTC_WaitForLastTask();
			now = align_to_rtc2();
			TIM_Cmd(TIM1, DISABLE);
			Sleep_Power();
			PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFE);
			High_Power();
			RTC_WaitForSynchro();
			aft = align_to_rtc2();
			TIM_Cmd(TIM1, ENABLE);
			break;
	}

	// Disable SEVONPEND and create-consume a dummy event.
	NVIC_SystemLPConfig(NVIC_LP_SEVONPEND, DISABLE);
	__SEV();
	__WFE();
#ifdef POWER_PROFILE_HACK
	power_event_wakeup(aft);
#endif
#ifdef POWER_PROFILE_RTC_WARN
	debug_wakeup = aft + POWER_DEBUG_WAKEUP;
	if (debug_wakeup > startup_time + POWER_DEBUG_WAKEUP_OFFSET)
		RTC_SetAlarm(debug_wakeup); // watch dog alarm in case we never sleep again.
#endif

	// Main system timer does not run during sleep so we have to fix up clock afterwards.
	// Do a three iteration floor estimate for the clock conversion.
	// Basically this is "leap ticks" and avoids both floating point and long division.
	uint32_t ticks;			 			  // 1st iteration
	if (aft >= now) { ticks = aft-now; }  // Check for roll-over
	else { ticks = aft-now+0xFFFFFFFF; }
	uint32_t ticks_extra;
	static uint32_t ticks_carried  = 0;  // 2nd iteration
	static uint32_t ticks_carried3  = 0; // 3nd iteration
	// 2nd iteration, every 62.5ms
	ticks_extra = (ticks+ticks_carried) / 2048;
	ticks_carried = (ticks+ticks_carried) % 2048;
	// 3rd iteration, every 8 seconds
	ticks_extra += (ticks_extra+ticks_carried3)/128 * 23;
	ticks_carried3 = (ticks_extra+ticks_carried3) % 128;
	// Add it up
	ticks = (ticks+ticks_extra) * 305;
	// Punch it in
	HAL_Time_AddClockTime(ticks);

	// Long term numerical error should be 312.5 ppb
	// Typical short term error of 1 tick (~30.5us) but max of 24 ticks (~732 us).
	// Long term error is about right for a 5-10ppm source (~10x better) but a real source of error if <= 1ppm.
	// Then again, the HSI contributes too and is something like 50,000 ppm so this is fundamentally broken anyway

	//irq.Release(); // Should already be locked from caller
}

// Shouldn't be used, possibly for unrecoverable error in debug mode.
void Halt() {
    SOFT_BREAKPOINT();
	while(1);
}

void Reset() {
	__disable_irq();
	NVIC_SystemReset();
	while(1);
}

void Shutdown() {
    SOFT_BREAKPOINT();
	while(1) { PWR_EnterSTANDBYMode(); }
}

void HAL_AssertEx() {
#if !defined(NDEBUG)
	if(JTAG_Attached()) {
		SOFT_BREAKPOINT(); // use SOFT_BREAKPOINT() because there are too many assertions being thrown, and some assertions might be false positives due to other companies submitting junk to CodePlex.
	}
	return;
#endif
}
