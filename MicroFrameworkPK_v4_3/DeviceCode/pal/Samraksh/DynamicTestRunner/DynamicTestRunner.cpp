/**
 * @file   DynamicTestRunner.cpp
 * @author Michael Andrew McGrath <Michael.McGrath@Samraksh.com>
 * @date   Oct 23, 2014
 * @copyright (C) 2014 The Samraksh Company
 * @brief  Library for sending/receiving messages to execute native tests.
 */

#include <tinyhal.h>

#include <Samraksh\DynamicTestRunner.h>

#include "GlobalTestsTable.h"


struct BufferHeader
{
    UINT32 length;
    UINT8* data;
};

struct DoublyLinkedNode
{
    struct DoublyLinkedNode *next;
    struct DoublyLinkedNode *prev;
    void* data;
};

struct DoublyLinkedList
{
    struct DoublyLinkedNode *firstNode;
    struct DoublyLinkedNode *lastNode;
};

struct CombinedNodeHeaderData
{
    struct DoublyLinkedNode node;
    struct BufferHeader header;
    UINT8 data[4];
};

static struct DoublyLinkedList BufferList;

void insertAfter(struct DoublyLinkedList *list, struct DoublyLinkedNode *node, struct DoublyLinkedNode *newNode)
{
     newNode->prev  = node;
     newNode->next  = node->next;
     if (node->next == NULL)
     {
         list->lastNode  = newNode;
     }
     else
     {
         node->next->prev  = newNode;
     }
     node->next  = newNode;
}

void insertBefore(struct DoublyLinkedList *list, struct DoublyLinkedNode *node, struct DoublyLinkedNode *newNode)
{
     newNode->prev  = node->prev;
     newNode->next  = node;
     if (node->prev == NULL)
     {
         list->firstNode  = newNode;
     }
     else
     {
         node->prev->next  = newNode;
     }
     node->prev  = newNode;
}

void insertBeginning(struct DoublyLinkedList *list, struct DoublyLinkedNode *newNode)
{
    if (list->firstNode == NULL)
    {
        list->firstNode  = newNode;
        list->lastNode   = newNode;
        newNode->prev    = NULL;
        newNode->next    = NULL;
    }
    else
    {
        insertBefore(list, list->firstNode, newNode);
    }
}

void insertEnd(struct DoublyLinkedList *list, struct DoublyLinkedNode *newNode)
{
     if (list->lastNode == NULL)
     {
         insertBeginning(list, newNode);
     }
     else
     {
         insertAfter(list, list->lastNode, newNode);
     }
}

void removeNode(struct DoublyLinkedList *list, struct DoublyLinkedNode *node)
{
    if (node->prev == NULL)
    {
        list->firstNode  = node->next;
    }
    else
    {
        node->prev->next  = node->next;
    }
    if (node->next == NULL)
    {
        list->lastNode  = node->prev;
    }
    else
    {
        node->next->prev  = node->prev;
    }
    private_free( node );
}

/// public:



bool Emote_DynamicTestRunner::Initialize()
{
    BufferList.firstNode = NULL;
    BufferList.lastNode = NULL;
    return false;
}

struct DoublyLinkedNode* findNodeByData(struct DoublyLinkedList* list, void* data)
{
    struct DoublyLinkedNode *ptr_node = list->firstNode;
    while(ptr_node != NULL)
    {
        if(ptr_node->data == data)
        {
            break;
        }
        ptr_node = ptr_node->next;
    }
    if(ptr_node != NULL && ptr_node->data != data)
    {
        ptr_node = NULL;
    }
    return ptr_node;
}

/**
 * @todo use index of global test table instead of address.
 * @param command number of arguments to accept
 * @param paramCount the equivalent of 'argc'
 * @param paramAddrs the equivalent of 'argv'
 * @param result what function returns.
 */
