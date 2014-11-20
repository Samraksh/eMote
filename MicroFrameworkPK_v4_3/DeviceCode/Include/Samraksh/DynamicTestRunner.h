/**
 * @file   DynamicTestRunner.h
 * @author Michael Andrew McGrath <Michael.McGrath@Samraksh.com>
 * @date   Oct 23, 2014
 * @brief  PAL library for calling native test code.
 * @copyright (C) 2014 The Samraksh Company
 */

#ifndef DYNAMICTESTRUNNER_H_
#define DYNAMICTESTRUNNER_H_

#include <tinyhal_types.h>  // UINT32

namespace Emote_DynamicTestRunner
{
    bool Initialize(void);

    bool Process (UINT32 command, UINT32 paramCount, UINT32* paramAddrs, void* &result);

    bool MallocByteBuffer(UINT32 *bufferAddr, size_t size);

    bool ReadByteBuffer(UINT32 *bufferAddr, UINT32 byteOffset, UINT32 &length, UINT8 *data);

    bool WriteByteBuffer(UINT32 *bufferAddr, UINT32 byteOffset, UINT32 &length, UINT8 *data);

    bool FreeByteBuffer(UINT32 *bufferAddr);

    bool FreeAllBuffers(void);

    bool ShowTests(UINT32 max_testCount, UINT32 &testCount, UINT32* testAddr);

    bool ShowByteBuffers(UINT32 max_bufferCount, UINT32 &bufferCount, UINT32* bufferAddr);
}
#endif /* DYNAMICTESTRUNNER_H_ */
