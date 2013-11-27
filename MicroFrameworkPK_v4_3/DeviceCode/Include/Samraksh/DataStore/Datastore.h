#ifndef _DATASTORE_H_INCLUDED_
#define _DATASTORE_H_INCLUDED_

//#include <BlockStorage_decl.h>
#include <Samraksh/DataStore/types.h>
#include <Samraksh/DataStore/AddressTable.h>
//#include <Samraksh/DataStore/FlashDevice.h>
//#include <Samraksh/DataStore/DatastoreInt.h>
#include <Samraksh/DataStore/DatastoreReg.h>

#define DEBUG_DATASTORE

#ifdef DEBUG_DATASTORE
#define PRINT_DEBUG(x) hal_printf(x)
#else
#define PRINT_DEBUG(x)
#endif

//typedefs
typedef unsigned int RECORD_ID;


/* Constants */
/* Threshold that needs to be always maintained*/
#define MIN_SPACE_REQUIRED_FOR_COMPACTION	    (1024 * 128)

#define ENABLE_TEST

/* Defines where the data-store begins in the flash */
#define DATA_STORE_OFFSET                       (8763)




#define FLAG_RECORD_ACTIVE      (1)     /* Because of flash properties, this has to be 1 */
#define FLAG_RECORD_INACTIVE    (0)

#ifdef DATASTORE_DEBUG_DEFINED
#define DATASTORE_ASSERT(condition, messageStr)     do{ASSERT(condition)}while(0);
#else
#define DATASTORE_ASSERT(condition, messageStr)
#endif

#define SKIP_TO_NEXT_SECTOR_FLAG    (unsigned char)(0xAA)    /* Header.zero will be kept 1 here intentially */

typedef void* LPVOID;

typedef enum _datastore_state
{
    DATASTORE_STATE_UNINIT,
    DATASTORE_STATE_READY,
    DATASTORE_STATE_INT_ERROR
}DATASTORE_STATE;


typedef enum _flashdevice_state
{
    FLASHDEVICE_STATE_UNINITIALIZED,
    FLASHDEVICE_STATE_READY,
    FLASHDEVICE_STATE_FAILED_TO_LOAD,
    FLASHDEVICE_STATE_CLOSED
    /* Do we need more states here? */
}FLASHDEVICE_STATE;


/* For the persistence function */
typedef enum _persistence_direction
{
	GO_LEFT,
	GO_RIGHT
}PERSISTENCE_DIRECTION;



/* Describes a cluster of blocks, used in DATASTORE_PROPERTIES, a cluster is a group
   of blocks that have different block sizes.
   This is to support the requirement of support flashes with blocks of multiple
   sizes */
typedef struct _datastore_cluster
{
    uint32 blockSize;  /* Block size in this cluster */
    uint32 numBlocks;  /* Number of blocks in this cluster */
}DATASTORE_CLUSTER;


/* Properties of the flash device that needs to be simulated */
typedef struct _datastore_properties
{
    DATASTORE_CLUSTER *clusters;      /* Number of clusters */
    int               numClusters;    /* Number of elements in Array BLOCK_CLUSTER *clusters */
    /* Till here its identical to the flash_properties */

    LPVOID            addressRangeStart;
    LPVOID            addressRangeEnd;
}DATASTORE_PROPERTIES;



typedef struct _record_header
{
    char zero:1;
    uint32 version:2;
    uint32 activeFlag:1;
    uint32 size:28;   /* sizeof(version+isActive+size) = 32bits */

    uint32 recordID;
    void*  nextLink;
}RECORD_HEADER;


class uniquePtr
{
private:
	LPVOID start, end;
	LPVOID lastVal;
public:
	uniquePtr()
	{
		start = (LPVOID) 0x0;
		end = (LPVOID) (1 << 32);
		lastVal = (LPVOID) 1;
	}

	void init()
	{
		start = (LPVOID) 0x0;
		end = (LPVOID) (1 << 32);
		lastVal = (LPVOID) 1;
	}

	uniquePtr(LPVOID start, LPVOID end);
	LPVOID getUniquePtr(uint32 numBytes);
};

class hiddenPersistanceMetaData
{/* class that handles storing and managing  */
private:
    uint32 curLoc;

public:
    /*  */
    hiddenPersistanceMetaData();
    DATASTORE_STATUS storeClearPtrOffset();
};

/* Prototypes */
int datastore_abs(int val);

/* Default properties of device */
static DATASTORE_CLUSTER cluster[] = { { 512,        1024  },
                                       { 4*1024,     256 },
                                       { 10*1024,    128 }};

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



///////////////////////////////////Main Data Store Class definition

// Data store class
class Data_Store
{
private:
    /* Flash device */
    //FlashDevice flashDevice;
	BlockStorageDevice* blockStorageDevice;
	const BlockDeviceInfo*  blockDeviceInformation;

	///// AnanthAtSamraksh - Remove below
	//BlockRegionInfo blockRegionInfo;


