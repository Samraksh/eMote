////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//-- Stream --//

#define  STREAM__OUT       STREAM__SERIAL //STREAM__SERIAL                = 0x10

//--  RamTest --// Not Executing RAM TEST as of now, the NativeSample generated it as
// a commented block

#define  BUS_WIDTH         0x00008000
#define  RAMTestBase       0x08400000
#define  RAMTestSize       0x00010000
#define  ENDIANESS         LE_ENDIAN //Defined as an ENUM

//--  Serial Port  --//

#define  COMTestPort       COM1

//--  GPIO --//
    
#define  GPIOTestPin       19 //#define GPIO_PIN_NONE               0xFFFFFFFF

//-- SPI --//

#define  SPIChipSelect     GPIO_PIN_NONE //#define GPIO_PIN_NONE               0xFFFFFFFF
#define  SPIModule         GPIO_PIN_NONE //#define GPIO_PIN_NONE               0xFFFFFFFF

//-- Timer --//

#define  DisplayInterval    5
#define  TimerDuration      30

//--//

