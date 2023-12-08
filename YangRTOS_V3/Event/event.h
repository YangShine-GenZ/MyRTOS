#ifndef __EVENT_H

#define __EVENT_H


#include "tinyOS.h"


void tEventInit(tEvent* event,tEventType type);
void tEventWait(tEvent *event, tTask *task, void *msg, uint32_t state, uint32_t timeout);
tTask* tEventWake(tEvent* event,void* msg,uint32_t result);
void tEventRemoveTask(tTask* task,void* msg,uint32_t result);
uint32_t tEventRemoveAll(tEvent* event,void* msg,uint32_t result);
uint32_t tEventWaitCount(tEvent* event);


#endif