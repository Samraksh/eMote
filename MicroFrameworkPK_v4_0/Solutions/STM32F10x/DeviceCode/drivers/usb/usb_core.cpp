/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : usb_core.c
* Author             : MCD Application Team
* Version            : V3.3.0
* Date               : 21-March-2011
* Description        : Standard protocol processing (USB v2.0)
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
//#include <tinyhal.h>
#include "usb_lib.h"
//Kartik : MF Integration
#include "netmf_usb.h"
#include <pal/COM/usb/usb.h>
#include <led/stm32f10x_led.h>

//Kartik : MF Integration
//Kartik : Debug Macro
#undef USB_DEBUG
//#define USB_DEBUG1
//#undef USB_DEBUG1

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ValBit(VAR,Place)    (VAR & (1 << Place))
#define SetBit(VAR,Place)    (VAR |= (1 << Place))
#define ClrBit(VAR,Place)    (VAR &= ((1 << Place) ^ 255))

#ifdef STM32F10X_CL
 #define Send0LengthData()  {PCD_EP_Write (0, 0, 0) ; vSetEPTxStatus(EP_TX_VALID);}
#else
#define Send0LengthData() { _SetEPTxCount(ENDP0, 0); \
    vSetEPTxStatus(EP_TX_VALID); \
  }
#endif /* STM32F10X_CL */

#define vSetEPRxStatus(st) (SaveRState = st)
#define vSetEPTxStatus(st) (SaveTState = st)

#define USB_StatusIn() Send0LengthData()
#define USB_StatusOut() vSetEPRxStatus(EP_RX_VALID)

#define StatusInfo0 StatusInfo.bw.bb1 /* Reverse bb0 & bb1 */
#define StatusInfo1 StatusInfo.bw.bb0

//--//
//Kartik :
//SETUP_PACKET g_Setup_Packet;
USB_SETUP_PACKET RequestPacket = {0,0,0,0,0};
//--//

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t_uint8_t StatusInfo;

bool Data_Mul_MaxPacketSize = false;
/* Private function prototypes -----------------------------------------------*/
//Kartik : Commented as they are defined in the global netmf_usb.h
//static void DataStageOut(void);
//static void DataStageIn(void);
//static void NoData_Setup0(void);
//static void Data_Setup0(void);
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Standard_GetConfiguration.
* Description    : Return the current configuration variable address.
* Input          : Length - How many bytes are needed.
* Output         : None.
* Return         : Return 1 , if the request is invalid when "Length" is 0.
*                  Return "Buffer" if the "Length" is not 0.
*******************************************************************************/
uint8_t *Standard_GetConfiguration(uint16_t Length)
{
#ifdef USB_DEBUG1
	debug_printf("In the Standard_GetConfiguration\n\r");
#endif

  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength =
      sizeof(pInformation->Current_Configuration);
    return 0;
  }
  pUser_Standard_Requests->User_GetConfiguration();
  return (uint8_t *)&pInformation->Current_Configuration;
}

/*******************************************************************************
* Function Name  : Standard_SetConfiguration.
* Description    : This routine is called to set the configuration value
*                  Then each class should configure device itself.
* Input          : None.
* Output         : None.
* Return         : Return USB_SUCCESS, if the request is performed.
*                  Return USB_UNSUPPORT, if the request is invalid.
*******************************************************************************/
RESULT Standard_SetConfiguration(void)
{
#ifdef USB_DEBUG1
	debug_printf("In the function Standard_SetConfiguration\n\r");
#endif

  if ((pInformation->USBwValue0 <=
      Device_Table.Total_Configuration) && (pInformation->USBwValue1 == 0)
      && (pInformation->USBwIndex == 0)) /*call Back usb spec 2.0*/
  {
    pInformation->Current_Configuration = pInformation->USBwValue0;
    pUser_Standard_Requests->User_SetConfiguration();
	
	USB_CONTROLLER_STATE *State = g_USB_Driver.pUsbControllerState;
	
	RequestPacket.bmRequestType = g_Setup_Packet.USBbmRequestType;
	RequestPacket.bRequest = g_Setup_Packet.USBbRequest;
	RequestPacket.wValue = g_Setup_Packet.USBwValues;
	RequestPacket.wIndex = g_Setup_Packet.USBwIndexs;
	RequestPacket.wLength = g_Setup_Packet.USBwLengths;
	
	State->DeviceState = USB_DEVICE_STATE_ADDRESS;
	
	USB_HandleSetConfiguration(State, &RequestPacket, FALSE);
	
	SetEPRxStatus(ENDP2, EP_RX_VALID);
    SetEPRxStatus(EP2_OUT, EP_RX_VALID);
	
    return USB_SUCCESS;
  }
  else
  {
    return USB_UNSUPPORT;
  }
}

/*******************************************************************************
* Function Name  : Standard_GetInterface.
* Description    : Return the Alternate Setting of the current interface.
* Input          : Length - How many bytes are needed.
* Output         : None.
* Return         : Return 0, if the request is invalid when "Length" is 0.
*                  Return "Buffer" if the "Length" is not 0.
*******************************************************************************/
uint8_t *Standard_GetInterface(uint16_t Length)
{
#ifdef USB_DEBUG1
	debug_printf("In the Standard_GetInterface\n\r");
#endif
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength =
      sizeof(pInformation->Current_AlternateSetting);
    return 0;
  }
  pUser_Standard_Requests->User_GetInterface();
  return (uint8_t *)&pInformation->Current_AlternateSetting;
}