bool Emote_DynamicTestRunner::Process (UINT32 command, UINT32 paramCount, UINT32* paramAddrs, void* &result)
{
    bool fRet = true;
    //hal_printf("[Emote_DynamicTestRunner_Process]   calling function 0x%X\n",command);

	//verify function address.
	if(command > 0x8020000) {

#if defined(COMPILE_THUMB2)
    if((command % 2) == 0) {
        command = command + 1;  //need odd address for thumb. user could have used even address from MAP file.
    }
#endif

		bool matchedFunction = false;
		struct TestEntry* ptr_test;
		for(int itr=0; itr < GlobalTestCount; ++itr) {
			ptr_test = &(GlobalTestTable[itr]);
			if(ptr_test->functionAddress == (generic_fptr)command) {
				matchedFunction = true;
				break;
			}
		}
		if(matchedFunction == false) {
			fRet = false;
		}
	}
	else if(command < GlobalTestCount) {
		command = (UINT32)GlobalTestTable[command].functionAddress;
	}
	else {
		fRet = false;
	}

    //verify addresses match allocated buffers
    struct DoublyLinkedNode *ptr_node;
    for(int itr=0; itr < paramCount; ++itr) {
        ptr_node = findNodeByData(&BufferList, (void*)(paramAddrs[itr]));
        if(ptr_node == NULL)
        {
			fRet = false;
			break;
        }
        paramAddrs[itr] = (UINT32)&(((struct BufferHeader*)(ptr_node->data))->data[0]);
    }

	if(fRet != false) {
		switch(paramCount)
		{
			case 0:
			{
				void* (*func_ptr)(void) = (void* (*)(void))command;
				result = func_ptr();
				fRet = true;
				break;
			}
			case 1:
			{
				void* (*func_ptr)(void*) = (void* (*)(void*))command;
				result = func_ptr((void*)(paramAddrs[0]));
				fRet = true;
				break;
			}
			case 2:
			{
				void* (*func_ptr)(void*, void*) = (void* (*)(void*,void*))command;
				result = func_ptr((void*)paramAddrs[0], (void*)paramAddrs[1]);
				fRet = true;
				break;
			}
			case 3:
			{
				void* (*func_ptr)(void*, void*, void*) = (void* (*)(void*, void*, void*))command;
				result = func_ptr((void*)paramAddrs[0], (void*)paramAddrs[1], (void*)paramAddrs[2]);
				fRet = true;
				break;
			}
			case 4:
			{
				void* (*func_ptr)(void*, void*, void*, void*) = (void* (*)(void*, void*, void*, void*))command;
				result = func_ptr((void*)paramAddrs[0], (void*)paramAddrs[1], (void*)paramAddrs[2], (void*)paramAddrs[3]);
				fRet = true;
				break;
			}
			default:
			{
				fRet = false;
			}
		}
	}

    return fRet;
}

bool Emote_DynamicTestRunner::MallocByteBuffer(UINT32* bufferAddr, size_t size)
{
    bool fRet = false;
    int size_alloc = sizeof(struct CombinedNodeHeaderData) + size; //should be >= 4 bytes too big
    struct CombinedNodeHeaderData* allocated = (struct CombinedNodeHeaderData*)private_malloc(size_alloc);
    struct DoublyLinkedNode *newNode = &(allocated->node);
    if(newNode != NULL)
    {
        memset(allocated, 0, size_alloc);
        struct BufferHeader *newBuffer = &(allocated->header);
        newNode->data = newBuffer;
        newBuffer->length = size;
        newBuffer->data = &(allocated->data[0]);
        insertEnd(&BufferList, newNode);
        *bufferAddr = (UINT32) newBuffer;
        fRet = true;
    }
    return fRet;
}

/**
 * @param bufferAddr pointer to location in memory that buffer should be, used as identification.
 * @param byteOffset offset into buffer where read should start.
 * @param length in: maximum amount to be read. out: amount read.
 * @param data destination of read data. must be at least length long.
 */
bool Emote_DynamicTestRunner::ReadByteBuffer(UINT32 *bufferAddr, UINT32 byteOffset, UINT32 &length, UINT8 *data)
{
    bool fRet = false;
    struct DoublyLinkedNode *ptr_node = findNodeByData(&BufferList, bufferAddr);
    if(ptr_node != NULL)
    {
        struct BufferHeader *buffer = (struct BufferHeader*)(ptr_node->data);
        if(buffer->length < byteOffset)
        {
            length = 0;
            fRet = false;
        }
        else
        {
			if(buffer->length < (byteOffset + length))
			{
				length = buffer->length - byteOffset;
				fRet = false; // but still try to read as much as possible.  setting to false helps host PC stop reading.
			}
			else
			{
				fRet = true;
			}
			memcpy(data, &(buffer->data[byteOffset]), length);
        }
    }
    return fRet;
}

bool Emote_DynamicTestRunner::WriteByteBuffer(UINT32 *bufferAddr, UINT32 byteOffset, UINT32 &length, UINT8 *data)
{
    bool fRet = false;
    struct DoublyLinkedNode *ptr_node = findNodeByData(&BufferList, bufferAddr);
    if(ptr_node != NULL)
    {
        struct BufferHeader *buffer = (struct BufferHeader*)(ptr_node->data);
        if(byteOffset < buffer->length)
        {
            if(buffer->length < byteOffset + length)
            {
                length = buffer->length - byteOffset;
            }
            memcpy((void*)&(buffer->data[byteOffset]), (void*)data, (unsigned long int)length);
            fRet = true;
        }
    }
    return fRet;
}

