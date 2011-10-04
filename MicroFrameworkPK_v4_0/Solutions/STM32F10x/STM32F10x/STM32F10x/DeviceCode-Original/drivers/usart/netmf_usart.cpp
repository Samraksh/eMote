////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include <netmf_usart.h>
#include <led/stm32f10x_led.h>

/*TODO 
	Add error handling
	Define the APIs completely
	#define the comPort
	Check how physical ports are mapped to logical ports
	Send data is non-blocking, skips data if continously transmitted
*/

BOOL CPU_USART_Initialize( int ComPortNum, int BaudRate, int Parity, int DataBits, int StopBits, int FlowValue )
{
    /*TODO 
	Add error handling.
	Define the APIs completely
	#define the comPort
    */  
  BOOL fRet = TRUE;
  //Initializing the USART structure for calling the Init on STM Driver
  USART_InitTypeDef USART_InitStructure; 
  
  //LED_GREEN();
  
  
  if(ComPortNum == 0) //CLR has it as 0
  {
	//LED_ORANGE();
    ComPortNum = COM1;
  }
  
  else
  {
    ComPortNum = COM2;
  }
  
  //TODO Switch among allowed values of baudrate and select or else error
  USART_InitStructure.USART_BaudRate = BaudRate;
  //TODO Switch among allowed values of databits and select or else error
  USART_InitStructure.USART_WordLength = DataBits;
  //TODO Switch among allowed values of stopbits and select or else error
  USART_InitStructure.USART_StopBits = StopBits;
  //TODO Switch among allowed values of parity and select or else error
  USART_InitStructure.USART_Parity = Parity;
  //TODO Switch among allowed values of flow value and select or else error
  USART_InitStructure.USART_HardwareFlowControl = FlowValue;
  //TODO Define Mode selection
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
  
  STM_EVAL_COMInit((COM_TypeDef) ComPortNum	, &USART_InitStructure);    
  
  return fRet;
    
}

BOOL CPU_USART_Uninitialize( int ComPortNum )
{
  if(ComPortNum == 0)
  {	
    USART_DeInit(USART1);
  }
  
  else if(ComPortNum == 2)
  {
    USART_DeInit(USART2);
  }
  
  else if(ComPortNum == 3)
  {
    USART_DeInit(USART3);
  }
  
  else if(ComPortNum == 4)
  {
    USART_DeInit(UART4);
  }
  
  else
  {
    if(ComPortNum == 5)
	{
	  USART_DeInit(UART5);
	}
  }    
}

BOOL CPU_USART_TxBufferEmpty( int ComPortNum )
{
    /* Check the BIT7 (TXE) of the USART Status register (USART_SR) 
	   and determine if content of TDR has been transferred to shift register.
	   We will use the USART_GetFlagStatus API for this.
	*/
	
	//Get the appropriate COM Port and assign corresponding USART
    if(ComPortNum == 0) //CLR calculates it as 0
	{
		//Set the flag to USART_FLAG_TXE
		//USART_GetFlagStatus = USART_FLAG_TXE;		
	
		if(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == SET)
		{
			/*0 data is not transferred to shift register
			/1 data is transferred to shift register*/

			//LED_BLUE();
			return TRUE;
		}
	
		else
		{
			//LED_RED();
			//LED_ORANGE();
			return FALSE;
		}
    }
  
	else if(ComPortNum == 2)
    {
		//Set the flag to USART_FLAG_TXE
		//USART_GetFlagStatus = USART_FLAG_TXE;		
	
		if(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == SET)
		{
			//0 data is not transferred to shift register
			//1 data is transferred to shift register
			return TRUE;
		}
	
		else
		{
			return FALSE;
		}
    } 
  
	else if(ComPortNum == 3)
    {
      //Set the flag to USART_FLAG_TXE
		//USART_GetFlagStatus = USART_FLAG_TXE;		
	
		if(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == SET)
		{
			//0 data is not transferred to shift register
			//1 data is transferred to shift register
			return TRUE;
		}
	
		else
		{
			return FALSE;
		}
    }
  
	else if(ComPortNum == 4)
    {
	  //Set the flag to USART_FLAG_TXE
		//USART_GetFlagStatus = USART_FLAG_TXE;		
	
		if(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == SET)
		{
			//0 data is not transferred to shift register
			//1 data is transferred to shift register
			return TRUE;
		}
	
		else
		{
			return FALSE;
		}      
	}
  
	else
    {
      if(ComPortNum == 5)
	  {
	    //Set the flag to USART_FLAG_TXE
		//USART_GetFlagStatus = USART_FLAG_TXE;		
	
		if(USART_GetFlagStatus(UART5, USART_FLAG_TXE) == SET)
		{
			//0 data is not transferred to shift register
			//1 data is transferred to shift register
			return TRUE;
		}
	
		else
		{
			return FALSE;
		}
	  }
    }		
	
}

