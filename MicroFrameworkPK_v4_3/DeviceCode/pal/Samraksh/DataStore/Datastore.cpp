#include <Samraksh/DataStore/Datastore.h>

Data_Store g_dataStoreObject;

/* AnanthAtSamraksh - To get rid of "undefined reference to static member error" */
int Data_Store::startCount = 0;
int Data_Store::endCount = 0;


/*
 * Datastore constructor
 */
Data_Store::Data_Store(char *flashDeviceName, bool eraseDataStore)
{
    state = DATASTORE_STATE_UNINIT;
    DATASTORE_STATUS status;

    lastErrorVal = DATASTORE_ERROR_INVALID_PARAM;
    status = initDataStore( flashDeviceName,
                            &defaultProperty, eraseDataStore );

    state = (status == DATASTORE_STATUS_OK)?(DATASTORE_STATE_READY):(DATASTORE_STATE_INT_ERROR);
}

/*
 * Datastore constructor
 */
Data_Store::Data_Store( char *flashDeviceName,
                           DATASTORE_PROPERTIES *property, bool eraseDataStore )
{
    state = DATASTORE_STATE_UNINIT;
    DATASTORE_STATUS status;

    lastErrorVal = DATASTORE_ERROR_INVALID_PARAM;
    status = initDataStore( flashDeviceName,
                            property, eraseDataStore );

    state = (status == DATASTORE_STATUS_OK)?(DATASTORE_STATE_READY):(DATASTORE_STATE_INT_ERROR);
}

/*
 * Initializes the datastore. If the parameter "eraseDataStore" is true, then NOR flash is erased.
 * Returns success or failure.
 */
DeviceStatus Data_Store::init(bool eraseDataStore)
{
	 if(initialized == TRUE)
		 return DS_Success;

	 myUniquePtrGen.init();

	 ByteAddress Address;

	 if (!BlockStorageList::FindDeviceForPhysicalAddress(&blockStorageDevice, DATASTORE_START_ADDRESS, Address))
	 {
		return DS_Fail;
	 }

	 if((blockDeviceInformation = blockStorageDevice->GetDeviceInfo()) == NULL)
		 return DS_Fail;


	 state = DATASTORE_STATE_UNINIT;
	 DATASTORE_STATUS status;

	 //lastErrorVal = DATASTORE_ERROR_INVALID_PARAM;

	 status = initDataStore( (char*)"NOR", &defaultProperty, eraseDataStore );

	 state = (status == DATASTORE_STATUS_OK)?(DATASTORE_STATE_READY):(DATASTORE_STATE_INT_ERROR);
	 initialized = TRUE;

	 return DS_Success;
}



// Helper function for the addressTable. Gets a table, copies it to a new table and returns it
LPVOID Data_Store::copyAddressTable(myVector *table)
{
	addressTable.copyEntry(table);
	return NULL;
}

/*
 * Helper function to increment log pointer.
 * Invoked by functions that create a new allocation, create a dummy allocation and
 * 		the function that compacts or does the garbage collection.
 */
void Data_Store::incrementLogPointer( uint32 amount )
{
    logPointByteOffset += amount;
    if(logPointByteOffset > dataStoreEndByteOffset){
        /* Going out of the range - correct it */
        logPointByteOffset = (logPointByteOffset - dataStoreEndByteOffset) + dataStoreStartByteOffset - 1;
    }
    DATASTORE_ASSERT( (logPointByteOffset >= dataStoreStartByteOffset && logPointByteOffset <= dataStoreEndByteOffset),
                      "logPointByteOffset went out of range!" );
    if((logPointByteOffset < dataStoreStartByteOffset ||
        logPointByteOffset > dataStoreEndByteOffset)){
        ASSERT(false);
    }
}

/*
 * Helper function to increment log pointer.
 * Invoked by the function that does the garbage collection.
 */
void Data_Store::incrementErasePoint(uint32 amount)
{
    erasePointByteOffset += amount;
    if(erasePointByteOffset > dataStoreEndByteOffset){
        /* Going out of the range - correct it */
        erasePointByteOffset = (erasePointByteOffset - dataStoreEndByteOffset) + dataStoreStartByteOffset - 1;
    }
    DATASTORE_ASSERT( (erasePointByteOffset >= dataStoreStartByteOffset && erasePointByteOffset <= dataStoreEndByteOffset),
                      "erasePointByteOffset went out of range!" );
    if((erasePointByteOffset < dataStoreStartByteOffset ||
        erasePointByteOffset > dataStoreEndByteOffset)){
        ASSERT(false);
    }
}

/*
 * Helper function to increment clear pointer.
 * Invoked by the function that does the garbage collection.
 */
void Data_Store::incrementClearPoint(uint32 amount)
{
    clearLogPointByOffset += amount;
    if(clearLogPointByOffset > dataStoreEndByteOffset){
        /* Going out of the range - correct it */
        clearLogPointByOffset = (clearLogPointByOffset - dataStoreEndByteOffset) + dataStoreStartByteOffset - 1;
    }
    DATASTORE_ASSERT( (clearLogPointByOffset >= dataStoreStartByteOffset && clearLogPointByOffset <= dataStoreEndByteOffset),
                      "erasePointByteOffset went out of range!" );
    if((clearLogPointByOffset < dataStoreStartByteOffset ||
        clearLogPointByOffset > dataStoreEndByteOffset)){
    	ASSERT(false);
    }
}

/*
 * Helper function to safely increment any location on the flash by the specified "numBytes".
 */
LPVOID Data_Store::incrementPointer(LPVOID inputPtr, int numBytes)
{
    char *lPtr = (char*)inputPtr;

    char *lDataStoreEndPtr = (char*)blockDeviceInformation->Regions->Start + dataStoreEndByteOffset;
    char *lDataStoreStartPtr = (char*)blockDeviceInformation->Regions->Start + dataStoreStartByteOffset;

    lPtr = lPtr + numBytes;
    if(lPtr > lDataStoreEndPtr){
        lPtr = lDataStoreStartPtr + (lPtr - lDataStoreEndPtr - 1);
    }else if(lPtr < lDataStoreStartPtr){
    	// AnanthAtSamraksh - Modifying existing pointer logic of data store
    	// The original piece of code no longer works
        lPtr = lDataStoreEndPtr - (lDataStoreStartPtr - lPtr - 1);
    }
    return lPtr;
}

/*
 * Calculate the bytes between 2 addresses specified.
 * Considers the cases where the destination address (toAddr)
 * 		could be smaller than the source address (fromAddr).
 */
// Inclusive of pointers passed - So, subtract 1 from the value if appropriate
int Data_Store::calculateNumBytes(LPVOID fromAddr, LPVOID toAddr)
{
    int retVal = 0;

    DATASTORE_ASSERT( (NULL != fromAddr && NULL != toAddr),
                       "Invalid parameters being passed!" );
    do{
        if( NULL == fromAddr ||
            NULL == toAddr ){
            retVal = 0;
            break;
        }

        if( fromAddr <= toAddr ){
            // There is no wrapping of pointers - Direct subtraction is enough
            retVal = (int)((char*)toAddr - (char*)fromAddr) + 1;
        }else{
            char *startAddr = (char*)blockDeviceInformation->Regions->Start + dataStoreStartByteOffset ;
        	char *endAddr   = (char*)blockDeviceInformation->Regions->Start + dataStoreEndByteOffset ;


            retVal = (int)((endAddr - (char*)fromAddr + 1) + ((char*)toAddr - startAddr + 1) );
        }
    }
    while(0);
    return retVal;
}


/*
 * Helper function to calculate log head-room (see comments for compactLog to understand what log head-room is). Also used to get amount of free space.
 */
uint32 Data_Store::calculateLogHeadRoom()
{
    uint32 retVal = 0;
    if(erasePointByteOffset > logPointByteOffset){
        // The free space doesn't overlap between end and beginning of flash
        retVal = erasePointByteOffset - logPointByteOffset + 1;
    }else{
        retVal = (dataStoreEndByteOffset - logPointByteOffset + 1) +
                    (erasePointByteOffset - dataStoreStartByteOffset + 1);
    }
	return retVal;
}

/*
 * Function that creates the allocation for a new record on the NOR.
 * 	-	This also creates an entry on the address table for the created record.
 * 	-	If sufficient space is not available for a record on a block, a dummy allocation is created
 * 			on the current block and the record is created in the adjacent block.
 */
