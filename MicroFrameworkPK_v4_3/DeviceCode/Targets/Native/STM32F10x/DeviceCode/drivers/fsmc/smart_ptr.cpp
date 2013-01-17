#include <smart_ptr.h>
#include "NOR\stm3210e_eval_fsmc_nor.h"

extern NOR_IDTypeDef NOR_ID;
extern void memory_init();

void smartPointer_test()
{
	int x = 0;
	int y = 2;
	int result = 0;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
	
	NOR_Init();
	
	NOR_ReadID(&NOR_ID);
	
	NOR_ReturnToReadMode();
	
	NOR_EraseChip();
	
	memory_init();
	
	smart_ptr<int> p(&x);
	//smart_ptr<int> q(&y);
	
	
	p.write(2);

	p.read();
	
	p.write(3);
	
	p.read();

}
