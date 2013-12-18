#include <Samraksh/DataStore/Datastore.h>

Data_Store g_dataStoreObject;

DeviceStatus Data_Store::init()
{
    	 if(initialized == TRUE)
    		 return DS_Success;

    	 myUniquePtrGen.init();
    	 //flashDevice.init();

    	 ByteAddress Address;

    	 if (!BlockStorageList::FindDeviceForPhysicalAddress(&blockStorageDevice, DATASTORE_START_ADDRESS, Address))
		{
			return DS_Fail;
		}

    	 /*if(!blockStorageDevice->InitializeDevice())
    		 return DS_Fail;*/

    	 if((blockDeviceInformation = blockStorageDevice->GetDeviceInfo()) == NULL)
    		 return DS_Fail;


    	 state = DATASTORE_STATE_UNINIT;
    	 DATASTORE_STATUS status;

    	 lastErrorVal = DATASTORE_ERROR_INVALID_PARAM;

		 status = initDataStore( "NOR", &defaultProperty );

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


LPVOID Data_Store::incrementPointer(LPVOID inputPtr, int numBytes)
{
    char *lPtr = (char*)inputPtr;

    //char *lDataStoreEndPtr = (char*)flashDevice.getDeviceBaseAddress() + dataStoreEndByteOffset;
    //char *lDataStoreStartPtr = (char*)flashDevice.getDeviceBaseAddress() + dataStoreStartByteOffset;

    char *lDataStoreEndPtr = (char*)blockDeviceInformation->Regions->Start + dataStoreEndByteOffset;
    char *lDataStoreStartPtr = (char*)blockDeviceInformation->Regions->Start + dataStoreStartByteOffset;

    lPtr = lPtr + numBytes;
    if(lPtr > lDataStoreEndPtr){
        lPtr = lDataStoreStartPtr + (lPtr - lDataStoreEndPtr - 1);
    }else if(lPtr < lDataStoreStartPtr){
    	// AnanthAtSamraksh - Modifying existing pointer logic of data store
    	// The original piece of code no longer works
        lPtr = lDataStoreEndPtr - (lDataStoreStartPtr - lPtr - 1);
    	////lPtr = lDataStoreStartPtr + numBytes;
    	//lPtr = *lDataStoreStartPtr + dataStoreStartByteOffset + lPtr;
    }
    return lPtr;
}

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
            /*char *startAddr = (char*)flashDevice.getDeviceBaseAddress() + dataStoreStartByteOffset ;
            char *endAddr   = (char*)flashDevice.getDeviceBaseAddress() + dataStoreEndByteOffset ;*/
        	char *startAddr = (char*)blockDeviceInformation->Regions->Start + dataStoreStartByteOffset ;
        	char *endAddr   = (char*)blockDeviceInformation->Regions->Start + dataStoreEndByteOffset ;


            retVal = (int)((endAddr - (char*)fromAddr + 1) + ((char*)toAddr - startAddr + 1) );
        }
    }
    while(0);
    return retVal;
}

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

