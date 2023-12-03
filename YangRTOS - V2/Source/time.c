#include "time.h"


uint32_t tickCount = 0;


void tSetSysTickPeriod(uint32_t ms){
	SysTick->LOAD = ms*SystemCoreClock/1000-1; //ms
	NVIC_SetPriority(SysTick_IRQn,(1<<__NVIC_PRIO_BITS)-1);
	SysTick->VAL  = 0;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;

}



void tTimeTaskWait(tTask* task,uint32_t tick){
	task->delayTicks = tick;
	tListAddLast(&tTaskDelayedList,&(task->delayNode));
	task->state |= TINYOS_TASK_STATE_DELAYED;
}


void tTimeTaskWakeUp(tTask* task){
	tListRemove(&tTaskDelayedList,&(task->delayNode));
	task->state &= ~TINYOS_TASK_STATE_DELAYED;
}



void tTaskSchedRdy(tTask* task){

	tListAddLast(&(taskTable[task->priority]),&(task->linkNode));
	tBitmapSet(&taskPrioBitmap,task->priority);

}


void tTaskSchedUnRdy(tTask* task){
	
	tListRemove(&(taskTable[task->priority]),&(task->linkNode));
	
	if(tListCount(&(taskTable[task->priority])) == 0){
		tBitmapClear(&taskPrioBitmap,task->priority);
	}

}





void tTaskDelay(uint32_t delay){
		
	uint32_t status = tTaskEnterCritical();
	
	tTimeTaskWait(currentTask,delay);
	tTaskSchedUnRdy(currentTask);
	
	tTaskExitCritical(status);	
	tTaskSched();

}






void tTaskSystemTickHandler(void){
	int i;
	tNode* node;
	uint32_t status = tTaskEnterCritical();
	
	for(node = tTaskDelayedList.headNode.nextNode;node!=&(tTaskDelayedList.headNode);node = node->nextNode){
		tTask *task =  (tTask *)tNodeParent(node, tTask, delayNode);
		if(--task->delayTicks == 0){
			tTimeTaskWakeUp(task);
			tTaskSchedRdy(task);
		
		}
	
	if(--currentTask->slice == 0){
		if(tListCount(&(taskTable[currentTask->priority])) > 0){
			tListRemoveFirst(&(taskTable[currentTask->priority]));
			tListAddLast(&(taskTable[currentTask->priority]),&(currentTask->linkNode));
			
			currentTask->slice = TINYOS_SLICE_MAX;
		}
	
	}
	
	tTaskExitCritical(status);
	tTaskSched();

}
}


void SysTick_Handler(){
	tTaskSystemTickHandler();

}