    /* Current state of the device */
    DATASTORE_STATE state;
    FLASHDEVICE_STATE flashdevice_state;

    /* Last error status - Stores any error that occured in previous API
       This value can be read by the Application using getLastErrorValue()
    */
    DATASTORE_ERROR lastErrorVal;

    /* This is to find the log, error and erase pointers */
#if ENABLE_PERSISTENCE
    PERSISTENCE_DIRECTION per_dir;
#endif
    /* Internal pointers that maintain the circular buffer locations */
    uint32 logPointByteOffset;
    uint32 erasePointByteOffset;
    uint32 clearLogPointByOffset;   /* Tells where the next record that can be checked for clearing is */

    /* Offsets telling start and end of flash data region - For ease of use */
    uint32 dataStoreStartByteOffset;
    uint32 dataStoreEndByteOffset;
    uint32 dataStoreDeviceSize;

    /* Address translation table */
    class DATASTORE_AddrTable addressTable;

    /* Unique Pointer generator */
    uniquePtr myUniquePtrGen;

    /* Offset where clearPtrOffset needs to be written for persistance */
       /* Not sure if its a good idea to abstract out this as a separate small class
          or keep this info here like this! - Chethan */
    UINT32 hiddenClearPtrOffsetStore;

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

    /* Helper function to increment pointers so that it doesn't run out of circular buffer */
    LPVOID incrementPointer(LPVOID inputPtr, int amount);

    /* Helper function to increment pointers so that it doesn't run out of circular buffer */
    int calculateNumBytes(LPVOID fromAddr, LPVOID toAddr);

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


    /* Function that stores the recent info of the clearLogPointByOffset in hidden region */
    DATASTORE_STATUS storeClearPtrOffset();

    /* Helper function for scanFLashDevice - used for getting the erase, log and clear pointers */
    LPVOID traversePointer(PERSISTENCE_DIRECTION per_dir, char* address);

    	/* Helper function for scanFLashDevice - used for scanning through the sectors */
    LPVOID readPointers();

   	/* Helper function for scanFLashDevice - used for scanning through the sectors */
   	int readRecordinBlock(int blockID);

    /* Helper function for scanFlashDevice */
    uint32 decideWhichOneisRecent(RECORD_HEADER tempHeader, RECORD_HEADER header);

    /* Fetches the the start point for scan */
    uint32 returnScanAddress();

    /* Scan the flash device and load all the records in the Flash Device
       Could use this at bootup to load all the records in the flash */
    LPVOID scanFlashDevice();

    /* create a dummy filler allocation in the datastore to make sure that records
           don't overlap between sectors */
    bool createDummyAllocation(int nextAllocationSize);

    //static Data_Store<T> instance;

    //static BOOL instanceFlag;
    static BOOL initialized;

public:
    // privatising constructors to ensure there is only one instance of data store in the
    // system
    Data_Store()
    {
    	Data_Store("NOR");
    }

    /* Helper function for the addressTable. Gets a table, copies it to a new table and returns it */
    	//LPVOID copyAddressTable(vector<DATASTORE_ADDR_TBL_ENTRY> *table);
    	LPVOID copyAddressTable(myVector *table);


        /* Load with default settings */
     Data_Store( char *flashDeviceName );

          /* Create a Datastore with specific property - Only used with emulator */
     Data_Store( char *flashDeviceName,
                      DATASTORE_PROPERTIES *property );


     DeviceStatus init();


#ifdef ENABLE_PERSISTENCE
	/* Helper function for the addressTable. Gets a table, copies it to a new table and returns it */
	LPVOID copyAddressTable(vector<DATASTORE_ADDR_TBL_ENTRY> *table);
#endif
    /* Create a record in the data store */
    LPVOID createRecord( RECORD_ID id, uint32 numBytes );

    /* Delete record */
    DATASTORE_STATUS deleteRecord( RECORD_ID id );

    /* Get base address for given Record_id */
    LPVOID getAddress( RECORD_ID id );

    /* Symmetric API to lookup ID using PTR */
    RECORD_ID getRecordID(LPVOID givenPtr);

    /* Write data to the store */
    uint32 writeData( LPVOID dest, uint32 *data, uint32 count );

    /* Write raw data - Can be of any word-size */
    uint32 writeRawData(LPVOID dest, void* data, uint32 numBytes);

    uint32 readData(LPVOID src, uint32* data, uint32 count);

    /* Read raw data */
    uint32 readRawData(LPVOID src, void *data, uint32 numBytes);

    /* Returns the error code of any error in the previous call */
    DATASTORE_ERROR getLastError();


	/* Function that returns the current value of the Log point */
	uint32 returnLogPoint();

	/* Function that returns the current value of the Clear point */
	uint32 returnClearPoint();

	/* Function that returns the current value of the Erase point */
	uint32 returnErasePoint();

    /* Destructor */
    ~Data_Store();
};

/*
template<class T>
Data_Store<T> Data_Store<T>::instance("NOR");
*/

BOOL Data_Store::initialized = FALSE;


#endif