BOOL CPU_USART_TxShiftRegisterEmpty( int ComPortNum )
{
	/*
	This register is empty when TXE is Full as TXE transfers data into the 
	shift register. Thus reversing the conditions on CPU_USART_TxBufferEmpty
	will determine if CPU_USART_TxShiftRegisterEmpty is true or false.
	*/
	
	//Get the appropriate COM Port and assign corresponding USART
    if(ComPortNum == 0) 
	{
      if(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
		{
			//This is set when data is still in TXE register.
			return TRUE;
		}
	
		else
		{
			//return FALSE;
			return TRUE; //KN just for now!
		}
				
    }
  
	else if(ComPortNum == 2)
    {
      if(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
		{
			//This is set when data is still in TXE register.
			return TRUE;
		}
	
		else
		{
			return FALSE;
		}
    } 
  
	else if(ComPortNum == 3)
    {
      if(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
		{
			//This is set when data is still in TXE register.
			return TRUE;
		}
	
		else
		{
			return FALSE;
		}
    }
  
	else if(ComPortNum == 4)
    {
	  if(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET)
		{
			//This is set when data is still in TXE register.
			return TRUE;
		}
	
		else
		{
			return FALSE;
		}      
	}
  
	else
    {
      if(ComPortNum == 5)
	  {
	    if(USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET)
		{
			//This is set when data is still in TXE register.
			return TRUE;
		}
	
		else
		{
			return FALSE;
		}
	  }
    }
		
	//Set the flag to USART_FLAG_TXE
	//USART_GetFlagStatus = USART_FLAG_TXE;		
	
	
}

void CPU_USART_WriteCharToTxBuffer( int ComPortNum, UINT8 c )
{
  //Get the appropriate COM Port and assign corresponding USART
    if(ComPortNum == 0)
	{
	  //LED_ORANGE();	  
      USART_SendData(USART1, c);
    }
  
	else if(ComPortNum == 2)
    {
      USART_SendData(USART2, c);
    } 
  
	else if(ComPortNum == 3)
    {
      USART_SendData(USART3, c);
    }
  
	else if(ComPortNum == 4)
    {
      USART_SendData(UART4, c);
	}
  
	else
    {
      if(ComPortNum == 5)
	  {
	    USART_SendData(UART5, c);
	  }
    }
	
	//Transmit data to USART specified.
	//USART_SendData(ComPortNum, c);
    
}

void CPU_USART_TxBufferEmptyInterruptEnable( int ComPortNum, BOOL Enable )
{
	/*
	Enables the interupt which indicates that the TX buffer is
	full. Sets the interupt enabled flag.
	
	Todo: Check if one is enabled, can we enable another one.
	
	
			
	//Get the appropriate COM Port and assign corresponding USART
    switch(ComPortNum)
	{	   	
	  case 1:
	  USART_ITConfig(USART1, USART_IT_TXE, Enable);	  
	  
	  case 2:
	  USART_ITConfig(USART2, USART_IT_TXE, Enable);      	  
	  
	  case 3:
	  USART_ITConfig(USART3, USART_IT_TXE, Enable);      	  
    }
  	
	/*
	else if(ComPortNum == 4)
    {
      USART_ITConfig(UART4, USART_IT, Enable);  	  
	}
  
	else
    {
      if(ComPortNum == 5)
	  {
	    USART_ITConfig(UART5, USART_IT, Enable);  		
	  }
    }
	*/
}

BOOL CPU_USART_TxBufferEmptyInterruptState( int ComPortNum )
{
	//Add code
	/*
	Check if enabled
	If so, 
	
	
	BOOL retValue = FALSE;
	
	switch(ComPortNum)
	{
	  case 1:
	  if(USART_GetITStatus(USART1, USART_IT_TXE) == SET)
	  {
	    retValue = TRUE;
	  }
	  
	  case 2:
	  if(USART_GetITStatus(USART2, USART_IT_TXE) == SET)
	  {
	    retValue = TRUE;
	  }
	  
	  case 3:
	  if(USART_GetITStatus(USART3, USART_IT_TXE) == SET)
	  {
	    retValue = TRUE;
	  }
	  
	  return retValue;
	  
	
	} */
	
      return TRUE;
}

void CPU_USART_RxBufferFullInterruptEnable( int ComPortNum, BOOL Enable )
{
    //Add code
}

BOOL CPU_USART_RxBufferFullInterruptState( int ComPortNum )
{
	//Add code
    return TRUE;
}

BOOL CPU_USART_TxHandshakeEnabledState( int comPort )
{
	/*
	Check for the value of CTS Pin and return the result. 
	Need to determine if HandShakeMode is enabled or not
	*/

    return TRUE; 
}

void CPU_USART_ProtectPins( int ComPortNum, BOOL On )
{
    /*
	Maybe we can skip this as STM_EVAL_COMInit sets the 
	appropriate GPIO pins.
	*/
}

UINT32 CPU_USART_PortsCount()
{
    return TOTAL_USART_PORT;
}

void CPU_USART_GetPins( int ComPortNum, GPIO_PIN& rxPin, GPIO_PIN& txPin,GPIO_PIN& ctsPin, GPIO_PIN& rtsPin )
{   
    //Return appropriate pins.
}

BOOL CPU_USART_SupportNonStandardBaudRate (int ComPortNum )
{
    return TRUE;
}

void CPU_USART_GetBaudrateBoundary( int ComPortNum, UINT32& maxBaudrateHz, UINT32& minBaudrateHz )
{
    maxBaudrateHz = MAX_BAUDRATE;
	minBaudrateHz = MIN_BAUDRATE;
}

BOOL CPU_USART_IsBaudrateSupported( int ComPortNum, UINT32& BaudrateHz )
{
  if (BaudrateHz <= MAX_BAUDRATE)
  {
    return true;
  }
  else
  {
    BaudrateHz = MAX_BAUDRATE;
    return false;
  }
}