
#include "TimerTest.h"


int testCount = 0;
int secCount=0;

void ISR_TIMER( void* Param );
void blink_led();
int TimerUnderTest=3;

TimerTest::TimerTest( UINT32 DisplayIntervalSeconds, UINT32 TimerDurationSeconds )
{

	// Initializes the gpio pins
	CPU_GPIO_Initialize();
	CPU_INTC_Initialize();
	//LED_BLUE();

	m_displayInterval = DisplayIntervalSeconds;
    m_timerDuration   = TimerDurationSeconds;
    thorp[0]          = '-';
    thorp[1]          = '/';
    thorp[2]          = '|';
    thorp[3]          = '\\';

};

BOOL TimerTest::Execute( LOG_STREAM Stream )
{

    Log& log = Log::InitializeLog( Stream, "Timers" );

    lcd_printf( "Timer Test Begin \n" );
    hal_printf("Timer Test Begin \n");

    /*for(UINT32 seconds=0; seconds<m_timerDuration; seconds++)
    {
        lcd_printf( "\r        %c",thorp[seconds % 4] );
        hal_printf( "\r        %c",thorp[seconds % 4] );

        if((seconds % m_displayInterval) == 0)
        {
            lcd_printf( "        %2d",seconds );
            hal_printf( "        %2d",seconds );
        }
        Events_WaitForEvents ( 0, 1000 );
    }*/


	// Indicates the start of the test
	//CPU_GPIO_EnableOutputPin(1,TRUE);

	// Configure Pin 2 as the pin of interest
	//CPU_GPIO_EnableOutputPin(2,FALSE);

	// Initialize Hal Level Time Driver
	if (!Timer_Driver :: Initialize (TimerUnderTest, TRUE, 0, 0, ISR_TIMER, NULL))
	{
		return FALSE;
	}

	// Sets compare value to 1000 count values

	Timer_Driver::SetCompare( TimerUnderTest, 625);

	/*int i,compare,counter;
	while(i < 10)
	{
		compare=Timer_Driver::GetCompare(TimerUnderTest);
		counter=Timer_Driver::GetCounter(4);
		i++;
	}*/


	while(secCount < 60)
	{
		/*if(secCount % 2 == 0){
				LED_BLUE();
			}else{
				LED_BLUE_OFF();
		}*/
	}


	// Indicates the end of the test
    //CPU_GPIO_SetPinState(1, FALSE);

	Timer_Driver :: Uninitialize(TimerUnderTest);

    return TRUE;
}; //Execute


void ISR_TIMER( void* Param )
{
	//LED_BLUE();
	switch(TimerUnderTest){
		case 2:
				if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
						  {
							TIM_ClearITPendingBit(TIM2, TIM_IT_CC1 );
						 }
				break;
		case 3:
			if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
					  {
						TIM_ClearITPendingBit(TIM3, TIM_IT_CC1 );
					 }
			break;
		case 4:
			if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
			  {
				TIM_ClearITPendingBit(TIM4, TIM_IT_CC1 );
			  }
			break;
	}
	/*if(testCount % 2 == 0)
		CPU_GPIO_SetPinState(2, TRUE);
	else
		CPU_GPIO_SetPinState(2,FALSE);*/

	if(testCount % 100 == 0){
		//blink_led();
		secCount++;
		//testCount=0;
	}

	testCount++;

}


//--//

