//-----------------------------------------------------------------------------
//
//                   ** WARNING! ** 
//    This file was generated automatically by a tool.
//    Re-running the tool will overwrite this file.
//    You should copy this file to a custom location
//    before adding any customization in the copy to
//    prevent loss of your changes when the tool is
//    re-run.
//
//-----------------------------------------------------------------------------


#include "InterfaceLibrary.h"
#include "InterfaceLibrary_Samraksh_FileSystem.h"

#define BLOCK_SIZE            512

using namespace Samraksh;


uint8_t Buffer_Block_Tx[BLOCK_SIZE], Buffer_Block_Rx[BLOCK_SIZE];

void Fill_Buffer(uint8_t *pBuffer, uint32_t BufferLength, uint32_t Offset)
{
  uint16_t index = 0;

  /* Put in global buffer same values */
  for (index = 0; index < BufferLength; index++ )
  {
    pBuffer[index] = index + Offset;
  }
}


INT32 FileSystem::Init( HRESULT &hr )
{
    INT32 retVal = 0;
	
	CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_SDIO, SDIO_ISR ,NULL);
	
	CPU_INTC_InterruptEnable(STM32_AITC::c_IRQ_INDEX_SDIO);
	
	retVal = SD_Init();
	
	if(retVal == 0)
		return 1;
	else
		return 0;
}

void SDIO_ISR( void* Param )
{
	SD_ProcessIRQSrc();
}


INT32 FileSystem::Write( CLR_RT_TypedArray_UINT8 param0, UINT32 param1, UINT16 param2, HRESULT &hr )
{
    INT32 retVal = 0; 
	INT32 index = 0;
	
	if(param2 > BLOCK_SIZE)
		return -1;
	
	do
	{
		Buffer_Block_Tx[index] = param0[index];
	
	}while(index++ < param2);
	
	Fill_Buffer(Buffer_Block_Tx,index,0x0);
	
	Status = SD_WriteBlock(Buffer_Block_Tx, param1, param2);
	
    return retVal;
}

INT32 FileSystem::Read( CLR_RT_TypedArray_UINT8 param0, UINT32 param1, UINT16 param2, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

INT32 FileSystem::Erase( UINT32 param0, UINT32 param1, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