/*******************************************************************************
* Function Name  : Standard_SetInterface.
* Description    : This routine is called to set the interface.
*                  Then each class should configure the interface them self.
* Input          : None.
* Output         : None.
* Return         : - Return USB_SUCCESS, if the request is performed.
*                  - Return USB_UNSUPPORT, if the request is invalid.
*******************************************************************************/
RESULT Standard_SetInterface(void)
{
#ifdef USB_DEBUG1
	debug_printf("In the Standard_SetInterface\n\r");
#endif
  RESULT Re;
  /*Test if the specified Interface and Alternate Setting are supported by
    the application Firmware*/
  Re = (*pProperty->Class_Get_Interface_Setting)(pInformation->USBwIndex0, pInformation->USBwValue0);

  if (pInformation->Current_Configuration != 0)
  {
    if ((Re != USB_SUCCESS) || (pInformation->USBwIndex1 != 0)
        || (pInformation->USBwValue1 != 0))
    {
      return  USB_UNSUPPORT;
    }
    else if (Re == USB_SUCCESS)
    {
      pUser_Standard_Requests->User_SetInterface();
      pInformation->Current_Interface = pInformation->USBwIndex0;
      pInformation->Current_AlternateSetting = pInformation->USBwValue0;
      return USB_SUCCESS;
    }

  }

  return USB_UNSUPPORT;
}

/*******************************************************************************
* Function Name  : Standard_GetStatus.
* Description    : Copy the device request data to "StatusInfo buffer".
* Input          : - Length - How many bytes are needed.
* Output         : None.
* Return         : Return 0, if the request is at end of data block,
*                  or is invalid when "Length" is 0.
*******************************************************************************/
uint8_t *Standard_GetStatus(uint16_t Length)
{
#ifdef USB_DEBUG1
	debug_printf("In the Standard_GetStatus\n\r");
#endif

  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = 2;
    return 0;
  }

  /* Reset Status Information */
  StatusInfo.w = 0;

  if (Type_Recipient == (STANDARD_REQUEST | DEVICE_RECIPIENT))
  {
    /*Get Device Status */
    uint8_t Feature = pInformation->Current_Feature;

    /* Remote Wakeup enabled */
    if (ValBit(Feature, 5))
    {
      SetBit(StatusInfo0, 1);
    }
    else
    {
      ClrBit(StatusInfo0, 1);
    }      

    /* Bus-powered */
    if (ValBit(Feature, 6))
    {
      SetBit(StatusInfo0, 0);
    }
    else /* Self-powered */
    {
      ClrBit(StatusInfo0, 0);
    }
  }
  /*Interface Status*/
  else if (Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT))
  {
    return (uint8_t *)&StatusInfo;
  }
  /*Get EndPoint Status*/
  else if (Type_Recipient == (STANDARD_REQUEST | ENDPOINT_RECIPIENT))
  {
    uint8_t Related_Endpoint;
    uint8_t wIndex0 = pInformation->USBwIndex0;

    Related_Endpoint = (wIndex0 & 0x0f);
    if (ValBit(wIndex0, 7))
    {
      /* IN endpoint */
      if (_GetTxStallStatus(Related_Endpoint))
      {
        SetBit(StatusInfo0, 0); /* IN Endpoint stalled */
      }
    }
    else
    {
      /* OUT endpoint */
      if (_GetRxStallStatus(Related_Endpoint))
      {
        SetBit(StatusInfo0, 0); /* OUT Endpoint stalled */
      }
    }

  }
  else
  {
    return 0;
  }
  pUser_Standard_Requests->User_GetStatus();
  return (uint8_t *)&StatusInfo;
}

/*******************************************************************************
* Function Name  : Standard_ClearFeature.
* Description    : Clear or disable a specific feature.
* Input          : None.
* Output         : None.
* Return         : - Return USB_SUCCESS, if the request is performed.
*                  - Return USB_UNSUPPORT, if the request is invalid.
*******************************************************************************/
RESULT Standard_ClearFeature(void)
{
  //Kartik : For this device request we don't need any MF services
  //we can use it as it is.
#ifdef USB_DEBUG1
	debug_printf("In the Standard_ClearFeature\n\r");
#endif

  uint32_t     Type_Rec = Type_Recipient;
  uint32_t     Status;


  if (Type_Rec == (STANDARD_REQUEST | DEVICE_RECIPIENT))
  {/*Device Clear Feature*/
    ClrBit(pInformation->Current_Feature, 5);
    return USB_SUCCESS;
  }
  else if (Type_Rec == (STANDARD_REQUEST | ENDPOINT_RECIPIENT))
  {/*EndPoint Clear Feature*/
    DEVICE* pDev;
    uint32_t Related_Endpoint;
    uint32_t wIndex0;
    uint32_t rEP;

    if ((pInformation->USBwValue != ENDPOINT_STALL)
        || (pInformation->USBwIndex1 != 0))
    {
      return USB_UNSUPPORT;
    }

    pDev = &Device_Table;
    wIndex0 = pInformation->USBwIndex0;
    rEP = wIndex0 & ~0x80;
    Related_Endpoint = ENDP0 + rEP;

    if (ValBit(pInformation->USBwIndex0, 7))
    {
      /*Get Status of endpoint & stall the request if the related_ENdpoint
      is Disabled*/
      Status = _GetEPTxStatus(Related_Endpoint);
    }
    else
    {
      Status = _GetEPRxStatus(Related_Endpoint);
    }

    if ((rEP >= pDev->Total_Endpoint) || (Status == 0)
        || (pInformation->Current_Configuration == 0))
    {
      return USB_UNSUPPORT;
    }


    if (wIndex0 & 0x80)
    {
      /* IN endpoint */
      if (_GetTxStallStatus(Related_Endpoint ))
      {
      #ifndef STM32F10X_CL
        ClearDTOG_TX(Related_Endpoint);
      #endif /* STM32F10X_CL */
        SetEPTxStatus(Related_Endpoint, EP_TX_VALID);
      }
    }
    else
    {
      /* OUT endpoint */
      if (_GetRxStallStatus(Related_Endpoint))
      {
        if (Related_Endpoint == ENDP0)
        {
          /* After clear the STALL, enable the default endpoint receiver */
          SetEPRxCount(Related_Endpoint, Device_Property.MaxPacketSize);
          _SetEPRxStatus(Related_Endpoint, EP_RX_VALID);
        }
        else
        {
        #ifndef STM32F10X_CL
          ClearDTOG_RX(Related_Endpoint);
        #endif /* STM32F10X_CL */
          _SetEPRxStatus(Related_Endpoint, EP_RX_VALID);
        }
      }
    }
	//Call to the netmf_usb STM_USB_ClearFeature function
    pUser_Standard_Requests->User_ClearFeature();
    return USB_SUCCESS;
  }

  return USB_UNSUPPORT;
}

