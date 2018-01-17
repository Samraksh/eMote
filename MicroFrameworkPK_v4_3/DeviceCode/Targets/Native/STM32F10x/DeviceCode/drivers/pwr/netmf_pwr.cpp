
/*

STM32F1x Microframework power HAL driver

Nathan Stohs
nathan.stohs@samraksh.com

*/

#include "netmf_pwr.h"
#include "netmf_pwr_wakelock.h"
#include "../usart/sam_usart.h"
#include "../Timer/netmf_rtc/netmf_rtc.h"

#define EMOTE_WAKELOCKS

static int pwr_hsi_clock_measure;
static int pwr_hsi_clock_measure_orig;
static enum stm_power_modes stm_power_state = POWER_STATE_DEFAULT;


#ifdef EMOTE_WAKELOCKS
static volatile uint32_t wakelock;
static volatile UINT64 waketime;

void WakeLockInit(void) {
	wakelock = 0;
	// TEMPORARY: Force awake for 1 minute until MFDeploy/VS is more robust
	waketime = HAL_Time_CurrentTicks() + CPU_MicrosecondsToTicks((UINT32)1000000 * 60 * 1);
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

uint32_t GetWakeLock(void) {
	return wakelock;
}

UINT64 GetWakeUntil(void) {
	return waketime;
}

#else // EMOTE_WAKELOCKS
void WakeLock(uint32_t lock) {}
void WakeUnlock(uint32_t lock) {}
void WakeUntil(UINT64 until) {}
uint32_t GetWakeLock(void) {return 0;}
UINT64 GetWakeUntil(void) {return 0;}
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

#ifdef DOTNOW_HSI_CALIB
	Low_Power();
	CalibrateHSI();
#else
	RCC_AdjustHSICalibrationValue(PWR_HSI_DEFAULT_TRIM);
#endif

	// Easiest to set these right at boot...
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
	FLASH_HalfCycleAccessCmd(FLASH_HalfCycleAccess_Disable);

	High_Power();
	RCC_LSICmd(DISABLE);

	RTC_wakeup_init();

#ifdef EMOTE_WAKELOCKS
	WakeLockInit();
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

static bool check_pending_isr(void) {
	const volatile unsigned icsr = *((volatile unsigned *)0xE000ED04);
	const unsigned ISRPENDING_MASK = 1<<22;
	return (icsr & ISRPENDING_MASK);
}

// Only to be called with interrupts disabled
static uint32_t align_to_rtc2(void) {
	uint32_t now = RTC_GetCounter();
	while (RTC_GetCounter() == now) ; // align by waiting until we tick
	return now+1;
}

void Snooze() {
	__DSB();
	__WFI();
}

// Exit in the same power state as we entered.
// TODO: Need to clean this up...
void Sleep() {

#ifdef EMOTE_WAKELOCKS
	BOOL doWFI = FALSE;

	if (waketime > 0) {
		UINT64 now = HAL_Time_CurrentTicks();
		if (waketime > now) {
			doWFI = TRUE; // Wakelocked
		}
		else {
		waketime = 0; // Time is past, clear the time and continue to sleep
	}
	}

	if (wakelock) { // A driver has signaled a wakelock
		doWFI = TRUE; // Wakelocked
	}

	if (doWFI) { // If wakelocked, use snooze mode.
	__DSB();
	__WFI();
		return; // Sleep completed, done here.
	}
#endif // EMOTE_WAKELOCKS

	USART_Flush(0); // Flush USART1 / COM0 before we sleep
	NVIC_SystemLPConfig(NVIC_LP_SEVONPEND, ENABLE);
	GLOBAL_LOCK(irq); // After sleep clocks are potentially unstable, so lock until returned.

	if (check_pending_isr()) { // Must ensure something didn't slip in
		NVIC_SystemLPConfig(NVIC_LP_SEVONPEND, DISABLE);
		__SEV();
		__WFE();
		return;
	}

	uint32_t now; // 32-bits can store about 90 days worth of ticks
	uint32_t aft; // post sleep RTC time;
	uint32_t wakeup_time; // when the RTC is supposed to wake us up.

	// Abort if our wakeup time is not at least 3 ticks in the future.
	// This is going to be a little screwy on roll-over but OK for now
	now = RTC_GetCounter();
	wakeup_time = g_STM32F10x_RTC.GetCompare();
	if(0 == wakeup_time || wakeup_time < now+3) {
		// Abort!
		NVIC_SystemLPConfig(NVIC_LP_SEVONPEND, DISABLE);
		__SEV();
		__WFE();
		return;
	}

	// DEBUGGING ONLY. Alert if sleep time is longer than 1 minute
	if (wakeup_time - now >= 1966080) {
		SOFT_BREAKPOINT();
	}

	switch(stm_power_state) {
		default:
		case POWER_STATE_LOW:
			now = align_to_rtc2();
			TIM_Cmd(TIM1, DISABLE);
			Sleep_Power();
			PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFE);
			Low_Power();
			RTC_WaitForSynchro();
			aft = align_to_rtc2();
			TIM_Cmd(TIM1, ENABLE);
			break;
		case POWER_STATE_MID:
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

	// Main system timer does not run during sleep
	// So we have to fix up clock afterwards.
	// Below code does a two iteration floor estimate for the clock conversion.
	// Fractional part (up to 4.5us worth) is carried.
	uint32_t ticks = aft-now;
	uint32_t ticks_extra;
	static uint32_t ticks_carried = 0; // only the fractional part
	ticks_extra = (ticks+ticks_carried)/256 * 45;
	ticks_carried = (ticks+ticks_carried) % 256;
	ticks = ticks * 305;
	HAL_Time_AddClockTime(ticks + ticks_extra);

	irq.Release();
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
