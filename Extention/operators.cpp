/*
 * operators.cpp
 *
 *  Created on: 29 θώνÿ 2016 γ.
 *      Author: hudienko_a
 */


#include "operators.h"

//#include <stdio.h>



void* operator new(size_t size)
{
	if(MemoryManager::MemoryManObj!=0)
	{
		return MemoryManager::MemoryManObj->Malloc(size);
	}

	return malloc(size);

}

void* operator new[](size_t size)
{
	if(MemoryManager::MemoryManObj!=0)
	{
		return MemoryManager::MemoryManObj->Malloc(size);
	}

	return malloc(size);

}

void operator delete (void* p)
{
	if(MemoryManager::MemoryManObj!=0)
	{
		MemoryManager::MemoryManObj->Free(p);
		return;
	}

	free(p);

}

 void operator delete[](void* p)
 {
	 if(MemoryManager::MemoryManObj!=0)
	 {
		 MemoryManager::MemoryManObj->Free(p);
		 return;
	 }

	 free(p);

 }


////playsment
//
//void* operator new (size_t size, MemoryManager* mem)
//{
//	void* ret;
//	ret = mem->Malloc(size);
//	return ret;
//}
//void* operator new[] (size_t size,  MemoryManager* mem)
//{
//	void* ret;
//		ret = mem->Malloc(size);
//		return ret;
//}
//
//void operator delete (void* ptr, MemoryManager* mem )
//{
//	mem->Free(ptr);
//}
//void operator delete[] (void* ptr,  MemoryManager* mem)
//{
//	mem->Free(ptr);
//}
//
////playcement
//void* operator new (size_t size, MemPool* mem)
//{
//	void* ret;
//	ret = mem->malloc(size);
//	return ret;
//}
//void* operator new[] (size_t size,  MemPool* mem)
//{
//	void* ret;
//		ret = mem->malloc(size);
//		return ret;
//}
//
//void operator delete (void* ptr, MemPool* mem )
//{
//	mem->free(ptr);
//}
//void operator delete[] (void* ptr,  MemPool* mem)
//{
//	mem->free(ptr);
//}

