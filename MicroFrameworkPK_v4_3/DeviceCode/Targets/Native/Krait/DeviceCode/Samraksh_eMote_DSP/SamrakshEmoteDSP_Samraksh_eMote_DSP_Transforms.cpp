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


#include "SamrakshEmoteDSP.h"
#include "SamrakshEmoteDSP_Samraksh_eMote_DSP_Transforms.h"
#include <..\math\Include\arm_math.h>

using namespace Samraksh::eMote::DSP;

INT8 Transforms::FFT( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, CLR_RT_TypedArray_UINT8 param1, UINT16 param2, HRESULT &hr )
{
    INT8 retVal = 0; 
    return retVal;
}

INT8 Transforms::FFT( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT16 param0, CLR_RT_TypedArray_UINT16 param1, UINT16 param2, HRESULT &hr )
{
    INT8 retVal = 0; 
    return retVal;
}

INT8 Transforms::FFT( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT32 param0, CLR_RT_TypedArray_UINT32 param1, UINT16 param2, HRESULT &hr )
{
    INT8 retVal = 0; 
    return retVal;
}

INT8 Transforms::FFT( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_INT16 param0, CLR_RT_TypedArray_INT16 param1, UINT16 param2, HRESULT &hr )
{
//public extern bool FFT(UInt16[] input, UInt16[] output,  UInt16 transformSize);
//void *memcpy(void *dest, const void *src, size_t n);
//void arm_cfft_radix4_q15(const arm_cfft_radix4_instance_q15 * S, q15_t * pSrc)
//arm_status arm_cfft_radix4_init_q15(arm_cfft_radix4_instance_q15 * S, uint16_t fftLen,  uint8_t ifftFlag,  uint8_t bitReverseFlag)
    INT8 retVal = 0; 
	
	uint8_t ifftFlag = 0;
	uint8_t doBitReverse = 1;	
	arm_cfft_radix4_instance_q15 S;
	memcpy(param1.GetBuffer(), param0.GetBuffer(), param2*2);
	arm_cfft_radix4_init_q15(&S, param2/2, ifftFlag, doBitReverse);
	arm_cfft_radix4_q15(&S, (q15_t*)param1.GetBuffer());	
	
    return retVal;
}

INT8 Transforms::IFFT( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, CLR_RT_TypedArray_UINT8 param1, UINT16 param2, HRESULT &hr )
{
    INT8 retVal = 0; 
    return retVal;
}

INT8 Transforms::IFFT( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT16 param0, CLR_RT_TypedArray_UINT16 param1, UINT16 param2, HRESULT &hr )
{
    INT8 retVal = 0; 
    return retVal;
}

INT8 Transforms::IFFT( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT32 param0, CLR_RT_TypedArray_UINT32 param1, UINT16 param2, HRESULT &hr )
{
    INT8 retVal = 0; 
    return retVal;
}

