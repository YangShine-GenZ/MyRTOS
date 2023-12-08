#ifndef __STATE_H

#define __STATE_H


#include "tinyOS.h"


void tTaskSchedRemove(tTask* task);
void tTimeTaskRemove(tTask* task);
void tTimeTaskWait(tTask *task,uint32_t tick);
void tTimeTaskWakeUp(tTask *task);
void tTaskSchedRdy(tTask *task);
void tTaskSchedUnRdy(tTask *task);
void tTaskSuspend(tTask* task);
void tTaskWakeUp(tTask* task);


void tTaskSetCleanFunc(tTask* task,void(*clean)(void* param),void* param);
void tTaskForceDelete(tTask* task);
void tTaskRequestDelete(tTask* task);
uint8_t tTaskGetDelete(void);
void tTaskDeleteSelf(void);

void tTaskGetInfo(tTask* task,tTaskInfo* info);


#endif