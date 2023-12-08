#include "sem.h"
#include "event.h"



void tSemInit(tSem* sem,uint32_t startCount,uint32_t maxCount){
	tEventInit(&(sem->event),tEventTypeSem);
	sem->maxCount = maxCount;
	if(maxCount == 0){
		sem->count = startCount;
	}
	else{
		sem->count = (startCount>maxCount) ? maxCount:startCount;
	}
	
}



uint32_t tSemWait(tSem* sem, uint32_t waitTicks){
	uint32_t status = tTaskEnterCritical();
	if(sem->count>0){
		sem->count--;
		tTaskExitCritical(status);
		return tErrorNoError;
	
	}
	else{
		tEventWait(&(sem->event),currentTask,(void*)0,0,waitTicks);
		tTaskExitCritical(status);
		tTaskSched();
		return currentTask->waitEventResult;
	}
	
}


uint32_t tSemNowaitGet(tSem* sem){
	uint32_t status = tTaskEnterCritical();
	if(sem->count>0){
		sem->count--;
		tTaskExitCritical(status);
		return tErrorNoError;
	
	}
	else{
		tTaskExitCritical(status);
		return tErrorResourceUnavailable;
	}

}


void tSemNotify(tSem* sem){
	uint32_t status = tTaskEnterCritical();
	
	if(tEventWaitCount(&(sem->event))> 0){
		tTask* task = tEventWake(&(sem->event),(void*)0,tErrorNoError);
		if(task->priority < currentTask->priority){
			tTaskSched();
		}
	}
	else{
	
		sem->count++;
		if((sem->maxCount!=0)&&(sem->count>sem->maxCount)){
			sem->count = sem->maxCount;
		}
		
	}
	tTaskExitCritical(status);

}

uint32_t tSemRemoveAll(tSem* sem){
	uint32_t count = 0;
	uint32_t status = tTaskEnterCritical();
	count = tEventRemoveAll(&(sem->event),(void*)0,tErrorDel);
	tTaskExitCritical(status);
	
	if(count>0){
		tTaskSched();
	
	}
	return count;
}

void tSemGetInfo(tSem* sem, tSemInfo* semInfo){
	
	uint32_t status = tTaskEnterCritical();
	
	semInfo->count = sem->count;
	semInfo->maxCount = sem->maxCount;
	semInfo->taskCount = tEventWaitCount(&(sem->event));
	
	tTaskExitCritical(status);
}













