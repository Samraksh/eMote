#include "RandomUtils.h"

#include <Samraksh/MAC/OMAC/SeedGenerator.h>

SeedGenerator rng;
UINT16 Seed = 0;

BOOL GetCPUSerial(UINT8 * ptr, UINT16 num_of_bytes ){
	UINT32 Device_Serial0;UINT32 Device_Serial1; UINT32 Device_Serial2;
	Device_Serial0 = *(UINT32*)(0x1FFFF7E8);
	Device_Serial1 = *(UINT32*)(0x1FFFF7EC);
	Device_Serial2 = *(UINT32*)(0x1FFFF7F0);
	if(num_of_bytes==12){
	    ptr[0] = (UINT8)(Device_Serial0 & 0x000000FF);
	    ptr[1] = (UINT8)((Device_Serial0 & 0x0000FF00) >> 8);
	    ptr[2] = (UINT8)((Device_Serial0 & 0x00FF0000) >> 16);
	    ptr[3] = (UINT8)((Device_Serial0 & 0xFF000000) >> 24);

	    ptr[4] = (UINT8)(Device_Serial1 & 0x000000FF);
	    ptr[5] = (UINT8)((Device_Serial1 & 0x0000FF00) >> 8);
	    ptr[6] = (UINT8)((Device_Serial1 & 0x00FF0000) >> 16);
	    ptr[7] = (UINT8)((Device_Serial1 & 0xFF000000) >> 24);

	    ptr[8] = (UINT8)(Device_Serial2 & 0x000000FF);
	    ptr[9] = (UINT8)((Device_Serial2 & 0x0000FF00) >> 8);
	    ptr[10] = (UINT8)((Device_Serial2 & 0x00FF0000) >> 16);
	    ptr[11] = (UINT8)((Device_Serial2 & 0xFF000000) >> 24);
	    return TRUE;
	}
	else return FALSE;
}

UINT16 GetNodeID(){
	UINT8 cpuserial[12];
	GetCPUSerial(cpuserial,12);
	UINT16 tempNum=0;
	UINT16 * temp = (UINT16 *) cpuserial;
	for (int i=0; i< 6; i++){
		tempNum=tempNum ^ temp[i]; //XOR 72-bit number to generate 16-bit hash
	}
	return tempNum;
}

//Very very bad implementation of random byte stream
int GetRandomBytes (unsigned char *buf, UINT16 length){
	UINT16 loopSize = length/2;
	UINT16 *result = (UINT16*) buf;

	UINT16 nodeID = GetNodeID();
	if(Seed==0){
		Seed = nodeID;
	}

	for (UINT16 i=0; i<loopSize; i++){
		result[i]= rng.Rand(&Seed,nodeID);
	}
	if (length % 2 != 0) {
		buf[length-1]= (UINT8)rng.Rand(&Seed,nodeID);
	}
	return length;
}

int GetRandomSeed (unsigned char *buf, UINT16 length){
	return GetRandomBytes(buf,length);
}

