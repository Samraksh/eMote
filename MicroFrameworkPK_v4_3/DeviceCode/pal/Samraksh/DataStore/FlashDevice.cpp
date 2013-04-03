#include "Samraksh/DataStore/FlashDevice.h"
#include "Samraksh/DataStore/Datastore.h"

/*
void test_flash_interface()
{
	FlashDevice<int> flashDevice("NOR");
}
*/

BOOL FlashDevice::createMemoryMapping()
{
	BOOL retVal = FALSE;

	ASSERT(device != NULL);

	// Set the readOnlyPtr to the start of the flash memory
	readOnlyPtr = (void *) deviceInfo->Regions->Start;

	// Set the writePtr to the start of the flash memory
	writePtr = (void *) deviceInfo->Regions->Start;

	return TRUE;
}


BOOL FlashDevice::loadFlashProperties(int deviceWordSize)
{
	int fileVersion = -1;
	int headerSize = -1;
	int numClusters = -1;

	BYTE buffer[4];

	BOOL retval = FALSE;

	do
	{
		UINT32 startAddress = deviceInfo->Regions->Start;

		if(device->Read( startAddress, sizeof(headerSize), buffer ))
		{
			headerSize = (int) buffer;
		}

		startAddress += sizeof(headerSize);

		if(device->Read( startAddress, sizeof(fileVersion), buffer ))
		{
			fileVersion = (int) buffer;
		}

		startAddress += sizeof(fileVersion);

		if(fileVersion != FLASHDEVICE_FILE_FORMAT_VERSION)
			break;



	}while(false);
}



int FlashDevice::writeFlashProperties(int deviceWordSize)
{/* ToDo :: Should we Serialize the data before storing??? */
    int currentLoc = 0;
    int headerSize = 0;
    int versionNum = FLASHDEVICE_FILE_FORMAT_VERSION;

    BYTE tempData[20];

    const BlockRegionInfo* Regions;

    bool retBoolVal = false;
    DWORD status;
    DWORD temp;

    /*
        Header format :: <HeaderSize><FILE_FORMAT_VERSION><WORD_SIZE><NumClusters><NumBlocks><BlockSize><NumBlocks><BlockSize>...
    */

    do
    {
        /* Writing HEADER-SIZE */
        /* Calculate header size */
    	/*
		  Header format :: <HeaderSize><FILE_FORMAT_VERSION><WORD_SIZE><NumClusters><NumBlocks><BlockSize><NumBlocks><BlockSize>...
    	 */
        headerSize = sizeof(headerSize) + sizeof(versionNum) + sizeof(int) + sizeof(int) + this->deviceInfo->NumRegions*(sizeof(int)*2);

        if(!this->device->Write(this->currAddress,sizeof(headerSize),(BYTE *) &headerSize,FALSE))
        {
        	debug_printf("Failed to write required num of bytes");
        	break;
        }

        updateCurrAddress(sizeof(headerSize));




        /* Writing VERSION-NUMBER */
        /* Store flash file format version number */
        versionNum = FLASHDEVICE_FILE_FORMAT_VERSION;

        if(!device->Write(currAddress,sizeof(versionNum),(BYTE *) &versionNum,FALSE))
        {
           	debug_printf("Failed to write version number to the header");
           	break;
        }

        updateCurrAddress(sizeof(versionNum));
        /* Writing DEVICE-WORD-SIZE */
        /* Store flash file format version number */
        if(!device->Write(currAddress,sizeof(deviceWordSize),(BYTE *) &deviceWordSize,FALSE))
        {
            debug_printf("Failed to write device wordSize to the header");
            break;
        }

        updateCurrAddress(sizeof(deviceWordSize));

        /* WRITING NUM-CLUSTERS */
        if(!device->Write(currAddress,sizeof(deviceInfo->NumRegions),(BYTE *) &deviceInfo->NumRegions,FALSE))
        {
            debug_printf("Failed to write number of clusters");
            break;
        }

        updateCurrAddress(sizeof(deviceInfo->NumRegions));

        Regions = deviceInfo->Regions;
        /* WRITE - CLUSTER DETAILS */
        for(int index = 0; index < deviceInfo->NumRegions; index++){

            /* Write - Number of blocks */
        	 if(!device->Write(currAddress,sizeof(Regions->NumBlocks),(BYTE *) &Regions->NumBlocks,FALSE))
        	 {
        	    debug_printf("Failed to write number of blocks to header");
        	    break;
        	 }
        	 updateCurrAddress(sizeof(deviceInfo->NumRegions));

        	 /* Write - Block size */
        	 if(!device->Write(currAddress,sizeof(Regions->BytesPerBlock),(BYTE *) &Regions->BytesPerBlock,FALSE))
        	 {
        	     debug_printf("Failed to write Block Size");
        	 	 break;
           	 }

        	 updateCurrAddress(sizeof(Regions->BytesPerBlock));


        } /* End of for-loop */
    }while(false);
    return headerSize;
}

