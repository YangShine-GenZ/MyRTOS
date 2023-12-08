#include "state.h"


//将任务从就绪队列删除
void tTaskSchedRemove(tTask* task){
	tListRemove(&(taskTable[task->priority]),&(task->linkNode));
	
	if(tListCount(&(taskTable[task->priority])) == 0){
		tBitmapClear(&taskPrioBitmap,task->priority);
	}

}

//将任务从延时队列删除
void tTimeTaskRemove(tTask* task){
	tListRemove(&tTaskDelayedList,&(task->delayNode));

}


//任务进入延时队列
void tTimeTaskWait(tTask* task,uint32_t tick){
	task->delayTicks = tick;
	tListAddLast(&tTaskDelayedList,&(task->delayNode));
	task->state |= TINYOS_TASK_STATE_DELAYED;
}

//从延时队列唤醒任务
void tTimeTaskWakeUp(tTask* task){
	tTimeTaskRemove(task);
	task->state &= ~TINYOS_TASK_STATE_DELAYED;
}


//任务进入就绪状态
void tTaskSchedRdy(tTask* task){

	tListAddLast(&(taskTable[task->priority]),&(task->linkNode));
	tBitmapSet(&taskPrioBitmap,task->priority);

}


//任务进入非就绪状态
void tTaskSchedUnRdy(tTask* task){
	
	tListRemove(&(taskTable[task->priority]),&(task->linkNode));
	
	if(tListCount(&(taskTable[task->priority])) == 0){
		tBitmapClear(&taskPrioBitmap,task->priority);
	}

}


void tTaskSuspend(tTask* task){
	uint32_t status = tTaskEnterCritical();
	
	//判断是否在任务延时状态
	if(!(task->state & TINYOS_TASK_STATE_DELAYED)){
		if(++task->suspendCount <= 1){
			task->state |= TINYOS_TASK_STATE_SUSPEND;
			tTaskSchedUnRdy(task);
			if(task == currentTask){
				tTaskSched();
			}
		}
		
	}
	tTaskExitCritical(status);

}


void tTaskWakeUp(tTask* task){
	uint32_t status = tTaskEnterCritical();
	
	if(task->state & TINYOS_TASK_STATE_SUSPEND){
		if(--task->suspendCount == 0){
			task->state &= ~TINYOS_TASK_STATE_SUSPEND;
			tTaskSchedRdy(task);
			tTaskSched();
		}
	
	}
	
	tTaskExitCritical(status);

}


//设置任务删除的回调函数
void tTaskSetCleanFunc(tTask* task,void(*clean)(void* param),void* param){
	task->clean = clean;
	task->Cleanparam = param;

}


//强制删除函数
void tTaskForceDelete(tTask* task){
	uint32_t status = tTaskEnterCritical();
	
	if(task->state & TINYOS_TASK_STATE_DELAYED){
		tTimeTaskRemove(task);
	}
	else if (!(task->state & TINYOS_TASK_STATE_SUSPEND)){
		tTaskSchedRemove(task);
	}
	if(task->clean){
		task->clean(task->Cleanparam);
	
	}
	if(currentTask == task){
		tTaskSched();
	}
	
	//task->state |= TINYOS_TASK_STATE_SUSPEND;
	tTaskExitCritical(status);
}


void tTaskRequestDelete(tTask* task){
	uint32_t status = tTaskEnterCritical();
	task->requestDeleteFlag = 1;
	
	tTaskExitCritical(status);
}



uint8_t tTaskGetDelete(void){
	
	uint32_t status = tTaskEnterCritical();
	uint8_t deleteTemp = currentTask->requestDeleteFlag;
	tTaskExitCritical(status);
	
	return deleteTemp;

}


void tTaskDeleteSelf(void){
	uint32_t status = tTaskEnterCritical();
	tTaskSchedRemove(currentTask);
	if(currentTask->clean){
		currentTask->clean(currentTask->Cleanparam);
	
	}
	tTaskExitCritical(status);
	tTaskSched();

}


void tTaskGetInfo(tTask* task,tTaskInfo* info){
	uint32_t status = tTaskEnterCritical();
	
	info->delayTicks = task->delayTicks;
	info->priority = task->priority;
	info->state = task->state;
	info->slice = task->slice;
	info->suspendCount = task->suspendCount;
	
	tTaskExitCritical(status);

}







