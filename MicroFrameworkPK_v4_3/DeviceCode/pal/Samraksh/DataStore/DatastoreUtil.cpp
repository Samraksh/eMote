#include "Samraksh/DataStore/Types.h"

typedef void* LPVOID;

#include "Samraksh/DataStore/DatastoreInt.h"



/*
	Constructor
*/
uniquePtr::uniquePtr(LPVOID lStart, LPVOID lEnd)
{
	lastVal = start = lStart;
	end   	= lEnd;
}


/*
	This function will get a unique pointer in the given range
*/
LPVOID uniquePtr::getUniquePtr(uint32 numBytes)
{
	LPVOID retVal = lastVal;
	lastVal = (char*)lastVal + numBytes;
	/* Its not as simple as this, but for now, works bug-free */
	return retVal;
}



int datastore_abs(int val){
    if(val < 0){
        val = val * (-1);
    }
    return val;
}