int FlashDevice::writeData( T *dataIn, int dataCount, void* addr )
{
	UINT32 address = (UINT32) addr;

	if(!device->Write(address,sizeof(T) * dataCount, (BYTE *) dataIn, FALSE))
	{
		debug_printf("Failed To Write to Memory");
	}
     return 0;
}

int FlashDevice::writeRawData( void *rawDataIn, int numBytes, void* addr )
{
	UINT32 address = (UINT32) addr;

	if(!device->Write(address, numBytes, (BYTE *) rawDataIn, FALSE))
	{
		debug_printf("Failed to write rawdata to Memory");
	}
	return 0;
}

FlashDevice::FlashDevice()
{
	this->flashDeviceName = "NOR";
	initFlashDevice("NOR");
}

FlashDevice::FlashDevice(char *flashName)
{
	//this->flashDeviceName = flashName;
	flashDeviceName = NULL;
	initFlashDevice( flashName);
}

Flash_Memory* FlashDevice::getDeviceBaseAddress()
{
	return (Flash_Memory*) deviceInfo->Regions->Start;
}

int FlashDevice::getBlockIDFromAddress( LPVOID address )
{
	int blockID = -1;
	int blockCount = 0;

	char *offset = (char *) address;

	do
	{
		for(int index = 0; index < conversionTblLen; index++)
		{
			if( conversionTbl[index].blockStartAddr <= address &&
			                conversionTbl[index].blockEndAddr >= address ){
			                /* Given address falls in this range, calculate the blockID */
			                blockID = ((char*)address - conversionTbl[index].blockStartAddr)/conversionTbl[index].blockSize;
			                blockID = blockID + conversionTbl[index].blockStartID;

			                ASSERT( blockID < conversionTbl[index].blockEndID);
			                break;
			 }
		}
	}while(false);

	return blockID;
}

Flash_Memory* FlashDevice::getBlockAddressFromBlockID(int blockID)
{
	ASSERT( state == FLASHDEVICE_STATE_READY);
	ASSERT( blockID >= 0);

	LPVOID retAddr = NULL;

	for(int index = 0; index < conversionTblLen; index++){
	        if( conversionTbl[index].blockStartID <= blockID &&
	            conversionTbl[index].blockEndID >= blockID ){
	                retAddr = conversionTbl[index].blockStartAddr + (blockID - conversionTbl[index].blockStartID)*conversionTbl[index].blockSize;
	            break;
	        }
	    }
	return (Flash_Memory*)retAddr;
}

FLASHDEVICE_STATUS FlashDevice::eraseBlock( int eraseBlockID )
{
	ASSERT(state == FLASHDEVICE_STATE_READY);
	ASSERT(eraseBlockID >= 0);

	Flash_Memory<T>* eraseBlockAddr = getBlockAddressFromBlockID(eraseBlockID);

	UINT32 eraseAddr = (UINT32) eraseBlockAddr;

	if(device->EraseBlock(eraseAddr))
		return FLASHDEVICE_STATUS_OK;

	return FLASHDEVICE_STATUS_NOT_OK;
}

int FlashDevice::getNumberofClusters()
{
	return 1;
}

FLASHDEVICE_STATUS FlashDevice::getDeviceProperties(FLASH_PROPERTIES *properties)
{

}

FLASHDEVICE_STATUS FlashDevice::getDeviceProperties(const BlockDeviceInfo* deviceInfo)
{
	deviceInfo = this->deviceInfo;
	return FLASHDEVICE_STATUS_OK;
}


FLASHDEVICE_STATUS FlashDevice::getBlockStatistic( int blockID,
                                                      FLASH_BLOCK_STATISTICS *statistic)
{
	return FLASHDEVICE_STATUS_NOT_OK;
}

FLASHDEVICE_STATUS FlashDevice::close()
{
	return FLASHDEVICE_STATUS_NOT_OK;
}

FlashDevice::~FlashDevice()
{
    if(state == FLASHDEVICE_STATE_READY){
        close();
    }
}

int FlashDevice::getBlockSize(int blockID)
{
	int blockSize = -1;

	ASSERT(FLASHDEVICE_STATE_READY == state);
	ASSERT(blockID >= 0);

	 for( int index = 0; index < conversionTblLen; index++ ){
	        if( conversionTbl[index].blockStartID <= blockID &&
	            conversionTbl[index].blockEndID >= blockID ){
	            blockSize = conversionTbl[index].blockSize;
	            break;
	       }
	}
	return blockSize;
}

