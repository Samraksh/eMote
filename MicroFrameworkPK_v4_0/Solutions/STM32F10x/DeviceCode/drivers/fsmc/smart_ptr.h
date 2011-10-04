#ifndef _SMARTPOINTER_H_
#define _SMARTPOINTER_H_

//#include <iostream>
#include <memory.h>

//using namespace std;
extern void memory_init();

template <class T> class smart_ptr
{
	T* ptr;
	bool updated;

public:
	smart_ptr()
	{
		ptr = NULL;
	}

	smart_ptr(T* p = 0)
	{
		ptr	= p;
		updated = false;
		memory_init();
	}
	
	~smart_ptr()
	{
		// Returns memory back to the flash 
		//delete ptr; 
		free_flash(ptr);
	}

	T& operator*()		{ return *ptr;}

	T* operator->()		{ return ptr;}


	// Semantics of a pointer to pointer copy is a little unclear, whether the copy would just involve two pointers pointing to the same location
	// or whether a copy of the object itself is stored. (deep or shallow copy)
	smart_ptr& operator=(const smart_ptr& newPtr)
	{
		
	}

	// Semantics of write involve:
	// 1. Obtain free memory from flash driver
	// 2. Update smart pointer to point to the new location
	// 3. Either erase the old block or mark it for deletion
	smart_ptr& update(T data)
	{
	
		T* oldPtr = this->ptr;
	
		//this->ptr = (T*) 0x0180000;
		this->ptr = (T*) malloc_flash(sizeof(T));
		
		write_flash(this->ptr,&data,sizeof(T));
		
		//free_flash(oldPtr);
		
		return *this;
	
	
	}
	
	smart_ptr& write(T data)
	{
		if(updated == true)
		{
			update(data);
			return *this;
		}

		//  Calls the lower level driver to free memory
		free_flash(this->ptr);

		// Calls the lower lever driver to obtain memory
		this->ptr = (T*) malloc_flash(sizeof(T));

		// Write data to flash
		write_flash(this->ptr,&data,sizeof(T));
		
		updated = true;
		
		return *this;
	}

	// Semantics of write involve:
	// 1. Mark data as constant
	// 2. Either erase the old block or mark it for deletion
	// 3. Obtain free memory from flash driver
	// 4. Update smart pointer to point to the new location
	
	/*
	smart_ptr& write(const T data)
	{
		// Checking data as constant
		this->constantValue = true;

		//  Calls the lower level driver to free memory
		free_flash(this->ptr);

		// Calls the lower lever driver to obtain memory
		this->ptr = (T*) malloc_flash(sizeof(T));

		write_flash(this->ptr,&data, sizeof(T));
	}
	*/

	// Read the contents of memory pointed to by this->ptr
	smart_ptr& read()
	{
		read_flash(this->ptr);
	}
	
};

#endif