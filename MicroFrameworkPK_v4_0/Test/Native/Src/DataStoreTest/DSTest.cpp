#include "DSTest.h"

#include <testMath.h>

#define MAX_RECORD_SIZE 24

#define NUM_WRITES 10000

extern Data_Store<UINT8> ds;

DataStoreTest::DataStoreTest(int numberOfEvents, int tx_ep, int rx_ep)
{
	this->numberOfEvents = numberOfEvents;
	this->m_tx_ep = tx_ep;
	this->m_rx_ep = rx_ep;
	InitializeXmitBuffer();
	InitializeRecvBuffer();
	currentRecordId = 0;
	bufferCounter = 0;
	lastRecord = 0;
	lastRecordSize = 0;
	InitializedsBuffer();
	this->usbControler= ConvertCOM_UsbController(USB1);
	this->usbPort = ConvertCOM_UsbStream(USB1);
}

void DataStoreTest::copyToXmitBuffer(BYTE* buffer, int len)
{
	for(UINT16 i = 0; i < len; i++)
	{
		m_xmitBuffer[i] = buffer[i];
	}
}

void DataStoreTest::InitializedsBuffer()
{
	INT16 i=0;
    for (i=0; i<DATASTORE_BUFFER_SIZE; i++)
    	m_dsBuffer[i] = 0;

}

void DataStoreTest::InitializeRecvBuffer()
{
	INT16 i=0;
    for (i=0; i<BUFFER_SIZE; i++)
    	m_recvBuffer[i] = 0;

}

void DataStoreTest::InitializeXmitBuffer()
{
	INT16 i=0;
    for (i=0; i<BUFFER_SIZE; i++)
    	m_xmitBuffer[i] = 0;
}

BOOL DataStoreTest::Level_0()
{
	Data_Store<int> ds("456");

    LPVOID givenPtr = ds.createRecord(9, 312);
	uint32 status ;
	int index = 0;
	char buff1[512] = { 0 };
	int len = 0;
    DATASTORE_ERROR err = ds.getLastError();

    BYTE inpBuffer[2];

    inpBuffer[0] = 0x10;
    inpBuffer[1] = 0x20;

    LPVOID second = ds.createRecord(10, 713);
    status = ds.writeRawData( second,
										inpBuffer,
									   2 );

    len = ds.readRawData(second,buff1,sizeof(buff1));

    inpBuffer[0] = 0x14;

    LPVOID third = ds.createRecord(100, 22);
    ds.writeRawData(third, inpBuffer, 2);

    inpBuffer[0] = 0x16;

    third = ds.createRecord(100, 61);
    ds.writeRawData(third, inpBuffer, 2);

    len = ds.readRawData(third, buff1, sizeof(buff1));
    err = ds.getLastError();
    return 0;

}

static BOOL dataCompare(char *inpData, char* readData, int len)
	{
		for(int i = 0; i < len; i++)
		{
			if(inpData[i] != readData[i])
				return FALSE;
		}
		return TRUE;
	}

static int writeToRecord(int recordIndex, char *ldata, int len){
	    if(recordIndex >= NUM_RECORDS){
	        return 0;
	    }
		char readData[256] = { 0 };

	    //memset(data[recordIndex].data, 0, sizeof(data[recordIndex].data));

	    //for(UINT8 i = 0; i < len; i++)
	    //	data[recordIndex].data[i] = ldata[i];
	    //strcpy( data[recordIndex].data, ldata);

		int randOffset = 10;
		CPU_GPIO_SetPinState(1,FALSE);
	    ds.writeRawData(data[recordIndex].handle, ldata, len);
	    CPU_GPIO_SetPinState(1,TRUE);
		//dataStoreHandle->writeRawData((char*)data[recordIndex].handle+randOffset, ldata+randOffset, strlen(ldata)-randOffset+1);

		memset(readData, 0, sizeof(readData));
		CPU_GPIO_SetPinState(2,FALSE);
		ds.readRawData(data[recordIndex].handle, readData, sizeof(readData));
		CPU_GPIO_SetPinState(2,TRUE);
		if(0 != dataCompare(readData, ldata, len)){
			hal_printf("Mismatch %d \n", recordIndex);
			ASSERT(false);
		}
	    return 0;
}

