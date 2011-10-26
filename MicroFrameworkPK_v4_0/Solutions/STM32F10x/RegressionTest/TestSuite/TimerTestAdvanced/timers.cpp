////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Timers.h"
#include "c:\MicroFrameworkPK_v4_0\Solutions\STM32F10x\DeviceCode\drivers\tim\netmf_timers.h"

//---//

int testCount = 0;

testMath g_testMathInstance;
bool Level_1_A_Test = false;
bool Level_1_B_Test = false;
bool Level_2_Test = false;
bool Level_0_A_Test = false;
bool Level_0_B_Test = false;

UINT16 randomBuffer[600];
UINT16 bufferIterator = 0;

void ISR_TIMER( void* Param );


Timers::Timers( int seedValue, int numberOfEvents )
{
	this->seedValue = seedValue;
	this->numberOfEvents = numberOfEvents;
	g_testMathInstance.prng_init(seedValue);
	//this->testMathInstance.pareto_prng()_init(seedValue);
	// Initializes the gpio pins
	CPU_GPIO_Initialize();
	CPU_INTC_Initialize();
	
	for(int i = 0; i < 600; i++)
		randomBuffer[i] = g_testMathInstance.pareto_prng();
	
};

BOOL Timers::testMathLibrary()
{
	int loopIterator = 0;

	unsigned int rndmValue = 0;

	for(loopIterator = 0; loopIterator < this->numberOfEvents; loopIterator++)
	{
		rndmValue = g_testMathInstance.pareto_prng();

		if(-1 == rndmValue)
		{
			return FAIL;
		}
		else
		{
			hal_printf("\n%d",rndmValue);
		}

	}

	return SUCCESS;

}

BOOL Timers::Level_0A(int Timer)
{
	// Initialize the timer in free running mode with ISR_TIMER_4 as interrupt handler
	if (!Timer_Driver :: Initialize (Timer, TRUE, 0, 0, ISR_TIMER, NULL))
	{
				return FALSE;
	}

	//Timer_Driver::SetCompare( Timer, g_testMathInstance.pareto_prng());
	//TIM_SetCounter(TIM2,0);
	//Timer_Driver::SetCompare( Timer, randomBuffer[bufferIterator++]);
	Timer_Driver::SetCompare( Timer, 55200);

	while(testCount < this->numberOfEvents)
	{

	}
	
	Timer_Driver::Uninitialize(Timer);

	return TRUE;

}

BOOL Timers::Level_0B()
{
}

BOOL Timers::Level_1A()
{
	long randomValue;
	Level_1_A_Test = true;

	randomValue = g_testMathInstance.pareto_prng();
	int Timer = (randomValue % 3) + 1;

	if (!Timer_Driver :: Initialize (2, TRUE, 0, 0, ISR_TIMER, NULL))
		{
					return FALSE;
		}
	if (!Timer_Driver :: Initialize (3, TRUE, 0, 0, ISR_TIMER, NULL))
			{
						return FALSE;
			}
	if (!Timer_Driver :: Initialize (4, TRUE, 0, 0, ISR_TIMER, NULL))
			{
						return FALSE;
			}


	Timer_Driver::SetCompare( Timer, randomValue);

	while(testCount < this->numberOfEvents)
	{

	}

	Timer_Driver::Uninitialize (2 );
	Timer_Driver::Uninitialize (3 );
	Timer_Driver::Uninitialize (4 );

	Level_1_A_Test = false;
	return TRUE;

}

