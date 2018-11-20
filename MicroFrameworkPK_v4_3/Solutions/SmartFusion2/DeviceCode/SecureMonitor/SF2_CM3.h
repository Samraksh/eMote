#ifndef _SF2_CM3_H_
#define _SF2_CM3_H_

//Platform/chip specific definitions.

//Embedded sram RAM
#define ERAM_ORIGIN 0x20000000
//#define ERAM_SIZE 0x18000 //96KB
//96KB is the actual size, but not a power of 2 so we are going up to the next power of 2, which is 128 KB
#define ERAM_SIZE 0x20000 //128KB
#define ERAM_SIZE_POWER 17 //This 2^17 is 128KB, but has a power of 2 will cover entier RAM
//#define ERAM_OFFSET 0x0

//Embedded Flash
#define EFLASH_MIRROR_ORIGIN 0x60000000
#define EFLASH_ORIGIN 0x00000000 //On SF2 the internal flash actually starts at 0x8000000, but it gets mapped to 0x1000000 at run time
#define EFLASH_SIZE 0x80000 //512K
#define EFLASH_SIZE_POWER 19 //2^20 is 512K

#define DEPLOY_BASE 0x60060000
#define DEPLOY_SIZE 0x20000 //128K
#define ROT_BASE 0xF000
#define ROT_SIZE 0x4000
#define KERNEL_SIZE 0x9000
#define KERNEL_END (ROT_BASE+ROT_SIZE+KERNEL_SIZE)
#define RUNTIME_SIZE EFLASH_SIZE-KERNEL_END



#define IO_ORIGIN 0x40000000
#define IO_SIZE 0x20000000
#define IO_SIZE_POWER 29

//processor private peripheral bus
#define CPU_PPB_BASE 0xE0000000
#define CPU_PPB_SIZE 0x100000 //1MB

#endif //_SF2_CM3_H_