/*******************************************************************************
* Function Name  : Standard_SetEndPointFeature
* Description    : Set or enable a specific feature of EndPoint
* Input          : None.
* Output         : None.
* Return         : - Return USB_SUCCESS, if the request is performed.
*                  - Return USB_UNSUPPORT, if the request is invalid.
*******************************************************************************/
RESULT Standard_SetEndPointFeature(void)
{
#ifdef USB_DEBUG1
	debug_printf("In the Standard_SetEndPointFeature\n\r");
#endif
  uint32_t    wIndex0;
  uint32_t    Related_Endpoint;
  uint32_t    rEP;
  uint32_t    Status;

  wIndex0 = pInformation->USBwIndex0;
  rEP = wIndex0 & ~0x80;
  Related_Endpoint = ENDP0 + rEP;

  if (ValBit(pInformation->USBwIndex0, 7))
  {
    /* get Status of endpoint & stall the request if the related_ENdpoint
    is Disabled*/
    Status = _GetEPTxStatus(Related_Endpoint);
  }
  else
  {
    Status = _GetEPRxStatus(Related_Endpoint);
  }

  if (Related_Endpoint >= Device_Table.Total_Endpoint
      || pInformation->USBwValue != 0 || Status == 0
      || pInformation->Current_Configuration == 0)
  {
    return USB_UNSUPPORT;
  }
  else
  {
    if (wIndex0 & 0x80)
    {
      /* IN endpoint */
      _SetEPTxStatus(Related_Endpoint, EP_TX_STALL);
    }

    else
    {
      /* OUT endpoint */
      _SetEPRxStatus(Related_Endpoint, EP_RX_STALL);
    }
  }
  pUser_Standard_Requests->User_SetEndPointFeature();
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : Standard_SetDeviceFeature.
* Description    : Set or enable a specific feature of Device.
* Input          : None.
* Output         : None.
* Return         : - Return USB_SUCCESS, if the request is performed.
*                  - Return USB_UNSUPPORT, if the request is invalid.
*******************************************************************************/
RESULT Standard_SetDeviceFeature(void)
{
#ifdef USB_DEBUG1
	debug_printf("Standard_SetDeviceFeature\n\r");
#endif

  SetBit(pInformation->Current_Feature, 5);
  pUser_Standard_Requests->User_SetDeviceFeature();
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : Standard_GetDescriptorData.
* Description    : Standard_GetDescriptorData is used for descriptors transfer.
*                : This routine is used for the descriptors resident in Flash
*                  or RAM
*                  pDesc can be in either Flash or RAM
*                  The purpose of this routine is to have a versatile way to
*                  response descriptors request. It allows user to generate
*                  certain descriptors with software or read descriptors from
*                  external storage part by part.
* Input          : - Length - Length of the data in this transfer.
*                  - pDesc - A pointer points to descriptor struct.
*                  The structure gives the initial address of the descriptor and
*                  its original size.
* Output         : None.
* Return         : Address of a part of the descriptor pointed by the Usb_
*                  wOffset The buffer pointed by this address contains at least
*                  Length bytes.
*******************************************************************************/
uint8_t *Standard_GetDescriptorData(uint16_t Length, ONE_DESCRIPTOR *pDesc)
{

  //Kartik : MF Integration added
#ifdef USB_DEBUG
  debug_printf("Standard_GetDescriptorData\n\r");
#endif

  UINT8  result;
  uint8_t* data;
  uint32_t  wOffset;
  
//#if 0 
  wOffset = pInformation->Ctrl_Info.Usb_wOffset;


  if (Length == 0)
  {
  pInformation->Ctrl_Info.Usb_wLength = pDesc->Descriptor_Size - wOffset;
  return 0;
  }

  return pDesc->Descriptor + wOffset;
  
//#endif
  
#if 0
  USB_CONTROLLER_STATE *State = g_USB_Driver.pUsbControllerState;
  //USB_SETUP_PACKET* Setup = (USB_SETUP_PACKET* )&g_USB_Driver.ControlPacketBuffer[0];
  USB_SETUP_PACKET* Setup = (USB_SETUP_PACKET*) &g_Setup_Packet;
  
  if((Setup->bRequest == USB_GET_DESCRIPTOR) && (ByteSwap(Setup->wValue) == USB_DEVICE_DESCRIPTOR_TYPE) && (Setup->wLength != 0x12))
  {
	g_USB_Driver.FirstDescriptorPacket = TRUE;
  }  
  else
  {
	g_USB_Driver.FirstDescriptorPacket = FALSE;
  }
  
  //Call to the PAL to extract the descriptor  
  result = USB_ControlCallback(State);
  
  if(result != USB_STATE_STALL)
  {
	//Kartik : replace with call to controlnext()
	if (State->DataCallback)
	{
		//Call the data call back function, this populates the State->Data
		State->DataCallback(State);
		pInformation->Ctrl_Info.Usb_wLength = State->DataSize;
		data = State->Data;
	}
  }
  return data; 
#endif

}

/*******************************************************************************
* Function Name  : DataStageOut.
* Description    : Data stage of a Control Write Transfer.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void DataStageOut(void)
{
#ifdef USB_DEBUG1
	debug_printf("In function DataStageOut\n\r");
#endif
  ENDPOINT_INFO *pEPinfo = &pInformation->Ctrl_Info;
  uint32_t save_rLength;

  save_rLength = pEPinfo->Usb_rLength;

  if (pEPinfo->CopyData && save_rLength)
  {
    uint8_t *Buffer;
    uint32_t Length;

    Length = pEPinfo->PacketSize;
    if (Length > save_rLength)
    {
      Length = save_rLength;
    }

    Buffer = (*pEPinfo->CopyData)(Length);
    pEPinfo->Usb_rLength -= Length;
    pEPinfo->Usb_rOffset += Length;

  #ifdef STM32F10X_CL  
    PCD_EP_Read(ENDP0, Buffer, Length); 
  #else  
    PMAToUserBufferCopy(Buffer, GetEPRxAddr(ENDP0), Length);
  #endif  /* STM32F10X_CL */
  }

  if (pEPinfo->Usb_rLength != 0)
  {
    vSetEPRxStatus(EP_RX_VALID);/* re-enable for next data reception */
    SetEPTxCount(ENDP0, 0);
    vSetEPTxStatus(EP_TX_VALID);/* Expect the host to abort the data OUT stage */
  }
  /* Set the next State*/
  if (pEPinfo->Usb_rLength >= pEPinfo->PacketSize)
  {
    pInformation->ControlState = OUT_DATA;
  }
  else
  {
    if (pEPinfo->Usb_rLength > 0)
    {
      pInformation->ControlState = LAST_OUT_DATA;
    }
    else if (pEPinfo->Usb_rLength == 0)
    {
      pInformation->ControlState = WAIT_STATUS_IN;
      USB_StatusIn();
    }
  }
}

