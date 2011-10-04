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
	INT32 WriteCount = 8;
	addr |= WRITE_ACCESS_COMMAND;
	UINT8 cmd = addr;
	UINT8 pl = data;
	UINT8* command = &cmd;
	UINT8* payload = &pl;

	CPU_GPIO_EnableOutputPin(Config.DeviceCS, false);
	//Send command to radio
	CPU_SPI_nWrite8_nRead8( Config, command, WriteCount, NULL, NULL, 0 );
	//Send data to radio
	CPU_SPI_nWrite8_nRead8( Config, payload, WriteCount, NULL, NULL, 0 );
	CPU_GPIO_EnableOutputPin(Config.DeviceCS, true);

	return retVal;
}

UINT8 spi_read(UINT8 addr)
{	
	UINT8 retVal = 0;			
	UINT8 read_buffer = 0;
	INT32 WriteCount = 1;
	addr |= READ_ACCESS_COMMAND;
	UINT8 cmd = addr;
	UINT8* command = &cmd;	
	UINT8* read_ptr = &read_buffer;
	UINT8 dummy_value = SPI_DUMMY_VALUE;
	UINT8* dummy_cmd = &dummy_value;

	CPU_GPIO_EnableOutputPin(Config.DeviceCS, false);
	//Send command read nothing	
	CPU_SPI_nWrite8_nRead8( Config, command, WriteCount, NULL, NULL, 0 );		
	//Send nothing read return
	CPU_SPI_nWrite8_nRead8( Config, dummy_cmd, WriteCount, &read_buffer, 1, 0 );	
	CPU_GPIO_EnableOutputPin(Config.DeviceCS, true);

	retVal = *read_ptr;
		
	return retVal;	
}

BOOL configure_gpio()
{
	bool retVal = TRUE;
	return retVal;	
}

BOOL slp_tr(BOOL state)
{
	//Perform error checking
	bool retVal = TRUE;
	if (state == FALSE)
	{
		CPU_GPIO_EnableOutputPin(SLP_TR, FALSE);		
	}
	else
	{
        CPU_GPIO_EnableOutputPin(SLP_TR, TRUE);

	}
	return retVal;
}

BOOL rst(BOOL state)
{
	bool retVal = TRUE;
	if (state == FALSE)
	{
		CPU_GPIO_EnableOutputPin(RST, FALSE);		
	}
	else
	{
        CPU_GPIO_EnableOutputPin(RST, TRUE);
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

//////////////////////////////////////////////////////////////////////////

	
  //STM_EVAL_PBInit(2, 1);
  
  //GPIO_InitTypeDef GPIO_InitStructure;
  //EXTI_InitTypeDef EXTI_InitStructure;
  //NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the BUTTON Clock */
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG | RCC_APB2Periph_AFIO, ENABLE);

  /* Configure Button pin as input floating */
  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  //GPIO_Init(GPIOG, &GPIO_InitStructure);


  /* Connect Button EXTI Line to Button GPIO Pin */
  //GPIO_EXTILineConfig(GPIO_PortSourceGPIOG, GPIO_PinSource8);

	/* Configure Button EXTI line */
  //EXTI_InitStructure.EXTI_Line = EXTI_Line8;
  //EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  //EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  

  //EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  //EXTI_Init(&EXTI_InitStructure);

  /* Enable and set Button EXTI Interrupt to the lowest priority */
  //NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  //NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  //NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  //NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  //NVIC_Init(&NVIC_InitStructure); 
  

  /* Generate software interrupt: simulate a falling edge applied on Key Button EXTI line */
  //EXTI_GenerateSWInterrupt(EXTI_Line8);
  

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
