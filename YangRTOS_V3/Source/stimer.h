#ifndef __STIMER_H

#define __STIMER_H


#include "tlib.h"



#define TIMER_CONFIG_TYPE_HARD              (1 << 0)
#define TIMER_CONFIG_TYPE_SOFT              (0 << 0)


typedef enum _tTimerState{
	tTimeCreated,
	tTimeStarted,
	tTimeRunning,
	tTimeStopped,
	tTimeDestroy
}tTimerState;




typedef struct _tTimer{
	tNode linkNode;
	uint32_t startDelayTicks;
	uint32_t durationTicks;
	uint32_t delayTicks;
	void (*timerFunc)(void* args);
	void* args;
	uint32_t config;
	tTimerState state;

}tTimer;


void tTimerInit(tTimer* timer,uint32_t delayTicks, uint32_t durationTicks,void (*timerFunc)(void* args),void* args,uint32_t config);
void tTimerModuleInit(void);
void tTimerModuleTickNotify(void);
void tTimerStart(tTimer* timer);
void tTimerStop(tTimer* timer);

#endif