/*******************************************************************************
* Function Name  : DataStageIn.
* Description    : Data stage of a Control Read Transfer.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void DataStageIn(void)
{
#ifdef USB_DEBUG1
	debug_printf("In function DataStageIn\n\r");
#endif

  USB_CONTROLLER_STATE *State = g_USB_Driver.pUsbControllerState;
  ENDPOINT_INFO *pEPinfo = &pInformation->Ctrl_Info;
  uint32_t save_wLength = pEPinfo->Usb_wLength;
  uint32_t ControlState = pInformation->ControlState;

  uint8_t *DataBuffer;
  uint32_t Length;

  if ((save_wLength == 0) && (ControlState == LAST_IN_DATA))
  {
#ifdef USB_DEBUG
	debug_printf("In function DataStageIn - LAST_IN_DATA\n\r");
#endif
    if(Data_Mul_MaxPacketSize == true)
    {
#ifdef USB_DEBUG
	debug_printf("In function DataStageIn - Data_Mul_MaxPacketSize TRUE\n\r");
#endif
      /* No more data to send and empty packet */
      Send0LengthData();
      ControlState = LAST_IN_DATA;
      Data_Mul_MaxPacketSize = false;
    }
    else 
    {
#ifdef USB_DEBUG
	debug_printf("In function DataStageIn - Data_Mul_MaxPacketSize FALSE\n\r");
#endif
      /* No more data to send so STALL the TX Status*/
      ControlState = WAIT_STATUS_OUT;

    #ifdef STM32F10X_CL      
      PCD_EP_Read (ENDP0, 0, 0);
    #endif  /* STM32F10X_CL */ 
    
    #ifndef STM32F10X_CL 
      vSetEPTxStatus(EP_TX_STALL);
    #endif  /* STM32F10X_CL */ 
    }
    
    goto Expect_Status_Out;
  }

  Length = pEPinfo->PacketSize;
  ControlState = (save_wLength <= Length) ? LAST_IN_DATA : IN_DATA;

  if (Length > save_wLength)
  {
    Length = save_wLength;
  }

  DataBuffer = (*pEPinfo->CopyData)(Length);
  //Kartik:
  //DataBuffer = State->Data;

#ifdef STM32F10X_CL
  PCD_EP_Write (ENDP0, DataBuffer, Length);
#else   
  UserToPMABufferCopy(DataBuffer, GetEPTxAddr(ENDP0), Length);
#endif /* STM32F10X_CL */ 

  SetEPTxCount(ENDP0, Length);

  pEPinfo->Usb_wLength -= Length;
  pEPinfo->Usb_wOffset += Length;
  vSetEPTxStatus(EP_TX_VALID);

  USB_StatusOut();/* Expect the host to abort the data IN stage */

