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


void* operator new (size_t size);

void* operator new[] (size_t size);
void operator delete (void* ptr);

void operator delete[] (void* ptr);

//playsment
void* operator new (size_t size, MemoryManager* mem);
void* operator new[] (size_t size,  MemoryManager* mem);

void operator delete (void* ptr, MemoryManager* mem );
void operator delete[] (void* ptr,  MemoryManager* mem);

#endif /* OPERATORS_H_ */
