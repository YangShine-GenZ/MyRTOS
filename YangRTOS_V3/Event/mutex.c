#include "mutex.h"
#include "tconfig.h"
#include "state.h"


void tMutexInit(tMutex* mutex){
	tEventInit(&mutex->event,tEventTypeMutex);
	mutex->lockedCount = 0;
	mutex->owner = (tTask*)0;
	mutex->ownerOriginPriority = TINYOS_PRO_COUNT;
}



uint32_t tMutexWait(tMutex* mutex,uint32_t waitTicks){
	uint32_t status = tTaskEnterCritical();
	if(mutex->lockedCount<=0){
		mutex->owner = currentTask;
		mutex->ownerOriginPriority = currentTask->priority;
		tTaskExitCritical(status);
		return tErrorNoError;
	}
	else{
		if(mutex->owner == currentTask){
			mutex->lockedCount++;
			tTaskExitCritical(status);
			return tErrorNoError;
		}
		else{
			if(currentTask->priority < mutex->ownerOriginPriority){
				tTask* owner = mutex->owner;
				if(owner->state == TINYOS_TASK_STATE_RDY){
					tTaskSchedUnRdy(owner);
					owner->priority = currentTask->priority;
					tTaskSchedRdy(owner);
				}
				else{
					owner->priority = currentTask->priority;
					
				}
			}
			tEventWait(&mutex->event,currentTask,(void*)0,0,waitTicks);
			tTaskExitCritical(status);
			tTaskSched();
			return currentTask->waitEventResult;
			
		
		}
		
	}

}


uint32_t tMutexNotify(tMutex* mutex){
	uint32_t status = tTaskEnterCritical();
	
	if(mutex->lockedCount<=0){
		tTaskExitCritical(status);
		return tErrorNoError;
	}
	if(mutex->owner!=currentTask){
		tTaskExitCritical(status);
		return tErrorOwner;
	}
	if(--mutex->lockedCount>0){
		tTaskExitCritical(status);
		return tErrorNoError;
	}
	if(mutex->ownerOriginPriority!=mutex->owner->priority){
		if(mutex->owner->state == TINYOS_TASK_STATE_RDY){
			tTaskSchedUnRdy(mutex->owner);
			mutex->owner->priority = currentTask->priority;
			tTaskSchedRdy(mutex->owner);
				}
		else{
			mutex->owner->priority = currentTask->priority;
					
		}
	}
	if(tEventWaitCount(&mutex->event)>0){
		tTask* task = tEventWake(&mutex->event,(void*)0,tErrorNoError);
		mutex->owner = task;
		if(task->priority<currentTask->priority){
			tTaskExitCritical(status);
			tTaskSched();
		}
		
	}
	tTaskExitCritical(status);
	return tErrorNoError;
	
}