Expect_Status_Out:
  pInformation->ControlState = ControlState;
}

/*******************************************************************************
* Function Name  : NoData_Setup0.
* Description    : Proceed the processing of setup request without data stage.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void NoData_Setup0(void)
{
#ifdef USB_DEBUG1
	debug_printf("In function NoData_Setup0\n\r");
#endif
  RESULT Result = USB_UNSUPPORT;
  uint32_t RequestNo = pInformation->USBbRequest;
  uint32_t ControlState;

  if (Type_Recipient == (STANDARD_REQUEST | DEVICE_RECIPIENT))
  {
    /* Device Request*/
#ifdef USB_DEBUG
	debug_printf("Device Request in NoData_Setup0\n\r");
#endif
    /* SET_CONFIGURATION*/
    if (RequestNo == SET_CONFIGURATION)
    {
#ifdef USB_DEBUG1
	debug_printf("In SET_CONFIGURATION\n\r");
#endif
      Result = Standard_SetConfiguration();
    }

    /*SET ADDRESS*/
    else if (RequestNo == SET_ADDRESS)
    {
#ifdef USB_DEBUG1
	debug_printf("In SET ADDRESS\n\r");
#endif
      if ((pInformation->USBwValue0 > 127) || (pInformation->USBwValue1 != 0)
          || (pInformation->USBwIndex != 0)
          || (pInformation->Current_Configuration != 0))
        /* Device Address should be 127 or less*/
      {
        ControlState = STALLED;
        goto exit_NoData_Setup0;
      }
      else
      {
        Result = USB_SUCCESS;

      #ifdef STM32F10X_CL
         SetDeviceAddress(pInformation->USBwValue0);
      #endif  /* STM32F10X_CL */
      }
    }
    /*SET FEATURE for Device*/
    else if (RequestNo == SET_FEATURE)
    {
#ifdef USB_DEBUG1
	debug_printf("In SET FEATURE for Device\n\r");
#endif
      if ((pInformation->USBwValue0 == DEVICE_REMOTE_WAKEUP) \
          && (pInformation->USBwIndex == 0))
      {
        Result = Standard_SetDeviceFeature();
      }
      else
      {
		//LED_RED();
        Result = USB_UNSUPPORT;
      }
    }
    /*Clear FEATURE for Device */
    else if (RequestNo == CLEAR_FEATURE)
    {	
#ifdef USB_DEBUG1
	debug_printf("In Clear FEATURE for Device\n\r");
#endif

      if (pInformation->USBwValue0 == DEVICE_REMOTE_WAKEUP
          && pInformation->USBwIndex == 0
          && ValBit(pInformation->Current_Feature, 5))
      {
        Result = Standard_ClearFeature();
      }
      else
      {		
        Result = USB_UNSUPPORT;
      }
    }

  }

  /* Interface Request*/
  else if (Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT))
  {
#ifdef USB_DEBUG1
	debug_printf("In Interface Request\n\r");
#endif
    /*SET INTERFACE*/
    if (RequestNo == SET_INTERFACE)
    {
#ifdef USB_DEBUG1
	debug_printf("In Interface Request - SET_INTERFACE\n\r");
#endif
      Result = Standard_SetInterface();
    }
  }

  /* EndPoint Request*/
  else if (Type_Recipient == (STANDARD_REQUEST | ENDPOINT_RECIPIENT))
  {
#ifdef USB_DEBUG1
	debug_printf("In Endpoint Request\n\r");
#endif
    /*CLEAR FEATURE for EndPoint*/
    if (RequestNo == CLEAR_FEATURE)
    {
#ifdef USB_DEBUG1
	debug_printf("In Endpoint Request - CLEAR_FEATURE\n\r");
#endif
      Result = Standard_ClearFeature();
    }
    /* SET FEATURE for EndPoint*/
    else if (RequestNo == SET_FEATURE)
    {
#ifdef USB_DEBUG1
	debug_printf("In Endpoint Request - SET_FEATURE\n\r");
#endif	
      Result = Standard_SetEndPointFeature();
    }
  }
  else
  {	
    Result = USB_UNSUPPORT;
  }


  if (Result != USB_SUCCESS)
  {	
    Result = (*pProperty->Class_NoData_Setup)(RequestNo);
    if (Result == USB_NOT_READY)
    {
      ControlState = PAUSE;
      goto exit_NoData_Setup0;
    }
  }

  if (Result != USB_SUCCESS)
  {	
    ControlState = STALLED;
    goto exit_NoData_Setup0;
  }

  ControlState = WAIT_STATUS_IN;/* After no data stage SETUP */

  USB_StatusIn();

exit_NoData_Setup0:
  pInformation->ControlState = ControlState;
  return;
}

