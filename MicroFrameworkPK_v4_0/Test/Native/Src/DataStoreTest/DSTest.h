#ifndef _DATASTORE_TEST_H_
#define _DATASTORE_TEST_H_
#include <tinyhal.h>
#include <usb/netmf_usb.h>
#include <datastore.h>

#define DSTEST_LEVEL0 0
#define DSTEST_LEVEL1 1
#define DSTEST_LEVEL2 2

#define MAXRECORDNUMBER 500

#define NUM_RECORDS 10

#define BUFFER_SIZE 24
#define DATASTORE_BUFFER_SIZE 1024

#define STARTLISTENING 0x0f
#define STARTSENDING 0xf0

#define STOPLISTENING 0x01
#define STOPSENDING 0x10

#define TRASMITTING 0x11

typedef void* LPVOID;

LPVOID handleMapping[MAXRECORDNUMBER];

typedef struct _testData{
    int    recordID;
    LPVOID handle;
    //char   data[MAX_RECORD_SIZE];
}TEST_DATA;

static TEST_DATA data[NUM_RECORDS];

int gIndex = 0;
void *watchPtr = NULL;

class DataStoreTest
{
	int lastRecord;
	int lastRecordSize;
	int numberOfEvents;
	int m_tx_ep;
	int m_rx_ep;
	int usbControler;
	int usbPort;
	BYTE m_xmitBuffer[BUFFER_SIZE];
	BYTE m_recvBuffer[BUFFER_SIZE];
	BYTE m_dsBuffer[DATASTORE_BUFFER_SIZE];
	int currentRecordId;
	UINT16 bufferCounter;

	void copyToXmitBuffer(BYTE* buffer, int len);
	void InitializeXmitBuffer();
	void InitializeRecvBuffer();
	void InitializedsBuffer();
	BOOL Level_0();
	BOOL Level_1();
	BOOL Level_2();

public:
	DataStoreTest(int numberOfEvents, int tx_ep, int rx_ep);

	BOOL execute(int level);
};

Data_Store<int> myDataStore("NOR");

#endif
