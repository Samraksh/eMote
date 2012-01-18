//-----------------------------------------------------------------------------
//
//                   ** WARNING! ** 
//    This file was generated automatically by a tool.
//    Re-running the tool will overwrite this file.
//    You should copy this file to a custom location
//    before adding any customization in the copy to
//    prevent loss of your changes when the tool is
//    re-run.
//
//-----------------------------------------------------------------------------


#include "InterfaceLibrary.h"
#include "InterfaceLibrary_Samraksh_ADC.h"

#include <flash/stm32f10x_flash.h>
using namespace Samraksh;

extern void hal_adc_init(uint8_t sampTime);
extern uint8_t hal_adc_getData(uint16_t *dataBuf, uint8_t startChannel, uint8_t numChannels);

UINT16 *buffer1 = (UINT16 *) 0x08096000;
UINT16 *buffer2 = (UINT16*)0x080D6900;
UINT16 *currAddress = (UINT16 *) 0x08096000;

INT32 writeInternalFlash(UINT16 src)
{
	UINT16 lSize = 0;
	
	while(FLASH->SR2 & FLASH_SR_BSY);
	FLASH->CR2 = FLASH_CR_PG;
	//*currAddress++ = *src++;
	*currAddress++ = src;
	//*currAddress++ = (UINT8) (src & 0xff);
	lSize++;
}

INT32 ADC::Init( INT32 param0, HRESULT &hr )
{
    INT32 retVal = 0; 
	
	hal_adc_init((UINT8) param0);
	
    return retVal;
}

INT32 ADC::getData( CLR_RT_TypedArray_UINT16 param0, UINT32 param1, UINT32 param2, HRESULT &hr )
{
    INT32 retVal = 0;
	//UINT16 Ival = 0;
	//UINT16 Qval = 0;
	
	CLR_RT_TypedArray_UINT16 dataBuf = param0;

	retVal = hal_adc_getData(dataBuf.GetBuffer(), (UINT8) param1, (UINT8) param2);
	
	//Ival = dataBuf[0];
	//Qval = dataBuf[1];
	
	//writeInternalFlash(Ival);
	//writeInternalFlash(Qval);
		
    return retVal;
}