LPVOID Data_Store::createAllocation( RECORD_ID recordID, LPVOID givenPtr, uint32 numBytes, uint32 dataType )
{
	UINT32 logPtr     = 0;
	int  currBlockID = 0;
	char *currBlockStartAddr = NULL;
	char *currBlockEndAddr   = NULL;
	int byteLeftInBlock = 0, byteLeftInBlockAfterAllocation = 0;
	logPtr = blockDeviceInformation->Regions->Start + logPointByteOffset;
	currBlockID = blockDeviceInformation->Regions->BlockIndexFromAddress(logPtr);
	currBlockStartAddr = (char*)blockDeviceInformation->Regions->BlockAddress(currBlockID);
	currBlockEndAddr   = currBlockStartAddr + blockDeviceInformation->Regions->BytesPerBlock - 1;
	byteLeftInBlock = (UINT32)currBlockEndAddr - logPtr + 1;

    LPVOID retVal = NULL;
    uint32 allocationSize = numBytes + sizeof(RECORD_HEADER);

    RECORD_HEADER header = { 0 };

    lastErrorVal = DATASTORE_ERROR_NONE;
    do{
    	/* Create a dummy/Skip allocation if required to make sure that this allocation
    	           doesn't fall in sectors -  return value tells if an allocation was done or not
    	           We don't care to know the status here */
    	if(byteLeftInBlock >= sizeof(RECORD_HEADER) && byteLeftInBlock < allocationSize)
    	{
    		createDummyAllocation(byteLeftInBlock);
    	}
    	else if(byteLeftInBlock > allocationSize)
    	{
        	byteLeftInBlockAfterAllocation = byteLeftInBlock - allocationSize;
    		if(byteLeftInBlockAfterAllocation < sizeof(RECORD_HEADER))
    		{
				// Increase size of current allocation by remaining space.
				allocationSize = allocationSize + byteLeftInBlockAfterAllocation;
    		}
    	}

        if(calculateLogHeadRoom() < (MIN_SPACE_REQUIRED_FOR_COMPACTION + allocationSize)){
            /* Free memory is below the minimum threshold, try creating some free
               space by moving active records, clearing inactive hence compacting flash */
            int prevHeadRoom = 0;
            int currentHeadRoom = calculateLogHeadRoom();

            while(prevHeadRoom != currentHeadRoom){
            /* Run till we are making some free space on each iteration */
                compactLog();                               /* Clear some memory */
                prevHeadRoom = currentHeadRoom;
                currentHeadRoom = calculateLogHeadRoom();   /* Check free space available now */
                if(currentHeadRoom >= (MIN_SPACE_REQUIRED_FOR_COMPACTION + allocationSize)){
                    /* Come out if we have enough space */
                    break;
                }
            }
        }
        if(calculateLogHeadRoom() < (MIN_SPACE_REQUIRED_FOR_COMPACTION + allocationSize)){
            /* If this is the case, its impossible to create more headRoom (is it?) */
            lastErrorVal = DATASTORE_ERROR_OUT_OF_FLASH_MEMORY;
            break;
        }

        if(allocationSize >= blockDeviceInformation->Regions->BytesPerBlock)
		{
			/* User cannot ask for space larger than size of a block */
			lastErrorVal = DATASTORE_ERROR_INVALID_PARAM;
			break;
		}

        if(addressTable.table.size() >= MAX_NUM_TABLE_ENTRIES){
			/* If this is the case, user wants to keep writing to flash. So erase previous entries in addressTable */
        	uint32 deleteIndex = 0;
        	DATASTORE_STATUS status;
        	while(addressTable.table[deleteIndex].givenPtr != 0)
			{
        		//Don't remove a record which has to be updated with a new location.
        		RECORD_ID recID = addressTable.table[deleteIndex].recordID;
        		if(recID == recordID) {
        			deleteIndex++;
        		}
        		else {
					status = addressTable.removeEntry(recID);
					if(status != DATASTORE_STATUS_OK)
					{
						break;
					}
        		}
			}
		}

        /* Now that we have created enough free space, continue with the allocation */
        retVal = incrementPointer((char*)blockDeviceInformation->Regions->Start, logPointByteOffset);
        incrementLogPointer(allocationSize);

        /* Now search for old allocation if any and mark them inactive - If this is the first record,
           then no harm done :)
           CHETHAN :: Should I pass a flag controlling whether this should be done or not, as in some
           cases its obviously unnecessary but still we would be doing it and lose some performance.
        */
        LPVOID oldRecAddr = addressTable.getCurrentLoc(recordID);         /* O(n) :( */
        if( NULL != oldRecAddr ){
            /* There is an old record already, mark it inactive */
        	oldRecAddr = incrementPointer( oldRecAddr, -1*sizeof(RECORD_HEADER));
            cyclicDataRead( &header,
                            oldRecAddr,
                            sizeof(RECORD_HEADER) );

            header.activeFlag = FLAG_RECORD_INACTIVE;

            cyclicDataWrite( &header,
                             oldRecAddr,
                             sizeof(RECORD_HEADER) );

            /* Now, update the header struct and write it back */
            header.activeFlag = FLAG_RECORD_ACTIVE;
            header.version++;                 /* Move to next value */
            header.zero       = 0;

            /* Writing at the end of the function */
            addressTable.updateCurrentLocation( recordID, incrementPointer(retVal, sizeof(RECORD_HEADER)));
        }else{
            /* This is the first time this record is being created - So init the header struct */
            DATASTORE_ADDR_TBL_ENTRY entry;

            //entry.allocationSize = numBytes;
            entry.allocationSize = allocationSize - sizeof(RECORD_HEADER);
            entry.currentLoc     = (char*)incrementPointer(retVal , sizeof(RECORD_HEADER));
            entry.givenPtr       = givenPtr;
            entry.recordID       = recordID;

            header.activeFlag = FLAG_RECORD_ACTIVE;

            if(0 == dataType)
				header.dataType = DATATYPE_BYTE;
			else if(1 == dataType)
				header.dataType = DATATYPE_INT16;
			else if(2 == dataType)
				header.dataType = DATATYPE_INT32;

            header.recordID   = recordID;
            //header.size       = numBytes;
            header.size       = allocationSize - sizeof(RECORD_HEADER);
            header.version    = 0;
            header.zero       = 0;

            addressTable.addEntry(&entry);
        }
        /* Now, write the header to flash  */
        cyclicDataWrite( &header,
                         retVal,
                         sizeof(RECORD_HEADER) );
    }while(0);
    return retVal;
}
//Data_Store::createAllocation


/*
 * Helper function for the write function to check if a write is happening on an existing record.
 */
bool Data_Store::detectOverWrite( void *addr, void *data, int dataLen, uint32 *conflictStartLoc )
{
    char *flashData = (char*)addr;
    char *givenData = (char*)data;
    bool retVal = false;

    *conflictStartLoc = dataLen;

    for(int index = 0; index < dataLen; ++index){
		if(flashData[index] != 0xFF)
		{
			retVal = true;
			*conflictStartLoc = index;
			break;
		}
    }

	return retVal;
}

/* Helper function to get count of objects in a block */
int Data_Store::objectCountInBlock(int blockID)
{
	RECORD_HEADER header;
	DATASTORE_ADDR_TBL_ENTRY entry;
	char *addr = (char *)blockDeviceInformation->Regions->BlockAddress(blockID);
	char *endAddr = addr + blockDeviceInformation->Regions->BytesPerBlock;
	int objectCount = 0;

	while(*addr != (char)0xFF && addr < (endAddr))
	{
		cyclicDataRead(&header, addr, sizeof(RECORD_HEADER));
		if(*((unsigned char*)&header) == SKIP_TO_NEXT_SECTOR_FLAG) {
			break;
		}
		if(FLAG_RECORD_ACTIVE == header.activeFlag ) {
			objectCount++;
		}
		addr = (char*)incrementPointer(addr, sizeof(RECORD_HEADER)+header.size);
		header = {0}; entry = {0};
	}
	return objectCount;
}

/*
 * Helper function to get record id from flash (if present)
 */
