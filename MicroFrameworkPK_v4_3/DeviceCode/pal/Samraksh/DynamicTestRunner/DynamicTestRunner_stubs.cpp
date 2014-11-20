/**
 * @file   DynamicTestRunner_stubs.cpp
 * @author Michael Andrew McGrath <Michael.McGrath@Samraksh.com>
 * @date   Oct 23, 2014
 * @copyright (C) 2014 The Samraksh Company
 * @brief  stubs file
 */

#include <Samraksh\DynamicTestRunner.h>

bool Emote_DynamicTestRunner::Process (UINT32 * command)
{
    bool fRet = false;
    return fRet;
}

bool Emote_DynamicTestRunner::Initialize(void)
{
    bool fRet = false;
    return fRet;
}

bool Emote_DynamicTestRunner::Process (UINT32 command, UINT32 paramCount, UINT32* paramAddrs, void* &result)
{
    bool fRet = false;
    return fRet;
}

bool Emote_DynamicTestRunner::MallocByteBuffer(UINT32 *bufferAddr, size_t size)
{
    bool fRet = false;
    return fRet;
}

bool Emote_DynamicTestRunner::ReadByteBuffer(UINT32 *bufferAddr, UINT32 byteOffset, UINT32 &length, UINT8 *data)
{
    bool fRet = false;
    return fRet;
}

bool Emote_DynamicTestRunner::WriteByteBuffer(UINT32 *bufferAddr, UINT32 byteOffset, UINT32 &length, UINT8 *data)
{
    bool fRet = false;
    return fRet;
}

bool Emote_DynamicTestRunner::FreeByteBuffer(UINT32 *bufferAddr)
{
    bool fRet = false;
    return fRet;
}

bool Emote_DynamicTestRunner::FreeAllBuffers(void)
{
    bool fRet = false;
    return fRet;
}

bool Emote_DynamicTestRunner::ShowTests(UINT32 max_testCount, UINT32 &testCount, UINT32* testAddr)
{
    bool fRet = false;
    return fRet;
}

bool Emote_DynamicTestRunner::ShowByteBuffers(UINT32 max_bufferCount, UINT32 &bufferCount, UINT32* bufferAddr)
{
    bool fRet = false;
    return fRet;
}


