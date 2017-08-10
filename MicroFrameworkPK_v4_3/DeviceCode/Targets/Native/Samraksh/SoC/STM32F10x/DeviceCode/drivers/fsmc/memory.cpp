// The Samraksh Company
// Desc
//
// Author : Nived Sivadas

#include <tinyhal.h>
#include "NOR\stm3210e_eval_fsmc_nor.h"

#define BUFFER_SIZE        0x400
#define FLASH_START_ADDRESS 0x8000

uint16_t TxBuffer[BUFFER_SIZE];

uint32_t WriteReadStatus = 0, Index = 0;
NOR_IDTypeDef NOR_ID;
uint32_t lastBlockAddress = 0x0;


typedef long Align;

union header {
	struct	{
		union header *ptr;
		unsigned size;
		}s;
		Align x;
	};
	
typedef union header Header;

static Header base;
static Header *freep = NULL;


struct flash{
	uint16_t* address;
	bool allocated;
	bool dirty;
	flash* next;
};

typedef struct flash Flash;

// Stores the flashMap in memory
Flash **flashMap;


void Fill_Buffer(uint16_t *pBuffer, uint16_t BufferLenght, uint32_t Offset)
{
  uint16_t IndexTmp = 0;

  /* Put in global buffer same values */
  for (IndexTmp = Offset; IndexTmp < BufferLenght; IndexTmp++ )
  {
    pBuffer[IndexTmp] = 0;
  }
}

void memory_init()
{
	lastBlockAddress = 0x0;
	*flashMap = NULL;
	
	while(lastBlockAddress <=  0x0FE000)
	{
			Flash *flashObj = (Flash*) private_malloc(sizeof(Flash));
			flashObj->address = (uint16_t *) lastBlockAddress;
			flashObj->allocated = false;
			flashObj->dirty = false;
			flashObj->next = *flashMap;
			*flashMap = flashObj;
			lastBlockAddress += 0x020000;
	
	}
	
}

// malloc_flash returns the address of the location where the write to flash has to happen. Write now for testing i am using default as 0x8000
void *malloc_flash(unsigned nbytes)
{
	Flash *flashState = *flashMap;
	
	while(flashState->next != NULL)
	{
		if(flashState->allocated == false)
		{
			flashState->allocated = true;
			 return (void *) flashState->address;
		}
		else
		{
			// If flash is full malloc_flash returns null to indicate no memory available
			return NULL;
		}	
		flashState = flashState->next;
	}
}

void free_flash(void* ap)
{
	Flash *flashState = *flashMap;
	while(flashState->address != ap);
	flashState->dirty = false;
	flashState->allocated = false;
	NOR_EraseBlock((uint32_t) flashState->address);
}

uint16_t* read_flash(void *ap)
{
	static uint16_t RxBuffer[BUFFER_SIZE];
	
	NOR_ReadBuffer(RxBuffer,(uint32_t) ap , BUFFER_SIZE);  
	
	return RxBuffer;
	
}

void write_flash(void *ap,void *data, int nbytes)
{	
	uint16_t *TxBufferPtr = (uint16_t *) data;
	
	 for(uint16_t i = 0; i < nbytes; i++)
	 {
		 TxBuffer[i] = TxBufferPtr[i];
	 }
	
	Fill_Buffer(TxBuffer, BUFFER_SIZE, nbytes);
	NOR_WriteBuffer(TxBuffer,(uint32_t) ap, BUFFER_SIZE);

}