LPVOID Data_Store::createAllocation( RECORD_ID recordID, LPVOID givenPtr, uint32 numBytes )
{
	//debug_printf("Data_Store::createAllocation\n");
    LPVOID retVal = NULL;
    uint32 allocationSize = numBytes + sizeof(RECORD_HEADER);
    RECORD_HEADER header = { 0 };

    lastErrorVal = DATASTORE_ERROR_NONE;
    do{
    	/* Create a dummy/Skip allocation if required to make sure that this allocation
    	           doesn't fall in sectors -  return value tells if an allocation was done or not
    	           We don't care to know the status here */
	    createDummyAllocation(numBytes);

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
                if(currentHeadRoom >= (MIN_SPACE_REQUIRED_FOR_COMPACTION+ allocationSize)){
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

        /* Now that we have created enough free space, continue with the allocation */
        //// AnanthAtSamraksh - commenting out the code, since persistence is not yet implemented.
        //// AnanthAtSamraksh - uncommenting the code, as address to which data is written is 0. Address has to be incremented to point to start of data store region
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
        	//debug_printf("Data_Store::createAllocation. Marking old record inactive.\n");
            oldRecAddr = incrementPointer( oldRecAddr, -1*sizeof(RECORD_HEADER));
            cyclicDataRead( &header,
                            oldRecAddr,
                            sizeof(RECORD_HEADER) );

            header.activeFlag = FLAG_RECORD_INACTIVE;
            header.nextLink = incrementPointer(retVal, sizeof(RECORD_HEADER));

            cyclicDataWrite( &header,
                             oldRecAddr,
                             sizeof(RECORD_HEADER) );

            /* Now, update the header struct and write it back */
            header.activeFlag = FLAG_RECORD_ACTIVE;
            header.nextLink   = (LPVOID)~0;
            header.version++;                 /* Move to next value */
            header.zero       = 0;

            /* Writing at the end of the function */
            addressTable.updateCurrentLocation( recordID, incrementPointer(retVal, sizeof(RECORD_HEADER)));
        }else{
            /* This is the first time this record is being created - So init the header struct */
            DATASTORE_ADDR_TBL_ENTRY entry;

            entry.allocationSize = numBytes;
            entry.currentLoc     = (char*)incrementPointer(retVal , sizeof(RECORD_HEADER));
            entry.givenPtr       = givenPtr;
            entry.recordID       = recordID;

            header.activeFlag = FLAG_RECORD_ACTIVE;
            header.nextLink   = (LPVOID)~0;
            header.recordID   = recordID;
            header.size       = numBytes;
            header.version    = 0;
            header.zero       = 0;

            addressTable.addEntry(&entry);
        }
        /* Now, write the header to flash  */
        //debug_printf("Data_Store::createAllocation. Writing record. retVal is %x\n", retVal);
        cyclicDataWrite( &header,
                         retVal,
                         sizeof(RECORD_HEADER) );
    }while(0);
    return retVal;
}


bool Data_Store::detectOverWrite( void *addr, void *data, int dataLen, uint32 *conflictStartLoc )
{
    char *flashData = (char*)addr;
    char *givenData = (char*)data;
    bool retVal = false;

    *conflictStartLoc = dataLen;

    for(int index = 0; index < dataLen; index++){
        if((flashData[index] & givenData[index]) != givenData[index]){
            // Overwrite detected
            retVal = true;
            *conflictStartLoc = index;
            break;
        }
    }
    return retVal;
}

// Get base address for given Record_id
LPVOID Data_Store::getAddress(RECORD_ID id)
{
    LPVOID retVal = NULL;
    lastErrorVal = DATASTORE_ERROR_NONE;

    if( NULL == (retVal = addressTable.getGivenAddress(id)) ){
        lastErrorVal = DATASTORE_ERROR_INVALID_RECORD_ID;
    }
    return retVal;
}

// Get base address for given Record_id
RECORD_ID Data_Store::getRecordID(LPVOID givenPtr)
{
    return addressTable.getRecordID(givenPtr);
}


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


