/*
 *
 *
 *
 *
 *
 *
 */


#include<tinyhal.h>



BOOL copyFlashProperties( FLASH_PROPERTIES *toProperties, const BlockDeviceInfo *fromProperties )
{
    BOOL retVal = false;

    const BlockRegionInfo* Regions;

    do
    {
        if(toProperties == NULL || fromProperties == NULL){
            retVal = false;
            break;
        }

        toProperties->numClusters = fromProperties->NumRegions;

        retVal = true;
    }while(0);
    return retVal;
}