LPVOID Data_Store::searchForID(RECORD_ID id)
{
	LPVOID retValTemp = NULL;
	UINT32 dataStoreStartAddr = blockDeviceInformation->Regions->Start + dataStoreStartByteOffset;
	UINT32 dataStoreEndAddr = blockDeviceInformation->Regions->Start + dataStoreEndByteOffset;
	int firstBlock = blockDeviceInformation->Regions->BlockIndexFromAddress(dataStoreStartAddr);
	int lastBlock = blockDeviceInformation->Regions->BlockIndexFromAddress(dataStoreEndAddr);

	static int prevBlockSearched = firstBlock;
	static int recordOffset = 0;
	int objectCountInBlk = 0;
	int totalNumOfObjectsRead = 0, objectsReadFromBlock = 0;
	bool readDone = false;
	uint32 deleteIndex = 0;
	DATASTORE_STATUS status;

	retValTemp = addressTable.getCurrentLoc(id);
	if(retValTemp != NULL) {
		//Found id
		return retValTemp;
	}
	else {
		/* An optimization to speed-up search for ID. Since data is written in a log-like manner,
		 * it is assumed that data will also be read and accessed in log-like manner. Hence, the
		 * previous block that was searched is stored and the next search starts from that block. */

		//Go through entire flash looking for id.
		for(int index = prevBlockSearched;index <= lastBlock; index++) {
			//Erase existing entries in addressTable
			if(! addressTable.eraseAddressTable()) {
				return NULL;
			}

			objectCountInBlk = objectCountInBlock(index);
			objectsReadFromBlock = 0;
			//Reset recordOffset for new block, else retain value as next record could be in the same block.
			if(index != prevBlockSearched) {
				recordOffset = 0;
			}
			//Fetch new set of entries into addressTable and search again
			while(objectsReadFromBlock < objectCountInBlk) {
				objectsReadFromBlock += readRecordinBlock(index, MAX_NUM_TABLE_ENTRIES, recordOffset, &readDone);
				recordOffset = objectsReadFromBlock;
				totalNumOfObjectsRead += objectsReadFromBlock;
				//AnanthAtSamraksh -- make sure that there are no corrupt regions in flash
				if(lastErrorVal != DATASTORE_ERROR_NONE )
					return NULL;

				retValTemp = addressTable.getCurrentLoc(id);
				if(retValTemp != NULL) {
					//Found id
					prevBlockSearched = index;
					//Even if addressTable gets erased, to make next search faster,
					//adjust recordOffset so that it starts searching from previous recordID (worst case)
					if((recordOffset - MAX_NUM_TABLE_ENTRIES - 1) < 0) {
						recordOffset = 0;
					} else {
						recordOffset = (recordOffset - MAX_NUM_TABLE_ENTRIES - 1);
					}
					return retValTemp;
				}
				if(objectCountInBlk > MAX_NUM_TABLE_ENTRIES) {
					//Erase existing entries in addressTable
					if(! addressTable.eraseAddressTable()) {
						return NULL;
					}
				}
			}
		}

		for(int index = firstBlock;index <= prevBlockSearched; index++) {
			//Erase existing entries in addressTable
			if(! addressTable.eraseAddressTable()) {
				return NULL;
			}

			objectCountInBlk = objectCountInBlock(index);
			objectsReadFromBlock = 0;
			recordOffset = 0;
			//Fetch new set of entries into addressTable and search again
			while(objectsReadFromBlock < objectCountInBlk) {
				objectsReadFromBlock += readRecordinBlock(index, MAX_NUM_TABLE_ENTRIES, recordOffset, &readDone);
				recordOffset = objectsReadFromBlock;
				totalNumOfObjectsRead += objectsReadFromBlock;
				//AnanthAtSamraksh -- make sure that there are no corrupt regions in flash
				if(lastErrorVal != DATASTORE_ERROR_NONE )
					return NULL;

				retValTemp = addressTable.getCurrentLoc(id);
				if(retValTemp != NULL) {
					//Found id
					prevBlockSearched = index;
					return retValTemp;
				}
				if(objectCountInBlk > MAX_NUM_TABLE_ENTRIES) {
					//Erase existing entries in addressTable
					if(! addressTable.eraseAddressTable()) {
						return NULL;
					}
				}
			}
		}

	}
	return retValTemp;
} //searchForID

/*
 * Get base address for given Record_id
 */
LPVOID Data_Store::getAddress(RECORD_ID id)
{
    LPVOID retVal = NULL;
    LPVOID retValTemp = NULL;
    lastErrorVal = DATASTORE_ERROR_NONE;

	retValTemp = addressTable.getCurrentLoc(id);

    if(retValTemp == NULL) {
    	retValTemp = searchForID(id);
    }

    if(retValTemp == NULL) {
    	return retValTemp;
    }
    else {
		if( NULL == ( retVal = ( retValTemp - sizeof(RECORD_HEADER) ) ) ){
			lastErrorVal = DATASTORE_ERROR_INVALID_RECORD_ID;
		}
		return retVal;
    }
}

/*
 * Get data type for given Record_id
 */
uint32 Data_Store::getDataType(RECORD_ID id)
{
    LPVOID dataCurLoc = NULL;
    RECORD_HEADER recHeader = { 0 };

    lastErrorVal = DATASTORE_ERROR_NONE;

    if( NULL == (dataCurLoc = addressTable.getCurrentLoc(id)) ) {
    	lastErrorVal = DATASTORE_ERROR_INVALID_RECORD_ID;
    }
    cyclicDataRead( &recHeader, (char*)dataCurLoc - sizeof(RECORD_HEADER), sizeof(RECORD_HEADER) );

    return recHeader.dataType;
}


/*
 * Get data type for given Record_id
 */
uint32 Data_Store::getAllocationSize(RECORD_ID id)
{
	uint32 size = addressTable.getAllocationSize(id);
	return size;

	/*LPVOID dataCurLoc = NULL;
    RECORD_HEADER recHeader = { 0 };

    lastErrorVal = DATASTORE_ERROR_NONE;

    if( NULL == (dataCurLoc = addressTable.getCurrentLoc(id)) ) {
    	lastErrorVal = DATASTORE_ERROR_INVALID_RECORD_ID;
    }
    cyclicDataRead( &recHeader, (char*)dataCurLoc - sizeof(RECORD_HEADER), sizeof(RECORD_HEADER) );

    return recHeader.size;*/
}

/*
 * Get base address for given Record_id
 */
RECORD_ID Data_Store::getRecordID(LPVOID givenPtr)
{
    return addressTable.getRecordID(givenPtr);
}


/*
 * Traverses through addressTable and returns most recent recordID
 */
RECORD_ID Data_Store::getRecentRecordID()
{
	uint32 recIdIndex = 0;

	while(addressTable.table[recIdIndex].recordID != 0)
	{
		++recIdIndex;
	}
	if(recIdIndex == 0)
		return recIdIndex;
	else{
		recIdIndex--;
		return addressTable.table[recIdIndex].recordID;
	}
}


/*
 * Function that initializes the datastore. Following events happen during initialization:
 * 	-	if "eraseDataStore" is true, then datastore is erased
 * 	-	invokes "scanFlashDevice()" that rebuilds the address table by scanning through records in the NOR flash.
 */
DATASTORE_STATUS Data_Store::initDataStore( char *datastoreName, DATASTORE_PROPERTIES *property, bool eraseDataStore )
{
    DATASTORE_STATUS status = DATASTORE_STATUS_INVALID_PARAM;

    do{
        if( NULL == datastoreName ||
            NULL == property ){
            lastErrorVal = DATASTORE_ERROR_INVALID_PARAM;
            break;
        }

        ////DATASTORE_ASSERT( flashDevice.getDeviceState()== EMULATOR_STATE_READY, "Created flash device not in ready state!" );
        ////// AnanthAtSamraksh - TODO - Add flash status to datastore.h and change initialize to add the status
        ////// AnanthAtSamraksh - TODO - Add initialization for the 3 block storage variables so they are pointing to what they need to be pointing to

        /* Initialize the internal variables */
        int lDataStoreStartBlockID = 0;      /* Will calculate BlockID of first block of datastore */
        LPVOID lDataStoreStartPtr = NULL;
        /* This represents the block where the given offset starts */
        lDataStoreStartBlockID = blockDeviceInformation->Regions->BlockIndexFromAddress( blockDeviceInformation->Regions->Start + DATA_STORE_OFFSET );
        /* Datastore should start at the beginning of next block */
        lDataStoreStartBlockID = lDataStoreStartBlockID + 1;

        /* Now We have the address of the first block of datastore - Calculate byte offset from the
           beginning of the flash */
        lDataStoreStartPtr = (LPVOID)blockDeviceInformation->Regions->BlockAddress(lDataStoreStartBlockID);

        dataStoreDeviceSize = blockDeviceInformation->Regions->NumBlocks * blockDeviceInformation->Regions->BytesPerBlock;
        dataStoreStartByteOffset = (uint32)((char*)lDataStoreStartPtr - (char*)blockDeviceInformation->Regions->Start);
        dataStoreEndByteOffset   = dataStoreDeviceSize - 1;

        //AnanthAtSamraksh -- erase flash if flag is set by user
        if(eraseDataStore)
        	EraseAllBlocks();

        /* Now, that the datastore is ready, we need to register it with the global
           registration table which is used for address translations later */
#if 0
        DATASTORE_REG_ENTRY regEntry;
        regEntry.addrRangeStart  = (LPVOID)property->addressRangeStart;
        regEntry.addrRangeEnd    = (LPVOID)property->addressRangeEnd;
        regEntry.dataStoreHandle = (LPVOID)this;

        status = DATASTORE_REG_CreateEntry(&regEntry);
        DATASTORE_ASSERT( DATASTORE_STATUS_OK == status,
                          "Registration Failed!" );
        if(DATASTORE_STATUS_OK != status){
            lastErrorVal = DATASTORE_ERROR_UNEXPECTED_ERROR;
            status       = DATASTORE_STATUS_INT_ERROR;
            break;
        }
#endif

        /* Now, scan the device for previously stored records and this should update
           my logPtr, erasePtr, cleanPtr */
        scanFlashDevice();
        if(lastErrorVal == DATASTORE_ERROR_INVALID_PARAM || lastErrorVal == DATASTORE_ERROR_OUT_OF_BOUND_ACCESS)
        {
        	status = DATASTORE_STATUS_NOT_OK;
        	state  = DATASTORE_STATE_UNINIT;
        	break;
        }

        lastErrorVal = DATASTORE_ERROR_NONE;
        status       = DATASTORE_STATUS_OK;
        state        = DATASTORE_STATE_READY;
    }while(0);
    return status;
}

