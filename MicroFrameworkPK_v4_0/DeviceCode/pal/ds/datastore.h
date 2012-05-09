#ifndef _DATASTORE_H_INCLUDED_
#define _DATASTORE_H_INCLUDED_


/* System include */

/* Intermodule includes */
#include "flash_device.h"

#include "types.h"
#include "datastore_int.h"
#include "datastore_reg.h"
#include "address_table.h"

/* Constants */
/* Threshold that needs to be always maintained*/
#define MIN_SPACE_REQUIRED_FOR_COMPACTION	    (2*1024)

/* Defines where the data-store begins in the flash */
#define DATA_STORE_OFFSET                   (0)

/* MACRO FUNCTIONs */

/* Default properties of device */
static DATASTORE_CLUSTER cluster[] = { { 1024,     32 }};

static DATASTORE_PROPERTIES defaultProperty = { cluster,
                                                sizeof(cluster)/sizeof(DATASTORE_CLUSTER),
                                                (LPVOID)0x1,
                                                (LPVOID)(((uint32)~0)>>2)
                                              };

/* Error values */
typedef enum _datastore_errors
{
    DATASTORE_ERROR_NONE,
    DATASTORE_ERROR_INVALID_PARAM,
    DATASTORE_ERROR_OUT_OF_MEMORY,
    DATASTORE_ERROR_INVALID_RECORD_ID,
    DATASTORE_ERROR_INVALID_GIVEN_ADDR,
    DATASTORE_ERROR_OUT_OF_BOUND_ACCESS,
    DATASTORE_ERROR_OUT_OF_FLASH_MEMORY,
    DATASTORE_ERROR_RECORD_ID_ALREADY_EXISTS,
    DATASTORE_ERROR_UNEXPECTED_ERROR
}DATASTORE_ERROR;


/* Data store class */
template<class T>
class Data_Store
{
private:
    /* Flash Emulation device */
    flash_device<T> *flashDevice;

    /* Current state of the device */
    DATASTORE_STATE state;
    /* Last error status - Stores any error that occured in previous API
       This value can be read by the Application using getLastErrorValue()
    */
    DATASTORE_ERROR lastErrorVal;

    /* Internal pointers that maintain the circular buffer locations */
    uint32 logPointByteOffset;
    uint32 erasePointByteOffset;
    uint32 clearLogPointByOffset;   /* Tells where the next record that can be checked for clearing is */

    /* Offsets telling start and end of flash data region - For ease of use */
    uint32 dataStoreStartByteOffset;
    uint32 dataStoreEndByteOffset;


    /* Address translation table */
    class DATASTORE_AddrTable addressTable;

    /* Unique Pointer generator */
    uniquePtr *myUniquePtrGen;

    /* Private methods */
    /* Locks */
    DATASTORE_STATUS lockDataStore();
    DATASTORE_STATUS unlockDataStore();

    /* PRIVATE FUNCTIONS */
    DATASTORE_STATUS initDataStore( char *datastoreName,
                                    DATASTORE_PROPERTIES *property );
    /* Compact function */
    DATASTORE_STATUS compactLog();

    /* Helper function to increment log-pointer cyclically */
    void incrementLogPointer(uint32 amount);

    /* Helper function to increment erase-point cyclically */
    void incrementErasePoint(uint32 amount);

    /* Helper function to increment erase-point cyclically */
    void incrementClearPoint(uint32 amount);

    /* Helper function to calculate Log-head room */
    uint32 calculateLogHeadRoom();

    /* Create new allocation */
    LPVOID createAllocation( RECORD_ID recordID, LPVOID givenPtr, uint32 numBytes);

    /* Detect overwrites - Not sure if this needs to be a member function */
    bool detectOverWrite( void *addr, void *data, int dataLen, uint32* conflictStartLoc );

    /* Reads record header from the flash handling cyclic copy */
    uint32 cyclicDataRead( LPVOID buff,
                           LPVOID currentLoc,
                           uint32 numBytes );

    /* Reads record header from the flash handling cyclic copy */
    uint32 cyclicDataWrite( LPVOID buff,
                            LPVOID currentLoc,
                            uint32 numBytes );

    /* Helper function for scanFlashDevice */
    uint32 decideWhichOneisRecent(RECORD_HEADER tempHeader, RECORD_HEADER header);

    /* Fetches the the start point for scan */
    uint32 returnScanAddress();

