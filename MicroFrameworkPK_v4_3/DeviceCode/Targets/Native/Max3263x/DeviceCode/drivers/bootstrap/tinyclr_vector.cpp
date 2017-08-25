//
// The Samraksh Company
//

#include <tinyhal.h>
#include <system_max3263x.h>
#include "mxc_config.h"

//#define VectorTableOffsetRegister 0xE000ED08

#if defined(TARGETLOCATION_RAM)
extern UINT32 Load$$ER_RAM$$Base
#elif defined(TARGETLOCATION_FLASH)
extern UINT32 Load$$ER_FLASH$$Base;
#endif

extern "C"
{

	void __section(SectionForBootstrapOperations) VectorRelocate()
	//void VectorRelocate()
	{
#if defined(TARGETLOCATION_RAM)
		const UINT32 VTABLE_ADDR = (UINT32) &Load$$ER_RAM$$Base;    // TODO: independent loader section for RAM VectorTable
#elif defined(TARGETLOCATION_FLASH)
		const UINT32 VTABLE_ADDR = (UINT32) &Load$$ER_FLASH$$Base;  // Hard-coded for now
		//TODO: if desired, copy vector table from flash to RAM and set VTABLE_ADDR = RAM target address.
#endif
		const UINT32 TBLOFF_MASK  = 0x00FFFFC0;        // Offset address is bits 28:7 but remove 28:24 to obtain peripheral (FLASH,SRAM) offset
		const UINT32 PERIPH_MASK  = 0x00FFFFFF;        // Sub-address in ram or flash
		const UINT32 ALIGN_MASK   = 0xFFFFFE00;        // Remove bottom 9 bytes for 512-byte alignment for STM32F1-XL
		const UINT32 TBLBASE_MASK = (0x1 << 29);       // Table base in code (0) or RAM(1)

		ASSERT(VTABLE_ADDR == (VTABLE_ADDR & ALIGN_MASK));  // TODO: use static_assert instead of runtime assert

		UINT32 newVTOR = (uint32_t) (VTABLE_ADDR & TBLOFF_MASK & PERIPH_MASK & ALIGN_MASK);

		if((VTABLE_ADDR & ~PERIPH_MASK) == 0x20000000)
		{
			newVTOR = newVTOR | TBLBASE_MASK;          // vector table in RAM
		}

		SCB->VTOR = (uint32_t)newVTOR;
	}

}