/*
 * Helper function for the "createAllocation" and "compactLog" function to create a dummy allocation,
 * when there is insufficient space to fit in a new record or while moving records to new blocks.
 */
bool Data_Store::createDummyAllocation(int nextAllocationSize)
{
    //char *logPtr     = NULL;
    UINT32 logPtr     = 0;
    int  currBlockID = 0;
    char *currBlockStartAddr = NULL;
    char *currBlockEndAddr   = NULL;
    int byteLeftInBlock = 0;

    bool retVal = false;

    /* Add the record header as this is the actual size of the allocation */
    nextAllocationSize = nextAllocationSize + sizeof(RECORD_HEADER);

    logPtr = blockDeviceInformation->Regions->Start + logPointByteOffset;

    currBlockID = blockDeviceInformation->Regions->BlockIndexFromAddress(logPtr);
    currBlockStartAddr = (char*)blockDeviceInformation->Regions->BlockAddress(currBlockID);
    currBlockEndAddr   = currBlockStartAddr + blockDeviceInformation->Regions->BytesPerBlock - 1;

    byteLeftInBlock = (UINT32)currBlockEndAddr - logPtr + 1;
    if(byteLeftInBlock == 0){
        /* I am sure this is impossible - Chethan */
        retVal = false;
    }
    else if(byteLeftInBlock < nextAllocationSize){
        /* This particular sector has less than required number of bytes, hence we
           need to fill this by a filler allocation using SKIP_TO_NEXT_SECTOR_FLAG */
		unsigned char lflag = SKIP_TO_NEXT_SECTOR_FLAG;

		UINT32 address = (UINT32) logPtr;
		//AnanthAtSamraksh - numBytes should be 2, because the NOR driver divides this by 2 to get the halfWords. If it is 1, NOR get 0 half words, which is a bug.
		int numBytes = 2;
		void *rawDataIn = &lflag;
		if(!blockStorageDevice->Write(address, numBytes, (BYTE *) rawDataIn, FALSE))
		{
			debug_printf("Failed to write raw data to Memory\n");
		}

        incrementLogPointer(byteLeftInBlock);   /* Now move to the next sector */
        retVal = true;
    }else{
        /* Nothing to be done I guess */
        retVal = false;
    }
    return retVal;
}

/*
 * Helper function that safely reads data into the buffer.
 */
uint32 Data_Store::cyclicDataRead( LPVOID buff,
                                      LPVOID currentLoc,
                                      uint32 numBytes )
{
    uint32 retVal = numBytes;
    uint32 offset = 0;

    if( NULL == currentLoc ||
        NULL == buff ||
        (char*)currentLoc < (char*)blockDeviceInformation->Regions->Start ||
        numBytes > dataStoreDeviceSize ){

        DATASTORE_ASSERT( false, "Error!!!");
        ASSERT(false);  //This should never happen!
        //return 0;
    }

    offset = (uint32)((char*)currentLoc - (char*)blockDeviceInformation->Regions->Start);
    do{
        DATASTORE_ASSERT( ((offset >= dataStoreStartByteOffset) && (offset <= dataStoreEndByteOffset)),
                           "Trying to read out of bound region!"  );

        if( offset < dataStoreStartByteOffset ||
            offset > dataStoreEndByteOffset ){
            /* Trying to read out of bound region */
            retVal       = 0;
            lastErrorVal = DATASTORE_ERROR_OUT_OF_BOUND_ACCESS;
            break;
        }

        if(offset + numBytes <= dataStoreEndByteOffset){

            /* Common case - Just read */

            memcpy( buff,
                    currentLoc,
                    numBytes );

        }else{
            /* Need to read in pieces */
            int byteTillEnd = dataStoreEndByteOffset - offset + 1;


            /* Copy first piece */
            memcpy( buff,
                    currentLoc,
                    byteTillEnd );

            /* Copy second piece */
            memcpy( (char*)buff+byteTillEnd,
                    (char*)blockDeviceInformation->Regions->Start + dataStoreStartByteOffset,
                    numBytes - byteTillEnd );
        }
    }while(0);

    return retVal;
}

/*
 * Helper function that safely writes data from the buffer into the address passed as a parameter.
 */
uint32 Data_Store::cyclicDataWrite( LPVOID buff,
                                       LPVOID currentLoc,
                                       uint32 numBytes )
{
    uint32 retVal = numBytes;
    uint32 offset = 0;

    if(numBytes == 0)
    	return 0;

    if( NULL == currentLoc ||
        NULL == buff ||
        (char*)currentLoc < (char*)blockDeviceInformation->Regions->Start ||
        numBytes > dataStoreDeviceSize ){

        DATASTORE_ASSERT( false, "Error!!!");
        ASSERT(false);  //This should never happen!
        //return 0;
    }
    offset = (uint32)((char*)currentLoc - (char*)blockDeviceInformation->Regions->Start);
    do{
        DATASTORE_ASSERT( ((offset >= dataStoreStartByteOffset) && (offset <= dataStoreEndByteOffset)),
                           "Trying to Write out of bound region!"  );

        if( offset < dataStoreStartByteOffset ||
            offset > dataStoreEndByteOffset ){
            /* Trying to read out of bound region */
            retVal       = 0;
            lastErrorVal = DATASTORE_ERROR_OUT_OF_BOUND_ACCESS;
            break;
        }

        if(offset + numBytes <= dataStoreEndByteOffset){
            /* Common case - Just Write */
        	UINT32 address = (UINT32) currentLoc;
			void *rawDataIn = buff;
			if(!blockStorageDevice->Write(address, numBytes, (BYTE *) rawDataIn, FALSE))
			{
				//PRINT_DEBUG("Failed to write raw data to Memory");
				lastErrorVal =  DATASTORE_ERROR_WRITE_TO_FLASH_MEMORY_FAILED;
				break;
			}
        }else{
            /* Need to read in pieces */
            int byteTillEnd = dataStoreEndByteOffset - offset + 1;

            /* Copy first piece */
            UINT32 address = (UINT32) currentLoc;
			void *rawDataIn = buff;
			if(!blockStorageDevice->Write(address, byteTillEnd, (BYTE *) rawDataIn, FALSE))
			{
				//PRINT_DEBUG("Failed to write raw data to Memory");
				lastErrorVal =  DATASTORE_ERROR_WRITE_TO_FLASH_MEMORY_FAILED;
				break;
			}
            /* Copy second piece */
			address = blockDeviceInformation->Regions->Start + dataStoreStartByteOffset;
			rawDataIn = (char*)buff + byteTillEnd;
			if(!blockStorageDevice->Write(address, (numBytes - byteTillEnd), (BYTE *) rawDataIn, FALSE))
			{
				//PRINT_DEBUG("Failed to write raw data to Memory");
				lastErrorVal =  DATASTORE_ERROR_WRITE_TO_FLASH_MEMORY_FAILED;
				break;
			}
        }
    }while(0);
    return retVal;
}


DATASTORE_STATUS Data_Store::storeClearPtrOffset()
{
    DATASTORE_STATUS status = DATASTORE_STATUS_NOT_OK;
    do{

    }while(0);
    return status;
}

/*
 * The garbage collector/compaction function. Whenever space runs out for a new record in the NOR flash, this function
 * is invoked to create space. Space is created by moving active records to a new block and by erasing
 * inactive records. All records between the clean and the erase pointers can be erased.
 * Some background info on garbage collection (ref.: http://www.google.com/patents/US6535949; http://rave.ohiolink.edu/etdc/view?acc_num=osu1365811711)
 * Log point   - Points to the current location in flash where new records can be written to
 * Erase point - Points to the location in flash that was last erased. Blocks are never partially erased. They are always fully erased.
 * Clean point - When there is shortage of space, the GC moves active records from the portion of flash starting from the location pointed to by this pointer.
 * 				 In other words, data could be written to flash until the location pointed to by clean point, provided that block is erased.
 * Log headroom - Space between the log and erase point is called the log headroom and is the space available for writing new data.
 */
