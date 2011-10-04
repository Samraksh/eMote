////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <usb/netmf_usb.h>

// the v3.1 arm compiler optimizations for RTM cause the USB not to work
#if !defined(DEBUG)
#pragma O1
#endif

//USB Debug Flag
#undef USB_DEBUG
//#define USB_NIVED_DEBUG
#undef USB_NIVED_DEBUG
//#define USB_DEBUG1
#undef USB_DEBUG1
//#define USB_DEBUG2
#undef USB_DEBUG2

USBCS_Driver g_USB_Driver;

USB_CONTROLLER_STATE UsbControllerState[1];     // Only 1 USB Controller for this device

Hal_Queue_KnownSize<USB_PACKET64,USB_QUEUE_PACKET_COUNT> QueueBuffers[USBCS_Driver::c_Used_Endpoints-1];

//Kartik : This points to the USB Configuration (UNION), need to remap!
//static USB_Driver::EndpointConfiguration EndpointInit[USBCS_Driver::c_Used_Endpoints];     // Corresponds to endpoint configuration RAM at PXA271_USB::UDCCRx

BOOL Send_Multiple_Packets = FALSE;
UINT32 State = IDLE;
//--//

//
//Kartik : Defining the STM Global Structures, which are called from usb_core
//
DEVICE Device_Table =
  {
    EP_NUM,
    1
  };

DEVICE_PROP Device_Property =
  {
    STM_USB_Init,
	STM_USB_Reset,
    STM_USB_Status_In,
    STM_USB_Status_Out,	
    STM_USB_Data_Setup,
    STM_USB_NoData_Setup,
    STM_USB_Get_Interface_Setting,
    STM_USB_GetDeviceDescriptor,
    STM_USB_GetConfigDescriptor,
    STM_USB_GetStringDescriptor,
	0,
    0x40 /*MAX PACKET SIZE*/
  };

USER_STANDARD_REQUESTS User_Standard_Requests =
  {
    STM_USB_GetConfiguration,
    STM_USB_SetConfiguration,
    STM_USB_GetInterface,
    STM_USB_SetInterface,
    STM_USB_GetStatus,
    STM_USB_ClearFeature,
    STM_USB_SetEndPointFeature,
    STM_USB_SetDeviceFeature,
    STM_USB_SetDeviceAddress
  };
//
//
void STM_USB_Init()
{
#ifdef USB_DEBUG
	debug_printf("In STM_USB_Init\n\r");
#endif
	//H/W init happens in the MF USB Init, just update state variables

	/* Update the serial number string descriptor with the data from the unique ID*/
	//Get_SerialNum();

	pInformation->Current_Configuration = 0;

	/* Connect the device */
	//PowerOn();

	/* Perform basic device initialization operations */
	//USB_SIL_Init();
	
	bDeviceState = UNCONNECTED;
}

void STM_USB_Reset()
{
#ifdef USB_DEBUG
	debug_printf("In STM_USB_Reset\n\r");
#endif

	/* Set the device as not configured */
	Device_Info.Current_Configuration = 0;

    /* Current Feature initialization */
    pInformation->Current_Feature = MASS_ConfigDescriptor[7];

    bDeviceState = ATTACHED;

	//CBW.dSignature = BOT_CBW_SIGNATURE;
	//Bot_State = BOT_IDLE;
	//USB_NotConfigured_LED();
}

void STM_USB_SetConfiguration(void)
{
#ifdef USB_DEBUG
	debug_printf("In STM_USB_SetConfiguration\n\r");
#endif

	if (pInformation->Current_Configuration != 0)
	{
		/* Device configured */
		bDeviceState = CONFIGURED;

		ClearDTOG_TX(ENDP1);
		ClearDTOG_RX(ENDP2);

		//Bot_State = BOT_IDLE; /* set the Bot state machine to the IDLE state */
	}
}

void STM_USB_ClearFeature(void)
{

#ifdef USB_DEBUG
	debug_printf("In STM_USB_ClearFeature\n\r");
#endif
	/* when the host send a CBW with invalid signature or invalid length the two
     Endpoints (IN & OUT) shall stall until receiving a Mass Storage Reset     */
	//if (CBW.dSignature != BOT_CBW_SIGNATURE)
	//Bot_Abort(BOTH_DIR);
}

void STM_USB_SetDeviceAddress (void)
{
#ifdef USB_DEBUG
	debug_printf("In STM_USB_SetDeviceAddress\n\r");
#endif
	bDeviceState = ADDRESSED;
}

void STM_USB_Status_In(void)
{
#ifdef USB_DEBUG
	debug_printf("In function STM_USB_Status_In\n\r\r");	
#endif
	return;
}

void STM_USB_Status_Out(void)
{
#ifdef USB_DEBUG
	debug_printf("In function STM_USB_Status_Out\n\r");
#endif
	return;
}

