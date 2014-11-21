/**
 * @file   DynamicTestRunnerTestsInstantiations.cpp
 * @author Michael Andrew McGrath <Michael.McGrath@Samraksh.com>
 * @date   Nov 21, 2014
 * @copyright (C) 2014 The Samraksh Company
 */

#include "DynamicTestRunnerTests.h"

namespace Emote_DynamicTestRunner_Tests {

template void* Add<UINT8>(void*,void*,void*,void*);
template void* Subtract<UINT8>(void*,void*,void*,void*);
template void* Add<UINT32>(void*,void*,void*,void*);
template void* Subtract<UINT32>(void*,void*,void*,void*);


}



