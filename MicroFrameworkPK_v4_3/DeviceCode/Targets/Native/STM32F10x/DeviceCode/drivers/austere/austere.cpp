#include "austere.h"


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

// void rad_power_monitor(GPIO_PIN Pin, BOOL PinState, void* Param) {
	// if (CPU_GPIO_GetPinState(39) == TRUE){
		// hal_printf("*** rad pwr good ***\r\n");
	// } else {
		// hal_printf("*** rad pwr bad ***\r\n");
	// }
// }