RESULT STM_USB_Data_Setup(uint8_t RequestNo)
{
	//debug_printf("In STM_USB_Data_Setup\n\r");
#ifdef USB_DEBUG2
	debug_printf("In STM_USB_Data_Setup\n\r");
#endif
#if 0
	uint8_t    *(*CopyRoutine)(uint16_t);

	CopyRoutine = NULL;
	if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT)) && (RequestNo == GET_MAX_LUN) && (pInformation->USBwValue == 0)
      && (pInformation->USBwIndex == 0) && (pInformation->USBwLength == 0x01))
	{
		CopyRoutine = Get_Max_Lun;
	}
	else
	{
		return USB_UNSUPPORT;
    }

	if (CopyRoutine == NULL)
	{
		return USB_UNSUPPORT;
	}

	pInformation->Ctrl_Info.CopyData = CopyRoutine;
	pInformation->Ctrl_Info.Usb_wOffset = 0;
	(*CopyRoutine)(0);
#endif
	return USB_SUCCESS;
}

RESULT STM_USB_NoData_Setup(uint8_t RequestNo)
{
	//debug_printf("In STM_USB_NoData_Setup\n\r");
#ifdef USB_DEBUG2
	debug_printf("In STM_USB_NoData_Setup\n\r");
#endif
#if 0
	if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
      && (RequestNo == MASS_STORAGE_RESET) && (pInformation->USBwValue == 0)
      && (pInformation->USBwIndex == 0) && (pInformation->USBwLength == 0x00))
	{
		/* Initialize Endpoint 1 */
		ClearDTOG_TX(ENDP1);

		/* Initialize Endpoint 2 */
		ClearDTOG_RX(ENDP2);  

		/*initialize the CBW signature to enable the clear feature*/
		//CBW.dSignature = BOT_CBW_SIGNATURE;
		//Bot_State = BOT_IDLE;

		return USB_SUCCESS;
	}
	return USB_UNSUPPORT;
#endif	
	return USB_SUCCESS;
}

RESULT STM_USB_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting)
{
#ifdef USB_DEBUG
	debug_printf("In STM_USB_Get_Interface_Setting\n\r");
#endif
	
	if (AlternateSetting > 0)
	{
		return USB_UNSUPPORT;/* in this application we don't have AlternateSetting*/
	}
	else if (Interface > 0)
	{
		return USB_UNSUPPORT;/*in this application we have only 1 interfaces*/
	}
	
	return USB_SUCCESS;
}

uint8_t* STM_USB_GetDeviceDescriptor(uint16_t Length)
{
#ifdef USB_DEBUG
	debug_printf("In STM_USB_GetDeviceDescriptor\n\r");
#endif	

#ifdef USB_NIVED_DEBUG
	debug_printf("In STM_USB_GetDeviceDescriptor\n\r");
#endif

	return Standard_GetDescriptorData(Length, &Device_Descriptor );
}

uint8_t* STM_USB_GetConfigDescriptor(uint16_t Length)
{
#ifdef USB_DEBUG
	debug_printf("In STM_USB_GetConfigDescriptor\n\r");
#endif	
#ifdef USB_NIVED_DEBUG
	debug_printf("In STM_USB_GetConfigDescriptor\n\r");
#endif

	return Standard_GetDescriptorData(Length, &Config_Descriptor );
}

uint8_t* STM_USB_GetStringDescriptor(uint16_t Length)
{
#ifdef USB_DEBUG
	debug_printf("In STM_USB_GetStringDescriptor\n\r");
#endif
#ifdef USB_NIVED_DEBUG
	debug_printf("In STM_USB_GetStringDescriptor\n\r");
#endif
	
	
	uint8_t wValue0 = pInformation->USBwValue0;	

	if (wValue0 > 5)
	{
	   return NULL;
	}
	else
	{
		return Standard_GetDescriptorData(Length, &String_Descriptor[wValue0]);
	}
}

uint8_t* Get_Max_Lun(uint16_t Length)
{
#ifdef USB_DEBUG
	debug_printf("In Get_Max_Lun\n\r");
#endif
	if (Length == 0)
	{
		pInformation->Ctrl_Info.Usb_wLength = LUN_DATA_LENGTH;
		return 0;
	}
	else
	{
	    return((uint8_t*)(&Max_Lun));
	}
}

void STM_USB_GetConfiguration()
{
#ifdef USB_DEBUG
	debug_printf("In STM_USB_GetConfiguration\n\r");
#endif
}

void STM_USB_GetInterface()
{
#ifdef USB_DEBUG
	debug_printf("In STM_USB_GetInterface\n\r");
#endif
}

void STM_USB_SetInterface()
{
#ifdef USB_DEBUG
	debug_printf("In STM_USB_SetInterface\n\r");
#endif
}

void STM_USB_GetStatus()
{
#ifdef USB_DEBUG
	debug_printf("In STM_USB_GetStatus\n\r");
#endif
}

void STM_USB_SetEndPointFeature()
{
#ifdef USB_DEBUG
	debug_printf("In STM_USB_SetEndPointFeature\n\r");
#endif
}

void STM_USB_SetDeviceFeature()
{
#ifdef USB_DEBUG
	debug_printf("In STM_USB_SetDeviceFeature\n\r");
#endif
}


