#ifndef __Address_Map_H__
#define __Address_Map_H__

typedef struct{
    void          *inputBaseAddr;
    unsigned int  addrSpaceLen;
    void          *outputBaseAddr;        
}ADDRESS_MAP;


/* Function prototypes */
bool  addAddressMapping    (ADDRESS_MAP *addrMapping);
bool  removeAddressMapping (void *addr);
void* lookUpAddress        (void *inputAddr);

#endif

