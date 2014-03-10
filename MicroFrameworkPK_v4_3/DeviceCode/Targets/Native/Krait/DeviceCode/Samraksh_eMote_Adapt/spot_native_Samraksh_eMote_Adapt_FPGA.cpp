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


#include "spot_native.h"
#include "spot_native_Samraksh_eMote_Adapt_FPGA.h"
#include "..\Krait_FPGA\Krait_FPGA.h"

using namespace Samraksh::eMote::Adapt;

INT8 FPGA::Initialize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT8 retVal = fpga_init(); 
    return retVal;
}