//
//NET MF Functions
//
USB_CONTROLLER_STATE* USBCS_Driver::GetState( int Controller )
{
#ifdef USB_DEBUG
	debug_printf("In USBCS_Driver::GetState\n\r");
#endif

    if( Controller != 0 )       // There is only one controller for this device
        return NULL;
    return &UsbControllerState[0];
}

HRESULT USBCS_Driver::Initialize( int Controller )
{   
#ifdef USB_DEBUG
	debug_printf("In USBCS_Driver::Initialize\n\r");
#endif	

	int endpointsUsed = 0;

    const USB_ENDPOINT_DESCRIPTOR  *ep    = NULL;
    const USB_INTERFACE_DESCRIPTOR *itfc  = NULL;

	USB_CONTROLLER_STATE &State = UsbControllerState[0];
	ASSERT(0 == Controller);	

	//Set up GPIO
	Set_System();
	//Enable Clock
	USB_Clock();
	//Enable uC Interrupts
	USB_Interrupts_Config();

	/* Connect the device */
	StartHardware();
	
	/* Perform basic device initialization operations */
	USB_SIL_Init();

	//Initialize STM state data structures
	pInformation = &Device_Info;
	pInformation->ControlState = 2;
	pProperty = &Device_Property;
	pUser_Standard_Requests = &User_Standard_Requests;
	/* Initialize devices one by one */
	pProperty->Init();

	//TODO: Initialize all end points to unused

	//Configure other end points (endpoint no 1)
	while( USB_NextEndpoint( &State, ep, itfc) ) // && logicalEndpoint < 11 )
    {
        // Figure out which endpoint we are initializing
        UINT8 endpointNum = ep->bEndpointAddress & 0x7F;

		UINT16 endpointSize = ep->wMaxPacketSize;
        
        // Check interface and endpoint numbers against hardware capability
        if( endpointNum >= USBCS_Driver::c_Used_Endpoints || itfc->bInterfaceNumber > 3 )
            return S_FALSE;

	    // Set the maximum size of the endpoint hardware FIFO
        if( (ep->bmAttributes & 0x03) == USB_ENDPOINT_ATTRIBUTE_BULK )
        {
			if( endpointSize != 8 && endpointSize != 16 && endpointSize != 32 && endpointSize != 64 )
			{
				return S_FALSE;
			}
            //TODO : Move this value (endpointSize) to the packet size in the EP register
			State.MaxPacketSize[endpointNum]   = endpointSize;
		}
		else if( (ep->bmAttributes & 0x03) == USB_ENDPOINT_ATTRIBUTE_INTERRUPT )
        {
            if( endpointSize == 0 || endpointSize > 64 )
			{
                return S_FALSE;
			}
            //TODO : Move this value (endpointSize) to the packet size in the EP register
            State.MaxPacketSize[endpointNum]   = endpointSize;
        }
        else        // Isochronous endpoint
        {
            if( endpointSize > 64 )
                endpointSize = 64;
            //TODO : Move this value (endpointSize) to the packet size in the EP register
            State.MaxPacketSize[endpointNum]   = endpointSize;
        }

		// Since endpoint 0 is only used for control, there is never a need to allocate a buffer for it
        // In fact State.Queues[0] is always NULL - it is a cheap placeholder to make the queueIndex = endpointIndex
        QueueBuffers[endpointNum-1].Initialize();                       // Clear queue before use
        State.Queues[endpointNum] = &QueueBuffers[endpointNum-1];       // Attach queue to endpoint

		if( (ep->bEndpointAddress & 0x80) ? 1 : 0 )        // If transmit endpoint
        {
            State.IsTxQueue[endpointNum] = TRUE;
			//TODO : Enable the interrupt for the end point, based on the KIND (ep->bmAttributes & 0x03)
			//TODO : Set direction of the end point
		}
		else        // Receive endpoint
        {
            State.IsTxQueue[endpointNum] = FALSE;
			//TODO : Enable the interrupt for the end point, based on the KIND (ep->bmAttributes & 0x03)
			//TODO : Set direction of the end point
		}

		//endpointsUsed++;

	}

    g_USB_Driver.pUsbControllerState  = &State;
    g_USB_Driver.PinsProtected        = TRUE;

    State.EndpointStatus = &g_USB_Driver.EndpointStatus[0];
    State.EndpointCount  = c_Used_Endpoints;
    //State->DeviceStatus   = USB_STATUS_DEVICE_SELF_POWERED;
    State.PacketSize     = c_default_ctrl_packet_size;
    
    State.FirstGetDescriptor = TRUE;
	
	SetEPRxStatus(EP2_OUT, EP_RX_VALID);

	//Kartik : Commented for now
    //ProtectPins( Controller, FALSE );

    return S_OK;    
}

