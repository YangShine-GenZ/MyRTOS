#ifndef __BLOCK_H

#define __BLOCK_H


#include "tinyOS.h"
#include "event.h"



typedef struct _tMemBlock{
	tEvent event;
	void* memStart;
	uint32_t blockSize;
	uint32_t maxCount;
	tList blockList;
}tMemBlock;


void tMemBlockInit(tMemBlock* block,uint8_t *memStart,uint32_t blocksize,uint32_t blockCnt);
uint32_t tMemBlockWait(tMemBlock *MemBlock,uint8_t **mem,uint32_t waitTicks);
uint32_t tMemBlockNoWaitGet(tMemBlock *MemBlock,uint8_t **mem);
void tMemBlockNotify(tMemBlock *MemBlock,uint8_t *mem);
uint32_t tMemBlockDestroy(tMemBlock *MemBlock);



#endif