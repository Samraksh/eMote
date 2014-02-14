//#include <assert.h>

#include "Samraksh/DataStore/types.h"
//#include "Samraksh/DataStore/DatastoreInt.h"
#include "Samraksh/DataStore/AddressTable.h"


/****************************************************************************
*
*  Function Name : DATASTORE_AddrTable
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
DATASTORE_AddrTable::DATASTORE_AddrTable(){
	 table.clear();
}

DATASTORE_STATUS DATASTORE_AddrTable::copyEntry(myVector *ltable)
{
	int index;
	for(index=0;index<table.size();index++)
	{
		ltable->insert(index,table[index]);
	}
	return DATASTORE_STATUS_OK;
}

/* Chethan :: Move this somewhere later */
__inline int comparePtrRange(DATASTORE_ADDR_TBL_ENTRY *entry, LPVOID addr){
    int retVal = -1;
    if(entry->givenPtr > addr){
        /* Given address is less than this address range */
        retVal = -1;
    }else if(((char*)entry->givenPtr + entry->allocationSize - 1 ) < addr ){
        /* Given address is greater than this address range */
        retVal = +1;
    }else{
        retVal = 0;
    }
    return retVal;
}



/****************************************************************************
*
*  Function Name : ~DATASTORE_AddrTable
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
DATASTORE_STATUS DATASTORE_AddrTable::search( LPVOID givenAddr,
                                              DATASTORE_ADDR_TBL_ENTRY *entry )
{
    DATASTORE_STATUS status = DATASTORE_STATUS_INVALID_PARAM;
    int low, high, middle;
    do{
        if( givenAddr == NULL ||
            entry == NULL ){
            status = DATASTORE_STATUS_INVALID_PARAM;
            break;
        }
        if(table.size() == 0){
            status = DATASTORE_STATUS_NOT_OK;
            break;
        }

        low = 0; high = table.size()-1;
        middle = (low+high)/2;
        while(low <= high){
            if(comparePtrRange( &table[middle], givenAddr) == +1){
                low = middle + 1;
                middle = (low+high)/2;
            }else if(comparePtrRange( &table[middle], givenAddr) == -1){
                high = middle - 1;
                middle = (low+high)/2;
            }else{
                entry->allocationSize = table[middle].allocationSize;
                entry->currentLoc     = table[middle].currentLoc;
                entry->givenPtr       = table[middle].givenPtr;
                entry->recordID       = table[middle].recordID;

                status = DATASTORE_STATUS_OK;
                break;
            }
        }
    }while(0);
    return status;
}


/****************************************************************************
*
*  Function Name : addEntry
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
DATASTORE_STATUS DATASTORE_AddrTable::addEntry(DATASTORE_ADDR_TBL_ENTRY *entry)
{
    /* Check if the recordID is already there, if not, add it to the able, or else
       don't */
    DATASTORE_STATUS status = DATASTORE_STATUS_INVALID_PARAM;
    int index = 0;
    do{
        if(NULL == entry){
            status = DATASTORE_STATUS_INVALID_PARAM;
            break;
        }
        /* Check if the entry for this record already exists - We need to check using
           recordID, as we need no guarantee that there is no duplication of recordID */
        if(NULL != getCurrentLoc(entry->recordID)){
            status = DATASTORE_STATUS_RECORD_ALREADY_EXISTS;
            break;
        }
        /* Also search for the address the given_address value to ensure no duplication property */
        if(NULL != getCurrentLoc(entry->givenPtr, (LPVOID)0x01, (LPVOID)(~0u))){
            status = DATASTORE_STATUS_RECORD_ALREADY_EXISTS;
            break;
        }
        /* Now we can create an enty in the address table */
        /* Since, we know that the givenAddr is usually an monotonically increasing value, its efficient
           to search for place to insert by searching from the last element */
		for(index = table.size() - 1; index >= 0; index--){
			if(table[index].givenPtr < entry->givenPtr){
				/* Insert here */
				table.insert(table.begin()+index+1, *entry);
				break;  /* Break is pretty important :) */
			}
		}

        if(index < 0){
            /* Didn't insert anywhere as the given element is the smallest element or table is empty, so
               insert at the beginning */
            table.insert(table.begin(), *entry);
        }
        status = DATASTORE_STATUS_OK;
    }while(0);
    return status;
#if 0
	DATASTORE_STATUS status = DATASTORE_STATUS_NOT_OK;
	if(NULL == getCurrentLoc(entry->recordID)){
		table.push_back(*entry);
		status = DATASTORE_STATUS_OK;
	}
	return status;
#endif
}


/****************************************************************************
*
*  Function Name : addEntry
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
LPVOID  DATASTORE_AddrTable::getCurrentLoc(RECORD_ID recordID)
{/* Need to do a linear search, so O(n) */
    LPVOID retVal = NULL;
    for(int index = 0; index < table.size(); index++){
		if(table[index].recordID == recordID){
			retVal = table[index].currentLoc;
			break;
		}
    }
    return retVal;
}


/****************************************************************************
*
*  Function Name : addEntry
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
LPVOID DATASTORE_AddrTable::getCurrentLoc(LPVOID givenPtr, LPVOID startPtr, LPVOID endPtr)
{/* Table is sorted in ascending order on givenPtr field, hence can do a binary search
    As, this is the API that is most extensively called, this optimization is justified */
    LPVOID retVal = NULL;
    DATASTORE_ADDR_TBL_ENTRY tblEntry;
    do{
        if( DATASTORE_STATUS_OK == search( givenPtr, &tblEntry ) ){
            retVal = tblEntry.currentLoc;
            // Nived : Consult change here
            //retVal = (char*)retVal + ((char*)givenPtr - tblEntry.givenPtr);
            retVal = (char*)retVal + ((char*)givenPtr - (char *)tblEntry.givenPtr);
        }
    }while(0);
    if((char*)retVal > (char*)endPtr){
        /* Need to wrap it to the beginning */
    	// Nived : Consult change here
    	retVal = (char*)startPtr + (((char*)retVal - (char*)endPtr) - 1);
        //retVal = (char*)startPtr + (((char*)retVal - (char*)endPtr) - 1);
    }
    return retVal;
}