HRESULT USBCS_Driver::Uninitialize( int Controller )
{ 
#ifdef USB_DEBUG
	debug_printf("In USBCS_Driver::Uninitialize\n\r");
#endif

    ASSERT( 0 == Controller );
    
    GLOBAL_LOCK(irq);

    ProtectPins( Controller, TRUE );

    g_USB_Driver.pUsbControllerState = NULL;

	//TODO : Disable Interrupts 
	//TODO : Disable Clocks

    return S_OK;
}

BOOL USBCS_Driver::StartOutput( USB_CONTROLLER_STATE* State, int endpoint )
{ 
#ifdef USB_DEBUG
	debug_printf("In USBCS_Driver::StartOutput\n\r");
#endif
//#if 0
	//debug_printf("In USBCS_Driver::StartOutput\n\r");
	
	USB_PACKET64* Packet64;
	Packet64 = USB_TxDequeue( State, endpoint, TRUE );
	//const char* Data = "Hello From USB";
	//USB_SIL_Write(EP1_IN, (uint8_t*) Data, 14);	
	USB_SIL_Write(EP1_IN, (uint8_t*) Packet64->Buffer, Packet64->Size);	
	SetEPTxValid(ENDP1); 		
	SetEPRxStatus(ENDP2, EP_RX_VALID);
    SetEPRxStatus(EP2_OUT, EP_RX_VALID);
//#endif
#if 0	
    ASSERT( State );
    ASSERT(endpoint < c_Used_Endpoints);

    GLOBAL_LOCK(irq);

    // If endpoint is not an output
    if( State->Queues[endpoint] == NULL || !State->IsTxQueue[endpoint] )
        return FALSE;

    /* if the halt feature for this endpoint is set, then just
       clear all the characters */
    if(State->EndpointStatus[endpoint] & USB_STATUS_ENDPOINT_HALT)
    {
        ClearTxQueue( State, endpoint );
        return TRUE;
    }

    //If TxRunning, interrupts will drain the queue
    if(!g_USB_Driver.TxRunning[endpoint])
    {
        g_USB_Driver.TxRunning[endpoint] = TRUE;

        // Calling both TxPacket & EP_TxISR in this routine could cause a TX FIFO overflow
        //TxPacket( State, endpoint );		
    }
    else if(irq.WasDisabled())
    {                        
		//LED_BLUE();
		//EP_TxISR( 1 );
        //PXA271_USB& USB = PXA271::USB();
        
        // This could be called during Flush with all interrupts off.  Just taking care of the endpoint
        // in question may cause a logjam if the host is expecting a response from another endpoint.
        
		
		//TODO : Check All endpoints for activity.

        //if( USB.UDCISR0 & USB.UDCICR0 & USB.UDCICR__BOTH )        // If endpoint 0 needs attention
        //    EP0_ISR( 0 );
        //for( int ep = 1; ep < c_Used_Endpoints; ep++ )
        //{
        //    // If no interrupt for this endpoint
        //    if( (((USB.UDCISR0 & USB.UDCICR0) >> (ep * 2)) & USB.UDCICR__BOTH) == 0 )
        //        continue;
        //    
        //    if(State->Queues[ep])
        //    {
        //        if( State->IsTxQueue[endpoint] )
        //            EP_TxISR( endpoint );
        //        else
        //            EP_RxISR( endpoint );
        //    }
        //}
    }
#endif
    return TRUE;

}

BOOL USBCS_Driver::GetInterruptState()
{
#ifdef USB_DEBUG
	debug_printf("In USBCS_Driver::GetInterruptState\n\r");
#endif

    GLOBAL_LOCK(irq);

    if(GetISTR()) 
    {
        return TRUE;
    }
    
    return FALSE;
}

//--//

void USBCS_Driver::ClearTxQueue( USB_CONTROLLER_STATE* State, int endpoint )
{
#ifdef USB_DEBUG
	debug_printf("In USBCS_Driver::ClearTxQueue\n\r");
#endif

    ASSERT(State);    
    State->Queues[endpoint]->Initialize();
}

//--//
void USBCS_Driver::StartHardware()
{       
#ifdef USB_DEBUG
	debug_printf("In USBCS_Driver::StartHardware\n\r");
#endif
  uint16_t wRegVal;
  
  /*** cable plugged-in ? ***/
  /*while(!CablePluggedIn());*/
  USB_Cable_Config(ENABLE);

  /*** CNTR_PWDN = 0 ***/
  wRegVal = CNTR_FRES;
  _SetCNTR(wRegVal);

  /*** CNTR_FRES = 0 ***/
  wInterrupt_Mask = 0;
  _SetCNTR(wInterrupt_Mask);
  /*** Clear pending interrupts ***/
  _SetISTR(0);
  /*** Set interrupt mask ***/
  wInterrupt_Mask = CNTR_RESETM | CNTR_SUSPM | CNTR_WKUPM | CNTR_ERRM; //| CNTR_CTRM | CNTR_DOVRM | CNTR_ESOFM | CNTR_SOFM | CNTR_RESUME | CNTR_FSUSP | CNTR_LPMODE | CNTR_PDWN | CNTR_FRES;
  _SetCNTR(wInterrupt_Mask);
}

