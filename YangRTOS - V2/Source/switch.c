#include "tinyOS.h"
#include "ARMCM3.h"





tTask* currentTask;
tTask* nextTask;
tTask* IdleTask;

tTask* taskTable[TINYOS_PRO_COUNT];


uint8_t schedLockCount; //调度锁计数器
tBitmap taskPrioBitmap; //优先级位图
tList tTaskDelayedList;//延时队列





void tTaskInit(tTask* task, void (*entry)(void *),void *parms,tTaskStack *stack,uint32_t priority){
	
	//before PendSV IRQ
	*(--stack) = (unsigned long)(1<<24);
	*(--stack) = (unsigned long)entry;
	*(--stack) = (unsigned long)0x14;
	*(--stack) = (unsigned long)0x12;
	*(--stack) = (unsigned long)0x3;
	*(--stack) = (unsigned long)0x2;
	*(--stack) = (unsigned long)0x1;
	*(--stack) = (unsigned long)parms;
	
	
	//entering PendSV IRQ
	*(--stack) = (unsigned long)0x11;
	*(--stack) = (unsigned long)0x10;
	*(--stack) = (unsigned long)0x9;
	*(--stack) = (unsigned long)0x8;
	*(--stack) = (unsigned long)0x7;
	*(--stack) = (unsigned long)0x6;
	*(--stack) = (unsigned long)0x5;
	*(--stack) = (unsigned long)0x4;
	
	
	task->stack = stack;
	task->delayTicks = 0;
	task->priority  = priority;
	task->state = TINYOS_TASK_STATE_RDY;
	tNodeInit(&(task->delayNode));
	
	taskTable[priority] = task;
	tBitmapSet(&taskPrioBitmap,priority);
	 
		
}




tTask* tTaskHighestReady(void){
	
	uint32_t highestPrio = tBitmapGetFirstSet(&taskPrioBitmap);
	return taskTable[highestPrio];

}



void tTaskSchedInit(void){
	tTaskSchedLockInit();
	tBitmapInit(&taskPrioBitmap);
	tTaskDelayedInit();

}






void tTaskSched(){
	
	tTask* tempTask;
	uint32_t status  = tTaskEnterCritical();
	
	if(schedLockCount > 0){
		tTaskExitCritical(status);
		return; 
	}
	
	tempTask = tTaskHighestReady();
	
	if(currentTask!=tempTask){
		nextTask = tempTask;
		tTaskExitCritical(status);
		tTaskSwitch();
		
	}
	
	tTaskExitCritical(status);
	
}



void tTaskRunFirst(){
	
	__set_PSP(0);
	//PendSV IRQ awake 
	MEM8(NVIC_SYSPRI2) = NVIC_PENDSV_PRI;
	MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;	

}


void tTaskDelayedInit(void){
	tListInit(&tTaskDelayedList);
}






void tTaskSwitch(){
	
	MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;	

}


void tTaskSchedLockInit(){
	schedLockCount = 0;
}


void tTaskSchedDisable(){

	uint32_t status = tTaskEnterCritical();
	
	if(schedLockCount < 255){
		schedLockCount++;
	
	}
	
	tTaskExitCritical(status);

}

void tTaskSchedEnable(){
	
	uint32_t status = tTaskEnterCritical();
	
	if(schedLockCount > 0){
		if(--schedLockCount==0){
			//tTaskExitCritical(status);
			tTaskSched();
		}
	
	}
	tTaskExitCritical(status);

}


__asm void PendSV_Handler (void)
{
	IMPORT currentTask
	IMPORT nextTask
	
	MRS R0,PSP
	CBZ R0,PendSVHandler_nosave
	//for taskX X!=0, save context r4-r11
	STMDB R0!,{R4-R11}
	
	//currentTask.stack = R0
	LDR R1, =currentTask
	LDR R1,[R1]
	STR R0,[R1]
	
	//for task0, do not save task0's context
PendSVHandler_nosave
	
	//currentTask = nextTask
	LDR R0, =currentTask
	LDR R1, =nextTask
	LDR R2, [R1]
	STR R2, [R0]
	
	//R2 = currentTask [R2] =*currentTask = stack
	LDR R0, [R2]
	//read the context of the currenttask
	LDMIA R0!,{R4-R11}
	//change the stack to the currentTask
	MSR PSP,R0
	ORR LR,LR,#0X04  //set the use of the PSP stack
	BX LR
	
	
}









