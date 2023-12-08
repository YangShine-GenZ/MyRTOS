#ifndef __MUTEX_H
#define __MUTEX_H


#include "event.h"
#include "tinyOS.h"


typedef struct _tMutex{
	tEvent event;
	uint32_t lockedCount;
	tTask* owner;
	uint32_t ownerOriginPriority;


}tMutex;

void tMutexInit(tMutex* mutex);




#endif