/*
This functions turns off the USB core
*/
void USBCS_Driver::StopHardware()
{    
#ifdef USB_DEBUG
	debug_printf("In USBCS_Driver::StopHardware\n\r");
#endif
	SetCNTR(CNTR_PDWN);
}

void USBCS_Driver::TxPacket( USB_CONTROLLER_STATE* State, int endpoint )
{
#ifdef USB_DEBUG
	debug_printf("In USBCS_Driver::TxPacket\n\r");
#endif

    ASSERT( endpoint < c_Used_Endpoints );    
    ASSERT( State );

    GLOBAL_LOCK(irq); 

    // transmit a packet on UsbPortNum, if there are no more packets to transmit, then die    
    USB_PACKET64* Packet64;


    // If this is not a legal transmit endpoint, there is nothing to do
    if( State->Queues[endpoint] == NULL || !State->IsTxQueue[endpoint] )
        return;

    for(;;)
    {
        Packet64 = USB_TxDequeue( State, endpoint, TRUE );

        if( Packet64 == NULL || Packet64->Size > 0 )
            break;
    }
    
    if( Packet64 )
    {
		// If a packet has been sent
        //if(USB.UDCCSRx[endpoint] & USB.UDCCSR__PC)              
		//{
			// Clear the Packet Complete bit
			//USB.UDCCSRx[endpoint] = USB.UDCCSR__PC;             
		//}
            
        // We should absolutely have an empty buffer to use
        //ASSERT((USB.UDCCSRx[endpoint] & USB.UDCCSR__FS) != 0);  

		// FIFO may only be loaded with full words
        //UINT32* packet4 = (UINT32*)Packet64->Buffer;            
		//USB_SIL_Write(EP1_IN, (uint8_t*) Packet64->Buffer, Packet64->Size);	
		//SetEPTxValid(ENDP1); 		
		
		if (Packet64->Size > BULK_MAX_PACKET_SIZE)
		{
			//Data needs to be packetized and sent			
			Send_Multiple_Packets = TRUE;
		}
        else
		{
			//can be sent in one transfer
			Send_Multiple_Packets = FALSE;						
		}
        //while( nLeft >= 4 )
        //{
			// Shove packet into FIFO 4 bytes at a time
            //USB.UDCDRx[endpoint] = *packet4++;                  
        //    nLeft -= 4;
        //}
		
		//Send_Data();

		// If packet is not the full size
        //if( Packet64->Size < State->MaxPacketSize[endpoint] )   
        //{
        //    UINT16* packet2 = (UINT16*)packet4;
            
        //    if( nLeft >= 2 )
        //    {
                //*(volatile UINT16*)&USB.UDCDRx[endpoint] = *packet2++;
        //        nLeft -= 2;
        //    }
        //    if( nLeft > 0)
        //    {
                //*(volatile UINT8*)&USB.UDCDRx[endpoint] = *(UINT8*)packet2;
        //    }
			// Send a short packet (full length packets are automatically sent)
            //USB.UDCCSRx[endpoint] = USB.UDCCSR__SP;             
        //}

        g_USB_Driver.TxNeedZLPS[endpoint] = (Packet64->Size == State->MaxPacketSize[endpoint]);
    }
    else
    {
        // send the zero length packet since we landed on the FIFO boundary before
        // (and we queued a zero length packet to transmit) 
        if(g_USB_Driver.TxNeedZLPS[endpoint])
        {
			// If a packet has been sent
            //if(USB.UDCCSRx[endpoint] & USB.UDCCSR__PC)              
			//{
				// Clear the Packet Complete bit
				//USB.UDCCSRx[endpoint] = USB.UDCCSR__PC;             
			//}
            
            
			// We should absolutely have an empty buffer to use
            //ASSERT((USB.UDCCSRx[endpoint] & USB.UDCCSR__FS) != 0);  

			// Send a short (zero length) packet            
			SetEPTxCount(ENDP1, 0); 
			//vSetEPTxStatus(EP_TX_VALID);			
            g_USB_Driver.TxNeedZLPS[endpoint] = FALSE;
        }

        // no more data
        g_USB_Driver.TxRunning[endpoint] = FALSE;
    }

    
}

void Send_Data()
{	
}

