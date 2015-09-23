
/*

STM32F1x Microframework power HAL driver

Nathan Stohs
nathan.stohs@samraksh.com

*/

#include "netmf_pwr.h"
#include "netmf_pwr_wakelock.h"
#include "../usart/sam_usart.h"

static int pwr_hsi_clock_measure;
static int pwr_hsi_clock_measure_orig;
static enum stm_power_modes stm_power_state = POWER_STATE_DEFAULT;


#ifdef EMOTE_WAKELOCKS
static volatile uint32_t wakelock;
static volatile UINT64 waketime;

void WakeLockInit(void) {
	wakelock = 0;
	waketime = 0;
}

void WakeLock(uint32_t lock) {
	GLOBAL_LOCK(irq);
	wakelock |= lock;
}

void WakeUnlock(uint32_t lock) {
	GLOBAL_LOCK(irq);
	wakelock &= ~lock;
}

void WakeUntil(UINT64 until) {
	INT64 now;

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

uint32_t GetWakeLock(void) {
	return wakelock;
}

UINT64 GetWakeUntil(void) {
	return waketime;
}

BOOL WakeLockEnabled(void) {
	return TRUE;
}
#else // EMOTE_WAKELOCKS
void WakeLock(uint32_t lock) {}
void WakeUnlock(uint32_t lock) {}
void WakeUntil(UINT64 until) {}
uint32_t GetWakeLock(void) {return 0;}
UINT64 GetWakeUntil(void) {return 0;}
void WakeLockInit(void) {}
BOOL WakeLockEnabled(void) {return FALSE;}
#endif // EMOTE_WAKELOCKS

void PowerInit() {

	WakeLockInit();

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

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

#if defined(SAM_APP_TINYBOOTER)
	//High_Power();
	Low_Power();
	return;
	// Its important that we return before doing HSI calibration.
	// For some reason if we do it here and TinyCLR it freezes.
	// Also no reason to waste time on it.
	// Guessing its a double-init or deinit-init issue.
#endif

#ifdef DOTNOW_HSI_CALIB
	Low_Power();
	CalibrateHSI();
#else
	RCC_AdjustHSICalibrationValue(PWR_HSI_DEFAULT_TRIM);
#endif

	High_Power();
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
	RCC_LSEConfig(RCC_LSE_ON);
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) {
		if (err++ == PWR_RTC_TIMEOUT) {
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
		int min_trim = trim;

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

void Low_Power() {

	// Make sure actually changing
	if (stm_power_state == POWER_STATE_LOW) {
		return;
	}

	GLOBAL_LOCK(irq);
	
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
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
	FLASH_SetLatency(FLASH_Latency_0);

	stm_power_state = POWER_STATE_LOW;

	USART_reinit(); // Clock sensitive. Must redo.
}

void High_Power() {

	// Make sure actually changing
	if (stm_power_state == POWER_STATE_HIGH) {
		return;
	}

	GLOBAL_LOCK(irq);
	
	pause_peripherals();

	// Setup PLL for 8/2*12 = 48 MHz
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_12);
	
	// Set timer prescaler for constant 8 MHz
	// Only TIM1 needed, TIM2 is slave
	// PCLK @ 24 MHz, TIM runs x2, so /6
	TIM_PrescalerConfig(TIM1, 5, TIM_PSCReloadMode_Immediate);

	// Set Bus Speeds
	RCC_HCLKConfig(RCC_SYSCLK_Div1);  // 48 MHz
	RCC_PCLK1Config(RCC_HCLK_Div2);   // 24 MHz
	RCC_PCLK2Config(RCC_HCLK_Div2);   // 24 MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); // 4 MHz
	
	// This is confusing, pay attention:
	// If the PCLK1/2 prescaler != 1
	// Then the associated TIM clock is x2
	// so PCLK @ 48/2 MHz really means TIM @ 48 MHz.

	// Set flash speeds
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
	FLASH_SetLatency(FLASH_Latency_1);

	// Enable PLL and spin waiting for PLL ready
	RCC_PLLCmd(ENABLE);
	while ( RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET ) { ; }
	
	// Set PLL as clock source and spin waiting for active
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while ( RCC_GetSYSCLKSource() != 0x08 ) { ; }
	
	stm_power_state = POWER_STATE_HIGH;

	USART_reinit(); // Clock sensitive. Must redo.
}

// Exit in the same power state as we entered.
void Sleep() {

#ifdef EMOTE_WAKELOCKS
	// First check wakelocks
	// Really should use WFI but need execution loop polling for now.
	// TODO above

	if (wakelock) {
		return;
	}

	if (waketime > 0) {
		UINT64 now = HAL_Time_CurrentTicks();
		if (waketime > now) {
			return; // Wakelocked
		}
		waketime = 0; // Time is past, clear the time and continue to sleep
	}
#endif // EMOTE_WAKELOCKS

#ifdef SAM_APP_TINYBOOTER // Normally not reachable anyway.
	__DSB();
	__WFI();
#else
	switch(stm_power_state) {
		case POWER_STATE_LOW:
			//PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI);
			__DSB();
			__WFI();
			break;
		default:
			__DSB();
			__WFI();
			break;
	}
#endif
}

// Shouldn't be used, possibly for unrecoverable error in debug mode.
void Halt() {
    SOFT_BREAKPOINT();
	while(1);
}

void Reset() {
	__disable_irq();
	NVIC_SystemReset();
}

void Shutdown() {
    SOFT_BREAKPOINT();
	while(1) { PWR_EnterSTANDBYMode(); }
}

void HAL_AssertEx() {
/*
// leave commented out because there was no response the pull request on 2014-03-14.
#if !defined(NDEBUG)
	if(JTAG_Attached()) {
		SOFT_BREAKPOINT(); // use SOFT_BREAKPOINT() because there are too many assertions being thrown, and some assertions might be false positives due to other companies submitting junk to CodePlex.
	}
	return;
#endif
*/
}
