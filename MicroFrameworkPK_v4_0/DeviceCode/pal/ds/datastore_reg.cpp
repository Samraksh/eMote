
#include <vector>

#include "types.h"

typedef void* LPVOID;

#include "datastore_int.h"
#include "datastore_reg.h"



using namespace std;

#define MAX(a, b) (((a) > (b))?(a):(b))
#define MIN(a, b) (((a) < (b))?(a):(b))

/* MACRO function to check overlap between intervals (a<->b) and (c<->d) */
/* Algorithm : http://stackoverflow.com/questions/4879315/what-is-a-tidy-algorithm-to-find-overlapping-intervals */
#define CHECK_OVERLAP(a, b, c, d)   ((MAX((a),(b)) >= MIN((c),(d))) && (MIN((a),(b)) <= MAX((c), (d))))


static vector<DATASTORE_REG_ENTRY> fDataStoreReg;

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
static bool DATASTORE_REG_checkOverLap(DATASTORE_REG_ENTRY *entry)
{
    /* Check for any overlaps in the address ranges
       Algorithm : http://stackoverflow.com/questions/4879315/what-is-a-tidy-algorithm-to-find-overlapping-intervals
    */
    bool retVal = false;
    if(entry == NULL) return true;  /* Garbage input, garbage output :) */
    for(int index = 0; index < fDataStoreReg.size(); index++){
        if( true == (retVal = CHECK_OVERLAP( entry->addrRangeStart, entry->addrRangeEnd, \
                                             fDataStoreReg[index].addrRangeStart, fDataStoreReg[index].addrRangeEnd)) ){
            /* found an overlap, awesome, return now with that status */
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
DATASTORE_STATUS DATASTORE_REG_CreateEntry(DATASTORE_REG_ENTRY *entry)
{
    DATASTORE_STATUS status = DATASTORE_STATUS_INVALID_PARAM;
    do{
        if(NULL == entry){
            status = DATASTORE_STATUS_INVALID_PARAM;
            break;
        }
        if( true == DATASTORE_REG_checkOverLap(entry)){
            status = DATASTORE_STATUS_OVERLAPPING_ADDRESS_SPACE;
            break;
        }

        /* I think, its now safe to add the entry into the registry */
        fDataStoreReg.push_back(*entry);
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
*  \brief    Look up the datastore handle based on the address
*
*  \param
*  \param
*  \return
*
******************************************************************************/
LPVOID DATASTORE_REG_LookupDataStoreHandle(LPVOID addr)
{
    LPVOID dataStoreHandle = NULL;
    do{
        for(int index = 0; index < fDataStoreReg.size(); index++){
            if(true == CHECK_OVERLAP( addr, addr, \
                                      fDataStoreReg[index].addrRangeStart, fDataStoreReg[index].addrRangeEnd)){
                dataStoreHandle = fDataStoreReg[index].dataStoreHandle;
                break;
            }
        }
    }while(0);
    return dataStoreHandle;
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
DATASTORE_STATUS DATASTORE_REG_RemoveEntry(LPVOID dataStoreHandle)
{
    DATASTORE_STATUS status = DATASTORE_STATUS_NOT_FOUND;
    do{
        for(int index = 0; index < fDataStoreReg.size(); index++){
            if( dataStoreHandle == fDataStoreReg[index].dataStoreHandle ){
                /* Delete this entry */
                fDataStoreReg.erase(fDataStoreReg.begin()+index);
                status = DATASTORE_STATUS_OK;
                break;
            }
        }
    }while(0);
    return status;
}