bool Emote_DynamicTestRunner::FreeByteBuffer(UINT32 *bufferAddr)
{
    bool fRet = false;
    struct DoublyLinkedNode *ptr_node = BufferList.firstNode;

    while(ptr_node != NULL)
    {
        if(ptr_node->data == (void*)bufferAddr)
        {
            removeNode(&BufferList,ptr_node);
            fRet = true;
            break;
        }
        ptr_node = ptr_node->next;
    }
    return fRet;
}

bool Emote_DynamicTestRunner::FreeAllBuffers()
{
    bool fRet = true;
    struct DoublyLinkedNode *ptr_node = BufferList.firstNode;
    struct DoublyLinkedNode *ptr_node_next = NULL;
    while(ptr_node != NULL)
    {
        ptr_node_next = ptr_node->next;
        private_free(ptr_node);
        ptr_node = ptr_node_next;
    }
    BufferList.firstNode = NULL;
    BufferList.lastNode = NULL;

    return fRet;
}

bool Emote_DynamicTestRunner::ShowTests(UINT32 max_testCount, UINT32 &testCount, UINT32* testAddr)
{
    bool fRet = true;
    max_testCount = (max_testCount < GlobalTestCount)? max_testCount : GlobalTestCount;
    testCount = 0;
    while(testCount < max_testCount )
    {
        testAddr[testCount] = (UINT32)(GlobalTestTable[testCount].functionAddress);
        ++testCount;
    }
    return fRet;
}

bool Emote_DynamicTestRunner::ShowByteBuffers(UINT32 max_bufferCount, UINT32 &bufferCount, UINT32* bufferAddr)
{
    bool fRet = true;
    bufferCount = 0;
    struct DoublyLinkedNode *ptr_node = BufferList.firstNode;
    while(ptr_node != NULL)
    {
        if(bufferCount >= max_bufferCount)
        {
            fRet = false;
            break;
        }
        bufferAddr[bufferCount] = (UINT32)(ptr_node->data);
        ++bufferCount;
        ptr_node = ptr_node->next;
    }
    return fRet;
}

void* test_function_cmd0() {
    volatile UINT32 ret = 0;
    volatile UINT32 a = 0;
    volatile UINT32 b = 0;
    volatile UINT32 c = 0;
    volatile UINT32 d = 0;
    a = a + 0;
    b = b + b;
    c = c + c + c;
    d = d + d + d + d;
    ret = a + b + c + d;
    return (void*)ret;
}

void* test_function_cmd1(void* a_in) {
    volatile UINT32 ret = 0;
    volatile UINT32 a = (UINT32)a_in;
    volatile UINT32 b = 0;
    volatile UINT32 c = 0;
    volatile UINT32 d = 0;
    a = a + 0;
    b = b + b;
    c = c + c + c;
    d = d + d + d + d;
    ret = a + b + c + d;
    return (void*)ret;
}

void* test_function_cmd2(void* a_in, void* b_in) {
    volatile UINT32 ret = 0;
    volatile UINT32 a = (UINT32)a_in;
    volatile UINT32 b = (UINT32)b_in;
    volatile UINT32 c = 0;
    volatile UINT32 d = 0;
    a = a + 0;
    b = b + b;
    c = c + c + c;
    d = d + d + d + d;
    ret = a + b + c + d;
    return (void*)ret;
}

void* test_function_cmd3(void* a_in, void* b_in, void* c_in) {
    volatile UINT32 ret = 0;
    volatile UINT32 a = (UINT32)a_in;
    volatile UINT32 b = (UINT32)b_in;
    volatile UINT32 c = (UINT32)c_in;
    volatile UINT32 d = 0;
    a = a + 0;
    b = b + b;
    c = c + c + c;
    d = d + d + d + d;
    ret = a + b + c + d;
    return (void*)ret;
}

void* test_function_cmd4(void* a_in, void* b_in, void* c_in, void* d_in) {
    volatile UINT32 ret = 0;
    volatile UINT32 a = (UINT32)a_in;
    volatile UINT32 b = (UINT32)b_in;
    volatile UINT32 c = (UINT32)c_in;
    volatile UINT32 d = (UINT32)d_in;
    a = a + 0;
    b = b + b;
    c = c + c + c;
    d = d + d + d + d;
    ret = a + b + c + d;
    return (void*)ret;
}