void USBCS_Driver::ControlNext()
{    
#ifdef USB_DEBUG
	debug_printf("In USBCS_Driver::ControlNext");
#endif

    ASSERT(g_USB_Driver.pUsbControllerState);
    //PXA271_USB& USB = PXA271::USB();
    USB_CONTROLLER_STATE *State = g_USB_Driver.pUsbControllerState;

    if( State->DataCallback )
    {
        /* this call can't fail */
        State->DataCallback( State );


        if( State->DataSize == 0 )
        {
			// Send zero length packet
            //USB.UDCCSRx[0] = PXA271_USB::UDCCSR__IPR;         
			// Stop sending stuff if we're done
            State->DataCallback = NULL;                         
        }
        else
        {
			// FIFO may only be loaded with full words
            UINT32* packet4 = (UINT32*)State->Data;             
            int     nLeft   = State->DataSize;
            while( nLeft >= 4 )
            {
				// Shove packet into FIFO 4 bytes at a time
                //USB.UDCDRx[0] = *packet4++;                     
                nLeft -= 4;
            }

			// If packet is less than full length
            if(State->DataSize < c_default_ctrl_packet_size)    
            {
                UINT16* packet2 = (UINT16*)packet4;
                
                if( nLeft >= 2 )
                {
                    //*(volatile UINT16*)&USB.UDCDRx[0] = *packet2++;
                    nLeft -= 2;
                }
                if( nLeft > 0)
                {
                    //*(volatile UINT8*)&USB.UDCDRx[0] = *(UINT8*)packet2;
                }
				// Send a short packet (full length packets are automatically sent)
                //USB.UDCCSRx[0] = USB.UDCCSR__IPR;

				// Stop sending stuff if we're done
                State->DataCallback = NULL;                     
            }


            // special handling the USB driver set address test, cannot use the first descriptor as the ADDRESS state is handle in the hardware
            if(g_USB_Driver.FirstDescriptorPacket)
            {
                State->DataCallback = NULL;
            }

       }
    }


}

void USBCS_Driver::SuspendEvent()
{
#ifdef USB_DEBUG
	debug_printf("In USBCS_Driver::SuspendEvent");
#endif

    ASSERT(g_USB_Driver.pUsbControllerState);

    //PXA271_USB& USB = PXA271::USB();
    
	USB_CONTROLLER_STATE *State = g_USB_Driver.pUsbControllerState;

    // SUSPEND event only happened when Host(PC) set the device to SUSPEND
    // as there is always SOF every 1ms on the BUS to keep the device from
    // suspending. Therefore, the REMOTE wake up is not necessary at the ollie side

    //USB.UDCICR1 = PXA271_USB::UDCICR1__IECC | PXA271_USB::UDCICR1__IERS | PXA271_USB::UDCICR1__IERU | PXA271_USB::UDCICR1__IESU;

    g_USB_Driver.PreviousDeviceState = State->DeviceState;

    State->DeviceState = USB_DEVICE_STATE_SUSPENDED;

    USB_StateCallback( State );

#if defined(DEBUG_USB)
    USB_debug_printf("In suspend event previous %d\n\r\r", g_USB_Driver.PreviousDeviceState);
#endif    
}

void USBCS_Driver::ResumeEvent()
{
#ifdef USB_DEBUG
	debug_printf("In USBCS_Driver::ResumeEvent\n\r");
#endif

	ASSERT(g_USB_Driver.pUsbControllerState);
	USB_CONTROLLER_STATE *State = g_USB_Driver.pUsbControllerState;

#if defined(DEBUG_USB)
    USB_debug_printf(" in Resume\r\n\r");
#endif

    //USB.UDCICR1 = PXA271_USB::UDCICR1__IECC | PXA271_USB::UDCICR1__IERS | PXA271_USB::UDCICR1__IERU | PXA271_USB::UDCICR1__IESU;

    State->DeviceState = g_USB_Driver.PreviousDeviceState;

    USB_StateCallback( State );
}



void USBCS_Driver::ResetEvent()
{
#ifdef USB_DEBUG1
	debug_printf("In USBCS_Driver::ResetEvent\n\r\r");
#endif

	//Call to the STM function
	Device_Property.Reset();

	USB_CONTROLLER_STATE *State = g_USB_Driver.pUsbControllerState;
	// The hardware handles most of the reset procedure.

	SetBTABLE(BTABLE_ADDRESS);
	// Initialize Endpoint 0 
	SetEPType(ENDP0, EP_CONTROL);
	SetEPTxStatus(ENDP0, EP_TX_NAK);
	SetEPRxAddr(ENDP0, ENDP0_RXADDR);
	SetEPRxCount(ENDP0, 0x40);
	SetEPTxAddr(ENDP0, ENDP0_TXADDR);
	Clear_Status_Out(ENDP0);
	SetEPRxValid(ENDP0);

    /* Initialize Endpoint 1 */
    SetEPType(ENDP1, EP_BULK);
    SetEPTxAddr(ENDP1, ENDP1_TXADDR);
    SetEPTxStatus(ENDP1, EP_TX_NAK);
    SetEPRxStatus(ENDP1, EP_RX_DIS);

    /* Initialize Endpoint 2 */
    SetEPType(ENDP2, EP_BULK);
    SetEPRxAddr(ENDP2, ENDP2_RXADDR);
    SetEPRxCount(ENDP2, 0x40);
    SetEPRxStatus(ENDP2, EP_RX_VALID);
    SetEPTxStatus(ENDP2, EP_TX_DIS);


    SetEPRxCount(ENDP0, 0x40);
    SetEPRxValid(ENDP0);

    /* Set the device to response on default address */
    SetDeviceAddress(0);


	/* clear all flags */
    USB_ClearEvent( 0, USB_EVENT_ALL);

#if defined(DEBUG_USB)
    USB_debug_printf(" in Reset\r\n\r");

    if( State->DeviceState >= USB_DEVICE_STATE_ADDRESS )
    {
        static int count = 0;
        USB_debug_printf("\fRESET: %d  \r\n\r", ++count);
    }
#endif

    for(int ep = 0; ep < c_Used_Endpoints; ep++)
    {
        g_USB_Driver.TxRunning [ep] = FALSE;
        g_USB_Driver.TxNeedZLPS[ep] = FALSE;
    }


    State->DeviceState        = USB_DEVICE_STATE_DEFAULT;
    State->Address            = 0;
    USB_StateCallback( State );    
    
}

