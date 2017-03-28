/*
 * MemoryManager.h
 *
 *  Created on: 27 марта 2017 г.
 *      Author: hudienko_a
 */

#ifndef EXTENTION_MEMORYMANAGER_H_
#define EXTENTION_MEMORYMANAGER_H_


extern "C"
{
	#include "../StdPeriph/cmsis_boot/stm32f10x.h"
	#include "../StdPeriph/stm_lib/inc/stm32f10x_gpio.h"
	#include "../StdPeriph/stm_lib/inc/stm32f10x_fsmc.h"
	#include "../StdPeriph/stm_lib/inc/stm32f10x_rcc.h"
}


#define LENGTH_36_NUM   (44)
#define LENGTH_52_NUM   (26)
#define LENGTH_64_NUM   (4)
#define LENGTH_128_NUM  (5)
#define LENGTH_132_NUM  (8)
#define LENGTH_256_NUM  (8)
#define LENGTH_512_NUM  (18)
#define LENGTH_640_NUM  (8)
#define LENGTH_1536_NUM (6)

#define MEM_USED        (1)
#define MEM_NO_USED     (0)

#define CALC_CNT    (0)
#define CALC_MAX    (1)

typedef struct _MEM_BLOCK_S {
    char used;
    int block_size;
    char *ptr;
    char *next;
} mem_block_s;


typedef struct _INFO_TBL_S
{
    int block_size;
    int num_max;
    mem_block_s *wm_head;
    int calc[2];
} info_TBL;


#define LENGTH_36_NUM   (44)
#define LENGTH_52_NUM   (26)
#define LENGTH_64_NUM   (4)
#define LENGTH_128_NUM  (5)
#define LENGTH_132_NUM  (8)
#define LENGTH_256_NUM  (8)
#define LENGTH_512_NUM  (18)
#define LENGTH_640_NUM  (8)
#define LENGTH_1536_NUM (6)

#define MEM_USED        (1)
#define MEM_NO_USED     (0)

#define CALC_CNT    (0)
#define CALC_MAX    (1)

#define MEM_TBL_MAX   7

class MemoryManager
{
public:
	MemoryManager(uint32_t StartAddr, unsigned int length);

	ErrorStatus Init();

	void* Malloc(unsigned int size);

	void Free(void* obj);

	void ShowMemory();

private:
	info_TBL _memInfoTbl[MEM_TBL_MAX];
	uint32_t _baseAddr;

	unsigned int _sramLength;
	bool MemHeapHasBeenInitialised;

	void SRAM_Write(uint32_t adr, uint16_t wert);

	uint16_t SRAM_Read(uint32_t adr);

};



#endif /* EXTENTION_MEMORYMANAGER_H_ */
