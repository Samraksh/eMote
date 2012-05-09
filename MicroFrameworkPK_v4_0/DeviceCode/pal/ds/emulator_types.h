#ifndef __EMULATOR_TYPES_H__
#define __EMULATOR_TYPES_H__

/* Just for readability */
#define BITWISE_AND &

#define EMULATOR_DO_VERIFIED_WRITE
#define ENABLE_ASSERTS

/* Breaking protocol of declaration for ease of use :) */
void emulatorDebugLogs(char *fname, int lineNum, char *funcName, int cricality, char *message);      


#define EMULATOR_DBG(messageStr)    emulatorDebugLogs(__FILE__, __LINE__, __FUNCTION__, EMULATOR_SEVERITY_DEBUG, messageStr)

#ifdef  ENABLE_ASSERTS
#define EMULATOR_ASSERT( condition, messageString )   do{if(!(condition)){EMULATOR_DBG(messageString);}}while(0);
#else
#define EMULATOR_ASSERT( condition, messageString )   /* Disabling ASSERT - Its better not to terminate at error conditions sometimes */
#endif


/* Enumeration of all the possible errors that can occur in the emulator */
typedef enum _emulator_status
{
    EMULATOR_STATUS_OK,
    EMULATOR_STATUS_NOT_OK
    /* Add more errors here */
}EMULATOR_STATUS;


/* Describes a cluster of blocks, used in FLASH_PROPERTIES, a cluster is a group 
   of blocks that have 
   This is to support the requirement of support flashes with blocks of multiple
   sizes */
typedef struct _block_cluster
{
    int blockSize;  /* Block size in this cluster */
    int numBlocks;  /* Number of blocks in this cluster */
}BLOCK_CLUSTER;


/* Properties of the flash device that needs to be simulated */
typedef struct _flash_properties
{
    BLOCK_CLUSTER *clusters;      /* Number of clusters */
    int           numClusters;    /* Number of elements in Array BLOCK_CLUSTER *clusters */      
    /* Add any more properties here */
}FLASH_PROPERTIES;


/* Block statistics, now just has number of erases, add any more required 
   properties into this */
typedef struct _flash_block_statistics
{
    int numErases;           /* Number of erases done to given Block */
    int numWritesAfterErase; /* Number of writes after previous erase */

    /* NOT AVAILABLE YET!!! */
    int totalWrites;         /* Absolute total number of writes since creating this flash file */
    /* Do we need any more statistics for each block apart from num of erases??? */
}FLASH_BLOCK_STATISTICS;


/* Structure used for blockID and Address Conversion table. 
   This conversion can be don't without a lookup table also, but its efficient this way */
typedef struct _addr_mapping
{
    int  blockStartID, blockEndID;
    char *blockStartAddr, *blockEndAddr;    /* Char Ptr for easy pointer arithmetic */
    int blockSize;
}ADDR_MAPPING;

#endif