/****************************************************************************
*
*  Function Name : addEntry
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
LPVOID DATASTORE_AddrTable::getGivenAddress(RECORD_ID recordID)
{/* Linear search */
    LPVOID retVal = NULL;
    for(int index = 0; index < table.size(); index++){
		if(table[index].recordID == recordID){
			retVal = table[index].givenPtr;
			break;
		}
    }
    return retVal;
}

/****************************************************************************
*
*  Function Name : getGivenPointer(LPVOID currentPtr)
*
******************************************************************************/
/*!
*  \brief:	For the passed currentPtr, returns the corresponding givenPtr.
*
*  \param
*  \param
*  \return
*
******************************************************************************/
LPVOID DATASTORE_AddrTable::getGivenPointer(LPVOID currentPtr)
{
	LPVOID retVal = NULL;
    for(int index = 0; index < table.size(); index++){
		if(table[index].currentLoc <= currentPtr && ((char*)table[index].currentLoc + table[index].allocationSize) > currentPtr){
			retVal = table[index].givenPtr;
			break;
		}
	}
	return retVal;
}


/****************************************************************************
*
*  Function Name :
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
RECORD_ID DATASTORE_AddrTable::getRecordID(LPVOID givenAddr)
{/* Binary search */
    RECORD_ID retVal = -1;
    DATASTORE_ADDR_TBL_ENTRY tblEntry;
    if( DATASTORE_STATUS_OK == search(givenAddr, &tblEntry) ){
        retVal = tblEntry.recordID;
    }
    return retVal;

#if 0
    RECORD_ID recordID = -1;
    for(int index = 0; index < table.size(); index++){
		char *startAddr = (char*)table[index].givenPtr;
		char *endAddr	= startAddr + table[index].allocationSize - 1;
		if( (startAddr <= (char*)addr) &&
			(endAddr >= (char*)addr) ){
				recordID = table[index].recordID;
				break;
		}
	}
	return recordID;
#endif
}


/****************************************************************************
*
*  Function Name : ~DATASTORE_AddrTable
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
uint32 DATASTORE_AddrTable::getMaxWriteSize(LPVOID givenAddr)
{
    uint32 retVal = 0;
    DATASTORE_ADDR_TBL_ENTRY entry;
    do{
        if( DATASTORE_STATUS_OK != search(givenAddr, &entry)){
            break;
        }

        // Nived Need to consult
        // retVal = entry.allocationSize - (uint32)((char*)givenAddr - entry.givenPtr);
        retVal = entry.allocationSize - (uint32)((char*)givenAddr - (char *) entry.givenPtr);
    }while(0);
	return retVal;
}


/****************************************************************************
*
*  Function Name : ~DATASTORE_AddrTable
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
uint32 DATASTORE_AddrTable::getAllocationSize(RECORD_ID recordID)
{
    uint32 retVal = 0;
    for(int index = 0; index < table.size(); index++){
        if(table[index].recordID == recordID){
			retVal = table[index].allocationSize;
            break;
        }
    }
    return retVal;
}


/****************************************************************************
*
*  Function Name : ~DATASTORE_AddrTable
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
DATASTORE_STATUS DATASTORE_AddrTable::removeEntry(RECORD_ID recordID)
{
	DATASTORE_STATUS status = DATASTORE_STATUS_NOT_OK;
    for(int index = 0; index < table.size(); index++){
		if(table[index].recordID == recordID){
			table.erase(table.begin()+index);
			status = DATASTORE_STATUS_OK;
			break;
		}
    }
	return status;
}


/****************************************************************************
*
*  Function Name : ~DATASTORE_AddrTable
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
DATASTORE_STATUS DATASTORE_AddrTable::removeEntry(LPVOID givenPtr)
{
	DATASTORE_STATUS status = DATASTORE_STATUS_NOT_OK;
	for(int index = 0; index < table.size(); index++){
		char *startAddr = (char*)table[index].givenPtr;
		char *endAddr	= startAddr + table[index].allocationSize - 1;
		if( (startAddr <= (char*)givenPtr) &&
			(endAddr >= (char*)givenPtr) ){
			table.erase(table.begin()+index);
			status = DATASTORE_STATUS_OK;
			break;
		}
	}
	return status;
}


/****************************************************************************
*
*  Function Name :
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
DATASTORE_STATUS DATASTORE_AddrTable::updateCurrentLocation(RECORD_ID recordID, LPVOID newLoc)
{
    DATASTORE_STATUS status = DATASTORE_STATUS_NOT_OK;
    for(int index = 0; index < table.size(); index++){
        if(table[index].recordID == recordID){
            table[index].currentLoc = newLoc;
            status = DATASTORE_STATUS_OK;
            break;
        }
    }
    return status;
}


/****************************************************************************
*
*  Function Name : ~DATASTORE_AddrTable
*
******************************************************************************/
/*!
*  \brief
*
*  \param
*  \param
*  \return
*
******************************************************************************/
DATASTORE_AddrTable::~DATASTORE_AddrTable(){
	table.clear();
}