DATASTORE_STATUS Data_Store::compactLog()
{
	//debug_printf("Data_Store::compactLog\n");
    uint32 initialClearPtrBlockID;
    uint32 currClearPtrBlockID;
    DATASTORE_STATUS status = DATASTORE_STATUS_NOT_OK;
    RECORD_HEADER recHeader = { 0 };
    char *clearPtr = NULL;

    clearPtr = (char*)blockDeviceInformation->Regions->Start + clearLogPointByOffset;

    initialClearPtrBlockID = currClearPtrBlockID = blockDeviceInformation->Regions->BlockIndexFromAddress( (UINT32)clearPtr );
    while(initialClearPtrBlockID == currClearPtrBlockID){
        /* Compact one block */
        cyclicDataRead( &recHeader, clearPtr, sizeof(RECORD_HEADER) );
        if(*((unsigned char*)&recHeader) == SKIP_TO_NEXT_SECTOR_FLAG){
                   /* Checking for the flag indicating to skip to the next sector */
                   int  currentBlockID = blockDeviceInformation->Regions->BlockIndexFromAddress((UINT32)clearPtr);
                   char *sectorEndByte = (char*)blockDeviceInformation->Regions->BlockAddress(currentBlockID) +
                		   	   	   	   	   	   	   	   	   	   	   	   blockDeviceInformation->Regions->BytesPerBlock - 1;
                   int numBytesToIncrement = sectorEndByte - clearPtr + 1;

                   incrementClearPoint(numBytesToIncrement);
                   clearPtr = (char*)blockDeviceInformation->Regions->Start + clearLogPointByOffset;
        }
        else if( 0 != recHeader.zero ){
			ASSERT(false);  /* This should never happen */
            break;  //When assert is removed
        }
        else if( 0 == recHeader.zero &&
            FLAG_RECORD_ACTIVE == recHeader.activeFlag ){
            /* Record that we are seeing is active, so we need to move it to the
               end of log - Can't use write method directly because current call
               could be coming from there :) */

        	  /* Create a dummy/skip allocation if required */
        	createDummyAllocation(recHeader.size);


        	//debug_printf("Data_Store::compactLog. Migrating record\n");
            /* First mark the current location as inactive */
            recHeader.activeFlag = FLAG_RECORD_INACTIVE;
            cyclicDataWrite( (LPVOID)&recHeader, clearPtr, sizeof(RECORD_HEADER) );

            /* Old record is marked inactive, now migrate the record to the end of log */
            recHeader.activeFlag = FLAG_RECORD_ACTIVE;

            cyclicDataWrite( &recHeader,
                             ((char*)blockDeviceInformation->Regions->Start) + logPointByteOffset,
                             sizeof(RECORD_HEADER));
            incrementLogPointer(sizeof(RECORD_HEADER));
            incrementClearPoint(sizeof(RECORD_HEADER));
            clearPtr = (char*)blockDeviceInformation->Regions->Start + clearLogPointByOffset;
            /* Now, clearPtr is pointing to the data_part of the record binging migrated */

            /* Copy data of the record being migrated */
			LPVOID lCurrLoc = ((char*)blockDeviceInformation->Regions->Start) + logPointByteOffset;
            cyclicDataWrite( clearPtr, lCurrLoc, recHeader.size );

            incrementLogPointer(recHeader.size);
            incrementClearPoint(recHeader.size);

            clearPtr = (char*)blockDeviceInformation->Regions->Start + clearLogPointByOffset;
			addressTable.updateCurrentLocation( recHeader.recordID, lCurrLoc );
        }else{
            /* Inactive record, just move the clear pointer to the next record */
        	//debug_printf("Data_Store::compactLog. Moving clearPointer\n");
            incrementClearPoint(recHeader.size + sizeof(RECORD_HEADER));
            clearPtr = (char*)blockDeviceInformation->Regions->Start + clearLogPointByOffset;
        }
        /* Now, check the blockID where my clearPointer is now */
        currClearPtrBlockID = blockDeviceInformation->Regions->BlockIndexFromAddress((UINT32)clearPtr);

        status = DATASTORE_STATUS_OK;   //In a loop :)
    }

    /* Now check if there is a complete block of inactive records that can be cleared */
    /*uint32 logPtrBlockID   = blockDeviceInformation->Regions->BlockIndexFromAddress((UINT32)clearPtr);
    uint32 clearPtrBlockID = blockDeviceInformation->Regions->BlockIndexFromAddress((UINT32)clearPtr);*/
    uint32 logPtrBlockID   = blockDeviceInformation->Regions->BlockIndexFromAddress(blockDeviceInformation->Regions->Start + logPointByteOffset);
    uint32 clearPtrBlockID = blockDeviceInformation->Regions->BlockIndexFromAddress(blockDeviceInformation->Regions->Start + clearLogPointByOffset);

    if(datastore_abs((int)clearPtrBlockID - (int)logPtrBlockID ) >= 1){
        /* There is a gap of atleast 1 block and initialClearPtrBlockID could be safely
           erased */
    	   /* Before erase, update the hidden sector with the new clearPtrOffset, which can be used while
    	           scanning the device on the reboot */

        ByteAddress eraseBlockAddress = blockDeviceInformation->Regions->BlockAddress(initialClearPtrBlockID);
        blockStorageDevice->EraseBlock(eraseBlockAddress);
        incrementErasePoint(blockDeviceInformation->Regions->BytesPerBlock);
    }

    return status;
}
//Data_Store::compactLog



/*
 * Helper function that safely traverses through the NOR flash.
 */
LPVOID Data_Store::traversePointer(PERSISTENCE_DIRECTION per_dir, char* address)
{
	int recordID;
	int blockID;
	LPVOID givenPtr;
	RECORD_HEADER header = { 0 };
	UINT32 dataStoreStartAddr = blockDeviceInformation->Regions->Start + dataStoreStartByteOffset;
	UINT32 dataStoreEndAddr = blockDeviceInformation->Regions->Start + dataStoreEndByteOffset;

	int lfirstBlockID = blockDeviceInformation->Regions->BlockIndexFromAddress(dataStoreStartAddr);
	int llastBlockID = blockDeviceInformation->Regions->BlockIndexFromAddress(dataStoreEndAddr);

	/* Traverse left of the address passed as parameter */
	if(per_dir == GO_LEFT){
		while(*address == (char)0xFF){
			blockID = blockDeviceInformation->Regions->BlockIndexFromAddress((UINT32)address);
			blockID--;
			if(blockID < lfirstBlockID){
				blockID = llastBlockID;
			}
			//address = (char*)flashDevice.getBlockAddressFromBlockID(blockID);
			address = (char*)blockDeviceInformation->Regions->BlockAddress(blockID);
		}

		/* Jump record at a time */
		cyclicDataRead(&header, address, sizeof(RECORD_HEADER));
		while(*((char*)&header) != (char)0xFF){
			if( SKIP_TO_NEXT_SECTOR_FLAG == *((unsigned char*)&header)){
				blockID = blockDeviceInformation->Regions->BlockIndexFromAddress((UINT32)address);
				blockID++;
				blockID = (blockID >llastBlockID)?(lfirstBlockID):(blockID);

				address = (char*)blockDeviceInformation->Regions->BlockAddress(blockID);
			}else{
				address = (char*)incrementPointer( address,
											header.size+sizeof(RECORD_HEADER));
			}
			cyclicDataRead(&header, address, sizeof(RECORD_HEADER));
		}
	}

	/* Traverse right of the address passed as parameter */
	else if(per_dir == GO_RIGHT){
		while(*address == (char)0xFF){
			blockID = blockDeviceInformation->Regions->BlockIndexFromAddress((UINT32)address);
			blockID++;
			if(blockID > llastBlockID){
				blockID = lfirstBlockID;
			}
			address = (char*)blockDeviceInformation->Regions->BlockAddress(blockID);
		}
		address = (char*)incrementPointer(address, -1);
	}
	return address;
}

/*
 * If the device failed during use, this function is invoked to read the records from the NOR flash.
 */
LPVOID Data_Store::readPointers()
{
	char* returnAddress;
	char* tempAddress1;
	char* tempAddress2;
	int numBytes = 0;
	int tempBlockID = 0;
	PERSISTENCE_DIRECTION per_dir;
	RECORD_HEADER header;

	UINT32 dataStoreStartAddr = blockDeviceInformation->Regions->Start + dataStoreStartByteOffset;
	UINT32 dataStoreEndAddr = blockDeviceInformation->Regions->Start + dataStoreEndByteOffset;
	int firstBlock = blockDeviceInformation->Regions->BlockIndexFromAddress(dataStoreStartAddr);
	int lastBlock  = blockDeviceInformation->Regions->BlockIndexFromAddress(dataStoreEndAddr);

	/* Traverse through the entire data store and search for the FF regions */
	for(int index = firstBlock;index <= lastBlock; index++){
		returnAddress = (char *)blockDeviceInformation->Regions->BlockAddress(index);
		cyclicDataRead(&header, returnAddress, sizeof(RECORD_HEADER));

		if(*returnAddress == (char)0xFF){
			per_dir = GO_LEFT;
			tempAddress1 = (char *)traversePointer(per_dir, returnAddress);
			per_dir = GO_RIGHT;
			tempAddress2 = (char *)traversePointer(per_dir, returnAddress);
			break;
		}
	}

	logPointByteOffset = (int)(char*)(tempAddress1 - (char*)blockDeviceInformation->Regions->Start);
	erasePointByteOffset = (int)(char*)(tempAddress2 - (char*)blockDeviceInformation->Regions->Start);
	clearLogPointByOffset = (uint32)((char*)incrementPointer(tempAddress2,1) - (char*)blockDeviceInformation->Regions->Start);

	return NULL;
}

