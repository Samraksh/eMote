//
// The Samraksh Company
//
// Author : Nived.Sivadas@samraksh.com
//
//
#include <tinyhal.h>
#include <stm32f10x.h>
#include "..\..\platform_selector.h"
#define VectorTableOffsetRegister 0xE000ED08

#define OFFSET 0x20000

extern "C"
{

void VectorRelocate()
{
#ifdef TINYCLR_SOLO
	*(__IO uint32_t *) VectorTableOffsetRegister |= (uint32_t) 0;
#else
	*(__IO uint32_t *) VectorTableOffsetRegister |= (uint32_t) OFFSET;
#endif

}
}
