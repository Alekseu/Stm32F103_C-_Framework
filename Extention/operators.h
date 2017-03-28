/*
 * operators.h
 *
 *  Created on: 07.08.2013
 *  Файл перегрузки стандартных операторов С++
 */

#ifndef OPERATORS_H_
#define OPERATORS_H_

#include <stdlib.h>
#include "MemoryManager.h"
#include "MemoryPool.h"


void* operator new (size_t size);

void* operator new[] (size_t size);
void operator delete (void* ptr);

void operator delete[] (void* ptr);

//playsment
void* operator new (size_t size, MemoryManager* mem);
void* operator new[] (size_t size,  MemoryManager* mem);

void operator delete (void* ptr, MemoryManager* mem );
void operator delete[] (void* ptr,  MemoryManager* mem);

//playsment
void* operator new (size_t size, MemPool* mem);
void* operator new[] (size_t size,  MemPool* mem);

void operator delete (void* ptr, MemPool* mem );
void operator delete[] (void* ptr,  MemPool* mem);

#endif /* OPERATORS_H_ */