BOOL Timers::Level_1B(int Timer)
{
	if (!Timer_Driver :: Initialize (Timer, TRUE, 0, 0, ISR_TIMER, NULL))
		{
					return FALSE;
		}

	Timer_Driver::SetCompare( Timer, this->testMathInstance.pareto_prng());

	while(testCount < this->numberOfEvents)
	{
		// Gating all the peripherals before the WFI command
		RCC_APB2PeriphClockCmd((uint32_t )RCC_APB2Periph_AFIO | (uint32_t )RCC_APB2Periph_GPIOA | (uint32_t )RCC_APB2Periph_GPIOB | (uint32_t )RCC_APB2Periph_GPIOC | (uint32_t )RCC_APB2Periph_GPIOD | (uint32_t )RCC_APB2Periph_GPIOE | (uint32_t )RCC_APB2Periph_GPIOF | (uint32_t )RCC_APB2Periph_GPIOG | (uint32_t ) RCC_APB2Periph_ADC1 | (uint32_t ) RCC_APB2Periph_ADC2, DISABLE);
		RCC_APB2PeriphClockCmd((uint32_t )RCC_APB2Periph_SPI1 | (uint32_t ) RCC_APB2Periph_USART1 | (uint32_t ) RCC_APB2Periph_ADC3 , DISABLE);
		RCC_APB1PeriphClockCmd((uint32_t )RCC_APB1Periph_SPI2 | (uint32_t )RCC_APB1Periph_SPI3 | (uint32_t )RCC_APB1Periph_USB | (uint32_t )RCC_APB1Periph_DAC,DISABLE);
		RCC_AHBPeriphClockCmd((uint32_t )RCC_AHBPeriph_FLITF | (uint32_t )RCC_AHBPeriph_SRAM, DISABLE);
		//RCC_HSEConfig((uint32_t )0x00000000);
		RCC_HCLKConfig((uint32_t )RCC_SYSCLK_Div64);
		__WFI();
		RCC_HCLKConfig((uint32_t )RCC_SYSCLK_Div1);
		RCC_APB2PeriphClockCmd((uint32_t )RCC_APB2Periph_AFIO | (uint32_t )RCC_APB2Periph_GPIOA | (uint32_t )RCC_APB2Periph_GPIOB | (uint32_t )RCC_APB2Periph_GPIOC | (uint32_t )RCC_APB2Periph_GPIOD | (uint32_t )RCC_APB2Periph_GPIOE | (uint32_t )RCC_APB2Periph_GPIOF | (uint32_t )RCC_APB2Periph_GPIOG | (uint32_t ) RCC_APB2Periph_ADC1 | (uint32_t ) RCC_APB2Periph_ADC2, ENABLE);
		RCC_APB2PeriphClockCmd((uint32_t )RCC_APB2Periph_SPI1 | (uint32_t ) RCC_APB2Periph_USART1 | (uint32_t ) RCC_APB2Periph_ADC3 , ENABLE);
		RCC_APB1PeriphClockCmd((uint32_t )RCC_APB1Periph_SPI2 | (uint32_t )RCC_APB1Periph_SPI3 | (uint32_t )RCC_APB1Periph_USB | (uint32_t )RCC_APB1Periph_DAC,ENABLE);
		RCC_AHBPeriphClockCmd((uint32_t )RCC_AHBPeriph_FLITF | (uint32_t )RCC_AHBPeriph_SRAM, ENABLE);
	}

	Timer_Driver::Uninitialize(Timer);

	return TRUE;
}

