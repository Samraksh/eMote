/*
	Microframework 4.3 HAL driver for basic power functions.
	Sleep via standalone power collapse.

	Michael McGrath
	michael.mcgrath@samraksh.com
	The Samraksh Company
	2013-10-01
*/

#include "..\Krait.h"
#include "Krait__POWER.h"
#include "..\Krait_PMIC\pmic_decl.h"

Krait_POWER_Driver g_Krait_POWER_Driver;


namespace {
    inline void outer_sync(void)
{ }
}
#define dsb() __asm__ __volatile__ ("dsb" : : : "memory")
#define dmb() __asm__ __volatile__ ("dmb" : : : "memory")
#define mb()            do { dsb(); outer_sync(); } while (0)
#define wmb()           mb()

bool ShutdownDrivers(void) {
#warning "STUB: ShutdownDrivers() Not implemented on SOC_ADAPT"
	return FALSE;
}

void Krait_POWER_Driver::Init() {
	///L2 SPM initialization
	__raw_writel(      0x01, 0x02012020);				//L2SCU_SAW2_SPM_CTL ...01=L2 retention. 0x61 =GDHS. 141=L2 Off.  use 5 at the end to inhibit start address reset on program end.
	__raw_writel(0x02020204, 0x02012024);				//L2SCU_SAW2_PMIC_DLY
	__raw_writel(0x00A000AE, 0x02012028);				//L2SCU_SAW2_PMIC_DATA_0
	__raw_writel(0x00A00020, 0x0201202C);				//L2SCU_SAW2_PMIC_DATA_1
	__raw_writel(0x20032000, 0x02012080);				//L2SCU_SAW2_SPM_SLP_SEQ_ENTRY_0
	__raw_writel(0x20000F00, 0x02012084);				//L2SCU_SAW2_SPM_SLP_SEQ_ENTRY_1
	__raw_writel(0x03486434, 0x02012088);				//L2SCU_SAW2_SPM_SLP_SEQ_ENTRY_2 ... note use 07 on end to power collapse with rpm.
	__raw_writel(0x64502048, 0x0201208C);				//L2SCU_SAW2_SPM_SLP_SEQ_ENTRY_3
	__raw_writel(0x0F503404, 0x02012090);				//L2SCU_SAW2_SPM_SLP_SEQ_ENTRY_4
	__raw_writel(0x64341000, 0x02012094);				//L2SCU_SAW2_SPM_SLP_SEQ_ENTRY_5
	__raw_writel(0x10480348, 0x02012098);				//L2SCU_SAW2_SPM_SLP_SEQ_ENTRY_6
	__raw_writel(0x34046450, 0x0201209C);				//L2SCU_SAW2_SPM_SLP_SEQ_ENTRY_7
	__raw_writel(0x0F0F0F50, 0x020120A0);				//L2SCU_SAW2_SPM_SLP_SEQ_ENTRY_8
	__raw_writel(0x0F0F0F0F, 0x020120A4);				//L2SCU_SAW2_SPM_SLP_SEQ_ENTRY_9

	///Krait0 SPM sequence initialization
	///80-N1622-3 Docs-based Krait SPM sequences: 0:WFI, 3:Retention with Dynamic Snoop Invalidation Mode,  0x10:Power Collapse Sleep Mode
	__raw_writel(0x400F0B03, 0x02089080);            	//SAW2_SPM_SLP_SEQ_ENTRY_0
	__raw_writel(0x0500101B, 0x02089084);            	//SAW2_SPM_SLP_SEQ_ENTRY_1
	__raw_writel(0x000B0103, 0x02089088);            	//SAW2_SPM_SLP_SEQ_ENTRY_2 //use 07 to contact RPM.
	__raw_writel(0x0F1B4010, 0x0208908C);            	//SAW2_SPM_SLP_SEQ_ENTRY_3
	__raw_writel(0x10542400, 0x02089090);            	//SAW2_SPM_SLP_SEQ_ENTRY_4
	__raw_writel(0x0B010309, 0x02089094);            	//SAW2_SPM_SLP_SEQ_ENTRY_5 //use 07 to contact RPM.
	__raw_writel(0x0C305410, 0x02089098);            	//SAW2_SPM_SLP_SEQ_ENTRY_6
	__raw_writel(0x0F0F3024, 0x0208909C);            	//SAW2_SPM_SLP_SEQ_ENTRY_7
	__raw_writel(0x0F0F0F0F, 0x020890A0);            	//SAW2_SPM_SLP_SEQ_ENTRY_8
	__raw_writel(0x0F0F0F0F, 0x020890A4);            	//SAW2_SPM_SLP_SEQ_ENTRY_9

	///Krait0 SPM config initialization
	__raw_writel(      0x1F, 0x02089008);	//SAME		//SAW2_CFG
	__raw_writel(0x40000000, 0x02089018);	//DIF		//SAW2_AVS_CTL  // or init value 0x58488064 in pm_boot_init
	__raw_writel(0x00020000, 0x0208901C);	//NEW		//SAW2_AVS_HYSTERESIS
	__raw_writel(      0x01, 0x02089020);   //SPECIFIC  //SAW2_SPM_CTL  //start address goes here.
	__raw_writel(0x03020004, 0x02089024);               //SAW2_SPM_PMIC_DLY
	__raw_writel(0x00840090, 0x02089028);   //ALT     	//SAW2_SPM_PMIC_DATA_0  // or 0x0094009C //or 0x008400A4  //or 0x00EB0093  //or 0x00A000AE
	__raw_writel(0x00A20084, 0x0208902C);   //ALT    	//SAW2_SPM_PMIC_DATA_1  // or 0x00A4001C //or 0x00A40024  //or 0x00EA0090  //or 0x00A20020
	wmb();
	return;
}

void /*__section(BootStrap)*/ Krait_POWER_Driver::Highpower() {
	return;
}

void /*__section(BootStrap)*/ Krait_POWER_Driver::Lowpower() {

}

void Krait_POWER_Driver::Reset() {
    pm8921_config_reset_pwr_off((unsigned)1);                //1=reset, 0=poweroff
    __raw_writel(0, MSM_PSHOLD_CTL_SU);
    //FIXME: watchdog-based reset.
}

void Krait_POWER_Driver::SleepWFI() {
	__raw_writel(0x01, 0x02089020);            //SAW2_SPM_CTL, start address = 0 for WFI sequence start address.
	wmb();
    msm_arch_idle();
}

void Krait_POWER_Driver::SleepRetention() {
	__raw_writel(0x31, 0x02089020);            	//SAW2_SPM_CTL, start address = 2 for Android-based RETENTION sequence start address.
	wmb();
    msm_arch_idle();
	__raw_writel(0x01, 0x02089020);				//SAW2_SPM_CTL
	wmb();
}