    /* Scan the flash device and load all the records in the Flash Device
       Could use this at bootup to load all the records in the flash */
    DATASTORE_STATUS scanFlashDevice();

public:
    /* Load with default settings */
    Data_Store( char *flashDeviceName );

    /* Create a Datastore with specific property - Only used with emulator */
    Data_Store( char *flashDeviceName,
                DATASTORE_PROPERTIES *property );

    /* Create a record in the data store */
    LPVOID createRecord( RECORD_ID id, uint32 numBytes );

    /* Delete record */
    DATASTORE_STATUS deleteRecord( RECORD_ID id );

    /* Get base address for given Record_id */
    LPVOID getAddress( RECORD_ID id );

    /* Symmetric API to lookup ID using PTR */
    RECORD_ID getRecordID(LPVOID givenPtr);

    /* Write data to the store */
    uint32 writeData( LPVOID dest, T *data, uint32 count );

    /* Write raw data - Can be of any word-size */
    uint32 writeRawData(LPVOID dest, void* data, uint32 numBytes);

    uint32 readData(LPVOID src, T* data, uint32 count);

    /* Read raw data */
    uint32 readRawData(LPVOID src, void *data, uint32 numBytes);

    /* Returns the error code of any error in the previous call */
    DATASTORE_ERROR getLastError();

