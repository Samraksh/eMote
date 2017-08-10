/*
 * Name : Krait_POWER.h
 *
 * TODO: consider moving this into the main Krait CPU header file.
 *
 * Created on: Oct 02, 2013
 * Author: Michael McGrath
 */

#ifndef KRAIT_POWER_H_
#define KRAIT_POWER_H_

struct Krait_POWER_Driver
{
	static void Init();
	static void Highpower();
	static void Lowpower();
	static void Reset();
	static void SleepWFI();
	static void SleepRetention();
};

#ifndef __ASSEMBLY__
extern "C" {
	int msm_arch_idle(void);
}
#endif

#endif /* KRAIT_POWER_H_ */
