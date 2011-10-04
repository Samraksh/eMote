#include "netmf_radio.h"
#include <gpio/stm32f10x_gpio.h>
#include <rcc/stm32f10x_rcc.h>
#include <exti/stm32f10x_exti.h>
#include <misc/misc.h>
#include <led/stm32f10x_led.h>
//#include <radio_it.h>

#define RST    (UINT32)					   12
#define SLP_TR (UINT32)					   14
#define KEY_BUTTON_PIN                   GPIO_Pin_8
#define KEY_BUTTON_GPIO_PORT             GPIOG
#define KEY_BUTTON_GPIO_CLK              RCC_APB2Periph_GPIOG
#define KEY_BUTTON_EXTI_LINE             EXTI_Line8
#define KEY_BUTTON_EXTI_PORT_SOURCE      GPIO_PortSourceGPIOG
#define KEY_BUTTON_EXTI_PIN_SOURCE       GPIO_PinSource8
#define KEY_BUTTON_EXTI_IRQn             EXTI9_5_IRQn

/*
 * Write access command of the transceiver
 */

#define SPI_DUMMY_VALUE                 (0x00)//Dummy command to the tranceiver
#define WRITE_ACCESS_COMMAND            (0xC0)//Write access command to the tranceiver
#define READ_ACCESS_COMMAND             (0x80)//Read access command to the tranceiver
#define TRX_CMD_FW                      (0x60)//Frame write command of transceiver
#define TRX_CMD_FR                      (0x20)//Frame read command of transceiver
#define TRX_CMD_SW                      (0x40)//SRAM write command of transceiver
#define TRX_CMD_SR                      (0x00)//SRAM read command of transceiver 

 

SPI_CONFIGURATION Config;

BOOL mfhal_init()
{
	BOOL retval = TRUE;

	//Check for SPI init
	if(configure_spi() != TRUE)
	{
		retval = FALSE;
		return retval;
	}
	//Check for GPIO init
	if(configure_gpio() != TRUE)
	{
		retval = FALSE;
		return retval;
	}
	
	//Set up timers
	if(configure_timers() != TRUE)
	{
		retval = FALSE;
		return retval;
	}

	return TRUE; //unreachable code
}

BOOL configure_spi()
{
	CPU_SPI_Initialize();		
	Config.DeviceCS               = 10; //TODO - fix me.. 
    Config.CS_Active              = false;
    Config.CS_Setup_uSecs         = 0;
    Config.CS_Hold_uSecs          = 0;
    Config.MSK_IDLE               = false;
    Config.MSK_SampleEdge         = false;
    Config.Clock_RateKHz          = 2;
    Config.SPI_mod                = 0;
    //Config.BusyPin.Pin            = 
    //Config.BusyPin.ActiveState    = 
	Config.MD_16bits = FALSE;    
	return TRUE;
}

BOOL spi_write(UINT8 addr, UINT8 data)
{
	bool retVal = TRUE;
	INT32 WriteCount = 2;
	addr |= WRITE_ACCESS_COMMAND;
	UINT8 Write[] = {addr, data};
	//Send command to radio
	CPU_SPI_nWrite8_nRead8( Config, Write, WriteCount, NULL, NULL, 0 );
	return retVal;
}

UINT8 spi_read(UINT8 addr)
{	
	UINT8 retVal = 0;			
	INT32 WriteCount = 2;
	INT32 ReadCount = 2;
	addr |= READ_ACCESS_COMMAND;
	UINT8 Write[] = {addr, SPI_DUMMY_VALUE};
	UINT8 Read[] = {0, 0};	
	CPU_SPI_nWrite8_nRead8( Config, Write, WriteCount, Read, ReadCount, 0 );		
	retVal = Read[1];		
	return retVal;	
}

UINT8* spi_frame_read(UINT8* data, UINT8 length)
{
	INT32 WriteCount = 130;
	INT32 ReadCount  = 130;
	UINT8* Read = data;	
	UINT8 addr = TRX_CMD_FR;
	UINT8 Write[130]; //= {addr, SPI_DUMMY_VALUE};
	Write[0] = addr;
	
	for (int i = 1; i < 130; i++)
	{
		Write[i] = SPI_DUMMY_VALUE;
	}	
	CPU_SPI_nWrite8_nRead8( Config, Write, WriteCount, Read, ReadCount, 0 );			
	return Read;
}

BOOL spi_frame_write(UINT8* data, UINT8 length)
{
	bool retVal = TRUE;
	INT32 WriteCount = length;
	data[0] = TRX_CMD_FW;	
	//Send command to radio
	CPU_SPI_nWrite8_nRead8( Config, data, WriteCount, NULL, NULL, 0 );
	return retVal;
}

BOOL configure_gpio()
{
	bool retVal = TRUE;
	/*CPU_GPIO_EnableOutputPin(SLP_TR, FALSE);		
	CPU_GPIO_EnableOutputPin(RST, FALSE);		*/
	return retVal;	
}

BOOL slp_tr(BOOL state)
{
	//Perform error checking
	bool retVal = TRUE;
	if (state == FALSE)
	{
		//CPU_GPIO_EnableOutputPin(SLP_TR, FALSE);		
		CPU_GPIO_SetPinState (SLP_TR, FALSE);
	}
	else
	{
        CPU_GPIO_EnableOutputPin(SLP_TR, TRUE);
		//CPU_GPIO_SetPinState (SLP_TR, TRUE);

	}
	return retVal;
}

BOOL rst(BOOL state)
{
	bool retVal = TRUE;
	if (state == FALSE)
	{
		//CPU_GPIO_EnableOutputPin(RST, FALSE);
		CPU_GPIO_SetPinState (RST, FALSE);
	}
	else
	{
		CPU_GPIO_EnableOutputPin(RST, TRUE);
        //CPU_GPIO_SetPinState (RST, TRUE);
	}
	return retVal;	
}

BOOL configure_exti()
{

	bool retVal = TRUE;
	//Configure GPIO Clock
	//Configure Pin as Input floating
	//Configure the EXTI line 
	//Configure the NVIC

	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//Enable CLK for the GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE); 

	//Configure Pin as i/p floating
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//EXTI line configuration
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

    // Configure Button EXTI line
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);   

	// Enable and set Button EXTI Interrupt to the lowest priority
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);   

	return retVal;	
}

BOOL read_exti()
{
	bool retVal = TRUE;
	return retVal;	
}

BOOL configure_timers()
{
	bool retVal = TRUE;
	return retVal;	
}

BOOL timer_delay(int duration)
{
	int i = 0;
	bool retVal = TRUE;

	//TODO - fix me.. 

	while (i < duration)
	{
		i++ ;
	}

	return retVal;	
}
