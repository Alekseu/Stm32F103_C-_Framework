/*
 * operators.cpp
 *
 *  Created on: 29 ���� 2016 �.
 *      Author: hudienko_a
 */


#include "operators.h"

#include <stdio.h>


void* operator new(size_t size)
{
	return malloc(size);
	//return _pool.malloc(size);
}

void* operator new[](size_t size)
{
	return  malloc(size);
	//return _pool.malloc(size);

}

 void operator delete (void* p)
{
	 free(p);
	// _pool.free(p);
}

void operator delete[](void* p)
{
	free(p);
	// _pool.free(p);
}


//playsment

void* operator new (size_t size, MemoryManager* mem)
{
	void* ret;
	ret = mem->Malloc(size);
	return ret;
}
void* operator new[] (size_t size,  MemoryManager* mem)
{
	void* ret;
		ret = mem->Malloc(size);
		return ret;
}

void operator delete (void* ptr, MemoryManager* mem )
{
	mem->Free(ptr);
}
void operator delete[] (void* ptr,  MemoryManager* mem)
{
	mem->Free(ptr);
}

