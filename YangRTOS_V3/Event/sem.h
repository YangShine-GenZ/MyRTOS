#ifndef __SEM_H
#define __SEM_H

#include"tinyOS.h"



typedef struct _tSem{
	tEvent event;
	uint32_t count;
	uint32_t maxCount;

}tSem;

typedef struct _tSemInfo{
	uint32_t count;
	uint32_t maxCount;
	uint32_t taskCount;

}tSemInfo;



void tSemInit(tSem* sem,uint32_t startCount,uint32_t maxCount);
uint32_t tSemWait(tSem* sem, uint32_t waitTicks);
uint32_t tSemNowaitGet(tSem* sem);
void tSemNotify(tSem* sem);
uint32_t tSemRemoveAll(tSem* sem);
void tSemGetInfo(tSem* sem, tSemInfo* semInfo);


#endif