/*
 * Keep going through the flash until the FF regions are reached. Store all records encountered into the address table.
 */
int Data_Store::readRecordinBlock(int blockID, int arrayLength, int startOffset, bool *readDone)
{
	char* addr = (char *)blockDeviceInformation->Regions->BlockAddress(blockID);
	char *endAddr = addr + blockDeviceInformation->Regions->BytesPerBlock;

	DATASTORE_ADDR_TBL_ENTRY entry;
	DATASTORE_STATUS status = DATASTORE_STATUS_OK;
	lastErrorVal = DATASTORE_ERROR_NONE;
	int count = 0;
	RECORD_HEADER header;
	while(*addr != (char)0xFF && addr < (endAddr))
	{
		cyclicDataRead(&header, addr, sizeof(RECORD_HEADER));
		if(*((unsigned char*)&header) == SKIP_TO_NEXT_SECTOR_FLAG){
			break;
		}

		if(FLAG_RECORD_ACTIVE == header.activeFlag )
		{
			entry.allocationSize = header.size;
			entry.currentLoc     = addr+sizeof(RECORD_HEADER);
			entry.givenPtr       = myUniquePtrGen.getUniquePtr(header.size);
			entry.recordID       = header.recordID;

			/* Start adding entries from startOffset and keep adding until length is reached */

			if(startCount >= startOffset)
			{
				if(endCount < arrayLength)
				{
					status = addressTable.addEntry(&entry);
					endCount++;
					count++;	//Actual entries added to addressTable
				}
				else if(endCount >= arrayLength)
				{
					/* AnanthAtSamraksh - ensure that values are reset before leaving. */
					startCount = 0;
					endCount = 0;
					*readDone = true;
					break;
				}
			}
			startCount++;	//To keep track of startOffset and length

			/* Change the condition to check for the STATUS duplicate recordID */
			if(DATASTORE_STATUS_RECORD_ALREADY_EXISTS == status){
				int oldEntryVer;
				int curEntryVer;
				LPVOID temp;
				RECORD_HEADER tempHeader;

				//Mukundan: Removed Flash_memory
				temp = addressTable.getCurrentLoc(header.recordID);
				memcpy( &tempHeader,
						(char*)temp-sizeof(RECORD_HEADER),
						sizeof(RECORD_HEADER) );

				oldEntryVer = tempHeader.version;
				curEntryVer = header.version;
				/* Check which header is recent */
				if(decideWhichOneisRecent(tempHeader, header) == curEntryVer){       /*If loop 3*/
					deleteRecord(header.recordID);
					addressTable.addEntry(&entry);
				}else{
					temp = entry.currentLoc;
					tempHeader.activeFlag = FLAG_RECORD_INACTIVE;
					cyclicDataWrite( &tempHeader,(char*)temp-sizeof(RECORD_HEADER),
												sizeof(RECORD_HEADER));
				}       /*End of if loop 3*/
			}else if(status == DATASTORE_STATUS_OK){
				//count++;
			}
			else
			{
				DATASTORE_ASSERT( false, "Address Table failed!");
			}
		}

		if(addressTable.table.size() >= MAX_NUM_TABLE_ENTRIES){
			/* If this is the case, remove first entry from addressTable */
			/*DATASTORE_STATUS status;
			status = addressTable.removeEntry(addressTable.table[0].recordID);
			if(status != DATASTORE_STATUS_OK)
				break;*/
			//lastErrorVal = DATASTORE_ERROR_INVALID_PARAM;

			/* AnanthAtSamraksh - ensure that values are reset before leaving. */
			startCount = 0;
			endCount = 0;
			*readDone = true;
			break;
		}

		addr = (char*)incrementPointer(addr, sizeof(RECORD_HEADER)+header.size);
		/* AnanthAtSamraksh -- check if addr has gone beyond legitimate dataStore end address.
		 * If yes, then set an error, so that use may take appropriate action such as erasing the dataStore while initializing.
		 * */
		if(addr >= (char*)DATASTORE_END_ADDRESS || addr < (char*)DATASTORE_START_ADDRESS)
		{
			lastErrorVal = DATASTORE_ERROR_OUT_OF_BOUND_ACCESS;
			break;
		}
	}
	return count;
}

/*
 * Helper function used while scanning the flash to find out which among 2 headers is the most recent one.
 */
uint32 Data_Store::decideWhichOneisRecent(RECORD_HEADER tempHeader, RECORD_HEADER header)
{
    return((tempHeader.version+1)>header.version?tempHeader.version:header.version);
}

/*
 * Function that recreates the address table during initialization of NOR flash
 */
LPVOID Data_Store::scanFlashDevice()
{
	UINT32 dataStoreStartAddr = blockDeviceInformation->Regions->Start + dataStoreStartByteOffset;
	UINT32 dataStoreEndAddr = blockDeviceInformation->Regions->Start + dataStoreEndByteOffset;

	int firstBlock = blockDeviceInformation->Regions->BlockIndexFromAddress(dataStoreStartAddr);
	int lastBlock  = blockDeviceInformation->Regions->BlockIndexFromAddress(dataStoreEndAddr);
	int numOfEntries = 0;
	char* returnAddress;
	bool readDone = false;

	for(int index = firstBlock;index <= lastBlock; index++){
		numOfEntries += readRecordinBlock(index, MAX_NUM_TABLE_ENTRIES, 0, &readDone);
		//AnanthAtSamraksh -- make sure that there are no corrupt regions in flash
		if(lastErrorVal != DATASTORE_ERROR_NONE )
			return NULL;
		if(addressTable.table.size() == MAX_NUM_TABLE_ENTRIES)
			break;
	}

	/* The device is new */
	if(numOfEntries == 0){
		logPointByteOffset = clearLogPointByOffset = dataStoreStartByteOffset;
		erasePointByteOffset = dataStoreEndByteOffset;
	}
	/* If the device has already some data and crashed during use, call the below function */
	else{
		readPointers();
	}
	return NULL;
}


/*
 * Populates array passed as parameter with list of dataIDs
 */
void Data_Store::getRecordIDAfterPersistence(uint32* recordID_array, ushort arrayLength, ushort dataIdOffset)
{
	/* Since addressTable can hold only MAX_NUM_TABLE_ENTRIES, to make room for a new set of recordIDs requested by user,
	 * copy entries from offset into start of addressTable. Make entries upwards of offset as 0. */
	lastErrorVal = DATASTORE_ERROR_NONE;
	uint32 copyIndex = 0;
	int numOfEntries = 0;
	ushort arrayLengthOrig = 0;
	bool readDone = false;

	arrayLength = (arrayLength > MAX_NUM_TABLE_ENTRIES ? MAX_NUM_TABLE_ENTRIES : arrayLength);
	arrayLengthOrig = arrayLength;

	/* Clear addressTable and rebuild from beginning */
	UINT32 dataStoreStartAddr = blockDeviceInformation->Regions->Start + dataStoreStartByteOffset;
	UINT32 dataStoreEndAddr = blockDeviceInformation->Regions->Start + dataStoreEndByteOffset;

	int firstBlock = blockDeviceInformation->Regions->BlockIndexFromAddress(dataStoreStartAddr);
	int lastBlock = blockDeviceInformation->Regions->BlockIndexFromAddress(dataStoreEndAddr);
	int blockID = firstBlock;

	if(! addressTable.eraseAddressTable()) {
		//Should never happen
		lastErrorVal = DATASTORE_ERROR_UNEXPECTED_ERROR;
	}

	/* AnanthAtSamraksh - values before entering loop should be same as the ones while leaving */
	startCount = 0;	endCount = 0; readDone = false;

	/* Keep reading until count of records in addressTable is equal to offset */
	while(readDone == false)
	{
		if(blockID > lastBlock) {
			break;
		}
		int objectCount = objectCountInBlock(blockID);
		if(dataIdOffset < objectCount) {
			numOfEntries = readRecordinBlock(blockID, arrayLength, dataIdOffset, &readDone);
		}
		else {
			dataIdOffset -= objectCount;
		}
		//AnanthAtSamraksh -- make sure that there are no corrupt regions in flash
		if(lastErrorVal != DATASTORE_ERROR_NONE )
		{
			readDone = true; startCount = 0; endCount = 0;
			break;
		}
		if(numOfEntries == 0 && lastBlock == blockID)
		{
			readDone = true;
			/* AnanthAtSamraksh - To ensure that these values are reset when this loop is exited.
			 * Otherwise, repeated calls will retain old values leading to bugs. */
			startCount = 0; endCount = 0;
		}
		++blockID;
	}

	/* Copy existing entries (recordID) from addressTable (from offset) into array passed by user. */
	if(lastErrorVal == DATASTORE_ERROR_NONE)
	{
		for(copyIndex = 0; copyIndex < arrayLengthOrig; copyIndex++)
		{
			recordID_array[copyIndex] = addressTable.table[copyIndex].recordID;
		}
	}
}

