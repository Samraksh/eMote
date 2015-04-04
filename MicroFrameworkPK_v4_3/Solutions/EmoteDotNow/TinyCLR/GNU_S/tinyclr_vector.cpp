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


extern "C"
{

void VectorRelocate()
{
	*(__IO uint32_t *) VectorTableOffsetRegister |= (uint32_t) FLASH_MEMORY_Base;
}
}
