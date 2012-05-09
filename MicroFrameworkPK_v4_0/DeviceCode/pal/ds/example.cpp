#include <iostream>
#include "datastore.h"

using namespace std;

int gIndex = 0;

    Data_Store<int> myDataStore("456");


int test_datastore()
{
    LPVOID givenPtr = myDataStore.createRecord(9, 312);
	uint32 status ;
	int index = 0;

    LPVOID second = myDataStore.createRecord(10, 713);
    status = myDataStore.writeRawData( second,
									   (void *) "second",
									   6+1 );
    LPVOID third = myDataStore.createRecord(100, 22);
    myDataStore.writeRawData(third, (void *) "third", 5+1);

    myDataStore.createRecord(141, 91);

	for(BYTE index = 0; index < 255; index++){
		BYTE buff[1] = { 0 };
		//sprintf(buff, "%012d", index++);
		buff[0] = (BYTE) index;
        gIndex++;
        debug_printf("%d\n", gIndex);
        if(54 == gIndex){
            gIndex = gIndex;
        }
        if(4293 == gIndex){
            gIndex = gIndex;
        }
		status = myDataStore.writeRawData( givenPtr,
										   buff,
										   sizeof(buff) );
	}

    third = myDataStore.createRecord(100, 61);
    myDataStore.writeRawData(third, (void *) "third", 5+1);

    char buff1[512] = { 0 };
    int len = myDataStore.readRawData(givenPtr, buff1, sizeof(buff1));
    DATASTORE_ERROR err = myDataStore.getLastError();

    len = myDataStore.readRawData(third, buff1, sizeof(buff1));
    err = myDataStore.getLastError();
    return 0;
}
