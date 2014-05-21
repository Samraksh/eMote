////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) The Samraksh Company.  All rights reserved.
// This is an auto generated file by the test create tool
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define __regtest

#include <tinyhal.h>
#include "RadioLRTest.h"
#include <Samraksh/RoutingEngine.h>

extern RadioLRTest testObject;

void Keep_Linker_Happy() {
	BootstrapCode();
	BootEntry();
}


HAL_DECLARE_NULL_HEAP();

void PostManagedEvent( UINT8 category, UINT8 subCategory, UINT16 data1, UINT32 data2 )
{
}

void ApplicationEntryPoint()
{
    BOOL result;
    

    do
    {
    	if(!testObject.Initialize(0, 100))
    	{
    		testObject.DisplayStats(FALSE,"Radio Initialization failed", NULL, NULL);
    		break;
    	}
    	
    	testObject.Execute(LEVEL_0A);

    } while(FALSE); // run only once!

    while(TRUE);
}
