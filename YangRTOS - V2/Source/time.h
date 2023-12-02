#ifndef __TIME_H
#define __TIME_H


#include "tinyOS.h"


extern uint32_t tickCount;




void tSetSysTickPeriod(uint32_t ms);
void tTaskSystemTickHandler(void);
void tTaskDelay(uint32_t delay);
void tTimeTaskWait(tTask *task,uint32_t tick);
void tTimeTaskWakeUp(tTask *task);
void tTaskSchedRdy(tTask *task);
void tTaskSchedUnRdy(tTask *task);







#endif