DATASTORE_STATUS Data_Store::initDataStore( char *datastoreName, DATASTORE_PROPERTIES *property )
{
    DATASTORE_STATUS status = DATASTORE_STATUS_INVALID_PARAM;
    //FLASH_PROPERTIES flashProp = { 0 };
    do{
        if( NULL == datastoreName ||
            NULL == property ){
            lastErrorVal = DATASTORE_ERROR_INVALID_PARAM;
            break;
        }

        ////DATASTORE_ASSERT( flashDevice.getDeviceState()== EMULATOR_STATE_READY, "Created flash device not in ready state!" );
        ////// AnanthAtSamraksh - TODO - Add flash status to datastore.h and change initialize to add the status
        ////// AnanthAtSamraksh - TODO - Add initialization for the 3 block storage variables so they are pointing to what they need to be pointing to

        /*if( FLASHDEVICE_STATE_READY != flashDevice.getDeviceState()){
            lastErrorVal = DATASTORE_ERROR_UNEXPECTED_ERROR;
            status       = DATASTORE_STATUS_INT_ERROR;
            break;
        }*/
        /* End of Flash Emulator creation */

        /* Initialize the Unique Pointer class */
        //myUniquePtrGen = new uniquePtr( property->addressRangeStart,
        //                                property->addressRangeEnd );
        /*
        DATASTORE_ASSERT( myUniquePtrGen != NULL,
                          "new failed to create uniquePtr Object" );
        if( NULL == myUniquePtrGen ){
            status       = DATASTORE_STATUS_OUT_OF_MEM;
            lastErrorVal = DATASTORE_ERROR_OUT_OF_MEMORY;
            break;
        }
        */

        /* Initialize the internal variables */
        int lDataStoreStartBlockID = 0;      /* Will calculate BlockID of first block of datastore */
        LPVOID lDataStoreStartPtr = NULL;
        /* This represents the block where the given offset starts */
        //lDataStoreStartBlockID = flashDevice.getBlockIDFromAddress( (char*)blockRegionInfo.Start + DATA_STORE_OFFSET );
        lDataStoreStartBlockID = blockDeviceInformation->Regions->BlockIndexFromAddress( blockDeviceInformation->Regions->Start + DATA_STORE_OFFSET );
        /* Datastore should start at the beginning of next block */
        lDataStoreStartBlockID = lDataStoreStartBlockID + 1;

        //// AnanthAtSamraksh - adding this temporarily
        ////blockStorageDevice->EraseBlock(lDataStoreStartBlockID);

        /* Now We have the address of the first block of datastore - Calculate byte offset from the
           beginning of the flash */
        lDataStoreStartPtr = (LPVOID)blockDeviceInformation->Regions->BlockAddress(lDataStoreStartBlockID);

        dataStoreDeviceSize = blockDeviceInformation->Regions->NumBlocks * blockDeviceInformation->Regions->BytesPerBlock;
        dataStoreStartByteOffset = (uint32)((char*)lDataStoreStartPtr - (char*)blockDeviceInformation->Regions->Start);
        //dataStoreEndByteOffset   = flashDevice.getDeviceSize() - 1;
        dataStoreEndByteOffset   = dataStoreDeviceSize - 1;

        //EraseAllBlocks();

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

        lastErrorVal = DATASTORE_ERROR_NONE;
        status       = DATASTORE_STATUS_OK;
        state        = DATASTORE_STATE_READY;
    }while(0);
    return status;
}

