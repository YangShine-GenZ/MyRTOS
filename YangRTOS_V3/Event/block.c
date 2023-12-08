#include "block.h"



void tMemBlockInit(tMemBlock* block,uint8_t *memStart,uint32_t blocksize,uint32_t blockCnt){

	uint8_t* blockStart = (uint8_t*)memStart;
	uint8_t* blockEnd = (uint8_t*)memStart + blocksize*blockCnt;

	if(blocksize<sizeof(tNode)){
		return;
	}
	tEventInit(&(block->event),tEventTypeMemBlock);
	block->memStart = memStart;
	block->blockSize = blocksize;
	block->maxCount = blockCnt;
	
	tListInit(&(block->blockList));
	
	while(blockStart<blockEnd){
		tNodeInit((tNode*)blockStart);
		tListAddLast(&(block->blockList),(tNode*)blockStart);
		blockStart += blocksize;
	}
		

}



uint32_t tMemBlockWait(tMemBlock *MemBlock,uint8_t **mem,uint32_t waitTicks){
	
	uint32_t status = tTaskEnterCritical();
	if(tListCount(&MemBlock->blockList)>0){
		*mem = (uint8_t *)tListRemoveFirst(&MemBlock->blockList);
		tTaskExitCritical(status);
		return tErrorNoError;
	}
	else{
		tEventWait(&MemBlock->event,currentTask,(void*)0,0,waitTicks);
		tTaskExitCritical(status);
		tTaskSched();
		
		*mem = currentTask->eventMsg;
		return currentTask->waitEventResult;
	}


}

uint32_t tMemBlockNoWaitGet(tMemBlock *MemBlock,uint8_t **mem){
	uint32_t status = tTaskEnterCritical();
	if(tListCount(&MemBlock->blockList)>0){
		*mem = (uint8_t *)tListRemoveFirst(&MemBlock->blockList);
		tTaskExitCritical(status);
		return tErrorNoError;
	}
	else{
		tTaskExitCritical(status);
		return tErrorResourceUnavailable; 
	}


}


void tMemBlockNotify(tMemBlock *MemBlock,uint8_t *mem){
	uint32_t status = tTaskEnterCritical();
	if(tEventWaitCount(&MemBlock->event)>0){
		tTask* task = tEventWake(&MemBlock->event,(void*)mem,tErrorNoError);
		if(task->priority < currentTask->priority){
			tTaskSched();
		
		}
	}
	else{
		tListAddLast(&MemBlock->blockList,(tNode*)mem);
	
	}
	tTaskExitCritical(status);

}


uint32_t tMemBlockDestroy(tMemBlock *MemBlock){
	uint32_t status = tTaskEnterCritical();
	uint32_t count = tEventRemoveAll(&MemBlock->event,(void*)0,tErrorDel);
	tTaskExitCritical(status);
	if(count>0){
		tTaskSched();
	
	}
	return count;

}












