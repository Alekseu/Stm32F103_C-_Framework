/*
 * MemoryPool.h
 *
 *  Created on: 28 марта 2017 г.
 *      Author: Alekseu
 */

#ifndef EXTENTION_MEMORYPOOL_H_
#define EXTENTION_MEMORYPOOL_H_
/*
 * Простенький менеджер памяти без горбатого :)))
 */
 typedef unsigned int SIZE_T;
 struct MemPoolBlock {
     MemPoolBlock *next;
     SIZE_T size;   // size not including this structure. This is a multiple of sizeof(MemPoolBlock)
     unsigned char alloc; // 0 = free, 1 = allocated
   };

class MemPool {
public:


  MemPool(void *buffer, SIZE_T bufferLength);

  void *malloc(SIZE_T size);
  void free(void const *ptr);

  void getStats(SIZE_T *largestFreeBlock, SIZE_T *totalFreeSize);
  SIZE_T getLargestFreeBlock(); // just an helper of getStats()

private:

  unsigned int  _baseAddr;

  void SRAM_Write(unsigned int adr, unsigned int wert);
  unsigned int SRAM_Read(unsigned int adr);

  MemPoolBlock *findFreeBlock(SIZE_T minsize);
  MemPoolBlock *findBlockFromPtr(void const *ptr);
  void mergeFreeBlocks();

  MemPoolBlock *m_blocks;
};

#endif /* EXTENTION_MEMORYPOOL_H_ */