/*
 * Returns total count of dataIDs
 */
uint32 Data_Store::getCountOfRecordIds()
{
	uint32 recIdIndex = 0;
	return addressTable.table.size();
}


/*
 * Delete a given record from the NOR flash. This is done by marking the record as inactive, and removing its entry from address table.
 */
DATASTORE_STATUS Data_Store::deleteRecord(RECORD_ID id)
{
    RECORD_HEADER header;
    DATASTORE_STATUS status = DATASTORE_STATUS_NOT_OK;
    lastErrorVal = DATASTORE_ERROR_NONE;
    do{
        LPVOID currLoc = addressTable.getCurrentLoc(id);
        if(currLoc == NULL) {
        	currLoc = searchForID(id);
        	if(currLoc == NULL) {
				lastErrorVal = DATASTORE_ERROR_INVALID_RECORD_ID;
				break;
			}
        }

        cyclicDataRead( (void*)&header,
                        (char*)currLoc - sizeof(RECORD_HEADER),
                        sizeof(RECORD_HEADER) );
        /* We need to mark this as inactive so that the garbage collector can clear
           this */
        header.activeFlag = FLAG_RECORD_INACTIVE;

        cyclicDataWrite( (LPVOID)&header,
                         (char*)currLoc - sizeof(RECORD_HEADER),
                         sizeof(RECORD_HEADER) );
        if(getLastError() != DATASTORE_ERROR_NONE)
        	break;

        addressTable.removeEntry(id);
        status = DATASTORE_STATUS_OK;
    }while(0);
    return status;
}

/*
 * Create a new record. This is done by adding an entry to the address table and creating an allocation
 * on the flash.
 */
LPVOID Data_Store::createRecord( RECORD_ID recordID, uint32 numBytes, uint32 dataType )
{
    RECORD_HEADER header;
    LPVOID lGivenPtr = NULL;
    LPVOID allocLoc  = NULL;
    lastErrorVal = DATASTORE_ERROR_NONE;
    //if(recordID >= 15600) {
    	//hal_printf("%d\n", recordID);
    //}

    // The STM Driver accepts half words as inputs so all writes are in half word sizes
    numBytes = numBytes + numBytes % 2;

    do{
        if(NULL != addressTable.getGivenAddress(recordID)){
            /* This record id is already present in the flash. So, can't proceed */
            lastErrorVal = DATASTORE_ERROR_RECORD_ID_ALREADY_EXISTS;
            break;
        }

        /* Generate an unique pointer and register it with the address_table */
        lGivenPtr = myUniquePtrGen.getUniquePtr(numBytes);
        allocLoc  = createAllocation( recordID, lGivenPtr, numBytes, dataType );
        if( NULL == allocLoc ){
            break;
        }
        /* Address table will be updated by createAllocation, so need not to bother here */
    }while(0);

    return allocLoc;
}

/*
 * Function that reads the data read from flash into given address.
 */
uint32 Data_Store::readRawData(LPVOID src, void *data, uint32 offset, uint32 numBytes)
{
	uint32 lNumBytes = 0;
    LPVOID curLoc = NULL;

    DATASTORE_ADDR_TBL_ENTRY tblEntry;
    LPVOID readAddress = src + sizeof(RECORD_HEADER);

    // AnanthAtSamraksh. The STM Driver accepts half words as inputs so all writes are in half word sizes
    numBytes = numBytes + numBytes % 2;

    LPVOID lDataStoreStartByteAddr = (char*)blockDeviceInformation->Regions->Start + dataStoreStartByteOffset;
    LPVOID lDataStoreEndByteAddr   = (char*)blockDeviceInformation->Regions->Start + dataStoreEndByteOffset;

    lastErrorVal = DATASTORE_ERROR_NONE;
    do{
        if(NULL == src || NULL == data){
            lastErrorVal = DATASTORE_ERROR_INVALID_PARAM;
            lNumBytes = numBytes = 0;
            break;
        }
        //curLoc updation is important when src address and givenAddr are different. If src address is
        //greater than given address, the numBytes is modified accordingly. But if curLoc is not updated, numBytes are read from starting address and not from src
        if( NULL == (curLoc = addressTable.getCurrentLoc(readAddress, lDataStoreStartByteAddr, lDataStoreEndByteAddr, tblEntry))){
            lastErrorVal = DATASTORE_ERROR_INVALID_GIVEN_ADDR;
            lNumBytes = numBytes = 0;
            break;
        }

        //lNumBytes = addressTable.getMaxWriteSize(readAddress);
        UINT32 temp = (UINT32*)readAddress - (UINT32*)tblEntry.currentLoc;
        lNumBytes = tblEntry.allocationSize - temp;

        if(lNumBytes < numBytes)
        {
        	hal_printf("Lets stop here for now\n\r");
        	LPVOID tempCurLoc = addressTable.table[0].currentLoc;
        	lNumBytes = addressTable.getMaxWriteSize(readAddress);
        }
        lNumBytes = (numBytes < lNumBytes)?numBytes:lNumBytes;

        memset(data, 0, lNumBytes);
        cyclicDataRead( data, (char*)curLoc + offset, lNumBytes);

    }while(0);
    return lNumBytes;
}

uint32 Data_Store::readData(LPVOID src, uint32* data, uint32 offset, uint32 count)
{
    return readRawData( src, data, offset*sizeof(int), count*sizeof(int));
}

uint32 Data_Store::writeData( LPVOID dest, uint32* data, uint32 offset, uint32 count )
{
    return writeRawData( dest, data, offset*sizeof(int), count*sizeof(int));
}

/*
 * Function that writes data stored in buffer into the mentioned flash address
 */