/*******************************************************************************
* Function Name  : Data_Setup0.
* Description    : Proceed the processing of setup request with data stage.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Data_Setup0(void)
{
#ifdef USB_DEBUG1
	debug_printf("In function Data_Setup0\n\r");
#endif
  USB_CONTROLLER_STATE *State = g_USB_Driver.pUsbControllerState;
  uint8_t *(*CopyRoutine)(uint16_t);
  RESULT Result;
  uint32_t Request_No = pInformation->USBbRequest;
  bool get_descriptor_flag = false;

  uint32_t Related_Endpoint, Reserved;
  uint32_t wOffset, Status;



  CopyRoutine = 0;
  wOffset = 0;
  
//-------------------------------------------------------------------//

  /*GET DESCRIPTOR*/
  if (Request_No == GET_DESCRIPTOR)
  {
  #ifdef USB_DEBUG1
		debug_printf("GET_DESCRIPTOR\n\r");
  #endif
    if (Type_Recipient == (STANDARD_REQUEST | DEVICE_RECIPIENT))
    {
      uint8_t wValue1 = pInformation->USBwValue1;
      if (wValue1 == DEVICE_DESCRIPTOR)
      {
	#ifdef USB_DEBUG1
		debug_printf("DEVICE_DESCRIPTOR\n\r");
	#endif
        CopyRoutine     = pProperty->GetDeviceDescriptor;		
		State->Data     = &g_USB_Driver.ControlPacketBuffer[0];
		State->DataSize = USB_DEVICE_DESCRIPTOR_LENGTH;
		//--//
		//Kartik : Retrieve the Device Descriptor from the PAL
		//--//		
      }
      else if (wValue1 == CONFIG_DESCRIPTOR)
      {
	#ifdef USB_DEBUG1
		debug_printf("CONFIG_DESCRIPTOR\n\r");
	#endif	  		
        CopyRoutine = pProperty->GetConfigDescriptor;
		State->Data     = &g_USB_Driver.ControlPacketBuffer[0];
		State->DataSize = USB_CONFIGURATION_DESCRIPTOR_LENGTH;
      }
      else if (wValue1 == STRING_DESCRIPTOR)
      {
	#ifdef USB_DEBUG1
		debug_printf("STRING_DESCRIPTOR\n\r");
	#endif
        CopyRoutine = pProperty->GetStringDescriptor;
		State->Data     = &g_USB_Driver.ControlPacketBuffer[0];
		State->DataSize = USB_STRING_DESCRIPTOR_MAX_LENGTH;
      }  /* End of GET_DESCRIPTOR */
    }
  }

  /*GET STATUS*/
  else if ((Request_No == GET_STATUS) && (pInformation->USBwValue == 0)
           && (pInformation->USBwLength == 0x0002)
           && (pInformation->USBwIndex1 == 0))
  {
#ifdef USB_DEBUG1
	debug_printf("GET_STATUS\n\r");
#endif
    /* GET STATUS for Device*/
    if ((Type_Recipient == (STANDARD_REQUEST | DEVICE_RECIPIENT))
        && (pInformation->USBwIndex == 0))
    {
	#ifdef USB_DEBUG1
		debug_printf("GET_STATUS FOR DEVICE\n\r");
	#endif
      CopyRoutine = Standard_GetStatus;
    }

    /* GET STATUS for Interface*/
    else if (Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT))
    {
	#ifdef USB_DEBUG1
		debug_printf("GET_STATUS FOR INTERFACE\n\r");
	#endif
    	
      if (((*pProperty->Class_Get_Interface_Setting)(pInformation->USBwIndex0, 0) == USB_SUCCESS)
          && (pInformation->Current_Configuration != 0))
      {
        CopyRoutine = Standard_GetStatus;
      }
    }

    /* GET STATUS for EndPoint*/
    else if (Type_Recipient == (STANDARD_REQUEST | ENDPOINT_RECIPIENT))
    {
	#ifdef USB_DEBUG1
		debug_printf("GET_STATUS FOR ENDPOINT\n\r");
	#endif    
      Related_Endpoint = (pInformation->USBwIndex0 & 0x0f);
      Reserved = pInformation->USBwIndex0 & 0x70;

      if (ValBit(pInformation->USBwIndex0, 7))
      {
        /*Get Status of endpoint & stall the request if the related_ENdpoint
        is Disabled*/
        Status = _GetEPTxStatus(Related_Endpoint);
      }
      else
      {
        Status = _GetEPRxStatus(Related_Endpoint);
      }

      if ((Related_Endpoint < Device_Table.Total_Endpoint) && (Reserved == 0)
          && (Status != 0))
      {
        CopyRoutine = Standard_GetStatus;
      }
    }

  }

  /*GET CONFIGURATION*/
  else if (Request_No == GET_CONFIGURATION)
  {
#ifdef USB_DEBUG1
	debug_printf("GET_CONFIGURATION\n\r");
#endif
    
    if (Type_Recipient == (STANDARD_REQUEST | DEVICE_RECIPIENT))
    {
#ifdef USB_DEBUG1
	debug_printf("GET_CONFIGURATION FOR DEVICE\n\r");
#endif
      CopyRoutine = Standard_GetConfiguration;
    }
  }
  /*GET INTERFACE*/
  else if (Request_No == GET_INTERFACE)
  {
#ifdef USB_DEBUG1
	debug_printf("GET_INTERFACE\n\r");
#endif
    if ((Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT))
        && (pInformation->Current_Configuration != 0) && (pInformation->USBwValue == 0)
        && (pInformation->USBwIndex1 == 0) && (pInformation->USBwLength == 0x0001)
        && ((*pProperty->Class_Get_Interface_Setting)(pInformation->USBwIndex0, 0) == USB_SUCCESS))
    {
#ifdef USB_DEBUG1
	debug_printf("GET_INTERFACE FOR INTERFACE\n\r");
#endif
      CopyRoutine = Standard_GetInterface;
    }

  }
  
