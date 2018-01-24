#ifndef HAL_RADAR_DRIVER_H_
#define HAL_RADAR_DRIVER_H_

#include <stm32f10x.h>

void detectHandler(GPIO_PIN Pin, BOOL PinState, void* Param);
void dataAlertHandler(GPIO_PIN Pin, BOOL PinState, void* Param);
void Radar_Handler(void *arg);
INT8 FPGA_RadarInit(UINT16 *chan1Ptr, UINT16 *chan2Ptr, UINT32 size);
INT8 getWindowOverThreshold();
INT8 getDetectionFinished();
INT32 getNetDisplacement(INT32 portion);
INT32 getAbsoluteDisplacement(INT32 portion);
INT32 getDisplacementRange(INT32 portion);
INT32 getCountOverTarget();
void setProcessingInProgress(int state);

#endif