BOOL Timers::Level_2()
{

	if (!Timer_Driver :: Initialize (2, TRUE, 0, 0, ISR_TIMER, NULL))
		{
					return FALSE;
		}
	if (!Timer_Driver :: Initialize (3, TRUE, 0, 0, ISR_TIMER, NULL))
			{
						return FALSE;
			}
	if (!Timer_Driver :: Initialize (4, TRUE, 0, 0, ISR_TIMER, NULL))
			{
						return FALSE;
			}

	Timer_Driver::SetCompare( 2, this->testMathInstance.pareto_prng());
	Timer_Driver::SetCompare( 3, this->testMathInstance.pareto_prng());
	Timer_Driver::SetCompare( 4, this->testMathInstance.pareto_prng());

	while(testCount < this->numberOfEvents)
	{
		// Gating all the peripherals before the WFI command
		RCC_APB2PeriphClockCmd((uint32_t )RCC_APB2Periph_AFIO | (uint32_t )RCC_APB2Periph_GPIOA | (uint32_t )RCC_APB2Periph_GPIOB | (uint32_t )RCC_APB2Periph_GPIOC | (uint32_t )RCC_APB2Periph_GPIOD | (uint32_t )RCC_APB2Periph_GPIOE | (uint32_t )RCC_APB2Periph_GPIOF | (uint32_t )RCC_APB2Periph_GPIOG | (uint32_t ) RCC_APB2Periph_ADC1 | (uint32_t ) RCC_APB2Periph_ADC2, DISABLE);
		RCC_APB2PeriphClockCmd((uint32_t )RCC_APB2Periph_SPI1 | (uint32_t ) RCC_APB2Periph_USART1 | (uint32_t ) RCC_APB2Periph_ADC3 , DISABLE);
		RCC_APB1PeriphClockCmd((uint32_t )RCC_APB1Periph_SPI2 | (uint32_t )RCC_APB1Periph_SPI3 | (uint32_t )RCC_APB1Periph_USB | (uint32_t )RCC_APB1Periph_DAC,DISABLE);
		RCC_AHBPeriphClockCmd((uint32_t )RCC_AHBPeriph_FLITF | (uint32_t )RCC_AHBPeriph_SRAM, DISABLE);
		//RCC_HSEConfig((uint32_t )0x00000000);
		RCC_HCLKConfig((uint32_t )RCC_SYSCLK_Div64);
		__WFI();
		RCC_HCLKConfig((uint32_t )RCC_SYSCLK_Div1);
		RCC_APB2PeriphClockCmd((uint32_t )RCC_APB2Periph_AFIO | (uint32_t )RCC_APB2Periph_GPIOA | (uint32_t )RCC_APB2Periph_GPIOB | (uint32_t )RCC_APB2Periph_GPIOC | (uint32_t )RCC_APB2Periph_GPIOD | (uint32_t )RCC_APB2Periph_GPIOE | (uint32_t )RCC_APB2Periph_GPIOF | (uint32_t )RCC_APB2Periph_GPIOG | (uint32_t ) RCC_APB2Periph_ADC1 | (uint32_t ) RCC_APB2Periph_ADC2, ENABLE);
		RCC_APB2PeriphClockCmd((uint32_t )RCC_APB2Periph_SPI1 | (uint32_t ) RCC_APB2Periph_USART1 | (uint32_t ) RCC_APB2Periph_ADC3 , ENABLE);
		RCC_APB1PeriphClockCmd((uint32_t )RCC_APB1Periph_SPI2 | (uint32_t )RCC_APB1Periph_SPI3 | (uint32_t )RCC_APB1Periph_USB | (uint32_t )RCC_APB1Periph_DAC,ENABLE);
		RCC_AHBPeriphClockCmd((uint32_t )RCC_AHBPeriph_FLITF | (uint32_t )RCC_AHBPeriph_SRAM, ENABLE);
	}

	Timer_Driver::Uninitialize(2);
	Timer_Driver::Uninitialize(3);
	Timer_Driver::Uninitialize(4);

	return TRUE;

}



