#include "stimer.h"
#include "tinyOS.h"
#include "sem.h"

static tList tTimerHardList;
static tList tTimerSoftList;
static tSem tTimeProtectionSem;
static tSem tTimerTickSem;

static tTask tTimerTask;
static tTaskStack tTimerTaskStack[TINYOS_TIMERTASK_STACK_SIZE];



void tTimerInit(tTimer* timer,uint32_t delayTicks, uint32_t durationTicks,void (*timerFunc)(void* args),void* args,uint32_t config){
	tNodeInit(&timer->linkNode);
	timer->startDelayTicks = delayTicks;
	timer->durationTicks = durationTicks;
	timer->timerFunc = timerFunc;
	timer->args = args;
	timer->config = config;
	
	if(delayTicks == 0){
		timer->delayTicks = durationTicks;
	
	}
	else{
		timer->delayTicks = timer->startDelayTicks;
	
	}
	timer->state = tTimeCreated;
	
}


static void tTimerCallFuncList(tList* timerList){
	
	tNode* node;
	for(node = timerList->headNode.nextNode;node!=&(timerList->headNode);node = node->nextNode){
		tTimer * timer = tNodeParent(node,tTimer,linkNode);
		if ((timer->delayTicks==0) || (--timer->delayTicks==0)){
			timer->state = tTimeRunning;
			timer->timerFunc(timer->args);
			timer->state = tTimeStarted;
			if(timer->durationTicks > 0){
				timer->delayTicks = timer->durationTicks;
			}
			else{
				tListRemove(timerList,&timer->linkNode);
				timer->state = tTimeStopped;
			}
		}
	
	}

}




static void tTimerSoftTask(void* param){
	for(;;){
		tSemWait(&tTimerTickSem,0);
		tSemWait(&tTimeProtectionSem,0);
		tTimerCallFuncList(&tTimerSoftList);
		
		tSemNotify(&tTimeProtectionSem);
		
	}

}


void tTimerModuleTickNotify(void){

	uint32_t status  =tTaskEnterCritical();
	
	tTimerCallFuncList(&tTimerHardList);
	
	tTaskExitCritical(status);
	
	tSemNotify(&tTimerTickSem);
}







void tTimerModuleInit(void){
	tListInit(&tTimerHardList);
	tListInit(&tTimerSoftList);
	//软定时器列表
	tSemInit(&tTimeProtectionSem,1,1);
	
	//硬定时器列表
	tSemInit(&tTimerTickSem,0,0);

#if TINYOS_TIMERTASK_PRIO >= (TINYOS_PRO_COUNT-1)
	#error "The proprity of timer tasker is wrong "
#endif
	tTaskInit(&tTimerTask,tTimerSoftTask,(void*)0,&tTimerTaskStack[TINYOS_TIMERTASK_STACK_SIZE],TINYOS_TIMERTASK_PRIO);
	
}


void tTimerStart(tTimer* timer){

	switch(timer->state){
		case tTimeCreated:
		case tTimeStopped:
			timer->delayTicks = timer->startDelayTicks ? timer->startDelayTicks:timer->durationTicks;
			timer->state = tTimeStarted;
			if(timer->config & TIMER_CONFIG_TYPE_HARD){
				uint32_t status = tTaskEnterCritical();
				tListAddLast(&tTimerHardList,&timer->linkNode);
				tTaskExitCritical(status);	
			}
			else{
				tSemWait(&tTimeProtectionSem,0);
				tListAddLast(&tTimerSoftList,&timer->linkNode);
				tSemNotify(&tTimeProtectionSem);
			}
			break;
		default:
			break;
	}

}

void tTimerStop(tTimer* timer){
	switch(timer->state){
		case tTimeStarted:
		case tTimeRunning:
			if(timer->config & TIMER_CONFIG_TYPE_HARD){
				uint32_t status = tTaskEnterCritical();
				tListRemove(&tTimerHardList,&timer->linkNode);
				tTaskExitCritical(status);
			
			}
			else{
				tSemWait(&tTimeProtectionSem,0);
				tListRemove(&tTimerSoftList,&timer->linkNode);
				tSemNotify(&tTimeProtectionSem);
			}
			
			break;
		default:
			break;
	}
}