uint32 Data_Store::writeRawData(LPVOID dest, void* dataToBeWritten, uint32 offset, uint32 numBytes)
{
	LPVOID fromRecBaseAddr       = NULL;    /* Base address of the current allocation */
    LPVOID recWriteStartAddr = NULL;    /* Address that correspond to the address where give data needs to be written */
    LPVOID recWriteEndAddr   = NULL;    /* Address of corresponding location in flash where last byte of data needs to be written */
    LPVOID recEndAddr        = NULL;    /* Address of the last byte of the current allocation */

    RECORD_ID recordID     = 0;         /* Record ID of the record where we are trying to write */
    uint32    recordSize   = 0;         /* Allocation size of this record */
    uint32 numBytesWritten = 0;         /* Actual number of bytes that can be written */
    uint32 numBytesToBeWritten = 0;

    uint32 overWriteStartOffset = 0;    /* Doesn't matter much in new implementation */
    bool   isOverWriteDetected  = false;

    RECORD_HEADER recHeader = { 0 };

    lastErrorVal = DATASTORE_ERROR_NONE;

    recWriteStartAddr = dest + sizeof(recHeader);

    // AnanthAtSamraksh. The STM Driver accepts half words as inputs so all writes are in half word sizes
	numBytes = numBytes + numBytes % 2;

    do{
        DATASTORE_ASSERT( dest != NULL && dataToBeWritten != NULL, "Invalid parameters being passed" );
        if(dest == NULL || dataToBeWritten == NULL){
            lastErrorVal = DATASTORE_ERROR_INVALID_PARAM;
            numBytesToBeWritten = 0;
            break;
        }

        /* Initialize all the pointers */
        fromRecBaseAddr = dest;
        if(NULL == fromRecBaseAddr){
            lastErrorVal = DATASTORE_ERROR_INVALID_GIVEN_ADDR;
            DATASTORE_ASSERT( NULL != fromRecBaseAddr,
                              "Invalid address being passed for write" );
            break;
        }

        //recordSize        = addressTable.getMaxWriteSize(addressTable.getGivenAddress(addressTable.getRecordID(dest)));
        //recordSize        = addressTable.getMaxWriteSize(addressTable.getCurrentLoc(11));
        recordSize        = addressTable.getMaxWriteSize(recWriteStartAddr);

        /*recWriteStartAddr = addressTable.getCurrentLoc( dest,
                                                        (char*)blockDeviceInformation->Regions->Start + dataStoreStartByteOffset,
                                                        (char*)blockDeviceInformation->Regions->Start + dataStoreEndByteOffset );*/

        numBytesToBeWritten   = recordSize;    /* Number of bytes till the end of record */
        /* Minimum of these two sizes */
        numBytesToBeWritten   = (numBytes < numBytesToBeWritten)?numBytes:numBytesToBeWritten;

        recWriteEndAddr   = incrementPointer((char*) recWriteStartAddr + offset, numBytesToBeWritten - 1);              /* corresponding End address on flash */
        recEndAddr        = incrementPointer(recWriteStartAddr , recordSize - 1);     /* Last byte address of the record on flash */

        /* Now check if there is a overwrite possibility */
        isOverWriteDetected = detectOverWrite( (char*) recWriteStartAddr + offset,
                                               dataToBeWritten,
                                               numBytesToBeWritten,
                                               &overWriteStartOffset );

        if(false == isOverWriteDetected){
            /* No overwrites are detected, so its safe to just write data */
        	//debug_printf("Data_Store::writeRawData. No overwrite detected\n");
        	/***int i = 0;
        	char *dtbw = (char*)dataToBeWritten;
        	while(i < numBytesToBeWritten)
        	{
        		hal_printf("write first: %d\r\n", dtbw[i]);
        		i++;
        	}***/
        	numBytesWritten = cyclicDataWrite( dataToBeWritten,
                                               (char*) recWriteStartAddr + offset,
                                               numBytesToBeWritten );
        }else{
            /* Possibility for Overwrite is detected.
               We need to create a new allocation and move the data.
                Now we have three regions to write
               1. Region before overwrite
               2. Overwrite region
               3. Region after overwrite */

        	/* Get the given pointer base value from the destination */
            LPVOID lGivenPtr = (void*)0x1;

            LPVOID newRecBaseStartAddr  = NULL;
            LPVOID newRecWriteStartAddr = NULL;

            ////AnanthAtSamraksh -- adding below line to get the data type from record header
            cyclicDataRead( &recHeader, (char*)fromRecBaseAddr, sizeof(RECORD_HEADER) );

            newRecBaseStartAddr = createAllocation( recHeader.recordID, lGivenPtr, recordSize, recHeader.dataType );
            /****LPVOID tempCurLoc = addressTable.table[0].currentLoc;
            if(tempCurLoc != (newRecBaseStartAddr + sizeof(recHeader)))
            {
            	hal_printf("somebody gonna get hurt real bad!!\r\n");
            }****/

            if( NULL == newRecBaseStartAddr ){
                /* Allocation failed! */
                lastErrorVal = DATASTORE_ERROR_OUT_OF_FLASH_MEMORY;
                numBytesToBeWritten = 0;
                break;
            }

            /* createAllocation returns the pointer to the header location */
            newRecBaseStartAddr = incrementPointer( newRecBaseStartAddr , sizeof(RECORD_HEADER));

            /* Temporary pointers that point to the write location and length at each step below */
            LPVOID tempCurrentRecWriteLoc = newRecBaseStartAddr;

            //uint32 tempCurrentRecWriteLen = calculateNumBytes(newRecBaseStartAddr, newRecWriteStartAddr) - 1;
            ////uint32 tempCurrentRecWriteLen = calculateNumBytes(fromRecBaseAddr, recWriteStartAddr + offset + overWriteStartOffset) - 1;
            uint32 tempCurrentRecWriteLen = calculateNumBytes(recWriteStartAddr, (char*)recWriteStartAddr + offset) - 1;

            /* Write region-1 */
            /***int i = 0;
            char *rwsa = (char*)recWriteStartAddr;
			while(i < tempCurrentRecWriteLen)
			{
				hal_printf("write one: %d\r\n", rwsa[i]);
				i++;
			}***/
            numBytesWritten = cyclicDataWrite( recWriteStartAddr,
                             	 	 	 	 	 tempCurrentRecWriteLoc,
                             	 	 	 	 	 tempCurrentRecWriteLen );

            /* Move the current location pointer */
            tempCurrentRecWriteLoc = incrementPointer(tempCurrentRecWriteLoc, tempCurrentRecWriteLen);
            tempCurrentRecWriteLen = numBytesToBeWritten;

            /* Write region-2 */
            /***i = 0;
            char *dtbw = (char*)dataToBeWritten;
			while(i < tempCurrentRecWriteLen)
			{
				hal_printf("write two: %d\r\n", dtbw[i]);
				i++;
			}***/
            numBytesWritten += cyclicDataWrite( dataToBeWritten,
							 	 	 	 	 	 tempCurrentRecWriteLoc,
							 	 	 	 	 	 tempCurrentRecWriteLen );

            /* Move the current location pointer */
            tempCurrentRecWriteLoc = incrementPointer(tempCurrentRecWriteLoc , tempCurrentRecWriteLen);

            /*if((recWriteStartAddr + offset + numBytesToBeWritten) > recEndAddr)
            {
            	hal_printf("Lets stop here for a moment\r\n");
            }*/
            tempCurrentRecWriteLen = calculateNumBytes((char*)recWriteStartAddr + offset + numBytesToBeWritten - 1, recEndAddr) - 1;

            /* Write region-3 */
            /***i = 0;
            char *dd = (char*)(recWriteStartAddr + offset + numBytesToBeWritten);
			while(i < tempCurrentRecWriteLen)
			{
				hal_printf("write three: %d\r\n", dd[i]);
				i++;
			}***/
            numBytesWritten += cyclicDataWrite( (char*)recWriteStartAddr + offset + numBytesToBeWritten,
							 	 	 	 	 	 tempCurrentRecWriteLoc,
							 	 	 	 	 	 tempCurrentRecWriteLen );

            /* Congrats, thats all you had to do, write completed successfully :) */
        }
    }while(0);
    return numBytesWritten;
}
//Data_Store::writeRawData

DATASTORE_ERROR Data_Store::getLastError()
{
    return lastErrorVal;
}

/*
 * Remove all entries from the address table
 */
void Data_Store::DeleteAll()
{
	// When an entry is removed from the addressTable, the entries prior to that are copied over to the existing entry.
	// So, only the 0th index is deleted at all times.
	lastErrorVal = DATASTORE_ERROR_NONE;
	uint32 deleteIndex = 0;
	DATASTORE_STATUS status;

	while(addressTable.table[deleteIndex].givenPtr != 0)
	{
		status = deleteRecord(addressTable.table[deleteIndex].recordID);
		if(status != DATASTORE_STATUS_OK)
		{
			lastErrorVal = DATASTORE_ERROR_UNEXPECTED_ERROR;
			break;
		}
	}
}

/*
 * Performs GC on flash
 */
void Data_Store::DataStoreGC()
{
	compactLog();
}

/*
 * Erases entire flash. Starts from first block and erases all blocks until final block.
 */
void Data_Store::EraseAllBlocks()
{
    lastErrorVal = DATASTORE_ERROR_NONE;
    UINT32 dataStoreStartAddr = blockDeviceInformation->Regions->Start + dataStoreStartByteOffset;
	UINT32 dataStoreEndAddr = blockDeviceInformation->Regions->Start + dataStoreEndByteOffset;

	int lfirstBlockID = blockDeviceInformation->Regions->BlockIndexFromAddress(dataStoreStartAddr);
	int llastBlockID = blockDeviceInformation->Regions->BlockIndexFromAddress(dataStoreEndAddr);
	for(int blockID = lfirstBlockID; blockID <= llastBlockID; ++blockID)
	{
		ByteAddress eraseBlockAddress = blockDeviceInformation->Regions->BlockAddress(blockID);
		if(blockStorageDevice->EraseBlock(eraseBlockAddress) != true)
		{
			lastErrorVal = DATASTORE_ERROR_UNEXPECTED_ERROR;
			break;
		}
	}
	// AnanthAtSamraksh - Very important to reset the values of pointers below. Else causes remarkable bugs.
	logPointByteOffset = clearLogPointByOffset = dataStoreStartByteOffset;
	erasePointByteOffset = dataStoreEndByteOffset;
}

Data_Store::~Data_Store()
{
    //DATASTORE_REG_RemoveEntry(this);
#ifdef _EMULATOR_MODE
    delete flashDevice;
    delete myUniquePtrGen;
#endif
}


/*
 * Function to return maximum allocation size in block storage device.
 */
uint32 Data_Store::maxAllocationSize()
{
	return blockDeviceInformation->Regions->BytesPerBlock;
}


/*
 * Function to return total space in the block storage device.
 */
uint32 Data_Store::returnTotalSpace()
{
	//NumBlocks - 1, because DataStore starts from one block after initial block.
	return ((blockDeviceInformation->Regions->NumBlocks - 1) * blockDeviceInformation->Regions->BytesPerBlock);
}

/*
 * Function to return amount of free space. Invokes calculateLogHeadRoom()
 */
uint32 Data_Store::returnFreeSpace()
{
	return calculateLogHeadRoom();
}

/*
 * Function that returns the current value of the Log point. Also used to get total amount of space used.
 */
uint32 Data_Store::returnLogPoint()
{
	return (logPointByteOffset - dataStoreStartByteOffset);
}

#ifdef ENABLE_TEST
uint32 Data_Store::returnClearPoint()
{
    return clearLogPointByOffset;
}

uint32 Data_Store::returnErasePoint()
{
	return erasePointByteOffset;
}
#endif
