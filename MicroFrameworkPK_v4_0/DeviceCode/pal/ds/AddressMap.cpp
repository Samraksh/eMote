/* 
    NOTE :: Because of the heavy usage of the look-up function, it has to be optimized 
            as much as possible to avoid any bottleneck for the application performace.
*/
#include <iostream>
#include <vector>

#include "AddressMap.h"

using namespace std;

static bool isTableInitialized = false;
static vector<ADDRESS_MAP> addrLookUpTbl;

/****************************************************************************
*
*  Function Name : initializeAddressLookUpTBL
*
******************************************************************************/
/*!
*  \brief          Internal function to create lookup table.
*  \param          None 
*  \return         true/false
*    
******************************************************************************/
static bool initializeAddressLookUpTBL()
{
    return true;
}


/****************************************************************************
*
*  Function Name : addAddressMapping
*
******************************************************************************/
/*!
*  \brief          Add new mapping to the table.
*  \param          addressMapping 
*  \return         true/false
*    
******************************************************************************/
bool addAddressMapping(ADDRESS_MAP *addrMapping)
{
    bool retVal = false;
    if( false == isTableInitialized ){
        isTableInitialized = initializeAddressLookUpTBL();
    }
    do{
        if(false == isTableInitialized){
            break;
        }
        addrLookUpTbl.push_back(*addrMapping);
		retVal = true;
    }while(0);
    return retVal;
}


/****************************************************************************
*
*  Function Name : removeAddressMapping
*
******************************************************************************/
/*!
*  \brief          Removes mapping which contains the given address from lookup table.
*  \param          None 
*  \return         true/false
*    
******************************************************************************/
bool removeAddressMapping(void *addr)
{
    bool retVal = false;

    for(register int index = 0; index < addrLookUpTbl.size(); index++){
		void *lStartAddr = (char*)(addrLookUpTbl[index].inputBaseAddr);
		void *lEndAddr   = (char*)(addrLookUpTbl[index].inputBaseAddr) + addrLookUpTbl[index].addrSpaceLen;
        
		if( (lStartAddr <= addr) && 
			(lEndAddr >= addr) ){
				addrLookUpTbl.erase(addrLookUpTbl.begin()+index);
				retVal = true;
				break;
		}
	}
	return retVal;
}

/****************************************************************************
*
*  Function Name : lookUpAddress
*
******************************************************************************/
/*!
*  \brief          Lookup the writeable address from read-only address.
*  \param          inputAddr 
*  \return         output address
*    
******************************************************************************/
void *lookUpAddress(void *inputAddr)
{
    void *retAddr = NULL;
    for(register int index = 0; index < addrLookUpTbl.size(); index++){
		int len = addrLookUpTbl[index].addrSpaceLen;
		void *lStartAddr = (char*)(addrLookUpTbl[index].inputBaseAddr);
		void *lEndAddr   = (char*)(addrLookUpTbl[index].inputBaseAddr) + addrLookUpTbl[index].addrSpaceLen;

		if( (lStartAddr <= inputAddr) && 
			(lEndAddr >= inputAddr) ){
			/* This is what I was looking for :) */	
			char *temp = (char*)(addrLookUpTbl[index].inputBaseAddr);
			char *retAddrBaseAddr = (char*)(addrLookUpTbl[index].outputBaseAddr);

			retAddr = (retAddrBaseAddr + ((char*)inputAddr - temp));
			break;
		}
    }
    return retAddr;
}

