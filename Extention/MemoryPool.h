/*
 * MemoryPool.h
 *
 *  Created on: 28 марта 2017 г.
 *      Author: Alekseu
 */

#ifndef EXTENTION_MEMORYPOOL_H_
#define EXTENTION_MEMORYPOOL_H_

extern "C"
{
	#include "../StdPeriph/cmsis_boot/stm32f10x.h"
	#include "../StdPeriph/stm_lib/inc/stm32f10x_gpio.h"
	#include "../StdPeriph/stm_lib/inc/stm32f10x_fsmc.h"
	#include "../StdPeriph/stm_lib/inc/stm32f10x_rcc.h"
}

/*
 * Простенький менеджер памяти без горбатого :)))
 */
 typedef unsigned int SIZE_T;
 struct MemPoolBlock {
     MemPoolBlock *next;
     SIZE_T size;   // size not including this structure. This is a multiple of sizeof(MemPoolBlock)
    bool alloc; // 0 = free, 1 = allocated
   };

class MemPool {
public:

	 static MemPool* PoolObj;

  MemPool(void *buffer, SIZE_T bufferLength);

  void *malloc(SIZE_T size);
  void free(void const *ptr);

  void getStats(SIZE_T *largestFreeBlock, SIZE_T *totalFreeSize);
  SIZE_T getLargestFreeBlock(); // just an helper of getStats()

private:

  unsigned int  _baseAddr;
  unsigned int _objectsCounter;



  void SRAM_Write(uint32_t adr, uint16_t wert);
  uint32_t SRAM_Read(uint32_t adr);

  MemPoolBlock *findFreeBlock(SIZE_T minsize);
  MemPoolBlock *findBlockFromPtr(void const *ptr);
  void mergeFreeBlocks();

  MemPoolBlock *m_blocks;
  MemPoolBlock *_lastBlock;
};

#endif /* EXTENTION_MEMORYPOOL_H_ */
