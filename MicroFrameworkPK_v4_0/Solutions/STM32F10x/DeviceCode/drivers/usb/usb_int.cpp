/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : usb_int.c
* Author             : MCD Application Team
* Version            : V3.3.0
* Date               : 21-March-2011
* Description        : Endpoint CTR (Low and High) interrupt's service routines
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
#ifndef STM32F10X_CL

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include <netmf_usb.h>
#include <tinyhal.h>

//#define USB_DEBUG1
//#undef USB_DEBUG1
//#define USB_DEBUG2


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint16_t SaveRState;
__IO uint16_t SaveTState;

/* Extern variables ----------------------------------------------------------*/
extern void (*pEpInt_IN[7])(void);    /*  Handles IN  interrupts   */
extern void (*pEpInt_OUT[7])(void);   /*  Handles OUT interrupts   */
extern void EP1_IN_Callback();
extern void EP2_OUT_Callback();

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : CTR_LP.
* Description    : Low priority Endpoint Correct Transfer interrupt's service
*                  routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CTR_LP()
//HAL_CALLBACK_FPN CTR_LP(void)
{
/*
#ifdef USB_DEBUG1
	debug_printf("~~~~In function - CTR_LP\n\r");
#endif*/
 //debug_printf("Hello World from USB!!\n\r");
  __IO uint16_t wEPVal = 0;
  /* stay in loop while pending interrupts */
  
  //2 is LED_red
  //STM_EVAL_LEDToggle((Led_TypeDef)2);
  
  while (((wIstr = _GetISTR()) & ISTR_CTR) != 0)
  {
    /* extract highest priority endpoint number */
    EPindex = (uint8_t)(wIstr & ISTR_EP_ID);
    if (EPindex == 0)
    {
      /* Decode and service control endpoint interrupt */
      /* calling related service routine */
      /* (Setup0_Process, In0_Process, Out0_Process) */

      /* save RX & TX status */
      /* and set both to NAK */


	    SaveRState = _GetENDPOINT(ENDP0);
	    SaveTState = SaveRState & EPTX_STAT;
	    SaveRState &=  EPRX_STAT;	

	    _SetEPRxTxStatus(ENDP0,EP_RX_NAK,EP_TX_NAK);

      /* DIR bit = origin of the interrupt */

      if ((wIstr & ISTR_DIR) == 0)
      {
        /* DIR = 0 */

        /* DIR = 0      => IN  int */
        /* DIR = 0 implies that (EP_CTR_TX = 1) always  */
		/* Device to Host */
#ifdef USB_DEBUG1
	debug_printf("In function - CTR_LP - Device to Host \n\r");
#endif

        _ClearEP_CTR_TX(ENDP0);
        In0_Process();

           /* before terminate set Tx & Rx status */

            _SetEPRxTxStatus(ENDP0,SaveRState,SaveTState);

            //Mukundan, commenting return
            //return;
      }
      else
      {
        /* DIR = 1 */

        /* DIR = 1 & CTR_RX       => SETUP or OUT int */
        /* DIR = 1 & (CTR_TX | CTR_RX) => 2 int pending */
		/* Host to Device */

        wEPVal = _GetENDPOINT(ENDP0);
        
        if ((wEPVal &EP_SETUP) != 0)
        {
          _ClearEP_CTR_RX(ENDP0); /* SETUP bit kept frozen while CTR_RX = 1 */
#ifdef USB_DEBUG1
	debug_printf("In function - CTR_LP -  SETUP Packet\n\r");
#endif
          Setup0_Process();
          /* before terminate set Tx & Rx status */

		      _SetEPRxTxStatus(ENDP0,SaveRState,SaveTState);

		      //Mukundan, commenting return
		      //return;
        }

        else if ((wEPVal & EP_CTR_RX) != 0)
        {
          _ClearEP_CTR_RX(ENDP0);
#ifdef USB_DEBUG1
	debug_printf("In function - CTR_LP -  Out0_Process \n\r");
#endif

          Out0_Process();
          /* before terminate set Tx & Rx status */
     
		     _SetEPRxTxStatus(ENDP0,SaveRState,SaveTState);

		     //Mukundan, commenting return
		     //return;
        }
      }
    }/* if(EPindex == 0) */
    else
    {
      /* Decode and service non control endpoints interrupt  */

	
	  //debug_printf("In function - CTR_LP - non control endpoints interrupt \n\r");
	  //debug_printf("Hello from non control endpoint\n");
      /* process related endpoint register */
      wEPVal = _GetENDPOINT(EPindex);
#ifdef USB_DEBUG1
	debug_printf("In function - CTR_LP - msg for endpoint: %d, state of endpoint: %d \n\r", EPindex,wEPVal);
#endif


		if ((wEPVal & EP_CTR_TX) != 0)
		{
			//STM_EVAL_LEDToggle((Led_TypeDef)2);
		  /* clear int flag */

		  _ClearEP_CTR_TX(EPindex);

		  USBCS_Driver::EP_TxISR(EPindex );

			//STM_EVAL_LEDToggle((Led_TypeDef)1);
		  /* call IN service function */
		  //(*pEpInt_IN[EPindex-1])();
			//EP1_IN_Callback();
		  //debug_printf("In function - CTR_LP - TX interrrupt \n\r");

		} /* if((wEPVal & EP_CTR_TX) != 0) */

      if ((wEPVal & EP_CTR_RX) != 0)
      {
		//STM_EVAL_LEDToggle((Led_TypeDef)0);
        /* clear int flag */

          _ClearEP_CTR_RX(EPindex);
          SetEPRxStatus(ENDP2, EP_RX_NAK);
          USBCS_Driver::EP_RxISR( EPindex );
          SetEPRxStatus(ENDP2, EP_RX_VALID);
       // debug_printf("In function - CTR_LP - RX interrrupt \n\r");
        /* call OUT service function */		
		//STM_EVAL_LEDToggle((Led_TypeDef)3);		
		//(*pEpInt_OUT[EPindex-1])();


      } /* if((wEPVal & EP_CTR_RX) */


    }/* if(EPindex == 0) else */

  }/* while(...) */
}

/*******************************************************************************
* Function Name  : CTR_HP.
* Description    : High Priority Endpoint Correct Transfer interrupt's service 
*                  routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CTR_HP(void* Param)
{
  uint32_t wEPVal = 0;

  while (((wIstr = _GetISTR()) & ISTR_CTR) != 0)
  {
    _SetISTR((uint16_t)CLR_CTR); /* clear CTR flag */
    /* extract highest priority endpoint number */
    EPindex = (uint8_t)(wIstr & ISTR_EP_ID);
    /* process related endpoint register */
    wEPVal = _GetENDPOINT(EPindex);
    if ((wEPVal & EP_CTR_RX) != 0)
    {
      /* clear int flag */
      _ClearEP_CTR_RX(EPindex);

      /* call OUT service function */
      (*pEpInt_OUT[EPindex-1])();

    } /* if((wEPVal & EP_CTR_RX) */
    else if ((wEPVal & EP_CTR_TX) != 0)
    {
      /* clear int flag */
      _ClearEP_CTR_TX(EPindex);

      /* call IN service function */
      (*pEpInt_IN[EPindex-1])();


    } /* if((wEPVal & EP_CTR_TX) != 0) */

  }/* while(...) */
}

#endif  /* STM32F10X_CL */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