//-------------------------------------------------------------------//  
  
  if (CopyRoutine)
  {
    pInformation->Ctrl_Info.Usb_wOffset = wOffset;
    pInformation->Ctrl_Info.CopyData = CopyRoutine;
    /* sb in the original the cast to word was directly */
    /* now the cast is made step by step */
    (*CopyRoutine)(0);
    Result = USB_SUCCESS;
  }
  else
  {
    Result = (*pProperty->Class_Data_Setup)(pInformation->USBbRequest);
    if (Result == USB_NOT_READY)
    {
	  //LED_RED();
      pInformation->ControlState = PAUSE;
      return;
    }
  }

  if (pInformation->Ctrl_Info.Usb_wLength == 0xFFFF)
  {
    /* Data is not ready, wait it */
    pInformation->ControlState = PAUSE;
    return;
  }
  if ((Result == USB_UNSUPPORT) || (pInformation->Ctrl_Info.Usb_wLength == 0))
  {
    /* Unsupported request */
    pInformation->ControlState = STALLED;
    return;
  }

  //-------------------------------------------------------------------//
  
  if (ValBit(pInformation->USBbmRequestType, 7))
  {
    /* Device ==> Host */
    __IO uint32_t wLength = pInformation->USBwLength;
     
    /* Restrict the data length to be the one host asks for */
    if (pInformation->Ctrl_Info.Usb_wLength > wLength)
    {
      pInformation->Ctrl_Info.Usb_wLength = wLength;
    }
    
    else if (pInformation->Ctrl_Info.Usb_wLength < pInformation->USBwLength)
    {
      if (pInformation->Ctrl_Info.Usb_wLength < pProperty->MaxPacketSize)
      {
        Data_Mul_MaxPacketSize = false;
      }
      else if ((pInformation->Ctrl_Info.Usb_wLength % pProperty->MaxPacketSize) == 0)
      {
        Data_Mul_MaxPacketSize = true;
      }
    }   

    pInformation->Ctrl_Info.PacketSize = pProperty->MaxPacketSize;
    DataStageIn();
  }
  else
  {
    pInformation->ControlState = OUT_DATA;
    vSetEPRxStatus(EP_RX_VALID); /* enable for next data reception */
  }
  
  //-------------------------------------------------------------------//
  
  return;
}

/*******************************************************************************
* Function Name  : Setup0_Process
* Description    : Get the device request data and dispatch to individual process.
* Input          : None.
* Output         : None.
* Return         : Post0_Process.
*******************************************************************************/
uint8_t Setup0_Process(void)
{
#ifdef USB_DEBUG1
	debug_printf("In function Setup0_Process\n\r");
#endif

  union
  {
    uint8_t* b;
    uint16_t* w;
  } pBuf;
  
#ifdef STM32F10X_CL
  USB_OTG_EP *ep;
  uint16_t offset = 0;
 
  ep = PCD_GetOutEP(ENDP0);
  pBuf.b = ep->xfer_buff;
#else  
  uint16_t offset = 1;
  
  pBuf.b = PMAAddr + (uint8_t *)(_GetEPRxAddr(ENDP0) * 2); /* *2 for 32 bits addr */
#endif /* STM32F10X_CL */

  if (pInformation->ControlState != PAUSE)
  {
	
	g_Setup_Packet.USBbmRequestType = *pBuf.b++;
    pInformation->USBbmRequestType   = g_Setup_Packet.USBbmRequestType; /* bmRequestType */
		
	g_Setup_Packet.USBbRequest = *pBuf.b++;
    pInformation->USBbRequest   = g_Setup_Packet.USBbRequest; /* bRequest */
	    
	pBuf.w += offset;  /* word not accessed because of 32 bits addressing */
	
	g_Setup_Packet.USBwValues = *pBuf.w++;
    pInformation->USBwValue   = ByteSwap(g_Setup_Packet.USBwValues); /* wValue */
	
	pBuf.w += offset;  /* word not accessed because of 32 bits addressing */
    
	g_Setup_Packet.USBwIndexs = *pBuf.w++;
	pInformation->USBwIndex   = ByteSwap(g_Setup_Packet.USBwIndexs); /* wIndex */
	
	pBuf.w += offset;  /* word not accessed because of 32 bits addressing */
    
	g_Setup_Packet.USBwLengths = *pBuf.w;
	pInformation->USBwLength = g_Setup_Packet.USBwLengths; /* wLength */    
	
	//Kartik : MF Integration
	//Kartik : Make the MF control packet structure point to the pBuf structure
	
	g_USB_Driver.ControlPacketBuffer[0] = pInformation->USBbmRequestType;	
	g_USB_Driver.ControlPacketBuffer[1] = pInformation->USBbRequest;		
	g_USB_Driver.ControlPacketBuffer[2] = pInformation->USBwValues.bw.bb1;
	g_USB_Driver.ControlPacketBuffer[3] = pInformation->USBwValues.bw.bb0;
	g_USB_Driver.ControlPacketBuffer[4] = pInformation->USBwIndexs.bw.bb1;	
	g_USB_Driver.ControlPacketBuffer[5] = pInformation->USBwIndexs.bw.bb0;	
	g_USB_Driver.ControlPacketBuffer[6] = pInformation->USBwLengths.bw.bb1;
	g_USB_Driver.ControlPacketBuffer[7] = pInformation->USBwLengths.bw.bb0;
	//--//
  }

  pInformation->ControlState = SETTING_UP;
  if (pInformation->USBwLength == 0)
  {
    /* Setup with no data stage */
    NoData_Setup0();
  }
  else
  {
    /* Setup with data stage */
    Data_Setup0();
  }
  return Post0_Process();
}