    /* Destructor */
    ~Data_Store();
};



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
template<class T>
void Data_Store<T>::incrementLogPointer( uint32 amount )
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
        assert(false);
    }
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
template<class T>
void Data_Store<T>::incrementErasePoint(uint32 amount)
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
        assert(false);
    }
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
template<class T>
void Data_Store<T>::incrementClearPoint(uint32 amount)
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
        assert(false);
    }
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
template<class T>
uint32 Data_Store<T>::calculateLogHeadRoom()
{
    uint32 retVal = 0;
    if(erasePointByteOffset > logPointByteOffset){
        /* The free space doesn't overlap between end and beginning of flash */
        retVal = erasePointByteOffset - logPointByteOffset + 1;
    }else{
        retVal = (dataStoreEndByteOffset - logPointByteOffset + 1) +
                    (erasePointByteOffset - dataStoreStartByteOffset + 1);
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
template<class T>
LPVOID Data_Store<T>::createAllocation( RECORD_ID recordID, LPVOID givenPtr, uint32 numBytes )
{
    LPVOID retVal = NULL;
    uint32 allocationSize = numBytes + sizeof(RECORD_HEADER);
    RECORD_HEADER header = { 0 };

    lastErrorVal = DATASTORE_ERROR_NONE;
    do{
        if(calculateLogHeadRoom() < (MIN_SPACE_REQUIRED_FOR_COMPACTION + allocationSize)){
            /* Free memory is below the minimum threshold, try creating some free
               space by moving active records, clearing inactive hence compacting flash */
            int tempCount = 0;
            int prevHeadRoom = 0;
            int currentHeadRoom = calculateLogHeadRoom();
            while(prevHeadRoom != currentHeadRoom){
            /* Run till we are making some free space on each iteration */
                compactLog();   /* Clear some memory */
                prevHeadRoom = currentHeadRoom;
                currentHeadRoom = calculateLogHeadRoom();   /* Check free space available now */
                if(currentHeadRoom >= (MIN_SPACE_REQUIRED_FOR_COMPACTION+ allocationSize)){
                    /* Come out if we have enough space */
                    break;
                }
#ifdef EMULATION
                printf("%d\n", tempCount++);
#else
                debug_printf("%d\n", tempCount++);
#endif
            }
        }
        if(calculateLogHeadRoom() < (MIN_SPACE_REQUIRED_FOR_COMPACTION + allocationSize)){
            /* If this is the case, its impossible to create more headRoom (is it?) */
            lastErrorVal = DATASTORE_ERROR_OUT_OF_FLASH_MEMORY;
            break;
        }

        /* Now that we have created enough free space, continue with the allocation */
        retVal = ((char*)flashDevice->getDeviceBaseAddress()) + logPointByteOffset;
        incrementLogPointer(allocationSize);

        /* Now search for old allocation if any and mark them inactive - If this is the first record,
           then no harm done :)
           CHETHAN :: Should I pass a flag controlling whether this should be done or not, as in some
           cases its obviously unnecessary but still we would be doing it and lose some performance.
        */
        LPVOID oldRecAddr = addressTable.getCurrentLoc(recordID);         /* O(n) :( */
        if( NULL != oldRecAddr ){
            /* There is an old record already, mark it inactive */
            oldRecAddr = (char*)oldRecAddr - sizeof(RECORD_HEADER);
            cyclicDataRead( &header,
                            oldRecAddr,
                            sizeof(RECORD_HEADER) );

            header.activeFlag = FLAG_RECORD_INACTIVE;
            header.nextLink   = (char*)retVal+sizeof(RECORD_HEADER);

            cyclicDataWrite( &header,
                             oldRecAddr,
                             sizeof(RECORD_HEADER) );

            /* Now, update the header struct and write it back */
            header.activeFlag = FLAG_RECORD_ACTIVE;
            header.nextLink   = (LPVOID)~0;
            header.version++;                 /* Move to next value */
            header.zero       = 0;

            /* Writing at the end of the function */
            addressTable.updateCurrentLocation( recordID, (LPVOID)((char*)retVal+sizeof(RECORD_HEADER)) );
        }else{
            /* This is the first time this record is being created - So init the header struct */
            DATASTORE_ADDR_TBL_ENTRY entry;

            entry.allocationSize = numBytes;
            entry.currentLoc     = (char*)retVal + sizeof(RECORD_HEADER);
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
        cyclicDataWrite( &header,
                         retVal,
                         sizeof(RECORD_HEADER) );
    }while(0);
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
template<class T>
bool Data_Store<T>::detectOverWrite( void *addr, void *data, int dataLen, uint32 *conflictStartLoc )
{
    char *flashData = (char*)addr;
    char *givenData = (char*)data;
    bool retVal = false;

    *conflictStartLoc = dataLen;

    for(int index = 0; index < dataLen; index++){
        if((flashData[index] & givenData[index]) != givenData[index]){
            /* Overwrite detected */
            retVal = true;
            *conflictStartLoc = index;
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
/* Get base address for given Record_id */
template <class T>
LPVOID Data_Store<T>::getAddress(RECORD_ID id)
{
    LPVOID retVal = NULL;
    lastErrorVal = DATASTORE_ERROR_NONE;

    if( NULL == (retVal = addressTable.getGivenAddress(id)) ){
        lastErrorVal = DATASTORE_ERROR_INVALID_RECORD_ID;
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
/* Get base address for given Record_id */
template <class T>
RECORD_ID Data_Store<T>::getRecordID(LPVOID givenPtr)
{
    return addressTable.getRecordID(givenPtr);
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
template<class T>
DATASTORE_STATUS Data_Store<T>::initDataStore( char *datastoreName,
                                               DATASTORE_PROPERTIES *property )
{
    DATASTORE_STATUS status = DATASTORE_STATUS_INVALID_PARAM;
    FLASH_PROPERTIES flashProp = { 0 };
    do{
        if( NULL == datastoreName ||
            NULL == property ){
            lastErrorVal = DATASTORE_ERROR_INVALID_PARAM;
            break;
        }
#ifdef EMULATOR_MODE
        /* Starting of Flash Emulator creation */
        /* Fill flash emulator structures and initialize the flash emulator */
        flashProp.numClusters = property->numClusters;
        flashProp.clusters = (BLOCK_CLUSTER *)malloc(sizeof(BLOCK_CLUSTER)*flashProp.numClusters);
        DATASTORE_ASSERT( flashProp.clusters != NULL,
                          "Failed to malloc memory for flashProp.clusters" );
        if( NULL == flashProp.clusters){
            lastErrorVal = DATASTORE_ERROR_OUT_OF_MEMORY;
            status = DATASTORE_STATUS_OUT_OF_MEM;
            break;
        }

        for(int index = 0; index < flashProp.numClusters; index++){
            flashProp.clusters[index].blockSize = property->clusters[index].blockSize;
            flashProp.clusters[index].numBlocks = property->clusters[index].numBlocks;
        }


        flashDevice = new flash_emulator<T>(datastoreName, &flashProp);
#else
        flashDevice = new flash_device<T>(datastoreName);
#endif

        DATASTORE_ASSERT( flashDevice != NULL,
                          "Creating Flash Device, new failed!" );
        if( NULL == flashDevice ){
            lastErrorVal = DATASTORE_ERROR_UNEXPECTED_ERROR;
            status = DATASTORE_STATUS_INT_ERROR;
            break;
        }

        DATASTORE_ASSERT( flashDevice->getDeviceState()== EMULATOR_STATE_READY,
                          "Created flash device not in ready state!" );
        if( FLASHDEVICE_STATE_READY != flashDevice->getDeviceState()){
            lastErrorVal = DATASTORE_ERROR_UNEXPECTED_ERROR;
            status       = DATASTORE_STATUS_INT_ERROR;
            break;
        }
        /* End of Flash Emulator creation */

        /* Initialize the Unique Pointer class */
        myUniquePtrGen = new uniquePtr( property->addressRangeStart,
                                        property->addressRangeEnd );
        DATASTORE_ASSERT( myUniquePtrGen != NULL,
                          "new failed to create uniquePtr Object" );
        if( NULL == myUniquePtrGen ){
            status       = DATASTORE_STATUS_OUT_OF_MEM;
            lastErrorVal = DATASTORE_ERROR_OUT_OF_MEMORY;
            break;
        }

        /* Initialize the internal variables */
        int lDataStoreStartBlockID = 0;      /* Will calculate BlockID of first block of datastore */
        LPVOID lDataStoreStartPtr = NULL;
        /* This represents the block where the given offset starts */
        lDataStoreStartBlockID = flashDevice->getBlockIDFromAddress( (char*)flashDevice->getDeviceBaseAddress() +
                                                                     DATA_STORE_OFFSET );
        /* Datastore should start at the beginning of next block */
        lDataStoreStartBlockID = lDataStoreStartBlockID + 1;

        /* Now We have the address of the first block of datastore - Calculate byte offset from the
           beginning of the flash */
        lDataStoreStartPtr = flashDevice->getBlockAddressFromBlockID(lDataStoreStartBlockID);

        dataStoreStartByteOffset = (uint32)((char*)lDataStoreStartPtr - (char*)flashDevice->getDeviceBaseAddress());
        dataStoreEndByteOffset   = flashDevice->getDeviceSize() - 1;

        /* Now, that the datastore is ready, we need to register it with the global
           registration table which is used for address translations later */
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

        /* Now, scan the device for previously stored records and this should update
           my logPtr, erasePtr, cleanPtr */
        scanFlashDevice();

        lastErrorVal = DATASTORE_ERROR_NONE;
        status       = DATASTORE_STATUS_OK;
        state        = DATASTORE_STATE_READY;
    }while(0);
    return status;
}


/****************************************************************************
*
*  Function Name : cyclicDataRead
*
******************************************************************************/
/*!
*  \brief    Reads data from the flash
*
*  \param
*  \param
*  \return
*
******************************************************************************/
template<class T>
uint32 Data_Store<T>::cyclicDataRead( LPVOID buff,
                                      LPVOID currentLoc,
                                      uint32 numBytes )
{
    uint32 retVal = numBytes;
    uint32 offset = 0;

    if( NULL == currentLoc ||
        NULL == buff ||
        (char*)currentLoc < (char*)flashDevice->getDeviceBaseAddress() ||
        numBytes > flashDevice->getDeviceSize() ){

        DATASTORE_ASSERT( false, "Error!!!");
        assert(false);  //This should never happen!
        //return 0;
    }

    offset = (uint32)((char*)currentLoc - (char*)flashDevice->getDeviceBaseAddress());
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
                    (char*)flashDevice->getDeviceBaseAddress()+dataStoreStartByteOffset,
                    numBytes - byteTillEnd );
        }
    }while(0);
    return retVal;
}


/****************************************************************************
*
*  Function Name : cyclicDataRead
*
******************************************************************************/
/*!
*  \brief    Reads data from the flash
*
*  \param
*  \param
*  \return
*
******************************************************************************/
template<class T>
uint32 Data_Store<T>::cyclicDataWrite( LPVOID buff,
                                       LPVOID currentLoc,
                                       uint32 numBytes )
{
    uint32 retVal = numBytes;
    uint32 offset = 0;

    LPVOID targetLoc = (char*)flashDevice->getDeviceBaseAddress()+4156;

    if( NULL == currentLoc ||
        NULL == buff ||
        (char*)currentLoc < (char*)flashDevice->getDeviceBaseAddress() ||
        numBytes > flashDevice->getDeviceSize() ){

        DATASTORE_ASSERT( false, "Error!!!");
        assert(false);  //This should never happen!
        //return 0;
    }
#if 0
    if( targetLoc >= currentLoc &&
        targetLoc <= (char*)currentLoc+numBytes){
        int i;
        i = numBytes;
        i++;
    }
#endif
    offset = (uint32)((char*)currentLoc - (char*)flashDevice->getDeviceBaseAddress());
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
            flashDevice->writeRawData( buff,
                                       numBytes,
                                       currentLoc );
        }else{
            /* Need to read in pieces */
            int byteTillEnd = dataStoreEndByteOffset - offset + 1;

            /* Copy first piece */
            flashDevice->writeRawData( buff,
                                       byteTillEnd,
                                       currentLoc );
            /* Copy second piece */
            flashDevice->writeRawData( (char*)buff+byteTillEnd,
                                       numBytes - byteTillEnd,
                                       (char*)flashDevice->getDeviceBaseAddress()+dataStoreStartByteOffset );
        }
    }while(0);
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
template<class T>
DATASTORE_STATUS Data_Store<T>::compactLog()
{
    uint32 initialClearPtrBlockID;
    uint32 currClearPtrBlockID;
    DATASTORE_STATUS status = DATASTORE_STATUS_NOT_OK;
    RECORD_HEADER recHeader = { 0 };
    char *clearPtr = NULL;

    clearPtr = (char*)flashDevice->getDeviceBaseAddress()+ clearLogPointByOffset;

    initialClearPtrBlockID = currClearPtrBlockID = flashDevice->getBlockIDFromAddress( clearPtr );
    while(initialClearPtrBlockID == currClearPtrBlockID){
        /* Compact one block */
        cyclicDataRead( &recHeader,
                        clearPtr,
                        sizeof(RECORD_HEADER) );
        DATASTORE_ASSERT( 0 == recHeader.zero ,
                          "Invalid record header is read!" );
        if( 0 != recHeader.zero ){
            assert(false);  /* This should never happen */
            break;  //When assert is removed
        }
        if( 0 == recHeader.zero &&
            FLAG_RECORD_ACTIVE == recHeader.activeFlag ){
            /* Record that we are seeing is active, so we need to move it to the
               end of log - Can't use write method directly because current call
               could be coming from there :) */

            /* First mark the current location as inactive */
            recHeader.activeFlag = FLAG_RECORD_INACTIVE;
            recHeader.nextLink   = (char*)flashDevice->getDeviceBaseAddress() + \
                                        logPointByteOffset + \
                                        sizeof(RECORD_HEADER);
            cyclicDataWrite( (LPVOID)&recHeader,
                             clearPtr,
                             sizeof(RECORD_HEADER) );

            /* Old record is marked inactive, now migrate the record to the end of log */
            recHeader.activeFlag = FLAG_RECORD_ACTIVE;
            recHeader.nextLink   = (LPVOID)~0u;

            cyclicDataWrite( &recHeader,
                             ((char*)flashDevice->getDeviceBaseAddress()) + logPointByteOffset,
                             sizeof(RECORD_HEADER));
            incrementLogPointer(sizeof(RECORD_HEADER));
            incrementClearPoint(sizeof(RECORD_HEADER));
            clearPtr = (char*)flashDevice->getDeviceBaseAddress()+ clearLogPointByOffset;
            /* Now, clearPtr is pointing to the data_part of the record binging migrated */

            /* Copy data of the record being migrated */
            cyclicDataWrite( clearPtr,
                             ((char*)flashDevice->getDeviceBaseAddress()) + logPointByteOffset,
                             recHeader.size );

            incrementLogPointer(recHeader.size);
            incrementClearPoint(recHeader.size);

            clearPtr = (char*)flashDevice->getDeviceBaseAddress()+ clearLogPointByOffset;
        }else{
            /* Inactive record, just move the clear pointer to the next record */
            incrementClearPoint(recHeader.size + sizeof(RECORD_HEADER));
            clearPtr = (char*)flashDevice->getDeviceBaseAddress()+ clearLogPointByOffset;
        }
        /* Now, check the blockID where my clearPointer is now */
        currClearPtrBlockID = flashDevice->getBlockIDFromAddress(clearPtr);

        status = DATASTORE_STATUS_OK;   //In a loop :)
    }

    /* Now check if there is a complete block of inactive records that can be cleared */
    uint32 logPtrBlockID   = flashDevice->getBlockIDFromAddress( (char*)flashDevice->getDeviceBaseAddress() + logPointByteOffset);
    uint32 clearPtrBlockID = flashDevice->getBlockIDFromAddress( (char*)flashDevice->getDeviceBaseAddress() + clearLogPointByOffset);

    if(datastore_abs((int)clearPtrBlockID - (int)logPtrBlockID ) >= 2){
        /* There is a gap of atleast 1 block and initialClearPtrBlockID could be safely
           erased */
        {
            /* Temporary block */
            int logPtrBlockID;
            int clearPtrBlockID;

            logPtrBlockID = flashDevice->getBlockIDFromAddress((char*)flashDevice->getDeviceBaseAddress()+logPointByteOffset);
            //clearPtrBlockID = flashDevice->getBlockIDFromAddress((char*)flashDevice->getDeviceBaseAddress()+clearLogPointByOffset);
            if(logPtrBlockID == initialClearPtrBlockID){
                flashDevice->eraseBlock(initialClearPtrBlockID);
            }
        }

        flashDevice->eraseBlock(initialClearPtrBlockID);
        incrementErasePoint(flashDevice->getBlockSize(initialClearPtrBlockID));
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
template <class T>
Data_Store<T>::Data_Store(char *flashDeviceName)
{
    state = DATASTORE_STATE_UNINIT;
    DATASTORE_STATUS status;

    lastErrorVal = DATASTORE_ERROR_INVALID_PARAM;
    status = initDataStore( flashDeviceName,
                            &defaultProperty );

    state = (status == DATASTORE_STATUS_OK)?(DATASTORE_STATE_READY):(DATASTORE_STATE_INT_ERROR);
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
template <class T>
Data_Store<T>::Data_Store( char *flashDeviceName,
                           DATASTORE_PROPERTIES *property )
{
    state = DATASTORE_STATE_UNINIT;
    DATASTORE_STATUS status;

    lastErrorVal = DATASTORE_ERROR_INVALID_PARAM;
    status = initDataStore( flashDeviceName,
                            property );

    state = (status == DATASTORE_STATUS_OK)?(DATASTORE_STATE_READY):(DATASTORE_STATE_INT_ERROR);
}

#if 1
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
template<class T>
DATASTORE_STATUS Data_Store<T>::scanFlashDevice()
{/* Need to scan the physical device first */

    /* For now, jsut reset the flash and use it as a new device, will implement
       the persistance later */
	int numBlocks = 0;
#ifdef EMULATOR

    FLASH_PROPERTIES property;
    property.numClusters = flashDevice->getNumberofClusters();
    property.clusters  = (BLOCK_CLUSTER *)malloc(sizeof(BLOCK_CLUSTER)*property.numClusters);

    flashDevice->getDeviceProperties(&property);
    for(int index = 0; index < property.numClusters; index++){
        numBlocks += property.clusters[index].numBlocks;
    }

    for(int index = 0; index < numBlocks; index++){
        flashDevice->eraseBlock(index);
    }


#else
	const BlockDeviceInfo* deviceInfo;

	flashDevice->getDeviceProperties(deviceInfo);

	numBlocks = deviceInfo->Regions[0].NumBlocks;

	for(int index = 0; index < numBlocks; index++){
	    flashDevice->eraseBlock(index);
	}

#endif
	logPointByteOffset    = dataStoreStartByteOffset;
	clearLogPointByOffset = logPointByteOffset;  //for now
	erasePointByteOffset  = dataStoreEndByteOffset;

    return DATASTORE_STATUS_OK;
}
#else



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
template<class T>
uint32 Data_Store<T>::decideWhichOneisRecent(RECORD_HEADER tempHeader, RECORD_HEADER header)
{
    return((tempHeader.version+1)>header.version?tempHeader.version:header.version);
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
template<class T>
uint32 Data_Store<T>::returnScanAddress()
{
    RECORD_HEADER header;
    DATASTORE_ADDR_TBL_ENTRY entry;
    int returnAddress;
	char *basePtr;
	basePtr = (char *)flashDevice->getDeviceBaseAddress()+dataStoreStartByteOffset;
	basePtr -= sizeof(int);
	cyclicDataRead(&returnAddress, basePtr, sizeof(int));
    return returnAddress;
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
template<class T>
DATASTORE_STATUS Data_Store<T>::scanFlashDevice()
{
#if 1
    /* Need to scan the physical device first */
    DATASTORE_ADDR_TBL_ENTRY entry;
    DATASTORE_STATUS status = DATASTORE_STATUS_RECORD_ALREADY_EXISTS;
    int *scanFlashData;
    int index = 0;
    int numBlocks = 0;
    LPVOID currentLink;
    uint32 basePtr;
    RECORD_HEADER header;
    FLASH_PROPERTIES property;
    property.numClusters = flashDevice->getNumberofClusters();
    property.clusters  = (BLOCK_CLUSTER *)malloc(sizeof(BLOCK_CLUSTER)*property.numClusters);
    flashDevice->getDeviceProperties(&property);

    basePtr = returnScanAddress();
	//basePtr += DATA_STORE_OFFSET;
    while(basePtr < flashDevice->getDeviceSize()){     /*While loop 1*/
		memcpy( &header, basePtr+(char *)flashDevice->getDeviceBaseAddress(), sizeof(RECORD_HEADER));
		/*get the start and end address of the areas that has been erased (all 1s)*/
		if( header.zero == 1 )
		{
			//logPointByteOffset = (DWORD)flashDevice->getBlockAddressFromBlockID(header.recordID);
			logPointByteOffset = ;
			unsigned char *localAddr;
			localAddr = (unsigned char *)(basePtr+(char *)flashDevice->getDeviceBaseAddress());
			while(*localAddr == 0xFFu)
			{
				localAddr++;
				if(localAddr == (unsigned char *)flashDevice->getDeviceSize())
					localAddr = (unsigned char *)flashDevice->getDeviceBaseAddress(); /*should this be getAddress(deviceID)*/
				else if(localAddr == (unsigned char *)logPointByteOffset)
					break;
			}
			erasePointByteOffset = (DWORD)localAddr;
		}
        if( header.activeFlag == FLAG_RECORD_ACTIVE &&
			header.zero == 0 ){
            entry.allocationSize = header.size;
            entry.currentLoc     = basePtr+(char *)flashDevice->getDeviceBaseAddress()+sizeof(RECORD_HEADER);
            entry.givenPtr       = getUniquePtr(header.size);
            entry.recordID       = header.recordID;

            /* Change the condition to check for the STATUS duplicate recordID */
            status = addressTable.addEntry(&entry);
            if(DATASTORE_STATUS_RECORD_ALREADY_EXISTS == status){  /*If loop 2*/
                int oldEntryVer;
                int curEntryVer;
                Flash_Memory<char> *temp;
                RECORD_HEADER tempHeader;

                temp = (Flash_Memory<char> *)addressTable.getCurrentLoc(header.recordID);
                memcpy( &tempHeader,
                        (char*)temp-sizeof(RECORD_HEADER),
                        sizeof(RECORD_HEADER) );

                oldEntryVer = tempHeader.version;
                curEntryVer = header.version;
                if(decideWhichOneisRecent(tempHeader, header) == curEntryVer){       /*If loop 3*/
                    deleteRecord(header.recordID);
                    addressTable.addEntry(&entry);
                }else{
                    tempHeader.activeFlag = FLAG_RECORD_INACTIVE;
                    flashDevice->writeRawData( &tempHeader,
                                               sizeof(RECORD_HEADER),
                                               (char*)temp-sizeof(RECORD_HEADER) );
                }       /*End of if loop 3*/
            }else{
                DATASTORE_ASSERT( false, "Address Table failed!");
            }       /*End of if loop 2*/
        }
        basePtr += header.size+ sizeof(RECORD_HEADER);
    }       /*End of while loop 1*/
#endif
	logPointByteOffset = DATA_STORE_OFFSET;
	erasePointByteOffset = flashDevice->getDeviceSize() -1;
    return DATASTORE_STATUS_OK;
}       /*End of function*/
#endif


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
template <class T>
DATASTORE_STATUS Data_Store<T>::deleteRecord(RECORD_ID id)
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
        addressTable.removeEntry(id);
        status = DATASTORE_STATUS_OK;
    }while(0);
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
template<class T>
LPVOID Data_Store<T>::createRecord( RECORD_ID recordID, uint32 numBytes )
{
    RECORD_HEADER header;
    LPVOID lGivenPtr = NULL;
    LPVOID allocLoc  = NULL;
    lastErrorVal = DATASTORE_ERROR_NONE;

    do{
        if(NULL != addressTable.getGivenAddress(recordID)){
            /* This record id is already present in the flash. So, can't proceed */
            lastErrorVal = DATASTORE_ERROR_RECORD_ID_ALREADY_EXISTS;
            break;
        }

        /* Generate an unique pointer and register it with the address_table */
        lGivenPtr = myUniquePtrGen->getUniquePtr(numBytes);
        allocLoc  = createAllocation( recordID, lGivenPtr, numBytes );
        if( NULL == allocLoc ){
            break;
        }
        /* Address table will be updated by createAllocation, so need not to bother here */
    }while(0);
    return lGivenPtr;
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
template<class T>
uint32 Data_Store<T>::readRawData(LPVOID src, void *data, uint32 numBytes)
{
    uint32 lNumBytes = 0;
    LPVOID curLoc = NULL;

    lastErrorVal = DATASTORE_ERROR_NONE;
    do{
        if(NULL == src || NULL == data){
            lastErrorVal = DATASTORE_ERROR_INVALID_PARAM;
            lNumBytes = numBytes = 0;
            break;
        }
        if( NULL == (curLoc = addressTable.getCurrentLoc(src))){
            lastErrorVal = DATASTORE_ERROR_INVALID_GIVEN_ADDR;
            lNumBytes = numBytes = 0;
            break;
        }

        lNumBytes = addressTable.getMaxWriteSize(src);
        lNumBytes = (numBytes < lNumBytes)?numBytes:lNumBytes;

        cyclicDataRead( data, curLoc, lNumBytes);
    }while(0);
    return lNumBytes;
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
template<class T>
uint32 Data_Store<T>::readData(LPVOID src, T* data, uint32 count)
{
    return readRawData( src, data, count*sizeof(T));
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
template<class T>
uint32 Data_Store<T>::writeData( LPVOID dest, T *data, uint32 count )
{
    return writeRawData( dest, data, count*sizeof(T));
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
template<class T>
uint32 Data_Store<T>::writeRawData(LPVOID dest, void* data, uint32 numBytes)
{
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

        recWriteStartAddr = addressTable.getCurrentLoc(dest);
        numBytesWritten   = addressTable.getMaxWriteSize(dest);    /* Number of bytes till the end of record */
        /* Minimum of these two sizes */
        numBytesWritten   = (numBytes < numBytesWritten)?numBytes:numBytesWritten;

        recWriteEndAddr   = (char*)recWriteStartAddr + numBytesWritten - 1;     /* corresponding End address on flash */
        recEndAddr        = (char*)recBaseAddr + recordSize - 1;                /* Last byte address of the record on flash */

        /* Now check if there is a overwrite possiblility */
        isOverWriteDetected = detectOverWrite( recWriteStartAddr,
                                               data,
                                               numBytesWritten,
                                               &overWriteStartOffset );
        if(false == isOverWriteDetected){
            /* No overwrites are detected, so its safe to just write data */
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
            LPVOID newRecWriteEndAddr   = NULL;
            LPVOID newRecEndAddr        = NULL;

            newRecBaseAddr = createAllocation( recordID, lGivenPtr, recordSize );
            if( NULL == newRecBaseAddr ){
                /* Allocation failed! */
                lastErrorVal = DATASTORE_ERROR_OUT_OF_FLASH_MEMORY;
                numBytesWritten = 0;
                break;
            }

            /* createAllocation returns the pointer to the header location */
            newRecBaseAddr       = (char*)newRecBaseAddr + sizeof(RECORD_HEADER);

            newRecWriteStartAddr = addressTable.getCurrentLoc(dest);
            newRecWriteEndAddr   = (char*)newRecWriteStartAddr + numBytesWritten;
            newRecEndAddr        = (char*)newRecBaseAddr + addressTable.getMaxWriteSize(newRecBaseAddr) - 1;

            /* Temporary pointers that point to the write location and length at each step below */
            LPVOID lCurrentWriteLoc = newRecBaseAddr;
            uint32 lCurrentWriteLen = (uint32)((char*)newRecWriteStartAddr - (char*)newRecBaseAddr);

            /* Write region-1 */
            cyclicDataWrite( recBaseAddr,
                             lCurrentWriteLoc,
                             lCurrentWriteLen );

            /* Move the current location pointer */
            lCurrentWriteLoc = (char*)lCurrentWriteLoc + lCurrentWriteLen;
            lCurrentWriteLen = numBytesWritten;

            /* Write region-2 */
            cyclicDataWrite( data,
                             lCurrentWriteLoc,
                             lCurrentWriteLen );

            /* Move the current location pointer */
            lCurrentWriteLoc = (char*)lCurrentWriteLoc + lCurrentWriteLen;
            lCurrentWriteLen = (uint32)((char*)recEndAddr - (char*)recWriteEndAddr);

            /* Write region-3 */
            cyclicDataWrite( (char*)recWriteEndAddr+1,
                             lCurrentWriteLoc,
                             lCurrentWriteLen );

            /* Congrats, thats all you had to do, write completed successfully :) */
        }
    }while(0);
    return numBytesWritten;
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
template<class T>
DATASTORE_ERROR Data_Store<T>::getLastError()
{
    return lastErrorVal;
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
template<class T>
Data_Store<T>::~Data_Store()
{
    DATASTORE_REG_RemoveEntry(this);
    delete flashDevice;
    delete myUniquePtrGen;
}

#endif
