#ifndef _TINYOS_H
#define _TINYOS_H


#include <stdint.h>
#include "ARMCM3.h"

#include "primask.h"
#include "tlib.h"
#include "tconfig.h"
#include "eventModule.h"
#include "stimer.h"


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
	//就绪队列节点
	tNode linkNode;
	
	//delayTicks
	uint32_t delayTicks;
	
	//延时队列节点
	tNode delayNode;
	
	//优先级
	uint32_t priority;
	//任务状态
	uint32_t state;
	//时间片
	uint32_t slice;
	
	//挂机次数
	uint32_t suspendCount;
	
	//用于任务删除
	void (*clean)(void* params);
	void* Cleanparam;
	uint8_t requestDeleteFlag;
	
	//事件组
	//任务正在等待的事件类型
	tEvent* waitEvent;
	//事件组中消息传递
	void* eventMsg;
	//等待事件的结果
	uint32_t waitEventResult;
	
	//用于事件标志组
	//请求事件类型
	//uint32_t waitFlagType;
	//请求结果标志
	//uint32_t eventFlag;
	
	
	
	
}tTask;


typedef struct _tTaskInfo{
	uint32_t delayTicks;
	uint32_t priority;
	uint32_t state;
	uint32_t slice;
	uint32_t suspendCount;

}tTaskInfo;





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