BOOL DataStoreTest::Level_1()
{
	    //remove("testDataStore2.flash");

		ds.init();

	    DATASTORE_STATUS status;

	    CPU_GPIO_EnableOutputPin(1,TRUE);
	    CPU_GPIO_EnableOutputPin(2,FALSE);

	    testMath testMathInstance;

	    testMathInstance.prng_init(500);

	    ASSERT(ds.getLastError() == DATASTORE_ERROR_NONE);

	    //srand(1932);    //Some seed value
		//srand(3321717521);

	    /* create records */
	    for(int index = 0; index < NUM_RECORDS; index++){
	        unsigned int randSize = testMathInstance.prng();
	        randSize = randSize % MAX_RECORD_SIZE;
	        randSize = 24;
	        data[index].handle = ds.createRecord(index, randSize);
	        if(NULL == data[index].handle){
	            //cout << "Error code = " << ds.getLastError();
	        	hal_printf("Error Code = %d", ds.getLastError());
	        }
	    }

	    for(int index = 0; index < NUM_WRITES; index++){
	        char buff[32] = {0};
	        int randNum = testMathInstance.prng();
	        int recordID = testMathInstance.prng()%NUM_RECORDS;
	        //sprintf(buff, "%d", randNum);
	        buff[0] = randNum >> 24;
	        buff[1] = randNum >> 16 & 0xff;
	        buff[2] = randNum >> 8 & 0xff;
	        buff[3] = randNum & 0xff;

	        writeToRecord(recordID, buff, sizeof(buff));    //Write into random records
	    }

	#if 0
	    bool isThereAFailure = false;
	    for(int index = 0; index < NUM_RECORDS; index++){
	        char buff[32] = { 0 };
	        ds.readRawData( data[index].handle,
	                                 buff,
	                                 sizeof(buff));
	        if(dataCompare(buff, data[index].data,sizeof(buff))){
	            isThereAFailure = true;
	           // cout << "Checking data failed for record num "<<index << "\n" << "Error code is"<<ds.getLastError()<<"\n";
	//            cout << "Expected = " <<data[index].data <<"  | Retreived = " << buff <<"\n";
	            hal_printf("Checking data failed for record num %d \n", index);
	            hal_printf("Error Code is %d \n", ds.getLastError());
	        }
	    }
	    //cout << (isThereAFailure == true?"There was a failure!":"The test was successful!") <<"\n";
	    if(isThereAFailure == TRUE)
	    	hal_printf("There was a failure !!!!\n");
	    else
	    	hal_printf("The test was successfull! \n");
	#endif
	    return TRUE;
}

BOOL DataStoreTest::Level_2()
{
	//Data_Store<int> ds = Data_Store<int>::getInstance();
	//Data_Store<int> ds;

	currentRecordId = 0;

	USB_CONTROLLER_STATE * UsbState = CPU_USB_GetState(0);
	if(!UsbState->Initialized)
	{
		int conf=USB_Configure( usbControler, &UsbDefaultConfiguration );

		if(!USB_Initialize( usbControler))
		{
		    		LED_RED();
		    		return FALSE;
		}
	}

	while(1){
		UINT32 rd_count = USB_Read( usbPort, (char *) DataStoreTest::m_recvBuffer, BUFFER_SIZE );

		if(rd_count != 0)
		{
			if(m_recvBuffer[0] == STARTLISTENING)
			{
				while(1)
				{
					rd_count = USB_Read( usbPort,(char *) DataStoreTest::m_recvBuffer, BUFFER_SIZE );

					if(m_recvBuffer[0] == STOPLISTENING)
					{
						void * handle = ds.createRecord(currentRecordId++, bufferCounter);
						ds.writeRawData(handle,m_dsBuffer,bufferCounter);
						handleMapping[currentRecordId - 1] = handle;
						lastRecord = currentRecordId -1;
						lastRecordSize = bufferCounter;
						InitializedsBuffer();
						break;
					}
					else if(m_recvBuffer[0] == TRASMITTING)
					{
						m_dsBuffer[bufferCounter++] = m_recvBuffer[3];
						if(bufferCounter == DATASTORE_BUFFER_SIZE)
						{
							void * handle = ds.createRecord(currentRecordId++, DATASTORE_BUFFER_SIZE);
							ds.writeRawData(handle,m_dsBuffer,DATASTORE_BUFFER_SIZE);
							handleMapping[currentRecordId - 1] = handle;
							InitializedsBuffer();
							bufferCounter = 0;
						}
					}
					else
					{
						InitializeXmitBuffer();
					}
					// Flushing the receive buffer
					InitializeRecvBuffer();
					USB_Flush(usbPort);
					USB_Flush(usbPort);
				}
			}
			else if((m_recvBuffer[0] == STARTSENDING) || (m_recvBuffer[1] == STARTSENDING))
			{
				//rd_count = USB_Read( usbPort,(char *) DataStoreTest::m_recvBuffer, BUFFER_SIZE );
				//rd_count = USB_Read( usbPort,(char *) DataStoreTest::m_recvBuffer, BUFFER_SIZE );
				lastRecord += 1;
				for(UINT32 i = 0; i < lastRecord; i++)
				{
					ds.readRawData(handleMapping[i], m_dsBuffer, DATASTORE_BUFFER_SIZE);
					copyToXmitBuffer(m_dsBuffer,DATASTORE_BUFFER_SIZE);
					if(!USB_Write( usbPort, (char *) DataStoreTest::m_xmitBuffer, DATASTORE_BUFFER_SIZE ))
					{
						debug_printf("Write Failed \n");
					}
					for(UINT32 j = 0; j < 10000; j++);
					InitializedsBuffer();
				}
				ds.readRawData(handleMapping[lastRecord], m_dsBuffer, lastRecordSize);
				copyToXmitBuffer(m_dsBuffer,lastRecordSize);
				if(!USB_Write( usbPort,(char *) DataStoreTest::m_xmitBuffer, lastRecordSize ))
				{
					debug_printf("Write Failed \n");
				}
			}

			InitializeRecvBuffer();
			USB_Flush(usbPort);
			USB_Flush(usbPort);
		}
	}

}

BOOL DataStoreTest::execute(int level)
{
	if(level == DSTEST_LEVEL1)
		Level_1();
	if(level == DSTEST_LEVEL2)
		Level_2();

}