BOOL FlashDevice::createNewFlashDeviceFile( char *flashName)
{
    BOOL retVal = FALSE;
    UINT32 location = 0x64000000;
    ByteAddress address;

    int versionNum = FLASHDEVICE_FILE_FORMAT_VERSION;

    ASSERT( flashName != NULL );

    do{
    	if (BlockStorageList::FindDeviceForPhysicalAddress( &device, location, address ))
    	{
    		UINT32 iRegion, iRange;
			const BlockDeviceInfo* deviceInfo = device->GetDeviceInfo() ;

			// Updating currAddress to start of the flash
			currAddress = location;

			if(!device->FindRegionFromAddress( location, iRegion, iRange ))
    		{
    		       debug_printf(" Invalid condition - Fail to find the block number from the ByteAddress %x \r\n",location);

				            return FALSE;
    		}
        /* Copy properties into the emulator class */
    		//copyFlashProperties( &this->deviceInfo,	deviceInfo );
			this->deviceInfo = deviceInfo;

        /* From properties calculate and store the flash Size */
			flashSize = deviceInfo->Size;

        /* Create header for the flash file */
			//headerSize = writeFlashProperties( device, sizeof(T), properties );
			//headerSize = writeFlashProperties(sizeof(T));
			// Not Writing to flash region
			 headerSize = sizeof(headerSize) + sizeof(versionNum) + sizeof(int) + sizeof(int) + deviceInfo->NumRegions*(sizeof(int)*2);

        /* Write Flash Area */
			// Nived : Commenting out for now, it initializes the entire flash region to 0
			//writeFlashRegion( fileHandle, headerSize, flashSize );

        /* Write the Statistics region of emulation file */
			//writeFlashStatisticsArea(*this);

			retVal = true;
    	}
    }while(false);

    return retVal;
}

// Not sure what is actually being written, can only see file pointers being moved around, currently i am writing null
bool writeFlashStatisticsArea(FlashDevice *fd)
{
    int numBlocks = 0;
    FLASH_BLOCK_STATISTICS initStatistics = { 0 };
    bool status = false;
    return true;       //Can anything go wrong above?
}

void writeFlashRegion(USHORT headerSize, DWORD fileSize )
{

}

void tempBreakPoint()
{
	debug_printf("In a breakPoint");
}


void FlashDevice::initFlashDevice( char * flashName)
{
    bool fileExists = false;
    bool retVal = false;

    ASSERT( NULL != flashName);

    /* 0. Create or load a file
       1. Copy any required data like properties into this object and calculate anything required.
       2. Initialize the file with header, content and statistics area
          create two io memory mappings with read-only and read-write
          permissions each */

    tempBreakPoint();

    state = FLASHDEVICE_STATE_UNINITIALIZED;

    fileExists = doesFileExist();

    if( false == fileExists ){
        /* File doesn't exist, create a new device emulation file */

        retVal = createNewFlashDeviceFile( flashName);
        ASSERT( true == retVal);
        if(true != retVal){
            state = FLASHDEVICE_STATE_FAILED_TO_LOAD;
        }
    }else{
        /* Create and initialize new flash device file */
        //flashProperties.numClusters = 0;
        //flashProperties.clusters    = NULL;

        //retVal = loadExistingFlashDevice( flashName, &flashProperties );
        //EMULATOR_ASSERT( true == retVal, "Failed to Open existing Flash Device!");

        //if( true ==  retVal ){
        //    flashSize  = calculateFlashSize(&flashProperties);
        //    headerSize = readHeaderSize(fileHandle);
        //}else{
        //    state = FLASHDEVICE_STATE_FAILED_TO_LOAD;
        //}

    }

    /* If we have the emulation file ready, we can set the state to active and
       CREATE FILE MEMORY-MAPPING */


       if( true == retVal ){
            retVal = createMemoryMapping();
            ASSERT( retVal == true);

            if( true == retVal){
                /* Now we can register the memory mapping, so that we can lookup the
                   required address from READ_ONLY view to READ_WRITE view mapping */
#if 0
                ADDRESS_MAP mapping;

                mapping.inputBaseAddr  = (void*)((char*)readOnlyPtr + headerSize); /* Register only the flash area */
                mapping.addrSpaceLen   = flashSize;
                mapping.outputBaseAddr = (void*)((char*)writePtr + headerSize);

                //retVal = addAddressMapping( &mapping );
                ASSERT( retVal == true);
#endif
                buildConversionTbl();       /* Build lookup table */

                /* Set Emulator object state based on the return value */
                state = (true == retVal)?(FLASHDEVICE_STATE_READY):(FLASHDEVICE_STATE_UNINITIALIZED);
            }

       }

    return;
}

// Current implementation have only one cluster, need to re examine this function when that assumption changes
void FlashDevice::buildConversionTbl()
{
	UINT32 i = 0;
	const BlockRegionInfo* regions = deviceInfo->Regions;
	conversionTbl[0].blockStartID = 0;
	conversionTbl[0].blockEndID = 	NUMBER_OF_FLASHBLOCKS - 1;
	conversionTbl[0].blockSize = regions->BytesPerBlock;
	conversionTbl[0].blockStartAddr = (char *) regions->Start;
	conversionTbl[0].blockEndAddr = conversionTbl[0].blockStartAddr + (conversionTbl[0].blockEndID - conversionTbl[0].blockStartID + 1) * conversionTbl[0].blockSize - 1;
	conversionTblLen = 1;
}
