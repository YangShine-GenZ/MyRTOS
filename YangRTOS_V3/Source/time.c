#include "time.h"
#include "event.h"



uint32_t tickCount = 0;


void tSetSysTickPeriod(uint32_t ms){
	SysTick->LOAD = ms*SystemCoreClock/1000-1; //ms
	NVIC_SetPriority(SysTick_IRQn,(1<<__NVIC_PRIO_BITS)-1);
	SysTick->VAL  = 0;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;

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
			if(task->waitEvent){
				tEventRemoveTask(task,(void*)0,tErrorTimeOut);
			
			}
			tTimeTaskWakeUp(task);
			tTaskSchedRdy(task);
		
		}
	}
	
	if(--currentTask->slice == 0){
		if(tListCount(&(taskTable[currentTask->priority])) > 0){
			tListRemoveFirst(&(taskTable[currentTask->priority]));
			tListAddLast(&(taskTable[currentTask->priority]),&(currentTask->linkNode));
			currentTask->slice = TINYOS_SLICE_MAX;
		}
	
	}

	tTaskExitCritical(status);
	
	tTimerModuleTickNotify();
	
	tTaskSched();
}


void SysTick_Handler(){
	tTaskSystemTickHandler();

}


