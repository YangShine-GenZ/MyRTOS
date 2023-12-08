#ifndef __TFLAG_H
#define __TFLAG_H




#include "tinyOS.h"
#include "event.h"

typedef struct _tFlagGroup{
	tEvent event;
	uint32_t flags;

}tFlagGroup;


void tFlagGroupInit(tFlagGroup* flagGroup,uint32_t flags);
uint32_t tFlagGroupWait(tFlagGroup* flagGroup,uint32_t waitType, uint32_t requestFlag,uint32_t *resultFlag,uint32_t waitTicks);
uint32_t tFlagGroupNoWaitGet(tFlagGroup* flagGroup,uint32_t waitType, uint32_t requestFlag,uint32_t *resultFlag);
void tFlagGroupNotify(tFlagGroup* flagGroup,uint8_t isSet,uint32_t flag);



#endif