BOOL Timers::Execute( int testLevel )
{

	// Indicates the start of the test
	CPU_GPIO_EnableOutputPin(9,TRUE);

	// Configure Pin 2 as the pin of interest
	CPU_GPIO_EnableOutputPin(10,FALSE);
	
	CPU_GPIO_EnableOutputPin(0,FALSE);
	
	//CPU_GPIO_SetPinState(10,TRUE);
	
	//CPU_GPIO_SetPinState(10,FALSE);

	if(testLevel == TESTRNDGEN)
	{
		if(Timers::testMathLibrary())
		{
			CPU_GPIO_SetPinState(9,FALSE);
			return SUCCESS;
		}
		else
		{
			CPU_GPIO_SetPinState(9,FALSE);
			return FAIL;
		}

	}
	if(testLevel == LEVEL_0_A)
	{
		for(int Timer = 2; Timer < 3; Timer++)
		{
			CPU_GPIO_SetPinState(9,TRUE);
			if(Timers::Level_0A(Timer))
			{
				CPU_GPIO_SetPinState(9,FALSE);
				return SUCCESS;
			}
			else
			{
				CPU_GPIO_SetPinState(9,FALSE);
				return FAIL;
			}
		}
	}
	if(testLevel == LEVEL_0_B)
	{
	}
	if(testLevel == LEVEL_1_A)
	{
		if(Timers::Level_1A())
		{
			CPU_GPIO_SetPinState(9,FALSE);
			return SUCCESS;
		}
		else
		{
			CPU_GPIO_SetPinState(9,FALSE);
			return FAIL;
		}
	}
	if(testLevel == LEVEL_1_B)
	{
		if(Timers::Level_1B(3))
		{
			CPU_GPIO_SetPinState(9,FALSE);
			return SUCCESS;
		}
		else
		{
			CPU_GPIO_SetPinState(9,FALSE);
			return FAIL;
		}
	}
	if(testLevel == LEVEL_2)
	{
		if(Timers::Level_2())
		{
			CPU_GPIO_SetPinState(9,FALSE);
			return SUCCESS;
		}
		else
		{
			CPU_GPIO_SetPinState(9,FALSE);
			return FAIL;
		}
	}


} //Execute

void ISR_TIMER( void* Param )
{
	UINT16 randomValue = 0;
	UINT16 diffValue = 0;
	UINT16 counterValue = 0;
    
	/*
	if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
	  {
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC1 );
			if(Level_1_A_Test == TRUE)
			{
				randomValue = g_testMathInstance.pareto_prng();
				Timer_Driver::SetCompare( (randomValue % 3) + 1, randomValue);
			}
			else
			{
				Timer_Driver::SetCompare( 4, TIM_GetCounter(TIM4) + randomBuffer[bufferIterator++]);
			}
	  }
	if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
	  {
		    TIM_ClearITPendingBit(TIM3, TIM_IT_CC1 );
		    if(Level_1_A_Test == TRUE)
		    {
		    	randomValue = g_testMathInstance.pareto_prng();
		    	Timer_Driver::SetCompare( (randomValue % 3) + 1, randomValue);
		    }
		    else
		    {
		    	Timer_Driver::SetCompare( 3, TIM_GetCounter(TIM3) + randomBuffer[bufferIterator++]);
		    }
	  }
	  */
	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
	  {
			TIM_ClearITPendingBit(TIM2, TIM_IT_CC1 );
		    if(Level_1_A_Test == TRUE)
		    {
			   	randomValue = g_testMathInstance.pareto_prng();
			   	Timer_Driver::SetCompare( (randomValue % 3) + 1, randomValue);
			}
			else
			{
				//TIM_SetCounter(TIM2,0);
				counterValue = TIM_GetCounter(TIM2);
				//randomValue = randomBuffer[bufferIterator++];
					
				//Timer_Driver::SetCompare( 2, counterValue + randomValue);
				Timer_Driver::SetCompare( 2, counterValue + 55200);
			}
	  }


	 
	if(testCount % 2 == 0)
		CPU_GPIO_SetPinState(10, TRUE);
	else
		CPU_GPIO_SetPinState(10,FALSE);
	testCount++;
	
}

/*
void Timers::ISR_TIMER_3( void* Param )
{
	if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
	  {
	    TIM_ClearITPendingBit(TIM3, TIM_IT_CC1 );
	    Timer_Driver::SetCompare( 3, this->testMathInstance.pareto_prng()());
	  }

	if(testCount % 2 == 0)
		CPU_GPIO_SetPinState(2, TRUE);
	else
		CPU_GPIO_SetPinState(2,FALSE);
	testCount++;

}

void Timers::ISR_TIMER_2( void* Param )
{
	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
	  {
	    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1 );
	    Timer_Driver::SetCompare( 2, this->testMathInstance.pareto_prng()());
	  }

	if(testCount % 2 == 0)
		CPU_GPIO_SetPinState(2, TRUE);
	else
		CPU_GPIO_SetPinState(2,FALSE);
	testCount++;

}
*/

//--//


