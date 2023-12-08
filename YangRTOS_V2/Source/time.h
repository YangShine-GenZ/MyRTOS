#ifndef __TIME_H
#define __TIME_H


#include "tinyOS.h"
#include "state.h"



extern uint32_t tickCount;




void tSetSysTickPeriod(uint32_t ms);
void tTaskSystemTickHandler(void);
void tTaskDelay(uint32_t delay);






#endif