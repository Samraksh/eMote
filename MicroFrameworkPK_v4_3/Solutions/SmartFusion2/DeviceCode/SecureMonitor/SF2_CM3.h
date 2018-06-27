#ifndef _SF2_CM3_H_
#define _SF2_CM3_H_

//Platform/chip specific definitions.

//Embedded sram RAM
#define ERAM_ORIGIN 0x20000000
#define ERAM_SIZE 0x18000 //96KB
#define ERAM_SIZE_POWER 17 //This 2^17 is 128KB, but has a power of 2 will cover entier RAM
//#define ERAM_OFFSET 0x0

//Embedded Flash
#define EFLASH_ORIGIN 0x10000000 //On SF2 the internal flash actually starts at 0x8000000, but it gets mapped to 0x1000000 at run time
#define EFLASH_SIZE 0x100000 //1MB
#define EFLASH_SIZE_POWER 20 //2^20 is 1MB

//#define FLASH_OFFSET 0x0

#endif _SF2_CM3_H_