/*******************************************************************************
* Function Name  : In0_Process
* Description    : Process the IN token on all default endpoint.
* Input          : None.
* Output         : None.
* Return         : Post0_Process.
*******************************************************************************/
uint8_t In0_Process(void)
{
#ifdef USB_DEBUG1
	debug_printf("In function In0_Process\n\r");
#endif

  uint32_t ControlState = pInformation->ControlState;

  if ((ControlState == IN_DATA) || (ControlState == LAST_IN_DATA))
  {
#ifdef USB_DEBUG
	debug_printf("ControlState == IN_DATA) || (ControlState == LAST_IN_DATA\n\r");
	debug_printf("Going to call DataStageIn\n\r");
#endif
    DataStageIn();
    /* ControlState may be changed outside the function */
    ControlState = pInformation->ControlState;
  }

  else if (ControlState == WAIT_STATUS_IN)
  {
#ifdef USB_DEBUG
	debug_printf("ControlState == WAIT_STATUS_IN\n\r");	
#endif
    if ((pInformation->USBbRequest == SET_ADDRESS) &&
        (Type_Recipient == (STANDARD_REQUEST | DEVICE_RECIPIENT)))
    {
#ifdef USB_DEBUG1
	debug_printf("Going to call SET_ADDRESS\n\r");	
#endif
      SetDeviceAddress(pInformation->USBwValue0);
      pUser_Standard_Requests->User_SetDeviceAddress();
    }
    (*pProperty->Process_Status_IN)();
    ControlState = STALLED;
  }

  else
  {
    ControlState = STALLED;
  }

  pInformation->ControlState = ControlState;

  return Post0_Process();
}

/*******************************************************************************
* Function Name  : Out0_Process
* Description    : Process the OUT token on all default endpoint.
* Input          : None.
* Output         : None.
* Return         : Post0_Process.
*******************************************************************************/
uint8_t Out0_Process(void)
{
#ifdef USB_DEBUG1
	debug_printf("In function Out0_Process\n\r");
#endif

  uint32_t ControlState = pInformation->ControlState;

  if ((ControlState == IN_DATA) || (ControlState == LAST_IN_DATA))
  {
#ifdef USB_DEBUG1
	debug_printf("In Out0_Process - CKP1\n\r");
#endif
    /* host aborts the transfer before finish */
    ControlState = STALLED;
  }
  else if ((ControlState == OUT_DATA) || (ControlState == LAST_OUT_DATA))
  {
#ifdef USB_DEBUG1
	debug_printf("In Out0_Process - CKP2\n\r");
#endif
    DataStageOut();
    ControlState = pInformation->ControlState; /* may be changed outside the function */
  }

  else if (ControlState == WAIT_STATUS_OUT)
  {
#ifdef USB_DEBUG1
	debug_printf("in Out0_Process - CKP3\n\r");
#endif  
    (*pProperty->Process_Status_OUT)();
  #ifndef STM32F10X_CL
    ControlState = STALLED;
  #endif /* STM32F10X_CL */
  }


  /* Unexpect state, STALL the endpoint */
  else
  {
#ifdef USB_DEBUG1
	debug_printf("in Out0_Process - CKP4\n\r");
#endif
    ControlState = STALLED;
  }

  pInformation->ControlState = ControlState;

  return Post0_Process();
}

/*******************************************************************************
* Function Name  : Post0_Process
* Description    : Stall the Endpoint 0 in case of error.
* Input          : None.
* Output         : None.
* Return         : - 0 if the control State is in PAUSE
*                  - 1 if not.
*******************************************************************************/
uint8_t Post0_Process(void)
{
#ifdef USB_DEBUG
	debug_printf("Post0_Process\n\r");
#endif

#ifdef STM32F10X_CL  
  USB_OTG_EP *ep;
#endif /* STM32F10X_CL */
      
  SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);

  if (pInformation->ControlState == STALLED)
  {
    vSetEPRxStatus(EP_RX_STALL);
    vSetEPTxStatus(EP_TX_STALL);
  }

#ifdef STM32F10X_CL
  else if ((pInformation->ControlState == OUT_DATA) ||
      (pInformation->ControlState == WAIT_STATUS_OUT))
  {
    ep = PCD_GetInEP(0);
    ep->is_in = 0;
    OTGD_FS_EP0StartXfer(ep);
    
    vSetEPTxStatus(EP_TX_VALID);
  }
  
  else if ((pInformation->ControlState == IN_DATA) || 
      (pInformation->ControlState == WAIT_STATUS_IN))
  {
    ep = PCD_GetInEP(0);
    ep->is_in = 1;
    OTGD_FS_EP0StartXfer(ep);    
  }  
#endif /* STM32F10X_CL */

  return (pInformation->ControlState == PAUSE);
}

/*******************************************************************************
* Function Name  : SetDeviceAddress.
* Description    : Set the device and all the used Endpoints addresses.
* Input          : - Val: device address.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetDeviceAddress(uint8_t Val)
{
#ifdef USB_DEBUG
	debug_printf("SetDeviceAddress\n\r");
#endif

#ifdef STM32F10X_CL 
  PCD_EP_SetAddress ((uint8_t)Val);
#else 
  uint32_t i;
  uint32_t nEP = Device_Table.Total_Endpoint;

  /* set address in every used endpoint */
  for (i = 0; i < nEP; i++)
  {
    _SetEPAddress((uint8_t)i, (uint8_t)i);
  } /* for */
  _SetDADDR(Val | DADDR_EF); /* set device address and enable function */
#endif  /* STM32F10X_CL */  
}

/*******************************************************************************
* Function Name  : NOP_Process
* Description    : No operation function.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void NOP_Process(void)
{
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
