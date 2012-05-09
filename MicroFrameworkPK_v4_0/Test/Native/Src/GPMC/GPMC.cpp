#include "GPMC.h"

// tries to access a region in nor flash
BOOL GPMCTest::Execute()
{

	int *tempMemory = (int *) 0x08000000;

	*tempMemory = 0x4;
	*tempMemory += 0x4;

}
