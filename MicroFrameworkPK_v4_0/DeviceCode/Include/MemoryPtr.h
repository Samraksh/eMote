/*
 *  Name : MemoryPtr.h
 *
 *	Desc : Interfaces the C# object and the datastore instance
 *
 */
#ifndef _MEMORY_PTR_H
#define _MEMORY_PTR_H

#include <datastore.h>



template<class T> class MemPtr
{
	T* ptr;
	INT recordId;
	UINT32 numOfBytes;
	static UINT32 currrecordId = 0;
	Data_store<T> ds;

	// Explicit assignment and copy constructors are blocked to avoid scenarios where two instances of
	// C# objects point to the same record. Disposing of one object may create dangling pointers.
	MemPtr(MemPtr<T> const&);
	MemPtr<T>& operator=(MemPtr<T> const&);


public:

	T& operator*()  { return *ptr;}

	T* operator->()   { return ptr;}


	// Called by the C# object constructor
	MemPtr(INT id, UINT32 size) : recordId(id),numOfBytes(size)
	{
		// create a record in datastore with record id and size
		ptr = (T*) ds.createRecord(recordId,size);

		// Call the instance of datastore

	}

	// called during other invocations from the c# object. The C# object maintains the record id
	MemPtr(INT Id) : recordId(Id)
	{
		ptr = (T*) ds.getAddress(recordId);
	}

	MemPtr& operator=(CLR_RT_TypedArray_UINT8 buffer)
	{
		CLR_RT_TypedArray_UINT8 localBuffer = buffer;
		ds.writeData(ptr, localbuffer.GetBuffer(),localbuffer.GetSize());
	}

	// returns the id of the newly created pointer
	UINT32 getId()
	{
		return recordId;
	}

	T* getPointer()
	{
		return ptr;
	}


	~MemPtr()
	{

	}

	void dispose()
	{
		ds.deleteRecord(recordId);
	}

};



#endif
