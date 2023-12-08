#include "tinyOS.h"
#include "ARMCM3.h"


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


tTask* currentTask;
tTask* nextTask;
tTask* IdleTask;

tTask* taskTable[2];





void tTaskInit(tTask* task, void (*entry)(void *),void *parms,tTaskStack *stack){
	
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
		
}



void tTaskSched(){
	
//	if(currentTask == taskTable[0]){
//		nextTask = taskTable[1];
//	}
//	else if(currentTask == taskTable[1]){
//		nextTask = taskTable[0];
//	
//	}
//	tTaskSwitch();
	
	
	if(currentTask == IdleTask){
		if(taskTable[0]->delayTicks == 0){
			nextTask = taskTable[0];
		}
		else if(taskTable[1]->delayTicks == 0){
			nextTask = taskTable[1];
		}
		else{
			return;
		}
	
	}
	else if(currentTask == taskTable[0]){
		if(taskTable[1]->delayTicks == 0){
			nextTask = taskTable[1];
		}
		else if(taskTable[0]->delayTicks!=0){
			nextTask = IdleTask;
		}
		else{
			return;
		}
	
	}
	else if(currentTask == taskTable[1]){
		if(taskTable[0]->delayTicks == 0){
			nextTask = taskTable[0];
		}
		else if(taskTable[1]->delayTicks!=0){
			nextTask = IdleTask;
		}
		else{
			return;
		}
	
	}
	tTaskSwitch();
	
}



void tTaskRunFirst(){
	
	__set_PSP(0);
	//PendSV IRQ awake 
	MEM8(NVIC_SYSPRI2) = NVIC_PENDSV_PRI;
	MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;	

}



void tTaskSwitch(){
	
	MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;	

}