//--//

BOOL USBCS_Driver::RxEnable( USB_CONTROLLER_STATE *State, int endpoint )
{
#ifdef USB_DEBUG
	debug_printf("In USBCS_Driver::RxEnable\n\r");
#endif

	if( NULL == State || endpoint >= c_Used_Endpoints )
        return FALSE;
    
    GLOBAL_LOCK(irq);

    /* unmask receive event */
    //USB.Set_Interrupt(endpoint, PXA271_USB::UDCICR__BOTH);      // Enable both interrupts for this endpoint
    
    return TRUE;        
}

BOOL USBCS_Driver::ProtectPins( int Controller, BOOL On )
{
#ifdef USB_DEBUG
	debug_printf("In USBCS_Driver::ProtectPins\n\r");
#endif

    ASSERT( 0 == Controller );
    USB_CONTROLLER_STATE *State = g_USB_Driver.pUsbControllerState;
    
    GLOBAL_LOCK(irq);

    // Initialized yet?
    if( State )
    {
        if( On )
        {
            if(!g_USB_Driver.PinsProtected)
            {
                // Disable the USB com, state change from Not protected to Protected

                g_USB_Driver.PinsProtected = TRUE;

                // clear USB Txbuffers
                for(int ep = 0; ep < c_Used_Endpoints; ep++)
                {
                    if( State->IsTxQueue[ep] )
                        ClearTxQueue( State, ep );
                }

                // stop clock, disable interrupt, dettach USB port
                // set the USBBus->DeviceState = DETACHED
				//Kartik: Set the correct pin number here; For signalling the host
                //CPU_GPIO_EnableInputPin(PXA271_USB::c_USBC_GPIOX_EN, FALSE, NULL, GPIO_INT_NONE, RESISTOR_DISABLED);     // Disable host signal driver
                State->DeviceState = USB_DEVICE_STATE_DETACHED;
                USB_StateCallback( State );

#if defined(DEBUG_USB)
                USB_debug_printf(" DETACHED USB in protect pin\n\r\r");
                lcd_printf("Detached USB in protect pin\n\r\r");
#endif
                StopHardware();
            }
        }
        else
        {
            if( g_USB_Driver.PinsProtected )
            {
                // Ready for USB to enable, state change from Protected to Not protected
                g_USB_Driver.PinsProtected = FALSE;

                // enable the clock,
                // set USB to attached/powered
                // set the device to a known state- Attached before it is set to the powered state (USB specf 9.1.1)

                //CPU_GPIO_EnableInputPin(PXA271_USB::c_USBC_GPION_DET, FALSE, NULL, GPIO_INT_NONE, RESISTOR_DISABLED);
                //CPU_GPIO_EnableOutputPin(PXA271_USB::c_USBC_GPIOX_EN, FALSE);       // Don't signal the host yet
                
				State->DeviceState = USB_DEVICE_STATE_ATTACHED;
                
                USB_StateCallback( State );

                StartHardware();
            }
        }

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void USBCS_Driver::Global_ISR( void* Param )
{ 
#ifdef USB_DEBUG
	debug_printf("In USBCS_Driver::Global_ISR\n\r");
#endif
}


void USBCS_Driver::EP0_ISR( UINT32 Param )
{   
#ifdef USB_DEBUG
	debug_printf("In USBCS_Driver::EP0_ISR\n\r");
#endif 
}

//void EP1_IN_Callback()
void USBCS_Driver::EP_TxISR( UINT32 endpoint )
{
#ifdef USB_DEBUG
	debug_printf("In USBCS_Driver::EP_TxISR\n\r");
#endif    		
	//TxPacket( g_USB_Driver.pUsbControllerState, 1 );
	//LED_BLUE();
}

void USBCS_Driver::EP_RxISR( UINT32 endpoint )
{
#ifdef USB_DEBUG
	debug_printf("In USBCS_Driver::EP_RxISR\n\r");
#endif    	
	//LED_RED();
}

void USBCS_Driver::USB_Interrupts_Config()
{
#ifdef USB_DEBUG
	debug_printf("In USBCS_Driver::USB_Interrupts_Config\n\r");
#endif

	//Replace with calls to Interrupt controller

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void USBCS_Driver::USB_Clock()
{
#ifdef USB_DEBUG
	debug_printf("In USB_Clock\n\r");
#endif

	RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
	/* Enable the USB clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}

void USBCS_Driver::Set_System()
{
#ifdef USB_DEBUG
	debug_printf("In Set_System\n\r");
#endif

	GPIO_InitTypeDef GPIO_InitStructure;
	//This configuration sets the USB GPIO to not use USB_USE_EXTERNAL_PULLUP, the external Pull up has a different configurations
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_DISCONNECT, ENABLE);

    /* Configure USB pull-up pin */
    GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);

}

void USBCS_Driver::USB_Cable_Config(bool state)
{
#ifdef USB_DEBUG
	debug_printf("In USB_Cable_Config\n\r");
#endif

  if (state != false)
  {
    GPIO_ResetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  }
  else
  {
    GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  }
}

//GLobal ISR for the USB
void usb_irq_handler()
{
	
#ifdef USB_DEBUG
	//debug_printf("In the USB Handler\n\r");
#endif	
	wIstr = _GetISTR();

#if (IMR_MSK & ISTR_CTR)
  if (wIstr & ISTR_CTR & wInterrupt_Mask)
  {
    /* servicing of the endpoint correct transfer interrupt */
    /* clear of the CTR flag into the sub */
#ifdef USB_DEBUG
	debug_printf("\nGoing to call CTR_LP\n\r");
#endif	
    CTR_LP();
#ifdef CTR_CALLBACK
    CTR_Callback();
#endif
  }
#endif  
  /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
#if (IMR_MSK & ISTR_RESET)
  if (wIstr & ISTR_RESET & wInterrupt_Mask)
  {
#ifdef USB_DEBUG
	debug_printf("\nGoing to call RESET\n\r");
#endif	
    _SetISTR((uint16_t)CLR_RESET);
	//Kartik : Calling this function from the ResetEvent to update state
    //Device_Property.Reset();
	USBCS_Driver::ResetEvent();
#ifdef RESET_CALLBACK
    RESET_Callback();
#endif
  }
#endif
  /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
#if (IMR_MSK & ISTR_DOVR)
  if (wIstr & ISTR_DOVR & wInterrupt_Mask)
  {
#ifdef USB_DEBUG
	debug_printf("\nGoing to call ISTR_DOVR\n\r");
#endif	
    _SetISTR((uint16_t)CLR_DOVR);
#ifdef DOVR_CALLBACK
    DOVR_Callback();
#endif
  }
#endif
  /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
#if (IMR_MSK & ISTR_ERR)
  if (wIstr & ISTR_ERR & wInterrupt_Mask)
  {
#ifdef USB_DEBUG
	debug_printf("\nGoing to call ISTR_ERR\n\r");
#endif	
    _SetISTR((uint16_t)CLR_ERR);
#ifdef ERR_CALLBACK
    ERR_Callback();
#endif
  }
#endif
  /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
#if (IMR_MSK & ISTR_WKUP)
  if (wIstr & ISTR_WKUP & wInterrupt_Mask)
  {
 #ifdef USB_DEBUG
	debug_printf("\nGoing to call ISTR_WKUP\n\r");
#endif	
    _SetISTR((uint16_t)CLR_WKUP);
    //Resume(RESUME_EXTERNAL);
#ifdef WKUP_CALLBACK
    WKUP_Callback();
#endif
  }
#endif
  /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
#if (IMR_MSK & ISTR_SUSP)
  if (wIstr & ISTR_SUSP & wInterrupt_Mask)
  {
#ifdef USB_DEBUG
	debug_printf("\nGoing to call SUSPEND\n\r");
#endif	
    /* check if SUSPEND is possible */
    //if (fSuspendEnabled)
    //{
    //  Suspend();
    //}
    //else
    //{
    //  /* if not possible then resume after xx ms */
    //  Resume(RESUME_LATER);
    //}
    /* clear of the ISTR bit must be done after setting of CNTR_FSUSP */
    _SetISTR((uint16_t)CLR_SUSP);
//#ifdef SUSP_CALLBACK
  //  SUSP_Callback();
//#endif
  }
#endif
  /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
//#if (IMR_MSK & ISTR_SOF)
  if (wIstr & ISTR_SOF & wInterrupt_Mask)
  {
#ifdef USB_DEBUG
	debug_printf("\nGoing to call Start of Frame\n\r");
#endif	
    _SetISTR((uint16_t)CLR_SOF);
    bIntPackSOF++;

//#ifdef SOF_CALLBACK
    //SOF_Callback();
//#endif
  }
//#endif
  /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
//#if (IMR_MSK & ISTR_ESOF)
  if (wIstr & ISTR_ESOF & wInterrupt_Mask)
  {
#ifdef USB_DEBUG
	debug_printf("\nGoing to call End of Start Frame\n\r");
#endif	
    _SetISTR((uint16_t)CLR_ESOF);
    /* resume handling timing is made with ESOFs */
    //Resume(RESUME_ESOF); /* request without change of the machine state */

//#ifdef ESOF_CALLBACK
    //ESOF_Callback();
//#endif
  }
//#endif
}