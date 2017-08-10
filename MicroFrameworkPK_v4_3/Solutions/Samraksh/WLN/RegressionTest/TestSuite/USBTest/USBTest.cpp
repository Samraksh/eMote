#include "USBTest.h"
//#include "../../Math/Random.h"
//--//

//Random g_RandomInstance;
extern const ADS_PACKED USB_DYNAMIC_CONFIGURATION UsbDefaultConfiguration;


uint8_t SomeValue( int value )
{
        return ((value + 0x50)  % 0x100);
}

void USBTest::CloseStreams(){
	 USB_Flush(usbPort );
	 USB_CloseStream( usbPort );
	 //USB_Uninitialize( usbControler );     //disable the USB for the next application
}

BOOL USBTest::Execute()
{
    //  Transmit a buffer of 8 bytes to end-point; ... end-point expected to echo byte stream.
    //  Following transmit, read 8 bytes from end-point and validate as same
    //  See test\tools for echo tool

    //Log& log = Log::InitializeLog( Stream, "USB" );
	USB_CONTROLLER_STATE * UsbState = CPU_USB_GetState(0);
    //if(UsbState->Initialized && (UsbState->DeviceState > USB_DEVICE_STATE_ADDRESS))
    if(UsbState->Initialized)
    {
        //log.CloseLog( FALSE, "USART init fail" );
        //return FALSE;
        //LED_ORANGE();
    }else {

    	int conf=USB_Configure( usbControler, &UsbDefaultConfiguration );
    	if(!USB_Initialize( usbControler))
    	{
    		LED_RED();
    		return FALSE;
    	}
    }

	//wait for a few secs, this is approx 25 secs

	//CloseStreams();

	for(int ik=0;ik<1000000;ik++);

	/*BOOL open = USB_OpenStream(usbPort,USBTest::m_tx_ep, USBTest::m_rx_ep);

	if(open){
		debug_printf("USBTest:: StreamOpen success");
		LED_GREEN_OFF();
	}else {
		LED_BLUE();
		return FALSE;
	}
   */
	int writeFlag=0;
	BOOL forceWrite=FALSE;
	int write_count=0;
	int rd_count=0;
	int counter=0;
	while(1){
		//LED_GREEN_OFF();
		if(forceWrite){
			LED_ORANGE();
			if(writeFlag){
				if(!USB_Write( usbPort, USBTest::bigBuffer, write_count ))
				{
					//log.CloseLog( FALSE, "write failed" );
					return FALSE;
				}
				writeFlag=0;
				forceWrite=FALSE;
				write_count=0;
			}
		}else if (rd_count=USB_Read( usbPort, USBTest::m_recvBuffer, BUFFER_SIZE )) {
			//LED_GREEN();
			LED_ORANGE_OFF();
			if(write_count + rd_count > BIG_BUFFER_SIZE){
				forceWrite=TRUE;
			}else {
				if(rd_count!=0){
					memcpy(&USBTest::bigBuffer[write_count],USBTest::m_recvBuffer,rd_count);
					write_count+=rd_count;
					writeFlag=1;
					counter++;
					if(counter % 2==0){
						LED_BLUE();
					}else{
						LED_BLUE_OFF();
					}
				}
			}

			 debug_printf("Received %d messages\r\n", counter);
		}else if(write_count > 0){
			if(!USB_Write( usbPort, USBTest::bigBuffer, write_count ))
			{
					//log.CloseLog( FALSE, "write failed" );
					return FALSE;
			}
			writeFlag=0;
			forceWrite=FALSE;
			write_count=0;
		}
	}

   /* else
    {
       //Events_WaitForEvents( 0, 100 );
    	LED_ORANGE();
        USB_Read( usbPort, USBTest::m_recvBuffer, BUFFER_SIZE );
        debug_printf("USBTest:: ");
        for (int i=0; i<BUFFER_SIZE; i++){
        	debug_printf("%c",USBTest::m_recvBuffer[i]);
        }
        debug_printf("\r\n");

       if(Validate())
       {
    	   LED_WARNING();
          // log.CloseLog( TRUE, NULL );
       }
       else
       {
           //log.CloseLog( FALSE, "validate failed" );
    	   LED_ORANGE();
       }
    }*/
    return TRUE;
}

USBTest::USBTest( int numberOfEvents, int tx_ep, int rx_ep )
{
	this->numberOfEvents = numberOfEvents;
	this->m_tx_ep = tx_ep;
	this->m_rx_ep = rx_ep;
	InitializeXmitBuffer( );
	this->usbControler= ConvertCOM_UsbController(USB1);
	this->usbPort = ConvertCOM_UsbStream(USB1);
	//this->usbTestStream=1;

	//if(numberOfEvents < MAX_EVENTS)
	/*for(int i = 0; i < this->numberOfEvents; i++)
			randomBuffer[i] = g_RandomInstance.pareto_prng();*/
}




void USBTest::InitializeXmitBuffer()
{
	INT16 i=0;
    for (i=0; i<BUFFER_SIZE; i++)
    	m_xmitBuffer[i] = SomeValue(i);
}
BOOL USBTest::Validate()
{
    for(INT16 i=0; i<BUFFER_SIZE; i++)
    {
        if(m_recvBuffer[i] != (char) SomeValue(i))
        {
            return false;
        }
    }
    return true;
}

//--//

