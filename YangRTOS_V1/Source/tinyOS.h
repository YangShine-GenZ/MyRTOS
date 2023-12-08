#ifndef _TINYOS_H
#define _TINYOS_H


#include <stdint.h>
#include "time.h"
#include "ARMCM3.h"


#define NVIC_INT_CTRL 0xE000Ed04
#define NVIC_PENDSVSET 0x10000000
#define NVIC_SYSPRI2 0XE000ED22
#define NVIC_PENDSV_PRI 0x000000FF


#define MEM32(addr)  *(volatile unsigned long *)(addr)
#define MEM8(addr) *(volatile unsigned char *)(addr)


typedef uint32_t  tTaskStack;


typedef struct _BlockType_t{
	
	unsigned long *stackPtr;


}BlockType_t;



typedef struct _tTask{
	tTaskStack *stack;
	uint32_t delayTicks;
}tTask;



extern tTask* currentTask;
extern tTask* nextTask;
extern tTask* IdleTask;


extern tTask* taskTable[2];



void tTaskInit(tTask* task, void (*entry)(void *),void *parms,tTaskStack *stack);
void tTaskSched(void);
void tTaskSwitch(void);
void tTaskRunFirst(void);


#endif 

