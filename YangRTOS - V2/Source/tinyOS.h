#ifndef _TINYOS_H
#define _TINYOS_H


#include <stdint.h>
#include "ARMCM3.h"

#include "primask.h"
#include "tlib.h"
#include "tconfig.h"


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
	tNode linkNode;
	uint32_t delayTicks;
	tNode delayNode;
	uint32_t priority;
	uint32_t state;
	uint32_t slice;
}tTask;



extern tTask* currentTask;
extern tTask* nextTask;
extern tTask* IdleTask;
extern tBitmap taskPrioBitmap;
extern tList tTaskDelayedList;


extern tList taskTable[TINYOS_PRO_COUNT]; 



void tTaskInit(tTask* task, void (*entry)(void *),void *parms,tTaskStack *stack,uint32_t priority);
void tTaskSched(void);
void tTaskSwitch(void);
void tTaskRunFirst(void);
void tTaskSchedInit(void);
void tTaskDelayedInit(void);
void tTaskSchedLockInit(void);
void tTaskSchedDisable(void);
void tTaskSchedEnable(void);
tTask* tTaskHighestReady(void);


#endif 

