#ifndef _ADDRESS_TABLE_H_INCLUDED_
#define _ADDRESS_TABLE_H_INCLUDED_

#include <vector>

using namespace std;

typedef struct
{
    RECORD_ID  recordID;           /* Unique allocation identifier */
    LPVOID     givenPtr;           /* Smart pointer that was given to the application */
    DWORD      allocationSize;     /* Size of the allocation */
    LPVOID     currentLoc;         /* Current location of the corresponding data on the flash */
}DATASTORE_ADDR_TBL_ENTRY;



/* 
    The table is optimized for the Lookup using given_address
*/
class DATASTORE_AddrTable
{
private:
    vector<DATASTORE_ADDR_TBL_ENTRY> table;
    /* Add anything required later */
    DATASTORE_STATUS search(LPVOID givenAddr, DATASTORE_ADDR_TBL_ENTRY *entry);
public:
    DATASTORE_AddrTable();
    DATASTORE_STATUS addEntry(DATASTORE_ADDR_TBL_ENTRY *entry);
    LPVOID           getCurrentLoc(RECORD_ID recordID);
    LPVOID           getCurrentLoc(LPVOID givenPtr);
    LPVOID           getGivenAddress(RECORD_ID recordID);
    RECORD_ID        getRecordID(LPVOID addr);
    uint32           getAllocationSize(RECORD_ID recordID);
    uint32           getMaxWriteSize(LPVOID addr);      /* Tells number of bytes that can be written from the given point */
    DATASTORE_STATUS updateCurrentLocation(RECORD_ID recordID, LPVOID newLoc);
    DATASTORE_STATUS removeEntry(RECORD_ID recordID); 
    DATASTORE_STATUS removeEntry(LPVOID givenPtr);
     ~DATASTORE_AddrTable();
};


#endif