bool Data_Store::createDummyAllocation(int nextAllocationSize)
{
    //char *logPtr     = NULL;
    UINT32 logPtr     = NULL;
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

		//flashDevice.writeRawData( &lflag, 1, logPtr);
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

uint32 Data_Store::cyclicDataWrite( LPVOID buff,
                                       LPVOID currentLoc,
                                       uint32 numBytes )
{
    uint32 retVal = numBytes;
    uint32 offset = 0;

    if(numBytes == 0)
    	return 0;
    //LPVOID targetLoc = (char*)flashDevice.getDeviceBaseAddress()+4156;

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
				PRINT_DEBUG("Failed to write raw data to Memory");
				lastErrorVal =  DATASTORE_ERROR_WRITE_TO_FLASH_MEMORY_FAILED;
				break;
			}
            //flashDevice.writeRawData( buff, numBytes, currentLoc );
        }else{
            /* Need to read in pieces */
            int byteTillEnd = dataStoreEndByteOffset - offset + 1;

            /* Copy first piece */
            UINT32 address = (UINT32) currentLoc;
			void *rawDataIn = buff;
			if(!blockStorageDevice->Write(address, byteTillEnd, (BYTE *) rawDataIn, FALSE))
			{
				PRINT_DEBUG("Failed to write raw data to Memory");
				lastErrorVal =  DATASTORE_ERROR_WRITE_TO_FLASH_MEMORY_FAILED;
				break;
			}
            //flashDevice.writeRawData( buff, byteTillEnd, currentLoc );
            /* Copy second piece */
			address = blockDeviceInformation->Regions->Start + dataStoreStartByteOffset;
			rawDataIn = buff + byteTillEnd;
			if(!blockStorageDevice->Write(address, (numBytes - byteTillEnd), (BYTE *) rawDataIn, FALSE))
			{
				PRINT_DEBUG("Failed to write raw data to Memory");
				lastErrorVal =  DATASTORE_ERROR_WRITE_TO_FLASH_MEMORY_FAILED;
				break;
			}
            //flashDevice.writeRawData( (char*)buff+byteTillEnd, numBytes - byteTillEnd, (char*)blockRegionInfo.Start+dataStoreStartByteOffset );
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

DATASTORE_STATUS Data_Store::compactLog()
{
	//debug_printf("Data_Store::compactLog\n");
    uint32 initialClearPtrBlockID;
    uint32 currClearPtrBlockID;
    DATASTORE_STATUS status = DATASTORE_STATUS_NOT_OK;
    RECORD_HEADER recHeader = { 0 };
    char *clearPtr = NULL;
    //UINT32* clearPtr = NULL;

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
			/*int  currentBlockID = blockDeviceInformation->Regions->BlockIndexFromAddress((UINT32)clearPtr);
			char *sectorEndByte = (char*)blockDeviceInformation->Regions->BlockAddress(currentBlockID) +
															   blockDeviceInformation->Regions->BytesPerBlock - 1;
			int numBytesToIncrement = sectorEndByte - clearPtr + 1;

			incrementClearPoint(numBytesToIncrement);
			clearPtr = (char*)blockDeviceInformation->Regions->Start + clearLogPointByOffset;*/

        	/*UINT32 dataStoreStartAddr = blockDeviceInformation->Regions->Start + dataStoreStartByteOffset;
			UINT32 dataStoreEndAddr = blockDeviceInformation->Regions->Start + dataStoreEndByteOffset;
			int lfirstBlockID = blockDeviceInformation->Regions->BlockIndexFromAddress(dataStoreStartAddr);
			int llastBlockID = blockDeviceInformation->Regions->BlockIndexFromAddress(dataStoreEndAddr);*/

        	//int blockID = blockDeviceInformation->Regions->BlockIndexFromAddress((UINT32)clearPtr);
        	//char* address = (char*)blockDeviceInformation->Regions->BlockAddress(blockID);

        	////cyclicDataRead( &recHeader, clearPtr, sizeof(RECORD_HEADER) );
        	////while(0 != recHeader.zero){
				/*blockID = blockDeviceInformation->Regions->BlockIndexFromAddress((UINT32)address);
				blockID++;
				if(blockID > llastBlockID){
					blockID = lfirstBlockID;
				}
				address = (char*)blockDeviceInformation->Regions->BlockAddress(blockID);

				char *sectorEndByte = (char*)blockDeviceInformation->Regions->BlockAddress(blockID) + blockDeviceInformation->Regions->BytesPerBlock - 1;
				int numBytesToIncrement = sectorEndByte - clearPtr + 1;*/

				/*incrementClearPoint(sizeof(RECORD_HEADER) + recHeader.size);
				clearPtr = (char*)blockDeviceInformation->Regions->Start + clearLogPointByOffset;
				cyclicDataRead( &recHeader, clearPtr, sizeof(RECORD_HEADER) );
			}*/


			////AnanthAtSamraksh - commenting out below to fix a bug where clear ptr is not being updated
        	//debug_printf("Data_Store::compactLog. Should never happen\n");
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
            recHeader.nextLink   = (char*)blockDeviceInformation->Regions->Start + \
                                        logPointByteOffset + \
                                        sizeof(RECORD_HEADER);
            cyclicDataWrite( (LPVOID)&recHeader, clearPtr, sizeof(RECORD_HEADER) );

            /* Old record is marked inactive, now migrate the record to the end of log */
            recHeader.activeFlag = FLAG_RECORD_ACTIVE;
            recHeader.nextLink   = (LPVOID)~0u;

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

        //flashDevice.eraseBlock(initialClearPtrBlockID);
    	ByteAddress eraseBlockAddress = blockDeviceInformation->Regions->BlockAddress(initialClearPtrBlockID);
        blockStorageDevice->EraseBlock(eraseBlockAddress);
        //incrementErasePoint(flashDevice.getBlockSize(initialClearPtrBlockID));
        incrementErasePoint(blockDeviceInformation->Regions->BytesPerBlock);
    }

    return status;
}

Data_Store::Data_Store(char *flashDeviceName)
{
    state = DATASTORE_STATE_UNINIT;
    DATASTORE_STATUS status;

    lastErrorVal = DATASTORE_ERROR_INVALID_PARAM;
    status = initDataStore( flashDeviceName,
                            &defaultProperty );

    state = (status == DATASTORE_STATUS_OK)?(DATASTORE_STATE_READY):(DATASTORE_STATE_INT_ERROR);
}

Data_Store::Data_Store( char *flashDeviceName,
                           DATASTORE_PROPERTIES *property )
{
    state = DATASTORE_STATE_UNINIT;
    DATASTORE_STATUS status;

    lastErrorVal = DATASTORE_ERROR_INVALID_PARAM;
    status = initDataStore( flashDeviceName,
                            property );

    state = (status == DATASTORE_STATUS_OK)?(DATASTORE_STATE_READY):(DATASTORE_STATE_INT_ERROR);
}


LPVOID Data_Store::traversePointer(PERSISTENCE_DIRECTION per_dir, char* address)
{
	int recordID;
	int blockID;
	LPVOID givenPtr;
	RECORD_HEADER header = { 0 };
	/*LPVOID dataStoreStartAddr = (char*)blockRegionInfo.Start + dataStoreStartByteOffset;
	LPVOID dataStoreEndAddr = (char*)blockRegionInfo.Start + dataStoreEndByteOffset;*/
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

LPVOID Data_Store::readPointers()
{
	char* returnAddress;
	char* tempAddress1;
	char* tempAddress2;
	int numBytes = 0;
	int tempBlockID = 0;
	PERSISTENCE_DIRECTION per_dir;
	RECORD_HEADER header;

	/*LPVOID dataStoreStartAddr = (char*)blockRegionInfo.Start + dataStoreStartByteOffset;
	LPVOID dataStoreEndAddr = (char*)blockRegionInfo.Start + dataStoreEndByteOffset;*/
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

int Data_Store::readRecordinBlock(int blockID)
{
	char* addr = (char *)blockDeviceInformation->Regions->BlockAddress(blockID);
	//char *endAddr = addr + flashDevice.getBlockSize(blockID);
	char *endAddr = addr + blockDeviceInformation->Regions->BytesPerBlock;
	int tempCount = 0;

	PRINT_DEBUG("Check Point 1.1 : In Function Read In Block");

	DATASTORE_ADDR_TBL_ENTRY entry;
	DATASTORE_STATUS status;
	int count = 0;
	RECORD_HEADER header;
	while(*addr != (char)0xFF && addr < (endAddr))
	{
		tempCount++;

		//hal_printf("Count Number %d\n", tempCount);

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


			/* Change the condition to check for the STATUS duplicate recordID */
			status = addressTable.addEntry(&entry);
			if(DATASTORE_STATUS_RECORD_ALREADY_EXISTS == status){
				int oldEntryVer;
				int curEntryVer;
				LPVOID temp;
				RECORD_HEADER tempHeader;

				//Mukundan: Removed Flash_memory
				//temp = (Flash_Memory<char> *)addressTable.getCurrentLoc(header.recordID);
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

		count++;
		addr = (char*)incrementPointer(addr, sizeof(RECORD_HEADER)+header.size);
	}
	PRINT_DEBUG("Check Point 1.2 : Exiting Function Read In Block");
	return count;
}

uint32 Data_Store::decideWhichOneisRecent(RECORD_HEADER tempHeader, RECORD_HEADER header)
{
    return((tempHeader.version+1)>header.version?tempHeader.version:header.version);
}

LPVOID Data_Store::scanFlashDevice()
{
	/*LPVOID dataStoreStartAddr = (char*)blockRegionInfo.Start + dataStoreStartByteOffset;
	LPVOID dataStoreEndAddr = (char*)blockRegionInfo.Start + dataStoreEndByteOffset;*/
	UINT32 dataStoreStartAddr = blockDeviceInformation->Regions->Start + dataStoreStartByteOffset;
	UINT32 dataStoreEndAddr = blockDeviceInformation->Regions->Start + dataStoreEndByteOffset;

	int firstBlock = blockDeviceInformation->Regions->BlockIndexFromAddress(dataStoreStartAddr);
	int lastBlock  = blockDeviceInformation->Regions->BlockIndexFromAddress(dataStoreEndAddr);
	int numOfEntries = 0;
	char* returnAddress;

	for(int index = firstBlock;index <= lastBlock; index++){
		numOfEntries += readRecordinBlock(index);
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


void Data_Store::getRecordIDAfterPersistence(uint32* recordID_array)
{
	uint32 persistenceIndex = 0;
	////UINT16 recordID_array[256];
	while(addressTable.table[persistenceIndex].recordID != 0)
	{
		recordID_array[persistenceIndex] = addressTable.table[persistenceIndex].recordID;
		persistenceIndex++;
	}
	////return recordID_array;
}

uint32 Data_Store::getCountOfRecordIds()
{
	uint32 recIdIndex = 0;
	////UINT16 recordID_array[256];
	while(addressTable.table[recIdIndex].recordID != 0)
	{
		++recIdIndex;
	}
	return recIdIndex;
}


DATASTORE_STATUS Data_Store::deleteRecord(RECORD_ID id)
{
    RECORD_HEADER header;
    DATASTORE_STATUS status = DATASTORE_STATUS_NOT_OK;
    lastErrorVal = DATASTORE_ERROR_NONE;
    do{
        LPVOID currLoc = addressTable.getCurrentLoc(id);
        if(currLoc == NULL){
            lastErrorVal = DATASTORE_ERROR_INVALID_RECORD_ID;
            break;
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

LPVOID Data_Store::createRecord( RECORD_ID recordID, uint32 numBytes )
{
    RECORD_HEADER header;
    LPVOID lGivenPtr = NULL;
    LPVOID allocLoc  = NULL;
    lastErrorVal = DATASTORE_ERROR_NONE;

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
        allocLoc  = createAllocation( recordID, lGivenPtr, numBytes );
        if( NULL == allocLoc ){
            break;
        }
        /* Address table will be updated by createAllocation, so need not to bother here */
    }while(0);
    return lGivenPtr;
}


uint32 Data_Store::readRawData(LPVOID src, void *data, uint32 numBytes)
{
	//debug_printf("Data_Store::readRawData\n");
    uint32 lNumBytes = 0;
    LPVOID curLoc = NULL;

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
        if( NULL == (curLoc = addressTable.getCurrentLoc(src, lDataStoreStartByteAddr, lDataStoreEndByteAddr))){
            lastErrorVal = DATASTORE_ERROR_INVALID_GIVEN_ADDR;
            lNumBytes = numBytes = 0;
            break;
        }
        //debug_printf("Data_Store::readRawData. curLoc is %x \n", curLoc);

        lNumBytes = addressTable.getMaxWriteSize(src);
        lNumBytes = (numBytes < lNumBytes)?numBytes:lNumBytes;

        cyclicDataRead( data, curLoc, lNumBytes);
    }while(0);
    return lNumBytes;
}

uint32 Data_Store::readData(LPVOID src, uint32* data, uint32 count)
{
    return readRawData( src, data, count*sizeof(int));
}

uint32 Data_Store::writeData( LPVOID dest, uint32* data, uint32 count )
{
    return writeRawData( dest, data, count*sizeof(int));
}

uint32 Data_Store::writeRawData(LPVOID dest, void* data, uint32 numBytes)
{
	//debug_printf("Data_Store::writeRawData\n");

    LPVOID recBaseAddr       = NULL;    /* Base address of the current allocation */
    LPVOID recWriteStartAddr = NULL;    /* Address that correspond to the address where give data needs to be written */
    LPVOID recWriteEndAddr   = NULL;    /* Address of corresponding location in flash where last byte of data needs to be written */
    LPVOID recEndAddr        = NULL;    /* Address of the last byte of the current allocation */

    RECORD_ID recordID     = 0;         /* Record ID of the record where we are trying to write */
    uint32    recordSize   = 0;         /* Allocation size of this record */
    uint32 numBytesWritten = 0;         /* Actual number of bytes that can be written */

    uint32 overWriteStartOffset = 0;    /* Doesn't matter much in new implementation */
    bool   isOverWriteDetected  = false;

    lastErrorVal = DATASTORE_ERROR_NONE;

    // AnanthAtSamraksh. The STM Driver accepts half words as inputs so all writes are in half word sizes
	numBytes = numBytes + numBytes % 2;

    do{
        DATASTORE_ASSERT( dest != NULL && data != NULL, "Invalid parameters being passed" );
        if(dest == NULL || data == NULL){
            lastErrorVal = DATASTORE_ERROR_INVALID_PARAM;
            numBytesWritten = 0;
            break;
        }

        /* Initialize all the pointers */
        recBaseAddr = addressTable.getCurrentLoc((recordID = addressTable.getRecordID(dest)));
        if(NULL == recBaseAddr){
            lastErrorVal = DATASTORE_ERROR_INVALID_GIVEN_ADDR;
            DATASTORE_ASSERT( NULL != recBaseAddr,
                              "Invalid address being passed for write" );
            break;
        }

        recordSize        = addressTable.getMaxWriteSize(addressTable.getGivenAddress(addressTable.getRecordID(dest)));

        recWriteStartAddr = addressTable.getCurrentLoc( dest,
                                                        (char*)blockDeviceInformation->Regions->Start + dataStoreStartByteOffset,
                                                        (char*)blockDeviceInformation->Regions->Start + dataStoreEndByteOffset );
        numBytesWritten   = addressTable.getMaxWriteSize(dest);    /* Number of bytes till the end of record */
        /* Minimum of these two sizes */
        numBytesWritten   = (numBytes < numBytesWritten)?numBytes:numBytesWritten;

        recWriteEndAddr   = incrementPointer(recWriteStartAddr , numBytesWritten - 1);              /* corresponding End address on flash */
        recEndAddr        = incrementPointer(recBaseAddr , recordSize - 1);     /* Last byte address of the record on flash */

        /* Now check if there is a overwrite possiblility */
        isOverWriteDetected = detectOverWrite( recWriteStartAddr,
                                               data,
                                               numBytesWritten,
                                               &overWriteStartOffset );
        if(false == isOverWriteDetected){
            /* No overwrites are detected, so its safe to just write data */
        	//debug_printf("Data_Store::writeRawData. No overwrite detected\n");
            numBytesWritten = cyclicDataWrite( data,
                                               recWriteStartAddr ,
                                               numBytesWritten );
        }else{
            /* Possibility for Overwrite is detected.
               We need to create a new allocation and move the data.
                Now we have three regions to write
               1. Region before overwrite
               2. Overwrite region
               3. Region after overwrite */

            /* Get the given pointer base value from the destination */
            LPVOID lGivenPtr = addressTable.getGivenAddress(addressTable.getRecordID(dest));

            LPVOID newRecBaseAddr       = NULL;
            LPVOID newRecWriteStartAddr = NULL;

            //debug_printf("Data_Store::writeRawData. Overwrite detected\n");

            newRecBaseAddr = createAllocation( recordID, lGivenPtr, recordSize );

            //debug_printf("Data_Store::writeRawData. Overwrite detected. After createAllocation\n");
            if( NULL == newRecBaseAddr ){
                /* Allocation failed! */
                lastErrorVal = DATASTORE_ERROR_OUT_OF_FLASH_MEMORY;
                numBytesWritten = 0;
                break;
            }

            /* createAllocation returns the pointer to the header location */
            newRecBaseAddr = incrementPointer( newRecBaseAddr , sizeof(RECORD_HEADER));

            newRecWriteStartAddr = addressTable.getCurrentLoc(dest,
                                                              (char*)blockDeviceInformation->Regions->Start + dataStoreStartByteOffset,
                                                              (char*)blockDeviceInformation->Regions->Start + dataStoreEndByteOffset );

            /* Temporary pointers that point to the write location and length at each step below */
            LPVOID lCurrentWriteLoc = newRecBaseAddr;
            uint32 lCurrentWriteLen = calculateNumBytes(newRecBaseAddr, newRecWriteStartAddr) - 1;

            /* Write region-1 */
            cyclicDataWrite( recBaseAddr,
                             lCurrentWriteLoc,
                             lCurrentWriteLen );

            /* Move the current location pointer */
            lCurrentWriteLoc = incrementPointer(lCurrentWriteLoc, lCurrentWriteLen);
            lCurrentWriteLen = numBytesWritten;

            /* Write region-2 */
            cyclicDataWrite( data,
                             lCurrentWriteLoc,
                             lCurrentWriteLen );

            /* Move the current location pointer */
            lCurrentWriteLoc = incrementPointer(lCurrentWriteLoc , lCurrentWriteLen);
            lCurrentWriteLen = calculateNumBytes(recWriteEndAddr, recEndAddr) - 1;

            /* Write region-3 */
            cyclicDataWrite( incrementPointer(recWriteEndAddr, 1),
                             lCurrentWriteLoc,
                             lCurrentWriteLen );

            /* Congrats, thats all you had to do, write completed successfully :) */
        }
    }while(0);
    return numBytesWritten;
}

DATASTORE_ERROR Data_Store::getLastError()
{
    return lastErrorVal;
}

void Data_Store::DeleteAll()
{
	// When an entry is removed from the addressTable, the entries prior to that are copied over to the existing entry.
	// So, only the 0th index is deleted at all times.
	uint32 deleteIndex = 0;
	DATASTORE_STATUS status;
	/*static int testErase = 0;
	if(testErase == 0){
		blockStorageDevice->EraseBlock(1);
		testErase++;
	}*/
	while(addressTable.table[deleteIndex].givenPtr != 0)
	{
		status = deleteRecord(addressTable.table[deleteIndex].recordID);
		if(status != DATASTORE_STATUS_OK)
		{
			break;
		}
	}
#if 0
	uint32 initialClearPtrBlockID;
	uint32 currClearPtrBlockID;
	DATASTORE_STATUS status = DATASTORE_STATUS_NOT_OK;
	RECORD_HEADER recHeader = { 0 };
	char *clearPtr = NULL;

	clearPtr = (char*)blockDeviceInformation->Regions->Start + clearLogPointByOffset;

	initialClearPtrBlockID = currClearPtrBlockID = blockDeviceInformation->Regions->BlockIndexFromAddress( (UINT32)clearPtr );
	while(initialClearPtrBlockID == currClearPtrBlockID)
	{
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
			/* Record that we are seeing is active, so we need to deactivate it
			 * - Can't use write method directly because current call
			   could be coming from there :) */

			  /* Create a dummy/skip allocation if required */
			createDummyAllocation(recHeader.size);


			/* First mark the current location as inactive */
			recHeader.activeFlag = FLAG_RECORD_INACTIVE;
			recHeader.nextLink   = (char*)blockDeviceInformation->Regions->Start + \
										logPointByteOffset + \
										sizeof(RECORD_HEADER);
			cyclicDataWrite( (LPVOID)&recHeader, clearPtr, sizeof(RECORD_HEADER) );

			incrementLogPointer(sizeof(RECORD_HEADER) + recHeader.size);
			incrementClearPoint(sizeof(RECORD_HEADER) + recHeader.size);

			clearPtr = (char*)blockDeviceInformation->Regions->Start + clearLogPointByOffset;
		}else{
			/* Inactive record, just move the clear pointer to the next record */
			incrementClearPoint(recHeader.size + sizeof(RECORD_HEADER));
			clearPtr = (char*)blockDeviceInformation->Regions->Start + clearLogPointByOffset;
		}
		/* Now, check the blockID where my clearPointer is now */
		currClearPtrBlockID = blockDeviceInformation->Regions->BlockIndexFromAddress((UINT32)clearPtr);

		status = DATASTORE_STATUS_OK;   //In a loop :)
	}
	return status;
#endif
}

void Data_Store::DataStoreGC()
{
	compactLog();
}

void Data_Store::EraseAllBlocks()
{
    ////AnanthAtSamraksh - adding this temporarily as sometimes datastore fails due to regions in flash blocks that have FFs. It is better to clean erase and start over.
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
}

Data_Store::~Data_Store()
{
    DATASTORE_REG_RemoveEntry(this);
#ifdef _EMULATOR_MODE
    delete flashDevice;
    delete myUniquePtrGen;
#endif
}

#ifdef ENABLE_TEST
uint32 Data_Store::returnLogPoint()
{
	return logPointByteOffset;
}

uint32 Data_Store::returnClearPoint()
{
    return clearLogPointByOffset;
}

uint32 Data_Store::returnErasePoint()
{
	return erasePointByteOffset;
}
#endif
